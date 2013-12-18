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

#ifndef HEARTBEATTYPES_H
#define HEARTBEATTYPES_H

#include "Framework/ObjectModel/PrismElement.h"

namespace WaveNs
{

#define                 DEFAULT_HEART_BEAT_PERIOD          10000
#define                 DEFAULT_MAX_LOST_HEARTBEATS        3

typedef enum
{
    HEARTBEAT_START = 0,
    HEARTBEAT_STOP,
    HEARTBEAT_PAUSE,
    HEARTBEAT_RESUME,
    HEARTBEAT_CONFIG,
    HEARTBEAT_REPORT,
    HEARTBEAT_GET_STAT,
    HEARTBEAT_SHOW_STAT,
    HEARTBEAT_LOST,
    HEARTBEAT_DISCONNECT_FROM_NODE

} HeartBeatOpCodes;

class HeartBeatStat
{
    private :
    protected :
    public :
        HeartBeatStat ();
        ~HeartBeatStat ();
         // Now the data members

    private :
    protected :
    public :

        UI32                      m_heartBeatSent;
        UI32                      m_heartBeatReplySent;
        
        UI32                      m_heartBeatReceived;
        UI32                      m_heartBeatReplyReceived;
        UI32                      m_lastHeartBeatReceived;
        UI32                      m_lastHeartBeatReplyReceived;
        
        UI32                      m_heartBeatLost;
        UI32                      m_heartBeatLostInRow;
        
        UI32                      m_heartBeatDelayed;
        UI32                      m_heartBeatDuplicate;

};

class HeartBeatNodeInfo
{
    private :
        HeartBeatNodeInfo ();

    protected :
    public :
        ~HeartBeatNodeInfo ();
         // Now the data members

    private :
        IpV4Address               m_ipAddress;
        UI16                      m_portNumber;
        UI8                       m_heartBeatCurrentState;
        UI32                      m_heartBeatInterval;
        UI32                      m_heartBeatFailureThreshold;
        UI8                       m_waitingForHeartBeatReply;
        TimerHandle               m_timerHandle;
        HeartBeatStat             m_heartBeatStat;


    protected :
    public :
        friend class HeartBeatObjectManager;
};

}

#endif //HEARTBEATTYPES_H
