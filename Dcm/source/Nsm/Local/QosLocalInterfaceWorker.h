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

#ifndef QOSLOCALINTERFACEWORKER_H
#define QOSLOCALINTERFACEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmLocalQosPfcConfigMessage;
	class NsmQosPfcConfigSPMessage;
	class NsmQosCosTailDropConfigMessage;
	class QosCosProfileAndThresholdConfigMessage;
	class QosPoProfileAndThresholdSPMessage;

    class QosLocalInterfaceWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmLocalQosPfcConfigMessageHandler( NsmLocalQosPfcConfigMessage *pNsmLocalQosPfcConfigMessage);
            void NsmQosPfcConfigSPMessageHandler ( NsmQosPfcConfigSPMessage *pNsmQosPfcConfigSPMessage);
			void NsmQosCosTailDropConfigMessageHandler (NsmQosCosTailDropConfigMessage *pNsmQosCosTailDropConfigMessage);
			void QosCosProfileAndThresholdConfigMessageHandler (QosCosProfileAndThresholdConfigMessage *pQosCosProfileAndThresholdConfigMessage);
			void  QosPoProfileAndThresholdSPMessageHandler(QosPoProfileAndThresholdSPMessage *pQosConfigSPMessage);
			ResourceId updatePOQosProfAndThresIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosPfcIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosPfcIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updatePOQosPfcIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosCosThresholdIntfStep (PrismSynchronousLinearSequencerContext
					*pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosCosThresholdIntfMOStep (PrismSynchronousLinearSequencerContext
					*pPrismSynchronousLinearSequencerContext);
			ResourceId updateQosCosProfileIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
    		ResourceId updateQosCosProfileIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);    
        protected:
        public:
            QosLocalInterfaceWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~QosLocalInterfaceWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //QOSLOCALINTERFACEWORKER_H
