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
 *   Author : bgangise		                                               *
 **************************************************************************/

#ifndef APPMLOCALSHOWWORKER_H
#define APPMLOCALSHOWWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMLocalObjectManager;
    class APPMLocalShowMessage;

    class APPMLocalShowWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void APPMLocalShowMessageHandler(APPMLocalShowMessage *pAPPMLocalShowMessage);
			ResourceId showMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void showMessageClusterStepCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			ResourceId showMessageClusterStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);

        protected:
        public:
            APPMLocalShowWorker ( APPMLocalObjectManager *pAPPMLocalObjectManager);
			virtual    ~APPMLocalShowWorker ();

		// Now the data members
		private:
		protected:
		public:
    };
}
#endif//APPMLOCALCONFWORKER_H
