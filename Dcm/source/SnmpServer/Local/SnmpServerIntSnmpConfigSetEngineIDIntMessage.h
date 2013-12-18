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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skumaras                                                     *
 **************************************************************************/

#ifndef SNMPSERVERINTSNMPCONFIGSETENGINEIDINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGSETENGINEIDINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigSetEngineIDIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigSetEngineIDIntMessage ();
            SnmpServerIntSnmpConfigSetEngineIDIntMessage (const string &EngineID);
            virtual    ~SnmpServerIntSnmpConfigSetEngineIDIntMessage ();
            void setEngineID(const string &EngineID);
            string getEngineID()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_EngineID;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGSETENGINEIDINTMESSAGE_H
