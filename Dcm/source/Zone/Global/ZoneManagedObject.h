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

#ifndef ZONEMANAGEDOBJECT_H
#define ZONEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class ZoneObjectManager;
    class ZoneMemberManagedObject;
    class ZoneManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ZoneManagedObject (ZoneObjectManager *pZoneObjectManager);
            ZoneManagedObject (ZoneObjectManager *pZoneObjectManager,const string &zoneName,const vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > &zoneMembers);
            virtual    ~ZoneManagedObject ();
            static string getClassName();
            void setZoneName(const string &zoneName);
            string getZoneName()  const;
            void setZoneMembers(const vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > &zoneMembers);
            vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > getZoneMembers()  const;
            void addZoneMembers(ZoneMemberManagedObject *zoneMember);
            int removeZoneMembers(const string zoneEntry);
            int numOfLeftZoneMembers(const string zoneEntry, unsigned int *maxListIndex);

// Now the data members

        private:
        protected:
        public:
            string  m_zoneName;
            vector<WaveManagedObjectPointer<ZoneMemberManagedObject> >  m_zoneMembers;
    };
}
#endif                                            //ZONEMANAGEDOBJECT_H
