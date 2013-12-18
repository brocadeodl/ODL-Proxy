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
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSMLOCALL3IPWORKER_H
#define NSMLOCALL3IPWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/Layer3/NsmLocalPhyIpMessage.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Layer3/NsmPhyIntfVrfConfigMessage.h"
#include "Nsm/Local/Layer3/NsmVeIntfVrfConfigMessage.h"
#include "Nsm/Local/Layer3/NsmLocalNodeIpMessage.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocalObjectManager;

    class NsmLocalL3IpWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            // deleting VRRP virtual IP within deleted subnet, if all is true all virtual IP are
            // removed, i.e. primary IP is removed
            void notifyVRRPIPDeletion(NsmL3ReadyManagedObject* l3ready, const string& ip, bool all= false, bool secondary=false);
            ResourceId validateVlanIntfIp(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validateVlanIpAddrConf(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doOverallSviMoStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doSviMo(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doSviIpAddrMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId deleteSviIpConfigMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updateSviIpConfigMo(NsmL3ReadyManagedObject *pL3ReadyMo, DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId loopbackIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validateLoIntfIp(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

        protected:
        public:
            NsmLocalL3IpWorker						(NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual ~NsmLocalL3IpWorker				();

            ResourceId validateLocalNodeIpStep		(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doLocalNodeIpStep			(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId doLocalNodeIpMoStep			(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            void NsmLocalNodeIpMessageHandler		(NsmLocalNodeIpMessage *pMessage);
            void NsmLocalPhyIpMessageHandler		(NsmLocalPhyIpMessage *pMessage);
            void NsmPhyIntfVrfConfigMessageHandler  (NsmPhyIntfVrfConfigMessage *pMessage);
            void NsmVeIntfVrfConfigMessageHandler   (NsmVeIntfVrfConfigMessage *pMessage);

            ResourceId doPhyIntfIpAddrMoStep		(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doPhyIntfIpMoStep			(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId doPhyIntfIpStep				(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validatePhyIntfIpStep		(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validatePhyIntfIpAddrStep	(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updatePhyIntfIpConfigMoStep	(DceSynchronousLinearSeqContext *pContext);
            ResourceId deletePhyIntfIpConfigMoStep	(DceSynchronousLinearSeqContext *pContext);
            ResourceId vrfConfigUpdateMOStep        (DceSynchronousLinearSeqContext *pContext);
            ResourceId validateVRFConfigStep        (DceSynchronousLinearSeqContext *pContext);
            ResourceId vrfConfigUpdateBackendStep   (DceSynchronousLinearSeqContext *pContext);
            ResourceId veVrfConfigUpdateMOStep      (DceSynchronousLinearSeqContext *pContext);
            ResourceId validateVeVRFConfigStep      (DceSynchronousLinearSeqContext *pContext);
            ResourceId veVrfConfigUpdateBackendStep (DceSynchronousLinearSeqContext *pContext);
            ResourceId updateErrorCodeStep           (DceSynchronousLinearSeqContext *pContext); 
//            string     getVrfNameByObjectId         (const ObjectId objid);
// Now the data members

        private:
        protected:
        public:
    };

}
#endif                                            // NSMLOCALL3IPWORKER_H
