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
 *   Author : vivekk                                                     *
 **************************************************************************/

//All FrameWork Files
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCStatus.h"

//All Global Files
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonEventManagedObject.h"
#include "Rmon/Global/RmonGlobalCreateRmonEventMessage.h"
#include "Rmon/Global/RmonGlobalDeleteRmonEventMessage.h"
#include "Rmon/Global/RmonAlarmManagedObject.h"
#include "Rmon/Global/RmonGlobalCreateRmonAlarmMessage.h"
#include "Rmon/Global/RmonGlobalDeleteRmonAlarmMessage.h"
#include "Rmon/Global/RmonGlobalTypes.h"
//All Local Files
#include "Rmon/Local/RmonCreateRmonEventSPMessage.h"
#include "Rmon/Local/RmonDeleteRmonEventSPMessage.h"
#include "Rmon/Local/RmonCreateRmonAlarmSPMessage.h"
#include "Rmon/Local/RmonDeleteRmonAlarmSPMessage.h"
#include "ClientInterface/Rmon/RmonUtils.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/Rmon/RmonStatisticsBookKeepLocalManagedObject.h"

namespace DcmNs
{

    RmonGlobalObjectManager::RmonGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        RmonEventManagedObject    RmonEventManagedObject    (this);
        RmonAlarmManagedObject    RmonAlarmManagedObject    (this);
        RmonEventManagedObject.setupOrm ();
        addManagedClass (RmonEventManagedObject::getClassName ());
        RmonAlarmManagedObject.setupOrm ();
        addManagedClass (RmonAlarmManagedObject::getClassName ());
        addOperationMap (RMONGLOBALCREATERMONEVENT,
			reinterpret_cast<PrismMessageHandler> (&RmonGlobalObjectManager::RmonGlobalCreateRmonEventMessageHandler));
        addOperationMap (RMONGLOBALDELETERMONEVENT,
			reinterpret_cast<PrismMessageHandler> (&RmonGlobalObjectManager::RmonGlobalDeleteRmonEventMessageHandler));
        addOperationMap (RMONGLOBALCREATERMONALARM,
			reinterpret_cast<PrismMessageHandler> (&RmonGlobalObjectManager::RmonGlobalCreateRmonAlarmMessageHandler));
        addOperationMap (RMONGLOBALDELETERMONALARM,
			reinterpret_cast<PrismMessageHandler> (&RmonGlobalObjectManager::RmonGlobalDeleteRmonAlarmMessageHandler));
    }

    RmonGlobalObjectManager::~RmonGlobalObjectManager ()
    {
    }

    RmonGlobalObjectManager  *RmonGlobalObjectManager::getInstance()
    {
        static RmonGlobalObjectManager *pRmonGlobalObjectManager = new RmonGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pRmonGlobalObjectManager, __FILE__, __LINE__);

        return (pRmonGlobalObjectManager);
    }

    string  RmonGlobalObjectManager::getClassName()
    {
        return ("RmonGlobal");
    }

    PrismServiceId  RmonGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *RmonGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case RMONGLOBALCREATERMONEVENT :
                pPrismMessage = new RmonGlobalCreateRmonEventMessage ();
                break;
            case RMONGLOBALDELETERMONEVENT :
                pPrismMessage = new RmonGlobalDeleteRmonEventMessage ();
                break;
            case RMONGLOBALCREATERMONALARM :
                pPrismMessage = new RmonGlobalCreateRmonAlarmMessage ();
                break;
            case RMONGLOBALDELETERMONALARM :
                pPrismMessage = new RmonGlobalDeleteRmonAlarmMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *RmonGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RmonEventManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonEventManagedObject(this);
        }
		else if ((RmonAlarmManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonAlarmManagedObject(this);
        }
        else
        {
            RMON_PLUGIN_TRACE_FATAL(string("RmonGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : ") + managedClassName);
        }
        return (pWaveManagedObject);
    }


 void RmonGlobalObjectManager::createRmonEventGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        RmonGlobalCreateRmonEventMessage *pRmonGlobalCreateEventMessage =
          dynamic_cast<RmonGlobalCreateRmonEventMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        RmonCreateRmonEventSPMessage *m = new RmonCreateRmonEventSPMessage();
        m->setEventIndex(pRmonGlobalCreateEventMessage->getEventIndex());
        m->setLog(pRmonGlobalCreateEventMessage->getLog());
        m->setCommunityName(pRmonGlobalCreateEventMessage->getCommunityName());
        m->setDescription(pRmonGlobalCreateEventMessage->getDescription());
        m->setOwner(pRmonGlobalCreateEventMessage->getOwner());

        send_ctx_p = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&RmonGlobalObjectManager::rmonGlobalSendToClusterCallback),
            pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        RMON_PLUGIN_TRACE_DEBUG(string("Sending msg to Cluster :") + pRmonGlobalCreateEventMessage->getEventIndex());
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
        return;
    }

	 void RmonGlobalObjectManager::createRmonEventGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        RmonEventManagedObject *pRmonEventMO = NULL;
        RMON_PLUGIN_TRACE();

        RmonGlobalCreateRmonEventMessage *pRmonGlobalCreateEventMessage =
          dynamic_cast<RmonGlobalCreateRmonEventMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        pRmonEventMO = new RmonEventManagedObject(dynamic_cast<RmonGlobalObjectManager*>(getPWaveObjectManager()));
        pRmonEventMO->setEventIndex(pRmonGlobalCreateEventMessage->getEventIndex());
        pRmonEventMO->setLog(pRmonGlobalCreateEventMessage->getLog());
        pRmonEventMO->setDescription(pRmonGlobalCreateEventMessage->getDescription());
        pRmonEventMO->setOwner(pRmonGlobalCreateEventMessage->getOwner());
        pRmonEventMO->setCommunityName(pRmonGlobalCreateEventMessage->getCommunityName());

		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pRmonEventMO);
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }	

	void  RmonGlobalObjectManager::RmonGlobalCreateRmonEventMessageHandler( RmonGlobalCreateRmonEventMessage *pRmonGlobalCreateRmonEventMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::createRmonEventGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::createRmonEventGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
          new PrismLinearSequencerContext (pRmonGlobalCreateRmonEventMessage, this, sequencerSteps,
              sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start();
    }

	 void RmonGlobalObjectManager::createRmonAlarmGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        RmonGlobalCreateRmonAlarmMessage *pRmonGlobalCreateAlarmMessage =
          dynamic_cast<RmonGlobalCreateRmonAlarmMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;
        RmonCreateRmonAlarmSPMessage *m = new RmonCreateRmonAlarmSPMessage();
        string snmpOid = pRmonGlobalCreateAlarmMessage->getSnmpOid();
        SI32 statsIndex = 0;
        UI32 locationId = 0;
        string ifName;
        vector<UI32>                locations;
        statsIndex = getEtherStatsIndexFromAlarmOid(snmpOid);
        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
        RMON_PLUGIN_TRACE_DEBUG(string("stats id =") + statsIndex);

        RmonStatisticsBookKeepLocalManagedObject *statsBookKeepMo = NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonStatisticsBookKeepLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectField("ifName");
        syncQueryCtxt.addAndAttribute (new AttributeSI32(statsIndex, "statsIndex"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
             if (0 == pResults->size ())
             {
                 status = WRC_RMON_DCM_STATS_INDEX_NOT_CONFIGURED;
				 WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                 pPrismLinearSequencerContext->executeNextStep(status);
				 if(m != NULL)
				 {
					delete m;
				 }

                 return;
             }

        statsBookKeepMo = dynamic_cast<RmonStatisticsBookKeepLocalManagedObject*>((*pResults)[0]);
        ifName = statsBookKeepMo->getIfName();
        locationId = NsmUtils::getLocationId(ifName);
        locations.push_back(locationId);
		}
		else
		{
                 status = WRC_RMON_DCM_STATS_INDEX_NOT_CONFIGURED;
                 pPrismLinearSequencerContext->executeNextStep(status);
				 if(m != NULL)
                 {
                    delete m;
                 }
                 return;
               }

        m->setAlarmIndex(pRmonGlobalCreateAlarmMessage->getAlarmIndex());
        m->setSnmpOid(snmpOid);
        m->setInterval(pRmonGlobalCreateAlarmMessage->getInterval());
        m->setSampleType(pRmonGlobalCreateAlarmMessage->getSampleType());
        m->setRisingThreshold(pRmonGlobalCreateAlarmMessage->getRisingThreshold());
        m->setRisingEventId(pRmonGlobalCreateAlarmMessage->getRisingEventId());
        m->setFallingThreshold(pRmonGlobalCreateAlarmMessage->getFallingThreshold());
        m->setFallingEventId(pRmonGlobalCreateAlarmMessage->getFallingEventId());
        m->setOwner(pRmonGlobalCreateAlarmMessage->getOwner());
		 send_ctx_p = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&RmonGlobalObjectManager::rmonGlobalSendToClusterCallback),
            pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setLocationsToSendToForPhase1(locations);
        RMON_PLUGIN_TRACE_DEBUG(string("Sending msg to Cluster :") + pRmonGlobalCreateAlarmMessage->getAlarmIndex());

		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
		if(pResults)
				{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				}
        return;
    }

	
	void RmonGlobalObjectManager::createRmonAlarmGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        RmonAlarmManagedObject *pRmonAlarmMO = NULL;
        RmonStatisticsBookKeepLocalManagedObject *statsBookKeepMO = NULL;
        string ifName;
        SI32 statsIndex = 0;

        RmonGlobalCreateRmonAlarmMessage *pRmonGlobalCreateAlarmMessage =
          dynamic_cast<RmonGlobalCreateRmonAlarmMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        string snmpOid = pRmonGlobalCreateAlarmMessage->getSnmpOid();
        statsIndex = getEtherStatsIndexFromAlarmOid(snmpOid);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonStatisticsBookKeepLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectField("ifName");
        syncQueryCtxt.addAndAttribute (new AttributeSI32(statsIndex, "statsIndex"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if(NULL != pResults)
        {
            statsBookKeepMO = dynamic_cast<RmonStatisticsBookKeepLocalManagedObject *>((*pResults)[0]);
            ifName = statsBookKeepMO->getIfName();
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        RMON_PLUGIN_TRACE();

        pRmonAlarmMO = new RmonAlarmManagedObject(dynamic_cast<RmonGlobalObjectManager*>(getPWaveObjectManager()));
        pRmonAlarmMO->setAlarmIndex(pRmonGlobalCreateAlarmMessage->getAlarmIndex());
        pRmonAlarmMO->setSnmpOid(pRmonGlobalCreateAlarmMessage->getSnmpOid());
        pRmonAlarmMO->setInterval(pRmonGlobalCreateAlarmMessage->getInterval());
        pRmonAlarmMO->setSampleType(pRmonGlobalCreateAlarmMessage->getSampleType());
        pRmonAlarmMO->setRisingThreshold(pRmonGlobalCreateAlarmMessage->getRisingThreshold());
        pRmonAlarmMO->setRisingEventId(pRmonGlobalCreateAlarmMessage->getRisingEventId());
        pRmonAlarmMO->setFallingThreshold(pRmonGlobalCreateAlarmMessage->getFallingThreshold());
        pRmonAlarmMO->setFallingEventId(pRmonGlobalCreateAlarmMessage->getFallingEventId());
        pRmonAlarmMO->setOwner(pRmonGlobalCreateAlarmMessage->getOwner());
        pRmonAlarmMO->setIfName(ifName);
		pRmonAlarmMO->setStatsIndex(statsIndex);
		
		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pRmonAlarmMO);
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }	

    void  RmonGlobalObjectManager::RmonGlobalCreateRmonAlarmMessageHandler(RmonGlobalCreateRmonAlarmMessage *pRmonGlobalCreateRmonAlarmMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::createRmonAlarmGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::createRmonAlarmGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
		  new PrismLinearSequencerContext (pRmonGlobalCreateRmonAlarmMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start();
    }

	void RmonGlobalObjectManager::deleteRmonEventValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	  	RMON_PLUGIN_TRACE();
		UI32 count = 0;
    	RmonGlobalDeleteRmonEventMessage *pRmonGlobalDeleteEventMessage =
		  dynamic_cast<RmonGlobalDeleteRmonEventMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		SI32 eventId = pRmonGlobalDeleteEventMessage->getEventIndex();
		ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;

    	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
    	syncQueryCtxt.addOrAttribute (new AttributeSI32(eventId, "risingEventId"));
    	syncQueryCtxt.addOrAttribute (new AttributeSI32(eventId, "fallingEventId"));
    	querySynchronouslyForCount (&syncQueryCtxt, count);
       	if (count > 0)
		{
		  	status = WRC_RMON_CANT_REMOVE_EVENT_REF_ALARM;
		}
		pPrismLinearSequencerContext->executeNextStep(status);
	}

	void RmonGlobalObjectManager::deleteRmonEventGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
    	RmonGlobalDeleteRmonEventMessage *pRmonGlobalDeleteEventMessage =
		  dynamic_cast<RmonGlobalDeleteRmonEventMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;
    	RmonDeleteRmonEventSPMessage *m = new RmonDeleteRmonEventSPMessage();
		SI32 eventId = pRmonGlobalDeleteEventMessage->getEventIndex();
    	m->setEventIndex(eventId);
        RMON_PLUGIN_TRACE_DEBUG(string("eventId=") + eventId);

		send_ctx_p = new WaveSendToClusterContext(this,
			reinterpret_cast<PrismAsynchronousCallback>(&RmonGlobalObjectManager::rmonGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    	prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    	send_ctx_p->setPPrismMessageForPhase1(m);

		/* Send the message to all nodes in the cluster */
        send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    	sendToWaveCluster(send_ctx_p);
    	return;
	}

	void RmonGlobalObjectManager::deleteRmonEventGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		SI32 eventId = 0;
    	RmonGlobalDeleteRmonEventMessage *pRmonGlobalDeleteEventMessage =
		  dynamic_cast<RmonGlobalDeleteRmonEventMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    	eventId = pRmonGlobalDeleteEventMessage->getEventIndex();
        RMON_PLUGIN_TRACE_DEBUG(string("eventId=") + eventId);

    	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonEventManagedObject::getClassName());
    	syncQueryCtxt.addAndAttribute (new AttributeSI32(eventId, "eventIndex"));
    	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    	RmonEventManagedObject *pRmonEventMO = NULL;

    	if (NULL != pResults) {
        	UI32 numberOfResults = pResults->size ();
        	if (1 == numberOfResults) {
            	RMON_PLUGIN_TRACE_DEBUG(string("RmonEvent MO found"));
            	pRmonEventMO = dynamic_cast<RmonEventManagedObject*>((*pResults)[0]);
        	} else {
            	RMON_PLUGIN_TRACE_ERROR(string("RmonEvent MO error"));
        	} 
    	}

		prismAssert (NULL != pRmonEventMO, __FILE__, __LINE__);
    	delete pRmonEventMO;

        if (NULL != pResults) {
		  	pResults->clear ();
            delete pResults;
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

    void  RmonGlobalObjectManager::RmonGlobalDeleteRmonEventMessageHandler( RmonGlobalDeleteRmonEventMessage *pRmonGlobalDeleteRmonEventMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::deleteRmonEventValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::deleteRmonEventGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::deleteRmonEventGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
		  new PrismLinearSequencerContext (pRmonGlobalDeleteRmonEventMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
		pPrismLinearSequencerContext->start();
    }

 void RmonGlobalObjectManager::deleteRmonAlarmGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        RmonGlobalDeleteRmonAlarmMessage *pRmonGlobalDeleteAlarmMessage =
          dynamic_cast<RmonGlobalDeleteRmonAlarmMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;
        RmonDeleteRmonAlarmSPMessage *m = new RmonDeleteRmonAlarmSPMessage();
        SI32 alarmId = pRmonGlobalDeleteAlarmMessage->getAlarmIndex();
        SI32 statsIndex = 0;
        UI32 locationId = 0;
        string ifName;
        vector<UI32>                locations;
        m->setAlarmIndex(alarmId);
        RMON_PLUGIN_TRACE_DEBUG(string("alarmId=") + alarmId);

        RmonStatisticsBookKeepLocalManagedObject *statsBookKeepMo = NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeSI32(alarmId, "alarmIndex"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        RmonAlarmManagedObject *pRmonAlarmMO = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (1 == numberOfResults) {
                RMON_PLUGIN_TRACE_DEBUG(string("RmonAlarm MO found"));
                pRmonAlarmMO = dynamic_cast<RmonAlarmManagedObject*>((*pResults)[0]);
                string ifNameForAlarm = pRmonAlarmMO->getIfName();
				RMON_PLUGIN_TRACE_DEBUG(string("ifNameForAlarm =") + ifNameForAlarm);
                LocationId locationIdForAlarm = NsmUtils::getLocationId(ifNameForAlarm);
				RMON_PLUGIN_TRACE_DEBUG(string("locationIdForAlarm =") + locationIdForAlarm);
                if(!(FrameworkToolKit::isAConnectedLocation (locationIdForAlarm) || locationIdForAlarm == FrameworkToolKit::getThisLocationId ()))
                {
                    RMON_PLUGIN_TRACE_DEBUG(string("just Inside isConnected"));
                    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
					return;
                }
                RMON_PLUGIN_TRACE_DEBUG(string("just outside isConnected"));
                string snmpOid = pRmonAlarmMO->getSnmpOid();
                statsIndex = getEtherStatsIndexFromAlarmOid(snmpOid);
                ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
                RMON_PLUGIN_TRACE_DEBUG(string("stats id =") + statsIndex);
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(RmonStatisticsBookKeepLocalManagedObject::getClassName());
                syncQueryCtxt2.addAndAttribute (new AttributeSI32(statsIndex, "statsIndex"));
                vector<WaveManagedObject *> *pResults2 = querySynchronously (&syncQueryCtxt2);

        		if ( (NULL != pResults2) && (pResults2->size() != 0) )
				{
                statsBookKeepMo = dynamic_cast<RmonStatisticsBookKeepLocalManagedObject*>((*pResults2)[0]);
                ifName = statsBookKeepMo->getIfName();
                locationId = NsmUtils::getLocationId(ifName);
                locations.push_back(locationId);
            		pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults2);
            		delete pResults2;
         		}
				else
				{
				 status = WRC_RMON_DCM_STATS_INDEX_NOT_CONFIGURED;
                 pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                 pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults2);
                 delete pResults;
                 delete pResults2;
                 pPrismLinearSequencerContext->executeNextStep(status);
                 return;
               }
			}
			else
			{
                RMON_PLUGIN_TRACE_ERROR(string("RmonAlarm MO error"));
            }
        }

        send_ctx_p = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&RmonGlobalObjectManager::rmonGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setLocationsToSendToForPhase1(locations);

        /* Send the message to all nodes in the cluster */
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        RMON_PLUGIN_TRACE_DEBUG(string("Sending msg to Cluster :") + pRmonGlobalDeleteAlarmMessage->getAlarmIndex());
        sendToWaveCluster(send_ctx_p);
        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        return;
    }

	void RmonGlobalObjectManager::deleteRmonAlarmGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		SI32 alarmId = 0;
    	RmonGlobalDeleteRmonAlarmMessage *pRmonGlobalDeleteAlarmMessage =
		  dynamic_cast<RmonGlobalDeleteRmonAlarmMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    	alarmId = pRmonGlobalDeleteAlarmMessage->getAlarmIndex();
        RMON_PLUGIN_TRACE_DEBUG(string("alarmId=") + alarmId);

    	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
    	syncQueryCtxt.addAndAttribute (new AttributeSI32(alarmId, "alarmIndex"));
    	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    	RmonAlarmManagedObject *pRmonAlarmMO = NULL;

    	if (NULL != pResults) {
        	UI32 numberOfResults = pResults->size ();
        	if (1 == numberOfResults) {
            	RMON_PLUGIN_TRACE_DEBUG(string("RmonAlarm MO found"));
            	pRmonAlarmMO = dynamic_cast<RmonAlarmManagedObject*>((*pResults)[0]);
        	} else {
            	RMON_PLUGIN_TRACE_ERROR(string("RmonAlarm MO error"));
        	} 
    	}

		prismAssert (NULL != pRmonAlarmMO, __FILE__, __LINE__);
    	delete pRmonAlarmMO;
        
        if (NULL != pResults) {
		  	pResults->clear ();
            delete pResults;
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

    void  RmonGlobalObjectManager::RmonGlobalDeleteRmonAlarmMessageHandler(RmonGlobalDeleteRmonAlarmMessage *pRmonGlobalDeleteRmonAlarmMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::deleteRmonAlarmGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::deleteRmonAlarmGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&RmonGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
		  new PrismLinearSequencerContext (pRmonGlobalDeleteRmonAlarmMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
		pPrismLinearSequencerContext->start();
    }

	void RmonGlobalObjectManager::rmonGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p)
    {
	  	RMON_PLUGIN_TRACE();
        LocationId locationid;
        ClusterGetFirstFailureLocation(ctx_p, locationid);
		ResourceId sendToClusterCompletionStatus =  ClusterGetStatusFromContext(ctx_p); 
        PrismLinearSequencerContext             *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
		
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
           trace (TRACE_LEVEL_DEVEL, "Phase 1 message succeeded on all locations");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED)
           trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED)
           trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes");
        else
           trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus:: Unknown Error : ") + sendToClusterCompletionStatus);

		vector<LocationId>  loc_vector;
        ResourceId  status = WAVE_MESSAGE_SUCCESS;

        loc_vector = ctx_p->getLocationsToSendToForPhase1();
        for (UI32 i = 0; i < loc_vector.size(); i++) {
            status = ctx_p->getCompletionStatusForPhase1(loc_vector[i]);
            if(status != WAVE_MESSAGE_SUCCESS)
            {
                trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus: ") + status);
            }
        }

        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p;
		RMON_PLUGIN_TRACE_DEBUG(string("Completion status=") + sendToClusterCompletionStatus);
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

	void RmonGlobalObjectManager::ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid)
	{
    	vector<LocationId>  locationsToSendToForPhase1 = ctx_p -> getLocationsToSendToForPhase1 ();

    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	for(unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++)
    	{
        	locationid = locationsToSendToForPhase1[i];
        	status = ctx_p -> getCompletionStatusForPhase1(locationid);
        	if (status != WAVE_MESSAGE_SUCCESS)
            	return;
    	}
	}

	void RmonGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage,
                                                                 PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        trace(TRACE_LEVEL_DEBUG, string("Cluster Collect function called"));
        data_p = dce_cluster_validation_collect_init(RMON_PLUGIN_GLOBAL_VERSION);

        if (data_p == NULL) {
            trace(TRACE_LEVEL_ERROR, string("Failed to init ValidationData"));
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
        return;
    }

	void RmonGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                                                    PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

        trace(TRACE_LEVEL_DEBUG, string("Cluster Validate function called"));
        pMessage->getValidationDetails(payload_p, len);
        trace(TRACE_LEVEL_DEBUG,string("Received Validation Data of LEN=") + len);
        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);
        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, string("Failed to parse ValidationData Err=") +
                                FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        trace(TRACE_LEVEL_DEBUG, string("Recieved version = ") + version);

        if (version != RMON_PLUGIN_GLOBAL_VERSION) {
            trace(TRACE_LEVEL_ERROR, string("Version mismatch, My-Version=") + RMON_PLUGIN_GLOBAL_VERSION +
                                string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(RMON_CLUSTER_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }

	void RmonGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        vector<LocationId>::iterator  it;
        ResourceId  status;

        trace (TRACE_LEVEL_DEVEL, string ("NsmGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));
        if (failoverReason == FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED)
        {
            NSM_PLUG_DBG("Uncontrolled failover rcvd.. Returning");
            pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pFailoverAsynchronousContext->callback ();
            return;
        }

        for (it = failedLocationIds.begin(); it != failedLocationIds.end(); it++)
        {
            NSM_PLUG_DBG_S(string("Failed Location = ") + *it);
            status = RmonGlobalObjectManager::RmonGlobalHandleFailoverForLocation(*it);
        }

        pFailoverAsynchronousContext->setCompletionStatus (status);
        pFailoverAsynchronousContext->callback ();
    }

    ResourceId RmonGlobalObjectManager::RmonGlobalHandleFailoverForLocation (UI32 &location_id)
    {
        ResourceId  status;
        status = RmonGlobalObjectManager::RmonGlobalCleanupForLocation(location_id);
        return status;
    }


	 ResourceId RmonGlobalObjectManager::RmonGlobalCleanupForLocation (UI32 &location_id)
    {
        SI32 mapped_id, mappedId_extr;
        string db_source_ifName;
        mapped_id = DcmToolKit::getMappedIdFromLocationId(location_id);
        NSM_PLUG_DBG_S(string("Found Mapped ID = ") + mapped_id);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        RmonAlarmManagedObject *loc_mo_p;
        startTransaction();
        if(NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            UI32 i = 0;
            RMON_PLUGIN_TRACE_DEBUG(string("Pos 0 : numberOfResults = ") + numberOfResults);
            for(; i < numberOfResults; i++)
            {
                loc_mo_p = dynamic_cast<RmonAlarmManagedObject*>((*pResults)[i]);
                db_source_ifName = loc_mo_p->getIfName();
                mappedId_extr = NsmUtils::getMappedIdFromIfName(db_source_ifName);
                RMON_PLUGIN_TRACE_DEBUG(string("Found Mapped ID from if_Name = ") + mappedId_extr);
                if(mappedId_extr == mapped_id)
                {
                    RMON_PLUGIN_TRACE_DEBUG(string("Found Mapped ID from if_Name = ") + mappedId_extr);
                    delete loc_mo_p;
                }
            }
        }

        commitTransaction();
		if (pResults) {
		        pResults->clear ();
		        delete pResults;
		}
		return WAVE_MESSAGE_SUCCESS;
    }

}
