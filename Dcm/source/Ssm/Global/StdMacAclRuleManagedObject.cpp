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

#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/StdMacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

namespace DcmNs
{

    StdMacAclRuleManagedObject::StdMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdMacAclRuleManagedObject::getClassName (), MacAclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclRuleManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdMacAclRuleManagedObject::StdMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid, const UI32 &action, const string &srcMac, const string &srcMacMask, 
		const string &srcHost, const AddrType &srcType, const bool &iscount, const bool &islog)        
		: PrismElement  		(pSsmGlobalObjectManager),
        PrismPersistableObject 	(StdMacAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject 		(pSsmGlobalObjectManager),
        DcmManagedObject 		(pSsmGlobalObjectManager),
        MacAclRuleManagedObject	(pSsmGlobalObjectManager, ruleid, action, srcMac, srcMacMask, srcHost, srcType, iscount, islog)
    {
    }

    StdMacAclRuleManagedObject::~StdMacAclRuleManagedObject ()
    {
    }

    string  StdMacAclRuleManagedObject::getClassName()
    {
        return ("StdMacAclRuleManagedObject");
    }

    void  StdMacAclRuleManagedObject::setupAttributesForPersistence()
    {
        MacAclRuleManagedObject::setupAttributesForPersistence ();
    }

    void  StdMacAclRuleManagedObject::setupAttributesForCreate()
    {
    }


}
