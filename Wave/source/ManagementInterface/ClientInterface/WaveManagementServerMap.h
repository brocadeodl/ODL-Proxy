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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMANAGEMENTSERVERMAP_H
#define WAVEMANAGEMENTSERVERMAP_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <string>
#include <map>

using namespace std;

namespace WaveNs
{

class WaveManagementServer;
class ManagementInterfaceMessage;

class WaveManagementServerMap
{
    private :
    protected :
    public :
                               WaveManagementServerMap                      ();
                              ~WaveManagementServerMap                      ();

         bool                  isAKnownServer                               (const string &ipAddress, const UI32 &port);
         bool                  isAKnownServerNoLock                         (const string &ipAddress, const UI32 &port);
         bool                  isAKnownServer                               (const UI32 &id);
         bool                  isAKnownServerNoLock                         (const UI32 &id);
         void                  addServer                                    (WaveManagementServer *pWaveManagementServer);
         WaveManagementServer *removeServer                                 (const string &ipAddress, const UI32 &port);
         WaveManagementServer *removeServer                                 (const UI32 &id);
         WaveManagementServer *getServer                                    (const string &ipAddress, const UI32 &port);
         UI32                  getServerId                                  (const string &ipAddress, const UI32 &port);
         UI32                  getServerIdIfKnown                           (const string &ipAddress, const UI32 &port);
         UI32                  getServerIdIfKnownForUniqueServerIdentifier  (const string &ipAddress, const SI32 &port, const UI32 &uniqueServerIdentifierFromServerPerspective);
         WaveManagementServer *getServer                                    (const UI32 &id);
         PrismServiceId        getPrismServiceIdForServiceName              (const UI32 &serverId, const string &serviceName);
         string                getPrismServiceNameForServiceId              (const UI32 &serverId, const PrismServiceId &serviceId);
         void                  addServiceCache                              (const UI32 &serverId, const vector<string> &serviceNames, const vector<UI32> &serviceIds);
         ResourceId            post                                         (ManagementInterfaceMessage *pManagementInterfaceMessage);
         void                  updateMessageVersionAndSerializationTypeForServer   (const UI32 &serverId, const string &version); 

    // Now the Data Members

    private :
        map<string, WaveManagementServer *> m_managementServers;
        map<UI32,   WaveManagementServer *> m_managementServersById;
        PrismMutex                          m_managementServersMutex;

    protected :
    public :
};

}

#endif // WAVEMANAGEMENTSERVERMAP_H
