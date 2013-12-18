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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "SystemManagement/WaveSystemManagementGateway.h"
#include "SystemManagement/WaveSystemManagementGatewayMessage.h"
#include "SystemManagement/WaveSystemManagementTypes.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "SystemManagement/WaveConfigurationSegmentWorkerMap.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"
#include "Framework/Database/DatabaseMultiConnection.h"
#include "Framework/Database/MultiDatabaseToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"

namespace WaveNs
{

map<string, ObjectId> WaveSystemManagementGatewayWorker::m_targetNodeNameObjectIdMap;

WaveSystemManagementGatewayWorker::WaveSystemManagementGatewayWorker (WaveObjectManager *pWaveObjectManager)
    :   WaveWorker (pWaveObjectManager)
{
    addOperationMap (WAVE_SYSTEM_MANAGEMENT_GATEWAY_MESSAGE, reinterpret_cast<PrismMessageHandler> (&WaveSystemManagementGatewayWorker::gatewayMessageHandler));
}

WaveSystemManagementGatewayWorker::~WaveSystemManagementGatewayWorker ()
{
}

/*
 * Static function called from each WaveConfigurationSegmentWorker when the OM 
 * creates the worker. This is so that the Gateway knows which Configuration 
 * Segments are registered. 
 */
void WaveSystemManagementGatewayWorker::registerConfigurationSegment (const string &configSegmentName, WaveConfigurationSegmentWorker *pWaveConfigurationSegmentWorker)
{
}

void WaveSystemManagementGatewayWorker::gatewayMessageHandler (WaveSystemManagementGatewayMessage *pWaveSystemManagementGatewayMessage)
{
    trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::gatewayMessageHandler : Entering ... ");

    string inputArguments   = pWaveSystemManagementGatewayMessage->getWriteInputArguments ();
    string subMode          = pWaveSystemManagementGatewayMessage->getWriteSubMode ();

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::gatewayMessageHandler : Input Arguments: '%s', subMode: '%s'.", inputArguments.c_str (), subMode.c_str ());

    string errorMessage("");

    ResourceId status = processWriteRequest (inputArguments, subMode, errorMessage, pWaveSystemManagementGatewayMessage->getIsWyserEaCompatibilityMode());

    pWaveSystemManagementGatewayMessage->setCommandErrorString(errorMessage);

    pWaveSystemManagementGatewayMessage->setCompletionStatus (status);
    reply (pWaveSystemManagementGatewayMessage);

}

YangElement *WaveSystemManagementGatewayWorker::getKeysAndAttributesForInput (const string &input, vector<string> &keyAttributeMoNames, vector<string> &keyAttributeNames, vector<string> &keyAttributeValues, vector<UI32> &keyAttributeUserTags, vector<YangElement *> &keyYangElement, string &targetNodeNameForData, YangElement *subModeTargetNodeName, bool isWyserEaCompatibilityMode)
{
    map<string, string> listItemsInTargetNodeNameMap; 
    vector<pair <string, string> > leafItemsInTargetNodeNamePair;
    
    // 1. Parse the input arguments and create a XPath representation of the input
    YangElement *pTargetYangElement = parseInputArguments (input, listItemsInTargetNodeNameMap, leafItemsInTargetNodeNamePair, subModeTargetNodeName, isWyserEaCompatibilityMode);

    // If no yangElement could be found for input string (that is, if yangElement is null),
    // return "null" to the caller, immediately. The caller of this function is assumed
    // to handle the case (probably throw error) when this function returns is null.
    if (pTargetYangElement == NULL) {
        return NULL;
    }

    map<string, string>::iterator itemsListIterator;

    for (itemsListIterator = listItemsInTargetNodeNameMap.begin ();
            itemsListIterator != listItemsInTargetNodeNameMap.end ();
            itemsListIterator++)
    {

        vector<YangElement *> pYangElements = (WaveSystemManagementObjectManager::getYangUserInterface ())->getYangElementsByTargetNodeName (itemsListIterator->first);

        if (pYangElements.size () == 0)
        {
            break;
        }

        YangList *pYangList = dynamic_cast<YangList *> (pYangElements[0]); 
       
        if (NULL != pYangList)
        {
            // Config Segment Name
            string configSegmentName = pYangList->getConfigurationSegmentName ();

            // Get MO Name from configSegmentName
            string moName = WaveConfigurationSegmentMap::getManagedObjectClassNameByConfigurationSegmentName (configSegmentName);

            // Find out Key Combinations from YANG tree and ORM and check for consistency 
            vector<string> keyCombinationFromYang = pYangList->getKeyCombination ();
            
            vector<string> keyCombinationFromOrm; 

            OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (moName, keyCombinationFromOrm);
            
            if (!(keyCombinationFromYang.size () == keyCombinationFromOrm.size ()))
            {
                trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::getKeysAndAttributesForInput: Key Combinations of Yang And ORM are different .. Returning without processing Write request");
                return (NULL);
            }
        
            // Package the Keys into the vectors
            for (UI32 keyComboCtr = 0; keyComboCtr < keyCombinationFromYang.size (); keyComboCtr++)
            {
                // Get YangElement for Key from Yang tree
                YangElement *pKeyElement = pYangList->getUniqueYangElementByName (keyCombinationFromYang[keyComboCtr]);
            
                keyAttributeMoNames.push_back   (moName);
                keyAttributeNames.push_back     (keyCombinationFromOrm[keyComboCtr]);
                keyAttributeValues.push_back    (listItemsInTargetNodeNameMap[pYangList->getCliTargetNodeNameForData ()]);
                keyAttributeUserTags.push_back  (pKeyElement->getUserTagValue ());
                keyYangElement.push_back        (pKeyElement);
            }

            keyCombinationFromYang.clear ();
            keyCombinationFromOrm.clear ();
        }
    }

    targetNodeNameForData = pTargetYangElement->getCliTargetNodeNameForData ();
    
    for (vector<pair <string, string> >::iterator nameValueItr = leafItemsInTargetNodeNamePair.begin();
    		nameValueItr != leafItemsInTargetNodeNamePair.end(); nameValueItr++)
    {
		keyAttributeMoNames.push_back   ("");
		keyAttributeNames.push_back     ((*nameValueItr).first);
		keyAttributeValues.push_back    ((*nameValueItr).second);
		keyAttributeUserTags.push_back  (0);
		keyYangElement.push_back        (NULL);
    }

    listItemsInTargetNodeNameMap.clear ();
    leafItemsInTargetNodeNamePair.clear();

    return (pTargetYangElement);
}

ResourceId WaveSystemManagementGatewayWorker::processWriteRequest (string inputArguments, string subMode, string &errorMessage, bool isWyserEaCompatibilityMode)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::processWriteRequest : %s, %s \n\r", inputArguments.c_str (), subMode.c_str ());

