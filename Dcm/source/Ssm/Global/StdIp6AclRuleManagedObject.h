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
 *   Author : Hari Om                                                      *
 **************************************************************************/

#ifndef STDIP6ACLRULEMANAGEDOBJECT_H
#define STDIP6ACLRULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/Ip6AclRuleManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class StdIp6AclRuleManagedObject : public Ip6AclRuleManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            StdIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            StdIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, const UI64 &ruleid,
				const UI32 &action, const WaveUnion  &srcUnion, const IpV6Address &hostIpAddress, 
				const IpV6AddressNetworkMask &srcIpAddress,
// const IpV6Address &srcIpAddressMask, 
				const IpAddrType &srcType, 
				const bool &iscount, const bool &islog);
            virtual    ~StdIp6AclRuleManagedObject ();
            static string getClassName();


        private:
        protected:
        public:
    };
}
#endif    //StdIp6AclRuleManagedObject_H                                       
