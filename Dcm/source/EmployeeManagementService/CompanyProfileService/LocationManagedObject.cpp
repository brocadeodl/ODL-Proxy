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

#include "EmployeeManagementService/CompanyProfileService/LocationManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyLocationService/CityManagedObject.h"
#include "companyprofile.h"

namespace DcmNs
{

LocationManagedObject::LocationManagedObject (CompanyManagementObjectManager *pCompanyManagementObjectManager)
        : PrismElement           (pCompanyManagementObjectManager),
          PrismPersistableObject (LocationManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject      (pCompanyManagementObjectManager),
          DcmManagedObject       (pCompanyManagementObjectManager)
{
}

LocationManagedObject::~LocationManagedObject ()
{
}

string LocationManagedObject::getClassName ()
{
    return ("LocationManagedObject");
}

void LocationManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString              (&m_locationName, "locationname", companyprofile_locationname));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_cityObjectId, "locationcityname", CityManagedObject::getClassName (), true, companyprofile_locationcityname));
}

void LocationManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString              (&m_locationName, "locationname", companyprofile_locationname));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_cityObjectId, "locationcityname", CityManagedObject::getClassName (), true, companyprofile_locationcityname));

    vector<string > keyName;
    keyName.push_back ("locationname");
    setUserDefinedKeyCombination (keyName);
}

void LocationManagedObject::setCityObjectId (const ObjectId &cityObjectId)
{
    m_cityObjectId = cityObjectId;
}

ObjectId LocationManagedObject::getCityObjectId () const
{
    return (m_cityObjectId);
}

void LocationManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void LocationManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void LocationManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void LocationManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    pContext->setMessage(NULL);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->setIsAssociatedMOToBeCreated (true);
    pContext->callback ();
}

void LocationManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["locationname"] = 1;
    attributeNamesGroupCodeMap ["locationcityname"] = 2;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("locationname");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("locationcityname");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;

}

void LocationManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{   
    string passName = pContext->getPassName();
    map < UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;

    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;

    vector<string> attributeNames;

    attributeNames.push_back(string("locationname"));
    attributeNames.push_back(string("locationcityname"));

    groupMap[8] = string("employee_management");
    backendAttributesMap[8] = attributeNames;
    groupCodeChoiceFlag[8] = false;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    trace (TRACE_LEVEL_DEVEL, string("LocationManagedObject::getHardwareConfigurationDetailsForPostboot: attributeNames size is ") + attributeNames.size());
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();

}
}

