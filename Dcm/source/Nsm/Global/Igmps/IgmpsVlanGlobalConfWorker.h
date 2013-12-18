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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSVLANGLOBALCONFWORKER_H
#define IGMPSVLANGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Global/VlanIntfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class NsmGlobalUpdateIgmpsVlanConfigMessage;
    class NsmGlobalUpdateIgmpsSgVlanConfigMessage;
    class NsmGlobalUpdateIgmpsMrtrVlanConfigMessage;

    class IgmpsVlanGlobalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmGlobalUpdateIgmpsMrtrVlanConfigMessageHandler( NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pNsmGlobalUpdateIgmpsMrtrVlanConfigMessage);
            void NsmGlobalUpdateIgmpsSgVlanConfigMessageHandler( NsmGlobalUpdateIgmpsSgVlanConfigMessage *pNsmGlobalUpdateIgmpsSgVlanConfigMessage);
            void NsmGlobalUpdateIgmpsVlanConfigMessageHandler( NsmGlobalUpdateIgmpsVlanConfigMessage *pNsmGlobalUpdateIgmpsVlanConfigMessage);

            void IgmpsVlanConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);

            void validateIgmpsMrtrVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsMrtrVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsMrtrVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsSgVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsSgVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void validateIgmpsVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void updateIgmpsVlanConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

            void validateVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            VlanIntfManagedObject * getVlanIntfMOForVid (UI32 vlanId);
            IgmpsVlanIntfConfManagedObject* IgmpsVlanGetChildMoByParentMo(VlanIntfManagedObject *pVlanIntfManagedObject);
            int getIndexOfSgMOForMessage (vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > &igmpsSgVlanConfigMOVector, NsmGlobalUpdateIgmpsSgVlanConfigMessage *pIgmpsSgVlanConfigMessage);
            int getIndexOfMrtrMOForMessage (vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > &igmpsMrtrVlanConfigMOVector, NsmGlobalUpdateIgmpsMrtrVlanConfigMessage *pIgmpsMrtrVlanConfigMessage);
            void validateIgmpsSgVlanConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

        protected:
        public:
            IgmpsVlanGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~IgmpsVlanGlobalConfWorker ();

// Now the data members

        private:
        protected:
        public:
			ResourceId  deleteIgmpsTenGigIntfMOForLocation (const vector<UI32> uiLocationIds);
			ResourceId  checkIntfBelongsToPortCh(string ifName);
    		ResourceId  checkmRouterConfPresent(NsmGlobalUpdateIgmpsVlanConfigMessage 
												*pNsmGlobalUpdateIgmpsVlanConfigMessage);

    };
}
#endif                                            //IGMPSVLANGLOBALCONFWORKER_H
