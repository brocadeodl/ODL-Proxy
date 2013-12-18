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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brunda                                                     *
 **************************************************************************/

#ifndef SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigRemoveContextMappingIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ();
            SnmpServerIntSnmpConfigRemoveContextMappingIntMessage (const string &contextName,const string &delFieldName);
            virtual    ~SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ();
            void setContextName(const string &contextName);
            string getContextName()  const;
            void setDelFieldName(const string &delFieldName);
            string getDelFieldName()const;

// Now the data members

        private:
            string  m_contextName;
            string  m_delFieldName;
        protected:
        public:

    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINTMESSAGE_H
