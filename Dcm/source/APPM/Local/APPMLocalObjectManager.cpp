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
 *   Author : kverma                                                       *
 **************************************************************************/

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalConfWorker.h"
#include "APPM/Local/APPMLocalShowWorker.h"
#include "APPM/Local/APPMVnmLocalWorker.h"
#include "APPM/Local/APPMLocalTypes.h"

/*
 * For Config Replay.
 */
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "APPM/Global/APPMVlanClassifierManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/APPMVlanManagedObject.h"
#include "APPM/Global/PfcRxTxMapManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/ProfileDomainManagedObject.h"
#include "APPM/Global/ProfileMacManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/APPM/APPMPortProfileDomainConfigMessage.h"

namespace DcmNs
{

    APPMLocalObjectManager::APPMLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pAPPMLocalConfWorker = new APPMLocalConfWorker (this);
        prismAssert (NULL != m_pAPPMLocalConfWorker, __FILE__, __LINE__);
        m_pAPPMLocalShowWorker = new APPMLocalShowWorker (this);
        prismAssert (NULL != m_pAPPMLocalShowWorker, __FILE__, __LINE__);
        m_pAPPMVnmLocalWorker = new APPMVnmLocalWorker (this);
        prismAssert (NULL != m_pAPPMVnmLocalWorker, __FILE__, __LINE__);
    }

    APPMLocalObjectManager::~APPMLocalObjectManager ()
    {
    }

    APPMLocalObjectManager  *APPMLocalObjectManager::getInstance()
    {
        static APPMLocalObjectManager *pAPPMLocalObjectManager = new APPMLocalObjectManager ();

        WaveNs::prismAssert (NULL != pAPPMLocalObjectManager, __FILE__, __LINE__);

        return (pAPPMLocalObjectManager);
    }

    PrismServiceId  APPMLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *APPMLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *APPMLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    UI8 APPMLocalObjectManager::APPMConfigReplayPortProfileCreate(ProfileManagedObject *pProfileManagedObject)
    {
        UI8 status = 0;

		if (!pProfileManagedObject) {
			return status;
		}
		
        prismAssert ((pProfileManagedObject != NULL), __FILE__, __LINE__);
        status = pProfileManagedObject->sendCreationRequestToClient();

        return status;
    }
	UI8 APPMLocalObjectManager::APPMConfigReplayIsAllow(ProfileManagedObject *pProfileManagedObject)
    {
        UI8 status = 0;
        prismAssert ((pProfileManagedObject != NULL), __FILE__, __LINE__);

        status = pProfileManagedObject->sendIsAllowRequestToClient();

        return status;
    }

	UI8 APPMLocalObjectManager::APPMConfigReplayIsFloodingRestricted(
		ProfileManagedObject *pProfileManagedObject)
	{
		UI8 status = 0;
		prismAssert((pProfileManagedObject != NULL), __FILE__, __LINE__);

		status = pProfileManagedObject->sendIsFloodingRestrictedRequestToClient();

		return status;
	}

    UI8 APPMLocalObjectManager::APPMConfigReplayPortProfileActivate(ProfileManagedObject *pProfileManagedObject)
    {
        UI8 status = 0;
		if (!pProfileManagedObject) {
			return status;
		}
		
        prismAssert ((pProfileManagedObject != NULL), __FILE__, __LINE__);
        status = pProfileManagedObject->sendActivationRequestToClient();

        return status;
    }

    UI8 APPMLocalObjectManager::APPMConfigReplayPortProfileAssociate(ProfileManagedObject *pProfileManagedObject)
    {
        UI8 status = 0;
		
		if (!pProfileManagedObject) {
			return status;
		}
		
        prismAssert ((pProfileManagedObject != NULL), __FILE__, __LINE__);
        status = pProfileManagedObject->sendAssociationRequestToClient();

        return status;
    } 

    void APPMLocalObjectManager::APPMConfigReplayVlanProfile(VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id)
    {
        //UI8 status = 0;

		if (!pVlanProfileCfgManagedObject) {
			return;
		}

		prismAssert ((pVlanProfileCfgManagedObject != NULL), __FILE__, __LINE__);

        pVlanProfileCfgManagedObject->sendBulkRequestToClient(profile_name, profile_id);

    }

    void APPMLocalObjectManager::APPMConfigReplaySecurityProfile(
			SecProfileCfgManagedObject *pSecProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id)
    {
        //UI8 status = 0;

		if (!pSecProfileCfgManagedObject) {
			return;
		}
		
        prismAssert ((pSecProfileCfgManagedObject != NULL), __FILE__, __LINE__);

        pSecProfileCfgManagedObject->sendBulkRequestToClient(profile_name, profile_id);

    }

    void APPMLocalObjectManager::APPMConfigReplayQosProfile(
			QosProfileCfgManagedObject *pQosProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id)
    {
        //UI8 status = 0;

		if (!pQosProfileCfgManagedObject) {
			return;
		}
		
        prismAssert ((pQosProfileCfgManagedObject != NULL), __FILE__, __LINE__);

        pQosProfileCfgManagedObject->sendBulkRequestToClient(profile_name, profile_id);

    }

    void APPMLocalObjectManager::APPMConfigReplayFcoeProfile(
			FcoeProfileCfgManagedObject *pFcoeProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id)
    {
        //UI8 status = 0;

		if (!pFcoeProfileCfgManagedObject) {
			return;
		}
		
        prismAssert ((pFcoeProfileCfgManagedObject != NULL), __FILE__, __LINE__);

        pFcoeProfileCfgManagedObject->sendBulkRequestToClient(profile_name, profile_id);

    }

    void APPMLocalObjectManager::APPMConfigReplayPortProfileAll()
    {
        UI8 status = 0;
        ProfileManagedObject *pProfileManagedObject = NULL;

    	VlanProfileCfgManagedObject *vlanProfileCfgManagedObject = NULL;
    	QosProfileCfgManagedObject *qosProfileCfgManagedObject = NULL;
    	SecProfileCfgManagedObject *secProfileCfgManagedObject = NULL;
    	FcoeProfileCfgManagedObject *fcoeProfileCfgManagedObject = NULL;

    	//Fetch all the instances of ProfileManagedObject
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults;
        string profile_name;
        UI32 profile_id = 0;
        UI32 i = 0;

        pResults = querySynchronously(&syncQueryCtxt);
		if (!pResults) {
			return;
		}
		
		//For each ProfileManagedObject, perform config replay.
       for (i = 0; i < pResults->size() ; i++)
       {
           pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[i]);
           profile_name = pProfileManagedObject->getProfileName();
           profile_id = pProfileManagedObject->getProfileId();
           NSM_PLUG_DBG_S(string("Configuring Port Profile Name : ") + profile_name);

           vlanProfileCfgManagedObject = NULL;
           qosProfileCfgManagedObject = NULL;
           secProfileCfgManagedObject = NULL;
           fcoeProfileCfgManagedObject = NULL;

   		//From the ProfileManagedObject, get all the sub profile
   		//managed objects and use the same in config replay of the
   		//sub profiles.
   		vector<WaveManagedObjectPointer<SubProfileManagedObject> > subProfileVector =
   				pProfileManagedObject->getSubProfileCfg();
                
        APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, string("Number of subprofiles<:") + subProfileVector.size()+ string(":>"));

   		for (unsigned int iterator = 0; iterator < subProfileVector.size();
   				iterator++) {

   			if (NULL != dynamic_cast<VlanProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
   				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "VLAN Profile Exists.");
   				vlanProfileCfgManagedObject =
   						dynamic_cast<VlanProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
   			} else if (NULL != dynamic_cast<QosProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
   				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "QOS Profile Exists.");
   				qosProfileCfgManagedObject =
   						dynamic_cast<QosProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
   			} else if (NULL != dynamic_cast<SecProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
   				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Sec Profile Exists.");
   				secProfileCfgManagedObject =
   						dynamic_cast<SecProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
   			} else if (NULL != dynamic_cast<FcoeProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
   				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "FCOE Profile Exists.");
   				fcoeProfileCfgManagedObject =
   						dynamic_cast<FcoeProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
   			} else {
   				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "NO Profile Exists.");
   			}
   		}

		// Port Profile Creation Config Replay
           status = APPMConfigReplayPortProfileCreate(pProfileManagedObject);

            //No need to execute sub-profile commands , if profile creation
            //itself failed.

           if( status ) 
           {
               //Do not replay the default vlan profile.
               if ( profile_name.compare("default") != 0 ) {
                   APPMConfigReplayVlanProfile(vlanProfileCfgManagedObject,profile_name,profile_id);
               }
               APPMConfigReplayQosProfile(qosProfileCfgManagedObject,profile_name,profile_id);
               APPMConfigReplaySecurityProfile(secProfileCfgManagedObject,profile_name,profile_id);
               APPMConfigReplayFcoeProfile(fcoeProfileCfgManagedObject,profile_name,profile_id);
           }
           status =  APPMConfigReplayPortProfileActivate(pProfileManagedObject);
           status =  APPMConfigReplayPortProfileAssociate(pProfileManagedObject);
		   if( profile_name.compare("default") == 0) {
			status =  APPMConfigReplayIsAllow(pProfileManagedObject);
			status = APPMConfigReplayIsFloodingRestricted(pProfileManagedObject);
			}
           NSM_PLUG_DBG_S(string("Configuring Port Profile Name :  ") + profile_name + string(",Status : ") + status);
       } 

       WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    }
	
	/*Postboot config replay of the port profile domains.*/
	void APPMLocalObjectManager::APPMConfigReplayAllPortProfileDomains() {
	
        UI32 status = 0;
        ProfileDomainManagedObject *pProfileDomainManagedObject = NULL;

    	/*Fetch all the instances of ProfileDomainManagedObject*/
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileDomainManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults;
        UI32 i = 0;

        pResults = querySynchronously(&syncQueryCtxt);
		if (!pResults) {
			return;
		}
		if (0 == (pResults->size())) {
			return;
		}
		
		//For each ProfileManagedObject, perform config replay.
       for (i = 0; i < pResults->size() ; i++) {
	   
           pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*pResults)[i]);
		   /* Postboot config replay of port-profile-domain create*/
           status = APPMConfigReplayPortProfileDomainCreate(pProfileDomainManagedObject);

           if (0 != status)
           {
               // FIXME : Sagar : Please handle the return status.  Otherwise remove the return status.
           }

		   /*Postboot config replay of port-profile-domain edit*/
           status = APPMConfigReplayPortProfileDomainEdit(pProfileDomainManagedObject);

           if (0 != status)
           {
               // FIXME : Sagar : Please handle the return status.  Otherwise remove the return status.
           }
       } 
       WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	/*Create all the port profile domains.*/
	UI32 APPMLocalObjectManager::APPMConfigReplayPortProfileDomainCreate(
			ProfileDomainManagedObject *pProfileDomainManagedObject) {
			
		APPMPortProfileDomainConfigMessage* pAPPMPortProfileDomainConfigMessage = new APPMPortProfileDomainConfigMessage();
		string portProfileDomainName = pProfileDomainManagedObject->getPortProfileDomainName();
		pAPPMPortProfileDomainConfigMessage->setPortProfileDomainName(portProfileDomainName);
		pAPPMPortProfileDomainConfigMessage->setCmdCode(PORT_PROFILE_DOMAIN_CREATE);
		UI32 status = sendSynchronouslyToWaveClient("nsm", pAPPMPortProfileDomainConfigMessage);
		if( (0 == status) || (WAVE_MESSAGE_SUCCESS == status) ) {
			tracePrintf(TRACE_LEVEL_INFO, 
				"APPMConfigReplayPortProfileDomainCreate succeeded for port-profile-domain <%s>", portProfileDomainName.c_str());
		} else {
			tracePrintf(TRACE_LEVEL_INFO, 
				"APPMConfigReplayPortProfileDomainCreate failed for port-profile-domain <%s>, with an error code <%d>"
				, portProfileDomainName.c_str(), status);
		}		
		delete pAPPMPortProfileDomainConfigMessage;
		return status;
	}
	
	/*Edit all the port profile domains.*/
	UI32 APPMLocalObjectManager::APPMConfigReplayPortProfileDomainEdit(
		ProfileDomainManagedObject *pProfileDomainManagedObject) {

		string portProfileDomainName = pProfileDomainManagedObject->getPortProfileDomainName();
		vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> > associatedPortProfilesOfDomain 
            = pProfileDomainManagedObject->getAssociatedPortProfiles();

        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMPortProfileDomainConfigMessage* pAPPMPortProfileDomainConfigMessage = NULL;
		
		for(unsigned int it = 0; it < associatedPortProfilesOfDomain.size(); it++) {
			pAPPMPortProfileDomainConfigMessage = new APPMPortProfileDomainConfigMessage();
			string portProfileName = (associatedPortProfilesOfDomain[it])->getPortProfileName();
			pAPPMPortProfileDomainConfigMessage->setCmdCode(PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN);
			pAPPMPortProfileDomainConfigMessage->setPortProfileDomainName(portProfileDomainName);
			pAPPMPortProfileDomainConfigMessage->setPortProfileName(portProfileName);
			status = sendSynchronouslyToWaveClient("nsm", pAPPMPortProfileDomainConfigMessage);
			if( (0 == status) || (WAVE_MESSAGE_SUCCESS == status) ) {
				/*tracePrintf(TRACE_LEVEL_INFO, 
					"APPMConfigReplayPortProfileDomainEdit : Addition of port-profile <%s> to"
					"port-profile-domain <%s> succeeded.", portProfileName.c_str(), portProfileDomainName.c_str());*/
			} else {
				/*tracePrintf(TRACE_LEVEL_INFO, 
					"APPMConfigReplayPortProfileDomainEdit : Addition of port-profile <%s> to"
					"port-profile-domain <%s> failed, with an error code <%d>"
					, portProfileName.c_str(), portProfileDomainName.c_str(), status);*/
			}			
			delete pAPPMPortProfileDomainConfigMessage;
		}
		return status;		
	}
		

    void APPMLocalObjectManager::postboot (WaveAsynchronousContextForPostbootPhase 
        *pWaveAsynchronousContextForPostBootPhase)
    {
        string name  = pWaveAsynchronousContextForPostBootPhase->getPassName();

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE3") {
            APPMConfigReplayPortProfileAll();
			APPMConfigReplayAllPortProfileDomains();
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();

    }

}
