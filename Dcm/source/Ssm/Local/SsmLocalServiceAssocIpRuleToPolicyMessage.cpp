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

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalServiceAssocIpRuleToPolicyMessage.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/Attributes.h"
namespace DcmNs
{

    SsmLocalServiceAssocIpRuleToPolicyMessage::SsmLocalServiceAssocIpRuleToPolicyMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMLOCALSERVICEASSOCIPRULETOPOLICY)
    {
    }

    SsmLocalServiceAssocIpRuleToPolicyMessage::SsmLocalServiceAssocIpRuleToPolicyMessage (
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
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (), SSMLOCALSERVICEASSOCIPRULETOPOLICY),
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

    SsmLocalServiceAssocIpRuleToPolicyMessage::~SsmLocalServiceAssocIpRuleToPolicyMessage ()
    {
    }
    
	void  SsmLocalServiceAssocIpRuleToPolicyMessage::setupAttributesForSerialization()
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

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  SsmLocalServiceAssocIpRuleToPolicyMessage::getRuleid() const
    {
        return (m_ruleid);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }


    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getAction() const
    {
        return (m_action);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcUnion(const string &srcUnion)
    {
        m_srcUnion  =  srcUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcUnion() const
    {
        return (m_srcUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcType() const
    {
        return (m_srcType);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcHostIpAddress(const IpV4Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcIpAddress(const IpV4Address &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcIpAddressMask(const IpV4Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getIsCount() const
    {
        return (m_isCount);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getIsLog() const
    {
        return (m_isLog);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstHostIpAddress(const IpV4Address &dstHostIpAddress)
    {
        m_dstHostIpAddress  =  dstHostIpAddress;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstHostIpAddress() const
    {
        return (m_dstHostIpAddress);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstIpAddress(const IpV4Address &dstIpAddress)
    {
        m_dstIpAddress  =  dstIpAddress;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstIpAddress() const
    {
        return (m_dstIpAddress);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstIpAddressMask(const IpV4Address &dstIpAddressMask)
    {
        m_dstIpAddressMask  =  dstIpAddressMask;
    }

    IpV4Address  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstIpAddressMask() const
    {
        return (m_dstIpAddressMask);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstUnion(const string &dstUnion)
    {
        m_dstUnion  =  dstUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstUnion() const
    {
        return (m_dstUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstType(const IpAddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    IpAddrType  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstType() const
    {
        return (m_dstType);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setProtocolTypeUnion(const string &ProtocolTypeUnion)
    {
        m_ProtocolTypeUnion  =  ProtocolTypeUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getProtocolTypeUnion() const
    {
        return (m_ProtocolTypeUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType)
    {
        m_l3l4CustomProtoType  =  l3l4CustomProtoType;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getL3l4CustomProtoType() const
    {
        return (m_l3l4CustomProtoType);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setL3l4WellknownProtoType(const L3L4wellknownProtoType 
		&l3l4WellknownProtoType)
    {
        m_l3l4WellknownProtoType  =  l3l4WellknownProtoType;
    }

    L3L4wellknownProtoType  SsmLocalServiceAssocIpRuleToPolicyMessage::getL3l4WellknownProtoType() const
    {
        return (m_l3l4WellknownProtoType);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortOperator(const PortOperator &srcPortOperator)
    {
        m_srcPortOperator  =  srcPortOperator;
    }

    PortOperator  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortOperator() const
    {
        return (m_srcPortOperator);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortNumberTcpUnion(const string &srcPortNumberTcpUnion)
    {
        m_srcPortNumberTcpUnion  =  srcPortNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortNumberTcpUnion() const
    {
        return (m_srcPortNumberTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortNumberUdpUnion(const string 
		&srcPortNumberUdpUnion)
    {
        m_srcPortNumberUdpUnion  =  srcPortNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortNumberUdpUnion() const
    {
        return (m_srcPortNumberUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortrangeLowerTcpUnion(const string 
		&srcPortrangeLowerTcpUnion)
    {
        m_srcPortrangeLowerTcpUnion  =  srcPortrangeLowerTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortrangeLowerTcpUnion() const
    {
        return (m_srcPortrangeLowerTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortrangeLowerUdpUnion(const string 
		&srcPortrangeLowerUdpUnion)
    {
        m_srcPortrangeLowerUdpUnion  =  srcPortrangeLowerUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortrangeLowerUdpUnion() const
    {
        return (m_srcPortrangeLowerUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortrangeHigherTcpUnion(const string 
		&srcPortrangeHigherTcpUnion)
    {
        m_srcPortrangeHigherTcpUnion  =  srcPortrangeHigherTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortrangeHigherTcpUnion() const
    {
        return (m_srcPortrangeHigherTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortrangeHigherUdpUnion(const string 
		&srcPortrangeHigherUdpUnion)
    {
        m_srcPortrangeHigherUdpUnion  =  srcPortrangeHigherUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortrangeHigherUdpUnion() const
    {
        return (m_srcPortrangeHigherUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomNumberTcp(const UI32 
		&srcPortCustomNumberTcp)
    {
        m_srcPortCustomNumberTcp  =  srcPortCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomNumberTcp() const
    {
        return (m_srcPortCustomNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownNumberTcp(const 
		PortWellknownNumberTcp &srcPortWellknownNumberTcp)
    {
        m_srcPortWellknownNumberTcp = srcPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownNumberTcp() const
    {
        return (m_srcPortWellknownNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp)
    {
        m_srcPortCustomNumberUdp  =  srcPortCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomNumberUdp() const
    {
        return (m_srcPortCustomNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownNumberUdp(const 
		PortWellknownNumberUdp &srcPortWellknownNumberUdp)
    {
        m_srcPortWellknownNumberUdp  =  srcPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownNumberUdp() const
    {
        return (m_srcPortWellknownNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomrangeLowerTcp(const UI32 
		&srcPortCustomrangeLowerTcp)
    {
        m_srcPortCustomrangeLowerTcp  =  srcPortCustomrangeLowerTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomrangeLowerTcp() const
    {
        return (m_srcPortCustomrangeLowerTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeLowerTcp(const 
		PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp)
    {
        m_srcPortWellknownrangeLowerTcp  =  srcPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeLowerTcp() const
    {
        return (m_srcPortWellknownrangeLowerTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomrangeHigherTcp(const UI32 
		&srcPortCustomrangeHigherTcp)
    {
        m_srcPortCustomrangeHigherTcp  =  srcPortCustomrangeHigherTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomrangeHigherTcp() const
    {
        return (m_srcPortCustomrangeHigherTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp)
    {
        m_srcPortWellknownrangeHigherTcp  =  srcPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeHigherTcp() const
    {
        return (m_srcPortWellknownrangeHigherTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomrangeLowerUdp(const UI32 
		&srcPortCustomrangeLowerUdp)
    {
        m_srcPortCustomrangeLowerUdp  =  srcPortCustomrangeLowerUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomrangeLowerUdp() const
    {
        return (m_srcPortCustomrangeLowerUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp)
    {
        m_srcPortWellknownrangeLowerUdp  =  srcPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeLowerUdp() const
    {
        return (m_srcPortWellknownrangeLowerUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortCustomrangeHigherUdp(const UI32 
		&srcPortCustomrangeHigherUdp)
    {
        m_srcPortCustomrangeHigherUdp  =  srcPortCustomrangeHigherUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortCustomrangeHigherUdp() const
    {
        return (m_srcPortCustomrangeHigherUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp)
    {
        m_srcPortWellknownrangeHigherUdp  =  srcPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortWellknownrangeHigherUdp() const
    {
        return (m_srcPortWellknownrangeHigherUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortNumberTcpUnion(const string &dstPortNumberTcpUnion)
    {
        m_dstPortNumberTcpUnion  =  dstPortNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortNumberTcpUnion() const
    {
        return (m_dstPortNumberTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortNumberUdpUnion(const string &dstPortNumberUdpUnion)
    {
        m_dstPortNumberUdpUnion  =  dstPortNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortNumberUdpUnion() const
    {
        return (m_dstPortNumberUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortrangeLowerTcpUnion(const string &dstPortrangeLowerTcpUnion)
    {
        m_dstPortrangeLowerTcpUnion  =  dstPortrangeLowerTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortrangeLowerTcpUnion() const
    {
        return (m_dstPortrangeLowerTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortrangeLowerUdpUnion(const string &dstPortrangeLowerUdpUnion)
    {
        m_dstPortrangeLowerUdpUnion  =  dstPortrangeLowerUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortrangeLowerUdpUnion() const
    {
        return (m_dstPortrangeLowerUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortrangeHigherTcpUnion(const string &dstPortrangeHigherTcpUnion)
    {
        m_dstPortrangeHigherTcpUnion  =  dstPortrangeHigherTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortrangeHigherTcpUnion() const
    {
        return (m_dstPortrangeHigherTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortrangeHigherUdpUnion(const string &dstPortrangeHigherUdpUnion)
    {
        m_dstPortrangeHigherUdpUnion  =  dstPortrangeHigherUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortrangeHigherUdpUnion() const
    {
        return (m_dstPortrangeHigherUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortOperator(const PortOperator &dstPortOperator)
    {
        m_dstPortOperator  =  dstPortOperator;
    }

    PortOperator  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortOperator() const
    {
        return (m_dstPortOperator);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp)
    {
        m_dstPortCustomNumberTcp  =  dstPortCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomNumberTcp() const
    {
        return (m_dstPortCustomNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownNumberTcp(const PortWellknownNumberTcp 
		&dstPortWellknownNumberTcp)
    {
        m_dstPortWellknownNumberTcp  =  dstPortWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownNumberTcp() const
    {
        return (m_dstPortWellknownNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp)
    {
        m_dstPortCustomNumberUdp  =  dstPortCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomNumberUdp() const
    {
        return (m_dstPortCustomNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownNumberUdp(const PortWellknownNumberUdp 
		&dstPortWellknownNumberUdp)
    {
        m_dstPortWellknownNumberUdp  =  dstPortWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownNumberUdp() const
    {
        return (m_dstPortWellknownNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomrangeLowerTcp(const UI32 
		&dstPortCustomrangeLowerTcp)
    {
        m_dstPortCustomrangeLowerTcp  =  dstPortCustomrangeLowerTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomrangeLowerTcp() const
    {
        return (m_dstPortCustomrangeLowerTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp 
		&dstPortWellknownrangeLowerTcp)
    {
        m_dstPortWellknownrangeLowerTcp  =  dstPortWellknownrangeLowerTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownrangeLowerTcp() const
    {
        return (m_dstPortWellknownrangeLowerTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomrangeHigherTcp(const UI32 
		&dstPortCustomrangeHigherTcp)
    {
        m_dstPortCustomrangeHigherTcp  =  dstPortCustomrangeHigherTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomrangeHigherTcp() const
    {
        return (m_dstPortCustomrangeHigherTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp)
    {
        m_dstPortWellknownrangeHigherTcp  =  dstPortWellknownrangeHigherTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownrangeHigherTcp() const
    {
        return (m_dstPortWellknownrangeHigherTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomrangeLowerUdp(const UI32 
		&dstPortCustomrangeLowerUdp)
    {
        m_dstPortCustomrangeLowerUdp  =  dstPortCustomrangeLowerUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomrangeLowerUdp() const
    {
        return (m_dstPortCustomrangeLowerUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownrangeLowerUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp)
    {
        m_dstPortWellknownrangeLowerUdp  =  dstPortWellknownrangeLowerUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownrangeLowerUdp() const
    {
        return (m_dstPortWellknownrangeLowerUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortCustomrangeHigherUdp(const UI32 
		&dstPortCustomrangeHigherUdp)
    {
        m_dstPortCustomrangeHigherUdp  =  dstPortCustomrangeHigherUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortCustomrangeHigherUdp() const
    {
        return (m_dstPortCustomrangeHigherUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortWellknownrangeHigherUdp(const 
		PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp)
    {
        m_dstPortWellknownrangeHigherUdp  =  dstPortWellknownrangeHigherUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortWellknownrangeHigherUdp() const
    {
        return (m_dstPortWellknownrangeHigherUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setWellknownPrecedence (const WellknownPrecedence &wellknownPrecedence)
    {
        m_wellknownPrecedence = wellknownPrecedence;
    }

    WellknownPrecedence SsmLocalServiceAssocIpRuleToPolicyMessage::getWellknownPrecedence() const
    {
        return (m_wellknownPrecedence);
    }

    void SsmLocalServiceAssocIpRuleToPolicyMessage::setCustomPrecedence (const UI32 &customPrecedence)
    {
        m_customPrecedence = customPrecedence;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getCustomPrecedence() const
    {
        return (m_customPrecedence);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setPrecedenceUnion(const string &precedenceUnion)
    {
        m_precedenceUnion = precedenceUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getPrecedenceUnion() const
    {
        return (m_precedenceUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDscpUnion(const string &dscpUnion)
    {
        m_dscpUnion = dscpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDscpUnion() const
    {
        return (m_dscpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setTosUnion(const string &tosUnion)
    {
        m_tosUnion = tosUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getTosUnion() const
    {
        return (m_tosUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setWellknownTos (const WellknownTos &wellknownTos)
    {
        m_wellknownTos = wellknownTos;
    }

    WellknownTos SsmLocalServiceAssocIpRuleToPolicyMessage::getWellknownTos() const
    {
        return (m_wellknownTos);
    }

    void SsmLocalServiceAssocIpRuleToPolicyMessage::setCustomTos (const UI32 &customTos)
    {
        m_customTos = customTos;
    }

	UI32 SsmLocalServiceAssocIpRuleToPolicyMessage::getCustomTos() const
	{
		return (m_customTos);
	}

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setWellknownDscp (const WellknownDscp &wellknownDscp)
    {
        m_wellknownDscp = wellknownDscp;
    }

    WellknownDscp SsmLocalServiceAssocIpRuleToPolicyMessage::getWellknownDscp() const
    {
        return (m_wellknownDscp);
    }

    void SsmLocalServiceAssocIpRuleToPolicyMessage::setCustomDscp (const UI32 &customDscp)
    {
        m_customDscp = customDscp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getCustomDscp() const
    {
        return (m_customDscp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setUrg(const bool &urg)
    {
        m_urg  =  urg;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getUrg() const
    {
        return (m_urg);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setAck(const bool &ack)
    {
        m_ack  =  ack;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getAck() const
    {
        return (m_ack);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setPush(const bool &push)
    {
        m_push  =  push;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getPush() const
    {
        return (m_push);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setFin(const bool &fin)
    {
        m_fin  =  fin;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getFin() const
    {
        return (m_fin);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setRst(const bool &rst)
    {
        m_rst  =  rst;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getRst() const
    {
        return (m_rst);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSync(const bool &sync)
    {
        m_sync  =  sync;
    }

    bool  SsmLocalServiceAssocIpRuleToPolicyMessage::getSync() const
    {
        return (m_sync);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    UI8  SsmLocalServiceAssocIpRuleToPolicyMessage::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltNumberTcpUnion(const string &srcPortltNumberTcpUnion)
    {
        m_srcPortltNumberTcpUnion  =  srcPortltNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltNumberTcpUnion() const
    {
        return (m_srcPortltNumberTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtNumberTcpUnion(const string &srcPortgtNumberTcpUnion)
    {
        m_srcPortgtNumberTcpUnion  =  srcPortgtNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtNumberTcpUnion() const
    {
        return (m_srcPortgtNumberTcpUnion);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltNumberUdpUnion(const string &srcPortltNumberUdpUnion)
    {
        m_srcPortltNumberUdpUnion  =  srcPortltNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltNumberUdpUnion() const
    {
        return (m_srcPortltNumberUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtNumberUdpUnion(const string &srcPortgtNumberUdpUnion)
    {
        m_srcPortgtNumberUdpUnion  =  srcPortgtNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtNumberUdpUnion() const
    {
        return (m_srcPortgtNumberUdpUnion);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp)
    {
        m_srcPortltCustomNumberTcp  =  srcPortltCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltCustomNumberTcp() const
    {
        return (m_srcPortltCustomNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp)
    {
        m_srcPortgtCustomNumberTcp  =  srcPortgtCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtCustomNumberTcp() const
    {
        return (m_srcPortgtCustomNumberTcp);
    }




    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp)
    {
        m_srcPortltWellknownNumberTcp  =  srcPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltWellknownNumberTcp() const
    {
        return (m_srcPortltWellknownNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp)
    {
        m_srcPortgtWellknownNumberTcp  =  srcPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtWellknownNumberTcp() const
    {
        return (m_srcPortgtWellknownNumberTcp);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp)
    {
        m_srcPortltCustomNumberUdp  =  srcPortltCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltCustomNumberUdp() const
    {
        return (m_srcPortltCustomNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp)
    {
        m_srcPortgtCustomNumberUdp  =  srcPortgtCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtCustomNumberUdp() const
    {
        return (m_srcPortgtCustomNumberUdp);
    }


    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp)
    {
        m_srcPortltWellknownNumberUdp  =  srcPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortltWellknownNumberUdp() const
    {
        return (m_srcPortltWellknownNumberUdp);
    }


    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp)
    {
        m_srcPortgtWellknownNumberUdp  =  srcPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getSrcPortgtWellknownNumberUdp() const
    {
        return (m_srcPortgtWellknownNumberUdp);
    }





    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltNumberTcpUnion(const string &dstPortltNumberTcpUnion)
    {
        m_dstPortltNumberTcpUnion  =  dstPortltNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltNumberTcpUnion() const
    {
        return (m_dstPortltNumberTcpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtNumberTcpUnion(const string &dstPortgtNumberTcpUnion)
    {
        m_dstPortgtNumberTcpUnion  =  dstPortgtNumberTcpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtNumberTcpUnion() const
    {
        return (m_dstPortgtNumberTcpUnion);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltNumberUdpUnion(const string &dstPortltNumberUdpUnion)
    {
        m_dstPortltNumberUdpUnion  =  dstPortltNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltNumberUdpUnion() const
    {
        return (m_dstPortltNumberUdpUnion);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtNumberUdpUnion(const string &dstPortgtNumberUdpUnion)
    {
        m_dstPortgtNumberUdpUnion  =  dstPortgtNumberUdpUnion;
    }

    string  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtNumberUdpUnion() const
    {
        return (m_dstPortgtNumberUdpUnion);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp)
    {
        m_dstPortltCustomNumberTcp  =  dstPortltCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltCustomNumberTcp() const
    {
        return (m_dstPortltCustomNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp)
    {
        m_dstPortgtCustomNumberTcp  =  dstPortgtCustomNumberTcp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtCustomNumberTcp() const
    {
        return (m_dstPortgtCustomNumberTcp);
    }




    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp)
    {
        m_dstPortltWellknownNumberTcp  =  dstPortltWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltWellknownNumberTcp() const
    {
        return (m_dstPortltWellknownNumberTcp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp)
    {
        m_dstPortgtWellknownNumberTcp  =  dstPortgtWellknownNumberTcp;
    }

    PortWellknownNumberTcp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtWellknownNumberTcp() const
    {
        return (m_dstPortgtWellknownNumberTcp);
    }



    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp)
    {
        m_dstPortltCustomNumberUdp  =  dstPortltCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltCustomNumberUdp() const
    {
        return (m_dstPortltCustomNumberUdp);
    }

    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp)
    {
        m_dstPortgtCustomNumberUdp  =  dstPortgtCustomNumberUdp;
    }

    UI32  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtCustomNumberUdp() const
    {
        return (m_dstPortgtCustomNumberUdp);
    }




    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp)
    {
        m_dstPortltWellknownNumberUdp  =  dstPortltWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortltWellknownNumberUdp() const
    {
        return (m_dstPortltWellknownNumberUdp);
    }


    void  SsmLocalServiceAssocIpRuleToPolicyMessage::setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp)
    {
        m_dstPortgtWellknownNumberUdp  =  dstPortgtWellknownNumberUdp;
    }

    PortWellknownNumberUdp  SsmLocalServiceAssocIpRuleToPolicyMessage::getDstPortgtWellknownNumberUdp() const
    {
        return (m_dstPortgtWellknownNumberUdp);
    }
}

	
