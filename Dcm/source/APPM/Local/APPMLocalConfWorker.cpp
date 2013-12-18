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
 *   Author : kverma, asharma, bgangise                                    *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalConfWorker.h"
#include "APPM/Local/APPMLocalAPPMProfileMessage.h"
#include "APPM/Local/APPMLocalProfileDomainMessage.h"
#include "APPM/Local/APPMLocalAPPMProfileBulkMessage.h"
#include "APPM/Local/APPMLocalAPPMVlanMessage.h"
#include "APPM/Local/APPMLocalAPPMQosMessage.h"
#include "APPM/Local/APPMLocalAPPMFcoeMessage.h"
#include "APPM/Local/APPMLocalAPPMSecurityMessage.h"
#include "APPM/Local/APPMLocalTypes.h"
#include "ClientInterface/APPM/APPMProfileMessage.h"
#include "ClientInterface/APPM/APPMPortProfileDomainConfigMessage.h"
#include "ClientInterface/APPM/APPMProfileBulkMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMQosProfileMessage.h"
#include "ClientInterface/APPM/APPMSecProfileMessage.h"
#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/APPM/APPMVlanProfileMessage.h"
#include "DcmCStatus.h"
#include "ssm_dcmd.h"
#include "nsm_dcm.h"
#include "APPM/Global/SubProfileManagedObject.h"
namespace DcmNs
{

