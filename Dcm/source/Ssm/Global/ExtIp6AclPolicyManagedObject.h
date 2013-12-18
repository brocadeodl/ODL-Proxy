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
 *   Author : bpatel                                                     *
 **************************************************************************/

#ifndef EXTIP6ACLPOLICYMANAGEDOBJECT_H
#define EXTIP6ACLPOLICYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include "Ssm/Global/ExtIp6AclRuleManagedObject.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class ExtIp6AclPolicyManagedObject : public Ip6AclPolicyManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            ExtIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            ExtIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &rules);
            virtual    ~ExtIp6AclPolicyManagedObject ();
            virtual void addRule(Ip6AclRuleManagedObject *ruleMO);
            virtual int deleteRule(const UI64 &ruleid);
            static string getClassName();
            void setRules(const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &rules);
            vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > getRules()  const;
            void sortRules();

// Now the data members

        private:
        protected:
        public:
            vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > m_extRules;
    };
}
#endif                                            //EXTIP666666ACLPOLICYMANAGEDOBJECT_H
