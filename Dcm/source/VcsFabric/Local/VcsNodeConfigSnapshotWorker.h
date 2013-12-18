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

/**
 *@file VcsNodeConfigSnapshotWorker.h 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              Worker class which is responsible for processing
 *              Create and Restore Snapshot request
 *         
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */
#ifndef VCSNODECONFIGSNAPSHOTWORKER_H
#define VCSNODECONFIGSNAPSHOTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
class VcsNodeConfigSnapshotMessage;
class VcsNodeLocalMessagingContext;

class VcsNodeConfigSnapshotWorker : public WaveWorker
{   
    private :
                                        VcsNodeConfigSnapshotWorker                         ( WaveObjectManager* pWaveObjectManager );
                    
                    PrismMessage       *createMessageInstance                               ( const UI32 &operationCode);

                    void                vcsNodeConfigSnapshotRestoreHandler                 (VcsNodeConfigSnapshotMessage *pVcsNodeConfigSnapshotMessage);
                    ResourceId          vcsNodeConfigSnapshotRestoreValidationStep          (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
                    ResourceId          copyDefaultToStartAndRebootIfNoCluster              ( VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext );
                    ResourceId          vcsNodeConfigSnapshotRestoreCommitTransactionStep   (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
                    ResourceId          disconnectPrincipalFromAllLocationStep              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
                    ResourceId          unconfigureThisNodeStep                             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
                    ResourceId          backupAllConfigSnapshotFiles                        ();
            
                    ResourceId          restoreAllBackedupConfigSnapshotFiles               ();

        static      void                setConfigSnapshotRestoreFlag                        (const bool &restoreFlag);

    protected :
    public :
        virtual                        ~VcsNodeConfigSnapshotWorker                         (); 

        static      bool                getConfigSnapshotRestoreFlag                        ();

                    // Now the data memebers

    private :
        static      bool                s_configSnapshotRestoreInProgressFlag;
        static      PrismMutex          s_configSnapshotRestoreInProgressFlagMutex;
    protected :
    public :

        friend class VcsFabricLocalObjectManager;

};

}
 
#endif // VCSNODECONFIGSNAPSHOTWORKER_H
