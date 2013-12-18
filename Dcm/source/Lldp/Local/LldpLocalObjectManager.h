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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef LLDPLOCALOBJECTMANAGER_H
#define LLDPLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Lldp/Global/LldpGlobalScalarsConfManagedObject.h"
#include "ClientInterface/Lldp/LldpClientScalarsConfSetMessage.h"
#include "Lldp/Local/CdpLocalConfWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpLocalConfWorker;

    class LldpLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void ConfigReplayLldpGlobalScalars();
			void ConfigReplayLldpGlobalProfile();
    		UI32 setGlobalConfigForReplay (LldpGlobalScalarsConfManagedObject
                   *pLldpGlobalScalarsConfMO, LldpClientScalarsConfSetMessage *m);
            
			void ConfigReplayCdpGlobal();

			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
        protected:
        public:
            LldpLocalObjectManager ();
            virtual    ~LldpLocalObjectManager ();
            static LldpLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:
            LldpLocalConfWorker  *m_pLldpLocalConfWorker;
            CdpLocalConfWorker   *m_pCdpLocalConfWorker;

        protected:
        public:
    };
}
#endif                                            //LLDPLOCALOBJECTMANAGER_H
