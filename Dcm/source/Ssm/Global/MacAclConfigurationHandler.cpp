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
#include "Ssm/Global/MacAclConfigurationHandler.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "Ssm/Global/SsmGlobalAddMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{


MacAclConfigurationHandler::MacAclConfigurationHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "macAclCP")
{
}
    
MacAclConfigurationHandler::~MacAclConfigurationHandler  ()
{
}

ManagementInterfaceMessage *MacAclConfigurationHandler::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    //tracePrintf (TRACE_LEVEL_INFO, "MacAclConfigurationHandler::createConfig : Number of User Tags %d, Number of Names %d, Number of Values %d \n\n", pWaveConfigurationSegmentCreateContext->getAttributeUserTags ().size (), pWaveConfigurationSegmentCreateContext->getAttributeNames ().size (), pWaveConfigurationSegmentCreateContext->getAttributeValues ().size ());

    vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();

    if (attributeNames.size () == attributeValues.size ())
    {
        trace (TRACE_LEVEL_INFO, "MacAclConfigurationHandler::createConfig : All the VALUES are same length");
    
        for (UI32 i=0; i<attributeUserTags.size (); i++)
        {
            if (attributeNames[i] == "policyName")
            {
                // Create policy 
                UI32 policyType = SSMACL_TYPE_L2_STD_E;
                string policyName = attributeValues[i];

                tracePrintf (TRACE_LEVEL_INFO, "MacAclConfigurationHandler::createConfig : Policy Name : %s\n", attributeValues[i].c_str ());
                
				SsmGlobalAddMacAclPolicyMessage *pMessage = new SsmGlobalAddMacAclPolicyMessage();
				pMessage->setPolicyName(policyName);
				pMessage->setPolicyType(policyType);

                return (pMessage);
            }
        }
    }

    return (NULL);
}

}
