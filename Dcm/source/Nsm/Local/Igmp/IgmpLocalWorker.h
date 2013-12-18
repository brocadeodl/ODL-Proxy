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
 *   Author : agidwani                                                     *
 **************************************************************************/

#ifndef IGMPLOCALWORKER_H
#define IGMPLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

class NsmLocalObjectManager;
class IgmpPoSviIntfLocalConfigMessage;
class IgmpSgIntfLocalConfigMessage;
class IgmpPhyIntfLocalConfigMessage;

class IgmpLocalWorker: public WaveWorker
{
    private:
        PrismMessage *createMessageInstance(const UI32 &operationCode);
        WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        void IgmpPoSviIntfLocalConfigMessageHandler(IgmpPoSviIntfLocalConfigMessage *pIgmpPoSviIntfLocalConfigMessage);
        void IgmpPhyIntfLocalConfigMessageHandler(IgmpPhyIntfLocalConfigMessage *pIgmpPhyIntfLocalConfigMessage);
        void IgmpSgIntfLocalConfigMessageHandler(IgmpSgIntfLocalConfigMessage *pIgmpSgIntfLocalConfigMessage);
        ResourceId UpdateIgmpPoSviIntfLocalConfigStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId UpdateIgmpPoSviIntfLocalConfigMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId validateIgmpPhyIntfLocalConfigStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext); 
        ResourceId UpdateIgmpPhyIntfLocalConfigStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
        ResourceId UpdateIgmpPhyIntfLocalConfigMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

        ResourceId validateIgmpSgIntfLocalConfigStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
        ResourceId UpdateIgmpSgIntfLocalConfigStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId UpdateIgmpSgIntfLocalConfigMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

    	NsmL3ReadyManagedObject* PoMoGetL3ReadyMO(PoIntfManagedObject* poIntfMO,PrismSynchronousLinearSequencerContext *pContext);
		NsmL3ReadyManagedObject* PoIdGetL3ReadyMO(UI32 poId,const SI32 &mapId, PrismSynchronousLinearSequencerContext *pContext,bool fromGlobal);
		NsmL3ReadyManagedObject* PhyIfNameGetL3ReadyMO(string ifName,PrismSynchronousLinearSequencerContext  *pContext,bool fromLocal);
    protected:
    public:
        IgmpLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
        virtual    ~IgmpLocalWorker ();

// Now the data members

    private:
    protected:
    public:
};
}
#endif                                            //IGMPLOCALWORKER_H
