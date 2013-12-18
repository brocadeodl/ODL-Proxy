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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef ZONEOBJECTMANAGER_H
#define ZONEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/DBAttributesManagedObject.h"
#include "Zone/Global/AliasManagedObject.h"
#include "Zone/Global/AliasMemberManagedObject.h"
#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/CfgMemberManagedObject.h"
#include "Zone/Global/DBAttributesManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/ZoneMemberManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

using namespace WaveNs;


#define ZONE_SAME_DEFAULT_ACCESS_ALL	0x01020000
#define ZONE_SAME_DEFAULT_ACCESS_NONE	0x00010000

namespace DcmNs
{

    class ZoneDbCreateMessage;
    class ZoneDbAddMessage;
    class ZoneDbRemoveMessage;
    class ZoneDbDeleteMessage;
    class ZoneDbSaveMessage;
    class ZoneDbClearMessage;
    class ZoneDbEnableMessage;
    class ZoneDbDisableMessage;
    class ZoneDbTransAbortMessage;
	class ZoneOperationInfoMessage;

    class ZoneDefinedCfgConfigurationHandler;
    class ZoneDefinedCfgMemberConfigurationHandler;
    class ZoneDefinedZoneConfigurationHandler;
    class ZoneDefinedZoneMemberConfigurationHandler;
    class ZoneDefinedAliasConfigurationHandler;
    class ZoneDefinedAliasMemberConfigurationHandler;
    class ZoneBulkConfigurationWorker;

    class ZoneObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void ZoneDbAddMessageHandler( ZoneDbAddMessage *pZoneDbAddMessage);
            void ZoneDbClearMessageHandler( ZoneDbClearMessage *pZoneDbClearMessage);
            void ZoneDbCreateMessageHandler( ZoneDbCreateMessage *pZoneDbCreateMessage);
            void ZoneDbDeleteMessageHandler( ZoneDbDeleteMessage *pZoneDbDeleteMessage);
            void ZoneDbDisableMessageHandler( ZoneDbDisableMessage *pZoneDbDisableMessage);
            void ZoneDbEnableMessageHandler( ZoneDbEnableMessage *pZoneDbEnableMessage);
            void ZoneDbRemoveMessageHandler( ZoneDbRemoveMessage *pZoneDbRemoveMessage);
            void ZoneDbSaveMessageHandler( ZoneDbSaveMessage *pZoneDbSaveMessage);
            void ZoneDbTransAbortMessageHandler( ZoneDbTransAbortMessage *pZoneDbTransAbortMessage);
	        void ZoneOperationInfoMessageHandler( ZoneOperationInfoMessage *pZoneOperationInfoMessage);

