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
 *   Copyright (C) 2005 - 2011 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef MANAGEMENTINTERFACEMESSAGE_H
#define MANAGEMENTINTERFACEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
//#include "Framework/Attributes/AttributesMap.h"

namespace WaveNs
{

class ManagementInterfaceMessage : public PrismMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                              ManagementInterfaceMessage    (const string &serviceName, const UI32 &operationId);
        virtual              ~ManagementInterfaceMessage    ();

                string        getServiceName                () const;

                string        getClientIpAddress            () const;
                void          setClientIpAddress            (const string &clientIpAddress);

                UI32          getClientPort                 () const;
                void          setClientPort                 (const UI32 &clientPort);

                UI32          getClientId                   () const;
                void          setClientId                   (const UI32 &clientId);

                string        getClientName                 () const;
                void          setClientName                 (const string &clientName);

                string        getClientNameWithoutExtension () const;
                void          setClientNameWithoutExtension (const string &clientNameWithoutExtension);

                UI32          getServerId                   () const;
                void          setServerId                   (const UI32 &serverId);

                SI32          getSlotInstance               () const;
                void          setSlotInstance               (const SI32 &instance);

                bool          getAllWaveClients             () const;
                void          setAllWaveClients             (const bool &allWaveClients);

                string        getTtyName                    () const;
                void          setTtyName                    (const string &ttyName);

        virtual PrismMessage *clone                         ();

    // Now the data members

    private :
        string m_serviceName;
        string m_clientIpAddress;
        UI32   m_clientPort;
        UI32   m_clientId;
        string m_clientName;
        string m_clientNameWithoutExtension;
        UI32   m_serverId;
        SI32   m_slotInstance;
        bool   m_allWaveClients;
        string m_ttyName;

    protected :
    public :
};

}

#endif // MANAGEMENTINTERFACEMESSAGE_H
