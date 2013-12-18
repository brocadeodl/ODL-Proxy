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

#ifndef SNMPSERVERINTSNMPCONFIGSETCOMMINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGSETCOMMINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "SnmpServer/Global/enumdatatype.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigSetCommIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigSetCommIntMessage ();
            SnmpServerIntSnmpConfigSetCommIntMessage (const string &community,const Access &access);
            virtual    ~SnmpServerIntSnmpConfigSetCommIntMessage ();
            void setCommunity(const string &community);
            string getCommunity()  const;
            void setAccess(const Access &access);
            Access getAccess()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_community;
            Access  m_access;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGSETCOMMINTMESSAGE_H
