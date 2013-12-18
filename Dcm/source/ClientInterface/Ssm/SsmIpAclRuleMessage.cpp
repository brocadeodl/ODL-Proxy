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

#include "ClientInterface/Ssm/SsmIpAclRuleMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"

namespace DcmNs
{

    SsmIpAclRuleMessage::SsmIpAclRuleMessage ()
        : DcmManagementInterfaceMessage (SSMIPACLRULE),
        SsmAssocIpRuleToPolicyPayloadMessage (this)
    {
    }

    SsmIpAclRuleMessage::SsmIpAclRuleMessage (
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
        const WellknownPrecedence &wellknownPrecedence, const WellknownTos &wellknownTos, const WellknownDscp 
		&wellknownDscp, const UI32 &customPrecedence, const UI32 &customTos, const UI32 &customDscp, 
		const string &precedenceUnion, const string &tosUnion, const string &dscpUnion, 
		const bool &urg, const bool &ack, const bool &push,
		const bool &fin, const bool &rst, const bool &sync, const UI32 &opcode, const UI8 &userEnteredSeqid)
		: DcmManagementInterfaceMessage (SSMIPACLRULE),
          SsmAssocIpRuleToPolicyPayloadMessage(this, policyName, policyType, ruleid, action, srcUnion,
            srcType, srcHostIpAddress, srcIpAddress, srcIpAddressMask, isCount, isLog, dstHostIpAddress,
            dstIpAddress, dstIpAddressMask, dstUnion, dstType, ProtocolTypeUnion, l3l4CustomProtoType,
            l3l4WellknownProtoType, srcPortOperator, srcPortNumberTcpUnion, srcPortltNumberTcpUnion,
            srcPortgtNumberTcpUnion, srcPortNumberUdpUnion, srcPortltNumberUdpUnion,
            srcPortgtNumberUdpUnion, srcPortrangeLowerTcpUnion, srcPortrangeLowerUdpUnion,
            srcPortrangeHigherTcpUnion, srcPortrangeHigherUdpUnion, srcPortCustomNumberTcp,
            srcPortltCustomNumberTcp, srcPortgtCustomNumberTcp,
            srcPortWellknownNumberTcp, srcPortltWellknownNumberTcp, srcPortgtWellknownNumberTcp,
            srcPortCustomNumberUdp, srcPortltCustomNumberUdp, srcPortgtCustomNumberUdp,
            srcPortWellknownNumberUdp, srcPortltWellknownNumberUdp, srcPortgtWellknownNumberUdp,
            srcPortCustomrangeLowerTcp, srcPortWellknownrangeLowerTcp,
            srcPortCustomrangeHigherTcp, srcPortWellknownrangeHigherTcp, srcPortCustomrangeLowerUdp,
            srcPortWellknownrangeLowerUdp, srcPortCustomrangeHigherUdp, srcPortWellknownrangeHigherUdp,
            dstPortNumberTcpUnion, dstPortltNumberTcpUnion, dstPortgtNumberTcpUnion,
            dstPortNumberUdpUnion, dstPortltNumberUdpUnion, dstPortgtNumberUdpUnion,
            dstPortrangeLowerTcpUnion, dstPortrangeLowerUdpUnion,
            dstPortrangeHigherTcpUnion, dstPortrangeHigherUdpUnion, dstPortOperator,
            dstPortCustomNumberTcp, dstPortltCustomNumberTcp, dstPortgtCustomNumberTcp,
            dstPortWellknownNumberTcp, dstPortltWellknownNumberTcp, dstPortgtWellknownNumberTcp,
            dstPortCustomNumberUdp, dstPortltCustomNumberUdp, dstPortgtCustomNumberUdp,
            dstPortWellknownNumberUdp, dstPortltWellknownNumberUdp, dstPortgtWellknownNumberUdp,
            dstPortCustomrangeLowerTcp, dstPortWellknownrangeLowerTcp,
            dstPortCustomrangeHigherTcp, dstPortWellknownrangeHigherTcp,
            dstPortCustomrangeLowerUdp, dstPortWellknownrangeLowerUdp, dstPortCustomrangeHigherUdp,
            dstPortWellknownrangeHigherUdp, wellknownPrecedence, wellknownTos, wellknownDscp, customPrecedence, 
			customTos, customDscp, precedenceUnion, tosUnion, dscpUnion, urg, ack, push, 
			fin, rst, sync, opcode, userEnteredSeqid)
    {
    }

