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

#ifndef NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALMESSAGE
#define NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALMESSAGE

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationAttachVirtualInterfaceLocalMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                            NetworkConfigurationAttachVirtualInterfaceLocalMessage ();
                            NetworkConfigurationAttachVirtualInterfaceLocalMessage (const string &networkName, const UI32 &networkId, const MacAddress &macAddress, const string &targetHost);
        virtual            ~NetworkConfigurationAttachVirtualInterfaceLocalMessage ();

                string      getNetworkName                                         () const;
                void        setNetworkName                                         (const string &networkName);

                UI32        getNetworkId                                           () const;
                void        setNetworkId                                           (const UI32 &networkId);

                MacAddress  getMacAddress                                          () const;
                void        setMacAddress                                          (const MacAddress &macAddress);

                string      getTargetHost                                          () const;
                void        setTargetHost                                          (const string &targetHost);


    // Now the data members

    private :
        string     m_networkName;
        UI32       m_networkId;
        MacAddress m_macAddress;
        string     m_targetHost;

    protected :
    public :

};

}

#endif // NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALMESSAGE
