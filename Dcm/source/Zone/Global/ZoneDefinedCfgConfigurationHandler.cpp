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
#include "Zone/Global/ZoneDefinedCfgConfigurationHandler.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneDbCreateMessage.h"
#include "Zone/Global/ZoneDbDeleteMessage.h"
#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{


ZoneDefinedCfgConfigurationHandler::ZoneDefinedCfgConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "zone_defined_cfg")
{
}
    
ZoneDefinedCfgConfigurationHandler::~ZoneDefinedCfgConfigurationHandler  ()
{
}

ManagementInterfaceMessage *ZoneDefinedCfgConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::createConfig : Number of User Tags %d, Number of Names %d, Number of Values %d", pWaveConfigurationSegmentCreateContext->getAttributeUserTags ().size (), pWaveConfigurationSegmentCreateContext->getAttributeNames ().size (), pWaveConfigurationSegmentCreateContext->getAttributeValues ().size ());

    vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();

    if (attributeNames.size () == attributeValues.size ())
    {
        //trace (TRACE_LEVEL_INFO, "ZoneDefinedCfgConfigurationHandler::createConfig : All the VALUES are same length");

        for (UI32 i=0; i < attributeUserTags.size (); i++)
        {
            if (attributeNames[i] == "cfgName")
            {
                // Create policy 
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::createConfig : Cfg Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
	                break;
                }
                /*else 
                {
	                if (!IsNameValid((char *)attributeValues[i].c_str())) 
                    {
	                    tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneCfgWyserEaConfigurationWorker invalid cfg name %s\n", attributeValues[i].c_str());
	                    break;
                    }
	            }*/

                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::createConfig : member-zone : %s", attributeValues[i].c_str ());

                ZoneDbCreateMessage* pMessage = new ZoneDbCreateMessage(
	                ZoneObjectManager::db_type_cfg, attributeValues[i], string(""), 0);

	            pMessage->setIsConfigurationChange(false);
                
                return (pMessage);
            }
        }
    }

    return (NULL);
}

ManagementInterfaceMessage *ZoneDefinedCfgConfigurationHandler::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::createConfig : Number of User Tags %d, Number of Names %d, Number of Values %d",
				 pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ().size (),
				 pWaveConfigurationSegmentDeleteContext->getAttributeNames ().size (),
				 pWaveConfigurationSegmentDeleteContext->getAttributeValues ().size ());

    vector<UI32> attributeUserTags = pWaveConfigurationSegmentDeleteContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentDeleteContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentDeleteContext->getAttributeValues ();

    if (attributeNames.size () == attributeValues.size ())
    {
        //trace (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::deleteConfig : All the VALUES are same length");
    
        for (UI32 i=0; i < attributeUserTags.size (); i++)
        {
            if (attributeNames[i] == "cfgName")
            {
                // Create policy 
                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::deleteConfig : Cfg Name : %s", attributeValues[i].c_str ());

                if (attributeValues[i].empty()) 
                {
	                break;
                } 
                /*else 
                {
	                if (!IsNameValid((char *)attributeValues[i].c_str())) 
                    {
	                    tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneCfgWyserEaConfigurationWorker invalid cfg name %s\n", attributeValues[i].c_str());
	                    break;
                    }
	            }*/

                tracePrintf (TRACE_LEVEL_DEBUG, "ZoneDefinedCfgConfigurationHandler::createConfig : Cfg Name : %s", attributeValues[i].c_str ());

                ZoneDbDeleteMessage* pMessage = new ZoneDbDeleteMessage(
	                ZoneObjectManager::db_type_cfg, attributeValues[i], 0);

	            pMessage->setIsConfigurationChange(false);
                
                return (pMessage);
            }
        }
    }

    return (NULL);
}

}
