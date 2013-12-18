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

#ifndef WAVEMANAGEMENTCLIENTINFORMATION_H
#define WAVEMANAGEMENTCLIENTINFORMATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagementClientInformation : virtual public SerializableObject
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                WaveManagementClientInformation         ();
                                WaveManagementClientInformation         (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &ipAddress, const UI32 &port, const PrismServiceId &userInterfaceServiceId, const string &messageVersion);
        virtual                ~WaveManagementClientInformation         ();

                string          getWaveClientName                       () const;
                void            setWaveClientName                       (const string &waveClientName);

                string          getWaveClientNameWithoutExtension       () const;
                void            setWaveClientNameWithoutExtension       (const string &waveClientName);

                string          getIpAddress                            () const;
                void            setIpAddress                            (const string &ipAddress);

                UI32            getPort                                 () const;
                void            setPort                                 (const UI32 &port);

                PrismServiceId  getUserInterfaceServiceId               () const;
                void            setUserInterfaceServiceId               (const PrismServiceId &userInterfaceServiceId);

                string          getServerIpAddressFromClientPerspective () const;
                void            setServerIpAddressFromClientPerspective (const string &serverIpAddressFromClientPerspective);

                UI32            getServerUniqueIdentifier               () const;
                void            setServerUniqueIdentifier               (const UI32 &uniqueIdentifier);

                string          getMessageVersion                       () const; 
                void            setMessageVersion                       (const string &messageVersion);


    // Now the data members

    private :
        string         m_waveClientName;
        string         m_waveClientNameWithoutExtension;
        string         m_ipAddress;
        UI32           m_port;
        PrismServiceId m_userInterfaceServiceId;
        string         m_serverIpAddressFromClientPerspective;
        UI32           m_serverUniqueIdentifierFromClientPerspective;
        vector<string> m_reservedFields;
        string         m_messageVersion; 
        
    protected :
    public :
};

}

#endif // WAVEMANAGEMENTCLIENTINFORMATION_H


