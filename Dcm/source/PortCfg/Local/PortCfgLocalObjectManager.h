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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef PORTCFGLOCALOBJECTMANAGER_H
#define PORTCFGLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "PortCfg/Local/PortFCCfgLocalManagedObject.h"
#include "Fcoe/Local/FcoeIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"
#include "VcsFabric/Local/VcsNodePortEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "sys/fabos/fabobj.h"


#define UnknownPort 0
#define AG_NPort 1
#define EPort 7
#define GPort 10
#define UPort 11
#define FPort 15
#define LPort 16
#define EXPort 19

using namespace WaveNs;

namespace DcmNs
{
    class PortCfgPortCfgPortStateUpdateMessage;
    class PortCfgPortCfgTrunkUpdateMessage;
    class PortCfgPortCfgCreateMessage;
    class PortCfgPortCfgPortTranslateMessage;
    class PortCfgUpdateFCInfoMessage;
    class PortCfgFCPortTranslateMessage;
    class PortCfgFCPortInterfaceInfoMessage;
    class PortCfgFCPortDetailedInfoMessage;
    class PortCfgFibreChannelClearMessage;
    class PortCfgFibreChannelSfpShowMessage;
    class PortCfgFCPortLoginInfoMessage;
    class PortCfgGetFCPortInfoMessage;

    class PortCfgLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void PortCfgPortCfgCreateMessageHandler( PortCfgPortCfgCreateMessage *pPortCfgPortCfgCreateMessage);
            void PortCfgPortCfgPortStateUpdateMessageHandler( PortCfgPortCfgPortStateUpdateMessage *pPortCfgPortCfgPortStateUpdateMessage);
            void PortCfgPortCfgTrunkUpdateMessageHandler( PortCfgPortCfgTrunkUpdateMessage *pPortCfgPortCfgTrunkUpdateMessage);
            void PortCfgPortCfgPortTranslateMessageHandler( PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage);
            void PortCfgFibreChannelClearMessageHandler( PortCfgFibreChannelClearMessage *pPortCfgFibreChannelClearMessage);

	        int addPortcfgEntry(int slot, int port, int startNewTransation, int switch_port, string porttype, string switchporttype, int vlan, int fcoeport, int ifIndex);
	        int addPortcfgEntry(string id, int startNewTransation, int switch_port, string switchporttype, int ifIndex);
	        void install( WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);
	        ResourceId updateTrunkEnable (PrismSynchronousLinearSequencerContext *pPortCfgPortCfgTrunkUpdateMessageContext);
	        ResourceId updatePortStateEnable (PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortStateUpdateMessageContext);
	        ResourceId createPortCfg (PrismSynchronousLinearSequencerContext *pPortCfgPortCfgCreateMessageContext);
	        ResourceId allocatedObjectCleanup (PrismSynchronousLinearSequencerContext *pMessageContext);
			ResourceId PortTranslateToSwitchPort(PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext);
            bool populatePortTranslationFromRbridgeSwitchport(PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage, PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext);
            bool populatePortTranslationFromIdType(PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage, PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext);
	        int removeDBPortcfgEntry();
            FcoeETIntfLocalManagedObject *queryFcoeETLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId);
            FcoeIntfLocalManagedObject *queryFcoeLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId);
            FcoeETIntfLocalManagedObject *queryFcoeETLocalManagedObjectFromId(string id);
            FcoeIntfLocalManagedObject *queryFcoeIntfLocalManagedObjectFromId(string id);

			void  externalStateSynchronization  (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);


            void PortCfgFCPortTranslateMessageHandler(PortCfgFCPortTranslateMessage *pPortCfgFCPortTranslateMessage);
            ResourceId FCPortTranslateToSwitchPort (PrismSynchronousLinearSequencerContext *pPortCfgFCPortTranslateMessageContext);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);

            // Translate Message for the Netconf CLI for FC      
            void PortCfgFCPortInterfaceInfoMessageHandler( PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);

            ResourceId FCPortInterfaceInfo (PrismSynchronousLinearSequencerContext *pPortCfgFCPortInterfaceInfoMessageContext);

			// register for all of the SCN event types that the port config plugin must handle
			void listenForEvents (WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);

			// handle port disabled SCN events from the kernel
    		void SwitchPortDisableEventHandler (const VcsNodeSwitchPortDisableEvent *&pEvent);


			// method that is invoked at bringup -- will copy port configuration data from the switch driver to
			// the DCMd database
			ResourceId backendSyncUpPortCfg(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            // FC DB Related Message
            void PortCfgUpdateFCInfoMessageHandler( PortCfgUpdateFCInfoMessage *pPortCfgUpdateFCInfoMessage);

            // Create FC DB Entries
            int addFcPortcfgEntries(int startNewTransation);

            // Query FC DB
            PortFCCfgLocalManagedObject *QueryFCLocalManagedObjectById(string id);
            PortFCCfgLocalManagedObject *QueryFCLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId, UI32 limitDisplay = 0);

            // method to update the FC port DB
            ResourceId FCInfoUpdateEntries (PrismSynchronousLinearSequencerContext *pPortCfgUpdateFCInfoMessageContext);

            void sendPortFCCfgSettingsToSwitchDriver(PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject,
                                                    const UI32 operationCode, reason_t disReason);

            // Used for populating Entries from /proc for Stats, Error, PortInfo
            void PortCfgFCPortDetailedInfoMessageHandler( PortCfgFCPortDetailedInfoMessage *pPortCfgFCPortDetailedInfoMessage);
            ResourceId FCPortDetailedInfo (PrismSynchronousLinearSequencerContext *pPortCfgFCPortDetailedInfoMessageContext);
            ResourceId FCPortClearCounters (PrismSynchronousLinearSequencerContext *pPortCfgFibreChannelClearMessageContext);
            // Used for populating SFP Show Details
            void PortCfgFibreChannelSfpShowMessageHandler( PortCfgFibreChannelSfpShowMessage *pPortCfgFibreChannelSfpShowMessage);
            ResourceId FCPortSFPShow (PrismSynchronousLinearSequencerContext *pPortCfgFibreChannelSfpShowMessageContext);
            // Used for population FC port Login Details
            ResourceId FCPortLoginShow (PrismSynchronousLinearSequencerContext *pPortCfgFCPortLoginInfoMessage);
            void PortCfgFCPortLoginInfoMessageHandler( PortCfgFCPortLoginInfoMessage *pPortCfgFCPortLoginInfoMessage);
            // Operate on FC DB Entries and retrieve info
            void PortCfgGetFCPortInfoMessageHandler( PortCfgGetFCPortInfoMessage *pPortCfgGetFCPortInfoMessage);
            ResourceId GetFCPortInfo (PrismSynchronousLinearSequencerContext *pPortCfgGetFCPortInfoMessageContext);

        protected:
        public:
            PortCfgLocalObjectManager ();
            virtual    ~PortCfgLocalObjectManager ();
            static PortCfgLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void backendSyncUp(PrismAsynchronousContext *pPrismAsynchronousContext);

