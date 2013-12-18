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
 *   Author : justinh                                                     *
 **************************************************************************/

#ifndef NSMGLOBALL3IPWORKER_H
#define NSMGLOBALL3IPWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

#include "Nsm/Global/Layer3/NsmGlobalVlanIpMessage.h"
#include "Nsm/Global/Layer3/NsmGlobalPoIpMessage.h"

#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"

#include "Nsm/Local/Layer3/NsmL3Util.h"

#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmGlobalL3IpWorker: public WaveWorker
    {
        private:
    		friend class DcmManagedObjectAnnouncer;

            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void NsmGlobalPoIpMessageHandler(NsmGlobalPoIpMessage *pMessage);

            ResourceId validatePoIntfIpStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId validatePoIntfIpAddrStep(NsmL3ReadyManagedObject *pNsmL3ReadyMo, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId doPoIntfIpStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId doPoIntfIpAddrMoStep(NsmL3ReadyManagedObject *pNsmL3ReadyMo, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId doPoIntfIpMoStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            ResourceId doNodePoMoStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            ResourceId doPoIntfSwitchportMoStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            ResourceId updatePoIntfIpConfigMoStep(NsmL3ReadyManagedObject *pL3ReadyMo,
                PrismSynchronousLinearSequencerContext *pContext);
            ResourceId deletePoIntfIpConfigMoStep(NsmL3ReadyManagedObject *pL3ReadyMo,
                PrismSynchronousLinearSequencerContext *pContext);
            void notifyVRRPIPDeletion(NsmL3ReadyManagedObject* l3ready, const string& ip, bool all = false);

        protected:
        public:
            NsmGlobalL3IpWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~NsmGlobalL3IpWorker ();


// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //NSMGLOBALL3IPWORKER_H
