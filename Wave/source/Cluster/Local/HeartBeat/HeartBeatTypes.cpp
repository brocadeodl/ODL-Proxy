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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Cluster/Local/HeartBeat/HeartBeatTypes.h"

namespace WaveNs
{

HeartBeatStat::HeartBeatStat ()
{
    m_heartBeatSent              = 0;
    m_heartBeatReplySent         = 0;
    m_lastHeartBeatReceived      = 0;
    m_heartBeatReceived          = 0;
    m_heartBeatLost              = 0;
    m_heartBeatLostInRow         = 0;
    m_heartBeatDelayed           = 0;
    m_heartBeatReplyReceived     = 0;
    m_lastHeartBeatReplyReceived = 0;
    m_heartBeatDuplicate         = 0;
}

HeartBeatStat::~HeartBeatStat ()
{
}

HeartBeatNodeInfo::HeartBeatNodeInfo ()
{
    m_heartBeatCurrentState  = 0;
    m_heartBeatFailureThreshold  = 0;
    m_heartBeatInterval  = 0;
    m_portNumber = 0;
    m_timerHandle = 0;
    m_waitingForHeartBeatReply = 0;
}

HeartBeatNodeInfo::~HeartBeatNodeInfo ()
{
}


}
