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

#ifndef NSMGLOBALCREATEVLANINTFMESSAGE_H
#define NSMGLOBALCREATEVLANINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalCreateVlanIntfMessage : public ManagementInterfaceMessage
    {
        private:
            virtual void setupAttributesForSerialization();
        protected:
        public:
            NsmGlobalCreateVlanIntfMessage ();
            NsmGlobalCreateVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc);
            NsmGlobalCreateVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc, UI32 type);
            virtual    ~NsmGlobalCreateVlanIntfMessage ();
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setVlanIfIndex(const UI32 &vlanIfIndex);
            UI32 getVlanIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setType(const UI32 &type);
            UI32 getType()  const;
            string getRangeVlanStr()  const;
            void setRangeVlanStr(const string &RangeVlanStr);
            bool isBulkVlanConfig() const;
            void setIsBulkVlanConfig(const bool &isBulk);
            void updateFailureStatus(const UI32 &vlanId, vector<string> &statusRbridge, const string &successRbridges);
            vector<UI32> getFailedVlanIds();
            map<UI32, map<SI32, string> > getFailureStatusRbridgeMappings();
            map<UI32, string> getSuccessfulRbridgesRange();

// Now the data members

        private:
            bool m_bulkVlanConfig;
            string m_RangeVlanStr;
            vector<UI32> failedVlanIds;
            vector<vector<string> > failStatusRbridgeMapping;
            vector<string> successfulRbridgesRange;
        protected:
        public:
            UI32  m_vlanId;
            string  m_vlanName;
            UI32  m_vlanIfIndex;
            string  m_desc;
            UI32  m_type;
    };
}
#endif                                            //NSMGLOBALCREATEVLANINTFMESSAGE_H
