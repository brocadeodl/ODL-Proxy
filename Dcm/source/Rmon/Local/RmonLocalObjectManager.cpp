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
 *   Author : vivekk													   *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Rmon/Global/RmonAlarmManagedObject.h"
#include "Rmon/Global/RmonEventManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCore/DcmToolKit.h"
#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Rmon/Local/RmonCreateRmonEventSPMessage.h"
#include "Rmon/Local/RmonDeleteRmonEventSPMessage.h"
#include "Rmon/Local/RmonCreateRmonAlarmSPMessage.h"
#include "Rmon/Local/RmonDeleteRmonAlarmSPMessage.h"
#include "Rmon/Local/RmonTypes.h"
#include "Rmon/Local/RmonLocalShowMessage.h"
#include "Rmon/Local/RmonClearMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

//ClientInterface
#include "ClientInterface/Rmon/RmonClientCreateRmonEventMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteRmonEventMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateRmonAlarmMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteRmonAlarmMessage.h"
#include "ClientInterface/Rmon/RmonUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Rmon/rmon_common.h"
#include "ClientInterface/Rmon/RmonShowMessage.h"
#include "ClientInterface/Rmon/RmonClientClearMessage.h"
#include "DcmCore/DcmToolKit.h"

#include "DcmCStatus.h"
#include "rmon_dcmd.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    RmonLocalObjectManager::RmonLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (RMONCREATERMONEVENTSP,
			reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonCreateRmonEventSPMessageHandler));
        addOperationMap (RMONDELETERMONEVENTSP,
			reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonDeleteRmonEventSPMessageHandler));
        addOperationMap (RMONCREATERMONALARMSP,
			reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonCreateRmonAlarmSPMessageHandler));
        addOperationMap (RMONDELETERMONALARMSP,
			reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonDeleteRmonAlarmSPMessageHandler));
        addOperationMap (RMONLOCALSHOW,
			reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonLocalShowMessageHandler));
		addOperationMap (RMON_LOCAL_CLEAR_MSG, reinterpret_cast<PrismMessageHandler> (&RmonLocalObjectManager::RmonLocalClearMessageHandler));
    }

    RmonLocalObjectManager::~RmonLocalObjectManager ()
    {
    }

    RmonLocalObjectManager  *RmonLocalObjectManager::getInstance()
    {
        static RmonLocalObjectManager *pRmonLocalObjectManager = new RmonLocalObjectManager ();

        WaveNs::prismAssert (NULL != pRmonLocalObjectManager, __FILE__, __LINE__);

        return (pRmonLocalObjectManager);
    }

    string  RmonLocalObjectManager::getClassName()
    {
        return ("Rmon");
    }

    PrismServiceId  RmonLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *RmonLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case RMONCREATERMONEVENTSP :
                pPrismMessage = new RmonCreateRmonEventSPMessage ();
                break;
            case RMONDELETERMONEVENTSP :
                pPrismMessage = new RmonDeleteRmonEventSPMessage ();
                break;
            case RMONCREATERMONALARMSP :
                pPrismMessage = new RmonCreateRmonAlarmSPMessage ();
                break;
            case RMONDELETERMONALARMSP :
                pPrismMessage = new RmonDeleteRmonAlarmSPMessage ();
                break;
            case RMONLOCALSHOW :
                pPrismMessage = new RmonLocalShowMessage();
                break;
            case RMON_LOCAL_CLEAR_MSG:
                pPrismMessage = new RmonClearMessage();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *RmonLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return (NULL);
    }
	
    void RmonLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string name = pWaveAsynchronousContextForPostBootPhase->getPassName();
        RMON_PLUGIN_TRACE_DEBUG(string("postboot Entered with pass:") + name);

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
				RMON_PLUGIN_TRACE_DEBUG("Started with configReplayRmonEvent\n");
				configReplayRmonEvent();
				RMON_PLUGIN_TRACE_DEBUG("Done with configReplayRmonEvent\n");
        }
        else if (name == "DCM_POSTBOOT_GLOBAL_STAGE3") {
				RMON_PLUGIN_TRACE_DEBUG("Started with configReplayRmonAlarm\n");
				configReplayRmonAlarm();
				RMON_PLUGIN_TRACE_DEBUG("Done with configReplayRmonAlarm\n");
		}

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

	void RmonLocalObjectManager::configReplayRmonAlarm()
    {
        RMON_PLUGIN_TRACE();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);
        string ifName;
        UI32 localLocationId = NsmUtils::getLocalLocationId();
        LocationId locationId = 0;

        RmonAlarmManagedObject * pRmonAlarmManagedObject = NULL;
        if (NULL != pResults && pResults->size() > 0)
        {
            for(unsigned int i = 0;i <  pResults->size(); i++)
            {
                pRmonAlarmManagedObject = dynamic_cast<RmonAlarmManagedObject *>((*pResults)[i]);
                ifName = pRmonAlarmManagedObject->getIfName();
                locationId = NsmUtils::getLocationId(ifName);
                if(locationId == localLocationId)
                {
                    RmonClientCreateRmonAlarmMessage *m = new RmonClientCreateRmonAlarmMessage();

                    m->setAlarmIndex(pRmonAlarmManagedObject->getAlarmIndex());
                    m->setSnmpOid(pRmonAlarmManagedObject->getSnmpOid());
                    m->setInterval(pRmonAlarmManagedObject->getInterval());
                    m->setSampleType(pRmonAlarmManagedObject->getSampleType());
                    m->setRisingThreshold(pRmonAlarmManagedObject->getRisingThreshold());
                    m->setRisingEventId(pRmonAlarmManagedObject->getRisingEventId());
                    m->setFallingThreshold(pRmonAlarmManagedObject->getFallingThreshold());
                    m->setFallingEventId(pRmonAlarmManagedObject->getFallingEventId());
                    m->setOwner(pRmonAlarmManagedObject->getOwner());
                    RMON_PLUGIN_TRACE_DEBUG(string("Sending RmonClientCreateRmonAlarmMessage to rmon Client\n"));
                    sendSynchronouslyToWaveClient ("rmon", m);
                    delete m;
                }
            }
            if(pResults)
            {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return ;
    }

    void RmonLocalObjectManager::configReplayRmonEvent()
    {
        RMON_PLUGIN_TRACE();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonEventManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        RmonEventManagedObject * pRmonEventManagedObject = NULL;
        if (NULL != pResults && pResults->size() > 0)
        {
            for(unsigned int i = 0;i <  pResults->size(); i++)
            {
                pRmonEventManagedObject = dynamic_cast<RmonEventManagedObject *>((*pResults)[i]);
                RmonClientCreateRmonEventMessage *m = new RmonClientCreateRmonEventMessage();

                m->setEventIndex(pRmonEventManagedObject->getEventIndex());
                m->setLog(pRmonEventManagedObject->getLog());
                m->setDescription(pRmonEventManagedObject->getDescription());
               	m->setOwner(pRmonEventManagedObject->getOwner());
                m->setCommunityName(pRmonEventManagedObject->getCommunityName());
                RMON_PLUGIN_TRACE_DEBUG(string("Sending RmonClientCreateRmonEventMessage to rmon Client\n"));
               	sendSynchronouslyToWaveClient ("rmon", m);
				delete m;
            }
            if(pResults)
            {
			  	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }
        return ;
    }

	ResourceId RmonLocalObjectManager::createRmonAlarmLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
    	RmonCreateRmonAlarmSPMessage *pRmonAlarmCreateMessage =
		  dynamic_cast<RmonCreateRmonAlarmSPMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    	ResourceId status;
		RmonClientCreateRmonAlarmMessage *m = new RmonClientCreateRmonAlarmMessage();

		m->setAlarmIndex(pRmonAlarmCreateMessage->getAlarmIndex());
		m->setSnmpOid(pRmonAlarmCreateMessage->getSnmpOid());
		m->setInterval(pRmonAlarmCreateMessage->getInterval());
		m->setSampleType(pRmonAlarmCreateMessage->getSampleType());
		m->setRisingThreshold(pRmonAlarmCreateMessage->getRisingThreshold());
		m->setRisingEventId(pRmonAlarmCreateMessage->getRisingEventId());
		m->setFallingThreshold(pRmonAlarmCreateMessage->getFallingThreshold());
		m->setFallingEventId(pRmonAlarmCreateMessage->getFallingEventId());
		m->setOwner(pRmonAlarmCreateMessage->getOwner());

		status = sendSynchronouslyToWaveClient ("rmon", m);
		status = errorCode(pRmonAlarmCreateMessage, m, status);
		delete m;
		return (status);
	}

    void  RmonLocalObjectManager::RmonCreateRmonAlarmSPMessageHandler( RmonCreateRmonAlarmSPMessage *pRmonCreateRmonAlarmSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::createRmonAlarmLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonCreateRmonAlarmSPMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId RmonLocalObjectManager::createRmonEventLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
    	RmonCreateRmonEventSPMessage *pRmonEventCreateMessage =
		  dynamic_cast<RmonCreateRmonEventSPMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    	ResourceId status;
		RmonClientCreateRmonEventMessage *m = new RmonClientCreateRmonEventMessage();

		m->setEventIndex(pRmonEventCreateMessage->getEventIndex());
		m->setLog(pRmonEventCreateMessage->getLog());
		m->setCommunityName(pRmonEventCreateMessage->getCommunityName());
		m->setDescription(pRmonEventCreateMessage->getDescription());
		m->setOwner(pRmonEventCreateMessage->getOwner());

		status = sendSynchronouslyToWaveClient ("rmon", m);
		status = errorCode(pRmonEventCreateMessage, m, status);
		delete m;
		return (status);
	}

    void  RmonLocalObjectManager::RmonCreateRmonEventSPMessageHandler( RmonCreateRmonEventSPMessage *pRmonCreateRmonEventSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::createRmonEventLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonCreateRmonEventSPMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId RmonLocalObjectManager::deleteRmonAlarmLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
    	RmonDeleteRmonAlarmSPMessage *pRmonAlarmDeleteMessage =
		  dynamic_cast<RmonDeleteRmonAlarmSPMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    	ResourceId status;
		RmonClientDeleteRmonAlarmMessage *m = new RmonClientDeleteRmonAlarmMessage();
		SI32 alarmId = pRmonAlarmDeleteMessage->getAlarmIndex();

        RMON_PLUGIN_TRACE_DEBUG(string("alarmId=") + alarmId);
		m->setAlarmIndex(alarmId);
		status = sendSynchronouslyToWaveClient ("rmon", m);
		status = errorCode(pRmonAlarmDeleteMessage, m, status);
		delete m;
		return (status);
	}

    void  RmonLocalObjectManager::RmonDeleteRmonAlarmSPMessageHandler( RmonDeleteRmonAlarmSPMessage *pRmonDeleteRmonAlarmSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::deleteRmonAlarmLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonDeleteRmonAlarmSPMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId RmonLocalObjectManager::deleteRmonEventLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
    	RmonDeleteRmonEventSPMessage *pRmonEventDeleteMessage =
		  dynamic_cast<RmonDeleteRmonEventSPMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    	ResourceId status;
		RmonClientDeleteRmonEventMessage *m = new RmonClientDeleteRmonEventMessage();
		SI32 eventId = pRmonEventDeleteMessage->getEventIndex();

        RMON_PLUGIN_TRACE_DEBUG(string("eventId=") + eventId);
		m->setEventIndex(eventId);
		status = sendSynchronouslyToWaveClient ("rmon", m);
		status = errorCode(pRmonEventDeleteMessage, m, status);
		delete m;
		return (status);
	}

    void  RmonLocalObjectManager::RmonDeleteRmonEventSPMessageHandler( RmonDeleteRmonEventSPMessage *pRmonDeleteRmonEventSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::deleteRmonEventLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonDeleteRmonEventSPMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    
	void
    RmonLocalObjectManager::RmonLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        RmonLocalShowMessage                    *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;
		RMON_PLUGIN_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p); 

        if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
           trace (TRACE_LEVEL_DEVEL, "Phase 1 message succeeded on all locations");

		seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p = dynamic_cast<RmonLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p =  dynamic_cast<RmonLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

		prismAssert (NULL != req_msg_p, __FILE__, __LINE__);
		prismAssert (NULL != resp_msg_p, __FILE__, __LINE__);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);
       
        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        RMON_PLUGIN_TRACE_DEBUG(string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i ==0) { 
                RMON_PLUGIN_TRACE_DEBUG(string("locations:") + locations[i]);
            } else { 
                RMON_PLUGIN_TRACE_DEBUG(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<RmonLocalShowMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
			prismAssert (NULL != sw_resp_p, __FILE__, __LINE__);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
              //  req_msg_p->copyBuffersFrom(*sw_resp_p);
                req_msg_p->copyAllBuffers(*sw_resp_p);
            }
        }

		seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        delete resp_msg_p;
        delete ctx_p;
        return;
    }
    

    void
    RmonLocalObjectManager::sendShowCommandToCluster ( PrismLinearSequencerContext *seq_ctx_p)
    {
        RmonLocalShowMessage        *msg_p = NULL;
        RmonLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
//		UI32                        locationId = 0;
        vector<UI32>                locations;
        //UI32 						cmdCode = 0;
//        bool                        sendToLocal = false; */
        msg_p = dynamic_cast<RmonLocalShowMessage *>
            (seq_ctx_p->getPPrismMessage ());

		prismAssert (NULL != msg_p, __FILE__, __LINE__);
		//cmdCode = msg_p->getCmdCode();

        loc_msg_p = new RmonLocalShowMessage(msg_p->getIndex(), msg_p->getShowAll(), msg_p->getShowBrief(), msg_p->getCmdCode(), false,
        						msg_p->getLocationIds(), msg_p->getStartIndex(), msg_p->getEndIndex());

		prismAssert (NULL != loc_msg_p, __FILE__, __LINE__);

        RMON_PLUGIN_TRACE_DEBUG(string("Sending Show Command to Cluster") + string(" CmdCode =") + msg_p->getCmdCode() +
                        string ("eventId =")  + msg_p->getIndex() + string ("showAll=")  + msg_p->getShowAll() +
                        string ("showBrief=")  + msg_p->getShowBrief() + string(" startIndex=") + msg_p->getStartIndex() +
                        string(" endIndex=") + msg_p->getEndIndex());

        RMON_PLUGIN_TRACE_DEBUG(string("RmonlocalObjectManager;;sendShowCommandToCluster;:locationIds size") +
        		msg_p->getLocationIds().size());

        locations = loc_msg_p->getLocationIds();
        UI32 i = 0;

		for(i = 0; i < locations.size(); i++) {
			RMON_PLUGIN_TRACE_DEBUG(string("RmonlocalObjectManager;;sendShowCommandToCluster;:locationId ") + locations[i]);
		}

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>
                (&RmonLocalObjectManager::RmonLocalShowSendToClusterCallback),
                seq_ctx_p);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);

        if (!send_ctx_p) { 
            return;
        }
        
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
            
