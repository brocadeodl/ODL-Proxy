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
 *   Author : Ojas                                                     *
 **************************************************************************/

#ifndef SSMIPACLSHOWMESSAGE_H
#define SSMIPACLSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class SsmIpAclShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmIpAclShowMessage ();
            virtual    ~SsmIpAclShowMessage ();

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void getOutputBufferRef(UI32 bufferNum, UI32 &size, void *&pBuffer);

            string getPolicyName() const;
            string getIfName() const;
			UI32 getBinding() const;
			UI32 getDetail() const;
            UI32 getPolicyType() const;
            UI32 getIfType() const;
            UI32 getOpCode() const;
            UI32 getMappedId ()  const;
            void setPolicyName(const string &name);
            void setIfName(const string &ifname);
			void setBinding(const UI32 &binding);
			void setDetail(const UI32 &detail);
            void setIfType(const UI32 &iftype);
            void setPolicyType(const UI32 &type);
            void setOpcode(const UI32 &opcode);
            void setMappedId (const UI32 &mappedId);

        private:
        protected:
        public:
			string		 m_policyName;
			UI32		 m_policyType;
			string		 m_ifname;
			UI32 		 m_binding;
			UI32 		 m_detail;
			UI32		 m_ifType;
			UI32		 m_opcode;
            UI32    	 m_mappedId;

    };
}
#endif                                            
