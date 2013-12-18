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
 *   Author : kverma, bgangise, asharma                                    *
 **************************************************************************/

#ifndef APPMLOCALCONFWORKER_H
#define APPMLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMLocalObjectManager;
    class APPMLocalAPPMProfileMessage;
    class APPMLocalProfileDomainMessage;
    class APPMLocalAPPMVlanMessage;
    class APPMLocalAPPMQosMessage;
    class APPMLocalAPPMFcoeMessage;
    class APPMLocalAPPMSecurityMessage;
    class DcmManagementInterfaceMessage;
	class APPMLocalAPPMProfileBulkMessage;

    class APPMLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void APPMLocalAPPMFcoeMessageHandler( APPMLocalAPPMFcoeMessage *pAPPMLocalAPPMFcoeMessage);
            void APPMLocalAPPMProfileMessageHandler( APPMLocalAPPMProfileMessage *pAPPMLocalAPPMProfileMessage);
            void APPMLocalProfileDomainMessageHandler( APPMLocalProfileDomainMessage* pAPPMLocalProfileDomainMessage);
            void APPMLocalAPPMQosMessageHandler( APPMLocalAPPMQosMessage *pAPPMLocalAPPMQosMessage);
			void APPMLocalAPPMSecurityMessageHandler( APPMLocalAPPMSecurityMessage *pAPPMLocalAPPMSecurityMessage);
			void APPMLocalAPPMVlanMessageHandler( APPMLocalAPPMVlanMessage *pAPPMLocalAPPMVlanMessage);
			void APPMLocalAPPMProfileBulkMessageHandler( APPMLocalAPPMProfileBulkMessage *pAPPMLocalAPPMProfileBulkMessage);

			ResourceId profileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId portProfileDomainSendToDaemonStep(PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext);
			ResourceId profileBulkMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId QosProfileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId secProfileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId vlanMessageStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId fcoeProfileMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId mapNsmErrorToDcmdError(int rc);
            ResourceId mapSsmErrorToDcmdError(int rc);
            ResourceId AppmNsmWyserEaReturn(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status);
            ResourceId AppmSsmWyserEaReturn(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status);
        protected:
        public:
            APPMLocalConfWorker ( APPMLocalObjectManager *pAPPMLocalObjectManager);
            virtual    ~APPMLocalConfWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //APPMLOCALCONFWORKER_H
