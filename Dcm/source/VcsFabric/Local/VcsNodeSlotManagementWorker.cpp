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
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosScnQueue.h"
#include "VcsFabric/Local/VcsNodeFabosSlotScnQueueObject.h"
#include "VcsFabric/Local/VcsNodeSlotManagementWorker.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeSystemEvents.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/ClusterLocalWaveSlotManagementWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"
#include "utils/slot.h"

namespace DcmNs
{

static vector<UI32> s_slotNumbers;
static vector<UI32> s_hwTypes;
static vector<UI32> s_slotStates;
static vector<UI32> s_objectTypes;
static vector<UI32> s_hwIds;
static vector<UI32> s_reasons;
static vector<UI32> s_versions;

VcsNodeSlotManagementWorker::VcsNodeSlotManagementWorker (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager)
    : WaveWorker (pVcsFabricLocalObjectManager)
{

    addOperationMap (VCS_NODE_SLOT_REMOVE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeSlotManagementWorker::slotRemoveMessageHandler));
    addOperationMap (VCS_NODE_CLEAR_LINECARD_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeSlotManagementWorker::clearLinecardMessageHandler)); 
    addOperationMap (VCS_NODE_SLOT_CONFIG_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeSlotManagementWorker::slotConfigMessageHandler)); 
    addOperationMap (VCS_NODE_SLOT_SET_LINECARD_MODE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeSlotManagementWorker::slotSetLinecardModeMessageHandler));
    addOperationMap (VCS_NODE_CREATE_DUMMY_SLOT_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeSlotManagementWorker::createDummySlotMessageHandler));
    addEventType    (VCS_NODE_SLOT_ONLINE);
    addEventType    (VCS_NODE_SLOT_OFFLINE);
}


VcsNodeSlotManagementWorker::~VcsNodeSlotManagementWorker ()
{
}

VcsNodeSlotManagementWorker *VcsNodeSlotManagementWorker::getInstance ()
{
    static VcsNodeSlotManagementWorker *pVcsNodeSlotManagementWorker = (VcsFabricLocalObjectManager::getInstance ())->m_vcsNodeSlotManagementWorker;
    WaveNs::prismAssert (NULL != pVcsNodeSlotManagementWorker, __FILE__, __LINE__);
    return pVcsNodeSlotManagementWorker;
}

string VcsNodeSlotManagementWorker::getClassName ()
{
    return ("VcsNodeSlotManagementWorker");
}

PrismMessage *VcsNodeSlotManagementWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_NODE_SLOT_REMOVE_MESSAGE:
            pPrismMessage = new VcsNodeSlotRemoveMessage ();
            break;
        case VCS_NODE_CLEAR_LINECARD_MESSAGE:
            pPrismMessage = new VcsNodeClearLinecardMessage ();
            break;
        case VCS_NODE_SLOT_CONFIG_MESSAGE:
            pPrismMessage = new VcsNodeSlotConfigMessage ();
            break;
        case VCS_NODE_SLOT_SET_LINECARD_MODE_MESSAGE:
            pPrismMessage = new VcsNodeSlotSetLinecardModeMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsNodeSlotManagementWorker::broadcastSlotOnlineEvent (const VcsNodeFabosSlotOnlineEvent *&pEvent)
{
    UI32 slotNumber = pEvent->getSlotNumber();
    UI32 hwType = pEvent->getHwType();
    UI32 slotState = pEvent->getSlotState();
    UI32 objectType = pEvent->getObjectType();
    UI32 hwId = pEvent->getHwId();
    UI32 reason = pEvent->getReason();
    UI32 version = pEvent->getVersion();

    VcsNodeSlotOnlineEvent *pVcsNodeSlotOnlineEvent = new VcsNodeSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version);
    prismAssert (NULL != pVcsNodeSlotOnlineEvent, __FILE__, __LINE__);
    broadcast (pVcsNodeSlotOnlineEvent);
    trace (TRACE_LEVEL_INFO, "VcsNodeSlotManagementWorker::broadcastSlotOnlineEvent VcsNodeSlotOnlineEvent event broadcast");
    
    return;
}

void VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = reinterpret_cast<ClusterLocalSlotOnlineMessage *>(pVcsNodeSlotMessagingContext->getSlotMessage ());
    prismAssert (NULL != pClusterLocalSlotOnlineMessage, __FILE__, __LINE__);
    
	UI32 slotNumber = pClusterLocalSlotOnlineMessage->getSlotNumber();
    UI32 hwType = pClusterLocalSlotOnlineMessage->getHwType();
    UI32 slotState = pClusterLocalSlotOnlineMessage->getSlotState();
    UI32 objectType = pClusterLocalSlotOnlineMessage->getObjectType();
    UI32 hwId = pClusterLocalSlotOnlineMessage->getHwId();
    UI32 reason = pClusterLocalSlotOnlineMessage->getReason();
    UI32 version = pClusterLocalSlotOnlineMessage->getVersion();
    ResourceId status = WAVE_MESSAGE_ERROR;
    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);

    // Updation of dummy slot is not allowed. Only creation of a dummy slot is allowed. Creation and Updation of normal slot is allowed
    if ((UNKNOWN_BLADE_ID != hwId) || (NULL == pVcsSlotLocalManagedObject))
    {
        if (NULL != pVcsSlotLocalManagedObject)
        {
            if (UNKNOWN_BLADE_ID == pVcsSlotLocalManagedObject->getHwId ())
            {
                pClusterLocalSlotOnlineMessage->setIsDummySlotUpdation (true);
            }
        }

        status = sendSynchronously (pClusterLocalSlotOnlineMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep: send async slot online message failed, status = " + FrameworkToolKit::localize (status));
        }
        else
        {
            // Get the WaveSlotObjectId and WaveNodeObjectId
            pVcsNodeSlotMessagingContext->setWaveNodeObjectId (pClusterLocalSlotOnlineMessage->getWaveNodeObjectId ());
            pVcsNodeSlotMessagingContext->setWaveSlotObjectId (pClusterLocalSlotOnlineMessage->getWaveSlotObjectId ());

            status = pClusterLocalSlotOnlineMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep: ClusterLocalSlotOnlineMessage completionStatus error  = ") +FrameworkToolKit::localize (status));
            }
            else
            {
                trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep: ClusterLocalSlotOnlineMessage completionStatus = success"));
            }
        }

        delete (pClusterLocalSlotOnlineMessage);
    }
    else
    {
        //We should not allow updation of dummy slot over the existing slot in the db.
        //Erroring here as we should not go ahead with updation of dummy slots
        status = WAVE_MESSAGE_ERROR;
    }

    pVcsNodeSlotMessagingContext->executeNextStep (status);
}

void VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = reinterpret_cast<ClusterLocalSlotOnlineMessage *>(pVcsNodeSlotMessagingContext->getSlotMessage ());
    prismAssert (NULL != pClusterLocalSlotOnlineMessage, __FILE__, __LINE__);
    
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber(); 
    UI32 objectType = pVcsNodeSlotMessagingContext->getObjectType (); 
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
    UI32 hwId = pVcsNodeSlotMessagingContext->getHwId ();
    UI32 linecardMode = pVcsNodeSlotMessagingContext->getLinecardMode ();
    UI32 slotState = pVcsNodeSlotMessagingContext->getSlotState ();
    UI32 reason = pVcsNodeSlotMessagingContext->getReason ();
    UI32 version = pVcsNodeSlotMessagingContext->getVersion ();
    string linecardName = string("") + slotNumber;

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject: linecardName = ")+linecardName+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );

    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer();

    if (NULL != pVcsSlotLocalManagedObject)
    {   
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject : slot exists, will be updated - linecardName = " )+linecardName+" slotNumber = "+slotNumber + " hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );

        // Updation of the following fields are allowed only if we have a dummy slot in the DB.
        if (UNKNOWN_BLADE_ID == pVcsSlotLocalManagedObject->getHwId ())
        {
            pVcsSlotLocalManagedObject->setObjectType (objectType);
            pVcsSlotLocalManagedObject->setHwType (hwType);
            pVcsSlotLocalManagedObject->setHwId (hwId);
            pVcsSlotLocalManagedObject->setLinecardType (getLinecardTypeFromHwId (hwId));
            pVcsSlotLocalManagedObject->setLinecardMode (linecardMode);
        }

        pVcsSlotLocalManagedObject->setSlotState (slotState);
        pVcsSlotLocalManagedObject->setReason (reason);
        pVcsSlotLocalManagedObject->setVersion (version);
        updateWaveManagedObject (pVcsSlotLocalManagedObject);
        pVcsNodeSlotMessagingContext->setSlotCreatedFlag (false);
    }
    else
    {
        // slot object not found so create a new object
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject : slot does not exist will be created - linecardName = " )+linecardName+" slotNumber = "+slotNumber + " hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" slotState = "+slotState+" reason = "+reason+" version = "+version );
        pVcsSlotLocalManagedObject = new VcsSlotLocalManagedObject (VcsFabricLocalObjectManager::getInstance(), linecardName, slotNumber, objectType, hwType, hwId, slotState, reason, version); 
        prismAssert (NULL != pVcsSlotLocalManagedObject, __FILE__, __LINE__);

        pVcsSlotLocalManagedObject->setLinecardType (getLinecardTypeFromHwId(hwId));
        pVcsSlotLocalManagedObject->setLinecardMode (linecardMode);
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer(pVcsSlotLocalManagedObject);
        pVcsNodeSlotMessagingContext->setSlotCreatedFlag (true);
    }

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();

    if (!pVcsNodeSlotMessagingContext->getSlotCreatedFlag ())
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO: No new SlotManagedObject so composition association will not be created"));
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    } 

    // Retrieve the VcsNodeLocalManagedObject
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    vector<WaveManagedObject *> *pWaveManagedObjects = NULL;
    bool isLocalLocationId = false;

    if (locationId == FrameworkToolKit::getMyLocationId ())
    {
        pVcsNodeLocalManagedObject = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObject ();
        isLocalLocationId = true;
        VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.lock();   
    }
    else
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsNodeLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeLocationId (locationId, "locationId"));
    
        pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);

        if (0 < pWaveManagedObjects->size ())
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[0]);

            pVcsNodeSlotMessagingContext->setVcsNodeManagedObjectPointer (pVcsNodeLocalManagedObject);

            for (UI32 i = 1; i < pWaveManagedObjects->size (); i++)
            {
                delete (dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[i]));
            }
        }
    }

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO local VcsNodeLocalManagedObject object not found so asserting, slotNumber = ")+slotNumber+" hwType = "+hwType);

        prismAssert (false, __FILE__, __LINE__);
    }

    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer();

    if (NULL != pVcsSlotLocalManagedObject)
    {
        pVcsNodeLocalManagedObject->addVcsSlotLocalManagedObject(pVcsSlotLocalManagedObject);
        updateWaveManagedObject (pVcsNodeLocalManagedObject);

        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO VcsSlot composition association added to VcsNodeLocalManagedObject"));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO pVcsSlotLocalManagedObject = NULL: should not happen so assert"));

        prismAssert (false, __FILE__, __LINE__);
    }

    if (pWaveManagedObjects != NULL)
    {
        pWaveManagedObjects->clear ();
        delete (pWaveManagedObjects);
    }

    if (isLocalLocationId)
    {
        VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    }

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::createAssociationWithWaveSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    ObjectId waveNodeObjectId = pVcsNodeSlotMessagingContext->getWaveNodeObjectId ();
    ObjectId waveSlotObjectId = pVcsNodeSlotMessagingContext->getWaveSlotObjectId ();

    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer();

    if (NULL != pVcsSlotLocalManagedObject)
    {
        if (!pVcsNodeSlotMessagingContext->getSlotCreatedFlag ())
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createAssociationWithWaveSlot: No new SlotManagedObject so association to WaveSlotLocalManagedObject will not be created"));
        } 
        else
        {
            if (ObjectId::NullObjectId != waveSlotObjectId) 
            {
                pVcsSlotLocalManagedObject->setWaveSlotObjectId (waveSlotObjectId);
            }
            else 
            {
                trace (TRACE_LEVEL_FATAL, "VcsNodeSlotManagementWorker::createAssociationWithWaveSlot : The WaveSlotObjectId to be associated with does not exist. Asserting now ... ");
        
                prismAssert (false, __FILE__, __LINE__);
            }

            if (ObjectId::NullObjectId != waveNodeObjectId)
            {                
                pVcsSlotLocalManagedObject->setWaveNodeObjectId (waveNodeObjectId);
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "VcsNodeSlotManagementWorker::createAssociationWithWaveSlot : The OwnerWaveNodeObjectId to be associated with does not exist. Asserting now ... ");

                prismAssert (false, __FILE__, __LINE__);
            }

            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::createAssociationWithWaveSlot : VcsSlot association to WaveSlotLocalManagedObject"));
        } 
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsNodeSlotManagementWorker::createAssociationWithWaveSlot pVcsSlotLocalManagedObject = NULL: should not happen so assert"));

        prismAssert (false, __FILE__, __LINE__);
    }

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::slotOnlineEventHandler (const VcsNodeFabosSlotOnlineEvent *&pOnlineEvent)
{
    prismAssert (NULL != pOnlineEvent, __FILE__, __LINE__);

    if (false == VcsFabricLocalObjectManager::getInstance ()->isNodeReadyForSlotSCNHandling ())
    {    
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::slotOnlineEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pOnlineEvent));
        return;
    }    
    
    ResourceId status = WAVE_MESSAGE_ERROR;
    UI32 eventId = pOnlineEvent->getEventId();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPSlotScnQueueObject (eventId);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::slotOnlineEventHandler: slot onLine event not in queue, may have been handled already eventId = ")+eventId);

        reply (reinterpret_cast<const PrismEvent *&> (pOnlineEvent));
        return;
    }

    slotCreationHandler (pOnlineEvent);
    broadcastSlotOnlineEvent (pOnlineEvent);

    reply (reinterpret_cast<const PrismEvent *&> (pOnlineEvent));
}

void VcsNodeSlotManagementWorker::slotCreationHandler (const VcsNodeFabosSlotOnlineEvent *&pOnlineEvent)
{
    prismAssert (NULL != pOnlineEvent, __FILE__, __LINE__);
    
    static UI32 numSlotOnlineEvents = 0;
    numSlotOnlineEvents++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotCreationHandler with async send event count = ")+numSlotOnlineEvents);

    UI32 slotNumber = pOnlineEvent->getSlotNumber();
    UI32 objectType = pOnlineEvent->getObjectType();
    UI32 hwType = pOnlineEvent->getHwType();
    UI32 hwId = pOnlineEvent->getHwId();
    UI32 slotState = pOnlineEvent->getSlotState();
    UI32 reason = pOnlineEvent->getReason();
    UI32 version = pOnlineEvent->getVersion();
    
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = new ClusterLocalSlotOnlineMessage (slotNumber, objectType, hwType, hwId, slotState, reason, version);
    prismAssert (NULL != pClusterLocalSlotOnlineMessage, __FILE__, __LINE__);
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotCreationHandler: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createAssociationWithWaveSlot),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (NULL , this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);

    pVcsNodeSlotMessagingContext->setSlotMessage (pClusterLocalSlotOnlineMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber); 
    pVcsNodeSlotMessagingContext->setRbridgeId (DcmToolKit::getThisVcsNodeDomainId ());
    pVcsNodeSlotMessagingContext->setObjectType (objectType);
    pVcsNodeSlotMessagingContext->setHwType (hwType); 
    pVcsNodeSlotMessagingContext->setHwId (hwId); 
    pVcsNodeSlotMessagingContext->setSlotState (slotState);
    pVcsNodeSlotMessagingContext->setReason (reason);
    pVcsNodeSlotMessagingContext->setVersion (version);
    pVcsNodeSlotMessagingContext->setLocationId (FrameworkToolKit::getThisLocationId ()); 

    // The linecard mode is only set to default if there are no existing linecard
    // configuration.  If there is an existing linecard configuration, the mode is
    // not updated.  
    pVcsNodeSlotMessagingContext->setLinecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT);

    pVcsNodeSlotMessagingContext->holdAll ();
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage = reinterpret_cast<ClusterLocalSlotOfflineMessage *>(pVcsNodeSlotMessagingContext->getSlotMessage ());

    prismAssert (NULL != pClusterLocalSlotOfflineMessage, __FILE__, __LINE__);
    
	UI32 slotNumber = pClusterLocalSlotOfflineMessage->getSlotNumber();
    UI32 hwType = pClusterLocalSlotOfflineMessage->getHwType();
    UI32 slotState = pClusterLocalSlotOfflineMessage->getSlotState();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType);
    
    ResourceId status = sendSynchronously (pClusterLocalSlotOfflineMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep: send async slot event message failed, status = " + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep: send async slot event message successfully sent");

        status = pClusterLocalSlotOfflineMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep: ClusterLocalSlotOfflineMessage completionStatus error  = ") +FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep: ClusterLocalSlotOfflineMessage completionStatus = success"));
        }
    }

    delete (pClusterLocalSlotOfflineMessage);

    pVcsNodeSlotMessagingContext->executeNextStep (status);
}

