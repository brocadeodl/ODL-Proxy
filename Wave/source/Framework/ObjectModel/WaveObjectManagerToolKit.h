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

#ifndef WAVEOBJECTMANAGERTOOLKIT_H
#define WAVEOBJECTMANAGERTOOLKIT_H

#include "WaveResourceIdEnums.h"
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/ObjectModel/PrismElement.h"
#include <vector>
#include <string>

using namespace std;

namespace WaveNs
{

class PrismMessage;
class WaveManagedObjectSynchronousQueryContext;
class WaveManagedObject;
class ManagementInterfaceMessage;

class WaveObjectManagerToolKit
{
    private :
    protected :
    public :

    // Now the data members

    private :
    protected :
    public :
        static WaveMessageStatus            sendOneWay                      (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        static WaveMessageStatus            sendOneWayToFront               (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        static WaveMessageStatus            sendSynchronously               (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        static ResourceId                   sendSynchronouslyToWaveClient   (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instnace = 0);
        static WaveMessageStatus            sendToWaveServer                (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds);
        static ResourceId                   sendOneWayToAllWaveClients      (ManagementInterfaceMessage *pManagementInterfaceMessage);

        static vector<WaveManagedObject *> *querySynchronously       (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        static vector<WaveManagedObject *> *querySynchronously       (const string &managedClassName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        static vector<WaveManagedObject *> *querySynchronously       (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema = OrmRepository::getWaveCurrentSchema ());
        static vector<WaveManagedObject *> *querySynchronouslyByName (const string &managedClassName, const string &managedObjectName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        static vector<WaveManagedObject *> *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className);
        static WaveManagedObject           *queryManagedObject       (const ObjectId &managedObjectId, const string &schema = OrmRepository::getWaveCurrentSchema ());
        static ResourceId                   querySynchronouslyForCount (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count);
        static ResourceId                   querySynchronouslyForCount (const string &managedClassName, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        static ResourceId                   querySynchronouslyForCount (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ()); 
        static void                         getWaveConfigEntry         (string &configName, Attribute *attribute, bool &configFound);
};

}

#endif // WAVEOBJECTMANAGERTOOLKIT_H
