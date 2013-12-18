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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef CONFIGHOSTWORKER_H
#define CONFIGHOSTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerObjectManager;
    class SnmpServerSnmpConfigAddHostMessage;
    class SnmpServerSnmpConfigDelHostMessage;
    class SnmpServerSnmpConfigChangeHostMessage;

    class ConfigHostWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SnmpServerSnmpConfigAddHostMessageHandler( SnmpServerSnmpConfigAddHostMessage *pSnmpServerSnmpConfigAddHostMessage);
            void SnmpServerSnmpConfigChangeHostMessageHandler( SnmpServerSnmpConfigChangeHostMessage *pSnmpServerSnmpConfigChangeHostMessage);
            void SnmpServerSnmpConfigDelHostMessageHandler( SnmpServerSnmpConfigDelHostMessage *pSnmpServerSnmpConfigDelHostMessage);
			void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void addHostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void changeHostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void delHostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);

		void installDefaults();
	void configReplayEnd(WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
        protected:
        public:
            ConfigHostWorker ( SnmpServerObjectManager *pSnmpServerObjectManager);
            virtual    ~ConfigHostWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //CONFIGHOSTWORKER_H
