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

#include "Framework/DistributedLog/DistributedLogAddLogEntryMessage.h"
#include "Framework/DistributedLog/DistributedLogTypes.h"
#include "Framework/DistributedLog/DistributedLogObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage ()
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (),
    m_logDescriptionType    (),
    m_description           (""),
    m_username              (""),
    m_prismServiceId        (),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (FrameworkToolKit::getThisWaveNodeObjectId ()),
    m_managedObjectId       ()
{
}

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (logType),
    m_logDescriptionType    (logDescriptionType),
    m_description           (description),
    m_username              (""),
    m_prismServiceId        (),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (FrameworkToolKit::getThisWaveNodeObjectId ()),
    m_managedObjectId       ()
{
}

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const string &username)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (logType),
    m_logDescriptionType    (logDescriptionType),
    m_description           (description),
    m_username              (username),
    m_prismServiceId        (),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (FrameworkToolKit::getThisWaveNodeObjectId ()),
    m_managedObjectId       ()
{
}

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const string &username, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (logType),
    m_logDescriptionType    (logDescriptionType),
    m_description           (description),
    m_username              (username),
    m_prismServiceId        (prismServiceId),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (waveNodeObjectId),
    m_managedObjectId       ()
{
}

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (logType),
    m_logDescriptionType    (logDescriptionType),
    m_description           (description),
    m_username              (""),
    m_prismServiceId        (prismServiceId),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (waveNodeObjectId),
    m_managedObjectId       ()
{
}

DistributedLogAddLogEntryMessage::DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId, const ObjectId &managedObjectId)
    : ManagementInterfaceMessage (DistributedLogObjectManager::getServiceName (), DISTRIBUTED_LOG_ADD_LOG_ENTRY),
    m_logType               (logType),
    m_logDescriptionType    (logDescriptionType),
    m_description           (description),
    m_username              (""),
    m_prismServiceId        (prismServiceId),
    m_dateTimeStamp         (),
    m_waveNodeObjectId      (waveNodeObjectId),
    m_managedObjectId       (managedObjectId)
{
}

DistributedLogAddLogEntryMessage::~DistributedLogAddLogEntryMessage ()
{
}

void DistributedLogAddLogEntryMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeResourceId      (&m_logType,            "logType"));
     addSerializableAttribute (new AttributeResourceId      (&m_logDescriptionType, "logDescriptionType"));
     addSerializableAttribute (new AttributeString          (&m_description,        "description"));
     addSerializableAttribute (new AttributeString          (&m_username,           "username"));
     addSerializableAttribute (new AttributePrismServiceId  (&m_prismServiceId,     "prismServiceId"));
     addSerializableAttribute (new AttributeDateTime        (&m_dateTimeStamp,      "dateTimeStamp"));
     addSerializableAttribute (new AttributeObjectId        (&m_waveNodeObjectId,   "waveNodeObjectId"));
     addSerializableAttribute (new AttributeObjectId        (&m_managedObjectId,    "managedObjectId"));
}

ResourceId DistributedLogAddLogEntryMessage::getLogType () const
{
    return (m_logType);
}

void DistributedLogAddLogEntryMessage::setLogType (const ResourceId &logType)
{
    m_logType = logType;
}

ResourceId DistributedLogAddLogEntryMessage::getLogDescriptionType () const
{
    return (m_logDescriptionType);
}

void DistributedLogAddLogEntryMessage::setLogDescriptionType (const ResourceId &logDescriptionType)
{
    m_logDescriptionType = logDescriptionType;
}

string DistributedLogAddLogEntryMessage::getDescription () const
{
    return (m_description);
}

void DistributedLogAddLogEntryMessage::setDescription (const string &description)
{
    m_description = description;
}

string DistributedLogAddLogEntryMessage::getUsername () const
{
    return (m_username);
}

void DistributedLogAddLogEntryMessage::setUsername (const string &username)
{
    m_username = username;
}

PrismServiceId DistributedLogAddLogEntryMessage::getPrismServiceId () const
{
    return (m_prismServiceId);
}

void DistributedLogAddLogEntryMessage::setPrismServiceId (PrismServiceId prismServiceId)
{
    m_prismServiceId = prismServiceId;
}

DateTime DistributedLogAddLogEntryMessage::getDateTimeStamp () const
{
    return (m_dateTimeStamp);
}

void DistributedLogAddLogEntryMessage::setDateTimeStamp (const DateTime &dateTimeStamp)
{
    m_dateTimeStamp = dateTimeStamp;
}

ObjectId DistributedLogAddLogEntryMessage::getWaveNodeObjectId () const
{
    return (m_waveNodeObjectId);
}

void DistributedLogAddLogEntryMessage::setWaveNodeObjectId (const ObjectId &waveNodeObjectId)
{
    m_waveNodeObjectId = waveNodeObjectId;
}

ObjectId DistributedLogAddLogEntryMessage::getManagedObjectId () const
{
    return (m_managedObjectId);
}

void DistributedLogAddLogEntryMessage::setManagedObjectId (const ObjectId &managedObjectId)
{
    m_managedObjectId = managedObjectId;
}

}

