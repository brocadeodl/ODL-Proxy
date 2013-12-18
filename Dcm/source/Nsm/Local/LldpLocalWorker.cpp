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
 *   Author : sanjeevj, mmohan                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/LldpLocalWorker.h"
#include "Nsm/Local/NsmLldpIntfConfSetMessage.h"
#include "Lldp/Global/LldpGlobalProfileGetObjectIdMessage.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "Nsm/Local/NsmLldpInternalMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Lldp/LldpUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"

namespace DcmNs
{

    LldpLocalWorker::LldpLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        LldpIntfConfLocalManagedObject    LldpIntfConfLocalManagedObject    (pNsmLocalObjectManager);
        LldpIntfConfLocalManagedObject.setupOrm ();
        addManagedClass (LldpIntfConfLocalManagedObject::getClassName (), this);
        addOperationMap (NSMLLDPINTFCONFSET, reinterpret_cast<PrismMessageHandler> (&LldpLocalWorker::NsmLldpIntfConfSetMessageHandler));
        addOperationMap (NSMLLDPINTERNAL, reinterpret_cast<PrismMessageHandler> (&LldpLocalWorker::NsmLldpInternalMessageHandler));
    }

    LldpLocalWorker::~LldpLocalWorker ()
    {
    }

    PrismMessage  *LldpLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMLLDPINTFCONFSET :
                pPrismMessage = new NsmLldpIntfConfSetMessage ();
                break;
            case NSMLLDPINTERNAL :
                pPrismMessage = new NsmLldpInternalMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LldpLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((LldpIntfConfLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LldpIntfConfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
			NSM_PLUG_INFO_S(string("Unknown Managed Class Name:") + managedClassName);
        }
        return (pWaveManagedObject);
    }
	
	ResourceId LldpLocalWorker::updateLldpPhyInfConf (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_INFO_S(string("Entered"));

        NsmLldpIntfConfSetMessage *pNsmLldpIntfConfSetMessage = dynamic_cast<NsmLldpIntfConfSetMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status;
        NsmLldpClientIntfConfSetMessage *m = new NsmLldpClientIntfConfSetMessage();
        UI32 cmdcode = pNsmLldpIntfConfSetMessage->getCmdcode();

		/* Read the header from received message and populate 
		 * sending message
		 */
		m->setMgmtFlags (pNsmLldpIntfConfSetMessage->getMgmtFlags());
        m->setMsgType (pNsmLldpIntfConfSetMessage->getMsgType());
        m->setCmdcode (pNsmLldpIntfConfSetMessage->getCmdcode());
        m->setIfType (pNsmLldpIntfConfSetMessage->getIfType());
		m->setLldpCeeStatus(pNsmLldpIntfConfSetMessage->getLldpCeeStatus());
	
		/* Three tupple to two tupple conversion */
        if (true  == NsmUtils::isValidThreeTuple(pNsmLldpIntfConfSetMessage->getIfName ())) {
			string twoTuppleIfName;
			string threeTupple = pNsmLldpIntfConfSetMessage->getIfName ();
			NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			m->setIfName (twoTuppleIfName.c_str());
		} else {
			m->setIfName (pNsmLldpIntfConfSetMessage->getIfName());
		}
		
		NSM_PLUG_INFO_S(string("ifName:- ") + m->getIfName() + string("  ifType:- ") +  m->getIfType());

		m->setId (pNsmLldpIntfConfSetMessage->getId());
		m->setIfIndex (pNsmLldpIntfConfSetMessage->getIfIndex());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DISABLE))
            m->setDisable (pNsmLldpIntfConfSetMessage->getDisable());
        if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DCBXVERSION))
            m->setDcbxVersion (pNsmLldpIntfConfSetMessage->getDcbxVersion());
        if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_PROFILE))
            m->setProfile (pNsmLldpIntfConfSetMessage->getProfile());
        if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_ISCSIPRIORITY))
            m->setIscsiPriority (pNsmLldpIntfConfSetMessage->getIscsiPriority());
        
		status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);

        pNsmLldpIntfConfSetMessage->setCompletionStatus(m->getCompletionStatus());
        if( status == WAVE_MESSAGE_SUCCESS) {
            if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
        		NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")	
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(m->getCompletionStatus())); 
                //bpatel: To avoid memory leak
				status = pNsmLldpIntfConfSetMessage->getCompletionStatus();
				delete m;
				return status;
			}
			NSM_PLUG_INFO_S(string ("Response processing: CompletionStatus ")
                		    + string("returned success: ")  + 
                  FrameworkToolKit::localize(m->getCompletionStatus())); 
			int ClientStatus = m->getClientStatus();
			if (ClientStatus != WRC_WAVE_MESSAGE_SUCCESS) {
				NSM_PLUG_INFO_S(string ("Response processing: ClientStatus ")
                		    + string("returned error: ")  + 
                  FrameworkToolKit::localize(ClientStatus)); 
				pNsmLldpIntfConfSetMessage->setCompletionStatus(ClientStatus);
				//bpatel: To avoid memory leak
				delete m;
				return ClientStatus;
			}
        } else {
			NSM_PLUG_INFO_S(string ("Response processing: sendSynchronously")
                		    + string(" to Backend Failed ")); 
        }
		//bpatel: To avoid memory leak
		if(m){
			delete m;
		}
        return status;
    }

	LldpIntfConfLocalManagedObject*
    LldpLocalWorker::LldpIntfConfGetChildMoByParentMo(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        NSM_PLUG_INFO_S (string ("LldpIntfConfGetChildMoByParentMo: Entered...."));

        LldpIntfConfLocalManagedObject *pLldpIntfConfLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpIntfConfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (pPhyIntfLocalManagedObject->getObjectId(), "ownerManagedObjectId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
                pLldpIntfConfLocalManagedObject = NULL;
            } else {
                pLldpIntfConfLocalManagedObject = dynamic_cast<LldpIntfConfLocalManagedObject*>((*pResults)[0]);
            }
            pResults->clear ();
            delete pResults;
        }
        return pLldpIntfConfLocalManagedObject;
    }

	ResourceId LldpLocalWorker::updateLldpPhyInfMO(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
		NSM_PLUG_INFO_S(string("Entered"));
        NsmLldpIntfConfSetMessage *pNsmLldpIntfConfSetMessage = dynamic_cast<NsmLldpIntfConfSetMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        string ifName = pNsmLldpIntfConfSetMessage->getIfName();
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pDceSynchronousLinearSeqContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));

        WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> pLldpIntfConfLocalMOPtr = NULL;
        LldpIntfConfLocalManagedObject *pLldpIntfConfLocalManagedObject = NULL;

        if (pPhyIntfLocalManagedObject != NULL) {
        	pLldpIntfConfLocalMOPtr = pPhyIntfLocalManagedObject->getLldpIntfConfig();
        	pLldpIntfConfLocalManagedObject = pLldpIntfConfLocalMOPtr.operator ->();
        } else {
			NSM_PLUG_INFO_S(string("pPhyIntfLocalManagedObject is NULL"));
            status = WAVE_MESSAGE_ERROR;
		}

		if (pLldpIntfConfLocalManagedObject != NULL) {
			UI32 cmdcode = pNsmLldpIntfConfSetMessage->getCmdcode();
			status = WAVE_MESSAGE_SUCCESS;
			NSM_PLUG_INFO_S(string("Update LldpIntfConfLocalManagedObject"));

				if (pNsmLldpIntfConfSetMessage->getMsgType() == LLDP_CREATE_CONF) {
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DISABLE))
            			pLldpIntfConfLocalManagedObject->setDisable (pNsmLldpIntfConfSetMessage->getDisable());
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DCBXVERSION))
            			pLldpIntfConfLocalManagedObject->setDcbxVersion ((LldpDcbxVersion)pNsmLldpIntfConfSetMessage->getDcbxVersion());
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_PROFILE)) {
						LldpGlobalProfileGetObjectIdMessage *profileMsg = new LldpGlobalProfileGetObjectIdMessage();
					if (profileMsg) {
						profileMsg->setProfileName(pNsmLldpIntfConfSetMessage->getProfile());
						NSM_PLUG_INFO_S(string("sending LldpGlobalProfileGetObjectIdMessage to Lldp"));
						status = sendSynchronously (profileMsg);
						if(status == WAVE_MESSAGE_SUCCESS) {
							if(profileMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
						pLldpIntfConfLocalManagedObject->setProfileId (profileMsg->getProfileObjId());
								pNsmLldpIntfConfSetMessage->setMessageString(profileMsg->getMessageString());
							} else {
								NSM_PLUG_INFO_S(string("Response processing: CompletionStatus")	
									+ string("returned error: ")  + 
									FrameworkToolKit::localize(profileMsg->getCompletionStatus()));
							}
							status = profileMsg->getCompletionStatus();
						}
						delete profileMsg;
					}
					}
                    if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_ISCSIPRIORITY))
                        pLldpIntfConfLocalManagedObject->setIscsiPriority ((UI32)pNsmLldpIntfConfSetMessage->getIscsiPriority());
					if(IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_CEE_ON_OFF))
						 pLldpIntfConfLocalManagedObject->setLldpCeeStatus(pNsmLldpIntfConfSetMessage->getLldpCeeStatus());

				}else if (pNsmLldpIntfConfSetMessage->getMsgType() == LLDP_DELETE_CONF) {
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DISABLE))
            			pLldpIntfConfLocalManagedObject->setDisable (pNsmLldpIntfConfSetMessage->getDisable());
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_DCBXVERSION))
            			pLldpIntfConfLocalManagedObject->setDcbxVersion ((LldpDcbxVersion)pNsmLldpIntfConfSetMessage->getDcbxVersion());
        			if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_PROFILE)) {
						pLldpIntfConfLocalManagedObject->setProfileId (ObjectId::NullObjectId);
					}
                    if (IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_ISCSIPRIORITY))
                        pLldpIntfConfLocalManagedObject->setIscsiPriority ((UI32)pNsmLldpIntfConfSetMessage->getIscsiPriority());
					if(IS_CMDCODE_SET(cmdcode, LLDP_INTF_CONF_CEE_ON_OFF))
						pLldpIntfConfLocalManagedObject->setLldpCeeStatus(pNsmLldpIntfConfSetMessage->getLldpCeeStatus());
				}

				updateWaveManagedObject (pLldpIntfConfLocalManagedObject);
		}

		NSM_PLUG_INFO_S(string("Done"));
        return status;
	}
	
	ResourceId LldpLocalWorker::validateLldpPhyInfConf (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
	{
        NsmLldpIntfConfSetMessage *pNsmLldpIntfConfSetMessage = 
				dynamic_cast<NsmLldpIntfConfSetMessage *> (pDceSynchronousLinearSeqContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        NSM_PLUG_INFO_S(string ("Entered"));

		/* Validate 1: Check if port mode is ISL. 
		 * If yes block the LLDP configuration
		 */

		string ifName = pNsmLldpIntfConfSetMessage->getIfName();
		NSM_PLUG_INFO_S(string("NsmLldpIntfConfSetMessage:ifName: ")  + ifName);
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<string> selectFields;
        selectFields.push_back("Portrole");
        selectFields.push_back("lldpIntfConfig");
        syncQueryCtxt.addSelectFields(selectFields);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults != NULL && pResults->size () == 1) {
        	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
            if (pPhyIntfLocalManagedObject) {
                //Store in cache to get used in subsequent steps
                pDceSynchronousLinearSeqContext->getCache()->put(PhyIntfLocalManagedObject::getClassName(), pPhyIntfLocalManagedObject);
                pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
                if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole())) {
                    status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                }
            }
            pResults->clear();
            delete (pResults);
		} else {
			status = WRC_NSM_INTERFACE_DOES_NOT_EXIST;
		}
			
		NSM_PLUG_INFO_S(string("returned status: ")  + FrameworkToolKit::localize(status)); 

		return (status);
	}


    void  LldpLocalWorker::NsmLldpIntfConfSetMessageHandler( NsmLldpIntfConfSetMessage *pNsmLldpIntfConfSetMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::validateLldpPhyInfConf),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::updateLldpPhyInfConf),
            
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::updateLldpPhyInfMO),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LldpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmLldpIntfConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute();
    }
    
	void LldpLocalWorker::NsmLldpInternalMessageHandler(NsmLldpInternalMessage *pNsmLldpInternalMessage)
	{
        NSM_PLUG_INFO_S(string ("Entered"));
		int status = WAVE_MESSAGE_SUCCESS;
		UI32 cmdcode = pNsmLldpInternalMessage->getCmdcode();

		/* Remove lldp profile from interface */
		if (cmdcode == LLDP_NSM_INTERNAL_DEL_PROFILE) { 
			string profileName = pNsmLldpInternalMessage->getProfile();
			LldpIntfConfLocalManagedObject *pLldpIntfConfLocalMO = NULL;
			LldpGlobalProfileConfManagedObject *pLldpGlobalProfileConfMO = NULL;
			UI32 i;
			LocationId locationId = 0;
			locationId = FrameworkToolKit::getThisLocationId ();
			ObjectId profileId = ObjectId::NullObjectId;

			NSM_PLUG_INFO_S(string ("Location Id ") + locationId);

			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute (new AttributeString(&profileName, "profileName"));
			syncQueryCtxt.addSelectField("objectId");
			vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

			if (pResults != NULL && pResults->size () > 0) {
				pLldpGlobalProfileConfMO = dynamic_cast<LldpGlobalProfileConfManagedObject *>((*pResults)[0]);
				if (pLldpGlobalProfileConfMO) {
					profileId = pLldpGlobalProfileConfMO->getObjectId();
					//get all lldpIntfConfMO which are associated with given profile name
					WaveManagedObjectSynchronousQueryContext lldpIntfConfSyncQryCtxt(LldpIntfConfLocalManagedObject::getClassName ());
					lldpIntfConfSyncQryCtxt.setLocationIdFilter(locationId);
					lldpIntfConfSyncQryCtxt.addAndAttribute(new AttributeObjectId(&profileId, "profileId"));
					vector<WaveManagedObject *> *pLldpIntfConfResults = querySynchronously (&lldpIntfConfSyncQryCtxt);
					if (NULL != pLldpIntfConfResults && pLldpIntfConfResults->size() > 0) {
						startTransaction();
						for (i = 0; i < pLldpIntfConfResults->size(); i++) {
							pLldpIntfConfLocalMO = dynamic_cast<LldpIntfConfLocalManagedObject *>((*pLldpIntfConfResults)[i]);
							pLldpIntfConfLocalMO->addAttributeToBeUpdated("profileId");
							pLldpIntfConfLocalMO->setProfileId(ObjectId::NullObjectId);
							updateWaveManagedObject(pLldpIntfConfLocalMO);
						}
						commitTransaction();
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pLldpIntfConfResults);
					} else {
						NSM_PLUG_INFO_S(string ("Profile is not asssociated with any interface"));
					}
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				NSM_PLUG_INFO_S(string ("Profile: ") + profileName + string(" does not exists"));
                status = WAVE_MESSAGE_ERROR;
			}
		/* Set Interface level lldp configuration to default */
		} else if (cmdcode == LLDP_NSM_INTERNAL_NO_LLDP) {
			LldpIntfConfLocalManagedObject *pLldpIntfConfLocalMO = NULL;
			UI32 i;

			LocationId locationId = 0;
			locationId = FrameworkToolKit::getThisLocationId ();
			vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, LldpIntfConfLocalManagedObject::getClassName ());

			if (NULL != pResults) {
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					NSM_PLUG_INFO_S(string ("No Lldp config is asssociated with any interface"));
				}else if (0 < numberOfResults) {
					//ObjectId profileObjId = ObjectId::NullObjectId;
					startTransaction();
					for(i=0; i<numberOfResults; i++) {
				    	pLldpIntfConfLocalMO = dynamic_cast<LldpIntfConfLocalManagedObject *>((*pResults)[i]);
						pLldpIntfConfLocalMO->addAttributeToBeUpdated("profileId");
						pLldpIntfConfLocalMO->addAttributeToBeUpdated("disable");
						pLldpIntfConfLocalMO->addAttributeToBeUpdated("iscsiPriority");

						pLldpIntfConfLocalMO->setProfileId(ObjectId::NullObjectId);
						pLldpIntfConfLocalMO->setDisable(false);
						pLldpIntfConfLocalMO->setIscsiPriority(DEFAULT_ISCSI_PRIORITY);
	            		updateWaveManagedObject (pLldpIntfConfLocalMO);
					}
					commitTransaction();
					NSM_PLUG_INFO_S(string ("Successfully resetted LldpIntfMO"));
				} else {
					NSM_PLUG_INFO_S(string ("numberOfResults:- ") + numberOfResults);
					status = WAVE_MESSAGE_ERROR;
				}
                //Sanjeev: Cleanup the query results.
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				NSM_PLUG_INFO_S(string ("Results == NULL"));
				status = WAVE_MESSAGE_ERROR;
			}

		}
		
		pNsmLldpInternalMessage->setCompletionStatus (status);
		reply(pNsmLldpInternalMessage);
	}
}
