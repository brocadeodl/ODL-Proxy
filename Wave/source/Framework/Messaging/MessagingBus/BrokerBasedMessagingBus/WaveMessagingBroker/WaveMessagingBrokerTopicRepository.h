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

#ifndef WAVEMESSAGINGBROKERTOPICREPOSITORY_H
#define WAVEMESSAGINGBROKERTOPICREPOSITORY_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveMessagingBrokerTopic;

class WaveMessagingBrokerTopicRepository
{
    private :
        WaveMessagingBrokerTopicRepository ();

    protected :
    public :
                                                   ~WaveMessagingBrokerTopicRepository    ();

                bool                                isAKnownWaveMessagingBrokerTopic      (const string &waveMessagingBrokerTopicName);
                void                                addWaveMessagingBrokerTopic           (const string &waveMessagingBrokerTopicName, const string &clientUniqueString);
                void                                addWaveMessagingBrokerTopicIfNotKnown (const string &waveMessagingBrokerTopicName, const string &clientUniqueString);
                void                                removeWaveMessagingBrokerTopic        (const string &waveMessagingBrokerTopicName, const string &clientUniqueString);
                void                                removeWaveMessagingBrokerTopicIfKnown (const string &waveMessagingBrokerTopicName, const string &clientUniqueString);
                void                                getCurrentlySubscribedClientsForTopic (const string &waveMessagingBrokerTopicName, vector<string> &currentlySubscribedClients);

        static  WaveMessagingBrokerTopicRepository *getInstance                           ();

    // Now the data members

    private :
        map<string, WaveMessagingBrokerTopic *> m_waveMesasgingBrokerTopics;
        PrismMutex                              m_waveMesasgingBrokerTopicsMutex;

    protected :
    public :
};
}

#endif // WAVEMESSAGINGBROKERTOPICREPOSITORY_H
