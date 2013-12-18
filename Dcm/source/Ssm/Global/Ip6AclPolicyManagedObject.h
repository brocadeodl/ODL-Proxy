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

#ifndef IP6ACLPOLICYMANAGEDOBJECT_H
#define IP6ACLPOLICYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/Ip6AclRuleManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class Ip6AclPolicyManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            Ip6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            Ip6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName);
            virtual    ~Ip6AclPolicyManagedObject ();
            static string getClassName();
            void setPolicyName(const string &policyName);
            virtual void addRule(Ip6AclRuleManagedObject *ruleMO);
            virtual int deleteRule(const UI64 &ruleid);
            string getPolicyName()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_policyName;
    };
}
#endif                                            //IP6ACLPOLICYMANAGEDOBJECT_H
