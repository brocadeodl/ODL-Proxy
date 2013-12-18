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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPLOCALOBJECTMANAGER_H
#define ARPLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"

#include "Arp/Local/ArpStaticConfigHandler.h"
#include "ArpStaticConfigLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpConfigLocalWorker;

    class ArpLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        protected:
        public:
            ArpLocalObjectManager ();
            virtual    ~ArpLocalObjectManager ();
            static ArpLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);

			void restoreStaticArp(UI32 slot);
			void deleteStaticArp(UI32 slot); 
			void slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);
      virtual void multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);
// Now the data members

        private:
            ArpConfigLocalWorker      *m_pArpConfigLocalWorker;
            ArpStaticConfigHandler    *m_pArpStaticConfigHandler;

        protected:
        public:
    };
}
#endif                                            //ARPLOCALOBJECTMANAGER_H