    UI32 typeOfOperation = 0; // 0 for No-Op, 1 for Create, 2 for Update and 3 for Delete

    vector<string> keyAttributeMoNames; 
    vector<string> keyAttributeNames; 
    vector<string> keyAttributeValues;
    vector<UI32>   keyAttributeUserTags;
    vector<YangElement *> keyYangElement;
    string targetNodeNameForData;
    string configSegmentName;
    string moName;
    YangElement *pTargetYangElement = NULL;
    YangElement *pSubModeTargetYangElement = NULL;

    bool isSubModePresent = false;
    ObjectId parentObjectId = ObjectId::NullObjectId;
    string subModeTargetNodeName = "";

    if (subMode != "none")
    { 
        isSubModePresent = true;
    }    

	if(strncasecmp(inputArguments.c_str(), "no ", 3) == 0 )
	{
		typeOfOperation = 3;
		// removing the "no" token from the inputArguments
		inputArguments.erase(inputArguments.begin(), inputArguments.begin()+2);
	}
	else
	{
		typeOfOperation = 1;
	}

    if (isSubModePresent == true)
    {
        StringUtils::stripAllOccurancesAtTheEnd (subMode, ' ');
        parentObjectId = getObjectIdForTargetNodeName (subMode);

        if (parentObjectId == ObjectId::NullObjectId)
        {
            tracePrintf (TRACE_LEVEL_WARN, true, false, "WaveSystemManagementGatewayWorker::processWriteRequest: OBJECTID IS NULL.");
        }
        string inputArgWithSubMode = subMode + " " + inputArguments;

        pTargetYangElement = getKeysAndAttributesForInput (inputArgWithSubMode, keyAttributeMoNames, keyAttributeNames, keyAttributeValues, keyAttributeUserTags, keyYangElement, targetNodeNameForData, pSubModeTargetYangElement, isWyserEaCompatibilityMode);
    }

