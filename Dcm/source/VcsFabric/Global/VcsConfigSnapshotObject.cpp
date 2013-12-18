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

/**
 *@file VcsConfigSnapshotObject.cpp
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file defines the
 *              Config snapshot Object used to store
 *              both meta-data and configuration 
 *         
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */

#include "VcsFabric/Global/VcsConfigSnapshotObject.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include <fstream>

using namespace std;

namespace DcmNs
{
VcsConfigSnapshotObject::VcsConfigSnapshotObject ()
    : SerializableObject        (),
      m_rbridgeId               (0),
      m_snapshotId              (""),
      m_SWBDModel               (-1),
      m_restoreOnDisconnectedNode (false),
      m_metadataOnlyObject      (false)  
{
}

VcsConfigSnapshotObject::~VcsConfigSnapshotObject ()
{
}

void VcsConfigSnapshotObject::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();
    //serialize only meta-data
    addSerializableAttribute (new AttributeUI32 (&m_rbridgeId, "rbridgeId"));
    addSerializableAttribute (new AttributeString (&m_snapshotId, "snapshotId"));
    addSerializableAttribute (new AttributeDateTime (&m_snapshotCreatedTime, "snapshotCreatedTime"));
    addSerializableAttribute (new AttributeSI32 (&m_SWBDModel, "SWBDModel"));
    addSerializableAttribute (new AttributeBool     (&m_restoreOnDisconnectedNode, "restoreOnDisconnectedNode"));
}

void VcsConfigSnapshotObject::setSnapshotId  (const string& snapshotId )
{   
    m_snapshotId = snapshotId ;
}

string VcsConfigSnapshotObject::getSnapshotId  () const
{
    return ( m_snapshotId );
}

void VcsConfigSnapshotObject::setRbridgeId (const UI32& rbridgeId )
{
    m_rbridgeId = rbridgeId;
}

UI32 VcsConfigSnapshotObject::getRbridgeId () const
{
    return ( m_rbridgeId );
}

void VcsConfigSnapshotObject::setSWBDModel ( const SI32 &SWBDModel)
{
    m_SWBDModel = SWBDModel;
}

SI32 VcsConfigSnapshotObject::getSWBDModel () const
{
    return ( m_SWBDModel) ;
}

void VcsConfigSnapshotObject::setSnapshotCreatedTime ( const DateTime &snapshotCreatedTime )
{   
    m_snapshotCreatedTime = snapshotCreatedTime;
}

DateTime VcsConfigSnapshotObject::getSnapshotCreatedTime () const
{
    return ( m_snapshotCreatedTime );
}

void VcsConfigSnapshotObject::setRestoreOnDisconnectedNode ( const bool &restoreOnDisconnectedNode )
{
    m_restoreOnDisconnectedNode = restoreOnDisconnectedNode;
}

bool VcsConfigSnapshotObject::getRestoreOnDisconnectedNode () const
{
    return ( m_restoreOnDisconnectedNode );
}

void VcsConfigSnapshotObject::setConfigData (const string &configData)
{
    m_configData = configData;
}

string VcsConfigSnapshotObject::getConfigData () const
{
    return (m_configData);
}

void VcsConfigSnapshotObject::setMetadataOnly ( const bool &metadataOnlyObject)
{
    m_metadataOnlyObject = metadataOnlyObject;
}

bool VcsConfigSnapshotObject::getMetadataOnly () const
{
    return (m_metadataOnlyObject);
}

ResourceId VcsConfigSnapshotObject::saveConfiguration (const string &configurationFileName)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (true == m_metadataOnlyObject )
    {
        //This check can be removed if not necessary
        //Currently we dont want to write only metdata into the file since the config will be lost
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::saveConfiguration: You cannot save config when the object has only meta-data. This will lose the actual snapshot config");
        WaveNs::prismAssert( false, __FILE__, __LINE__ );
        return (WAVE_MESSAGE_ERROR);
    }

    ofstream configurationFileToWrite (configurationFileName.c_str ());

    if (true == (configurationFileToWrite.is_open ()))
    {
        string serializedData;

        //serialize only meta-data
        serialize2 (serializedData);
        
        //Add delimiters to comment out meta-data
        serializedData = string("!") + serializedData + "\n"; ;
    
        //Append snapshot configuration 
        serializedData += m_configData;

        configurationFileToWrite << serializedData;         
        configurationFileToWrite.close ();

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::saveConfiguration: Cannot open file : " + configurationFileName);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsConfigSnapshotObject::loadConfiguration (const string &configurationFileName )
{   
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    ifstream configurationFileToRead (configurationFileName.c_str ());

    if (true == (configurationFileToRead.is_open ()))
    {
        string serializedData;
        UI32 configDataSize = 0;
        UI32 savedPos = 0;

        getline (configurationFileToRead, serializedData);
        savedPos = configurationFileToRead.tellg();
        
        prepareForSerialization ();
        loadFromSerializedData2 (serializedData);
        serializedData.clear();
        
        if ( false == m_metadataOnlyObject )
        {
            configurationFileToRead.seekg (0, configurationFileToRead.end);
            UI32 currentPos = configurationFileToRead.tellg();
            configDataSize = currentPos - savedPos;
    
            WaveNs::tracePrintf (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::loadConfiguration: file size is :%d " + currentPos);
            configurationFileToRead.seekg (savedPos, configurationFileToRead.beg);
    
            char *pConfigDataBuffer = new char [configDataSize + 1]; // +1 for appending '\0' in the end 
            prismAssert ( NULL != pConfigDataBuffer, __FILE__, __LINE__);
            configurationFileToRead.read ( pConfigDataBuffer, configDataSize);
            configurationFileToRead.close ();
            pConfigDataBuffer[configDataSize] = '\0'; // read does not terminate the lines simply reads data blocks
            m_configData = pConfigDataBuffer;
    
            delete [] pConfigDataBuffer;
        }    
        status = validateConfiguration ();
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::loadConfiguration: Cannot open file : " + configurationFileName);    
    }

    return (status);    
}

ResourceId VcsConfigSnapshotObject::validateConfiguration ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if ( 0 ==  m_rbridgeId )
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::validateConfiguration: rbridgeId is invalid ");
        status = WAVE_MESSAGE_ERROR;
    }
    else if ( true == m_snapshotId.empty() )
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::validateConfiguration: snapshotId is empty ");
        status = WAVE_MESSAGE_ERROR;
    }
    else if ( -1 == m_SWBDModel )
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsConfigSnapshotObject::validateConfiguration: SWBDModel in invalid ");
        status = WAVE_MESSAGE_ERROR;
    }
    return (status);
}

}

