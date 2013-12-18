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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef MULTIPARTITIONOBJECTMANAGER_H
#define MULTIPARTITIONOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Cluster/MultiPartition/Global/MultiPartitionMessages.h"
#include "Cluster/MultiPartition/Global/MultiPartitionSequencerContext.h"

namespace WaveNs
{

class MultiPartitionObjectManager : public WaveObjectManager
{
    private :
                // sequencer methods.
                
                // ADD
                ResourceId                     validateInputsForAddPartition                    (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);
                ResourceId                     createPartitionManagedObjectStep                 (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);

                // DELETE
                ResourceId                     validateInputsForDeletePartition                 (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);
                ResourceId                     deletePartitionManagedObject                     (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);
                ResourceId                     triggerPartitionCleanupForLocalServicesStep      (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);
                ResourceId                     triggerPartitionCleanupForGlocalServicesStep     (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext);
                void                            getObjectIdForPartitionNameMessageHandler       (MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage *pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage);

    protected :
    public :
                                                MultiPartitionObjectManager                     ();
        virtual                                ~MultiPartitionObjectManager                     ();

        virtual PrismMessage                   *createMessageInstance                           (const UI32 &operationCode);
                WaveManagedObject              *createManagedObjectInstance                     (const string &managedClassName);
                void                            initialize                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            install                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            boot                                            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
//              void                            shutdown                                        (WaveAsynchronousContextForShutdownPhases *pWaveAsynchronousContextForShutdownPhases);
                void                            haboot                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                            failover                                        (FailoverAsynchronousContext *pFailoverAsynchronousContext);


        static  string                          getServiceName                                  ();
        static  MultiPartitionObjectManager    *getInstance                                     ();
        static  PrismServiceId                  getPrismServiceId                               ();

        // Message Handlers.
                void                            addPartitionMessageHandler                      (MultiPartitionAddPartitionMessage *pMultiPartitionAddPartitionMessage); 
                void                            deletePartitionMessageHandler                   (MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage); 
                ObjectId                        getObjectIdForPartitionName                     (const string &partitionName);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // MULTIPARTITIONOBJECTMANAGER_H
