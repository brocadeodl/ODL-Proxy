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
 *   Copyright (C) 2005 - 2012 Brocade Communications Systems, Inc.         *
 *   All rights reserved.                                                   *
 *   Author : Balachandar Mani                                              *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "Ssm/Global/ExtIpAclRuleConfigurationHandler.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"
#include "Ssm/Global/SsmGlobalAssocIpRuleToPolicyMessage.h"
#include "DcmResourceIdEnums.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/Capabilities.h"

namespace DcmNs
{

ExtIpAclRuleConfigurationHandler::ExtIpAclRuleConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "extIpaclRuleCP", false)
{
}

ExtIpAclRuleConfigurationHandler::~ExtIpAclRuleConfigurationHandler  ()
{
}

string ExtIpAclRuleConfigurationHandler::getErrorString(ManagementInterfaceMessage* pMessage)
{
	string errorMsg("");

	if( pMessage->getCompletionStatus() != WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE)
	{
		vector<Attribute *> errorArguments;

		if( pMessage->getCompletionStatus() == WRC_SSM_DCM_ERR_ACL_RULE_ALREADY_EXISTS
				|| pMessage->getCompletionStatus() == WRC_SSM_DCM_ERR_ACL_RULE_SEQ_ALREADY_EXISTS )
		{
			SsmGlobalAssocIpRuleToPolicyMessage *ruleMsg = dynamic_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(pMessage);
			errorArguments.push_back(new AttributeUI64(ruleMsg->getRuleid(), "seqid"));
		}
        else if ( pMessage->getCompletionStatus() == WRC_SSM_DCM_ERR_ACL_RULE_LIMIT )
        {
            UI32 maxRules = 256;

        	switch (Capabilities::instance()->getSwModel())
        	{
				case SWBD1000:
				case SWBD1001:
				case SWBD1002:
				case SWBD1004:
				{
					maxRules = 2048;
					errorArguments.push_back(new AttributeUI32(maxRules));
					break;
				}
				case SWBD95:
				case SWBD96:
				case SWBD97:
				case SWBD107:
				case SWBD116:
				{
					maxRules = 256;
					errorArguments.push_back(new AttributeUI32(maxRules));
					break;
				}
				case SWBD131:
				case SWBD137:
				{
					maxRules = 256;
					errorArguments.push_back(new AttributeUI32(maxRules));
					break;
				}
				default:
					errorArguments.push_back(new AttributeUI32(maxRules));
					break;
        	}
        }

		errorMsg = FrameworkToolKit::localize (pMessage->getCompletionStatus(),errorArguments);
	}

	return errorMsg;
}

