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

 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalGetObjectIdWorker.h"
#include "Cluster/Local/ClusterLocalWaveSlotManagementWorker.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

namespace WaveNs
{

ClusterLocalWaveSlotManagementWorker::ClusterLocalWaveSlotManagementWorker (ClusterLocalObjectManager *pClusterLocalObjectmanager)
    : WaveWorker (pClusterLocalObjectmanager)
{
    addOperationMap (CLUSTER_LOCAL_SLOT_ONLINE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalWaveSlotManagementWorker::slotOnlineMessageHandler));
    addOperationMap (CLUSTER_LOCAL_SLOT_OFFLINE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalWaveSlotManagementWorker::slotOfflineMessageHandler));
    addOperationMap (CLUSTER_LOCAL_SLOT_REMOVE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalWaveSlotManagementWorker::slotRemoveMessageHandler));
    addOperationMap (CLUSTER_LOCAL_SLOT_REMOVE_ALL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&ClusterLocalWaveSlotManagementWorker::slotRemoveAllMessageHandler));
}

ClusterLocalWaveSlotManagementWorker::~ClusterLocalWaveSlotManagementWorker ()
{
}

ClusterLocalWaveSlotManagementWorker *ClusterLocalWaveSlotManagementWorker::getInstance ()
{
    static ClusterLocalWaveSlotManagementWorker *pVcsNodeSlotManagementWorker = (ClusterLocalObjectManager::getInstance ())->m_pClusterLocalWaveSlotManagementWorker;
    WaveNs::prismAssert (NULL != pVcsNodeSlotManagementWorker, __FILE__, __LINE__);
    return pVcsNodeSlotManagementWorker;
}

string ClusterLocalWaveSlotManagementWorker::getClassName ()
{
    return ("ClusterLocalWaveSlotManagementWorker");
}

