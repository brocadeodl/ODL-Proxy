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

#ifndef FCOEGLOBALWORKER_H
#define FCOEGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Global/FcoeFipManagedObject.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Common/FcoeCommonWorker.h"
#include "Fcoe/Global/FcoeGlobalFsbManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
    
    // Global Messages
    class FcoeGlobalObjectManager;
    class FcoeGlobalFcoeFabricMapMessage;
    class FcoeGlobalFcoeMapMessage;
    class FcoeGlobalFipMessage;
    class FcoeGlobalFcoeClearMessage;
    class FcoeGlobalFcoeShowMessage;
    class FcoeGlobalFcoeInternalMessage;   
	class FcoeGlobalFcoeFsbMessage;
 
    // Local Messages
    class FcoeLocalFcoeFabricMapMessage;
    class FcoeLocalFcoeMapMessage;
    class FcoeLocalFcoeFipMessage;
	class FcoeLocalFcoeFsbMessage;
    
    
    class FcoeGlobalWorker: public FcoeCommonWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);

            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            // Fabric Map Message Handlers
            void FcoeGlobalFcoeFabricMapMessageHandler ( FcoeGlobalFcoeFabricMapMessage *pFcoeGlobalFcoeFabricMapMessage);
                        
            // FCOE Map Message Handlers
            void FcoeGlobalFcoeMapMessageHandler( FcoeGlobalFcoeMapMessage *pFcoeGlobalFcoeMapMessage);

            // FCOE FIP
            void FcoeGlobalFipMessageHandler ( FcoeGlobalFipMessage *pFcoeGlobalFipMessage);
            
            //CLEAR
            void FcoeGlobalFcoeClearMessageHandler (FcoeGlobalFcoeClearMessage *pClearMessage);

            // Show
            void FcoeGlobalFcoeShowMessageHandler (FcoeGlobalFcoeShowMessage *pShowMessage);

            // Internal
            void FcoeGlobalFcoeInternalMessageHandler (FcoeGlobalFcoeInternalMessage *pInternalMessage);

			// FSB
            void FcoeGlobalFcoeFsbMessageHandler(FcoeGlobalFcoeFsbMessage *pFcoeGlobalFcoeFsbMessage);

            // Configuration Handlers
            void FcoeGlobalFcoeFabricMapHandleConfig(FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeMapHandleConfig(FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeFipHandleConfig(FcoeLinearSequencerContext *seq_ctx_p);
			void FcoeGlobalFcoeFsbHandleConfig(FcoeLinearSequencerContext *seq_ctx_p);
            
            // Functions to send messages to Local Plugin
            void FcoeGlobalFcoeFabricMapSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeMapSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeFipSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeFabricMapSendToNsmGlobal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeClearSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeShowSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
            void FcoeGlobalFcoeInternalSendToLocal (FcoeLinearSequencerContext *seq_ctx_p);
			void FcoeGlobalFcoeFsbSendToLocal(FcoeLinearSequencerContext *seq_ctx_p);

            virtual void FcoeLinearSequencerFailedStep (FcoeLinearSequencerContext *seq_ctx_p);
            
            // Rollback message creation functions
            FcoeLocalFcoeMapMessage* 
            FcoeGlobalFcoeMapGetRollbackMessage (PrismMessage *prism_msg_p);

            FcoeLocalFcoeFabricMapMessage *
            FcoeGlobalFcoeFabricMapGetRollbackMessage (PrismMessage *prism_msg_p);
            
            FcoeLocalFcoeFipMessage *
            FcoeGlobalFcoeFipGetRollbackMessage (PrismMessage *prism_msg_p);

            void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

            void upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase);
            ObjectId FcoeFabricMapGetObjectIdByName (string &fab_map_name,
                                                     FcoeLinearSequencerContext *seq_ctx_p = NULL);
            ObjectId FcoeMapGetObjectIdByName (string &map_name,
                                               FcoeLinearSequencerContext *seq_ctx_p = NULL);
            ObjectId CeeMapGetObjectIdByName (string &cee_map_name,
                                              FcoeLinearSequencerContext *seq_ctx_p = NULL);
            
            // Get MOs
            FcoeFabricMapManagedObject  *FcoeFabricMapGetMOByName(string &fab_map_name, FcoeLinearSequencerContext *seq_ctx_p = NULL);
            FcoeMapManagedObject        *FcoeMapGetMOByName (string &map_name, FcoeLinearSequencerContext *seq_ctx_p = NULL);
            FcoeFipManagedObject        *FcoeFipGetMO (FcoeLinearSequencerContext *seq_ctx_p = NULL);
			FcoeGlobalFsbManagedObject  *FcoeFsbGetMO (FcoeLinearSequencerContext *seq_ctx_p = NULL);
            
            void FcoeGlobalSendToClusterCallback (PrismAsynchronousContext *ctx_p);
            void FcoeGlobalTransFailRollbackCompleteCallback (PrismAsynchronousContext *ctx_p);

            bool FcoeFabricMapNeedToCreateRollbackMessage(const UI32 &oper);


            ResourceId FcoeFabricMapVlanUpdateSanityCheck();
            
            ResourceId FcoeGlobalFabricMapValidatePriorityChange(const uint32_t priority,
                                                                 FcoeLinearSequencerContext *seq_ctx_p);
            int isAgModeEnabled(void);
            UI32 recalculateMaxEnodes(FcoeLinearSequencerContext *seq_ctx_p);
           
        protected:
        public:
            FcoeGlobalWorker ( FcoeGlobalObjectManager *pFcoeGlobalObjectManager);
            virtual    ~FcoeGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //FCOEGLOBALWORKER_H
