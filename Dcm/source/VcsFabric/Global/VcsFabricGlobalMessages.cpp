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

#include "VcsFabric/Global/VcsFabricGlobalMessages.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Framework/Attributes/AttributeDateTime.h"


namespace DcmNs
{
VcsFabConfigSnapshotMessage::VcsFabConfigSnapshotMessage (const UI32 &opcode)
    : PrismMessage (VcsFabObjectManager::getPrismServiceId (), opcode),
      m_snapshotId          (""),
      m_rbridgeId           (0),
      m_localConfigData     ("")
{
}

VcsFabConfigSnapshotMessage::~VcsFabConfigSnapshotMessage ()
{
}

void VcsFabConfigSnapshotMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString      (&m_snapshotId,             "snapshotId"));
     addSerializableAttribute (new AttributeUI32        (&m_rbridgeId,              "rbridgeId"));
     addSerializableAttribute (new AttributeString      (&m_localConfigData,        "localConfigData"));
     addSerializableAttribute (new AttributeDateTime    (&m_snapshotCreatedTime,    "snapshotCreatedTime"));
}

void VcsFabConfigSnapshotMessage::setSnapshotId  (const string& snapshotId )
{
    m_snapshotId = snapshotId ;
}

string VcsFabConfigSnapshotMessage::getSnapshotId  () const
{
    return ( m_snapshotId );
}

void VcsFabConfigSnapshotMessage::setRbridgeId (const UI32& rbridgeId )
{
    m_rbridgeId = rbridgeId;
}

UI32 VcsFabConfigSnapshotMessage::getRbridgeId () const
{   
    return ( m_rbridgeId );
}

void VcsFabConfigSnapshotMessage::setSnapshotCreatedTime ( const DateTime &snapshotCreatedTime )
{   
    m_snapshotCreatedTime = snapshotCreatedTime;
}

DateTime VcsFabConfigSnapshotMessage::getSnapshotCreatedTime () const
{   
    return ( m_snapshotCreatedTime );
}

void VcsFabConfigSnapshotMessage::setLocalConfigData ( const string &localConfigData )
{
    m_localConfigData = localConfigData;
}

string VcsFabConfigSnapshotMessage::getLocalConfigData () const
{
    return ( m_localConfigData );
}

}
