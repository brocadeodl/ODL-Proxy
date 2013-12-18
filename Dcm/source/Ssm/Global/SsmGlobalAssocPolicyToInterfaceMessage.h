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

#ifndef SSMGLOBALASSOCPOLICYTOINTERFACEMESSAGE_H
#define SSMGLOBALASSOCPOLICYTOINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalAssocPolicyToInterfaceMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmGlobalAssocPolicyToInterfaceMessage ();
            SsmGlobalAssocPolicyToInterfaceMessage (const string &ifName,const string &policyName,const UI32 &opcode);
            virtual    ~SsmGlobalAssocPolicyToInterfaceMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            string  m_policyName;
            UI32  m_opcode;
    };
}
#endif                                            //SSMGLOBALASSOCPOLICYTOINTERFACEMESSAGE_H
