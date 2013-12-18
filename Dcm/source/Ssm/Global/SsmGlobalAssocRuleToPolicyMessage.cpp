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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    SsmGlobalAssocRuleToPolicyMessage::SsmGlobalAssocRuleToPolicyMessage ()
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALASSOCRULETOPOLICY),
          SsmAssocRuleToPolicyMsgPayload(this)
    {
    }

	 SsmGlobalAssocRuleToPolicyMessage::SsmGlobalAssocRuleToPolicyMessage (const string &policyName, 
		 const UI32 &policyType, const UI64 &ruleid, const UI32 &action, const string &srcMac, 
		 const bool &srcMask, const string &dstMac, const bool &dstMask, const string &appType, 
		 const bool &isCount, const UI32 &opcode)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALASSOCRULETOPOLICY),
          SsmAssocRuleToPolicyMsgPayload(this, policyName, policyType, ruleid, action, srcMac, srcMask, dstMac, dstMask, appType, isCount, opcode)
    {
    }   	
 
	SsmGlobalAssocRuleToPolicyMessage::SsmGlobalAssocRuleToPolicyMessage (const string &policyName,
		const UI32 &policyType, const UI64 &ruleid, const UI32 &action, const string &srcMac, 
		const string &srcMacMask, const AddrType &srcType, const string &dstMac, const string &dstMacMask, 
		const AddrType &dstType, const string &appType, const EtherType &appTypeEnum, const UI16 &appTypeInt,
		const bool &isCount, const bool &isLog, const UI32 &opcode, const UI8 &userEnteredSeqid)
        : ManagementInterfaceMessage (SsmGlobalObjectManager::getClassName (),SSMGLOBALASSOCRULETOPOLICY),
          SsmAssocRuleToPolicyMsgPayload(this, policyName, policyType, ruleid, action, srcMac, 
			  srcMacMask, srcType, dstMac, dstMacMask, dstType, appType, appTypeEnum, appTypeInt, 
			  isCount, isLog, opcode, userEnteredSeqid)
    {
    }

    SsmGlobalAssocRuleToPolicyMessage::~SsmGlobalAssocRuleToPolicyMessage ()
    {
    }

    void  SsmGlobalAssocRuleToPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
		SsmAssocRuleToPolicyMsgPayload::setupAttributesForSerialization ();
    }

}
