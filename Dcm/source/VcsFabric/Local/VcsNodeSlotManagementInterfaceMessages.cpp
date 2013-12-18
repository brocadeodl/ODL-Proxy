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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh		                                       *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeSlotManagementInterfaceMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "DcmCore/DcmToolKit.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

namespace DcmNs
{

VcsNodeSlotRemoveMessage::VcsNodeSlotRemoveMessage ()
: ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_REMOVE_MESSAGE),
    m_slotNumber (0),
    m_rbridgeId (0),
    m_hwType (0), 
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
    
VcsNodeSlotRemoveMessage::VcsNodeSlotRemoveMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwType)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_REMOVE_MESSAGE),
        m_slotNumber (slotNumber),
        m_rbridgeId (rbridgeId),
        m_hwType (hwType),
        m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotRemoveMessage::VcsNodeSlotRemoveMessage (const UI32 &slotNumber, const UI32 & rbridgeId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_REMOVE_MESSAGE),
    m_slotNumber (slotNumber),
    m_rbridgeId (rbridgeId),
    m_hwType (SYS_SLOT_LC),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotRemoveMessage::VcsNodeSlotRemoveMessage (const string &objectManagerName, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwType)
    : ManagementInterfaceMessage (objectManagerName, operationCode),
     m_slotNumber (slotNumber),
     m_rbridgeId (rbridgeId),
     m_hwType (hwType),
     m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
                
void VcsNodeSlotRemoveMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_slotNumber,     "slotNumber"));
    addSerializableAttribute (new AttributeUI32 (&m_rbridgeId,      "rbridgeId"));
    addSerializableAttribute (new AttributeUI32 (&m_hwType,         "hwType"));
    addSerializableAttribute (new AttributeUI32 (&m_returnStatus,   "returnStatus"));
}

VcsNodeSlotRemoveMessage::~VcsNodeSlotRemoveMessage ()
{
}

UI32 VcsNodeSlotRemoveMessage::getSlotNumber ()
{
	return m_slotNumber;
}

void VcsNodeSlotRemoveMessage::setSlotNumber (const  UI32 &slotNumber)
{
	m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotRemoveMessage::getRbridgeId ()
{
	return m_rbridgeId;
}

void VcsNodeSlotRemoveMessage::setRbridgeId (const  UI32 &rbridgeId)
{
	m_rbridgeId = rbridgeId;
}

UI32 VcsNodeSlotRemoveMessage::getHwType ()
{
	return m_hwType;
}

void VcsNodeSlotRemoveMessage::setHwType (const  UI32 &hwType)
{
	m_hwType = hwType;
}

ResourceId VcsNodeSlotRemoveMessage::getReturnStatus ()
{
	return m_returnStatus;
}

void VcsNodeSlotRemoveMessage::setReturnStatus (const ResourceId &returnStatus)
{
	m_returnStatus = returnStatus;
}

//----------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeSlotTypeQueryMessage-----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

VcsNodeSlotTypeQueryMessage::VcsNodeSlotTypeQueryMessage ()
: ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_TYPE_QUERY),
    m_slotNumber (0),
    m_hwType (0),
    m_slotType (0),
    m_linecardMode (0)
{
}

VcsNodeSlotTypeQueryMessage::VcsNodeSlotTypeQueryMessage (const UI32 &slotNumber)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_TYPE_QUERY),
        m_slotNumber (slotNumber),
        m_hwType (SYS_SLOT_LC),
        m_slotType (UNKNOWN_BLADE_ID),
        m_linecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT)
{
}

VcsNodeSlotTypeQueryMessage::VcsNodeSlotTypeQueryMessage (const UI32 &slotNumber, const UI32 &hwType)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_TYPE_QUERY),
        m_slotNumber (slotNumber),
        m_hwType (hwType),
        m_slotType (UNKNOWN_BLADE_ID),
        m_linecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT)
{
}

void VcsNodeSlotTypeQueryMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_slotNumber,     "slotNumber"));
    addSerializableAttribute (new AttributeUI32 (&m_hwType,         "hwType"));
    addSerializableAttribute (new AttributeUI32 (&m_slotType,       "slotType"));
    addSerializableAttribute (new AttributeUI32 (&m_linecardMode,   "linecardMode"));
}

VcsNodeSlotTypeQueryMessage::~VcsNodeSlotTypeQueryMessage ()
{
}

