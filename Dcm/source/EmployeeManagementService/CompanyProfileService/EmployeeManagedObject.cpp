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

#include "EmployeeManagementService/CompanyProfileService/EmployeeManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/GroupManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/LocationManagedObject.h"
#include "companyprofile.h"

namespace DcmNs
{

EmployeeManagedObject::EmployeeManagedObject (CompanyManagementObjectManager *pCompanyManagementObjectManager)
        : PrismElement           (pCompanyManagementObjectManager),
          PrismPersistableObject (EmployeeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject      (pCompanyManagementObjectManager),
          DcmManagedObject       (pCompanyManagementObjectManager)
{
}

EmployeeManagedObject::~EmployeeManagedObject ()
{
}

string EmployeeManagedObject::getClassName ()
{
    return ("EmployeeManagedObject");
}

void EmployeeManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_employeeName, "employeename", companyprofile_employeename));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_groupObjectId, "groupObjectId", GroupManagedObject::getClassName (), true, companyprofile_employeegroup));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_locationObjectId, "locationObjectId", LocationManagedObject::getClassName (), true, companyprofile_employeelocation));
}

void EmployeeManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_employeeName, "employeename", companyprofile_employeename));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_groupObjectId, "groupObjectId", GroupManagedObject::getClassName (), true, companyprofile_employeegroup));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_locationObjectId, "locationObjectId", LocationManagedObject::getClassName (), true, companyprofile_employeelocation));

    vector<string > keyName;
    keyName.push_back ("employeename");
    setUserDefinedKeyCombination (keyName);
}

void EmployeeManagedObject::setEmployeeName (const string &employeeName)
{
    m_employeeName = employeeName;
}

string EmployeeManagedObject::getEmployeeName () const
{
    return (m_employeeName);
}

void EmployeeManagedObject::setGroupObjectId (const ObjectId &groupObjectId)
{
    m_groupObjectId = groupObjectId;
}

ObjectId EmployeeManagedObject::getGroupObjectId () const
{
    return (m_groupObjectId);
}

void EmployeeManagedObject::setLocationObjectId (const ObjectId &locationObjectId)
{
    m_locationObjectId = locationObjectId;
}

ObjectId EmployeeManagedObject::getLocationObjectId () const
{
    return (m_locationObjectId);
}

void EmployeeManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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

void EmployeeManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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

void EmployeeManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);

    pContext->setIsUpdateBackend (false);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void EmployeeManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    pContext->setMessage(NULL);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->setIsAssociatedMOToBeCreated (true);
    pContext->callback ();
}

void EmployeeManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["employeename"] = 1;
    attributeNamesGroupCodeMap ["groupObjectId"] = 2;
    attributeNamesGroupCodeMap ["locationObjectId"] = 3;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";
    groupCodeBackendNamesMap[3] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("employeename");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("groupObjectId");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("locationObjectId");
    groupCodeAttributeGroupMap[3] = attributeNamesVector;
}

void EmployeeManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    string passName = pContext->getPassName();
    map < UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    
    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;

    vector<string> attributeNames;

    attributeNames.push_back("employeename");
    attributeNames.push_back("groupObjectId");
    attributeNames.push_back("locationObjectId");
    
    groupMap[10] = "employee_management";
    backendAttributesMap[10] = attributeNames;
    groupCodeChoiceFlag[10] = false;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    trace (TRACE_LEVEL_INFO, string("EmployeeManagedObject::getHardwareConfigurationDetailsForPostboot: attributeNames size is ") + attributeNames.size());
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();

}
}
