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

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/*************************************************************************
* Domain Valid Event
*************************************************************************/

VcsNodeDomainValidEvent::VcsNodeDomainValidEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_VALID)
{
    m_domainId = 0;
    m_principalDomainId = 0;
    m_eventId = 0;
}

VcsNodeDomainValidEvent::~VcsNodeDomainValidEvent ()
{
}

void VcsNodeDomainValidEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,            "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_principalDomainId,   "principalDomainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,             "eventId"));
}

UI32 VcsNodeDomainValidEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeDomainValidEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32 VcsNodeDomainValidEvent::getPrincipalDomainId () const
{
    return (m_principalDomainId);
}

void VcsNodeDomainValidEvent::setPrincipalDomainId (const UI32 &domainId)
{
    m_principalDomainId = domainId;
}

UI32    VcsNodeDomainValidEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeDomainValidEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Domain Invalid Event
*************************************************************************/

VcsNodeDomainInvalidEvent::VcsNodeDomainInvalidEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_INVALID)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeDomainInvalidEvent::~VcsNodeDomainInvalidEvent ()
{
}

void VcsNodeDomainInvalidEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,    "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeDomainInvalidEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeDomainInvalidEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeDomainInvalidEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeDomainInvalidEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Domain Capabilities Event
*************************************************************************/


VcsNodeDomainCapabilitiesAvailableEvent::VcsNodeDomainCapabilitiesAvailableEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_CAPABILITIES_AVAILABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeDomainCapabilitiesAvailableEvent::~VcsNodeDomainCapabilitiesAvailableEvent ()
{
}

void VcsNodeDomainCapabilitiesAvailableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,    "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeDomainCapabilitiesAvailableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeDomainCapabilitiesAvailableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeDomainCapabilitiesAvailableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeDomainCapabilitiesAvailableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}


/*************************************************************************
* Domain Reachable Event
*************************************************************************/

VcsNodeDomainReachableEvent::VcsNodeDomainReachableEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_REACHABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeDomainReachableEvent::~VcsNodeDomainReachableEvent ()
{
}

void VcsNodeDomainReachableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,    "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeDomainReachableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeDomainReachableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeDomainReachableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeDomainReachableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Domain Unreachable Event
*************************************************************************/

VcsNodeDomainUnreachableEvent::VcsNodeDomainUnreachableEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_UNREACHABLE)
{
    m_domainId = 0;
    m_eventId = 0;
}

VcsNodeDomainUnreachableEvent::~VcsNodeDomainUnreachableEvent ()
{
}

void VcsNodeDomainUnreachableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_domainId,    "domainId"));
     addSerializableAttribute (new AttributeUI32   (&m_eventId,     "eventId"));
}

UI32 VcsNodeDomainUnreachableEvent::getDomainId () const
{
    return (m_domainId);
}

void VcsNodeDomainUnreachableEvent::setDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

UI32    VcsNodeDomainUnreachableEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeDomainUnreachableEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************************
* Switch Offline Event
*************************************************************************/

VcsNodeSwitchOfflineEvent::VcsNodeSwitchOfflineEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_SWITCH_OFFLINE),
    m_eventId (0)
{
}

VcsNodeSwitchOfflineEvent::~VcsNodeSwitchOfflineEvent ()
{
}

void VcsNodeSwitchOfflineEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_eventId, "eventId"));
}

UI32    VcsNodeSwitchOfflineEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeSwitchOfflineEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}



/*************************************************************
* Zone Changed Event
*************************************************************************/

VcsNodeZoneChangedEvent::VcsNodeZoneChangedEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_ZONE_CHANGED),
    m_eventId (0)
{
}

VcsNodeZoneChangedEvent::~VcsNodeZoneChangedEvent ()
{
}

UI32    VcsNodeZoneChangedEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeZoneChangedEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

/*************************************************************
* Zone Aborted Event
*************************************************************************/

VcsNodeZoneAbortedEvent::VcsNodeZoneAbortedEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_ZONE_ABORTED),
    m_eventId (0)
{
}

VcsNodeZoneAbortedEvent::~VcsNodeZoneAbortedEvent ()
{
}

UI32    VcsNodeZoneAbortedEvent::getEventId () const
{
    return m_eventId;
}

void    VcsNodeZoneAbortedEvent::setEventId (const UI32 &eventId)
{
    m_eventId = eventId;
}

}

