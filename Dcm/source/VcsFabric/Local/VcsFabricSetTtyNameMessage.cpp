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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricSetTtyNameMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsFabricSetTtyNameMessage::VcsFabricSetTtyNameMessage ()
    :   ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_FABRIC_SET_TTYNAME),
        m_newTtyName ("")
{
}

VcsFabricSetTtyNameMessage::~VcsFabricSetTtyNameMessage ()
{
}

void VcsFabricSetTtyNameMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization (); 

    addSerializableAttribute (new AttributeString (&m_newTtyName, "newTtyName"));
}
        
void VcsFabricSetTtyNameMessage::setNewTtyName (const string &ttyName)
{
    m_newTtyName    = ttyName; 
}
string VcsFabricSetTtyNameMessage::getNewTtyName () const
{
    return (m_newTtyName);
}

}
