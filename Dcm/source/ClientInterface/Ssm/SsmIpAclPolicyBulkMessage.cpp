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

#include "ClientInterface/Ssm/SsmIpAclPolicyBulkMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmIpAclPolicyBulkMessage::SsmIpAclPolicyBulkMessage ()
        : DcmManagementInterfaceMessage (SSMIPACLBULKPOLICY),
		m_numRules (0)
    {
    }

    SsmIpAclPolicyBulkMessage::~SsmIpAclPolicyBulkMessage ()
    {
    }

    void  SsmIpAclPolicyBulkMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType, "policyType"));
        addSerializableAttribute (new AttributeUI64(&m_numRules, "numRules"));
		addSerializableAttribute (new AttributeUI64Vector(&m_ruleid,"ruleid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_action, "action"));
        addSerializableAttribute (new AttributeStringVector(&m_srcUnion, "srcUnion"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcType, "srcType"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_srcHostIpAddress, "srcHostIpAddress"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_srcIpAddress, "srcIpAddress"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_srcIpAddressMask, "srcIpAddressMask"));
        addSerializableAttribute (new AttributeBoolVector(&m_isCount, "isCount"));
        addSerializableAttribute (new AttributeBoolVector(&m_isLog, "isLog"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_dstHostIpAddress, "dstHostIpAddress"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_dstIpAddress, "dstIpAddress"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_dstIpAddressMask, "dstIpAddressMask"));
        addSerializableAttribute (new AttributeStringVector(&m_dstUnion, "dstUnion"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstType, "dstType"));
        addSerializableAttribute (new AttributeStringVector(&m_ProtocolTypeUnion, "ProtocolTypeUnion"));
        addSerializableAttribute (new AttributeUI32Vector(&m_l3l4CustomProtoType, "l3l4CustomProtoType"));
        addSerializableAttribute (new AttributeUI32Vector(&m_l3l4WellknownProtoType, "l3l4WellknownProtoType"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortOperator, "srcPortOperator"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortNumberTcpUnion, "srcPortNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortltNumberTcpUnion, "srcPortltNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortgtNumberTcpUnion, "srcPortgtNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortNumberUdpUnion, "srcPortNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortltNumberUdpUnion, "srcPortltNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortgtNumberUdpUnion, "srcPortgtNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortrangeLowerTcpUnion, "srcPortrangeLowerTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortrangeLowerUdpUnion, "srcPortrangeLowerUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortrangeHigherTcpUnion, "srcPortrangeHigherTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_srcPortrangeHigherUdpUnion, "srcPortrangeHigherUdpUnion"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomNumberTcp, "srcPortCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortltCustomNumberTcp, "srcPortltCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortgtCustomNumberTcp, "srcPortgtCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownNumberTcp,"srcPortWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortltWellknownNumberTcp,"srcPortltWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortgtWellknownNumberTcp,"srcPortgtWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomNumberUdp,"srcPortCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortltCustomNumberUdp,"srcPortltCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortgtCustomNumberUdp,"srcPortgtCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownNumberUdp,"srcPortWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortltWellknownNumberUdp,"srcPortltWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortgtWellknownNumberUdp,"srcPortgtWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomrangeLowerTcp,"srcPortCustomrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownrangeLowerTcp,
            "srcPortWellknownrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomrangeHigherTcp,
            "srcPortCustomrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownrangeHigherTcp,
            "srcPortWellknownrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomrangeLowerUdp, "srcPortCustomrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownrangeLowerUdp,
            "srcPortWellknownrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortCustomrangeHigherUdp,
            "srcPortCustomrangeHigherUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_srcPortWellknownrangeHigherUdp,
            "srcPortWellknownrangeHigherUdp"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortNumberTcpUnion, "dstPortNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortltNumberTcpUnion, "dstPortltNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortgtNumberTcpUnion, "dstPortgtNumberTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortNumberUdpUnion, "dstPortNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortltNumberUdpUnion, "dstPortltNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortgtNumberUdpUnion, "dstPortgtNumberUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortrangeLowerTcpUnion, "dstPortrangeLowerTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortrangeLowerUdpUnion, "dstPortrangeLowerUdpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortrangeHigherTcpUnion, "dstPortrangeHigherTcpUnion"));
        addSerializableAttribute (new AttributeStringVector(&m_dstPortrangeHigherUdpUnion, "dstPortrangeHigherUdpUnion"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortOperator, "dstPortOperator"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomNumberTcp, "dstPortCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortltCustomNumberTcp, "dstPortltCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortgtCustomNumberTcp, "dstPortgtCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownNumberTcp, "dstPortWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortltWellknownNumberTcp, "dstPortltWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortgtWellknownNumberTcp, "dstPortgtWellknownNumberTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomNumberUdp, "dstPortCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortltCustomNumberUdp, "dstPortltCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortgtCustomNumberUdp, "dstPortgtCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownNumberUdp, "dstPortWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortltWellknownNumberUdp, "dstPortltWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortgtWellknownNumberUdp, "dstPortgtWellknownNumberUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomrangeLowerTcp,
            "dstPortCustomrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownrangeLowerTcp,
            "dstPortWellknownrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomrangeHigherTcp, "dstPortCustomrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownrangeHigherTcp,
            "dstPortWellknownrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomrangeLowerUdp, "dstPortCustomrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownrangeLowerUdp,
            "dstPortWellknownrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortCustomrangeHigherUdp,
            "dstPortCustomrangeHigherUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dstPortWellknownrangeHigherUdp,
            "dstPortWellknownrangeHigherUdp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_customPrecedence, "customPrecedence"));
        addSerializableAttribute (new AttributeUI32Vector(&m_wellknownPrecedence, "wellknownPrecedence"));
        addSerializableAttribute (new AttributeUI32Vector(&m_customTos, "customTos"));
        addSerializableAttribute (new AttributeUI32Vector(&m_wellknownTos, "wellknownTos"));
        addSerializableAttribute (new AttributeUI32Vector(&m_customDscp, "customDscp"));
        addSerializableAttribute (new AttributeUI32Vector(&m_wellknownDscp, "wellknownDscp"));
		addSerializableAttribute (new AttributeStringVector(&m_precedenceUnion, "precedenceUnion"));
		addSerializableAttribute (new AttributeStringVector(&m_dscpUnion, "dscpUnion"));
		addSerializableAttribute (new AttributeStringVector(&m_tosUnion, "tosUnion"));
        addSerializableAttribute (new AttributeBoolVector(&m_urg, "urg"));
        addSerializableAttribute (new AttributeBoolVector(&m_ack, "ack"));
        addSerializableAttribute (new AttributeBoolVector(&m_push, "push"));
        addSerializableAttribute (new AttributeBoolVector(&m_fin, "fin"));
        addSerializableAttribute (new AttributeBoolVector(&m_rst, "rst"));
        addSerializableAttribute (new AttributeBoolVector(&m_sync, "sync"));
        addSerializableAttribute (new AttributeUI32Vector(&m_userEnteredSeqid, "userEnteredSeqid"));
		addSerializableAttribute (new AttributeUI32(&m_opcode, "opcode"));
    }

    void  SsmIpAclPolicyBulkMessage::setPolicyName(const string &policyName)
    {
        m_policyName = policyName;
    }

    string  SsmIpAclPolicyBulkMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmIpAclPolicyBulkMessage::setOpCode(const UI32 &opCode)
    {
        m_opcode  =  opCode;
    }

    UI32  SsmIpAclPolicyBulkMessage::getOpCode() const
    {
        return (m_opcode);
    }

    void  SsmIpAclPolicyBulkMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmIpAclPolicyBulkMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmIpAclPolicyBulkMessage::setNumRules(const UI64 &rules)
    {
        m_numRules  =  rules;
    }

	void SsmIpAclPolicyBulkMessage::setStdRuleParams(const UI32 &userEnteredSeqid, const UI64 &ruleid, 
		const UI32 &action, const string &srcUnion, const UI32 &srcType, const IpV4Address &srcIpAddress, 
		const IpV4Address &srcIpAddressMask, const IpV4Address &srcHostIpAddress, 
		const bool &isCount, const bool &isLog)
	{
		m_userEnteredSeqid.push_back(userEnteredSeqid);
		m_ruleid.push_back(ruleid);
		m_action.push_back(action); 
		m_srcUnion.push_back(srcUnion);
		m_srcType.push_back(srcType);
		m_srcIpAddress.push_back(srcIpAddress);
        m_srcIpAddressMask.push_back(srcIpAddressMask);
		m_srcHostIpAddress.push_back(srcHostIpAddress);		
        m_isCount.push_back(isCount);
        m_isLog.push_back(isLog);
	}
	
	void SsmIpAclPolicyBulkMessage::setExtRuleParams(const UI32 &userEnteredSeqid, const UI64 &ruleid,
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
		const UI32 &srcPortCustomNumberTcp, 
		const UI32 &srcPortltCustomNumberTcp,
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
        const UI32 &srcPortWellknownrangeHigherUdp,const string &dstPortNumberTcpUnion,
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
		const bool &urg, const bool &ack, const bool &push, const bool &fin, const bool &rst, const bool &sync)
	{
        m_userEnteredSeqid.push_back(userEnteredSeqid);
        m_ruleid.push_back(ruleid);
        m_action.push_back(action);
        m_srcUnion.push_back(srcUnion);
        m_srcType.push_back(srcType);
        m_srcIpAddress.push_back(srcIpAddress);
        m_srcIpAddressMask.push_back(srcIpAddressMask);
        m_srcHostIpAddress.push_back(srcHostIpAddress);
        m_isCount.push_back(isCount);
        m_isLog.push_back(isLog);
        m_dstUnion.push_back(dstUnion);
        m_dstType.push_back(dstType);
        m_dstIpAddress.push_back(dstIpAddress);
        m_dstIpAddressMask.push_back(dstIpAddressMask);
		m_dstHostIpAddress.push_back(dstHostIpAddress);
		m_ProtocolTypeUnion.push_back(ProtocolTypeUnion);
		m_l3l4CustomProtoType.push_back(l3l4CustomProtoType);
		m_l3l4WellknownProtoType.push_back(l3l4WellknownProtoType);
		m_srcPortOperator.push_back(srcPortOperator);
		m_srcPortNumberTcpUnion.push_back(srcPortNumberTcpUnion);
		m_srcPortltNumberTcpUnion.push_back(srcPortltNumberTcpUnion);
		m_srcPortgtNumberTcpUnion.push_back(srcPortgtNumberTcpUnion);
        m_srcPortNumberUdpUnion.push_back(srcPortNumberUdpUnion);
        m_srcPortltNumberUdpUnion.push_back(srcPortltNumberUdpUnion);
        m_srcPortgtNumberUdpUnion.push_back(srcPortgtNumberUdpUnion);
        m_srcPortrangeLowerTcpUnion.push_back(srcPortrangeLowerTcpUnion);
        m_srcPortrangeLowerUdpUnion.push_back(srcPortrangeLowerUdpUnion);
        m_srcPortrangeHigherTcpUnion.push_back(srcPortrangeHigherTcpUnion);
        m_srcPortrangeHigherUdpUnion.push_back(srcPortrangeHigherUdpUnion);
		m_srcPortCustomNumberTcp.push_back(srcPortCustomNumberTcp);
		m_srcPortltCustomNumberTcp.push_back(srcPortltCustomNumberTcp);
		m_srcPortgtCustomNumberTcp.push_back(srcPortgtCustomNumberTcp);
		m_srcPortWellknownNumberTcp.push_back(srcPortWellknownNumberTcp);
		m_srcPortltWellknownNumberTcp.push_back(srcPortltWellknownNumberTcp);
		m_srcPortgtWellknownNumberTcp.push_back(srcPortgtWellknownNumberTcp);
		m_srcPortCustomNumberUdp.push_back(srcPortCustomNumberUdp);
		m_srcPortltCustomNumberUdp.push_back(srcPortltCustomNumberUdp);
		m_srcPortgtCustomNumberUdp.push_back(srcPortgtCustomNumberUdp);
		m_srcPortWellknownNumberUdp.push_back(srcPortWellknownNumberUdp);
		m_srcPortltWellknownNumberUdp.push_back(srcPortltWellknownNumberUdp);
		m_srcPortgtWellknownNumberUdp.push_back(srcPortgtWellknownNumberUdp);
		m_srcPortCustomrangeLowerTcp.push_back(srcPortCustomrangeLowerTcp);
		m_srcPortWellknownrangeLowerTcp.push_back(srcPortWellknownrangeLowerTcp);
		m_srcPortCustomrangeHigherTcp.push_back(srcPortCustomrangeHigherTcp);
		m_srcPortWellknownrangeHigherTcp.push_back(srcPortWellknownrangeHigherTcp);
		m_srcPortCustomrangeLowerUdp.push_back(srcPortCustomrangeLowerUdp);
		m_srcPortWellknownrangeLowerUdp.push_back(srcPortWellknownrangeLowerUdp);
		m_srcPortCustomrangeHigherUdp.push_back(srcPortCustomrangeHigherUdp);
		m_srcPortWellknownrangeHigherUdp.push_back(srcPortWellknownrangeHigherUdp);
		m_dstPortNumberTcpUnion.push_back(dstPortNumberTcpUnion);
		m_dstPortltNumberTcpUnion.push_back(dstPortltNumberTcpUnion);
		m_dstPortgtNumberTcpUnion.push_back(dstPortgtNumberTcpUnion);
		m_dstPortNumberUdpUnion.push_back(dstPortNumberUdpUnion);
		m_dstPortltNumberUdpUnion.push_back(dstPortltNumberUdpUnion);
		m_dstPortgtNumberUdpUnion.push_back(dstPortgtNumberUdpUnion);
		m_dstPortrangeLowerTcpUnion.push_back(dstPortrangeLowerTcpUnion);
		m_dstPortrangeLowerUdpUnion.push_back(dstPortrangeLowerUdpUnion);
		m_dstPortrangeHigherTcpUnion.push_back(dstPortrangeHigherTcpUnion);
		m_dstPortrangeHigherUdpUnion.push_back(dstPortrangeHigherUdpUnion);
		m_dstPortOperator.push_back(dstPortOperator);
		m_dstPortCustomNumberTcp.push_back(dstPortCustomNumberTcp);
		m_dstPortltCustomNumberTcp.push_back(dstPortltCustomNumberTcp);
		m_dstPortgtCustomNumberTcp.push_back(dstPortgtCustomNumberTcp);
		m_dstPortWellknownNumberTcp.push_back(dstPortWellknownNumberTcp);
		m_dstPortltWellknownNumberTcp.push_back(dstPortltWellknownNumberTcp);
		m_dstPortgtWellknownNumberTcp.push_back(dstPortgtWellknownNumberTcp);
		m_dstPortCustomNumberUdp.push_back(dstPortCustomNumberUdp);
		m_dstPortltCustomNumberUdp.push_back(dstPortltCustomNumberUdp);
		m_dstPortgtCustomNumberUdp.push_back(dstPortgtCustomNumberUdp);
		m_dstPortWellknownNumberUdp.push_back(dstPortWellknownNumberUdp);
		m_dstPortltWellknownNumberUdp.push_back(dstPortltWellknownNumberUdp);
		m_dstPortgtWellknownNumberUdp.push_back(dstPortgtWellknownNumberUdp);
		m_dstPortCustomrangeLowerTcp.push_back(dstPortCustomrangeLowerTcp);
		m_dstPortWellknownrangeLowerTcp.push_back(dstPortWellknownrangeLowerTcp);
		m_dstPortCustomrangeHigherTcp.push_back(dstPortCustomrangeHigherTcp);
		m_dstPortWellknownrangeHigherTcp.push_back(dstPortWellknownrangeHigherTcp);
		m_dstPortCustomrangeLowerUdp.push_back(dstPortCustomrangeLowerUdp);
		m_dstPortWellknownrangeLowerUdp.push_back(dstPortWellknownrangeLowerUdp);
		m_dstPortCustomrangeHigherUdp.push_back(dstPortCustomrangeHigherUdp);
		m_dstPortWellknownrangeHigherUdp.push_back(dstPortWellknownrangeHigherUdp);
		m_customPrecedence.push_back(customPrecedence);
		m_wellknownPrecedence.push_back(wellknownPrecedence);
		m_customTos.push_back(customTos);
		m_wellknownTos.push_back(wellknownTos);
		m_customDscp.push_back(customDscp);
		m_wellknownDscp.push_back(wellknownDscp);
		m_precedenceUnion.push_back(precedenceUnion);
		m_tosUnion.push_back(tosUnion);
		m_dscpUnion.push_back(dscpUnion);
		m_urg.push_back(urg);
		m_ack.push_back(ack);
		m_push.push_back(push);
		m_fin.push_back(fin);
		m_rst.push_back(rst);
		m_sync.push_back(sync);
	}


    UI64  SsmIpAclPolicyBulkMessage::getNumRules() const
    {
        return (m_numRules);
    }

    vector<UI64>  SsmIpAclPolicyBulkMessage::getRuleidVector() const
    {
        return (m_ruleid);
    }


    vector<UI32>  SsmIpAclPolicyBulkMessage::getActionVector() const
    {
        return (m_action);
    }


    vector<bool>  SsmIpAclPolicyBulkMessage::getIsCountVector() const
    {
        return (m_isCount);
    }


    vector<bool>  SsmIpAclPolicyBulkMessage::getIsLogVector() const
    {
        return (m_isLog);
    }
	
	const void *SsmIpAclPolicyBulkMessage::getCStructureForInputs ()
	{
        ssmAcl_dcmd_l3_rule_bulk_t *pInput			= NULL;
        string 						pname			= "";
       	UI64 num									= m_numRules;
        UI64 i;
        
		
        if (num > 0) {
        	pInput = (ssmAcl_dcmd_l3_rule_bulk_t *)calloc(1, 
				(sizeof (ssmAcl_dcmd_l3_rule_bulk_t) + (num - 1) * sizeof(ssmAcl_dcmd_l3_rule_t)));
			
			if (pInput == NULL) {
				return (pInput);
			}
			
        	pInput->numrules 						= num;
        	pname 									= m_policyName;
			
        	strncpy (pInput->acl_name, pname.c_str (), MAX_NAME_STR);
        	pInput->type 							= (ssmAcl_type) m_policyType;
			/* Fix Me Set Below And Above one As per ssmAcl_dcmd_public.h */	
			pInput->opcode 							= (ssmAcl_dcmd_opcode_t) SSMACL_DCMD_CREATE_L3_IPV4_ACL;
        	for (i = 0; i < num; i++) {
				
				string srcIpAddress                         = "";
				string srcIpAddressmask                     = "";
				string srcHostIpAddress                     = "";
				struct in_addr src_addr;
				struct in_addr src_addr_mask;
				string dstIpAddress                         = "";
				string dstIpAddressmask                     = "";
				string dstHostIpAddress                     = "";
				struct in_addr dst_addr;
				struct in_addr dst_addr_mask;
				
				(pInput->rules[i]).opcode  = SSMACL_DCMD_ADD_RULE;
				strncpy (pInput->rules[i].acl_name, pname.c_str (), MAX_NAME_STR);
				
				if (m_userEnteredSeqid[i]) {
					pInput->rules[i].user_entered_sequence_number        = m_userEnteredSeqid[i];
					((pInput->rules[i]).rule.generic).seq_num            = m_ruleid[i];
				}

				if (m_isCount[i])
					(((pInput->rules[i]).rule.generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_COUNT_E;

				if (m_isLog[i])
					(((pInput->rules[i]).rule.generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_LOG_E;


				(((pInput->rules[i]).rule.generic).results).forwarding = (ssmAcl_forwarding_result)m_action[i];


				if ((strcmp (m_srcUnion[i].c_str (), "srcType") == 0) && (m_srcType[i] == IP_ADDR_TYPE_HOST)) {
					(pInput->rules[i]).rule.src_ip.valid     = SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
					srcHostIpAddress                = m_srcHostIpAddress[i].toString ();
					inet_aton(srcHostIpAddress.c_str (), &(src_addr));
					((pInput->rules[i]).rule.src_ip).addr    = src_addr.s_addr;
					srcIpAddressmask                = "0.0.0.0";
					inet_aton(srcIpAddressmask.c_str (), &(src_addr_mask));
					((pInput->rules[i]).rule.src_ip).mask    = src_addr_mask.s_addr;
				} else if ((strcmp (m_srcUnion[i].c_str (), "srcIpAddress") == 0)) {
					(pInput->rules[i]).rule.src_ip.valid     = SSMACL_KEY_ELEMENT_VALID;
					srcIpAddress                    = m_srcIpAddress[i].toString ();
					inet_aton(srcIpAddress.c_str (), &(src_addr));
					(((pInput->rules[i]).rule).src_ip).addr    = src_addr.s_addr;
					srcIpAddressmask = m_srcIpAddressMask[i].toString ();
					inet_aton(srcIpAddressmask.c_str (), &(src_addr_mask));
					(((pInput->rules[i]).rule).src_ip).mask    = src_addr_mask.s_addr;
				} else if ((strcmp (m_srcUnion[i].c_str (), "srcType") == 0) && (m_srcType[i] == IP_ADDR_TYPE_ANY)) {
					((pInput->rules[i]).rule).src_ip.valid     = SSMACL_KEY_ELEMENT_ANY;
				}

				if (m_policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
					if (strcmp (m_ProtocolTypeUnion[i].c_str (), "l3l4CustomProtoType") == 0) {
						((pInput->rules[i]).rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_VALID;
						((pInput->rules[i]).rule).ip_protocol.value = m_l3l4CustomProtoType[i];
						((pInput->rules[i]).rule).ip_protocol.mask  = 0xFF;
					} else if (strcmp (m_ProtocolTypeUnion[i].c_str (), "l3l4WellknownProtoType") == 0) {
						if (m_l3l4WellknownProtoType[i] != L3L4_PROTOTYPE_IP) {
							((pInput->rules[i]).rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_VALID;
							((pInput->rules[i]).rule).ip_protocol.value = m_l3l4WellknownProtoType[i];
							((pInput->rules[i]).rule).ip_protocol.mask  = 0xFF;
						} else {
							((pInput->rules[i]).rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_ANY;
						}
					}   else
						((pInput->rules[i]).rule).ip_protocol.valid = 0;


					if ((strcmp (m_dstUnion[i].c_str (), "dstType") == 0) && (m_dstType[i] == IP_ADDR_TYPE_HOST)) {
						((pInput->rules[i]).rule).dst_ip.valid     = SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
						dstHostIpAddress                = m_dstHostIpAddress[i].toString ();
						inet_aton(dstHostIpAddress.c_str (), &(dst_addr));
						(((pInput->rules[i]).rule).dst_ip).addr    = dst_addr.s_addr;
						dstIpAddressmask                = "0.0.0.0";
						inet_aton(dstIpAddressmask.c_str (), &(dst_addr_mask));
						(((pInput->rules[i]).rule).dst_ip).mask    = dst_addr_mask.s_addr;
					} else if ((strcmp (m_dstUnion[i].c_str (), "dstIpAddress") == 0)) {
						((pInput->rules[i]).rule).dst_ip.valid     = SSMACL_KEY_ELEMENT_VALID;
						dstIpAddress                    = m_dstIpAddress[i].toString ();
						inet_aton(dstIpAddress.c_str (), &(dst_addr));
						(((pInput->rules[i]).rule).dst_ip).addr    = dst_addr.s_addr;
						dstIpAddressmask = m_dstIpAddressMask[i].toString ();
						inet_aton(dstIpAddressmask.c_str (), &(dst_addr_mask));
						(((pInput->rules[i]).rule).dst_ip).mask    = dst_addr_mask.s_addr;
					} else if ((strcmp (m_dstUnion[i].c_str (), "dstType") == 0) && (m_dstType[i] == IP_ADDR_TYPE_ANY)) {
						((pInput->rules[i]).rule).dst_ip.valid     = SSMACL_KEY_ELEMENT_ANY;
					}

					if (m_l3l4WellknownProtoType[i] == L3L4_PROTOTYPE_TCP) {

						if (m_srcPortOperator[i]) {
							(((pInput->rules[i]).rule).src_protocol_port).valid    = SSMACL_KEY_ELEMENT_VALID;
							(((pInput->rules[i]).rule).src_protocol_port).oper     
								= (ssmAcl_port_range_operators) m_srcPortOperator[i];
							
							if (m_srcPortOperator[i] == PORTOPERATOR_RANGE) {
								if (strcmp (m_srcPortrangeLowerTcpUnion[i].c_str (), "srcPortCustomrangeLowerTcp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortCustomrangeLowerTcp[i];
								
								else if (strcmp (m_srcPortrangeLowerTcpUnion[i].c_str (), "srcPortWellknownrangeLowerTcp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortWellknownrangeLowerTcp[i];
								
								if (strcmp (m_srcPortrangeHigherTcpUnion[i].c_str (), "srcPortCustomrangeHigherTcp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).ending_port_number 
										= m_srcPortCustomrangeHigherTcp[i];
								
								else if (strcmp (m_srcPortrangeHigherTcpUnion[i].c_str (), "srcPortWellknownrangeHigherTcp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).ending_port_number 
										= m_srcPortWellknownrangeHigherTcp[i];
							
							} else {
								if (strcmp (m_srcPortNumberTcpUnion[i].c_str (), "srcPortWellknownNumberTcp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortWellknownNumberTcp[i];
								
								else if ((strcmp (m_srcPortNumberTcpUnion[i].c_str (), "srcPortCustomNumberTcp") == 0)
									&& (m_srcPortCustomNumberTcp[i] != DEFAULT_PORTNUM ))
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortCustomNumberTcp[i];

                                else if (strcmp (m_srcPortltNumberTcpUnion[i].c_str (), "srcPortltWellknownNumberTcp") == 0)
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortltWellknownNumberTcp[i];

                                else if ((strcmp (m_srcPortltNumberTcpUnion[i].c_str (), "srcPortltCustomNumberTcp") == 0)
									&& (m_srcPortltCustomNumberTcp[i]!= DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortltCustomNumberTcp[i];

                                else if (strcmp (m_srcPortgtNumberTcpUnion[i].c_str (), "srcPortgtWellknownNumberTcp") == 0)
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortgtWellknownNumberTcp[i];

                                else if ((strcmp (m_srcPortgtNumberTcpUnion[i].c_str (), "srcPortgtCustomNumberTcp") == 0)
									&& (m_srcPortgtCustomNumberTcp[i]!= DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortgtCustomNumberTcp[i];

							}
						}


						if (m_dstPortOperator[i]) {
							(((pInput->rules[i]).rule).dst_protocol_port).valid    = SSMACL_KEY_ELEMENT_VALID;
							(((pInput->rules[i]).rule).dst_protocol_port).oper     = (ssmAcl_port_range_operators) m_dstPortOperator[i];
							if (m_dstPortOperator[i] == PORTOPERATOR_RANGE) {
								if (strcmp (m_dstPortrangeLowerTcpUnion[i].c_str (), "dstPortCustomrangeLowerTcp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortCustomrangeLowerTcp[i];
								
								else if (strcmp (m_dstPortrangeLowerTcpUnion[i].c_str (), "dstPortWellknownrangeLowerTcp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortWellknownrangeLowerTcp[i];
								
								if (strcmp (m_dstPortrangeHigherTcpUnion[i].c_str (), "dstPortCustomrangeHigherTcp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).ending_port_number 
										= m_dstPortCustomrangeHigherTcp[i];
								
								else if (strcmp (m_dstPortrangeHigherTcpUnion[i].c_str (), "dstPortWellknownrangeHigherTcp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).ending_port_number 
										= m_dstPortWellknownrangeHigherTcp[i];
							
							} else {
								if (strcmp (m_dstPortNumberTcpUnion[i].c_str (), "dstPortWellknownNumberTcp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortWellknownNumberTcp[i];
								
								else if ((strcmp (m_dstPortNumberTcpUnion[i].c_str (), "dstPortCustomNumberTcp") == 0)
									&& (m_dstPortCustomNumberTcp[i] != DEFAULT_PORTNUM ))
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortCustomNumberTcp[i];

                                else if (strcmp (m_dstPortltNumberTcpUnion[i].c_str (), "dstPortltWellknownNumberTcp") == 0)
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortltWellknownNumberTcp[i];

                                else if ((strcmp (m_dstPortltNumberTcpUnion[i].c_str (), "dstPortltCustomNumberTcp") == 0)
									&& (m_dstPortltCustomNumberTcp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortltCustomNumberTcp[i];

                                else if (strcmp (m_dstPortgtNumberTcpUnion[i].c_str (), "dstPortgtWellknownNumberTcp") == 0)
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortgtWellknownNumberTcp[i];

                                else if ((strcmp (m_dstPortgtNumberTcpUnion[i].c_str (), "dstPortgtCustomNumberTcp") == 0)
									&& (m_dstPortgtCustomNumberTcp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortgtCustomNumberTcp[i];

							}
						}
					}

					if (m_l3l4WellknownProtoType[i] == L3L4_PROTOTYPE_UDP) {
						if (m_srcPortOperator[i]) {
							(((pInput->rules[i]).rule).src_protocol_port).valid    = SSMACL_KEY_ELEMENT_VALID;
							(((pInput->rules[i]).rule).src_protocol_port).oper     
								= (ssmAcl_port_range_operators) m_srcPortOperator[i];
							
							if (m_srcPortOperator[i] == PORTOPERATOR_RANGE) {
								if (strcmp (m_srcPortrangeLowerUdpUnion[i].c_str (), "srcPortCustomrangeLowerUdp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortCustomrangeLowerUdp[i];
								
								else if (strcmp (m_srcPortrangeLowerUdpUnion[i].c_str (), "srcPortWellknownrangeLowerUdp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortWellknownrangeLowerUdp[i];
								
								if (strcmp (m_srcPortrangeHigherUdpUnion[i].c_str (), "srcPortCustomrangeHigherUdp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).ending_port_number 
										= m_srcPortCustomrangeHigherUdp[i];
								
								else if (strcmp (m_srcPortrangeHigherUdpUnion[i].c_str (), "srcPortWellknownrangeHigherUdp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).ending_port_number 
										= m_srcPortWellknownrangeHigherUdp[i];
							
							} else {
								if (strcmp (m_srcPortNumberUdpUnion[i].c_str (), "srcPortWellknownNumberUdp") == 0)
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortWellknownNumberUdp[i];
								
								else if ((strcmp (m_srcPortNumberUdpUnion[i].c_str (), "srcPortCustomNumberUdp") == 0)
									&& (m_srcPortCustomNumberUdp[i] != DEFAULT_PORTNUM ))
									(((pInput->rules[i]).rule).src_protocol_port).starting_port_number 
										= m_srcPortCustomNumberUdp[i];

                                else if (strcmp (m_srcPortltNumberUdpUnion[i].c_str (), "srcPortltWellknownNumberUdp") == 0)
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortltWellknownNumberUdp[i];

                                else if ((strcmp (m_srcPortltNumberUdpUnion[i].c_str (), "srcPortltCustomNumberUdp") == 0)
									&& (m_srcPortltCustomNumberUdp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortltCustomNumberUdp[i];

                                else if (strcmp (m_srcPortgtNumberUdpUnion[i].c_str (), "srcPortgtWellknownNumberUdp") == 0)
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortgtWellknownNumberUdp[i];

                                else if ((strcmp (m_srcPortgtNumberUdpUnion[i].c_str (), "srcPortgtCustomNumberUdp") == 0)
									&& ( m_srcPortgtCustomNumberUdp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).src_protocol_port).starting_port_number
                                        = m_srcPortgtCustomNumberUdp[i];
							}
						}


						if (m_dstPortOperator[i]) {
							(((pInput->rules[i]).rule).dst_protocol_port).valid    = SSMACL_KEY_ELEMENT_VALID;
							(((pInput->rules[i]).rule).dst_protocol_port).oper     
								= (ssmAcl_port_range_operators) m_dstPortOperator[i];
							
							if (m_dstPortOperator[i] == PORTOPERATOR_RANGE) {
								if (strcmp (m_dstPortrangeLowerUdpUnion[i].c_str (), "dstPortCustomrangeLowerUdp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortCustomrangeLowerUdp[i];
								else if (strcmp (m_dstPortrangeLowerUdpUnion[i].c_str (), "dstPortWellknownrangeLowerUdp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortWellknownrangeLowerUdp[i];
								
								if (strcmp (m_dstPortrangeHigherUdpUnion[i].c_str (), "dstPortCustomrangeHigherUdp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).ending_port_number 
										= m_dstPortCustomrangeHigherUdp[i];
								
								else if (strcmp (m_dstPortrangeHigherUdpUnion[i].c_str (), "dstPortWellknownrangeHigherUdp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).ending_port_number 
										= m_dstPortWellknownrangeHigherUdp[i];
							
							} else {
								if (strcmp (m_dstPortNumberUdpUnion[i].c_str (), "dstPortWellknownNumberUdp") == 0)
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortWellknownNumberUdp[i];
								
								else if ((strcmp (m_dstPortNumberUdpUnion[i].c_str (), "dstPortCustomNumberUdp") == 0)
									&& (m_dstPortCustomNumberUdp[i] != DEFAULT_PORTNUM )) 
									(((pInput->rules[i]).rule).dst_protocol_port).starting_port_number 
										= m_dstPortCustomNumberUdp[i];

                                else if (strcmp (m_dstPortltNumberUdpUnion[i].c_str (), "dstPortltWellknownNumberUdp") == 0)
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortltWellknownNumberUdp[i];

                                else if ((strcmp (m_dstPortltNumberUdpUnion[i].c_str (), "dstPortltCustomNumberUdp") == 0)
									&& (m_dstPortltCustomNumberUdp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortltCustomNumberUdp[i];

                                else if (strcmp (m_dstPortgtNumberUdpUnion[i].c_str (), "dstPortgtWellknownNumberUdp") == 0)
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortgtWellknownNumberUdp[i];

                                else if ((strcmp (m_dstPortgtNumberUdpUnion[i].c_str (), "dstPortgtCustomNumberUdp") == 0)
									&& (m_dstPortgtCustomNumberUdp[i] != DEFAULT_PORTNUM ))
                                    (((pInput->rules[i]).rule).dst_protocol_port).starting_port_number
                                        = m_dstPortgtCustomNumberUdp[i];
							}
						}

					}

					if ((strcmp (m_precedenceUnion[i].c_str(), "customPrecedence") == 0) 
						&& (m_customPrecedence[i] != DEFAULT_PREC_DSCP_TOS_VAL )){
						(((pInput->rules[i]).rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_PRECEDENCE_VALID) ;
						(((pInput->rules[i]).rule).tos).value = (m_customPrecedence[i] << 3);
						(((pInput->rules[i]).rule).tos).mask  = 0xFF;
					} else if ((strcmp (m_dscpUnion[i].c_str(), "customDscp") == 0)
						&& (m_customDscp[i] != DEFAULT_PREC_DSCP_TOS_VAL )){
						(((pInput->rules[i]).rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_DSCP_VALID);
						(((pInput->rules[i]).rule).tos).value = m_customDscp[i];
						(((pInput->rules[i]).rule).tos).mask  = 0xFF;
					} else if ((strcmp (m_tosUnion[i].c_str(), "customTos") == 0) 
						&& (m_customTos[i] != DEFAULT_PREC_DSCP_TOS_VAL )){
						(((pInput->rules[i]).rule).tos).valid  = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_TOS_VALID);
						(((pInput->rules[i]).rule).tos).value  = m_customTos[i];
						(((pInput->rules[i]).rule).tos).mask   = 0xFF;
					} else if (strcmp (m_precedenceUnion[i].c_str(), "wellknownPrecedence") == 0) {
						(((pInput->rules[i]).rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_PRECEDENCE_VALID) ;
						(((pInput->rules[i]).rule).tos).value = (m_wellknownPrecedence[i] << 3);
						(((pInput->rules[i]).rule).tos).mask  = 0xFF;
					} else if (strcmp (m_dscpUnion[i].c_str(), "wellknownDscp") == 0) {
						(((pInput->rules[i]).rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_DSCP_VALID);
						(((pInput->rules[i]).rule).tos).value = m_wellknownDscp[i];
						(((pInput->rules[i]).rule).tos).mask  = 0xFF;
					} else if (strcmp (m_tosUnion[i].c_str(), "wellknownTos") == 0) {
						(((pInput->rules[i]).rule).tos).valid  = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_TOS_VALID);
						(((pInput->rules[i]).rule).tos).value  = m_wellknownTos[i];
						(((pInput->rules[i]).rule).tos).mask   = 0xFF;
					}

					if ((m_urg[i]) || (m_ack[i]) ||(m_push[i]) ||(m_fin[i]) || (m_rst[i]) || (m_sync[i])) {
						(((pInput->rules[i]).rule).tcp_flags).valid = SSMACL_KEY_ELEMENT_VALID;
						
						if (m_urg[i])
							(((pInput->rules[i]).rule).tcp_flags).urg      = SSMACL_FLAG_STATE_SET_E;
						if (m_ack[i])
							(((pInput->rules[i]).rule).tcp_flags).ack      = SSMACL_FLAG_STATE_SET_E;
						if (m_push[i])
							(((pInput->rules[i]).rule).tcp_flags).push     = SSMACL_FLAG_STATE_SET_E;
						if (m_fin[i])
							(((pInput->rules[i]).rule).tcp_flags).fin      = SSMACL_FLAG_STATE_SET_E;
						if (m_rst[i])
							(((pInput->rules[i]).rule).tcp_flags).rst      = SSMACL_FLAG_STATE_SET_E;
						if (m_sync[i])
							(((pInput->rules[i]).rule).tcp_flags).sync     = SSMACL_FLAG_STATE_SET_E;
					}
				}
			}
		} else  {
        	pInput = (ssmAcl_dcmd_l3_rule_bulk_t *)calloc(1, (sizeof (ssmAcl_dcmd_l3_rule_bulk_t)));
			pInput->numrules                        = num;
            pname                                   = m_policyName;

            strncpy (pInput->acl_name, pname.c_str (), MAX_NAME_STR);
            pInput->type                            = (ssmAcl_type) m_policyType;
            /* Fix Me Set Below And Above one As per ssmAcl_dcmd_public.h */
            pInput->opcode                          = (ssmAcl_dcmd_opcode_t) SSMACL_DCMD_CREATE_L3_IPV4_ACL;

		}
        
        return (pInput);
	}

	void SsmIpAclPolicyBulkMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
