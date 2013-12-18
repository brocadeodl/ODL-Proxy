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

#ifndef IPACLPOLICYMANAGEDOBJECT_H
#define IPACLPOLICYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/IpAclRuleManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class IpAclPolicyManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            IpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            IpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName);
            virtual    ~IpAclPolicyManagedObject ();
            static string getClassName();
            void setPolicyName(const string &policyName);
            virtual void addRule(IpAclRuleManagedObject *ruleMO);
            virtual int deleteRule(const UI64 &ruleid);
            string getPolicyName()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_policyName;
    };
}
#endif                                            //IPACLPOLICYMANAGEDOBJECT_H
