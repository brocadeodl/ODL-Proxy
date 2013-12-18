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

#include "EmployeeManagementService/CompanyProfileService/GroupManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/EmployeeManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/SubGroupLocalManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/LocationManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "companyprofile.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

GroupManagedObject::GroupManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement           (pWaveObjectManager),
          PrismPersistableObject (GroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject      (pWaveObjectManager),
          DcmManagedObject       (pWaveObjectManager)
{
}

GroupManagedObject::~GroupManagedObject ()
{
}

string GroupManagedObject::getClassName ()
{
    return ("GroupManagedObject");
}

void GroupManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString(&m_groupName, "groupname", companyprofile_groupname));
    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_employeeObjectId, "groupemployee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_groupemployee));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <SubGroupLocalManagedObject> (&m_subGroupManagedObject, "subgroup", getClassName (), getObjectId (), SubGroupLocalManagedObject::getClassName (), companyprofile_subgroup));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_locationObjectId, "grouplocation", LocationManagedObject::getClassName (), true, companyprofile_grouplocation));
}

void GroupManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString(&m_groupName, "groupname", companyprofile_groupname));
    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_employeeObjectId, "groupemployee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_groupemployee));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <SubGroupLocalManagedObject> (&m_subGroupManagedObject, "subgroup", getClassName (), getObjectId (), SubGroupLocalManagedObject::getClassName (), companyprofile_subgroup));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_locationObjectId, "grouplocation", LocationManagedObject::getClassName (), true, companyprofile_grouplocation));

    vector<string > keyName;
    keyName.push_back ("groupname");
    setUserDefinedKeyCombination (keyName);
}

void GroupManagedObject::setGroupName (const string &groupName)
{
    m_groupName = groupName;
}

string GroupManagedObject::getGroupName () const
{
    return (m_groupName);
}

/*
void GroupManagedObject::addEmployeeInGroup (const ObjectId &employeeObjectId)
{
    m_employeeObjectId.push_back (employeeObjectId);
}

void GroupManagedObject::deleteEmployeeFromGroup (const ObjectId &employeeObjectId)
{
    vector<ObjectId>::iterator    employeeId;

    employeeId = find (m_employeeObjectId.begin (), m_employeeObjectId.end (), employeeObjectId);

    if (employeeId != m_employeeObjectId.end ())
    {
        m_employeeObjectId.erase (employeeId);
    }
}

*/

void GroupManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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

void GroupManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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

void GroupManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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

void GroupManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["groupname"] = 1;
    attributeNamesGroupCodeMap ["groupemployee"] = 2;
    attributeNamesGroupCodeMap ["grouplocation"] = 3;
    attributeNamesGroupCodeMap ["subgroup"] = 4;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";
    groupCodeBackendNamesMap[3] = "employee_management";
    groupCodeBackendNamesMap[4] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("groupname");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("groupemployee");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("grouplocation");
    groupCodeAttributeGroupMap[3] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("subgroup");
    groupCodeAttributeGroupMap[4] = attributeNamesVector;
}

void GroupManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{   
    string passName = pContext->getPassName();
    map < UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    
    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;

    vector<string> attributeNames;        

    attributeNames.push_back("groupname");
    //attributeNames.push_back("grouplocation");
    groupMap[11]= ("employee_management");
    backendAttributesMap[11] = attributeNames;
    groupCodeChoiceFlag[11] = false;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    trace (TRACE_LEVEL_DEVEL, string("GroupManagedObject::getHardwareConfigurationDetailsForPostboot: attributeNames size is ") + attributeNames.size());
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();
}

bool GroupManagedObject::getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{
    prismServiceId = FrameworkToolKit::getServiceIdByName (CompanyManagementLocalObjectManager::getClassName ());
    return (true);
}

}
