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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef NSMFCOELOCALWORKER_H
#define NSMFCOELOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Fcoe/Common/FcoeCommonWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class FcoeLocalFcoeportAttrMessage;
    class PhyIntfLocalManagedObject;
    class PoIntfManagedObject;
    class FcoeportAttrLocalManagedObject;
    class FcoeMapManagedObject;
    class FcoeportAttrGlobalManagedObject;
    class NsmFcoeportAttrMessage;
    class FcoeClientEthConfigMessage;

    class NsmFcoeLocalWorker: public FcoeCommonWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void FcoeLocalFcoeportAttrMessageHandler(FcoeLocalFcoeportAttrMessage *pFcoeLocalFcoeportAttrMessage);
            
            ResourceId FcoeLocalFcoeportAttrSendToDaemon (FcoeLinearSequencerContext *ctx_p, string &map_name,
                                                          UI64 &ifindex);

            void
            FcoeLocalFcoeportAttrHandleMoUpdate (FcoeLinearSequencerContext *ctx_p);
        
            PoIntfManagedObject* GetPoIntfMoById (string &if_name,
                                                  FcoeLinearSequencerContext *ctx_p = NULL);

            PhyIntfLocalManagedObject  *GetPhyIntfMoById (string &if_name, FcoeLinearSequencerContext *ctx_p = NULL);
            ObjectId  FcoeMapGetObjectIdByName (string &map_name, FcoeLinearSequencerContext *ctx_p = NULL);
            FcoeMapManagedObject *FcoeMapGetMoByName(string &map_name, FcoeLinearSequencerContext *ctx_p = NULL);

            void FcoeLocalFcoeportProcessBackend (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalFcoeportAttrValidate (FcoeLinearSequencerContext *ctx_p);

            ResourceId FcoeLocalSendEthConfigMessageToDaemon (NsmFcoeportAttrMessage *msg_p,
                                                              bool rollback = false);

            FcoeClientEthConfigMessage* FcoeLocalCreateEthConfigMessage(UI32 oper, bool roolback);

            ResourceId FcoeLocalFcoeportAttrLagPort (FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalFcoeportAttrPhyPort (FcoeLinearSequencerContext *ctx_p);
			void getPoMemberIfindexListForLocalNode(const string  &PoName, vector<UI64> &ifx_list);
            void configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
            void fileReplayEnd (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd);
            ResourceId pushFcoeProvisioningConfig ();
        
        protected:
        public:
            NsmFcoeLocalWorker ( NsmLocalObjectManager *pFcoeLocalObjectManager);
            virtual    ~NsmFcoeLocalWorker ();

            FcoeportAttrLocalManagedObject* FcoeportGetChildMoByParentMo(PhyIntfLocalManagedObject *if_mo_p,
                                                                         FcoeLinearSequencerContext *ctx_p = NULL);

            FcoeportAttrGlobalManagedObject* FcoeportGetChildMoByParentLagMo(PoIntfManagedObject *if_mo_p,
                                                                             FcoeLinearSequencerContext *ctx_p = NULL);

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //NSMFCOELOCALWORKER_H
