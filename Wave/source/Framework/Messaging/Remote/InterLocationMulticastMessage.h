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

#ifndef INTERLOCATION_MULTICAST_MESSAGE_H
#define INTERLOCATION_MULTICAST_MESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/PrismMutex.h"
#include <vector>
#include <set>
#include <string>
#include <map>

using namespace std; 

namespace WaveNs
{

class InterLocationMulticastMessage : public PrismMessage
{
 public:
    
                                             InterLocationMulticastMessage              ();
    virtual                                 ~InterLocationMulticastMessage              ();

            string                          &getSerializedStringToSend                  ();
            void                             setSerializedStringToSend                  (const string &serializedString);

            ResourceId                       getStatusForALocation                      (LocationId &locationId);
            void                             setStatusForALocation                      (LocationId &locationId, ResourceId &locationStatus);

            void                             getLocationsToSend                         (set<LocationId> &locationsId);
            void                             setLocationsToSend                         (set<LocationId> &locationsId);
            InterLocationMulticastMessage   &operator ++                                ();
            InterLocationMulticastMessage   &operator --                                ();
            void                             setMessageIdForMessageToMulticast          (const UI32 &messageId);
            UI32                             getMessageIdForMessageToMulticast          ();
            bool                             areAllMessageReplyReceived                 ();
            
            void                             messageOperationAccess                     ();
            void                             messageOperationReleaseAccess              ();

            bool                             isMessageSentToThisLocation                (const LocationId &locationId);
            bool                             isMessageSentToThisLocationNotReplied      (const LocationId &locationId) const;
            void                             setMessageRepliedToThisLocation            (const LocationId &locationId);
 protected:
 private:
            void                             lockAccess                                 ();
            void                             unlockAccess                               ();

 public:
 protected:
 private:

    string                          m_serializeString;
    map<LocationId, ResourceId>     m_locationStatus;
    map<LocationId, LocationId>     m_locationReplied;
    set<LocationId>                 m_locationsId;
    UI32                            m_numberOfMessagesSent;
    UI32                            m_originalMessageId;
    PrismMutex                      m_accessMutex;
    PrismMutex                      m_messageAccessMutex;
};

}

#endif //INTERLOCATION_MULTICAST_MESSAGE_H
