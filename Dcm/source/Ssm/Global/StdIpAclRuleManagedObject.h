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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef STDIPACLRULEMANAGEDOBJECT_H
#define STDIPACLRULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/IpAclRuleManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class StdIpAclRuleManagedObject : public IpAclRuleManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            StdIpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            StdIpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, const UI64 &ruleid,
				const UI32 &action, const WaveUnion  &srcUnion, const IpV4Address &hostIpAddress, 
				const IpV4Address &srcIpAddress, const IpV4Address &srcIpAddressMask, const IpAddrType &srcType, 
				const bool &iscount, const bool &islog);
            virtual    ~StdIpAclRuleManagedObject ();
            static string getClassName();


        private:
        protected:
        public:
    };
}
#endif    //STDIPACLRULEMANAGEDOBJECT_H                                       
