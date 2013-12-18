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

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    AAALocalAssocIpRuleToPolicyMessage::AAALocalAssocIpRuleToPolicyMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALASSOCIPRULETOPOLICY)
    {
    }

    AAALocalAssocIpRuleToPolicyMessage::AAALocalAssocIpRuleToPolicyMessage (
        const string &policyName, const UI32 &policyType, const UI64 &ruleid,
        const UI32 &action, const string &srcUnion, const IpAddrType &srcType,
        const IpV4Address &srcHostIpAddress, const IpV4Address &srcIpAddress,
        const IpV4Address &srcIpAddressMask, const bool &isCount,
        const bool &isLog, const IpV4Address &dstHostIpAddress,
        const IpV4Address &dstIpAddress, const IpV4Address &dstIpAddressMask,
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
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALASSOCIPRULETOPOLICY),
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

    AAALocalAssocIpRuleToPolicyMessage::~AAALocalAssocIpRuleToPolicyMessage ()
    {
    }
    
	void  AAALocalAssocIpRuleToPolicyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization (); 
        addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType, "policyType"));
        addSerializableAttribute (new AttributeUI64(&m_ruleid, "ruleid"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
        addSerializableAttribute (new AttributeString(&m_srcUnion, "srcUnion"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_srcType, "srcType"));
        addSerializableAttribute (new AttributeIpV4Address(&m_srcHostIpAddress, "srcHostIpAddress"));
        addSerializableAttribute (new AttributeIpV4Address(&m_srcIpAddress, "srcIpAddress"));
        addSerializableAttribute (new AttributeIpV4Address(&m_srcIpAddressMask, "srcIpAddressMask"));
        addSerializableAttribute (new AttributeBool(&m_isCount, "isCount"));
        addSerializableAttribute (new AttributeBool(&m_isLog, "isLog"));
        addSerializableAttribute (new AttributeIpV4Address(&m_dstHostIpAddress, "dstHostIpAddress"));
        addSerializableAttribute (new AttributeIpV4Address(&m_dstIpAddress, "dstIpAddress"));
        addSerializableAttribute (new AttributeIpV4Address(&m_dstIpAddressMask, "dstIpAddressMask"));
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

    void  AAALocalAssocIpRuleToPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  AAALocalAssocIpRuleToPolicyMessage::getRuleid() const
    {
        return (m_ruleid);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }


    UI32  AAALocalAssocIpRuleToPolicyMessage::getAction() const
    {
        return (m_action);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcUnion(const string &srcUnion)
    {
        m_srcUnion  =  srcUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcUnion() const
    {
        return (m_srcUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  AAALocalAssocIpRuleToPolicyMessage::getSrcType() const
    {
        return (m_srcType);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcHostIpAddress(const IpV4Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcIpAddress(const IpV4Address &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcIpAddressMask(const IpV4Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getIsCount() const
    {
        return (m_isCount);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getIsLog() const
    {
        return (m_isLog);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstHostIpAddress(const IpV4Address &dstHostIpAddress)
    {
        m_dstHostIpAddress  =  dstHostIpAddress;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getDstHostIpAddress() const
    {
        return (m_dstHostIpAddress);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstIpAddress(const IpV4Address &dstIpAddress)
    {
        m_dstIpAddress  =  dstIpAddress;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getDstIpAddress() const
    {
        return (m_dstIpAddress);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstIpAddressMask(const IpV4Address &dstIpAddressMask)
    {
        m_dstIpAddressMask  =  dstIpAddressMask;
    }

    IpV4Address  AAALocalAssocIpRuleToPolicyMessage::getDstIpAddressMask() const
    {
        return (m_dstIpAddressMask);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstUnion(const string &dstUnion)
    {
        m_dstUnion  =  dstUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstUnion() const
    {
        return (m_dstUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstType(const IpAddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    IpAddrType  AAALocalAssocIpRuleToPolicyMessage::getDstType() const
    {
        return (m_dstType);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setProtocolTypeUnion(const string &ProtocolTypeUnion)
    {
        m_ProtocolTypeUnion  =  ProtocolTypeUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getProtocolTypeUnion() const
    {
        return (m_ProtocolTypeUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType)
    {
        m_l3l4CustomProtoType  =  l3l4CustomProtoType;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getL3l4CustomProtoType() const
    {
        return (m_l3l4CustomProtoType);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setL3l4WellknownProtoType(const L3L4wellknownProtoType 
		&l3l4WellknownProtoType)
    {
        m_l3l4WellknownProtoType  =  l3l4WellknownProtoType;
    }

    L3L4wellknownProtoType  AAALocalAssocIpRuleToPolicyMessage::getL3l4WellknownProtoType() const
    {
        return (m_l3l4WellknownProtoType);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortOperator(const PortOperator &srcPortOperator)
    {
        m_srcPortOperator  =  srcPortOperator;
    }

    PortOperator  AAALocalAssocIpRuleToPolicyMessage::getSrcPortOperator() const
    {
        return (m_srcPortOperator);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortNumberTcpUnion(const string &srcPortNumberTcpUnion)
    {
        m_srcPortNumberTcpUnion  =  srcPortNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortNumberTcpUnion() const
    {
        return (m_srcPortNumberTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortNumberUdpUnion(const string 
		&srcPortNumberUdpUnion)
    {
        m_srcPortNumberUdpUnion  =  srcPortNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortNumberUdpUnion() const
    {
        return (m_srcPortNumberUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortrangeLowerTcpUnion(const string 
		&srcPortrangeLowerTcpUnion)
    {
        m_srcPortrangeLowerTcpUnion  =  srcPortrangeLowerTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortrangeLowerTcpUnion() const
    {
        return (m_srcPortrangeLowerTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortrangeLowerUdpUnion(const string 
		&srcPortrangeLowerUdpUnion)
    {
        m_srcPortrangeLowerUdpUnion  =  srcPortrangeLowerUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortrangeLowerUdpUnion() const
    {
        return (m_srcPortrangeLowerUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortrangeHigherTcpUnion(const string 
		&srcPortrangeHigherTcpUnion)
    {
        m_srcPortrangeHigherTcpUnion  =  srcPortrangeHigherTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortrangeHigherTcpUnion() const
    {
        return (m_srcPortrangeHigherTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortrangeHigherUdpUnion(const string 
		&srcPortrangeHigherUdpUnion)
    {
        m_srcPortrangeHigherUdpUnion  =  srcPortrangeHigherUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortrangeHigherUdpUnion() const
    {
        return (m_srcPortrangeHigherUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomNumberTcp(const UI32 
		&srcPortCustomNumberTcp)
    {
        m_srcPortCustomNumberTcp  =  srcPortCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomNumberTcp() const
    {
        return (m_srcPortCustomNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownNumberTcp(const 
		PortWellknownNumberTcp &srcPortWellknownNumberTcp)
    {
        m_srcPortWellknownNumberTcp = srcPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownNumberTcp() const
    {
        return (m_srcPortWellknownNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp)
    {
        m_srcPortCustomNumberUdp  =  srcPortCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomNumberUdp() const
    {
        return (m_srcPortCustomNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownNumberUdp(const 
		PortWellknownNumberUdp &srcPortWellknownNumberUdp)
    {
        m_srcPortWellknownNumberUdp  =  srcPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownNumberUdp() const
    {
        return (m_srcPortWellknownNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomrangeLowerTcp(const UI32 
		&srcPortCustomrangeLowerTcp)
    {
        m_srcPortCustomrangeLowerTcp  =  srcPortCustomrangeLowerTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomrangeLowerTcp() const
    {
        return (m_srcPortCustomrangeLowerTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeLowerTcp(const 
		PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp)
    {
        m_srcPortWellknownrangeLowerTcp  =  srcPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeLowerTcp() const
    {
        return (m_srcPortWellknownrangeLowerTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomrangeHigherTcp(const UI32 
		&srcPortCustomrangeHigherTcp)
    {
        m_srcPortCustomrangeHigherTcp  =  srcPortCustomrangeHigherTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomrangeHigherTcp() const
    {
        return (m_srcPortCustomrangeHigherTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp)
    {
        m_srcPortWellknownrangeHigherTcp  =  srcPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeHigherTcp() const
    {
        return (m_srcPortWellknownrangeHigherTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomrangeLowerUdp(const UI32 
		&srcPortCustomrangeLowerUdp)
    {
        m_srcPortCustomrangeLowerUdp  =  srcPortCustomrangeLowerUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomrangeLowerUdp() const
    {
        return (m_srcPortCustomrangeLowerUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp)
    {
        m_srcPortWellknownrangeLowerUdp  =  srcPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeLowerUdp() const
    {
        return (m_srcPortWellknownrangeLowerUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortCustomrangeHigherUdp(const UI32 
		&srcPortCustomrangeHigherUdp)
    {
        m_srcPortCustomrangeHigherUdp  =  srcPortCustomrangeHigherUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortCustomrangeHigherUdp() const
    {
        return (m_srcPortCustomrangeHigherUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp)
    {
        m_srcPortWellknownrangeHigherUdp  =  srcPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeHigherUdp() const
    {
        return (m_srcPortWellknownrangeHigherUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortNumberTcpUnion(const string &dstPortNumberTcpUnion)
    {
        m_dstPortNumberTcpUnion  =  dstPortNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortNumberTcpUnion() const
    {
        return (m_dstPortNumberTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortNumberUdpUnion(const string &dstPortNumberUdpUnion)
    {
        m_dstPortNumberUdpUnion  =  dstPortNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortNumberUdpUnion() const
    {
        return (m_dstPortNumberUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortrangeLowerTcpUnion(const string &dstPortrangeLowerTcpUnion)
    {
        m_dstPortrangeLowerTcpUnion  =  dstPortrangeLowerTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortrangeLowerTcpUnion() const
    {
        return (m_dstPortrangeLowerTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortrangeLowerUdpUnion(const string &dstPortrangeLowerUdpUnion)
    {
        m_dstPortrangeLowerUdpUnion  =  dstPortrangeLowerUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortrangeLowerUdpUnion() const
    {
        return (m_dstPortrangeLowerUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortrangeHigherTcpUnion(const string &dstPortrangeHigherTcpUnion)
    {
        m_dstPortrangeHigherTcpUnion  =  dstPortrangeHigherTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortrangeHigherTcpUnion() const
    {
        return (m_dstPortrangeHigherTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortrangeHigherUdpUnion(const string &dstPortrangeHigherUdpUnion)
    {
        m_dstPortrangeHigherUdpUnion  =  dstPortrangeHigherUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortrangeHigherUdpUnion() const
    {
        return (m_dstPortrangeHigherUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortOperator(const PortOperator &dstPortOperator)
    {
        m_dstPortOperator  =  dstPortOperator;
    }

    PortOperator  AAALocalAssocIpRuleToPolicyMessage::getDstPortOperator() const
    {
        return (m_dstPortOperator);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp)
    {
        m_dstPortCustomNumberTcp  =  dstPortCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomNumberTcp() const
    {
        return (m_dstPortCustomNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownNumberTcp(const PortWellknownNumberTcp 
		&dstPortWellknownNumberTcp)
    {
        m_dstPortWellknownNumberTcp  =  dstPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownNumberTcp() const
    {
        return (m_dstPortWellknownNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp)
    {
        m_dstPortCustomNumberUdp  =  dstPortCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomNumberUdp() const
    {
        return (m_dstPortCustomNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownNumberUdp(const PortWellknownNumberUdp 
		&dstPortWellknownNumberUdp)
    {
        m_dstPortWellknownNumberUdp  =  dstPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownNumberUdp() const
    {
        return (m_dstPortWellknownNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomrangeLowerTcp(const UI32 
		&dstPortCustomrangeLowerTcp)
    {
        m_dstPortCustomrangeLowerTcp  =  dstPortCustomrangeLowerTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomrangeLowerTcp() const
    {
        return (m_dstPortCustomrangeLowerTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp 
		&dstPortWellknownrangeLowerTcp)
    {
        m_dstPortWellknownrangeLowerTcp  =  dstPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownrangeLowerTcp() const
    {
        return (m_dstPortWellknownrangeLowerTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomrangeHigherTcp(const UI32 
		&dstPortCustomrangeHigherTcp)
    {
        m_dstPortCustomrangeHigherTcp  =  dstPortCustomrangeHigherTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomrangeHigherTcp() const
    {
        return (m_dstPortCustomrangeHigherTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp)
    {
        m_dstPortWellknownrangeHigherTcp  =  dstPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownrangeHigherTcp() const
    {
        return (m_dstPortWellknownrangeHigherTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomrangeLowerUdp(const UI32 
		&dstPortCustomrangeLowerUdp)
    {
        m_dstPortCustomrangeLowerUdp  =  dstPortCustomrangeLowerUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomrangeLowerUdp() const
    {
        return (m_dstPortCustomrangeLowerUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownrangeLowerUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp)
    {
        m_dstPortWellknownrangeLowerUdp  =  dstPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownrangeLowerUdp() const
    {
        return (m_dstPortWellknownrangeLowerUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortCustomrangeHigherUdp(const UI32 
		&dstPortCustomrangeHigherUdp)
    {
        m_dstPortCustomrangeHigherUdp  =  dstPortCustomrangeHigherUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortCustomrangeHigherUdp() const
    {
        return (m_dstPortCustomrangeHigherUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp)
    {
        m_dstPortWellknownrangeHigherUdp  =  dstPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getDstPortWellknownrangeHigherUdp() const
    {
        return (m_dstPortWellknownrangeHigherUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setWellknownPrecedence (const WellknownPrecedence &wellknownPrecedence)
    {
        m_wellknownPrecedence = wellknownPrecedence;
    }

    WellknownPrecedence AAALocalAssocIpRuleToPolicyMessage::getWellknownPrecedence() const
    {
        return (m_wellknownPrecedence);
    }

    void AAALocalAssocIpRuleToPolicyMessage::setCustomPrecedence (const UI32 &customPrecedence)
    {
        m_customPrecedence = customPrecedence;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getCustomPrecedence() const
    {
        return (m_customPrecedence);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setPrecedenceUnion(const string &precedenceUnion)
    {
        m_precedenceUnion = precedenceUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getPrecedenceUnion() const
    {
        return (m_precedenceUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDscpUnion(const string &dscpUnion)
    {
        m_dscpUnion = dscpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDscpUnion() const
    {
        return (m_dscpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setTosUnion(const string &tosUnion)
    {
        m_tosUnion = tosUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getTosUnion() const
    {
        return (m_tosUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setWellknownTos (const WellknownTos &wellknownTos)
    {
        m_wellknownTos = wellknownTos;
    }

    WellknownTos AAALocalAssocIpRuleToPolicyMessage::getWellknownTos() const
    {
        return (m_wellknownTos);
    }

    void AAALocalAssocIpRuleToPolicyMessage::setCustomTos (const UI32 &customTos)
    {
        m_customTos = customTos;
    }

	UI32 AAALocalAssocIpRuleToPolicyMessage::getCustomTos() const
	{
		return (m_customTos);
	}

    void  AAALocalAssocIpRuleToPolicyMessage::setWellknownDscp (const WellknownDscp &wellknownDscp)
    {
        m_wellknownDscp = wellknownDscp;
    }

    WellknownDscp AAALocalAssocIpRuleToPolicyMessage::getWellknownDscp() const
    {
        return (m_wellknownDscp);
    }

    void AAALocalAssocIpRuleToPolicyMessage::setCustomDscp (const UI32 &customDscp)
    {
        m_customDscp = customDscp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getCustomDscp() const
    {
        return (m_customDscp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setUrg(const bool &urg)
    {
        m_urg  =  urg;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getUrg() const
    {
        return (m_urg);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setAck(const bool &ack)
    {
        m_ack  =  ack;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getAck() const
    {
        return (m_ack);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setPush(const bool &push)
    {
        m_push  =  push;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getPush() const
    {
        return (m_push);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setFin(const bool &fin)
    {
        m_fin  =  fin;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getFin() const
    {
        return (m_fin);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setRst(const bool &rst)
    {
        m_rst  =  rst;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getRst() const
    {
        return (m_rst);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSync(const bool &sync)
    {
        m_sync  =  sync;
    }

    bool  AAALocalAssocIpRuleToPolicyMessage::getSync() const
    {
        return (m_sync);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    UI8  AAALocalAssocIpRuleToPolicyMessage::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltNumberTcpUnion(const string &srcPortltNumberTcpUnion)
    {
        m_srcPortltNumberTcpUnion  =  srcPortltNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltNumberTcpUnion() const
    {
        return (m_srcPortltNumberTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtNumberTcpUnion(const string &srcPortgtNumberTcpUnion)
    {
        m_srcPortgtNumberTcpUnion  =  srcPortgtNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtNumberTcpUnion() const
    {
        return (m_srcPortgtNumberTcpUnion);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltNumberUdpUnion(const string &srcPortltNumberUdpUnion)
    {
        m_srcPortltNumberUdpUnion  =  srcPortltNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltNumberUdpUnion() const
    {
        return (m_srcPortltNumberUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtNumberUdpUnion(const string &srcPortgtNumberUdpUnion)
    {
        m_srcPortgtNumberUdpUnion  =  srcPortgtNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtNumberUdpUnion() const
    {
        return (m_srcPortgtNumberUdpUnion);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp)
    {
        m_srcPortltCustomNumberTcp  =  srcPortltCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltCustomNumberTcp() const
    {
        return (m_srcPortltCustomNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp)
    {
        m_srcPortgtCustomNumberTcp  =  srcPortgtCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtCustomNumberTcp() const
    {
        return (m_srcPortgtCustomNumberTcp);
    }




    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp)
    {
        m_srcPortltWellknownNumberTcp  =  srcPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltWellknownNumberTcp() const
    {
        return (m_srcPortltWellknownNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp)
    {
        m_srcPortgtWellknownNumberTcp  =  srcPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtWellknownNumberTcp() const
    {
        return (m_srcPortgtWellknownNumberTcp);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp)
    {
        m_srcPortltCustomNumberUdp  =  srcPortltCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltCustomNumberUdp() const
    {
        return (m_srcPortltCustomNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp)
    {
        m_srcPortgtCustomNumberUdp  =  srcPortgtCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtCustomNumberUdp() const
    {
        return (m_srcPortgtCustomNumberUdp);
    }


    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp)
    {
        m_srcPortltWellknownNumberUdp  =  srcPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortltWellknownNumberUdp() const
    {
        return (m_srcPortltWellknownNumberUdp);
    }


    void  AAALocalAssocIpRuleToPolicyMessage::setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp)
    {
        m_srcPortgtWellknownNumberUdp  =  srcPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getSrcPortgtWellknownNumberUdp() const
    {
        return (m_srcPortgtWellknownNumberUdp);
    }





    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltNumberTcpUnion(const string &dstPortltNumberTcpUnion)
    {
        m_dstPortltNumberTcpUnion  =  dstPortltNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortltNumberTcpUnion() const
    {
        return (m_dstPortltNumberTcpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtNumberTcpUnion(const string &dstPortgtNumberTcpUnion)
    {
        m_dstPortgtNumberTcpUnion  =  dstPortgtNumberTcpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtNumberTcpUnion() const
    {
        return (m_dstPortgtNumberTcpUnion);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltNumberUdpUnion(const string &dstPortltNumberUdpUnion)
    {
        m_dstPortltNumberUdpUnion  =  dstPortltNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortltNumberUdpUnion() const
    {
        return (m_dstPortltNumberUdpUnion);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtNumberUdpUnion(const string &dstPortgtNumberUdpUnion)
    {
        m_dstPortgtNumberUdpUnion  =  dstPortgtNumberUdpUnion;
    }

    string  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtNumberUdpUnion() const
    {
        return (m_dstPortgtNumberUdpUnion);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp)
    {
        m_dstPortltCustomNumberTcp  =  dstPortltCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortltCustomNumberTcp() const
    {
        return (m_dstPortltCustomNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp)
    {
        m_dstPortgtCustomNumberTcp  =  dstPortgtCustomNumberTcp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtCustomNumberTcp() const
    {
        return (m_dstPortgtCustomNumberTcp);
    }




    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp)
    {
        m_dstPortltWellknownNumberTcp  =  dstPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getDstPortltWellknownNumberTcp() const
    {
        return (m_dstPortltWellknownNumberTcp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp)
    {
        m_dstPortgtWellknownNumberTcp  =  dstPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtWellknownNumberTcp() const
    {
        return (m_dstPortgtWellknownNumberTcp);
    }



    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp)
    {
        m_dstPortltCustomNumberUdp  =  dstPortltCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortltCustomNumberUdp() const
    {
        return (m_dstPortltCustomNumberUdp);
    }

    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp)
    {
        m_dstPortgtCustomNumberUdp  =  dstPortgtCustomNumberUdp;
    }

    UI32  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtCustomNumberUdp() const
    {
        return (m_dstPortgtCustomNumberUdp);
    }




    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp)
    {
        m_dstPortltWellknownNumberUdp  =  dstPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getDstPortltWellknownNumberUdp() const
    {
        return (m_dstPortltWellknownNumberUdp);
    }


    void  AAALocalAssocIpRuleToPolicyMessage::setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp)
    {
        m_dstPortgtWellknownNumberUdp  =  dstPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  AAALocalAssocIpRuleToPolicyMessage::getDstPortgtWellknownNumberUdp() const
    {
        return (m_dstPortgtWellknownNumberUdp);
    }
}

	
