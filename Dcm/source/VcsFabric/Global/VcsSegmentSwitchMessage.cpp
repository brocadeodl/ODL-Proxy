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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Geetha Aparna                                                *
 ***************************************************************************/

#include "VcsFabric/Global/VcsSegmentSwitchMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

namespace DcmNs
{

VcsSegmentSwitchMessage::VcsSegmentSwitchMessage ()
    : ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_SEGMENT_SWITCH)
{
}

VcsSegmentSwitchMessage::VcsSegmentSwitchMessage (const vector<UI32> &domainId, const vector<UI32> &segmentReason)
    : ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_SEGMENT_SWITCH),
      m_domainId                 (domainId),
      m_segmentReason            (segmentReason)
{
}

VcsSegmentSwitchMessage::~VcsSegmentSwitchMessage ()
{
}

void VcsSegmentSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32Vector (&m_domainId,        "domainId"));
     addSerializableAttribute (new AttributeUI32Vector (&m_segmentReason,   "segmentReason"));
}

vector<UI32> VcsSegmentSwitchMessage::getDomainId () const
{
    return (m_domainId);
}

void VcsSegmentSwitchMessage::setDomainId (const vector<UI32> &domainId)
{
    m_domainId = domainId;
}

vector<UI32> VcsSegmentSwitchMessage::getSegmentReason () const
{
    return (m_segmentReason);
}

void VcsSegmentSwitchMessage::setSegmentReason (const vector<UI32> &segmentReason)
{
    m_segmentReason = segmentReason;
}
}