void VcsNodeSlotManagementWorker::broadcastSlotOfflineEventStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 slotState = pVcsNodeSlotMessagingContext->getSlotState();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType ();

    VcsNodeSlotOfflineEvent *pVcsNodeSlotOfflineEvent = new VcsNodeSlotOfflineEvent (slotNumber, hwType, slotState);
    prismAssert (NULL != pVcsNodeSlotOfflineEvent, __FILE__, __LINE__);
    broadcast (pVcsNodeSlotOfflineEvent);
    trace (TRACE_LEVEL_INFO, "VcsNodeSlotManagementWorker::broadcastSlotOfflineEventStep VcsNodeSlotOfflineEvent event broadcast");
    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::slotOfflineEventHandler (const VcsNodeFabosSlotOfflineEvent *&pSlotOfflineEvent)
{
    prismAssert (NULL != pSlotOfflineEvent, __FILE__, __LINE__);

    if (false == VcsFabricLocalObjectManager::getInstance ()->isNodeReadyForSlotSCNHandling ())
    {    
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::slotOfflineEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pSlotOfflineEvent));
        return;
    }    

    ResourceId status  = WAVE_MESSAGE_ERROR;
    UI32       eventId = pSlotOfflineEvent->getEventId();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPSlotScnQueueObject (eventId);   

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::slotOfflineEventHandler: slot offline event not in queue, may have been handled already eventId = ")+eventId);
        reply (reinterpret_cast<const PrismEvent *&> (pSlotOfflineEvent));
        return;
    }
    static UI32 numSlotOfflineEvents = 0;
    numSlotOfflineEvents++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotOfflineEventHandler Event count = ")+numSlotOfflineEvents);

    UI32 slotNumber = pSlotOfflineEvent->getSlotNumber();
    UI32 hwType = pSlotOfflineEvent->getHwType ();
    UI32 slotState = pSlotOfflineEvent->getSlotState();

    VcsNodeSlotOfflineEvent *pVcsNodeSlotOfflineEvent = new VcsNodeSlotOfflineEvent (slotNumber, hwType, slotState);
    prismAssert (NULL != pVcsNodeSlotOfflineEvent, __FILE__, __LINE__);
    broadcast (pVcsNodeSlotOfflineEvent);
    trace (TRACE_LEVEL_INFO, "VcsNodeSlotManagementWorker::broadcastSlotOfflineEventStep VcsNodeSlotOfflineEvent event broadcast");

    slotDeleteEventHandler (pSlotOfflineEvent);
    reply (reinterpret_cast<const PrismEvent *&> (pSlotOfflineEvent));
}

void VcsNodeSlotManagementWorker::slotDeleteEventHandler (const VcsNodeFabosSlotOfflineEvent *&pSlotOfflineEvent)
{
    UI32 slotNumber = pSlotOfflineEvent->getSlotNumber();
    UI32 objectType = pSlotOfflineEvent->getObjectType();
    UI32 hwType = pSlotOfflineEvent->getHwType ();
    UI32 hwId = pSlotOfflineEvent->getHwId();
    UI32 slotState = pSlotOfflineEvent->getSlotState();
    UI32 reason = pSlotOfflineEvent->getReason();
    UI32 version = pSlotOfflineEvent->getVersion();

    ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage = new ClusterLocalSlotOfflineMessage (slotNumber, hwType, slotState);

    prismAssert (NULL != pClusterLocalSlotOfflineMessage, __FILE__, __LINE__);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createSlotZero),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotOfflineMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::resetLinecardReadyBitStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::updateSlotManagedObject),
        //reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::broadcastSlotOfflineEventStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (NULL , this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);

    pVcsNodeSlotMessagingContext->setSlotMessage (pClusterLocalSlotOfflineMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setObjectType (objectType);
    pVcsNodeSlotMessagingContext->setHwType (hwType);
    pVcsNodeSlotMessagingContext->setHwId (hwId);
    pVcsNodeSlotMessagingContext->setSlotState (slotState);
    pVcsNodeSlotMessagingContext->setReason (reason);
    pVcsNodeSlotMessagingContext->setVersion (version);
    pVcsNodeSlotMessagingContext->setLocationId (FrameworkToolKit::getThisLocationId ());
    pVcsNodeSlotMessagingContext->holdAll ();
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeSlotManagementWorker::slotFailoverToPluginsCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    ResourceId completionStatus;
    FrameworkObjectManagerStartSlotFailoverMessage *pFrameworkObjectManagerStartSlotFailoverMessage = reinterpret_cast<FrameworkObjectManagerStartSlotFailoverMessage *> (pPrismMessage);
    
    if (FRAMEWORK_SUCCESS == frameworkStatus) 
    {
        prismAssert ((NULL != pFrameworkObjectManagerStartSlotFailoverMessage), __FILE__, __LINE__);
        completionStatus =  pFrameworkObjectManagerStartSlotFailoverMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotFailoverToPluginsCallback : ClusterLocalSlotOnlineMessage completionStatus error  = ") +FrameworkToolKit::localize (completionStatus));
        } 
        else 
        {
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::slotFailoverToPluginsCallback : ClusterLocalSlotOnlineMessage completionStatus = success"));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotFailoverToPluginsCallback : framework status error = ")+ FrameworkToolKit::localize (frameworkStatus));
        pVcsNodeSlotMessagingContext->incrementNumberOfFailures ();
    }
    
    --(*pVcsNodeSlotMessagingContext);
    delete (pFrameworkObjectManagerStartSlotFailoverMessage);
   
    if ((pVcsNodeSlotMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep () == 0) && (pVcsNodeSlotMessagingContext->getNumberOfFailures () > 0))
    {
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
    else
    {
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);      
    }
    
}

void VcsNodeSlotManagementWorker::sendSlotFailoverToPluginsStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    FrameworkObjectManagerStartSlotFailoverMessage *pFrameworkObjectManagerStartSlotFailoverMessage = new FrameworkObjectManagerStartSlotFailoverMessage (pVcsNodeSlotMessagingContext->getSlotNumber ());
    ++(*pVcsNodeSlotMessagingContext);
    
    ResourceId status = send (pFrameworkObjectManagerStartSlotFailoverMessage, 
        reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeSlotManagementWorker::slotFailoverToPluginsCallback), 
        pVcsNodeSlotMessagingContext, 0, FrameworkToolKit::getThisLocationId ());
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::sendSlotFailoverToPluginsStep : send async slot failover message, status = " + FrameworkToolKit::localize (status));
        --(*pVcsNodeSlotMessagingContext);
        return;
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "VcsNodeSlotManagementWorker::sendSlotFailoverToPluginsStep : send async slot failover message successfully sent");
        return;
    }
}

void VcsNodeSlotManagementWorker::sendSlotRemoveMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber ();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType ();
    
    ClusterLocalSlotRemoveMessage *pClusterLocalSlotRemoveMessage = new ClusterLocalSlotRemoveMessage (slotNumber,hwType, locationId);

    ResourceId status = sendSynchronously (pClusterLocalSlotRemoveMessage, FrameworkToolKit::getThisLocationId ());
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::sendSlotRemoveMessageStep: send async slot remove message failed, status = " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = pClusterLocalSlotRemoveMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::sendSlotRemoveMessageStep: ClusterLocalSlotRemoveMessage completionStatus error  = ") +FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::sendSlotRemoveMessageStep: ClusterLocalSlotRemoveMessage completionStatus = success"));
        }
    }
   
    delete (pClusterLocalSlotRemoveMessage);

    pVcsNodeSlotMessagingContext->executeNextStep (status);
}

