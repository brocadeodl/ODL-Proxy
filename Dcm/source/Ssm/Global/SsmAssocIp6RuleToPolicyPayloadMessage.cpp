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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : bpatel                                                     *
 **************************************************************************/

#include "Ssm/Global/SsmAssocIp6RuleToPolicyPayloadMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "vcs.h"

namespace DcmNs
{

	SsmAssocIp6RuleToPolicyPayloadMessage::SsmAssocIp6RuleToPolicyPayloadMessage (PrismMessage *pPrismMsg)
    			: WaveMessagePayloadBase(pPrismMsg)
	{
    }

	SsmAssocIp6RuleToPolicyPayloadMessage::SsmAssocIp6RuleToPolicyPayloadMessage (PrismMessage *pPrismMsg, 
        const string &policyName, const UI32 &policyType, const UI64 &ruleid, 
		const UI32 &action, const string &srcUnion, const IpAddrType &srcType, 
		const IpV6Address &srcHostIpAddress, const IpV6AddressNetworkMask &srcIpAddress,
        const IpV6Address &srcIpAddressMask, const bool &isCount, 
		const bool &isLog, const IpV6Address &dstHostIpAddress, 
		const IpV6AddressNetworkMask &dstIpAddress, const IpV6Address &dstIpAddressMask, 
		const string &dstUnion, const IpAddrType &dstType, 
		const string &ProtocolTypeUnion, const UI32 &l3l4CustomProtoType, 
		const L3L4wellknownProtoType &l3l4WellknownProtoType, const PortOperator
        &srcPortOperator, const string &srcPortNumberTcpUnion, const string &srcPortltNumberTcpUnion,
        const string &srcPortgtNumberTcpUnion, const string &srcPortNumberUdpUnion,
        const string &srcPortltNumberUdpUnion, const string &srcPortgtNumberUdpUnion,
        const string &srcPortrangeLowerTcpUnion, const string &srcPortrangeLowerUdpUnion,
        const string  &srcPortrangeHigherTcpUnion, const string &srcPortrangeHigherUdpUnion,
        const UI32 &srcPortCustomNumberTcp, const UI32 &srcPortltCustomNumberTcp,
        const UI32 &srcPortgtCustomNumberTcp, 
		const PortWellknownNumberTcp &srcPortWellknownNumberTcp,
		const PortWellknownNumberTcp &srcPortltWellknownNumberTcp,
		const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp,
		const UI32 &srcPortCustomNumberUdp, const UI32 &srcPortltCustomNumberUdp,
        const UI32 &srcPortgtCustomNumberUdp, 
		const PortWellknownNumberUdp &srcPortWellknownNumberUdp,
        const PortWellknownNumberUdp &srcPortltWellknownNumberUdp,
        const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp,
        const UI32 &srcPortCustomrangeLowerTcp,
        const PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp,
        const UI32 &srcPortCustomrangeHigherTcp,
        const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp,
        const UI32 &srcPortCustomrangeLowerUdp,
        const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp,
        const UI32 &srcPortCustomrangeHigherUdp,
        const PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp,
        const string &dstPortNumberTcpUnion, const string &dstPortltNumberTcpUnion,
        const string &dstPortgtNumberTcpUnion, const string &dstPortNumberUdpUnion,
        const string &dstPortltNumberUdpUnion, const string &dstPortgtNumberUdpUnion,
        const string &dstPortrangeLowerTcpUnion, const string &dstPortrangeLowerUdpUnion,
        const string &dstPortrangeHigherTcpUnion, const string &dstPortrangeHigherUdpUnion,
        const PortOperator &dstPortOperator, const UI32 &dstPortCustomNumberTcp,
        const UI32 &dstPortltCustomNumberTcp, const UI32 &dstPortgtCustomNumberTcp,
        const PortWellknownNumberTcp &dstPortWellknownNumberTcp,
        const PortWellknownNumberTcp &dstPortltWellknownNumberTcp,
        const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp,
        const UI32 &dstPortCustomNumberUdp, const UI32 &dstPortltCustomNumberUdp,
        const UI32 &dstPortgtCustomNumberUdp,
        const PortWellknownNumberUdp &dstPortWellknownNumberUdp,
        const PortWellknownNumberUdp &dstPortltWellknownNumberUdp,
        const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp,
        const UI32 &dstPortCustomrangeLowerTcp,
        const PortWellknownNumberTcp &dstPortWellknownrangeLowerTcp,
        const UI32 &dstPortCustomrangeHigherTcp,
        const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp,
        const UI32 &dstPortCustomrangeLowerUdp,
        const PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp,
        const UI32 &dstPortCustomrangeHigherUdp,
        const PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp,
                const WellknownPrecedence &wellknownPrecedence,
                const WellknownTos &wellknownTos, const WellknownDscp &wellknownDscp, 
				const UI32 &customPrecedence,
                const UI32 &customTos, const UI32 &customDscp, const string &precedenceUnion, 
				const string &tosUnion,
                const string &dscpUnion, const bool &urg, const bool &ack, const bool &push, const bool &fin,
                const bool &rst, const bool &sync, const UI32 &opcode, const UI8 &userEnteredSeqid)
        :WaveMessagePayloadBase(pPrismMsg), 
        m_policyName    (policyName),
        m_policyType    (policyType),
        m_ruleid    (ruleid),
        m_action    (action),
        m_srcUnion    (srcUnion),
        m_srcType    (srcType),
        m_srcHostIpAddress    (srcHostIpAddress),
        m_srcIpAddress    (srcIpAddress),
        m_srcIpAddressMask    (srcIpAddressMask),
        m_isCount    (isCount),
        m_isLog    (isLog),
        m_dstHostIpAddress    (dstHostIpAddress),
        m_dstIpAddress    (dstIpAddress),
        m_dstIpAddressMask    (dstIpAddressMask),
        m_dstUnion    (dstUnion),
        m_dstType    (dstType),
        m_ProtocolTypeUnion    (ProtocolTypeUnion),
        m_l3l4CustomProtoType    (l3l4CustomProtoType),
        m_l3l4WellknownProtoType    (l3l4WellknownProtoType),
        m_srcPortOperator    (srcPortOperator),
        m_srcPortNumberTcpUnion    (srcPortNumberTcpUnion),
        m_srcPortltNumberTcpUnion    (srcPortltNumberTcpUnion),
        m_srcPortgtNumberTcpUnion    (srcPortgtNumberTcpUnion),
		m_srcPortNumberUdpUnion    (srcPortNumberUdpUnion),
		m_srcPortltNumberUdpUnion    (srcPortltNumberUdpUnion),
		m_srcPortgtNumberUdpUnion    (srcPortgtNumberUdpUnion),
        m_srcPortrangeLowerTcpUnion    (srcPortrangeLowerTcpUnion),
        m_srcPortrangeLowerUdpUnion    (srcPortrangeLowerUdpUnion),
        m_srcPortrangeHigherTcpUnion    (srcPortrangeHigherTcpUnion),
        m_srcPortrangeHigherUdpUnion    (srcPortrangeHigherUdpUnion),
        m_srcPortCustomNumberTcp    (srcPortCustomNumberTcp),
        m_srcPortltCustomNumberTcp    (srcPortltCustomNumberTcp),
        m_srcPortgtCustomNumberTcp    (srcPortgtCustomNumberTcp),
        m_srcPortWellknownNumberTcp    (srcPortWellknownNumberTcp),
        m_srcPortltWellknownNumberTcp    (srcPortltWellknownNumberTcp),
        m_srcPortgtWellknownNumberTcp    (srcPortgtWellknownNumberTcp),
        m_srcPortCustomNumberUdp    (srcPortCustomNumberUdp),
        m_srcPortltCustomNumberUdp    (srcPortltCustomNumberUdp),
        m_srcPortgtCustomNumberUdp    (srcPortgtCustomNumberUdp),
        m_srcPortWellknownNumberUdp    (srcPortWellknownNumberUdp),
        m_srcPortltWellknownNumberUdp    (srcPortltWellknownNumberUdp),
        m_srcPortgtWellknownNumberUdp    (srcPortgtWellknownNumberUdp),
        m_srcPortCustomrangeLowerTcp    (srcPortCustomrangeLowerTcp),
        m_srcPortWellknownrangeLowerTcp    (srcPortWellknownrangeLowerTcp),
        m_srcPortCustomrangeHigherTcp    (srcPortCustomrangeHigherTcp),
        m_srcPortWellknownrangeHigherTcp    (srcPortWellknownrangeHigherTcp),
        m_srcPortCustomrangeLowerUdp    (srcPortCustomrangeLowerUdp),
        m_srcPortWellknownrangeLowerUdp    (srcPortWellknownrangeLowerUdp),
        m_srcPortCustomrangeHigherUdp    (srcPortCustomrangeHigherUdp),
        m_srcPortWellknownrangeHigherUdp    (srcPortWellknownrangeHigherUdp),
        m_dstPortNumberTcpUnion    (dstPortNumberTcpUnion),
        m_dstPortltNumberTcpUnion    (dstPortltNumberTcpUnion),
        m_dstPortgtNumberTcpUnion    (dstPortgtNumberTcpUnion),
        m_dstPortNumberUdpUnion    (dstPortNumberUdpUnion),
        m_dstPortltNumberUdpUnion    (dstPortltNumberUdpUnion),
        m_dstPortgtNumberUdpUnion    (dstPortgtNumberUdpUnion),
        m_dstPortrangeLowerTcpUnion    (dstPortrangeLowerTcpUnion),
        m_dstPortrangeLowerUdpUnion    (dstPortrangeLowerUdpUnion),
        m_dstPortrangeHigherTcpUnion    (dstPortrangeHigherTcpUnion),
        m_dstPortrangeHigherUdpUnion    (dstPortrangeHigherUdpUnion),
        m_dstPortOperator    (dstPortOperator),
        m_dstPortCustomNumberTcp    (dstPortCustomNumberTcp),
        m_dstPortltCustomNumberTcp    (dstPortltCustomNumberTcp),
        m_dstPortgtCustomNumberTcp    (dstPortgtCustomNumberTcp),
        m_dstPortWellknownNumberTcp    (dstPortWellknownNumberTcp),
        m_dstPortltWellknownNumberTcp    (dstPortltWellknownNumberTcp),
        m_dstPortgtWellknownNumberTcp    (dstPortgtWellknownNumberTcp),
        m_dstPortCustomNumberUdp    (dstPortCustomNumberUdp),
        m_dstPortltCustomNumberUdp    (dstPortltCustomNumberUdp),
        m_dstPortgtCustomNumberUdp    (dstPortgtCustomNumberUdp),
        m_dstPortWellknownNumberUdp    (dstPortWellknownNumberUdp),
        m_dstPortltWellknownNumberUdp    (dstPortltWellknownNumberUdp),
        m_dstPortgtWellknownNumberUdp    (dstPortgtWellknownNumberUdp),
        m_dstPortCustomrangeLowerTcp    (dstPortCustomrangeLowerTcp),
        m_dstPortWellknownrangeLowerTcp    (dstPortWellknownrangeLowerTcp),
        m_dstPortCustomrangeHigherTcp    (dstPortCustomrangeHigherTcp),
        m_dstPortWellknownrangeHigherTcp    (dstPortWellknownrangeHigherTcp),
        m_dstPortCustomrangeLowerUdp    (dstPortCustomrangeLowerUdp),
        m_dstPortWellknownrangeLowerUdp    (dstPortWellknownrangeLowerUdp),
        m_dstPortCustomrangeHigherUdp    (dstPortCustomrangeHigherUdp),
        m_dstPortWellknownrangeHigherUdp    (dstPortWellknownrangeHigherUdp),
		m_customPrecedence (customPrecedence),
		m_wellknownPrecedence (wellknownPrecedence),
		m_customTos (customTos),
		m_wellknownTos (wellknownTos),
		m_customDscp (customDscp),
		m_wellknownDscp (wellknownDscp),
		m_precedenceUnion (precedenceUnion),
		m_tosUnion (tosUnion),
		m_dscpUnion (dscpUnion),
        m_urg    (urg),
        m_ack    (ack),
        m_push    (push),
        m_fin    (fin),
        m_rst    (rst),
        m_sync    (sync),
        m_opcode    (opcode),
		m_userEnteredSeqid (userEnteredSeqid)
    {
    }
    SsmAssocIp6RuleToPolicyPayloadMessage::~SsmAssocIp6RuleToPolicyPayloadMessage ()
    {
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setupAttributesForSerialization()
    {
        addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType, "policyType"));
        addSerializableAttribute (new AttributeUI64(&m_ruleid, "ruleid"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
        addSerializableAttribute (new AttributeString(&m_srcUnion, "srcUnion"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcType, "srcType"));
        addSerializableAttribute (new AttributeIpV6Address(&m_srcHostIpAddress, "srcHostIpAddress"));
        addSerializableAttribute (new AttributeIpV6AddressNetworkMask(&m_srcIpAddress, "srcIpAddress"));
//        addSerializableAttribute (new AttributeIpV6Address(&m_srcIpAddressMask, "srcIpAddressMask"));
        addSerializableAttribute (new AttributeBool(&m_isCount, "isCount"));
        addSerializableAttribute (new AttributeBool(&m_isLog, "isLog"));
        addSerializableAttribute (new AttributeIpV6Address(&m_dstHostIpAddress, "dstHostIpAddress"));
        addSerializableAttribute (new AttributeIpV6AddressNetworkMask(&m_dstIpAddress, "dstIpAddress"));
        addSerializableAttribute (new AttributeIpV6Address(&m_dstIpAddressMask, "dstIpAddressMask"));
        addSerializableAttribute (new AttributeString(&m_dstUnion, "dstUnion"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstType, "dstType"));
        addSerializableAttribute (new AttributeString(&m_ProtocolTypeUnion, "ProtocolTypeUnion"));
        addSerializableAttribute (new AttributeUI32(&m_l3l4CustomProtoType, "l3l4CustomProtoType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_l3l4WellknownProtoType, "l3l4WellknownProtoType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcPortOperator, "srcPortOperator"));
        /* Src port Union eq-neq and lt and gt */
        addSerializableAttribute (new AttributeString(&m_srcPortNumberTcpUnion, "srcPortNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortltNumberTcpUnion, "srcPortltNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortgtNumberTcpUnion, "srcPortgtNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortNumberUdpUnion, "srcPortNumberUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortltNumberUdpUnion, "srcPortltNumberUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortgtNumberUdpUnion, "srcPortgtNumberUdpUnion")); 
		
        addSerializableAttribute (new AttributeString(&m_srcPortrangeLowerTcpUnion, "srcPortrangeLowerTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortrangeLowerUdpUnion, "srcPortrangeLowerUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortrangeHigherTcpUnion, "srcPortrangeHigherTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_srcPortrangeHigherUdpUnion, "srcPortrangeHigherUdpUnion"));
        
        /* source port  Custom number TCP */
        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomNumberTcp, "srcPortCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortltCustomNumberTcp, "srcPortltCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortgtCustomNumberTcp, "srcPortgtCustomNumberTcp"));

        /* source port wellknown number TCP */
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_srcPortWellknownNumberTcp), "srcPortWellknownNumberTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcPortltWellknownNumberTcp), "srcPortltWellknownNumberTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcPortgtWellknownNumberTcp), "srcPortgtWellknownNumberTcp"));

