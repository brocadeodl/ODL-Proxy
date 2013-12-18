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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/

#include "Framework/CliBlockManagement/CliBlockServiceIndependentMessage.h"
#include "Framework/CliBlockManagement/CliBlockMessage.h"
#include "Framework/CliBlockManagement/CliBlockType.h"
#include "Framework/Attributes/Attributes.h" 
#include "Framework/Attributes/AttributeResourceId.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{

CliBlockServiceIndependentMessage::CliBlockServiceIndependentMessage (const CliBlockMessage &cliBlockMessage)
    : WaveServiceIndependentMessage (ClusterLocalObjectManager::getPrismServiceId (), CLI_BLOCK_SERVICE_INDEPENDENT)
{

    m_operation         = cliBlockMessage.getOperation            ();
    m_reason            = cliBlockMessage.getReason               ();
    m_originator        = cliBlockMessage.getOriginator           ();
    m_prismServiceId    = cliBlockMessage.getPrismServiceId       ();

}

CliBlockServiceIndependentMessage::CliBlockServiceIndependentMessage ()
    : WaveServiceIndependentMessage (ClusterLocalObjectManager::getPrismServiceId (), CLI_BLOCK_SERVICE_INDEPENDENT)
{
    m_operation      = true;
    m_reason         = WAVE_FRAMEWORK_STATE_UNKNOWN_STATE;
    m_originator     = 0;
    m_prismServiceId = 0;
}

CliBlockServiceIndependentMessage::~CliBlockServiceIndependentMessage ()
{
}

void CliBlockServiceIndependentMessage::setupAttributesForSerialization ()
{

    WaveServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_operation, "operation"));
    addSerializableAttribute (new AttributeResourceId (&m_reason, "reason"));
    addSerializableAttribute (new AttributeLocationId (&m_originator, "originator"));
    addSerializableAttribute (new AttributeUI32 (&m_prismServiceId, "prismServiceId"));

}

const ResourceId CliBlockServiceIndependentMessage::getReason () const
{
    return (m_reason);
}

const bool CliBlockServiceIndependentMessage::getOperation () const
{
    return (m_operation);
}

const LocationId CliBlockServiceIndependentMessage::getOriginator () const
{
    return (m_originator);
}

const PrismServiceId CliBlockServiceIndependentMessage::getPrismServiceId () const
{
    return (m_prismServiceId);
}

}
