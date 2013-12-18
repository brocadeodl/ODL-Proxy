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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef L2SYSGLOBALOBJECTMANAGER_H
#define L2SYSGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalClearMessage.h"
#include "L2sys/Global/MacAddressTableAgingTimeConfigHandler.h"
#include "L2sys/Global/MacAddressTableStaticConfigHandler.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysGlobalUpdateL2sysAgingTimeoutMessage;
    class L2sysGlobalUpdateL2sysGlobalStaticMacMessage;

    class L2sysGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void L2sysGlobalUpdateL2sysAgingTimeoutMessageHandler( L2sysGlobalUpdateL2sysAgingTimeoutMessage *pL2sysGlobalUpdateL2sysAgingTimeoutMessage);
            void L2sysGlobalUpdateL2sysGlobalStaticMacMessageHandler( L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage);
            void L2sysGlobalUpdateL2sysGlobalClearMessageHandler( L2sysGlobalUpdateL2sysGlobalClearMessage *pL2sysGlobalUpdateL2sysGlobalClearMessage);
            void validateL2sysUpdateMac(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void L2sysUpdateMac(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void L2sysUpdateMacMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateL2sysAging (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void updateL2sysAgingMO (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void L2sysClearMac(PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void L2sysGlobalProfileConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
            void L2sysGlobalAgingConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
            void L2sysGlobalClearSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
            LocationId L2sysGlobalGetDestLocationId(PrismLinearSequencerContext *pPrismLinearSequencerContext, L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage);

            ResourceId validateIfName (const int interfaceType, const string& ifName);
            bool isMOAlreadyExisting (PrismLinearSequencerContext *pPrismLinearSequencerContext, ResourceId &status);

        protected:
        public:
            L2sysGlobalObjectManager ();
            virtual    ~L2sysGlobalObjectManager ();
            static L2sysGlobalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);
			void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
            void clusterCreateValidate(WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
// Now the data members

        private:
            MacAddressTableAgingTimeConfigHandler       *m_pMacAddressTableAgingTimeConfigHandler;
            MacAddressTableStaticConfigHandler          *m_pMacAddressTableStaticConfigHandler;

        protected:
        public:
    };
}
#endif
