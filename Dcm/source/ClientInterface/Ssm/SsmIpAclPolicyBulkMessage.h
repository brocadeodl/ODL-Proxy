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

#ifndef SSMIPACLPOLICYBULKMESSAGE_H
#define SSMIPACLPOLICYBULKMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Ssm/Local/SsmTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class SsmIpAclPolicyBulkMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmIpAclPolicyBulkMessage ();
            virtual    ~SsmIpAclPolicyBulkMessage ();
            virtual const void *getCStructureForInputs    	();
            virtual       void  loadOutputsFromCStructure 	(const void *pOutputCStructure);
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
            vector<UI64> 	getRuleidVector()  const;
            vector<UI32> 	getActionVector()  const;
            vector<bool> 	getIsCountVector()  const;
            vector<bool> 	getIsLogVector()  const;
            UI64 getNumRules()  const;
    		void setNumRules(const UI64 &rules);
			UI32 getOpCode()  const;
			void setOpCode(const UI32 &opcode);
			
			void setExtRuleParams(const UI32 &userEnteredSeqid, const UI64 &ruleid,
				const UI32 &action, const string &srcUnion, const UI32 &srcType, const IpV4Address &srcIpAddress,
				const IpV4Address &srcIpAddressMask, const IpV4Address &srcHostIpAddress,
				const bool &isCount, const bool &isLog, const string &dstUnion, const UI32 &dstType,
				const IpV4Address &dstIpAddress, const IpV4Address &dstIpAddressMask, const IpV4Address &dstHostIpAddress,
				const string &ProtocolTypeUnion, const UI32 &l3l4CustomProtoType, const UI32 &l3l4WellknownProtoType,
				const UI32 &srcPortOperator, const string &srcPortNumberTcpUnion, 
				const string &srcPortltNumberTcpUnion,
				const string &srcPortgtNumberTcpUnion,
				const string &srcPortNumberUdpUnion,
				const string &srcPortltNumberUdpUnion,
				const string &srcPortgtNumberUdpUnion,
				const string &srcPortrangeLowerTcpUnion, const string &srcPortrangeLowerUdpUnion,
				const string &srcPortrangeHigherTcpUnion, const string &srcPortrangeHigherUdpUnion,
				const UI32 &srcPortCustomNumberTcp, const UI32 &srcPortltCustomNumberTcp, 
				const UI32 &srcPortgtCustomNumberTcp,
				const UI32 &srcPortWellknownNumberTcp, 
				const UI32 &srcPortltWellknownNumberTcp,
				const UI32 &srcPortgtWellknownNumberTcp,
				const UI32 &srcPortCustomNumberUdp,
				const UI32 &srcPortltCustomNumberUdp,
				const UI32 &srcPortgtCustomNumberUdp,
				const UI32 &srcPortWellknownNumberUdp, 
				const UI32 &srcPortltWellknownNumberUdp,
				const UI32 &srcPortgtWellknownNumberUdp,
				const UI32 &srcPortCustomrangeLowerTcp,
				const UI32 &srcPortWellknownrangeLowerTcp,const UI32 &srcPortCustomrangeHigherTcp,
				const UI32 &srcPortWellknownrangeHigherTcp,const UI32 &srcPortCustomrangeLowerUdp,
				const UI32 &srcPortWellknownrangeLowerUdp,const UI32 &srcPortCustomrangeHigherUdp,
				const UI32 &srcPortWellknownrangeHigherUdp,
				const string &dstPortNumberTcpUnion,
				const string &dstPortltNumberTcpUnion,
				const string &dstPortgtNumberTcpUnion,
				const string &dstPortNumberUdpUnion, 
				const string &dstPortltNumberUdpUnion,
				const string &dstPortgtNumberUdpUnion,
				const string &dstPortrangeLowerTcpUnion,
				const string &dstPortrangeLowerUdpUnion, const string &dstPortrangeHigherTcpUnion,
				const string &dstPortrangeHigherUdpUnion, const UI32 &dstPortOperator, 
				const UI32 &dstPortCustomNumberTcp,	
				const UI32 &dstPortltCustomNumberTcp,
				const UI32 &dstPortgtCustomNumberTcp,
				const UI32 &dstPortWellknownNumberTcp,
				const UI32 &dstPortltWellknownNumberTcp,
				const UI32 &dstPortgtWellknownNumberTcp,
				const UI32 &dstPortCustomNumberUdp, 
				const UI32 &dstPortltCustomNumberUdp,
				const UI32 &dstPortgtCustomNumberUdp,
				const UI32 &dstPortWellknownNumberUdp,
				const UI32 &dstPortltWellknownNumberUdp,
				const UI32 &dstPortgtWellknownNumberUdp,
				const UI32 &dstPortCustomrangeLowerTcp, const UI32 &dstPortWellknownrangeLowerTcp,
				const UI32 &dstPortCustomrangeHigherTcp, const UI32 &dstPortWellknownrangeHigherTcp,
				const UI32 &dstPortCustomrangeLowerUdp, const UI32 &dstPortWellknownrangeLowerUdp,
				const UI32 &dstPortCustomrangeHigherUdp, const UI32 &dstPortWellknownrangeHigherUdp,
				const WellknownPrecedence &wellknownPrecedence, const WellknownTos &wellknownTos, 
				const WellknownDscp &wellknownDscp, const UI32 &customPrecedence, const UI32 &customTos, 
				const UI32 &customDscp, const string &precedenceUnion, const string &tosUnion, const string &dscpUnion, 
				const bool &urg, const bool &ack, const bool &push, const bool &fin, const bool &rst, const bool &sync);
				
			void setStdRuleParams(const UI32 &userEnteredSeqid, const UI64 &ruleid, const UI32 &action, 
				const string &srcUnion, const UI32 &srcType, const IpV4Address &srcIpAddress,
				const IpV4Address &srcIpAddressMask, const IpV4Address &srcHostIpAddress,
				const bool &iscount, const bool &islog);	

        private:
        protected:
        public:

			string							m_policyName;
            UI32							m_policyType;
			UI64							m_numRules;
            vector<UI64>					m_ruleid;
            vector<UI32>					m_action;
            vector<string>                  m_srcUnion;
            vector<UI32>              		m_srcType;
            vector<IpV4Address>             m_srcHostIpAddress;
            vector<IpV4Address>             m_srcIpAddress;
            vector<IpV4Address>             m_srcIpAddressMask;
            vector<bool>                    m_isCount;
            vector<bool>                    m_isLog;
            vector<IpV4Address>             m_dstHostIpAddress;
            vector<IpV4Address>             m_dstIpAddress;
            vector<IpV4Address>             m_dstIpAddressMask;
            vector<string>                  m_dstUnion;
            vector<UI32>              		m_dstType;
            vector<string>                  m_ProtocolTypeUnion;
            vector<UI32>                    m_l3l4CustomProtoType;
            vector<UI32>  					m_l3l4WellknownProtoType;
            vector<UI32>            		m_srcPortOperator;
            vector<string>                  m_srcPortNumberTcpUnion;
            vector<string>                  m_srcPortltNumberTcpUnion;
            vector<string>                  m_srcPortgtNumberTcpUnion;
            vector<string>                  m_srcPortNumberUdpUnion;
            vector<string>                  m_srcPortltNumberUdpUnion;
            vector<string>                  m_srcPortgtNumberUdpUnion;
            vector<string>                  m_srcPortrangeLowerTcpUnion;
            vector<string>                  m_srcPortrangeLowerUdpUnion;
            vector<string>                  m_srcPortrangeHigherTcpUnion;
            vector<string>                  m_srcPortrangeHigherUdpUnion;
            vector<UI32>                    m_srcPortCustomNumberTcp;
            vector<UI32>                    m_srcPortltCustomNumberTcp;
            vector<UI32>                    m_srcPortgtCustomNumberTcp;
            vector<UI32>  					m_srcPortWellknownNumberTcp;
            vector<UI32>  					m_srcPortltWellknownNumberTcp;
            vector<UI32>  					m_srcPortgtWellknownNumberTcp;
            vector<UI32>                    m_srcPortCustomNumberUdp;
            vector<UI32>                    m_srcPortltCustomNumberUdp;
            vector<UI32>                    m_srcPortgtCustomNumberUdp;
            vector<UI32>  					m_srcPortWellknownNumberUdp;
            vector<UI32>  					m_srcPortltWellknownNumberUdp;
            vector<UI32>  					m_srcPortgtWellknownNumberUdp;
            vector<UI32>                    m_srcPortCustomrangeLowerTcp;
            vector<UI32>  					m_srcPortWellknownrangeLowerTcp;
            vector<UI32>                    m_srcPortCustomrangeHigherTcp;
            vector<UI32>  					m_srcPortWellknownrangeHigherTcp;
            vector<UI32>                    m_srcPortCustomrangeLowerUdp;
            vector<UI32>  					m_srcPortWellknownrangeLowerUdp;
            vector<UI32>                    m_srcPortCustomrangeHigherUdp;
            vector<UI32>  					m_srcPortWellknownrangeHigherUdp;
            vector<string>                  m_dstPortNumberTcpUnion;
            vector<string>                  m_dstPortltNumberTcpUnion;
            vector<string>                  m_dstPortgtNumberTcpUnion;
            vector<string>                  m_dstPortNumberUdpUnion;
            vector<string>                  m_dstPortltNumberUdpUnion;
            vector<string>                  m_dstPortgtNumberUdpUnion;
            vector<string>                  m_dstPortrangeLowerTcpUnion;
            vector<string>                  m_dstPortrangeLowerUdpUnion;
            vector<string>                  m_dstPortrangeHigherTcpUnion;
            vector<string>                  m_dstPortrangeHigherUdpUnion;
            vector<UI32>            		m_dstPortOperator;
            vector<UI32>                    m_dstPortCustomNumberTcp;
            vector<UI32>                    m_dstPortltCustomNumberTcp;
            vector<UI32>                    m_dstPortgtCustomNumberTcp;
            vector<UI32>  					m_dstPortWellknownNumberTcp;
            vector<UI32>  					m_dstPortltWellknownNumberTcp;
            vector<UI32>  					m_dstPortgtWellknownNumberTcp;
            vector<UI32>                    m_dstPortCustomNumberUdp;
            vector<UI32>                    m_dstPortltCustomNumberUdp;
            vector<UI32>                    m_dstPortgtCustomNumberUdp;
            vector<UI32>  					m_dstPortWellknownNumberUdp;
            vector<UI32>  					m_dstPortltWellknownNumberUdp;
            vector<UI32>  					m_dstPortgtWellknownNumberUdp;
            vector<UI32>                    m_dstPortCustomrangeLowerTcp;
            vector<UI32>				  	m_dstPortWellknownrangeLowerTcp;
            vector<UI32>                    m_dstPortCustomrangeHigherTcp;
            vector<UI32>  					m_dstPortWellknownrangeHigherTcp;
            vector<UI32>                    m_dstPortCustomrangeLowerUdp;
            vector<UI32>  					m_dstPortWellknownrangeLowerUdp;
            vector<UI32>                    m_dstPortCustomrangeHigherUdp;
            vector<UI32>  					m_dstPortWellknownrangeHigherUdp;
            vector<UI32>                    m_customPrecedence;
            vector<UI32> 			        m_wellknownPrecedence;
            vector<UI32>                    m_customTos;
            vector<UI32>		            m_wellknownTos;
            vector<UI32>                    m_customDscp;
            vector<UI32> 		            m_wellknownDscp;
            vector<string>                  m_precedenceUnion;
            vector<string>                  m_tosUnion;
            vector<string>                  m_dscpUnion;
            vector<bool>                    m_urg;
            vector<bool>                    m_ack;
            vector<bool>                    m_push;
            vector<bool>                    m_fin;
            vector<bool>                    m_rst;
            vector<bool>                    m_sync;
            UI32                    		m_opcode;
            vector<UI32>                    m_userEnteredSeqid;

    };
}
#endif //SSMIPACLPOLICYBULKMESSAGE_H                                           
