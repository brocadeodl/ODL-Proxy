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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Types/HostUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigChangeV3HostIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigChangeV3HostIntMessage ();
            SnmpServerIntSnmpConfigChangeV3HostIntMessage (const HostUC &ip,const string &username, const NotifyType &notify,const UI32 &udpPort,const string &engineid, const SevLevel &sevLevel);
            virtual    ~SnmpServerIntSnmpConfigChangeV3HostIntMessage ();
            void setHostUC(const HostUC &ip);
            HostUC getHostUC()  const;
            void setNotify(const NotifyType &notify);
            NotifyType getNotify()  const;
            void setUdpPort(const UI32 &udpPort);
            UI32 getUdpPort()  const;
            void setUsername(const string &userName);
            string getUsername()  const;
            void setEngineid(const string &engineid);
            string getEngineid()  const;
            void setSevLevel(const SevLevel &sevLevel);
            SevLevel getSevLevel()  const;


// Now the data members

        private:
        protected:
        public:
            HostUC  m_ip;
            NotifyType  m_notify;
            UI32  m_udpPort;
            string  m_username;
            string  m_engineid;
            SevLevel m_sevLevel;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINTMESSAGE_H
