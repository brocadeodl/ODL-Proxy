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

#ifndef SSMGLOBALRESEQMACACLPOLICYMESSAGE_H
#define SSMGLOBALRESEQMACACLPOLICYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalReseqMacAclPolicyMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmGlobalReseqMacAclPolicyMessage ();
            SsmGlobalReseqMacAclPolicyMessage  (const string &policyName,const UI64 &SeqId,const UI64 &IncSeqId);
            virtual    ~SsmGlobalReseqMacAclPolicyMessage ();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
			void setSeqId(const  UI64   &SeqId);
		    UI64	 getSeqId() const;
			void  setIncSeqId(const  UI64 &IncSeqId);
			UI64  getIncSeqId() const;

// Now the data members

        private:
        protected:
        public:
            string   m_policyName;
            UI64     m_seqId;
            UI64     m_incSeqId;
    };
}
#endif                                            //SsmGlobalReseqMacAclPolicyMessage_H
