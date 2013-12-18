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

#include "EmployeeManagementService/CompanyProfileService/CompanyManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "companyprofile.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

CompanyManagedObject::CompanyManagedObject (CompanyManagementObjectManager *pCompanyManagementObjectManager)
        : PrismElement           (pCompanyManagementObjectManager),
          PrismPersistableObject (CompanyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject      (pCompanyManagementObjectManager),
          DcmManagedObject       (pCompanyManagementObjectManager)
{
}

CompanyManagedObject::~CompanyManagedObject ()
{
}

string CompanyManagedObject::getClassName ()
{
    return ("CompanyManagedObject");
}

void CompanyManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString(&m_companyName, "companyname", companyprofile_companyname));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <EmployeeManagedObject>(&m_employeeObject, "Employee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_employee));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <GroupManagedObject> (&m_groupManagedObject, "Group", getClassName (), getObjectId (), GroupManagedObject::getClassName (), companyprofile_group));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <LocationManagedObject> (&m_locationManagedObject, "Location", getClassName (), getObjectId (), LocationManagedObject::getClassName (), companyprofile_location));
}

void CompanyManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString(&m_companyName, "companyname", companyprofile_companyname));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <EmployeeManagedObject> (&m_employeeObject, "Employee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_employee));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <GroupManagedObject> (&m_groupManagedObject, "Group", getClassName (), getObjectId (), GroupManagedObject::getClassName (), companyprofile_group));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <LocationManagedObject> (&m_locationManagedObject, "Location", getClassName (), getObjectId (), LocationManagedObject::getClassName (), companyprofile_location));

    vector<string > keyName;
    keyName.push_back ("companyname");
    setUserDefinedKeyCombination (keyName);

}

void CompanyManagedObject::setCompanyName (const string &companyName)
{
    m_companyName = companyName;
}

string CompanyManagedObject::getCompanyName () const
{
    return (m_companyName);
}


void CompanyManagedObject::addCompositionToManagedObject (WaveManagedObject *pWaveManagedObject)
{

    if (EmployeeManagedObject::getClassName () == pWaveManagedObject->getClassName ())
    {

    }
    else if (GroupManagedObject::getClassName () == pWaveManagedObject->getClassName ())
    {

    }
    else if (LocationManagedObject::getClassName () == pWaveManagedObject->getClassName ())
    {

    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "No such composition exists");
        prismAssert (false, __FILE__, __LINE__);
    }
}


void CompanyManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    vector<Attribute *>         attributeVector      = pContext->getAttributes ();
    UI32                        numberOfAttribute    = attributeVector.size (); 

    trace (TRACE_LEVEL_INFO, "CompanyManagedObject::addNewAttributeToBeUpdatedForCreate : GroupManagedObject ..............");
    for (UI32 i = 0; i < numberOfAttribute; i++)
    {
        Attribute *pAttribute = attributeVector [i];

        if ("companyname" == pAttribute->getAttributeName ())
        {
            // This case means we are creating company managed object
            GroupManagedObject *pGroupManagedObject = new GroupManagedObject (CompanyManagementObjectManager::getInstance ());
            LocationManagedObject *pLocationManagedObject = new LocationManagedObject (CompanyManagementObjectManager::getInstance ());

            m_groupManagedObject.push_back (WaveManagedObjectPointer< GroupManagedObject > (pGroupManagedObject));
            m_locationManagedObject.push_back (WaveManagedObjectPointer< LocationManagedObject > (pLocationManagedObject));

            pContext->addNewAttributeName ("Group");
            pContext->addNewAttributeName ("Location");
        }
    }

    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}




void CompanyManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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

void CompanyManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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

void CompanyManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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

void CompanyManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["companyname"] = 1;
    attributeNamesGroupCodeMap ["employee"] = 2;
    attributeNamesGroupCodeMap ["group"] = 3;
    attributeNamesGroupCodeMap ["location"] = 4;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";
    groupCodeBackendNamesMap[3] = "employee_management";
    groupCodeBackendNamesMap[4] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("companyname");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("employee");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("group");
    groupCodeAttributeGroupMap[3] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("location");
    groupCodeAttributeGroupMap[4] = attributeNamesVector;
}

void CompanyManagedObject::getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    string passName = pContext->getPassName();
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;

    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;

    vector<string> attributeNames;

    attributeNames.push_back("companyname");
    attributeNames.push_back("Employee");

    groupMap[6] = "employee_management";
    backendAttributesMap[6] = attributeNames;
    groupCodeChoiceFlag[6] = false;

    attributeNames.clear();

    attributeNames.push_back("Group");
    groupMap[7] = "employee_management";
    backendAttributesMap[7] = attributeNames;
    groupCodeChoiceFlag[7] = false;

    attributeNames.clear();

    attributeNames.push_back("Location");
    groupMap[8] = "employee_management"; 
    backendAttributesMap[8] = attributeNames;
    groupCodeChoiceFlag[8] = false;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    trace (TRACE_LEVEL_DEVEL, string("CompanyManagedObject::getHardwareConfigurationDetailsForPostboot: BackEndMap size is ") + backendAttributesMap.size() + " groupMap size is " + groupMap.size() + " passName is " + passName);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();

}

bool CompanyManagedObject::getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{
    prismServiceId = FrameworkToolKit::getServiceIdByName (CompanyManagementLocalObjectManager::getClassName ());
    return (true);
}

}

