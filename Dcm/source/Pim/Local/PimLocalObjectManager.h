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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMLOCALOBJECTMANAGER_H
#define PIMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Nsm/Local/Pim/PimIntfCfgLocalManagedObject.h"
#include "ClientInterface/Pim/PimRtrCfgClientMsg.h"
#include "ClientInterface/Pim/PimBsrCandClientMsg.h"
#include "ClientInterface/Pim/PimStaticRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpPrefixCfgClientMsg.h"
#include "ClientInterface/Pim/PimIntfCfgClientMsg.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimWorker;

    class PimLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void slotFailover(WaveAsynchronousContextForSlotFailover *pCtxt);
        protected:
        public:
            PimLocalObjectManager ();
            virtual    ~PimLocalObjectManager ();
            static PimLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            void setPimRtrCfgClientMsgFieldsForReplay(PimRtrCfgClientMsg *msg, const PimRtrCfgLocalManagedObject *pMo);
            void setPimBsrCandClientMsgFieldsForReplay(PimBsrCandClientMsg *msg, const BsrCandidateLocalManagedObject *pMo);

            void setPimStaticRpCfgClientMsgFieldsForReplay(PimStaticRpCfgClientMsg *msg, const StaticRpLocalManagedObject *pMo);

            void setPimCandRpCfgClientMsgFieldsForReplay(PimCandRpCfgClientMsg *msg, const RpCandidateLocalManagedObject *pMo);

            void setPimCandRpPrefixCfgClientMsgFieldsForReplay(PimCandRpPrefixCfgClientMsg *msg, const RpCandidatePrefixLocalManagedObject *pMo);

            void setPimIntfCfgClientMsgFieldsForReplay(PimIntfCfgClientMsg *msg, const PimIntfCfgLocalManagedObject *pMo, const UI8 intfType, const string intfName);
 
            void postboot(WaveAsynchronousContextForPostbootPhase *
                pWaveAsynchronousContextForPostBootPhase);
            void restorePimRtrCfg();
            void restoreStaticRpCfg();
			void restoreRpCandPrefixCfg();
            void restorePhyIntfPimCfg(SI32 slotNum);
            void restoreVlanIntfPimCfg();
            void restorePoIntfPimCfg();
            void replayPimIntfCfg(PimIntfCfgLocalManagedObject *pMo, 
                UI8 intfType, string intfName);
            void configReplayPimPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
            void restoreRpCandCfg(SI32 slotNum);
            void restoreBsrCandCfg(SI32 slotNum);
            void delRpCandOnSlotFailover(SI32 slotNum);
            void delBsrCandOnSlotFailover(SI32 slotNum);
            UI8 xltToPimIntfType(UI8 intfType);

// Now the data members
        private:
            PimWorker  *m_pPimWorker;
        protected:
        public:
    };
}
#endif                                            //PIMLOCALOBJECTMANAGER_H
