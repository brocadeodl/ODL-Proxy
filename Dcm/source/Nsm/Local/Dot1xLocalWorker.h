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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef DOTXLOCALWORKER_H
#define DOTXLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmDot1xUpdateConfigSPMessage;
    class NsmDot1xUpdatePortConfigMessage;
	class DcmManagementInterfaceMessage;

    class Dot1xLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmDot1xUpdateConfigSPMessageHandler( NsmDot1xUpdateConfigSPMessage *pNsmDot1xUpdateConfigSPMessage);

            void NsmDot1xUpdatePortConfigMessageHandler( NsmDot1xUpdatePortConfigMessage *pNsmDot1xUpdatePortConfigMessage);
			ResourceId Dot1xUpdate(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId Dot1xUpdateMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);

        protected:
        public:
            Dot1xLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~Dot1xLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //DOTXLOCALWORKER_H