    if ( NULL != pTargetYangElement )
    {
    	configSegmentName = pTargetYangElement->getConfigurationSegmentName ();
    }

    // checking without submode, to handle the corner cases like global command (mac-address-table..)
    // comes inside other submode like vlan.
    if ( configSegmentName.length() == 0 )
    {
        keyAttributeMoNames.clear();
        keyAttributeNames.clear();
        keyAttributeValues.clear();
        keyAttributeUserTags.clear();
        keyYangElement.clear();

        pTargetYangElement = getKeysAndAttributesForInput (inputArguments, keyAttributeMoNames, keyAttributeNames, keyAttributeValues, keyAttributeUserTags, keyYangElement, targetNodeNameForData, pSubModeTargetYangElement, isWyserEaCompatibilityMode);
    }

    if (NULL == pTargetYangElement)
    {
        trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::processWriteRequest : TARGET YANG ELEMENT FROM INPUT IS NULL !!! The INPUT arguments dont match any Yang Target Node Name");
        status = WAVE_MESSAGE_ERROR;
        errorMessage = FrameworkToolKit::localize(status);
        return (status);
    }

    // Config Segment Name
    configSegmentName = pTargetYangElement->getConfigurationSegmentName ();

    trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::processWriteRequest: configSegmentName = " + configSegmentName);
    ConfigurationSegmentImportantInfo *pConfigurationSegmentImportantInfo = NULL; 
    
    if (true == MultiDatabaseToolKit::getIsMultiModeEnabled ())
    {
        // Get ConfigSegmentImportantInfo for configSegmentName
        pConfigurationSegmentImportantInfo = WaveConfigurationSegmentMap::getConfigurationSegmentImportantInfoFromMap (configSegmentName);

        // Get MO Name from configSegmentName
        moName = pConfigurationSegmentImportantInfo->getManagedObjectName ();
    }
    else 
    {
    moName = WaveConfigurationSegmentMap::getManagedObjectClassNameByConfigurationSegmentName (configSegmentName);
    }
   
