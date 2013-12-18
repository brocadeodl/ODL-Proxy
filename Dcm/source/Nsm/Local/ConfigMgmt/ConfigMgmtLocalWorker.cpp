/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*******************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.              *
 *   All rights reserved.                                                      *
 *   Author : Mobin Mohan                                                      *
 ******************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
#include "DcmCore/DcmToolKit.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/ConfigMgmt/ConfigMgmtLocalWorker.h"
#include "Nsm/Local/NsmTypes.h"

#include "ClientInterface/Nsm/configmgmt_dcm_def.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/ConfigMgmtShowMessage.h"
#include "ClientInterface/Nsm/configmgmt_dcm_def.h"

#include "DcmCore/DcmToolKit.h"
#include "errno.h"

namespace DcmNs
{

    ConfigMgmtLocalWorker::ConfigMgmtLocalWorker ( 
					NsmLocalObjectManager *	pWaveObjectManager)
			: WaveWorker  (pWaveObjectManager)
    {
        addOperationMap (CONFIGMGMTLOCALSHOW, 
						reinterpret_cast<PrismMessageHandler> (
						&ConfigMgmtLocalWorker::ConfigMgmtLocalShowMessageHandler));
    }

    ConfigMgmtLocalWorker::~ConfigMgmtLocalWorker ()
    {
    }

