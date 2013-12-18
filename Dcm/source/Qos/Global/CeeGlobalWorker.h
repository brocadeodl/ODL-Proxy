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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef CEEGLOBALWORKER_H
#define CEEGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class CeeGlobalCreateCeeMapMessage;
    class CeeGlobalDeleteCeeMapMessage;
    class CeeGlobalCreatePriorityGroupMessage;
    class CeeGlobalDeletePriorityGroupMessage;
    class CeeGlobalCreatePriorityMapMessage;
    class CeeGlobalDeletePriorityMapMessage;
	class CeeGlobalCreateCeeRemapMessage;
	class PriorityGroupManagedObject;
	class CeeMapManagedObject;

    class CeeGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

			void CeeGlobalCreateCeeRemapMessageHandler (CeeGlobalCreateCeeRemapMessage *pCeeGlobalCreateCeeRemapMessage);
            void createCeeRemapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createCeeRemapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void CeeGlobalCreateCeeMapMessageHandler( CeeGlobalCreateCeeMapMessage *pCeeGlobalCreateCeeMapMessage);
			void createCeeMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
		    void createCeeMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void CeeGlobalCreatePriorityGroupMessageHandler( CeeGlobalCreatePriorityGroupMessage *pCeeGlobalCreatePriorityGroupMessage);
			void createPriorityGroupValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createPriorityGroupStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void createPriorityGroupMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void CeeGlobalCreatePriorityMapMessageHandler( CeeGlobalCreatePriorityMapMessage *pCeeGlobalCreatePriorityMapMessage);
            void createPriorityMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createPriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void createPriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void CeeGlobalDeleteCeeMapMessageHandler( CeeGlobalDeleteCeeMapMessage *pCeeGlobalDeleteCeeMapMessage);
            void deleteCeeMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deleteCeeMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void CeeGlobalDeletePriorityGroupMessageHandler( CeeGlobalDeletePriorityGroupMessage *pCeeGlobalDeletePriorityGroupMessage);
            void deletePriorityGroupValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deletePriorityGroupStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deletePriorityGroupMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void CeeGlobalDeletePriorityMapMessageHandler( CeeGlobalDeletePriorityMapMessage *pCeeGlobalDeletePriorityMapMessage);
            void deletePriorityMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deletePriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void deletePriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void QosGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
		    ResourceId vcsQosSanity(SI32 oper, string mapName, int reservePriorityBitmap);
		    WaveManagedObjectPointer<PriorityGroupManagedObject>* findPriorityGroupByName(vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &pgid_vector, string pgIdName);
			void configReplayEnd(WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
            void configReplayPriorityMapMessageHandler(WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
            void configReplaySendPrirityMapMsg(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void configReplaySendPrirityGroupMsg1(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void configReplaySendPrirityGroupMsg2(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId validateRemapPriorityValue(SI32 remapPriority);
        protected:
        public:
            CeeGlobalWorker ( QosGlobalObjectManager *pQosGlobalObjectManager);
            virtual    ~CeeGlobalWorker ();

            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //CEEGLOBALWORKER_H
