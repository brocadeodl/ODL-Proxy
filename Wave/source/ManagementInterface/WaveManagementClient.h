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

#ifndef WAVEMANAGEMENTCLIENT_H
#define WAVEMANAGEMENTCLIENT_H

#include "Framework/Types/Types.h"

#include <string>

using namespace std;

namespace WaveNs
{

class ClientStreamingSocket;
class ManagementInterfaceMessage;

class WaveManagementClient
{
    private :
    protected :
    public :
                        WaveManagementClient      (const string &name, const string &ipAddress, const SI32 &port, const PrismServiceId &userInterfaceServiceId);
                       ~WaveManagementClient      ();

        ResourceId      connect                   (const string &serverIpAddressFromClientPerspective, const UI32 &numberOfRetries = 0, const UI32 &maximumNumberOfSecondsToTryFor = 0, const UI32 &serverUniqueIdentifierFromServerPerspective = 0);
        void            indicateClosing           ();

        UI32            getId                     () const;
        string          getName                   () const;
        string          getIpAddress              () const;
        SI32            getPort                   () const;
        PrismServiceId  getUserInterfaceServiceId () const;

        bool            operator <<               (ManagementInterfaceMessage *pManagementInterfaceMessage);

        void            updateMessageVersionAndSerializationTypeForSocket (const string& msgVersion);

    // Now the Data Members

    private :
        UI32                   m_id;
        string                 m_name;
        string                 m_ipAddress;
        SI32                   m_port;
        PrismServiceId         m_userInterfaceServiceId;
        ClientStreamingSocket *m_pClientStreamingSocket;

    protected :
    public :
};

}

#endif // WAVEMANAGEMENTCLIENT_H
