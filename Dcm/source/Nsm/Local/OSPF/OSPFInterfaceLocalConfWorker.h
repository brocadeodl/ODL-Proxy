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
 *   Author : kverma                                                       *
 **************************************************************************/

#ifndef OSPFINTERFACELOCALCONFWORKER_H
#define OSPFINTERFACELOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalMessage.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "DcmCore/DcmToolKit.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmUpdateIgmpsVlanLocalConfigMessage;
    class NsmUpdateIgmpsSgVlanLocalConfigMessage;
    class NsmUpdateIgmpsMrtrVlanLocalConfigMessage;

    class OSPFInterfaceLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			ResourceId ospfInterfaceConfigStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId ospfInterfaceConfigMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			void  NsmLocalOSPFInterfaceConfigMessageHandler( OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage);

            NsmL3ReadyManagedObject *ospfGetL3RdyMo(DceSynchronousLinearSeqContext *pSeqCtx, OSPFInterfaceLocalMessage *pMsg);
            void ospfUpdateLocalMessage(OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage, OSPFInterfaceConfigMessage *pOSPFInterfaceConfigMessage);
            OSPFInterfaceLocalManagedObject *OSPFGetOSPFInterfaceLocalManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId validateKeyLength(OSPFInterfaceConfigMessage *);

        protected:
        public:
            OSPFInterfaceLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~OSPFInterfaceLocalConfWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //OSPFINTERFACELOCALCONFWORKER_H
