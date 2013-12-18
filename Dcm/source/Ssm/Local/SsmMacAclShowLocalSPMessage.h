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

#ifndef SSMMACACLSHOWLOCALMESSAGE_H
#define SSMMACACLSHOWLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class SsmMacAclShowLocalSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmMacAclShowLocalSPMessage ();
            SsmMacAclShowLocalSPMessage (const string &ifname, const UI32 &binding, const UI32 &detail, const UI32 &iftype, const UI32 &opcode, bool isGwRequest);
            virtual    ~SsmMacAclShowLocalSPMessage ();

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

            string getPolicyName() const;
            string getIfName() const;
			UI32 getBinding() const;
			UI32 getDetail() const;
            UI32 getPolicyType() const;
            UI32 getIfType() const;
            UI32 getOpCode() const;
            bool getIsGwRequest ()  const;            
            void setPolicyName(const string &name);
            void setIfName(const string &ifname);
			void setBinding(const UI32 &binding);
			void setDetail(const UI32 &detail);
            void setIfType(const UI32 &iftype);
            void setPolicyType(const UI32 &type);
            void setOpcode(const UI32 &opcode);
            void setIsGwRequest (bool isGwRequest);

 //           void addCountFrames(const UI64 &count);

        private:
        protected:
        public:
			string		 m_policyName;
			UI32		 m_policyType;
			string		 m_ifname;
			UI32         m_binding;
			UI32         m_detail;
			UI32		 m_ifType;
			UI32		 m_opcode;
            bool    	 m_isGwRequest;
    };
}
#endif                                            
