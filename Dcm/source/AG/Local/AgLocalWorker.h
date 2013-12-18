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
 *   Author : rhkumar/ranantha                                             *
 **************************************************************************/

#ifndef AGLOCALWORKERWORKER_H
#define AGLOCALWORKERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgLocalShowMessage.h"
#include "AG/Local/AgPolicyMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class AgLocalObjectManager;
    class AgLocalConfigMessage;
    class AgPGFnmtovMessage;
    class AgRelCounterMessage;

    class AgLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void AgLocalConfigMessageHandler( AgLocalConfigMessage *pAgLocalConfigMessage);
            void AgLocalShowMessageHandler(AgLocalShowMessage *pAgLocalShowMessage);
            void AgPolicyMessageHandler(AgPolicyMessage *pAgPolicyMessage);

            ResourceId sendShowCommandToAgDaemon(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendEnableStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	        ResourceId rebootLocalNodeStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	        ResourceId rebootLocalNode (const bool &isStandbyRebootOnly);
            ResourceId validationForEnableStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateFnmtovMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendRelCounterStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void AgRelCounterMessageHandler( AgRelCounterMessage *pRelCounterMessage);

            ResourceId updateRelCounterMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendFnmtovStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            void AgPGFnmtovMessageHandler( AgPGFnmtovMessage *pAgPGFnmtovMessage);


            ResourceId validatePolicyStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendPolicyStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePolicyMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

	
        protected:
        public:
            AgLocalWorker ( AgLocalObjectManager *pAgLocalObjectManager);
            virtual    ~AgLocalWorker ();

    	    static ResourceId convert_agerror_to_dcmderror(int agerror);

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //AGLOCALWORKERWORKER_H
