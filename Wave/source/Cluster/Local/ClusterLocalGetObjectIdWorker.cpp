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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/
#include "Cluster/Local/ClusterLocalGetObjectIdMessages.h"
#include "Cluster/Local/ClusterLocalGetObjectIdWorker.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include  "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

ClusterLocalGetObjectIdWorker::ClusterLocalGetObjectIdWorker (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : WaveWorker (pClusterLocalObjectManager)
{
    addOperationMap (CLUSTER_LOCAL_GET_SLOT_OBJECTID, reinterpret_cast<PrismMessageHandler> (&ClusterLocalGetObjectIdWorker::getSlotObjectIdMessageHandler));
    addOperationMap (CLUSTER_LOCAL_GET_LOCATION_OBJECTID, reinterpret_cast<PrismMessageHandler> (&ClusterLocalGetObjectIdWorker::getLocationObjectIdMessageHandler));
    addOperationMap (CLUSTER_LOCAL_GET_SLOTNUMBER, reinterpret_cast<PrismMessageHandler> (&ClusterLocalGetObjectIdWorker::getSlotNumberMessageHandler));
    addOperationMap (CLUSTER_LOCAL_GET_LOCATIONID, reinterpret_cast<PrismMessageHandler> (&ClusterLocalGetObjectIdWorker::getLocationIdMessageHandler));
}

ClusterLocalGetObjectIdWorker::~ClusterLocalGetObjectIdWorker ()
{
}

void ClusterLocalGetObjectIdWorker::getSlotObjectIdMessageHandler (ClusterLocalGetSlotObjectIdMessage *pClusterLocalGetSlotObjectIdMessage)
{
    trace (TRACE_LEVEL_DEBUG, "ClusterLocalGetObjectIdWorker::getSlotObjectIdMessageHandler : Entering ..  ");
    
    UI32       slotNumber       = pClusterLocalGetSlotObjectIdMessage->getSlotNumber ();
    LocationId locationId       = pClusterLocalGetSlotObjectIdMessage->getLocationId ();
    ObjectId   retSlotObjectId;
    ResourceId status           = WAVE_MESSAGE_SUCCESS;
    
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveSlotLocalManagedObject::getClassName());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));

    synchronousQueryContext.addAndAttribute (new AttributeObjectId (getObjectIdForLocation (locationId), "ownerWaveNodeObjectId"));

    vector<WaveManagedObject *> *pResultsWaveSlotLocalManagedObjects = querySynchronously (&synchronousQueryContext);

    if (0 == pResultsWaveSlotLocalManagedObjects->size ())
    {
        trace (TRACE_LEVEL_DEBUG, "ClusterLocalGetObjectIdWorker::getSlotObjectIdMessageHandler : Found no slot local managed objects for provided slot number ..  ");
        status = WAVE_MESSAGE_ERROR;
    }
    else if (1 < pResultsWaveSlotLocalManagedObjects->size ())
    {
        trace (TRACE_LEVEL_DEBUG, "ClusterLocalGetObjectIdWorker::getSlotObjectIdMessageHandler : Found too many slot local managed objects for provided slot number ..  ");
        status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsWaveSlotLocalManagedObjects);
    }
    else
    {
        WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = dynamic_cast<WaveSlotLocalManagedObject *> ((*pResultsWaveSlotLocalManagedObjects)[0]);
    
        retSlotObjectId = pWaveSlotLocalManagedObject->getObjectId ();    

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsWaveSlotLocalManagedObjects);
    }
        
    pClusterLocalGetSlotObjectIdMessage->setSlotObjectId (retSlotObjectId);
    
    pClusterLocalGetSlotObjectIdMessage->setCompletionStatus (status);

    reply (pClusterLocalGetSlotObjectIdMessage);
    
}

