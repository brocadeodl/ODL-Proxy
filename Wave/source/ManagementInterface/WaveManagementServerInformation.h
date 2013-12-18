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

#ifndef WAVEMANAGEMENTSERVERINFORMATION_H
#define WAVEMANAGEMENTSERVERINFORMATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagementServerInformation : virtual public SerializableObject
{
    private :
        virtual void setupAttributesForSerialization                            ();

    protected :
    public :
                                WaveManagementServerInformation                 ();
                                WaveManagementServerInformation                 (const string &ipAddress, const UI32 &port);
        virtual                ~WaveManagementServerInformation                 ();

                string          getIpAddress                                    () const;
                void            setIpAddress                                    (const string &ipAddress);

                UI32            getPort                                         () const;
                void            setPort                                         (const UI32 &port);

                void            addServiceInformation                           (const string &serviceName, const PrismServiceId &serviceId);
                UI32            getNumberOfServices                             () const;
                string          getServiceNameAtIndex                           (const UI32 &serviceIndex) const;
                UI32            getServiceIdAtIndex                             (const UI32 &serviceIndex) const;
                vector<string>  getServiceNames                                 () const;
                vector<UI32>    getServiceIds                                   () const;

                void            setMessageVersion                               (const string &version); 
                string          getMessageVersion                               () const; 

                UI32            getServerUniqueIdentifierFromServerPerspective  ();
                void            setServerUniqueIdentifierFromServerPerspective  (const UI32 &serverUniqueIdentifierFromServerPerspective);
    // Now the data members

    private :
        string                 m_ipAddress;
        UI32                   m_port;
        vector<string>         m_serviceNames;
        vector<PrismServiceId> m_serviceIds;
        UI32                   m_serverUniqueIdentifierFromServerPerspective;
        vector<string>         m_reservedFields;
        string                 m_messageVersion;

    protected :
    public :
};

}

#endif // WAVEMANAGEMENTSERVERINFORMATION_H


