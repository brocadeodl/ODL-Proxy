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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#ifndef POLICERGLOBALWORKER_H
#define POLICERGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Utils/DceLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalObjectManager;
    class PolicerGlobalPriorityMapMessage;
    class PolicerGlobalClassMapMessage;
    class PolicerGlobalPolicyClassMapMessage;

    class PolicerGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void PolicerGlobalClassMapMessageHandler( PolicerGlobalClassMapMessage *pPolicerGlobalClassMapMessage);
            void PolicerGlobalPolicyClassMapMessageHandler( PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage);
            void PolicerGlobalPriorityMapMessageHandler( PolicerGlobalPriorityMapMessage *pPolicerGlobalPriorityMapMessage);
			void PriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void PriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
        	void ClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void ClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteReferenceToClassMap(ObjectId classMapObjId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void deleteReferenceToPriorityMap(ObjectId prioMapObjId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void PolicyMapValidationStep(DceLinearSeqContext *pPrismLinearSequencerContext);
            void PolicyMapStep(DceLinearSeqContext *pPrismLinearSequencerContext);
            void cleanupServicePolicyMapReferenceInFailedOverNodes(DceLinearSeqContext *pPrismLinearSequencerContext);
            void PolicyMapMOStep(DceLinearSeqContext *pPrismLinearSequencerContext);
			ObjectId getIdFromPriorityMapName(string Name, PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ObjectId getIdFromClassMapName(string Name, PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void PolicerGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
			void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
		protected:
        public:
            PolicerGlobalWorker ( PolicerGlobalObjectManager *pPolicerGlobalObjectManager);
            virtual    ~PolicerGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //POLICERGLOBALWORKER_H
