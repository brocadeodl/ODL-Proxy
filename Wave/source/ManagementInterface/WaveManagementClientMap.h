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

#ifndef WAVEMANAGEMENTCLIENTMAP_H
#define WAVEMANAGEMENTCLIENTMAP_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <string>
#include <map>

using namespace std;

namespace WaveNs
{

class WaveManagementClient;
class ManagementInterfaceMessage;

class WaveManagementClientMap
{
    private :
    protected :
    public :
                              WaveManagementClientMap ();
                             ~WaveManagementClientMap ();

        bool                  isAKnownClient          (const string &name);
        bool                  isAKnownClient          (const string &ipAddress, const UI32 &port);
        bool                  isAKnownClient          (const UI32 &id);
        void                  addClient               (WaveManagementClient *pWaveManagementClient);
        WaveManagementClient *removeClient            (const string &ipAddress, const UI32 &port);
        WaveManagementClient *removeClient            (const UI32 &id);
        WaveManagementClient *getClient               (const string &ipAddress, const UI32 &port);
        UI32                  getClientId             (const string &ipAddress, const UI32 &port);
        UI32                  getClientId             (const string &name);
        WaveManagementClient *getClient               (const UI32 &id);
        WaveManagementClient *getClient               (const string &name);
        vector<WaveManagementClient *> *removeAllClient (const string &ipAddress);
        ResourceId            post                    (ManagementInterfaceMessage *pManagementInterfaceMessage);
        vector<string>        getClientsConnected     () const;

    // Now the Data Members

    private :
        map<string, WaveManagementClient *> m_managementClientsByName;
        map<string, WaveManagementClient *> m_managementClients;
        map<UI32,   WaveManagementClient *> m_managementClientsById;
        map<UI32,   string>                 m_clientMessageVersionById;
        PrismMutex                          m_managementClientsMutex;

    protected :
    public :
};

}

#endif // WAVEMANAGEMENTCLIENTMAP_H