    PrismMessage  *ConfigMgmtLocalWorker::createMessageInstance(
													const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case CONFIGMGMTLOCALSHOW:
                pPrismMessage = new ConfigMgmtLocalShowMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigMgmtLocalWorker::createManagedObjectInstance(
												const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
            
		trace (TRACE_LEVEL_DEBUG, "ConfigMgmtLocalWorker::"
						"createManagedObjectInstance : " + managedClassName);

        return (pWaveManagedObject);
    }


    void
    ConfigMgmtLocalWorker::ConfigMgmtLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        ConfigMgmtLocalShowMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering ConfigMgmtLocalWorker::"
								"ConfigMgmtLocalShowSendToClusterCallbac\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<ConfigMgmtLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<ConfigMgmtLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<ConfigMgmtLocalShowMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
				if(req_msg_p->getCmdCode() == CONFIGMGMT_CMD_SHOW_SYSTEM) {
				 	string result;
					result = req_msg_p->getResult() + sw_resp_p->getResult();
					req_msg_p->setResult(result); 
				} else {
	                req_msg_p->copyAllBuffers(*sw_resp_p);
				}
            } else {
                 trace(TRACE_LEVEL_ERROR, string("Response processing:"
							"The location: ") + locations[i] + 
							string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void
    ConfigMgmtLocalWorker::sendConfigMgmtShowCommandToCluster (
            						PrismLinearSequencerContext *seq_ctx_p)
    {
        ConfigMgmtLocalShowMessage        *msg_p = NULL;
        ConfigMgmtLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
	      UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
    	//bool                        sendToLocal = false;

        trace(TRACE_LEVEL_DEBUG, "Entering ConfigMgmtLocalWorker::"
									"sendConfigMgmtShowCommandToCluster\n");
        msg_p = dynamic_cast<ConfigMgmtLocalShowMessage *> (
											seq_ctx_p->getPPrismMessage ());

        loc_msg_p = new ConfigMgmtLocalShowMessage ();
        loc_msg_p->setCmdCode(msg_p->getCmdCode());
        loc_msg_p->setMappedId(msg_p->getMappedId());
        loc_msg_p->setIsGwRequest(false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster") +
                        string(" CmdCode = ") + msg_p->getCmdCode() + 
                        string(" MappedId = ") + msg_p->getIsGwRequest() + 
						string (" false"));

        send_ctx_p = new WaveSendToClusterContext(this, 
           reinterpret_cast<PrismAsynchronousCallback>
           (&ConfigMgmtLocalWorker::ConfigMgmtLocalShowSendToClusterCallback),
           seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		
		if (msg_p->getCmdCode() == CONFIGMGMT_CMD_SHOW_SYSTEM) {
			//sendToLocal = false;
			mappedId = msg_p->getMappedId();
			locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
			locations.push_back(locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}

       /* Dead code
        if (sendToLocal) {
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
       */

        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId 
    ConfigMgmtLocalWorker::sendShowToConfigMgmtDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        ConfigMgmtLocalShowMessage *msg_p = dynamic_cast<ConfigMgmtLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        
		trace(TRACE_LEVEL_DEBUG, "Entering ConfigMgmtLocalWorker::"
												"sendShowToIgmpsDaemon");
        trace(TRACE_LEVEL_INFO, string("ConfigMgmtLocalWorker::"
								"sendShowToConfigMgmtDaemon::")
						+ string("cmdCode= ") + msg_p->getCmdCode() + 
						string(" mappedId= ") + msg_p->getCmdCode());

		FILE	*fp;
		int		rc;
		char	cmd_output[512] = {0};
		string result = "";
        ConfigMgmtShowMessage *cl_msg_p = NULL;
        ResourceId      status;
		UI32 size = 0;
		void *data;
		config_mgmt_buffer_num_t  buff_num;
		config_mgmt_show_system_resp_t *msg;

		cl_msg_p = new ConfigMgmtShowMessage();
		cl_msg_p->setCmdCode(msg_p->getCmdCode());
		cl_msg_p->setMappedId(msg_p->getMappedId());
       
		/* Send message to nsm */
		status = sendSynchronouslyToWaveClient("nsm", cl_msg_p);
        
		if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, 
                        string("Resp: Client Completion Error MsgId: ") + 
                        cl_msg_p->getMessageId() +  
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				delete cl_msg_p;
                return msg_p->getCompletionStatus();
            }

            if (cl_msg_p->getClientStatus() !=  WAVE_MESSAGE_SUCCESS) { 
                trace(TRACE_LEVEL_ERROR, string("Resp:  Client Error MsgId: ") + 
                                  cl_msg_p->getMessageId() + 
                FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				delete cl_msg_p;
                return msg_p->getCompletionStatus();
            }

        }

		/* Read the buffer and get te and fc port counts */
		buff_num.buff.mapped_id = msg_p->getMappedId();
		buff_num.buff.tag = CONFIGMGMT_SHOW_BUF_SYSTEM;
	    cl_msg_p->getOutputBufferRef(buff_num.num, size, data);
		msg = (config_mgmt_show_system_resp_t *) data;
		int teport = msg->teport;
		int fcport = msg->fcport;
		int giport = msg->giport;
		int foport = msg->foport;
				
		trace (TRACE_LEVEL_INFO, string("sendShowToConfigMgmtDaemon:: teport ")
						 + teport +" fcport " +fcport+" giport" +giport + " foport " + foport);

		/* Run the script and generate the output */
		char command[512];
		snprintf(command, 512, "/fabos/bin/imishow_system %d %d %d %d", fcport, teport, giport, foport);

		if ((fp = popen(command, "r")) != NULL) {
			while (fgets(cmd_output, 512, fp)) {
				trace (TRACE_LEVEL_INFO, string("sendShowToConfigMgmtDaemon::"
										"In fgets : ") + cmd_output);
				result += cmd_output;
			}
			if ((rc = pclose(fp)) < 0) {
				trace (TRACE_LEVEL_INFO, string("sendShowToConfigMgmtDaemon:: "
					"pclose failed - CMD = ") + command + string (" rc=") + 
											rc + string (" errno=") + -errno);
				result = "Failed";
			}
		} else {
			trace (TRACE_LEVEL_INFO, string("sendShowToConfigMgmtDaemon: "
				"popen failed - CMD =  ") + command + string (" errno=") + -errno);
			result = "Command execution failed.\n";
		}

		result += '\n';
		msg_p->setResult(result);
        
		msg_p->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		trace (TRACE_LEVEL_INFO, "sendShowToConfigMgmtDaemon replied");
		
		delete cl_msg_p;
		return WAVE_MESSAGE_SUCCESS;
    }

    void  
    ConfigMgmtLocalWorker::ConfigMgmtLocalShowMessageHandler (
            ConfigMgmtLocalShowMessage *pConfigMgmtLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering ConfigMgmtLocalWorker::ConfigMgmtLocalShowMessageHandler\n");
        if (pConfigMgmtLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&ConfigMgmtLocalWorker::sendConfigMgmtShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&ConfigMgmtLocalWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&ConfigMgmtLocalWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pConfigMgmtLocalShowMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&ConfigMgmtLocalWorker::sendShowToConfigMgmtDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&ConfigMgmtLocalWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&ConfigMgmtLocalWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pConfigMgmtLocalShowMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }
}
