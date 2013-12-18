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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef WAVEDEBUGINFORMATIONWORKER_H
#define WAVEDEBUGINFORMATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class WaveObjectManagerGetDebugInformationMessage;
class WaveObjectManagerResetDebugInformationMessage;
class WaveDebugInformationContext;
class WaveAsynchronousContextForDebugInformation;
class WaveObjectManager;

class WaveDebugInformationWorker : public WaveWorker
{
    private :
                PrismMessage   *createMessageInstance                           (const UI32 &operationCode);

                void            getDebugInformationHandler                      (WaveObjectManagerGetDebugInformationMessage *pWaveObjectManagerGetDebugInformationMessage);
                void            getDebugInformationWorkersStep                  (WaveDebugInformationContext *pWaveDebugInformationContext);
                void            getDebugInformationWorkersStepCallback          (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);
                void            getDebugInformationObjectManagerStep            (WaveDebugInformationContext *pWaveDebugInformationContext);
                void            getDebugInformationObjectManagerStepCallback    (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);

                void            resetDebugInformationHandler                    (WaveObjectManagerResetDebugInformationMessage *pWaveObjectManagerResetDebugInformationMessage);
                void            resetDebugInformationWorkersStep                (WaveDebugInformationContext *pWaveDebugInformationContext);
                void            resetDebugInformationWorkersStepCallback        (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);
                void            resetDebugInformationObjectManagerStep          (WaveDebugInformationContext *pWaveDebugInformationContext);
                void            resetDebugInformationObjectManagerStepCallback  (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation);

    protected :
    public :
                                WaveDebugInformationWorker                      (WaveObjectManager *pWaveObjectManger);
        virtual                ~WaveDebugInformationWorker                      ();

    // Now the data members

    private :
    protected :
    public :
};

}
#endif // WAVEDEBUGINFORMATIONWORKER_H

