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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef HEARTBEATFAILUREAGENT_H
#define HEARTBEATFAILUREAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class HeartBeatFailureAgentContext;

class HeartBeatFailureAgent : protected WaveWorker
{
    private :
    protected :
    public :
                            HeartBeatFailureAgent                (WaveObjectManager *pWaveObjectManager, IpV4Address ipAddress, UI16 portNumber);
        virtual            ~HeartBeatFailureAgent                ();
        virtual ResourceId  execute                              ();

                ResourceId  getListOfEnabledServicesStep         (HeartBeatFailureAgentContext *pHeartBeatFailureAgentContext);
                ResourceId  sendHeartBeatFailureStep             (HeartBeatFailureAgentContext *pHeartBeatFailureAgentContext);

                bool        requiresHeartBeatFailureNotification (const PrismServiceId &prismServiceId);

    // Now the data members

    private :
		IpV4Address m_ipAddress;
		UI16        m_portNumber;
    protected :
    public :
};

}

#endif // HeartBeatFailureAgent_H