#if 0        
		if (sendToLocal) {
            /* It is not interface specific command or the interface is
             * two tuple one on the local switch or the command should
			 * be executed on local switch.
             */
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) { 
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
#endif		
        	/* Send the message to all nodes in the cluster */
        send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		/*if(locations.size() > 0) {
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}*/
		RMON_PLUGIN_TRACE_DEBUG(string("RmonLocalObjectManager::sendShowCommandToCluster::sending to Wave cluster"));
		sendToWaveCluster(send_ctx_p);
        return;
    }
    

    ResourceId 
    RmonLocalObjectManager::sendShowToRmonDaemon ( PrismSynchronousLinearSequencerContext *ctx_p)
    {
        RmonLocalShowMessage *msg_p = dynamic_cast<RmonLocalShowMessage *> (ctx_p->getPPrismMessage ());
        RmonShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;

        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) { 
            mappedId = 0;
        }

        RMON_PLUGIN_TRACE_DEBUG(string("Sending Show Command to Rmon Daemon :: Local Message") + string(" CmdCode =") + msg_p->getCmdCode() +
                                string ("eventId =")  + msg_p->getIndex() + string ("showAll=")  + msg_p->getShowAll() +
                                string ("showBrief=")  + msg_p->getShowBrief());
        
		cl_msg_p = new RmonShowMessage(msg_p->getIndex(), msg_p->getShowAll(),
			msg_p->getShowBrief(), msg_p->getCmdCode(), mappedId, msg_p->getStartIndex(), msg_p->getEndIndex());

		RMON_PLUGIN_TRACE_DEBUG(string("Sending Show Command to Rmon Daemon::Client Handler Message") + string(" MsgId =")  + cl_msg_p->getMessageId()  +
		                        string(" CmdCode =") + cl_msg_p->getCmdCode() + string(" showAll =") + cl_msg_p->getShowAll() +
		                        string(" showBrief =") + cl_msg_p->getShowBrief() + string(" index  =")  + cl_msg_p->getIndex() +
		                        string(" mappedId =") + cl_msg_p->getMappedId() + string(" startIndex=") + cl_msg_p->getStartIndex() +
		                        string(" endIndex=") + cl_msg_p->getEndIndex());

		status = sendSynchronouslyToWaveClient("rmon", cl_msg_p);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                RMON_PLUGIN_TRACE_ERROR( string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() +  
                       FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return msg_p->getCompletionStatus();
            }

            if (cl_msg_p->getClientStatus() != 0) { 
                RMON_PLUGIN_TRACE_ERROR(string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + 
                FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				 if(cl_msg_p){
					 delete cl_msg_p;
				 }
				return msg_p->getCompletionStatus();
            }

        }
        
        if (status != WAVE_MESSAGE_SUCCESS) {
            RMON_PLUGIN_TRACE_DEBUG( string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + 
                    string("reason:") +  FrameworkToolKit::localize(status));
			if(cl_msg_p){
				delete cl_msg_p;
			}
			return status;
        }

        RMON_PLUGIN_TRACE_DEBUG( string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

        //msg_p->copyBuffersFrom(*cl_msg_p);
        msg_p->copyAllBuffers(*cl_msg_p);
		
		status = cl_msg_p->getCompletionStatus();
		delete cl_msg_p;
		return status;
    }

    void  
    RmonLocalObjectManager::RmonLocalShowMessageHandler ( RmonLocalShowMessage *pRmonLocalShowMessage)
    {

        if (pRmonLocalShowMessage->getIsGwRequest())
		{
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::sendShowCommandToCluster),
                    reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::prismLinearSequencerSucceededStep),
                    reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pRmonLocalShowMessage, 
                    this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;

        }
		else
		{ 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::sendShowToRmonDaemon),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext=
			  new PrismSynchronousLinearSequencerContext( pRmonLocalShowMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

    void
    RmonLocalObjectManager::sendRmonClearCommandToCluster( PrismLinearSequencerContext *seq_ctx_p)
    {
        RmonClearMessage *msg_p = NULL;
        RmonClearMessage *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        vector<UI32>                locations;
        msg_p = dynamic_cast<RmonClearMessage *> (seq_ctx_p->getPPrismMessage ());

        prismAssert (NULL != msg_p, __FILE__, __LINE__);
        loc_msg_p = new RmonClearMessage(msg_p->getStatsId(), msg_p->getCmdCode(), false);
        prismAssert (NULL != loc_msg_p, __FILE__, __LINE__);

        RMON_PLUGIN_TRACE_DEBUG(string("Sending Clear Command to Cluster") + string(" CmdCode =") + msg_p->getCmdCode());


        send_ctx_p = new WaveSendToClusterContext(this,
                reinterpret_cast<PrismAsynchronousCallback>
                (&RmonLocalObjectManager::RmonLocalClearSendToClusterCallback),
                seq_ctx_p);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        sendToWaveCluster(send_ctx_p);
        return;
    }

    void RmonLocalObjectManager::RmonLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        RMON_PLUGIN_TRACE_DEBUG("Entering ...");
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        RmonClearMessage 	*resp_msg_p = NULL;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        resp_msg_p =  dynamic_cast<RmonClearMessage *> (ctx_p->getPPrismMessageForPhase1());
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;
    }

    ResourceId
    RmonLocalObjectManager::sendClearToRmonDaemon( PrismSynchronousLinearSequencerContext *ctx_p)
    {
        RmonClearMessage *msg_p = dynamic_cast<RmonClearMessage *> (ctx_p->getPPrismMessage ());
        RmonClientClearMessage *cl_msg_p = NULL;
        ResourceId      status;

        cl_msg_p = new RmonClientClearMessage(msg_p->getStatsId(), msg_p->getCmdCode());;
        status = sendSynchronouslyToWaveClient("rmon", cl_msg_p);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : Sagar : Needs handling.  Otherwise please remove the status variable.
        }

        RMON_PLUGIN_TRACE_DEBUG(string("Sending Clear Command to Cluster") + string(" MsgId =")  + cl_msg_p->getMessageId()  +
                        string(" CmdCode =") + cl_msg_p->getCmdCode() + string(" StatsId") + cl_msg_p->getStatsId());

        delete cl_msg_p;
        return WAVE_MESSAGE_SUCCESS;
    }



    void RmonLocalObjectManager::RmonLocalClearMessageHandler(RmonClearMessage * pRmonClearMessage)
    {

        if (pRmonClearMessage->getIsGwRequest())

    {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::sendRmonClearCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&RmonLocalObjectManager::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pRmonClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::sendClearToRmonDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RmonLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pRmonClearMessage, this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


	ResourceId RmonLocalObjectManager::errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
        pMsg->setCompletionStatus(status);
        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                status = mapBackEndErrorToDcmdError(clientMsg->getClientStatus());
                RMON_PLUGIN_TRACE_DEBUG(string("Client status=") + status);
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
    }

    ResourceId RmonLocalObjectManager::mapBackEndErrorToDcmdError(int rc)
    {
        ResourceId errorId = WAVE_MESSAGE_SUCCESS;
		switch (rc)
		{
			case RMON_DCM_EVENT_ALREADY_EXISTS:
				errorId = WRC_RMON_EVENT_ALREADY_EXISTS;
			break;
			case RMON_DCM_CANT_SET_EVENT_COMMUNITY_WORD:
				errorId = WRC_RMON_CANT_SET_EVENT_COMMUNITY_WORD;
			break;
			case RMON_DCM_CANT_SET_EVENT_DESCRIPTION:
				errorId = WRC_RMON_CANT_SET_EVENT_DESCRIPTION;
			break;
			case RMON_DCM_CANT_SET_EVENT_OWNER_NAME:
				errorId = WRC_RMON_CANT_SET_EVENT_OWNER_NAME;
			break;
			case RMON_DCM_CANT_SET_EVENT_TYPE:
				errorId = WRC_RMON_CANT_SET_EVENT_TYPE;
			break;
			case RMON_DCM_EVENT_ACTIVATION_FAILED:
				errorId = WRC_RMON_EVENT_ACTIVATION_FAILED;
			break;
			case RMON_DCM_CANT_REMOVE_EVENT:
				errorId = WRC_RMON_CANT_REMOVE_EVENT;
			break;
			case RMON_DCM_INVALID_ALARM_VARIABLE:
				errorId = WRC_RMON_INVALID_ALARM_VARIABLE;
			break;
			case RMON_DCM_INVALID_ALARM_INPUT:
				errorId = WRC_RMON_INVALID_ALARM_INPUT;
			break;
			case RMON_DCM_INVALID_ALARM_VARIABLE_INDEX:
				errorId = WRC_RMON_INVALID_ALARM_VARIABLE_INDEX;
			break;
			case RMON_DCM_CANT_REMOVE_RMON_ALARM:
				errorId = WRC_RMON_CANT_REMOVE_RMON_ALARM;
			break;
			case RMON_DCM_ANOTHER_ALARM_ALREADY_EXISTS:
				errorId = WRC_RMON_ANOTHER_ALARM_ALREADY_EXISTS;
			break;
			
			default:
            	errorId = WAVE_MESSAGE_SUCCESS;
        	break;
		}
        return errorId;
    }
}
