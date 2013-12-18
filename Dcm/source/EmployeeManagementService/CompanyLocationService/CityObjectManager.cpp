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

#include "EmployeeManagementService/CompanyLocationService/CityObjectManager.h"
#include "EmployeeManagementService/CompanyLocationService/CityManagedObject.h"
#include "ManagementInterface/ClientInterface/UnifiedClientBackendDetails.h"

namespace DcmNs
{

CityObjectManager::CityObjectManager ()
    : WaveObjectManager  (getClassName ())
{

    CityManagedObject cityManagedObject (this);
    cityManagedObject.setupOrm ();
    addManagedClass (CityManagedObject::getClassName ());

    populatePostbootMap();

    UnifiedClientBackendDetails *backendDetails  = new UnifiedClientBackendDetails();
    string ClientName = "TEST_CLIENT";
    UI32  backendValue = 5;
    backendDetails->setClientName(ClientName);
    backendDetails->setBackendNotifyValue(backendValue);
    map<string, map <string , UnifiedClientBackendDetails *> > shutdownPhaseAttributeMap;
    map<string,UnifiedClientBackendDetails *>  shutdownAtt;
    shutdownAtt["notify"] = backendDetails; 
    shutdownPhaseAttributeMap["shutdown"] = shutdownAtt;
    WaveObjectManager::setBackendAttributeMap(shutdownPhaseAttributeMap);
}

CityObjectManager::~CityObjectManager ()
{
}

CityObjectManager  *CityObjectManager::getInstance ()
{
    static CityObjectManager *pCityObjectManager = new CityObjectManager ();

    WaveNs::prismAssert (NULL != pCityObjectManager, __FILE__, __LINE__);

    return (pCityObjectManager);
}

string  CityObjectManager::getClassName ()
{
    return ("City");
}

PrismServiceId  CityObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage  *CityObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
         default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject  *CityObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (CityManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new CityManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "CityObjectManager::createManagedObjectInstanc Unsupported Class Name " + managedClassName);
    }

    return pWaveManagedObject;
}

void CityObjectManager::populatePostbootMap()
{
    map<string, vector<string> > postbootManagedObjectNames;
    vector<string> managedObjectNames;
    managedObjectNames.push_back("CityManagedObject");
    postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE4"]= managedObjectNames;
    setPostbootMap( postbootManagedObjectNames );
    return;
}

}

