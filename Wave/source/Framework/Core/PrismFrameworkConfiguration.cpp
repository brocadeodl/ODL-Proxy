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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include <fstream>

using namespace std;

namespace WaveNs
{

PrismFrameworkConfiguration::PrismFrameworkConfiguration ()
    : SerializableObject (),
      m_thisLocationPort (0),
      m_thisLocationRole (0),
      m_thisLocationLocationId (0),
      m_clusterPrimaryPort (0),
      m_clusterPrimaryLocationId (0),
      m_isStartupValid (false),
      m_startupFileType (0),
      m_lastUsedLocationId (0),
      m_dbRestoreIncomplete (false)
{
}

PrismFrameworkConfiguration::~PrismFrameworkConfiguration ()
{
}

string PrismFrameworkConfiguration::getThisLocationIpAddress () const
{
    return (m_thisLocationIpAddress);
}

SI32 PrismFrameworkConfiguration::getThisLocationPort () const
{
    return (m_thisLocationPort);
}

LocationRole PrismFrameworkConfiguration::getThisLocationRole () const
{
    return (static_cast<LocationRole> (m_thisLocationRole));
}

void PrismFrameworkConfiguration::setThisLocationRole (const LocationRole& role)
{
    m_thisLocationRole = role;
}

LocationId PrismFrameworkConfiguration::getThisLocationLocationId () const
{
    return (m_thisLocationLocationId);
}

void PrismFrameworkConfiguration::setThisLocationLocationId (const LocationId& locationid)
{
    m_thisLocationLocationId = locationid;
}

string PrismFrameworkConfiguration::getClusterPrimaryIpAddress () const
{
    return (m_clusterPrimaryIpAddress);
}

SI32 PrismFrameworkConfiguration::getClusterPrimaryPort () const
{
    return (m_clusterPrimaryPort);
}

void PrismFrameworkConfiguration::setClusterPrimaryPort (const SI32& port)
{
    m_clusterPrimaryPort = port;
}

LocationId PrismFrameworkConfiguration::getClusterPrimaryLocationId () const
{
    return (m_clusterPrimaryLocationId);
}

void PrismFrameworkConfiguration::setClusterPrimaryLocationId (const LocationId& locationid)
{
    m_clusterPrimaryLocationId = locationid;
}

vector<string> PrismFrameworkConfiguration::getKnownLocationIpAddresses () const
{
    return (m_knownLocationIpAddresses);
}


void PrismFrameworkConfiguration::setKnownLocationIpAddresses (const vector<string>& knownlocIpAddress)
{
    m_knownLocationIpAddresses.clear ();

    for (UI8 i = 0; i < knownlocIpAddress.size (); i++)
    {
        m_knownLocationIpAddresses.push_back (knownlocIpAddress [i]);
    }
}

vector<SI32> PrismFrameworkConfiguration::getKnownLocationPorts () const
{
    return (m_knownLocationPorts);
}

void PrismFrameworkConfiguration::setKnownLocationPorts (const vector<SI32>& ports)
{
    m_knownLocationPorts.clear ();

    for (UI8 i = 0; i < ports.size(); i++)
    {
        m_knownLocationPorts.push_back (ports[i]);
    }
}

vector<LocationId> PrismFrameworkConfiguration::getKnownLocationLocationIds () const
{
    return (m_knownLocationLocationIds);
}

void PrismFrameworkConfiguration::setKnownLocationLocationIds (const vector<LocationId>& loc)
{
    m_knownLocationLocationIds.clear ();

    for (UI8 i = 0; i < loc.size(); i++)
    {
        m_knownLocationLocationIds.push_back (loc[i]);
    }
}

vector<string> PrismFrameworkConfiguration::getKnownSubLocationIpAddress () const
{
    return (m_knownSubLocationIpAddresses);
}

vector<SI32> PrismFrameworkConfiguration::getKnownSubLocationPorts () const
{
    return (m_knownSubLocationPorts);
}

vector<LocationId> PrismFrameworkConfiguration::getKnownSubLocationLocationIds () const
{
    return (m_knownSubLocationLocationIds);
}

bool PrismFrameworkConfiguration::getIsStartupValid () const
{
    return (m_isStartupValid);
}

void PrismFrameworkConfiguration::setIsStartupValid (const bool isValid)
{
    m_isStartupValid = isValid;
}

string PrismFrameworkConfiguration::getStartupFileName () const
{
    return (m_startupFileName);
}

void PrismFrameworkConfiguration::setStartupFileName (const string &fileName)
{
    m_startupFileName = fileName;
}

ResourceId PrismFrameworkConfiguration::getStartupFileType () const
{
    return (m_startupFileType);
}

void PrismFrameworkConfiguration::setStartupFileType (const ResourceId &fileType)
{
    m_startupFileType = fileType;
}

LocationId PrismFrameworkConfiguration::getLastUsedLocationId () const
{
    return (m_lastUsedLocationId);
}

void PrismFrameworkConfiguration::setLastUsedLocationId (const LocationId& locationid)
{
    m_lastUsedLocationId = locationid;
}

void PrismFrameworkConfiguration::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString           (&m_thisLocationIpAddress,                               "thisLocationIpAddress"));
    addSerializableAttribute (new AttributeSI32             (&m_thisLocationPort,                                    "thisLocationPort"));
    addSerializableAttribute (new AttributeUI32             (&m_thisLocationRole,                                    "thisLocationRole"));
    addSerializableAttribute (new AttributeLocationId       (&m_thisLocationLocationId,                              "thisLocationLocationId"));

    addSerializableAttribute (new AttributeString           (&m_clusterPrimaryIpAddress,                             "clusterPrimaryIpAddress"));
    addSerializableAttribute (new AttributeSI32             (&m_clusterPrimaryPort,                                  "clusterPrimaryPort"));
    addSerializableAttribute (new AttributeLocationId       (&m_clusterPrimaryLocationId,                            "clusterPrimaryLocationId"));

    addSerializableAttribute (new AttributeStringVector     (&m_knownLocationIpAddresses,                            "knownLocationIpAddresses"));
    addSerializableAttribute (new AttributeSI32Vector       (&m_knownLocationPorts,                                  "knownLocationPorts"));
    addSerializableAttribute (new AttributeLocationIdVector (&m_knownLocationLocationIds,                            "knownLocationLocationIds"));

    addSerializableAttribute (new AttributeStringVector     (&m_knownSubLocationIpAddresses,                         "knownSubLocationIpAddresses"));
    addSerializableAttribute (new AttributeSI32Vector       (&m_knownSubLocationPorts,                               "knownSubLocationPorts"));
    addSerializableAttribute (new AttributeLocationIdVector (&m_knownSubLocationLocationIds,                         "knownSubLocationLocationIds"));
    addSerializableAttribute (new AttributeBool             (&m_isStartupValid,                                      "isStartupValid"));
    addSerializableAttribute (new AttributeString           (&m_startupFileName,                                     "startupFileName"));
    addSerializableAttribute (new AttributeEnum             (&m_startupFileType,                                     "startupFileType"));
    addSerializableAttribute (new AttributeLocationId       (&m_lastUsedLocationId,                                  "lastUsedLocationId"));

    //Attributes for MOs related to schema change
    addSerializableAttribute (new AttributeStringVector     (&m_managedObjectNamesForSchemaChange,                   "managedObjectNamesForSchemaChange"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesStringsForSchemaChange,                    "fieldNamesStringsForSchemaChange"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesTypesForSchemaChange,                      "fieldNamesTypesForSchemaChange"));
    addSerializableAttribute (new AttributeUI32Vector       (&m_classIds,                                            "classIds"));
    addSerializableAttribute (new AttributeUI32Vector       (&m_parentTableIds,                                      "parentTableIds"));
    addSerializableAttribute (new AttributeBool             (&m_dbRestoreIncomplete,                                 "dbRestoreIncomplete"));
    addSerializableAttribute (new AttributeStringVector     (&m_userDefinedKeyCombinationsFieldNamesForSchemaChange, "userDefinedKeyCombinationsFieldNamesForSchemaChange"));
    addSerializableAttribute (new AttributeStringVector     (&m_userDefinedKeyCombinationsFieldTypesForSchemaChange, "userDefinedKeyCombinationsFieldTypesForSchemaChange"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesExpandedTypesForSchemaChange,               "fieldNamesExpandedTypesForSchemaChange"));
}

void PrismFrameworkConfiguration::setupAttributesForSerializationInAttributeOrderFormat ()
{
    SerializableObject::setupAttributesForSerializationInAttributeOrderFormat ();

    addAttributeNameForOrderToNameMapping ("thisLocationIpAddress");
    addAttributeNameForOrderToNameMapping ("thisLocationPort");
    addAttributeNameForOrderToNameMapping ("thisLocationRole");
    addAttributeNameForOrderToNameMapping ("thisLocationLocationId");

    addAttributeNameForOrderToNameMapping ("clusterPrimaryIpAddress");
    addAttributeNameForOrderToNameMapping ("clusterPrimaryPort");
    addAttributeNameForOrderToNameMapping ("clusterPrimaryLocationId");

    addAttributeNameForOrderToNameMapping ("knownLocationIpAddresses");
    addAttributeNameForOrderToNameMapping ("knownLocationPorts");
    addAttributeNameForOrderToNameMapping ("knownLocationLocationIds");

    addAttributeNameForOrderToNameMapping ("knownSubLocationIpAddresses");
    addAttributeNameForOrderToNameMapping ("knownSubLocationPorts");
    addAttributeNameForOrderToNameMapping ("knownSubLocationLocationIds");
    addAttributeNameForOrderToNameMapping ("isStartupValid");
    addAttributeNameForOrderToNameMapping ("startupFileName");
    addAttributeNameForOrderToNameMapping ("startupFileType");
    addAttributeNameForOrderToNameMapping ("lastUsedLocationId");

    addAttributeNameForOrderToNameMapping ("managedObjectNamesForSchemaChange");
    addAttributeNameForOrderToNameMapping ("fieldNamesStringsForSchemaChange");
    addAttributeNameForOrderToNameMapping ("fieldNamesTypesForSchemaChange");
    addAttributeNameForOrderToNameMapping ("classIds");
    addAttributeNameForOrderToNameMapping ("parentTableIds");
    addAttributeNameForOrderToNameMapping ("dbRestoreIncomplete");
    addAttributeNameForOrderToNameMapping ("userDefinedKeyCombinationsFieldNamesForSchemaChange");
    addAttributeNameForOrderToNameMapping ("userDefinedKeyCombinationsFieldTypesForSchemaChange");
    addAttributeNameForOrderToNameMapping ("fieldNamesExpandedTypesForSchemaChange");
}

bool PrismFrameworkConfiguration::getDBRestoreIncomplete () const
{
    return (m_dbRestoreIncomplete);
}       

void PrismFrameworkConfiguration::setDBRestoreIncomplete (const bool dbRestoreIncompleteFlag)
{   
    m_dbRestoreIncomplete = dbRestoreIncompleteFlag;
}

ResourceId PrismFrameworkConfiguration::initializeConfiguration ()
{
    UI32 i                    = 0;
    UI32 numberOfLocations    = 0;
    UI32 numberOfSubLocations = 0;

    m_thisLocationIpAddress  = FrameworkToolKit::getThisLocationIpAddress ();
    m_thisLocationPort       = FrameworkToolKit::getThisLocationPort ();
    m_thisLocationRole       = FrameworkToolKit::getThisLocationRole ();
    m_thisLocationLocationId = FrameworkToolKit::getThisLocationId ();

    m_clusterPrimaryIpAddress  = FrameworkToolKit::getClusterPrimaryIpAddress ();
    m_clusterPrimaryPort       = FrameworkToolKit::getClusterPrimaryPort ();
    m_clusterPrimaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
    m_lastUsedLocationId       = FrameworkToolKit::getLastUsedLocationId ();

    FrameworkToolKit::getKnownLocations (m_knownLocationLocationIds);

    numberOfLocations = m_knownLocationLocationIds.size ();

    for (i = 0; i < numberOfLocations; i++)
    {
        m_knownLocationIpAddresses.push_back (FrameworkToolKit::getIpAddressForLocationId (m_knownLocationLocationIds[i]));
        m_knownLocationPorts.push_back (FrameworkToolKit::getPortForLocationId (m_knownLocationLocationIds[i]));
    }

    FrameworkToolKit::getKnownSubLocations (m_knownSubLocationLocationIds);

    numberOfSubLocations = m_knownSubLocationLocationIds.size ();

    for (i = 0; i < numberOfSubLocations; i++)
    {
        m_knownSubLocationIpAddresses.push_back (FrameworkToolKit::getIpAddressForLocationId (m_knownSubLocationLocationIds[i]));
        m_knownSubLocationPorts.push_back (FrameworkToolKit::getPortForLocationId (m_knownSubLocationLocationIds[i]));
    }

    m_isStartupValid = FrameworkToolKit::getIsStartupValid ();

    m_startupFileType = FrameworkToolKit::getStartupFileType ();
    m_startupFileName = FrameworkToolKit::getStartupFileName ();

    m_dbRestoreIncomplete = FrameworkToolKit::getIsDbRestoreIncomplete ();
    
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkConfiguration::saveConfiguration (const string &configurationFileName)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    ofstream configurationFileToWrite (configurationFileName.c_str ());

    if (true == (configurationFileToWrite.is_open ()))
    {
        string serializedData;

        serialize2 (serializedData);
        configurationFileToWrite << serializedData;
        configurationFileToWrite.close ();

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkConfiguration::saveConfiguration: Cannot open file : " + configurationFileName);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId PrismFrameworkConfiguration::loadConfiguration (const string &configurationFileName)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    ifstream configurationFileToRead (configurationFileName.c_str ());

    if (true == (configurationFileToRead.is_open ()))
    {
        string serializedData;

        getline (configurationFileToRead, serializedData);
        configurationFileToRead.close ();

        prepareForSerialization ();
        loadFromSerializedData2 (serializedData);

        status = validateConfiguration ();
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

void PrismFrameworkConfiguration::display () const
{
    UI32 numberOfKnownLocations    = m_knownLocationIpAddresses.size ();
    UI32 numberOfKnownSubLocations = m_knownSubLocationIpAddresses.size ();
    UI32 i                         = 0;

    trace (TRACE_LEVEL_INFO, string ("___________________________________________________"));

    trace (TRACE_LEVEL_INFO, string ("Ip Address                  : ") + m_thisLocationIpAddress);
    trace (TRACE_LEVEL_INFO, string ("Port                        : ") + m_thisLocationPort);
    trace (TRACE_LEVEL_INFO, string ("Role                        : ") + FrameworkToolKit::localize (m_thisLocationRole));
    trace (TRACE_LEVEL_INFO, string ("Location Id                 : ") + m_thisLocationLocationId);
    trace (TRACE_LEVEL_INFO, string ("Cluster Primary Ip Address  : ") + m_clusterPrimaryIpAddress);
    trace (TRACE_LEVEL_INFO, string ("Cluster Primary Port        : ") + m_clusterPrimaryPort);
    trace (TRACE_LEVEL_INFO, string ("Cluster Primary Location Id : ") + m_clusterPrimaryLocationId);
    trace (TRACE_LEVEL_INFO, string ("Last Used Location Id       : ") + m_lastUsedLocationId);

    for (i = 0; i < numberOfKnownLocations; i++)
    {
        trace (TRACE_LEVEL_INFO, string ("    Location ") + (i + 1));
        trace (TRACE_LEVEL_INFO, string ("        Ip Address  : ") + m_knownLocationIpAddresses[i]);
        trace (TRACE_LEVEL_INFO, string ("        Port        : ") + m_knownLocationPorts[i]);
        trace (TRACE_LEVEL_INFO, string ("        Location Id : ") + m_knownLocationLocationIds[i]);
    }

    for (i = 0; i < numberOfKnownSubLocations; i++)
    {
        trace (TRACE_LEVEL_INFO, string ("    Sub Location ") + (i + 1));
        trace (TRACE_LEVEL_INFO, string ("        Ip Address  : ") + m_knownSubLocationIpAddresses[i]);
        trace (TRACE_LEVEL_INFO, string ("        Port        : ") + m_knownSubLocationPorts[i]);
        trace (TRACE_LEVEL_INFO, string ("        Location Id : ") + m_knownSubLocationLocationIds[i]);
    }

    trace (TRACE_LEVEL_INFO, string ("isStartupValid              : ") + m_isStartupValid);

    trace (TRACE_LEVEL_INFO, string ("Startup File Type          : ") + m_startupFileType);
    trace (TRACE_LEVEL_INFO, string ("Startup File Name          : ") + m_startupFileName);

    trace (TRACE_LEVEL_INFO, string ("dbRestoreIncompleteFlag        : ") + m_dbRestoreIncomplete);
    trace (TRACE_LEVEL_INFO, string ("___________________________________________________"));
}

void PrismFrameworkConfiguration::displayToTerminal () const
{
    UI32 numberOfKnownLocations    = m_knownLocationIpAddresses.size ();
    UI32 numberOfKnownSubLocations = m_knownSubLocationIpAddresses.size ();
    UI32 i                         = 0;

    cout << string ("___________________________________________________") << endl;

    cout << string ("Ip Address                  : ") + m_thisLocationIpAddress << endl;
    cout << string ("Port                        : ") + m_thisLocationPort << endl;
    cout << string ("Role                        : ") + FrameworkToolKit::localize (m_thisLocationRole) << endl;
    cout << string ("Location Id                 : ") + m_thisLocationLocationId << endl; 
    cout << string ("Cluster Primary Ip Address  : ") + m_clusterPrimaryIpAddress << endl;
    cout << string ("Cluster Primary Port        : ") + m_clusterPrimaryPort << endl;
    cout << string ("Cluster Primary Location Id : ") + m_clusterPrimaryLocationId << endl;
    cout << string ("Last Used Location Id       : ") + m_lastUsedLocationId << endl;

    for (i = 0; i < numberOfKnownLocations; i++)
    {
        cout << string ("    Location ") + (i + 1) << endl;
        cout << string ("        Ip Address  : ") + m_knownLocationIpAddresses[i] << endl;
        cout << string ("        Port        : ") + m_knownLocationPorts[i] << endl;
        cout << string ("        Location Id : ") + m_knownLocationLocationIds[i] << endl;
    }

    for (i = 0; i < numberOfKnownSubLocations; i++)
    {
        cout << string ("    Sub Location ") + (i + 1) << endl;
        cout << string ("        Ip Address  : ") + m_knownSubLocationIpAddresses[i] << endl;
        cout << string ("        Port        : ") + m_knownSubLocationPorts[i] << endl;
        cout << string ("        Location Id : ") + m_knownSubLocationLocationIds[i] << endl;
    }

    cout << string ("isStartupValid              : ") + m_isStartupValid << endl;

    cout << string ("Startup File Type           : ") + m_startupFileType << endl;
    cout << string ("Startup File Name           : ") + m_startupFileName << endl;

    cout << string ("dbRestoreIncompleteFlag     : ") + m_dbRestoreIncomplete << endl;
    cout << string ("___________________________________________________") << endl;
}

size_t PrismFrameworkConfiguration::getSchemaChangeInfo (vector<string> & managedObjectNames, vector<string> & managedObjectFieldNamesStrings,vector<string> & managedObjectFieldTypesStrings,vector<UI32> & classIds, vector<UI32> & parentClassIds, vector<string> & userDefinedKeyCombinationsFieldNamesForSchemaChange, vector<string> & userDefinedKeyCombinationsFieldTypesForSchemaChange, vector<string> &fieldNamesExpandedTypesForSchemaChange) const
{
    managedObjectNames              = m_managedObjectNamesForSchemaChange;
    managedObjectFieldNamesStrings  = m_fieldNamesStringsForSchemaChange;
    managedObjectFieldTypesStrings  = m_fieldNamesTypesForSchemaChange;
    classIds                        = m_classIds;
    parentClassIds                  = m_parentTableIds;
    userDefinedKeyCombinationsFieldNamesForSchemaChange = m_userDefinedKeyCombinationsFieldNamesForSchemaChange;
    userDefinedKeyCombinationsFieldTypesForSchemaChange = m_userDefinedKeyCombinationsFieldTypesForSchemaChange;
    fieldNamesExpandedTypesForSchemaChange = m_fieldNamesExpandedTypesForSchemaChange;

    return (m_parentTableIds.size ());
}

/*
 * Save the prism Configuration after updating the vectors
 */
bool PrismFrameworkConfiguration::setSchemaChangeInfo (vector<string> & managedObjectNames, vector<string> & managedObjectFieldNamesStrings,vector<string> & managedObjectFieldTypesStrings,vector<UI32> & classIds, vector<UI32> & parentClassIds, vector<string> & userDefinedKeyCombinationsFieldNamesForSchemaChange, vector<string> & userDefinedKeyCombinationsFieldTypesForSchemaChange, vector<string> &fieldNamesExpandedTypesForSchemaChange)
{
    m_managedObjectNamesForSchemaChange =  managedObjectNames;
    m_fieldNamesStringsForSchemaChange = managedObjectFieldNamesStrings;
    m_fieldNamesTypesForSchemaChange = managedObjectFieldTypesStrings;
    m_classIds = classIds;
    m_parentTableIds = parentClassIds;
    m_userDefinedKeyCombinationsFieldNamesForSchemaChange = userDefinedKeyCombinationsFieldNamesForSchemaChange;
    m_userDefinedKeyCombinationsFieldTypesForSchemaChange = userDefinedKeyCombinationsFieldTypesForSchemaChange;
    m_fieldNamesExpandedTypesForSchemaChange = fieldNamesExpandedTypesForSchemaChange;
    
    return (true);
}

ResourceId PrismFrameworkConfiguration::validateConfiguration ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (strcmp (m_thisLocationIpAddress.c_str (), "0.0.0.0") == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (m_thisLocationPort == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (m_thisLocationRole == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (m_thisLocationLocationId == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (strcmp (m_clusterPrimaryIpAddress.c_str (), "0.0.0.0") == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (m_clusterPrimaryPort == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (m_clusterPrimaryLocationId == 0)
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

}