// Now the data members

        private:
			bool m_ConfigPushed;

        protected:
        public:
            enum fc_db_type {
                fc_db_type_speed,
                fc_db_type_fillWord,
                fc_db_type_longDistance,
                fc_db_type_vcLinkInit,
                fc_db_type_islRRdyMode,
                fc_db_type_shutdown,
                fc_db_type_trunk,
				fc_db_type_modelock,
                fc_db_type_desireddistance
            };
    };

    inline string  PortCfgLocalObjectManager::getClassName() 
    { 
        return ("PortCfg"); 
    }

    /* FC Command Codes & Error Code For Clear Counters */
    typedef enum
    {
        FC_CMD_CODE_CLEAR_SUCCESS      = 0,
        FC_CMD_CODE_CLEAR_UPORT        = 1,
        FC_CMD_CODE_CLEAR_ALL          = 2,
        FC_CMD_CODE_CLEAR_SLOTID       = 3,
        FC_CMD_CODE_ERR_CLEAR_UPORT    = 4,
        FC_CMD_CODE_ERR_INVALID_UPORT  = 5
    }FcClearCmdCode;

    /* FC Command Codes For SFP Show */
    typedef enum
    {
        FC_CMD_CODE_ONE_PORT  = 0,
        FC_CMD_CODE_ALL_PORTS = 1
    }FcSfpCmdCode;

    /* FC Command Code for Login Show */
    typedef enum
    {
        FC_CMD_CODE_RBRIDGE_ID               = 0,
        FC_CMD_CODE_INTERFACE_ID             = 1,
        FC_CMD_CODE_ERR_INVALID_INTERFACE_ID = 2,
		FC_CMD_CODE_LOCAL = 3
    }FcLoginCmdCode;

    /* FC Command & Response Codes to retrieve Info */
    typedef enum
    {
        FC_RESP_CODE_NULL          = 0,
        FC_RESP_CODE_ALL_SHUT     = 1,
        FC_RESP_CODE_NOT_ALL_SHUT = 2,
        FC_CMD_CODE_ALL_SHUT      = 3
    }FcGetInfoCmdCode;
}

    /* FC Command Config Modes */
    typedef enum
    {
        FC_CFG_MODE_AUTO     = 1,
        FC_CFG_MODE_E_PORT   = 2,
        FC_CFG_MODE_N_PORT   = 3,
        FC_CFG_MODE_F_PORT   = 4
    }FcConfigModes;
#endif                                            //PORTCFGLOCALOBJECTMANAGER_H
