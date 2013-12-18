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

#ifndef ZONEOPERATIONINFOMESSAGE_H
#define ZONEOPERATIONINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneOperationInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ZoneOperationInfoMessage ();
            ZoneOperationInfoMessage (const UI32 &dbTransToken,const UI32 &dbTransFlag,const UI32 &dbMaxSize,const UI32 &dbAvailableSize,const UI32 &dbCommittedSize,const UI32 &dbTransactionSize,const string &lastZoneChangedTimestamp,const string &lastZoneCommittedTimestamp);
            virtual    ~ZoneOperationInfoMessage ();
            void setDbTransToken(const UI32 &dbTransToken);
            UI32 getDbTransToken()  const;
            void setDbTransFlag(const UI32 &dbTransFlag);
            UI32 getDbTransFlag()  const;
            void setDbMaxSize(const UI32 &dbMaxSize);
            UI32 getDbMaxSize()  const;
            void setDbAvailableSize(const UI32 &dbAvailableSize);
            UI32 getDbAvailableSize()  const;
            void setDbCommittedSize(const UI32 &dbCommittedSize);
            UI32 getDbCommittedSize()  const;
            void setDbTransactionSize(const UI32 &dbTransactionSize);
            UI32 getDbTransactionSize()  const;
            void setLastZoneChangedTimestamp(const string &lastZoneChangedTimestamp);
            string getLastZoneChangedTimestamp()  const;
			void setLastZoneCommittedTimestamp(const string &lastZoneCommittedTimestamp);
            string getLastZoneCommittedTimestamp()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_dbTransToken;
            UI32  m_dbTransFlag;
            UI32  m_dbMaxSize;
            UI32  m_dbAvailableSize;
            UI32  m_dbCommittedSize;
            UI32  m_dbTransactionSize;
            string  m_lastZoneChangedTimestamp;
            string  m_lastZoneCommittedTimestamp;
    };
}
#endif                                            //ZONEOPERATIONINFOMESSAGE_H
