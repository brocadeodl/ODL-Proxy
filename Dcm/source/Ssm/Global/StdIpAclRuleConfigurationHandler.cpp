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
#include "Ssm/Global/StdIpAclRuleConfigurationHandler.h"
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

StdIpAclRuleConfigurationHandler::StdIpAclRuleConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "stdIpaclRuleCP", false)
{
}

StdIpAclRuleConfigurationHandler::~StdIpAclRuleConfigurationHandler  ()
{
}

string StdIpAclRuleConfigurationHandler::getErrorString(ManagementInterfaceMessage* pMessage)
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

ManagementInterfaceMessage *StdIpAclRuleConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();
	ObjectId parentObjectId = pWaveConfigurationSegmentCreateContext->getParentObjectId ();

	if (attributeUserTags.size () ==  attributeNames.size ())
	{
		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("StdIpAclRuleConfigurationHandler : "
					"attributeUserTags[") + i + string("] ->") + attributeUserTags[i] + string(
							": attributeNames[") + i + string("] ->") + attributeNames[i] + string(" -> ") + attributeValues[i]);
		}

		int                             argc                                = attributeNames.size();
		UI64						    seqid 								= 0;
		UI32							action 								= SSMACL_FORWARDING_RESULT_INVALID_E;
		IpAddrType  					srcType 							= IP_ADDR_TYPE_NONE;
		string 							hkey 								= attributeNames[1];
		ssmAcl_dcmd_opcode_t 			opcode;
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
		string 							srcport 							= "";
		string 							srcportlow 							= "";
		string 							srcporthigh 						= "";
		string 							dstport 							= "";
		string 							dstportlow 							= "";
		string 							dstporthigh 						= "";
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

		UI32 							ptype								= SSMACL_TYPE_L3_IPV4_STD_E;
		bool 							iscount 							= 0;
		bool 							islog 								= 0;
		/* Following flags are for error condition handling */
//		bool 		                    srcHostIpAddressflag            	= 0;
//		bool        		            dstHostIpAddressflag            	= 0;
//		bool                		    srcIpAddressMaskflag            	= 0;
//		bool            		        dstIpAddressMaskflag            	= 0;
		bool                    		srcIpAddressSlashflag           	= 0;
//		bool                    		dstIpAddressSlashflag           	= 0;

		SsmGlobalAssocIpRuleToPolicyMessage *pmsg = new SsmGlobalAssocIpRuleToPolicyMessage();

		if ( attributeNames.size() < 4 && pmsg != NULL )
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

		ptype = SSMACL_TYPE_L3_IPV4_STD_E;

		if (strncasecmp(attributeNames[3].c_str(),"any",3) == 0)
		{
			/*ANY case*/
			srcType = IP_ADDR_TYPE_ANY;
			iscount =((argc == 5)||(argc == 6))?((strncasecmp(attributeNames[4].c_str(),"count",5) == 0)? 1: 0):0;
			islog = ((argc == 5)?((strncasecmp(attributeNames[4].c_str(),"log" ,3) == 0) ? 1 :0) :0)
									||((argc == 6)?((strncasecmp(attributeNames[5].c_str(),"log" ,3) == 0) ? 1 :0) :0);
			srcUnion = "srcType";
		}
		else if (strncasecmp(attributeNames[3].c_str(),"host",4) == 0)
		{
			/*HOST case*/
			srcHostIpAddress.fromString (attributeNames[4].c_str());
			iscount =((argc == 6)||(argc == 7))?((strncasecmp(attributeNames[5].c_str(),"count",5) == 0)? 1: 0):0;
			islog = ((argc == 6)?((strncasecmp(attributeNames[5].c_str(),"log" ,3) == 0) ? 1 :0) :0)
									||((argc == 7)?((strncasecmp(attributeNames[6].c_str(),"log" ,3) == 0) ? 1 :0) :0);
			srcType = IP_ADDR_TYPE_HOST;
			srcUnion = "srcType";
//			srcHostIpAddressflag = 1;
		}
		else if ((srcType != IP_ADDR_TYPE_ANY) || (srcType != IP_ADDR_TYPE_HOST))
		{
			/*MASK case*/
			string srcIpAddressHold = attributeNames[3].c_str();
			int position = srcIpAddressHold.find("/");
			if (position >= 0) {
				getSrcdstIpAddressandSrcdstIpMaskAddress (srcIpAddressHold, srcIpAddressMask,
						srcIpAddress, srcIpAddressSlashflag, position);
				iscount =(((argc == 5)||(argc == 6))?((strncasecmp(attributeNames[4].c_str(), "count", 5) == 0) ? 1: 0):0);
				islog = ((argc == 5)?((strncasecmp(attributeNames[4].c_str(), "log" , 3) == 0) ? 1 :0) :0)
										||((argc == 6)?((strncasecmp(attributeNames[5].c_str(), "log" , 3) == 0) ? 1 :0) :0);
			}
			else
			{
				srcIpAddress.fromString (attributeNames[3]);
				srcIpAddressMask.fromString (attributeNames[4]);
				iscount =((argc == 6)||(argc == 7))?((strncasecmp(attributeNames[5].c_str(), "count", 5) == 0)? 1: 0):0;
				islog = ((argc == 6)?((strncasecmp(attributeNames[5].c_str(), "log" , 3) == 0) ? 1 :0) :0)
										||((argc == 7)?((strncasecmp(attributeNames[6].c_str(), "log" , 3) == 0) ? 1 :0) :0);
//				srcIpAddressMaskflag = 1;
			}
			srcType = IP_ADDR_TYPE_IP_MASK;
			srcUnion = "srcIpAddress";
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

ManagementInterfaceMessage *StdIpAclRuleConfigurationHandler::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentDeleteContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentDeleteContext->getAttributeValues ();
	//ObjectId parentObjectId = pWaveConfigurationSegmentDeleteContext->getParentObjectId ();

	if (attributeUserTags.size () ==  attributeNames.size ())
	{
		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("StdIpAclRuleConfigurationHandler : "
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

void StdIpAclRuleConfigurationHandler::getSrcdstIpAddressandSrcdstIpMaskAddress
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

}
