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
 *   Author : Jayanth Venkataraman                                          *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "Ssm/Global/MacAclRuleConfigurationHandler.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"
#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/Capabilities.h"

namespace DcmNs
{

MacAclRuleConfigurationHandler::MacAclRuleConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "stdAclRuleCP", false)
{
}
    
MacAclRuleConfigurationHandler::~MacAclRuleConfigurationHandler  ()
{
}

int MacAclRuleConfigurationHandler::convertToMacStringFormat (string mac, char *output, UI32 outputSize)
{
    unsigned char mac_addr [6];
    unsigned int macls = 0;
    const char *input = mac.c_str();

    if (sscanf (input, "%4hx.%4hx.%4hx", (unsigned short *)&mac_addr[0], (unsigned short *)&mac_addr[2], (unsigned short *)&mac_addr[4]) != 3)
    {
        return (-1);
    }
    else
    {
        unsigned char *pTemp3 = &mac_addr[4];
        macls = (unsigned int) htons(*(unsigned short *)pTemp3);
        macls <<= 16;
    }

    snprintf(output, outputSize, MAC_FORMAT_STR, MAC_SET_VAL(mac_addr));

    return (0);
}

string MacAclRuleConfigurationHandler::getErrorString(ManagementInterfaceMessage* pMessage)
{
	string errorMsg("");

	if( pMessage->getCompletionStatus() != WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE)
	{
		vector<Attribute *> errorArguments;

		if( pMessage->getCompletionStatus() == WRC_SSM_DCM_ERR_ACL_RULE_ALREADY_EXISTS
				|| pMessage->getCompletionStatus() == WRC_SSM_DCM_ERR_ACL_RULE_SEQ_ALREADY_EXISTS )
		{
			SsmGlobalAssocRuleToPolicyMessage *ruleMsg = dynamic_cast<SsmGlobalAssocRuleToPolicyMessage *>(pMessage);
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

ManagementInterfaceMessage *MacAclRuleConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();
	ObjectId parentObjectId = pWaveConfigurationSegmentCreateContext->getParentObjectId ();

	if (attributeUserTags.size () ==  attributeNames.size ())
	{
		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("MacAclRuleConfigurationHandler : "
					"attributeUserTags[") + i + string("] ->") + attributeUserTags[i] + string(
							": attributeNames[") + i + string("] ->") + attributeNames[i] + string(" -> ") + attributeValues[i]);
		}

		UI32	  action				= SSMACL_FORWARDING_RESULT_INVALID_E;
		AddrType  srcType				= ADDR_TYPE_NONE;
		string    srcMac;
		string    srcMacMask			= "";
		string    policyName;
		UI64      seqid = 0;
		UI32      policyType			= SSMACL_TYPE_L2_STD_E;
		bool      iscount				= 0;
		bool      islog					= 0;
		UI32	  opcode 				= SSMACL_DCMD_ADD_RULE;
		string 	  tempSrcDstMacMask;
		string 	  srcHost;
		char 	  buf[15];
		buf[14]	  = '\0';


		SsmGlobalAssocRuleToPolicyMessage *pMessage = new SsmGlobalAssocRuleToPolicyMessage();

		/*Incomplete command check for std acl*/
		if (attributeNames.size() > 7 || attributeNames.size() < 4)
		{
			pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WRC_SSM_DCM_ERR_INCOMPLETE_COMMAND);
    		return pMessage;
		}

		if (attributeNames[0] == "policyName")
		{
			policyName = attributeValues[0];
		}

		if (attributeNames[1] == "ruleid")
		{
			seqid = strtoll(attributeValues[1].c_str (), NULL, 10);
		}

		if (attributeNames[2] == "permit")
		{
			action = 1;
		}
		else if (attributeNames[2] == "deny")
		{
			action = 2;
		}
		else if (attributeNames[2] == "hard-drop")
		{
			action = 3;
		}

		/*Setting src type */
		if (strncasecmp(attributeNames[3].c_str (), "any", 3) == 0)
		{
			srcType = ADDR_TYPE_ANY;
		}
		else if (strncasecmp(attributeNames[3].c_str (), "host", 4) ==0)
		{
			srcType = ADDR_TYPE_HOST;
			tempSrcDstMacMask = attributeNames[4].c_str ();
			convertToMacStringFormat(tempSrcDstMacMask, buf, sizeof(buf));
			srcHost = buf;
		}
		else if (srcType == ADDR_TYPE_NONE)
		{
			tempSrcDstMacMask = attributeNames[3].c_str ();
			convertToMacStringFormat(tempSrcDstMacMask, buf, sizeof(buf));
			srcMac = buf;
			tempSrcDstMacMask = attributeNames[4].c_str ();
			convertToMacStringFormat(tempSrcDstMacMask, buf, sizeof(buf));
			srcMacMask = buf;
		}

		/* Finding if count and log option is entered by user*/
		for (size_t i=4; i < attributeNames.size(); i++)
		{
			if (strncasecmp(attributeNames[i].c_str(), "count", 5) == 0)
				iscount = 1;
			if (strncasecmp(attributeNames[i].c_str(), "log" , 3) == 0)
				islog = 1;
		}

		if (pMessage != NULL)
		{
			pMessage->setPolicyName(policyName);
			pMessage->setPolicyType(policyType);
			pMessage->setRuleid(seqid);
			pMessage->setAction(action);
			pMessage->setSrcMac(srcMac);
			if (srcType == ADDR_TYPE_HOST)
				pMessage->setSrcMac(srcHost);
			pMessage->setSrcMacMask(srcMacMask);
			pMessage->setSrcType(srcType);
			pMessage->setIsCount(iscount);
			pMessage->setOpcode(opcode);
			pMessage->setIsLog(islog);
			/* Note : It will always be 1 */
			pMessage->setUserEnteredSeqid(1);
			pMessage->setPolicyObjectId (parentObjectId);
			return pMessage;
		}
	}
	return (NULL);
}

