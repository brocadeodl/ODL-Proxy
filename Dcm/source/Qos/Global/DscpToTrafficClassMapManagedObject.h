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
 *   Author : nbellari                                                     *
 **************************************************************************/

#ifndef DSCPTOTRAFFICCLASSMAPMANAGEDOBJECT_H
#define DSCPTOTRAFFICCLASSMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class DscpMarkListManagedObject;
    class DscpToTrafficClassMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            DscpToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            DscpToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &dscpToTrafficClassMapName,const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList);
            virtual    ~DscpToTrafficClassMapManagedObject ();
            static string getClassName();
            void setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName);
            string getDscpToTrafficClassMapName()  const;
            void setDscpMarkList(const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList);
            vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > getDscpMarkList()  const;
			void addDscpMarkList(DscpMarkListManagedObject *pDscpMarkListManagedObject);
			SI32 delDscpMarkList(const string &markList);
			ObjectId getDscpMarkListObjectId(const string &markList);

// Now the data members

        private:
        protected:
        public:
            string  m_dscpToTrafficClassMapName;
            vector<WaveManagedObjectPointer<DscpMarkListManagedObject> >  m_dscpMarkList;
    };
}
#endif                                            //DSCPTOTRAFFICCLASSMAPMANAGEDOBJECT_H