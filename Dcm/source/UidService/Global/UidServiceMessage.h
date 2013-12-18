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
 *   Author : sholla                                                     *
 **************************************************************************/

#ifndef UIDSERVICEMESSAGE_H
#define UIDSERVICEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class UidServiceMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            UidServiceMessage (const UI32 opcode);
            UidServiceMessage (const UI32 opcode, const string &entityName,const UI32 &idType,const UI32 &id);
            virtual    ~UidServiceMessage ();
            void setEntityName(const string &entityName);
            string getEntityName()  const;
            void setIdType(const UI32 &idType);
            UI32 getIdType()  const;
            void setId(const UI32 &id);
            UI32 getId()  const;

            bool isEntityNamePresent();
            bool isIdTypePresent();
            bool isIdPresent();

// Now the data members

        private:
        protected:
        public:
            string  m_entityName;
            UI32  m_idType;
            UI32  m_id;
    };
}
#endif                                            //UIDSERVICEMESSAGE_H
