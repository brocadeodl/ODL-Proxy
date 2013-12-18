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

#ifndef NETWORKCONFIGURATIONCREATEINTERFACEMESSAGE
#define NETWORKCONFIGURATIONCREATEINTERFACEMESSAGE

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;
using namespace DcmNs;

namespace NspNs
{

class NetworkConfigurationCreateInterfaceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                        NetworkConfigurationCreateInterfaceMessage ();
                                        NetworkConfigurationCreateInterfaceMessage (const UI32 &neworkId, const NetworkInterfaceStatus &networkInterfaceStatus);
                                        NetworkConfigurationCreateInterfaceMessage (const string &neworkUuId, const NetworkInterfaceStatus &networkInterfaceStatus);
        virtual                        ~NetworkConfigurationCreateInterfaceMessage ();

                UI32                    getNetworkId                               () const;
                void                    setNetworkId                               (const UI32 &networkId);

                NetworkInterfaceStatus  getInterfaceStatus                         () const;
                void                    setInterfacestatus                         (const NetworkInterfaceStatus &networkInterfaceStatus);

                string                  getNetworkUuid                             () const;
                void                    setNetworkUuid                             (const string &networkUuid);

                string                  getInterfaceUuid                           () const;
                void                    setInterfaceUuid                           (const string &interfaceUuid);

    // Now the data members

    private :
        UI32                   m_networkId;
        string                 m_networkUuid;
        NetworkInterfaceStatus m_interfaceStatus;
        string                 m_interfaceUuid;

    protected :
    public :

};

}

#endif // NETWORKCONFIGURATIONCREATEINTERFACEMESSAGE
