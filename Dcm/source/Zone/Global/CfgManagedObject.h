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

#ifndef CFGMANAGEDOBJECT_H
#define CFGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneObjectManager;
    class CfgMemberManagedObject;
    class CfgManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CfgManagedObject (ZoneObjectManager *pZoneObjectManager);
            CfgManagedObject (ZoneObjectManager *pZoneObjectManager,const string &cfgName,const vector<WaveManagedObjectPointer<CfgMemberManagedObject> > &cfgMembers);
            virtual    ~CfgManagedObject ();
            static string getClassName();
            void setCfgName(const string &cfgName);
            string getCfgName()  const;
            void setCfgMembers(const vector<WaveManagedObjectPointer<CfgMemberManagedObject> > &cfgMembers);
            vector<WaveManagedObjectPointer<CfgMemberManagedObject> > getCfgMembers()  const;
            void addCfgMembers(CfgMemberManagedObject *cfgMember);
            int removeCfgMembers(const string cfgEntry);
            int numOfLeftCfgMembers(const string cfgEntry, unsigned int *maxListIndex);

// Now the data members

        private:
        protected:
        public:
            string  m_cfgName;
            vector<WaveManagedObjectPointer<CfgMemberManagedObject> >  m_cfgMembers;
    };
}
#endif                                            //CFGMANAGEDOBJECT_H
