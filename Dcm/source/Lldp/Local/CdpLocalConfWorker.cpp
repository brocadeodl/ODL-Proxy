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

/***************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/CdpLocalConfWorker.h"
#include "Lldp/Local/CdpConfSetMessage.h"
#include "Lldp/Local/LldpTypes.h"
#include "Lldp/Common/LldpPluginDebug.h"

#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/CdpClientConfSetMessage.h"
#include "DcmCStatus.h"
#include "ClientInterface/Lldp/LldpUtils.h"

namespace DcmNs
{

    CdpLocalConfWorker::CdpLocalConfWorker (LldpLocalObjectManager *pLldpLocalObjectManager)
        : WaveWorker  (pLldpLocalObjectManager)
    {
        addOperationMap (CDPCONFSET, reinterpret_cast<PrismMessageHandler> (&CdpLocalConfWorker::CdpConfSetMessageHandler));
    }

    CdpLocalConfWorker::~CdpLocalConfWorker ()
    {
    }

    PrismMessage  *CdpLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case CDPCONFSET :
                pPrismMessage = new CdpConfSetMessage ();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *CdpLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
    
	ResourceId CdpLocalConfWorker::updateCdpConf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        CdpConfSetMessage *pCdpConfSetMessage = dynamic_cast<CdpConfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		int status = WAVE_MESSAGE_SUCCESS;
    	CdpClientConfSetMessage *m = new CdpClientConfSetMessage();
        UI32 cmdcode = pCdpConfSetMessage->getCmdcode();
		
		LLDP_PLUG_DBG (
				string ("Msg Type:: ") + pCdpConfSetMessage->getMsgType()
			  + string (" Cmd Code::") + pCdpConfSetMessage->getCmdcode());

        /* Read the header from received message and populate 
		 * sending message
		 */
 		m->setMgmtFlags (pCdpConfSetMessage->getMgmtFlags());
        m->setMsgType (pCdpConfSetMessage->getMsgType());
        m->setCmdcode (pCdpConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, CDP_GLOBAL_CONF_PROTO_STATUS))
            m->setProtoEnable (pCdpConfSetMessage->getProtoEnable());
		
        /* Send the message to backend deamon */
        status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
		
		pCdpConfSetMessage->setCompletionStatus(m->getCompletionStatus());
        if(status == WAVE_MESSAGE_SUCCESS) {
            if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(string("Response processing: CompletionStatus") 
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(m->getCompletionStatus())); 
			  	//bpatel
				status = m->getCompletionStatus();
				delete  m;
				return  status;
			}
			LLDP_PLUG_DBG_S(string("Response processing: CompletionStatus") 
                		    + string("returned success: ")  + 
                  FrameworkToolKit::localize(m->getCompletionStatus())); 
			int ClientStatus = m->getClientStatus();
			if (ClientStatus != WRC_WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(string("Response processing: ClientStatus") 
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(ClientStatus)); 
				pCdpConfSetMessage->setCompletionStatus(ClientStatus);
				delete  m;
				return  ClientStatus;
			}
		}
		if(m){
			delete m;
		}
        return status;
    }
	

    void  CdpLocalConfWorker::CdpConfSetMessageHandler( CdpConfSetMessage *pCdpConfSetMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&CdpLocalConfWorker::updateCdpConf),
            
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&CdpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&CdpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pCdpConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}




