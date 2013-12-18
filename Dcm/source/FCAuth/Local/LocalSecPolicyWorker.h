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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef LOCALSECPOLICYWORKER_H
#define LOCALSECPOLICYWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"        
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "FCAuth/Local/DefinedPolicyLocalManagedObject.h"
#include "FCAuth/Local/DefinedPolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/ActivePolicyLocalManagedObject.h"
#include "FCAuth/Local/ActivePolicyMemberLocalManagedObject.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/ConfigReplayCompletedEvent.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalObjectManager;
    class FCAuthLocalAddPolicyMemberMessage;
    class FCAuthLocalDeletePolicyMemberMessage;
    class FCAuthLocalActivatePolicyMessage;
    class LocalSecPolicyFileReplayEndMessage;

    class LocalSecPolicyWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void FCAuthLocalActivatePolicyMessageHandler( FCAuthLocalActivatePolicyMessage *pFCAuthLocalActivatePolicyMessage);
            void FCAuthLocalAddPolicyMemberMessageHandler( FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage);
            void FCAuthLocalDeletePolicyMemberMessageHandler( FCAuthLocalDeletePolicyMemberMessage *pFCAuthLocalDeletePolicyMemberMessage);
			void LocalSecPolicyFileReplayEndMessageHandler (LocalSecPolicyFileReplayEndMessage *pLocalSecPolicyFileReplayEndMessage);
			void LocalSecPolicyFileReplayEndMessageCallback (WaveSendToClusterContext *pWaveSendToClusterContext);

			DefinedPolicyLocalManagedObject *queryDefinedPolicyLocalManagedObjectByName(string policyName);
			ActivePolicyLocalManagedObject *queryActivePolicyLocalManagedObjectByName(string policyName);
			ResourceId secpolicyActivatePlugin();
			ResourceId secPolicyDefinedAddPlugin(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage);
			ResourceId secPolicyActiveAddPlugin(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage);
			ResourceId secPolicyDeletePlugin(FCAuthLocalDeletePolicyMemberMessage *pFCAuthLocalDeletePolicyMemberMessage);
			ResourceId secPolicyDefinedImport(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage);
			ResourceId secPolicyActiveImport(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage);
			ResourceId SecpolicyRespToStatus(UI32 respCode);
			void configImport();
			void configReplayCompletedEventHandler (const ConfigReplayCompletedEvent* &pEvent);
			void fileReplayEnd (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
			void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        protected:
        public:
            LocalSecPolicyWorker ( FCAuthLocalObjectManager *pFCAuthLocalObjectManager);
            virtual    ~LocalSecPolicyWorker ();
			int isSupportedPlatform();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LOCALSECPOLICYWORKER_H
