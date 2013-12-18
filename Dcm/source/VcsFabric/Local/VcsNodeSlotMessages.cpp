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

#include "VcsFabric/Local/VcsNodeSlotMessages.h"
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

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeClearLinecardMessage-----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsNodeClearLinecardMessage::VcsNodeClearLinecardMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CLEAR_LINECARD_MESSAGE),
    m_slotNumber (0),
    m_hwType (SYS_SLOT_LC),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
    
VcsNodeClearLinecardMessage::VcsNodeClearLinecardMessage (const UI32 &slotNumber)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CLEAR_LINECARD_MESSAGE),
    m_slotNumber (slotNumber),
    m_hwType (SYS_SLOT_LC),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
    
VcsNodeClearLinecardMessage::VcsNodeClearLinecardMessage (const UI32 &slotNumber, const UI32 &hwType)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CLEAR_LINECARD_MESSAGE),
    m_slotNumber (slotNumber),
    m_hwType (hwType),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeClearLinecardMessage::VcsNodeClearLinecardMessage (const PrismServiceId &serviceId,const UI32 &operationCode, const UI32 &slotNumber, const UI32 &hwType)
    : PrismMessage (serviceId, operationCode),
    m_slotNumber (slotNumber),
    m_hwType (hwType),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}


VcsNodeClearLinecardMessage::~VcsNodeClearLinecardMessage ()
{
}

void VcsNodeClearLinecardMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_slotNumber,    "slotNumber"));
     addSerializableAttribute (new AttributeUI32 (&m_hwType,        "hwType"));
     addSerializableAttribute (new AttributeUI32 (&m_returnStatus,  "returnStatus"));
}

UI32 VcsNodeClearLinecardMessage::getSlotNumber ()
{
	return m_slotNumber;
}

void VcsNodeClearLinecardMessage::setSlotNumber (const  UI32 &slotNumber)
{
	m_slotNumber = slotNumber;
}

UI32 VcsNodeClearLinecardMessage::getHwType ()
{
	return m_hwType;
}

void VcsNodeClearLinecardMessage::setHwType (const  UI32 &hwType)
{
	m_hwType = hwType;
}

ResourceId VcsNodeClearLinecardMessage::getReturnStatus ()
{
	return m_returnStatus;
}

void VcsNodeClearLinecardMessage::setReturnStatus (const ResourceId &returnStatus)
{
	m_returnStatus = returnStatus;
}



//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeSlotSetLinecardModeMessage-----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
                                
VcsNodeSlotSetLinecardModeMessage::VcsNodeSlotSetLinecardModeMessage ()
  : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_SET_LINECARD_MODE_MESSAGE),
    m_slotNumber (0),
    m_rbridgeId (0),
    m_hwId (0),
    m_linecardMode (0),
    m_hwType (0),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
    
VcsNodeSlotSetLinecardModeMessage::VcsNodeSlotSetLinecardModeMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId)
  : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_SET_LINECARD_MODE_MESSAGE),
    m_slotNumber (slotNumber),
    m_rbridgeId (rbridgeId),
    m_hwId (hwId),
    m_linecardMode (VCS_NODE_SLOT_LINECARD_MODE_DEFAULT),
    m_hwType (SYS_SLOT_LC),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotSetLinecardModeMessage::VcsNodeSlotSetLinecardModeMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType)
  : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_SET_LINECARD_MODE_MESSAGE),
    m_slotNumber (slotNumber),
    m_rbridgeId (rbridgeId),
    m_hwId (hwId),
    m_linecardMode (linecardMode),
    m_hwType (hwType),
    m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsNodeSlotSetLinecardModeMessage::VcsNodeSlotSetLinecardModeMessage (const PrismServiceId &serviceId, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType)
   : PrismMessage (serviceId, operationCode),
     m_slotNumber (slotNumber),
     m_rbridgeId (rbridgeId),
     m_hwId (hwId),
     m_linecardMode (linecardMode),
     m_hwType (hwType),
     m_returnStatus (WAVE_MESSAGE_SUCCESS)
{
}
                
void VcsNodeSlotSetLinecardModeMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_slotNumber,    "slotNumber"));
     addSerializableAttribute (new AttributeUI32 (&m_rbridgeId,     "rbridgeId"));
     addSerializableAttribute (new AttributeUI32 (&m_hwId,          "hwId"));
     addSerializableAttribute (new AttributeUI32 (&m_linecardMode,  "linecardMode"));
     addSerializableAttribute (new AttributeUI32 (&m_hwType,        "hwType"));
     addSerializableAttribute (new AttributeUI32 (&m_returnStatus,  "returnStatus"));
}

