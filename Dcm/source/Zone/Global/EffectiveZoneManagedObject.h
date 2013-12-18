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

#ifndef EFFECTIVEZONEMANAGEDOBJECT_H
#define EFFECTIVEZONEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneObjectManager;
    class EffectiveZoneMemberManagedObject;
    class EffectiveZoneManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            EffectiveZoneManagedObject (ZoneObjectManager *pZoneObjectManager);
            EffectiveZoneManagedObject (ZoneObjectManager *pZoneObjectManager,const string &effectiveZoneName,const vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> > &effectiveZoneMembers);
            virtual    ~EffectiveZoneManagedObject ();
            static string getClassName();
            void setEffectiveZoneName(const string &effectiveZoneName);
            string getEffectiveZoneName()  const;
            void setEffectiveZoneMembers(const vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> > &effectiveZoneMembers);
            vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> > getEffectiveZoneMembers()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_effectiveZoneName;
            vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> >  m_effectiveZoneMembers;
    };
}
#endif                                            //EFFECTIVEZONEMANAGEDOBJECT_H