void VcsNodeSlotManagementWorker::validateRemoveSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getWaveSlotManagedObjectPointer ();
    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer ();
    if (NULL != pVcsSlotLocalManagedObject)
    {
        UI32 slotState = pVcsSlotLocalManagedObject->getSlotState();
        if ((SLOT_IN == slotState) || (SLOT_OUT == slotState))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::validateRemoveSlot remove slot validation successful for slotNumber = ")+slotNumber+" hwType = "+hwType);
            // delete slot object here to prevent possible double delete in Cluster/Local due to race conditions
            if (NULL != pWaveSlotLocalManagedObject)
            {
                delete (pWaveSlotLocalManagedObject);
                pVcsNodeSlotMessagingContext->setWaveSlotManagedObjectPointer (NULL);
            }

            pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            VcsNodeClearLinecardMessage *pVcsNodeClearLinecardMessage = reinterpret_cast<VcsNodeClearLinecardMessage *> (pVcsNodeSlotMessagingContext->getPPrismMessage());
            pVcsNodeClearLinecardMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_WRONG_STATE_ERROR);
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::validateRemoveSlot cannot remove slot in SLOT_ON state slotNumber = ")+slotNumber+" hwType = "+hwType);
            // delete slot object here to prevent possible double delete in Cluster/Local due to race conditions
            if (NULL != pWaveSlotLocalManagedObject)
            {
                delete (pWaveSlotLocalManagedObject);
                pVcsNodeSlotMessagingContext->setWaveSlotManagedObjectPointer (NULL);
            }

            pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::validateRemoveSlot slot not found slotNumber = ")+slotNumber+" hwType = "+hwType);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void VcsNodeSlotManagementWorker::getSlotObjects (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeSlotManagementWorker::getSlotObjects: Entering... "));
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getSlotObjects: quering slot object for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());

    // Retrieve the VcsSlotLocalManagedObject
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);
    if (0 < pWaveManagedObjects->size ())
    {
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer(dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[0]));

        UI32 size = pWaveManagedObjects->size ();
        for (UI32 i = 1; i < size; i++)
        {   
            delete (dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[i]));
        }
    }
    else
    {
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer (NULL);
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getSlotObjects: No vcs slot objects found for query slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());
    }
    pWaveManagedObjects->clear ();
    delete (pWaveManagedObjects);

    // Retrieve the WaveSlotLocalManagedObject
    WaveManagedObjectSynchronousQueryContext waveSynchronousQueryContext (WaveSlotLocalManagedObject::getClassName ());

    waveSynchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
    waveSynchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    waveSynchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    pWaveManagedObjects = querySynchronously (&waveSynchronousQueryContext);
    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);
    if (0 < pWaveManagedObjects->size ())
    {
        pVcsNodeSlotMessagingContext->setWaveSlotManagedObjectPointer(dynamic_cast<WaveSlotLocalManagedObject *> ((*pWaveManagedObjects)[0]));
        UI32 size = pWaveManagedObjects->size ();

        for (UI32 i = 1; i < size; i++)
        {   
            delete (dynamic_cast<WaveSlotLocalManagedObject *> ((*pWaveManagedObjects)[i]));
        }
    }
    else
    {
        pVcsNodeSlotMessagingContext->setWaveSlotManagedObjectPointer (NULL);
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getSlotObjects: No wave slot objects found for query slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());
    }
    pWaveManagedObjects->clear ();
    delete (pWaveManagedObjects);

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::resetLinecardReadyBitStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeSlotManagementWorker::resetLinecardReadyBitStep: Entering... "));
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    
    VcsFabricLocalObjectManager::getInstance ()->m_linecardReadySlotIdBitMap.reset (slotNumber);
    VcsFabricLocalObjectManager::getInstance ()->m_linecardReadyPostBootCompleteBitMap.reset (slotNumber);

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::clearLinecardMessageHandler (VcsNodeClearLinecardMessage *pVcsNodeclearLinecardMessage)
{
    UI32 slotNumber = pVcsNodeclearLinecardMessage->getSlotNumber();
    UI32 hwType = pVcsNodeclearLinecardMessage->getHwType ();
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::clearLinecardMessageHandler SlotNumber for remove slot = ")+slotNumber + " hwType = "+hwType);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::validateRemoveSlot),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotFailoverToPluginsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotRemoveMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::removeSlotCompositionAssociation),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::removeSlot),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeclearLinecardMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);
    
    pVcsNodeSlotMessagingContext->setSlotMessage (pVcsNodeclearLinecardMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setHwType (hwType);
    pVcsNodeSlotMessagingContext->setLocationId (FrameworkToolKit::getThisLocationId ()); // this should be the surrogate locationId if the remove request is for another node
    
// TODO
//    pVcsNodeSlotMessagingContext->holdAll ();
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeSlotManagementWorker::removeSlotViaSendToWaveClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = reinterpret_cast<VcsNodeSlotMessagingContext *>(pWaveSendToClusterContext->getPCallerContext());
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationId);
    
    VcsNodeClearLinecardMessage *pVcsNodeClearLinecardMessage = reinterpret_cast<VcsNodeClearLinecardMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locationId));
    VcsNodeSlotRemoveMessage *pVcsNodeSlotRemoveMessage = reinterpret_cast<VcsNodeSlotRemoveMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());
    pVcsNodeSlotRemoveMessage->setReturnStatus (pVcsNodeClearLinecardMessage->getReturnStatus ());

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1());
    delete (pWaveSendToClusterContext);
 
    pVcsNodeSlotMessagingContext->executeNextStep (sendToClusterCompletionStatus);
}

void VcsNodeSlotManagementWorker::removeSlotViaSendToWaveCluster (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
    UI32 rbridgeId = pVcsNodeSlotMessagingContext->getRbridgeId ();
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    vector<LocationId> locationsToSendToForPhase1;
    locationsToSendToForPhase1.push_back (locationId);
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::removeSlotViaSendToWaveCluster SlotNumber for remove slot = ")+slotNumber + " hwType = "+hwType+" rbridgeId = "+rbridgeId+" locationId = "+locationId);
    
    VcsNodeClearLinecardMessage *pVcsNodeClearLinecardMessage = new VcsNodeClearLinecardMessage (slotNumber, hwType);
    prismAssert (NULL != pVcsNodeClearLinecardMessage, __FILE__, __LINE__);
    pVcsNodeClearLinecardMessage->setNeedSurrogateSupportFlag (true);
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (VcsNodeSlotManagementWorker::getInstance (), reinterpret_cast<PrismAsynchronousCallback> (&VcsNodeSlotManagementWorker::removeSlotViaSendToWaveClusterCallback), pVcsNodeSlotMessagingContext);
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);
    pWaveSendToClusterContext->setPPrismMessageForPhase1(pVcsNodeClearLinecardMessage);
    pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationsToSendToForPhase1);
    sendToWaveCluster (pWaveSendToClusterContext);
}

void VcsNodeSlotManagementWorker::slotRemoveMessageHandler (VcsNodeSlotRemoveMessage *pVcsNodeSlotRemoveMessage)
{
    UI32 slotNumber = pVcsNodeSlotRemoveMessage->getSlotNumber();
    UI32 hwType = pVcsNodeSlotRemoveMessage->getHwType ();
    UI32 rbridgeId = pVcsNodeSlotRemoveMessage->getRbridgeId ();
    LocationId locationId = DcmToolKit::getLocationIdFromMappedId (rbridgeId);
    if (0 == locationId)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotRemoveMessageHandler: node not found for rbridgeId = ")+rbridgeId+" locationId = "+locationId);
        pVcsNodeSlotRemoveMessage->setCompletionStatus (LINECARD_MANAGEMENT_FAILED_RBRIDGEID_ERROR);
        pVcsNodeSlotRemoveMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_RBRIDGEID_ERROR);
        reply (pVcsNodeSlotRemoveMessage);
        return;
    }

    // Check for valid slot numbers
    UI32 numSlots = 0;
    numSlots = chassisNumSlots ();
    UI32 numLinecards = 0;
    UI32 slotType = SYS_SLOT_UNKNOWN;

    while ((numSlots > 0) && (slotType != SYS_SLOT_LC))
    {
        slot_no2id((int)numSlots, (int *)(&slotType), (int *)(&numLinecards));
        numSlots--;
    }

    if ((slotNumber < 1) || (slotNumber > numLinecards))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotRemoveMessageHandler: Invalid slot number: ") + slotNumber + ", maximum slot number is: " + numSlots);
        pVcsNodeSlotRemoveMessage->setCompletionStatus (LINECARD_MANAGEMENT_FAILED_SLOT_NUM_ERROR);
        pVcsNodeSlotRemoveMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_SLOT_NUM_ERROR);
        reply (pVcsNodeSlotRemoveMessage);
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotRemoveMessageHandler SlotNumber for remove slot = ")+slotNumber + " hwType = "+hwType+" rbridgeId = "+rbridgeId+" locationId = "+locationId);
    
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::removeSlotViaSendToWaveCluster),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };
    
    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSlotRemoveMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);
    pVcsNodeSlotMessagingContext->setSlotMessage (pVcsNodeSlotRemoveMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setHwType (hwType);
    pVcsNodeSlotMessagingContext->setRbridgeId (rbridgeId);
    pVcsNodeSlotMessagingContext->setLocationId (locationId);
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeSlotManagementWorker::replaySlotScnQueueEvents ()
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: Entering...");
    VcsNodeFabosSlotScnQueueObject *pVcsNodeFabosSlotScnQueueObject = NULL;
        
	UI32 slotNumber;
    UI32 hwType;
    UI32 slotState;
    UI32 objectType;
    UI32 hwId;
    UI32 reason;
    UI32 version;
    UI32 eventId = 0;
        
    trace (TRACE_LEVEL_INFO,string ("VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: SlotScnQsize = ")+ VcsNodeFabosInterfaceObjectManager::getInstance ()->slotScnQueueSize ());
        
        while (0 != VcsNodeFabosInterfaceObjectManager::getInstance ()->slotScnQueueSize ())
        {
            pVcsNodeFabosSlotScnQueueObject = VcsNodeFabosInterfaceObjectManager::getInstance ()->getFrontSlotScnQueueObject();
            slotNumber = pVcsNodeFabosSlotScnQueueObject->getSlotNumber ();
            hwType = pVcsNodeFabosSlotScnQueueObject->getHwType ();
            slotState = pVcsNodeFabosSlotScnQueueObject->getSlotState ();
            objectType = pVcsNodeFabosSlotScnQueueObject->getObjectType ();
            hwId = pVcsNodeFabosSlotScnQueueObject->getHwId ();
            reason = pVcsNodeFabosSlotScnQueueObject->getReason ();
            version = pVcsNodeFabosSlotScnQueueObject->getVersion ();
        eventId = pVcsNodeFabosSlotScnQueueObject->getSlotScnObjectId();

            switch (slotState)
            {
                case SLOT_ON:
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: Replaying Slot Online event for slot number = ")+ slotNumber);
                const VcsNodeFabosSlotOnlineEvent *pSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
                    slotOnlineEventHandler (pSlotOnlineEvent);
                    break;
                }

                case SLOT_IN:
                case SLOT_OUT:
                case SLOT_FAULT:
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: Replaying Slot Offline event for slot number = ")+ slotNumber);
                const VcsNodeFabosSlotOfflineEvent *pSlotOfflineEvent = new VcsNodeFabosSlotOfflineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
                    slotOfflineEventHandler(pSlotOfflineEvent);
                    break;
                }

                case SLOT_READY:
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: Replaying Slot READY event for slot number = ")+ slotNumber);
                    ResourceId recoveryType = reason;
                const VcsNodeFabosLinecardReadyEvent *pLinecardReadyEvent = new VcsNodeFabosLinecardReadyEvent (slotNumber, recoveryType, eventId);

                    VcsFabricLocalObjectManager::getInstance ()->linecardReadyEventHandler (pLinecardReadyEvent);
                    break;
                }

                default:
                    trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::replaySlotScnQueueEvents: Unknown SlotScn type = ")+ slotState+ " for slot number = " + slotNumber);
                VcsNodeFabosInterfaceObjectManager::getInstance ()->popPSlotScnQueueObject (eventId); // pop this unhandled event
                    break;
            }
        }
}

