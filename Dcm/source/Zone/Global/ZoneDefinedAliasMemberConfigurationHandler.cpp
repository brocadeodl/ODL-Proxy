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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Jayanth Venkataraman                                          *
 ***************************************************************************/

#include <netinet/in.h>
#include "Zone/Global/ZoneDefinedAliasMemberConfigurationHandler.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneDbCreateMessage.h"
#include "Zone/Global/ZoneDbRemoveMessage.h"
#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{


ZoneDefinedAliasMemberConfigurationHandler::ZoneDefinedAliasMemberConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "zone_defined_alias_member", false)
{
}
    
ZoneDefinedAliasMemberConfigurationHandler::~ZoneDefinedAliasMemberConfigurationHandler  ()
{
}

string ZoneDefinedAliasMemberConfigurationHandler::getErrorString(ManagementInterfaceMessage* pMessage)
{
	string errorMsg("");

	tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefAliasMemCfgHandler::getErrorString : getCompletionStatus() = %s", (FrameworkToolKit::localize (pMessage->getCompletionStatus())).c_str ());
	
	if(pMessage->getCompletionStatus() != WRC_WAVE_MESSAGE_SUCCESS)
	{
		vector<Attribute *> errorArguments;

		if (pMessage->getMessageString().size()) {
			tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefAliasMemCfgHandler::getErrorString : getMessageString() = %s", pMessage->getMessageString().c_str ());
			errorArguments.clear();
			errorArguments.push_back(new AttributeString(pMessage->getMessageString(), "string"));
		}


		errorMsg = FrameworkToolKit::localize (pMessage->getCompletionStatus(), errorArguments);
		tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefAliasMemCfgHandler::getErrorString : errorMsg = %s", errorMsg.c_str ());

	}

	return errorMsg;
}

ManagementInterfaceMessage *ZoneDefinedAliasMemberConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::createConfig : Number of User Tags %d, Number of Names %d, Number of Values %d", pWaveConfigurationSegmentCreateContext->getAttributeUserTags ().size (), pWaveConfigurationSegmentCreateContext->getAttributeNames ().size (), pWaveConfigurationSegmentCreateContext->getAttributeValues ().size ());

    vector<UI32> attributeUserTags  = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
    vector<string> attributeNames   = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
    vector<string> attributeValues  = pWaveConfigurationSegmentCreateContext->getAttributeValues ();
    ObjectId parentObjectId         = pWaveConfigurationSegmentCreateContext->getParentObjectId ();
    string aliasName;
    string aliasEntry;

    if (attributeNames.size () == attributeValues.size ())
    {
        //trace (TRACE_LEVEL_INFO, "ZoneDefinedAliasMemberConfigurationHandler::createConfig : All the VALUES are same length");
        
        for (UI32 i = 0; i < attributeUserTags.size (); i++)
        {
            if (attributeNames[i] == "aliasEntry")
            {
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::createConfig : AliasEntry Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
                    return (NULL);
                } 

                aliasEntry = attributeValues[i];
            }
            else if (attributeNames[i] == "aliasName")
            {
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::createConfig : Alias Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
                    return (NULL);
                } 
                
                aliasName = attributeValues[i];
            }
        }

        ZoneDbCreateMessage* pMessage = new ZoneDbCreateMessage(ZoneObjectManager::db_type_alias, aliasName, aliasEntry, 0);

	    pMessage->setIsConfigurationChange(false);
        pMessage->setParentMoObjectId (parentObjectId);

        return (pMessage);
    }

    return (NULL);
}

ManagementInterfaceMessage *ZoneDefinedAliasMemberConfigurationHandler::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::deleteConfig : Number of User Tags %d, Number of Names %d, Number of Values %d",
                 pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ().size (),
                 pWaveConfigurationSegmentDeleteContext->getAttributeNames ().size (),
                 pWaveConfigurationSegmentDeleteContext->getAttributeValues ().size ());

    vector<UI32> attributeUserTags  = pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ();
    vector<string> attributeNames   = pWaveConfigurationSegmentDeleteContext->getAttributeNames ();
    vector<string> attributeValues  = pWaveConfigurationSegmentDeleteContext->getAttributeValues ();
    ObjectId parentObjectId         = pWaveConfigurationSegmentDeleteContext->getParentObjectId ();
    string aliasName;
    string aliasEntry;

    if (attributeNames.size () == attributeValues.size ())
    {
        //trace (TRACE_LEVEL_INFO, "ZoneDefinedAliasMemberConfigurationHandler::deleteConfig : All the VALUES are same length");
        
        for (UI32 i = 0; i < attributeUserTags.size (); i++)
        {
            if (attributeNames[i] == "aliasEntry")
            {
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::deleteConfig : AliasEntry Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
                    return (NULL);
                } 

                aliasEntry = attributeValues[i];
            }
            else if (attributeNames[i] == "aliasName")
            {
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedAliasMemberConfigurationHandler::deleteConfig : Alias Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
                    return (NULL);
                } 
                
                aliasName = attributeValues[i];
            }
        }

        ZoneDbRemoveMessage* pMessage = new ZoneDbRemoveMessage(ZoneObjectManager::db_type_alias, aliasName, aliasEntry, 0);

	    pMessage->setIsConfigurationChange(false);
        pMessage->setParentMoObjectId (parentObjectId);

        return (pMessage);
    }

    return (NULL);
}

}
