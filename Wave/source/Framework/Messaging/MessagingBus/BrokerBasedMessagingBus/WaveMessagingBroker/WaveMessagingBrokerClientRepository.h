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

#ifndef WAVEMESSAGINGBROKERCLIENTREPOSITORY_H
#define WAVEMESSAGINGBROKERCLIENTREPOSITORY_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveMessagingBrokerClient;

class WaveMessagingBrokerClientRepository
{
    private :
        WaveMessagingBrokerClientRepository ();

    protected :
    public :
                                                   ~WaveMessagingBrokerClientRepository         ();

        static string                               getWaveMessagingBrokerClientUniqueString    (const string &ipAddress, const SI32 &port);
               bool                                 isAKnownWaveMessagingBrokerClient           (const string &ipAddress, const SI32 &port);
               void                                 addWaveMessagingBrokerClient                (const string &name, const string &ipAddress, const SI32 &port);
               bool                                 addWaveMessagingBrokerClientIfNotKnown      (const string &name, const string &ipAddress, const SI32 &port);
               void                                 removeWaveMessagingBrokerClient             (const string &ipAddress, const SI32 &port);
               void                                 addWaveMessagingBrokerClientSubscription    (const string &ipAddress, const SI32 &port, const string &subscriptionTopic);
               void                                 removeWaveMessagingBrokerClientSubscription (const string &ipAddress, const SI32 &port, const string &subscriptionTopic);

               WaveMessagingBrokerClient           *checkoutWaveMessagingBrokerClient           (const string &ipAddress, const SI32 &port);
               WaveMessagingBrokerClient           *checkoutWaveMessagingBrokerClient           (const string &waveMessagingBrokerClientUniqueString);
               void                                 checkinWaveMessagingBrokerClient            (WaveMessagingBrokerClient *pWaveMessagingBrokerClient);

        static WaveMessagingBrokerClientRepository *getInstance                                 ();

               void                                 print                                       ();

    // Now the data members

    private :
        map<string, WaveMessagingBrokerClient *> m_waveMessagingBrokerClientsMap;

        PrismMutex                               m_waveMessagingBrokerClientsMutex;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCLIENTREPOSITORY_H
