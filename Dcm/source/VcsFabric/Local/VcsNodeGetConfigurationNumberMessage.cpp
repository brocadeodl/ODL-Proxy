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

#include "VcsFabric/Local/VcsNodeGetConfigurationNumberMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeGetConfigurationNumberMessage::VcsNodeGetConfigurationNumberMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_CONFIGURATION_NUMBER)
{
}

VcsNodeGetConfigurationNumberMessage::~VcsNodeGetConfigurationNumberMessage ()
{
}

void VcsNodeGetConfigurationNumberMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI64Vector (&m_configurationNumber, "configurationNumber"));
}

void VcsNodeGetConfigurationNumberMessage::getConfigurationNumber (UI16 configurationNumber[16])
{
    UI32 i ;
    UI32 sizeOfConfigurationNumber = m_configurationNumber.size ();

    for (i = 0; i < sizeOfConfigurationNumber; i++)
    {
        UI64 number = m_configurationNumber [i];
        
        configurationNumber[(i * 4)] = number;

        configurationNumber[(i * 4) + 1] = (number >> 16);
    
        configurationNumber[(i * 4) + 2] = (number >> 32);

        configurationNumber[(i * 4) + 3] = (number >> 48);
    }
}

void VcsNodeGetConfigurationNumberMessage::setconfigurationNumber (const vector<UI64> &configurationNumber)
{
    m_configurationNumber = configurationNumber;
}

}
