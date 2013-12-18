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

#ifndef WAVEMESSAGINGBROKERCLIENT_H
#define WAVEMESSAGINGBROKERCLIENT_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class ClientStreamingSocket;
class WaveBrokerPublishMessage;
class WaveBrokerBasedMessagePayload;

class WaveMessagingBrokerClient
{
    private :
    protected :
    public :
                    WaveMessagingBrokerClient       (const string &name, const string &ipAddress, const SI32 &port);
                   ~WaveMessagingBrokerClient       ();

        string      getName                         () const;
        void        setName                         (const string &name);

        string      getIpAddress                    () const;
        void        setIpAddress                    (const string &ipAddress);

        SI32        getPort                         () const;
        void        setPort                         (const SI32 &port);

        bool        isAKnownSubscriptionTopic       (const string &subscriptionTopic) const;
        void        addSubscriptionTopic            (const string &subscriptionTopic);
        void        removeSubscriptionTopic         (const string &subscriptionTopic);
        void        removeAllSubscriptionTopics     ();

        string      getUniqueString                 () const;

        ResourceId  connect                         (const UI32 &numberOfRetries = 0, const UI32 &maximumNumberOfSecondsToTryFor = 0);
        void        indicateClosing                 ();

        void        checkout                        ();
        void        checkin                         ();

        UI32        getNumberOfOutstandingCheckouts ();

        bool        operator <<                     (WaveBrokerPublishMessage      *pWaveBrokerPublishMessage);
        bool        operator <<                     (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload);

        void        getAllSubscribedTopics          (vector<string> &allSubscribedTopics);


        // Now the data members

    private :
        string                 m_name;
        string                 m_ipAddress;
        SI32                   m_port;
        map<string, string>    m_subscriptionTopics;
        ClientStreamingSocket *m_pClientStreamingSocket;
        PrismMutex             m_mutex;
        UI32                   m_numberOfOutstandingCheckouts;


    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCLIENT_H
