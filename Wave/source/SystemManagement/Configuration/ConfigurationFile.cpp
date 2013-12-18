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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/Configuration/ConfigurationFile.h"

namespace WaveNs
{

ConfigurationFile::ConfigurationFile ()
    : m_isAlreadyLoaded (false)
{
}

ConfigurationFile::ConfigurationFile (const string &configurationFilePath)
    : m_configurationFilePath (configurationFilePath),
      m_isAlreadyLoaded (false)

{
}

ConfigurationFile::~ConfigurationFile ()
{
}

string ConfigurationFile::getConfigurationFilePath () const
{
    return (m_configurationFilePath);
}

void ConfigurationFile::setConfigurationFilePath (const string &configurationFilePath)
{
    m_configurationFilePath = configurationFilePath;
}

bool ConfigurationFile::getIsAlreadyLoaded () const
{
    return (m_isAlreadyLoaded);
}

void ConfigurationFile::setIsAlreadyLoaded (const bool &isAlreadyLoaded)
{
    m_isAlreadyLoaded = isAlreadyLoaded;
}

}
