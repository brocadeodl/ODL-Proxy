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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPLOCALCONFWORKER_H
#define VRRPLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmUpdateVRRPLocalConfigMessage;
    class NsmUpdateVRRPLocalConfigPhyIntfMessage;
    class VRRPLocalShowMessage;

    class VRRPLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmUpdateVRRPLocalConfigMessageHandler(NsmUpdateVRRPLocalConfigMessage *pNsmUpdateVRRPLocalConfigMessage);
            void NsmUpdateVRRPLocalConfigPhyIntfMessageHandler(NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage);
            ResourceId validateVRRPLocalConfigCommon (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext, NsmL3ReadyManagedObject *&pL3ReadyMo, UI32 &choice, UI32 &op_type);
            template <class VRRPSessionConfigLocalManagedObjectClass,
                      WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObjectClass> (NsmL3ReadyManagedObject::*pGetVRRPIntfSessionFunc)(UI8 vrid)>
                ResourceId validateVRRPLocalConfigStepTemplate (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validateVRRPLocalConfigIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validateVRRPLocalConfigStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId validateVRRPELocalConfigStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId createVRRPLocalConfigPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId createVRRPLocalConfigPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId VRRPLocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId VRRPELocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId VRRPVCSLocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
            ResourceId updateVRRPLocalConfigSessionMO (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext, UI32 choice);
			int mapBackEndErrorsToDcmdErrors(int err);
            void VRRPLocalShowMessageHandler(VRRPLocalShowMessage *pVRRPLocalShowMessage);
			ResourceId sendShowToVRRPDaemon (DceSynchronousLinearSeqContext *ctx_p);

        protected:
        public:
            VRRPLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~VRRPLocalConfWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //VRRPLOCALCONFWORKER_H
