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
 *   Author : suw                                                     *
 **************************************************************************/

#ifndef IPPOLICYLOCALWORKERS_H
#define IPPOLICYLOCALWORKERS_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "IpPolicy/Framework/IpPolicyAsPathAccessListLocalMessage.h"
#include "IpPolicy/Framework/IpCommunityAccessListLocalMessage.h"
#include "IpPolicy/Framework/FilterChangeUpdateLocalMessage.h"
#include "IpPolicy/Framework/PbrVrfNexthopMessage.h"
#include "WyserEaGateway/GenericClientMessage.h"
#include "IpPolicy/Framework/PBRShowMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpPolicyLocalObjectManager;
    class IpPolicyRouteMapMessage;
    class IpPolicyIpPrefixMessage;
	class IpPolicyGetPBRObjectMessage;
	class PBRShowMessage;

    class IpPolicyLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void IpPolicyIpPrefixMessageHandler( IpPolicyIpPrefixMessage *pIpPolicyCreateIpPrefixMessage);
            void IpPolicyRouteMapMessageHandler( IpPolicyRouteMapMessage *pIpPolicyCreateRouteMapMessage);
            void PbrVrfNexthopMessageHandler(PbrVrfNexthopMessage *pPbrVrfNexthopMessage);
			void deleteFromPBRMO(string routeMapName, DceSynchronousLinearSeqContext *seq);
            ResourceId validatePbrVrfNexthop(PrismSynchronousLinearSequencerContext *seq);
            ResourceId forwardPbrVrfNexthop(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writePbrVrfNexthopMO(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writePbrNexthopMO(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writePbrVrfMO(PrismSynchronousLinearSequencerContext *seq);
			ResourceId writePbrGlobalNexthopMO (PrismSynchronousLinearSequencerContext *seq);
            ResourceId validateRouteMap(DceSynchronousLinearSeqContext *seq);
            ResourceId forwardRouteMap(DceSynchronousLinearSeqContext *seq);
            ResourceId writeRouteMapMO(DceSynchronousLinearSeqContext *seq);
            ResourceId validateIpPrefix(PrismSynchronousLinearSequencerContext *seq);
            ResourceId forwardIpPrefix(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writeIpPrefixMO(PrismSynchronousLinearSequencerContext *seq);
            void IpPolicyAsPathAccessListLocalMessageHandler( IpPolicyAsPathAccessListLocalMessage *pIpPolicyAsPathAccessListLocalMessage);
            ResourceId validateAsPathAccessList(PrismSynchronousLinearSequencerContext *seq);
            ResourceId forwardAsPathAccessList(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writeAsPathAccessListMO(PrismSynchronousLinearSequencerContext *seq);
            void IpCommunityAccessListLocalMessageHandler( IpCommunityAccessListLocalMessage *pIpCommunityAccessListLocalMessage);
            ResourceId validateIpCommunityAccessList(PrismSynchronousLinearSequencerContext *seq);
            ResourceId forwardIpCommunityAccessList(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writeIpCommunityAccessListMO(PrismSynchronousLinearSequencerContext *seq);
            void FilterChangeUpdateLocalMessageHandler(FilterChangeUpdateLocalMessage *pFilterChangeUpdateLocalMessage);
            ResourceId validateFilterChangeUpdateDelay(PrismSynchronousLinearSequencerContext *seq);
            ResourceId forwardFilterChangeUpdateDelay(PrismSynchronousLinearSequencerContext *seq);
            ResourceId writeFilterChangeUpdateDelayMO(PrismSynchronousLinearSequencerContext *seq);
			void IpPolicyGetRouteMapMessageHandler (IpPolicyGetPBRObjectMessage *pIpPolicyGetPBRObjectMessage);
			void PBRShowMessageHandler(PBRShowMessage *pPBRShowMessage);
			void sendShowRouteMapCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
			void RouteMapLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			ResourceId sendShowToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

        protected:
        public:
            IpPolicyLocalWorker ( IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
            virtual    ~IpPolicyLocalWorker ();
            ResourceId restoreRouteMapMO(int slot_id = -1);
			ResourceId ConfigReplayPbrGlobalIntf(const UI32 ifType);
			ResourceId restorePBRMO(SI32 slot_id);
			GenericClientMessage_* ReplayPbrGlobalIntf(const UI32 ifType, WaveManagedObject *pWaveManagedObject);
            ResourceId restoreIpPrefixMO();
            ResourceId restoreAsPathAccessListMO();
            ResourceId restoreIpCommunityStdAccessListMO();
            ResourceId restoreIpCommunityExtAccessListMO();
            ResourceId restoreFilterChangeUpdateDelayMO();

// Now the data members

        private:
            vector<ObjectId> objectIds;
        protected:
        public:
    };

}
#endif                                            // IPPOLICYLOCALWORKERS_H
