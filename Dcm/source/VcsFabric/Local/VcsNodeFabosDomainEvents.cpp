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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/*************************************************************************
* Domain Valid Event
*************************************************************************/

VcsNodeFabosDomainValidEvent::VcsNodeFabosDomainValidEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_VALID)
{
    m_domainId = 0;
    m_principalDomainId = 0;
    m_eventId = 0;
}

VcsNodeFabosDomainValidEvent::~VcsNodeFabosDomainValidEvent ()
{
}

void VcsNodeFabosDomainValidEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,            "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_principalDomainId,   "principalDomainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,             "eventId"));
}

UI32 VcsNodeFabosDomainValidEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeFabosDomainValidEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32 VcsNodeFabosDomainValidEvent::getPrincipalDomainId () const
{
    return (m_principalDomainId);
}

void VcsNodeFabosDomainValidEvent::setPrincipalDomainId (const UI32 &domainId)
{
    m_principalDomainId = domainId;
}

UI32    VcsNodeFabosDomainValidEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosDomainValidEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Domain Invalid Event
*************************************************************************/

VcsNodeFabosDomainInvalidEvent::VcsNodeFabosDomainInvalidEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_INVALID)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeFabosDomainInvalidEvent::~VcsNodeFabosDomainInvalidEvent ()
{
}

void VcsNodeFabosDomainInvalidEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId, "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,  "eventId"));
}

UI32 VcsNodeFabosDomainInvalidEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeFabosDomainInvalidEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeFabosDomainInvalidEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosDomainInvalidEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Domain Capabilities Event
*************************************************************************/


VcsNodeFabosDomainCapabilitiesAvailableEvent::VcsNodeFabosDomainCapabilitiesAvailableEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeFabosDomainCapabilitiesAvailableEvent::~VcsNodeFabosDomainCapabilitiesAvailableEvent ()
{
}

void VcsNodeFabosDomainCapabilitiesAvailableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId, "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,  "eventId"));
}

UI32 VcsNodeFabosDomainCapabilitiesAvailableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeFabosDomainCapabilitiesAvailableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeFabosDomainCapabilitiesAvailableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosDomainCapabilitiesAvailableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Domain Reachable Event
*************************************************************************/

VcsNodeFabosDomainReachableEvent::VcsNodeFabosDomainReachableEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_REACHABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeFabosDomainReachableEvent::~VcsNodeFabosDomainReachableEvent ()
{
}

void VcsNodeFabosDomainReachableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId, "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,  "eventId"));
}

UI32 VcsNodeFabosDomainReachableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeFabosDomainReachableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeFabosDomainReachableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosDomainReachableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Domain Unreachable Event
*************************************************************************/

VcsNodeFabosDomainUnreachableEvent::VcsNodeFabosDomainUnreachableEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_UNREACHABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeFabosDomainUnreachableEvent::~VcsNodeFabosDomainUnreachableEvent ()
{
}

void VcsNodeFabosDomainUnreachableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId, "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,  "eventId"));
}

UI32 VcsNodeFabosDomainUnreachableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeFabosDomainUnreachableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeFabosDomainUnreachableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosDomainUnreachableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Switch Offline Event
*************************************************************************/

VcsNodeFabosSwitchOfflineEvent::VcsNodeFabosSwitchOfflineEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_OFFLINE),
    m_eventId (0)
{
}

VcsNodeFabosSwitchOfflineEvent::~VcsNodeFabosSwitchOfflineEvent ()
{
}

void VcsNodeFabosSwitchOfflineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeFabosSwitchOfflineEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosSwitchOfflineEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Zone Changed Event
*************************************************************************/

VcsNodeFabosZoneChangedEvent::VcsNodeFabosZoneChangedEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_ZONE_CHANGED)
{
}

VcsNodeFabosZoneChangedEvent::~VcsNodeFabosZoneChangedEvent ()
{
}

UI32    VcsNodeFabosZoneChangedEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosZoneChangedEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Zone Aborted Event
*************************************************************************/

VcsNodeFabosZoneAbortedEvent::VcsNodeFabosZoneAbortedEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_ZONE_ABORTED)
{
}

VcsNodeFabosZoneAbortedEvent::~VcsNodeFabosZoneAbortedEvent ()
{
}

UI32    VcsNodeFabosZoneAbortedEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeFabosZoneAbortedEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

}
