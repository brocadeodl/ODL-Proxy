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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "DcmCStatus.h"
#include "DcmResourceIdEnums.h"

#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    APPMGlobalObjectManager::APPMGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pAPPMGlobalConfWorker = new APPMGlobalConfWorker (this);
        prismAssert (NULL != m_pAPPMGlobalConfWorker, __FILE__, __LINE__);
    }

    APPMGlobalObjectManager::~APPMGlobalObjectManager ()
    {
    }

    APPMGlobalObjectManager  *APPMGlobalObjectManager::getInstance()
    {
        static APPMGlobalObjectManager *pAPPMGlobalObjectManager = new APPMGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pAPPMGlobalObjectManager, __FILE__, __LINE__);

        return (pAPPMGlobalObjectManager);
    }

    PrismServiceId  APPMGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *APPMGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *APPMGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	void APPMGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage,
			PrismAsynchronousContext *pPrismAsynchronousContext)
	{
		dce_cluster_validation_data_t   *data_p;
		ResourceId                      status = WAVE_MESSAGE_SUCCESS;
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
		data_p = dce_cluster_validation_collect_init(AMPP_PLUGIN_GLOBAL_VERSION);
		if (data_p == NULL) {
			pPrismAsynchronousContext->setCompletionStatus (DCE_CLUSTER_UTIL_ALLOC_FAILED);
			pPrismAsynchronousContext->callback ();
			return;
		}
		status = dce_cluster_validation_collect_done(pMessage, data_p);
		if (status != DCE_CLUSTER_UTIL_SUCCESS) {
			pPrismAsynchronousContext->setCompletionStatus (status);
			pPrismAsynchronousContext->callback ();
			return;
		}

		pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pPrismAsynchronousContext->callback ();
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
		return;

	}
 void APPMGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
            PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);

        pMessage->getValidationDetails(payload_p, len);


        APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,string("Received Validation Data of LEN=") + len);
        
            data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,string(" clusterCreateValidate ")  +
                    FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,string(" clusterCreateValidate ") + version);

        if (version != AMPP_PLUGIN_GLOBAL_VERSION) {
            APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,string(" clusterCreateValidate ")  + AMPP_PLUGIN_GLOBAL_VERSION +
                    string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(AMPP_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
        return;

    }


	bool APPMGlobalObjectManager::isVlanPartOfActiveProfile(UI32 vlanId, bool &isChangeVlanList)
	{
		/*Validate if it is ok to go ahead with vlan deletion.
		  We will check if the vlan is a part of any activated profile. 
		  If yes, we do not allow vlan deletion.*/

		WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

		vector<WaveManagedObject *> *pVlanResults = querySynchronously (&vlanSyncQueryCtxt);
		VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;
		isChangeVlanList = false;				
		bool changeVlanList = false;

		if (NULL != pVlanResults)
		{
			UI32 numberOfResults = pVlanResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG," No MO in VlanProfileCfgManagedObject found");
			}else {
				UI32 count = 0;

				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, 
					string(" numberOfResults for VlanProfileCfgManagedObject = ") + 
					numberOfResults);

				while(count != numberOfResults)
				{
					changeVlanList = false;
					pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pVlanResults)[count]);

					APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, 
						string(" count for VlanProfileCfgManagedObject = ") + count);

					APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, 
						string(" for VlanProfileCfgManagedObject name = ")
						+ pVlanProfileCfgManagedObject->getDescription());

					string profName("\0");

					profName = pVlanProfileCfgManagedObject->getDescription();
					/*We do not make any changes to the default profile*/

					if (profName.compare("default") == 0)
					{
						count++;
						continue;
					}

					if(!(pVlanProfileCfgManagedObject->getIsAllowedVlanNone()))
					{
						if(pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
						{
							vector<UI32>::iterator moremoveiter;	
							UI32Range moRemoveVlanrange = UI32Range("");
							vector <UI32> moRemoveVlanRangeVector;
							moRemoveVlanrange = pVlanProfileCfgManagedObject->getVlanRangeRemove();
							moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);     

							for (moremoveiter = moRemoveVlanRangeVector.begin(); moremoveiter != moRemoveVlanRangeVector.end(); 
								moremoveiter++)
							{
								if(*moremoveiter == vlanId)
								{
									isChangeVlanList = true;
									changeVlanList = true;
									break;
								}
							}
						}
						else
						{/*Search if it is a part of trunk or access vlan*/
							if(pVlanProfileCfgManagedObject->getSwMode() == VLAN_SW_MODE_ACCESS)
							{
								if(pVlanProfileCfgManagedObject->getAccessVlan() == vlanId)
								{
									changeVlanList = true;
									isChangeVlanList = true;
								}
							}
							else if (pVlanProfileCfgManagedObject->getSwMode() == VLAN_SW_MODE_TRUNK)
							{
								WaveManagedObjectPointer<APPMNativeVlanManagedObject> pNativeVlanMo = 
									pVlanProfileCfgManagedObject->getNativeVlanIntfConfig();

								APPMNativeVlanManagedObject *nativeTmp = pNativeVlanMo.operator-> (); 
								if(nativeTmp->getNativeVlanId() == vlanId)
								{
									changeVlanList = true;
									isChangeVlanList = true;
								}
								else
								{
									vector<UI32>::iterator moadditer;	
									UI32Range moAddVlanrange = UI32Range("");
									vector <UI32> moAddVlanRangeVector;
									moAddVlanrange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
									moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);     

									for (moadditer = moAddVlanRangeVector.begin(); moadditer != moAddVlanRangeVector.end(); moadditer++)
									{
										if(*moadditer == vlanId)
										{
											isChangeVlanList = true;
											changeVlanList = true;
											break;
										}
									}
								}
							}
						}

						if(changeVlanList)
						{
							APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " changeVlanList is true!!");

							WaveManagedObjectSynchronousQueryContext profileSyncQueryCtxt(ProfileManagedObject::getClassName());
							profileSyncQueryCtxt.addSelectField ("isActivated");

							profileSyncQueryCtxt.addAndAttribute (new AttributeString 
								(pVlanProfileCfgManagedObject->getDescription(), "profileName"));
							vector<WaveManagedObject *> *pProfileResults = querySynchronously (&profileSyncQueryCtxt);

							ProfileManagedObject *pProfileManagedObject = NULL;

							if (NULL != pProfileResults)
							{
								UI32 numberOfPPResults = pProfileResults->size ();
								if (0 == numberOfPPResults) {
									APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," cannot find Port-profile Managed object");
								}else if (1 == numberOfPPResults) {
									pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pProfileResults)[0]);
									APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Found Port-profile Managed Object");
								} else {
									APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," found Two or more objects exists");} 

								if(pProfileManagedObject->getIsActivated())
								{
									APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," cannot delete this vlan, an activated port-profile exists with this vlan");
									isChangeVlanList = true;
									WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pProfileResults);
									return (true);
								}
							}
							if(NULL != pProfileResults)
								WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pProfileResults);
						}
					}
					count++;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlanResults);
		}
		return(false);
	}
	bool APPMGlobalObjectManager::isFcoeEnabledInDefaultProfile(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext,
		PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		bool ret = false;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString ("default", "description"));
		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
		FcoeProfileCfgManagedObject *pFcoeProfileCfgManagedObject = NULL;

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (1 == numberOfResults) {
				pFcoeProfileCfgManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pResults)[0]);
				ret = pFcoeProfileCfgManagedObject->getFcoePort();
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string(" isFcoeEnabledInDefaultProfile "
						"FCoE config flag: ") + ret);
			}

			/* GC the results */
			if (pPrismSynchronousLinearSequencerContext) {
				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				delete (pResults);
			} else if (pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				delete (pResults);
			}
			else {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
		}
		return (ret);
	}
}
