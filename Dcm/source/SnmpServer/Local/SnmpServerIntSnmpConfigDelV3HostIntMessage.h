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

#ifndef SNMPSERVERINTSNMPCONFIGDELV3HOSTINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGDELV3HOSTINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/HostUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigDelV3HostIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigDelV3HostIntMessage ();
            SnmpServerIntSnmpConfigDelV3HostIntMessage (const HostUC &ip,const string &username);
            virtual    ~SnmpServerIntSnmpConfigDelV3HostIntMessage ();
            void setHostUC(const HostUC &ip);
            HostUC getHostUC()  const;
            void setUsername(const string &username);
            string getUsername()  const;

// Now the data members

        private:
        protected:
        public:
            HostUC  m_ip;
            string  m_username;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGDELV3HOSTINTMESSAGE_H
