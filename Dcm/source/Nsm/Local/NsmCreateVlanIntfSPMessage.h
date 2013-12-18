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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMCREATEVLANINTFSPMESSAGE_H
#define NSMCREATEVLANINTFSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmCreateVlanIntfSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmCreateVlanIntfSPMessage ();
            NsmCreateVlanIntfSPMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc);
            virtual    ~NsmCreateVlanIntfSPMessage ();
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setVlanIfIndex(const UI32 &vlanIfIndex);
            UI32 getVlanIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;

            bool isBulkVlanConfig() const;
            void setIsBulkVlanConfig(const bool &isBulk);
            void setRangeVlanStr(const string &RangeVlanStr);
            string getRangeVlanStr()  const;
            void  updateFailureStatus(const UI32 &vlanId, const SI32 &status);
            vector<UI32> getFailedVlanIds();
            vector<SI32> getFailureStatus();
// Now the data members

        private:
            bool m_bulkVlanConfig;
            string  m_RangeVlanStr;
            vector<UI32> m_failedVlanIds;
            vector<SI32> m_failureStatus;
        protected:
        public:
            UI32  m_vlanId;
            string  m_vlanName;
            UI32  m_vlanIfIndex;
            string  m_desc;
    };
}
#endif                                            //NSMCREATEVLANINTFSPMESSAGE_H
