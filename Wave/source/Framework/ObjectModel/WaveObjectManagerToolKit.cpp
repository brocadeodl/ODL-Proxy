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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

WaveMessageStatus WaveObjectManagerToolKit::sendOneWay (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendOneWay (pPrismMessage, locationId));
}

WaveMessageStatus WaveObjectManagerToolKit::sendOneWayToFront (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendOneWayToFront (pPrismMessage, locationId));
}

WaveMessageStatus WaveObjectManagerToolKit::sendSynchronously (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendSynchronously (pPrismMessage, locationId));
}

ResourceId WaveObjectManagerToolKit::sendSynchronouslyToWaveClient (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instance)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendSynchronouslyToWaveClient (waveClientName, pManagementInterfaceMessage, Instance));
}

WaveMessageStatus WaveObjectManagerToolKit::sendToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendToWaveServer (waveServerId, pManagementInterfaceMessage, messageCallback, pPrismMessageSender, pInputContext, timeOutInMilliSeconds));
}

ResourceId WaveObjectManagerToolKit::sendOneWayToAllWaveClients (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->sendOneWayToAllWaveClients (pManagementInterfaceMessage));
}

vector<WaveManagedObject *> *WaveObjectManagerToolKit::querySynchronously (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronously (pWaveManagedObjectSynchronousQueryContext));
}

vector<WaveManagedObject *> *WaveObjectManagerToolKit::querySynchronously (const string &managedClassName, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronously (managedClassName, schema));
}

vector<WaveManagedObject *> *WaveObjectManagerToolKit::querySynchronously (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronously (managedClassName, objectIds, schema));
}

vector<WaveManagedObject *> *WaveObjectManagerToolKit::querySynchronouslyByName (const string &managedClassName, const string &managedObjectName, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronouslyByName (managedClassName, managedObjectName, schema));
}

vector<WaveManagedObject *> *WaveObjectManagerToolKit:: querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronouslyLocalManagedObjectsForLocationId (locationId, className));
}

WaveManagedObject *WaveObjectManagerToolKit::queryManagedObject (const ObjectId &managedObjectId, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->queryManagedObject (managedObjectId, schema));
}

ResourceId WaveObjectManagerToolKit::querySynchronouslyForCount (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronouslyForCount (pWaveManagedObjectSynchronousQueryContext, count));
}

ResourceId WaveObjectManagerToolKit::querySynchronouslyForCount (const string &managedClassName, UI32 &count, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronouslyForCount (managedClassName, count, schema));
}

ResourceId WaveObjectManagerToolKit::querySynchronouslyForCount (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    return (pWaveObjectManagerForCurrentThread->querySynchronouslyForCount (managedClassName, fieldName, range, count, schema));
}

void WaveObjectManagerToolKit::getWaveConfigEntry (string &configName, Attribute *attribute, bool &configFound)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();

    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    pWaveObjectManagerForCurrentThread->getWaveConfigEntry (configName, attribute, configFound);

	return;
}
}
