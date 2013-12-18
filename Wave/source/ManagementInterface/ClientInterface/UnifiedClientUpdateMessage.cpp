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
 *   Author : Anand Kumar Subramanian                                      *
 **************************************************************************/

#include "ManagementInterface/ClientInterface/UnifiedClientUpdateMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

UnifiedClientUpdateMessage::UnifiedClientUpdateMessage ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), UNIFIEDCLIENTUPDATE),
      m_warningResourceId (0),
      m_isConfigReplayInProgress (false)

{
}
UnifiedClientUpdateMessage::UnifiedClientUpdateMessage (const UI32 opCode)
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), opCode),
      m_warningResourceId (0),
      m_isConfigReplayInProgress (false)
{
}

UnifiedClientUpdateMessage::UnifiedClientUpdateMessage (const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext)
   : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), UNIFIEDCLIENTUPDATE),
     m_warningResourceId (0),
     m_isConfigReplayInProgress (false)
{
    initializeFromContext( pWaveManagedObjectAsynchronousPluginContext );
}

UnifiedClientUpdateMessage::UnifiedClientUpdateMessage (const UI32 opCode, const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext) 
   : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), opCode),
     m_warningResourceId (0),
     m_isConfigReplayInProgress (false)
{
    initializeFromContext( pWaveManagedObjectAsynchronousPluginContext );
}

void UnifiedClientUpdateMessage::initializeFromContext( const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext )
{   
    vector<Attribute* > attributesVector = pWaveManagedObjectAsynchronousPluginContext->getAttributes ();
    string              attributeString;

    if (0 == attributesVector.size ())
    {   
        trace (TRACE_LEVEL_FATAL, string ("UnifiedClientUpdateMessage::UnifiedClientUpdateMessage:: No attributes to send to the backend"));
        prismAssert (false, __FILE__, __LINE__);
    }

    for (UI32 i = 0; i < attributesVector.size (); i++)
    {   
        m_attributeTypes.push_back((attributesVector[i]->getAttributeType ()).getAttributeTypeResourceId ());
        m_attributeNames.push_back (attributesVector[i]->getAttributeName ());
        attributesVector[i]->toString (attributeString);
        trace(TRACE_LEVEL_DEVEL,string("Attribute Name = ")+attributesVector[i]->getAttributeName ()+string(" Attribute Value = ")+attributeString + string(" Attribute Type = ")+ FrameworkToolKit::localize( m_attributeTypes[i] ));
        m_attributeValues.push_back (attributeString);
    }

    m_managedObjectName = pWaveManagedObjectAsynchronousPluginContext->getManagedObjectName ();
    m_groupCode = pWaveManagedObjectAsynchronousPluginContext->getGroupCode ();
    pWaveManagedObjectAsynchronousPluginContext->getParentManagedObjectNames(m_parentManagedObjectNames);

    trace(TRACE_LEVEL_DEVEL,string("Managed Object Name = ")+pWaveManagedObjectAsynchronousPluginContext->getManagedObjectName ());
    trace(TRACE_LEVEL_DEVEL,string("Group Code = ")+pWaveManagedObjectAsynchronousPluginContext->getGroupCode ());
    trace(TRACE_LEVEL_DEVEL,string("isConfigReplayInProgress = ")+m_isConfigReplayInProgress);
}

UnifiedClientUpdateMessage::~UnifiedClientUpdateMessage ()
{
}

void UnifiedClientUpdateMessage::setupAttributesForSerialization()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32             (&m_groupCode,                  "groupCode"));
    addSerializableAttribute (new AttributeString           (&m_managedObjectName,          "managedObjectName"));
    addSerializableAttribute (new AttributeResourceIdVector (&m_attributeTypes,             "attributeTypes"));
    addSerializableAttribute (new AttributeStringVector     (&m_attributeNames,             "attributeNames"));
    addSerializableAttribute (new AttributeStringVector     (&m_attributeValues,            "attributeValues"));
    addSerializableAttribute (new AttributeStringVector     (&m_parentManagedObjectNames,   "parentManagedObjectNames"));
    addSerializableAttribute (new AttributeUI32             (&m_warningResourceId,          "warningResourceId"));
    addSerializableAttribute (new AttributeBool             (&m_isConfigReplayInProgress,   "isConfigReplayInProgress"));
    addSerializableAttribute (new AttributeStringVector     (&m_attributeNamesFromBackend,  "attributeNamesFromBackend"));
    addSerializableAttribute (new AttributeStringVector     (&m_attributeValuesFromBackend, "attributeValuesFromBackend"));
}