    // Find the WCSW pointer for CS Name
    if (WaveConfigurationSegmentWorkerMap::isConfigurationSegmentNameInMap (configSegmentName) == true)
    {
        WaveConfigurationSegmentWorker *pWaveConfigurationSegmentWorker = WaveConfigurationSegmentWorkerMap::m_configurationSegmentWorkerMap[configSegmentName];
    
        if (pWaveConfigurationSegmentWorker != NULL)
        {
        // Get the 
            bool isConfigExistValidationRequired = pWaveConfigurationSegmentWorker->getIsConfigExistValidationRequired ();

            if (true == isConfigExistValidationRequired)
            {
                typeOfOperation = doesManagedObjectForKeyExist (moName, keyAttributeMoNames, keyAttributeNames, keyAttributeValues, keyAttributeUserTags, targetNodeNameForData, isSubModePresent, parentObjectId, subModeTargetNodeName, true);
            }

            if (typeOfOperation == 1)
            {
                // CREATE 
                WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext = new WaveConfigurationSegmentCreateContext ();

                pWaveConfigurationSegmentCreateContext->setManagedObjectClassName (moName);
                pWaveConfigurationSegmentCreateContext->setAttributeUserTags (keyAttributeUserTags);
                pWaveConfigurationSegmentCreateContext->setAttributeNames (keyAttributeNames);
                pWaveConfigurationSegmentCreateContext->setAttributeValues (keyAttributeValues);
                pWaveConfigurationSegmentCreateContext->setParentObjectId (parentObjectId);
                
                ManagementInterfaceMessage *pMessage = pWaveConfigurationSegmentWorker->createConfig (pWaveConfigurationSegmentCreateContext);

                status = pWaveConfigurationSegmentCreateContext->getConfigHandlerStatus();

                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    status = sendManagementInterfaceMessage (pMessage, pConfigurationSegmentImportantInfo, pWaveConfigurationSegmentCreateContext->getLocationId());

                    if (WAVE_MESSAGE_SUCCESS == status)
                    {
                        if (true == isConfigExistValidationRequired)
                        {
                            doesManagedObjectForKeyExist (moName, keyAttributeMoNames, keyAttributeNames, keyAttributeValues, keyAttributeUserTags, targetNodeNameForData, isSubModePresent, parentObjectId, subModeTargetNodeName, false);
                        }
                    }
                    else
                    {
    					errorMessage = pWaveConfigurationSegmentWorker->getErrorString(pMessage);
                    }
                }
                else
                {
                	errorMessage = pWaveConfigurationSegmentCreateContext->getErrorString();
                }

                if ( pMessage )
                {
                	delete (pMessage);
                }
            }
            else if (typeOfOperation == 2)
            {
        		pWaveConfigurationSegmentWorker->updateConfig ();
            }
            else if (typeOfOperation == 3)
            {
                // DELETE
                WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext = new WaveConfigurationSegmentDeleteContext ();

                pWaveConfigurationSegmentDeleteContext->setManagedObjectClassName (moName);
                pWaveConfigurationSegmentDeleteContext->setAttributeUserTags (keyAttributeUserTags);
                pWaveConfigurationSegmentDeleteContext->setAttributeNames (keyAttributeNames);
                pWaveConfigurationSegmentDeleteContext->setAttributeValues (keyAttributeValues);
                pWaveConfigurationSegmentDeleteContext->setParentObjectId (parentObjectId);

                ManagementInterfaceMessage *pMessage = pWaveConfigurationSegmentWorker->deleteConfig (pWaveConfigurationSegmentDeleteContext);

                status = pWaveConfigurationSegmentDeleteContext->getConfigHandlerStatus();

                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    status = sendManagementInterfaceMessage (pMessage, pConfigurationSegmentImportantInfo, pWaveConfigurationSegmentDeleteContext->getLocationId());

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                    	errorMessage = pWaveConfigurationSegmentWorker->getErrorString(pMessage);
                    }
                }
                else
                {
					errorMessage = pWaveConfigurationSegmentDeleteContext->getErrorString();
                }

                if ( pMessage )
                {
                	delete (pMessage);
                }
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveSystemManagementGatewayWorker::processWriteRequest : pWaveConfigurationSegmentWorker is NULL");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveSystemManagementGatewayWorker::processWriteRequest : Config Segment Name is not found in Map");
    }

    keyAttributeMoNames.clear ();
    keyAttributeNames.clear ();
    keyAttributeValues.clear ();
    keyAttributeUserTags.clear ();
    keyYangElement.clear ();
        
    return (status);
}