void ClusterLocalGetObjectIdWorker::getLocationObjectIdMessageHandler (ClusterLocalGetLocationObjectIdMessage *pClusterLocalGetLocationObjectIdMessage)
{
    LocationId locationId   = pClusterLocalGetLocationObjectIdMessage->getLocationId ();

    ObjectId retLocationObjectId = getObjectIdForLocation (locationId);

    pClusterLocalGetLocationObjectIdMessage->setLocationObjectId (retLocationObjectId);
    
    if (ObjectId::NullObjectId == retLocationObjectId)
    {    
        pClusterLocalGetLocationObjectIdMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }
    else
    {
        pClusterLocalGetLocationObjectIdMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }

    reply (pClusterLocalGetLocationObjectIdMessage);
}

ObjectId ClusterLocalGetObjectIdWorker::getObjectIdForLocation (LocationId locationId)

{
    ObjectId locationObjectId;

    if (0 == locationId)
    {
        locationId = FrameworkToolKit::getThisLocationId ();
    }

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveNode::getClassName());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

    vector<WaveManagedObject *> *pResultsWaveNodes = querySynchronously (&synchronousQueryContext);

    if (0 == pResultsWaveNodes->size ())
    {
        trace (TRACE_LEVEL_DEBUG, "ClusterLocalGetObjectIdWorker::getObjectIdForLocation : no wave node found for this location id");
    }
    else if (1 != pResultsWaveNodes->size ())
    {
        trace (TRACE_LEVEL_WARN, string ("ClusterLocalGetObjectIdWorker::getObjectIdForLocation : More then one Wave Node found --- ") + pResultsWaveNodes->size ());
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsWaveNodes);
    }
    else 
    {
        WaveNode *pWaveNode = dynamic_cast<WaveNode *>((*pResultsWaveNodes)[0]);
    
        locationObjectId = pWaveNode->getObjectId ();

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsWaveNodes);
    }
    
    return (locationObjectId);

}
                
void ClusterLocalGetObjectIdWorker::getSlotNumberMessageHandler (ClusterLocalGetSlotNumberMessage *pClusterLocalGetSlotNumberMessage)
{
    WaveManagedObject          *pWaveManagedObject          = NULL;
    WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject = NULL;
    ObjectId                    slotObjectId                = pClusterLocalGetSlotNumberMessage->getSlotObjectId ();
    UI32                        slotNumber                  = 0;

    pWaveManagedObject = queryManagedObject (slotObjectId);

    if (NULL != pWaveManagedObject)
    {
        pWaveSlotLocalManagedObject = dynamic_cast<WaveSlotLocalManagedObject *> (pWaveManagedObject);

        prismAssert (NULL != pWaveSlotLocalManagedObject, __FILE__, __LINE__);

        slotNumber = pWaveSlotLocalManagedObject->getSlotNumber ();

        delete pWaveSlotLocalManagedObject;
    }

    pClusterLocalGetSlotNumberMessage->setSlotNumber (slotNumber);

    pClusterLocalGetSlotNumberMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    
    reply (pClusterLocalGetSlotNumberMessage);

}

void ClusterLocalGetObjectIdWorker::getLocationIdMessageHandler (ClusterLocalGetLocationIdMessage *pClusterLocalGetLocationIdMessage)
{
    WaveManagedObject *pWaveManagedObject   = NULL;
    WaveNode          *pWaveNode            = NULL;
    ObjectId           locationObjectId     = pClusterLocalGetLocationIdMessage->getLocationObjectId ();
    LocationId         locationId           = 0;

    pWaveManagedObject = queryManagedObject (locationObjectId);

    if (NULL != pWaveManagedObject)
    {
        pWaveNode = dynamic_cast<WaveNode *> (pWaveManagedObject);

        prismAssert (NULL != pWaveNode, __FILE__, __LINE__);

        locationId = pWaveNode->getLocationId ();

        delete pWaveNode;
    }

    pClusterLocalGetLocationIdMessage->setLocationId (locationId);

    pClusterLocalGetLocationIdMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    
    reply (pClusterLocalGetLocationIdMessage);
}

}
