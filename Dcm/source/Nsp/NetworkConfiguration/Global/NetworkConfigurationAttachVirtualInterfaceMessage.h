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

#ifndef NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEMESSAGE_H
#define NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationAttachVirtualInterfaceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                            NetworkConfigurationAttachVirtualInterfaceMessage ();
                            NetworkConfigurationAttachVirtualInterfaceMessage (const string &networkUuid, const string &interfaceUuid, const MacAddress &macAddress, const string &targetHost);
        virtual            ~NetworkConfigurationAttachVirtualInterfaceMessage ();

                string      getNetworkUuid                                    () const;
                void        setNetworkUuid                                    (const string &networkUuid);

                string      getInterfaceUuid                                  () const;
                void        setInterfaceUuid                                  (const string &interfaceUuid);

                MacAddress  getMacAddress                                     () const;
                void        setMacAddress                                     (const MacAddress &macAddress);

                string      getTargetHost                                     () const;
                void        setTargetHost                                     (const string &targetHost);

    // Now the Data Members

    private :
        string     m_networkUuid;
        string     m_interfaceUuid;
        MacAddress m_macAddress;
        string     m_targetHost;

    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEMESSAGE_H
