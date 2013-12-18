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

#include "Framework/DistributedLog/Test/DistributedLogTestMessages.h"
#include "Framework/DistributedLog/Test/DistributedLogTestObjectManager.h"
#include "Framework/DistributedLog/DistributedLogTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

DistributedLogInvokeAddLogApiMessage::DistributedLogInvokeAddLogApiMessage ()
    : PrismMessage (DistributedLogTestObjectManager::getPrismServiceId (), DISTRIBUTED_LOG_INVOKE_ADD_LOG_API),
    m_logType               (),
    m_logDescriptionType    (),
    m_managedObjectId       (),
    m_numberOfLogsToAdd     (1)
{
}

DistributedLogInvokeAddLogApiMessage::~DistributedLogInvokeAddLogApiMessage ()
{
}

void DistributedLogInvokeAddLogApiMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeResourceId (&m_logType,             "logType"));
     addSerializableAttribute (new AttributeResourceId (&m_logDescriptionType,  "logDescriptionType"));
     addSerializableAttribute (new AttributeObjectId (&m_managedObjectId,       "managedObjectId"));
     addSerializableAttribute (new AttributeUI32 (&m_numberOfLogsToAdd,          "numberOfLogsToAdd"));
}

ResourceId DistributedLogInvokeAddLogApiMessage::getLogType ()
{
    return (m_logType);
}

void DistributedLogInvokeAddLogApiMessage::setLogType (ResourceId logType)
{
    m_logType = logType;
}

ResourceId DistributedLogInvokeAddLogApiMessage::getLogDescriptionType ()
{
    return (m_logDescriptionType);
}

void DistributedLogInvokeAddLogApiMessage::setLogDescriptionType (ResourceId logDescriptionType)
{
    m_logDescriptionType = logDescriptionType;
}

ObjectId DistributedLogInvokeAddLogApiMessage::getManagedObjectId ()
{
    return (m_managedObjectId);
}

void DistributedLogInvokeAddLogApiMessage::setManagedObjectId (ObjectId managedObjectId)
{
    m_managedObjectId = managedObjectId;
}

UI32 DistributedLogInvokeAddLogApiMessage::getNumberOfLogsToAdd ()
{
    return (m_numberOfLogsToAdd);
}

void DistributedLogInvokeAddLogApiMessage::setNumberOfLogsToAdd (UI32 numberOfLogsToAdd)
{
    m_numberOfLogsToAdd = numberOfLogsToAdd;
}

}
