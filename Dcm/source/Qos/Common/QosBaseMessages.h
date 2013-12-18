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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/
#ifndef QOSBASEMESSAGES_H
#define QOSBASEMESSAGES_H 

#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"


using namespace WaveNs;


namespace DcmNs
{

    class QosGlobalNasMessage;
    class QosLocalNasMessage;

    class QosNasBaseMessage
    {
        public:
            QosNasBaseMessage ();
            QosNasBaseMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber);
            virtual    ~QosNasBaseMessage ();
            void setOpCode (const UI32 &opCode);
            UI32 getOpCode();
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode();
            void setCos (const UI32 &cos);
            UI32 getCos();
            void setDscp(const UI32 &dscp);
            UI32 getDscp();
            void setServerIpAddressPrefix(const IpV4AddressNetworkMask &serverIpAddressPrefix);
            IpV4AddressNetworkMask  getServerIpAddressPrefix();
            void setVrfName (const string &vrfName);
            string getVrfName ();
            void setVlanNumber (const UI32 &vlanNumber);
            UI32 getVlanNumber ();
            //void copyFromGlobal(QosGlobalNasMessage *pQosGlobalNasMessage);
            //void copyFromLocal (QosLocalNasMessage *pQosLocalNasMessage);


// Now the data members

            UI32 m_opCode;
            UI32 m_cmdCode;
            UI32 m_cos;
            UI32 m_dscp;
            IpV4AddressNetworkMask m_serverIpAddressPrefix;
            string m_vrfName;
            UI32 m_vlanNumber;
    };
}

#endif //QOSBASEMESSAGES_H