void VcsNodeSlotManagementWorker::updateSlotManagedObject (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeSlotManagementWorker::updateSlotManagedObject: Entering...");

    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    UI32 hwType = pVcsNodeSlotMessagingContext->getHwType ();
    UI32 slotState = pVcsNodeSlotMessagingContext->getSlotState ();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::updateSlotManagedObject SlotNumber = ")+slotNumber + " hwType = "+hwType+" slotState = "+slotState);

    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer();

    if (NULL != pVcsSlotLocalManagedObject)
    {
        // slot object already exists so update it
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::updateSlotManagedObject slot exists will be updated slotNumber = ")+slotNumber+" hwType = "+hwType+" slotState = "+slotState);
        pVcsSlotLocalManagedObject->setSlotState (slotState); // the other fields should be unchanged
        updateWaveManagedObject (pVcsSlotLocalManagedObject);
    }
    else
    {
        // slot object not found 
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::updateSlotManagedObject : no slot managed object to update"));
    }
    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeSlotManagementWorker::removeSlotCompositionAssociation (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    VcsNodeClearLinecardMessage *pVcsNodeClearLinecardMessage = reinterpret_cast<VcsNodeClearLinecardMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());

    prismAssert (NULL != pVcsNodeClearLinecardMessage, __FILE__, __LINE__);

    UI32 slotNumber = pVcsNodeClearLinecardMessage->getSlotNumber();
    UI32 hwType = pVcsNodeClearLinecardMessage->getHwType ();
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    vector<WaveManagedObject *> *pWaveManagedObjects = NULL;
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isLocalLocationId = false;

    if (locationId == FrameworkToolKit::getMyLocationId ())
    {
        pVcsNodeLocalManagedObject = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObject ();
        isLocalLocationId = true;
        VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.lock();   
    }
    else
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsNodeLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeLocationId (locationId, "locationId"));

        pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);

        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pWaveManagedObjects)[0]);
    }

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsNodeSlotManagementWorker::removeSlotCompositionAssociation VcsNodeLocalManagedObject for locationId not found so asserting, locationId = ")+locationId);

        prismAssert (false, __FILE__, __LINE__);
    }

    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer ();

    if (NULL != pVcsSlotLocalManagedObject)
    {
        pVcsNodeLocalManagedObject->removeVcsSlotLocalManagedObject(pVcsSlotLocalManagedObject);

        updateWaveManagedObject (pVcsNodeLocalManagedObject);

        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::removeSlotCompositionAssociation removed composition association for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId);

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::removeSlotCompositionAssociation not found slotNumber = ")+slotNumber+" hwType = "+hwType);

        status = WAVE_MESSAGE_ERROR;
    }

    if (pWaveManagedObjects != NULL)
    {
        pWaveManagedObjects->clear ();

        delete pWaveManagedObjects;
    }

    if (isLocalLocationId)
    {
        VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    }
    else
    {
        if (pVcsNodeLocalManagedObject != NULL)
        {
            delete (pVcsNodeLocalManagedObject);
        }
    }

    pVcsNodeSlotMessagingContext->executeNextStep (status);
    return;
}

void VcsNodeSlotManagementWorker::removeSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    VcsNodeClearLinecardMessage *pVcsNodeClearLinecardMessage = reinterpret_cast<VcsNodeClearLinecardMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());
    prismAssert (NULL != pVcsNodeClearLinecardMessage, __FILE__, __LINE__);

    UI32 slotNumber = pVcsNodeClearLinecardMessage->getSlotNumber();
    UI32 hwType = pVcsNodeClearLinecardMessage->getHwType ();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::removeSlot: remove slot with slotNumber = ")+slotNumber+" hwType = "+hwType);
    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer ();
    if (NULL != pVcsSlotLocalManagedObject)
    {
        delete (pVcsSlotLocalManagedObject);
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer(NULL);
        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::removeSlot: deleted slotNumber = ")+slotNumber+" hwType = "+hwType);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }
    else
    {
        pVcsNodeClearLinecardMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_MISSING_SLOT_ERROR);
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::removeSlot: not found slotNumber = ")+slotNumber+" hwType = "+hwType);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
}

void VcsNodeSlotManagementWorker::configSlotViaSendToWaveClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeSlotManagementWorker::configSlotViaSendToWaveClusterCallback : Entering ...");

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = reinterpret_cast<VcsNodeSlotMessagingContext *>(pWaveSendToClusterContext->getPCallerContext());
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationId);
    
    VcsNodeSlotSetLinecardModeMessage *pVcsNodeSlotSetLinecardModeMessage = reinterpret_cast<VcsNodeSlotSetLinecardModeMessage *>(pWaveSendToClusterContext->getResultingMessageForPhase1(locationId));
    VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage = reinterpret_cast<VcsNodeSlotConfigMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());
    pVcsNodeSlotConfigMessage->setReturnStatus (pVcsNodeSlotSetLinecardModeMessage->getReturnStatus ());

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1());
    delete (pWaveSendToClusterContext);
 
    pVcsNodeSlotMessagingContext->executeNextStep (sendToClusterCompletionStatus);
}

void VcsNodeSlotManagementWorker::configSlotViaSendToWaveCluster (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage = reinterpret_cast<VcsNodeSlotConfigMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());
    UI32 slotNumber = pVcsNodeSlotConfigMessage->getSlotNumber();
    UI32 hwId = pVcsNodeSlotConfigMessage->getHwId ();
    UI32 linecardMode = pVcsNodeSlotConfigMessage->getLinecardMode ();
    UI32 hwType = pVcsNodeSlotConfigMessage->getHwType ();
    UI32 rbridgeId = pVcsNodeSlotMessagingContext->getRbridgeId ();
    LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
    vector<LocationId> locationsToSendToForPhase1;
    locationsToSendToForPhase1.push_back (locationId);
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::configSlotViaSendToWaveCluster SlotNumber for config slot = ")+slotNumber+" hwId = "+hwId+" linecardMode = "+linecardMode+" hwType = "+hwType+" rbridgeId = "+rbridgeId+" locationId = "+locationId);
    
    VcsNodeSlotSetLinecardModeMessage *pVcsNodeSlotSetLinecardModeMessage = new VcsNodeSlotSetLinecardModeMessage (slotNumber, rbridgeId, hwId, linecardMode, hwType);
    prismAssert (NULL != pVcsNodeSlotSetLinecardModeMessage, __FILE__, __LINE__);
    pVcsNodeSlotSetLinecardModeMessage->setNeedSurrogateSupportFlag (true);
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (VcsNodeSlotManagementWorker::getInstance (), reinterpret_cast<PrismAsynchronousCallback> (&VcsNodeSlotManagementWorker::configSlotViaSendToWaveClusterCallback), pVcsNodeSlotMessagingContext);
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);
    pWaveSendToClusterContext->setPPrismMessageForPhase1(pVcsNodeSlotSetLinecardModeMessage);
    pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationsToSendToForPhase1);
    sendToWaveCluster (pWaveSendToClusterContext);
}

