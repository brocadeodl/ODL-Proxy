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

#ifndef DHCPRELLOCALOBJECTMANAGER_H
#define DHCPRELLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalShowClrWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class DhcpRelLocalObjectManager: public WaveLocalObjectManager
    {
		// Class methods
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void replayDhcpRelVeIntfConfig(LocationId locId);
            void slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
			void clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
			void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
        protected:
        public:
            DhcpRelLocalObjectManager ();
            virtual ~DhcpRelLocalObjectManager ();
            static DhcpRelLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            void replayDhcpRelPhyIntfConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);


		// Class data members
        private:
			DhcpRelLocalShowClrWorker  *m_pDhcpLocalShowClrWorker;
        protected:
        public:
    };
}
#endif                                            //DHCPRELLOCALOBJECTMANAGER_H