ResourceId WaveSystemManagementGatewayWorker::sendManagementInterfaceMessage (ManagementInterfaceMessage *pMessage, ConfigurationSegmentImportantInfo *pConfigurationSegmentImportantInfo, const LocationId &locationId)
{
    if (pMessage == NULL)
    {
        return (WAVE_MESSAGE_ERROR);
    }
    
    ResourceId status = WAVE_MESSAGE_ERROR;  

    if (pConfigurationSegmentImportantInfo != NULL)
    {
        // Get the right server to send the message to 
        WaveClientSynchronousConnection *pWaveClientSynchronousConnection = pConfigurationSegmentImportantInfo->getWaveClientSynchronousConnection ();
        
        prismAssert (pWaveClientSynchronousConnection != NULL, __FILE__, __LINE__);

        if (true == (pWaveClientSynchronousConnection->isCurrentlyConnected ()))
        {
            status = pWaveClientSynchronousConnection->sendSynchronouslyToWaveServer (pMessage, locationId);
        }
        else
        {
            status = pWaveClientSynchronousConnection->getConnectionStatus ();
            trace (TRACE_LEVEL_ERROR, "WaveSystemManagementGatewayWorker::sendManagementInterfaceMessage : Wave Client Synchronous Connection is not set up properly. Status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        // Enable HA config intents for Wyser write path.  This is to support reconciliation between backend and config database for write commands that were cut short due to ha failover.

        pMessage->setIsAConfigurationIntent (true);

        status = sendSynchronously (pMessage, locationId);
    }

    if (WAVE_MESSAGE_SUCCESS == status) 
    {
        status = pMessage->getCompletionStatus();
        if (status == WAVE_MESSAGE_SUCCESS)
        {            
            trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::sendManagementInterfaceMessage : Successfully Completed Message processing.");
        }
        else 
        {
            tracePrintf(TRACE_LEVEL_ERROR, true, false, "WaveSystemManagementGatewayWorker::sendManagementInterfaceMessage : Message Completion Status : %s. locationId = %d.", FrameworkToolKit::localize (status).c_str(), locationId);
            status = WAVE_MESSAGE_ERROR;
        }
    } 
    else 
    {
        tracePrintf(TRACE_LEVEL_ERROR, true, false, "WaveSystemManagementGatewayWorker::sendManagementInterfaceMessage : Failed to send message.  Status : %s. locationId = %d.", FrameworkToolKit::localize (status).c_str(), locationId);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

UI32 WaveSystemManagementGatewayWorker::doesManagedObjectForKeyExist (const string &managedObjectName, vector<string> &keyAttributeMoNames, vector<string> &keyAttributeNames, vector<string> &keyAttributeValues, vector<UI32> &keyAttributeUserTags, string &targetNodeName, bool isSubModePresent, ObjectId parentObjectId, string &subModeTargetNodeName, bool isFirstCall)
{
    UI32 typeOfOperation = -1;

    string targetNodeNameForData = targetNodeName;

    if (!(keyAttributeNames.size () == keyAttributeValues.size ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveSystemManagementGatewayWorker::doesManagedObjectForKeyExists : Input vector arguments are not the same size");
        return (typeOfOperation);
    }
    
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt (managedObjectName);

    for (UI32 i = 0; i < keyAttributeNames.size (); i++)
    {
        if (managedObjectName != keyAttributeMoNames[i])
        {
            continue;
        }

        AttributeType type = PrismPersistableObject::getTypeFromUserTag (keyAttributeMoNames[i], keyAttributeUserTags[i]);
        // FIXME: We are supporting only Attribute String as the key .. This is only for POC and will be changed to support all data types later on
        if (type == AttributeType::AttributeTypeString)
        {
            AttributeString *pAttributeKeyString = new AttributeString (keyAttributeValues[i], keyAttributeNames[i], keyAttributeUserTags[i]);
            syncQueryCtxt.addAndAttribute (pAttributeKeyString);
        }
        else if (type == AttributeType::AttributeTypeUI64)
        {
            AttributeUI64 *pAttributeKeyUI64 = new AttributeUI64 ((UI64)(atoi(keyAttributeValues[i].c_str ())), keyAttributeNames[i], keyAttributeUserTags[i]);
            syncQueryCtxt.addAndAttribute (pAttributeKeyUI64);
        }
        else
        {
            return (-1);
        }

        targetNodeNameForData += " ";
        targetNodeNameForData += keyAttributeValues[i];
    }
   
    if (isSubModePresent == true && parentObjectId != ObjectId::NullObjectId)
    {
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (parentObjectId, "ownerManagedObjectId"));
    }
    
    syncQueryCtxt.addSelectField ("objectId");
    vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
    UI32 numberOfResults = pResults->size ();

    if (numberOfResults == 0) 
    { 
        typeOfOperation = 1; // Create
    }
    else if (numberOfResults == 1)
    {
        if (subModeTargetNodeName != "")
        {    
            targetNodeNameForData = subModeTargetNodeName + " " + targetNodeNameForData;
        }

        ObjectId oid = (*pResults)[0]->getObjectId ();

        if (oid != ObjectId::NullObjectId)
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::doesManagedObjectForKeyExists: Target Node Name : %s...%s.", targetNodeNameForData.c_str (), oid.toString ().c_str ());
        }
        else
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::doesManagedObjectForKeyExists: Target Node Name : %s...Null ObjectId.", targetNodeNameForData.c_str ());
        
        }

        addTargetNodeNameObjectIdMap (targetNodeNameForData, (*pResults)[0]->getObjectId ());
        typeOfOperation = 2; // Update 
    }
            
    delete pResults;
      

    return (typeOfOperation);
}

YangElement *WaveSystemManagementGatewayWorker::parseInputArguments (const string &targetNodeName, map<string, string> &listItemsInTargetNodeNameMap, vector<pair <string, string> > &leafItemsInTargetNodeNamePair, YangElement *subModeTargetYangElement, bool isWyserEaCompatibilityMode)
{
    // 1. Tokenize the keys
    vector<string> targetNodeNameTokens;
    StringUtils::tokenize (targetNodeName, targetNodeNameTokens);

    string tempNode = "";
    string nodeNameWithoutKey = "";
    YangList *pYangList = NULL;
    YangElement *pTargetYangElement = NULL;

    // Loop over the tokens from the last one and find out the Yang Element for each token
    for (UI32 i=0; i< targetNodeNameTokens.size (); i++)
    {
        vector<YangElement *> pYangElements;

        pYangElements = WaveSystemManagementObjectManager::getYangUserInterface ()->getYangElementsByTargetNodeName ((tempNode + targetNodeNameTokens[i]));

        if( pYangElements.size() == 0 && true == isWyserEaCompatibilityMode )
        {
        	pYangElements = WaveSystemManagementObjectManager::getYangUserInterface ()->getYangElementsByTargetNodeNameForDropNode ((tempNode + targetNodeNameTokens[i]));
        }

    	if(pYangElements.size() != 0 && pYangElements[0]->getDropNodeNameForCli())
    	{
    		continue;
    	}

    	tempNode += targetNodeNameTokens[i];

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::parseInputArguments - Input is : %s.", tempNode.c_str ());

        if (pYangElements.size () != 0)
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveSystemManagementGatewayWorker::parseInputArguments - YangElement is : %s.", pYangElements[0]->getName ().c_str());
        
            nodeNameWithoutKey += targetNodeNameTokens[i];

            nodeNameWithoutKey += " ";

            // Find out if Yang Element is a list 
            pYangList = dynamic_cast<YangList *> (pYangElements[0]);

            if (NULL != pYangList)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::parseInputArguments: This is a list");

                string keyValue = "";

                int noOfKeys = pYangList->getKeyCombination().size();

                for (int j = 0; j < noOfKeys; j++)
                {
                    if (j != 0)
                    {
                        keyValue += " ";
                    }

                    keyValue += targetNodeNameTokens[++i];
                }

                listItemsInTargetNodeNameMap[pYangList->getCliTargetNodeNameForData()] = keyValue;

                pYangList = NULL;
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGatewayWorker::parseInputArguments: This is NOT a list");
            }
        } 
        else 
        {
            //tracePrintf (TRACE_LEVEL_INFO, false, false, "leafItemsInTargetNodeNameMap item is : %s\n", targetNodeNameTokens[i].c_str ());
        	leafItemsInTargetNodeNamePair.push_back( make_pair(targetNodeNameTokens[i], "1") );
        }

        tempNode += " ";
    }

    StringUtils::stripFromLastOccuranceOf (nodeNameWithoutKey, ' ');

    // Find out YangElement for nodeNameWithoutKey 
    vector<YangElement *> pYangElements = (WaveSystemManagementObjectManager::
    		getYangUserInterface ())->getYangElementsByTargetNodeName (nodeNameWithoutKey);
    
    if (pYangElements.size () != 0)
    {
        pTargetYangElement = pYangElements[0];
    }
    else {
        trace (TRACE_LEVEL_ERROR, string ("WaveSystemManagementGatewayWorker::parseInputArguments:")
                + "No YangElement found for targetNodeName \"" + targetNodeName + "\".");
    }

    return (pTargetYangElement);

}


