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
 *   Author : abhaskar                                                     *
 **************************************************************************/
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCStatus.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/Igmps/IgmpsVlanGlobalConfWorker.h"
#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsVlanConfigMessage.h"
#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsSgVlanConfigMessage.h"
#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsMrtrVlanConfigMessage.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsSgVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsMrtrVlanConfigManagedObject.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsVlanLocalConfigMessage.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsMrtrVlanLocalConfigMessage.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsSgVlanLocalConfigMessage.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "Nsm/Common/PVlanUtils.h"

namespace DcmNs
{

    IgmpsVlanGlobalConfWorker::IgmpsVlanGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        IgmpsVlanIntfConfManagedObject    IgmpsVlanIntfConfManagedObject    (pNsmGlobalObjectManager);
        IgmpsSgVlanConfigManagedObject    IgmpsSgVlanConfigManagedObject    (pNsmGlobalObjectManager);
        IgmpsMrtrVlanConfigManagedObject    IgmpsMrtrVlanConfigManagedObject    (pNsmGlobalObjectManager);
        IgmpsVlanIntfConfManagedObject.setupOrm ();
        addManagedClass (IgmpsVlanIntfConfManagedObject::getClassName (), this);
        IgmpsSgVlanConfigManagedObject.setupOrm ();
        addManagedClass (IgmpsSgVlanConfigManagedObject::getClassName (), this);
        IgmpsMrtrVlanConfigManagedObject.setupOrm ();
        addManagedClass (IgmpsMrtrVlanConfigManagedObject::getClassName (), this);
        addOperationMap (NSMGLOBALUPDATEIGMPSVLANCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsVlanConfigMessageHandler));
        addOperationMap (NSMGLOBALUPDATEIGMPSSGVLANCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsSgVlanConfigMessageHandler));
        addOperationMap (NSMGLOBALUPDATEIGMPSMRTRVLANCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsMrtrVlanConfigMessageHandler));
    }

    IgmpsVlanGlobalConfWorker::~IgmpsVlanGlobalConfWorker ()
    {
    }

    PrismMessage  *IgmpsVlanGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALUPDATEIGMPSVLANCONFIG :
                pPrismMessage = new NsmGlobalUpdateIgmpsVlanConfigMessage ();
                break;
            case NSMGLOBALUPDATEIGMPSSGVLANCONFIG :
                pPrismMessage = new NsmGlobalUpdateIgmpsSgVlanConfigMessage ();
                break;
            case NSMGLOBALUPDATEIGMPSMRTRVLANCONFIG :
                pPrismMessage = new NsmGlobalUpdateIgmpsMrtrVlanConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IgmpsVlanGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IgmpsVlanIntfConfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IgmpsVlanIntfConfManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IgmpsSgVlanConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IgmpsSgVlanConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IgmpsMrtrVlanConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IgmpsMrtrVlanConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IgmpsVlanGlobalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void
    IgmpsVlanGlobalConfWorker::IgmpsVlanConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace (TRACE_LEVEL_DEVEL, "IgmpsVlanConfigSendToClusterCallback::Entering");
        ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
        PrismLinearSequencerContext *pPrismLinearSequencerContext =
            reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());

        if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message completely/partially succeeded on all locations");
        else 
			trace (TRACE_LEVEL_ERROR, string("getCompletionStatus failed with error : ") + sendToClusterCompletionStatus);

        ResourceId         returnStatus = WAVE_MESSAGE_SUCCESS;

		/*
		 * Sudhanshu :Changes for partial success(Hercules) 
		 */
		 
        vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        for (UI32 i = 0; i < locations.size(); i++) 
		{
            returnStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locations[i]); 	
			trace (TRACE_LEVEL_DEBUG, string("IgmpsVlanConfigSendToClusterCallback::getCompletionStatus: ") + returnStatus
					  + (" for location: ") + locations[i]);
        }

		returnStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
		delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
		delete pWaveSendToClusterContext->getPPrismMessageForPhase2();
		delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_DEVEL, "IgmpsVlanConfigSendToClusterCallback::Done");

        pPrismLinearSequencerContext->executeNextStep(returnStatus);
        return;
    }

    VlanIntfManagedObject *
    IgmpsVlanGlobalConfWorker::getVlanIntfMOForVid (UI32 vlanId)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId, "id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (1 == numberOfResults) {
                pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
            } else {
                pVlanIntfManagedObject = NULL;
            }
            pResults->clear ();
            delete pResults;
        }
        return pVlanIntfManagedObject;
    }

	void IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsMrtrVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

		NsmUpdateIgmpsMrtrVlanLocalConfigMessage *m = new NsmUpdateIgmpsMrtrVlanLocalConfigMessage();
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep: Entered....");

		m->setIfType(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getIfType());
		m->setIfName(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getIfName());
		m->setVlanId(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getVlanId());
		m->setVlanIfName(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getVlanIfName());
		m->setVlanIfIndex(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getVlanIfIndex());
		m->setNegation(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getNegation());

        vector<UI32>    locations;
        LocationId      locationId;
        string			IfName = m->getIfName();

        trace (TRACE_LEVEL_DEBUG, string("The value of IfName is") + m->getIfName() + ("The value of IfType is")
								  + m->getIfType());
        
		/*
		 * Sudhanshu: Defect fix: If the interface type is fortygigabit/tengigabit/gigabit, then 2/3 touple makes sense. For e.g.
		              If the interface is port-channel, then we cannot know the location-Id. Also this being
					  a global command, it can be sent to all the rbridges in the cluster in case it is not tengigbit.
		 */
		if ((IGMPS_IF_TYPE_FORTYGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_TENGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_GIGABIT == m->getIfType()))
		{
			if(true == NsmUtils::isValidThreeTuple(m->getIfName())) 
			{
				SI32 mappedId = NsmUtils::getMappedIdFromIfName(m->getIfName());
				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

				string twoTuppleIfName;
				NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
				m->setIfName(twoTuppleIfName);
			} 
			else 
			{
				locationId = NsmUtils::getLocalLocationId();
			}

            locations.push_back(locationId);
        	trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep:locationId=" + locationId);
			if((locationId != NsmUtils::getLocalLocationId()) 
			   && (false == FrameworkToolKit::isAConnectedLocation(locationId)))
			{
        		trace (TRACE_LEVEL_DEBUG, string("IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep:locationId=")
				       + locationId + "has failed over.");
			    if(false == pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getNegation())
				{
					pPrismLinearSequencerContext->executeNextStep(WRC_IGMP_ERR_INVALID_COMMAND);
				}
				else
				{
					pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
				}
				return;
			}

			if(WAVE_MESSAGE_SUCCESS == checkIntfBelongsToPortCh(IfName))
			{
				pPrismLinearSequencerContext->executeNextStep(WRC_IGMP_ERR_NO_VALID_CONFIG);
				return;
			}
        }

		/* create clustercontext and initialize with callback func */
        WaveSendToClusterContext    *pWaveSendToClusterContext = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IgmpsVlanGlobalConfWorker::IgmpsVlanConfigSendToClusterCallback),
            pPrismLinearSequencerContext);

        if ((IGMPS_IF_TYPE_FORTYGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_TENGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_GIGABIT == m->getIfType())) {
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
        }

        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		
		/* Sudhanshu start: Changes for partial success(Hercules) */
		bool partialFlag = true;
	 	pWaveSendToClusterContext->setPartialSuccessFlag(partialFlag);
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(partialFlag);

		/* Sudhanshu end: Changes for partial success(Hercules) */
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep:sendToWaveCluster");
		/* Send the message to all nodes in the cluster */
        sendToWaveCluster (pWaveSendToClusterContext);
		return;
	}

	void IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigMOStep: Entered....");
        UI32 numberOfResults = 0;
		IgmpsMrtrVlanConfigManagedObject *pIgmpsMrtrVlanConfigManagedObject = NULL;
		NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage =
            dynamic_cast<NsmGlobalUpdateIgmpsMrtrVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string ifName = pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getIfName();

        bool negation = pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getNegation();
        UI32 vlanId = pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getVlanId();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(VlanIntfManagedObject::getClassName());
        syncQueryCtxt1.addSelectFieldForObjectId("objectId");
        syncQueryCtxt1.addAndAttribute (new AttributeUI32(&vlanId, "id"));
        vector<WaveManagedObject *> *pResultsVlanMO = querySynchronously (&syncQueryCtxt1);
		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

   		
		if(NULL != pResultsVlanMO) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsVlanMO)[0]);

			if (negation) {
    	            trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigMOStep: Negation");
        	        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsMrtrVlanConfigManagedObject::getClassName());
	
    	            syncQueryCtxt.addSelectFieldForObjectId("objectId");
        	        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifname"));
            	    syncQueryCtxt.addAndAttribute (new AttributeObjectId(pVlanIntfManagedObject->getObjectId(), "ownerManagedObjectId"));
                	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	
    	            if (NULL != pResults) {
        	            numberOfResults = pResults->size ();
            	        if (numberOfResults == 0) {
                	        trace (TRACE_LEVEL_DEBUG, string("Can't find the MO"));
                    	} else if (1 == numberOfResults) {
	                        pIgmpsMrtrVlanConfigManagedObject= dynamic_cast<IgmpsMrtrVlanConfigManagedObject *>((*pResults)[0]);
    	                    deleteFromComposition(VlanIntfManagedObject::getClassName(), IgmpsMrtrVlanConfigManagedObject::getClassName(),
        	                    "igmpsMrtrVlanConfig", pVlanIntfManagedObject->getObjectId(), pIgmpsMrtrVlanConfigManagedObject->getObjectId());
            	        }
                	    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    	delete pResults;
	                }
    	            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsVlanMO);
        	        delete pResultsVlanMO;
	        } else {	
    	        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigMOStep: Create");
	
        	        numberOfResults = pResultsVlanMO->size();
            	    if(numberOfResults == 0) {
                	    trace (TRACE_LEVEL_DEBUG, string("Can't find the VLAN MO"));
	                } else if (1 == numberOfResults) {
        	            pIgmpsMrtrVlanConfigManagedObject = new IgmpsMrtrVlanConfigManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
            	        prismAssert (NULL != pIgmpsMrtrVlanConfigManagedObject, __FILE__, __LINE__);
	
    	                pIgmpsMrtrVlanConfigManagedObject->setIfType(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getIfType());
        	            pIgmpsMrtrVlanConfigManagedObject->setIfName(pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getIfName());
            	        pIgmpsMrtrVlanConfigManagedObject->setOwnerManagedObjectId(pVlanIntfManagedObject->getObjectId());
                	    addToComposition(VlanIntfManagedObject::getClassName(), IgmpsMrtrVlanConfigManagedObject::getClassName(),
                    	    "igmpsMrtrVlanConfig", pVlanIntfManagedObject->getObjectId(), pIgmpsMrtrVlanConfigManagedObject->getObjectId());

	                    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pIgmpsMrtrVlanConfigManagedObject);
    	            }
        	        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsVlanMO);
            	    delete pResultsVlanMO;
        	}
		}     

        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
		return;
	}

    void  IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsMrtrVlanConfigMessageHandler( NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::validateIgmpsMrtrVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsMrtrVlanConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = 
            new PrismLinearSequencerContext (
                pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage, this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	void IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NsmGlobalUpdateIgmpsSgVlanConfigMessage *pNsmGlobalUpdateIgmpsSgVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsSgVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

		NsmUpdateIgmpsSgVlanLocalConfigMessage *m = new NsmUpdateIgmpsSgVlanLocalConfigMessage();
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep: Entered....");

		m->setGroupAddress(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getGroupAddress());
		m->setInterface(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getInterface());
		m->setIfType(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfType());
		m->setIfName(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfName());
		m->setVlanId(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getVlanId());
		m->setVlanIfName(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getVlanIfName());
		m->setVlanIfIndex(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getVlanIfIndex());
		m->setNegation(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getNegation());

        vector<UI32>    locations;
        LocationId      locationId;
		string			IfName = m->getIfName();

		/*
		 * Sudhanshu: Defect fix: If the interface type is fortygigabit/tengigabit/gigabit, then 2/3 touple makes sense. For e.g.
		              If the interface is port-channel, then we cannot know the location-Id. Also this being
					  a global command, it can be sent to all the rbridges in the cluster in case it is not tengigbit.
		 */
		if ((IGMPS_IF_TYPE_FORTYGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_TENGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_GIGABIT == m->getIfType()))
		{
			if(true == NsmUtils::isValidThreeTuple(m->getIfName())) 
			{
				SI32 mappedId = NsmUtils::getMappedIdFromIfName(m->getIfName());
				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

				string twoTuppleIfName;
				NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
				m->setIfName(twoTuppleIfName);
			} 
			else 
			{
				locationId = NsmUtils::getLocalLocationId();
			}
            locations.push_back(locationId); 

        	trace (TRACE_LEVEL_DEBUG, string("IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep:locationId=") 
			       + locationId);
			if((locationId != NsmUtils::getLocalLocationId()) 
			   && (false == FrameworkToolKit::isAConnectedLocation(locationId)))
			{
        		trace (TRACE_LEVEL_DEBUG, string("IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep:locationId=") 
				       + locationId + "has failed over.");
			    if(false == pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getNegation())
				{
					pPrismLinearSequencerContext->executeNextStep(WRC_IGMP_ERR_INVALID_COMMAND);
				}
				else
				{
					pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
				}
				return;
			}

			if(WAVE_MESSAGE_SUCCESS == checkIntfBelongsToPortCh(IfName))
			{
				pPrismLinearSequencerContext->executeNextStep(WRC_IGMP_ERR_NO_VALID_CONFIG);
				return;
			}
        }
        
		/* create clustercontext and initialize with callback func */
        WaveSendToClusterContext    *pWaveSendToClusterContext = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IgmpsVlanGlobalConfWorker::IgmpsVlanConfigSendToClusterCallback),
            pPrismLinearSequencerContext);
		
        if ((IGMPS_IF_TYPE_FORTYGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_TENGIGABIT == m->getIfType()) || (IGMPS_IF_TYPE_GIGABIT == m->getIfType())) {
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
        }

        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);

        /* Sudhanshu start: Changes for partial success(Hercules) */
        bool partialFlag = true;
        pWaveSendToClusterContext->setPartialSuccessFlag(partialFlag);
        pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(partialFlag);

        /* Sudhanshu end: Changes for partial success(Hercules) */

        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep:SendtoCluster");
        /* Send the message to all nodes in the cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

		return;
	}

	void IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigMOStep: Entered....");
        UI32 numberOfResults = 0;
		IgmpsSgVlanConfigManagedObject *pIgmpsSgVlanConfigManagedObject = NULL;
		NsmGlobalUpdateIgmpsSgVlanConfigMessage *pNsmGlobalUpdateIgmpsSgVlanConfigMessage =
            dynamic_cast<NsmGlobalUpdateIgmpsSgVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string ifName = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfName();
		UI32 ifType = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfType();
		IpV4Address groupAddress = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getGroupAddress();
        UI32 vlanId = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getVlanId();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(VlanIntfManagedObject::getClassName());
        syncQueryCtxt1.addSelectFieldForObjectId("objectId");
        syncQueryCtxt1.addAndAttribute (new AttributeUI32(&vlanId, "id"));
        vector<WaveManagedObject *> *pResultsVlanMO = querySynchronously (&syncQueryCtxt1);
        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

		bool negation = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getNegation();
		if(NULL != pResultsVlanMO) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsVlanMO)[0]);
	        if (negation) {
    	        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsSgVlanConfigManagedObject::getClassName());
	
    	        syncQueryCtxt.addSelectFieldForObjectId("objectId");
            	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifname"));
	            syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifType, "iftype"));
    	        syncQueryCtxt.addAndAttribute (new AttributeIpV4Address(&groupAddress, "groupaddress"));
				syncQueryCtxt.addAndAttribute (new AttributeObjectId(pVlanIntfManagedObject->getObjectId(), "ownerManagedObjectId"));
        	    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	
    	        if (NULL != pResults) {
        	        numberOfResults = pResults->size ();
            	    if (numberOfResults == 0) {
                	    trace (TRACE_LEVEL_DEBUG, string("Can't find the MO"));
	                } else if (1 == numberOfResults) {
    	                pIgmpsSgVlanConfigManagedObject = dynamic_cast<IgmpsSgVlanConfigManagedObject *>((*pResults)[0]);
        	            deleteFromComposition(VlanIntfManagedObject::getClassName(), IgmpsSgVlanConfigManagedObject::getClassName(),
            	            "igmpsSgVlanConfig", pVlanIntfManagedObject->getObjectId(), pIgmpsSgVlanConfigManagedObject->getObjectId());
                	}
	                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
    	            delete pResults;
        	    }
	        } else {
    	        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigMOStep: Creation");
	
        	        numberOfResults = pResultsVlanMO->size();
            	    if(numberOfResults == 0) {
                	    trace (TRACE_LEVEL_DEBUG, string("Can't find the VLAN MO"));
	                } else if (1 == numberOfResults) {
    	                trace (TRACE_LEVEL_DEBUG, string("Creating Static Group"));
            	        pIgmpsSgVlanConfigManagedObject = new IgmpsSgVlanConfigManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
                	    prismAssert (NULL != pIgmpsSgVlanConfigManagedObject, __FILE__, __LINE__);
                    	pIgmpsSgVlanConfigManagedObject->setGroupAddress(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getGroupAddress());
	                    pIgmpsSgVlanConfigManagedObject->setInterface(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getInterface());
    	                pIgmpsSgVlanConfigManagedObject->setIfType(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfType());
        	            pIgmpsSgVlanConfigManagedObject->setIfName(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfName());
            	        pIgmpsSgVlanConfigManagedObject->setOwnerManagedObjectId(pVlanIntfManagedObject->getObjectId());
                	    addToComposition(VlanIntfManagedObject::getClassName(), IgmpsSgVlanConfigManagedObject::getClassName(),
                    	    "igmpsSgVlanConfig", pVlanIntfManagedObject->getObjectId(), pIgmpsSgVlanConfigManagedObject->getObjectId());
						pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pIgmpsSgVlanConfigManagedObject);
    	            }	
        	}
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsVlanMO);
			delete pResultsVlanMO;
		}

        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
		return;
	}

    void  IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsSgVlanConfigMessageHandler( NsmGlobalUpdateIgmpsSgVlanConfigMessage *pNsmGlobalUpdateIgmpsSgVlanConfigMessage)
    {
// Validations and Queries should go here
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsSgVlanConfigMessageHandler: Entered....");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::validateIgmpsSgVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalUpdateIgmpsSgVlanConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	void IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        UI32 cmdcode = pNsmGlobalUpdateIgmpsVlanConfigMessage->getOpCode();
        if (IS_CMDCODE_SET(cmdcode, IGMPS_GLOBAL_SNOOP_ENABLE)) {
            /* global snooping enable cli */
            pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
            return;
        }

		NsmUpdateIgmpsVlanLocalConfigMessage *m = new NsmUpdateIgmpsVlanLocalConfigMessage();
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigStep: Entered....");

		m->setLmqi(pNsmGlobalUpdateIgmpsVlanConfigMessage->getLmqi());
		m->setQmrt(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQmrt());
		m->setEnable(pNsmGlobalUpdateIgmpsVlanConfigMessage->getEnable());
		m->setFastLeave(pNsmGlobalUpdateIgmpsVlanConfigMessage->getFastLeave());
		m->setQueryInterval(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQueryInterval());
		m->setQuerierEnable(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQuerierEnable());
		m->setMrouterTimeout(pNsmGlobalUpdateIgmpsVlanConfigMessage->getMrouterTimeout());
		m->setVlanId(pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanId());
		m->setVlanIfName(pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanIfName());
		m->setVlanIfIndex(pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanIfIndex());
		m->setOpCode(pNsmGlobalUpdateIgmpsVlanConfigMessage->getOpCode());

        /* create clustercontext and initialize with callback func */
        WaveSendToClusterContext    *pWaveSendToClusterContext = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IgmpsVlanGlobalConfWorker::IgmpsVlanConfigSendToClusterCallback),
            pPrismLinearSequencerContext);
        
        
        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
        
		/* Sudhanshu start: Changes for partial success(Hercules) */
        bool partialFlag = true;
        pWaveSendToClusterContext->setPartialSuccessFlag(partialFlag);
        pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(partialFlag);

        /* Sudhanshu end: Changes for partial success(Hercules) */
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigStep:SendtoCluster");
		/* Send the message to all nodes in the cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

		return;
	}

    IgmpsVlanIntfConfManagedObject*
    IgmpsVlanGlobalConfWorker::IgmpsVlanGetChildMoByParentMo(VlanIntfManagedObject *pVlanIntfManagedObject)
    {
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::IgmpsVlanGetChildMoByParentMo: Entered....");

        IgmpsVlanIntfConfManagedObject *pIgmpsVlanIntfConfManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (pVlanIntfManagedObject->getObjectId(), "ownerManagedObjectId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
                pIgmpsVlanIntfConfManagedObject = NULL;
            } else {
                pIgmpsVlanIntfConfManagedObject = dynamic_cast<IgmpsVlanIntfConfManagedObject*>((*pResults)[0]);
            }
            pResults->clear ();
            delete pResults;
        }
        return pIgmpsVlanIntfConfManagedObject;
    }

	void IgmpsVlanGlobalConfWorker::validateVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::validateVlanConfigStep: Entered....");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        UI32 vlanId = pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanId();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
		querySynchronouslyForCount (&syncQueryCtxt, count);
		if (count == 0 || count > 1) {
			trace (TRACE_LEVEL_ERROR, "IgmpsVlanGlobalConfWorker::validateVlanConfigStep: IGMPS VlanMO not found");
			status =  WAVE_MESSAGE_ERROR;
		} else if (PVlanUtils::isSecondaryVlan(vlanId)) {
			status = WRC_NSM_CANNOT_CONFIGURE_IGMPS_ON_SEC_PVLAN;
		}
		else if (NsmUtils::isRemoteSpanVlan(vlanId)) {
			status = WRC_SPAN_CANNOT_CONFIGURE_IGMPS_ON_RSPAN_VLAN;
		}

        pPrismLinearSequencerContext->executeNextStep(status);
        return;
    }

	void IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigMOStep: Entered....");

        IgmpsVlanIntfConfManagedObject *pIgmpsVlanConfigManagedObject = NULL;

		NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage =
            dynamic_cast<NsmGlobalUpdateIgmpsVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        UI32 cmdcode = pNsmGlobalUpdateIgmpsVlanConfigMessage->getOpCode();
        
        if (IS_CMDCODE_SET(cmdcode, IGMPS_GLOBAL_SNOOP_ENABLE)) {
            /* global snooping enable cli */
            /* Update all Igmps Vlan MO's */
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size ();

                for (UI32 i = 0; i < numberOfResults; i++) {
                    IgmpsVlanIntfConfManagedObject *pIgmpsVlanMO = dynamic_cast<IgmpsVlanIntfConfManagedObject*>((*pResults)[i]);
                    pIgmpsVlanMO->addAttributeToBeUpdated("enable");
                    pIgmpsVlanMO->setEnable(pNsmGlobalUpdateIgmpsVlanConfigMessage->getEnable());
                    updateWaveManagedObject (pIgmpsVlanMO);
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
            }

            pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
            return;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32 (pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanId(), "vlanId"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (numberOfResults == 0 || numberOfResults > 1) {
				trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
				pIgmpsVlanConfigManagedObject = NULL;
			} else {
				pIgmpsVlanConfigManagedObject = dynamic_cast<IgmpsVlanIntfConfManagedObject*>((*pResults)[0]);
			}
		}

        prismAssert (NULL != pIgmpsVlanConfigManagedObject, __FILE__, __LINE__);

        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_SNOOP_ENABLE)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("enable");
            pIgmpsVlanConfigManagedObject->setEnable(pNsmGlobalUpdateIgmpsVlanConfigMessage->getEnable());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_QUERIER_ENABLE)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("querierEnable");
            pIgmpsVlanConfigManagedObject->setQuerierEnable(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQuerierEnable());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_FAST_LEAVE)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("fastLeave");
            pIgmpsVlanConfigManagedObject->setFastLeave(pNsmGlobalUpdateIgmpsVlanConfigMessage->getFastLeave());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_MROUTER_TIMEOUT)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("mrouterTimeout");
            pIgmpsVlanConfigManagedObject->setMrouterTimeout(pNsmGlobalUpdateIgmpsVlanConfigMessage->getMrouterTimeout());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_LMQI)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("lmqi");
            pIgmpsVlanConfigManagedObject->setLmqi(pNsmGlobalUpdateIgmpsVlanConfigMessage->getLmqi());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_QMRT)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("qmrt");
            pIgmpsVlanConfigManagedObject->setQmrt(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQmrt());
        }
        if (IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_QUERY_INTERVAL)) {
        	pIgmpsVlanConfigManagedObject->addAttributeToBeUpdated("queryInterval");
            pIgmpsVlanConfigManagedObject->setQueryInterval(pNsmGlobalUpdateIgmpsVlanConfigMessage->getQueryInterval());
        }

        updateWaveManagedObject(pIgmpsVlanConfigManagedObject);

        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
		return;
	}

    void  IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsVlanConfigMessageHandler( NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage)
    {
        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::NsmGlobalUpdateIgmpsVlanConfigMessageHandler: Entered....");
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::validateVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::validateIgmpsVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::updateIgmpsVlanConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsVlanGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalUpdateIgmpsVlanConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
     	pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	/*
	 * Sudhanshu: RBridge Failover (Hercules MC)
	 * Description: This function will delete all mrouter port MO and static group port MO for fortygigabit/tengigabit/gigabit interfaces for a given 
	 *              location Id, since that location is undergoing controlled rebooting(e.g. command 'no vcs enable' on that rbridge).
	 * Note        : startTransaction() and commitTransaction() is already taken care by caller of this function.
	 */
	ResourceId IgmpsVlanGlobalConfWorker::deleteIgmpsTenGigIntfMOForLocation (const vector<UI32> uiLocationIds)
	{
                 vector<string>rbridgeId;
          
                 for(UI32 i=0;i<uiLocationIds.size();i++) {
                     string rbrId = getString(DcmToolKit::getMappedIdFromLocationId(uiLocationIds[i]));     
                     rbrId.append("/%");
                     trace(TRACE_LEVEL_DEVEL,string("formed rbridge: ") +rbrId);
                     rbridgeId.push_back(rbrId);
                     rbrId.clear();
                 }
                 WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion(IgmpsSgVlanConfigManagedObject::getClassName());
                 for(UI32 j=0;j<rbridgeId.size();j++) {
                     deleteCtxt->addOrAttribute (new AttributeString (rbridgeId[j], "ifName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
                 }
                 deletePartialManagedObjectCompositions(VlanIntfManagedObject::getClassName(),"igmpsSgVlanConfig",deleteCtxt);
                 
                 WaveManagedObjectSynchronousQueryContextForDeletion *deleteMrtrCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion(IgmpsMrtrVlanConfigManagedObject::getClassName());
                 for(UI32 j=0;j<rbridgeId.size();j++) {
                     deleteMrtrCtxt->addOrAttribute (new AttributeString (rbridgeId[j], "ifName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
                 }
                 deletePartialManagedObjectCompositions(VlanIntfManagedObject::getClassName(),"igmpsMrtrVlanConfig",deleteMrtrCtxt);

	        trace (TRACE_LEVEL_DEBUG, "IgmpsVlanGlobalConfWorker::deleteTenGigIntfMOForLocation:Exiting...........");

		return WAVE_MESSAGE_SUCCESS;
		
	} /* end function */

	/*
	 * Sudhanshu:This function will return success if the input interface belongs to a port-channel. 
	 *           Defect fix: If the Fortygigabit/Tengigabit/Gigabit interface is associated to a port-channel, then return error
	 *            since this is an invalid configuration for mrouter and static group commands.
	 */

    ResourceId IgmpsVlanGlobalConfWorker::checkIntfBelongsToPortCh(string ifName)
    {
        ResourceId  				status       					= WAVE_MESSAGE_ERROR;
		PhyIntfLocalManagedObject 	*pPhyIntfLocalManagedObject 	= NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectFieldForObjectId("poId");
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"id"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() == 1) 
		{
            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
            if (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getPoId())
			{
				status = WAVE_MESSAGE_SUCCESS; 
		    }
        }

        if(pResults)
        {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return status;
    }
 
 	/*
	 * Sudhanshu: This function will validate the Vlan view related IGMP commands. 
	 			  All the validations for IGMP command in vlan view must go here.
	 */
    void IgmpsVlanGlobalConfWorker::validateIgmpsVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		/* Validation 1: If query is enabled, then there should not be mrouter configurations */
		ResourceId status = WAVE_MESSAGE_SUCCESS; /* By default, the status should be success */
 
		NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        UI32 cmdcode = pNsmGlobalUpdateIgmpsVlanConfigMessage->getOpCode();

        if ((IS_CMDCODE_SET(cmdcode, IGMPS_VLAN_QUERIER_ENABLE)) 
		    && (false == pNsmGlobalUpdateIgmpsVlanConfigMessage->getNegation())) 
		{
			if (WAVE_MESSAGE_SUCCESS == checkmRouterConfPresent(pNsmGlobalUpdateIgmpsVlanConfigMessage))
			{
				status = WRC_IGMP_ERR_NO_VALID_CONFIG;
				goto EXIT;
			}

        }
		
		/* Validation 2:......... */

		EXIT:
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
   }
	
	/*
	 * Sudhanshu:This function will return success if the vlanMO corresponding to IGMPS has an mrouter MO. 
	 *           Defect fix: If the querier command is given on IGMPS in vlan view, then no static mrouter
	 *           should be configured on that vlan.
	 */
    ResourceId IgmpsVlanGlobalConfWorker::checkmRouterConfPresent(NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage)
    {
        ResourceId  				status       					= WAVE_MESSAGE_ERROR;

        VlanIntfManagedObject *pVlanIntfManagedObject = 
								getVlanIntfMOForVid (pNsmGlobalUpdateIgmpsVlanConfigMessage->getVlanId());

		assert(NULL != pVlanIntfManagedObject);		
		if(pVlanIntfManagedObject->m_igmpsMrtrVlanConfig.size() > 0)
		{
			status = WAVE_MESSAGE_SUCCESS;
		}
		
		/* Problem of using legacy code */
		delete pVlanIntfManagedObject;

		return status;
    }

 	/*
	 * Sudhanshu: This function will validate the Vlan view related IGMP mrouter command. 
     *			  All the validations for IGMP mrouter command in vlan view  must go here.
	 */
    void IgmpsVlanGlobalConfWorker::validateIgmpsMrtrVlanConfigStep 
									(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		/* Validation 1: If mrouter enable command is received, then there should not be querier configuration. */
		ResourceId status = WAVE_MESSAGE_SUCCESS; /* By default, the status should be success */
		IgmpsVlanIntfConfManagedObject *pIgmpsVlanConfigManagedObject = NULL;
		NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsMrtrVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		UI32 vlanId = pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage->getVlanId();
		if (NsmUtils::isRemoteSpanVlan(vlanId)) {
			PLUG_DBG_ERR_S(string("The vlan is a remote-span vlan. Cannot hold IGMP configuration"));
			status = WRC_SPAN_CANNOT_CONFIGURE_IGMPS_ON_RSPAN_VLAN;
		}
		else {
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute (new AttributeUI32 (&vlanId, "vlanId"));
			vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

			if (NULL != pResults) {
				UI32 numberOfResults = pResults->size ();
				if (numberOfResults == 0 || numberOfResults > 1) {
					trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
					pIgmpsVlanConfigManagedObject = NULL;
				} else {
					pIgmpsVlanConfigManagedObject = dynamic_cast<IgmpsVlanIntfConfManagedObject*>((*pResults)[0]);
				}
			}

			assert(NULL != pIgmpsVlanConfigManagedObject);

			if (true == pIgmpsVlanConfigManagedObject->getQuerierEnable())
			{
				status = WRC_IGMP_ERR_NO_VALID_CONFIG;
			}
		}

		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pIgmpsVlanConfigManagedObject);
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
   }
	void IgmpsVlanGlobalConfWorker::validateIgmpsSgVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{

		NsmGlobalUpdateIgmpsSgVlanConfigMessage *pNsmGlobalUpdateIgmpsSgVlanConfigMessage = dynamic_cast<NsmGlobalUpdateIgmpsSgVlanConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		PLUG_DBG_S("IgmpsVlanGlobalConfWorker::updateIgmpsSgVlanConfigStep: Entered....");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string ifName = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getIfName();
		bool isL3ConfigPresent = false;
		if(pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getNegation() != true) 
		{
			NsmL3ReadyManagedObject *pL3ReadyMo  =NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, NULL);	
			if(pL3ReadyMo)  
			{
				isL3ConfigPresent = pL3ReadyMo->isL3ConfigPresent();
				pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
			}
			if(isL3ConfigPresent == true)
			{
				PLUG_DBG_S("Port is in L3 Mode and has some L3 config");
				status =  WRC_NSM_ERR_INTF_NOT_MODE_L2;
			}
		}
		UI32 vlanId = pNsmGlobalUpdateIgmpsSgVlanConfigMessage->getVlanId();
		if (NsmUtils::isRemoteSpanVlan(vlanId)) {
			PLUG_DBG_ERR_S(string("The vlan is a remote-span vlan. Cannot hold IGMP configuration"));
			status = WRC_SPAN_CANNOT_CONFIGURE_IGMPS_ON_RSPAN_VLAN;
		}
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
	}

} /* end namespace */

