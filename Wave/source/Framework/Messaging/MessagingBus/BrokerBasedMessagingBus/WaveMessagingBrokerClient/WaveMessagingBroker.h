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

#ifndef WAVEMESSAGINGBROKER_H
#define WAVEMESSAGINGBROKER_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>

using namespace std;

namespace WaveNs
{

class ClientStreamingSocket;
class WaveBrokerSubscribeMessage;
class WaveBrokerPublishMessage;

class WaveMessagingBroker
{
    private :
        void postCurrentSubscriptions ();
        void addServiceSubscription   (const string &topicName, const PrismServiceId &prismServiceId);

    protected :
    public :
                           WaveMessagingBroker                        (const string &name, const string &ipAddress, const SI32 &port);
                          ~WaveMessagingBroker                        ();

               string      getName                                    () const;
               void        setName                                    (const string &name);

               string      getIpAddress                               () const;
               void        setIpAddress                               (const string &ipAddress);

               SI32        getPort                                    () const;
               void        setPort                                    (const SI32 &port);

               string      getUniqueString                            () const;

               ResourceId  connect                                    (const UI32 &numberOfRetries = 0, const UI32 &maximumNumberOfSecondsToTryFor = 0);
               void        indicateClosing                            ();

               void        checkout                                   ();
               void        checkin                                    ();

               UI32        getNumberOfOutstandingCheckouts            ();

               void        invalidateConnection                       ();
               bool        isCurrentlyConnected                       ();

        static void        requestToTryForConnection                  (const string &waveMessagingBrokerName);

               bool        operator <<                                (WaveBrokerSubscribeMessage *pWaveBrokerSubscribeMessage);
               bool        operator <<                                (WaveBrokerPublishMessage   *pWaveBrokerPublishMessage);

               void        getCurrentlySubscribedServicesForTopicName (const string &topicName, vector<PrismServiceId> &currentlySubscribedServices);

        // Now the data members

    private :
        string                                             m_name;
        string                                             m_ipAddress;
        SI32                                               m_port;
        ClientStreamingSocket                             *m_pClientStreamingSocket;
        PrismMutex                                         m_mutex;
        UI32                                               m_numberOfOutstandingCheckouts;
        map<string, string>                                m_currentSubscritptions;
        map<string, map<PrismServiceId, PrismServiceId> >  m_currentlySubscribedServicesByTopicName;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKER_H
