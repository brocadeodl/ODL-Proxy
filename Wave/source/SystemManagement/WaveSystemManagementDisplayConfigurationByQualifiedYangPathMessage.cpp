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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveSystemManagementTypes.h"

namespace WaveNs
{

WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage ()
    : ManagementInterfaceMessage (WaveSystemManagementObjectManager::getClassName (), WAVE_SYSTEM_MANAGEMENT_DISPLAY_CURRENT_CONFIGURATION_BY_QUALIFIED_YANG_PATH),
      m_schemaName               ("wavecurrent")
{
}

WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage (const string &qualifiedYangPath)
    : ManagementInterfaceMessage (WaveSystemManagementObjectManager::getClassName (), WAVE_SYSTEM_MANAGEMENT_DISPLAY_CURRENT_CONFIGURATION_BY_QUALIFIED_YANG_PATH),
      m_qualifiedYangPath        (qualifiedYangPath),
      m_schemaName               ("wavecurrent")
{
}

WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage (const string &qualifiedYangPath, const string &schemaName)
    : ManagementInterfaceMessage (WaveSystemManagementObjectManager::getClassName (), WAVE_SYSTEM_MANAGEMENT_DISPLAY_CURRENT_CONFIGURATION_BY_QUALIFIED_YANG_PATH),
      m_qualifiedYangPath        (qualifiedYangPath),
      m_schemaName               (schemaName)
{
}

WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::~WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage ()
{
}

void WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_qualifiedYangPath,    "qualifiedYangPath"));
    addSerializableAttribute (new AttributeString (&m_schemaName,           "schemaName"));
}

string WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::getQualifiedYangPath () const
{
    return (m_qualifiedYangPath);
}

void WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::setQualifiedYangPath (const string &qualifiedYangPath)
{
    m_qualifiedYangPath = qualifiedYangPath;
}

string WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::getSchemaName () const
{
    return (m_schemaName);
}

void WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage::setSchemaName (const string &schemaName)
{
    m_schemaName = schemaName;
}

}
