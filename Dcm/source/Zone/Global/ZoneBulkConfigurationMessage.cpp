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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "Zone/Global/ZoneTypes.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"

using namespace WaveNs;

namespace DcmNs
{

ZoneBulkConfigurationMessage::ZoneBulkConfigurationMessage ()
    : ManagementInterfaceMessage (ZoneObjectManager::getClassName (), ZONE_BULK_CONFIGURATION)
{
}

ZoneBulkConfigurationMessage::~ZoneBulkConfigurationMessage ()
{
}

void ZoneBulkConfigurationMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    // This class Specific Attributes below

    addSerializableAttribute (new AttributeStringVector       (&m_cfgs,        "cfgs"));
    addSerializableAttribute (new AttributeStringVectorVector (&m_cfgMembers,  "cfgMembers"));
    addSerializableAttribute (new AttributeStringVector       (&m_zones,       "zones"));
    addSerializableAttribute (new AttributeStringVectorVector (&m_zoneMembers, "zoneMembers"));
    addSerializableAttribute (new AttributeStringVector       (&m_cfgActions,  "cfgActions"));
}

void ZoneBulkConfigurationMessage::addCfg (const string &cfg,  const vector<string> cfgMembers)
{
    m_cfgs.push_back (cfg);
    m_cfgMembers.push_back (cfgMembers);
}

void ZoneBulkConfigurationMessage::addZone (const string &zone, const vector<string> zoneMembers)
{
    m_zones.push_back (zone);
    m_zoneMembers.push_back (zoneMembers);
}

vector<string> &ZoneBulkConfigurationMessage::getCfgs ()
{
    return (m_cfgs);
}

vector<vector<string> > &ZoneBulkConfigurationMessage::getCfgMembers ()
{
    return (m_cfgMembers);
}

vector<string> &ZoneBulkConfigurationMessage::getZones ()
{
    return (m_zones);
}

vector<vector<string> > &ZoneBulkConfigurationMessage::getZoneMembers ()
{
    return (m_zoneMembers);
}

}

