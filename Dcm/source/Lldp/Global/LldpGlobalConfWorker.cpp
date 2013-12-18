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
 *   Author : mmohan                                                       *
 **************************************************************************/
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalConfWorker.h"
#include "Lldp/Global/LldpGlobalScalarsConfSetMessage.h"
#include "Lldp/Global/LldpGlobalProfileConfSetMessage.h"
#include "Lldp/Global/LldpAdvertiseParamsManagedObject.h"
#include "Lldp/Global/LldpGlobalScalarsConfManagedObject.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Lldp/Common/LldpPluginDebug.h"
#include "Lldp/Local/LldpScalarsConfSetMessage.h"
#include "Lldp/Local/LldpProfileConfSetMessage.h"
#include "Lldp/Global/LldpGlobalProfileGetObjectIdMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpUtils.h"
#include "Nsm/Local/NsmLldpInternalMessage.h"
#include "Utils/DceClusterUtils.h"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    LldpGlobalConfWorker::LldpGlobalConfWorker ( LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : WaveWorker  (pLldpGlobalObjectManager)
    {
        LldpAdvertiseParamsManagedObject    LldpAdvertiseParamsManagedObject    (pLldpGlobalObjectManager);
        LldpGlobalScalarsConfManagedObject    LldpGlobalScalarsConfManagedObject    (pLldpGlobalObjectManager);
        LldpGlobalProfileConfManagedObject    LldpGlobalProfileConfManagedObject    (pLldpGlobalObjectManager);
        LldpAdvertiseParamsManagedObject.setupOrm ();
        addManagedClass (LldpAdvertiseParamsManagedObject::getClassName (), this);
        LldpGlobalScalarsConfManagedObject.setupOrm ();
        addManagedClass (LldpGlobalScalarsConfManagedObject::getClassName (), this);
        LldpGlobalProfileConfManagedObject.setupOrm ();
        addManagedClass (LldpGlobalProfileConfManagedObject::getClassName (), this);
        addOperationMap (LLDPGLOBALSCALARSCONFSET, reinterpret_cast<PrismMessageHandler> (&LldpGlobalConfWorker::LldpGlobalScalarsConfSetMessageHandler));
        addOperationMap (LLDPGLOBALPROFILECONFSET, reinterpret_cast<PrismMessageHandler> (&LldpGlobalConfWorker::LldpGlobalProfileConfSetMessageHandler));
        addOperationMap (LLDPGLOBALPROFILEGETOBJECTID, reinterpret_cast<PrismMessageHandler> (&LldpGlobalConfWorker::LldpGlobalProfileGetObjectIdMessageHandler));
    }

    LldpGlobalConfWorker::~LldpGlobalConfWorker ()
    {
    }

    PrismMessage  *LldpGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case LLDPGLOBALSCALARSCONFSET :
                pPrismMessage = new LldpGlobalScalarsConfSetMessage ();
                break;
            case LLDPGLOBALPROFILECONFSET :
                pPrismMessage = new LldpGlobalProfileConfSetMessage ();
                break;
            case LLDPGLOBALPROFILEGETOBJECTID :
                pPrismMessage = new LldpGlobalProfileGetObjectIdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LldpGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((LldpAdvertiseParamsManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LldpAdvertiseParamsManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((LldpGlobalScalarsConfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LldpGlobalScalarsConfManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((LldpGlobalProfileConfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LldpGlobalProfileConfManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
			LLDP_PLUG_DBG(string("Unknown Managed Class Name:- ") + managedClassName);
        }
        return (pWaveManagedObject);
    }
	
	void
	LldpGlobalConfWorker::LldpGlobalProfileConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		LLDP_PLUG_DBG_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			LLDP_PLUG_DBG(string("Phase 1 message succeeded on all locations"));
		} else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on all locations"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes"));
		} else {
			LLDP_PLUG_DBG(string("CompletionStatus::Unknown Error:- ") + sendToClusterCompletionStatus);
		}
        delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		LLDP_PLUG_DBG(string("Done"));
        return;
     }
	
	void
	LldpGlobalConfWorker::updateLldpGlobalProfileConf (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpGlobalProfileConfSetMessage *pLldpGlobalProfileConfSetMessage = dynamic_cast<LldpGlobalProfileConfSetMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

        LldpProfileConfSetMessage *m = new LldpProfileConfSetMessage();
        UI32 cmdcode = pLldpGlobalProfileConfSetMessage->getCmdcode();
		WaveSendToClusterContext    *pWaveSendToClusterContext;
		
		/* create clustercontext and initialize with callback func */
		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&LldpGlobalConfWorker::LldpGlobalProfileConfigSendToClusterCallback), pPrismLinearSequencerContext);

        /* Read the header from received message and populate 
		 * sending message
		 */
		m->setMgmtFlags (pLldpGlobalProfileConfSetMessage->getMgmtFlags());
        m->setMsgType (pLldpGlobalProfileConfSetMessage->getMsgType());
        m->setCmdcode (pLldpGlobalProfileConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PROFILE))
            m->setProfileName (pLldpGlobalProfileConfSetMessage->getProfileName());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DESC))
            m->setDescription ((pLldpGlobalProfileConfSetMessage->getDescription()).getStringValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_HELLO))
            m->setHello ((pLldpGlobalProfileConfSetMessage->getHello()).getUI32Value());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MODE))
            m->setMode ((pLldpGlobalProfileConfSetMessage->getMode()).getUI32Value());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER))
            m->setMultiplier ((pLldpGlobalProfileConfSetMessage->getMultiplier()).getUI32Value());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv ((pLldpGlobalProfileConfSetMessage->getDcbxFcoeAppTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv ((pLldpGlobalProfileConfSetMessage->getDcbxFcoeLogicalLinkTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV))
            m->setDcbxIscsiAppTlv ((pLldpGlobalProfileConfSetMessage->getDcbxIscsiAppTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXTLV))
            m->setDcbxTlv ((pLldpGlobalProfileConfSetMessage->getDcbxTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV))
            m->setDot1xTlv ((pLldpGlobalProfileConfSetMessage->getDot1xTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV))
            m->setDot3xTlv ((pLldpGlobalProfileConfSetMessage->getDot3xTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv ((pLldpGlobalProfileConfSetMessage->getMgmtAddrTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV))
            m->setPortDescrTlv ((pLldpGlobalProfileConfSetMessage->getPortDescrTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV))
            m->setSysCapTlv ((pLldpGlobalProfileConfSetMessage->getSysCapTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV))
            m->setSysDescrTlv ((pLldpGlobalProfileConfSetMessage->getSysDescrTlv()).getBoolValue());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV))
            m->setSysNameTlv ((pLldpGlobalProfileConfSetMessage->getSysNameTlv()).getBoolValue());

		/* Enable surrogate support */
		m->setNeedSurrogateSupportFlag(true);
		/* Set Phase 1 message */
		pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		/* Enable partial success flag */
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		/* Enable flag to treat msg to failing over node as success */
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
		/* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

        return;

    }

	void LldpGlobalConfWorker::copyMasterMoToProfileMo (PrismLinearSequencerContext *pPrismLinearSequencerContext,
					LldpGlobalProfileConfManagedObject *m)
	{
		LLDP_PLUG_DBG_TRACE();
        LldpGlobalScalarsConfManagedObject *gm = NULL;
		WaveManagedObjectSynchronousQueryContext gsyncQueryCtxt(LldpGlobalScalarsConfManagedObject::getClassName());
		vector<WaveManagedObject *> *gpResults = querySynchronously (&gsyncQueryCtxt);
		gm = dynamic_cast<LldpGlobalScalarsConfManagedObject*>((*gpResults)[0]);

		StringUC descriptionUC (gm->getDescription(), false); 
		m->setDescription (descriptionUC);
		UI32UC helloUC (gm->getHello(), false); 
		m->setHello (helloUC);
		EnumUC modeUC ((LldpMode)(gm->getMode()), false);
		m->setMode (modeUC);
		UI32UC multiplierUC (gm->getMultiplier(), false); 
		m->setMultiplier (multiplierUC);
		BoolUC dcbxFcoeAppTlvUC (gm->getDcbxFcoeAppTlv(), false); 
		m->setDcbxFcoeAppTlv (dcbxFcoeAppTlvUC);
		BoolUC dcbxFcoeLogicalLinkTlvUC (gm->getDcbxFcoeLogicalLinkTlv(), false); 
		m->setDcbxFcoeLogicalLinkTlv (dcbxFcoeLogicalLinkTlvUC);
		BoolUC dcbxIscsiAppTlvUC (gm->getDcbxFcoeAppTlv(), false); 
		m->setDcbxIscsiAppTlv (dcbxIscsiAppTlvUC);
//		UI32UC iscsiPriorityUC (gm->getIscsiPriority(), false); 
//		m->setIscsiPriority (gm->getIscsiPriority());
		BoolUC dcbxTlvUC (gm->getDcbxTlv(), false); 
		m->setDcbxTlv (dcbxTlvUC);
		BoolUC dot1xTlvUC (gm->getDot1xTlv(), false); 
		m->setDot1xTlv (dot1xTlvUC);
		BoolUC dot3xTlvUC (gm->getDot3xTlv(), false); 
		m->setDot3xTlv (dot3xTlvUC);
		BoolUC mgmtAddrTlvUC (gm->getMgmtAddrTlv(), false); 
		m->setMgmtAddrTlv (mgmtAddrTlvUC);
		BoolUC portDescrTlvUC (gm->getPortDescrTlv(), false); 
		m->setPortDescrTlv (portDescrTlvUC);
		BoolUC sysCapTlvUC (gm->getSysCapTlv(), false); 
		m->setSysCapTlv (sysCapTlvUC);
		BoolUC sysDescrTlvUC (gm->getSysDescrTlv(), false); 
		m->setSysDescrTlv (sysDescrTlvUC);
		BoolUC sysNameTlvUC (gm->getSysNameTlv(), false); 
		m->setSysNameTlv (sysNameTlvUC);
		
		// Sanjeev: mark the query results for GC.
		pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
                    *gpResults);
		delete gpResults;
	}

  void LldpGlobalConfWorker::updateLldpGlobalProfileMO (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        int createFlag = 1, updateFlag = 0;
        int status = 0;
        LldpGlobalProfileConfManagedObject *m = NULL;
        LldpGlobalProfileConfSetMessage *pLldpGlobalProfileConfSetMessage = dynamic_cast<LldpGlobalProfileConfSetMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		const string profileName = pLldpGlobalProfileConfSetMessage->getProfileName();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		UI32 cmdcode = pLldpGlobalProfileConfSetMessage->getCmdcode();
	        
        if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
				LLDP_PLUG_DBG(string("numberOfResults = 0"));
		        createFlag = 1;
		        status = WAVE_MESSAGE_SUCCESS;
	        } else if (1 == numberOfResults) {
		        m = dynamic_cast<LldpGlobalProfileConfManagedObject*>((*pResults)[0]);
				LLDP_PLUG_DBG(string("numberOfResults = 1"));
		        createFlag = 0;
		        status = WAVE_MESSAGE_SUCCESS;
            } else {
				LLDP_PLUG_DBG(string("numberOfResults = ") + numberOfResults);
                status = WAVE_MESSAGE_ERROR;
            }
        }

	    if (createFlag == 0) {
			LLDP_PLUG_DBG(string("createFlag = 0"));
			if(IS_CMDCODE_SET(pLldpGlobalProfileConfSetMessage->getMsgType(),LLDP_DELETE_CONF)) {
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PROFILE)) {
					LLDP_PLUG_DBG(string("Deleted ") + profileName);
					delete m;
				}
			} else { 
				LLDP_PLUG_DBG(string("Update"));
	            updateWaveManagedObject (m);
            	prismAssert (NULL != m, __FILE__, __LINE__);
				updateFlag = 1;
			}
        } else {
            m = new LldpGlobalProfileConfManagedObject(dynamic_cast<LldpGlobalObjectManager*>(getPWaveObjectManager()));
			m->setProfileName (profileName);
			LLDP_PLUG_DBG(string("Created profile:- ") + profileName);
			copyMasterMoToProfileMo (pPrismLinearSequencerContext, m);
			
        }
        
		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DESC))
            m->setDescription (pLldpGlobalProfileConfSetMessage->getDescription());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_HELLO))
            m->setHello (pLldpGlobalProfileConfSetMessage->getHello());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MODE))
            m->setMode ((pLldpGlobalProfileConfSetMessage->getMode()));
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER))
            m->setMultiplier (pLldpGlobalProfileConfSetMessage->getMultiplier());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv (pLldpGlobalProfileConfSetMessage->getDcbxFcoeAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv (pLldpGlobalProfileConfSetMessage->getDcbxFcoeLogicalLinkTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV))
            m->setDcbxIscsiAppTlv (pLldpGlobalProfileConfSetMessage->getDcbxIscsiAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXTLV))
            m->setDcbxTlv (pLldpGlobalProfileConfSetMessage->getDcbxTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV))
            m->setDot1xTlv (pLldpGlobalProfileConfSetMessage->getDot1xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV))
            m->setDot3xTlv (pLldpGlobalProfileConfSetMessage->getDot3xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv (pLldpGlobalProfileConfSetMessage->getMgmtAddrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV))
            m->setPortDescrTlv (pLldpGlobalProfileConfSetMessage->getPortDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV))
            m->setSysCapTlv (pLldpGlobalProfileConfSetMessage->getSysCapTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV))
            m->setSysDescrTlv (pLldpGlobalProfileConfSetMessage->getSysDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV))
            m->setSysNameTlv (pLldpGlobalProfileConfSetMessage->getSysNameTlv());
        
        if ( (createFlag) && (m)) { 
            //Sanjeev: mark the MO for GC 
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
                    m);
        }

		if (createFlag || updateFlag) {
            // Sanjeev: mark the query results for GC.
            if (pResults) { 
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
                        *pResults);
            }
		}

        if (pResults) { 
            delete pResults;
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : sagar : Do we need to handle this case?
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
  }

    void  LldpGlobalConfWorker::LldpGlobalProfileConfSetMessageHandler( LldpGlobalProfileConfSetMessage *pLldpGlobalProfileConfSetMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::updateLldpGlobalProfileConf),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::updateLldpGlobalProfileMO),

            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pLldpGlobalProfileConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }
	
	void
	LldpGlobalConfWorker::LldpGlobalScalarsConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		LLDP_PLUG_DBG_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			LLDP_PLUG_DBG(string("Phase 1 message succeeded on all locations"));
		} else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on all locations"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes"));
		} else {
			LLDP_PLUG_DBG(string("Unknown Error:- ") + sendToClusterCompletionStatus);
		}

        delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		LLDP_PLUG_DBG(string("Done"));
        return;
     }
	
	void LldpGlobalConfWorker::updateLldpGlobalScalarsConf (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        LldpGlobalScalarsConfSetMessage *pLldpGlobalScalarsConfSetMessage = dynamic_cast<LldpGlobalScalarsConfSetMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

        LldpScalarsConfSetMessage *m = new LldpScalarsConfSetMessage();
        UI32 cmdcode = pLldpGlobalScalarsConfSetMessage->getCmdcode();
		WaveSendToClusterContext    *pWaveSendToClusterContext;
		
		/* create clustercontext and initialize with callback func */
		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&LldpGlobalConfWorker::LldpGlobalScalarsConfigSendToClusterCallback), pPrismLinearSequencerContext);
		
        /* Read the header from received message and populate 
		 * sending message
		 */
		m->setMgmtFlags (pLldpGlobalScalarsConfSetMessage->getMgmtFlags());
        m->setMsgType (pLldpGlobalScalarsConfSetMessage->getMsgType());
        m->setCmdcode (pLldpGlobalScalarsConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DESC))
            m->setDescription (pLldpGlobalScalarsConfSetMessage->getDescription());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_HELLO))
            m->setHello (pLldpGlobalScalarsConfSetMessage->getHello());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MODE))
            m->setMode (pLldpGlobalScalarsConfSetMessage->getMode());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MULTIPLIER))
            m->setMultiplier (pLldpGlobalScalarsConfSetMessage->getMultiplier());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv (pLldpGlobalScalarsConfSetMessage->getDcbxFcoeAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv (pLldpGlobalScalarsConfSetMessage->getDcbxFcoeLogicalLinkTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV))
            m->setDcbxIscsiAppTlv (pLldpGlobalScalarsConfSetMessage->getDcbxIscsiAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_ISCSIPRIORITY))
            m->setIscsiPriority (pLldpGlobalScalarsConfSetMessage->getIscsiPriority());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXTLV))
            m->setDcbxTlv (pLldpGlobalScalarsConfSetMessage->getDcbxTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT1XTLV))
            m->setDot1xTlv (pLldpGlobalScalarsConfSetMessage->getDot1xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT3XTLV))
            m->setDot3xTlv (pLldpGlobalScalarsConfSetMessage->getDot3xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv (pLldpGlobalScalarsConfSetMessage->getMgmtAddrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PORTDESCRTLV))
            m->setPortDescrTlv (pLldpGlobalScalarsConfSetMessage->getPortDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSCAPTLV))
            m->setSysCapTlv (pLldpGlobalScalarsConfSetMessage->getSysCapTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCRTLV))
            m->setSysDescrTlv (pLldpGlobalScalarsConfSetMessage->getSysDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAMETLV))
            m->setSysNameTlv (pLldpGlobalScalarsConfSetMessage->getSysNameTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAME))
            m->setSysName (pLldpGlobalScalarsConfSetMessage->getSysName());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCR))
            m->setSysDescr (pLldpGlobalScalarsConfSetMessage->getSysDescr());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DISABLE))
            m->setDisable (pLldpGlobalScalarsConfSetMessage->getDisable());
        
		/* Enable surrogate support */
		m->setNeedSurrogateSupportFlag(true);
		/* Set Phase 1 message */
		pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		/* Enable partial success flag */
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		/* Enable flag to treat msg to failing over node as success */
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
		/* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

        return;

    }

  
	int LldpGlobalConfWorker::LldpGlobalDeleteAllProfiles()
	{
		LLDP_PLUG_DBG_TRACE();
        // Delete all entries from the table in one query.
        deleteAllManagedObjectInstances(LldpGlobalProfileConfManagedObject::getClassName());

        return WAVE_MESSAGE_SUCCESS;
	}
		

  void LldpGlobalConfWorker::updateNonUserProfileConfigs (PrismLinearSequencerContext *pPrismLinearSequencerContext)
  	{
		LLDP_PLUG_DBG_TRACE();
        LldpGlobalScalarsConfSetMessage *m = dynamic_cast<LldpGlobalScalarsConfSetMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        LldpGlobalProfileConfManagedObject *profileMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		UI32 cmdcode = m->getCmdcode();
	
		if (NULL == pResults)
			return;
		
		UI32 numberOfResults = pResults->size ();
		if (numberOfResults > 0) {
			LLDP_PLUG_DBG(string("numberOfResults = ") + numberOfResults);
			for (UI32 i=0; i<numberOfResults; i++) {
				profileMo = dynamic_cast<LldpGlobalProfileConfManagedObject *>((*pResults)[i]);
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DESC)) {
					StringUC  descriptionUC = profileMo->getDescription();
					if (!descriptionUC.getIsUserConfigured ()) {
						descriptionUC.setStringValue (m->getDescription());
						profileMo->setDescription (descriptionUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_HELLO)) {
					UI32UC  helloUC = profileMo->getHello();
					if (!helloUC.getIsUserConfigured ()) {
						helloUC.setUI32Value (m->getHello());
						profileMo->setHello (helloUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MODE)) {
					EnumUC modeUC = profileMo->getMode();
					if (!modeUC.getIsUserConfigured ()) {
						modeUC.setUI32Value (m->getMode());
						profileMo->setMode (modeUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MULTIPLIER)) {
					UI32UC multiplierUC = profileMo->getMultiplier();
					if (!multiplierUC.getIsUserConfigured ()) {
						multiplierUC.setUI32Value (m->getMultiplier());
						profileMo->setMultiplier (multiplierUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV)) {
					BoolUC dcbxFcoeAppTlvUC = profileMo->getDcbxFcoeAppTlv();
					if (!dcbxFcoeAppTlvUC.getIsUserConfigured ()) {
						dcbxFcoeAppTlvUC.setBoolValue(m->getDcbxFcoeAppTlv());
						profileMo->setDcbxFcoeAppTlv (dcbxFcoeAppTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV)) {
					BoolUC dcbxFcoeLogicalLinkTlvUC = profileMo->getDcbxFcoeLogicalLinkTlv();
					if (!dcbxFcoeLogicalLinkTlvUC.getIsUserConfigured ()) {
						dcbxFcoeLogicalLinkTlvUC.setBoolValue(m->getDcbxFcoeLogicalLinkTlv());
						profileMo->setDcbxFcoeLogicalLinkTlv (dcbxFcoeLogicalLinkTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV)) {
					BoolUC dcbxIscsiAppTlvUC = profileMo->getDcbxIscsiAppTlv();
					if (!dcbxIscsiAppTlvUC.getIsUserConfigured ()) {
						dcbxIscsiAppTlvUC.setBoolValue(m->getDcbxIscsiAppTlv());
						profileMo->setDcbxIscsiAppTlv (dcbxIscsiAppTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXTLV)) {
					BoolUC dcbxTlvUC = profileMo->getDcbxTlv();
					if (!dcbxTlvUC.getIsUserConfigured ()) {
						dcbxTlvUC.setBoolValue(m->getDcbxTlv());
						profileMo->setDcbxTlv (dcbxTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT1XTLV)) {
					BoolUC dot1xTlvUC = profileMo->getDot1xTlv();
					if (!dot1xTlvUC.getIsUserConfigured ()) {
						dot1xTlvUC.setBoolValue(m->getDot1xTlv());
						profileMo->setDot1xTlv (dot1xTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT3XTLV)) {
					BoolUC dot3xTlvUC = profileMo->getDot3xTlv();
					if (!dot3xTlvUC.getIsUserConfigured ()) {
						dot3xTlvUC.setBoolValue(m->getDot3xTlv());
						profileMo->setDot3xTlv (dot3xTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MGMTADDRTLV)) {
					BoolUC mgmtAddrTlvUC = profileMo->getMgmtAddrTlv();
					if (!mgmtAddrTlvUC.getIsUserConfigured ()) {
						mgmtAddrTlvUC.setBoolValue(m->getMgmtAddrTlv());
						profileMo->setMgmtAddrTlv (mgmtAddrTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PORTDESCRTLV)) {
					BoolUC portDescrTlvUC = profileMo->getPortDescrTlv();
					if (!portDescrTlvUC.getIsUserConfigured ()) {
						portDescrTlvUC.setBoolValue(m->getPortDescrTlv());
						profileMo->setPortDescrTlv (portDescrTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSCAPTLV)) {
					BoolUC sysCapTlvUC = profileMo->getSysCapTlv();
					if (!sysCapTlvUC.getIsUserConfigured ()) {
						sysCapTlvUC.setBoolValue(m->getSysCapTlv());
						profileMo->setSysCapTlv (sysCapTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCRTLV)) {
					BoolUC sysDescrTlvUC = profileMo->getSysDescrTlv();
					if (!sysDescrTlvUC.getIsUserConfigured ()) {
						sysDescrTlvUC.setBoolValue(m->getSysDescrTlv());
						profileMo->setSysDescrTlv (sysDescrTlvUC);
					}
				}
				if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAMETLV)) {
					BoolUC sysNameTlvUC = profileMo->getSysNameTlv();
					if (!sysNameTlvUC.getIsUserConfigured ()) {
						sysNameTlvUC.setBoolValue(m->getSysNameTlv());
						profileMo->setSysNameTlv (sysNameTlvUC);
					}
				}
            	updateWaveManagedObject (profileMo);
			}
		} else {
			LLDP_PLUG_DBG(string("no profile present::numberOfResults = ") + numberOfResults);
		}
		
		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
							*pResults);
			delete pResults;
		}
	}

  void LldpGlobalConfWorker::updateLldpGlobalScalarsMO (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        int createFlag = 1;
        int status = 0;
        LldpGlobalScalarsConfManagedObject *m = NULL;
        LldpGlobalScalarsConfSetMessage *pLldpGlobalScalarsConfSetMessage = dynamic_cast<LldpGlobalScalarsConfSetMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalScalarsConfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		UI32 cmdcode = pLldpGlobalScalarsConfSetMessage->getCmdcode();
		
        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        string sysdescname;

        if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
				LLDP_PLUG_DBG(string("numberOfResults = 0"));
		        createFlag = 1;
		        status = WAVE_MESSAGE_SUCCESS;
	        } else if (1 == numberOfResults) {
		        m = dynamic_cast<LldpGlobalScalarsConfManagedObject*>((*pResults)[0]);
				LLDP_PLUG_DBG(string("numberOfResults = 1"));
		        createFlag = 0;
		        status = WAVE_MESSAGE_SUCCESS;
            } else {
				LLDP_PLUG_DBG(string("ERROR:numberOfResults = ") + numberOfResults);
                status = WAVE_MESSAGE_ERROR;
            }
        }

	    if (createFlag == 0) {
			LLDP_PLUG_DBG(string("update"));
            updateWaveManagedObject (m);
            prismAssert (NULL != m, __FILE__, __LINE__);
        } else {
			LLDP_PLUG_DBG(string("create"));
            m = new LldpGlobalScalarsConfManagedObject(dynamic_cast<LldpGlobalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != m, __FILE__, __LINE__);
        }
        
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_LLDP)) {
            m->setProtocolFlag (LLDP_DEFAULT_PROTOCOL_FLAG);
		}
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_NO_LLDP)) {
            m->setProtocolFlag (LLDP_DEFAULT_PROTOCOL_FLAG);
			m->setDescription(LLDP_DEFAULT_DESCRIPTION);	
			m->setHello(LLDP_DEFAULT_HELLO);	
			m->setMode(LLDP_DEFAULT_MODE);	
			m->setMultiplier(LLDP_DEFAULT_MULTIPLIER);	
			if(vcsMode != VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE) {
			m->setDcbxFcoeAppTlv(LLDP_DEFAULT_DCBXFCOEAPPTLV);	
			m->setDcbxFcoeLogicalLinkTlv(LLDP_DEFAULT_DCBXFCOELOGICALLINKTLV);
           		}
			else {
				m->setDcbxFcoeAppTlv(!LLDP_DEFAULT_DCBXFCOEAPPTLV);	
				m->setDcbxFcoeLogicalLinkTlv(!LLDP_DEFAULT_DCBXFCOELOGICALLINKTLV);
			}
            m->setDcbxIscsiAppTlv(LLDP_DEFAULT_DCBXISCSIAPPTLV);
            m->setIscsiPriority(DEFAULT_ISCSI_PRIORITY);
			m->setDcbxTlv(LLDP_DEFAULT_DCBXTLV);	
			m->setDot1xTlv(LLDP_DEFAULT_DOT1XTLV);	
			m->setDot3xTlv(LLDP_DEFAULT_DOT3XTLV);	
			m->setMgmtAddrTlv(LLDP_DEFAULT_MGMTADDRTLV);	
			m->setPortDescrTlv(LLDP_DEFAULT_PORTDESCRTLV);	
			m->setSysCapTlv(LLDP_DEFAULT_SYSCAPTLV);	
			m->setSysDescrTlv(LLDP_DEFAULT_SYSDESCRTLV);	
			m->setSysNameTlv(LLDP_DEFAULT_SYSNAMETLV);	
			m->setSysName(LLDP_DEFAULT_SYSTEM_NAME);	
            LldpUtils::GetSystemDescriptionName(sysdescname);
            m->setSysDescr(sysdescname);
			m->setDisable(LLDP_DEFAULT_DISABLE);

			LldpGlobalDeleteAllProfiles();
		}
		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DESC))
            m->setDescription (pLldpGlobalScalarsConfSetMessage->getDescription());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_HELLO))
            m->setHello (pLldpGlobalScalarsConfSetMessage->getHello());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MODE))
            m->setMode ((LldpMode)(pLldpGlobalScalarsConfSetMessage->getMode()));
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MULTIPLIER))
            m->setMultiplier (pLldpGlobalScalarsConfSetMessage->getMultiplier());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV))
            m->setDcbxFcoeAppTlv (pLldpGlobalScalarsConfSetMessage->getDcbxFcoeAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV))
            m->setDcbxFcoeLogicalLinkTlv (pLldpGlobalScalarsConfSetMessage->getDcbxFcoeLogicalLinkTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV))
            m->setDcbxIscsiAppTlv (pLldpGlobalScalarsConfSetMessage->getDcbxIscsiAppTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_ISCSIPRIORITY))
            m->setIscsiPriority (pLldpGlobalScalarsConfSetMessage->getIscsiPriority());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DCBXTLV))
            m->setDcbxTlv (pLldpGlobalScalarsConfSetMessage->getDcbxTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT1XTLV))
            m->setDot1xTlv (pLldpGlobalScalarsConfSetMessage->getDot1xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DOT3XTLV))
            m->setDot3xTlv (pLldpGlobalScalarsConfSetMessage->getDot3xTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_MGMTADDRTLV))
            m->setMgmtAddrTlv (pLldpGlobalScalarsConfSetMessage->getMgmtAddrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_PORTDESCRTLV))
            m->setPortDescrTlv (pLldpGlobalScalarsConfSetMessage->getPortDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSCAPTLV))
            m->setSysCapTlv (pLldpGlobalScalarsConfSetMessage->getSysCapTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCRTLV))
            m->setSysDescrTlv (pLldpGlobalScalarsConfSetMessage->getSysDescrTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAMETLV))
            m->setSysNameTlv (pLldpGlobalScalarsConfSetMessage->getSysNameTlv());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSNAME))
            m->setSysName (pLldpGlobalScalarsConfSetMessage->getSysName());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_SYSDESCR))
            m->setSysDescr (pLldpGlobalScalarsConfSetMessage->getSysDescr());
        if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_CONF_DISABLE))
            m->setDisable (pLldpGlobalScalarsConfSetMessage->getDisable());

		if (IS_CMDCODE_SET(cmdcode, LLDP_GLOBAL_AND_PROFILE_COMMON_CONF))
			updateNonUserProfileConfigs (pPrismLinearSequencerContext);
        
        if ( (createFlag) && (m)) {
            //Sanjeev: mark the MO for GC
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
                    m);
        }

        if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : sagar : Do we need to handle this case?
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        
        return;
  }
    void  LldpGlobalConfWorker::LldpGlobalScalarsConfSetMessageHandler( LldpGlobalScalarsConfSetMessage *pLldpGlobalScalarsConfSetMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			// Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::updateLldpGlobalScalarsConf),
			reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerStartTransactionStep),
			// Your configuration change code goes here
			reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::updateLldpGlobalScalarsMO),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pLldpGlobalScalarsConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }
    
	void LldpGlobalConfWorker::LldpGlobalProfileGetObjectIdMessageHandler(LldpGlobalProfileGetObjectIdMessage *pLldpGlobalProfileGetObjectIdMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::lldpGlobalProfileValidateCreate),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::lldpGlobalProfileGetObjectId),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&LldpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pLldpGlobalProfileGetObjectIdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }
	
	void
	LldpGlobalConfWorker::lldpGlobalProfileValidateCreateSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		LLDP_PLUG_DBG_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			LLDP_PLUG_DBG(string("Phase 1 message succeeded on all locations"));
		} else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on all locations"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes"));
		} else {
			LLDP_PLUG_DBG(string("Unknown Error:- ") + sendToClusterCompletionStatus);
		}
		
        delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		LLDP_PLUG_DBG(string("Done"));
        return;
     }
	
	void
	LldpGlobalConfWorker::lldpGlobalProfileValidateCreate (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
		LldpGlobalProfileGetObjectIdMessage *pLldpGlobalProfileGetObjectIdMessage = 
				dynamic_cast<LldpGlobalProfileGetObjectIdMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		const string profileName = pLldpGlobalProfileGetObjectIdMessage->getProfileName();
        UI32 numberOfResults  = 0;
        ResourceId result = WAVE_MESSAGE_SUCCESS;
		
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        result = querySynchronouslyForCount(&syncQueryCtxt, numberOfResults);
        if (WAVE_MESSAGE_SUCCESS == result) {
			if (0 == numberOfResults) {
				LLDP_PLUG_DBG (string("profile doesn't exit"));
				
				LldpProfileConfSetMessage *m = new LldpProfileConfSetMessage();
				WaveSendToClusterContext    *pWaveSendToClusterContext;
				
				/* create clustercontext and initialize with callback func */
				pWaveSendToClusterContext = new WaveSendToClusterContext(
						this, reinterpret_cast<PrismAsynchronousCallback>(
						&LldpGlobalConfWorker::lldpGlobalProfileValidateCreateSendToClusterCallback), 
						pPrismLinearSequencerContext);
				
				m->setMsgType (LLDP_CREATE_CONF);
				m->setCmdcode (LLDP_GLOBAL_CONF_PROFILE);
				m->setProfileName (profileName);
				
				/* Set Phase 1 message */
				pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
				
				/* Enable partial success flag */
				pWaveSendToClusterContext->setPartialSuccessFlag(true);
				
				/* Enable flag to treat msg to failing over node as success */
				pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
				
				/* Send the message to all nodes in cluster */
				sendToWaveCluster (pWaveSendToClusterContext);
				
			} else {
				LLDP_PLUG_DBG (string("profile exists"));
			
				pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			}
        } else {
            LLDP_PLUG_DBG (string("Query failed while retrieving count of global profiles"));
            pPrismLinearSequencerContext->executeNextStep(result);
        }

		return;
    }
    
	void LldpGlobalConfWorker::lldpGlobalProfileGetObjectId(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		LLDP_PLUG_DBG_TRACE();
		LldpGlobalProfileGetObjectIdMessage *pLldpGlobalProfileGetObjectIdMessage = 
				dynamic_cast<LldpGlobalProfileGetObjectIdMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		const string profileName = pLldpGlobalProfileGetObjectIdMessage->getProfileName();
		LldpGlobalProfileConfManagedObject *profileMO = NULL;
		int status = WAVE_MESSAGE_ERROR;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				LLDP_PLUG_DBG (string("Can't find the profile object"));
				
				/* create the profile in the MO */
				startTransaction();
				profileMO = new LldpGlobalProfileConfManagedObject(
								dynamic_cast<LldpGlobalObjectManager*>(getPWaveObjectManager()));
				if (profileMO) {
					profileMO->setProfileName(profileName);
					copyMasterMoToProfileMo(pPrismLinearSequencerContext, profileMO);
        			
					status = commitTransaction ();
        			if (status != FRAMEWORK_SUCCESS) {
        				LLDP_PLUG_DBG_ERR ("Profile MO commit failed");
       					status = WAVE_MESSAGE_ERROR;
        			} else {
						LLDP_PLUG_DBG_S ("Profile Mo created");
						pLldpGlobalProfileGetObjectIdMessage->setProfileObjId(profileMO->getObjectId ());
       					status = WAVE_MESSAGE_SUCCESS;
					}
					delete profileMO;
				} else {
					rollbackTransaction();
				}
			} else if (1 == numberOfResults) {
				LLDP_PLUG_DBG ("Profile exists");
				profileMO = dynamic_cast<LldpGlobalProfileConfManagedObject *>((*pResults)[0]);
				pLldpGlobalProfileGetObjectIdMessage->setProfileObjId(profileMO->getObjectId ());
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				LLDP_PLUG_DBG_ERR ("Two or more object Exists");
				status = WAVE_MESSAGE_ERROR;
			}
            //Sanjeev: Cleanup the query results.
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		} else {
			status = WAVE_MESSAGE_ERROR;
			LLDP_PLUG_DBG_ERR ("Message creation failed");
		}

		pLldpGlobalProfileGetObjectIdMessage->setCompletionStatus (status);
		pPrismLinearSequencerContext->executeNextStep(status);
	}
    
	void LldpGlobalConfWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
		LLDP_PLUG_DBG_TRACE();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string sysdescname;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

		LldpGlobalScalarsConfManagedObject *m;

        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        LLDP_PLUG_DBG(string("VCSMODE =") +vcsMode);

        startTransaction();

        m = new LldpGlobalScalarsConfManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        m->setProtocolFlag (LLDP_DEFAULT_PROTOCOL_FLAG);
		m->setDescription(LLDP_DEFAULT_DESCRIPTION);	
		m->setHello(LLDP_DEFAULT_HELLO);	
		m->setMode(LLDP_DEFAULT_MODE);	
		m->setMultiplier(LLDP_DEFAULT_MULTIPLIER);	

		if(vcsMode != VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE) {
			m->setDcbxFcoeAppTlv(LLDP_DEFAULT_DCBXFCOEAPPTLV);	
			m->setDcbxFcoeLogicalLinkTlv(LLDP_DEFAULT_DCBXFCOELOGICALLINKTLV);	
		}
		else {
			m->setDcbxFcoeAppTlv(!LLDP_DEFAULT_DCBXFCOEAPPTLV);	
			m->setDcbxFcoeLogicalLinkTlv(!LLDP_DEFAULT_DCBXFCOELOGICALLINKTLV);	
		}
        m->setDcbxIscsiAppTlv(LLDP_DEFAULT_DCBXISCSIAPPTLV);
        m->setIscsiPriority(DEFAULT_ISCSI_PRIORITY);
        m->setDcbxTlv(LLDP_DEFAULT_DCBXTLV);	
		m->setDot1xTlv(LLDP_DEFAULT_DOT1XTLV);	
		m->setDot3xTlv(LLDP_DEFAULT_DOT3XTLV);	
		m->setMgmtAddrTlv(LLDP_DEFAULT_MGMTADDRTLV);	
		m->setPortDescrTlv(LLDP_DEFAULT_PORTDESCRTLV);	
		m->setSysCapTlv(LLDP_DEFAULT_SYSCAPTLV);	
		m->setSysDescrTlv(LLDP_DEFAULT_SYSDESCRTLV);	
		m->setSysNameTlv(LLDP_DEFAULT_SYSNAMETLV);	
		m->setSysName(LLDP_DEFAULT_SYSTEM_NAME);	
        LldpUtils::GetSystemDescriptionName(sysdescname);
		m->setSysDescr(sysdescname);
		m->setDisable(LLDP_DEFAULT_DISABLE);	

        status = commitTransaction ();
        if (status == FRAMEWORK_SUCCESS) {
            status = WAVE_MESSAGE_SUCCESS;
        } else {
        }

		delete m;
    
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

}
