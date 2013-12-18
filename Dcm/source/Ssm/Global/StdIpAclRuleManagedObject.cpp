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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Global/IpAclRuleManagedObject.h"
#include "Ssm/Global/StdIpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    StdIpAclRuleManagedObject::StdIpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIpAclRuleManagedObject::getClassName (), IpAclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclRuleManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdIpAclRuleManagedObject::StdIpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid, const UI32 &action, const WaveUnion  &srcUnion, const IpV4Address &hostIpAddress, 
		const IpV4Address &srcIpAddress, const IpV4Address &srcIpAddressMask, const IpAddrType &srcType, 
		const bool &iscount, const bool &islog)        
		: PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIpAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclRuleManagedObject(pSsmGlobalObjectManager, ruleid, action, srcUnion, hostIpAddress, srcIpAddress, 
		srcIpAddressMask, srcType, iscount, islog)
    {
    }

    StdIpAclRuleManagedObject::~StdIpAclRuleManagedObject ()
    {
    }

    string  StdIpAclRuleManagedObject::getClassName()
    {
        return ("StdIpAclRuleManagedObject");
    }

    void  StdIpAclRuleManagedObject::setupAttributesForPersistence()
    {
        IpAclRuleManagedObject::setupAttributesForPersistence ();
    }

    void  StdIpAclRuleManagedObject::setupAttributesForCreate()
    {
	}


}
