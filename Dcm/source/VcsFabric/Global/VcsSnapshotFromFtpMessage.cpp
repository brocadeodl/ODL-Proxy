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
*   Author : Vcs Infra Team                                               *
***************************************************************************/
#include "VcsFabric/Global/VcsSnapshotFromFtpMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Framework/Attributes/AttributeDateTime.h"

namespace DcmNs
{

VcsCopySnapshotFromFtpMessage::VcsCopySnapshotFromFtpMessage ()
    :   ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FAB_CONFIG_SNAPSHOT_COPY_FROM_FTP),
        m_snapshotId    (""),
        m_rbridgeId     (0),
        m_localFilename (""),
        m_globalFilename("")
{
}

VcsCopySnapshotFromFtpMessage::VcsCopySnapshotFromFtpMessage (const string &snapshotId, const UI32 &rbridgeId, const string &localFilename, const string &globalFilename)
    :   ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FAB_CONFIG_SNAPSHOT_COPY_FROM_FTP),
        m_snapshotId    (snapshotId),
        m_rbridgeId     (rbridgeId),
        m_localFilename (localFilename),
        m_globalFilename(globalFilename)
{
}

VcsCopySnapshotFromFtpMessage::~VcsCopySnapshotFromFtpMessage ()
{
}

void VcsCopySnapshotFromFtpMessage::setupAttributesForSerialization ()
{   
    ManagementInterfaceMessage::setupAttributesForSerialization ();
        
    addSerializableAttribute (new AttributeString   (&m_snapshotId,         "snapshotId"));
    addSerializableAttribute (new AttributeUI32     (&m_rbridgeId,          "rbridgeId"));
    addSerializableAttribute (new AttributeString   (&m_localFilename,      "localFilename"));
    addSerializableAttribute (new AttributeDateTime (&m_snapshotCreatedTime,"snapshotCreatedTime"));
    addSerializableAttribute (new AttributeString   (&m_globalFilename,     "globalFilename"));
}

void VcsCopySnapshotFromFtpMessage::setSnapshotId  (const string& snapshotId)
{   
    m_snapshotId = snapshotId ;
}

string VcsCopySnapshotFromFtpMessage::getSnapshotId  () const
{
    return (m_snapshotId);
}

void VcsCopySnapshotFromFtpMessage::setRbridgeId (const UI32& rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

UI32 VcsCopySnapshotFromFtpMessage::getRbridgeId () const
{
    return (m_rbridgeId);
}

void VcsCopySnapshotFromFtpMessage::setLocalConfigFilename (const string &localFilename)
{
    m_localFilename = localFilename;
}

string VcsCopySnapshotFromFtpMessage::getLocalConfigFilename () const
{
    return (m_localFilename);
}

void VcsCopySnapshotFromFtpMessage::setGlobalConfigFilename (const string &globalFilename)
{
    m_globalFilename = globalFilename;
}

string VcsCopySnapshotFromFtpMessage::getGlobalConfigFilename () const
{
    return (m_globalFilename);
}

void VcsCopySnapshotFromFtpMessage::setSnapshotCreatedTime (const DateTime &snapshotCreatedTime)
{
    m_snapshotCreatedTime = snapshotCreatedTime;
}

DateTime VcsCopySnapshotFromFtpMessage::getSnapshotCreatedTime () const
{
    return (m_snapshotCreatedTime);
}

}
