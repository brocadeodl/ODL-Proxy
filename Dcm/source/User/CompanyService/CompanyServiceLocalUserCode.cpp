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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : apurva                                                     *
 **************************************************************************/


#include "CompanyService.h"
#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService/Local/CompanyServiceLocalStockOptionManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
/* Generating the UserCode */

namespace DcmNs
{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    /*
    if (pContext->getIsAssociatedMOSingleton ())
    {
    pContext->setAssociatedAttributeName("isStockOption");
    trace (TRACE_LEVEL_INFO, string ("CompanyServiceStockExchangeManagedObject::isAssociatedManagedObjectToBeCreated==true:: getAttributeName()") + pContext->getAssociatedAttributeName());
    Attribute *attribute1 = Attribute::getAttributeFromAttributeType(WAVE_ATTRIBUTE_TYPE_BOOL);
    attribute1->setAttributeName("employeeStockOption");
    attribute1->fromString(pContext->getAssociatedAttributeValue ());
    pContext->addToAttributeVector    (attribute1);
    }
    */
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["employeelocation"] = COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION;
        vectorOfAttributes.push_back("employeelocation");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["employeegroup"] = COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP;
        vectorOfAttributes.push_back("employeegroup");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["employeeStockOption"] = COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION;
        vectorOfAttributes.push_back("employeeStockOption");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["employeename"] = COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME;
        vectorOfAttributes.push_back("employeename");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("employeelocation"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("employeegroup"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("employeeStockOption"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("employeename"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupEmployee"] = COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE;
        vectorOfAttributes.push_back("groupEmployee");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupLocation"] = COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION;
        vectorOfAttributes.push_back("groupLocation");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["subgroup"] = COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP;
        vectorOfAttributes.push_back("subgroup");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupname"] = COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME;
        vectorOfAttributes.push_back("groupname");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupmanager"] = COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER;
        vectorOfAttributes.push_back("groupmanager");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupEmployee"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupLocation"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("subgroup"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupname"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupmanager"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupEmployee"] = COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE;
        vectorOfAttributes.push_back("groupEmployee");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupLocation"] = COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION;
        vectorOfAttributes.push_back("groupLocation");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["subgroupname"] = COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME;
        vectorOfAttributes.push_back("subgroupname");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["subgroupmanager"] = COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER;
        vectorOfAttributes.push_back("subgroupmanager");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupEmployee"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupLocation"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("subgroupname"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("subgroupmanager"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    /*
    if ( 0 == numberOfAttributes )
    {
        stockOptionPresent (true);
        pContext->addNewAttributeName("stockOptionPresent");
    }
    */
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    /*
    if ( 0 == numberOfAttributes )
    {
        stockOptionPresent (false);
        pContext->addNewAttributeName("stockOptionPresent");
    }
    */
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["isStockOption"] = COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION;
        vectorOfAttributes.push_back("isStockOption");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["stockshare"] = COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE;
        vectorOfAttributes.push_back("stockshare");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE] = vectorOfAttributes;
    }

//Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["stockOptionPresent"] = COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT;
        vectorOfAttributes.push_back("stockOptionPresent");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("isStockOption"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("stockshare"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("stockOptionPresent"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT] = false;
    }   
    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getManagedObjectUserTag: Get the userTag for single ton MO
 * @uBreezeClose
 */
UI32 CompanyServiceLocalStockOptionManagedObject::getManagedObjectUserTag () const
{
    return (companyservice_isStockOption);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["groupmanagername"] = COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME;
        vectorOfAttributes.push_back("groupmanagername");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("groupmanagername"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = true;
    PrismMessage *pPrismMessage = NULL;/* Add the Message Constructor as needed*/
    pContext->setMessage(pPrismMessage);
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    //pContext->setClientName ("Backend Name");
    map <string, UI32> attributeNamesGroupCodeMap;
    map <UI32, string> groupCodeBackendNamesMap;
    map <UI32, vector <string> > groupCodeAttributeGroupMap;
    setHardwareConfigurationDetails(attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap);
    updateBackend = true;
    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool updateBackend = false;
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    map<UI32, bool> groupTypeMap;
    map<UI32, map<UI32, vector<UI32> > > choiceToCasesMap;
    setPostbootHardwareConfigurationDetails(backendAttributesMap, groupMap, groupTypeMap, choiceToCasesMap);
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupTypeMap );
    pContext->setChoiceCaseGroup( choiceToCasesMap );
    updateBackend = true;
    pContext->setIsUpdateBackend (updateBackend);
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
{
    static map <string, UI32> s_attributeNamesGroupCodeMap;
    static map <UI32, string> s_groupCodeBackendNamesMap;
    static map <UI32, vector <string> > s_groupCodeAttributeGroupMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        groupCodeBackendNamesMap = s_groupCodeBackendNamesMap;
        attributeNamesGroupCodeMap = s_attributeNamesGroupCodeMap;
        groupCodeAttributeGroupMap = s_groupCodeAttributeGroupMap;
        return;
    }
    //Adding the Group realted details for backend
    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["subgroupmanagername"] = COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME;
        vectorOfAttributes.push_back("subgroupmanagername");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
        map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap)
{
    static map <UI32, vector <string> > s_backendAttributesMap;
    static map<UI32, string > s_groupMap;
    static map<UI32, bool> s_groupTypeMap;
    static map<UI32, map<UI32, vector<UI32> > > s_choiceToCasesMap;
    static bool s_isMapPopulated = false;
    if ( true == s_isMapPopulated )
    {
        backendAttributesMap = s_backendAttributesMap;
        groupCodeMap = s_groupMap;
        groupTypeMap = s_groupTypeMap;
        choiceToCasesMap = s_choiceToCasesMap;
        return;
    }
    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("subgroupmanagername"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


}