void VcsNodeSlotManagementWorker::slotConfigMessageHandler (VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage)
{
    UI32 slotNumber = pVcsNodeSlotConfigMessage->getSlotNumber();
    UI32 hwId = pVcsNodeSlotConfigMessage->getHwId ();
    UI32 linecardMode = pVcsNodeSlotConfigMessage->getLinecardMode ();
    UI32 hwType = pVcsNodeSlotConfigMessage->getHwType ();
    UI32 rbridgeId = pVcsNodeSlotConfigMessage->getRbridgeId ();
    LocationId locationId = DcmToolKit::getLocationIdFromMappedId (rbridgeId);
    if (0 == locationId)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotConfigMessageHandler: node not found for rbridgeId = ")+rbridgeId+" locationId = "+locationId);
        pVcsNodeSlotConfigMessage->setCompletionStatus (LINECARD_MANAGEMENT_FAILED_RBRIDGEID_ERROR);
        pVcsNodeSlotConfigMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_RBRIDGEID_ERROR);
        reply (pVcsNodeSlotConfigMessage);
        return;
    }

    // Check for valid slot numbers
    UI32 numSlots = 0;
    numSlots = chassisNumSlots ();
    UI32 numLinecards = 0;
    UI32 slotType = SYS_SLOT_UNKNOWN;

    while ((numSlots > 0) && (slotType != SYS_SLOT_LC))
    {
        slot_no2id((int)numSlots, (int *)(&slotType), (int *)(&numLinecards));
        numSlots--;
    }

    if ((slotNumber < 1) || (slotNumber > numLinecards))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::slotConfigMessageHandler: Invalid slot number: ") + slotNumber + ", maximum slot number is: " + numLinecards);
        pVcsNodeSlotConfigMessage->setCompletionStatus (LINECARD_MANAGEMENT_FAILED_SLOT_NUM_ERROR);
        pVcsNodeSlotConfigMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_SLOT_NUM_ERROR);
        reply (pVcsNodeSlotConfigMessage);
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotConfigMessageHandler SlotNumber for config slot = ")+slotNumber+" hwId = "+hwId+" linecardMode = "+linecardMode+" hwType = "+hwType+" rbridgeId = "+rbridgeId+" locationId = "+locationId);
    
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::configSlotViaSendToWaveCluster),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };
    
    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSlotConfigMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);
    pVcsNodeSlotMessagingContext->setSlotMessage (pVcsNodeSlotConfigMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setHwType (hwType);
    pVcsNodeSlotMessagingContext->setRbridgeId (rbridgeId);
    pVcsNodeSlotMessagingContext->setLocationId (locationId);
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeSlotManagementWorker::validateConfigSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer ();
    if (NULL != pVcsSlotLocalManagedObject)
    {
        UI32 slotState = pVcsSlotLocalManagedObject->getSlotState();
        if (SLOT_OUT == slotState)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::validateConfigSlot config slot validation successful for slotNumber = ")+slotNumber);
            pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage = reinterpret_cast<VcsNodeSlotConfigMessage *> (pVcsNodeSlotMessagingContext->getSlotMessage ());
            pVcsNodeSlotConfigMessage->setReturnStatus (LINECARD_MANAGEMENT_FAILED_WRONG_STATE_ERROR);
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::validateConfigSlot cannot config slot in ")+slotState+" state slotNumber = "+slotNumber);
            pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::validateConfigSlot slot not found slotNumber = ")+slotNumber);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void VcsNodeSlotManagementWorker::slotSetLinecardModeMessageHandler (VcsNodeSlotSetLinecardModeMessage *pVcsNodeSlotSetLinecardModeMessage)
{
    UI32 slotNumber = pVcsNodeSlotSetLinecardModeMessage->getSlotNumber();
    UI32 objectType = 0;
    UI32 hwType = pVcsNodeSlotSetLinecardModeMessage->getHwType();
    UI32 hwId = pVcsNodeSlotSetLinecardModeMessage->getHwId();
    UI32 linecardMode = pVcsNodeSlotSetLinecardModeMessage->getLinecardMode();
    UI32 slotState = SLOT_OUT;
    UI32 reason = 0;
    UI32 version = 0;
    UI32 rbridgeId = pVcsNodeSlotSetLinecardModeMessage->getRbridgeId();
    
    ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage = new ClusterLocalSlotOnlineMessage (slotNumber, objectType, hwType, hwId, slotState, reason, version);
    prismAssert (NULL != pClusterLocalSlotOnlineMessage, __FILE__, __LINE__);
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotSetLinecardModeMessageHandler: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::getSlotObjects),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::validateConfigSlot),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotOnlineMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createOrUpdateSlotManagedObject),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createCompositionAssociationIfNewSlotMO),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::createAssociationWithWaveSlot),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismLinearSequencerFailedStep)
    };

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSlotSetLinecardModeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);
    pVcsNodeSlotMessagingContext->setSlotMessage (pClusterLocalSlotOnlineMessage);
    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setHwType (hwType);
    pVcsNodeSlotMessagingContext->setHwId (hwId); 
    pVcsNodeSlotMessagingContext->setLinecardMode (linecardMode); 
    pVcsNodeSlotMessagingContext->setObjectType (objectType); 
    pVcsNodeSlotMessagingContext->setReason (reason); 
    pVcsNodeSlotMessagingContext->setVersion (version); 
    pVcsNodeSlotMessagingContext->setSlotState (slotState);
    pVcsNodeSlotMessagingContext->setLocationId (FrameworkToolKit::getThisLocationId ()); // this should be the surrogate locationId if the remove request is for another node
    pVcsNodeSlotMessagingContext->setRbridgeId (rbridgeId); 
    pVcsNodeSlotMessagingContext->holdAll ();
    pVcsNodeSlotMessagingContext->start ();
}

ResourceId VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep (VcsNodeSlotSynchronousMessagingContext *pVcsNodeSlotSynchronousMessagingContext)
{
    LocationId locationId = pVcsNodeSlotSynchronousMessagingContext->getLocationId ();
    UI32 hwType = pVcsNodeSlotSynchronousMessagingContext->getHwType ();
    
    ClusterLocalSlotRemoveAllMessage *pClusterLocalSlotRemoveAllMessage = new ClusterLocalSlotRemoveAllMessage (locationId, hwType);
    
    ResourceId status = sendSynchronously (pClusterLocalSlotRemoveAllMessage, FrameworkToolKit::getThisLocationId ());
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep: send async slot remove message failed, status = " + FrameworkToolKit::localize (status));
        delete (pClusterLocalSlotRemoveAllMessage);
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        ResourceId completionStatus;
        completionStatus = pClusterLocalSlotRemoveAllMessage->getCompletionStatus ();

        delete (pClusterLocalSlotRemoveAllMessage);

        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep: ClusterLocalSlotRemoveAllMessage completionStatus error  = ") +FrameworkToolKit::localize (completionStatus));
            return WAVE_MESSAGE_ERROR;
        } 
        else 
        {
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep: ClusterLocalSlotRemoveAllMessage completionStatus = success"));
        }

        trace (TRACE_LEVEL_SUCCESS, "VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep: send async slot remove message successfully sent");
        return WAVE_MESSAGE_SUCCESS;
    }
}

ResourceId VcsNodeSlotManagementWorker::removeAllSlots (VcsNodeSlotSynchronousMessagingContext *pVcsNodeSlotSynchronousMessagingContext)
{
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::removeAllSlots: remove all slots "));
    LocationId locationId = pVcsNodeSlotSynchronousMessagingContext->getLocationId ();
    bool isLocalLocationId = false;
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

    if (locationId == FrameworkToolKit::getMyLocationId ())
    {
        pVcsNodeLocalManagedObject = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObject ();
        isLocalLocationId = true;
        VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.lock();
    }
    else
    {
        pVcsNodeLocalManagedObject = VcsFabricLocalObjectManager::getInstance ()->getLocalManagedObjectFromLocationId (locationId);
    }

    if (NULL != pVcsNodeLocalManagedObject)
    {
        pVcsNodeLocalManagedObject->removeAllVcsSlotLocalManagedObjects ();
        updateWaveManagedObject (pVcsNodeLocalManagedObject);

        UI32 hwType = pVcsNodeSlotSynchronousMessagingContext->getHwType ();
        ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);
    
        // Retrieve the VcsSlotLocalManagedObject
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());
    
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
        synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);
        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        if (0 < pWaveManagedObjects->size ())
        {
            UI32 size = pWaveManagedObjects->size ();
            for (UI32 i = 0; i < size; i++)
            {
                delete (dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[i]));
            }
        }

        // Clean up query results
        pWaveManagedObjects->clear ();
        delete pWaveManagedObjects;

        if (isLocalLocationId)
        {
            VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.unlock ();
        }
        else
        {
            if (pVcsNodeLocalManagedObject != NULL)
            {
                delete (pVcsNodeLocalManagedObject);
            }
        }

        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeSlotManagementWorker::removeAllSlots: deleted all slots "));

        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if (isLocalLocationId)
        {
            VcsFabricLocalObjectManager::getInstance ()->m_thisVcsNodeLocalManagedObjectMutex.unlock ();
        }

        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::removeAllSlots: VcsNodeLocalManagedObject not found "));

        return WAVE_MESSAGE_ERROR;
    }
}

void VcsNodeSlotManagementWorker::clearAllLinecardHandler ()
{
    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::clearAllLinecardHandler "));

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::removeAllSlots),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::sendSlotRemoveAllMessageStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeSlotManagementWorker::prismSynchronousLinearSequencerFailedStep)
    };

    UI32 hwType = SYS_SLOT_LC;

    VcsNodeSlotSynchronousMessagingContext *pVcsNodeSlotSynchronousMessagingContext = new VcsNodeSlotSynchronousMessagingContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeSlotSynchronousMessagingContext, __FILE__, __LINE__);
    
    pVcsNodeSlotSynchronousMessagingContext->setLocationId (FrameworkToolKit::getThisLocationId ()); 
    pVcsNodeSlotSynchronousMessagingContext->setHwType (hwType);
    
    pVcsNodeSlotSynchronousMessagingContext->execute ();
}

