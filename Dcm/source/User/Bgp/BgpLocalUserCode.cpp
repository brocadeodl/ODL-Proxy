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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborIpAddrManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NetworkManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4UcastManagedObject.h"
#include "Bgp/Local/BgpLocalAggregateIpv4AddressManagedObject.h"
#include "Bgp/Local/BgpLocalBasicConfigManagedObject.h"
#include "Bgp/Local/BgpLocalIpv4StaticNetworkManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "brocade-bgp.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/Layer3LicenseUtil.h"
#include "User/Bgp/BgpUtils.h"
/* Generating the UserCode */

namespace DcmNs
{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::validateForCreateAtThisNode: Add the validation function for Create
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::validateForCreateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes         = pContext->getAttributes ();
    UI32               numberOfAttributes = attributes.size ();
    trace(TRACE_LEVEL_INFO, string("BgpLocalBasicConfigManagedObject::validateForCreateAtThisNode : called with attributes count ") + numberOfAttributes );
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        Attribute* pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());
    	if(pAttribute->getAttributeName()== "vrfName") { 
			trace (TRACE_LEVEL_INFO, string("BgpLocalUserCode. Check license"));
			if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
				trace (TRACE_LEVEL_INFO, string("BgpLocalUserCode. LAYER_3_LICENSE not present"));
				status = WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR;
			}
        }
	}

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */

    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
	string localAsString;
	AttributeString *localAsAttributeString;
	
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if(pAttribute->getAttributeName()== "localAsNum") {
			localAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
			if (localAsAttributeString != NULL)
			{
			  	localAsString = localAsAttributeString->getValue();
				localAsString = BgpUtils::getASinIntegerFormat(localAsString);
				trace (TRACE_LEVEL_INFO, string("BgpLocalBasicConfigManagedObject::localAsString=")+localAsString);
				if (BgpUtils::isValidAsNum(localAsString))
				{
					setlocalAsNum(localAsString);
					localAsAttributeString->fromString(localAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForUpdate incorrect AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
			}
			else
			{
				trace (TRACE_LEVEL_ERROR, string("BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForUpdate dynamic_cast error"));
				status = WAVE_MESSAGE_ERROR;
			}
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalBasicConfigManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_VRF_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["vrfName"] = BGP_GROUP_CODE_VRF_NAME;
        vectorOfAttributes.push_back("vrfName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_VRF_NAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_LOCAL_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["localAsNum"] = BGP_GROUP_CODE_LOCAL_AS;
        vectorOfAttributes.push_back("localAsNum");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_LOCAL_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_ALWAYS_COMPARE_MED] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["alwaysCompareMedFlag"] = BGP_GROUP_CODE_ALWAYS_COMPARE_MED;
        vectorOfAttributes.push_back("alwaysCompareMedFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_ALWAYS_COMPARE_MED] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AS_PATH_IGNORE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asPathIgnoreFlag"] = BGP_GROUP_CODE_AS_PATH_IGNORE;
        vectorOfAttributes.push_back("asPathIgnoreFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AS_PATH_IGNORE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_CAPABILITY_AS4] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["capabilityAs4Flag"] = BGP_GROUP_CODE_CAPABILITY_AS4;
        vectorOfAttributes.push_back("capabilityAs4Flag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_CAPABILITY_AS4] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceClusterId"] = BGP_GROUP_CODE_CHOICE_CLUSTER_ID;
        vectorOfAttributes.push_back("choiceClusterId");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_CLUSTER_ID] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["clusterId"] = BGP_GROUP_CODE_CLUSTER_ID;
        vectorOfAttributes.push_back("clusterId");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_CLUSTER_ID] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_CLUSTER_IP_ADDR] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["clusterIdIpAddress"] = BGP_GROUP_CODE_CLUSTER_IP_ADDR;
        vectorOfAttributes.push_back("clusterIdIpAddress");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_CLUSTER_IP_ADDR] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_COMPARE_MED_ASPATH] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["compareMedEmptyAsPathFlag"] = BGP_GROUP_CODE_COMPARE_MED_ASPATH;
        vectorOfAttributes.push_back("compareMedEmptyAsPathFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_COMPARE_MED_ASPATH] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_COMPARE_ROUTERID] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["compareRouterIdFlag"] = BGP_GROUP_CODE_COMPARE_ROUTERID;
        vectorOfAttributes.push_back("compareRouterIdFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_COMPARE_ROUTERID] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultLocalPreference"] = BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE;
        vectorOfAttributes.push_back("defaultLocalPreference");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_DISTANCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["extRoutedistance"] = BGP_GROUP_CODE_DISTANCE;
        vectorOfAttributes.push_back("extRoutedistance");
        attributeNamesGroupCodeMap["intRouteDistance"] = BGP_GROUP_CODE_DISTANCE;
        vectorOfAttributes.push_back("intRouteDistance");
        attributeNamesGroupCodeMap["lclRouteDistance"] = BGP_GROUP_CODE_DISTANCE;
        vectorOfAttributes.push_back("lclRouteDistance");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_DISTANCE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_ENFORCE_FIRST_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["enforceFirstAsFlag"] = BGP_GROUP_CODE_ENFORCE_FIRST_AS;
        vectorOfAttributes.push_back("enforceFirstAsFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_ENFORCE_FIRST_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["fastExternalFallOverFlag"] = BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER;
        vectorOfAttributes.push_back("fastExternalFallOverFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_INSTALL_IGP_COST] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["installIgpCostFlag"] = BGP_GROUP_CODE_INSTALL_IGP_COST;
        vectorOfAttributes.push_back("installIgpCostFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_INSTALL_IGP_COST] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_LOG_DAMPENING_DEBUG] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["logDampeningDebug"] = BGP_GROUP_CODE_LOG_DAMPENING_DEBUG;
        vectorOfAttributes.push_back("logDampeningDebug");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_LOG_DAMPENING_DEBUG] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_MAX_AS_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["maxAsLimit"] = BGP_GROUP_CODE_MAX_AS_LIMIT;
        vectorOfAttributes.push_back("maxAsLimit");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_MAX_AS_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_MED_MISSING_AS_WORST] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["medMissingAsWorstFlag"] = BGP_GROUP_CODE_MED_MISSING_AS_WORST;
        vectorOfAttributes.push_back("medMissingAsWorstFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_MED_MISSING_AS_WORST] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_TIMERS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routerBgpModeTimersKeepAliveTime"] = BGP_GROUP_CODE_TIMERS;
        vectorOfAttributes.push_back("routerBgpModeTimersKeepAliveTime");
        attributeNamesGroupCodeMap["routerBgpModeTimerHoldTime"] = BGP_GROUP_CODE_TIMERS;
        vectorOfAttributes.push_back("routerBgpModeTimerHoldTime");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_TIMERS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routerBgpNeighborPeerGrp"] = BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP;
        vectorOfAttributes.push_back("routerBgpNeighborPeerGrp");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routerBgpNeighborIpxAddr"] = BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR;
        vectorOfAttributes.push_back("routerBgpNeighborIpxAddr");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routerBgpAddressIpv4Uc"] = BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST;
        vectorOfAttributes.push_back("routerBgpAddressIpv4Uc");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("vrfName"));
        backendAttributesMap[BGP_GROUP_CODE_VRF_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_VRF_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_VRF_NAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("localAsNum"));
        backendAttributesMap[BGP_GROUP_CODE_LOCAL_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_LOCAL_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_LOCAL_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("alwaysCompareMedFlag"));
        backendAttributesMap[BGP_GROUP_CODE_ALWAYS_COMPARE_MED] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_ALWAYS_COMPARE_MED] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_ALWAYS_COMPARE_MED] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asPathIgnoreFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AS_PATH_IGNORE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AS_PATH_IGNORE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AS_PATH_IGNORE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("capabilityAs4Flag"));
        backendAttributesMap[BGP_GROUP_CODE_CAPABILITY_AS4] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_CAPABILITY_AS4] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_CAPABILITY_AS4] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceClusterId"));
        backendAttributesMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("clusterId"));
        backendAttributesMap[BGP_GROUP_CODE_CLUSTER_ID] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_CLUSTER_ID] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_CHOICE_CLUSTER_ID);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_cluster_id);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_CLUSTER_ID);
        userTagToGroupIdsMap[bgp_ca_cluster_id] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("clusterIdIpAddress"));
        backendAttributesMap[BGP_GROUP_CODE_CLUSTER_IP_ADDR] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_CLUSTER_IP_ADDR] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_CHOICE_CLUSTER_ID);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_cluster_id_ipv4_address);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_CLUSTER_IP_ADDR);
        userTagToGroupIdsMap[bgp_ca_cluster_id_ipv4_address] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_CHOICE_CLUSTER_ID] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("compareMedEmptyAsPathFlag"));
        backendAttributesMap[BGP_GROUP_CODE_COMPARE_MED_ASPATH] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_COMPARE_MED_ASPATH] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_COMPARE_MED_ASPATH] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("compareRouterIdFlag"));
        backendAttributesMap[BGP_GROUP_CODE_COMPARE_ROUTERID] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_COMPARE_ROUTERID] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_COMPARE_ROUTERID] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultLocalPreference"));
        backendAttributesMap[BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_DEFAULT_LOCAL_PREFERENCE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("extRoutedistance"));
        attributeNames.push_back(string("intRouteDistance"));
        attributeNames.push_back(string("lclRouteDistance"));
        backendAttributesMap[BGP_GROUP_CODE_DISTANCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_DISTANCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_DISTANCE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("enforceFirstAsFlag"));
        backendAttributesMap[BGP_GROUP_CODE_ENFORCE_FIRST_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_ENFORCE_FIRST_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_ENFORCE_FIRST_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("fastExternalFallOverFlag"));
        backendAttributesMap[BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_FAST_EXTERNAL_FALLOVER] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("installIgpCostFlag"));
        backendAttributesMap[BGP_GROUP_CODE_INSTALL_IGP_COST] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_INSTALL_IGP_COST] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_INSTALL_IGP_COST] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("logDampeningDebug"));
        backendAttributesMap[BGP_GROUP_CODE_LOG_DAMPENING_DEBUG] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_LOG_DAMPENING_DEBUG] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_LOG_DAMPENING_DEBUG] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("maxAsLimit"));
        backendAttributesMap[BGP_GROUP_CODE_MAX_AS_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_MAX_AS_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_MAX_AS_LIMIT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("medMissingAsWorstFlag"));
        backendAttributesMap[BGP_GROUP_CODE_MED_MISSING_AS_WORST] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_MED_MISSING_AS_WORST] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_MED_MISSING_AS_WORST] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routerBgpModeTimersKeepAliveTime"));
        attributeNames.push_back(string("routerBgpModeTimerHoldTime"));
        backendAttributesMap[BGP_GROUP_CODE_TIMERS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_TIMERS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_TIMERS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routerBgpNeighborPeerGrp"));
        backendAttributesMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_PEER_GRP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routerBgpNeighborIpxAddr"));
        backendAttributesMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_ROUTER_BGP_NEIGHBOR_IPX_ADDR] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routerBgpAddressIpv4Uc"));
        backendAttributesMap[BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_ROUTER_BGP_IPV4_UCAST] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::validateForCreateAtThisNode: Add the validation function for Create
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::validateForCreateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes         = pContext->getAttributes ();
    UI32               numberOfAttributes = attributes.size ();
	string ipaddress;
    trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::validateForCreateAtThisNode : called with attributes count ") + numberOfAttributes );
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        Attribute* pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());

		 if(pAttribute->getAttributeName()== "neighborIpvXAddress") {
            pAttribute->getPlainString(ipaddress);
            if (!IpVxAddress::isValidIpV4Address(ipaddress))
            {
                trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdate: invalid ip address "));
                status = BGP_IPADDRESS_V6_NOT_SUPPORTED;
            }
        }
    }

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */

    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode: Add the validation function for Update
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes         = pContext->getAttributes ();
    UI32               numberOfAttributes = attributes.size ();
	string rbridgeIdString;
	string interfaceValue;
	UI32 rbridgeId;
	LocationId locationId;
	AttributeString *interfaceAttributeString;
	size_t pos;
    trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdate: called with attributes count ") + numberOfAttributes );
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        Attribute* pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if(pAttribute->getAttributeName()== "ethInterface") {
		  interfaceAttributeString = dynamic_cast<AttributeString*> (pAttribute);
		  if (interfaceAttributeString != NULL)
		  {
		      interfaceValue = interfaceAttributeString->getValue();
		      pos = interfaceValue.find ("/", 0);
		      rbridgeIdString = interfaceValue.substr(0, pos);


		      rbridgeId = atoi(rbridgeIdString.c_str());
              trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode rbridgeid=") + rbridgeId);
		      locationId = DcmToolKit::getLocationIdFromMappedId (rbridgeId);
              if (locationId == 0)
              {
                  trace (TRACE_LEVEL_DEBUG, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode location 0"));
		          status = BGP_INVALID_RBRIDGE;
              }
		      else if (locationId != FrameworkToolKit::getThisLocationId ())
		      {
              	  trace (TRACE_LEVEL_DEBUG, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode not local"));
			      status = BGP_INVALID_RBRIDGE;
		      }
		   }
		   else
		   {
              trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObject::validateForUpdateAtThisNode dynamic_cast error"));
			  status = WAVE_MESSAGE_ERROR;
		   }
	    }
    }

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */

    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
	//isAssociatedManagedObjectToBeCreated will be called only if the associated object does not exist. 
	status = BGP_CLI_ERR_PG_NOT_EXIST;
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
	string remoteAsString;
    AttributeString *remoteAsAttributeString;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if(pAttribute->getAttributeName()== "remoteAsNum") {
            remoteAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
            if (remoteAsAttributeString != NULL)
            {
                remoteAsString = remoteAsAttributeString->getValue();
                remoteAsString = BgpUtils::getASinIntegerFormat(remoteAsString);
                trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::remoteAsString=")+remoteAsString);
				if (BgpUtils::isValidAsNum(remoteAsString))
				{
                	setremoteAsNum(remoteAsString);
                	remoteAsAttributeString->fromString(remoteAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObjec::addNewAttributeToBeUpdatedForCreate incorrect remote AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForCreate dynamic_cast error"));
                status = WAVE_MESSAGE_ERROR;
            }
        }

    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
	string remoteAsString;
    AttributeString *remoteAsAttributeString;
	string localAsString;
    AttributeString *localAsAttributeString;

    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;

		if ((pAttribute->getAttributeName()== "as4EnableFlag")) {
			setas4DisableFlag (false);
			addAttributeToBeUpdated("as4DisableFlag");
		}
		if ((pAttribute->getAttributeName()== "as4DisableFlag")) {
			setas4EnableFlag (false);
			addAttributeToBeUpdated("as4EnableFlag");
		}
		if ((pAttribute->getAttributeName()== "enforceFirstAsFlag")) {
			setenforceFirstAsDisableFlag (false);
			addAttributeToBeUpdated("enforceFirstAsDisableFlag");
		}
		if ((pAttribute->getAttributeName()== "enforceFirstAsDisableFlag")) {
			setenforceFirstAsFlag (false);
			addAttributeToBeUpdated("enforceFirstAsFlag");
		}
		if ((pAttribute->getAttributeName()== "shutdownGenerateRibOutFlag")) {
            setshutdownFlag(true);
            addAttributeToBeUpdated("shutdownFlag");
        }
		if(pAttribute->getAttributeName()== "remoteAsNum") {
            remoteAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
            if (remoteAsAttributeString != NULL)
            {
                remoteAsString = remoteAsAttributeString->getValue();
                remoteAsString = BgpUtils::getASinIntegerFormat(remoteAsString);
                trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::remoteAsString=")+remoteAsString);
				if (BgpUtils::isValidAsNum(remoteAsString))
				{
                	setremoteAsNum(remoteAsString);
                	remoteAsAttributeString->fromString(remoteAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObjec::addNewAttributeToBeUpdatedForUpdate incorrect remote AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForUpdate dynamic_cast error"));
                status = WAVE_MESSAGE_ERROR;
            }
        }
		if(pAttribute->getAttributeName()== "localAsNum") {
			localAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
            if (localAsAttributeString != NULL)
            {
                localAsString = localAsAttributeString->getValue();
                localAsString = BgpUtils::getASinIntegerFormat(localAsString);
                trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::localAsString=")+localAsString);
				if (BgpUtils::isValidAsNum(localAsString))
				{
                	setlocalAsNum(localAsString);
                	localAsAttributeString->fromString(localAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForUpdate incorrect AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForUpdate local as dynamic_cast error"));
                status = WAVE_MESSAGE_ERROR;
            }
        }

    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}

void BgpLocalNeighborIpxAddrManagedObject::CleanupAssociatedNeighborsIpv4Mos(UpdateNewAttributeInManagedObjectContext *pContext)
{
    BgpLocalAfIpv4NeighborIpAddrManagedObject *mo;
    ObjectId objectid = getObjectId();
    ObjectId ownerObjectId;
    vector<string> selectedFields;
    selectedFields.push_back("objectId");
    selectedFields.push_back("ownerManagedObjectId");
    LocationId locationId= FrameworkToolKit::getThisLocationId ();
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeObjectId (&objectid, "afIpv4NeighborIpAddress"));
    syncQueryCtxt.addSelectFields(selectedFields);
    vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(locationId, &syncQueryCtxt);
    if(pResults && pResults->size() >= 1) {
        trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborIpxAddrManagedObject::CleanupAssociatedNeighborsIpv4Mos deleting mos"));
        mo = dynamic_cast<BgpLocalAfIpv4NeighborIpAddrManagedObject*>((*pResults)[0]);
        ownerObjectId = mo->getOwnerManagedObjectId();
        objectid = mo->getObjectId();
        deleteFromComposition(BgpLocalAfIpv4UcastManagedObject::getClassName(), 
            BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(), string("addressFamilyNeighborIpAddr"),
            ownerObjectId, objectid);
        pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }
    ownerObjectId = getOwnerManagedObjectId();
    objectid = getObjectId();
    deleteFromComposition(BgpLocalBasicConfigManagedObject::getClassName(), 
        BgpLocalNeighborIpxAddrManagedObject::getClassName(), string("routerBgpNeighborIpxAddr"),
        ownerObjectId, objectid);
}

void BgpLocalNeighborIpxAddrManagedObject::CleanupAssociatedNeighbors(UpdateNewAttributeInManagedObjectContext *pContext,  Attribute *pAttribute)
{
    if(pAttribute->getAttributeName()== "remoteAsNum") { 
        if (getassociatePeerGroup () == ObjectId::NullObjectId) {
        	/*knock off all neighbor if it is not associated peer group*/
        	CleanupAssociatedNeighborsIpv4Mos(pContext);
		}
		else {
		  	// if the neighbor is associated with a peer-group
			ObjectId peerGroupObjectid = getassociatePeerGroup ();
			WaveManagedObject *pWaveManagedObject = NULL;
			BgpLocalNeighborPeerGrpManagedObject *peerGroupMo = NULL;
			pWaveManagedObject = queryManagedObject (peerGroupObjectid);
			if (pWaveManagedObject != NULL)
			{
				peerGroupMo = dynamic_cast <BgpLocalNeighborPeerGrpManagedObject *> (pWaveManagedObject);
				if (peerGroupMo != NULL)
				{
					string peerGroupRemoteAs = peerGroupMo->getremoteAsNum();
					if (peerGroupRemoteAs == "")
					{
						/*knock off all neighbor config only if the associated peer group has no remote-as configured.*/
						CleanupAssociatedNeighborsIpv4Mos(pContext);
					}
				}
				else
				{
					trace(TRACE_LEVEL_ERROR, string("peerGroupMo is NULL"));
				}
				pContext->addManagedObjectForGarbageCollection(pWaveManagedObject);
			}
			else
			{
        		trace(TRACE_LEVEL_ERROR, string("CleanupAssociatedNeighbors query returned NULL"));
			}
    	}
	}
    else if((pAttribute->getAttributeName()== "associatePeerGroup") &&
        (getremoteAsNum() == ""))
    {
        /*knock off all neighbor only if remote-as is not set*/
        CleanupAssociatedNeighborsIpv4Mos(pContext);
    }
}

/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
        CleanupAssociatedNeighbors(pContext, pAttribute);
		if ((pAttribute->getAttributeName()== "localAsNum")) {
			pContext->addNewAttributeName("localAsNoPrependFlag");
			setlocalAsNoPrependFlag(false);
		}
		if ((pAttribute->getAttributeName()== "shutdownGenerateRibOutFlag")) {
			setshutdownFlag(false);
		}
		if ((pAttribute->getAttributeName()== "shutdownFlag")) {
			setshutdownGenerateRibOutFlag (false);
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["remoteAsNum"] = BGP_GROUP_CODE_NEI_REMOTE_AS;
        vectorOfAttributes.push_back("remoteAsNum");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["description"] = BGP_GROUP_CODE_NEI_DESCRIPTION;
        vectorOfAttributes.push_back("description");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["shutdownFlag"] = BGP_GROUP_CODE_NEI_SHUTDOWN;
        vectorOfAttributes.push_back("shutdownFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["shutdownGenerateRibOutFlag"] = BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT;
        vectorOfAttributes.push_back("shutdownGenerateRibOutFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["localAsNum"] = BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND;
        vectorOfAttributes.push_back("localAsNum");
        attributeNamesGroupCodeMap["localAsNoPrependFlag"] = BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND;
        vectorOfAttributes.push_back("localAsNoPrependFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceNextHopSelfType"] = BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF;
        vectorOfAttributes.push_back("choiceNextHopSelfType");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopSelfStatusFlag"] = BGP_GROUP_CODE_NEI_NEXT_HOP_SELF;
        vectorOfAttributes.push_back("nextHopSelfStatusFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopSelfAlwaysFlag"] = BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS;
        vectorOfAttributes.push_back("nextHopSelfAlwaysFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["advertisementInterval"] = BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL;
        vectorOfAttributes.push_back("advertisementInterval");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceEbgpMultiHopType"] = BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE;
        vectorOfAttributes.push_back("choiceEbgpMultiHopType");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ebgpMultiHopFlag"] = BGP_GROUP_CODE_NEI_EBGP_MULTIHOP;
        vectorOfAttributes.push_back("ebgpMultiHopFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ebgpMultiHopCount"] = BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT;
        vectorOfAttributes.push_back("ebgpMultiHopCount");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceMaxasLimit"] = BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT;
        vectorOfAttributes.push_back("choiceMaxasLimit");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asPathNum"] = BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH;
        vectorOfAttributes.push_back("asPathNum");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["maxasDisableFlag"] = BGP_GROUP_CODE_NEI_MAX_AS_DISABLE;
        vectorOfAttributes.push_back("maxasDisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_PASSWORD] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["bgpPassword"] = BGP_GROUP_CODE_NEI_PASSWORD;
        vectorOfAttributes.push_back("bgpPassword");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_PASSWORD] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["enforceFirstAsFlag"] = BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS;
        vectorOfAttributes.push_back("enforceFirstAsFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["enforceFirstAsDisableFlag"] = BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE;
        vectorOfAttributes.push_back("enforceFirstAsDisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["as4EnableFlag"] = BGP_GROUP_CODE_NEI_AS4_ENABLE;
        vectorOfAttributes.push_back("as4EnableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["as4DisableFlag"] = BGP_GROUP_CODE_NEI_AS4_DISABLE;
        vectorOfAttributes.push_back("as4DisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_TIMERS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["NeikeepAliveTime"] = BGP_GROUP_CODE_NEI_TIMERS;
        vectorOfAttributes.push_back("NeikeepAliveTime");
        attributeNamesGroupCodeMap["NeiHoldTime"] = BGP_GROUP_CODE_NEI_TIMERS;
        vectorOfAttributes.push_back("NeiHoldTime");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_TIMERS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceUpdateSource"] = BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE;
        vectorOfAttributes.push_back("choiceUpdateSource");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["sipIpv4Address"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4;
        vectorOfAttributes.push_back("sipIpv4Address");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ethInterfaceType"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH;
        vectorOfAttributes.push_back("ethInterfaceType");
        attributeNamesGroupCodeMap["ethInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH;
        vectorOfAttributes.push_back("ethInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["loopInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP;
        vectorOfAttributes.push_back("loopInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["virtualInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI;
        vectorOfAttributes.push_back("virtualInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["removePrivateAsFlag"] = BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS;
        vectorOfAttributes.push_back("removePrivateAsFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asOverRideFlag"] = BGP_GROUP_CODE_NEI_AS_OVERRIDE;
        vectorOfAttributes.push_back("asOverRideFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["softReconfigurationInboundFlag"] = BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND;
        vectorOfAttributes.push_back("softReconfigurationInboundFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_IPVX_ADDRESS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["neighborIpvXAddress"] = BGP_GROUP_CODE_NEI_IPVX_ADDRESS;
        vectorOfAttributes.push_back("neighborIpvXAddress");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_IPVX_ADDRESS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_IP_PEER_GROUP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["associatePeerGroup"] = BGP_GROUP_CODE_NEI_IP_PEER_GROUP;
        vectorOfAttributes.push_back("associatePeerGroup");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_IP_PEER_GROUP] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("remoteAsNum"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("description"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("shutdownFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("shutdownGenerateRibOutFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("localAsNum"));
        attributeNames.push_back(string("localAsNoPrependFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceNextHopSelfType"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopSelfStatusFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_next_hop_self_status);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_NEXT_HOP_SELF);
        userTagToGroupIdsMap[bgp_ca_next_hop_self_status] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopSelfAlwaysFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_next_hop_self_always);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS);
        userTagToGroupIdsMap[bgp_ca_next_hop_self_always] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("advertisementInterval"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceEbgpMultiHopType"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ebgpMultiHopFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ebgp_multihop_flag);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_EBGP_MULTIHOP);
        userTagToGroupIdsMap[bgp_ca_ebgp_multihop_flag] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ebgpMultiHopCount"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ebgp_multihop_count);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT);
        userTagToGroupIdsMap[bgp_ca_ebgp_multihop_count] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceMaxasLimit"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asPathNum"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_maxas_limit_enable);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH);
        userTagToGroupIdsMap[bgp_ca_maxas_limit_enable] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("maxasDisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_maxas_limit_disable);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_MAX_AS_DISABLE);
        userTagToGroupIdsMap[bgp_ca_maxas_limit_disable] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("bgpPassword"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_PASSWORD] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_PASSWORD] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_PASSWORD] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("enforceFirstAsFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("enforceFirstAsDisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("as4EnableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("as4DisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("NeikeepAliveTime"));
        attributeNames.push_back(string("NeiHoldTime"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_TIMERS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_TIMERS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_TIMERS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceUpdateSource"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("sipIpv4Address"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ipv4);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4);
        userTagToGroupIdsMap[bgp_ca_ipv4] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ethInterfaceType"));
        attributeNames.push_back(string("ethInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_eth);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH);
        userTagToGroupIdsMap[bgp_ca_eth] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("loopInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_loopback);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP);
        userTagToGroupIdsMap[bgp_ca_loopback] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("virtualInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ve);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI);
        userTagToGroupIdsMap[bgp_ca_ve] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("removePrivateAsFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asOverRideFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("softReconfigurationInboundFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("neighborIpvXAddress"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_IPVX_ADDRESS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_IPVX_ADDRESS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_IPVX_ADDRESS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("associatePeerGroup"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_IP_PEER_GROUP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_IP_PEER_GROUP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_IP_PEER_GROUP] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode: Add the validation function for Update
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes         = pContext->getAttributes ();
    UI32               numberOfAttributes = attributes.size ();
	string rbridgeIdString;
	string interfaceValue;
	UI32 rbridgeId;
	LocationId locationId;
	AttributeString *interfaceAttributeString;
	size_t pos;
    trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborPeerGrpManagedObject::validateForUpdate: called with attributes count ") + numberOfAttributes );
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        Attribute* pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if(pAttribute->getAttributeName()== "ethInterface") {
		  interfaceAttributeString = dynamic_cast<AttributeString*> (pAttribute);
		  if (interfaceAttributeString != NULL)
		  {
		      interfaceValue = interfaceAttributeString->getValue();
		      pos = interfaceValue.find ("/", 0);
		      rbridgeIdString = interfaceValue.substr(0, pos);


		      rbridgeId = atoi(rbridgeIdString.c_str());
              trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode rbridgeid=") + rbridgeId);
		      locationId = DcmToolKit::getLocationIdFromMappedId (rbridgeId);
              if (locationId == 0)
              {
                  trace (TRACE_LEVEL_DEBUG, string("BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode location 0"));
		          status = BGP_INVALID_RBRIDGE;
              }
		      else if (locationId != FrameworkToolKit::getThisLocationId ())
		      {
              	  trace (TRACE_LEVEL_DEBUG, string("BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode not local"));
			      status = BGP_INVALID_RBRIDGE;
		      }
		   }
		   else
		   {
              trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborPeerGrpManagedObject::validateForUpdateAtThisNode dynamic_cast error"));
			  status = WAVE_MESSAGE_ERROR;
		   }
	    }
    }

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Please add any Validation handling for the code as required
     * @uBreezeClose
     */

    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
	string remoteAsString;
    AttributeString *remoteAsAttributeString;
	string localAsString;
	AttributeString *localAsAttributeString;


    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;

		if ((pAttribute->getAttributeName()== "as4EnableFlag")) {
			setas4DisableFlag (false);
			addAttributeToBeUpdated("as4DisableFlag");
		}
		if ((pAttribute->getAttributeName()== "as4DisableFlag")) {
			setas4EnableFlag (false);
			addAttributeToBeUpdated("as4EnableFlag");
		}
		if ((pAttribute->getAttributeName()== "enforceFirstAsFlag")) {
			setenforceFirstAsDisableFlag (false);
			addAttributeToBeUpdated("enforceFirstAsDisableFlag");
		}
		if ((pAttribute->getAttributeName()== "enforceFirstAsDisableFlag")) {
			setenforceFirstAsFlag (false);
			addAttributeToBeUpdated("enforceFirstAsFlag");
		}
		if ((pAttribute->getAttributeName()== "shutdownGenerateRibOutFlag")) {
            setshutdownFlag(true);
            addAttributeToBeUpdated("shutdownFlag");
        }
		if(pAttribute->getAttributeName()== "remoteAsNum") {
            remoteAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
            if (remoteAsAttributeString != NULL)
            {
                remoteAsString = remoteAsAttributeString->getValue();
                remoteAsString = BgpUtils::getASinIntegerFormat(remoteAsString);
                trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborPeerGrpManagedObject::remoteAsString=")+remoteAsString);
				if (BgpUtils::isValidAsNum(remoteAsString))
				{
                	setremoteAsNum(remoteAsString);
                	remoteAsAttributeString->fromString(remoteAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForCreate incorrect remote AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate dynamic_cast error"));
                status = WAVE_MESSAGE_ERROR;
            }
        }
		if(pAttribute->getAttributeName()== "localAsNum") {
            localAsAttributeString = dynamic_cast<AttributeString*> (pAttribute);
            if (localAsAttributeString != NULL)
            {
                localAsString = localAsAttributeString->getValue();
                localAsString = BgpUtils::getASinIntegerFormat(localAsString);
                trace (TRACE_LEVEL_INFO, string("BgpLocalNeighborPeerGrpManagedObject::localAsString=")+localAsString);
				if (BgpUtils::isValidAsNum(localAsString))
				{
					setlocalAsNum(localAsString);
					localAsAttributeString->fromString(localAsString);
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, string("BgpLocalBasicConfigManagedObject::addNewAttributeToBeUpdatedForUpdate incorrect AS num"));
					status = BGP_CLI_ERR_INVALID_AS;
				}
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate local as dynamic_cast error"));
                status = WAVE_MESSAGE_ERROR;
            }
        }
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}

void BgpLocalNeighborPeerGrpManagedObject::CleanupAssociatedNeighborPeerGroupMos(UpdateNewAttributeInManagedObjectContext *pContext)
{
    BgpLocalAfIpv4NeighborPeerGrpManagedObject *mo;
    ObjectId objectid = getObjectId();
    ObjectId ownerObjectId;
    vector<string> selectedFields;
    selectedFields.push_back("objectId");
    selectedFields.push_back("ownerManagedObjectId");
    LocationId locationId= FrameworkToolKit::getThisLocationId ();
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeObjectId (&objectid, "afIpv4NeighborPeerGrpName"));
    syncQueryCtxt.addSelectFields(selectedFields);
    vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(locationId, &syncQueryCtxt);
    if(pResults && pResults->size() >= 1) {
        trace(TRACE_LEVEL_INFO, string("BgpLocalNeighborPeerGrpManagedObject::CleanupAssociatedNeighborPeerGroupMos deleting mos"));
        mo = dynamic_cast<BgpLocalAfIpv4NeighborPeerGrpManagedObject*>((*pResults)[0]);
        ownerObjectId = mo->getOwnerManagedObjectId();
        objectid = mo->getObjectId();
        deleteFromComposition(BgpLocalAfIpv4UcastManagedObject::getClassName(), 
            BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(), string("addressFamilyNeighborPeerGrp"),
            ownerObjectId, objectid);
        pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }
    ownerObjectId = getOwnerManagedObjectId();
    objectid = getObjectId();
    deleteFromComposition(BgpLocalBasicConfigManagedObject::getClassName(), 
        BgpLocalNeighborPeerGrpManagedObject::getClassName(), string("routerBgpNeighborPeerGrp"),
        ownerObjectId, objectid);
}

void BgpLocalNeighborPeerGrpManagedObject::CleanupAssociatedPeerGroups(UpdateNewAttributeInManagedObjectContext *pContext,  Attribute *pAttribute)
{
    if(pAttribute->getAttributeName()== "peerGrpFlag") { 
		/*knock off peer group configurations*/
		CleanupAssociatedNeighborPeerGroupMos(pContext);
	}
}

/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		CleanupAssociatedPeerGroups(pContext, pAttribute);
		if ((pAttribute->getAttributeName()== "localAsNum")) {
			pContext->addNewAttributeName("localAsNoPrependFlag");
			setlocalAsNoPrependFlag(false);
		}
		if ((pAttribute->getAttributeName()== "shutdownGenerateRibOutFlag")) {
			setshutdownFlag(false);
		}
		if ((pAttribute->getAttributeName()== "shutdownFlag")) {
			setshutdownGenerateRibOutFlag (false);
		}

    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["remoteAsNum"] = BGP_GROUP_CODE_NEI_REMOTE_AS;
        vectorOfAttributes.push_back("remoteAsNum");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["description"] = BGP_GROUP_CODE_NEI_DESCRIPTION;
        vectorOfAttributes.push_back("description");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["shutdownFlag"] = BGP_GROUP_CODE_NEI_SHUTDOWN;
        vectorOfAttributes.push_back("shutdownFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["shutdownGenerateRibOutFlag"] = BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT;
        vectorOfAttributes.push_back("shutdownGenerateRibOutFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["localAsNum"] = BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND;
        vectorOfAttributes.push_back("localAsNum");
        attributeNamesGroupCodeMap["localAsNoPrependFlag"] = BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND;
        vectorOfAttributes.push_back("localAsNoPrependFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceNextHopSelfType"] = BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF;
        vectorOfAttributes.push_back("choiceNextHopSelfType");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopSelfStatusFlag"] = BGP_GROUP_CODE_NEI_NEXT_HOP_SELF;
        vectorOfAttributes.push_back("nextHopSelfStatusFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopSelfAlwaysFlag"] = BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS;
        vectorOfAttributes.push_back("nextHopSelfAlwaysFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["advertisementInterval"] = BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL;
        vectorOfAttributes.push_back("advertisementInterval");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceEbgpMultiHopType"] = BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE;
        vectorOfAttributes.push_back("choiceEbgpMultiHopType");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ebgpMultiHopFlag"] = BGP_GROUP_CODE_NEI_EBGP_MULTIHOP;
        vectorOfAttributes.push_back("ebgpMultiHopFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ebgpMultiHopCount"] = BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT;
        vectorOfAttributes.push_back("ebgpMultiHopCount");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceMaxasLimit"] = BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT;
        vectorOfAttributes.push_back("choiceMaxasLimit");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asPathNum"] = BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH;
        vectorOfAttributes.push_back("asPathNum");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["maxasDisableFlag"] = BGP_GROUP_CODE_NEI_MAX_AS_DISABLE;
        vectorOfAttributes.push_back("maxasDisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_PASSWORD] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["bgpPassword"] = BGP_GROUP_CODE_NEI_PASSWORD;
        vectorOfAttributes.push_back("bgpPassword");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_PASSWORD] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["enforceFirstAsFlag"] = BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS;
        vectorOfAttributes.push_back("enforceFirstAsFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["enforceFirstAsDisableFlag"] = BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE;
        vectorOfAttributes.push_back("enforceFirstAsDisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["as4EnableFlag"] = BGP_GROUP_CODE_NEI_AS4_ENABLE;
        vectorOfAttributes.push_back("as4EnableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["as4DisableFlag"] = BGP_GROUP_CODE_NEI_AS4_DISABLE;
        vectorOfAttributes.push_back("as4DisableFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_TIMERS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["NeikeepAliveTime"] = BGP_GROUP_CODE_NEI_TIMERS;
        vectorOfAttributes.push_back("NeikeepAliveTime");
        attributeNamesGroupCodeMap["NeiHoldTime"] = BGP_GROUP_CODE_NEI_TIMERS;
        vectorOfAttributes.push_back("NeiHoldTime");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_TIMERS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceUpdateSource"] = BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE;
        vectorOfAttributes.push_back("choiceUpdateSource");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["sipIpv4Address"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4;
        vectorOfAttributes.push_back("sipIpv4Address");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ethInterfaceType"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH;
        vectorOfAttributes.push_back("ethInterfaceType");
        attributeNamesGroupCodeMap["ethInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH;
        vectorOfAttributes.push_back("ethInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["loopInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP;
        vectorOfAttributes.push_back("loopInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["virtualInterface"] = BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI;
        vectorOfAttributes.push_back("virtualInterface");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["removePrivateAsFlag"] = BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS;
        vectorOfAttributes.push_back("removePrivateAsFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asOverRideFlag"] = BGP_GROUP_CODE_NEI_AS_OVERRIDE;
        vectorOfAttributes.push_back("asOverRideFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["softReconfigurationInboundFlag"] = BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND;
        vectorOfAttributes.push_back("softReconfigurationInboundFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["neighborPeerGrpName"] = BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY;
        vectorOfAttributes.push_back("neighborPeerGrpName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["peerGrpFlag"] = BGP_GROUP_CODE_NEI_PEER_GROUP_NAME;
        vectorOfAttributes.push_back("peerGrpFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("remoteAsNum"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_REMOTE_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("description"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_DESCRIPTION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("shutdownFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SHUTDOWN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("shutdownGenerateRibOutFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SHUTDOWN_GENERATE_RIB_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("localAsNum"));
        attributeNames.push_back(string("localAsNoPrependFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_LOCAL_AS_NO_PREPEND] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceNextHopSelfType"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopSelfStatusFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_next_hop_self_status);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_NEXT_HOP_SELF);
        userTagToGroupIdsMap[bgp_ca_next_hop_self_status] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopSelfAlwaysFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_next_hop_self_always);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_NEXT_HOP_SELF_ALWAYS);
        userTagToGroupIdsMap[bgp_ca_next_hop_self_always] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_NEXTHOP_SELF] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("advertisementInterval"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ADVERTISEMENT_INTERVAL] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceEbgpMultiHopType"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ebgpMultiHopFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ebgp_multihop_flag);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_EBGP_MULTIHOP);
        userTagToGroupIdsMap[bgp_ca_ebgp_multihop_flag] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ebgpMultiHopCount"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ebgp_multihop_count);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_EBGP_MULTIHOP_COUNT);
        userTagToGroupIdsMap[bgp_ca_ebgp_multihop_count] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_EBPG_MULTI_HOP_TYPE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceMaxasLimit"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asPathNum"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_maxas_limit_enable);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_MAX_AS_IN_PATH);
        userTagToGroupIdsMap[bgp_ca_maxas_limit_enable] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("maxasDisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_MAX_AS_DISABLE] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_maxas_limit_disable);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_MAX_AS_DISABLE);
        userTagToGroupIdsMap[bgp_ca_maxas_limit_disable] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_MAXAS_LIMIT] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("bgpPassword"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_PASSWORD] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_PASSWORD] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_PASSWORD] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("enforceFirstAsFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("enforceFirstAsDisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_ENFORCE_FIRST_AS_DISABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("as4EnableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS4_ENABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("as4DisableFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS4_DISABLE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("NeikeepAliveTime"));
        attributeNames.push_back(string("NeiHoldTime"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_TIMERS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_TIMERS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_TIMERS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceUpdateSource"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("sipIpv4Address"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ipv4);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_IPV4);
        userTagToGroupIdsMap[bgp_ca_ipv4] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ethInterfaceType"));
        attributeNames.push_back(string("ethInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_eth);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_ETH);
        userTagToGroupIdsMap[bgp_ca_eth] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("loopInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_loopback);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_LOOP);
        userTagToGroupIdsMap[bgp_ca_loopback] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("virtualInterface"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_ve);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_NEI_UPDATE_SOURCE_VI);
        userTagToGroupIdsMap[bgp_ca_ve] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_NEI_CHOICE_UPDATE_SOURCE] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("removePrivateAsFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_REMOTE_PRIVATE_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asOverRideFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_AS_OVERRIDE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("softReconfigurationInboundFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_SOFT_RECONFIGURATION_INBOUND] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("neighborPeerGrpName"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME_KEY] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("peerGrpFlag"));
        backendAttributesMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_NEI_PEER_GROUP_NAME] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    if ( 0 == numberOfAttributes )
    {
        setpresenceFlag (true);
        pContext->addNewAttributeName("presenceFlag");
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;

		//redistribute connected
		if ((pAttribute->getAttributeName()== "unicastMetric") || (pAttribute->getAttributeName()== "redistributeRouteMapName")) { 
        	trace(TRACE_LEVEL_INFO, string("setting setredistributeConnectedFlag:"));;
			setredistributeConnectedFlag(true);
			addAttributeToBeUpdated("redistributeConnectedFlag");
		}
		//redistribute static 
		if ((pAttribute->getAttributeName()== "unicastStaticMetric") || (pAttribute->getAttributeName()== "staticRouteMapName")) { 
        	trace(TRACE_LEVEL_INFO, string("setting setredistributeStaticFlag:"));;
			setredistributeStaticFlag(true);
			addAttributeToBeUpdated("redistributeStaticFlag");
		}
		//redistribute ospf 
		if ((pAttribute->getAttributeName()== "ospfMatchInternal") || (pAttribute->getAttributeName()== "ospfMatchExternalOne") ||
			(pAttribute->getAttributeName()== "ospfMatchExternalTwo") || (pAttribute->getAttributeName()== "ospfMetric") ||
			(pAttribute->getAttributeName()== "ospfRouteMapName")) { 
        	trace(TRACE_LEVEL_INFO, string("setting setredistributeOspfFlag:"));;
			setredistributeOspfFlag(true);
			addAttributeToBeUpdated("redistributeOspfFlag");
		}
		//maximum-paths
		if (pAttribute->getAttributeName()== "useLoadSharingFlag") {
		  	setloadSharingValue(0);
			addAttributeToBeUpdated("loadSharingValue");
		}
		if (pAttribute->getAttributeName()== "loadSharingValue") {
		  	setuseLoadSharingFlag(false);
			addAttributeToBeUpdated("useLoadSharingFlag");
		}
		//multipath
		if (pAttribute->getAttributeName()== "multiPathEbgp") {
		  	setmultiPathIbgp (false);
			addAttributeToBeUpdated("multiPathIbgp");
		}
		if (pAttribute->getAttributeName()== "multiPathIbgp") {
		  	setmultiPathEbgp (false);
			addAttributeToBeUpdated("multiPathEbgp");
		}

    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    if ( 0 == numberOfAttributes )
    {
        setpresenceFlag (false);
        pContext->addNewAttributeName("presenceFlag");
    }
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;

		if ((pAttribute->getAttributeName()== "redistributeConnectedFlag")) { 
			setunicastMetric(0);
			setredistributeRouteMapName("");
		}
		if ((pAttribute->getAttributeName()== "redistributeOspfFlag")) { 
		  	setospfMatchInternal(false);
			setospfMatchExternalOne(false);
			setospfMatchExternalTwo(false);
			setospfMetric(0);
			setospfRouteMapName("");
		}
		if ((pAttribute->getAttributeName()== "redistributeStaticFlag")) { 
		  	setunicastStaticMetric(0);
			setstaticRouteMapName("");
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalAfIpv4UcastManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["aggregateAddress"] = BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS;
        vectorOfAttributes.push_back("aggregateAddress");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_NETWORK] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["network"] = BGP_GROUP_CODE_AF_IPV4_NETWORK;
        vectorOfAttributes.push_back("network");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_NETWORK] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["redistributeConnectedFlag"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED;
        vectorOfAttributes.push_back("redistributeConnectedFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["unicastMetric"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC;
        vectorOfAttributes.push_back("unicastMetric");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["redistributeRouteMapName"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP;
        vectorOfAttributes.push_back("redistributeRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["redistributeOspfFlag"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF;
        vectorOfAttributes.push_back("redistributeOspfFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ospfMatchInternal"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL;
        vectorOfAttributes.push_back("ospfMatchInternal");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ospfMatchExternalOne"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE;
        vectorOfAttributes.push_back("ospfMatchExternalOne");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ospfMatchExternalTwo"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO;
        vectorOfAttributes.push_back("ospfMatchExternalTwo");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ospfMetric"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC;
        vectorOfAttributes.push_back("ospfMetric");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ospfRouteMapName"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP;
        vectorOfAttributes.push_back("ospfRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["redistributeStaticFlag"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC;
        vectorOfAttributes.push_back("redistributeStaticFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["unicastStaticMetric"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC;
        vectorOfAttributes.push_back("unicastStaticMetric");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["staticRouteMapName"] = BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP;
        vectorOfAttributes.push_back("staticRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["bgpRedistributeInternalFlag"] = BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL;
        vectorOfAttributes.push_back("bgpRedistributeInternalFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["staticNetwork"] = BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK;
        vectorOfAttributes.push_back("staticNetwork");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["loadSharingValue"] = BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE;
        vectorOfAttributes.push_back("loadSharingValue");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MAX_PATH_EBGP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ebgpLoadSharingValue"] = BGP_GROUP_CODE_AF_MAX_PATH_EBGP;
        vectorOfAttributes.push_back("ebgpLoadSharingValue");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MAX_PATH_EBGP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MAX_PATH_IBGP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ibgpLoadSharingValue"] = BGP_GROUP_CODE_AF_MAX_PATH_IBGP;
        vectorOfAttributes.push_back("ibgpLoadSharingValue");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MAX_PATH_IBGP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["useLoadSharingFlag"] = BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING;
        vectorOfAttributes.push_back("useLoadSharingFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MULTI_PATH_EBGP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["multiPathEbgp"] = BGP_GROUP_CODE_AF_MULTI_PATH_EBGP;
        vectorOfAttributes.push_back("multiPathEbgp");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MULTI_PATH_EBGP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MULTI_PATH_IBGP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["multiPathIbgp"] = BGP_GROUP_CODE_AF_MULTI_PATH_IBGP;
        vectorOfAttributes.push_back("multiPathIbgp");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MULTI_PATH_IBGP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_MULTI_PATH_AS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["multiPathMultiAs"] = BGP_GROUP_CODE_AF_MULTI_PATH_AS;
        vectorOfAttributes.push_back("multiPathMultiAs");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_MULTI_PATH_AS] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["alwaysPropogateFlag"] = BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE;
        vectorOfAttributes.push_back("alwaysPropogateFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultInformationOriginateFlag"] = BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE;
        vectorOfAttributes.push_back("defaultInformationOriginateFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["ribRouteLimit"] = BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT;
        vectorOfAttributes.push_back("ribRouteLimit");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["clientToClientReflectionFlag"] = BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION;
        vectorOfAttributes.push_back("clientToClientReflectionFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["choiceDampening"] = BGP_GROUP_CODE_AF_CHOICE_DAMPENING;
        vectorOfAttributes.push_back("choiceDampening");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_DAMPENING] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["dampeningFlag"] = BGP_GROUP_CODE_AF_DAMPENING;
        vectorOfAttributes.push_back("dampeningFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_DAMPENING] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_DAMPENING_TIME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["dampeningHalfTime"] = BGP_GROUP_CODE_AF_DAMPENING_TIME;
        vectorOfAttributes.push_back("dampeningHalfTime");
        attributeNamesGroupCodeMap["dampeningReuseValue"] = BGP_GROUP_CODE_AF_DAMPENING_TIME;
        vectorOfAttributes.push_back("dampeningReuseValue");
        attributeNamesGroupCodeMap["dampeningStartSuppressTime"] = BGP_GROUP_CODE_AF_DAMPENING_TIME;
        vectorOfAttributes.push_back("dampeningStartSuppressTime");
        attributeNamesGroupCodeMap["dampeningMaxSuppressTime"] = BGP_GROUP_CODE_AF_DAMPENING_TIME;
        vectorOfAttributes.push_back("dampeningMaxSuppressTime");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_DAMPENING_TIME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["dampeningRouteMapName"] = BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP;
        vectorOfAttributes.push_back("dampeningRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_DEFAULT_METRIC] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultMetric"] = BGP_GROUP_CODE_AF_DEFAULT_METRIC;
        vectorOfAttributes.push_back("defaultMetric");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_DEFAULT_METRIC] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopEnableDefault"] = BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT;
        vectorOfAttributes.push_back("nextHopEnableDefault");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_TABLE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["tableMapRouteMapName"] = BGP_GROUP_CODE_AF_TABLE_MAP;
        vectorOfAttributes.push_back("tableMapRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_TABLE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_UPDATE_TIME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["updateTime"] = BGP_GROUP_CODE_AF_UPDATE_TIME;
        vectorOfAttributes.push_back("updateTime");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_UPDATE_TIME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["addressFamilyNeighborIpAddr"] = BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR;
        vectorOfAttributes.push_back("addressFamilyNeighborIpAddr");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["addressFamilyNeighborPeerGrp"] = BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP;
        vectorOfAttributes.push_back("addressFamilyNeighborPeerGrp");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["presenceFlag"] = BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE;
        vectorOfAttributes.push_back("presenceFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["nextHopRecursionFlag"] = BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION;
        vectorOfAttributes.push_back("nextHopRecursionFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("aggregateAddress"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_AGGREGATE_ADDRESS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("network"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_NETWORK] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_NETWORK] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_NETWORK] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("redistributeConnectedFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("unicastMetric"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_METRIC] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("redistributeRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_CONNECTED_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("redistributeOspfFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ospfMatchInternal"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_INTERNAL] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ospfMatchExternalOne"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALONE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ospfMatchExternalTwo"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_EXTERNALTWO] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ospfMetric"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_METRIC] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ospfRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_OSPF_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("redistributeStaticFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("unicastStaticMetric"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_METRIC] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("staticRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_REDISTRIBUTE_STATIC_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("bgpRedistributeInternalFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_REDISTRIBUTE_INTERNAL] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("staticNetwork"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_STATIC_NETWORK] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("loadSharingValue"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MAX_PATH_LOAD_SHARING_VALUE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ebgpLoadSharingValue"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MAX_PATH_EBGP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MAX_PATH_EBGP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MAX_PATH_EBGP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ibgpLoadSharingValue"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MAX_PATH_IBGP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MAX_PATH_IBGP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MAX_PATH_IBGP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("useLoadSharingFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MAX_PATH_USE_LOAD_SHARING] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("multiPathEbgp"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MULTI_PATH_EBGP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MULTI_PATH_EBGP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MULTI_PATH_EBGP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("multiPathIbgp"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MULTI_PATH_IBGP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MULTI_PATH_IBGP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MULTI_PATH_IBGP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("multiPathMultiAs"));
        backendAttributesMap[BGP_GROUP_CODE_AF_MULTI_PATH_AS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_MULTI_PATH_AS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_MULTI_PATH_AS] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("alwaysPropogateFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ALWAYS_PROPAGATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultInformationOriginateFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_DEFAULT_INFO_ORIGINATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("ribRouteLimit"));
        backendAttributesMap[BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_RIB_ROUTE_LIMIT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("clientToClientReflectionFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_CLIENT_TO_CLIENT_REFLECTION] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("choiceDampening"));
        backendAttributesMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = true;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("dampeningFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_DAMPENING] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_DAMPENING] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_AF_CHOICE_DAMPENING);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_dampening_flag);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_AF_DAMPENING);
        userTagToGroupIdsMap[bgp_ca_dampening_flag] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("dampeningHalfTime"));
        attributeNames.push_back(string("dampeningReuseValue"));
        attributeNames.push_back(string("dampeningStartSuppressTime"));
        attributeNames.push_back(string("dampeningMaxSuppressTime"));
        backendAttributesMap[BGP_GROUP_CODE_AF_DAMPENING_TIME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_DAMPENING_TIME] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_AF_CHOICE_DAMPENING);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_dampening_specify_values);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_AF_DAMPENING_TIME);
        userTagToGroupIdsMap[bgp_ca_dampening_specify_values] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("dampeningRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP] = "bgp";

        map<UI32, vector<UI32> > userTagToGroupIdsMap;

        map<UI32, map<UI32, vector<UI32> > >::iterator it = choiceToCasesMap.find(BGP_GROUP_CODE_AF_CHOICE_DAMPENING);

        if (it != choiceToCasesMap.end())
        {
            userTagToGroupIdsMap = it->second;
        }

        vector<UI32> groupIds;

        map<UI32, vector<UI32> >::iterator groupIdsIter = userTagToGroupIdsMap.find(bgp_ca_dampening_route_map);

        if (groupIdsIter != userTagToGroupIdsMap.end())
        {
            groupIds = groupIdsIter->second;
        }

        groupIds.push_back(BGP_GROUP_CODE_AF_DAMPENING_ROUTE_MAP);
        userTagToGroupIdsMap[bgp_ca_dampening_route_map] = groupIds;
        choiceToCasesMap[BGP_GROUP_CODE_AF_CHOICE_DAMPENING] = userTagToGroupIdsMap;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultMetric"));
        backendAttributesMap[BGP_GROUP_CODE_AF_DEFAULT_METRIC] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_DEFAULT_METRIC] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_DEFAULT_METRIC] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopEnableDefault"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEXT_HOP_ENABLE_DEFAULT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("tableMapRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_TABLE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_TABLE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_TABLE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("updateTime"));
        backendAttributesMap[BGP_GROUP_CODE_AF_UPDATE_TIME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_UPDATE_TIME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_UPDATE_TIME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("addressFamilyNeighborIpAddr"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEIGHBOR_IPADDR] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("addressFamilyNeighborPeerGrp"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEIGHBOR_PEER_GRP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("presenceFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_UCAST_PRESENCE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("nextHopRecursionFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_UCAST_NEXT_HOP_RECURSION] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::getManagedObjectUserTag: Get the userTag for single ton MO
 * @uBreezeClose
 */
UI32 BgpLocalAfIpv4UcastManagedObject::getManagedObjectUserTag () const
{
    return (bgp_ipv4_unicast);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 
	//isAssociatedManagedObjectToBeCreated will be called only if the associated object does not exist. 
	status = BGP_CLI_ERR_CONFIGURE_REMOTE_AS_FIRST;
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if ((pAttribute->getAttributeName()== "defaultOriginateRouteMapName")) {
			addAttributeToBeUpdated("defaultOriginateStatus");
			setdefaultOriginateStatus(true);
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if ((pAttribute->getAttributeName()== "defaultOriginateRouteMapName")) {
			addAttributeToBeUpdated("defaultOriginateStatus");
			setdefaultOriginateStatus(true);
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if ((pAttribute->getAttributeName()== "maxiumPrefixLimit")) {
			setmaxiumPrefixLimit(0);
			setmaxiumPrefixthreshold(0);
			setmaxiumPrefixTeardownFlag(false);
		}
		if ((pAttribute->getAttributeName()== "defaultOriginateStatus")) { 
		  	setdefaultOriginateRouteMapName("");
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["activateFlag"] = BGP_GROUP_CODE_AF_NEI_ACTIVATE;
        vectorOfAttributes.push_back("activateFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["filterListDirectionInAclName"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME;
        vectorOfAttributes.push_back("filterListDirectionInAclName");
        attributeNamesGroupCodeMap["filterListdirectionInAclNameFlag"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME;
        vectorOfAttributes.push_back("filterListdirectionInAclNameFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["filterListDirectionOutAclName"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME;
        vectorOfAttributes.push_back("filterListDirectionOutAclName");
        attributeNamesGroupCodeMap["filterListdirectionOutAclNameFlag"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME;
        vectorOfAttributes.push_back("filterListdirectionOutAclNameFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["maxiumPrefixLimit"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixLimit");
        attributeNamesGroupCodeMap["maxiumPrefixthreshold"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixthreshold");
        attributeNamesGroupCodeMap["maxiumPrefixTeardownFlag"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixTeardownFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultOriginateStatus"] = BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE;
        vectorOfAttributes.push_back("defaultOriginateStatus");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultOriginateRouteMapName"] = BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP;
        vectorOfAttributes.push_back("defaultOriginateRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["prefixListInName"] = BGP_GROUP_CODE_AF_PREFIX_LIST_IN;
        vectorOfAttributes.push_back("prefixListInName");
        attributeNamesGroupCodeMap["prefixListInFlag"] = BGP_GROUP_CODE_AF_PREFIX_LIST_IN;
        vectorOfAttributes.push_back("prefixListInFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["prefixListOutName"] = BGP_GROUP_CODE_AF_PREFIX_LIST_OUT;
        vectorOfAttributes.push_back("prefixListOutName");
        attributeNamesGroupCodeMap["prefixListOutFlag"] = BGP_GROUP_CODE_AF_PREFIX_LIST_OUT;
        vectorOfAttributes.push_back("prefixListOutFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeMapNameIn"] = BGP_GROUP_CODE_AF_ROUTE_MAP_IN;
        vectorOfAttributes.push_back("routeMapNameIn");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeMapNameOut"] = BGP_GROUP_CODE_AF_ROUTE_MAP_OUT;
        vectorOfAttributes.push_back("routeMapNameOut");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeReflectorClientFlag"] = BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT;
        vectorOfAttributes.push_back("routeReflectorClientFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["sendCommunityBoth"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityBoth");
        attributeNamesGroupCodeMap["sendCommunityExtended"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityExtended");
        attributeNamesGroupCodeMap["sendCommunityStandard"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityStandard");
        attributeNamesGroupCodeMap["sendCommunityStatus"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityStatus");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["unSuppressRouteMapName"] = BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP;
        vectorOfAttributes.push_back("unSuppressRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_WEIGHT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["weight"] = BGP_GROUP_CODE_AF_WEIGHT;
        vectorOfAttributes.push_back("weight");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_WEIGHT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["afIpv4NeighborIpAddress"] = BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS;
        vectorOfAttributes.push_back("afIpv4NeighborIpAddress");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("activateFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("filterListDirectionInAclName"));
        attributeNames.push_back(string("filterListdirectionInAclNameFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("filterListDirectionOutAclName"));
        attributeNames.push_back(string("filterListdirectionOutAclNameFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("maxiumPrefixLimit"));
        attributeNames.push_back(string("maxiumPrefixthreshold"));
        attributeNames.push_back(string("maxiumPrefixTeardownFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultOriginateStatus"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultOriginateRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("prefixListInName"));
        attributeNames.push_back(string("prefixListInFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("prefixListOutName"));
        attributeNames.push_back(string("prefixListOutFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeMapNameIn"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeMapNameOut"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeReflectorClientFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("sendCommunityBoth"));
        attributeNames.push_back(string("sendCommunityExtended"));
        attributeNames.push_back(string("sendCommunityStandard"));
        attributeNames.push_back(string("sendCommunityStatus"));
        backendAttributesMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("unSuppressRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("weight"));
        backendAttributesMap[BGP_GROUP_CODE_AF_WEIGHT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_WEIGHT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_WEIGHT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("afIpv4NeighborIpAddress"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_IP_ADDRESS] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 
	//isAssociatedManagedObjectToBeCreated will be called only if the associated object does not exist. 
	status = BGP_CLI_ERR_PG_NOT_EXIST;
    pContext->setCompletionStatus (status);
    pContext->setIsAssociatedMOToBeCreated (isAssociatedMOToBeCreated);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if ((pAttribute->getAttributeName()== "defaultOriginateRouteMapName")) {
			addAttributeToBeUpdated("defaultOriginateStatus");
			setdefaultOriginateStatus(true);
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if ((pAttribute->getAttributeName()== "defaultOriginateRouteMapName")) {
			addAttributeToBeUpdated("defaultOriginateStatus");
			setdefaultOriginateStatus(true);
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;

		if ((pAttribute->getAttributeName()== "maxiumPrefixLimit")) { 
			setmaxiumPrefixLimit(0);
			setmaxiumPrefixthreshold(0);
			setmaxiumPrefixTeardownFlag(false);
		}
		if ((pAttribute->getAttributeName()== "defaultOriginateStatus")) { 
		  	setdefaultOriginateRouteMapName("");
		}
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["activateFlag"] = BGP_GROUP_CODE_AF_NEI_ACTIVATE;
        vectorOfAttributes.push_back("activateFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["filterListDirectionInAclName"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME;
        vectorOfAttributes.push_back("filterListDirectionInAclName");
        attributeNamesGroupCodeMap["filterListdirectionInAclNameFlag"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME;
        vectorOfAttributes.push_back("filterListdirectionInAclNameFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["filterListDirectionOutAclName"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME;
        vectorOfAttributes.push_back("filterListDirectionOutAclName");
        attributeNamesGroupCodeMap["filterListdirectionOutAclNameFlag"] = BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME;
        vectorOfAttributes.push_back("filterListdirectionOutAclNameFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["maxiumPrefixLimit"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixLimit");
        attributeNamesGroupCodeMap["maxiumPrefixthreshold"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixthreshold");
        attributeNamesGroupCodeMap["maxiumPrefixTeardownFlag"] = BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT;
        vectorOfAttributes.push_back("maxiumPrefixTeardownFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultOriginateStatus"] = BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE;
        vectorOfAttributes.push_back("defaultOriginateStatus");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["defaultOriginateRouteMapName"] = BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP;
        vectorOfAttributes.push_back("defaultOriginateRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["prefixListInName"] = BGP_GROUP_CODE_AF_PREFIX_LIST_IN;
        vectorOfAttributes.push_back("prefixListInName");
        attributeNamesGroupCodeMap["prefixListInFlag"] = BGP_GROUP_CODE_AF_PREFIX_LIST_IN;
        vectorOfAttributes.push_back("prefixListInFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["prefixListOutName"] = BGP_GROUP_CODE_AF_PREFIX_LIST_OUT;
        vectorOfAttributes.push_back("prefixListOutName");
        attributeNamesGroupCodeMap["prefixListOutFlag"] = BGP_GROUP_CODE_AF_PREFIX_LIST_OUT;
        vectorOfAttributes.push_back("prefixListOutFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeMapNameIn"] = BGP_GROUP_CODE_AF_ROUTE_MAP_IN;
        vectorOfAttributes.push_back("routeMapNameIn");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeMapNameOut"] = BGP_GROUP_CODE_AF_ROUTE_MAP_OUT;
        vectorOfAttributes.push_back("routeMapNameOut");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["routeReflectorClientFlag"] = BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT;
        vectorOfAttributes.push_back("routeReflectorClientFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["sendCommunityBoth"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityBoth");
        attributeNamesGroupCodeMap["sendCommunityExtended"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityExtended");
        attributeNamesGroupCodeMap["sendCommunityStandard"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityStandard");
        attributeNamesGroupCodeMap["sendCommunityStatus"] = BGP_GROUP_CODE_AF_SEND_COMMUNITY;
        vectorOfAttributes.push_back("sendCommunityStatus");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["unSuppressRouteMapName"] = BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP;
        vectorOfAttributes.push_back("unSuppressRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_WEIGHT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["weight"] = BGP_GROUP_CODE_AF_WEIGHT;
        vectorOfAttributes.push_back("weight");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_WEIGHT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["afIpv4NeighborPeerGrpName"] = BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME;
        vectorOfAttributes.push_back("afIpv4NeighborPeerGrpName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("activateFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_ACTIVATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("filterListDirectionInAclName"));
        attributeNames.push_back(string("filterListdirectionInAclNameFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_IN_ACL_NAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("filterListDirectionOutAclName"));
        attributeNames.push_back(string("filterListdirectionOutAclNameFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_FILTER_LIST_OUT_ACL_NAME] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("maxiumPrefixLimit"));
        attributeNames.push_back(string("maxiumPrefixthreshold"));
        attributeNames.push_back(string("maxiumPrefixTeardownFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_MAX_PREFIX_LIMIT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultOriginateStatus"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("defaultOriginateRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NEI_DEFAULT_ORIGINATE_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("prefixListInName"));
        attributeNames.push_back(string("prefixListInFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_IN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("prefixListOutName"));
        attributeNames.push_back(string("prefixListOutFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_PREFIX_LIST_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeMapNameIn"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_IN] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeMapNameOut"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_MAP_OUT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("routeReflectorClientFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_ROUTE_REFLECTOR_CLIENT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("sendCommunityBoth"));
        attributeNames.push_back(string("sendCommunityExtended"));
        attributeNames.push_back(string("sendCommunityStandard"));
        attributeNames.push_back(string("sendCommunityStatus"));
        backendAttributesMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_SEND_COMMUNITY] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("unSuppressRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_UNSUPPRESS_ROUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("weight"));
        backendAttributesMap[BGP_GROUP_CODE_AF_WEIGHT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_WEIGHT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_WEIGHT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("afIpv4NeighborPeerGrpName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_IPV4_NEIGHBOR_PEER_GRP_NAME] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["networkIpv4AddressPrefixMask"] = BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX;
        vectorOfAttributes.push_back("networkIpv4AddressPrefixMask");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NETWORK_WEIGHT] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["networkWeight"] = BGP_GROUP_CODE_AF_NETWORK_WEIGHT;
        vectorOfAttributes.push_back("networkWeight");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NETWORK_WEIGHT] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NETWORK_BACKDOOR] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["networkbackDoorFlag"] = BGP_GROUP_CODE_AF_NETWORK_BACKDOOR;
        vectorOfAttributes.push_back("networkbackDoorFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NETWORK_BACKDOOR] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["networkRouteMapName"] = BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP;
        vectorOfAttributes.push_back("networkRouteMapName");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("networkIpv4AddressPrefixMask"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NETWORK_ADDR_PREFIX] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("networkWeight"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NETWORK_WEIGHT] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NETWORK_WEIGHT] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NETWORK_WEIGHT] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("networkbackDoorFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NETWORK_BACKDOOR] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NETWORK_BACKDOOR] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NETWORK_BACKDOOR] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("networkRouteMapName"));
        backendAttributesMap[BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AF_NETWORK_ROUTE_MAP] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["advertiseMap"] = BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP;
        vectorOfAttributes.push_back("advertiseMap");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_AS_SET] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["asSetFlag"] = BGP_GROUP_CODE_AGGR_ADDR_AS_SET;
        vectorOfAttributes.push_back("asSetFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_AS_SET] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["attributeMap"] = BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP;
        vectorOfAttributes.push_back("attributeMap");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["summaryOnlyFlag"] = BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY;
        vectorOfAttributes.push_back("summaryOnlyFlag");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["suppressMap"] = BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP;
        vectorOfAttributes.push_back("suppressMap");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_AGGR_ADDR_PREFIX] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["aggregateIpv4PrefixMask"] = BGP_GROUP_CODE_AGGR_ADDR_PREFIX;
        vectorOfAttributes.push_back("aggregateIpv4PrefixMask");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_AGGR_ADDR_PREFIX] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("advertiseMap"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_ADVERTISE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("asSetFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_AS_SET] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_AS_SET] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_AS_SET] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("attributeMap"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_ATTRIBUTE_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("summaryOnlyFlag"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_SUMMARY_ONLY] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("suppressMap"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_SUPPRESS_MAP] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("aggregateIpv4PrefixMask"));
        backendAttributesMap[BGP_GROUP_CODE_AGGR_ADDR_PREFIX] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_AGGR_ADDR_PREFIX] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_AGGR_ADDR_PREFIX] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::isAssociatedManagedObjectToBeCreated: Add code handling if we need to create the Associated Managed Object
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForCreate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForUpdate: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
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

void BgpLocalIpv4StaticNetworkManagedObject::cleanupStaticNetworkConfiguration(UpdateNewAttributeInManagedObjectContext *pContext)
{
    ObjectId ownerObjectId = getOwnerManagedObjectId();
    ObjectId objectid = getObjectId();

    deleteFromComposition(BgpLocalAfIpv4UcastManagedObject::getClassName(),
        BgpLocalIpv4StaticNetworkManagedObject::getClassName(), string("staticNetwork"),
        ownerObjectId, objectid);
}



/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForDelete: Add new Attributes for Create
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<Attribute*> attributes = pContext->getAttributes();
    UI32 numberOfAttributes =  attributes.size();
    Attribute *pAttribute = NULL;
    for (UI32 i=0; i<numberOfAttributes; i++)
    {
        pAttribute = attributes[i];
        trace(TRACE_LEVEL_INFO, string("Attribute Name :") + pAttribute->getAttributeName());;
		if(pAttribute->getAttributeName()== "staticNetworkDistance") {
        	cleanupStaticNetworkConfiguration(pContext);
        }
    }
 
    pContext->setCompletionStatus (status);
    pContext->callback ();
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForCreate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForUpdate: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForDelete: Get the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForPostboot: Get postboot Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::setHardwareConfigurationDetails(map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap)
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
    groupCodeBackendNamesMap[BGP_GROUP_CODE_STATIC_NETWORK_PREFIX] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["staticNetworkAddressPrefix"] = BGP_GROUP_CODE_STATIC_NETWORK_PREFIX;
        vectorOfAttributes.push_back("staticNetworkAddressPrefix");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_STATIC_NETWORK_PREFIX] = vectorOfAttributes;
    }

    //Adding the Group realted details
    groupCodeBackendNamesMap[BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE] = "bgp";
    {
        vector<string> vectorOfAttributes;
        attributeNamesGroupCodeMap["staticNetworkDistance"] = BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE;
        vectorOfAttributes.push_back("staticNetworkDistance");
        groupCodeAttributeGroupMap[BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE] = vectorOfAttributes;
    }

    s_groupCodeBackendNamesMap = groupCodeBackendNamesMap;
    s_attributeNamesGroupCodeMap = attributeNamesGroupCodeMap;
    s_groupCodeAttributeGroupMap = groupCodeAttributeGroupMap;
    s_isMapPopulated = true;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setPostbootHardwareConfigurationDetails: Set the Backend Communication details
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::setPostbootHardwareConfigurationDetails(map<UI32, vector <string> > &backendAttributesMap,
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
        attributeNames.push_back(string("staticNetworkAddressPrefix"));
        backendAttributesMap[BGP_GROUP_CODE_STATIC_NETWORK_PREFIX] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_STATIC_NETWORK_PREFIX] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_STATIC_NETWORK_PREFIX] = false;
    }

    //Adding the Group related details
    {
        vector <string> attributeNames;
        attributeNames.push_back(string("staticNetworkDistance"));
        backendAttributesMap[BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE] = attributeNames;

        groupCodeMap[BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE] = "bgp";
        groupTypeMap[BGP_GROUP_CODE_STATIC_NETWORK_DISTANCE] = false;
    }

    s_backendAttributesMap = backendAttributesMap;
    s_groupMap = groupCodeMap;
    s_groupTypeMap = groupTypeMap;
    s_choiceToCasesMap = choiceToCasesMap;
    s_isMapPopulated = true;
}


}

