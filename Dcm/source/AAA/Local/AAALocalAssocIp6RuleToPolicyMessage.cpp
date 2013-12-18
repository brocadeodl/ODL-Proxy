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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Local/AAALocalAssocIp6RuleToPolicyMessage.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    AAALocalAssocIp6RuleToPolicyMessage::AAALocalAssocIp6RuleToPolicyMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALASSOCIP6RULETOPOLICY)
    {
    }

    AAALocalAssocIp6RuleToPolicyMessage::AAALocalAssocIp6RuleToPolicyMessage (
        const string &policyName, const UI32 &policyType, const UI64 &ruleid,
        const UI32 &action, const string &srcUnion, const IpAddrType &srcType,
        const IpV6Address &srcHostIpAddress, const IpV6AddressNetworkMask &srcIpAddress,
//        const IpV6Address &srcIpAddressMask,
		 const bool &isCount,
        const bool &isLog, const IpV6Address &dstHostIpAddress,
        const IpV6AddressNetworkMask &dstIpAddress,
// const IpV6Address &dstIpAddressMask,
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
		const WellknownPrecedence &wellknownPrecedence, const WellknownTos &wellknownTos, 
		const WellknownDscp &wellknownDscp, const UI32 &customPrecedence, const UI32 &customTos,
        const UI32 &customDscp, const string &precedenceUnion, const string &tosUnion, const string &dscpUnion, 
		const bool &urg, const bool &ack, const bool &push, const bool &fin, const bool &rst, 
		const bool &sync, const UI32 &opcode, const UI8 &userEnteredSeqid)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALASSOCIP6RULETOPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType),
        m_ruleid    (ruleid),
        m_action    (action),
        m_srcUnion    (srcUnion),
        m_srcType    (srcType),
        m_srcHostIpAddress    (srcHostIpAddress),
        m_srcIpAddress    (srcIpAddress),
//        m_srcIpAddressMask    (srcIpAddressMask),
        m_isCount    (isCount),
        m_isLog    (isLog),
        m_dstHostIpAddress    (dstHostIpAddress),
        m_dstIpAddress    (dstIpAddress),
//        m_dstIpAddressMask    (dstIpAddressMask),
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

    AAALocalAssocIp6RuleToPolicyMessage::~AAALocalAssocIp6RuleToPolicyMessage ()
    {
    }
    
	void  AAALocalAssocIp6RuleToPolicyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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
