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

#ifndef FCOELOCALWORKERWORKER_H
#define FCOELOCALWORKERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Fcoe/Common/FcoeCommonWorker.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Local/FcoeIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeEnodeLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
    #define FCOE_DEL_CLIENT_MSG         true
    #define FCOE_DONT_DEL_CLIENT_MSG    false
    #define FCOE_SEND_SYNC_TO_DAEMON(_name, _msg_p, _status, _del_msg) \
        { \
            _status = sendSynchronouslyToWaveClient((_name), (_msg_p)); \
            if ((_status) != WAVE_MESSAGE_SUCCESS) {\
                FCOE_PLUG_DBG_ERR_S(string("Send To Daemon, Message-Status=[") + \
                                    status + string("]:") + \
                                    FrameworkToolKit::localize((_status))); \
            } \
            if ((_msg_p)->getClientStatus() != WAVE_MESSAGE_SUCCESS) { \
                FCOE_PLUG_DBG_ERR_S(string("Send To Daemon, Client Status=[") + \
                                    (_msg_p)->getClientStatus() + string("]:") + \
                                    FrameworkToolKit::localize(((_msg_p)->getClientStatus()))); \
                (_status) = (((_msg_p)->getClientStatus())); \
            } \
            if ((_del_msg) == true) { \
                delete (_msg_p); \
            } \
        }
    class FcoeLocalObjectManager;
    class FcoeLocalFcoeIntfMessage;
    class FcoeLocalFcoeFabricMapMessage;
    class FcoeLocalFcoeMapMessage;
    class FcoeLocalFcoeFipMessage;
    class FcoeLocalFcoeShowMessage;
    class FcoeLocalFcoeClearMessage;
    class FcoeLocalETIntfMessage;
    class FcoeLocalFcoeInternalMessage;
    class FcoeLocalEnodeMessage;
    class PhyIntfLocalManagedObject;
	class FcoeLocalFcoeFsbMessage;

    class FcoeLocalWorker: public FcoeCommonWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void FcoeLocalFcoeFabricMapMessageHandler( FcoeLocalFcoeFabricMapMessage *pFcoeLocalFcoeFabricMapMessage);
            void FcoeLocalFcoeFipMessageHandler( FcoeLocalFcoeFipMessage *pFcoeLocalFcoeFipMessage);
            void FcoeLocalFcoeMapMessageHandler( FcoeLocalFcoeMapMessage *pFcoeLocalFcoeMapMessage);
            void FcoeLocalFcoeIntfMessageHandler( FcoeLocalFcoeIntfMessage *pFcoeLocalFcoeIntfMessage);
            void FcoeLocalFcoeShowMessageHandler (FcoeLocalFcoeShowMessage *pFcoeLocalFcoeShowMessage);
            void FcoeLocalFcoeClearMessageHandler (FcoeLocalFcoeClearMessage *pFcoeLocalFcoeClearMessage);
            void FcoeLocalClearSendToDaemon(FcoeLinearSequencerContext *ctx_p);
            
            
            void FcoeLocalFcoeMapSendToDaemon (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalFcoeFabricMapSendToDaemon (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalFcoeFipSendToDaemon (FcoeLinearSequencerContext *ctx_p);
            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void FcoeLocalFcoeFabricMapSendToLldpDaemon(FcoeLinearSequencerContext *ctx_p);

            void FcoeLocalIntfSendToDaemon(FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalIntfHandleConfig(FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalETIntfHandleConfig(FcoeLinearSequencerContext *ctx_p);

            void FcoeLocalShowSendToDaemon (FcoeLinearSequencerContext *ctx_p);

            void NoShutdown (FcoeLinearSequencerContext *ctx_p);
            void shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

            ResourceId FcoeLocalFcoePluginStatusSendToDaemon (uint32_t plugin_status);
            void FcoeLocalInstallETInterfaceStep (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalInstallInterfaceStep (FcoeLinearSequencerContext *ctx_p);

            FcoeIntfLocalManagedObject *FcoeLocalIntfGetMOByName(string &if_name,
                                                 FcoeLinearSequencerContext *ctx_p = NULL);
            FcoeETIntfLocalManagedObject *FcoeLocalETIntfGetMOByName(string &if_name,
                                                 FcoeLinearSequencerContext *ctx_p = NULL);
            
            void FcoeLocalETIntfMessageHandler(FcoeLocalETIntfMessage *pFcoeLocalETIntfMessage);
            void FcoeLocalETIntfSendToDaemon(FcoeLinearSequencerContext *ctx_p);
            
            void FcoeLocalFcoeInternalMessageHandler (FcoeLocalFcoeInternalMessage *pFcoeLocalFcoeInternalMessage);
            void FcoeLocalInternalSendToDaemon (FcoeLinearSequencerContext *ctx_p);

            ResourceId FcoeLocalCreateInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                     fcoe_intf_config_msg_t *arg_data_p = NULL);
            ResourceId FcoeLocalDeleteInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                     fcoe_intf_config_msg_t *arg_data_p = NULL);

            ResourceId FcoeLocalCreateInterface (FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalHandleInterfaceStatusUpdate(FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalUpdateInterfaceBinding(FcoeLinearSequencerContext *ctx_p);
			ResourceId FcoeLocalUpdateInterfaceBindingCommon(string if_name, FcoeIntfLocalManagedObject *fcoe_if_p, UI32 binding_type, string binding, FcoeLinearSequencerContext *ctx_p);
	        int FcoeLocalIntfSendToDaemonOneBinding(string if_name, FcoeIntfLocalManagedObject *fcoe_if_p, UI32 binding_type, string binding, FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalRemoveInterfaceBinding(FcoeLinearSequencerContext *ctx_p);

            ResourceId FcoeLocalCreateETInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                       fcoe_intf_config_msg_t *arg_data_p = NULL);
            ResourceId FcoeLocalDeleteETUportBulk (FcoeLinearSequencerContext *ctx_p);

            ResourceId FcoeLocalCreateETInterface(FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalDeleteETInterface(FcoeLinearSequencerContext *ctx_p);

            ResourceId FcoeLocalHandleETInterfaceISLStatusUpdate (FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalHandleETInterfaceTrunkStatusUpdate (FcoeLinearSequencerContext *ctx_p);
            ResourceId FcoeLocalHandleETInterfaceStatusUpdate (FcoeLinearSequencerContext *ctx_p);

            PhyIntfLocalManagedObject* GetPhyIntfMoById (string &if_name,
                                                         FcoeLinearSequencerContext *ctx_p = NULL);

            void FcoeLocalBackendSyncUpETInterfaceStep (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalBackendSyncUpInterfaceStep (FcoeLinearSequencerContext *ctx_p);
            void backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext);
			void FcoeLocalFcoeFsbMessageHandler(FcoeLocalFcoeFsbMessage *pFcoeLocalFcoeFsbMessage);
			void FcoeLocalFcoeFsbSendToDaemon (FcoeLinearSequencerContext *ctx_p);

            // fcoe enodes functions 
            void FcoeLocalFcoeEnodeSendToDaemon (FcoeLinearSequencerContext *ctx_p);
            void FcoeLocalEnodesConfigUpdate(FcoeLinearSequencerContext *ctx_p);
            FcoeEnodeLocalManagedObject *  FcoeLocalEnodeGetMOByName(FcoeLinearSequencerContext *ctx_p);
            void  FcoeLocalEnodeMessageHandler( FcoeLocalEnodeMessage *pFcoeLocalEnodeMessage);
            void upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);
            void FcoeLocalFcoeFabricMapUpdateEnodes (FcoeLinearSequencerContext *ctx_p);
            UI32 FcoeLocalCalculateFcoeEnodes(FcoeLinearSequencerContext *ctx_p);

        protected:
        public:
            FcoeLocalWorker ( FcoeLocalObjectManager *pFcoeLocalObjectManager);
            virtual    ~FcoeLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //FCOELOCALWORKERWORKER_H
