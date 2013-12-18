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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Cluster/MultiPartition/Global/MultiPartitionMessages.h"
#include "Cluster/MultiPartition/Global/MultiPartitionTypes.h"
#include "Cluster/MultiPartition/Global/MultiPartitionObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

MultiPartitionAddPartitionMessage::MultiPartitionAddPartitionMessage ()
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_ADD_PARTITION),
    m_partitionName            (""),
    m_partitionLocationId     (FrameworkToolKit::getThisLocationId ())
{
}

MultiPartitionAddPartitionMessage::MultiPartitionAddPartitionMessage (const string &partitionName)
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_ADD_PARTITION),
    m_partitionName            (partitionName),
    m_partitionLocationId     (FrameworkToolKit::getThisLocationId ())
{
}

MultiPartitionAddPartitionMessage::MultiPartitionAddPartitionMessage (const string &partitionName, const LocationId &partitionLocationId)
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_ADD_PARTITION),
    m_partitionName            (partitionName),
    m_partitionLocationId     (partitionLocationId)
{
}

MultiPartitionAddPartitionMessage::~MultiPartitionAddPartitionMessage ()
{
}

void MultiPartitionAddPartitionMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeString      (&m_partitionName,                          "partitionName"));
     addSerializableAttribute (new AttributeLocationId  (&m_partitionLocationId,                    "partitionLocationId"));
     addSerializableAttribute (new AttributeObjectId    (&m_createdWavePartitionManagedObjectId,    "createdWavePartitionManagedObjectId"));
}

string MultiPartitionAddPartitionMessage::getPartitionName(void) const
{
    return m_partitionName;
}

void   MultiPartitionAddPartitionMessage::setPartitionName(const string &partitionName)
{
    m_partitionName = partitionName;
}

LocationId MultiPartitionAddPartitionMessage::getPartitionLocationId(void) const 
{
    return m_partitionLocationId;
}

void   MultiPartitionAddPartitionMessage::setPartitionLocationId(const LocationId &partitionLocationId)
{
    m_partitionLocationId = partitionLocationId;
}

void   MultiPartitionAddPartitionMessage::setCreatedWavePartitionManagedObjectId  (const ObjectId &createdWavePartitionManagedObjectId)
{
    m_createdWavePartitionManagedObjectId = createdWavePartitionManagedObjectId;
}

ObjectId       MultiPartitionAddPartitionMessage::getCreatedWavePartitionManagedObjectId (void) const
{
    return (m_createdWavePartitionManagedObjectId);
}



//
// DeletePartition Message.
//

MultiPartitionDeletePartitionMessage::MultiPartitionDeletePartitionMessage ()
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_DELETE_PARTITION),
    m_partitionName         (""),
    m_senderServiceId       (0),
    m_partitionLocationId   (FrameworkToolKit::getThisLocationId ()),
    m_isPartialCleanup      (false),
    m_partialCleanupTag     (FRAMEWORK_ERROR)
{
}

MultiPartitionDeletePartitionMessage::MultiPartitionDeletePartitionMessage (const string &partitionName )
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_DELETE_PARTITION),
    m_partitionName         (partitionName),
    m_senderServiceId       (0),
    m_partitionLocationId   (FrameworkToolKit::getThisLocationId ()),
    m_isPartialCleanup      (false),
    m_partialCleanupTag     (FRAMEWORK_ERROR)
{
}

MultiPartitionDeletePartitionMessage::MultiPartitionDeletePartitionMessage (const string &partitionName, const LocationId &partitionLocationId)
    : ManagementInterfaceMessage (MultiPartitionObjectManager::getServiceName (), MULTI_PARTITION_DELETE_PARTITION),
    m_partitionName         (partitionName),
    m_senderServiceId       (0),
    m_partitionLocationId   (partitionLocationId),
    m_isPartialCleanup      (false),
    m_partialCleanupTag     (FRAMEWORK_ERROR)
{
}

MultiPartitionDeletePartitionMessage::~MultiPartitionDeletePartitionMessage ()
{
}

void MultiPartitionDeletePartitionMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeString      (&m_partitionName,              "partitionName"));
     addSerializableAttribute (new AttributeUI32        (&m_senderServiceId,            "senderServiceId"));
     addSerializableAttribute (new AttributeLocationId  (&m_partitionLocationId,        "partitionLocationId"));
     addSerializableAttribute (new AttributeBool        (&m_isPartialCleanup,           "isPartialCleanup"));
     addSerializableAttribute (new AttributeResourceId  (&m_partialCleanupTag,          "partialCleanupTag"));

}

string MultiPartitionDeletePartitionMessage::getPartitionName(void) const
{
    return m_partitionName;
}

void   MultiPartitionDeletePartitionMessage::setPartitionName(const string &partitionName)
{
    m_partitionName = partitionName;
}

LocationId MultiPartitionDeletePartitionMessage::getPartitionLocationId(void) const 
{
    return m_partitionLocationId;
}

void   MultiPartitionDeletePartitionMessage::setPartitionLocationId(const LocationId &partitionLocationId)
{
    m_partitionLocationId = partitionLocationId;
}


void    MultiPartitionDeletePartitionMessage::setSenderServiceId(const UI32 &senderServiceId)
{
    m_senderServiceId = senderServiceId;
}

UI32    MultiPartitionDeletePartitionMessage::getSenderServiceId(void) const  
{
    return m_senderServiceId;
}

bool MultiPartitionDeletePartitionMessage::getIsPartialCleanup () const
{
    return m_isPartialCleanup;
}

void MultiPartitionDeletePartitionMessage::setPartialCleanupTag (const ResourceId &partialCleanupTag)
{
    m_isPartialCleanup  = true;

    m_partialCleanupTag = partialCleanupTag;
}

ResourceId MultiPartitionDeletePartitionMessage::getPartialCleanupTag ()
{
    prismAssert (true == m_isPartialCleanup, __FILE__, __LINE__);

    return m_partialCleanupTag;
}


// Get Object Id for a Partition Name.

MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage ()
    : PrismMessage              (MultiPartitionObjectManager::getPrismServiceId (), MULTI_PARTITION_GET_OBJECTID_FOR_PARTITION_NAME)
{
}

MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage (const string &partitionName)
    : PrismMessage              (MultiPartitionObjectManager::getPrismServiceId (), MULTI_PARTITION_GET_OBJECTID_FOR_PARTITION_NAME),
      m_partitionName           (partitionName)
{
}

MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::~MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage ()
{
}

void MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString       (&m_partitionName,     "partitionName"));
     addSerializableAttribute (new AttributeObjectId     (&m_partitionObjectId, "partitionObjectId"));
}

string MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::getPartitionName () const
{
    return m_partitionName;
}

void MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::setPartitionName (const string &partitionName)
{
    m_partitionName = partitionName;
}

ObjectId MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::getPartitionObjectId () const
{
    return m_partitionObjectId;
}

void MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage::setPartitionObjectId (const ObjectId &partitionObjectId)
{
    m_partitionObjectId = partitionObjectId;
}

}

