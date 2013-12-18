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

#ifndef SSMMACACLPOLICYBULKMESSAGE_H
#define SSMMACACLPOLICYBULKMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Ssm/Local/SsmTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class SsmMacAclPolicyBulkMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmMacAclPolicyBulkMessage ();
            //SsmMacAclPolicyBulkMessage (const string &policyName,const UI32 &policyType,const UI32 &ruleid,const UI32 &action,const string &srcMac,const bool &srcMask,const string &dstMac,const bool &dstMask,const string &appType,const bool &isCount,const UI32 &opcode);
            virtual    ~SsmMacAclPolicyBulkMessage ();
			unsigned char *macAddressfromStringtoBytes      (const string srcDstMac);
            virtual const void *getCStructureForInputs    	();
            virtual       void  loadOutputsFromCStructure 	(const void *pOutputCStructure);
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
            vector<UI64> 	getRuleidVector()  const;
            vector<UI32> 	getActionVector()  const;
            vector<string> 	getSrcMacVector()  const;
            vector<string> 	getSrcMacMaskVector()  const;
            vector<string> 	getDstMacVector()  const;
            vector<string> 	getDstMacMaskVector()  const;
            vector<string> 	getAppTypeVector()  const;
            vector<bool> 	getIsCountVector()  const;
            vector<bool> 	getIsLogVector()  const;
            UI64 getNumRules()  const;
    		void setNumRules(const UI64 &rules);
			UI32 getOpCode()  const;
			void setOpCode(const UI32 &opcode);
			void setExtRuleParams(const UI64 &ruleid, const UI32 &action, const string &srcmac, 
				const string &srcMacMask, const string &dstmac, const string &dstMacMask, const string &appType, 
				const bool &iscount, const bool &islog, const UI32 &srcType, const UI32 &dstType, 
				const UI32 &appTypeInt, const UI32 &appTypeEnum);
			void setStdRuleParams(const UI64 &ruleid, const UI32 &action, const string &srcmac, 
				const string &srcMacMask, const bool &iscount, const bool &islog, const UI32 &srcType);

        private:
        protected:
        public:
			string          m_policyName;
            UI32            m_policyType;
			UI64			m_numRules;
            vector<UI64>    m_ruleid;
            vector<UI32>    m_action;
            vector<string>  m_srcMac;
            vector<string>  m_srcMacMask;
            vector<string>  m_dstMac;
            vector<string>  m_dstMacMask;
            vector<string>  m_appType;
            vector<bool>    m_isCount;
			vector<bool>    m_isLog;			
			UI32			m_opcode;
			vector<UI32>    m_srcType;
			vector<UI32>    m_dstType;
			vector<UI32>	m_appTypeInt;
			vector<UI32>    m_appTypeEnum;

    };
}
#endif                                            