ResourceId ClusterLocalWaveSlotManagementWorker::getSlotObjects (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    
    UI32 slotNumber = pClusterLocalMessagingContext->getSlotNumber();
    UI32 hwType = pClusterLocalMessagingContext->getHwType();
    LocationId locationId = pClusterLocalMessagingContext->getLocationId ();
    
    ObjectId waveNodeObjectId = ((ClusterLocalObjectManager::getInstance())->ClusterLocalObjectManager::m_pClusterLocalGetObjectIdWorker)->getObjectIdForLocation (locationId);

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveSlotLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::getSlotObjects: # slot objects found in DB = ")+pWaveManagedObjects->size ());
    if (0 < pWaveManagedObjects->size ())
    {
        pClusterLocalMessagingContext->setWaveSlotManagedObjectPointer(dynamic_cast<WaveSlotLocalManagedObject *> ((*pWaveManagedObjects)[0]));
    }
    else
    {
        pClusterLocalMessagingContext->setWaveSlotManagedObjectPointer (NULL);
    }
    pWaveManagedObjects->clear ();
    delete (pWaveManagedObjects);
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId ClusterLocalWaveSlotManagementWorker::validateSlotAdd (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = reinterpret_cast<ClusterLocalSlotOnlineMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pClusterLocalSlotOnlineMessage != NULL, __FILE__, __LINE__);
    UI32 slotNumberInMsg = pClusterLocalSlotOnlineMessage->getSlotNumber();
    UI32 hwTypeInMsg = pClusterLocalSlotOnlineMessage->getHwType ();
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotAdd SlotNumberInMsg = ")+slotNumberInMsg + " hwTypeInMsg = "+hwTypeInMsg);
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer();
    if (NULL != pWaveSlotLocalManagedObject)
    {
        UI32 slotNumberInDb = pWaveSlotLocalManagedObject->getSlotNumber();
        UI32 hwTypeInDb = pWaveSlotLocalManagedObject->getHwType();
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotAdd: slot object found in DB: slotNumberInDb = ")+slotNumberInDb+" hwTypeInDb = "+hwTypeInDb);
        if (pClusterLocalSlotOnlineMessage->getHwType() == pWaveSlotLocalManagedObject->getHwType())
        {
            trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotAdd: slot object in slot scn matches slot object found in DB"));
            return (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotAdd: slot object in slot scn does not match any slot object found in DB: slotNumberInMsg = ")+slotNumberInMsg+" slotNumInDb = "+slotNumberInDb+" hwTypeInMsg = "+hwTypeInMsg+" hwTypeInDb = "+hwTypeInDb);
            return (WAVE_MESSAGE_ERROR);
        }
    }
    else 
    {
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotAdd: slot object not found in DB slotNumInMsg = ")+slotNumberInMsg + " hwTypeInMsg = "+hwTypeInMsg);
        // No slot object with the slotNumber and hwType found - normal for create slot
        return WAVE_MESSAGE_SUCCESS;
    }
}

ResourceId ClusterLocalWaveSlotManagementWorker::createOrUpdateSlotManagedObject(ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = reinterpret_cast<ClusterLocalSlotOnlineMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pClusterLocalSlotOnlineMessage != NULL, __FILE__, __LINE__);
    
    UI32 slotNumber = pClusterLocalSlotOnlineMessage->getSlotNumber();
    UI32 objectType = pClusterLocalSlotOnlineMessage->getObjectType ();
    UI32 hwType = pClusterLocalSlotOnlineMessage->getHwType ();
    UI32 hwId = pClusterLocalSlotOnlineMessage->getHwId ();
    UI32 slotState = pClusterLocalSlotOnlineMessage->getSlotState ();
    UI32 reason = pClusterLocalSlotOnlineMessage->getReason ();
    UI32 version = pClusterLocalSlotOnlineMessage->getVersion ();
    
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::createOrUpdateSlotManagedObject SlotNumber = ")+slotNumber + " hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );
    
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer();
    
    if (NULL != pWaveSlotLocalManagedObject)
    {
        //slot object already exists so update it
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::createOrUpdateSlotManagedObject slot exists will be updated slotNumber = ")+slotNumber + " hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );

        if (pClusterLocalSlotOnlineMessage->getIsDummySlotUpdation ())
        {
            pWaveSlotLocalManagedObject->setObjectType (objectType);
            pWaveSlotLocalManagedObject->setHwType (hwType);
            pWaveSlotLocalManagedObject->setHwId (hwId);
        }

        pWaveSlotLocalManagedObject->setSlotState (slotState);
        pWaveSlotLocalManagedObject->setReason (reason);
        pWaveSlotLocalManagedObject->setVersion (version);
        updateWaveManagedObject (pWaveSlotLocalManagedObject);
        pClusterLocalMessagingContext->setSlotCreatedFlag (false);
    }
    else 
    {
        //slot object not found so create a new object
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::createOrUpdateSlotManagedObject slot does not exist will be created slotNumber = ")+slotNumber + " hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );
        pWaveSlotLocalManagedObject = new WaveSlotLocalManagedObject (ClusterLocalObjectManager::getInstance(), slotNumber, objectType, hwType, hwId, slotState, reason, version);
        prismAssert (NULL != pWaveSlotLocalManagedObject, __FILE__, __LINE__);
        pClusterLocalMessagingContext->setWaveSlotManagedObjectPointer(pWaveSlotLocalManagedObject);
        pClusterLocalMessagingContext->setSlotCreatedFlag (true);
    }

    pClusterLocalSlotOnlineMessage->setWaveSlotObjectId (pWaveSlotLocalManagedObject->getObjectId ());

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    UI32 slotNumber = pClusterLocalMessagingContext->getSlotNumber();
    UI32 hwType = pClusterLocalMessagingContext->getHwType();
    if (!pClusterLocalMessagingContext->getSlotCreatedFlag ())
    {
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO: No new SlotManagedObject so composition association will not be created"));
        return WAVE_MESSAGE_SUCCESS;
    }
    
    WaveNode *pWaveNode = ClusterLocalObjectManager::getInstance ()->getThisWaveNodeManagedObject ();
    if (NULL == pWaveNode)
    {
        trace (TRACE_LEVEL_FATAL, string ("ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO local WaveNode object not found so asserting, slotNumber = ")+slotNumber+" hwType = "+hwType);
        prismAssert (false, __FILE__, __LINE__);
    }
    
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer();
    
    if (NULL != pWaveSlotLocalManagedObject)
    {
        pWaveNode->addWaveSlotLocalManagedObject(pWaveSlotLocalManagedObject);
        updateWaveManagedObject (pWaveNode);    

        // Send the WaveNodeObjectId back
        ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = reinterpret_cast<ClusterLocalSlotOnlineMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());

        prismAssert (pClusterLocalSlotOnlineMessage != NULL, __FILE__, __LINE__);
    
        pClusterLocalSlotOnlineMessage->setWaveNodeObjectId (pWaveNode->getObjectId ());

        trace (TRACE_LEVEL_SUCCESS, string ("ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO waveSlot composition association added to WaveNode"));
    }
    else 
    {
        //slot object not found so create a new object
        trace (TRACE_LEVEL_FATAL, string ("ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO pPwaveSlotLocalManagedObject = NULL: should not happen so assert"));
        prismAssert (false, __FILE__, __LINE__);
    }

    pClusterLocalMessagingContext->setWaveNodePointer (pWaveNode);

    return WAVE_MESSAGE_SUCCESS;
}