void WaveSystemManagementGatewayWorker::addTargetNodeNameObjectIdMap (const string &targetNodeName, ObjectId moObjectId)
{
    m_targetNodeNameObjectIdMap[targetNodeName] = moObjectId;
}

bool WaveSystemManagementGatewayWorker::deleteTargetNodeNameObjectIdMap (const string &targetNodeName)
{
	map<string, ObjectId>::iterator targetNodeNameObjectIdMapIterator;

	targetNodeNameObjectIdMapIterator = m_targetNodeNameObjectIdMap.find (targetNodeName);

	if (targetNodeNameObjectIdMapIterator != m_targetNodeNameObjectIdMap.end ())

	{
		m_targetNodeNameObjectIdMap.erase(targetNodeNameObjectIdMapIterator);
		return (true);
	}

	return (false);
}

/**
 * To do the bulk deletion of the objectIds in the map, only if the objectId's key(target node name)
 * matches with the given base target node name.
 * Returns the number of deleted objectIds.
 */
UI32 WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName  (const string &baseTargetNodeName)
{
	UI32 deletedObjectIdCount = 0;
	map<string, ObjectId>::iterator targetNodeNameObjectIdMapIterator;
	size_t baseTargetNodeNameLength = baseTargetNodeName.length();

	for( targetNodeNameObjectIdMapIterator = m_targetNodeNameObjectIdMap.begin();
			targetNodeNameObjectIdMapIterator != m_targetNodeNameObjectIdMap.end(); )
	{
		string targetNodeName = targetNodeNameObjectIdMapIterator->first;

		if ( targetNodeName.compare(0, baseTargetNodeNameLength, baseTargetNodeName) == 0 )
		{
			m_targetNodeNameObjectIdMap.erase(targetNodeNameObjectIdMapIterator ++);
			deletedObjectIdCount ++;
		}
		else
		{
			++ targetNodeNameObjectIdMapIterator;
		}
	}

	return deletedObjectIdCount;
}

