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

#include "VcsFabric/Local/VcsNodeSegmentSwitchMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSegmentSwitchMessage::VcsNodeSegmentSwitchMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SEGMENT_SWITCH)
{
}

VcsNodeSegmentSwitchMessage::VcsNodeSegmentSwitchMessage (const vector<WorldWideName> &domainWwn, const vector<UI32> &segmentReason)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SEGMENT_SWITCH),
      m_domainWwn                (domainWwn),
      m_segmentReason            (segmentReason)
{
}

VcsNodeSegmentSwitchMessage::~VcsNodeSegmentSwitchMessage ()
{
}

void VcsNodeSegmentSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeWorldWideNameVector (&m_domainWwn,      "domainWwn"));
     addSerializableAttribute (new AttributeUI32Vector          (&m_segmentReason,  "segmentReason"));
}

vector<WorldWideName> VcsNodeSegmentSwitchMessage::getDomainWwn () const
{
    return (m_domainWwn);
}

void VcsNodeSegmentSwitchMessage::setDomainWwn (const vector<WorldWideName> &domainWwn)
{
    m_domainWwn = domainWwn;
}

vector<UI32> VcsNodeSegmentSwitchMessage::getSegmentReason () const
{
    return (m_segmentReason);
}

void VcsNodeSegmentSwitchMessage::setSegmentReason (const vector<UI32> &segmentReason)
{
    m_segmentReason = segmentReason;
}
}