    SsmIpAclRuleMessage::~SsmIpAclRuleMessage ()
    {
    }

    void  SsmIpAclRuleMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		SsmAssocIpRuleToPolicyPayloadMessage::setupAttributesForSerialization();
    }

	const void *SsmIpAclRuleMessage::getCStructureForInputs ()
	{
        ssmAcl_dcmd_l3_rule_t *pInput				= (ssmAcl_dcmd_l3_rule_t *)calloc (1, sizeof(ssmAcl_dcmd_l3_rule_t));

		string srcIpAddress							= "";
		string srcIpAddressmask						= "";
		string srcHostIpAddress						= "";
		struct in_addr src_addr;
		struct in_addr src_addr_mask;
		string dstIpAddress							= "";
		string dstIpAddressmask						= "";
		string dstHostIpAddress                     = "";
		struct in_addr dst_addr;
		struct in_addr dst_addr_mask;

		strncpy (pInput->acl_name, m_policyName.c_str (), SSMACL_NAME_SIZE);
		
		pInput->opcode = (ssmAcl_dcmd_opcode_t) m_opcode;

		if (m_userEnteredSeqid)	{
			pInput->user_entered_sequence_number 		= m_userEnteredSeqid;
			((pInput->rule).generic).seq_num 			= m_ruleid;
        }

		if (m_isCount)
			(((pInput->rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_COUNT_E;
        
		if (m_isLog)
			(((pInput->rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_LOG_E;
		
		
        (((pInput->rule).generic).results).forwarding = (ssmAcl_forwarding_result)m_action;
			
			
		if ((strcmp (m_srcUnion.c_str (), "srcType") == 0) && (m_srcType == IP_ADDR_TYPE_HOST)) {
			(pInput->rule).src_ip.valid 	= SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
			srcHostIpAddress				= m_srcHostIpAddress.toString ();
			inet_aton(srcHostIpAddress.c_str (), &(src_addr));
			((pInput->rule).src_ip).addr 	= src_addr.s_addr;
			srcIpAddressmask 				= "0.0.0.0";
			inet_aton(srcIpAddressmask.c_str (), &(src_addr_mask));
			((pInput->rule).src_ip).mask 	= src_addr_mask.s_addr;
		} else if ((strcmp (m_srcUnion.c_str (), "srcIpAddress") == 0)) {
			(pInput->rule).src_ip.valid 	= SSMACL_KEY_ELEMENT_VALID;
			srcIpAddress 					= m_srcIpAddress.toString ();
			inet_aton(srcIpAddress.c_str (), &(src_addr));
			((pInput->rule).src_ip).addr 	= src_addr.s_addr;
			srcIpAddressmask = m_srcIpAddressMask.toString ();
			inet_aton(srcIpAddressmask.c_str (), &(src_addr_mask));
			((pInput->rule).src_ip).mask 	= src_addr_mask.s_addr;
		} else if ((strcmp (m_srcUnion.c_str (), "srcType") == 0) && (m_srcType == IP_ADDR_TYPE_ANY)) {
			(pInput->rule).src_ip.valid 	= SSMACL_KEY_ELEMENT_ANY;
		}
	
		if (m_policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
			if (strcmp (m_ProtocolTypeUnion.c_str (), "l3l4CustomProtoType") == 0) {
				(pInput->rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_VALID;
				(pInput->rule).ip_protocol.value = m_l3l4CustomProtoType; 
				(pInput->rule).ip_protocol.mask	 = 0xFF;
			} else if (strcmp (m_ProtocolTypeUnion.c_str (), "l3l4WellknownProtoType") == 0) {
				if(m_l3l4WellknownProtoType != L3L4_PROTOTYPE_IP) {
					(pInput->rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_VALID;
					(pInput->rule).ip_protocol.value = m_l3l4WellknownProtoType;
					(pInput->rule).ip_protocol.mask  = 0xFF;
				} else {
					(pInput->rule).ip_protocol.valid = SSMACL_KEY_ELEMENT_ANY;
				}
			} else 
				(pInput->rule).ip_protocol.valid = 0;


			if ((strcmp (m_dstUnion.c_str (), "dstType") == 0) && (m_dstType == IP_ADDR_TYPE_HOST)) {
				(pInput->rule).dst_ip.valid 	= SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
				dstHostIpAddress 				= m_dstHostIpAddress.toString ();
				inet_aton(dstHostIpAddress.c_str (), &(dst_addr));
				((pInput->rule).dst_ip).addr 	= dst_addr.s_addr;
				dstIpAddressmask 				= "0.0.0.0";
				inet_aton(dstIpAddressmask.c_str (), &(dst_addr_mask));
				((pInput->rule).dst_ip).mask    = dst_addr_mask.s_addr;
			} else if ((strcmp (m_dstUnion.c_str (), "dstIpAddress") == 0)) {
				(pInput->rule).dst_ip.valid 	= SSMACL_KEY_ELEMENT_VALID;
				dstIpAddress 					= m_dstIpAddress.toString ();
				inet_aton(dstIpAddress.c_str (), &(dst_addr));
				((pInput->rule).dst_ip).addr 	= dst_addr.s_addr;
				dstIpAddressmask = m_dstIpAddressMask.toString ();
				inet_aton(dstIpAddressmask.c_str (), &(dst_addr_mask));
				((pInput->rule).dst_ip).mask 	= dst_addr_mask.s_addr;
			} else if ((strcmp (m_dstUnion.c_str (), "dstType") == 0) && (m_dstType == IP_ADDR_TYPE_ANY)) {
				(pInput->rule).dst_ip.valid 	= SSMACL_KEY_ELEMENT_ANY;
			}

			if (m_l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
				
				if (m_srcPortOperator) {
					((pInput->rule).src_protocol_port).valid 	= SSMACL_KEY_ELEMENT_VALID;
					((pInput->rule).src_protocol_port).oper 	= (ssmAcl_port_range_operators) m_srcPortOperator;
					if (m_srcPortOperator == PORTOPERATOR_RANGE) {
						if (strcmp (m_srcPortrangeLowerTcpUnion.c_str (), "srcPortCustomrangeLowerTcp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortCustomrangeLowerTcp;	
						else if (strcmp (m_srcPortrangeLowerTcpUnion.c_str (), "srcPortWellknownrangeLowerTcp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortWellknownrangeLowerTcp;
						if (strcmp (m_srcPortrangeHigherTcpUnion.c_str (), "srcPortCustomrangeHigherTcp") == 0)
							((pInput->rule).src_protocol_port).ending_port_number = m_srcPortCustomrangeHigherTcp;	
						else if (strcmp (m_srcPortrangeHigherTcpUnion.c_str (), "srcPortWellknownrangeHigherTcp") == 0)
							((pInput->rule).src_protocol_port).ending_port_number = m_srcPortWellknownrangeHigherTcp;
					} else {
						if (strcmp (m_srcPortNumberTcpUnion.c_str (), "srcPortWellknownNumberTcp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortWellknownNumberTcp;
						else if ((strcmp (m_srcPortNumberTcpUnion.c_str (), "srcPortCustomNumberTcp") == 0)
							&& (m_srcPortCustomNumberTcp != DEFAULT_PORTNUM ))
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortCustomNumberTcp;
						else if (strcmp (m_srcPortltNumberTcpUnion.c_str (), "srcPortltWellknownNumberTcp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortltWellknownNumberTcp;
						else if (strcmp (m_srcPortgtNumberTcpUnion.c_str (), "srcPortgtWellknownNumberTcp") == 0)
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortgtWellknownNumberTcp;
                        else if ((strcmp (m_srcPortltNumberTcpUnion.c_str (), "srcPortltCustomNumberTcp") == 0)
							&& (m_srcPortltCustomNumberTcp != DEFAULT_PORTNUM ))
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortltCustomNumberTcp;
                        else if ((strcmp (m_srcPortgtNumberTcpUnion.c_str (), "srcPortgtCustomNumberTcp") == 0)
							&& (m_srcPortgtCustomNumberTcp != DEFAULT_PORTNUM ))
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortgtCustomNumberTcp;
					}
				}


				if (m_dstPortOperator) {
					((pInput->rule).dst_protocol_port).valid 	= SSMACL_KEY_ELEMENT_VALID;
					((pInput->rule).dst_protocol_port).oper 	= (ssmAcl_port_range_operators) m_dstPortOperator;
					if (m_dstPortOperator == PORTOPERATOR_RANGE) {
						if (strcmp (m_dstPortrangeLowerTcpUnion.c_str (), "dstPortCustomrangeLowerTcp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortCustomrangeLowerTcp;
						else if (strcmp (m_dstPortrangeLowerTcpUnion.c_str (), "dstPortWellknownrangeLowerTcp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortWellknownrangeLowerTcp;
						if (strcmp (m_dstPortrangeHigherTcpUnion.c_str (), "dstPortCustomrangeHigherTcp") == 0)
							((pInput->rule).dst_protocol_port).ending_port_number = m_dstPortCustomrangeHigherTcp;
						else if (strcmp (m_dstPortrangeHigherTcpUnion.c_str (), "dstPortWellknownrangeHigherTcp") == 0)
							((pInput->rule).dst_protocol_port).ending_port_number = m_dstPortWellknownrangeHigherTcp;
					} else {
						if (strcmp (m_dstPortNumberTcpUnion.c_str (), "dstPortWellknownNumberTcp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortWellknownNumberTcp;
						else if ((strcmp (m_dstPortNumberTcpUnion.c_str (), "dstPortCustomNumberTcp") == 0)
							&& (m_dstPortCustomNumberTcp != DEFAULT_PORTNUM ))
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortCustomNumberTcp;
                        else if (strcmp (m_dstPortltNumberTcpUnion.c_str (), "dstPortltWellknownNumberTcp") == 0)
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortltWellknownNumberTcp;
                        else if (strcmp (m_dstPortgtNumberTcpUnion.c_str (), "dstPortgtWellknownNumberTcp") == 0)
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortgtWellknownNumberTcp;
                        else if ((strcmp (m_dstPortltNumberTcpUnion.c_str (), "dstPortltCustomNumberTcp") == 0)
							&& (m_dstPortltCustomNumberTcp != DEFAULT_PORTNUM ))
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortltCustomNumberTcp;
                        else if ((strcmp (m_dstPortgtNumberTcpUnion.c_str (), "dstPortgtCustomNumberTcp") == 0)
							&& (m_dstPortgtCustomNumberTcp != DEFAULT_PORTNUM ))
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortgtCustomNumberTcp;
					}
				}
			}

			if (m_l3l4WellknownProtoType == L3L4_PROTOTYPE_UDP) {
				if (m_srcPortOperator) {
					((pInput->rule).src_protocol_port).valid 	= SSMACL_KEY_ELEMENT_VALID;
					((pInput->rule).src_protocol_port).oper 	= (ssmAcl_port_range_operators) m_srcPortOperator;
					if (m_srcPortOperator == PORTOPERATOR_RANGE) {
						if (strcmp (m_srcPortrangeLowerUdpUnion.c_str (), "srcPortCustomrangeLowerUdp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortCustomrangeLowerUdp;
						else if (strcmp (m_srcPortrangeLowerUdpUnion.c_str (), "srcPortWellknownrangeLowerUdp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortWellknownrangeLowerUdp;
						if (strcmp (m_srcPortrangeHigherUdpUnion.c_str (), "srcPortCustomrangeHigherUdp") == 0)
							((pInput->rule).src_protocol_port).ending_port_number = m_srcPortCustomrangeHigherUdp;
						else if (strcmp (m_srcPortrangeHigherUdpUnion.c_str (), "srcPortWellknownrangeHigherUdp") == 0)
							((pInput->rule).src_protocol_port).ending_port_number = m_srcPortWellknownrangeHigherUdp;

					} else {
						if (strcmp (m_srcPortNumberUdpUnion.c_str (), "srcPortWellknownNumberUdp") == 0)
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortWellknownNumberUdp;
						else if ((strcmp (m_srcPortNumberUdpUnion.c_str (), "srcPortCustomNumberUdp") == 0)
							&& (m_srcPortCustomNumberUdp != DEFAULT_PORTNUM ))
							((pInput->rule).src_protocol_port).starting_port_number = m_srcPortCustomNumberUdp;
                        else if (strcmp (m_srcPortltNumberUdpUnion.c_str (), "srcPortltWellknownNumberUdp") == 0)
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortltWellknownNumberUdp;
                        else if (strcmp (m_srcPortgtNumberUdpUnion.c_str (), "srcPortgtWellknownNumberUdp") == 0)
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortgtWellknownNumberUdp;
                        else if ((strcmp (m_srcPortltNumberUdpUnion.c_str (), "srcPortltCustomNumberUdp") == 0)
							&& (m_srcPortltCustomNumberUdp != DEFAULT_PORTNUM ))
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortltCustomNumberUdp;
                        else if ((strcmp (m_srcPortgtNumberUdpUnion.c_str (), "srcPortgtCustomNumberUdp") == 0)
							&& (m_srcPortgtCustomNumberUdp != DEFAULT_PORTNUM ))
                            ((pInput->rule).src_protocol_port).starting_port_number = m_srcPortgtCustomNumberUdp;
					}
				}
			

				if (m_dstPortOperator) {
					((pInput->rule).dst_protocol_port).valid 	= SSMACL_KEY_ELEMENT_VALID;
					((pInput->rule).dst_protocol_port).oper 	= (ssmAcl_port_range_operators) m_dstPortOperator;
					if (m_dstPortOperator == PORTOPERATOR_RANGE) {
						if (strcmp (m_dstPortrangeLowerUdpUnion.c_str (), "dstPortCustomrangeLowerUdp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortCustomrangeLowerUdp;
						else if (strcmp (m_dstPortrangeLowerUdpUnion.c_str (), "dstPortWellknownrangeLowerUdp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortWellknownrangeLowerUdp;
						if (strcmp (m_dstPortrangeHigherUdpUnion.c_str (), "dstPortCustomrangeHigherUdp") == 0)
							((pInput->rule).dst_protocol_port).ending_port_number = m_dstPortCustomrangeHigherUdp;
						else if (strcmp (m_dstPortrangeHigherUdpUnion.c_str (), "dstPortWellknownrangeHigherUdp") == 0)
							((pInput->rule).dst_protocol_port).ending_port_number = m_dstPortWellknownrangeHigherUdp;
					} else {
						if (strcmp (m_dstPortNumberUdpUnion.c_str (), "dstPortWellknownNumberUdp") == 0)
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortWellknownNumberUdp;
						else if ((strcmp (m_dstPortNumberUdpUnion.c_str (), "dstPortCustomNumberUdp") == 0)
							&& (m_dstPortCustomNumberUdp != DEFAULT_PORTNUM ))
							((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortCustomNumberUdp;
                        else if (strcmp (m_dstPortltNumberUdpUnion.c_str (), "dstPortltWellknownNumberUdp") == 0)
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortltWellknownNumberUdp;
                        else if (strcmp (m_dstPortgtNumberUdpUnion.c_str (), "dstPortgtWellknownNumberUdp") == 0)
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortgtWellknownNumberUdp;
                        else if ((strcmp (m_dstPortltNumberUdpUnion.c_str (), "dstPortltCustomNumberUdp") == 0)
							&& (m_dstPortltCustomNumberUdp != DEFAULT_PORTNUM ))
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortltCustomNumberUdp;
                        else if ((strcmp (m_dstPortgtNumberUdpUnion.c_str (), "dstPortgtCustomNumberUdp") == 0)
							&& (m_dstPortgtCustomNumberUdp != DEFAULT_PORTNUM ))
                            ((pInput->rule).dst_protocol_port).starting_port_number = m_dstPortgtCustomNumberUdp;
					}
				}
			
			}
		
			if ((strcmp (m_precedenceUnion.c_str(), "customPrecedence") == 0) 
				&& (m_customPrecedence != DEFAULT_PREC_DSCP_TOS_VAL)) {
				((pInput->rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_PRECEDENCE_VALID);
				((pInput->rule).tos).value = (m_customPrecedence << 3);
				((pInput->rule).tos).mask  = 0xFF;
			} else if ((strcmp (m_dscpUnion.c_str(), "customDscp") == 0) 
				&& (m_customDscp != DEFAULT_PREC_DSCP_TOS_VAL)) {
				((pInput->rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_DSCP_VALID);
				((pInput->rule).tos).value = m_customDscp;
				((pInput->rule).tos).mask  = 0xFF;
			} else if ((strcmp (m_tosUnion.c_str(), "customTos") == 0) 
				&& (m_customTos != DEFAULT_PREC_DSCP_TOS_VAL)) {
				((pInput->rule).tos).valid  = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_TOS_VALID);
				((pInput->rule).tos).value  = m_customTos;
				((pInput->rule).tos).mask   = 0xFF;
			} else if (strcmp (m_precedenceUnion.c_str(), "wellknownPrecedence") == 0) {
				((pInput->rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_PRECEDENCE_VALID);
				((pInput->rule).tos).value = (m_wellknownPrecedence << 3);
				((pInput->rule).tos).mask  = 0xFF;
			} else if (strcmp (m_dscpUnion.c_str(), "wellknownDscp") == 0) {
				((pInput->rule).tos).valid = (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_DSCP_VALID);
				((pInput->rule).tos).value = m_wellknownDscp;
				((pInput->rule).tos).mask  = 0xFF;
			} else if (strcmp (m_tosUnion.c_str(), "wellknownTos") == 0) {
				((pInput->rule).tos).valid	= (SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_TOS_VALID);
				((pInput->rule).tos).value	= m_wellknownTos;
				((pInput->rule).tos).mask	= 0xFF;
			}
			
			if ((m_urg)|| (m_ack) ||(m_push) || (m_fin) || (m_rst) || (m_sync)) {
				((pInput->rule).tcp_flags).valid = SSMACL_KEY_ELEMENT_VALID;
				
				if (m_urg)
					((pInput->rule).tcp_flags).urg 		= SSMACL_FLAG_STATE_SET_E;
				if (m_ack)
					((pInput->rule).tcp_flags).ack 		= SSMACL_FLAG_STATE_SET_E;
				if (m_push)
					((pInput->rule).tcp_flags).push		= SSMACL_FLAG_STATE_SET_E;
				if (m_fin)
					((pInput->rule).tcp_flags).fin 		= SSMACL_FLAG_STATE_SET_E;
				if (m_rst)
					((pInput->rule).tcp_flags).rst 		= SSMACL_FLAG_STATE_SET_E;
				if (m_sync)
					((pInput->rule).tcp_flags).sync 	= SSMACL_FLAG_STATE_SET_E;
			}
		}
        
		return (pInput);
		
	}

	void SsmIpAclRuleMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (NULL != pOutputCStructure) {
            m_ruleid = *((unsigned long long *)pOutputCStructure);
        }
	}

}
