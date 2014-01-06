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

#ifndef EMPLOYEEMANAGEDOBJECT_H
#define EMPLOYEEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class CompanyManagementObjectManager;

    class EmployeeManagedObject : virtual public DcmManagedObject
    {
        private:
                    void        getAttributeGroupsInformation               (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeattributeGroupMap);
        protected:
            virtual void        setupAttributesForPersistence               ();
            virtual void        setupAttributesForCreate                    ();
        public:
                                EmployeeManagedObject                       (CompanyManagementObjectManager *pCompanyManagementObjectManager);
            virtual            ~EmployeeManagedObject                       ();
            virtual void        getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext);
            static  string      getClassName                                ();

                    void        setEmployeeName                             (const string &employeeName);
                    string      getEmployeeName                             ()  const;

                    void        setGroupObjectId                            (const ObjectId &groupObjectId);
                    ObjectId    getGroupObjectId                            () const;

                    void        setLocationObjectId                         (const ObjectId &locationObjectId);
                    ObjectId    getLocationObjectId                         () const;

            virtual void        getHardwareConfigurationDetailsForUpdate    (GetHardwareConfigurationDetailsForUpdateContext *pContext);
            virtual void        getHardwareConfigurationDetailsForCreate    (GetHardwareConfigurationDetailsForCreateContext *pContext);
            virtual void        getHardwareConfigurationDetailsForDelete    (GetHardwareConfigurationDetailsForDeleteContext *pContext);                    
            virtual void        isAssociatedManagedObjectToBeCreated        (IsAssociatedManagedObjectToBeCreatedContext *pContext);
// Now the data members

        private:
        protected:
        public:
                    string      m_employeeName;
                    ObjectId    m_groupObjectId;
                    ObjectId    m_locationObjectId;
    };
}
#endif //EMPLOYEEMANAGEDOBJECT_H