        /* source port Custom number UDP */
        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomNumberUdp, "srcPortCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortltCustomNumberUdp, "srcPortltCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortgtCustomNumberUdp, "srcPortgtCustomNumberUdp"));

        /* source port wellknown number UDP */
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcPortWellknownNumberUdp), "srcPortWellknownNumberUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcPortltWellknownNumberUdp), "srcPortltWellknownNumberUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcPortgtWellknownNumberUdp), "srcPortgtWellknownNumberUdp"));

        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomrangeLowerTcp,"srcPortCustomrangeLowerTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcPortWellknownrangeLowerTcp, 
			"srcPortWellknownrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomrangeHigherTcp,
			"srcPortCustomrangeHigherTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcPortWellknownrangeHigherTcp,
			"srcPortWellknownrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomrangeLowerUdp, "srcPortCustomrangeLowerUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcPortWellknownrangeLowerUdp,
			"srcPortWellknownrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPortCustomrangeHigherUdp, 
			"srcPortCustomrangeHigherUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcPortWellknownrangeHigherUdp, 
			"srcPortWellknownrangeHigherUdp"));
   
        /* Destination port eq-neq, lt and gt union */
        addSerializableAttribute (new AttributeString(&m_dstPortNumberTcpUnion, "dstPortNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortltNumberTcpUnion, "dstPortltNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortgtNumberTcpUnion, "dstPortgtNumberTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortNumberUdpUnion, "dstPortNumberUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortltNumberUdpUnion, "dstPortltNumberUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortgtNumberUdpUnion, "dstPortgtNumberUdpUnion"));
 
		
        addSerializableAttribute (new AttributeString(&m_dstPortrangeLowerTcpUnion, "dstPortrangeLowerTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortrangeLowerUdpUnion, "dstPortrangeLowerUdpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortrangeHigherTcpUnion, "dstPortrangeHigherTcpUnion"));
        addSerializableAttribute (new AttributeString(&m_dstPortrangeHigherUdpUnion, "dstPortrangeHigherUdpUnion"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstPortOperator, "dstPortOperator"));
       
        /* destination port  Custom number TCP */
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomNumberTcp, "dstPortCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortltCustomNumberTcp, "dstPortltCustomNumberTcp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortgtCustomNumberTcp, "dstPortgtCustomNumberTcp"));

        /* destination port wellknown number TCP */
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_dstPortWellknownNumberTcp), "dstPortWellknownNumberTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstPortltWellknownNumberTcp), "dstPortltWellknownNumberTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstPortgtWellknownNumberTcp), "dstPortgtWellknownNumberTcp"));

        /* destination port Custom number UDP */
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomNumberUdp, "dstPortCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortltCustomNumberUdp, "dstPortltCustomNumberUdp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortgtCustomNumberUdp, "dstPortgtCustomNumberUdp"));

        /* destination port wellknown number UDP */
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstPortWellknownNumberUdp), "dstPortWellknownNumberUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstPortltWellknownNumberUdp), "dstPortltWellknownNumberUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstPortgtWellknownNumberUdp), "dstPortgtWellknownNumberUdp"));
		
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomrangeLowerTcp,
			"dstPortCustomrangeLowerTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstPortWellknownrangeLowerTcp,
			"dstPortWellknownrangeLowerTcp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomrangeHigherTcp, "dstPortCustomrangeHigherTcp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstPortWellknownrangeHigherTcp,
			"dstPortWellknownrangeHigherTcp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomrangeLowerUdp, "dstPortCustomrangeLowerUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstPortWellknownrangeLowerUdp, 
			"dstPortWellknownrangeLowerUdp"));
        addSerializableAttribute (new AttributeUI32(&m_dstPortCustomrangeHigherUdp,
			"dstPortCustomrangeHigherUdp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_dstPortWellknownrangeHigherUdp,
			"dstPortWellknownrangeHigherUdp"));
        addSerializableAttribute (new AttributeUI32(&m_customPrecedence, "customPrecedence"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_wellknownPrecedence), "wellknownPrecedence"));
        addSerializableAttribute (new AttributeUI32(&m_customTos, "customTos"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_wellknownTos), "wellknownTos"));
        addSerializableAttribute (new AttributeUI32(&m_customDscp, "customDscp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_wellknownDscp), "wellknownDscp"));
		addSerializableAttribute (new AttributeString(&m_precedenceUnion, "precedenceUnion"));
		addSerializableAttribute (new AttributeString(&m_dscpUnion, "dscpUnion"));
		addSerializableAttribute (new AttributeString(&m_tosUnion, "tosUnion"));
        addSerializableAttribute (new AttributeBool(&m_urg, "urg"));
        addSerializableAttribute (new AttributeBool(&m_ack, "ack"));
        addSerializableAttribute (new AttributeBool(&m_push, "push"));
        addSerializableAttribute (new AttributeBool(&m_fin, "fin"));
        addSerializableAttribute (new AttributeBool(&m_rst, "rst"));
        addSerializableAttribute (new AttributeBool(&m_sync, "sync"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_opcode, "opcode"));
    	addSerializableAttribute (new AttributeUI8(&m_userEnteredSeqid,"userEnteredSeqid"));
	}

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  SsmAssocIp6RuleToPolicyPayloadMessage::getRuleid() const
    {
        return (m_ruleid);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }


    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getAction() const
    {
        return (m_action);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcUnion(const string &srcUnion)
    {
        m_srcUnion  =  srcUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcUnion() const
    {
        return (m_srcUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcType() const
    {
        return (m_srcType);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcHostIpAddress(const IpV6Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV6Address SsmAssocIp6RuleToPolicyPayloadMessage::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcIpAddress(const IpV6AddressNetworkMask &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV6AddressNetworkMask SsmAssocIp6RuleToPolicyPayloadMessage::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcIpAddressMask(const IpV6Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV6Address  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getIsCount() const
    {
        return (m_isCount);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getIsLog() const
    {
        return (m_isLog);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstHostIpAddress(const IpV6Address &dstHostIpAddress)
    {
        m_dstHostIpAddress  =  dstHostIpAddress;
    }

    IpV6Address SsmAssocIp6RuleToPolicyPayloadMessage::getDstHostIpAddress() const
    {
        return (m_dstHostIpAddress);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstIpAddress(const IpV6AddressNetworkMask &dstIpAddress)
    {
        m_dstIpAddress  =  dstIpAddress;
    }

    IpV6AddressNetworkMask SsmAssocIp6RuleToPolicyPayloadMessage::getDstIpAddress() const
    {
        return (m_dstIpAddress);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstIpAddressMask(const IpV6Address &dstIpAddressMask)
    {
        m_dstIpAddressMask  =  dstIpAddressMask;
    }

    IpV6Address  SsmAssocIp6RuleToPolicyPayloadMessage::getDstIpAddressMask() const
    {
        return (m_dstIpAddressMask);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstUnion(const string &dstUnion)
    {
        m_dstUnion  =  dstUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstUnion() const
    {
        return (m_dstUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstType(const IpAddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    IpAddrType  SsmAssocIp6RuleToPolicyPayloadMessage::getDstType() const
    {
        return (m_dstType);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setProtocolTypeUnion(const string &ProtocolTypeUnion)
    {
        m_ProtocolTypeUnion  =  ProtocolTypeUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getProtocolTypeUnion() const
    {
        return (m_ProtocolTypeUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType)
    {
        m_l3l4CustomProtoType  =  l3l4CustomProtoType;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getL3l4CustomProtoType() const
    {
        return (m_l3l4CustomProtoType);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setL3l4WellknownProtoType(const L3L4wellknownProtoType 
		&l3l4WellknownProtoType)
    {
        m_l3l4WellknownProtoType  =  l3l4WellknownProtoType;
    }

    L3L4wellknownProtoType  SsmAssocIp6RuleToPolicyPayloadMessage::getL3l4WellknownProtoType() const
    {
        return (m_l3l4WellknownProtoType);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortOperator(const PortOperator &srcPortOperator)
    {
        m_srcPortOperator  =  srcPortOperator;
    }

    PortOperator  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortOperator() const
    {
        return (m_srcPortOperator);
    }
	
	void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortNumberTcpUnion(const string &srcPortNumberTcpUnion)
    {
        m_srcPortNumberTcpUnion  =  srcPortNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortNumberTcpUnion() const
    {
        return (m_srcPortNumberTcpUnion);
    }


    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltNumberTcpUnion(const string &srcPortltNumberTcpUnion)
    {
        m_srcPortltNumberTcpUnion  =  srcPortltNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltNumberTcpUnion() const
    {
        return (m_srcPortltNumberTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtNumberTcpUnion(const string &srcPortgtNumberTcpUnion)
    {
        m_srcPortgtNumberTcpUnion  =  srcPortgtNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtNumberTcpUnion() const
    {
        return (m_srcPortgtNumberTcpUnion);
    } 
	
    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortNumberUdpUnion(const string 
		&srcPortNumberUdpUnion)
    {
        m_srcPortNumberUdpUnion  =  srcPortNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortNumberUdpUnion() const
    {
        return (m_srcPortNumberUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltNumberUdpUnion(const string &srcPortltNumberUdpUnion)
    {
        m_srcPortltNumberUdpUnion  =  srcPortltNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltNumberUdpUnion() const
    {
        return (m_srcPortltNumberUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtNumberUdpUnion(const string &srcPortgtNumberUdpUnion)
    {
        m_srcPortgtNumberUdpUnion  =  srcPortgtNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtNumberUdpUnion() const
    {
        return (m_srcPortgtNumberUdpUnion);
    }   
 
	void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortrangeLowerTcpUnion(const string 
		&srcPortrangeLowerTcpUnion)
    {
        m_srcPortrangeLowerTcpUnion  =  srcPortrangeLowerTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortrangeLowerTcpUnion() const
    {
        return (m_srcPortrangeLowerTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortrangeLowerUdpUnion(const string 
		&srcPortrangeLowerUdpUnion)
    {
        m_srcPortrangeLowerUdpUnion  =  srcPortrangeLowerUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortrangeLowerUdpUnion() const
    {
        return (m_srcPortrangeLowerUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortrangeHigherTcpUnion(const string 
		&srcPortrangeHigherTcpUnion)
    {
        m_srcPortrangeHigherTcpUnion  =  srcPortrangeHigherTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortrangeHigherTcpUnion() const
    {
        return (m_srcPortrangeHigherTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortrangeHigherUdpUnion(const string 
		&srcPortrangeHigherUdpUnion)
    {
        m_srcPortrangeHigherUdpUnion  =  srcPortrangeHigherUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortrangeHigherUdpUnion() const
    {
        return (m_srcPortrangeHigherUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomNumberTcp(const UI32 
		&srcPortCustomNumberTcp)
    {
        m_srcPortCustomNumberTcp  =  srcPortCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomNumberTcp() const
    {
        return (m_srcPortCustomNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp)
    {
        m_srcPortltCustomNumberTcp  =  srcPortltCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltCustomNumberTcp() const
    {
        return (m_srcPortltCustomNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp)
    {
        m_srcPortgtCustomNumberTcp  =  srcPortgtCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtCustomNumberTcp() const
    {
        return (m_srcPortgtCustomNumberTcp);
    }    

	void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownNumberTcp(const 
		PortWellknownNumberTcp &srcPortWellknownNumberTcp)
    {
        m_srcPortWellknownNumberTcp = srcPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownNumberTcp() const
    {
        return (m_srcPortWellknownNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp)
    {
        m_srcPortltWellknownNumberTcp  =  srcPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltWellknownNumberTcp() const
    {
        return (m_srcPortltWellknownNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp)
    {
        m_srcPortgtWellknownNumberTcp  =  srcPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtWellknownNumberTcp() const
    {
        return (m_srcPortgtWellknownNumberTcp);
    }

	
    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp)
    {
        m_srcPortCustomNumberUdp  =  srcPortCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomNumberUdp() const
    {
        return (m_srcPortCustomNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp)
    {
        m_srcPortltCustomNumberUdp  =  srcPortltCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltCustomNumberUdp() const
    {
        return (m_srcPortltCustomNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp)
    {
        m_srcPortgtCustomNumberUdp  =  srcPortgtCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtCustomNumberUdp() const
    {
        return (m_srcPortgtCustomNumberUdp);
    }
    
	void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownNumberUdp(const 
		PortWellknownNumberUdp &srcPortWellknownNumberUdp)
    {
        m_srcPortWellknownNumberUdp  =  srcPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownNumberUdp() const
    {
        return (m_srcPortWellknownNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp)
    {
        m_srcPortltWellknownNumberUdp  =  srcPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortltWellknownNumberUdp() const
    {
        return (m_srcPortltWellknownNumberUdp);
    }


    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp)
    {
        m_srcPortgtWellknownNumberUdp  =  srcPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortgtWellknownNumberUdp() const
    {
        return (m_srcPortgtWellknownNumberUdp);
    }

	
    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomrangeLowerTcp(const UI32 
		&srcPortCustomrangeLowerTcp)
    {
        m_srcPortCustomrangeLowerTcp  =  srcPortCustomrangeLowerTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomrangeLowerTcp() const
    {
        return (m_srcPortCustomrangeLowerTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownrangeLowerTcp(const 
		PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp)
    {
        m_srcPortWellknownrangeLowerTcp  =  srcPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownrangeLowerTcp() const
    {
        return (m_srcPortWellknownrangeLowerTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomrangeHigherTcp(const UI32 
		&srcPortCustomrangeHigherTcp)
    {
        m_srcPortCustomrangeHigherTcp  =  srcPortCustomrangeHigherTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomrangeHigherTcp() const
    {
        return (m_srcPortCustomrangeHigherTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp)
    {
        m_srcPortWellknownrangeHigherTcp  =  srcPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownrangeHigherTcp() const
    {
        return (m_srcPortWellknownrangeHigherTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomrangeLowerUdp(const UI32 
		&srcPortCustomrangeLowerUdp)
    {
        m_srcPortCustomrangeLowerUdp  =  srcPortCustomrangeLowerUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomrangeLowerUdp() const
    {
        return (m_srcPortCustomrangeLowerUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp)
    {
        m_srcPortWellknownrangeLowerUdp  =  srcPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownrangeLowerUdp() const
    {
        return (m_srcPortWellknownrangeLowerUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortCustomrangeHigherUdp(const UI32 
		&srcPortCustomrangeHigherUdp)
    {
        m_srcPortCustomrangeHigherUdp  =  srcPortCustomrangeHigherUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortCustomrangeHigherUdp() const
    {
        return (m_srcPortCustomrangeHigherUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSrcPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp)
    {
        m_srcPortWellknownrangeHigherUdp  =  srcPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getSrcPortWellknownrangeHigherUdp() const
    {
        return (m_srcPortWellknownrangeHigherUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortNumberTcpUnion(const string &dstPortNumberTcpUnion)
    {
        m_dstPortNumberTcpUnion  =  dstPortNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortNumberTcpUnion() const
    {
        return (m_dstPortNumberTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltNumberTcpUnion(const string &dstPortltNumberTcpUnion)
    {
        m_dstPortltNumberTcpUnion  =  dstPortltNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltNumberTcpUnion() const
    {
        return (m_dstPortltNumberTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtNumberTcpUnion(const string &dstPortgtNumberTcpUnion)
    {
        m_dstPortgtNumberTcpUnion  =  dstPortgtNumberTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtNumberTcpUnion() const
    {
        return (m_dstPortgtNumberTcpUnion);
    }


    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortNumberUdpUnion(const string &dstPortNumberUdpUnion)
    {
        m_dstPortNumberUdpUnion  =  dstPortNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortNumberUdpUnion() const
    {
        return (m_dstPortNumberUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltNumberUdpUnion(const string &dstPortltNumberUdpUnion)
    {
        m_dstPortltNumberUdpUnion  =  dstPortltNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltNumberUdpUnion() const
    {
        return (m_dstPortltNumberUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtNumberUdpUnion(const string &dstPortgtNumberUdpUnion)
    {
        m_dstPortgtNumberUdpUnion  =  dstPortgtNumberUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtNumberUdpUnion() const
    {
        return (m_dstPortgtNumberUdpUnion);
    }


    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortrangeLowerTcpUnion(const string &dstPortrangeLowerTcpUnion)
    {
        m_dstPortrangeLowerTcpUnion  =  dstPortrangeLowerTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortrangeLowerTcpUnion() const
    {
        return (m_dstPortrangeLowerTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortrangeLowerUdpUnion(const string &dstPortrangeLowerUdpUnion)
    {
        m_dstPortrangeLowerUdpUnion  =  dstPortrangeLowerUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortrangeLowerUdpUnion() const
    {
        return (m_dstPortrangeLowerUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortrangeHigherTcpUnion(const string &dstPortrangeHigherTcpUnion)
    {
        m_dstPortrangeHigherTcpUnion  =  dstPortrangeHigherTcpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortrangeHigherTcpUnion() const
    {
        return (m_dstPortrangeHigherTcpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortrangeHigherUdpUnion(const string &dstPortrangeHigherUdpUnion)
    {
        m_dstPortrangeHigherUdpUnion  =  dstPortrangeHigherUdpUnion;
    }

    string  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortrangeHigherUdpUnion() const
    {
        return (m_dstPortrangeHigherUdpUnion);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortOperator(const PortOperator &dstPortOperator)
    {
        m_dstPortOperator  =  dstPortOperator;
    }

    PortOperator  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortOperator() const
    {
        return (m_dstPortOperator);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp)
    {
        m_dstPortCustomNumberTcp  =  dstPortCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomNumberTcp() const
    {
        return (m_dstPortCustomNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp)
    {
        m_dstPortltCustomNumberTcp  =  dstPortltCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltCustomNumberTcp() const
    {
        return (m_dstPortltCustomNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp)
    {
        m_dstPortgtCustomNumberTcp  =  dstPortgtCustomNumberTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtCustomNumberTcp() const
    {
        return (m_dstPortgtCustomNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownNumberTcp(const PortWellknownNumberTcp 
		&dstPortWellknownNumberTcp)
    {
        m_dstPortWellknownNumberTcp  =  dstPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownNumberTcp() const
    {
        return (m_dstPortWellknownNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp)
    {
        m_dstPortltWellknownNumberTcp  =  dstPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltWellknownNumberTcp() const
    {
        return (m_dstPortltWellknownNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp)
    {
        m_dstPortgtWellknownNumberTcp  =  dstPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtWellknownNumberTcp() const
    {
        return (m_dstPortgtWellknownNumberTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp)
    {
        m_dstPortCustomNumberUdp  =  dstPortCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomNumberUdp() const
    {
        return (m_dstPortCustomNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp)
    {
        m_dstPortltCustomNumberUdp  =  dstPortltCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltCustomNumberUdp() const
    {
        return (m_dstPortltCustomNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp)
    {
        m_dstPortgtCustomNumberUdp  =  dstPortgtCustomNumberUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtCustomNumberUdp() const
    {
        return (m_dstPortgtCustomNumberUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownNumberUdp(const PortWellknownNumberUdp 
		&dstPortWellknownNumberUdp)
    {
        m_dstPortWellknownNumberUdp  =  dstPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownNumberUdp() const
    {
        return (m_dstPortWellknownNumberUdp);
    }


    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortltWellknownNumberUdp() const
    {
        return (m_dstPortltWellknownNumberUdp);
    }

	
    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortgtWellknownNumberUdp() const
    {
        return (m_dstPortgtWellknownNumberUdp);
    }
    
	void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomrangeLowerTcp(const UI32 
		&dstPortCustomrangeLowerTcp)
    {
        m_dstPortCustomrangeLowerTcp  =  dstPortCustomrangeLowerTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomrangeLowerTcp() const
    {
        return (m_dstPortCustomrangeLowerTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp 
		&dstPortWellknownrangeLowerTcp)
    {
        m_dstPortWellknownrangeLowerTcp  =  dstPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownrangeLowerTcp() const
    {
        return (m_dstPortWellknownrangeLowerTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomrangeHigherTcp(const UI32 
		&dstPortCustomrangeHigherTcp)
    {
        m_dstPortCustomrangeHigherTcp  =  dstPortCustomrangeHigherTcp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomrangeHigherTcp() const
    {
        return (m_dstPortCustomrangeHigherTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp)
    {
        m_dstPortWellknownrangeHigherTcp  =  dstPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownrangeHigherTcp() const
    {
        return (m_dstPortWellknownrangeHigherTcp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomrangeLowerUdp(const UI32 
		&dstPortCustomrangeLowerUdp)
    {
        m_dstPortCustomrangeLowerUdp  =  dstPortCustomrangeLowerUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomrangeLowerUdp() const
    {
        return (m_dstPortCustomrangeLowerUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownrangeLowerUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp)
    {
        m_dstPortWellknownrangeLowerUdp  =  dstPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownrangeLowerUdp() const
    {
        return (m_dstPortWellknownrangeLowerUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortCustomrangeHigherUdp(const UI32 
		&dstPortCustomrangeHigherUdp)
    {
        m_dstPortCustomrangeHigherUdp  =  dstPortCustomrangeHigherUdp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortCustomrangeHigherUdp() const
    {
        return (m_dstPortCustomrangeHigherUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp)
    {
        m_dstPortWellknownrangeHigherUdp  =  dstPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  SsmAssocIp6RuleToPolicyPayloadMessage::getDstPortWellknownrangeHigherUdp() const
    {
        return (m_dstPortWellknownrangeHigherUdp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setWellknownPrecedence (const WellknownPrecedence &wellknownPrecedence)
    {
        m_wellknownPrecedence = wellknownPrecedence;
    }

    WellknownPrecedence SsmAssocIp6RuleToPolicyPayloadMessage::getWellknownPrecedence() const
    {
        return (m_wellknownPrecedence);
    }

    void SsmAssocIp6RuleToPolicyPayloadMessage::setCustomPrecedence (const UI32 &customPrecedence)
    {
        m_customPrecedence = customPrecedence;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getCustomPrecedence() const
    {
        return (m_customPrecedence);
    }

	void  SsmAssocIp6RuleToPolicyPayloadMessage::setPrecedenceUnion(const string &precedenceUnion)
	{
		m_precedenceUnion = precedenceUnion;
	}

	string  SsmAssocIp6RuleToPolicyPayloadMessage::getPrecedenceUnion() const
	{
		return (m_precedenceUnion);
	}

	void  SsmAssocIp6RuleToPolicyPayloadMessage::setDscpUnion(const string &dscpUnion)
	{
		m_dscpUnion = dscpUnion;
	}

	string  SsmAssocIp6RuleToPolicyPayloadMessage::getDscpUnion() const
	{
		return (m_dscpUnion);
	}

	void  SsmAssocIp6RuleToPolicyPayloadMessage::setTosUnion(const string &tosUnion)
	{
		m_tosUnion = tosUnion;
	}
	
	string  SsmAssocIp6RuleToPolicyPayloadMessage::getTosUnion() const
	{
		return (m_tosUnion);
	}

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setWellknownTos (const WellknownTos &wellknownTos)
    {
        m_wellknownTos = wellknownTos;
    }

    WellknownTos SsmAssocIp6RuleToPolicyPayloadMessage::getWellknownTos() const
    {
        return (m_wellknownTos);
    }

    void SsmAssocIp6RuleToPolicyPayloadMessage::setCustomTos (const UI32 &customTos)
    {
        m_customTos = customTos;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getCustomTos() const
    {
        return (m_customTos);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setWellknownDscp (const WellknownDscp &wellknownDscp)
    {
        m_wellknownDscp = wellknownDscp;
    }

    WellknownDscp SsmAssocIp6RuleToPolicyPayloadMessage::getWellknownDscp() const
    {
        return (m_wellknownDscp);
    }

    void SsmAssocIp6RuleToPolicyPayloadMessage::setCustomDscp (const UI32 &customDscp)
    {
        m_customDscp = customDscp;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getCustomDscp() const
    {
        return (m_customDscp);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setUrg(const bool &urg)
    {
        m_urg  =  urg;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getUrg() const
    {
        return (m_urg);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setAck(const bool &ack)
    {
        m_ack  =  ack;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getAck() const
    {
        return (m_ack);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setPush(const bool &push)
    {
        m_push  =  push;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getPush() const
    {
        return (m_push);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setFin(const bool &fin)
    {
        m_fin  =  fin;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getFin() const
    {
        return (m_fin);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setRst(const bool &rst)
    {
        m_rst  =  rst;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getRst() const
    {
        return (m_rst);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setSync(const bool &sync)
    {
        m_sync  =  sync;
    }

    bool  SsmAssocIp6RuleToPolicyPayloadMessage::getSync() const
    {
        return (m_sync);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmAssocIp6RuleToPolicyPayloadMessage::getOpcode() const
    {
        return (m_opcode);
    }

    UI8  SsmAssocIp6RuleToPolicyPayloadMessage::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp)
    {
        m_dstPortltWellknownNumberUdp  =  dstPortltWellknownNumberUdp;
    }

    void  SsmAssocIp6RuleToPolicyPayloadMessage::setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp)
    {
        m_dstPortgtWellknownNumberUdp  =  dstPortgtWellknownNumberUdp;
    }

	
}
