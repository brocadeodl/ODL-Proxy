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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/Core/WaveClientConfiguration.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

WaveClientConfiguration::WaveClientConfiguration ()
    : m_applicationName   ("Wave"),
      m_isADaemon         (false),
      m_isSysLogRequired  (false),
      m_isDatabaseEnabled (false),
      m_databasePort      (9013),
      m_needShell         (true)
{
}

WaveClientConfiguration::~WaveClientConfiguration ()
{
}

string WaveClientConfiguration::getApplicationName () const
{
    return (m_applicationName);
}

void WaveClientConfiguration::setApplicationName (const string &applicationName)
{
    m_applicationName = applicationName;
}

bool WaveClientConfiguration::getIsADaemon () const
{
    return (m_isADaemon);
}

void WaveClientConfiguration::setIsADaemon (const bool &isADaemon)
{
    m_isADaemon = isADaemon;
}

bool WaveClientConfiguration::getIsSysLogRequired () const
{
    return (m_isSysLogRequired);
}

void WaveClientConfiguration::setIsSysLogRequired (const bool &isSysLogRequired)
{
    m_isSysLogRequired = isSysLogRequired;
}

bool WaveClientConfiguration::getIsDatabaseEnabled () const
{
    return (m_isDatabaseEnabled);
}

void WaveClientConfiguration::setIsDatabaseEnabled (const bool &isDatabaseEnabled)
{
    m_isDatabaseEnabled = isDatabaseEnabled;
}

SI32 WaveClientConfiguration::getDatabasePort () const
{
    return (m_databasePort);
}

void WaveClientConfiguration::setDatabasePort (const SI32 &databasePort)
{
    m_databasePort = databasePort;
}

bool WaveClientConfiguration::getNeedShell () const
{
    return (m_needShell);
}

void WaveClientConfiguration::setNeedShell (const bool &needShell)
{
    m_needShell = needShell;
}

string WaveClientConfiguration::getApplicationCompactName () const
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

}
