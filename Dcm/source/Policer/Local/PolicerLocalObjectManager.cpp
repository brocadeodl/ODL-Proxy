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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sbalodia,Shivanand                                           *
 **************************************************************************/

#include "Policer/Local/PolicerLocalObjectManager.h"
#include "Policer/Local/PolicerLocalWorker.h"
#include "Policer/Local/PolicerTypes.h"
#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PolicerPriorityMapManagedObject.h"
#include "Policer/Global/PoClassMapManagedObject.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "ClientInterface/Policer/PolicerClientClassMapMessage.h"
#include "ClientInterface/Policer/PolicerClientPriorityMapMessage.h"
#include "ClientInterface/Policer/PolicerClientPolicyClassMapMessage.h"
#include "ClientInterface/Policer/PolicerClientTeMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "ssm_policer_dcmd_public.h"
#include "Utils/Capabilities.h"

#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "Qos/Common/QosUtils.h"
#include "Qos/Global/QosGlobalTypes.h"



namespace DcmNs
{

    PolicerLocalObjectManager::PolicerLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pPolicerLocalWorker = new PolicerLocalWorker (this);
        prismAssert (NULL != m_pPolicerLocalWorker, __FILE__, __LINE__);
    }

    PolicerLocalObjectManager::~PolicerLocalObjectManager ()
    {
    }

    PolicerLocalObjectManager  *PolicerLocalObjectManager::getInstance()
    {
        static PolicerLocalObjectManager *pPolicerLocalObjectManager = new PolicerLocalObjectManager ();

        WaveNs::prismAssert (NULL != pPolicerLocalObjectManager, __FILE__, __LINE__);

        return (pPolicerLocalObjectManager);
    }

    string  PolicerLocalObjectManager::getClassName()
    {
        return ("Policer");
    }

    PrismServiceId  PolicerLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *PolicerLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PolicerLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	void PolicerLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase) {

		string func_name = "PolicreLocalObjectManager::postboot() ";
		string pass_name = pWaveAsynchronousContextForPostBootPhase->getPassName();
		int32_t slotId = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
		LocationId locId = FrameworkToolKit::getThisLocationId();
		trace (TRACE_LEVEL_INFO, func_name + " Entered with pass: " + pass_name + " SlotId = " + slotId +" LocationId = " + locId);

		//Global config is replayed on all the switches irrespective of the platform.
        if (pass_name.compare("DCM_POSTBOOT_GLOBAL_STAGE1") == 0) {
            //replay Class maps
            ConfigReplayClassMaps();
            //replay priority maps
            ConfigReplayPriorityMaps();
            //replay policy maps
            ConfigReplayPolicyMaps();
		}
		else if (pass_name == "DCM_POSTBOOT_SLOT_STAGE2") {
            if(Capabilities::isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
			    postBootPhyPolicerConfiguration(slotId, locId);
            } else {
                trace (TRACE_LEVEL_INFO, "qos service-policy is not supported in this platform"); 
            }
		} else if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE3") {
            if(Capabilities::isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
                postBootPoPolicerConfiguration(slotId, locId);
            } else {
                trace (TRACE_LEVEL_INFO, "qos service-policy is not supported in this platform");
            }
		}
		
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
	}

	void PolicerLocalObjectManager::postBootPhyPolicerConfiguration (int32_t slotId, LocationId locId)
	{
			NSM_PLUG_INFO_S (string("PHY::Entering..") + "SlotId = " + slotId +
							" LocationId = " + locId);

			PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
			vector<PhyIntfLocalManagedObject *> results;

			NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);

			for (UI32 i = 0; i < results.size(); i++) {
				pPhyIntfLocalManagedObject = results[i];

				if(pPhyIntfLocalManagedObject != NULL)
				{
					if(pPhyIntfLocalManagedObject->getPortrole() == false)
					{
						trace (TRACE_LEVEL_INFO, string("Reply configuration Edge Port :")+ pPhyIntfLocalManagedObject->getIfName());
						//add input policy map
						PolicerLocalObjectManager::configReplayPhyPolicyMaps(pPhyIntfLocalManagedObject, (PoIntfManagedObject*) NULL);
					}
					else
					{
						trace (TRACE_LEVEL_INFO, string("No Reply configuration ISL Port :") + pPhyIntfLocalManagedObject->getIfName());
					}
					delete pPhyIntfLocalManagedObject;
				}
			}
			return;
	}


	//Postboot config replay of po config.
	void PolicerLocalObjectManager::postBootPoPolicerConfiguration (int32_t slotId, LocationId locId)
	{
		NSM_PLUG_INFO_S (string("PHY::Entering.. postBootPoPolicerConfiguration") + "SlotId = " + slotId +
									" LocationId = " + locId);
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
        if(pResults != NULL)
        {
            if (pResults->size() > 0)
            {
                for(unsigned int i = 0;i <  pResults->size(); i++)
                {
                    pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
                    PolicerLocalObjectManager::configReplayPhyPolicyMaps( (PhyIntfLocalManagedObject*) NULL, pPoIntfManagedObject);
                }
            }
            else
            {
            	NSM_PLUG_INFO_S ("pResults size is zero");
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        else
        {
        	NSM_PLUG_INFO_S ("pResults is NULL");
        }
        return ;
    }


	void PolicerLocalObjectManager::configReplayPhyPolicyMaps( PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,
            PoIntfManagedObject *pPoIntfManagedObject ) {

		if ( NULL == pPhyIntfLocalManagedObject && NULL == pPoIntfManagedObject ) {
			return;
		}
		
		ObjectId inputPolicyMap = ObjectId::NullObjectId;
		ObjectId outputPolicyMap = ObjectId::NullObjectId;
		string ifName = "";
        string policyMapName = "";
		InterfaceType ifType = IF_TYPE_INVALID;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

		if ( NULL != pPhyIntfLocalManagedObject ) {

			ifName = pPhyIntfLocalManagedObject->getIfName();
			ifType = pPhyIntfLocalManagedObject->getPhyType();

			if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName())) {
				NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(),
						ifName);
			} else {
				ifName = pPhyIntfLocalManagedObject->getIfName();
			}

			//ifName.insert(0, "te");
			char buffer[MAX_STR];
			buffer[MAX_STR - 1] = '\0';
			UI32 bufferSize = sizeof (buffer);
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer,
					bufferSize);
			ifName = buffer;

			inputPolicyMap = pPhyIntfLocalManagedObject->getInputPolicyMapId();
			outputPolicyMap =
					pPhyIntfLocalManagedObject->getOutputPolicyMapId();
		} else if ( NULL != pPoIntfManagedObject ) {

			ifName = pPoIntfManagedObject->getIfName();
			ifType = IF_TYPE_PO;

			char buffer[MAX_STR];
			buffer[MAX_STR - 1] = '\0';
			UI32 bufferSize = sizeof (buffer);
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(ifType, ifName.c_str(), buffer,
					bufferSize);
			ifName = buffer;

			inputPolicyMap = pPoIntfManagedObject->getInputPolicyMapId();
			outputPolicyMap =
					pPoIntfManagedObject->getOutputPolicyMapId();
		}


		PolicerClientTeMessage *pPolicyMessage = NULL;

		//Assign input policy map for the interface.
		if (inputPolicyMap != ObjectId::NullObjectId) {
			PolicyClassMapManagedObject *pInputPolicyMapMO =
					dynamic_cast<PolicyClassMapManagedObject *>(WaveObjectManagerToolKit::queryManagedObject(
							inputPolicyMap));
            policyMapName = pInputPolicyMapMO->getPolicyClassMapName();
			if (pInputPolicyMapMO != NULL) {
				pPolicyMessage = new PolicerClientTeMessage();
				if (pPolicyMessage != NULL) {
					pPolicyMessage->setIfName(ifName);
					pPolicyMessage->setMapName(
							pInputPolicyMapMO->getPolicyClassMapName());
					pPolicyMessage->setOpCode(CONFIGURE_INTERFACE_QOS);
					pPolicyMessage->setCmdCode(FB_POLICYMAP_BIND_INTERFACE_IN);
					status = WaveObjectManagerToolKit::sendSynchronouslyToWaveClient("qos", pPolicyMessage);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if( WAVE_MESSAGE_SUCCESS == pPolicyMessage->getCompletionStatus()) {
                            if(WAVE_MESSAGE_SUCCESS == pPolicyMessage->getClientStatus()) {
                            } else {
                                WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <")+ policyMapName +
                                        + ">on an interface<" + ifName + ">failed with clientstatus<" + 
                                        FrameworkToolKit::localize(pPolicyMessage->getClientStatus()) + ">");
                            }
                        } else {
                            WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <") + policyMapName +
                                    + ">on an interface<" + ifName + ">failed with completionstatus<" +
                                    FrameworkToolKit::localize(pPolicyMessage->getCompletionStatus()) + ">");
                        }
                    } else {
                        WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <") + policyMapName +
                                + ">on an interface<" + ifName + ">failed with status<" +
                                FrameworkToolKit::localize(status) + ">");

                    }
					delete pPolicyMessage;
					pPolicyMessage = NULL;
					delete pInputPolicyMapMO;
					pInputPolicyMapMO = NULL;
				}
			}
		}

		//Assign output policy map for the interface.
		if (outputPolicyMap != ObjectId::NullObjectId) {
			PolicyClassMapManagedObject *pOutputPolicyMapMO =
					dynamic_cast<PolicyClassMapManagedObject *>(WaveObjectManagerToolKit::queryManagedObject(
							outputPolicyMap));
            policyMapName = pOutputPolicyMapMO->getPolicyClassMapName();
			if (pOutputPolicyMapMO != NULL) {
				pPolicyMessage = new PolicerClientTeMessage();
				if (pPolicyMessage != NULL) {
					pPolicyMessage->setIfName(ifName);
					pPolicyMessage->setMapName(
							pOutputPolicyMapMO->getPolicyClassMapName());
					pPolicyMessage->setOpCode(CONFIGURE_INTERFACE_QOS);
					pPolicyMessage->setCmdCode(FB_POLICYMAP_BIND_INTERFACE_OUT);
					status = WaveObjectManagerToolKit::sendSynchronouslyToWaveClient("qos", pPolicyMessage);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if( WAVE_MESSAGE_SUCCESS == pPolicyMessage->getCompletionStatus()) {
                            if(WAVE_MESSAGE_SUCCESS == pPolicyMessage->getClientStatus()) {
                            } else {
                                WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <") + policyMapName +
                                        + ">on an interface<" + ifName + ">failed with clientstatus<" +
                                        FrameworkToolKit::localize(pPolicyMessage->getClientStatus()) + ">");
                            }
                        } else {
                            WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <") + policyMapName +
                                    + ">on an interface<" + ifName + ">failed with completionstatus<" +
                                    FrameworkToolKit::localize(pPolicyMessage->getCompletionStatus()) + ">");
                        }
                    } else {
                        WaveNs::trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Applying policy-map <") + policyMapName +
                                + ">on an interface<" + ifName + ">failed with status<" +
                                FrameworkToolKit::localize(status) + ">");

                    }
                    delete pOutputPolicyMapMO;
					pOutputPolicyMapMO = NULL;
					delete pPolicyMessage;
					pPolicyMessage = NULL;
				}
			}

		}
	}

	
	void PolicerLocalObjectManager::ConfigReplayClassMaps() {

		ClassMapManagedObject *classMapMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		trace (TRACE_LEVEL_INFO, "Config replay policy class maps");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ClassMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {

			UI32 numberOfResults = pResults->size ();
			trace(TRACE_LEVEL_INFO, string("Number of Policer Class Maps: ") + numberOfResults);
																            
			for (UI32 i = 0; i < numberOfResults; i++) {

				classMapMO = dynamic_cast<ClassMapManagedObject *>((*pResults)[i]);
				//Creation of class map message:
				PolicerClientClassMapMessage *pPolicerClientClassMapMessage =
						new PolicerClientClassMapMessage();
				pPolicerClientClassMapMessage->setOpcode(FB_CLASSMAP_CREATE);
				pPolicerClientClassMapMessage->setClassMapName(
						classMapMO->getClassMapName());
				status = sendSynchronouslyToWaveClient("qos",
						pPolicerClientClassMapMessage);
                if (WAVE_MESSAGE_SUCCESS == status) {
                    if( WAVE_MESSAGE_SUCCESS == pPolicerClientClassMapMessage->getCompletionStatus()) {
                        if(WAVE_MESSAGE_SUCCESS == pPolicerClientClassMapMessage->getClientStatus()) {
                        } else {
                            trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating class-map <") + classMapMO->getClassMapName() +
                                    ">failed with clientstatus<" +
                                    FrameworkToolKit::localize(pPolicerClientClassMapMessage->getClientStatus()) + ">");
                        }
                    } else {
                        trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating class-map <") + classMapMO->getClassMapName() +
                                ">failed with completionstatus<" +
                                FrameworkToolKit::localize(pPolicerClientClassMapMessage->getCompletionStatus()) + ">");
                    }
                } else {
                    trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating class-map <") + classMapMO->getClassMapName() +
                            ">failed with status<" +
                            FrameworkToolKit::localize(status) + ">");
                }

				delete pPolicerClientClassMapMessage;

				//Add acl filter to the created class map
                if("" != classMapMO->getAccessListName()) {
                    
                    pPolicerClientClassMapMessage = new PolicerClientClassMapMessage();
                    pPolicerClientClassMapMessage->setOpcode(FB_ADD_FILTER_TO_CLASS);
                    pPolicerClientClassMapMessage->setClassMapName(
						classMapMO->getClassMapName());
                    pPolicerClientClassMapMessage->setAccessListName(
						classMapMO->getAccessListName());
                    pPolicerClientClassMapMessage->setFilterCriteria(
						classMapMO->getFilterCriteria());
                    status = sendSynchronouslyToWaveClient("qos",
						pPolicerClientClassMapMessage);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if( WAVE_MESSAGE_SUCCESS == pPolicerClientClassMapMessage->getCompletionStatus()) {
                            if(WAVE_MESSAGE_SUCCESS == pPolicerClientClassMapMessage->getClientStatus()) {
                            } else {
                                trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Editing class-map <") + classMapMO->getClassMapName() +
                                        ">failed with clientstatus<" +
                                        FrameworkToolKit::localize(pPolicerClientClassMapMessage->getClientStatus()) + ">");
                            }
                        } else {
                            trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Editing class-map <") + classMapMO->getClassMapName() +
                                    ">failed with completionstatus<" +
                                    FrameworkToolKit::localize(pPolicerClientClassMapMessage->getCompletionStatus()) + ">");
                        }
                    } else {
                        trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Editing class-map <") + classMapMO->getClassMapName() +
                                ">failed with status<" +
                                FrameworkToolKit::localize(status) + ">");
                    }

                    delete pPolicerClientClassMapMessage;
                }
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	void PolicerLocalObjectManager::ConfigReplayPriorityMaps()
	{
		PolicerPriorityMapManagedObject *priorityMapMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		trace (TRACE_LEVEL_INFO, "Config replay policer priority maps");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicerPriorityMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults)
		{
			UI32 numberOfResults = pResults->size ();
			trace(TRACE_LEVEL_INFO, string("Number of Policer Priority Maps") + numberOfResults);

			for (UI32 i = 0; i < numberOfResults; i++)
			{
				priorityMapMO = dynamic_cast<PolicerPriorityMapManagedObject *>((*pResults)[i]);
				
				PolicerClientPriorityMapMessage *pPolicerClientPriorityMapMessage = new PolicerClientPriorityMapMessage();	

				if(pPolicerClientPriorityMapMessage != NULL)
				{
					pPolicerClientPriorityMapMessage->setPriorityMapName(priorityMapMO->getPriorityMapName());
					pPolicerClientPriorityMapMessage->setConformPriorityMap0(priorityMapMO->getConformPriorityMap0());
					pPolicerClientPriorityMapMessage->setConformPriorityMap1(priorityMapMO->getConformPriorityMap1());
					pPolicerClientPriorityMapMessage->setConformPriorityMap2(priorityMapMO->getConformPriorityMap2());
					pPolicerClientPriorityMapMessage->setConformPriorityMap3(priorityMapMO->getConformPriorityMap3());
					pPolicerClientPriorityMapMessage->setConformPriorityMap4(priorityMapMO->getConformPriorityMap4());
					pPolicerClientPriorityMapMessage->setConformPriorityMap5(priorityMapMO->getConformPriorityMap5());
					pPolicerClientPriorityMapMessage->setConformPriorityMap6(priorityMapMO->getConformPriorityMap6());
					pPolicerClientPriorityMapMessage->setConformPriorityMap7(priorityMapMO->getConformPriorityMap7());

					pPolicerClientPriorityMapMessage->setExceedPriorityMap0(priorityMapMO->getExceedPriorityMap0());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap1(priorityMapMO->getExceedPriorityMap1());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap2(priorityMapMO->getExceedPriorityMap2());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap3(priorityMapMO->getExceedPriorityMap3());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap4(priorityMapMO->getExceedPriorityMap4());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap5(priorityMapMO->getExceedPriorityMap5());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap6(priorityMapMO->getExceedPriorityMap6());
					pPolicerClientPriorityMapMessage->setExceedPriorityMap7(priorityMapMO->getExceedPriorityMap7());
					
					if(pPolicerClientPriorityMapMessage->getConformPriorityMap0() != POLICER_NOT_USR_CFGED_VAL)
						pPolicerClientPriorityMapMessage->setConformCos(1);
					
					if(pPolicerClientPriorityMapMessage->getExceedPriorityMap0() != POLICER_NOT_USR_CFGED_VAL)
						pPolicerClientPriorityMapMessage->setExceedCos(1);
					
					pPolicerClientPriorityMapMessage->setOpcode(SSM_POL_PRIORITYMAP_CREATE);

					status = sendSynchronouslyToWaveClient("qos",pPolicerClientPriorityMapMessage);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if( WAVE_MESSAGE_SUCCESS == pPolicerClientPriorityMapMessage->getCompletionStatus()) {
                            if(WAVE_MESSAGE_SUCCESS == pPolicerClientPriorityMapMessage->getClientStatus()) {
                            } else {
                                trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating police-priority-map <") + priorityMapMO->getPriorityMapName() +
                                        ">failed with clientstatus<" +
                                        FrameworkToolKit::localize(pPolicerClientPriorityMapMessage->getClientStatus()) + ">");
                            }
                        } else {
                            trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating police-priority-map <") + priorityMapMO->getPriorityMapName() +
                                    ">failed with completionstatus<" +
                                    FrameworkToolKit::localize(pPolicerClientPriorityMapMessage->getCompletionStatus()) + ">");
                        }
                    } else {
                        trace(TRACE_LEVEL_INFO, string("MQC_POSTBOOT : Creating police-priority-map <") + priorityMapMO->getPriorityMapName() +
                                ">failed with status<" +
                                FrameworkToolKit::localize(status) + ">");
                    }

					delete pPolicerClientPriorityMapMessage;
				}
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	void PolicerLocalObjectManager::ConfigReplayPolicyMaps() {

		PolicyClassMapManagedObject *policyMapMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		trace (TRACE_LEVEL_INFO, "Config replay qos service-policy maps");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicyClassMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {

			UI32 numberOfResults = pResults->size ();
			trace(TRACE_LEVEL_INFO, string("Number of qos service-policy Maps") + numberOfResults);

			for (UI32 i = 0; i < numberOfResults; i++) {

				policyMapMO = dynamic_cast<PolicyClassMapManagedObject *>((*pResults)[i]);

				/*first create this policy maps*/
				trace(TRACE_LEVEL_INFO, string("Postboot config replay of qos service-policy map ") + policyMapMO->getPolicyClassMapName());
				PolicerClientPolicyClassMapMessage *pPolicyCreateMessage = new PolicerClientPolicyClassMapMessage();
				
				if(pPolicyCreateMessage != NULL) {

					pPolicyCreateMessage->setPolicyClassMapName(policyMapMO->getPolicyClassMapName());
					pPolicyCreateMessage->setOpcode(FB_POLICYMAP_CREATE);
					status = sendSynchronouslyToWaveClient("qos",pPolicyCreateMessage);
					delete pPolicyCreateMessage;

					/*now create poclass maps and set attributes*/
					vector<WaveManagedObjectPointer<PoClassMapManagedObject> > vectPoClassMapMO = policyMapMO->getPoManObj();
					UI32 numberOfClassMaps = vectPoClassMapMO.size();
					for (UI32 j = 0; j < numberOfClassMaps; j++)
					{
						/*add po class map. opcode FB_ADD_CLASS_MAP_TO_POLICY_MAP*/
						PolicerClientPolicyClassMapMessage *pPolicyClassCreateMessage = new PolicerClientPolicyClassMapMessage();
						if(pPolicyClassCreateMessage != NULL)
						{
							PoClassMapManagedObject *pPoClassMapMO = vectPoClassMapMO[j].operator-> ();
							pPolicyClassCreateMessage->setPolicyClassMapName(policyMapMO->getPolicyClassMapName());

							ClassMapManagedObject *classMapMO = dynamic_cast<ClassMapManagedObject *>(queryManagedObject(pPoClassMapMO->getClassMapId()));

							trace(TRACE_LEVEL_INFO, string("creating po class map ") + classMapMO->getClassMapName());

							pPolicyClassCreateMessage->setClassMapId(classMapMO->getClassMapName());
							pPolicyClassCreateMessage->setOpcode(FB_ADD_CLASS_MAP_TO_POLICY_MAP);
							status = sendSynchronouslyToWaveClient("qos",pPolicyClassCreateMessage);
							delete pPolicyClassCreateMessage;

							/*set attributes. Opcode FB_SET_POLICER_ATTR*/
							trace(TRACE_LEVEL_INFO, "setting attributes");
								if (POLICER_NOT_USR_CFGED_CIR_CBS
										!= pPoClassMapMO->getCir()) {
									trace(TRACE_LEVEL_INFO, "Config replay of policer action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());
									pPolicerClientPolicyClassMapMessage->setCir(
											pPoClassMapMO->getCir());
									pPolicerClientPolicyClassMapMessage->setCbs(
											pPoClassMapMO->getCbs());
									pPolicerClientPolicyClassMapMessage->setEir(
											pPoClassMapMO->getEir());
									pPolicerClientPolicyClassMapMessage->setEbs(
											pPoClassMapMO->getEbs());
									pPolicerClientPolicyClassMapMessage->setConformDscp(
											pPoClassMapMO->getConformDscp());
									pPolicerClientPolicyClassMapMessage->setConformPrecedence(
											pPoClassMapMO->getConformPrecedence());
									pPolicerClientPolicyClassMapMessage->setConformType(
											pPoClassMapMO->getConformType());
									pPolicerClientPolicyClassMapMessage->setConformTc(
											pPoClassMapMO->getConformTc());
									pPolicerClientPolicyClassMapMessage->setExceedDscp(
											pPoClassMapMO->getExceedDscp());
									pPolicerClientPolicyClassMapMessage->setExceedPrecedence(
											pPoClassMapMO->getExceedPrecedence());
									pPolicerClientPolicyClassMapMessage->setExceedType(
											pPoClassMapMO->getExceedType());
									pPolicerClientPolicyClassMapMessage->setExceedTc(
											pPoClassMapMO->getExceedTc());

									if (pPoClassMapMO->getPriorityMapId()
											!= ObjectId::NullObjectId) {
										PolicerPriorityMapManagedObject *priorityMapMO =
												dynamic_cast<PolicerPriorityMapManagedObject *>(queryManagedObject(
														pPoClassMapMO->getPriorityMapId()));
										pPolicerClientPolicyClassMapMessage->setPriorityMapId(
												priorityMapMO->getPriorityMapName());
									    delete priorityMapMO;
								    }

									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_SET_POLICER_ATTR);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;

								} 
                                if (FB_DEFAULT_COS_VALUE
										!= pPoClassMapMO->getCoS()) {

									trace(TRACE_LEVEL_INFO, "Config replay of set cos and tc action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());


									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_SET_COS_TC_ACTION_CREATE);
									pPolicerClientPolicyClassMapMessage->setCoS(
											pPoClassMapMO->getCoS());
									pPolicerClientPolicyClassMapMessage->setTrafficClass(
											pPoClassMapMO->getTrafficClass());
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (FB_DEFAULT_DSCP_VALUE
										!= pPoClassMapMO->getDscp()) {

									trace(TRACE_LEVEL_INFO, "Config replay of set dscp action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_SET_DSCP_ACTION_CREATE);
									pPolicerClientPolicyClassMapMessage->setDscp(
											pPoClassMapMO->getDscp());
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (FB_DEFAULT_SHAPING_RATE_VALUE
										!= pPoClassMapMO->getShapingRate()) {
									trace(TRACE_LEVEL_INFO, "Config replay of shaping rate action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());


									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_SHAPING_RATE_ACTION_CREATE);
									pPolicerClientPolicyClassMapMessage->setShapingRate(
											pPoClassMapMO->getShapingRate());
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getCoSMutationMap()) {
									trace(TRACE_LEVEL_INFO, "Config replay of cosmutation map action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());


									string cosMutaionMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_COS_MUTATION_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											cosMutaionMapName,
											pPoClassMapMO->getCoSMutationMap(),
											COS_MUTATION_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setCoSMutationMapName(
											cosMutaionMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
											!= pPoClassMapMO->getCoSTrafficClassMap()) {
									trace(TRACE_LEVEL_INFO, "Config replay of costrafficclass map action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());


									string cosTrafficClassMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											cosTrafficClassMapName,
											pPoClassMapMO->getCoSTrafficClassMap(),
											COS_TRAFFIC_CLASS_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setCoSTrafficClassMapName(
											cosTrafficClassMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getDscpMutationMap()) {
									trace(TRACE_LEVEL_INFO, "Config replay of dscpmutation map action");

									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();
									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									string dscpMutaionMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_DSCP_MUTATION_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											dscpMutaionMapName,
											pPoClassMapMO->getDscpMutationMap(),
											DSCP_MUTATION_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setDscpMutationMapName(
											dscpMutaionMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getDscpTrafficClassMap()) {

									trace(TRACE_LEVEL_INFO, "Config replay of dscptrafficclass map action");

									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									string dscpTrafficClassMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											dscpTrafficClassMapName,
											pPoClassMapMO->getDscpTrafficClassMap(),
											DSCP_TRAFFIC_CLASS_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setDscpTrafficClassMapName(
											dscpTrafficClassMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getDscpCoSMap()) {
									trace(TRACE_LEVEL_INFO, "Config replay of dscp cos action");

									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									string dscpCoSMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_DSCP_COS_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											dscpCoSMapName,
											pPoClassMapMO->getDscpCoSMap(),
											DSCP_COS_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setDscpCoSMapName(
											dscpCoSMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getSflowMap()) {

									trace(TRACE_LEVEL_INFO, "Config replay of sflow map action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									string sflowMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_SFLOW_MAP_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											sflowMapName, pPoClassMapMO->getSflowMap(),
											SFLOW_POLICY_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setSflowMapName(
											sflowMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (ObjectId::NullObjectId
										!= pPoClassMapMO->getCeeMap()) {

									trace(TRACE_LEVEL_INFO, "Config replay of cee map action");
									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									string ceeMapName = "";
									pPolicerClientPolicyClassMapMessage->setOpcode(
											FB_CEE_ACTION_CREATE);
									status = QosUtils::getQosMapNameByObjectId(
											ceeMapName, pPoClassMapMO->getCeeMap(),
											CEE_MAP, NULL);
									pPolicerClientPolicyClassMapMessage->setCeeMapName(
											ceeMapName);
									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;
								} 
                                if (FB_DEFAULT_QUEUE_SCHEDULER_TYPE
										!= pPoClassMapMO->m_scheduler_type) {

									trace(TRACE_LEVEL_INFO, "Config replay of scheduler action");

									PolicerClientPolicyClassMapMessage *pPolicerClientPolicyClassMapMessage = new PolicerClientPolicyClassMapMessage();

									pPolicerClientPolicyClassMapMessage->setPolicyClassMapName(
											policyMapMO->getPolicyClassMapName());
									pPolicerClientPolicyClassMapMessage->setClassMapId(
											classMapMO->getClassMapName());

									pPolicerClientPolicyClassMapMessage->setStrictPriority(
											pPoClassMapMO->getStrictPriority());

									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass0(
											pPoClassMapMO->getDwrrTrafficClass0());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass1(
											pPoClassMapMO->getDwrrTrafficClass1());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass2(
											pPoClassMapMO->getDwrrTrafficClass2());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass3(
											pPoClassMapMO->getDwrrTrafficClass3());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass4(
											pPoClassMapMO->getDwrrTrafficClass4());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass5(
											pPoClassMapMO->getDwrrTrafficClass5());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass6(
											pPoClassMapMO->getDwrrTrafficClass6());
									pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass7(
										FB_DEFAULT_QUEUE_DWRR_VALUE);


									/*
									 * getDwrrTrafficClassLast() will always be the last dwrr
									 * value configured. So, based on strict-priority assign
									 * getDwrrTrafficClassLast() to appropriate traffic class
									 */

									switch (pPoClassMapMO->getStrictPriority()) {

									case 7: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass0(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 6: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass1(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 5: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass2(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 4: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass3(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 3: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass4(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 2: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass5(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 1: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass6(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}
									case 0: {
										pPolicerClientPolicyClassMapMessage->setDwrrTrafficClass7(
												pPoClassMapMO->getDwrrTrafficClassLast());
										break;
									}

									}


									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass0(
											pPoClassMapMO->getShapingRateTrafficClass0());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass1(
											pPoClassMapMO->getShapingRateTrafficClass1());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass2(
											pPoClassMapMO->getShapingRateTrafficClass2());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass3(
											pPoClassMapMO->getShapingRateTrafficClass3());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass4(
											pPoClassMapMO->getShapingRateTrafficClass4());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass5(
											pPoClassMapMO->getShapingRateTrafficClass5());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass6(
											pPoClassMapMO->getShapingRateTrafficClass6());
									pPolicerClientPolicyClassMapMessage->setShapingRateTrafficClass7(
											pPoClassMapMO->getShapingRateTrafficClass7());
                                    pPolicerClientPolicyClassMapMessage->setOpcode(
                                            FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE);


									status = sendSynchronouslyToWaveClient("qos",
											pPolicerClientPolicyClassMapMessage);
									delete pPolicerClientPolicyClassMapMessage;

							}
							delete classMapMO;
						}
					}
				}
			}
		}

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : Sagar : Do we need to address this case?  Otherwise, please remove the status variable.
        }

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
}
