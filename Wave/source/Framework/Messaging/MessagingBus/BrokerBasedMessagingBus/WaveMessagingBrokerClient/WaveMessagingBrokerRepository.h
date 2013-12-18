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

#ifndef WAVEMESSAGINGBROKERREPOSITORY_H
#define WAVEMESSAGINGBROKERREPOSITORY_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>

using namespace std;

namespace WaveNs
{

class WaveMessagingBroker;
class WaveBrokerPublishMessage;

class WaveMessagingBrokerRepository
{
    private :
        WaveMessagingBrokerRepository ();

    protected :
    public :
                                             ~WaveMessagingBrokerRepository       ();

        static WaveMessagingBrokerRepository *getInstance                         ();

        static string                         getWaveMessagingBrokerUniqueString  (const string &ipAddress, const SI32 &port);
               bool                           isAKnownBrokerByName                (const string &name);
               bool                           isAKnownBrokerByUniqueString        (const string &uniqueString);
               bool                           isAKnownBroker                      (const string& name, const string &ipAddress, const SI32 &port);
               bool                           isAKnownBroker                      (const string &ipAddress, const SI32 &port);
               bool                           addBrokerIfNotAlreadyKnown          (const string &name, const string &ipAddress, const SI32 &port);
               void                           addBroker                           (const string &name, const string &ipAddress, const SI32 &port);
               void                           removeBroker                        (const string &name);
               WaveMessagingBroker           *checkoutBroker                      (const string &name);
               WaveMessagingBroker           *checkoutBroker                      (const string &ipAddress, const SI32 &port);
               void                           checkinBroker                       (WaveMessagingBroker *pWaveMessagingBroker);
               string                         getBrokerName                       (const string &ipAddress, const SI32 &port);

        static WaveMessageBrokerStatus        subscribeToMessageBroker            (const string &brokerName, const vector<string> &topicNames);
        static WaveMessageBrokerStatus        subscribeToMessageBroker            (const string &brokerName, const string &topicName);
        static WaveMessageBrokerStatus        unsubscribeToMessageBroker          (const string &brokerName, const vector<string> &topicNames);
        static WaveMessageBrokerStatus        unsubscribeToMessageBroker          (const string &brokerName, const string &topicName);
        static WaveMessageBrokerStatus        publishToMessageBroker              (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage);


    // Now the data members

    private :
        map<string, WaveMessagingBroker *> m_waveMessagingBrokersMapByName;
        map<string, WaveMessagingBroker *> m_waveMessagingBrokersMapByUniqueString;
        PrismMutex                         m_waveMessagingBrokersMutex;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERREPOSITORY_H
