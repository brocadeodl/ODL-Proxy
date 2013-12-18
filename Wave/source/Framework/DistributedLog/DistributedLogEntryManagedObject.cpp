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

#include "Framework/DistributedLog/DistributedLogEntryManagedObject.h"
#include "Framework/DistributedLog/DistributedLogObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

DistributedLogEntryManagedObject::DistributedLogEntryManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager)
    : PrismElement              (pDistributedLogObjectManager),
      PrismPersistableObject    (DistributedLogEntryManagedObject::getClassName (), WaveManagedObject::getClassName()),
      WaveManagedObject         (pDistributedLogObjectManager),
      m_logId                   (0),
      m_logType                 (),
      m_logDescriptionType      (),
      m_dateTimeStamp           (),
      m_prismServiceId          (),
      m_username                (""),
      m_description             (""),
      m_waveNodeObjectId        (),
      m_managedObjectId         ()
{
}

DistributedLogEntryManagedObject::DistributedLogEntryManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager, const string &name)
    : PrismElement              (pDistributedLogObjectManager),
      PrismPersistableObject    (DistributedLogEntryManagedObject::getClassName (), WaveManagedObject::getClassName()),
      WaveManagedObject         (pDistributedLogObjectManager),
      m_logId                   (0),
      m_logType                 (),
      m_logDescriptionType      (),
      m_dateTimeStamp           (),
      m_prismServiceId          (),
      m_username                (""),
      m_description             (""),
      m_waveNodeObjectId        (),
      m_managedObjectId         ()
{
    setName (name);
}

DistributedLogEntryManagedObject::~DistributedLogEntryManagedObject ()
{
}

string DistributedLogEntryManagedObject::getClassName ()
{
    return ("DistributedLogEntryManagedObject");
}

void DistributedLogEntryManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();
    
    addPersistableAttribute (new AttributeUI64 (&m_logId, "logId"));
    addPersistableAttribute (new AttributeResourceId (&m_logType, "logType"));
    addPersistableAttribute (new AttributeResourceId (&m_logDescriptionType, "logDescriptionType"));
    addPersistableAttribute (new AttributeDateTime (&m_dateTimeStamp, "dateTimeStamp"));
    addPersistableAttribute (new AttributePrismServiceId (&m_prismServiceId, "prismServiceId"));
    addPersistableAttribute (new AttributeString (&m_username, "username"));
    addPersistableAttribute (new AttributeString (&m_description, "description"));
    addPersistableAttribute (new AttributeObjectId (&m_waveNodeObjectId, "waveNodeObjectId"));
    addPersistableAttribute (new AttributeObjectId (&m_managedObjectId, "managedObjectId"));
}

void DistributedLogEntryManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI64 (&m_logId, "logId"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_logType, "logType"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_logDescriptionType, "logDescriptionType"));
    addPersistableAttributeForCreate (new AttributeDateTime (&m_dateTimeStamp, "dateTimeStamp"));
    addPersistableAttributeForCreate (new AttributePrismServiceId (&m_prismServiceId, "prismServiceId"));
    addPersistableAttributeForCreate (new AttributeString (&m_username, "username"));
    addPersistableAttributeForCreate (new AttributeString (&m_description, "description"));
    addPersistableAttributeForCreate (new AttributeObjectId (&m_waveNodeObjectId, "waveNodeObjectId"));
    addPersistableAttributeForCreate (new AttributeObjectId (&m_managedObjectId, "managedObjectId"));
}

void DistributedLogEntryManagedObject::setupKeys ()
{
    vector<string> userDefinedKeycombination;

    userDefinedKeycombination.push_back ("logId");

    setUserDefinedKeyCombination (userDefinedKeycombination);
}

UI64 DistributedLogEntryManagedObject::getLogId () const
{
    return (m_logId);
}

void DistributedLogEntryManagedObject::setLogId (const UI64 &logId)
{
    m_logId = logId;
}

ResourceId DistributedLogEntryManagedObject::getLogType () const
{
    return (m_logType);
}

void DistributedLogEntryManagedObject::setLogType (const ResourceId &logType)
{
    m_logType = logType;
}

ResourceId DistributedLogEntryManagedObject::getLogDescriptionType () const
{
    return (m_logDescriptionType);
}

void DistributedLogEntryManagedObject::setLogDescriptionType (const ResourceId &logDescriptionType)
{
    m_logDescriptionType = logDescriptionType;
}

DateTime DistributedLogEntryManagedObject::getDateTimeStamp () const
{
    return (m_dateTimeStamp);
}

void DistributedLogEntryManagedObject::setDateTimeStamp (const DateTime &dateTimeStamp)
{
    m_dateTimeStamp = dateTimeStamp;
}

PrismServiceId DistributedLogEntryManagedObject::getPrismServiceId () const
{
    return (m_prismServiceId);
}

void DistributedLogEntryManagedObject::setPrismServiceId (const PrismServiceId &prismServiceId)
{
    m_prismServiceId = prismServiceId;
}

string DistributedLogEntryManagedObject::getUsername () const
{
    return (m_username);
}

void DistributedLogEntryManagedObject::setUsername (const string &username)
{
    m_username = username;
}

string DistributedLogEntryManagedObject::getDescription () const
{
    return (m_description);
}

void DistributedLogEntryManagedObject::setDescription (const string &description)
{
    m_description = description;
}

ObjectId DistributedLogEntryManagedObject::getWaveNodeObjectId () const
{
    return (m_waveNodeObjectId);
}

void DistributedLogEntryManagedObject::setWaveNodeObjectId (const ObjectId &waveNodeObjectId)
{
    m_waveNodeObjectId = waveNodeObjectId;
}

ObjectId DistributedLogEntryManagedObject::getManagedObjectId () const
{
    return (m_managedObjectId);
}

void DistributedLogEntryManagedObject::setManagedObjectId (const ObjectId &managedObjectId)
{
    m_managedObjectId = managedObjectId;
}

}

