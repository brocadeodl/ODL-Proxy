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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/DistributedLog/DistributedLogUpdateMaxLogEntriesMessage.h"
#include "Framework/DistributedLog/DistributedLogTypes.h"
#include "Framework/DistributedLog/DistributedLogObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

DistributedLogUpdateMaxLogEntriesMessage::DistributedLogUpdateMaxLogEntriesMessage ()
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES),
    m_maxLogEntries (10000)
{
}

DistributedLogUpdateMaxLogEntriesMessage::DistributedLogUpdateMaxLogEntriesMessage (const UI32 &maxLogEntries)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES),
    m_maxLogEntries (maxLogEntries)
{
}

DistributedLogUpdateMaxLogEntriesMessage::~DistributedLogUpdateMaxLogEntriesMessage ()
{
}

void DistributedLogUpdateMaxLogEntriesMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeUI32 (&m_maxLogEntries, "maxLogEntries"));
}

UI32 DistributedLogUpdateMaxLogEntriesMessage::getMaxLogEntries () const
{
    return (m_maxLogEntries);
}
 
}
