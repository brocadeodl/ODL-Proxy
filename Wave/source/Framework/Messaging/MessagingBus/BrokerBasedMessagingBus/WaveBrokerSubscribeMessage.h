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

#ifndef WAVEBROKERSUBSCRIBEMESSAGE_H
#define WAVEBROKERSUBSCRIBEMESSAGE_H

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessage.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class WaveBrokerSubscribeMessage : public WaveBrokerBasedMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                        WaveBrokerSubscribeMessage ();
                                        WaveBrokerSubscribeMessage (const vector<string> &topicNames);
                                        WaveBrokerSubscribeMessage (const string &topicName);
        virtual                        ~WaveBrokerSubscribeMessage ();

                vector<string>          getTopicNames              () const;
                void                    setTopicNames              (const vector<string> &topicNames);
                void                    addTopicNames              (const vector<string> &topicNames);

        static  WaveBrokerBasedMessage *createMessageInstance      ();

    // now the data members

    private :
        vector<string> m_topicNames;

    protected :
    public :
};

}

#endif // WAVEBROKERSUBSCRIBEMESSAGE_H
