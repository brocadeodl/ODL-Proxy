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
#include "CompanyService/Global/CompanyServiceBoardOfDirectorManagedObject.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "CompanyService/Global/CompanyServiceCompanyManagedObject.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService/Global/CompanyServiceTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

/* Generating the UserCode */

namespace DcmNs
{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:CompanyServiceCityManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityname"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME;
        vectorOfAttributes.push_back("cityname");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["citycode"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE;
        vectorOfAttributes.push_back("citycode");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_POPULATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["population"] = COMPANYSERVICE_GROUP_CODE_CITY_POPULATION;
        vectorOfAttributes.push_back("population");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_POPULATION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ismetro"] = COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO;
        vectorOfAttributes.push_back("ismetro");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityIpV4Address"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS;
        vectorOfAttributes.push_back("cityIpV4Address");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityIpV4AddressNetworkMask"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK;
        vectorOfAttributes.push_back("cityIpV4AddressNetworkMask");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityIpV6Address"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS;
        vectorOfAttributes.push_back("cityIpV6Address");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityIpV6AddressNetworkMask"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK;
        vectorOfAttributes.push_back("cityIpV6AddressNetworkMask");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["cityIpVxAddress"] = COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS;
        vectorOfAttributes.push_back("cityIpVxAddress");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("cityname"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("citycode"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("population"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_POPULATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_POPULATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_POPULATION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ismetro"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("cityIpV4Address"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("cityIpV4AddressNetworkMask"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("cityIpV6Address"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("cityIpV6AddressNetworkMask"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("cityIpVxAddress"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getPluginDetailsForDistribution: Distribute the plugin configuration 
 * @uBreezeClose
 */
bool CompanyServiceCompanyManagedObject::getPluginDetailsForDistribution(ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{
trace (TRACE_LEVEL_INFO, "CompanyServiceCompanyManagedObject::getPluginDetailsForDistribution inside ");
    prismServiceId = FrameworkToolKit::getServiceIdByName("CompanyServiceLocalObjectManager");
    isNeedSurrogateSupportFlag = false;
    isPartialSuccessFlag = true;
    /* Please add the Location IDS as needed  */
    return (true);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:CompanyServiceCompanyManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["companyname"] = COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME;
        vectorOfAttributes.push_back("companyname");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["employee"] = COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE;
        vectorOfAttributes.push_back("employee");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["location"] = COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION;
        vectorOfAttributes.push_back("location");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["region"] = COMPANYSERVICE_GROUP_CODE_CHOICE_REGION;
        vectorOfAttributes.push_back("region");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_REGION_EMEA] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["region_EMEA_country"] = COMPANYSERVICE_GROUP_CODE_REGION_EMEA;
        vectorOfAttributes.push_back("region_EMEA_country");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_REGION_EMEA] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["region_AsiaPac_country"] = COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC;
        vectorOfAttributes.push_back("region_AsiaPac_country");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["region_Americas_country"] = COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS;
        vectorOfAttributes.push_back("region_Americas_country");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["group"] = COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP;
        vectorOfAttributes.push_back("group");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["boardOfDirector"] = COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR;
        vectorOfAttributes.push_back("boardOfDirector");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("companyname"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("employee"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("location"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("region"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("region_EMEA_country"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_REGION_EMEA] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_REGION_EMEA] = "employee_management";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(COMPANYSERVICE_GROUP_CODE_CHOICE_REGION);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(companyservice_EMEA);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(COMPANYSERVICE_GROUP_CODE_REGION_EMEA);
        userTagToGroupIdsMap[companyservice_EMEA] = groupIds;
        choiceToCasesMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("region_AsiaPac_country"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC] = "employee_management";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(COMPANYSERVICE_GROUP_CODE_CHOICE_REGION);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(companyservice_AsiaPac);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC);
        userTagToGroupIdsMap[companyservice_AsiaPac] = groupIds;
        choiceToCasesMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("region_Americas_country"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS] = "employee_management";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(COMPANYSERVICE_GROUP_CODE_CHOICE_REGION);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(companyservice_Americas);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS);
        userTagToGroupIdsMap[companyservice_Americas] = groupIds;
        choiceToCasesMap[COMPANYSERVICE_GROUP_CODE_CHOICE_REGION] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("group"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("boardOfDirector"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}

/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["boardOfDirectorname"] = COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME;
        vectorOfAttributes.push_back("boardOfDirectorname");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION] = "employee_management";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["boardOfDirectorlocation"] = COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION;
        vectorOfAttributes.push_back("boardOfDirectorlocation");
        groupCodeAttributeGroupMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("boardOfDirectorname"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("boardOfDirectorlocation"));
        backendAttributesMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION] = attributeNames;

        groupCodeMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION] = "employee_management";
        groupTypeMap[COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}



}

