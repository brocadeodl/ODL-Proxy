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

#ifndef ACTIVEPOLICYMEMBERLOCALMANAGEDOBJECT_H
#define ACTIVEPOLICYMEMBERLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalObjectManager;
    class ActivePolicyMemberLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ActivePolicyMemberLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager);
            ActivePolicyMemberLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const string &policyMemberEntry,const SI32 &listIndex);
            virtual    ~ActivePolicyMemberLocalManagedObject ();
            static string getClassName();
            void setPolicyMemberEntry(const string &policyMemberEntry);
            string getPolicyMemberEntry()  const;
            void setListIndex(const SI32 &listIndex);
            SI32 getListIndex()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_policyMemberEntry;
            SI32  m_listIndex;
    };
}
#endif                                            //ACTIVEPOLICYMEMBERLOCALMANAGEDOBJECT_H