void ClusterLocalWaveSlotManagementWorker::slotOnlineMessageHandler (ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage)
{
    trace (TRACE_LEVEL_DEBUG, "Entering ClusterLocalWaveSlotManagementWorker::slotOnlineMessageHandler");
    
    UI32 slotNumber = pClusterLocalSlotOnlineMessage->getSlotNumber();
    UI32 objectType = pClusterLocalSlotOnlineMessage->getObjectType();
    UI32 hwType = pClusterLocalSlotOnlineMessage->getHwType();
    UI32 hwId = pClusterLocalSlotOnlineMessage->getHwId();
    UI32 slotState = pClusterLocalSlotOnlineMessage->getSlotState();
    UI32 reason = pClusterLocalSlotOnlineMessage->getReason();
    UI32 version = pClusterLocalSlotOnlineMessage->getVersion();
    LocationId locationId = FrameworkToolKit::getThisLocationId ();
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::slotOnlineMessageHandler: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::validateSlotAdd),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::createOrUpdateSlotManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::createCompositionAssociationIfNewSlotMO),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerFailedStep)
    };

    ClusterLocalMessagingContext *pClusterLocalMessagingContext = new ClusterLocalMessagingContext (reinterpret_cast<PrismMessage *> (pClusterLocalSlotOnlineMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pClusterLocalMessagingContext->setSlotNumber(slotNumber);
    pClusterLocalMessagingContext->setHwType(hwType);
    pClusterLocalMessagingContext->setLocationId (locationId);
    pClusterLocalMessagingContext->execute ();
    
}

ResourceId ClusterLocalWaveSlotManagementWorker::updateSlotManagedObject(ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage = reinterpret_cast<ClusterLocalSlotOfflineMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pClusterLocalSlotOfflineMessage != NULL, __FILE__, __LINE__);
    
    UI32 slotNumber = pClusterLocalSlotOfflineMessage->getSlotNumber();
    UI32 hwType = pClusterLocalSlotOfflineMessage->getHwType ();
    UI32 slotState = pClusterLocalSlotOfflineMessage->getSlotState ();
    
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::updateSlotManagedObject SlotNumber = ")+slotNumber + " hwType = "+hwType+" slotState = "+slotState);
    
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer();
    
    if (NULL != pWaveSlotLocalManagedObject)
    {
        //slot object already exists so update it
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::updateSlotManagedObject slot exists will be updated slotNumber = ")+slotNumber + " hwType = "+hwType+" slotState = "+slotState);
        pWaveSlotLocalManagedObject->setSlotState (slotState); // the other fields should be unchanged
        updateWaveManagedObject (pWaveSlotLocalManagedObject);  
    }
    else 
    {
        //slot object not found so update a new object
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::updateSlotManagedObject : no slot managed object to update"));
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId ClusterLocalWaveSlotManagementWorker::validateSlotUpdate (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage = reinterpret_cast<ClusterLocalSlotOfflineMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pClusterLocalSlotOfflineMessage != NULL, __FILE__, __LINE__);
    UI32 slotNumberInMsg = pClusterLocalSlotOfflineMessage->getSlotNumber();
    UI32 hwTypeInMsg = pClusterLocalSlotOfflineMessage->getHwType();
    UI32 slotStateInMsg = pClusterLocalSlotOfflineMessage->getSlotState ();
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotUpdate SlotNumberInMsg = ")+slotNumberInMsg + " hwTypeInMsg = "+hwTypeInMsg+" slotStateInMsg = "+slotStateInMsg);
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer();
    if (NULL != pWaveSlotLocalManagedObject)
    {
        UI32 slotNumberInDb = pWaveSlotLocalManagedObject->getSlotNumber();
        UI32 hwTypeInDb = pWaveSlotLocalManagedObject->getHwType();
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotUpdate: slot object found in DB: slotNumberInDb = ")+slotNumberInDb+" hwTypeInDb = "+hwTypeInDb);
        if (pClusterLocalSlotOfflineMessage->getHwType() == pWaveSlotLocalManagedObject->getHwType())
        {
            trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotUpdate: slot object in slot scn matches slot object found in DB"));
        }
        else
        {
            // If no slot MO is found, we will log it and continue processing
            // because we still want to broadcast the offline event
            trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotUpdate: slot object in slot scn does not match any slot object found in DB: slotNumberInMsg = ")+slotNumberInMsg+" slotNumInDb = "+slotNumberInDb+" hwTypeInMsg = "+hwTypeInMsg+" hwTypeInDb = "+hwTypeInDb);
        }
    }
    else 
    {
        // No slot object with the slotNumber and hwType found
        // If no slot MO is found, we will log it and continue processing
        // because we still want to broadcast the offline event
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::validateSlotUpdate: slot object not found in DB slotNumInMsg = ")+slotNumberInMsg + " hwTypeInMsg = "+hwTypeInMsg);
    }
    return (WAVE_MESSAGE_SUCCESS);
}

void ClusterLocalWaveSlotManagementWorker::slotOfflineMessageHandler (ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage)
{
    trace (TRACE_LEVEL_DEBUG, "Entering ClusterLocalWaveSlotManagementWorker::slotOfflineMessageHandler");
    
	UI32 slotNumber = pClusterLocalSlotOfflineMessage->getSlotNumber();
    UI32 slotState = pClusterLocalSlotOfflineMessage->getSlotState();
    UI32 hwType = pClusterLocalSlotOfflineMessage->getHwType ();
    LocationId locationId = FrameworkToolKit::getThisLocationId ();
    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::slotOfflineMessageHandler: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType);

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::validateSlotUpdate),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::updateSlotManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerFailedStep)
    };

    ClusterLocalMessagingContext *pClusterLocalMessagingContext = new ClusterLocalMessagingContext (reinterpret_cast<PrismMessage *> (pClusterLocalSlotOfflineMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pClusterLocalMessagingContext->setSlotNumber(slotNumber);
    pClusterLocalMessagingContext->setHwType(hwType);
    pClusterLocalMessagingContext->setLocationId (locationId);
    pClusterLocalMessagingContext->execute ();
}

ResourceId ClusterLocalWaveSlotManagementWorker::removeSlot (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotRemoveMessage *pSlotRemoveMessage = reinterpret_cast<ClusterLocalSlotRemoveMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pSlotRemoveMessage != NULL, __FILE__, __LINE__);
    UI32 slotNumber = pSlotRemoveMessage->getSlotNumber();
    UI32 hwType = pSlotRemoveMessage->getHwType ();

    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::removeSlot: remove slot with slotNumber = ")+slotNumber+" hwType = "+hwType);
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer ();
    if (NULL != pWaveSlotLocalManagedObject)
    {
        delete (pWaveSlotLocalManagedObject);
        pClusterLocalMessagingContext->setWaveSlotManagedObjectPointer(NULL);
        trace (TRACE_LEVEL_SUCCESS, string ("ClusterLocalWaveSlotManagementWorker::removeSlot: deleted slotNumber = ")+slotNumber+" hwType = "+hwType);
        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::removeSlot: not found slotNumber = ")+slotNumber+" hwType = "+hwType);
        return WAVE_MESSAGE_ERROR;
    }
}

