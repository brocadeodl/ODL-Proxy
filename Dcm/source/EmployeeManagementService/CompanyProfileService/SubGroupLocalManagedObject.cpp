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

#include "EmployeeManagementService/CompanyProfileService/SubGroupLocalManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/LocationManagedObject.h"
#include "EmployeeManagementService/CompanyProfileService/GroupLocalObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/EmployeeManagedObject.h"
#include "companyprofile.h"

namespace DcmNs
{

SubGroupLocalManagedObject::SubGroupLocalManagedObject (GroupLocalObjectManager *pGroupLocalObjectManager)
        : PrismElement               (pGroupLocalObjectManager),
          PrismPersistableObject     (SubGroupLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject          (pGroupLocalObjectManager),
          DcmManagedObject           (pGroupLocalObjectManager),
          WaveLocalManagedObjectBase (this),
          DcmLocalManagedObjectBase  (this),
          DcmLocalManagedObject      (pGroupLocalObjectManager)
{
}

SubGroupLocalManagedObject::~SubGroupLocalManagedObject ()
{
}

string SubGroupLocalManagedObject::getClassName ()
{
    return ("SubGroupLocalManagedObject");
}

void SubGroupLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString(&m_groupName, "subgroupname", companyprofile_subgroupname));
    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_employeeObjectId, "subgroupemplyoyee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_subgroupemployee));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_locationObjectId, "subgrouplocation", LocationManagedObject::getClassName (), true, companyprofile_subgrouplocation));

}

void SubGroupLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString(&m_groupName, "subgroupname", companyprofile_subgroupname));
    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_employeeObjectId, "subgroupemplyoyee", getClassName (), getObjectId (), EmployeeManagedObject::getClassName (), companyprofile_subgroupemployee));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_locationObjectId, "subgrouplocation", LocationManagedObject::getClassName (), true, companyprofile_subgrouplocation));

    vector<string > keyName;
    keyName.push_back ("subgroupname");
    setUserDefinedKeyCombination (keyName);
}

void SubGroupLocalManagedObject::setGroupName (const string &groupName)
{
    m_groupName = groupName;
}

string SubGroupLocalManagedObject::getGroupName () const
{
    return (m_groupName);
}

void SubGroupLocalManagedObject::validateForCreateAtThisNode (ValidateForCreateAtThisNodeContext *pContext)
{
    vector<Attribute *> attributes          = pContext->getAttributes ();
    UI32                numberOfAttributes  = attributes.size ();
    Attribute          *pAttribute          = NULL;

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        pAttribute = attributes [i];

        trace (TRACE_LEVEL_INFO, "SubGroupLocalManagedObject::validateForCreateAtThisNode Attribute Name: " + pAttribute->getAttributeName ());

        if ("subgroupname" == pAttribute->getAttributeName ())
        {
            trace (TRACE_LEVEL_INFO, "creating subgroup");
        }
    }

    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void SubGroupLocalManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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

void SubGroupLocalManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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

void SubGroupLocalManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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

void SubGroupLocalManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["subgroupname"] = 1;
    attributeNamesGroupCodeMap ["subgroupemployee"] = 2;
    attributeNamesGroupCodeMap ["subgrouplocation"] = 3;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";
    groupCodeBackendNamesMap[3] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("subgroupname");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("subgroupemployee");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("subgrouplocation");
    groupCodeAttributeGroupMap[3] = attributeNamesVector;
}

void SubGroupLocalManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{   
    string passName = pContext->getPassName();
    map < UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    
    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;

    vector<string> attributeNames;

    attributeNames.push_back(string("subgroupname"));
    attributeNames.push_back(string("subgrouplocation"));

    groupMap[20]= string("employee_management");
    backendAttributesMap[20] = attributeNames;
    groupCodeChoiceFlag[20] = false;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    trace (TRACE_LEVEL_DEVEL, string("SubGroupLocalManagedObject::getHardwareConfigurationDetailsForPostboot: attributeNames size is ") + attributeNames.size());

    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();
}

}