UI32 VcsNodeSlotManagementWorker::getLinecardTypeFromHwId (const UI32 hwId)
{
    UI32 linecardType = VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN;

    switch (hwId)
    {
         case UNKNOWN_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN;
             break;
         case MERCURY_LC48X10G_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_48X10G;
             break;
         case MERCURY_LC12X40G_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_12X40G;
             break;
         case ELARA2E_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_ELARA2E;
             break;
         case ELARA2F_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_ELARA2F;
             break;
         case CALLISTOE_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_CALLISTOE;
             break;
         case CALLISTOF_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_CALLISTOF;
             break;
         case CARME_E_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_CARME_E;
             break;
         case MERCURY_LC72X1G_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_72X1G;
             break;
         case MERCURY_LC48X1G_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_48X1G;
             break;
         case CASTOR_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_CASTOR;
             break;
         case POLLUX_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_POLLUX;
             break;
         case CASTORT_BLADE_ID:
             linecardType = VCS_NODE_SLOT_LINECARD_TYPE_CASTORT;
             break;
        default:
             WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsNodeSlotManagementWorker::getLinecardTypeFromHwId : Unsupported enum: ") + hwId);
            break;
    }

    return linecardType;
}

void VcsNodeSlotManagementWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsNodeSlotManagementWorker::installKnownSlotsWhileJoiningCluster (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) || (WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason))
    {
        UI32 numberOfSlotsOnThisNode = s_slotNumbers.size ();
        UI32 i                       = 0;
        UI32 eventId = 0;

        for (i = 0; i < numberOfSlotsOnThisNode; i++)
        {
                  UI32                         slotNumber                   = s_slotNumbers[i];
                  UI32                         hwType                       = s_hwTypes[i];
                  UI32                         slotState                    = s_slotStates[i];
                  UI32                         objectType                   = s_objectTypes[i];
                  UI32                         hwId                         = s_hwIds[i];
                  UI32                         reason                       = s_reasons[i];
                  UI32                         version                      = s_versions[i];

            const VcsNodeFabosSlotOnlineEvent *pVcsNodeFabosSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);

            slotCreationHandler (pVcsNodeFabosSlotOnlineEvent);

            delete (pVcsNodeFabosSlotOnlineEvent);
        }
    }
}

void VcsNodeSlotManagementWorker::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    UI32 slotNumber           = 0;
    UI32 hwType               = 0;
    UI32 slotState            = 0;
    UI32 objectType           = 0;
    UI32 hwId                 = 0;
    UI32 reason               = 0;
    UI32 version              = 0;

    UI32 i                    = 0;
    UI32 numberOfQueryResults = 0;

    s_slotNumbers.clear ();
    s_hwTypes.clear     ();
    s_slotStates.clear  ();
    s_objectTypes.clear ();
    s_hwIds.clear       ();
    s_reasons.clear     ();
    s_versions.clear    ();

    vector<WaveManagedObject *> *pQueryResults = querySynchronouslyLocalManagedObjectsForLocationId (FrameworkToolKit::getThisLocationId (), VcsSlotLocalManagedObject::getClassName ());

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    numberOfQueryResults = pQueryResults->size ();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::shutdown : Number Of slots on this node : ") + numberOfQueryResults);

    for (i = 0; i < numberOfQueryResults; i++)
    {
        VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = dynamic_cast<VcsSlotLocalManagedObject *> ((*pQueryResults)[i]);

        prismAssert (NULL != pVcsSlotLocalManagedObject, __FILE__, __LINE__);

        slotNumber = pVcsSlotLocalManagedObject->getSlotNumber ();
        hwType     = pVcsSlotLocalManagedObject->getHwType     ();
        slotState  = pVcsSlotLocalManagedObject->getSlotState  ();
        objectType = pVcsSlotLocalManagedObject->getObjectType ();
        hwId       = pVcsSlotLocalManagedObject->getHwId       ();
        reason     = pVcsSlotLocalManagedObject->getReason     ();
        version    = pVcsSlotLocalManagedObject->getVersion    ();

        s_slotNumbers.push_back (slotNumber);
        s_hwTypes.push_back     (hwType);
        s_slotStates.push_back  (slotState);
        s_objectTypes.push_back (objectType);
        s_hwIds.push_back       (hwId);
        s_reasons.push_back     (reason);
        s_versions.push_back    (version);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsNodeSlotManagementWorker::slotStateQueryMessageHandler (VcsNodeSlotStateQueryMessage *pSlotStateQueryMessage)
{
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeSlotManagementWorker::slotStateQueryMessageHandler: Entering... "));

    UI32       slotNumber       = pSlotStateQueryMessage->getSlotNumber ();
    UI32       hwType           = pSlotStateQueryMessage->getHwType ();
    LocationId locationId       = FrameworkToolKit::getThisLocationId (); 
    ObjectId   waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotStateQueryMessageHandler: quering slot object for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());

    // Retrieve the VcsSlotLocalManagedObject
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);

    VcsSlotLocalManagedObject * pVcsSlotLocalManagedObject = NULL;

    if (0 < pWaveManagedObjects->size ())
    {
        pVcsSlotLocalManagedObject = dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[0]);

        UI32 slotState = pVcsSlotLocalManagedObject->getSlotState ();

        pSlotStateQueryMessage->setSlotState (slotState);
        pSlotStateQueryMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotStateQueryMessageHandler: vcs slot object found for query slotNumber = ") + slotNumber + " slotState = " + slotState);
    }
    else
    {
        pSlotStateQueryMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);

        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::slotStateQueryMessageHandler: No vcs slot objects found for query slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());
    }

    if (pWaveManagedObjects != NULL)
    {
        pWaveManagedObjects->clear ();
        delete (pWaveManagedObjects);
    }

    reply (pSlotStateQueryMessage);
}

void VcsNodeSlotManagementWorker::createSlotZero (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeSlotManagementWorker::createSlotZero : Entering... "));

    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();

    if (slotNumber == 0)
    {
        UI32 hwType = pVcsNodeSlotMessagingContext->getHwType();
        LocationId locationId = pVcsNodeSlotMessagingContext->getLocationId ();
        ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);

        trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createSlotZero : quering slot object for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());

        // Retrieve the VcsSlotLocalManagedObject
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
        synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));

        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);

        if (0 < pWaveManagedObjects->size ())
        {
            trace (TRACE_LEVEL_DEVEL, string ("VcsNodeSlotManagementWorker::createSlotZero : Vcs slot ob        jects found, size = ") + pWaveManagedObjects->size ());
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createSlotZero : No vcs slot objects found for query slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());

            UI32 eventId    = 0;
            UI32 slotState  = pVcsNodeSlotMessagingContext->getSlotState ();
            UI32 objectType = pVcsNodeSlotMessagingContext->getObjectType ();
            UI32 hwId       = pVcsNodeSlotMessagingContext->getHwId ();
            UI32 reason     = pVcsNodeSlotMessagingContext->getReason ();
            UI32 version    = pVcsNodeSlotMessagingContext->getVersion ();

            const VcsNodeFabosSlotOnlineEvent *pVcsNodeFabosSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);

            slotCreationHandler (pVcsNodeFabosSlotOnlineEvent);

            delete (pVcsNodeFabosSlotOnlineEvent);
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjects);
    }

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void VcsNodeSlotManagementWorker::getLinecardsFromStartupDB ()
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeSlotManagementWorker::getLinecardsFromStartupDB : Entering ...");

    // Retrieve the VcsSlotLocalManagedObject
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

    UI32     hwType           = SYS_SLOT_LC;
    ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (FrameworkToolKit::getThisLocationId ());

    synchronousQueryContext.setSchemaToQuery (OrmRepository::getWaveStartSchema ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

    if (0 < pWaveManagedObjects->size ())
    {
        UI32 size      = pWaveManagedObjects->size ();

        for (UI32 i = 0; i < size; i++)
        {   
            VcsSlotLocalManagedObject * pVcsSlotLocalManagedObject = dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[i]);

            UI32 eventId      = 0;
            UI32 slotNumber   = pVcsSlotLocalManagedObject->getSlotNumber ();     
            UI32 objectType   = pVcsSlotLocalManagedObject->getObjectType ();     
            UI32 hwId         = pVcsSlotLocalManagedObject->getHwId ();
//              UI32 linecardMode = pVcsSlotLocalManagedObject->getLinecardMode ();
            UI32 slotState    = pVcsSlotLocalManagedObject->getSlotState ();
            UI32 reason       = pVcsSlotLocalManagedObject->getReason ();
            UI32 version      = pVcsSlotLocalManagedObject->getVersion ();
        
            const VcsNodeFabosSlotOnlineEvent *pVcsNodeFabosSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);

            trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getLinecardsFromStartupDB : Successfully create vcs slot online event: slotNumber = ") + slotNumber + " objectType = " + objectType + " hwId = " + hwId + " slotState = " + slotState + " reason = " + reason + " version = " + version);

            slotCreationHandler (pVcsNodeFabosSlotOnlineEvent);

            delete (pVcsNodeFabosSlotOnlineEvent);
        }   
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjects);
}

