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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Brunda                                                       *
 **************************************************************************/

#ifndef SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "SnmpServer/Global/enumdatatype.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigSetContextMappingIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigSetContextMappingIntMessage ();
            SnmpServerIntSnmpConfigSetContextMappingIntMessage (const string &contextName,const string &vrfName);
            virtual    ~SnmpServerIntSnmpConfigSetContextMappingIntMessage ();
            void setContextName(const string &contextName);
            string getContextName()  const;
            void setVrfName(const string &vrfName);
            string getVrfName()  const;
		
// Now the data members

        private:
        protected:
        public:
            string  m_contextName;
            string  m_vrfName;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINTMESSAGE_H
