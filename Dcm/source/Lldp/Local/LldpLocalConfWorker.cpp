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
#include "Lldp/Local/LldpLocalConfWorker.h"
#include "Lldp/Local/LldpScalarsConfSetMessage.h"
#include "Lldp/Local/LldpProfileConfSetMessage.h"
#include "Lldp/Local/LldpLocalShowMessage.h"
#include "Lldp/Local/LldpLocalClearMessage.h"
#include "Lldp/Local/LldpLocalLldpDebugMessage.h"
#include "Lldp/Common/LldpPluginDebug.h"
#include "Lldp/Local/LldpTypes.h"

#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "DcmCStatus.h"
#include "ClientInterface/Lldp/LldpClientScalarsConfSetMessage.h"
#include "ClientInterface/Lldp/LldpClientProfileConfSetMessage.h"
#include "ClientInterface/Lldp/LldpShowMessage.h"
#include "ClientInterface/Lldp/LldpClearMessage.h"
#include "ClientInterface/Lldp/LldpClientDebugMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/NsmLldpInternalMessage.h"

#include "DcmCore/DcmToolKit.h"

#include "ClientInterface/Lldp/LldpUtils.h"

namespace DcmNs
{

    LldpLocalConfWorker::LldpLocalConfWorker ( LldpLocalObjectManager *pLldpLocalObjectManager)
        : WaveWorker  (pLldpLocalObjectManager)
    {
        addOperationMap (LLDPSCALARSCONFSET, reinterpret_cast<PrismMessageHandler> (&LldpLocalConfWorker::LldpScalarsConfSetMessageHandler));

        addOperationMap (LLDPPROFILECONFSET, reinterpret_cast<PrismMessageHandler> (&LldpLocalConfWorker::LldpProfileConfSetMessageHandler));

        addOperationMap (LLDPLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LldpLocalConfWorker::LldpLocalShowMessageHandler));
        
		addOperationMap (LLDPLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&LldpLocalConfWorker::LldpLocalClearMessageHandler));

        addOperationMap (LLDPLOCALLLDPDEBUG, reinterpret_cast<PrismMessageHandler> (&LldpLocalConfWorker::LldpLocalLldpDebugMessageHandler));

    }

    LldpLocalConfWorker::~LldpLocalConfWorker ()
    {
    }

    PrismMessage  *LldpLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case LLDPSCALARSCONFSET :
                pPrismMessage = new LldpScalarsConfSetMessage ();
                break;
            case LLDPPROFILECONFSET :
                pPrismMessage = new LldpProfileConfSetMessage ();
                break;

            case LLDPLOCALSHOW :
                pPrismMessage = new LldpLocalShowMessage();
                break;

            case LLDPLOCALCLEAR :
                pPrismMessage = new LldpLocalClearMessage();
                break;

            case LLDPLOCALLLDPDEBUG :
                pPrismMessage = new LldpLocalLldpDebugMessage();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LldpLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
	ResourceId LldpLocalConfWorker::updateLldpProfileConf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpProfileConfSetMessage *pLldpProfileConfSetMessage = dynamic_cast<LldpProfileConfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		int status = WAVE_MESSAGE_SUCCESS;
    	LldpClientProfileConfSetMessage *m = new LldpClientProfileConfSetMessage();
        UI32 cmdcode = pLldpProfileConfSetMessage->getCmdcode();
		
		LLDP_PLUG_DBG (
				  string ("Msg Type:") + pLldpProfileConfSetMessage->getMsgType() 
				+ string (" Cmdcode: ")	+ pLldpProfileConfSetMessage->getCmdcode() 
				+ string(" Profile: ")+pLldpProfileConfSetMessage->getProfileName());
        
        /* Read the header from received message and populate 
		 * sending message
		 */
 		m->setMgmtFlags (pLldpProfileConfSetMessage->getMgmtFlags());
        m->setMsgType (pLldpProfileConfSetMessage->getMsgType());
        m->setCmdcode (pLldpProfileConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PROFILE))
            m->setProfileName (pLldpProfileConfSetMessage->getProfileName());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DESC))
            m->setDescription (pLldpProfileConfSetMessage->getDescription());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_HELLO))
            m->setHello (pLldpProfileConfSetMessage->getHello());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MODE))
            m->setMode (pLldpProfileConfSetMessage->getMode());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER))
            m->setMultiplier (pLldpProfileConfSetMessage->getMultiplier());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv (pLldpProfileConfSetMessage->getDcbxFcoeAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv (pLldpProfileConfSetMessage->getDcbxFcoeLogicalLinkTlv());
		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV))
			m->setDcbxIscsiAppTlv (pLldpProfileConfSetMessage->getDcbxIscsiAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXTLV))
            m->setDcbxTlv (pLldpProfileConfSetMessage->getDcbxTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV))
            m->setDot1xTlv (pLldpProfileConfSetMessage->getDot1xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV))
            m->setDot3xTlv (pLldpProfileConfSetMessage->getDot3xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv (pLldpProfileConfSetMessage->getMgmtAddrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV))
            m->setPortDescrTlv (pLldpProfileConfSetMessage->getPortDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV))
            m->setSysCapTlv (pLldpProfileConfSetMessage->getSysCapTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV))
            m->setSysDescrTlv (pLldpProfileConfSetMessage->getSysDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV))
            m->setSysNameTlv (pLldpProfileConfSetMessage->getSysNameTlv());
		
        /* Send the message to backend deamon */
        status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);

		pLldpProfileConfSetMessage->setCompletionStatus(m->getCompletionStatus());
        if( status == WAVE_MESSAGE_SUCCESS) {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(string("Response processing: CompletionStatus") 
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(m->getCompletionStatus())); 
			  	//bpatel
				status = m->getCompletionStatus();
				delete  m;
				return  status;
			}
			int ClientStatus = m->getClientStatus();
			if (ClientStatus != WRC_WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(string("Response processing: ClientStatus") 
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(ClientStatus)); 
				pLldpProfileConfSetMessage->setCompletionStatus(ClientStatus);
				delete  m;
				return  ClientStatus;
			}
		}
		//bpatel:
		if(m){
			delete m;
		}
        return status;
    }
	
	ResourceId LldpLocalConfWorker::updateLldpProfileConfMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpProfileConfSetMessage *pLldpProfileConfSetMessage = dynamic_cast<LldpProfileConfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		int status = WAVE_MESSAGE_SUCCESS;
        UI32 cmdcode = pLldpProfileConfSetMessage->getCmdcode();
		
		LLDP_PLUG_DBG (
				  string ("Msg Type:") + pLldpProfileConfSetMessage->getMsgType() 
				+ string (" Cmdcode: ")	+ pLldpProfileConfSetMessage->getCmdcode() 
				+ string(" Profile: ")+pLldpProfileConfSetMessage->getProfileName());
        
        /* 
		 * Send message to Nsm, for an update in Lldp Intf Mo
		 */
		if (IS_CMDCODE_SET(pLldpProfileConfSetMessage->getMsgType(),LLDP_DELETE_CONF)) {
			if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PROFILE)) {
				NsmLldpInternalMessage *m = new NsmLldpInternalMessage();
				if (m) {
					m->setProfile(pLldpProfileConfSetMessage->getProfileName());
					m->setCmdcode(LLDP_NSM_INTERNAL_DEL_PROFILE);
					LLDP_PLUG_DBG(string("send NsmLldpInternalMessage to Nsm Local Plugin"));
					status = sendSynchronously (m);
					pLldpProfileConfSetMessage->setCompletionStatus(m->getCompletionStatus());
					if (status == WAVE_MESSAGE_SUCCESS) {
						if (m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
							LLDP_PLUG_DBG_ERR(string("Response processing: CompletionStatus") 
								+ string("returned error: ")  + 
								FrameworkToolKit::localize(m->getCompletionStatus())); 
							status = m->getCompletionStatus();
							delete  m;
							return  status;
						}
						LLDP_PLUG_DBG_S(string("Response processing: CompletionStatus") 
                		    + string("returned success: ")  + 
							FrameworkToolKit::localize(m->getCompletionStatus()));
					}
					delete m;
				}
			}
		}
		return status;
	}

    void  LldpLocalConfWorker::LldpProfileConfSetMessageHandler( LldpProfileConfSetMessage *pLldpProfileConfSetMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::updateLldpProfileConf),

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::updateLldpProfileConfMO),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLldpProfileConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    
	ResourceId LldpLocalConfWorker::updateLldpScalarsConf (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpScalarsConfSetMessage *pLldpScalarsConfSetMessage = dynamic_cast<LldpScalarsConfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		int status = WAVE_MESSAGE_SUCCESS;
    	LldpClientScalarsConfSetMessage *m = new LldpClientScalarsConfSetMessage();
        UI32 cmdcode = pLldpScalarsConfSetMessage->getCmdcode();
		
		LLDP_PLUG_DBG (
				string ("Msg Type:: ") + pLldpScalarsConfSetMessage->getMsgType()
			  + string (" Cmdcode::") + pLldpScalarsConfSetMessage->getCmdcode());

        /* Read the header from received message and populate 
		 * sending message
		 */
 		m->setMgmtFlags (pLldpScalarsConfSetMessage->getMgmtFlags());
        m->setMsgType (pLldpScalarsConfSetMessage->getMsgType());
        m->setCmdcode (pLldpScalarsConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DESC))
            m->setDescription (pLldpScalarsConfSetMessage->getDescription());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_HELLO))
            m->setHello (pLldpScalarsConfSetMessage->getHello());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MODE))
            m->setMode (pLldpScalarsConfSetMessage->getMode());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MULTIPLIER))
            m->setMultiplier (pLldpScalarsConfSetMessage->getMultiplier());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv (pLldpScalarsConfSetMessage->getDcbxFcoeAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv (pLldpScalarsConfSetMessage->getDcbxFcoeLogicalLinkTlv());
		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV))
			m->setDcbxIscsiAppTlv (pLldpScalarsConfSetMessage->getDcbxIscsiAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_ISCSIPRIORITY))
            m->setIscsiPriority (pLldpScalarsConfSetMessage->getIscsiPriority());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXTLV))
            m->setDcbxTlv (pLldpScalarsConfSetMessage->getDcbxTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT1XTLV))
            m->setDot1xTlv (pLldpScalarsConfSetMessage->getDot1xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT3XTLV))
            m->setDot3xTlv (pLldpScalarsConfSetMessage->getDot3xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv (pLldpScalarsConfSetMessage->getMgmtAddrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PORTDESCRTLV))
            m->setPortDescrTlv (pLldpScalarsConfSetMessage->getPortDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSCAPTLV))
            m->setSysCapTlv (pLldpScalarsConfSetMessage->getSysCapTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCRTLV))
            m->setSysDescrTlv (pLldpScalarsConfSetMessage->getSysDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAMETLV))
            m->setSysNameTlv (pLldpScalarsConfSetMessage->getSysNameTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAME))
            m->setSysName (pLldpScalarsConfSetMessage->getSysName());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCR))
            m->setSysDescr (pLldpScalarsConfSetMessage->getSysDescr());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DISABLE))
            m->setDisable (pLldpScalarsConfSetMessage->getDisable());
		
        /* When Vcs is enabled, we need to set Lldp System description by default in specific
         * format. When "no protocol lldp" command is issued, it resets all lldp configs to
         * default. It causes the System Description set to hardware mode name, which is not
         * intended in VCS mode. In Db we are already setting System descr like this. This
         * code will set to backend also.
         */
        if ((IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_NO_LLDP)) && (DcmToolKit::isVcsEnabled())) {
            string sysdescname;
            LldpUtils::GetSystemDescriptionName(sysdescname);
            m->setSysDescr(sysdescname);
            m->setCmdcode(LLDP_GLOBAL_CONF_NO_LLDP | LLDP_GLOBAL_CONF_SYSDESCR);
        }
        /* Send the message to backend deamon */
        status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
		
		pLldpScalarsConfSetMessage->setCompletionStatus(m->getCompletionStatus());
        if( status == WAVE_MESSAGE_SUCCESS) {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
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
				pLldpScalarsConfSetMessage->setCompletionStatus(ClientStatus);
				delete  m;
				return  ClientStatus;
			}
		}
		//bpatel
		if(m){
			delete m;
		}
        return status;
    }
	
	ResourceId LldpLocalConfWorker::updateLldpScalarsConfMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpScalarsConfSetMessage *pLldpScalarsConfSetMessage = 
				dynamic_cast<LldpScalarsConfSetMessage *> (
				pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		int status = WAVE_MESSAGE_SUCCESS;
        UI32 cmdcode = pLldpScalarsConfSetMessage->getCmdcode();
		
		LLDP_PLUG_DBG (
				string ("Msg Type:: ") + pLldpScalarsConfSetMessage->getMsgType()
			  + string (" Cmdcode::") + pLldpScalarsConfSetMessage->getCmdcode());

        /* 
		 * Send message to Nsm, for an update in Lldp Intf Mo
		 */
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_NO_LLDP)) {
			NsmLldpInternalMessage *m = new NsmLldpInternalMessage();
			if (m) {
				m->setCmdcode(LLDP_NSM_INTERNAL_NO_LLDP);
				LLDP_PLUG_DBG(string("send NsmLldpInternalMessage to Nsm Local Plugin"));
				status = sendSynchronously (m);
				pLldpScalarsConfSetMessage->setCompletionStatus(m->getCompletionStatus());
				if (status == WAVE_MESSAGE_SUCCESS) {
					if (m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
						LLDP_PLUG_DBG_ERR(string("Response processing: CompletionStatus") 
                		    + string("returned error: ")  + 
							FrameworkToolKit::localize(m->getCompletionStatus())); 
						status = m->getCompletionStatus();
						delete  m;
						return  status;
					}
					LLDP_PLUG_DBG_S(string("Response processing: CompletionStatus") 
                		    + string("returned success: ")  + 
						FrameworkToolKit::localize(m->getCompletionStatus()));
				}
				delete m;
			}
		}		
        return status;
    }

    void  LldpLocalConfWorker::LldpScalarsConfSetMessageHandler( LldpScalarsConfSetMessage *pLldpScalarsConfSetMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::updateLldpScalarsConf),
            
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::updateLldpScalarsConfMO),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLldpScalarsConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void
    LldpLocalConfWorker::LldpLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        LldpLocalShowMessage                    *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;
		ResourceId sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        LLDP_PLUG_DBG_TRACE();

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<LldpLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<LldpLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        LLDP_PLUG_ASSERT_PTR(req_msg_p);
        LLDP_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);
       
        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        LLDP_PLUG_DBG_S(string("Response processing: Num resp:") 
                       + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i ==0) { 
                LLDP_PLUG_DBG_S(string("locations:") + locations[i]);
            } else { 
                LLDP_PLUG_DBG_S(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<LldpLocalShowMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            LLDP_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
              //  req_msg_p->copyBuffersFrom(*sw_resp_p);
                LLDP_PLUG_DBG_ERR(string("WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus() "));
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 LLDP_PLUG_DBG_ERR(string("Response processing: The location: ") 
                            + locations[i] + string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 
				 sendToClusterCompletionStatus = sw_resp_p->getCompletionStatus();
				 req_msg_p->setCompletionStatus(sw_resp_p->getCompletionStatus());

            }
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }
    

    void
    LldpLocalConfWorker::sendShowCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        LldpLocalShowMessage        *msg_p = NULL;
        LldpLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        msg_p = dynamic_cast<LldpLocalShowMessage *>
            (seq_ctx_p->getPPrismMessage ());

        LLDP_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new LldpLocalShowMessage(msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             msg_p->getLastIfIndex(),
                                             msg_p->getNumRecords(),
                                             false);
        LLDP_PLUG_ASSERT_PTR(loc_msg_p);

        LLDP_PLUG_DBG_S(string("Sending Show Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() + 
                        string ("ifName =")  + msg_p->getIfName() +
                        string ("LastIfIndex =") + msg_p->getLastIfIndex() +
                        string ("NumRecords =")  + msg_p->getNumRecords());

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>
                (&LldpLocalConfWorker::LldpLocalShowSendToClusterCallback),
                seq_ctx_p);
        LLDP_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) { 
            return;
        }
        
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) { 
            /* If the interface context is not there, go to the local only
             * if it is LLDP_MGMT_CMD_SHOW_GBL
             */
            if (LLDP_MGMT_CMD_SHOW_GBL == msg_p->getCmdCode()) { 
                sendToLocal = true;
            }
        } else { 
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the 
                 * mappedId. 
                 */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
			         LLDP_PLUG_DBG_S(string("error in locationid"));
                    delete send_ctx_p;
                    delete loc_msg_p;
                    seq_ctx_p->executeNextStep(LLDP_API_ERR_IF_NOT_EXIST);
                    return;
                }
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        } 

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
			LLDP_PLUG_DBG_S(string("sending to Local location id"));
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) { 
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        } 
            
        sendToWaveCluster(send_ctx_p);

        return;
    }
    

    ResourceId 
    LldpLocalConfWorker::sendShowToLldpDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        LldpLocalShowMessage *msg_p = dynamic_cast<LldpLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        LldpShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = ""; 

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) { 
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
			LLDP_PLUG_DBG_S(string("valid three tupple") + msg_p->getIfName());
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else { 
            ifName = msg_p->getIfName();
        }
        LLDP_PLUG_DBG_S(string(" ifName  tupple") + ifName);
        cl_msg_p = new LldpShowMessage(ifName, msg_p->getCmdCode(), mappedId,
        		msg_p->getLastIfIndex(), msg_p->getNumRecords());

        //set timeout value of 120 sec for message. If daemon doesn't responsd within
        //120 sec, message will be timesout.
        cl_msg_p->setTimeOutInMilliSeconds (120000);

        status = sendSynchronouslyToWaveClient(LLDP_DCM_CLIENT_NAME, 
                                               cl_msg_p);

        LLDP_PLUG_DBG_S(string("Sending Show Command to Cluster") +
                        string(" MsgId =")  + cl_msg_p->getMessageId()  +
                        string(" CmdCode =") + cl_msg_p->getCmdCode() + 
                        string(" ifName =")  + cl_msg_p->getIfName() + 
                        string(" mappedId =") + cl_msg_p->getMappedId() +
                        string(" LastIfIndex =") + cl_msg_p->getLastIfIndex() +
                        string(" NumRecords =") + cl_msg_p->getNumRecords());

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(
                        string("Resp: Client Completion Error MsgId: ") + 
                        cl_msg_p->getMessageId() +  
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel
				status = msg_p->getCompletionStatus();
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return status;
            }

            if (cl_msg_p->getClientStatus() != WRC_WAVE_MESSAGE_SUCCESS) { 
                LLDP_PLUG_DBG_ERR(string("Resp:  Client Error MsgId: ") + 
                                  cl_msg_p->getMessageId() + 
                FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
    			//bpatel
                status = msg_p->getCompletionStatus();
				if(cl_msg_p){                
					delete cl_msg_p;
				}
                return status;             
            }

        }
        
        if (status != WAVE_MESSAGE_SUCCESS) {
            LLDP_PLUG_DBG(
            string("Client Message sending failed for MsgId: ") + 
                    cl_msg_p->getMessageId() + 
                    string("reason:") +  FrameworkToolKit::localize(status));
            	//bpatel
				if(cl_msg_p){
                    delete msg_p;
                }
				return status;
        }

        LLDP_PLUG_DBG(
                string("Client Message processing succesful for MsgId: ") + 
                cl_msg_p->getMessageId());
                      

        //msg_p->copyBuffersFrom(*cl_msg_p);
        msg_p->copyAllBuffers(*cl_msg_p);

        //bpatel
		status = cl_msg_p->getCompletionStatus();
		delete  cl_msg_p;
		return  status; 
    }

    void  
    LldpLocalConfWorker::LldpLocalShowMessageHandler (
            LldpLocalShowMessage *pLldpLocalShowMessage)
    {

        if (pLldpLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::sendShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pLldpLocalShowMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&LldpLocalConfWorker::sendShowToLldpDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&LldpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&LldpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pLldpLocalShowMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


    void
    LldpLocalConfWorker::LldpLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        LldpLocalClearMessage                    *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;
		ResourceId sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        LLDP_PLUG_DBG_TRACE();

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<LldpLocalClearMessage *>
                    (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<LldpLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        LLDP_PLUG_ASSERT_PTR(req_msg_p);
        LLDP_PLUG_ASSERT_PTR(resp_msg_p);
       
        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        LLDP_PLUG_DBG_S(string("Response processing: Num resp:") 
                       + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i ==0) { 
                LLDP_PLUG_DBG_S(string("locations:") + locations[i]);
            } else { 
                LLDP_PLUG_DBG_S(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<LldpLocalClearMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            LLDP_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                LLDP_PLUG_DBG_S(string("WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus() "));
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 LLDP_PLUG_DBG_ERR(string("Response processing: The location: ") 
                            + locations[i] + string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 
				 sendToClusterCompletionStatus = sw_resp_p->getCompletionStatus();
				 req_msg_p->setCompletionStatus(sw_resp_p->getCompletionStatus());

            }
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }
    

    void
    LldpLocalConfWorker::sendClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        LldpLocalClearMessage        *msg_p = NULL;
        LldpLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        msg_p = dynamic_cast<LldpLocalClearMessage *>
            (seq_ctx_p->getPPrismMessage ());

        LLDP_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new LldpLocalClearMessage(msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);
        LLDP_PLUG_ASSERT_PTR(loc_msg_p);
		loc_msg_p->setInterface(msg_p->getInterface());

        LLDP_PLUG_DBG_S(string("Sending Clear Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() + 
                        string ("ifName =")  + msg_p->getIfName());

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>
                (&LldpLocalConfWorker::LldpLocalClearSendToClusterCallback),
                seq_ctx_p);
        LLDP_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) { 
            return;
        }
        
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) { 
            /* If the interface context is not there, distribute cluster wide
             */
        } else { 
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the 
                 * mappedId. 
                 */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
				if (locationId == 0) {
                    delete send_ctx_p;
                    delete loc_msg_p;
                    seq_ctx_p->executeNextStep(LLDP_API_ERR_IF_NOT_EXIST);
                    return;
                }
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        } 

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) { 
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        } 
            
        sendToWaveCluster(send_ctx_p);

        return;
    }
    

    ResourceId 
    LldpLocalConfWorker::sendClearToLldpDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        LldpLocalClearMessage *msg_p = dynamic_cast<LldpLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        LldpClearMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = ""; 

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) { 
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else { 
            ifName = msg_p->getIfName();
        }


        cl_msg_p = new LldpClearMessage(ifName, msg_p->getCmdCode(), mappedId);
		cl_msg_p->setInterface(msg_p->getInterface());

        //set timeout value of 120 sec for message. If daemon doesn't responsd within
        //120 sec, message will be timesout.
        cl_msg_p->setTimeOutInMilliSeconds (120000);

        status = sendSynchronouslyToWaveClient(LLDP_DCM_CLIENT_NAME, 
                                               cl_msg_p);

        LLDP_PLUG_DBG_S(string("Sending Clear Command to Cluster") +
                        string(" MsgId =")  + cl_msg_p->getMessageId()  +
                        string(" CmdCode =") + cl_msg_p->getCmdCode() + 
                        string(" ifName =")  + cl_msg_p->getIfName() + 
                        string(" mappedId =") + cl_msg_p->getMappedId());

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(
                        string("Resp: Client Completion Error MsgId: ") + 
                        cl_msg_p->getMessageId() +  
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				status = msg_p->getCompletionStatus();
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return status;
            }

            if (cl_msg_p->getClientStatus() != WRC_WAVE_MESSAGE_SUCCESS) { 
                LLDP_PLUG_DBG_ERR(string("Resp:  Client Error MsgId: ") + 
                                  cl_msg_p->getMessageId() + 
                FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                status = msg_p->getCompletionStatus();
				if(cl_msg_p){                
					delete cl_msg_p;
				}
                return status;             
            }

        }
        
        if (status != WAVE_MESSAGE_SUCCESS) {
            LLDP_PLUG_DBG(
            string("Client Message sending failed for MsgId: ") + 
                    cl_msg_p->getMessageId() + 
                    string("reason:") +  FrameworkToolKit::localize(status));
			if(cl_msg_p){
				delete msg_p;
			}
			return status;
        }

        LLDP_PLUG_DBG(
                string("Client Message processing succesful for MsgId: ") + 
                cl_msg_p->getMessageId());
                      
        msg_p->copyAllBuffers(*cl_msg_p);

		status = cl_msg_p->getCompletionStatus();
		delete  cl_msg_p;
		return  status; 
    }

    void  
    LldpLocalConfWorker::LldpLocalClearMessageHandler (
            LldpLocalClearMessage *pLldpLocalClearMessage)
    {

        if (pLldpLocalClearMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::sendClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&LldpLocalConfWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pLldpLocalClearMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&LldpLocalConfWorker::sendClearToLldpDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&LldpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&LldpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pLldpLocalClearMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


    void
    LldpLocalConfWorker::LldpLocalLldpDebugSendToClusterCallback (
        WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        LldpLocalLldpDebugMessage               *req_msg_p = NULL,
        *resp_msg_p = NULL,
        *sw_resp_p = NULL;

        LLDP_PLUG_DBG_TRACE();

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
                    (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<LldpLocalLldpDebugMessage *>
                    (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<LldpLocalLldpDebugMessage *>
                      (ctx_p->getPPrismMessageForPhase1());

        LLDP_PLUG_ASSERT_PTR(req_msg_p);
        LLDP_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        LLDP_PLUG_DBG_S(string("Response processing: Num resp:") 
                        + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i ==0) { 
                LLDP_PLUG_DBG_S(string("locations:") + locations[i]);
            } else { 
                LLDP_PLUG_DBG_S(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<LldpLocalLldpDebugMessage *>
                        (ctx_p->getResultingMessageForPhase1(locations[i]));
            LLDP_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
              //  req_msg_p->copyBuffersFrom(*sw_resp_p);
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                LLDP_PLUG_DBG_ERR(string("Response processing: The location: ") 
                                  + locations[i] + string("returned error: ")  + 
                                  FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 

            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }


    void
    LldpLocalConfWorker::sendLldpDebugCommandToCluster (
        PrismLinearSequencerContext *seq_ctx_p)
    {
        LldpLocalLldpDebugMessage   *msg_p = NULL;
        LldpLocalLldpDebugMessage   *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        msg_p = dynamic_cast<LldpLocalLldpDebugMessage *>
                (seq_ctx_p->getPPrismMessage ());

        LLDP_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new LldpLocalLldpDebugMessage(msg_p->getIfName(),
                                                  msg_p->getCmdCode(),
                                                  msg_p->getDbgFlags(),
                                                  msg_p->getLastIfIndex(),
                                                  msg_p->getNumRecords(),
                                                  false);
        LLDP_PLUG_ASSERT_PTR(loc_msg_p);

        LLDP_PLUG_DBG_S(string("Sending Lldp Debug Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() + 
                        string(" DbgFlags =") + msg_p->getDbgFlags() + 
                        string ("ifName =")  + msg_p->getIfName() +
                        string ("LastIfIndex =")  + msg_p->getLastIfIndex() +
                        string ("NumRecords =")  + msg_p->getNumRecords());

        send_ctx_p = new WaveSendToClusterContext(this, 
            reinterpret_cast<PrismAsynchronousCallback>
            (&LldpLocalConfWorker::LldpLocalLldpDebugSendToClusterCallback),
            seq_ctx_p);
        LLDP_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) { 
            return;
        }

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (0 == msg_p->getIfName().length()) {
            /* If the interface context is not there, go to the local only
             * if it is LLDP_MGMT_CMD_SHOW_GBL
             */
            if (LLDP_DEBUG_CMD_CODE_DUMP_ALL == msg_p->getCmdCode()) {
                sendToLocal = true;
            }
        } else {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
                /* Send to the targetted LocationId pointed to by the 
                 * mappedId. 
                 */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
		if (locationId == 0) {
                    delete send_ctx_p;
                    delete loc_msg_p;
                    seq_ctx_p->executeNextStep(LLDP_API_ERR_IF_NOT_EXIST);
                    return;
                }
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) { 
                locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        } 

        sendToWaveCluster(send_ctx_p);

        return;
    }


    ResourceId 
    LldpLocalConfWorker::sendLldpDebugToLldpDaemon (
        PrismSynchronousLinearSequencerContext *ctx_p)
    {
        LldpLocalLldpDebugMessage *msg_p = dynamic_cast<LldpLocalLldpDebugMessage *>
                                      (ctx_p->getPPrismMessage ());
        LldpClientDebugMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = ""; 

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) { 
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
        } else { 
            ifName = msg_p->getIfName();
        }

        cl_msg_p = new LldpClientDebugMessage(ifName, msg_p->getCmdCode(),
                                              msg_p->getDbgFlags(), mappedId,
                                              msg_p->getLastIfIndex(), msg_p->getNumRecords());

        //set timeout value of 120 sec for message. If daemon doesn't responsd within
        //120 sec, message will be timesout.
        cl_msg_p->setTimeOutInMilliSeconds (120000);

        status = sendSynchronouslyToWaveClient(LLDP_DCM_CLIENT_NAME, 
                                               cl_msg_p);

        LLDP_PLUG_DBG_S(string("Sending Lldp Debug Command to Cluster") +
                        string(" MsgId =")  + cl_msg_p->getMessageId()  +
                        string(" CmdCode =") + cl_msg_p->getCmdCode() + 
                        string(" DbgFlags =") + cl_msg_p->getDbgFlags() + 
                        string(" ifName =")  + cl_msg_p->getIfName() + 
                        string(" mappedId =") + cl_msg_p->getMappedId() +
                        string(" LastIfIndex =") + cl_msg_p->getLastIfIndex() +
                        string(" NumRecords =") + cl_msg_p->getNumRecords());

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                LLDP_PLUG_DBG_ERR(
                                  string("Resp: Client Completion Error MsgId: ") + 
                                  cl_msg_p->getMessageId() +  
                                  FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel
				if(cl_msg_p){
					delete cl_msg_p;
				}
                return msg_p->getCompletionStatus();
            }

            if (cl_msg_p->getClientStatus() != 0) { 
                LLDP_PLUG_DBG_ERR(string("Resp:  Client Error MsgId: ") + 
                                  cl_msg_p->getMessageId() + 
                                  FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel
                if(cl_msg_p){
                    delete cl_msg_p;
                }
                return msg_p->getCompletionStatus();
            }

        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            LLDP_PLUG_DBG(
                          string("Client Message sending failed for MsgId: ") + 
                          cl_msg_p->getMessageId() + 
                          string("reason:") +  FrameworkToolKit::localize(status));
            //bpatel
                if(cl_msg_p){
                    delete cl_msg_p;
                }
            return status;
        }

        LLDP_PLUG_DBG(
                      string("Client Message processing succesful for MsgId: ") + 
                      cl_msg_p->getMessageId());


        //msg_p->copyBuffersFrom(*cl_msg_p);
        msg_p->copyAllBuffers(*cl_msg_p);

		status = cl_msg_p->getCompletionStatus();
		delete  cl_msg_p;
		return  status;
    }

    void
    LldpLocalConfWorker::LldpLocalLldpDebugMessageHandler (
        LldpLocalLldpDebugMessage *pLldpLocalLldpDebugMessage)
    {

        if (pLldpLocalLldpDebugMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep>
                        (&LldpLocalConfWorker::sendLldpDebugCommandToCluster),

                reinterpret_cast<PrismLinearSequencerStep>
                        (&LldpLocalConfWorker::prismLinearSequencerSucceededStep),

                reinterpret_cast<PrismLinearSequencerStep>
                        (&LldpLocalConfWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pLldpLocalLldpDebugMessage, 
                                                this, sequencerSteps, 
                                                sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&LldpLocalConfWorker::sendLldpDebugToLldpDaemon),

                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&LldpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),

                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&LldpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)   
            };

            PrismSynchronousLinearSequencerContext 
                    *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                pLldpLocalLldpDebugMessage, this, sequencerSteps, 
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }




}