ObjectId WaveSystemManagementGatewayWorker::getObjectIdForTargetNodeName (const string &targetNodeName)
{
    if (true == isTargetNodeNameInObjectIdMap(targetNodeName))
    {
        return (m_targetNodeNameObjectIdMap[targetNodeName]);
    }
    else
    {
        return (ObjectId::NullObjectId);
    }
}

bool WaveSystemManagementGatewayWorker::isTargetNodeNameInObjectIdMap (const string &targetNodeName)
{
    map<string, ObjectId>::iterator targetNodeNameObjectIdMapIterator; 

    targetNodeNameObjectIdMapIterator = m_targetNodeNameObjectIdMap.find (targetNodeName);

    if (targetNodeNameObjectIdMapIterator != m_targetNodeNameObjectIdMap.end ())
    {
        return (true);    
    }
    else
    {
        return (false);
    }
}

bool WaveSystemManagementGatewayWorker::clearTargetNodeNameObjectIdMap ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, "WaveSystemManagementGatewayWorker::clearTargetNodeNameObjectIdMap : Entering ...");

    m_targetNodeNameObjectIdMap.clear ();

    return (true);
}

void WaveSystemManagementGatewayWorker::printContentsOfTargetNodeNameObjectIdMap ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, "WaveSystemManagementGatewayWorker::printContentsOfTargetNodeNameObjectIdMap : Entering ...");

    map<string, ObjectId>::iterator targetNodeNameObjectIdMapIterator;

    for (targetNodeNameObjectIdMapIterator = m_targetNodeNameObjectIdMap.begin ();
            targetNodeNameObjectIdMapIterator != m_targetNodeNameObjectIdMap.end ();
                targetNodeNameObjectIdMapIterator++)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("Target Node Name : ") + targetNodeNameObjectIdMapIterator->first.c_str () + string ("  Object Id : ") + targetNodeNameObjectIdMapIterator->second.toString().c_str ());
    }
}


}
