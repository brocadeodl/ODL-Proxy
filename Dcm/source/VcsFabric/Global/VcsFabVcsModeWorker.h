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
 *@file VcsFabVcsModeWorker.h
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file declares the
 *              Worker class which is responsible for installing
 *              and updating the global VcsModeManagedObject.
 *
 * Author :     Brian Adaniya
 * Date :       12/13/2010
 */

#ifndef VCSFABVCSMODEWORKER_H
#define VCSFABVCSMODEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabObjectManager;
class VcsModeLinearSequencerContext;

class VcsFabVcsModeWorker : public WaveWorker
{
    private:
                void                getVcsModeFromQueriedVcsNodeLocalManagedObjectStep  (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext);
                void                createVcsModeManagedObjectStep                      (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext);
                void                updateVcsModeManagedObjectStep                      (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext);
                
    protected:
    public:
                                    VcsFabVcsModeWorker                                 (VcsFabObjectManager *pVcsFabObjectManager);
        virtual                    ~VcsFabVcsModeWorker                                 ();

                WaveManagedObject  *createManagedObjectInstance                         (const string &managedClassName);
                void                install                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                dbInconsistencyCheck                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    // Now for the data members
    private:
    protected:
    public:
};

}

#endif //VCSFABVCSMODEWORKER_H
