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
 *   Author : Hari Om                                                      *
 **************************************************************************/

#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include "Ssm/Global/StdIp6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    StdIp6AclRuleManagedObject::StdIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIp6AclRuleManagedObject::getClassName (), Ip6AclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclRuleManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdIp6AclRuleManagedObject::StdIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid, const UI32 &action, const WaveUnion  &srcUnion, const IpV6Address &hostIpAddress, 
		const IpV6AddressNetworkMask &srcIpAddress,
// const IpV6Address &srcIpAddressMask,
		 const IpAddrType &srcType, 
		const bool &iscount, const bool &islog)        
		: PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIp6AclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclRuleManagedObject(pSsmGlobalObjectManager, ruleid, action, srcUnion, hostIpAddress, srcIpAddress, 
//		srcIpAddressMask,
		 srcType, iscount, islog)
    {
    }

    StdIp6AclRuleManagedObject::~StdIp6AclRuleManagedObject ()
    {
    }

    string  StdIp6AclRuleManagedObject::getClassName()
    {
        return ("StdIp6AclRuleManagedObject");
    }

    void  StdIp6AclRuleManagedObject::setupAttributesForPersistence()
    {
        Ip6AclRuleManagedObject::setupAttributesForPersistence ();
    }

    void  StdIp6AclRuleManagedObject::setupAttributesForCreate()
    {
	}


}