ResourceId ClusterLocalWaveSlotManagementWorker::removeSlotCompositionAssociation (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    UI32 slotNumber = pClusterLocalMessagingContext->getSlotNumber();
    UI32 hwType = pClusterLocalMessagingContext->getHwType();
    LocationId locationId = pClusterLocalMessagingContext->getLocationId ();

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveNode::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeLocationId (locationId, "locationId"));
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);
    WaveNode *pWaveNode = dynamic_cast<WaveNode *>((*pWaveManagedObjects)[0]);
    
    if (NULL == pWaveNode)
    {
        trace (TRACE_LEVEL_FATAL, string ("ClusterLocalWaveSlotManagementWorker::removeSlotCompositionAssociation WaveNode for locationId not found so asserting, locationId = ")+locationId);
        prismAssert (false, __FILE__, __LINE__);
    }
    
    pClusterLocalMessagingContext->setWaveNodePointer (pWaveNode);

    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pClusterLocalMessagingContext->getWaveSlotManagedObjectPointer ();
    if (NULL != pWaveSlotLocalManagedObject)
    {
        pWaveNode->removeWaveSlotLocalManagedObject(pWaveSlotLocalManagedObject);
        updateWaveManagedObject (pWaveNode);  
        pWaveManagedObjects->clear ();
        delete pWaveManagedObjects;
        trace (TRACE_LEVEL_SUCCESS, string ("ClusterLocalWaveSlotManagementWorker::removeSlotCompositionAssociation removed composition association for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId);
        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        pWaveManagedObjects->clear ();
        delete pWaveManagedObjects;
        trace (TRACE_LEVEL_ERROR, string ("ClusterLocalWaveSlotManagementWorker::removeSlotCompositionAssociation not found slotNumber = ")+slotNumber+" hwType = "+hwType);
        return WAVE_MESSAGE_ERROR;
    }
}

void ClusterLocalWaveSlotManagementWorker::slotRemoveMessageHandler (ClusterLocalSlotRemoveMessage *pSlotRemoveMessage)
{
    trace (TRACE_LEVEL_DEBUG, "Entering ClusterLocalWaveSlotManagementWorker::slotRemoveMessageHandler");
    UI32 slotNumber = pSlotRemoveMessage->getSlotNumber();
    UI32 hwType = pSlotRemoveMessage->getHwType ();
    LocationId locationId = pSlotRemoveMessage->getLocationId ();

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::removeSlotCompositionAssociation),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::removeSlot),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerFailedStep)
    };

    ClusterLocalMessagingContext *pClusterLocalMessagingContext = new ClusterLocalMessagingContext (reinterpret_cast<PrismMessage *> (pSlotRemoveMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pClusterLocalMessagingContext->setSlotNumber(slotNumber);
    pClusterLocalMessagingContext->setHwType(hwType);
    pClusterLocalMessagingContext->setLocationId (locationId);
    pClusterLocalMessagingContext->execute ();
}
   
ResourceId ClusterLocalWaveSlotManagementWorker::removeAllSlots (ClusterLocalMessagingContext *pClusterLocalMessagingContext)
{
    ClusterLocalSlotRemoveAllMessage *pSlotRemoveAllMessage = reinterpret_cast<ClusterLocalSlotRemoveAllMessage *> (pClusterLocalMessagingContext->getPPrismMessage ());
    prismAssert (pSlotRemoveAllMessage != NULL, __FILE__, __LINE__);

    trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::removeAllSlots: remove all slots "));

    WaveNode *pWaveNode = ClusterLocalObjectManager::getInstance ()->getThisWaveNodeManagedObject ();
    if (NULL != pWaveNode)
    {
        pWaveNode->removeAllWaveSlotLocalManagedObjects ();
        updateWaveManagedObject (pWaveNode);

        UI32 hwType = pSlotRemoveAllMessage->getHwType ();
        //ObjectId waveNodeObjectId = pSlotRemoveAllMessage->getWaveNodeObjectId ();
        ObjectId waveNodeObjectId = ClusterLocalObjectManager::getInstance ()->getThisWaveNodeObjectId ();

        // Retrieve the WaveSlotLocalManagedObject
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveSlotLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
        synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);
        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

        if (0 < pWaveManagedObjects->size ())
        {
            UI32 size = pWaveManagedObjects->size (); 
            for (UI32 i = 0; i < size; i++)
            {
                delete (dynamic_cast<WaveSlotLocalManagedObject *> ((*pWaveManagedObjects)[i])); 
            }
        }

        // Clean up query results
        pWaveManagedObjects->clear ();
        delete pWaveManagedObjects;
        
        pClusterLocalMessagingContext->setWaveNodePointer (pWaveNode);
        trace (TRACE_LEVEL_SUCCESS, string ("ClusterLocalWaveSlotManagementWorker::removeAllSlots: deleted all slots "));
        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("ClusterLocalWaveSlotManagementWorker::removeAllSlots: WaveNode not found "));
        return WAVE_MESSAGE_ERROR;
    }
}

void ClusterLocalWaveSlotManagementWorker::slotRemoveAllMessageHandler (ClusterLocalSlotRemoveAllMessage *pSlotRemoveAllMessage)
{
    trace (TRACE_LEVEL_DEBUG, "Entering ClusterLocalWaveSlotManagementWorker::slotRemoveAllMessageHandler");
    LocationId locationId = pSlotRemoveAllMessage->getLocationId ();

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::removeAllSlots),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ClusterLocalWaveSlotManagementWorker::prismSynchronousLinearSequencerFailedStep)
    };

    ClusterLocalMessagingContext *pClusterLocalMessagingContext = new ClusterLocalMessagingContext (reinterpret_cast<PrismMessage *> (pSlotRemoveAllMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pClusterLocalMessagingContext->setLocationId (locationId);
    pClusterLocalMessagingContext->execute ();
}

}