//        addSerializableAttribute (new AttributeIpV6Address(&m_dstIpAddressMask, "dstIpAddressMask"));
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

    void  AAALocalAssocIp6RuleToPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  AAALocalAssocIp6RuleToPolicyMessage::getRuleid() const
    {
        return (m_ruleid);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }


    UI32  AAALocalAssocIp6RuleToPolicyMessage::getAction() const
    {
        return (m_action);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcUnion(const string &srcUnion)
    {
        m_srcUnion  =  srcUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcUnion() const
    {
        return (m_srcUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  AAALocalAssocIp6RuleToPolicyMessage::getSrcType() const
    {
        return (m_srcType);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcHostIpAddress(const IpV6Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV6Address  AAALocalAssocIp6RuleToPolicyMessage::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcIpAddress(const IpV6AddressNetworkMask &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV6AddressNetworkMask AAALocalAssocIp6RuleToPolicyMessage::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }

/*
    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcIpAddressMask(const IpV6Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV6Address  AAALocalAssocIp6RuleToPolicyMessage::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }
*/

    void  AAALocalAssocIp6RuleToPolicyMessage::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getIsCount() const
    {
        return (m_isCount);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getIsLog() const
    {
        return (m_isLog);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstHostIpAddress(const IpV6Address &dstHostIpAddress)
    {
        m_dstHostIpAddress  =  dstHostIpAddress;
    }

    IpV6Address  AAALocalAssocIp6RuleToPolicyMessage::getDstHostIpAddress() const
    {
        return (m_dstHostIpAddress);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstIpAddress(const IpV6AddressNetworkMask &dstIpAddress)
    {
        m_dstIpAddress  =  dstIpAddress;
    }

    IpV6AddressNetworkMask AAALocalAssocIp6RuleToPolicyMessage::getDstIpAddress() const
    {
        return (m_dstIpAddress);
    }
/*
    void  AAALocalAssocIp6RuleToPolicyMessage::setDstIpAddressMask(const IpV6Address &dstIpAddressMask)
    {
        m_dstIpAddressMask  =  dstIpAddressMask;
    }

    IpV6Address  AAALocalAssocIp6RuleToPolicyMessage::getDstIpAddressMask() const
    {
        return (m_dstIpAddressMask);
    }
*/
    void  AAALocalAssocIp6RuleToPolicyMessage::setDstUnion(const string &dstUnion)
    {
        m_dstUnion  =  dstUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstUnion() const
    {
        return (m_dstUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstType(const IpAddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    IpAddrType  AAALocalAssocIp6RuleToPolicyMessage::getDstType() const
    {
        return (m_dstType);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setProtocolTypeUnion(const string &ProtocolTypeUnion)
    {
        m_ProtocolTypeUnion  =  ProtocolTypeUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getProtocolTypeUnion() const
    {
        return (m_ProtocolTypeUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType)
    {
        m_l3l4CustomProtoType  =  l3l4CustomProtoType;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getL3l4CustomProtoType() const
    {
        return (m_l3l4CustomProtoType);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setL3l4WellknownProtoType(const L3L4wellknownProtoType 
		&l3l4WellknownProtoType)
    {
        m_l3l4WellknownProtoType  =  l3l4WellknownProtoType;
    }

    L3L4wellknownProtoType  AAALocalAssocIp6RuleToPolicyMessage::getL3l4WellknownProtoType() const
    {
        return (m_l3l4WellknownProtoType);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortOperator(const PortOperator &srcPortOperator)
    {
        m_srcPortOperator  =  srcPortOperator;
    }

    PortOperator  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortOperator() const
    {
        return (m_srcPortOperator);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortNumberTcpUnion(const string &srcPortNumberTcpUnion)
    {
        m_srcPortNumberTcpUnion  =  srcPortNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortNumberTcpUnion() const
    {
        return (m_srcPortNumberTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortNumberUdpUnion(const string 
		&srcPortNumberUdpUnion)
    {
        m_srcPortNumberUdpUnion  =  srcPortNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortNumberUdpUnion() const
    {
        return (m_srcPortNumberUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortrangeLowerTcpUnion(const string 
		&srcPortrangeLowerTcpUnion)
    {
        m_srcPortrangeLowerTcpUnion  =  srcPortrangeLowerTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortrangeLowerTcpUnion() const
    {
        return (m_srcPortrangeLowerTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortrangeLowerUdpUnion(const string 
		&srcPortrangeLowerUdpUnion)
    {
        m_srcPortrangeLowerUdpUnion  =  srcPortrangeLowerUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortrangeLowerUdpUnion() const
    {
        return (m_srcPortrangeLowerUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortrangeHigherTcpUnion(const string 
		&srcPortrangeHigherTcpUnion)
    {
        m_srcPortrangeHigherTcpUnion  =  srcPortrangeHigherTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortrangeHigherTcpUnion() const
    {
        return (m_srcPortrangeHigherTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortrangeHigherUdpUnion(const string 
		&srcPortrangeHigherUdpUnion)
    {
        m_srcPortrangeHigherUdpUnion  =  srcPortrangeHigherUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortrangeHigherUdpUnion() const
    {
        return (m_srcPortrangeHigherUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomNumberTcp(const UI32 
		&srcPortCustomNumberTcp)
    {
        m_srcPortCustomNumberTcp  =  srcPortCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomNumberTcp() const
    {
        return (m_srcPortCustomNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownNumberTcp(const 
		PortWellknownNumberTcp &srcPortWellknownNumberTcp)
    {
        m_srcPortWellknownNumberTcp = srcPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownNumberTcp() const
    {
        return (m_srcPortWellknownNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp)
    {
        m_srcPortCustomNumberUdp  =  srcPortCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomNumberUdp() const
    {
        return (m_srcPortCustomNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownNumberUdp(const 
		PortWellknownNumberUdp &srcPortWellknownNumberUdp)
    {
        m_srcPortWellknownNumberUdp  =  srcPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownNumberUdp() const
    {
        return (m_srcPortWellknownNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomrangeLowerTcp(const UI32 
		&srcPortCustomrangeLowerTcp)
    {
        m_srcPortCustomrangeLowerTcp  =  srcPortCustomrangeLowerTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomrangeLowerTcp() const
    {
        return (m_srcPortCustomrangeLowerTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownrangeLowerTcp(const 
		PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp)
    {
        m_srcPortWellknownrangeLowerTcp  =  srcPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownrangeLowerTcp() const
    {
        return (m_srcPortWellknownrangeLowerTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomrangeHigherTcp(const UI32 
		&srcPortCustomrangeHigherTcp)
    {
        m_srcPortCustomrangeHigherTcp  =  srcPortCustomrangeHigherTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomrangeHigherTcp() const
    {
        return (m_srcPortCustomrangeHigherTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp)
    {
        m_srcPortWellknownrangeHigherTcp  =  srcPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownrangeHigherTcp() const
    {
        return (m_srcPortWellknownrangeHigherTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomrangeLowerUdp(const UI32 
		&srcPortCustomrangeLowerUdp)
    {
        m_srcPortCustomrangeLowerUdp  =  srcPortCustomrangeLowerUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomrangeLowerUdp() const
    {
        return (m_srcPortCustomrangeLowerUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp)
    {
        m_srcPortWellknownrangeLowerUdp  =  srcPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownrangeLowerUdp() const
    {
        return (m_srcPortWellknownrangeLowerUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortCustomrangeHigherUdp(const UI32 
		&srcPortCustomrangeHigherUdp)
    {
        m_srcPortCustomrangeHigherUdp  =  srcPortCustomrangeHigherUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortCustomrangeHigherUdp() const
    {
        return (m_srcPortCustomrangeHigherUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp)
    {
        m_srcPortWellknownrangeHigherUdp  =  srcPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortWellknownrangeHigherUdp() const
    {
        return (m_srcPortWellknownrangeHigherUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortNumberTcpUnion(const string &dstPortNumberTcpUnion)
    {
        m_dstPortNumberTcpUnion  =  dstPortNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortNumberTcpUnion() const
    {
        return (m_dstPortNumberTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortNumberUdpUnion(const string &dstPortNumberUdpUnion)
    {
        m_dstPortNumberUdpUnion  =  dstPortNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortNumberUdpUnion() const
    {
        return (m_dstPortNumberUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortrangeLowerTcpUnion(const string &dstPortrangeLowerTcpUnion)
    {
        m_dstPortrangeLowerTcpUnion  =  dstPortrangeLowerTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortrangeLowerTcpUnion() const
    {
        return (m_dstPortrangeLowerTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortrangeLowerUdpUnion(const string &dstPortrangeLowerUdpUnion)
    {
        m_dstPortrangeLowerUdpUnion  =  dstPortrangeLowerUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortrangeLowerUdpUnion() const
    {
        return (m_dstPortrangeLowerUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortrangeHigherTcpUnion(const string &dstPortrangeHigherTcpUnion)
    {
        m_dstPortrangeHigherTcpUnion  =  dstPortrangeHigherTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortrangeHigherTcpUnion() const
    {
        return (m_dstPortrangeHigherTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortrangeHigherUdpUnion(const string &dstPortrangeHigherUdpUnion)
    {
        m_dstPortrangeHigherUdpUnion  =  dstPortrangeHigherUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortrangeHigherUdpUnion() const
    {
        return (m_dstPortrangeHigherUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortOperator(const PortOperator &dstPortOperator)
    {
        m_dstPortOperator  =  dstPortOperator;
    }

    PortOperator  AAALocalAssocIp6RuleToPolicyMessage::getDstPortOperator() const
    {
        return (m_dstPortOperator);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp)
    {
        m_dstPortCustomNumberTcp  =  dstPortCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomNumberTcp() const
    {
        return (m_dstPortCustomNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownNumberTcp(const PortWellknownNumberTcp 
		&dstPortWellknownNumberTcp)
    {
        m_dstPortWellknownNumberTcp  =  dstPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownNumberTcp() const
    {
        return (m_dstPortWellknownNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp)
    {
        m_dstPortCustomNumberUdp  =  dstPortCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomNumberUdp() const
    {
        return (m_dstPortCustomNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownNumberUdp(const PortWellknownNumberUdp 
		&dstPortWellknownNumberUdp)
    {
        m_dstPortWellknownNumberUdp  =  dstPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownNumberUdp() const
    {
        return (m_dstPortWellknownNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomrangeLowerTcp(const UI32 
		&dstPortCustomrangeLowerTcp)
    {
        m_dstPortCustomrangeLowerTcp  =  dstPortCustomrangeLowerTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomrangeLowerTcp() const
    {
        return (m_dstPortCustomrangeLowerTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp 
		&dstPortWellknownrangeLowerTcp)
    {
        m_dstPortWellknownrangeLowerTcp  =  dstPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownrangeLowerTcp() const
    {
        return (m_dstPortWellknownrangeLowerTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomrangeHigherTcp(const UI32 
		&dstPortCustomrangeHigherTcp)
    {
        m_dstPortCustomrangeHigherTcp  =  dstPortCustomrangeHigherTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomrangeHigherTcp() const
    {
        return (m_dstPortCustomrangeHigherTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp)
    {
        m_dstPortWellknownrangeHigherTcp  =  dstPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownrangeHigherTcp() const
    {
        return (m_dstPortWellknownrangeHigherTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomrangeLowerUdp(const UI32 
		&dstPortCustomrangeLowerUdp)
    {
        m_dstPortCustomrangeLowerUdp  =  dstPortCustomrangeLowerUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomrangeLowerUdp() const
    {
        return (m_dstPortCustomrangeLowerUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownrangeLowerUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp)
    {
        m_dstPortWellknownrangeLowerUdp  =  dstPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownrangeLowerUdp() const
    {
        return (m_dstPortWellknownrangeLowerUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortCustomrangeHigherUdp(const UI32 
		&dstPortCustomrangeHigherUdp)
    {
        m_dstPortCustomrangeHigherUdp  =  dstPortCustomrangeHigherUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortCustomrangeHigherUdp() const
    {
        return (m_dstPortCustomrangeHigherUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp)
    {
        m_dstPortWellknownrangeHigherUdp  =  dstPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortWellknownrangeHigherUdp() const
    {
        return (m_dstPortWellknownrangeHigherUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setWellknownPrecedence (const WellknownPrecedence &wellknownPrecedence)
    {
        m_wellknownPrecedence = wellknownPrecedence;
    }

    WellknownPrecedence AAALocalAssocIp6RuleToPolicyMessage::getWellknownPrecedence() const
    {
        return (m_wellknownPrecedence);
    }

    void AAALocalAssocIp6RuleToPolicyMessage::setCustomPrecedence (const UI32 &customPrecedence)
    {
        m_customPrecedence = customPrecedence;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getCustomPrecedence() const
    {
        return (m_customPrecedence);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setPrecedenceUnion(const string &precedenceUnion)
    {
        m_precedenceUnion = precedenceUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getPrecedenceUnion() const
    {
        return (m_precedenceUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDscpUnion(const string &dscpUnion)
    {
        m_dscpUnion = dscpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDscpUnion() const
    {
        return (m_dscpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setTosUnion(const string &tosUnion)
    {
        m_tosUnion = tosUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getTosUnion() const
    {
        return (m_tosUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setWellknownTos (const WellknownTos &wellknownTos)
    {
        m_wellknownTos = wellknownTos;
    }

    WellknownTos AAALocalAssocIp6RuleToPolicyMessage::getWellknownTos() const
    {
        return (m_wellknownTos);
    }

    void AAALocalAssocIp6RuleToPolicyMessage::setCustomTos (const UI32 &customTos)
    {
        m_customTos = customTos;
    }

	UI32 AAALocalAssocIp6RuleToPolicyMessage::getCustomTos() const
	{
		return (m_customTos);
	}

    void  AAALocalAssocIp6RuleToPolicyMessage::setWellknownDscp (const WellknownDscp &wellknownDscp)
    {
        m_wellknownDscp = wellknownDscp;
    }

    WellknownDscp AAALocalAssocIp6RuleToPolicyMessage::getWellknownDscp() const
    {
        return (m_wellknownDscp);
    }

    void AAALocalAssocIp6RuleToPolicyMessage::setCustomDscp (const UI32 &customDscp)
    {
        m_customDscp = customDscp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getCustomDscp() const
    {
        return (m_customDscp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setUrg(const bool &urg)
    {
        m_urg  =  urg;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getUrg() const
    {
        return (m_urg);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setAck(const bool &ack)
    {
        m_ack  =  ack;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getAck() const
    {
        return (m_ack);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setPush(const bool &push)
    {
        m_push  =  push;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getPush() const
    {
        return (m_push);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setFin(const bool &fin)
    {
        m_fin  =  fin;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getFin() const
    {
        return (m_fin);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setRst(const bool &rst)
    {
        m_rst  =  rst;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getRst() const
    {
        return (m_rst);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSync(const bool &sync)
    {
        m_sync  =  sync;
    }

    bool  AAALocalAssocIp6RuleToPolicyMessage::getSync() const
    {
        return (m_sync);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    UI8  AAALocalAssocIp6RuleToPolicyMessage::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltNumberTcpUnion(const string &srcPortltNumberTcpUnion)
    {
        m_srcPortltNumberTcpUnion  =  srcPortltNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltNumberTcpUnion() const
    {
        return (m_srcPortltNumberTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtNumberTcpUnion(const string &srcPortgtNumberTcpUnion)
    {
        m_srcPortgtNumberTcpUnion  =  srcPortgtNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtNumberTcpUnion() const
    {
        return (m_srcPortgtNumberTcpUnion);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltNumberUdpUnion(const string &srcPortltNumberUdpUnion)
    {
        m_srcPortltNumberUdpUnion  =  srcPortltNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltNumberUdpUnion() const
    {
        return (m_srcPortltNumberUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtNumberUdpUnion(const string &srcPortgtNumberUdpUnion)
    {
        m_srcPortgtNumberUdpUnion  =  srcPortgtNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtNumberUdpUnion() const
    {
        return (m_srcPortgtNumberUdpUnion);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp)
    {
        m_srcPortltCustomNumberTcp  =  srcPortltCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltCustomNumberTcp() const
    {
        return (m_srcPortltCustomNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp)
    {
        m_srcPortgtCustomNumberTcp  =  srcPortgtCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtCustomNumberTcp() const
    {
        return (m_srcPortgtCustomNumberTcp);
    }




    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp)
    {
        m_srcPortltWellknownNumberTcp  =  srcPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltWellknownNumberTcp() const
    {
        return (m_srcPortltWellknownNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp)
    {
        m_srcPortgtWellknownNumberTcp  =  srcPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtWellknownNumberTcp() const
    {
        return (m_srcPortgtWellknownNumberTcp);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp)
    {
        m_srcPortltCustomNumberUdp  =  srcPortltCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltCustomNumberUdp() const
    {
        return (m_srcPortltCustomNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp)
    {
        m_srcPortgtCustomNumberUdp  =  srcPortgtCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtCustomNumberUdp() const
    {
        return (m_srcPortgtCustomNumberUdp);
    }


    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp)
    {
        m_srcPortltWellknownNumberUdp  =  srcPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortltWellknownNumberUdp() const
    {
        return (m_srcPortltWellknownNumberUdp);
    }


    void  AAALocalAssocIp6RuleToPolicyMessage::setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp)
    {
        m_srcPortgtWellknownNumberUdp  =  srcPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getSrcPortgtWellknownNumberUdp() const
    {
        return (m_srcPortgtWellknownNumberUdp);
    }





    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltNumberTcpUnion(const string &dstPortltNumberTcpUnion)
    {
        m_dstPortltNumberTcpUnion  =  dstPortltNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltNumberTcpUnion() const
    {
        return (m_dstPortltNumberTcpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtNumberTcpUnion(const string &dstPortgtNumberTcpUnion)
    {
        m_dstPortgtNumberTcpUnion  =  dstPortgtNumberTcpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtNumberTcpUnion() const
    {
        return (m_dstPortgtNumberTcpUnion);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltNumberUdpUnion(const string &dstPortltNumberUdpUnion)
    {
        m_dstPortltNumberUdpUnion  =  dstPortltNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltNumberUdpUnion() const
    {
        return (m_dstPortltNumberUdpUnion);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtNumberUdpUnion(const string &dstPortgtNumberUdpUnion)
    {
        m_dstPortgtNumberUdpUnion  =  dstPortgtNumberUdpUnion;
    }

    string  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtNumberUdpUnion() const
    {
        return (m_dstPortgtNumberUdpUnion);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp)
    {
        m_dstPortltCustomNumberTcp  =  dstPortltCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltCustomNumberTcp() const
    {
        return (m_dstPortltCustomNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp)
    {
        m_dstPortgtCustomNumberTcp  =  dstPortgtCustomNumberTcp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtCustomNumberTcp() const
    {
        return (m_dstPortgtCustomNumberTcp);
    }




    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp)
    {
        m_dstPortltWellknownNumberTcp  =  dstPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltWellknownNumberTcp() const
    {
        return (m_dstPortltWellknownNumberTcp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp)
    {
        m_dstPortgtWellknownNumberTcp  =  dstPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtWellknownNumberTcp() const
    {
        return (m_dstPortgtWellknownNumberTcp);
    }



    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp)
    {
        m_dstPortltCustomNumberUdp  =  dstPortltCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltCustomNumberUdp() const
    {
        return (m_dstPortltCustomNumberUdp);
    }

    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp)
    {
        m_dstPortgtCustomNumberUdp  =  dstPortgtCustomNumberUdp;
    }

    UI32  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtCustomNumberUdp() const
    {
        return (m_dstPortgtCustomNumberUdp);
    }




    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp)
    {
        m_dstPortltWellknownNumberUdp  =  dstPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortltWellknownNumberUdp() const
    {
        return (m_dstPortltWellknownNumberUdp);
    }


    void  AAALocalAssocIp6RuleToPolicyMessage::setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp)
    {
        m_dstPortgtWellknownNumberUdp  =  dstPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIp6RuleToPolicyMessage::getDstPortgtWellknownNumberUdp() const
    {
        return (m_dstPortgtWellknownNumberUdp);
    }
}

	
