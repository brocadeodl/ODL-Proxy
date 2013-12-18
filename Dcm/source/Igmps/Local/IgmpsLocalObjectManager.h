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

#ifndef IGMPSLOCALOBJECTMANAGER_H
#define IGMPSLOCALOBJECTMANAGER_H

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsLocalConfigWorker;

    class IgmpsLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            IgmpsVlanIntfConfManagedObject *IgmpsVlanGetChildMoByParentMo(VlanIntfManagedObject *pVlanIntfManagedObject);
            void ConfigReplayIgmpsGlobalConfig();
            //void ConfigReplayIgmpsVlanIntfConfig(vector<WaveManagedObject *>* &pVlanResults);
            //void ConfigReplayIgmpsMrtrVlanConfig(vector<WaveManagedObject *>* &pVlanResults);
            //void ConfigReplayIgmpsSgVlanConfig(vector<WaveManagedObject *>* &pVlanResults);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
        protected:
        public:
            IgmpsLocalObjectManager ();
            virtual    ~IgmpsLocalObjectManager ();
            static IgmpsLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:
            IgmpsLocalConfigWorker  *m_pIgmpsLocalConfigWorker;

        protected:
        public:
    };
}
#endif                                            //IGMPSLOCALOBJECTMANAGER_H
