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
 *   Author : cgangwar                                                     *
 ***************************************************************************/

#ifndef ALIASCONFIGOBJECTMANAGER_H
#define ALIASCONFIGOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "AliasConfig/Global/AliasConfigObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"

using namespace WaveNs;

namespace DcmNs
{
class WyserEaCliCommandAliasMessage;
class WyserEaAliasUserMessage;

class AliasConfigObjectManager : public WaveObjectManager
{
    private :
                                                AliasConfigObjectManager            ();
        virtual     PrismMessage               *createMessageInstance               (const UI32 &operationCode);
        virtual     WaveManagedObject          *createManagedObjectInstance         (const string &managedClassName);

                      // Alias Config Message Handler.
                    void                        AliasValidationStep                 (PrismLinearSequencerContext *ctx_p);
                    void                        WyserEaCliCommandAliasMessageHandler  (WyserEaCliCommandAliasMessage *msg_p);
                    void                        WyserEaCliCommandAliasMOStep          (PrismLinearSequencerContext *ctx_p);
                    void                        WyserEaUserAliasMessageHandler        (WyserEaAliasUserMessage *msg_p);
                    void                        WyserEaAliasUserMOStep                (PrismLinearSequencerContext *ctx_p);
                    void                        AaaCacheReload                      (TimerHandle &timerHandle);
                    void                        AAACleanCacheTimerStep              (PrismLinearSequencerContext *ctx_p);
                    void                        UserValidationStep                  (PrismLinearSequencerContext *ctx_p);

                    void                        WyserEaCliCommandGlobalAliasMessageHandler  (WyserEaCliCommandAliasMessage *msg_p);
                    void                        WyserEaCliCommandGlobalAliasMOStep          (PrismLinearSequencerContext *ctx_p);
                    void                        nodeAddedEventHandler                     (const WaveNewNodesAddedEvent* &pEvent);

    protected :
    public :
        virtual                                ~AliasConfigObjectManager            ();

        static      string                      getClassName                        ();
        static      AliasConfigObjectManager   *getInstance                         ();
        static      PrismServiceId              getPrismServiceId                   ();
                    void                        listenForEvents                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        // Now the data members

    private :
    protected :
    public :
    
        TimerHandle                 m_cleanCacheTimerHandle;
 
        static const UI32                       m_cleanCacheTimerInterval;
                     bool                       m_startTimer;
                     UI32                       m_aliasType;

};

}

#endif // ALIASCONFIGOBJECTMANAGER_H