UI32 VcsNodeSlotTypeQueryMessage::getSlotNumber ()
{
    return m_slotNumber;
}

void VcsNodeSlotTypeQueryMessage::setSlotNumber (const  UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotTypeQueryMessage::getSlotType ()
{
    return m_slotType;
}

void VcsNodeSlotTypeQueryMessage::setSlotType (const  UI32 &slotType)
{
    m_slotType = slotType;
}

UI32 VcsNodeSlotTypeQueryMessage::getHwType ()
{
    return m_hwType;
}

void VcsNodeSlotTypeQueryMessage::setHwType (const  UI32 &hwType)
{
    m_hwType = hwType;
}

UI32 VcsNodeSlotTypeQueryMessage::getLinecardMode ()
{
    return m_linecardMode;
}

void VcsNodeSlotTypeQueryMessage::setLinecardMode (const  UI32 &linecardMode)
{
    m_linecardMode = linecardMode;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeSlotConfigMessage-----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
                                
VcsNodeSlotConfigMessage::VcsNodeSlotConfigMessage ()
  : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_CONFIG_MESSAGE),
    m_slotNumber (0),
    m_rbridgeId (0),
    m_hwId (0),
    m_linecardMode (0),
    m_hwType (0),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
    
VcsNodeSlotConfigMessage::VcsNodeSlotConfigMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId)
  : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_CONFIG_MESSAGE),
    m_slotNumber (slotNumber),
    m_rbridgeId (rbridgeId),
    m_hwId (hwId),
    m_linecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT),
    m_hwType (SYS_SLOT_LC),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotConfigMessage::VcsNodeSlotConfigMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SLOT_CONFIG_MESSAGE),
      m_slotNumber (slotNumber),
      m_rbridgeId (rbridgeId),
      m_hwId (hwId),
      m_linecardMode (linecardMode),
      m_hwType (hwType),
      m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotConfigMessage::VcsNodeSlotConfigMessage (const string &objectManagerName, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType)
   : ManagementInterfaceMessage (objectManagerName, operationCode),
     m_slotNumber (slotNumber),
     m_rbridgeId (rbridgeId),
     m_hwId (hwId),
     m_linecardMode (linecardMode),
     m_hwType (hwType),
     m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
                
void VcsNodeSlotConfigMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_slotNumber,     "slotNumber"));
    addSerializableAttribute (new AttributeUI32 (&m_rbridgeId,      "rbridgeId"));
    addSerializableAttribute (new AttributeUI32 (&m_hwId,           "hwId"));
    addSerializableAttribute (new AttributeUI32 (&m_linecardMode,   "linecardMode"));
    addSerializableAttribute (new AttributeUI32 (&m_hwType,         "hwType"));
    addSerializableAttribute (new AttributeUI32 (&m_returnStatus,   "returnStatus"));
}

VcsNodeSlotConfigMessage::~VcsNodeSlotConfigMessage ()
{
}

UI32 VcsNodeSlotConfigMessage::getSlotNumber ()
{
	return m_slotNumber;
}

void VcsNodeSlotConfigMessage::setSlotNumber (const  UI32 &slotNumber)
{
	m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotConfigMessage::getRbridgeId ()
{
	return m_rbridgeId;
}

void VcsNodeSlotConfigMessage::setRbridgeId (const  UI32 &rbridgeId)
{
	m_rbridgeId = rbridgeId;
}

UI32 VcsNodeSlotConfigMessage::getHwId ()
{
	return m_hwId;
}

void VcsNodeSlotConfigMessage::setHwId (const  UI32 &hwId)
{
	m_hwId = hwId;
}

UI32 VcsNodeSlotConfigMessage::getLinecardMode ()
{
	return m_linecardMode;
}

void VcsNodeSlotConfigMessage::setLinecardMode (const  UI32 &linecardMode)
{
	m_linecardMode = linecardMode;
}

UI32 VcsNodeSlotConfigMessage::getHwType ()
{
	return m_hwType;
}

void VcsNodeSlotConfigMessage::setHwType (const  UI32 &hwType)
{
	m_hwType = hwType;
}

ResourceId VcsNodeSlotConfigMessage::getReturnStatus ()
{
	return m_returnStatus;
}

void VcsNodeSlotConfigMessage::setReturnStatus (const ResourceId &returnStatus)
{
	m_returnStatus = returnStatus;
}



}

