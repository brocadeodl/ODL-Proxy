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

#ifndef VRFLOCALOBJECTMANAGER_H
#define VRFLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "Vrf/Local/VrfLocalSequencerContext.h"

namespace DcmNs
{

class VrfLocalObjectManager : public WaveLocalObjectManager
{
    private :
        // sequencer methods.
        // Creation.
                ResourceId                      validateInputsForAddVrf                         (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      requestBackendDaemonToCreateVRFStep             (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      requestFrameworkToCreatePartitionManagedObject  (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      createVrfLocalManagedObjectStep                 (VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                                                VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      vrfMoStep(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      updateVrfMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                                                  VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      updateVrfAfIpv4Mo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                                                  VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      updateVrfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                                                       VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      updateVrfAfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                                                       VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);

        // Deletion 
                ResourceId                      validateInputsForDeleteVrf                      (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      requestBackendDaemonToDeleteVRFStep             (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      requestFrameworkToCleanupPartitionManagedObject (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
                ResourceId                      cleanupVrfManagedObject                         (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext);
    protected :
    public :
                                                VrfLocalObjectManager                           ();
        virtual                                ~VrfLocalObjectManager                           ();

        virtual PrismMessage                   *createMessageInstance                           (const UI32 &operationCode);
                WaveManagedObject              *createManagedObjectInstance                     (const string &managedClassName);
                void                            initialize                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            install                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            boot                                            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
//              void                            shutdown                                        (WaveAsynchronousContextForShutdownPhases *pWaveAsynchronousContextForShutdownPhases);
                void                            haboot                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			    void                            postboot                                        (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);

        static  string                          getServiceName                                  ();
        static  VrfLocalObjectManager          *getInstance                                     ();
        static  PrismServiceId                  getPrismServiceId                               ();

        // Message Handlers.
                void                            addVrfMessageHandler                            (VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage); 
                void                            deleteVrfMessageHandler                         (VrfLocalDeleteVrfMessage *pVrfLocalVrfPartitionMessage); 
                ResourceId                      requestDependentPluginsToCleanUpStaleEntrys(UI32 tag, const string &vrfName);
    // Now the data members

    private :
    protected :
    public :
};

}

#endif // VRFLOCALOBJECTMANAGER_H
