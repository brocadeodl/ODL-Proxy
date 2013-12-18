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
 *   Author : statpatt                                                     *
 **************************************************************************/


#include "Framework/Utils/FrameworkToolKit.h"
#include "Udld/Global/UdldGlobalConfigManagedObject.h"
#include "Udld/Global/UdldGlobalObjectManager.h"
#include "Udld/Global/UdldGlobalTypes.h"
#include "Nsm/Local/UdldIntfConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "brocade-udld.h"
/* Generating the UserCode */

namespace DcmNs
{


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::updateHardwareBeforeWaveManagedObject: Do we need to update the hardware before MO 
 * @uBreezeClose
 */
bool UdldGlobalConfigManagedObject::updateHardwareBeforeWaveManagedObject (const vector<Attribute *> &attributeVector)
{
    return (true);
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getPluginDetailsForDistribution: Distribute the plugin configuration 
 * @uBreezeClose
 */
bool UdldGlobalConfigManagedObject::getPluginDetailsForDistribution(ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{
    prismServiceId = FrameworkToolKit::getServiceIdByName("UdldLocalObjectManager");
    isNeedSurrogateSupportFlag = false;
    isPartialSuccessFlag = true;
    /* Please add the Location IDS as needed  */
    return (true);
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    bool isAssociatedMOToBeCreated = false;
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
 * Brief:UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    if ( 0 == numberOfAttributes )
    {
        setUdldPresent (true);
        pContext->addNewAttributeName("UdldPresent");
    }
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
 * Brief:UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    if ( 0 == numberOfAttributes )
    {
        setUdldPresent (false);
        pContext->addNewAttributeName("UdldPresent");
    }
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
 * Brief:UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:UdldGlobalConfigManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[UDLD_GROUP_CODE_PROTO_UDLD] = "UdldDCMClient";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["UdldPresent"] = UDLD_GROUP_CODE_PROTO_UDLD;
        vectorOfAttributes.push_back("UdldPresent");
        groupCodeAttributeGroupMap[UDLD_GROUP_CODE_PROTO_UDLD] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[UDLD_GROUP_CODE_HELLO] = "UdldDCMClient";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["Hello"] = UDLD_GROUP_CODE_HELLO;
        vectorOfAttributes.push_back("Hello");
        groupCodeAttributeGroupMap[UDLD_GROUP_CODE_HELLO] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[UDLD_GROUP_CODE_MULTIPLIER] = "UdldDCMClient";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["Multiplier"] = UDLD_GROUP_CODE_MULTIPLIER;
        vectorOfAttributes.push_back("Multiplier");
        groupCodeAttributeGroupMap[UDLD_GROUP_CODE_MULTIPLIER] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[UDLD_GROUP_CODE_SHUTDOWN] = "UdldDCMClient";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["Shutdown"] = UDLD_GROUP_CODE_SHUTDOWN;
        vectorOfAttributes.push_back("Shutdown");
        groupCodeAttributeGroupMap[UDLD_GROUP_CODE_SHUTDOWN] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("UdldPresent"));
        backendAttributesMap[UDLD_GROUP_CODE_PROTO_UDLD] = attributeNames;

        groupCodeMap[UDLD_GROUP_CODE_PROTO_UDLD] = "UdldDCMClient";
        groupTypeMap[UDLD_GROUP_CODE_PROTO_UDLD] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("Hello"));
        backendAttributesMap[UDLD_GROUP_CODE_HELLO] = attributeNames;

        groupCodeMap[UDLD_GROUP_CODE_HELLO] = "UdldDCMClient";
        groupTypeMap[UDLD_GROUP_CODE_HELLO] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("Multiplier"));
        backendAttributesMap[UDLD_GROUP_CODE_MULTIPLIER] = attributeNames;

        groupCodeMap[UDLD_GROUP_CODE_MULTIPLIER] = "UdldDCMClient";
        groupTypeMap[UDLD_GROUP_CODE_MULTIPLIER] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("Shutdown"));
        backendAttributesMap[UDLD_GROUP_CODE_SHUTDOWN] = attributeNames;

        groupCodeMap[UDLD_GROUP_CODE_SHUTDOWN] = "UdldDCMClient";
        groupTypeMap[UDLD_GROUP_CODE_SHUTDOWN] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getManagedObjectUserTag: Get the userTag for single ton MO
 * @uBreezeClose
 */
UI32 UdldGlobalConfigManagedObject::getManagedObjectUserTag () const
{
    return (udld_udld);
}

void UdldGlobalConfigManagedObject::isManagedObjectsAssociatedToCurrentMONeedsToBeDeleted (WaveManagedObjectDeleteContext *pContext)
{
    tracePrintf(TRACE_LEVEL_INFO, "UdldGlobalConfigManagedObject::isManagedObjectsAssociatedToCurrentMONeedsToBeDeleted : Notifying NSM thru UdldIntfConfigMessage, opCode=NSM_UDLD_RESET_ALL");
    UdldIntfConfigMessage *m = new UdldIntfConfigMessage(NSM_UDLD_RESET_ALL);
    m->setNeedSurrogateSupportFlag(true);
    pContext->setMessage(m);
}



}