void VcsNodeSlotManagementWorker::createDummySlotMessageHandler (VcsNodeCreateDummySlotMessage *pVcsNodeCreateDummySlotMessage)
{
    UI32 eventId      = 0;
    UI32 objectType   = 0;
    UI32 hwType       = SYS_SLOT_LC;
    UI32 hwId         = UNKNOWN_BLADE_ID;
    UI32 slotState    = SLOT_OUT;
    UI32 reason       = 0;
    UI32 version      = 0;
    UI32 slotNumber   = pVcsNodeCreateDummySlotMessage->getSlotNumber ();

    const VcsNodeFabosSlotOnlineEvent *pVcsNodeFabosSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::createDummySlot: Successfully create vcs slot online event: slotNumber = ") + slotNumber + " objectType = " + objectType + " hwId = " + hwId + " slotState = " + slotState + " reason = " + reason + " version = " + version);

    slotCreationHandler (pVcsNodeFabosSlotOnlineEvent);

    delete (pVcsNodeFabosSlotOnlineEvent);
}

void VcsNodeSlotManagementWorker::backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    ResourceId status     = WAVE_MESSAGE_SUCCESS;
    int        rc         = 0;
    UI32       totalSlots = 0;

    totalSlots = chassisNumSlots ();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::backendSyncUp: Total slots in the system = ") + totalSlots);

    // Only perform backend sync up for chassis systems and ignore for pizza boxes.
    if (1 < totalSlots)
    {
        //Query the slot information from the backend.

        rc = getSlotInformationFromBackend (totalSlots);
        if (rc != 0) 
        {
            trace(TRACE_LEVEL_ERROR, "VcsNodeSlotManagementWorker::backendSyncUp: Retreiving slot information from backend failed");
            status = WAVE_MESSAGE_ERROR;
            pPrismAsynchronousContext->setCompletionStatus (status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        //Query the slot information from the database.
        //Compare the slot informations and take necessary actions.
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

        UI32     hwTypeLC         = SYS_SLOT_LC;
        bool     match            = false;
        bool     slotFoundInDB    = false;
        ObjectId waveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (FrameworkToolKit::getThisLocationId ());

        synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwTypeLC, "hwType"));
        synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));

        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

        VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = NULL;
        UI32 slotNumber = 0, hwType = 0, slotState = 0, objectType = 0, hwId = 0, reason = 0, version = 0, eventId = 0;

        for (UI32 i = 0; i < s_slotNumbers.size (); i++)
        {
            match         = false;
            slotFoundInDB = false;

            for (UI32 j = 0; j < pWaveManagedObjects->size (); j++)
            {
                pVcsSlotLocalManagedObject = dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[j]);
                slotNumber   = pVcsSlotLocalManagedObject->getSlotNumber ();
                slotState    = pVcsSlotLocalManagedObject->getSlotState ();

                if (s_slotNumbers[i] == slotNumber)
                {
                    slotFoundInDB = true;
                    if (s_slotStates[i] == slotState)
                    {
                        //Slot state of LC and DB match. Don't perform any operation.
                        match = true;

                        trace(TRACE_LEVEL_INFO, "VcsNodeSlotManagementWorker::backendSyncUp: slot state matching with that in the DB. NO corrective action will be taken.");
                    }

                    break;
                }
            }
            
            if (!match)
            {

                trace(TRACE_LEVEL_INFO, "VcsNodeSlotManagementWorker::backendSyncUp: slot state NOT matching with that in the DB. Corrective action will be taken.");

                slotNumber  = s_slotNumbers[i];
                slotState   = s_slotStates[i];
                eventId     = 0;

                //Slot state of LC NOT matching with that in DB. Take corrective action.
                if (SLOT_ON == s_slotStates[i])
                {
                    hwType      = s_hwTypes[i];
                    objectType  = s_objectTypes[i];
                    hwId        = s_hwIds[i];
                    reason      = s_reasons[i];
                    version     = s_versions[i];

                    const VcsNodeFabosSlotOnlineEvent *pVcsNodeFabosSlotOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
                    prismAssert (NULL != pVcsNodeFabosSlotOnlineEvent, __FILE__, __LINE__);
                    slotCreationHandler (pVcsNodeFabosSlotOnlineEvent);
                    
                    delete (pVcsNodeFabosSlotOnlineEvent);
                }
                else if (slotFoundInDB)
                {
                    //Update slot Offline state only if the slot information is found in the DB.

                    hwType      = pVcsSlotLocalManagedObject->getHwType ();
                    objectType  = pVcsSlotLocalManagedObject->getObjectType ();
                    hwId        = pVcsSlotLocalManagedObject->getHwId ();
                    reason      = pVcsSlotLocalManagedObject->getReason ();
                    version     = pVcsSlotLocalManagedObject->getVersion ();

                    const VcsNodeFabosSlotOfflineEvent *pVcsNodeFabosSlotOfflineEvent = new VcsNodeFabosSlotOfflineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
                    prismAssert (NULL != pVcsNodeFabosSlotOfflineEvent, __FILE__, __LINE__);
                    slotDeleteEventHandler (pVcsNodeFabosSlotOfflineEvent);
                    delete (pVcsNodeFabosSlotOfflineEvent);                        
                }

            }

        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjects);
        s_slotNumbers.clear ();
        s_hwTypes.clear     ();
        s_slotStates.clear  ();
        s_objectTypes.clear ();
        s_hwIds.clear       ();
        s_reasons.clear     ();
        s_versions.clear    ();

    }

    pPrismAsynchronousContext->setCompletionStatus (status);
    pPrismAsynchronousContext->callback ();

}

int VcsNodeSlotManagementWorker::getSlotInformationFromBackend (UI32 totalSlots)
{
    sysModHandle_t *pSysModHandle = NULL;
    sysCtrlState_t  ctrlData;
    sysCtrlData_t  *pNextPtr;
    int             err = 0;
    UI32            i = 0;
    UI32            slotNo = 0, hwType = 0, slotState = 0, objectType = 0, hwId = 0, reason = 0, version = 0, slotIterator = 0;

    pNextPtr = &(ctrlData.data[0]);

    err = sysModInit (&pSysModHandle);
    if ((err != 0) || (pSysModHandle == NULL)) 
    {
        trace (TRACE_LEVEL_WARN, "VcsNodeSlotManagementWorker::getSlotInformationFromBackend: Fabsys system initialization failed");
        return (-1);
    }

    s_slotNumbers.clear ();
    s_hwTypes.clear ();
    s_slotStates.clear ();
    s_objectTypes.clear ();
    s_hwIds.clear ();
    s_reasons.clear ();
    s_versions.clear ();

    for (i = 1; i <= totalSlots; i++) 
    {
        ctrlData.total = 1;
        fabSysStateObject (pNextPtr) = SYS_HW_SLOT;
        fabSysStateUnit (pNextPtr) = (sysCtrlSS_e) i;
        fabSysStateReason (pNextPtr) = 0;
        fabSysStateStatus (pNextPtr) = (sysCtrlSS_e) 0;

        err = sysCtrlGetSwState (pSysModHandle, &ctrlData);

        if (err) 
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeSlotManagementWorker::getSlotInformationFromBackend: Getting slot information for slotNumber ") + i + string (" failed with error code ") + err);
            continue;
        } 
        else 
        {
            hwType = pNextPtr->record.type;

            if ((SYS_BLADE_AP == hwType) || (SYS_BLADE_SW == hwType))
            {
                slotIterator++;
                slotNo = slotIterator;

                switch (pNextPtr->state)
                {
                    case SYS_UNIT_ON:
                        slotState = SLOT_ON;
                        break;
                    case SYS_UNIT_IN:
                        slotState = SLOT_IN;
                        break;
                    case SYS_UNIT_OUT:
                        slotState = SLOT_OUT;
                        break;
                    case SYS_UNIT_FAULT:
                        slotState = SLOT_FAULT;
                        break;
                    default:
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getSlotInformationFromBackend: Slot in one of the intermediate states and hence ignoring, slotState = ")+slotState+" slotNumber = "+slotNo);
                        continue;
                }
            
                objectType = pNextPtr->object;
                hwId       = pNextPtr->record.id;
                reason     = pNextPtr->reason;
                version    = pNextPtr->record.version;
            }
            else
            {
                //Do not get other (MM, SFM, dummys) slot information. We need only LC information.
                continue;
            }

        }

        if ((hwType == SYS_BLADE_AP) || (hwType == SYS_BLADE_SW))
        {    
            hwType = SYS_SLOT_LC;
        }    

        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeSlotManagementWorker::getSlotInformationFromBackend: slotState = ")+slotState+" slotNumber = "+slotNo+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);

        s_slotNumbers.push_back (slotNo);
        s_hwTypes.push_back (hwType);
        s_slotStates.push_back (slotState);
        s_objectTypes.push_back (objectType);
        s_hwIds.push_back (hwId);
        s_reasons.push_back (reason);
        s_versions.push_back (version);

    }    

    sysModClose (&pSysModHandle);
    return (0);
}

}

