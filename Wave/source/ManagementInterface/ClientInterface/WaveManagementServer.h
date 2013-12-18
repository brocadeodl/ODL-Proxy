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

#ifndef WAVEMANAGEMENTSERVER_H
#define WAVEMANAGEMENTSERVER_H

#include "Framework/Types/Types.h"

#include <string>
#include <map>

using namespace std;

namespace WaveNs
{

class ClientStreamingSocket;
class ManagementInterfaceMessage;

class WaveManagementServer
{
    private :
    protected :
    public :
                        WaveManagementServer            (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &ipAddress, const SI32 &port);
                       ~WaveManagementServer            ();

        ResourceId      connect                         (const UI32 &numberOfRetries = 0, const UI32 &maximumNumberOfSecondsToTryFor = 0);
        void            indicateClosing                 ();

        UI32            getId                           () const;
        string          getIpAddress                    () const;
        SI32            getPort                         () const;
        bool            operator <<                     (ManagementInterfaceMessage *pManagementInterfaceMessage);
        void            addServiceCache                 (const vector<string> &serviceNames, const vector<UI32> &serviceIds);
        PrismServiceId  getPrismServiceIdForServiceName (const string &serviceName);
        string          getPrismServiceNameForServiceId (const PrismServiceId &serviceId);

        UI32            getServerUniqueIdentifier       () const;

        void            updateMessageVersionAndSerializationTypeForSocket (const string& msgVersion);
        
    // Now the Data Members

    private :
        UI32                   m_id;
        string                 m_waveClientName;
        string                 m_waveClientNameWithoutExtension;
        string                 m_ipAddress;
        SI32                   m_port;
        ClientStreamingSocket *m_pClientStreamingSocket;

        vector<string>         m_serviceNames;
        vector<UI32>           m_serviceIds;
        map<string, UI32>      m_serviceMapByName;
        map<UI32, string>      m_serviceMapById;
        UI32                   m_serverUniqueIdentifier;

    protected :
    public :
};

}

#endif // WAVEMANAGEMENTSERVER_H
