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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#ifndef DHCPRELLOCALSHOWCLRWORKER_H
#define DHCPRELLOCALSHOWCLRWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalShowMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalClearMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class DhcpRelLocalObjectManager;

    class DhcpRelLocalShowClrWorker: public WaveWorker
    {
		// Class methods
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void DhcpRelLocalShowMessageHandler (DhcpRelLocalShowMessage *pDhcpRelLocalShowMessage);
            void DhcpRelLocalClearMessageHandler (DhcpRelLocalClearMessage *pDhcpRelLocalClearMessage);
            ResourceId sendShowToIphelperDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            ResourceId sendClearToIphelperDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendClearToCluster(PrismLinearSequencerContext *seq_ctx_p);
            void DhcpRelClearCmdClusterCallback(WaveSendToClusterContext *ctx_p);
        protected:
        public:
            DhcpRelLocalShowClrWorker (DhcpRelLocalObjectManager *pDhcpRelLocalObjectManager);
            virtual    ~DhcpRelLocalShowClrWorker ();

		// Class data members
        private:
        protected:
        public:
    };
}
#endif                                            //DHCPRELLOCALSHOWCLRWORKER_H