ManagementInterfaceMessage *MacAclRuleConfigurationHandler::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
	vector<UI32> attributeUserTags = pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ();
	vector<string> attributeNames  = pWaveConfigurationSegmentDeleteContext->getAttributeNames ();
	vector<string> attributeValues = pWaveConfigurationSegmentDeleteContext->getAttributeValues ();
	ObjectId parentObjectId = pWaveConfigurationSegmentDeleteContext->getParentObjectId ();


	if (attributeUserTags.size () ==  attributeNames.size ())
	{
	    string    srcMac    			= "";
	    string    srcMacMask			= "";
		AddrType  srcType				= ADDR_TYPE_NONE;

		trace (TRACE_LEVEL_DEBUG, "MacAclRuleConfigurationHandler::deleteConfig : All the VALUES are same length");

		for (UI32 i=0; i<attributeUserTags.size (); i++)
		{
			trace (TRACE_LEVEL_DEBUG, string("MacAclRuleConfigurationHandler : "
					"attributeUserTags[") + i + string("] ->") + attributeUserTags[i] + string(
							": attributeNames[") + i + string("] ->") + attributeNames[i] + string(" -> ") + attributeValues[i]);
		}

		string    policyName;
		UI64      seqid = 0;
		UI32      policyType			= SSMACL_TYPE_L2_STD_E;
		UI32	  opcode 				= SSMACL_DCMD_DEL_RULE;
		UI32	  action				= SSMACL_FORWARDING_RESULT_INVALID_E;

		if (attributeNames[0] == "policyName")
		{
			policyName = attributeValues[0];
		}

		if (attributeNames[1] == "ruleid")
		{
			seqid = (UI64) (atoi (attributeValues[1].c_str ()));
		}

		SsmGlobalAssocRuleToPolicyMessage *pMessage = new SsmGlobalAssocRuleToPolicyMessage();

		if (pMessage != NULL)
		{
			pMessage->setPolicyName(policyName);
			pMessage->setPolicyType(policyType);
			pMessage->setRuleid(seqid);
			pMessage->setOpcode(opcode);
			pMessage->setSrcMac(srcMac);
			pMessage->setSrcMacMask(srcMacMask);
			pMessage->setSrcType(srcType);
			pMessage->setAction(action);

			/* Note : It will always be 1 */
			pMessage->setUserEnteredSeqid(1);
			pMessage->setPolicyObjectId (parentObjectId);
			return pMessage;
		}
	}
	return (NULL);
}

}
