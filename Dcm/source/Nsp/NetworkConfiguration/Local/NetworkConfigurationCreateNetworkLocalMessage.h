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

#ifndef NETWORKCONFIGURATIONCREATENETWORKLOCALMESSAGE_H
#define NETWORKCONFIGURATIONCREATENETWORKLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationCreateNetworkLocalMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        NetworkConfigurationCreateNetworkLocalMessage ();
                        NetworkConfigurationCreateNetworkLocalMessage (const string &networkName, const UI32 &networkId);
        virtual        ~NetworkConfigurationCreateNetworkLocalMessage ();

                string  getNetworkName                                () const;
                void    setNetworkName                                (const string &networkName);

                UI32    getNetworkId                                  () const;
                void    setNetworkId                                  (const UI32 &networkId);

    // Now the data members

    private :
        string m_networkName;
        UI32   m_networkId;

    protected :
    public :

};

}

#endif // NETWORKCONFIGURATIONCREATENETWORKLOCALMESSAGE_H
