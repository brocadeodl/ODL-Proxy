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

#ifndef COMPANYMANAGEDOBJECT_H
#define COMPANYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectCreateContext.h"
#include "EmployeeManagementService/CompanyProfileService/EmployeeManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/GroupManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/LocationManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class CompanyManagementObjectManager;

    class CompanyManagedObject : virtual public DcmManagedObject
    {
        private:
                    void    getAttributeGroupsInformation   (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeattributeGroupMap);
        protected:
            virtual void    setupAttributesForPersistence   ();
            virtual void    setupAttributesForCreate        ();
            virtual void    addCompositionToManagedObject   (WaveManagedObject *pWaveManagedObject);
        public:
                            CompanyManagedObject            (CompanyManagementObjectManager *pCompanyManagementObjectManager);
            virtual void    getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext);
            virtual        ~CompanyManagedObject            ();
            static  string  getClassName                    ();

                    void    setCompanyName                  (const string &companyName);
                    string  getCompanyName                  ()  const;

            virtual void    getHardwareConfigurationDetailsForUpdate         (GetHardwareConfigurationDetailsForUpdateContext *pContext);
            virtual void    getHardwareConfigurationDetailsForCreate         (GetHardwareConfigurationDetailsForCreateContext *pContext);
            virtual void    getHardwareConfigurationDetailsForDelete         (GetHardwareConfigurationDetailsForDeleteContext *pContext);                    
            virtual bool    getPluginDetailsForDistribution                  (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
            virtual void    addNewAttributeToBeUpdatedForCreate              (UpdateNewAttributeInManagedObjectContext *pContext);
// Now the data members

        private:
                    string                                                       m_companyName;
                    vector<WaveManagedObjectPointer <EmployeeManagedObject> >    m_employeeObject;
                    vector<WaveManagedObjectPointer <GroupManagedObject> >       m_groupManagedObject;
                    vector<WaveManagedObjectPointer <LocationManagedObject> >    m_locationManagedObject;
        protected:
        public:
    };
}
#endif //COMPANYMANAGEDOBJECT_H