VcsNodeSlotSetLinecardModeMessage::~VcsNodeSlotSetLinecardModeMessage ()
{
}

UI32 VcsNodeSlotSetLinecardModeMessage::getSlotNumber ()
{
	return m_slotNumber;
}

void VcsNodeSlotSetLinecardModeMessage::setSlotNumber (const  UI32 &slotNumber)
{
	m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotSetLinecardModeMessage::getRbridgeId ()
{
	return m_rbridgeId;
}

void VcsNodeSlotSetLinecardModeMessage::setRbridgeId (const  UI32 &rbridgeId)
{
	m_rbridgeId = rbridgeId;
}

UI32 VcsNodeSlotSetLinecardModeMessage::getHwId ()
{
	return m_hwId;
}

void VcsNodeSlotSetLinecardModeMessage::setHwId (const  UI32 &hwId)
{
	m_hwId = hwId;
}

UI32 VcsNodeSlotSetLinecardModeMessage::getLinecardMode ()
{
	return m_linecardMode;
}

void VcsNodeSlotSetLinecardModeMessage::setLinecardMode (const  UI32 &linecardMode)
{
	m_linecardMode = linecardMode;
}

UI32 VcsNodeSlotSetLinecardModeMessage::getHwType ()
{
	return m_hwType;
}

void VcsNodeSlotSetLinecardModeMessage::setHwType (const  UI32 &hwType)
{
	m_hwType = hwType;
}

ResourceId VcsNodeSlotSetLinecardModeMessage::getReturnStatus ()
{
	return m_returnStatus;
}

void VcsNodeSlotSetLinecardModeMessage::setReturnStatus (const ResourceId &returnStatus)
{
	m_returnStatus = returnStatus;
}

//----------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeSlotStateQueryMessage-----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

VcsNodeSlotStateQueryMessage::VcsNodeSlotStateQueryMessage ()
: PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_TYPE_QUERY),
    m_slotNumber (0),
    m_hwType (0),
    m_slotState (0)
{
}

VcsNodeSlotStateQueryMessage::VcsNodeSlotStateQueryMessage (const UI32 &slotNumber)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_TYPE_QUERY),
        m_slotNumber (slotNumber),
        m_hwType (SYS_SLOT_LC),
        m_slotState (SLOT_OUT)
{
}

VcsNodeSlotStateQueryMessage::VcsNodeSlotStateQueryMessage (const UI32 &slotNumber, const UI32 &hwType)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SLOT_TYPE_QUERY),
        m_slotNumber (slotNumber),
        m_hwType (hwType),
        m_slotState (SLOT_OUT)
{
}

void VcsNodeSlotStateQueryMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_slotNumber,    "slotNumber"));
     addSerializableAttribute (new AttributeUI32 (&m_hwType,        "hwType"));
     addSerializableAttribute (new AttributeUI32 (&m_slotState,     "slotState"));
}

VcsNodeSlotStateQueryMessage::~VcsNodeSlotStateQueryMessage ()
{
}

UI32 VcsNodeSlotStateQueryMessage::getSlotNumber ()
{
    return m_slotNumber;
}

void VcsNodeSlotStateQueryMessage::setSlotNumber (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

UI32 VcsNodeSlotStateQueryMessage::getHwType ()
{
	return m_hwType;
}

void VcsNodeSlotStateQueryMessage::setHwType (const UI32 &hwType)
{
	m_hwType = hwType;
}

UI32 VcsNodeSlotStateQueryMessage::getSlotState ()
{
    return m_slotState;
}

void VcsNodeSlotStateQueryMessage::setSlotState (const UI32 &slotState)
{
    m_slotState = slotState;
}

bool VcsNodeSlotStateQueryMessage::getIsEnabled ()
{
    if (m_slotState == SLOT_ON)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsNodeCreateDummySlotMessage-----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsNodeCreateDummySlotMessage::VcsNodeCreateDummySlotMessage (const UI32 &slotNumber)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_CREATE_DUMMY_SLOT_MESSAGE),
    m_slotNumber (slotNumber)
{
}

VcsNodeCreateDummySlotMessage::~VcsNodeCreateDummySlotMessage ()
{
}

void VcsNodeCreateDummySlotMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_slotNumber, "slotNumber"));
}

UI32 VcsNodeCreateDummySlotMessage::getSlotNumber ()
{
    return m_slotNumber;
}

void VcsNodeCreateDummySlotMessage::setSlotNumber (const  UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
}

}

