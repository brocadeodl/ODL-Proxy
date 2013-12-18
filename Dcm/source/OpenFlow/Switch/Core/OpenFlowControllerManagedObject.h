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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWCONTROLLERMANAGEDOBJECT_H
#define OPENFLOWCONTROLLERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Types/IpV4Address.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

namespace OpenFlowNs
{

class OpenFlowControllerManagedObject : public DcmManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
                                                  OpenFlowControllerManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual                                  ~OpenFlowControllerManagedObject ();

        static  string                            getClassName                    ();

                string                            getId                           () const;
                void                              setId                           (const string &id);

                OpenFlowControllerRole            getRole                         () const;
                void                              setRole                         (const OpenFlowControllerRole &openFlowControllerRole);

                IpV4AddressNetworkMask            getIpPrefix                     () const;
                void                              setIpPrefix                     (const IpV4AddressNetworkMask &ipPrefix);

                UI32                              getPort                         () const;
                void                              setPort                         (const UI32 &port);

                IpV4Address                       getLocalIpAddress               () const;
                void                              setLocalIpAddress               (const IpV4Address &localIpAddress);

                UI32                              getLocalPort                    () const;
                void                              setLocalPort                    (const UI32 &localPort);

                OpenFlowChannelConnectionProtocol getProtocol                     () const;
                void                              setProtocol                     (const OpenFlowChannelConnectionProtocol &protocol);

    // Now the data members

    private :
        string                            m_id;
        OpenFlowControllerRole            m_role;
        IpV4AddressNetworkMask            m_ipPrefix;
        UI32                              m_port;
        IpV4Address                       m_localIpAddress;
        UI32                              m_localPort;
        OpenFlowChannelConnectionProtocol m_protocol;

    protected :
    public :
};

}

#endif // OPENFLOWCONTROLLERMANAGEDOBJECT_H
