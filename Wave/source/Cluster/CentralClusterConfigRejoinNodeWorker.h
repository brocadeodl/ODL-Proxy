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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                   *
 ***************************************************************************/

#ifndef CENTRALCLUSTERCONFIGREJOINNODEWORKER_H
#define CENTRALCLUSTERCONFIGREJOINNODEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{
class CentralClusterConfigObjectManager;
class ClusterObjectManagerRejoinNodeMessage;
class FrameworkObjectManagerRejoinNodesToClusterMessage;
class ClusterRejoinContext;

class CentralClusterConfigRejoinNodeWorker : public WaveWorker
{
    private :
                      CentralClusterConfigRejoinNodeWorker        (CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager);

virtual PrismMessage *createMessageInstance                       (const UI32 &operationCode);

        void          rejoinNodeMessageHandler                    (ClusterObjectManagerRejoinNodeMessage *pClusterObjectManagerRejoinNodeMessage);
        void          rejoinNodeValidateStep                      (ClusterRejoinContext *pClusterRejoinContext);
        void          rejoinNodeRequestFrameworkToRejoinNodeStep  (ClusterRejoinContext *pClusterRejoinContext);
        void          rejoinNodeRequestFrameworkToRejoinNodeCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinNodesToClusterMessage *  pFrameworkObjectManagerRejoinNodesToClusterMessage, void *pContext);
        void          rejoinNodeCommitStep                        (ClusterRejoinContext *pClusterRejoinContext);
        void          rejoinNodeStartHeartBeatsStep               (ClusterRejoinContext *pClusterRejoinContext);
    protected :
    public :

    virtual    ~CentralClusterConfigRejoinNodeWorker ();

    // Now the data members

    private :
    protected :
    public :

    friend class CentralClusterConfigObjectManager;
};

}

#endif // CENTRALCLUSTERCONFIGREJOINNODEWORKER_H
