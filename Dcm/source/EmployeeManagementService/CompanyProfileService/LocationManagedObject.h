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
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#ifndef LOCATIONMANAGEDOBJECT_H
#define LOCATIONMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class CompanyManagementObjectManager;

    class LocationManagedObject : virtual public DcmManagedObject
    {
        private:
                    void        getAttributeGroupsInformation   (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeattributeGroupMap);
        protected:
            virtual void        setupAttributesForPersistence   ();
            virtual void        setupAttributesForCreate        ();
        public:
                                LocationManagedObject            (CompanyManagementObjectManager *pCompanyManagementObjectManager);
            virtual            ~LocationManagedObject            ();
            virtual void        getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext);
            static  string      getClassName                     ();

                    void        setCityObjectId                  (const ObjectId &cityObjectId);
                    ObjectId    getCityObjectId                  () const;
            virtual void        isAssociatedManagedObjectToBeCreated    (IsAssociatedManagedObjectToBeCreatedContext *pContext);

            virtual void        getHardwareConfigurationDetailsForUpdate         (GetHardwareConfigurationDetailsForUpdateContext *pContext);
            virtual void        getHardwareConfigurationDetailsForCreate         (GetHardwareConfigurationDetailsForCreateContext *pContext);
            virtual void        getHardwareConfigurationDetailsForDelete         (GetHardwareConfigurationDetailsForDeleteContext *pContext);

// Now the data members

        private:
                    string      m_locationName;
                    ObjectId    m_cityObjectId;
        protected:
        public:
    };
}
#endif //LOCATIONMANAGEDOBJECT_H

