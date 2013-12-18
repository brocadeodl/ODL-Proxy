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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef LLDPGLOBALCONFWORKER_H
#define LLDPGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpGlobalObjectManager;
    class LldpGlobalScalarsConfSetMessage;
    class LldpGlobalProfileConfSetMessage;
    class LldpGlobalProfileGetObjectIdMessage;

    class LldpGlobalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void LldpGlobalProfileConfSetMessageHandler( LldpGlobalProfileConfSetMessage *pLldpGlobalProfileConfSetMessage);
			void lldpGlobalProfileGetObjectId(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void LldpGlobalProfileGetObjectIdMessageHandler( LldpGlobalProfileGetObjectIdMessage *pLldpGlobalProfileGetObjectIdMessage);
            void LldpGlobalScalarsConfSetMessageHandler( LldpGlobalScalarsConfSetMessage *pLldpGlobalScalarsConfSetMessage);
            void updateLldpGlobalScalarsMO (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateLldpGlobalProfileMO (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateLldpGlobalScalarsConf (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateLldpGlobalProfileConf (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void LldpGlobalScalarsConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
			void LldpGlobalProfileConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
			void lldpGlobalProfileValidateCreateSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			int LldpGlobalDeleteAllProfiles();
			void copyMasterMoToProfileMo (PrismLinearSequencerContext *pPrismLinearSequencerContext,
					LldpGlobalProfileConfManagedObject *m);
  			void updateNonUserProfileConfigs (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void lldpGlobalProfileValidateCreate (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        protected:
        public:
            LldpGlobalConfWorker ( LldpGlobalObjectManager *pLldpGlobalObjectManager);
            virtual    ~LldpGlobalConfWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LLDPGLOBALCONFWORKER_H
