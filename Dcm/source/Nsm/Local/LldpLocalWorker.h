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
 *   Author : sanjeevj                                                     *
 **************************************************************************/

#ifndef LLDPLOCALWORKER_H
#define LLDPLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmLldpIntfConfSetMessage;
    class NsmLldpInternalMessage;

    class LldpLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmLldpIntfConfSetMessageHandler( NsmLldpIntfConfSetMessage *pNsmLldpIntfConfSetMessage);
			void NsmLldpInternalMessageHandler(NsmLldpInternalMessage *pNsmLldpInternalMessage);
			ResourceId validateLldpPhyInfConf (DceSynchronousLinearSeqContext *pDceSynchronousLinearSequencerContext);
			ResourceId updateLldpPhyInfConf(DceSynchronousLinearSeqContext *pDceSynchronousLinearSequencerContext);
			ResourceId updateLldpPhyInfMO(DceSynchronousLinearSeqContext *pDceSynchronousLinearSequencerContext);
			LldpIntfConfLocalManagedObject* LldpIntfConfGetChildMoByParentMo(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);

        protected:
        public:
            LldpLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~LldpLocalWorker ();
		
// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //LLDPLOCALWORKER_H
