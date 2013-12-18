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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMGLOBALADDMACACLPOLICYMESSAGE_H
#define SSMGLOBALADDMACACLPOLICYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalAddMacAclPolicyMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmGlobalAddMacAclPolicyMessage ();
            SsmGlobalAddMacAclPolicyMessage (const string &policyName,const UI32 &policyType);
            virtual    ~SsmGlobalAddMacAclPolicyMessage ();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
			void setOldPolicyId(const ObjectId &policyId);
			ObjectId getOldPolicyId() const;
			void setNewPolicyId(const ObjectId &policyId);
			ObjectId getNewPolicyId() const;

// Now the data members

        private:
        protected:
        public:
            string   m_policyName;
            UI32     m_policyType;
			ObjectId m_oldPolicyId;
			ObjectId m_newPolicyId;
    };
}
#endif                                            //SSMGLOBALADDMACACLPOLICYMESSAGE_H
