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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef ZONEDBDELETEMESSAGE_H
#define ZONEDBDELETEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
// #include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneDbDeleteMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ZoneDbDeleteMessage ();
            ZoneDbDeleteMessage (const UI32 &dbEntryType,const string &dbEntryName,const UI32 &respCode);
            virtual    ~ZoneDbDeleteMessage ();
            void setDbEntryType(const UI32 &dbEntryType);
            UI32 getDbEntryType()  const;
            void setDbEntryName(const string &dbEntryName);
            string getDbEntryName()  const;
            void setRespCode(const UI32 &respCode);
            UI32 getRespCode()  const;
			void setParentMoObjectId (const ObjectId parentMoObjectId);
            ObjectId getParentMoObjectId ()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_dbEntryType;
            string  m_dbEntryName;
            UI32  m_respCode;
            ObjectId    m_parentMoObjectId;
    };
}
#endif                                            //ZONEDBDELETEMESSAGE_H
