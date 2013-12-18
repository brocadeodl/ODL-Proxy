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

#ifndef POLICERLOCALWORKER_H
#define POLICERLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerLocalObjectManager;
    class PolicerPriorityMapMessage;
    class PolicerClassMapMessage;
    class PolicerPolicyClassMapMessage;

    class PolicerLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void PolicerClassMapMessageHandler( PolicerClassMapMessage *pPolicerClassMapMessage);
            void PolicerPolicyClassMapMessageHandler( PolicerPolicyClassMapMessage *pPolicerPolicyClassMapMessage);
            void PolicerPriorityMapMessageHandler( PolicerPriorityMapMessage *pPolicerPriorityMapMessage);
			ResourceId PriorityMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId ClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId PolicyMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status); 
       protected:
        public:
            PolicerLocalWorker ( PolicerLocalObjectManager *pPolicerLocalObjectManager);
            virtual    ~PolicerLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //POLICERLOCALWORKER_H
