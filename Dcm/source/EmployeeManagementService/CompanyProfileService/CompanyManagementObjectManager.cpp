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

#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagedObject.h"

namespace DcmNs
{

CompanyManagementObjectManager::CompanyManagementObjectManager ()
    : WaveObjectManager  (getClassName ())
{

    CompanyManagedObject companyManagedObject (this);
    companyManagedObject.setupOrm ();
    addManagedClass (CompanyManagedObject::getClassName ());

    EmployeeManagedObject employeeManagedObject (this);
    employeeManagedObject.setupOrm ();
    addManagedClass (EmployeeManagedObject::getClassName ());

    GroupManagedObject groupManagedObject (this);
    groupManagedObject.setupOrm ();
    addManagedClass (GroupManagedObject::getClassName ());

    LocationManagedObject locationManagedObject (this);
    locationManagedObject.setupOrm ();
    addManagedClass (LocationManagedObject::getClassName ());

    map<string, vector<string> > postbootManagedObjectNames;
    vector<string> managedObjectNames;
    managedObjectNames.push_back("CompanyManagedObject");
    postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE1"]= managedObjectNames;
    postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE2"]= managedObjectNames;

    setPostbootMap( postbootManagedObjectNames );    
}

CompanyManagementObjectManager::~CompanyManagementObjectManager ()
{
}

CompanyManagementObjectManager  *CompanyManagementObjectManager::getInstance ()
{
    static CompanyManagementObjectManager *pCompanyManagementObjectManager = new CompanyManagementObjectManager ();

    WaveNs::prismAssert (NULL != pCompanyManagementObjectManager, __FILE__, __LINE__);

    return (pCompanyManagementObjectManager);
}

string  CompanyManagementObjectManager::getClassName ()
{
    return ("CompanyManagement");
}

PrismServiceId  CompanyManagementObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage  *CompanyManagementObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
         default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject  *CompanyManagementObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (CompanyManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new CompanyManagedObject (this);
    }
    else if (GroupManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new GroupManagedObject (this);
    }
    else if (EmployeeManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new EmployeeManagedObject (this);
    }
    else if (LocationManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new LocationManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "CompanyManagementObjectManager::createManagedObjectInstanc Unsupported Class Name " + managedClassName);
    }

    return pWaveManagedObject;
}

}
