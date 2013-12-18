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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef HEARTBEATOBJECTMANAGER_H
#define HEARTBEATOBJECTMANAGER_H

#include "App/AppInterface.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatDebug.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include <string>
#include <vector>

namespace WaveNs
{
class HeartBeatNodeInfo;
class HeartBeatConfigLocalManagedObject;

class HeartBeatObjectManager : public WaveLocalObjectManager
{
    private :
                                        HeartBeatObjectManager      ();
        virtual PrismMessage           *createMessageInstance       (const UI32 &operationCode);
                WaveManagedObject      *createManagedObjectInstance (const string &managedClassName);
                void                    setThisLocationIpAddr       (const string &thisLocationIpAddr);
    protected :
    public :
        virtual                        ~HeartBeatObjectManager      ();
        static  HeartBeatObjectManager *getInstance                 ();
        static  PrismServiceId          getPrismServiceId           ();

                void                    initialize                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                    install                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                    shutdown                    (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

                void                    startHeartBeat              (StartHeartBeatMessage *pMessage);
                void                    stopHeartBeat               (StopHeartBeatMessage *pMessage);
                void                    pauseHeartBeat              (PauseHeartBeatMessage *pMessage);
                void                    resumeHeartBeat             (ResumeHeartBeatMessage *pMessage);
                void                    configHeartBeat             (ConfigHeartBeatMessage *pMessage);
                void                    reportHeartBeat             (ReportHeartBeatMessage *pMessage);
                void                    processHeartBeatReply       (FrameworkStatus frameworkStatus, ReportHeartBeatMessage *pMessage, HeartBeatNodeInfo *pHeartBeatInfo);
                void                    heartBeatTimerHandler       (TimerHandle timerHandle, HeartBeatNodeInfo *pHeartBeatInfo);
                void                    sendHeartBeat               (HeartBeatNodeInfo *pHeartBeatInfo);

                void                    getHeartBeatStat            (GetHeartBeatStatMessage *pMessage);
                void                    showHeartBeatStat           (ShowHeartBeatStatMessage *pMessage);
                void                    showStatistic               (ShowHeartBeatStatMessage *pMessage);

                void                    disconnectFromNodeMessageHandler    (DisconnectFromNodeMessage *pDisconnectFromNodeMessage);
                void                    validateDisconnectFromNodeRequest   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                    processDisconnectFromNodeMessage    (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                
                
                HeartBeatConfigLocalManagedObject *getConfigManagedObjectInstance (); 

        // Now the data members

    private :
                IpV4Address                  m_thisLocationIpAddr;
                UI16                         m_thisLocationPort;
                vector <HeartBeatNodeInfo *> m_heartBeatNodes;
                HeartBeatDebug               m_heartBeatDebug;


    protected :
    public :

    friend class LocalClusterConfigObjectManager;
};

}

#endif // HEARTBEATBJECTMANAGER_H
