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

#ifndef LOCALCLUSTERCONFIGOBJECTMANAGER_H
#define LOCALCLUSTERCONFIGOBJECTMANAGER_H

#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/NodeManagedObject.h"
#include "App/AppInterface.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"

namespace WaveNs
{

class LocalClusterConfigObjectManager : public WaveLocalObjectManager
{
    private :
                    LocalClusterConfigObjectManager ();

    protected :
    public :
        virtual                                 ~LocalClusterConfigObjectManager    ();
        static  LocalClusterConfigObjectManager *getInstance                        ();
        static  PrismServiceId                   getPrismServiceId                  ();

                void                             reportPrimaryNodeChanged           (LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage *pMessage);
                void                             reportRemovedNodeFromCluster       (LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage *pMessage);

                void                             sendLocalHeartBeat                 ();
                void                             showStatistic                      ();
                void                             getNodeRoleHandler                 (LocalClusterConfigObjectManagerGetNodeRoleMessage *pMessage);
                void                             getClusterConfigHandler            (LocalClusterConfigObjectManagerGetClusterConfigMessage *pMessage);
                UI32                             sendRemoteCommandReply             (void *pContext, UI32 resultLen, void *resultPtr);
                UI32                             informFrameworkLostHearBeat        (string ipAddress, UI32 port);
                void                             processFrameworkHeartBeatLostReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage, void *pContext);

                void                             reportLostHeartBeat                (HeartBeatLostMessage *pMessage);
        static  int                              getSwitchHaConfig                  (haNodeStatus_t *primaryNodes, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes);
    // Now the data members

    private :
    protected :
    public :

};

}

#endif // LOCALCLUSTERCONFIGOBJECTMANAGER_H
