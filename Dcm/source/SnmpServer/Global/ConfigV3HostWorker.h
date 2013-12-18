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

#ifndef CONFIGV3HOSTWORKER_H
#define CONFIGV3HOSTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerObjectManager;
    class SnmpServerSnmpConfigAddV3HostMessage;
    class SnmpServerSnmpConfigDelV3HostMessage;
    class SnmpServerSnmpConfigChangeV3HostMessage;

    class ConfigV3HostWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SnmpServerSnmpConfigAddV3HostMessageHandler( SnmpServerSnmpConfigAddV3HostMessage *pSnmpServerSnmpConfigAddV3HostMessage);
            void SnmpServerSnmpConfigChangeV3HostMessageHandler( SnmpServerSnmpConfigChangeV3HostMessage *pSnmpServerSnmpConfigChangeV3HostMessage);
            void SnmpServerSnmpConfigDelV3HostMessageHandler( SnmpServerSnmpConfigDelV3HostMessage *pSnmpServerSnmpConfigDelV3HostMessage);
            void addV3HostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void setV3HostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);
            void delV3HostSTCCB(WaveNs::WaveSendToClusterContext *pWaveSendToClusterContext);


//			void               install                                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        protected:
        public:
            ConfigV3HostWorker ( SnmpServerObjectManager *pSnmpServerObjectManager);
            virtual    ~ConfigV3HostWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //CONFIGV3HOSTWORKER_H