ManagementInterfaceMessage *ExtIpAclRuleConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();
	ObjectId parentObjectId = pWaveConfigurationSegmentCreateContext->getParentObjectId ();

	if (attributeUserTags.size () ==  attributeNames.size ())
	{
		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("ExtIpAclRuleConfigurationHandler : "
					"attributeUserTags[") + i + string("] ->") + attributeUserTags[i] + string(
							": attributeNames[") + i + string("] ->") + attributeNames[i] + string(" -> ") + attributeValues[i]);
		}

		int                             argc                                = attributeNames.size();
		UI64						    seqid 								= 0;
		UI32							action 								= SSMACL_FORWARDING_RESULT_INVALID_E;
		IpAddrType  					srcType 							= IP_ADDR_TYPE_NONE;
		ssmAcl_dcmd_opcode_t 			opcode;
		UI32 							srcport_num 						= DEFAULT_PORTNUM;
		UI32 							dstport_num 						= DEFAULT_PORTNUM;
		UI32 							dstport_num_low 					= DEFAULT_PORTNUM;
		UI32 							srcport_num_low 					= DEFAULT_PORTNUM;
		UI32 							srcport_num_high 					= DEFAULT_PORTNUM;
		UI32 							dstport_num_high 					= DEFAULT_PORTNUM;
		IpV4Address 					srcIpAddress("0.0.0.0");
		IpV4Address 					srcHostIpAddress("0.0.0.0");
		IpV4Address 					srcIpAddressMask("0.0.0.0");
		string 							policyName;
		string 							policyType;
		string 							srcUnion 							= "";
		IpAddrType  					dstType 							= IP_ADDR_TYPE_NONE;

		string 							dstUnion 							= "";
		string 							ProtocolTypeUnion 					= "";
		string 							srcPortNumberTcpUnion 				= "";
		string 							srcPortNumberUdpUnion 				= "";
		string 							dstPortNumberTcpUnion 				= "";
		string 							dstPortNumberUdpUnion 				= "";
		string 							srcPortrangeLowerTcpUnion 			= "";
		string 							dstPortrangeLowerTcpUnion 			= "";
		string 							srcPortrangeHigherTcpUnion 			= "";
		string 							dstPortrangeHigherTcpUnion 			= "";

		string 							srcPortrangeLowerUdpUnion 			= "";
		string 							dstPortrangeLowerUdpUnion 			= "";
		string 							srcPortrangeHigherUdpUnion 			= "";
		string 							dstPortrangeHigherUdpUnion 			= "";

		string 							srcPortltNumberTcpUnion 			= "";
		string 							srcPortgtNumberTcpUnion 			= "";
		string 							srcPortltNumberUdpUnion 			= "";
		string 							srcPortgtNumberUdpUnion 			= "";
		string 							dstPortltNumberTcpUnion 			= "";
		string 							dstPortgtNumberTcpUnion 			= "";
		string 							dstPortltNumberUdpUnion 			= "";
		string 							dstPortgtNumberUdpUnion 			= "";

		UI32        					srcPortltCustomNumberTcp 			= DEFAULT_PORTNUM;
		UI32        					srcPortgtCustomNumberTcp 			= DEFAULT_PORTNUM;
		UI32        					srcPortltCustomNumberUdp 			= DEFAULT_PORTNUM;
		UI32        					srcPortgtCustomNumberUdp 			= DEFAULT_PORTNUM;
		UI32        					dstPortltCustomNumberTcp 			= DEFAULT_PORTNUM;
		UI32        					dstPortgtCustomNumberTcp 			= DEFAULT_PORTNUM;
		UI32        					dstPortltCustomNumberUdp 			= DEFAULT_PORTNUM;
		UI32        					dstPortgtCustomNumberUdp 			= DEFAULT_PORTNUM;
		IpV4Address 					dstHostIpAddress("0.0.0.0");
		IpV4Address 					dstIpAddress("0.0.0.0");
		IpV4Address 					dstIpAddressMask("0.0.0.0");
		UI32 							l3l4CustomProtoType 				= 0;
		L3L4wellknownProtoType  		l3l4WellknownProtoType 				= L3L4_PROTOTYPE_UNKNOWN;
		PortOperator 					srcPortOperator 					= PORTOPERATOR_UNKNOWN;
		PortOperator 					dstPortOperator 					= PORTOPERATOR_UNKNOWN;
		bool							urg 								= 0;
		bool							ack 								= 0;
		bool							push								= 0;
		bool							fin 								= 0;
		bool							rst 								= 0;
		bool							sync 								= 0;
		string 							precedenceUnion 			        = "";
		string 							dscpUnion			                = "";
		string 							tosUnion			                = "";
		WellknownPrecedence     		wellknownPrecedence 	            = WELLKNOWN_PRECEDENCE_UNKNOWN;
		WellknownDscp           		wellknownDscp           	        = WELLKNOWN_DSCP_UNKNOWN;
		WellknownTos            		wellknownTos                	    = WELLKNOWN_TOS_UNKNOWN;
		UI32                    		customPrecedence                	= DEFAULT_PREC_DSCP_TOS_VAL;
		UI32                    		customTos                       	= DEFAULT_PREC_DSCP_TOS_VAL;
		UI32                    		customDscp                      	= DEFAULT_PREC_DSCP_TOS_VAL;
		UI32							precedence 							= DEFAULT_PREC_DSCP_TOS_VAL;
		UI32							tos 								= DEFAULT_PREC_DSCP_TOS_VAL;
		UI32							dscp 								= DEFAULT_PREC_DSCP_TOS_VAL;
		int								PrecDscpTosFlag 					= 1;
		UI32							srcPortCustomNumberTcp 				= DEFAULT_PORTNUM;
		UI32							srcPortCustomNumberUdp 				= DEFAULT_PORTNUM;
		UI32							srcPortCustomrangeLowerTcp 			= DEFAULT_PORTNUM;
		UI32							srcPortCustomrangeHigherTcp 		= DEFAULT_PORTNUM;
		UI32							srcPortCustomrangeLowerUdp 			= DEFAULT_PORTNUM;
		UI32							srcPortCustomrangeHigherUdp 		= DEFAULT_PORTNUM;

		UI32							dstPortCustomNumberTcp 				= DEFAULT_PORTNUM;
		UI32							dstPortCustomNumberUdp 				= DEFAULT_PORTNUM;
		UI32							dstPortCustomrangeLowerTcp 			= DEFAULT_PORTNUM;
		UI32							dstPortCustomrangeHigherTcp 		= DEFAULT_PORTNUM;
		UI32							dstPortCustomrangeLowerUdp 			= DEFAULT_PORTNUM;
		UI32							dstPortCustomrangeHigherUdp 		= DEFAULT_PORTNUM;
		UI32 							dport6 								= 0;
		string 							srcport 							= "";
		string 							srcportlow 							= "";
		string 							srcporthigh 						= "";
		UI32 							srcport_num_well 					= DEFAULT_PORTNUM;
		UI32 							srcport_num_low_well 				= DEFAULT_PORTNUM;
		UI32 							srcport_num_high_well 				= DEFAULT_PORTNUM;
		string 							dstport 							= "";
		string 							dstportlow 							= "";
		string 							dstporthigh 						= "";
		UI32 							dstport_num_well 					= DEFAULT_PORTNUM;
		UI32 							dstport_num_low_well 				= DEFAULT_PORTNUM;
		UI32	 						dstport_num_high_well 				= DEFAULT_PORTNUM;

		PortWellknownNumberTcp  		srcPortWellknownNumberTcp 			= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		srcPortWellknownNumberUdp 			= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberTcp  		srcPortWellknownrangeLowerTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		srcPortWellknownrangeLowerUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberTcp  		srcPortWellknownrangeHigherTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		srcPortWellknownrangeHigherUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;

		PortWellknownNumberTcp  		dstPortWellknownNumberTcp 			= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		dstPortWellknownNumberUdp 			= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberTcp  		dstPortWellknownrangeLowerTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		dstPortWellknownrangeLowerUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberTcp  		dstPortWellknownrangeHigherTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		dstPortWellknownrangeHigherUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;

		PortWellknownNumberTcp  		srcPortltWellknownNumberTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberTcp  		srcPortgtWellknownNumberTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		srcPortltWellknownNumberUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberUdp  		srcPortgtWellknownNumberUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberTcp  		dstPortltWellknownNumberTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberTcp  		dstPortgtWellknownNumberTcp 		= WELLKNOWNPORT_TCP_UNKNOWN;
		PortWellknownNumberUdp  		dstPortltWellknownNumberUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;
		PortWellknownNumberUdp  		dstPortgtWellknownNumberUdp 		= WELLKNOWNPORT_UDP_UNKNOWN;

		UI32 							ptype								= SSMACL_TYPE_L3_IPV4_EXT_E;
		UI32 							proto;
		bool 							iscount 							= 0;
		bool 							islog 								= 0;
		bool 							is_eq_gt_lt_neq 					= 0;
		bool 							is_range 							= 0;
        /* Following flags are for error condition handling */
        bool 		                    srcHostIpAddressflag            	= 0;
        bool        		            dstHostIpAddressflag            	= 0;
        bool                		    srcIpAddressMaskflag            	= 0;
        bool            		        dstIpAddressMaskflag            	= 0;
        bool                    		srcIpAddressSlashflag           	= 0;
        bool                    		dstIpAddressSlashflag           	= 0;

		SsmGlobalAssocIpRuleToPolicyMessage *pmsg = new SsmGlobalAssocIpRuleToPolicyMessage();

		if ( attributeNames.size() < 6 && pmsg != NULL )
		{
			pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_INCOMPLETE_COMMAND);
			return pmsg;
		}

		if (attributeNames[0] == "policyName")
		{
			policyName = attributeValues[0];
		}

		if (attributeNames[1] == "ruleid")
		{
			seqid = strtoll(attributeValues[1].c_str (), NULL, 10);
		}

		opcode = SSMACL_DCMD_ADD_RULE;

		/*permit or deny or hard-drop*/
		if (strncasecmp(attributeNames[2].c_str(),"permit",6) == 0)
			action = SSMACL_FORWARDING_RESULT_PERMIT_E;
		else if (strncasecmp(attributeNames[2].c_str(),"deny",4) == 0)
			action = SSMACL_FORWARDING_RESULT_SOFT_DROP_E;
		else if (strncasecmp(attributeNames[2].c_str(),"hard-drop", 9) == 0)
			action = SSMACL_FORWARDING_RESULT_HARD_DROP_E;

		char* strArgv3 = NULL;

		if(strcmp(attributeNames[3].c_str(),"6") == 0){
			strArgv3 = (char *)"tcp";
		} else if(strcmp(attributeNames[3].c_str(),"17") == 0) {
			strArgv3 = (char *)"udp";
		} else {
			strArgv3 = const_cast<char *>(attributeNames[3].c_str());
		}

		/*Finding Ip-protocol*/
		if (strncasecmp(strArgv3,"udp" ,3) == 0)
		{
			proto = L3L4_PROTOTYPE_UDP;
			l3l4WellknownProtoType = L3L4_PROTOTYPE_UDP;
			ProtocolTypeUnion = "l3l4WellknownProtoType";
		}
		else if (strncasecmp(strArgv3,"tcp" ,3) == 0)
		{
			proto = L3L4_PROTOTYPE_TCP;
			l3l4WellknownProtoType = L3L4_PROTOTYPE_TCP;
			ProtocolTypeUnion = "l3l4WellknownProtoType";
		}
		else if (strncasecmp(strArgv3,"ip" ,2) == 0)
		{
			proto = L3L4_PROTOTYPE_IP;
			l3l4WellknownProtoType = L3L4_PROTOTYPE_IP;
			ProtocolTypeUnion = "l3l4WellknownProtoType";
		}
		else if (strncasecmp(strArgv3,"icmp" ,4) ==0)
		{
			proto = L3L4_PROTOTYPE_ICMP;
			l3l4WellknownProtoType = L3L4_PROTOTYPE_ICMP;
			ProtocolTypeUnion = "l3l4WellknownProtoType";
		}
		else
		{
			string custom_prot_num = strArgv3;
			l3l4CustomProtoType = atoi(custom_prot_num.c_str());
			if (l3l4CustomProtoType == L3L4_PROTOTYPE_ICMP) {
				proto = L3L4_PROTOTYPE_ICMP;
				l3l4WellknownProtoType = L3L4_PROTOTYPE_ICMP;
				ProtocolTypeUnion = "l3l4WellknownProtoType";
			} else if (l3l4CustomProtoType == L3L4_PROTOTYPE_IP) {
				proto = L3L4_PROTOTYPE_IP;
				l3l4WellknownProtoType = L3L4_PROTOTYPE_IP;
				ProtocolTypeUnion = "l3l4WellknownProtoType";
			} else if (l3l4CustomProtoType == L3L4_PROTOTYPE_UDP) {
				proto = L3L4_PROTOTYPE_UDP;
				l3l4WellknownProtoType = L3L4_PROTOTYPE_UDP;
				ProtocolTypeUnion = "l3l4WellknownProtoType";
			} else if (l3l4CustomProtoType == L3L4_PROTOTYPE_TCP) {
				proto = L3L4_PROTOTYPE_TCP;
				l3l4WellknownProtoType = L3L4_PROTOTYPE_TCP;
				ProtocolTypeUnion = "l3l4WellknownProtoType";
			} else {
				proto = L3L4_PROTOTYPE_UNKNOWN;
				ProtocolTypeUnion = "l3l4CustomProtoType";
			}
		}

		/*Finding srcType,host SIP ,Source mask*/
		if (strncasecmp(attributeNames[4].c_str(),"any",3) == 0)
		{
			srcType = IP_ADDR_TYPE_ANY;
			srcUnion = "srcType";
		}
		else if (strncasecmp(attributeNames[4].c_str(),"host",4) == 0)
		{
			srcHostIpAddress.fromString (attributeNames[5]);
			srcType 				= IP_ADDR_TYPE_HOST;
			srcUnion 				= "srcType";
			srcHostIpAddressflag 	= 1;
		}
		else if ((srcType != IP_ADDR_TYPE_ANY) || (srcType != IP_ADDR_TYPE_HOST))
		{
			string srcIpAddressHold = attributeNames[4];
			int position = srcIpAddressHold.find("/");
			if (position >= 0)
			{
				getSrcdstIpAddressandSrcdstIpMaskAddress(srcIpAddressHold, srcIpAddressMask,
					srcIpAddress, srcIpAddressSlashflag, position);
			}
			else if (argc >= 6)
			{
				srcIpAddress.fromString (attributeNames[4]);
				srcIpAddressMask.fromString (attributeNames[5]);
				srcIpAddressMaskflag 	= 1;
			}
			srcType 				= IP_ADDR_TYPE_IP_MASK;
			srcUnion 				= "srcIpAddress";
		}

		/*Setting precedence,dscp,tos,count ,log ,ack,fin,rst,sync */
		for (int i = 5; i <argc ; i++)
		{
			if (strncasecmp(attributeNames[i].c_str(),"precedence",10)== 0)
			{
				 string prec_dscp_tos = attributeNames[i+1].c_str();
				 precedence = atoi(prec_dscp_tos.c_str());
				 if ((IsWellknownPrecedence(prec_dscp_tos.c_str(),precedence,PrecDscpTosFlag) == WELLKNOWN_PRECEDENCE_UNKNOWN)
					 && (PrecDscpTosFlag == 0))
				 {
					 customPrecedence = precedence;
					 precedenceUnion = "customPrecedence";
				 }
				 else
				 {
					 wellknownPrecedence = (WellknownPrecedence)(IsWellknownPrecedence(prec_dscp_tos.c_str(),
							 precedence,PrecDscpTosFlag));
					 precedenceUnion = "wellknownPrecedence";
				 }
			}
			if (strncasecmp(attributeNames[i].c_str(),"tos",3)== 0)
			{
				 string prec_dscp_tos = attributeNames[i+1];
				 tos = atoi(prec_dscp_tos.c_str());

				 if ((IsWellknownTos(prec_dscp_tos.c_str(),tos,PrecDscpTosFlag) == WELLKNOWN_TOS_UNKNOWN)
					 && (PrecDscpTosFlag == 0))
					 {
					 customTos = tos;
					 tosUnion = "customTos";
				 }
				 else
				 {
					 wellknownTos = (WellknownTos)(IsWellknownTos(prec_dscp_tos.c_str(),tos,PrecDscpTosFlag));
					 tosUnion = "wellknownTos";
				 }
			}
			if (strncasecmp(attributeNames[i].c_str(),"dscp",4) == 0)
			{
				 string prec_dscp_tos = attributeNames[i+1];
				 dscp = atoi(prec_dscp_tos.c_str());
				 if ((IsWellknownDscp(prec_dscp_tos.c_str(),dscp,PrecDscpTosFlag) == WELLKNOWN_DSCP_UNKNOWN)
					 && (PrecDscpTosFlag == 0))
				 {
					 customDscp = dscp;
					 dscpUnion = "customDscp";
				 }
				 else
				 {
					 wellknownDscp = (WellknownDscp)(IsWellknownDscp(prec_dscp_tos.c_str(),dscp,PrecDscpTosFlag));
					 dscpUnion = "wellknownDscp" ;
				 }
			}
			if (strncasecmp(attributeNames[i].c_str(),"count",5) == 0)
				 iscount = 1;
			if (strncasecmp(attributeNames[i].c_str(),"log",3) == 0)
				 islog = 1;
			if (strncasecmp(attributeNames[i].c_str(),"urg" ,3) ==0)
				 urg = 1;
			if (strncasecmp(attributeNames[i].c_str(),"ack" ,3) ==0)
				 ack = 1;
			if (strncasecmp(attributeNames[i].c_str(),"push" ,4) ==0)
				 push = 1;
			if (strncasecmp(attributeNames[i].c_str(),"fin" ,3) ==0)
				fin = 1;
			if (strncasecmp(attributeNames[i].c_str(),"rst" ,3) ==0)
				rst = 1;
			if (strncasecmp(attributeNames[i].c_str(),"sync" ,4) ==0)
				sync = 1;
		}

		switch(proto)
		{
			case L3L4_PROTOTYPE_ICMP:
			case L3L4_PROTOTYPE_IP:
			case L3L4_PROTOTYPE_UNKNOWN:
			{
				/*Setting destination parameters*/
				if (((argc >= 6) && (strncasecmp(attributeNames[5].c_str(),"any",3) == 0) && ((srcType == IP_ADDR_TYPE_ANY)
							|| ((srcType == IP_ADDR_TYPE_IP_MASK) && (srcIpAddressSlashflag))))
					|| ((argc >= 7) && (strncasecmp(attributeNames[6].c_str(),"any",3) == 0) &&
						((srcType == IP_ADDR_TYPE_HOST) || ((srcType == IP_ADDR_TYPE_IP_MASK)
															&& (!srcIpAddressSlashflag)))))
				{
					dstType = IP_ADDR_TYPE_ANY;
					dstUnion = "dstType";
				}
				else if (((argc >= 6) && (strncasecmp(attributeNames[5].c_str(), "host", 4) == 0) &&
						((srcType == IP_ADDR_TYPE_ANY) || ((srcType == IP_ADDR_TYPE_IP_MASK)
														   &&(srcIpAddressSlashflag))))
					|| ( (argc >= 7) && (strncasecmp(attributeNames[6].c_str(), "host", 4) == 0) &&
						((srcType == IP_ADDR_TYPE_HOST) || ((srcType == IP_ADDR_TYPE_IP_MASK)
															&& (!srcIpAddressSlashflag)))))
				{
					if ((srcType == IP_ADDR_TYPE_ANY) || ((srcType == IP_ADDR_TYPE_IP_MASK)
														  &&(srcIpAddressSlashflag)))
						dstHostIpAddress.fromString (attributeNames[6]);
					else
						dstHostIpAddress.fromString (attributeNames[7]);

					dstType 				= IP_ADDR_TYPE_HOST;
					dstHostIpAddressflag 	= 1;
					dstUnion 				= "dstType";
				}
				else if (dstType == IP_ADDR_TYPE_NONE)
				{
					if ((srcType == IP_ADDR_TYPE_ANY) ||
							((srcType == IP_ADDR_TYPE_IP_MASK) && (srcIpAddressSlashflag)))
					{
						string dstIpAddressHold = attributeNames[5];
						int position = dstIpAddressHold.find("/");
						if (position >= 0) {
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
							dstIpAddressSlashflag   = 1;
						}
						else
						{
							dstIpAddress.fromString (attributeNames[5]);
							dstIpAddressMask.fromString (attributeNames[6]);
							dstIpAddressMaskflag 	= 1 ;
						}
					}
					else if (((srcType == IP_ADDR_TYPE_HOST) || (srcType == IP_ADDR_TYPE_IP_MASK
								&& (!srcIpAddressSlashflag))) && (argc >= 7))
					{
						string dstIpAddressHold = attributeNames[6];
                        int position = dstIpAddressHold.find("/");
                        if (position >= 0)
						{
                        	getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
                        }
						else
						{
							dstIpAddress.fromString (attributeNames[6]);
							dstIpAddressMask.fromString (attributeNames[7]);
							dstIpAddressMaskflag	= 1 ;
						}
					}
					dstType = IP_ADDR_TYPE_IP_MASK;
					dstUnion = "dstIpAddress";
				}
				break;
			}
			case L3L4_PROTOTYPE_UDP:
			case L3L4_PROTOTYPE_TCP:
			{
				/*Setting sportoperator for udp-tcp*/
				int max_iter;

				if ((srcType == IP_ADDR_TYPE_ANY) ||
						((srcType == IP_ADDR_TYPE_IP_MASK) &&(srcIpAddressSlashflag)))
					max_iter = 6;
				else
					max_iter = 7;

   				for (int dst_iter = 5;((dst_iter < max_iter) && (dst_iter < argc)); dst_iter++)
		   		{
					/* Fix Me : Not sure purpose of below condition */
					if ((dst_iter == 6) && (dstType!= IP_ADDR_TYPE_NONE))
						continue;

					if ((argc >= (dst_iter+2)) && (strncasecmp(attributeNames[dst_iter].c_str(),"eq" ,2) ==0))
   					{
		   				srcport= attributeNames[dst_iter+1]; /*String*/
				   		srcport_num = atoi(srcport.c_str()); /*integer*/
						is_eq_gt_lt_neq = 1;
   						srcPortOperator = PORTOPERATOR_EQ;
		   			}
					else if ((argc >= (dst_iter+2)) && (strncasecmp(attributeNames[dst_iter].c_str(),"gt" ,2) == 0))
   					{
		   				srcport= attributeNames[dst_iter+1];
				   		srcport_num = atoi(srcport.c_str());
						is_eq_gt_lt_neq = 1;
   						srcPortOperator = PORTOPERATOR_GT;
		   			}
					else if ((argc >= (dst_iter+2)) && (strncasecmp(attributeNames[dst_iter].c_str(),"lt" ,2) == 0))
   					{
		   				srcport= attributeNames[dst_iter+1];
				   		srcport_num = atoi(srcport.c_str());
						is_eq_gt_lt_neq = 1;
   						srcPortOperator = PORTOPERATOR_LT;
		   			}
					else if ((argc >= (dst_iter+2)) && (strncasecmp(attributeNames[dst_iter].c_str(),"neq" ,3) == 0))
   					{
		   				srcport = attributeNames[dst_iter+1];
				   		srcport_num = atoi(srcport.c_str());
						is_eq_gt_lt_neq = 1;
   						srcPortOperator = PORTOPERATOR_NEQ;
		   			}
					else if ((argc >= (dst_iter+3)) && (strncasecmp(attributeNames[dst_iter].c_str(),"range" ,5) == 0))
					{
  						srcportlow = attributeNames[dst_iter+1];
						srcporthigh = attributeNames[dst_iter+2];
						srcport_num_low = atoi(srcportlow.c_str());
						srcport_num_high = atoi(srcporthigh.c_str());
						is_range = 1;
						srcPortOperator = PORTOPERATOR_RANGE;
					}
				}
				/*Setting sportoperator for udp-tcp completed*/

				/*Setting sport other parameters for tcp-udp*/
				if (srcPortOperator != PORTOPERATOR_UNKNOWN)
				{
					if (strncasecmp(strArgv3,"tcp" ,3) == 0)
					{
						srcport_num_well = StringtoWellknownNumberTcp(srcport.c_str());
						srcport_num_low_well = StringtoWellknownNumberTcp(srcportlow.c_str());
						srcport_num_high_well = StringtoWellknownNumberTcp(srcporthigh.c_str());
					}
					else
					{
						srcport_num_well = StringtoWellknownNumberUdp(srcport.c_str());
						srcport_num_low_well = StringtoWellknownNumberUdp(srcportlow.c_str());
						srcport_num_high_well = StringtoWellknownNumberUdp(srcporthigh.c_str());
					}

					if ((srcport_num_well == WELLKNOWNPORT_TCP_UNKNOWN) && (strncasecmp(strArgv3,"tcp" ,3) == 0))
						srcport_num_well = NumbertoWellknownNumberTcp(srcport_num);
					else if ((srcport_num_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
						srcport_num_well = NumbertoWellknownNumberUdp(srcport_num);

					if ((srcport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN)	&& (strncasecmp(strArgv3,"tcp" ,3) == 0))
						srcport_num_low_well = NumbertoWellknownNumberTcp(srcport_num_low);
					else if ((srcport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
						srcport_num_low_well = NumbertoWellknownNumberUdp(srcport_num_low);

					if ((srcport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN)
						&& (strncasecmp(strArgv3,"tcp" ,3) == 0))
						srcport_num_high_well = NumbertoWellknownNumberTcp(srcport_num_high);
					else if ((srcport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
						srcport_num_high_well = NumbertoWellknownNumberUdp(srcport_num_high);

					if ((srcPortOperator == PORTOPERATOR_RANGE) &&
						(((srcport_num_low_well >= srcport_num_high_well) &&
							((srcport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_low_well != WELLKNOWNPORT_UDP_UNKNOWN))&&
							((srcport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_high_well != WELLKNOWNPORT_UDP_UNKNOWN)))

						||((srcport_num_low >= srcport_num_high) &&
							((srcport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN))&&
							((srcport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN)))

						||((srcport_num_low_well >= srcport_num_high) &&
							((srcport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_low_well != WELLKNOWNPORT_UDP_UNKNOWN))&&
							((srcport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN)))

						||((srcport_num_low >= srcport_num_high_well) &&
							((srcport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN))&&
							((srcport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN)
							 ||(srcport_num_high_well !=WELLKNOWNPORT_UDP_UNKNOWN)))))
					{
						pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_ACL_PORT_RANGE_ERROR);
						return pmsg;
					}

					if ((srcport_num_well != WELLKNOWNPORT_TCP_UNKNOWN) &&	(strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					{
						if (srcPortOperator == PORTOPERATOR_LT)
						{
							srcPortltNumberTcpUnion = "srcPortltWellknownNumberTcp";
							srcPortltWellknownNumberTcp = (PortWellknownNumberTcp)srcport_num_well;
						}
						else if (srcPortOperator == PORTOPERATOR_GT)
						{
							srcPortgtNumberTcpUnion = "srcPortgtWellknownNumberTcp";
							srcPortgtWellknownNumberTcp = (PortWellknownNumberTcp)srcport_num_well;
						}
						else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ))
						{
							srcPortNumberTcpUnion = "srcPortWellknownNumberTcp";
							srcPortWellknownNumberTcp = (PortWellknownNumberTcp)srcport_num_well;
						}
					}

					if ((srcport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)	&& (strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					{
						srcPortrangeLowerTcpUnion = "srcPortWellknownrangeLowerTcp";
						srcPortWellknownrangeLowerTcp = (PortWellknownNumberTcp)srcport_num_low_well;
					}

					if ((srcport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN) && (strncasecmp(strArgv3,"tcp" ,3) == 0))
					{
						srcPortrangeHigherTcpUnion = "srcPortWellknownrangeHigherTcp";
						srcPortWellknownrangeHigherTcp = (PortWellknownNumberTcp)srcport_num_high_well;
					}

					if ((srcport_num_well != WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
                        if (srcPortOperator == PORTOPERATOR_LT)
                        {
                            srcPortltNumberUdpUnion = "srcPortltWellknownNumberUdp";
                            srcPortltWellknownNumberUdp = (PortWellknownNumberUdp)srcport_num_well;
                        }
                        else if (srcPortOperator == PORTOPERATOR_GT)
                        {
                            srcPortgtNumberUdpUnion = "srcPortgtWellknownNumberUdp";
                            srcPortgtWellknownNumberUdp = (PortWellknownNumberUdp)srcport_num_well;
                        }
                        else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ))
                        {
							srcPortNumberUdpUnion = "srcPortWellknownNumberUdp";
							srcPortWellknownNumberUdp = (PortWellknownNumberUdp)srcport_num_well;
						}
					}

					if ((srcport_num_low_well != WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
						srcPortrangeLowerUdpUnion = "srcPortWellknownrangeLowerUdp";
						srcPortWellknownrangeLowerUdp = (PortWellknownNumberUdp)srcport_num_low_well;
					}

					if ((srcport_num_high_well != WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
						srcPortrangeHigherUdpUnion = "srcPortWellknownrangeHigherUdp";
						srcPortWellknownrangeHigherUdp = (PortWellknownNumberUdp)srcport_num_high_well;
					}

					if (strncasecmp(strArgv3,"tcp" ,3) == 0 )						{
                        if ((srcPortOperator == PORTOPERATOR_LT) && (srcPortltWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
                            srcPortltNumberTcpUnion = "srcPortltCustomNumberTcp";
                            srcPortltCustomNumberTcp = srcport_num;
                        }
                        else if ((srcPortOperator == PORTOPERATOR_GT) && (srcPortgtWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
                            srcPortgtNumberTcpUnion = "srcPortgtCustomNumberTcp";
                            srcPortgtCustomNumberTcp = srcport_num;
                        }
                        else if (( (srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) &&
							(srcPortWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
							srcPortNumberTcpUnion = "srcPortCustomNumberTcp";
							srcPortCustomNumberTcp = srcport_num;
						}
					}

					if (strncasecmp(strArgv3,"udp" ,3) == 0)
					{
                        if ((srcPortOperator == PORTOPERATOR_LT) && (srcPortltWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
                            srcPortltNumberUdpUnion = "srcPortltCustomNumberUdp";
                            srcPortltCustomNumberUdp = srcport_num;
                        }
                        else if ((srcPortOperator == PORTOPERATOR_GT)&& (srcPortgtWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
                            srcPortgtNumberUdpUnion = "srcPortgtCustomNumberUdp";
                            srcPortgtCustomNumberUdp = srcport_num;
                        }
                        else if (( (srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) &&
							(srcPortWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
							srcPortNumberUdpUnion = "srcPortCustomNumberUdp";
							srcPortCustomNumberUdp = srcport_num;
						}
					}

					if ((strncasecmp(strArgv3,"tcp" ,3) == 0 ) && (srcPortWellknownrangeLowerTcp == WELLKNOWNPORT_TCP_UNKNOWN))
					{
						srcPortrangeLowerTcpUnion = "srcPortCustomrangeLowerTcp";
						srcPortCustomrangeLowerTcp = srcport_num_low;
					}

					if ((strncasecmp(strArgv3,"tcp" ,3) == 0 ) &&
						(srcPortWellknownrangeHigherTcp == WELLKNOWNPORT_TCP_UNKNOWN))
					{
						srcPortrangeHigherTcpUnion = "srcPortCustomrangeHigherTcp";
						srcPortCustomrangeHigherTcp = srcport_num_high;
					}

					if ((strncasecmp(strArgv3,"udp" ,3) == 0) &&
						(srcPortWellknownrangeLowerUdp  == WELLKNOWNPORT_UDP_UNKNOWN))
					{
						srcPortrangeLowerUdpUnion = "srcPortCustomrangeLowerUdp";
						srcPortCustomrangeLowerUdp = srcport_num_low;
					}

					if ((strncasecmp(strArgv3,"udp" ,3) == 0) &&
						(srcPortWellknownrangeHigherUdp == WELLKNOWNPORT_UDP_UNKNOWN))
					{
						srcPortrangeHigherUdpUnion = "srcPortCustomrangeHigherUdp";
						srcPortCustomrangeHigherUdp = srcport_num_high;
					}
				}
				/*Finished Setting sport other parameters for tcp-udp*/

				/*Setting destination parameters*/

				if (((srcType == IP_ADDR_TYPE_ANY) ||
					((srcType == IP_ADDR_TYPE_IP_MASK) &&(srcIpAddressSlashflag)))&& (!is_range)&& (!is_eq_gt_lt_neq))
				{
					if ((argc >= 6) && (strncasecmp(attributeNames[5].c_str(),"any",3) == 0))
					{
						dstType = IP_ADDR_TYPE_ANY;
						dstUnion = "dstType";

					}
					else if ((argc >= 7) && (strncasecmp(attributeNames[5].c_str(),"host",4) == 0))
					{
						dstHostIpAddress.fromString (attributeNames[6]);
						dstType 				= IP_ADDR_TYPE_HOST;
						dstUnion	 			= "dstType";
						dstHostIpAddressflag 	= 1;
					}
					else if ((argc >= 6) && ((dstType != IP_ADDR_TYPE_ANY)||(dstType != IP_ADDR_TYPE_HOST)))
					{
						/* Fix Me : Add logic here */
						string dstIpAddressHold = attributeNames[5];
						int position = dstIpAddressHold.find("/");
						if (position >= 0)
						{
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
						}
						else
						{
							dstIpAddress.fromString (attributeNames[5]);
							dstIpAddressMask.fromString (attributeNames[6]);
							dstIpAddressMaskflag		= 1;
						}

						dstType                 = IP_ADDR_TYPE_IP_MASK;
						dstUnion                = "dstIpAddress";
					}
				}
				else if (((srcType == IP_ADDR_TYPE_HOST)||
					((srcType == IP_ADDR_TYPE_IP_MASK) && (!srcIpAddressSlashflag))) && (!is_range) && (!is_eq_gt_lt_neq))
				{

					if (argc == 6)
					{
						pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_INCOMPLETE_COMMAND);
						return pmsg;
					}

					if ((argc >= 7) && (strncasecmp(attributeNames[6].c_str(),"any",3) == 0))
					{
						dstType = IP_ADDR_TYPE_ANY;
						dstUnion = "dstType";
					}
					else if ((argc >= 8) && (strncasecmp(attributeNames[6].c_str(),"host",4) == 0))
					{
						dstHostIpAddress.fromString (attributeNames[7]);
						dstType 				= IP_ADDR_TYPE_HOST;
						dstHostIpAddressflag	= 1;
						dstUnion 				= "dstType";
					}
					else if ((argc >= 7) && ((dstType != IP_ADDR_TYPE_ANY)||(dstType != IP_ADDR_TYPE_HOST)))
					{
						string dstIpAddressHold = attributeNames[6];
						int position = dstIpAddressHold.find("/");
						if (position >= 0)
						{
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
						}
						else
						{
							dstIpAddress.fromString (attributeNames[6]);
							dstIpAddressMask.fromString (attributeNames[7]);
							dstIpAddressMaskflag		= 1;
						}
						dstType 					= IP_ADDR_TYPE_IP_MASK;
						dstUnion 					= "dstIpAddress";
					}
				}
				else if ((((srcType == IP_ADDR_TYPE_ANY) ||
					((srcType == IP_ADDR_TYPE_IP_MASK) &&(srcIpAddressSlashflag))) && (is_eq_gt_lt_neq)))
				{
					if ((argc >= 8) && ((strncasecmp(attributeNames[7].c_str(),"any",3) == 0)))
					{
						dstType = IP_ADDR_TYPE_ANY;
						dstUnion = "dstType";
					}
					else if ((argc >= 9) && (strncasecmp(attributeNames[7].c_str(),"host",4) == 0))
					{
						dstHostIpAddress.fromString (attributeNames[8]);
						dstType 				= IP_ADDR_TYPE_HOST;
						dstHostIpAddressflag	= 1;
						dstUnion 				= "dstType";
					}
					else if ((argc >= 8) && ((dstType != IP_ADDR_TYPE_ANY)||(dstType != IP_ADDR_TYPE_HOST)))
					{
						string dstIpAddressHold = attributeNames[7];
						int position = dstIpAddressHold.find("/");
						if (position >= 0)
						{
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
						}
						else
						{
							dstIpAddress.fromString (attributeNames[7]);
							dstIpAddressMask.fromString (attributeNames[8]);
							dstIpAddressMaskflag		= 1;
						}
						dstType                 = IP_ADDR_TYPE_IP_MASK;
						dstUnion                = "dstIpAddress";
					}
				}
				else if (((((srcType == IP_ADDR_TYPE_HOST)
								|| ((srcType == IP_ADDR_TYPE_IP_MASK) && (!srcIpAddressSlashflag))) && (is_eq_gt_lt_neq))
							|| ((((srcType == IP_ADDR_TYPE_ANY)
								|| ((srcType == IP_ADDR_TYPE_IP_MASK) && (srcIpAddressSlashflag))) 	&& (is_range))))
						&& (argc >= 9))
				{
					if ((argc >= 9) && (strncasecmp(attributeNames[8].c_str(),"any",3) == 0))
					{
						dstType = IP_ADDR_TYPE_ANY;
						dstUnion = "dstType";
					}
					else if ((argc >= 10) && (strncasecmp(attributeNames[8].c_str(),"host",4) == 0))
					{
						dstHostIpAddress.fromString (attributeNames[9]);
						dstType 				= IP_ADDR_TYPE_HOST;
						dstHostIpAddressflag	= 1;
						dstUnion 				= "dstType";
					}
					else if ((argc >= 9) && ((dstType != IP_ADDR_TYPE_ANY)||(dstType != IP_ADDR_TYPE_HOST)))
					{
						string dstIpAddressHold = attributeNames[8];
						int position = dstIpAddressHold.find("/");
						if (position >= 0)
						{
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
						}
						else
						{
							dstIpAddress.fromString (attributeNames[8]);
							dstIpAddressMask.fromString (attributeNames[9]);
							dstIpAddressMaskflag	= 1;
						}
						dstType 				= IP_ADDR_TYPE_IP_MASK;
						dstUnion 				= "dstIpAddress";
					}
				}
				else if (((((srcType == IP_ADDR_TYPE_HOST) ||
								((srcType == IP_ADDR_TYPE_IP_MASK) && (!srcIpAddressSlashflag))))
						&& (is_range)) && (argc >= 10))
				{
					if ((argc >= 10) && (strncasecmp(attributeNames[9].c_str(),"any",3) == 0))
					{
						dstType = IP_ADDR_TYPE_ANY;
						dstUnion = "dstType";
					}
					else if ((argc >= 11) && (strncasecmp(attributeNames[9].c_str(),"host",4) == 0))
					{
						dstHostIpAddress.fromString (attributeNames[10]);
						dstType 			 = IP_ADDR_TYPE_HOST;
						dstHostIpAddressflag = 1;
						dstUnion 			 = "dstType";
					}
					else if ((argc >= 10) && ((dstType != IP_ADDR_TYPE_ANY)||(dstType != IP_ADDR_TYPE_HOST)))
					{
						string dstIpAddressHold = attributeNames[9];
						int position = dstIpAddressHold.find("/");
						if (position >= 0)
						{
							getSrcdstIpAddressandSrcdstIpMaskAddress(dstIpAddressHold, dstIpAddressMask,
								dstIpAddress, dstIpAddressSlashflag, position);
						}
						else
						{
							dstIpAddress.fromString (attributeNames[9]);
							dstIpAddressMask.fromString (attributeNames[10]);
							dstIpAddressMaskflag 	= 1;
						}
						dstType 				= IP_ADDR_TYPE_IP_MASK;
						dstUnion 				= "dstIpAddress";
					}
				}

				/*Setting destination parameters completed*/

				/*Setting the dport operator*/

				/**
				 * Added the slash flag in the condition for both the source,
				 * and destination type
				 */
				dport6 = (((srcType == IP_ADDR_TYPE_ANY)
						|| ((srcType == IP_ADDR_TYPE_IP_MASK) && (srcIpAddressMaskflag || srcIpAddressSlashflag)))
					&& ((dstType == IP_ADDR_TYPE_ANY) ||
						((dstType == IP_ADDR_TYPE_IP_MASK) && (dstIpAddressMaskflag || dstIpAddressSlashflag))));

				for (int iter1 = 6;iter1 < argc; iter1++)
				{
					if ((iter1 == 6)&&(!dport6))
						continue;

					if (strncasecmp(attributeNames[iter1].c_str(),"eq" ,2) ==0)
					{
						dstport= attributeNames[iter1+1];
						dstport_num = atoi(dstport.c_str());
						dstPortOperator = PORTOPERATOR_EQ;
					}
					else if (strncasecmp(attributeNames[iter1].c_str(),"gt" ,2) == 0)
					{
						dstport= attributeNames[iter1+1];
						dstport_num = atoi(dstport.c_str());
						dstPortOperator = PORTOPERATOR_GT;
					}
					else if (strncasecmp(attributeNames[iter1].c_str(),"lt" ,2) == 0)
					{
						dstport= attributeNames[iter1+1];
						dstport_num = atoi(dstport.c_str());
						dstPortOperator = PORTOPERATOR_LT;
					}
					else if (strncasecmp(attributeNames[iter1].c_str(),"neq" ,3) == 0)
					{
						dstport= attributeNames[iter1+1];
						dstport_num = atoi(dstport.c_str());
						dstPortOperator = PORTOPERATOR_NEQ;
					}
					else if (strncasecmp(attributeNames[iter1].c_str(),"range" ,5) == 0)
					{
						dstportlow = attributeNames[iter1+1];
						dstporthigh = attributeNames[iter1+2];
						dstport_num_low = atoi(dstportlow.c_str());
						dstport_num_high = atoi(dstporthigh.c_str());
						dstPortOperator = PORTOPERATOR_RANGE;
					}
				}

				if (strncasecmp(strArgv3,"tcp" ,3) == 0 )
				{
					dstport_num_well = StringtoWellknownNumberTcp(dstport.c_str());
					dstport_num_low_well = StringtoWellknownNumberTcp(dstportlow.c_str());
					dstport_num_high_well = StringtoWellknownNumberTcp(dstporthigh.c_str());
				}
				else
				{
					dstport_num_well = StringtoWellknownNumberUdp(dstport.c_str());
					dstport_num_low_well = StringtoWellknownNumberUdp(dstportlow.c_str());
					dstport_num_high_well = StringtoWellknownNumberUdp(dstporthigh.c_str());
				}

				if ((dstport_num_well == WELLKNOWNPORT_TCP_UNKNOWN) && (strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					dstport_num_well = NumbertoWellknownNumberTcp(dstport_num);
				else if ((dstport_num_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					dstport_num_well = NumbertoWellknownNumberUdp(dstport_num);

				if ((dstport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN) &&	(strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					dstport_num_low_well = NumbertoWellknownNumberTcp(dstport_num_low);
				else if ((dstport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					dstport_num_low_well = NumbertoWellknownNumberUdp(dstport_num_low);

				if ((dstport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN) &&	(strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					dstport_num_high_well = NumbertoWellknownNumberTcp(dstport_num_high);
				else if ((dstport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					dstport_num_high_well = NumbertoWellknownNumberUdp(dstport_num_high);

				if ((dstPortOperator == PORTOPERATOR_RANGE) &&
					(((dstport_num_low_well >= dstport_num_high_well) &&
					  ((dstport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)
					   ||(dstport_num_low_well != WELLKNOWNPORT_UDP_UNKNOWN))&&
						((dstport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_high_well != WELLKNOWNPORT_UDP_UNKNOWN)))

					||((dstport_num_low >= dstport_num_high) &&
						((dstport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN))&&
						((dstport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN)))

					||((dstport_num_low_well >= dstport_num_high) &&
						((dstport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_low_well != WELLKNOWNPORT_UDP_UNKNOWN))&&
						((dstport_num_high_well == WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_high_well == WELLKNOWNPORT_UDP_UNKNOWN)))

					||((dstport_num_low >= dstport_num_high_well) &&
						((dstport_num_low_well == WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_low_well == WELLKNOWNPORT_UDP_UNKNOWN))&&
						((dstport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN)
						 ||(dstport_num_high_well != WELLKNOWNPORT_UDP_UNKNOWN)))))
				{
					pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_ACL_PORT_RANGE_ERROR);
					return pmsg;
				}

				/*Setting dport other parameters for tcp-udp*/
				if (dstPortOperator != PORTOPERATOR_UNKNOWN)
				{
					if ((dstport_num_well != WELLKNOWNPORT_TCP_UNKNOWN) && (strncasecmp(strArgv3,"tcp" ,3) == 0) )
					{
                        if (dstPortOperator == PORTOPERATOR_LT)
                        {
                            dstPortltNumberTcpUnion = "dstPortltWellknownNumberTcp";
                            dstPortltWellknownNumberTcp = (PortWellknownNumberTcp)dstport_num_well;
                        }
                        else if (dstPortOperator == PORTOPERATOR_GT)
                        {
                            dstPortgtNumberTcpUnion = "dstPortgtWellknownNumberTcp";
                            dstPortgtWellknownNumberTcp = (PortWellknownNumberTcp)dstport_num_well;
                        }
                        else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ))
                        {
							dstPortNumberTcpUnion = "dstPortWellknownNumberTcp";
							dstPortWellknownNumberTcp = (PortWellknownNumberTcp)dstport_num_well;
						}
					}

					if ((dstport_num_low_well != WELLKNOWNPORT_TCP_UNKNOWN)
						&& (strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					{
						dstPortrangeLowerTcpUnion = "dstPortWellknownrangeLowerTcp";
						dstPortWellknownrangeLowerTcp = (PortWellknownNumberTcp)dstport_num_low_well;
					}

					if ((dstport_num_high_well != WELLKNOWNPORT_TCP_UNKNOWN) && (strncasecmp(strArgv3,"tcp" ,3) == 0 ))
					{
						dstPortrangeHigherTcpUnion = "dstPortWellknownrangeHigherTcp";
						dstPortWellknownrangeHigherTcp = (PortWellknownNumberTcp)dstport_num_high_well;
					}

					if ((dstport_num_well != WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
                        if (dstPortOperator == PORTOPERATOR_LT)
                        {
                            dstPortltNumberUdpUnion = "dstPortltWellknownNumberUdp";
                            dstPortltWellknownNumberUdp = (PortWellknownNumberUdp)dstport_num_well;
                        }
                        else if (dstPortOperator == PORTOPERATOR_GT)
                        {
                            dstPortgtNumberUdpUnion = "dstPortgtWellknownNumberUdp";
                            dstPortgtWellknownNumberUdp = (PortWellknownNumberUdp)dstport_num_well;
                        }
                        else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ))
                        {
							dstPortNumberUdpUnion = "dstPortWellknownNumberUdp";
							dstPortWellknownNumberUdp = (PortWellknownNumberUdp)dstport_num_well;
						}
					}

					if ((dstport_num_low_well  != WELLKNOWNPORT_UDP_UNKNOWN) && (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
						dstPortrangeLowerUdpUnion = "dstPortWellknownrangeLowerUdp";
						dstPortWellknownrangeLowerUdp = (PortWellknownNumberUdp)dstport_num_low_well;
					}

					if ((dstport_num_high_well  != WELLKNOWNPORT_UDP_UNKNOWN)
						&& (strncasecmp(strArgv3,"udp" ,3) == 0))
					{
						dstPortrangeHigherUdpUnion = "dstPortWellknownrangeHigherUdp";
						dstPortWellknownrangeHigherUdp = (PortWellknownNumberUdp)dstport_num_high_well;
					}

					if (strncasecmp(strArgv3,"tcp" ,3) == 0 )
					{
                        if ((dstPortOperator == PORTOPERATOR_LT) && (dstPortltWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
                            dstPortltNumberTcpUnion = "dstPortltCustomNumberTcp";
                            dstPortltCustomNumberTcp = dstport_num;
                        }
                        else if ((dstPortOperator == PORTOPERATOR_GT) && (dstPortgtWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
                            dstPortgtNumberTcpUnion = "dstPortgtCustomNumberTcp";
                            dstPortgtCustomNumberTcp = dstport_num;
                        }
                        else if (( (dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) &&
							(dstPortWellknownNumberTcp == WELLKNOWNPORT_TCP_UNKNOWN))
                        {
							dstPortNumberTcpUnion = "dstPortCustomNumberTcp";
							dstPortCustomNumberTcp = dstport_num;
						}
					}

					if (strncasecmp(strArgv3,"udp" ,3) == 0)
					{
                        if ((dstPortOperator == PORTOPERATOR_LT)  && (dstPortltWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
                            dstPortltNumberUdpUnion = "dstPortltCustomNumberUdp";
                            dstPortltCustomNumberUdp = dstport_num;
                        }
                        else if ((dstPortOperator == PORTOPERATOR_GT) && (dstPortgtWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
                            dstPortgtNumberUdpUnion = "dstPortgtCustomNumberUdp";
                            dstPortgtCustomNumberUdp = dstport_num;
                        }
                        else if (( (dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) &&
							(dstPortWellknownNumberUdp == WELLKNOWNPORT_UDP_UNKNOWN))
                        {
							dstPortNumberUdpUnion = "dstPortCustomNumberUdp";
							dstPortCustomNumberUdp = dstport_num;
						}
					}

					if ((strncasecmp(strArgv3,"tcp" ,3) == 0)
						&& (dstPortWellknownrangeLowerTcp == WELLKNOWNPORT_TCP_UNKNOWN))
					{
						dstPortrangeLowerTcpUnion = "dstPortCustomrangeLowerTcp";
						dstPortCustomrangeLowerTcp = dstport_num_low;
					}

					if ((strncasecmp(strArgv3,"tcp" ,3) == 0 )
						&& (dstPortWellknownrangeHigherTcp == WELLKNOWNPORT_TCP_UNKNOWN))
					{
						dstPortrangeHigherTcpUnion = "dstPortCustomrangeHigherTcp";
						dstPortCustomrangeHigherTcp = dstport_num_high;
					}

					if ((strncasecmp(strArgv3,"udp" ,3) == 0)
						&& (dstPortWellknownrangeLowerUdp  == WELLKNOWNPORT_UDP_UNKNOWN))
					{
						dstPortrangeLowerUdpUnion = "dstPortCustomrangeLowerUdp";
						dstPortCustomrangeLowerUdp = dstport_num_low;
					}

					if ((strncasecmp(strArgv3,"udp" ,3) == 0)
						&& (dstPortWellknownrangeHigherUdp == WELLKNOWNPORT_UDP_UNKNOWN))
					{
						dstPortrangeHigherUdpUnion = "dstPortCustomrangeHigherUdp";
						dstPortCustomrangeHigherUdp = dstport_num_high;
					}
				}
				/*Setting dport other params done*/
				break;
			}
			default:
				break;
		}

		if (((((srcIpAddressMaskflag)) || (srcHostIpAddressflag)
					|| (srcIpAddressSlashflag) || (srcType == IP_ADDR_TYPE_ANY))
					&& (!( (dstIpAddressMaskflag) || (dstHostIpAddressflag) || (dstIpAddressSlashflag)
					|| (dstType == IP_ADDR_TYPE_ANY))))  && (ptype == SSMACL_TYPE_L3_IPV4_EXT_E))	{

			pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_INCOMPLETE_COMMAND);
			return pmsg;
		}

		if (srcPortNumberTcpUnion.compare("") == 0) {
			srcPortNumberTcpUnion   = "srcPortCustomNumberTcp";
			srcPortCustomNumberTcp  = DEFAULT_PORTNUM;
		}
		if (dstPortNumberTcpUnion.compare("") == 0) {
			dstPortNumberTcpUnion   = "dstPortCustomNumberTcp";
			dstPortCustomNumberTcp  =  DEFAULT_PORTNUM;
		}
		if (srcPortltNumberTcpUnion.compare("") == 0) {
			srcPortltNumberTcpUnion = "srcPortltCustomNumberTcp";
			srcPortltCustomNumberTcp  = DEFAULT_PORTNUM;
		}
		if (srcPortgtNumberTcpUnion.compare("") == 0) {
			srcPortgtNumberTcpUnion = "srcPortgtCustomNumberTcp";
			srcPortgtCustomNumberTcp  = DEFAULT_PORTNUM;
		}
		if (srcPortrangeLowerTcpUnion.compare("") == 0) {
			srcPortrangeLowerTcpUnion = "srcPortCustomrangeLowerTcp";
			srcPortCustomrangeLowerTcp = DEFAULT_PORTNUM;
		}
		if (srcPortrangeHigherTcpUnion.compare("") == 0) {
			srcPortrangeHigherTcpUnion= "srcPortCustomrangeHigherTcp";
			srcPortCustomrangeHigherTcp = DEFAULT_PORTNUM;
		}
		if (dstPortrangeLowerTcpUnion.compare("") == 0) {
			dstPortrangeLowerTcpUnion = "dstPortCustomrangeLowerTcp";
			dstPortCustomrangeLowerTcp = DEFAULT_PORTNUM;
		}
		if (dstPortrangeHigherTcpUnion.compare("") == 0) {
			dstPortrangeHigherTcpUnion= "dstPortCustomrangeHigherTcp";
			dstPortCustomrangeHigherTcp = DEFAULT_PORTNUM;
		}
		if (dstPortltNumberTcpUnion.compare("") == 0) {
			dstPortltNumberTcpUnion = "dstPortltCustomNumberTcp";
			dstPortltCustomNumberTcp  = DEFAULT_PORTNUM;
		}
		if (dstPortgtNumberTcpUnion.compare("") == 0) {
			dstPortgtNumberTcpUnion = "dstPortgtCustomNumberTcp";
			dstPortgtCustomNumberTcp  = DEFAULT_PORTNUM;
		}

		if (srcPortrangeLowerUdpUnion.compare("") == 0) {
			srcPortrangeLowerUdpUnion = "srcPortCustomrangeLowerUdp";
			srcPortCustomrangeLowerUdp = DEFAULT_PORTNUM;
		}
		if (srcPortrangeHigherUdpUnion.compare("") == 0) {
			srcPortrangeHigherUdpUnion = "srcPortCustomrangeHigherUdp";
			srcPortCustomrangeHigherUdp = DEFAULT_PORTNUM;
		}
		if (dstPortrangeLowerUdpUnion.compare("") == 0) {
			dstPortrangeLowerUdpUnion = "dstPortCustomrangeLowerUdp";
			dstPortCustomrangeLowerUdp = DEFAULT_PORTNUM;
		}
		if (dstPortrangeHigherUdpUnion.compare("") == 0) {
			dstPortrangeHigherUdpUnion = "dstPortCustomrangeHigherUdp";
			dstPortCustomrangeHigherUdp = DEFAULT_PORTNUM;
		}
		if (srcPortNumberUdpUnion.compare("") == 0) {
			srcPortNumberUdpUnion   = "srcPortCustomNumberUdp";
			srcPortCustomNumberUdp  = DEFAULT_PORTNUM;
		}
		if (dstPortNumberUdpUnion.compare("") == 0) {
			dstPortNumberUdpUnion   = "dstPortCustomNumberUdp";
			dstPortCustomNumberUdp  =  DEFAULT_PORTNUM;
		}
		if (srcPortltNumberUdpUnion.compare("") == 0) {
			srcPortltNumberUdpUnion = "srcPortltCustomNumberUdp";
			srcPortltCustomNumberUdp  = DEFAULT_PORTNUM;
		}
		if (srcPortgtNumberUdpUnion.compare("") == 0) {
			srcPortgtNumberUdpUnion = "srcPortgtCustomNumberUdp";
			srcPortgtCustomNumberUdp  = DEFAULT_PORTNUM;
		}
		if (dstPortltNumberUdpUnion.compare("") == 0) {
			dstPortltNumberUdpUnion = "dstPortltCustomNumberUdp";
			dstPortltCustomNumberUdp = DEFAULT_PORTNUM;
		}
		if (dstPortgtNumberUdpUnion.compare("") == 0) {
			dstPortgtNumberUdpUnion = "dstPortgtCustomNumberUdp";
			dstPortgtCustomNumberUdp  = DEFAULT_PORTNUM;
		}

		if (precedenceUnion.compare("") == 0) {
			precedenceUnion = "customPrecedence";
			customPrecedence = DEFAULT_PREC_DSCP_TOS_VAL;
		}
		if (dscpUnion.compare("") == 0) {
			dscpUnion = "customDscp";
			customDscp = DEFAULT_PREC_DSCP_TOS_VAL;
		}
		if (tosUnion.compare("") == 0){
			tosUnion = "customTos" ;
			customTos = DEFAULT_PREC_DSCP_TOS_VAL;
		}

		if (srcType == IP_ADDR_TYPE_IP_MASK)
			srcType = IP_ADDR_TYPE_NONE;
		if (dstType == IP_ADDR_TYPE_IP_MASK)
			dstType = IP_ADDR_TYPE_NONE;

		if (pmsg != NULL) {
			pmsg->setPolicyName(policyName);
			pmsg->setPolicyType(ptype);
			pmsg->setRuleid(seqid);
			pmsg->setAction(action);
			pmsg->setSrcIpAddress(srcIpAddress);
			pmsg->setSrcIpAddressMask(srcIpAddressMask);
			pmsg->setSrcHostIpAddress(srcHostIpAddress);
			pmsg->setSrcUnion(srcUnion);
			pmsg->setIsCount(iscount);
			pmsg->setIsLog(islog);
			pmsg->setOpcode(opcode);
			pmsg->setSrcType(srcType);
			pmsg->setDstType(dstType);
			pmsg->setDstIpAddress(dstIpAddress);
			pmsg->setDstIpAddressMask(dstIpAddressMask);
			pmsg->setDstHostIpAddress(dstHostIpAddress);

			pmsg->setDstUnion(dstUnion);
			pmsg->setProtocolTypeUnion(ProtocolTypeUnion);
			pmsg->setL3l4CustomProtoType(l3l4CustomProtoType);
			pmsg->setL3l4WellknownProtoType(l3l4WellknownProtoType);
			pmsg->setSrcPortOperator(srcPortOperator);
			pmsg->setDstPortOperator(dstPortOperator);

			pmsg->setSrcPortNumberTcpUnion(srcPortNumberTcpUnion);
			pmsg->setDstPortNumberTcpUnion(dstPortNumberTcpUnion);
			pmsg->setSrcPortNumberUdpUnion(srcPortNumberUdpUnion);
			pmsg->setDstPortNumberUdpUnion(dstPortNumberUdpUnion);

			pmsg->setSrcPortrangeLowerTcpUnion(srcPortrangeLowerTcpUnion);
			pmsg->setDstPortrangeLowerTcpUnion(dstPortrangeLowerTcpUnion);

			pmsg->setSrcPortrangeHigherTcpUnion(srcPortrangeHigherTcpUnion);
			pmsg->setDstPortrangeHigherTcpUnion(dstPortrangeHigherTcpUnion);

			pmsg->setSrcPortrangeLowerUdpUnion(srcPortrangeLowerUdpUnion);
			pmsg->setDstPortrangeLowerUdpUnion(dstPortrangeLowerUdpUnion);

			pmsg->setSrcPortrangeHigherUdpUnion(srcPortrangeHigherUdpUnion);
			pmsg->setDstPortrangeHigherUdpUnion(dstPortrangeHigherUdpUnion);

			pmsg->setSrcPortCustomNumberTcp(srcPortCustomNumberTcp);
			pmsg->setDstPortCustomNumberTcp(dstPortCustomNumberTcp);
			pmsg->setSrcPortCustomNumberUdp(srcPortCustomNumberUdp);
			pmsg->setDstPortCustomNumberUdp(dstPortCustomNumberUdp);

			pmsg->setSrcPortWellknownNumberTcp(srcPortWellknownNumberTcp);
			pmsg->setDstPortWellknownNumberTcp(dstPortWellknownNumberTcp);
			pmsg->setSrcPortWellknownNumberUdp(srcPortWellknownNumberUdp);
			pmsg->setDstPortWellknownNumberUdp(dstPortWellknownNumberUdp);

			pmsg->setSrcPortCustomrangeLowerTcp(srcPortCustomrangeLowerTcp);
			pmsg->setDstPortCustomrangeLowerTcp(dstPortCustomrangeLowerTcp);
			pmsg->setSrcPortCustomrangeLowerUdp(srcPortCustomrangeLowerUdp);
			pmsg->setDstPortCustomrangeLowerUdp(dstPortCustomrangeLowerUdp);

			pmsg->setSrcPortCustomrangeHigherTcp(srcPortCustomrangeHigherTcp);
			pmsg->setDstPortCustomrangeHigherTcp(dstPortCustomrangeHigherTcp);
			pmsg->setSrcPortCustomrangeHigherUdp(srcPortCustomrangeHigherUdp);
			pmsg->setDstPortCustomrangeHigherUdp(dstPortCustomrangeHigherUdp);

			pmsg->setSrcPortWellknownrangeLowerUdp(srcPortWellknownrangeLowerUdp);
			pmsg->setDstPortWellknownrangeLowerUdp(dstPortWellknownrangeLowerUdp);
			pmsg->setSrcPortWellknownrangeLowerTcp(srcPortWellknownrangeLowerTcp);
			pmsg->setDstPortWellknownrangeLowerTcp(dstPortWellknownrangeLowerTcp);

			pmsg->setSrcPortWellknownrangeHigherUdp(srcPortWellknownrangeHigherUdp);
			pmsg->setDstPortWellknownrangeHigherUdp(dstPortWellknownrangeHigherUdp);
			pmsg->setSrcPortWellknownrangeHigherTcp(srcPortWellknownrangeHigherTcp);
			pmsg->setDstPortWellknownrangeHigherTcp(dstPortWellknownrangeHigherTcp);

			pmsg->setSrcPortltNumberTcpUnion(srcPortltNumberTcpUnion);
			pmsg->setDstPortltNumberTcpUnion(dstPortltNumberTcpUnion);
			pmsg->setSrcPortltNumberUdpUnion(srcPortltNumberUdpUnion);
			pmsg->setDstPortltNumberUdpUnion(dstPortltNumberUdpUnion);

			pmsg->setSrcPortgtNumberTcpUnion(srcPortgtNumberTcpUnion);
			pmsg->setDstPortgtNumberTcpUnion(dstPortgtNumberTcpUnion);
			pmsg->setSrcPortgtNumberUdpUnion(srcPortgtNumberUdpUnion);
			pmsg->setDstPortgtNumberUdpUnion(dstPortgtNumberUdpUnion);

			pmsg->setSrcPortltCustomNumberTcp(srcPortltCustomNumberTcp);
			pmsg->setDstPortltCustomNumberTcp(dstPortltCustomNumberTcp);
			pmsg->setSrcPortltCustomNumberUdp(srcPortltCustomNumberUdp);
			pmsg->setDstPortltCustomNumberUdp(dstPortltCustomNumberUdp);

			pmsg->setSrcPortgtCustomNumberTcp(srcPortgtCustomNumberTcp);
			pmsg->setDstPortgtCustomNumberTcp(dstPortgtCustomNumberTcp);
			pmsg->setSrcPortgtCustomNumberUdp(srcPortgtCustomNumberUdp);
			pmsg->setDstPortgtCustomNumberUdp(dstPortgtCustomNumberUdp);

			pmsg->setSrcPortltWellknownNumberTcp(srcPortltWellknownNumberTcp);
			pmsg->setDstPortltWellknownNumberTcp(dstPortltWellknownNumberTcp);
			pmsg->setSrcPortltWellknownNumberUdp(srcPortltWellknownNumberUdp);
			pmsg->setDstPortltWellknownNumberUdp(dstPortltWellknownNumberUdp);

			pmsg->setSrcPortgtWellknownNumberTcp(srcPortgtWellknownNumberTcp);
			pmsg->setDstPortgtWellknownNumberTcp(dstPortgtWellknownNumberTcp);
			pmsg->setSrcPortgtWellknownNumberUdp(srcPortgtWellknownNumberUdp);
			pmsg->setDstPortgtWellknownNumberUdp(dstPortgtWellknownNumberUdp);

			pmsg->setCustomPrecedence(customPrecedence);
			pmsg->setWellknownPrecedence(wellknownPrecedence);
			pmsg->setPrecedenceUnion(precedenceUnion);
			pmsg->setCustomTos(customTos);
			pmsg->setWellknownTos(wellknownTos);
			pmsg->setTosUnion(tosUnion);
			pmsg->setCustomDscp(customDscp);
			pmsg->setWellknownDscp(wellknownDscp);
			pmsg->setDscpUnion(dscpUnion);

			pmsg->setUrg(urg);
			pmsg->setAck(ack);
			pmsg->setPush(push);
			pmsg->setFin(fin);
			pmsg->setRst(rst);
			pmsg->setSync(sync);
		}

		if (pmsg == NULL) {
			return NULL;
		}

		pmsg->setUserEnteredSeqid(1);
		pmsg->setPolicyObjectId (parentObjectId);

		return pmsg;
	}

	return NULL;
}

ManagementInterfaceMessage *ExtIpAclRuleConfigurationHandler::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentDeleteContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentDeleteContext->getAttributeValues ();
	//ObjectId parentObjectId = pWaveConfigurationSegmentDeleteContext->getParentObjectId ();

	if (attributeUserTags.size () ==  attributeNames.size ())
	{
		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("ExtIpAclRuleConfigurationHandler : "
					"attributeUserTags[") + i + string("] ->") + attributeUserTags[i] + string(
							": attributeNames[") + i + string("] ->") + attributeNames[i] + string(" -> ") + attributeValues[i]);
		}

		string    policyName;
		UI64      seqid = 0;
		UI32      policyType			= SSMACL_TYPE_L3_IPV4_EXT_E;
		UI32	  opcode 				= SSMACL_DCMD_DEL_RULE;

		if (attributeNames[0] == "policyName")
		{
			policyName = attributeValues[0];
		}

		if (attributeNames[1] == "ruleid")
		{
			seqid = (UI64) (atoi (attributeValues[1].c_str ()));
		}

		SsmGlobalAssocIpRuleToPolicyMessage *pMessage = new SsmGlobalAssocIpRuleToPolicyMessage();

		if (pMessage != NULL)
		{
			pMessage->setPolicyName(policyName);
			pMessage->setPolicyType(policyType);
			pMessage->setRuleid(seqid);
			pMessage->setOpcode(opcode);
			pMessage->setUserEnteredSeqid(1);

			return pMessage;
		}
	}
	return (NULL);
}

UI32 ExtIpAclRuleConfigurationHandler::StringtoWellknownNumberTcp(const char *ipprotocol)
{
	if (strncasecmp(ipprotocol ,"bgp" ,3) == 0)
		return WELLKNOWNPORT_TCP_BGP;
	else if (strncasecmp(ipprotocol ,"echo",4) == 0)
		return WELLKNOWNPORT_TCP_ECHO;
	else if (strncasecmp(ipprotocol ,"daytime",7) ==0)
		return WELLKNOWNPORT_TCP_DAYTIME;
	else if (strncasecmp(ipprotocol ,"ftp-data" ,8) ==0)
		return WELLKNOWNPORT_TCP_FTPDATA;
	else if (strncasecmp(ipprotocol ,"ftp" ,3) ==0)
		return WELLKNOWNPORT_TCP_FTP;
	else if (strncasecmp(ipprotocol ,"telnet" ,6) ==0)
		return WELLKNOWNPORT_TCP_TELNET;
	else if (strncasecmp(ipprotocol ,"smtp" ,4) ==0)
		return WELLKNOWNPORT_TCP_SMTP;
	else if (strncasecmp(ipprotocol ,"time" ,4) ==0)
		return WELLKNOWNPORT_TCP_TIME;
	else if (strncasecmp(ipprotocol ,"tacacs" ,6) ==0)
		return WELLKNOWNPORT_TCP_TACAS;
	else if (strncasecmp(ipprotocol ,"domain" ,6) ==0)
		return WELLKNOWNPORT_TCP_DOMAIN;
	else if (strncasecmp(ipprotocol ,"www" ,3) ==0)
		return WELLKNOWNPORT_TCP_WWW;
	else if (strncasecmp(ipprotocol ,"hostname" ,8) ==0)
		return WELLKNOWNPORT_TCP_HOSTNAME;
	else if (strncasecmp(ipprotocol ,"pim-auto-rp" ,11) ==0)
		return WELLKNOWNPORT_TCP_PIM_AUTO_RP;
	else if (strncasecmp(ipprotocol ,"login" ,5) ==0)
		return WELLKNOWNPORT_TCP_LOGIN;
	else if (strncasecmp(ipprotocol ,"syslog" ,6) ==0)
		return WELLKNOWNPORT_TCP_SYSLOG;
	else if (strncasecmp(ipprotocol ,"talk" ,4) ==0)
		return WELLKNOWNPORT_TCP_TALK;
	else
		return WELLKNOWNPORT_TCP_UNKNOWN;
}

UI32 ExtIpAclRuleConfigurationHandler::StringtoWellknownNumberUdp(const char *ipprotocol)
{
 	if (strncasecmp(ipprotocol ,"echo",4) ==0)
		return WELLKNOWNPORT_UDP_ECHO;
	else if (strncasecmp(ipprotocol ,"tacacs",6) ==0)
		return WELLKNOWNPORT_UDP_TACAS;
	else if (strncasecmp(ipprotocol ,"domain",6) ==0)
		return WELLKNOWNPORT_UDP_DOMAIN;
	else if (strncasecmp(ipprotocol ,"bootps",6) ==0)
		return WELLKNOWNPORT_UDP_BOOTPS;
	else if (strncasecmp(ipprotocol ,"bootpc",6) ==0)
		return WELLKNOWNPORT_UDP_BOOTPC;
	else if (strncasecmp(ipprotocol ,"tftp",4) ==0)
		return WELLKNOWNPORT_UDP_TFTP;
	else if (strncasecmp(ipprotocol ,"ntp",3) ==0)
		return WELLKNOWNPORT_UDP_NTP;
	else if (strncasecmp(ipprotocol ,"snmp",4 ) ==0)
		return WELLKNOWNPORT_UDP_SNMP;
	else if (strncasecmp(ipprotocol ,"pim-auto-rp",11) ==0)
		return WELLKNOWNPORT_UDP_PIM_AUTO_RP;
	else if (strncasecmp(ipprotocol ,"syslog",6 ) ==0)
		return WELLKNOWNPORT_UDP_SYSLOG;
	else if (strncasecmp(ipprotocol ,"rip",3) ==0)
		return WELLKNOWNPORT_UDP_RIP;
	else
		return WELLKNOWNPORT_UDP_UNKNOWN;
}

UI32 ExtIpAclRuleConfigurationHandler::NumbertoWellknownNumberTcp (UI32 ipprotocol)
{
	if (ipprotocol == WELLKNOWNPORT_TCP_BGP)
		return WELLKNOWNPORT_TCP_BGP;
	else if (ipprotocol == WELLKNOWNPORT_TCP_ECHO)
		return WELLKNOWNPORT_TCP_ECHO;
	else if (ipprotocol == WELLKNOWNPORT_TCP_DAYTIME)
		return WELLKNOWNPORT_TCP_DAYTIME;
	else if (ipprotocol == WELLKNOWNPORT_TCP_FTPDATA)
		return WELLKNOWNPORT_TCP_FTPDATA;
	else if (ipprotocol == WELLKNOWNPORT_TCP_FTP)
		return WELLKNOWNPORT_TCP_FTP;
	else if (ipprotocol == WELLKNOWNPORT_TCP_TELNET)
		return WELLKNOWNPORT_TCP_TELNET;
	else if (ipprotocol == WELLKNOWNPORT_TCP_SMTP)
		return WELLKNOWNPORT_TCP_SMTP;
	else if (ipprotocol == WELLKNOWNPORT_TCP_TIME)
		return WELLKNOWNPORT_TCP_TIME;
	else if (ipprotocol == WELLKNOWNPORT_TCP_TACAS)
		return WELLKNOWNPORT_TCP_TACAS;
	else if (ipprotocol == WELLKNOWNPORT_TCP_DOMAIN)
		return WELLKNOWNPORT_TCP_DOMAIN;
	else if (ipprotocol == WELLKNOWNPORT_TCP_WWW)
		return WELLKNOWNPORT_TCP_WWW;
	else if (ipprotocol == WELLKNOWNPORT_TCP_HOSTNAME)
		return WELLKNOWNPORT_TCP_HOSTNAME;
	else if (ipprotocol == WELLKNOWNPORT_TCP_PIM_AUTO_RP)
		return WELLKNOWNPORT_TCP_PIM_AUTO_RP;
	else if (ipprotocol == WELLKNOWNPORT_TCP_LOGIN)
		return WELLKNOWNPORT_TCP_LOGIN;
	else if (ipprotocol == WELLKNOWNPORT_TCP_SYSLOG)
		return WELLKNOWNPORT_TCP_SYSLOG;
	else if (ipprotocol == WELLKNOWNPORT_TCP_TALK)
		return WELLKNOWNPORT_TCP_TALK;
	else
		return WELLKNOWNPORT_TCP_UNKNOWN;
}

UI32 ExtIpAclRuleConfigurationHandler::NumbertoWellknownNumberUdp (UI32 ipprotocol)
{
	if (ipprotocol == WELLKNOWNPORT_UDP_ECHO)
		return WELLKNOWNPORT_UDP_ECHO;
	else if (ipprotocol == WELLKNOWNPORT_UDP_TACAS)
		return WELLKNOWNPORT_UDP_TACAS;
	else if (ipprotocol == WELLKNOWNPORT_UDP_DOMAIN)
		return WELLKNOWNPORT_UDP_DOMAIN;
	else if (ipprotocol == WELLKNOWNPORT_UDP_BOOTPS)
		return WELLKNOWNPORT_UDP_BOOTPS;
	else if (ipprotocol == WELLKNOWNPORT_UDP_BOOTPC)
		return WELLKNOWNPORT_UDP_BOOTPC;
	else if (ipprotocol == WELLKNOWNPORT_UDP_TFTP)
		return WELLKNOWNPORT_UDP_TFTP;
	else if (ipprotocol == WELLKNOWNPORT_UDP_NTP)
		return WELLKNOWNPORT_UDP_NTP;
	else if (ipprotocol == WELLKNOWNPORT_UDP_SNMP)
		return WELLKNOWNPORT_UDP_SNMP;
	else if (ipprotocol == WELLKNOWNPORT_UDP_PIM_AUTO_RP)
		return WELLKNOWNPORT_UDP_PIM_AUTO_RP;
	else if (ipprotocol == WELLKNOWNPORT_UDP_SYSLOG)
		return WELLKNOWNPORT_UDP_SYSLOG;
	else if (ipprotocol == WELLKNOWNPORT_UDP_RIP)
		return WELLKNOWNPORT_UDP_RIP;
	else
		return WELLKNOWNPORT_UDP_UNKNOWN;
}

void ExtIpAclRuleConfigurationHandler::getSrcdstIpAddressandSrcdstIpMaskAddress
		(string &srcDstIpAddressHold, IpV4Address &srcDstIpAddressMask,
		IpV4Address &srcDstIpAddress, bool &srcDstIpAddressSlashflag, int position)
	{
		string maskvalue = srcDstIpAddressHold.substr(position + 1);
		int maskvalueint = atoi(maskvalue.c_str());
		char *addr_mask;
		struct sockaddr_in ipadr;
		ipadr.sin_addr.s_addr   = ((UINT32_MAX) >> (maskvalueint));
		ipadr.sin_addr.s_addr   = htonl(ipadr.sin_addr.s_addr);
		addr_mask               = inet_ntoa(ipadr.sin_addr);
		srcDstIpAddressMask.fromString(addr_mask);
		char *srcDstIpAddressArray = (char *) malloc (position + 1);
		//char srcIpAddressArray[position + 1];
		int length = srcDstIpAddressHold.copy(srcDstIpAddressArray,position,0);
		srcDstIpAddressArray[length] = '\0';
		string tmpDestStr = string(srcDstIpAddressArray);
		srcDstIpAddress.fromString (tmpDestStr);
		srcDstIpAddressSlashflag   = 1;
		free(srcDstIpAddressArray);
	}

UI32 ExtIpAclRuleConfigurationHandler::IsWellknownPrecedence (const char *PrecDscpTosString,UI32 precedence,int &PrecDscpTosFlag)
{
	if ((precedence == WELLKNOWN_PRECEDENCE_CRITICAL) || (strncmp (PrecDscpTosString ,"critical" ,8) == 0))
		return WELLKNOWN_PRECEDENCE_CRITICAL;
    else if ((precedence == WELLKNOWN_PRECEDENCE_FLASH_OVERRIDE) || (strncmp (PrecDscpTosString ,"flash-override" ,14) == 0))
        return WELLKNOWN_PRECEDENCE_FLASH_OVERRIDE;
	else if ((precedence == WELLKNOWN_PRECEDENCE_FLASH) || (strncmp (PrecDscpTosString ,"flash" ,5) == 0))
		return WELLKNOWN_PRECEDENCE_FLASH;
    else if ((precedence == WELLKNOWN_PRECEDENCE_IMMEDIATE) || (strncmp (PrecDscpTosString ,"immediate" ,9) == 0))
        return WELLKNOWN_PRECEDENCE_IMMEDIATE;
    else if ((precedence == WELLKNOWN_PRECEDENCE_INTERNET) || (strncmp (PrecDscpTosString ,"internet" ,8) == 0))
        return WELLKNOWN_PRECEDENCE_INTERNET;
    else if ((precedence == WELLKNOWN_PRECEDENCE_NETWORK) || (strncmp (PrecDscpTosString ,"network" ,7) == 0))
        return WELLKNOWN_PRECEDENCE_NETWORK;
    else if ((precedence == WELLKNOWN_PRECEDENCE_PRIORITY) || (strncmp (PrecDscpTosString ,"priority" ,8) == 0))
        return WELLKNOWN_PRECEDENCE_PRIORITY;
	else if ((precedence == WELLKNOWN_PRECEDENCE_ROUTINE) || (strncmp (PrecDscpTosString ,"routine" ,7) == 0))
		return WELLKNOWN_PRECEDENCE_ROUTINE;
    else
	{
		PrecDscpTosFlag = 0;
        return WELLKNOWN_PRECEDENCE_UNKNOWN;
	}
}

UI32 ExtIpAclRuleConfigurationHandler::IsWellknownDscp (const char *PrecDscpTosString,UI32 dscp,int &PrecDscpTosFlag)
{
	if ((dscp == WELLKNOWN_DSCP_AF11) || (strncmp (PrecDscpTosString ,"af11" ,4) == 0))
		return WELLKNOWN_DSCP_AF11;
	else if ((dscp == WELLKNOWN_DSCP_AF12) || (strncmp (PrecDscpTosString ,"af12" ,4) == 0))
        return WELLKNOWN_DSCP_AF12;
    else if ((dscp == WELLKNOWN_DSCP_AF13) || (strncmp (PrecDscpTosString ,"af13" ,4) == 0))
        return WELLKNOWN_DSCP_AF13;
    else if ((dscp == WELLKNOWN_DSCP_AF21) || (strncmp (PrecDscpTosString ,"af21" ,4) == 0))
        return WELLKNOWN_DSCP_AF21;
    else if ((dscp == WELLKNOWN_DSCP_AF22) || (strncmp (PrecDscpTosString ,"af22" ,4) == 0))
        return WELLKNOWN_DSCP_AF22;
    else if ((dscp == WELLKNOWN_DSCP_AF23) || (strncmp (PrecDscpTosString ,"af23" ,4) == 0))
        return WELLKNOWN_DSCP_AF23;
    else if ((dscp == WELLKNOWN_DSCP_AF31) || (strncmp (PrecDscpTosString ,"af31" ,4) == 0))
        return WELLKNOWN_DSCP_AF31;
    else if ((dscp == WELLKNOWN_DSCP_AF32) || (strncmp (PrecDscpTosString ,"af32" ,4) == 0))
        return WELLKNOWN_DSCP_AF32;
    else if ((dscp == WELLKNOWN_DSCP_AF33) || (strncmp (PrecDscpTosString ,"af33" ,4) == 0))
        return WELLKNOWN_DSCP_AF33;
    else if ((dscp == WELLKNOWN_DSCP_AF41) || (strncmp (PrecDscpTosString ,"af41" ,4) == 0))
        return WELLKNOWN_DSCP_AF41;
    else if ((dscp == WELLKNOWN_DSCP_AF42) || (strncmp (PrecDscpTosString ,"af42" ,4) == 0))
        return WELLKNOWN_DSCP_AF42;
    else if ((dscp == WELLKNOWN_DSCP_AF43) || (strncmp (PrecDscpTosString ,"af43" ,4) == 0))
        return WELLKNOWN_DSCP_AF43;
    else if ((dscp == WELLKNOWN_DSCP_CS1) || (strncmp (PrecDscpTosString ,"cs1" ,3) == 0))
        return WELLKNOWN_DSCP_CS1;
    else if ((dscp == WELLKNOWN_DSCP_CS2) || (strncmp (PrecDscpTosString ,"cs2" ,3) == 0))
        return WELLKNOWN_DSCP_CS2;
    else if ((dscp == WELLKNOWN_DSCP_CS3) || (strncmp (PrecDscpTosString ,"cs3" ,3) == 0))
        return WELLKNOWN_DSCP_CS3;
    else if ((dscp == WELLKNOWN_DSCP_CS4) || (strncmp (PrecDscpTosString ,"cs4" ,3) == 0))
        return WELLKNOWN_DSCP_CS4;
    else if ((dscp == WELLKNOWN_DSCP_CS5) || (strncmp (PrecDscpTosString ,"cs5" ,3) == 0))
        return WELLKNOWN_DSCP_CS5;
    else if ((dscp == WELLKNOWN_DSCP_CS6) || (strncmp (PrecDscpTosString ,"cs6" ,3) == 0))
        return WELLKNOWN_DSCP_CS6;
    else if ((dscp == WELLKNOWN_DSCP_CS7) || (strncmp (PrecDscpTosString ,"cs7" ,3) == 0))
        return WELLKNOWN_DSCP_CS7;
    else if ((dscp == WELLKNOWN_DSCP_EF) || (strncmp (PrecDscpTosString ,"ef" ,2) == 0))
        return WELLKNOWN_DSCP_EF;
	else if ((dscp == WELLKNOWN_DSCP_DEFAULT) || (strncmp (PrecDscpTosString ,"default" ,7) == 0))
		return WELLKNOWN_DSCP_DEFAULT;
    else
	{
		PrecDscpTosFlag = 0;
        return WELLKNOWN_DSCP_UNKNOWN;
	}
}

UI32 ExtIpAclRuleConfigurationHandler :: IsWellknownTos (const char *PrecDscpTosString,UI32 tos,int &PrecDscpTosFlag)
{
	if ((tos == WELLKNOWN_TOS_MAX_RELIABILITY) || (strncmp (PrecDscpTosString ,"max-reliability" ,15) ==0))
		return WELLKNOWN_TOS_MAX_RELIABILITY;
	else if ((tos == WELLKNOWN_TOS_MAX_THROUGHPUT) || (strncmp (PrecDscpTosString ,"max-throughput" ,14) ==0))
		return WELLKNOWN_TOS_MAX_THROUGHPUT;
    else if ((tos == WELLKNOWN_TOS_MIN_DELAY) || (strncmp (PrecDscpTosString ,"min-delay" ,9) ==0))
        return WELLKNOWN_TOS_MIN_DELAY;
	else if ((tos == WELLKNOWN_TOS_NORMAL)  || (strncmp (PrecDscpTosString ,"normal" ,6) ==0))
		return WELLKNOWN_TOS_NORMAL;
    else
	{
		PrecDscpTosFlag = 0;
        return WELLKNOWN_TOS_UNKNOWN;
	}
}

}
