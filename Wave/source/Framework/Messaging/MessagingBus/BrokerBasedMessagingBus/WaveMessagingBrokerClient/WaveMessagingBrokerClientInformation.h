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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMESSAGINGBROKERCLIENTINFORMATION_H
#define WAVEMESSAGINGBROKERCLIENTINFORMATION_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/SerializableObject.h"

namespace WaveNs
{

class WaveMessagingBrokerClientInformation : public SerializableObject
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                WaveMessagingBrokerClientInformation ();
                WaveMessagingBrokerClientInformation (const string &name, const string &ipAddress, const SI32 &port);
               ~WaveMessagingBrokerClientInformation ();

        string  getName                              () const;
        void    setName                              (const string &name);

        string  getIpAddress                         () const;
        void    setIpAddress                         (const string &ipAddress);

        SI32    getPort                              () const;
        void    setPort                              (const SI32 &port);

        // Now the data members

    private :
        string m_name;
        string m_ipAddress;
        SI32   m_port;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCLIENTINFORMATION_H
