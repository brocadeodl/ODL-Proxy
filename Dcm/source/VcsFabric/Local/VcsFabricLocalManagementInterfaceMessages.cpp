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
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalManagementInterfaceMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

// VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage

VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_CURRENT_RBRIDGE_IDS_AND_LOCATION_IDS)
{
}

VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::~VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage ()
{
}

void VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32Vector       (&m_rbridgeIds,  "rbridgeIds"));
    addSerializableAttribute (new AttributeLocationIdVector (&m_locationIds, "locationIds"));
}

const vector<UI32> & VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::getRbridgeIds () const
{
    return (m_rbridgeIds);
}

void VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::setRbridgeIds (const vector<UI32> &rbridgeIds)
{
    m_rbridgeIds = rbridgeIds;
}

const vector<LocationId> & VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::getLocationIds () const
{
    return (m_locationIds);
}

void VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage::setLocationIds (const vector<LocationId> &locationIds)
{
    m_locationIds = locationIds;
}


VcsWyserEaRateLimitingDelayMessage::VcsWyserEaRateLimitingDelayMessage (const UI32 &operationId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), operationId)
{
}

VcsWyserEaRateLimitingDelayMessage::~VcsWyserEaRateLimitingDelayMessage ()
{
}

void VcsWyserEaRateLimitingDelayMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString  (&m_wyserUserSessionContext, "wyserUserSessionContext"));
    addSerializableAttribute (new AttributeUI32    (&m_delayInMicroseconds,  "delayInMicroseconds"));
}

const string & VcsWyserEaRateLimitingDelayMessage::getWyserUserInfoContext () const
{
    return (m_wyserUserSessionContext);
}

void VcsWyserEaRateLimitingDelayMessage::setWyserUserInfoContext (const string &wyserUserSessionContext)
{
    m_wyserUserSessionContext = wyserUserSessionContext;
}

const UI32 & VcsWyserEaRateLimitingDelayMessage::getDelayInMicroseconds () const
{
    return (m_delayInMicroseconds);
}

void VcsWyserEaRateLimitingDelayMessage::setDelayInMicroseconds (const UI32 &delayInMicroseconds)
{
    m_delayInMicroseconds = delayInMicroseconds;
}


VcsSetWyserEaRateLimitingDelayMessage::VcsSetWyserEaRateLimitingDelayMessage ()
    : VcsWyserEaRateLimitingDelayMessage (VCS_SET_WYSEREA_RATE_LIMITING_DELAY)
{
}

VcsSetWyserEaRateLimitingDelayMessage::~VcsSetWyserEaRateLimitingDelayMessage ()
{
}


VcsGetWyserEaRateLimitingDelayMessage::VcsGetWyserEaRateLimitingDelayMessage ()
    : VcsWyserEaRateLimitingDelayMessage (VCS_GET_WYSEREA_RATE_LIMITING_DELAY)
{
}

VcsGetWyserEaRateLimitingDelayMessage::~VcsGetWyserEaRateLimitingDelayMessage ()
{
}


VcsSetWyserEaConfigFilterServiceIndependentMessage::VcsSetWyserEaConfigFilterServiceIndependentMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_SET_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT),
    m_userSessionId                     (0),
    m_sessionConfigFilterContextType    (DCM_CONFIG_FILTER_CONTEXT_NONE)
{
}

VcsSetWyserEaConfigFilterServiceIndependentMessage::VcsSetWyserEaConfigFilterServiceIndependentMessage (const ResourceId &sessionConfigFilterContextType, const SI32 &userSessionId)
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_SET_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT),
    m_userSessionId                     (userSessionId),
    m_sessionConfigFilterContextType    (sessionConfigFilterContextType)
{
}

VcsSetWyserEaConfigFilterServiceIndependentMessage::~VcsSetWyserEaConfigFilterServiceIndependentMessage ()
{
}

void VcsSetWyserEaConfigFilterServiceIndependentMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeSI32         (&m_userSessionId,                  "userSessionId"));
    addSerializableAttribute (new AttributeResourceId   (&m_sessionConfigFilterContextType, "sessionConfigFilterContextType"));
}

const SI32 &VcsSetWyserEaConfigFilterServiceIndependentMessage::getUserSessionId () const
{
    return (m_userSessionId);
}

void VcsSetWyserEaConfigFilterServiceIndependentMessage::setUserSessionId (const SI32 &userSessionId)
{
    m_userSessionId = userSessionId;
}

const ResourceId &VcsSetWyserEaConfigFilterServiceIndependentMessage::getSessionConfigFilterContextType () const
{
    return (m_sessionConfigFilterContextType);
}

void VcsSetWyserEaConfigFilterServiceIndependentMessage::setSessionConfigFilterContextType (const ResourceId &sessionConfigFilterContextType)
{
    m_sessionConfigFilterContextType = sessionConfigFilterContextType;
}


VcsRemoveWyserEaConfigFilterServiceIndependentMessage::VcsRemoveWyserEaConfigFilterServiceIndependentMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_REMOVE_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT),
    m_userSessionId (0)
{
}

VcsRemoveWyserEaConfigFilterServiceIndependentMessage::VcsRemoveWyserEaConfigFilterServiceIndependentMessage (const SI32 &userSessionId)
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_REMOVE_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT),
    m_userSessionId (userSessionId)
{
}

VcsRemoveWyserEaConfigFilterServiceIndependentMessage::~VcsRemoveWyserEaConfigFilterServiceIndependentMessage ()
{
}

void VcsRemoveWyserEaConfigFilterServiceIndependentMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeSI32 (&m_userSessionId, "userSessionId"));
}

const SI32 &VcsRemoveWyserEaConfigFilterServiceIndependentMessage::getUserSessionId () const
{
    return (m_userSessionId);
}

void VcsRemoveWyserEaConfigFilterServiceIndependentMessage::setUserSessionId (const SI32 &userSessionId)
{
    m_userSessionId = userSessionId;
}

}
