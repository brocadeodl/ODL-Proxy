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

#ifndef EXTMACACLPOLICYMANAGEDOBJECT_H
#define EXTMACACLPOLICYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/ExtMacAclRuleManagedObject.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class ExtMacAclPolicyManagedObject : public MacAclPolicyManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            ExtMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            ExtMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > &rules, const vector<ObjectId> &interfaces);
            virtual    ~ExtMacAclPolicyManagedObject ();
            virtual void addRule(MacAclRuleManagedObject *ruleMO);
            virtual int deleteRule(const UI64 &ruleid);
            static string getClassName();
            void setRules(const vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > &rules);
            vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > getRules()  const;
            void sortRules();

// Now the data members

        private:
        protected:
        public:
            vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > m_extRules;
    };
}
#endif                                            //MACACLPOLICYMANAGEDOBJECT_H