void UnifiedClientUpdateMessage::setManagedObjectName (const string &managedObjectName)
{
    m_managedObjectName = managedObjectName;
}

string UnifiedClientUpdateMessage::getManagedObjectName ()
{
    return (m_managedObjectName);
}

void UnifiedClientUpdateMessage::setAttributeTypes (const vector<ResourceId> &attributeTypes)
{
    m_attributeTypes =  attributeTypes;
}

vector<ResourceId> UnifiedClientUpdateMessage::getAttributeTypes () const
{
    return (m_attributeTypes);
}

void UnifiedClientUpdateMessage::setAttributeNames (const vector<string> &attributeNames)
{
    m_attributeNames = attributeNames;
}

vector<string> UnifiedClientUpdateMessage::getAttributeNames () const
{
    return (m_attributeNames);
}

void UnifiedClientUpdateMessage::setAttributeValues (const vector<string> &attributeValues)
{
    m_attributeValues = attributeValues;
}

vector<string> UnifiedClientUpdateMessage::getAttributeValues () const
{
    return (m_attributeValues);
}

UI32 UnifiedClientUpdateMessage::getGroupCode () const
{
    return (m_groupCode);
}

void UnifiedClientUpdateMessage::setGroupCode (UI32 groupCode)
{
    m_groupCode = groupCode;
}

void UnifiedClientUpdateMessage::setParentManagedObjectNames   ( const vector<string> &parentManagedObjectNames)
{
    m_parentManagedObjectNames = parentManagedObjectNames;
}

void UnifiedClientUpdateMessage::getParentManagedObjectNames   ( vector<string> &parentManagedObjectNames) const
{
    parentManagedObjectNames = m_parentManagedObjectNames;
}


const void *UnifiedClientUpdateMessage::getCStructureForInputs ()
{
    UnifiedClientMessage_t *pInput                  = new UnifiedClientMessage_t ();
    UI32                    attributeNamesSize      = m_attributeNames.size ();
    UI32                    attributeValuesSize     = m_attributeValues.size ();
    UI32                    attributeTypeSize       = m_attributeTypes.size ();
    UI32                    i                       = 0;

    UI32                    parentManagedObjectNamesSize = m_parentManagedObjectNames.size();

    prismAssert (NULL != pInput, __FILE__, __LINE__);
    
    if (0 != attributeNamesSize)
    {
        prismAssert (attributeNamesSize == attributeValuesSize, __FILE__, __LINE__);
        prismAssert (attributeNamesSize == attributeTypeSize, __FILE__, __LINE__);


        // Allocate memory for attributeNames
        pInput->attributeNames = new const char *[attributeNamesSize];
        prismAssert (NULL != pInput->attributeNames, __FILE__, __LINE__);

        //Allocate memory for all the attributeValues 
        pInput->attributeValues = new WaveCValue [attributeValuesSize];
        prismAssert (NULL != pInput->attributeValues, __FILE__, __LINE__);

        if ( 0 < parentManagedObjectNamesSize )
        {
            //Allocate memory for all parentManagedObjectNames
            pInput->parentManagedObjectNames = new const char *[parentManagedObjectNamesSize];
            prismAssert (NULL != pInput->parentManagedObjectNames, __FILE__, __LINE__);
            pInput->parentManagedObjectNamesSize = parentManagedObjectNamesSize;

            for ( UI32 j = 0; j < parentManagedObjectNamesSize; j++ )
            {
                pInput->parentManagedObjectNames[j] = m_parentManagedObjectNames[j].c_str();
            }
        }
        else
        {
            pInput->parentManagedObjectNames = NULL ;
            pInput->parentManagedObjectNamesSize = 0;
        }

        for (i = 0; i < attributeNamesSize; i++)
        {
            pInput->attributeNames[i] = m_attributeNames[i].c_str ();
            trace(TRACE_LEVEL_DEVEL,string("Attribute Name = ")+ m_attributeNames[i] +string(" Attribute Value = ")+ m_attributeValues[i] + string(" Attribute Type = ")+ FrameworkToolKit::localize( m_attributeTypes[i] ));    
            // Get WaveCValue of the attributeValue and copy into the PD structure.
           
            getAttributeCValue (m_attributeTypes[i], m_attributeValues[i], &(pInput->attributeValues[i]));
        }

        pInput->attributeSize = attributeValuesSize;
        pInput->managedObjectName = m_managedObjectName.c_str ();
        pInput->groupCode = m_groupCode;
        pInput->numberOfRecordsRequested = 0;

        if (true == m_isConfigReplayInProgress)
        {
            pInput->isConfigReplayInProgress = 1;
        }
        else
        {
            pInput->isConfigReplayInProgress = 0;
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("UnifiedClientUpdateMessage::getCStructureForInputs :: No attributes to send to the backend"));
        prismAssert (false, __FILE__, __LINE__);
        delete (pInput);
        pInput = NULL;
    }

    return (pInput);
}

void UnifiedClientUpdateMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{

    if ( NULL == pOutputCStructure )
    {
        return ;
    }

    UnifiedClientMessageOutputForConfig_t *pResult = (UnifiedClientMessageOutputForConfig_t *)pOutputCStructure;
    prismAssert ( NULL != pResult, __FILE__, __LINE__);
 
    UI32 warningCode = pResult->warningCode;
    
    setWarningResourceId (warningCode);

    for (int i = 0; i < pResult->attributeSize; i++)
    {
        m_attributeNamesFromBackend.push_back  (pResult->attributeNames[i]);
        m_attributeValuesFromBackend.push_back (pResult->attributeValues[i]);
    }

    free (pResult); 
}

void UnifiedClientUpdateMessage::getAttributeCValue (const ResourceId attributeType, const string &attributeValue, WaveCValue *pCValue)
{
    Attribute *pAttribute = Attribute::getAttributeFromAttributeType (attributeType);
    
    pAttribute->fromString (attributeValue);
    pAttribute->getCValue (pCValue);

    delete (pAttribute);
    pAttribute = NULL;
}

void UnifiedClientUpdateMessage::deleteCStructureForInputs (const void *pInputStruct)
{
    if (pInputStruct)
    {
        UnifiedClientMessage_t *pInput                  = (UnifiedClientMessage_t *) pInputStruct;

        for (SI32 i = 0; i < pInput->attributeSize; i++)
        {
            wave_delete_cvalue (&pInput->attributeValues[i]);
        }

        if ( 0 < pInput->parentManagedObjectNamesSize )
        {
            delete [] (pInput->parentManagedObjectNames);
            pInput->parentManagedObjectNames = NULL;
        }


        delete (pInput->attributeValues);
        pInput->attributeValues = NULL;

        delete (pInput->attributeNames);
        pInput->attributeNames = NULL;

        delete (pInput);
    }
    
}

UI32 UnifiedClientUpdateMessage::getWarningResourceId () const
{
    return (m_warningResourceId);
}
 
void UnifiedClientUpdateMessage::setWarningResourceId (const UI32 warningResourceId)
{
    m_warningResourceId = warningResourceId;
}

void UnifiedClientUpdateMessage::setConfigReplayInProgressFlag (const bool isConfigReplayInProgress)
{
    m_isConfigReplayInProgress = isConfigReplayInProgress;
}

bool UnifiedClientUpdateMessage::getConfigReplayInProgressFlag () const
{
    return (m_isConfigReplayInProgress);
}

 
vector<string> UnifiedClientUpdateMessage::getAttributeNamesFromBackend () const
{
    return (m_attributeNamesFromBackend);
}
 
vector<string> UnifiedClientUpdateMessage::getAttributeValuesFromBackend () const
{
    return (m_attributeValuesFromBackend);
}

}
