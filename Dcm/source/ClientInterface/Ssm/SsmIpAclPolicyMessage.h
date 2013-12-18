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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef SSMIPACLPOLICYMESSAGE_H
#define SSMIPACLPOLICYMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ssmAcl_dcmd_public.h"
#include "Framework/Attributes/AttributeEnum.h"
using namespace WaveNs;

namespace DcmNs
{

    class SsmIpAclPolicyMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmIpAclPolicyMessage ();
            SsmIpAclPolicyMessage (const string &policyName, const UI32 &policyType, const UI32 opCode);
			SsmIpAclPolicyMessage (const string &policyName, const string &ifName, const UI32 &policyType, const UI32 opCode, const ssmAcl_binding_direction direction);
            virtual    ~SsmIpAclPolicyMessage ();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            void setDirection(const ssmAcl_binding_direction &direction);
			ssmAcl_binding_direction getDirection () const;
            void setIfname(const string &ifname);
            string getIfname()  const;
	
			
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            string  					m_policyName;
            string						m_ifName;
			UI32  						m_policyType;
            UI32  						m_opCode;
			ssmAcl_binding_direction    m_direction;
    };
}
#endif                                            //SSMIPACLPOLICYMESSAGE_H
