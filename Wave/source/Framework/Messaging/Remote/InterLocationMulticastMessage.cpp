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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Messaging/Remote/InterLocationTypes.h"
#include "Framework/Messaging/Remote/InterLocationMulticastMessage.h"
#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include <vector>
#include <string>
#include <map>

namespace WaveNs
{
InterLocationMulticastMessage::InterLocationMulticastMessage ()
    : PrismMessage (InterLocationMessageTransportObjectManager::getPrismServiceId (), INTERLOCATION_MESSAGE_FOR_MULTICAST_SEND),
    m_numberOfMessagesSent (0)
{
}

InterLocationMulticastMessage::~InterLocationMulticastMessage ()
{
}

string &InterLocationMulticastMessage::getSerializedStringToSend ()
{
    return (m_serializeString);
}

void InterLocationMulticastMessage::setSerializedStringToSend (const string &serializedString)
{
    m_serializeString = serializedString;
}

ResourceId InterLocationMulticastMessage::getStatusForALocation (LocationId &locationId)
{
    lockAccess ();

    ResourceId status = m_locationStatus [locationId];

    unlockAccess ();

    return (status);
}

void InterLocationMulticastMessage::setStatusForALocation (LocationId &locationId, ResourceId &locationStatus)
{
    lockAccess ();
 
    m_locationStatus [locationId] = locationStatus;

    unlockAccess ();
}

void InterLocationMulticastMessage::getLocationsToSend (set<LocationId> &locationsId)
{
    locationsId = m_locationsId;
}

void InterLocationMulticastMessage::setLocationsToSend (set<LocationId> &locationsId)
{
    m_locationsId = locationsId;
}

InterLocationMulticastMessage &InterLocationMulticastMessage::operator ++ ()
{
    lockAccess ();

    ++m_numberOfMessagesSent;

    unlockAccess ();

    return (*this);

} 

InterLocationMulticastMessage &InterLocationMulticastMessage::operator -- ()
{
    lockAccess ();

    --m_numberOfMessagesSent;

    unlockAccess ();

    return (*this);
}

void InterLocationMulticastMessage::setMessageIdForMessageToMulticast (const UI32 &messageId)
{
    m_originalMessageId = messageId;
}

UI32 InterLocationMulticastMessage::getMessageIdForMessageToMulticast ()
{
    return (m_originalMessageId);
}

void InterLocationMulticastMessage::lockAccess ()
{
    m_accessMutex.lock ();
}

void InterLocationMulticastMessage::unlockAccess ()
{
    m_accessMutex.unlock ();
}

bool InterLocationMulticastMessage::areAllMessageReplyReceived ()
{
    lockAccess ();

    if (0 == m_numberOfMessagesSent)
    {
        unlockAccess ();
        return (true);
    }

    unlockAccess ();
    return (false);
}

void InterLocationMulticastMessage::messageOperationAccess ()
{
    m_messageAccessMutex.lock ();
}

void InterLocationMulticastMessage::messageOperationReleaseAccess ()
{
    m_messageAccessMutex.unlock ();
}

bool InterLocationMulticastMessage::isMessageSentToThisLocation (const LocationId &locationId)
{
    set<LocationId>::iterator it;

    it = m_locationsId.find (locationId);

    if (it != m_locationsId.end ())
    {
        return (true);
    }

    return (false);
}

bool InterLocationMulticastMessage::isMessageSentToThisLocationNotReplied (const LocationId &locationId) const
{
    map<LocationId, LocationId>::const_iterator element    = m_locationReplied.find (locationId);
    map<LocationId, LocationId>::const_iterator endElement = m_locationReplied.end ();

    if (element == endElement)
    {
        return (true);
    }

    return (false);
}

void InterLocationMulticastMessage::setMessageRepliedToThisLocation (const LocationId &locationId)
{
    map<LocationId, LocationId>::iterator element    = m_locationReplied.find (locationId);
    map<LocationId, LocationId>::iterator endElement = m_locationReplied.end ();

    if (element == endElement)
    {
        m_locationReplied [locationId] = locationId;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

}

