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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CENTRALCLUSTERCONFIGDELETECLUSTERWORKER_H
#define CENTRALCLUSTERCONFIGDELETECLUSTERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class FrameworkObjectManagerDestroyClusterMessage;
class ClusterDeleteContext;
class ClusterObjectManagerDeleteClusterMessage;

class CentralClusterConfigDeleteClusterWorker : public WaveWorker
{
    private :
                      CentralClusterConfigDeleteClusterWorker              (WaveObjectManager *pWaveObjectManager);

        PrismMessage *createMessageInstance                                (const UI32 &operationCode);

        void          deleteClusterMessageHandler                          (ClusterObjectManagerDeleteClusterMessage *pClusterObjectManagerDeleteClusterMessage);
        void          deleteClusterValidateStep                            (ClusterDeleteContext *pClusterDeleteContext);
        void          deleteClusterStopHeartBeatsStep                      (ClusterDeleteContext *pClusterDeleteContext);
        void          deleteClusterRequestFrameworkToDeleteClusterStep     (ClusterDeleteContext *pClusterDeleteContext);
        void          deleteClusterRequestFrameworkToDeleteClusterCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage, void *pContext);
        void          deleteClusterCommitStep                              (ClusterDeleteContext *pClusterDeleteContext);

    protected :
    public :
        ~CentralClusterConfigDeleteClusterWorker ();

    // now the data members

    private :
    protected :
    public :

    friend class CentralClusterConfigObjectManager;
};

}

#endif // CENTRALCLUSTERCONFIGDELETECLUSTERWORKER_H
