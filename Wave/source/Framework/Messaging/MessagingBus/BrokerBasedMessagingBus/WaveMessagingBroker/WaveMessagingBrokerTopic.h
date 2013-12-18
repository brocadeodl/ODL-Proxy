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

#ifndef WAVEMESSAGINGBROKERTOPIC_H
#define WAVEMESSAGINGBROKERTOPIC_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveMessagingBrokerClient;

class WaveMessagingBrokerTopic
{
    private :
    protected :
    public :
              WaveMessagingBrokerTopic     (const string &name);
             ~WaveMessagingBrokerTopic     ();

        bool  isAKnownSubscription         (const string &ipAddress, const SI32 &port) const;
        void  addSubscription              (const string &ipAddress, const SI32 &port);
        void  addSubscription              (const string &uniqueString);
        void  removeSubscription           (const string &ipAddress, const SI32 &port);
        void  removeSubscription           (const string &uniqueString);
        void  getSubscribedClients         (vector<string> &subscribedClients) const;
        UI32  getNumberOfSubscribedClients () const;

    // Now the data members

    private :
        string              m_name;
        map<string, string> m_subscribedClients;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERTOPIC_H