    APPMLocalConfWorker::APPMLocalConfWorker ( APPMLocalObjectManager *pAPPMLocalObjectManager)
        : WaveWorker  (pAPPMLocalObjectManager)
    {
        addOperationMap (APPMLOCALAPPMPROFILE, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMProfileMessageHandler));
        addOperationMap (APPMLOCALPORTPROFILEDOMAIN, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalProfileDomainMessageHandler));
        addOperationMap (APPMLOCALAPPMVLAN, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMVlanMessageHandler));
        addOperationMap (APPMLOCALAPPMQOS, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMQosMessageHandler));
        addOperationMap (APPMLOCALAPPMFCOE, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMFcoeMessageHandler));
        addOperationMap (APPMLOCALAPPMSECURITY, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMSecurityMessageHandler));
		addOperationMap (APPMLOCALAPPMPROFILEBULK, reinterpret_cast<PrismMessageHandler> (&APPMLocalConfWorker::APPMLocalAPPMProfileBulkMessageHandler));
    }

    APPMLocalConfWorker::~APPMLocalConfWorker ()
    {
    }

    PrismMessage  *APPMLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case APPMLOCALAPPMPROFILE :
                pPrismMessage = new APPMLocalAPPMProfileMessage ();
                break;
            case APPMLOCALPORTPROFILEDOMAIN :
                pPrismMessage = new APPMLocalProfileDomainMessage();
                break;
            case APPMLOCALAPPMVLAN :
                pPrismMessage = new APPMLocalAPPMVlanMessage ();
                break;
            case APPMLOCALAPPMQOS :
                pPrismMessage = new APPMLocalAPPMQosMessage ();
                break;
            case APPMLOCALAPPMFCOE :
                pPrismMessage = new APPMLocalAPPMFcoeMessage ();
                break;
            case APPMLOCALAPPMSECURITY :
                pPrismMessage = new APPMLocalAPPMSecurityMessage ();
                break;
            case APPMLOCALAPPMPROFILEBULK :
                pPrismMessage = new APPMLocalAPPMProfileBulkMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

	ResourceId APPMLocalConfWorker::profileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMLocalAPPMProfileMessage *pAPPMLocalAPPMProfileMessage =
			dynamic_cast<APPMLocalAPPMProfileMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		APPMProfileMessage *mNsm = new APPMProfileMessage
			(pAPPMLocalAPPMProfileMessage->getOpCode(),pAPPMLocalAPPMProfileMessage->getCmdCode(),
			 pAPPMLocalAPPMProfileMessage->getProfileName(),pAPPMLocalAPPMProfileMessage->getProfileId(),
			 pAPPMLocalAPPMProfileMessage->getIsActivated(), pAPPMLocalAPPMProfileMessage->getProfileMac(),pAPPMLocalAPPMProfileMessage->getIsAllow(),
			 pAPPMLocalAPPMProfileMessage->getIsFloodingRestricted());

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
		
        status = sendSynchronouslyToWaveClient ("nsm",mNsm);
		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " port-profile message sent to NSM backend");

        status = AppmNsmWyserEaReturn(pAPPMLocalAPPMProfileMessage, mNsm, status);
       
		delete mNsm;

		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
		return (status);
	}

    ResourceId APPMLocalConfWorker::portProfileDomainSendToDaemonStep(PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        APPMLocalProfileDomainMessage *pAPPMLocalProfileDomainMessage =
            dynamic_cast<APPMLocalProfileDomainMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

        APPMPortProfileDomainConfigMessage *mNsm = new APPMPortProfileDomainConfigMessage;

        mNsm->setOpCode(pAPPMLocalProfileDomainMessage->getOpCode());
        mNsm->setCmdCode(pAPPMLocalProfileDomainMessage->getCmdCode());
        mNsm->setPortProfileName(pAPPMLocalProfileDomainMessage->getPortProfileName());
        mNsm->setPortProfileDomainName(pAPPMLocalProfileDomainMessage->getPortProfileDomainName());

        tracePrintf(TRACE_LEVEL_INFO, "Sending APPMLocalProfileDomainMessage from local plugin to nsmd"
                "Opcode<%d> CmdCode<%d> port-profile-domain-name<%s> port-profile-name<%s>.",
                mNsm->getOpCode(), mNsm->getCmdCode(),
                (mNsm->getPortProfileDomainName()).c_str(),
                (mNsm->getPortProfileName()).c_str());

        status = sendSynchronouslyToWaveClient ("nsm",mNsm);

        status = AppmNsmWyserEaReturn(pAPPMLocalProfileDomainMessage, mNsm, status);

        delete mNsm;

        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
        return (status);
    }

    void  APPMLocalConfWorker::APPMLocalProfileDomainMessageHandler
        ( APPMLocalProfileDomainMessage *pAPPMLocalProfileDomainMessage) {

        PrismSynchronousLinearSequencerStep sequencerSteps[] = {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::portProfileDomainSendToDaemonStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext 
            = new PrismSynchronousLinearSequencerContext (pAPPMLocalProfileDomainMessage, this, 
                sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    ResourceId APPMLocalConfWorker::profileBulkMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMProfileBulkMessage *pAPPMLocalAPPMProfileBulkMessage =
            dynamic_cast<APPMLocalAPPMProfileBulkMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

        	APPMProfileBulkMessage *mNsm = new
			  APPMProfileBulkMessage(pAPPMLocalAPPMProfileBulkMessage->getOpCode(),pAPPMLocalAPPMProfileBulkMessage->getCmdCode(),
				  pAPPMLocalAPPMProfileBulkMessage->getProfileName(),pAPPMLocalAPPMProfileBulkMessage->getProfileId(),
				  pAPPMLocalAPPMProfileBulkMessage->getIsActivated(),pAPPMLocalAPPMProfileBulkMessage->getIsAllow(),pAPPMLocalAPPMProfileBulkMessage->getMacCount(),pAPPMLocalAPPMProfileBulkMessage->getProfileMacs());

        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);

        status = sendSynchronouslyToWaveClient ("nsm",mNsm);
        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " port-profile Bulk message sent to NSM backend");

		pAPPMLocalAPPMProfileBulkMessage->copyAllBuffers(*mNsm);
        status = AppmNsmWyserEaReturn(pAPPMLocalAPPMProfileBulkMessage, mNsm, status);
		delete mNsm;
        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
        return (status);
    }

	ResourceId APPMLocalConfWorker::secProfileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMLocalAPPMSecurityMessage *pAPPMLocalAPPMSecurityMessage =
			dynamic_cast<APPMLocalAPPMSecurityMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		APPMSecProfileMessage *pMsg = new APPMSecProfileMessage
			(pAPPMLocalAPPMSecurityMessage->getOpCode(),pAPPMLocalAPPMSecurityMessage->getCmdCode(),
			 pAPPMLocalAPPMSecurityMessage->getProfileName(),pAPPMLocalAPPMSecurityMessage->getProfileId(),
			 pAPPMLocalAPPMSecurityMessage->getAccessGroup());
		
		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " security profile Local message recived");

		if (pMsg) {
	        status = sendSynchronouslyToWaveClient("ssm",pMsg);

    	    status = AppmSsmWyserEaReturn(pAPPMLocalAPPMSecurityMessage, pMsg, status);

			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " security profile message sent to SSM backend");
			delete pMsg;
		}
		return (status);
	}

    WaveManagedObject  *APPMLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	ResourceId APPMLocalConfWorker::fcoeProfileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status1 = WAVE_MESSAGE_SUCCESS;
        
		APPMLocalAPPMFcoeMessage *pAPPMLocalAPPMFcoeMessage =
			dynamic_cast<APPMLocalAPPMFcoeMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		APPMFcoeProfileMessage *mNsm = new APPMFcoeProfileMessage
			(pAPPMLocalAPPMFcoeMessage->getOpCode(),pAPPMLocalAPPMFcoeMessage->getCmdCode(),
			 pAPPMLocalAPPMFcoeMessage->getProfileName(),pAPPMLocalAPPMFcoeMessage->getProfileId(),
			 pAPPMLocalAPPMFcoeMessage->getFcoePort());
        APPMFcoeProfileMessage *mSsm = NULL;

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " fcoe-profile Local message recived");
		if (!mNsm) {
			return WAVE_MESSAGE_ERROR;
		}
		
        status1 = sendSynchronouslyToWaveClient("nsm",mNsm);
        status1 = AppmNsmWyserEaReturn(pAPPMLocalAPPMFcoeMessage, mNsm, status1);
        if ((status1 == WAVE_MESSAGE_SUCCESS))
        {
		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " fcoe-profile message sent to NSM backend");
        
        /*
         * Send the message to ssm as well as it should be 
         * aware of fcoe profile config to enable/disable
         * qos-profile's non cee parameters.
         */
        mSsm = new APPMFcoeProfileMessage (pAPPMLocalAPPMFcoeMessage->getOpCode(),
            pAPPMLocalAPPMFcoeMessage->getCmdCode(), pAPPMLocalAPPMFcoeMessage->getProfileName(),
            pAPPMLocalAPPMFcoeMessage->getProfileId(), pAPPMLocalAPPMFcoeMessage->getFcoePort());
		if (!mSsm)
		{
			delete mNsm;
			return WAVE_MESSAGE_ERROR;
		}
        if (pAPPMLocalAPPMFcoeMessage->getProfileName() !=  "default"){
            status1 = sendSynchronouslyToWaveClient("qos",mSsm);
            status1 = AppmSsmWyserEaReturn(pAPPMLocalAPPMFcoeMessage, mSsm, status1);
            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " fcoe-profile message sent to SSM backend"); 
        }
        } else {
            delete (mNsm);
            return status1;
        }
        delete (mNsm);
        delete (mSsm);
        return (status1);
	}

    void  APPMLocalConfWorker::APPMLocalAPPMFcoeMessageHandler( APPMLocalAPPMFcoeMessage *pAPPMLocalAPPMFcoeMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::fcoeProfileMessageStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMFcoeMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId APPMLocalConfWorker::QosProfileMessageStep (PrismSynchronousLinearSequencerContext  *pPrismSynchronousLinearSequencerContext)
    {
    
        ResourceId status;
        APPMLocalAPPMQosMessage *pAPPMLocalAPPMQosMessage =
            dynamic_cast<APPMLocalAPPMQosMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

        APPMQosProfileMessage *m = new APPMQosProfileMessage();
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
		if (!m) {
			return WAVE_MESSAGE_ERROR;
		}
		
        m->setOpCode(pAPPMLocalAPPMQosMessage->getOpCode());
        m->setCmdCode(pAPPMLocalAPPMQosMessage->getCmdCode());
        m->setQosProfileName(pAPPMLocalAPPMQosMessage->getProfileName());
        m->setProfileId(pAPPMLocalAPPMQosMessage->getProfileId());
        switch(pAPPMLocalAPPMQosMessage->getCmdCode())
        {
        case QOS_PROFILE_CEE_MAP:
            m->setQosMapName(pAPPMLocalAPPMQosMessage->getCeeMapName());
            break;
        case QOS_COS_TO_COS_MUTATION:
            m->setQosMapName(pAPPMLocalAPPMQosMessage->getCosToCosMutationMap());
            break;
        case QOS_COS_TO_TRAFFIC_MAP:
            m->setQosMapName(pAPPMLocalAPPMQosMessage->getCosToTrafficClassMap());
            break;
        case QOS_NO_DEFAULT_COS:
        case QOS_DEFAULT_COS:
            m->setQosCos(pAPPMLocalAPPMQosMessage->getDefaultCosValue());
            break;
        case QOS_FLOW_CTRL_PFC:
        case QOS_NO_FLOW_CTRL_PFC:
            m->setQosCos(pAPPMLocalAPPMQosMessage->getPfcValue());
            m->setQosTx(pAPPMLocalAPPMQosMessage->getTxValue());
            m->setQosRx(pAPPMLocalAPPMQosMessage->getRxValue());
            break;

        case QOS_FLOW_CTRL_ALL:
        case QOS_NO_FLOW_CTRL_ALL:
            m->setQosTx(pAPPMLocalAPPMQosMessage->getFlowControlTxEnable());
            m->setQosRx(pAPPMLocalAPPMQosMessage->getFlowControlRxEnable());

        }

        status = sendSynchronouslyToWaveClient("qos",m);

        status = AppmSsmWyserEaReturn(pAPPMLocalAPPMQosMessage, m, status);
		delete m;
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
        return status;
    }

    void  APPMLocalConfWorker::APPMLocalAPPMProfileMessageHandler( APPMLocalAPPMProfileMessage *pAPPMLocalAPPMProfileMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::profileMessageStep),
// Your configuration change code goes here

            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMProfileMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  APPMLocalConfWorker::APPMLocalAPPMProfileBulkMessageHandler( APPMLocalAPPMProfileBulkMessage *pAPPMLocalAPPMProfileBulkMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::profileBulkMessageStep),
// Your configuration change code goes here

            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMProfileBulkMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  APPMLocalConfWorker::APPMLocalAPPMQosMessageHandler( APPMLocalAPPMQosMessage *pAPPMLocalAPPMQosMessage)
    {

// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
    //        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

      //      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::QosProfileMessageStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMQosMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  APPMLocalConfWorker::APPMLocalAPPMSecurityMessageHandler( APPMLocalAPPMSecurityMessage *pAPPMLocalAPPMSecurityMessage)
    {
// Validations and Queries should go here

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::secProfileMessageStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMSecurityMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
    }

    ResourceId APPMLocalConfWorker::vlanMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        APPMLocalAPPMVlanMessage *pAPPMLocalAPPMVlanMessage =
            dynamic_cast<APPMLocalAPPMVlanMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

        

        if(pAPPMLocalAPPMVlanMessage->getOpCode() != APPM_VCENTER)
        {

        APPMVlanProfileMessage *m = new APPMVlanProfileMessage
            (pAPPMLocalAPPMVlanMessage->getOpCode(),pAPPMLocalAPPMVlanMessage->getCmdCode(),
            pAPPMLocalAPPMVlanMessage->getProfileName(),pAPPMLocalAPPMVlanMessage->getProfileId(),
            pAPPMLocalAPPMVlanMessage->getSwModeFlag(),pAPPMLocalAPPMVlanMessage->getSwMode(),
            pAPPMLocalAPPMVlanMessage->getIsAllowedVlanAll(),pAPPMLocalAPPMVlanMessage->getIsAllowedVlanNone(),
			pAPPMLocalAPPMVlanMessage->getAccessVlan(),pAPPMLocalAPPMVlanMessage->getVlanRange(),
            pAPPMLocalAPPMVlanMessage->getVlanClassifierGroupId(),pAPPMLocalAPPMVlanMessage->getVlanClassifierVlan());

		if (m) {
			m->setNativeVlanId(pAPPMLocalAPPMVlanMessage->getNativeVlanId());
            m->setTrunkVlanId(pAPPMLocalAPPMVlanMessage->getTrunkVlanId());
            m->setVlanClassificationType(pAPPMLocalAPPMVlanMessage->getVlanClassificationType());
            m->setCtag(pAPPMLocalAPPMVlanMessage->getCtag());
            m->setMac(pAPPMLocalAPPMVlanMessage->getMac());
            m->setMacGroup(pAPPMLocalAPPMVlanMessage->getMacGroup());

	        status = sendSynchronouslyToWaveClient("nsm",m);

	        pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());

	        status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
			delete m;
		}
        return status;
        }
        else //VCENTER REJOIN ONLY CASE
        {
        APPMVlanProfileMessage *m = new APPMVlanProfileMessage;
        string profileName = pAPPMLocalAPPMVlanMessage->getProfileName();
        ProfileManagedObject *pProfileManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        syncQueryCtxt.addSelectField("profileId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

                if (NULL != pResults) {
                    UI32 numberOfResults = pResults->size ();
                    if (0 == numberOfResults) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM: Could not"
                            " find the profile Managed object with key: " + profileName);
                        goto last;
                    } else if (1 == numberOfResults) {
                        pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM: Found the"
                            " Managed Object with key: " + profileName);
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM:"
                            " Two or more objects exists with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM:"
                        " pResults ptr in NULL, Quering DB is failed");
                    goto last;
                }

                if (pProfileManagedObject == NULL) {
                  APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "pProfileManagedObject is NULL");
                    goto last;
                }
        pAPPMLocalAPPMVlanMessage->setProfileId(pProfileManagedObject->getProfileId());

        switch (pAPPMLocalAPPMVlanMessage->getCmdCode())
            {
              case VCENTER_ACCESS_VLAN_PROFILE_CREATE:
                {
                    trace (TRACE_LEVEL_INFO, "Command CODE: VCENTER_ACCESS_VLAN_PROFILE_CREATE....." );
                            m->setOpCode(APPM_CREATE);
                            m->setCmdCode(VLAN_PROFILE_CREATE);
                            m->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            if(m)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
                                delete m;
                            }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m2 = new APPMVlanProfileMessage;
                            m2->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m2->setOpCode(APPM_SET_ELEM);
                            m2->setCmdCode(VLAN_SW_MODE_FLAG);
                            m2->setSwModeFlag(true);
                            m2->setSwMode(VLAN_SW_MODE_ACCESS);
                            if(m2)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m2);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m2->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m2, status);
                                delete m2;
                            }                            
                         }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m3 = new APPMVlanProfileMessage;
                            m3->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m3->setOpCode(APPM_SET_ELEM);
                            m3->setCmdCode(VLAN_ACCESS_VLAN);
                            m3->setAccessVlan(pAPPMLocalAPPMVlanMessage->getAccessVlan());
                            if(m3)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m3);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m3->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m3, status);
                                delete m3;
                            }
                         }
                }
                break;
                case VCENTER_TRUNK_VLAN_PROFILE_CREATE:
                {
                    trace (TRACE_LEVEL_INFO, "Command CODE: VCENTER_TRUNK_VLAN_PROFILE_CREATE....." );
                            m->setOpCode(APPM_CREATE);
                            m->setCmdCode(VLAN_PROFILE_CREATE);
                            m->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            if(m)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
                                delete m;
                            }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m2 = new APPMVlanProfileMessage;
                            m2->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m2->setOpCode(APPM_SET_ELEM);
                            m2->setCmdCode(VLAN_SW_MODE_FLAG);
                            m2->setSwModeFlag(true);
                            m2->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m2)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m2);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m2->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m2, status);
                                delete m2;
                            }
                         }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m3 = new APPMVlanProfileMessage;
                            m3->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m3->setOpCode(APPM_SET_ELEM);
                            m3->setCmdCode(VLAN_SW_MODE);
                            m3->setSwModeFlag(true);
                            m3->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m3)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m3);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m3->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m3, status);
                                delete m3;
                            }
                         }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m4 = new APPMVlanProfileMessage;
                            m4->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m4->setOpCode(APPM_SET_ELEM);
                            m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_ADD);
                            m4->setVlanRange(pAPPMLocalAPPMVlanMessage->getVlanRange());

                            if(m4)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m4);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m4->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m4, status);
                                delete m4;
                            }
                         }
                }
                break;
                case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL:
                {
                    trace (TRACE_LEVEL_INFO, "Command CODE: VCENTER_TRUNK_VLAN_PROFILE_CREATE....." );
                            m->setOpCode(APPM_CREATE);
                            m->setCmdCode(VLAN_PROFILE_CREATE);
                            m->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            if(m)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
                                delete m;
                            }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m2 = new APPMVlanProfileMessage;
                            m2->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m2->setOpCode(APPM_SET_ELEM);
                            m2->setCmdCode(VLAN_SW_MODE_FLAG);
                            m2->setSwModeFlag(true);
                            m2->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m2)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m2);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m2->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m2, status);
                                delete m2;
                            }
                         }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m3 = new APPMVlanProfileMessage;
                            m3->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m3->setOpCode(APPM_SET_ELEM);
                            m3->setCmdCode(VLAN_SW_MODE);
                            m3->setSwModeFlag(true);
                            m3->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m3)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m3);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m3->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m3, status);
                                delete m3;
                            }
                         }
                      if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m4 = new APPMVlanProfileMessage;
                            m4->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m4->setOpCode(APPM_SET_ELEM);
                            m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_ALL);
                            m4->setIsAllowedVlanAll(pAPPMLocalAPPMVlanMessage->getIsAllowedVlanAll());
                            if(m4)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m4);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m4->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m4, status);
                                delete m4;
                            }
                         }
                }
                break;
                case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE:
                {
                    trace (TRACE_LEVEL_INFO, "Command CODE: VCENTER_TRUNK_VLAN_PROFILE_CREATE....." );
                            m->setOpCode(APPM_CREATE);
                            m->setCmdCode(VLAN_PROFILE_CREATE);
                            m->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            if(m)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
                                delete m;
                            }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m2 = new APPMVlanProfileMessage;
                            m2->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m2->setOpCode(APPM_SET_ELEM);
                            m2->setCmdCode(VLAN_SW_MODE_FLAG);
                            m2->setSwModeFlag(true);
                            m2->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m2)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m2);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m2->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m2, status);
                                delete m2;
                            }
                         }
                       if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m3 = new APPMVlanProfileMessage;
                            m3->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m3->setOpCode(APPM_SET_ELEM);
                            m3->setCmdCode(VLAN_SW_MODE);
                            m3->setSwModeFlag(true);
                            m3->setSwMode(VLAN_SW_MODE_TRUNK);
                            if(m3)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m3);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m3->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m3, status);
                                delete m3;
                            }
                         }
                      if (status != WAVE_MESSAGE_SUCCESS)
                        {
                            APPMVlanProfileMessage *m4 = new APPMVlanProfileMessage;
                            m4->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                            m4->setOpCode(APPM_SET_ELEM);
                            m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_NONE);
                            m4->setIsAllowedVlanNone(pAPPMLocalAPPMVlanMessage->getIsAllowedVlanNone());
                            if(m4)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m4);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m4->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m4, status);
                                delete m4;
                            }
                         }
                }
                break;
                case VCENTER_VLAN_PROFILE_DELETE:
                {
                    trace (TRACE_LEVEL_INFO, "OPCODE: VCENTER_VLAN_PROFILE_DELETE....." );
                    m->setProfileName(pAPPMLocalAPPMVlanMessage->getProfileName());
                    m->setOpCode(APPM_SET_ELEM);
                    m->setCmdCode(VLAN_PROFILE_DEL);
                          if(m)
                            {
                                status = sendSynchronouslyToWaveClient("nsm",m);
                                pAPPMLocalAPPMVlanMessage->setCompletionStatus(m->getCompletionStatus());
                                status = AppmNsmWyserEaReturn(pAPPMLocalAPPMVlanMessage, m, status);
                                delete m;
                            }

                }

            }
        last:
        if (pResults != NULL) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete (pResults);
        }

        }
        return status;
    }

    void  APPMLocalConfWorker::APPMLocalAPPMVlanMessageHandler( APPMLocalAPPMVlanMessage *pAPPMLocalAPPMVlanMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::vlanMessageStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalAPPMVlanMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId APPMLocalConfWorker::AppmNsmWyserEaReturn(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
        pMsg->setCompletionStatus(status);
        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, string("clientMsg->getClientStatus()") + clientMsg->getClientStatus());
                status = mapNsmErrorToDcmdError(clientMsg->getClientStatus());
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
    }

    ResourceId APPMLocalConfWorker::AppmSsmWyserEaReturn(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
        pMsg->setCompletionStatus(status);
        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, string("clientMsg->getClientStatus()") + clientMsg->getClientStatus());
                status = mapSsmErrorToDcmdError(clientMsg->getClientStatus());
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
    
    }

    ResourceId APPMLocalConfWorker::mapNsmErrorToDcmdError(int rc)
    {

        ResourceId errorId = rc;

        switch(rc)
        {

		case NSM_DCM_SUCCESS:
			errorId = WAVE_MESSAGE_SUCCESS;
			break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_NUMBER:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_NUMBER;
            break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_CREATE_STATUS:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_CREATE_STATUS;
            break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_EXIST:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_EXIST;
            break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_STATE:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_STATE;
            break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_DELETE_STATUS:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_DELETE_STATUS;
            break;

        case NSM_ERR_DCM_APPM_VLAN_PROFILE_MODE_INVALID:
            errorId = WRC_NSM_ERR_DCM_APPM_VLAN_PROFILE_MODE_INVALID;
            break;

        case NSM_ERR_DCM_APPM_VLAN_PROFILE_ACCESS_MODE:
            errorId = WRC_NSM_ERR_DCM_APPM_VLAN_PROFILE_ACCESS_MODE;
            break;

        case NSM_ERR_DCM_APPM_VLAN_PROFILE_TRUNK_MODE:
            errorId = WRC_NSM_ERR_DCM_APPM_VLAN_PROFILE_TRUNK_MODE;
            break;

        case NSM_ERR_DCM_APPM_VLAN_PROFILE_INPUT:
            errorId = WRC_NSM_ERR_DCM_APPM_VLAN_PROFILE_INPUT;
            break;

        case NSM_ERR_DCM_APPM_NO_PROFILE_MAC:
            errorId = WRC_NSM_ERR_DCM_APPM_NO_PROFILED_MAC;
            break;

        case NSM_ERR_DCM_APPM_MAC_ASSOC:
            errorId = WRC_NSM_ERR_DCM_APPM_MAC_ASSOC;
            break;

        case NSM_ERR_DCM_APPM_MAC_NO_ASSOC:
            errorId = WRC_NSM_ERR_DCM_APPM_MAC_NO_ASSOC;
            break;

        case NSM_ERR_DCM_APPM_PROFILE_GROUP_OVERLAP:
            errorId = WRC_NSM_ERR_DCM_APPM_PROFILE_GROUP_OVERLAP;
            break;

        case NSM_ERR_DCM_APPM_PORT_NOT_PROFILED:
            errorId = WRC_NSM_ERR_DCM_APPM_PROFILED_PORT;
            break;

        case NSM_ERR_DCM_APPM_PORT_PROFILE_NOT_ACTIVE:
            errorId = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_NOT_ACTIVE;
            break;

        case NSM_ERR_DCM_MCAST_MAC:
			errorId = WRC_NSM_ERR_DCM_MCAST_MAC_NOT_ALLOWED;
           break; 

        case NSM_ERR_DCM_INVALID_MAC:
			errorId = WRC_NSM_ERR_DCM_INVALID_MAC;
			break;

        case NSM_ERR_DCM_FPMA_MAC:
			errorId = WRC_NSM_ERR_DCM_FPMA_MAC;
			break;
 

		case NSM_ERR_DCM_ACTIVE_PROFILES_ON_PORT:
			errorId = WRC_NSM_ERR_DCM_ACTIVE_PROFILES_ON_PORT;
			break;

		default:
            break;

        }

        return errorId;

     }

    ResourceId APPMLocalConfWorker::mapSsmErrorToDcmdError(int rc)
    {

        ResourceId errorId = rc;

        switch(rc)
        {

		case SSM_DCM_SUCCESS:
			errorId = WAVE_MESSAGE_SUCCESS;
			break;

        case SSM_DCM_ERR_APPM_PORT_PROFILE_NUMBER:
            errorId = WRC_SSM_DCM_ERR_APPM_PORT_PROFILE_NUMBER;
            break;

        case SSM_DCM_ERR_APPM_PORT_PROFILE_CREATE_STATUS:
            errorId = WRC_SSM_DCM_ERR_APPM_PORT_PROFILE_CREATE_STATUS;
            break;

        case SSM_DCM_ERR_APPM_PORT_PROFILE_EXIST:
            errorId = WRC_SSM_DCM_ERR_APPM_PORT_PROFILE_EXIST;
            break;

        case SSM_DCM_ERR_APPM_PORT_PROFILE_STATE:
            errorId = WRC_SSM_DCM_ERR_APPM_PORT_PROFILE_STATE;
            break;

        case SSM_DCM_ERR_APPM_PORT_PROFILE_DELETE_STATUS:
            errorId = WRC_SSM_DCM_ERR_APPM_PORT_PROFILE_DELETE_STATUS;
            break;

        case SSM_DCM_ERR_APPM_SEC_PROFILE_DUP_ACL:
            errorId = WRC_SSM_DCM_ERR_APPM_SEC_PROFILE_DUP_ACL;
            break;

        case SSM_DCM_ERR_APPM_SEC_PROFILE_PROFILE_STATE:
            errorId = WRC_SSM_DCM_ERR_APPM_SEC_PROFILE_PROFILE_STATE;
            break;

        case SSM_DCM_ERR_APPM_SEC_PROFILE_COMMIT_VALIDATION:
            errorId = WRC_SSM_DCM_ERR_APPM_SEC_PROFILE_COMMIT_VALIDATION;
            break;

        case SSM_DCM_ERR_APPM_SEC_PROFILE_APPLIED:
            errorId = WRC_SSM_DCM_ERR_APPM_SEC_PROFILE_APPLIED;
            break;

        case SSM_DCM_ERR_APPM_SEC_PROFILE_ACL_PRESENT:
            errorId = WRC_SSM_DCM_ERR_APPM_SEC_PROFILE_ACL_PRESENT;
            break;

        case SSM_DCM_ERR_APPM_PROFILED_PORT:
            errorId = WRC_SSM_DCM_ERR_APPM_PROFILED_PORT;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_CEE_MAP_EXIST:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_CEE_MAP_EXIST;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_COS_MUT_MAP_EXIST:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_COS_MUT_MAP_EXIST;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_COS_TC_MAP_EXIST:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_COS_TC_MAP_EXIST;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_MAP_NOT_APPLIED:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_MAP_NOT_APPLIED;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_QOS_MODE:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_QOS_MODE;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_PFC:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_PFC;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_PAUSE:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_PAUSE;
            break;

        case SSM_DCM_ERR_APPM_QOS_PROFILE_SET_MAP:
            errorId = WRC_SSM_DCM_ERR_APPM_QOS_PROFILE_SET_MAP;
            break;

        case SSM_DCM_ERR_ACL_EEXIST:
            errorId = WRC_SSM_DCM_ERR_ACL_EEXIST;
            break;

        case SSM_DCM_ERR_ACL_INVALID_NAME:
            errorId = WRC_SSM_DCM_ERR_ACL_INVALID_NAME;
            break;

        case SSM_DCM_QOS_CEE_FCOE:
            errorId = WRC_SSM_DCM_QOS_CEE_FCOE;
            break;

        case SSM_DCM_ERR_APPM_QOS_NON_DEFCFG:
            errorId = WRC_SSM_DCM_ERR_APPM_PROFILED_PORT_CONFIG;
            break;

        case SSM_DCM_ERR_APPM_FCOE_PORT:
            errorId = WRC_SSM_DCM_ERR_APPM_FCOE_PORT_CONFIG;
            break;

        case SSM_QOS_NO_PFC_TC_MAP_STILL_ACTIVATED:
            errorId = WRC_SSM_QOS_NO_PFC_TC_MAP_STILL_ACTIVATED;
            break;

		case SSM_INTF_COS7_NOT_ALLOWED_IN_VCS:
			errorId = WRC_SSM_INTF_COS7_NOT_ALLOWED_IN_VCS;
			break;

        default:
            break;

         }

         return errorId;
     }
}
