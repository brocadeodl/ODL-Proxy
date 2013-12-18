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
 *   Author : mgarg                                                        *
 **************************************************************************/

#ifndef FABRICSERVICELOCALOBJECTMANAGER_H
#define FABRICSERVICELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "dcm/fos_headers.h"

using namespace WaveNs;

typedef struct ngbhr_info {
    u_char nbrdispType;
    u_char nbrtupleBlade;
    u_char nbrtuplePort;
} ngbhr_info_t;

namespace DcmNs
{

    class FabricServiceGetFabricInfoMessage;
    class FabricServicePortLogMessage;
    class FabricServicePortLogClearMessage;
    class FabricServiceGetislshowMessage;
    class FabricServiceGetTrunkShowMessage;
    class FabricServiceSetLocalDomainIdMessage;
    class FabricServiceSwitchShowMessage;
    class FabricServiceSetInsistentDomainIdMessage;
    class FabricServiceDisableIslsToNeighborMessage;
    class FabricServiceGetlinkInfoMessage;

    class FabricServiceLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void FabricServiceGetFabricInfoMessageHandler( FabricServiceGetFabricInfoMessage *pFabricServiceGetFabricInfoMessage);
            void FabricServiceGetislshowMessageHandler( FabricServiceGetislshowMessage *pFabricServiceGetislshowMessage);
            ResourceId getFabricInfo (PrismSynchronousLinearSequencerContext *pFabricServiceGetFabricInfoMessageContext);
            ResourceId show_fabric_isl (PrismSynchronousLinearSequencerContext *pFabricServiceGetislshowMessageContext);
            void FabricServiceGetTrunkShowMessageHandler (FabricServiceGetTrunkShowMessage *pFabricServiceGetTrunkShowMessage);
            ResourceId TrunkShowGetEntries (PrismSynchronousLinearSequencerContext *pFabricServiceGetTrunkShowMessageContext);
            void FabricServiceSetLocalDomainIdMessageHandler( FabricServiceSetLocalDomainIdMessage *pFabricServiceSetLocalDomainIdMessage);
            ResourceId SetLocalDomainId (PrismSynchronousLinearSequencerContext *pFabricServiceSetLocalDomainIdMessageContext);
            void FabricServiceSwitchShowMessageHandler( FabricServiceSwitchShowMessage *pFabricSwitchShowMessage);
            ResourceId switch_show_isl (PrismSynchronousLinearSequencerContext *pFabricServiceSwitchShowMessageContext);
            void FabricServicePortLogMessageHandler( FabricServicePortLogMessage *pFabricPortLogMessage);
            ResourceId fabric_port_log_show (PrismSynchronousLinearSequencerContext *pFabricServicePortLogMessageContext);
            void FabricServicePortLogClearMessageHandler( FabricServicePortLogClearMessage *pFabricPortLogClearMessage);
            ResourceId fabric_port_log_clear (PrismSynchronousLinearSequencerContext *pFabricServicePortLogClearMessageContext);
            void FabricServiceSetInsistentDomainIdMessageHandler( FabricServiceSetInsistentDomainIdMessage *pFabricServiceSetInsistentDomainIdMessage);
            ResourceId SetInsistentDomainId (PrismSynchronousLinearSequencerContext *pFabricServiceSetInsistentDomainIdMessageContext);
            void FabricServiceDisableIslsToNeighborMessageHandler( FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage);
            ResourceId DisableIslsToNeighbor (PrismSynchronousLinearSequencerContext *pFabricServiceFabricServiceDisableIslsToNeighborMessageMessageContext);
            void FabricServiceGetlinkInfoMessageHandler( FabricServiceGetlinkInfoMessage *pFabricServiceGetlinkInfoMessage);
            ResourceId LinkInfoShowGetEntries (PrismSynchronousLinearSequencerContext *pFabricServiceGetlinkInfoMessageContext);

            // List of over-ridden functions to gain certain priviledges
            bool isOperationAllowedBeforeEnabling (const UI32 &operationCode);

        protected:
        public:
            FabricServiceLocalObjectManager ();
            virtual    ~FabricServiceLocalObjectManager ();
            static FabricServiceLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            static int neighborInterfaceTuple(links_info_t *linkinfo, int remdomain, int remport, int domaincount, ngbhr_info_t *nbr_interface);

// Now the data members

        private:

        protected:
        public:
    };
}
#endif //FABRICSERVICELOCALOBJECTMANAGER_H
