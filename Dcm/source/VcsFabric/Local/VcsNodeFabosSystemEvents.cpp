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
 *   Author : Leifang Hu, Cindy Hou                                        *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosSystemEvents.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace DcmNs
{

/*************************************************************
* SwitchReadyToAcceptCommands Event
*************************************************************************/

VcsNodeFabosSwitchReadyToAcceptCommandsEvent::VcsNodeFabosSwitchReadyToAcceptCommandsEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS),
    m_eventId (0)
{
}

VcsNodeFabosSwitchReadyToAcceptCommandsEvent::~VcsNodeFabosSwitchReadyToAcceptCommandsEvent ()
{
}

void VcsNodeFabosSwitchReadyToAcceptCommandsEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeFabosSwitchReadyToAcceptCommandsEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosSwitchReadyToAcceptCommandsEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

VcsNodeFabosSwitchActivationCompleteEvent::VcsNodeFabosSwitchActivationCompleteEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE),
	m_eventId (0)
{
}

VcsNodeFabosSwitchActivationCompleteEvent::~VcsNodeFabosSwitchActivationCompleteEvent ()
{
}

void VcsNodeFabosSwitchActivationCompleteEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeFabosSwitchActivationCompleteEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosSwitchActivationCompleteEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Sync Start Event
*************************************************************************/

VcsNodeFabosSyncStartEvent::VcsNodeFabosSyncStartEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYNC_START),
      m_peerHaIpAddress (""),
      m_peerHaIpPort (0),
      m_isChassisFirmwaredownload (false),
      m_eventId (0)
{
}

VcsNodeFabosSyncStartEvent::VcsNodeFabosSyncStartEvent (string &peerHaIpAddress, SI32 &peerHaIpPort)
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYNC_START),
      m_peerHaIpAddress (peerHaIpAddress),
      m_peerHaIpPort (peerHaIpPort),
      m_isChassisFirmwaredownload (false),
      m_eventId (0)
{
}

VcsNodeFabosSyncStartEvent::~VcsNodeFabosSyncStartEvent ()
{
}

void VcsNodeFabosSyncStartEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_peerHaIpAddress,          "peerHaIpAddress"));
    addSerializableAttribute (new AttributeSI32   (&m_peerHaIpPort,             "peerHaIpPort"));
    addSerializableAttribute (new AttributeBool   (&m_isChassisFirmwaredownload,"isChassisFirmwaredownload"));
    addSerializableAttribute (new AttributeUI32   (&m_eventId,                  "eventId"));
}

string VcsNodeFabosSyncStartEvent::getPeerHaIpAddress () const
{
    return (m_peerHaIpAddress);
}

void VcsNodeFabosSyncStartEvent::setPeerHaIpAddress (const string &peerHaIpAddress)
{
    m_peerHaIpAddress = peerHaIpAddress;
}

SI32 VcsNodeFabosSyncStartEvent::getPeerHaIpPort () const
{
    return (m_peerHaIpPort);
}

void VcsNodeFabosSyncStartEvent::setPeerHaIpPort (const SI32 &peerHaIpPort)
{
    m_peerHaIpPort = peerHaIpPort;
}

bool VcsNodeFabosSyncStartEvent::getIsChassisFirmwaredownload () const
{
    return (m_isChassisFirmwaredownload);
}

void VcsNodeFabosSyncStartEvent::setIsChassisFirmwaredownload (const bool &isChassisFirmwaredownload)
{
    m_isChassisFirmwaredownload = isChassisFirmwaredownload;
}

UI32 VcsNodeFabosSyncStartEvent::getEventId () const
{
    return (m_eventId);
}

void VcsNodeFabosSyncStartEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Service Ready Event
*************************************************************************/

VcsNodeFabosServiceReadyEvent::VcsNodeFabosServiceReadyEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SERVICE_READY),
    m_recoveryType (0),
	m_eventId (0)
{
}

VcsNodeFabosServiceReadyEvent::~VcsNodeFabosServiceReadyEvent()
{
}

void VcsNodeFabosServiceReadyEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeResourceId   (&m_recoveryType,   "recoveryType"));
 	addSerializableAttribute (new AttributeUI32         (&m_eventId,        "eventId"));
}

ResourceId  VcsNodeFabosServiceReadyEvent::getRecoveryType () const
{
    return (m_recoveryType);
}

void  VcsNodeFabosServiceReadyEvent::setRecoveryType (const ResourceId &recoveryType)
{
    m_recoveryType = recoveryType;
}

UI32 VcsNodeFabosServiceReadyEvent::getEventId () const
{
    return (m_eventId);
}

void VcsNodeFabosServiceReadyEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Linecard Ready Event
*************************************************************************/

VcsNodeFabosLinecardReadyEvent::VcsNodeFabosLinecardReadyEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_LINECARD_READY),
    m_recoveryType (0),
    m_slotId (0),
    m_eventId (0)
{
}

VcsNodeFabosLinecardReadyEvent::VcsNodeFabosLinecardReadyEvent (const UI32 &slotId, const ResourceId &recoveryType, const UI32 &eventId)
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_LINECARD_READY),
    m_recoveryType (recoveryType),
    m_slotId (slotId),
    m_eventId (eventId)
{
}

VcsNodeFabosLinecardReadyEvent::~VcsNodeFabosLinecardReadyEvent()
{
}

void VcsNodeFabosLinecardReadyEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeResourceId   (&m_recoveryType,   "recoveryType"));
    addSerializableAttribute (new AttributeUI32         (&m_slotId,         "slotId"));
	addSerializableAttribute (new AttributeUI32         (&m_eventId,        "eventId"));
}

ResourceId  VcsNodeFabosLinecardReadyEvent::getRecoveryType () const
{
    return m_recoveryType;
}

void  VcsNodeFabosLinecardReadyEvent::setRecoveryType (const ResourceId &recoveryType)
{
    m_recoveryType = recoveryType;
}

UI32  VcsNodeFabosLinecardReadyEvent::getSlotId () const
{
    return m_slotId;
}

void  VcsNodeFabosLinecardReadyEvent::setSlotId (const UI32 &slotId)
{
    m_slotId = slotId;
}

UI32 VcsNodeFabosLinecardReadyEvent::getEventId () const
{
    return (m_eventId);
}

void VcsNodeFabosLinecardReadyEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Service Ready Event
*************************************************************************/

VcsNodeFabosSystemReadyEvent::VcsNodeFabosSystemReadyEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYSTEM_READY),
    m_recoveryType (0)
{
}

VcsNodeFabosSystemReadyEvent::~VcsNodeFabosSystemReadyEvent()
{
}

void VcsNodeFabosSystemReadyEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeResourceId  (&m_recoveryType, "recoveryType"));
}

ResourceId  VcsNodeFabosSystemReadyEvent::getRecoveryType () const
{
    return m_recoveryType;
}

void  VcsNodeFabosSystemReadyEvent::setRecoveryType (const ResourceId &recoveryType)
{
    m_recoveryType = recoveryType;
}

UI32 VcsNodeFabosSystemReadyEvent::getEventId () const
{
    return (m_eventId);
}

void VcsNodeFabosSystemReadyEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

}