            void install( WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);
            void simulatorInstall();
            void realSwitchInstall_optimized(int setCfgActionNone, UI32 bootUpType, UI32 op_type);
	        int clearDcmdZoneDB();
	        int dumpDcmdZoneDB();
            int overwriteDcmdFromZoned(int setCfgActionNone, UI32 bootUpType, UI32 op_type);
            void  externalStateSynchronization  (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
            int removeDBAttributesManagedObject();
            int removeZoneMemberManagedObject();
            int removeZoneManagedObject();
            int removeCfgMemberManagedObject();
            int removeCfgManagedObject();
			int removeAliasMemberManagedObject();
			int removeAliasManagedObject();
            void listenForEvents (WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);
            void zoneChangedEventHandler (const VcsNodeZoneChangedEvent *&pEvent);
            void zoneAbortedEventHandler (const VcsNodeZoneAbortedEvent *&pEvent);
            ResourceId dbCreate (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbRemove (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbDelete (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbEnable (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbDisable (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbSave (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbClear (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext);
            ResourceId dbAbort (PrismSynchronousLinearSequencerContext *pZoneDbTransAbortMessageContext);
            ResourceId getOperationInfo (PrismSynchronousLinearSequencerContext *pZoneOperationInfoMessage);
            CfgManagedObject *queryCfgManagedObjectByName(string cfgName, int loadRelationships);
			int queryCfgMemberManagedObjectForMemberCount(ObjectId parentMoObjectId);
			CfgMemberManagedObject *queryCfgMemberManagedObjectByName(ObjectId parentMoObjectId, string cfgEntry);
            ZoneManagedObject *queryZoneManagedObjectByName(string zoneName, int loadRelationships);
			int queryZoneMemberManagedObjectForMemberCount(ObjectId parentMoObjectId);
			ZoneMemberManagedObject *queryZoneMemberManagedObjectByName(ObjectId parentMoObjectId, string zoneEntry);
            AliasManagedObject *queryAliasManagedObjectByName(string aliasName, int loadRelationships);
			int queryAliasMemberManagedObjectForMemberCount(ObjectId parentMoObjectId);
			AliasMemberManagedObject *queryAliasMemberManagedObjectByName(ObjectId parentMoObjectId, string aliasEntry);
            DBAttributesManagedObject *queryDBAttributesManagedObject(int loadRelationships);
            int updateTransToken(PrismSynchronousLinearSequencerContext *pZoneMessageContext, int cached_token);
            int chkFirstTimeTransFailure(PrismSynchronousLinearSequencerContext *pZoneMessageContext);
	        void failover(FailoverAsynchronousContext *pFailoverAsynchronousContext);
	        void addDBAttributesManagedObjectToFree(DBAttributesManagedObject
	            *pDBAttributesManagedObject);
	        void addCfgManagedObjectToFree(CfgManagedObject
	            *pCfgManagedObject);
	        void addZoneManagedObjectToFree(ZoneManagedObject
	            *pZoneManagedObject);
			void addAliasManagedObjectToFree(AliasManagedObject
	            *pAliasManagedObject);
	        ResourceId freeAllocatedManagedObjects();
	        ResourceId backendWyserZonePluginCreate();
	        vector<CfgManagedObject *> &getCreatedCfgManagedObject();
	        vector<ZoneManagedObject *> &getCreatedZoneManagedObject();
	        vector<AliasManagedObject *> &getCreatedAliasManagedObject();

			int queryIfAliasMOsExist();
			int queryManagedObjectForMemberCount(ObjectId parentMoObjectId, int znObjType);
            string wwnStrToLower(string strToConvert);

			void updateLastZoneCommittedTimestamp(DBAttributesManagedObject *pDBAttributesManagedObject);

        protected:
        public:
            ZoneObjectManager ();
            virtual    ~ZoneObjectManager ();
            static ZoneObjectManager *getInstance();
            static string getClassName();

            static PrismServiceId getPrismServiceId();
			static int zone_db_parse(char *cfg, int size,
							  vector <string> *dCfgName, vector <string> *dZoneNameInCfg,
							  vector <string> *dZoneName, vector <string> *dZoneEntry,
							  vector <string> *eCfgName, vector <string> *eZoneNameInCfg,
							  vector <string> *eZoneName, vector <string> *eZoneEntry,
							  vector <string> *dAliasName, vector <string> *dAliasEntry);
            static ResourceId zoneRespToStatus(UI32 respCode);

			static UI32 loadZoningBulkConfiguration (UI32 argc, vector<string> argv);

// Now the data members

        private:
	        DBAttributesManagedObject *allocatedDBAttributesManagedObject;
	        vector <CfgManagedObject *> allocatedCfgManagedObjects;
	        vector <ZoneManagedObject *> allocatedZoneManagedObjects;
	        vector <AliasManagedObject *> allocatedAliasManagedObjects;

            ZoneDefinedCfgConfigurationHandler *m_pZoneDefinedCfgConfigurationHandler;
            ZoneDefinedCfgMemberConfigurationHandler *m_pZoneDefinedCfgMemberConfigurationHandler;
            ZoneDefinedZoneConfigurationHandler *m_pZoneDefinedZoneConfigurationHandler;
            ZoneDefinedZoneMemberConfigurationHandler *m_pZoneDefinedZoneMemberConfigurationHandler;
			ZoneDefinedAliasConfigurationHandler *m_pZoneDefinedAliasConfigurationHandler;
            ZoneDefinedAliasMemberConfigurationHandler *m_pZoneDefinedAliasMemberConfigurationHandler;
            ZoneBulkConfigurationWorker *m_pZoneBulkConfigurationWorker;

            bool skipSCNReload;
            bool firstTimeTransFailure;

        protected:
        public:
            enum db_type {
                db_type_cfg,
                db_type_zone,
                db_type_def_zone,
				db_type_alias
            };

            enum db_cfg_type {
                db_cfg_type_defined,
                db_cfg_type_effective
            };

            enum db_boot_type {
                db_boot_type_normal = 0,
                db_boot_type_clear = 1
            };

            enum cfgaction_type {
                no_op,
                cfgclear_op,
                cfgsave_op,
                cfgenable_op,
				cfgdisable_op,
                cfgtransabort_op,
                defzone_mode_change_op,
                reloadAll_op
            };
    };

    inline string  ZoneObjectManager::getClassName()
    {
        return ("Zone");
    }

}
#endif                                            //ZONEOBJECTMANAGER_H
