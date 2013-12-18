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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeGetConfigurationTimeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Attributes/AttributeDateTime.h"

namespace DcmNs
{

VcsNodeGetConfigurationTimeMessage::VcsNodeGetConfigurationTimeMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_CONFIGURATION_TIME)
{
}

VcsNodeGetConfigurationTimeMessage::~VcsNodeGetConfigurationTimeMessage ()
{
}

void VcsNodeGetConfigurationTimeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeDateTime (&m_configurationTime, "configurationTime"));
}

string VcsNodeGetConfigurationTimeMessage::getConfigurationTime () const
{
    return (m_configurationTime.toString ());
}

void VcsNodeGetConfigurationTimeMessage::setConfigurationTime (const DateTime &configurationTime)
{
    m_configurationTime = configurationTime;
}

DateTime VcsNodeGetConfigurationTimeMessage::getConfigurationDateTimeData () const
{
    return m_configurationTime;
}

}
