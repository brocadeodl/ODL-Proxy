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

/******************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.             *
 *   All rights reserved.                                                     *
 *   Author : Mobin Mohan                                                     *
 *****************************************************************************/

#ifndef CONFIGMGMTLOCALWORKER_H
#define CONFIGMGMTLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/ConfigMgmt/ConfigMgmtLocalShowMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class ConfigMgmtLocalShowMessage;

    class ConfigMgmtLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            
			/* ConfigMgmt */
            ResourceId sendShowToConfigMgmtDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void sendConfigMgmtShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
            void ConfigMgmtLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void ConfigMgmtLocalShowMessageHandler (ConfigMgmtLocalShowMessage *pConfigMgmtLocalShowMessage);
						
        protected:
        public:
            ConfigMgmtLocalWorker ( NsmLocalObjectManager *pWaveObjectManager);
            virtual    ~ConfigMgmtLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //CONFIGMGMTLOCALWORKER_H
