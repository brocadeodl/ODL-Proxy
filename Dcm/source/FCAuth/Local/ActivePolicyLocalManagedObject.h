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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef ACTIVEPOLICYLOCALMANAGEDOBJECT_H
#define ACTIVEPOLICYLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalObjectManager;
    class ActivePolicyMemberLocalManagedObject;
    class ActivePolicyLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ActivePolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager);
            ActivePolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > &PolicyMembers);
            virtual    ~ActivePolicyLocalManagedObject ();
            static string getClassName();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyMembers(const vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > &PolicyMembers);
            vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > getPolicyMembers()  const;
			void addPolicyMembers(ActivePolicyMemberLocalManagedObject *policyMember);
            int removePolicyMembers(const string policyMember);
            int numOfLeftPolicyMembers(const string policyMember, int *maxListIndex);
// Now the data members

        private:
        protected:
        public:
            string  m_policyName;
            vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> >  m_PolicyMembers;
    };
}
#endif                                            //ACTIVEPOLICYLOCALMANAGEDOBJECT_H
