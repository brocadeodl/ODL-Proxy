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

#ifndef WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGER_H
#define WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

namespace WaveNs
{

class WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage;
class WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage;

class WaveMessagingBrokerConnectionObjectManager : public WaveLocalObjectManager
{
    private :
        bool isAKnownWaveMessagingBroker                   (const string &waveMesagingBrokerName);
        void requestToTryForConnectionMessageHandler       (WaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage       *pWaveMessagingBrokerConnectionObjectManagerRequestToTryForConnectionMessage);
        void cancelRequestToTryForConnectionMessageHandler (WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage *pWaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage);

        void startTimerIfNeeded                            ();

        void timerCallback                                 (TimerHandle timeHandle, void *pContext);

        void removeWaveMessagingBrokerNameFromPendingList  (const string &waveMessagingBrokerName);

    protected :
    public :
                                                            WaveMessagingBrokerConnectionObjectManager ();
        virtual                                            ~WaveMessagingBrokerConnectionObjectManager ();

        static  string                                      getServiceName                             ();
        static  PrismServiceId                              getPrismServiceId                          ();
        static  WaveMessagingBrokerConnectionObjectManager *getInstance                                ();

    // Now the data members

    private :
        map<string, string> m_pendingWaveMessagingBrokerNamesToTryForConnection;
        UI32                m_tryIntervalInMilliSeconds;
        TimerHandle         m_timerHandle;

    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGER_H
