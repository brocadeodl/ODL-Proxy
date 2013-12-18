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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef ALIASMANAGEDOBJECT_H
#define ALIASMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneObjectManager;
    class AliasMemberManagedObject;
    class AliasManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            AliasManagedObject (ZoneObjectManager *pZoneObjectManager);
            AliasManagedObject (ZoneObjectManager *pZoneObjectManager,const string &aliasName,const vector<WaveManagedObjectPointer<AliasMemberManagedObject> > &aliasMembers);
            virtual    ~AliasManagedObject ();
            static string getClassName();
            void setAliasName(const string &aliasName);
            string getAliasName()  const;
            void setAliasMembers(const vector<WaveManagedObjectPointer<AliasMemberManagedObject> > &aliasMembers);
            vector<WaveManagedObjectPointer<AliasMemberManagedObject> > getAliasMembers()  const;
			int removeAliasMembers(const string aliasEntry);
			int numOfLeftAliasMembers(const string aliasEntry, unsigned int *maxListIndex);
			int queryAliasMembersForEntry(const string entryToFind);
			int numOfAliasMembers();

// Now the data members

        private:
        protected:
        public:
            string  m_aliasName;
            vector<WaveManagedObjectPointer<AliasMemberManagedObject> >  m_aliasMembers;
    };
}
#endif                                            //ALIASMANAGEDOBJECT_H
