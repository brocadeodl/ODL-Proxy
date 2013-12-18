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

#ifndef SNMPSERVERINTSNMPCONFIGDELHOSTINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGDELHOSTINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigDelHostIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigDelHostIntMessage ();
            SnmpServerIntSnmpConfigDelHostIntMessage (const HostUC &ip,const string &community);
            virtual    ~SnmpServerIntSnmpConfigDelHostIntMessage ();
			void setHostUC(const HostUC &ip);
            HostUC getHostUC()  const;
            void setCommunity(const string &community);
            string getCommunity()  const;

// Now the data members

        private:
        protected:
        public:
            HostUC m_ip;
            string  m_community;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGDELHOSTINTMESSAGE_H