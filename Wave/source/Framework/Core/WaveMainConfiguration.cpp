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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/WaveMainConfiguration.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

WaveMainConfiguration::WaveMainConfiguration ()
    : m_applicationName                                 ("Wave"),
      m_isADaemon                                       (false),
      m_isSysLogRequired                                (false),
      m_isDatabaseEnabled                               (false),
      m_port                                            (3016),
      m_databasePort                                    (9013),
      m_managementPort                                  (9110),
      m_ethernetInterface                               (""),
      m_managementClientPort                            (0),
      m_databaseEmptyType                               ("auto"),
      m_databaseEmptyTypeAutoDetectionThresholdValue    (75000)
{
}

WaveMainConfiguration::~WaveMainConfiguration ()
{
}

string WaveMainConfiguration::getApplicationName () const
{
    return (m_applicationName);
}

void WaveMainConfiguration::setApplicationName (const string &applicationName)
{
    m_applicationName = applicationName;
}

bool WaveMainConfiguration::getIsADaemon () const
{
    return (m_isADaemon);
}

void WaveMainConfiguration::setIsADaemon (const bool &isADaemon)
{
    m_isADaemon = isADaemon;
}

bool WaveMainConfiguration::getIsSysLogRequired () const
{
    return (m_isSysLogRequired);
}

void WaveMainConfiguration::setIsSysLogRequired (const bool &isSysLogRequired)
{
    m_isSysLogRequired = isSysLogRequired;
}

bool WaveMainConfiguration::getIsDatabaseEnabled () const
{
    return (m_isDatabaseEnabled);
}

void WaveMainConfiguration::setIsDatabaseEnabled (const bool &isDatabaseEnabled)
{
    m_isDatabaseEnabled = isDatabaseEnabled;
}

bool WaveMainConfiguration::getIsDatabaseLogEnabled () const
{
    return (m_isDatabaseLogEnabled);
}

void WaveMainConfiguration::setIsDatabaseLogEnabled (const bool isDatabaseLogEnabled)
{
    m_isDatabaseLogEnabled = isDatabaseLogEnabled;
}

SI32 WaveMainConfiguration::getPort () const
{
    return (m_port);
}

void WaveMainConfiguration::setPort (const SI32 &port)
{
    m_port = port;
}

SI32 WaveMainConfiguration::getDatabasePort () const
{
    return (m_databasePort);
}

void WaveMainConfiguration::setDatabasePort (const SI32 &databasePort)
{
    m_databasePort = databasePort;
}

string WaveMainConfiguration::getDatabaseLogFileName () const
{
    return (m_databaseLogFileName);
}

void WaveMainConfiguration::setDatabaseLogFileName (const string &databaseLogFileName)
{
    m_databaseLogFileName = databaseLogFileName;
}

string WaveMainConfiguration::getDatabaseSharedBuffers () const
{
    return (m_databaseSharedBuffers);
}

void WaveMainConfiguration::setDatabaseSharedBuffers (const string &databaseSharedBuffers)
{
    m_databaseSharedBuffers = databaseSharedBuffers;
}

SI32 WaveMainConfiguration::getManagementPort () const
{
    return (m_managementPort);
}

void WaveMainConfiguration::setManagementPort (const SI32 &managementPort)
{
    m_managementPort = managementPort;
}

string WaveMainConfiguration::getEthernetInterface () const
{
    return (m_ethernetInterface);
}

void WaveMainConfiguration::setEthernetInterface (const string &ethernetInterface)
{
    m_ethernetInterface = ethernetInterface;
}

SI32 WaveMainConfiguration::getManagementClientPort () const
{
    return (m_managementClientPort);
}

void WaveMainConfiguration::setManagementClientPort (const SI32 &managementClientPort)
{
    m_managementClientPort = managementClientPort;
}

string WaveMainConfiguration::getChangeDirectoryTo () const
{
    return (m_changeDirectoryTo);
}

void WaveMainConfiguration::setChangeDirectoryTo (const string &changeDirectoryTo)
{
    m_changeDirectoryTo = changeDirectoryTo;
}

string WaveMainConfiguration::getApplicationCompactName () const
{
    vector<string> tokensInApplicationName;

    tokenize (m_applicationName, tokensInApplicationName, '/');

    UI32 numberOfTokens = tokensInApplicationName.size ();

    if (0 < numberOfTokens)
    {
        return (tokensInApplicationName[numberOfTokens - 1]);
    }
    else
    {
        return (string (""));
    }
}

vector<string> WaveMainConfiguration::getYinPaths () const
{
    return (m_yinPaths);
}

void WaveMainConfiguration::setYinPaths (const vector<string> &yinPaths)
{
    m_yinPaths = yinPaths;
}

string WaveMainConfiguration::getWyserTagsFilePath () const
{
    return (m_wyserTagsFilePath);
}

void WaveMainConfiguration::setWyserTagsFilePath (const string &wyserTagsFilePath)
{
    m_wyserTagsFilePath = wyserTagsFilePath;
}

string WaveMainConfiguration::getDatabaseEmptyType () const
{
    return (m_databaseEmptyType);
}

void WaveMainConfiguration::setDatabaseEmptyType (const string &emptyDatabaseType)
{
    m_databaseEmptyType = emptyDatabaseType;
}

UI32 WaveMainConfiguration::getDatabaseEmptyTypeAutoDetectionThresholdValue () const
{
    return (m_databaseEmptyTypeAutoDetectionThresholdValue);
}


void WaveMainConfiguration::setDatabaseEmptyTypeAutoDetectionThresholdValue (const UI32 &thresholdValue)
{
    m_databaseEmptyTypeAutoDetectionThresholdValue = thresholdValue;
}

}
