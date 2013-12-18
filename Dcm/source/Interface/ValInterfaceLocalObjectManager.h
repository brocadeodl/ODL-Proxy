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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VALINTERFACELOCALOBJECTMANAGER_H
#define VALINTERFACELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Interface/WyserEaAsyncDemoLinearSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{

class BootCompleteForThisLocationEvent;
class ValInterfaceLocalObjectManagerConfigureInterfaceMessage;
class ValInterfaceLocalObjectManagerGetInterfaceMessage;
class ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage;
class InterfacePriorityConfigureInterfacePriorityMessage;
class InterfacePriorityGetInterfacePriorityMessage;
class InterfaceClassConfigureInterfaceClassMessage;
class InterfaceClassGetInterfaceClassMessage;
class InterfaceNodeSpecificConfigureMessage;
class InterfaceContainerInListConfigureMessage;
class InterfaceContainerInListGetMessage;
class InterfaceDataTypeConfigureInterfaceDataTypeMessage;
class InterfaceDataTypeGetInterfaceDataTypeMessage;
class ValInterfaceConfigureVlanMessage;
class InterfaceInstallDemoObjectMessage;
class WyserEaAsyncDemoMessage;

class ValInterfaceLocalObjectManager : public WaveLocalObjectManager
{
    private :
                                    ValInterfaceLocalObjectManager ();

        virtual WaveManagedObject *createManagedObjectInstance             (const string &managedClassName);
        virtual PrismMessage      *createMessageInstance                   (const UI32 &operationCode);

                void               listenForEvents                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void               install                                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void               postboot                                (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);

                void               bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

                void               configureInterfaceMessageHandler        (ValInterfaceLocalObjectManagerConfigureInterfaceMessage *pValInterfaceLocalObjectManagerConfigureInterfaceMessage);
                void               getInterfaceMessageHandler              (ValInterfaceLocalObjectManagerGetInterfaceMessage       *pValInterfaceLocalObjectManagerGetInterfaceMessage);
                void                configureInterfaceChildrenMessageHandler (ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage *pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage);
                void                configureInterfaceAssociationVlanMessageHandler (ValInterfaceConfigureVlanMessage *pValInterfaceConfigureVlanMessage);
                void               InterfacePriorityConfigureInterfacePriorityMessageHandler(InterfacePriorityConfigureInterfacePriorityMessage *pInterfacePriorityConfigureInterfacePriorityMessage);
                void               InterfacePriorityGetInterfacePriorityMessageHandler(InterfacePriorityGetInterfacePriorityMessage *pInterfacePriorityGetInterfacePriorityMessage);
                void               InterfaceClassConfigureInterfaceClassMessageHandler(InterfaceClassConfigureInterfaceClassMessage *pInterfaceClassConfigureInterfaceClassMessage);
                void               InterfaceClassGetInterfaceClassMessageHandler(InterfaceClassGetInterfaceClassMessage *pInterfaceClassGetInterfaceClassMessage);
                void               InterfaceNodeSpecificConfigureMessageHandler(InterfaceNodeSpecificConfigureMessage *pInterfaceNodeSpecificConfigureMessage);
                void               InterfaceContainerInListConfigureMessageHandler(InterfaceContainerInListConfigureMessage *pInterfaceContainerInListConfigureMessage);
                void               InterfaceContainerInListGetMessageHandler(InterfaceContainerInListGetMessage *pInterfaceContainerInListGetMessage);
                void               InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler(InterfaceDataTypeConfigureInterfaceDataTypeMessage *pInterfaceDataTypeConfigureInterfaceDataTypeMessage);
                void               InterfaceDataTypeGetInterfaceDataTypeMessageHandler(InterfaceDataTypeGetInterfaceDataTypeMessage *pInterfaceDataTypeGetInterfaceDataTypeMessage);
                void               InstallDemoObjectMessageHandler (InterfaceInstallDemoObjectMessage *message);
                void               configReplayEnd                         (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);

                void               WyserEaAsyncDemoMessageHandler            (WyserEaAsyncDemoMessage *pWyserEaAsyncDemoMessage);
                void               WyserEaAsyncDemoMessageMultipleReplyStep  (WyserEaAsyncDemoLinearSequencerContext *pWyserEaAsyncDemoLinearSequencerContext);
                void               WyserEaAsyncDemoMessageTimerCallback      (TimerHandle timerHandle, void *pContext);

    protected :
    public :
        virtual ~ValInterfaceLocalObjectManager                    ();

        static   string                          getClassName      ();
        static   ValInterfaceLocalObjectManager *getInstance       ();
        static   PrismServiceId                  getPrismServiceId ();

    // Now the Data Members

    private :
    protected :
    public :
};

inline string ValInterfaceLocalObjectManager::getClassName ()
{
    return ("DCM Example for Interface Managed Object");
}

}

#endif // VALINTERFACELOCALOBJECTMANAGER_H
