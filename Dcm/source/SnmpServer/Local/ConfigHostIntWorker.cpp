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

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigHostIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigChangeHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetEngineIDIntMessage.h"
#include "SnmpServer/Local/EngineIDLocalManagedObject.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/HostsManagedObject.h"
#include "SnmpServer/Global/V3HostsManagedObject.h"
#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "SnmpServer/Global/SnmpAuthCrypto.h"
#include "SnmpServer/Global/ContextMappingManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include <snmp/snmplib.h>
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#define DEFAULT_ENGINEID {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 9}
#define DEFAULT_IPV4_SIZE 8

namespace DcmNs
{

    ConfigHostIntWorker::ConfigHostIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGADDHOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigHostIntWorker::SnmpServerIntSnmpConfigAddHostIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGDELHOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigHostIntWorker::SnmpServerIntSnmpConfigDelHostIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGCHANGEHOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigHostIntWorker::SnmpServerIntSnmpConfigChangeHostIntMessageHandler));
        addOperationMap (SNMPSERVERINTCONFIGUREDEFAULTS, reinterpret_cast<PrismMessageHandler> (&ConfigHostIntWorker::SnmpServerIntConfigureDefaultsMessageHandler));
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (pSnmpServerIntLocalObjectManager->getServiceId ());
    }

    ConfigHostIntWorker::~ConfigHostIntWorker ()
    {
    }

    PrismMessage  *ConfigHostIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGADDHOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigAddHostIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGDELHOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigDelHostIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGCHANGEHOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigChangeHostIntMessage ();
                break;
            case SNMPSERVERINTCONFIGUREDEFAULTS :
                pPrismMessage = new SnmpServerIntConfigureDefaultsMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigHostIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
	
	int ConfigHostIntWorker::snmpdConfigPush ()
	{
		string CommStr[MAX_COMMUNITY_ENTRIES], userName[MAX_USM_ENTRIES];
		string authPswd[MAX_USM_ENTRIES], privPswd[MAX_USM_ENTRIES];
        string groupName[MAX_USM_ENTRIES];
		string contextName[MAX_CONTEXT_MAPPING_ENTRIES], vrfName[MAX_CONTEXT_MAPPING_ENTRIES];
		int    CommAccess[MAX_COMMUNITY_ENTRIES], authType[MAX_USM_ENTRIES], privType[MAX_USM_ENTRIES];
		bool   encPswdflag[MAX_USM_ENTRIES];
		ObjectId	ObjId[MAX_COMMUNITY_ENTRIES], hostObjId, userObjId[MAX_USM_ENTRIES], hostV3ObjId;
		agtcfg_t agtcfgin;
		HostUC ipVal;
		int ret = 0, j = 0, k = 3, resSize = 0, hc = 0, hv2 = 0, resV3Size = 0, i = 0, rc = 0, resUserSize = 0;
        int usmIdx = 0, tSize = 0;
		int version1 = 0, version2c = 1;
		unsigned char *base64AuthPaswd = NULL;
		unsigned char *base64PrivPaswd = NULL;
		char authPaswd[256];
		char privPaswd[256];
		int encLen = 0;
		ret = agtcfgXMGet(&agtcfgin);
		if (ret == -1) {
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush Get Failed");
			return (-1);
		} 

		/*
		** Read community MO from DB and populate the backend deamon.
		** If no MO's found, clear community structure.
		** If size of MO result is less than default, clear the rest of locations.
		*/

		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new 
			WaveManagedObjectSynchronousQueryContext(
			CommunityManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

		if ((NULL == pResults) || (pResults->size() == 0)) {
			trace (TRACE_LEVEL_INFO,
				"ConfigHostIntWorker::snmpdConfigPush No community MO");
			for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
				memset (agtcfgin.agtcommunityentries[i].communitystr, 0x0, 
						AGT_MAX_SECRET_SIZE);
			}
		} else {
			resSize = pResults->size();
			trace (TRACE_LEVEL_INFO,
				string ("ConfigHostIntWorker::snmpdConfigPush Community resSize : ") +resSize);
			for (i = 0; i < resSize; i ++) {
				CommunityManagedObject *pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResults)[i]);
				if (pCommManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR, 
						"ConfigHostIntWorker::snmpdConfigPush: pCommManagedObject dynamic_cast failed");
				}
				CommStr[i] = pCommManagedObject->getCommunity();
				CommAccess[i] = pCommManagedObject->getAccess();
				ObjId[i]	= pCommManagedObject->getObjectId();
				delete pCommManagedObject;
			}
			
			tSize = (resSize>MAX_COMMUNITY_ENTRIES) ? MAX_COMMUNITY_ENTRIES : resSize;
			for (i = 0; i < tSize; i ++) {
					memset (agtcfgin.agtcommunityentries[j].communitystr, 0x0, 
						AGT_MAX_SECRET_SIZE);
					strncpy(agtcfgin.agtcommunityentries[j].communitystr,
							CommStr[i].c_str(), CommStr[i].length());
					agtcfgin.agtcommunityentries[j].access = CommAccess[i];
					j++;
				}

			for (; j < MAX_COMMUNITY_ENTRIES; j++) {
				memset (agtcfgin.agtcommunityentries[j].communitystr, 0x0,
					   AGT_MAX_SECRET_SIZE);
				agtcfgin.agtcommunityentries[j].access = 0;
			}
		}
		delete synchronousQueryContext;
		delete pResults;

		/*
		** Read host v1 MO from DB and populate the backend deamon.
		** If no MO's found, clear community structure.
		** If size of MO result is less than default, clear the rest of locations.
		*/

		WaveManagedObjectSynchronousQueryContext *syncQueryHost = new 
			WaveManagedObjectSynchronousQueryContext(
			HostsManagedObject::getClassName());
		syncQueryHost->addAndAttribute (new AttributeEnum((UI32 *)&version1, "version"));

		vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);
		if ((NULL == pResultsHost) || (pResultsHost->size() == 0))
		{
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush No such host v1 managed object");
			for (i = 0; i < MAX_AGTPTY_ENTRIES ; i++) {
				memset(agtcfgin.agtptyEntries[i].agt_tAddr, 0x0,
					   TARGET_IP_ADDRESS_SIZE);
				strncpy((char *)agtcfgin.agtptyEntries[i].agt_tAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
				agtcfgin.agtptyEntries[i].agt_port = 162;
                agtcfgin.agtptyEntries[i].agt_trapSeverityLevel = None;
			}
		} else {
			int v1hostSize = pResultsHost->size();
			trace (TRACE_LEVEL_INFO,
				string ("ConfigHostIntWorker::snmpdConfigPush Host v1hostSize : ") +v1hostSize);
			for (hc = 0; hc < v1hostSize; hc ++) {
				HostsManagedObject *pHostManagedObject = dynamic_cast
					<HostsManagedObject *> ((*pResultsHost)[hc]);
				if (pHostManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR,
						"ConfigHostIntWorker::snmpdConfigPush : pHostManagedObject dynamic_cast failed");
				}
				ipVal = pHostManagedObject->getHostUC();
				memset (agtcfgin.agtptyEntries[hc].agt_tAddr, 0x0,
						TARGET_IP_ADDRESS_SIZE);
				memset (agtcfgin.agtptyEntries[hc].nv_commuStr, 0x0,
						AGT_MAX_SECRET_SIZE);
				string ipStr = ipVal.getHostValue();
				strncpy((char *)agtcfgin.agtptyEntries[hc].agt_tAddr,
							ipStr.c_str(), ipStr.length());
				hostObjId = pHostManagedObject->getCommunity();
				//int ObjIdSize = sizeof(ObjId);
				for (int id = 0; id < resSize; id ++) {
					if (hostObjId == ObjId[id]) {
						strncpy((char *)agtcfgin.agtptyEntries[hc].nv_commuStr,
							CommStr[id].c_str(), CommStr[id].length());
						break;
					}
				}
				agtcfgin.agtptyEntries[hc].agt_port = pHostManagedObject->getUdpPort();
				agtcfgin.agtptyEntries[hc].agt_trapSeverityLevel = pHostManagedObject->getSevLevel();
				delete pHostManagedObject;	
			}

			for (; hc < MAX_AGTPTY_ENTRIES; hc ++) {
				strncpy((char *)agtcfgin.agtptyEntries[hc].agt_tAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
				agtcfgin.agtptyEntries[hc].agt_port = 162;
                agtcfgin.agtptyEntries[hc].agt_trapSeverityLevel = None;
			}
		}
		delete syncQueryHost;
		delete pResultsHost;

		/*
		** Read host v2c MO from DB and populate the backend deamon.
		** If no MO's found, clear community structure.
		** If size of MO result is less than default, clear the rest of locations.
		*/
		WaveManagedObjectSynchronousQueryContext *syncQueryvHost = new 
			WaveManagedObjectSynchronousQueryContext(
			HostsManagedObject::getClassName());
		syncQueryvHost->addAndAttribute (new 
			AttributeEnum((UI32 *)&version2c, "version"));

		vector<WaveManagedObject *> *vResultsHost = querySynchronously (syncQueryvHost);
		if ((NULL == vResultsHost) || (vResultsHost->size() == 0))
		{
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush No such host v2c managed object");
			for (i = 0; i < MAX_AGTPTY_ENTRIES ; i++) {
				memset(agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr, 0x0,
					   TARGET_IP_ADDRESS_SIZE);
				strncpy((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
				agtcfgin.agtptyv2cEntries[i].agtv2c_port = 162;
				agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel = None;
			}
		} else {
			int hostSize = vResultsHost->size();
			trace (TRACE_LEVEL_INFO,
				string ("ConfigHostIntWorker::snmpdConfigPush Host hostSize : ") +hostSize);
			for (hv2 = 0; hv2 < hostSize; hv2 ++) {
				HostsManagedObject *vHostManagedObject = dynamic_cast
					<HostsManagedObject *> ((*vResultsHost)[hv2]);
				if (vHostManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR,
						"ConfigHostIntWorker::snmpdConfigPush : vHostManagedObject dynamic_cast failed");
				}
				ipVal = vHostManagedObject->getHostUC();
				memset (agtcfgin.agtptyv2cEntries[hv2].agtv2c_tAddr, 0x0,
						TARGET_IP_ADDRESS_SIZE);
				memset (agtcfgin.agtptyv2cEntries[hv2].nv_commuStrv2c, 0x0,
						AGT_MAX_SECRET_SIZE);
				string ipStr = ipVal.getHostValue();
				strncpy((char *)agtcfgin.agtptyv2cEntries[hv2].agtv2c_tAddr,
							ipStr.c_str(), ipStr.length());
				hostObjId = vHostManagedObject->getCommunity();
		//		int ObjIdSize = sizeof(ObjId);
				for (int id = 0; id < resSize; id ++) {
					if (hostObjId == ObjId[id]) {
						strncpy((char *)agtcfgin.agtptyv2cEntries[hv2].nv_commuStrv2c,
							CommStr[id].c_str(), CommStr[id].length());
						break;
					}
				}
				agtcfgin.agtptyv2cEntries[hv2].agtv2c_port = vHostManagedObject->getUdpPort();
				agtcfgin.agtptyv2cEntries[hv2].agtv2c_trapSeverityLevel = vHostManagedObject->getSevLevel();
				delete vHostManagedObject;	
			}
			for (; hv2 < MAX_AGTPTY_ENTRIES; hv2 ++) {
				strncpy ((char *)agtcfgin.agtptyv2cEntries[hv2].agtv2c_tAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
				agtcfgin.agtptyv2cEntries[hv2].agtv2c_port = 162;
				agtcfgin.agtptyv2cEntries[hv2].agtv2c_trapSeverityLevel = None; 
			}
		}

		delete syncQueryvHost;
		delete vResultsHost;
		/*
		** Read systemgroup v1 MO from DB and populate the backend deamon.
		** If no MO's found, do nothing since no operation doesnt delete the MOs.
		** If size of MO result is less than default, clear the rest of locations.
		*/

		SystemGroupManagedObject *pSysGroupMO = NULL;
		WaveManagedObjectSynchronousQueryContext *syncQuerySysGroup = new 
			WaveManagedObjectSynchronousQueryContext(
			SystemGroupManagedObject::getClassName());

		vector<WaveManagedObject *> *pResultsSG = querySynchronously (syncQuerySysGroup);
		if ((NULL == pResultsSG) || (pResultsSG->size() == 0)) {
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush No such SG managed object");
		} else {
            pSysGroupMO = dynamic_cast<SystemGroupManagedObject *> ((*pResultsSG)[0]);
			if (pSysGroupMO == NULL) {
				trace (TRACE_LEVEL_ERROR, 
					"ConfigHostIntWorker::snmpdConfigPush : pSysGroupMO dynamic_cast failed");
			}
            string contactStr = pSysGroupMO->getContact();
            string locationStr = pSysGroupMO->getLocation();
            string sysDescrStr = pSysGroupMO->getSysDescr();

            memset (agtcfgin.sysContact, 0x0, M2DISPLAYSTRSIZE);
            strncpy( (char *)agtcfgin.sysContact, contactStr.c_str(),
                    contactStr.length());
            memset (agtcfgin.sysLocation, 0x0, M2DISPLAYSTRSIZE);
            strncpy( (char *)agtcfgin.sysLocation, locationStr.c_str(),
                    locationStr.length());
            memset (agtcfgin.sysDescr, 0x0, M2DISPLAYSTRSIZE);
            strncpy( (char *)agtcfgin.sysDescr, sysDescrStr.c_str(),
                    sysDescrStr.length());
            delete pSysGroupMO;
        }
        delete pResultsSG;
        delete syncQuerySysGroup;

		/*
		** Read user MO from DB and populate the backend deamon.
		** If no MO's found, clear user structure.
		** If size of MO result is less than default, clear the rest of locations.
		*/

		WaveManagedObjectSynchronousQueryContext *syncQueryUser = new 
			WaveManagedObjectSynchronousQueryContext(
			SnmpUserManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsUser = querySynchronously (syncQueryUser);
        
        int idx = 0;
		j = 0, k = 3;

		if ((NULL == pResultsUser) || (pResultsUser->size() == 0)) {
			trace (TRACE_LEVEL_INFO,
				"ConfigHostIntWorker::snmpdConfigPush No user MO");
			for (i = 0; i < MAX_USM_ENTRIES; i++) {
				memset (agtcfgin.agtusmEntries[i].userName, 0x0, 
						MAX_USERNAME_SIZE);
				agtcfgin.agtusmEntries[i].agt_encpswdflag   = false;
                agtcfgin.agtusmEntries[i].authProtocol = NO_AUTH_PROTOCOL;
                agtcfgin.agtusmEntries[i].privProtocol = NO_PRIV_PROTOCOL;
                agtcfgin.agtusmEntries[i].agt_privSecret[0] = '\0';
                agtcfgin.agtusmEntries[i].agt_authSecret[0] = '\0';
			}
		} else {
			resUserSize = pResultsUser->size();
			trace (TRACE_LEVEL_INFO,
			 	string ("ConfigHostIntWorker::snmpdConfigPush Got User MO resUserSize : ") +resUserSize);
			for (i = 0; i < resUserSize; i++) {
				SnmpUserManagedObject *pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsUser)[i]);
				if (pUserManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR, 
						"ConfigHostIntWorker::snmpdConfigPush: pUserManagedObject dynamic_cast failed");
				}
				userName[i] = pUserManagedObject->getUsername();
				encPswdflag[i] = pUserManagedObject->getEncpswdflag();
				groupName[i] = pUserManagedObject->getGroupname();
				userObjId[i] = pUserManagedObject->getObjectId();
                authType[i]  = pUserManagedObject->getAuth();
                authPswd[i]  = pUserManagedObject->getAuthpswd();
                privType[i]  = pUserManagedObject->getPriv();
                privPswd[i]  = pUserManagedObject->getPrivpswd();

				base64AuthPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)authPswd[i].c_str(),&encLen);
				if (base64AuthPaswd != NULL)
				{
					memset(authPaswd, 0, 256);
					strncpy(authPaswd,(char *)base64AuthPaswd,encLen);	
					authPaswd[encLen+1] = '\0';
					authPswd[i].assign(authPaswd);
					free(base64AuthPaswd);
				}
				encLen = 0;
				if (!privPswd[i].empty()) 
				{
					base64PrivPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)privPswd[i].c_str(),&encLen);
					if (base64PrivPaswd != NULL)
					{
						memset(privPaswd, 0, 256);
						strncpy(privPaswd,(char *)base64PrivPaswd,encLen);
						privPaswd[encLen+1] = '\0';
						privPswd[i].assign(privPaswd);	
						free(base64PrivPaswd);
					}
				}
				delete pUserManagedObject;
			}

			for (i = 0; i < resUserSize; i ++) {
				if (groupName[i].compare("snmpadmin") == 0) {
                    idx = j;
                    j++;
                } else if (groupName[i].compare("snmpuser") == 0) {
                    idx = k;
                    k++;
                }

                memset (agtcfgin.agtusmEntries[idx].userName, 0x0, 
                    MAX_USERNAME_SIZE);
                strncpy((char *)agtcfgin.agtusmEntries[idx].userName,
                        userName[i].c_str(), userName[i].length());
                 agtcfgin.agtusmEntries[idx].agt_authSecret[0] = '\0';
				agtcfgin.agtusmEntries[idx].agt_encpswdflag   = encPswdflag[i];

                 if (authType[i] != AuthNone) {
                     memset (agtcfgin.agtusmEntries[idx].agt_authSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                     strncpy((char *)agtcfgin.agtusmEntries[idx].agt_authSecret,
							 authPswd[i].c_str(), authPswd[i].length());
                 }
                 agtcfgin.agtusmEntries[idx].authProtocol = authType[i]+1;
                 switch (privType[i]) {
                     case PrivNone:
                         agtcfgin.agtusmEntries[idx].privProtocol = NO_PRIV_PROTOCOL;
                         break;
                     case DES:
                         agtcfgin.agtusmEntries[idx].privProtocol = PRIV_PROTOCOL_DES;
                         break;
                     case AES128:
                         agtcfgin.agtusmEntries[idx].privProtocol = PRIV_PROTOCOL_AES_128;
                         break;
                 }
                 agtcfgin.agtusmEntries[idx].agt_privSecret[0] = '\0';
                 if ( privType[i] != PrivNone) {
                    memset (agtcfgin.agtusmEntries[idx].agt_privSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                    strncpy ((char *)agtcfgin.agtusmEntries[idx].agt_privSecret,
							privPswd[i].c_str(), privPswd[i].length());
                 }
			}

			for (; j < (MAX_USM_ENTRIES/2); j ++) {
				memset (agtcfgin.agtusmEntries[j].userName, 0x0,
					   MAX_USERNAME_SIZE);
				agtcfgin.agtusmEntries[j].agt_encpswdflag   = false;
                agtcfgin.agtusmEntries[j].authProtocol = NO_AUTH_PROTOCOL;
                agtcfgin.agtusmEntries[j].privProtocol = NO_PRIV_PROTOCOL;
                agtcfgin.agtusmEntries[j].agt_privSecret[0] = '\0';
                agtcfgin.agtusmEntries[j].agt_authSecret[0] = '\0';
                                                                
			}
			for (; k < MAX_USM_ENTRIES; k ++) {
				memset (agtcfgin.agtusmEntries[k].userName, 0x0,
					   MAX_USERNAME_SIZE);
				agtcfgin.agtusmEntries[k].agt_encpswdflag   = false;
	            agtcfgin.agtusmEntries[k].authProtocol = NO_AUTH_PROTOCOL;
                agtcfgin.agtusmEntries[k].privProtocol = NO_PRIV_PROTOCOL;
                agtcfgin.agtusmEntries[k].agt_privSecret[0] = '\0';
                agtcfgin.agtusmEntries[k].agt_authSecret[0] = '\0';
            }
        }
		delete syncQueryUser;
		delete pResultsUser;

        /*
         ** Read v3 Host configuration from DB and populate the backend deamon.
         ** If No MOs found then delete corresponding entries from SNMP DB.
         ** Push the config from DB and for rest of the entries, clear SNMP DB. 
         */
        fixed_OctetString manager_engineID[6] = {
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID
        };

        /* To get rid of x86 unused variable warning compiler error */
        manager_engineID[0].length = 9;

        string engineId;
        int mRet = snmpManagerEngineIdsGet(&(manager_engineID[0]));
 		if (mRet == -1)
    		trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush:snmpManagerEngineIdsGet failed" );
       
        WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new 
			WaveManagedObjectSynchronousQueryContext(
			V3HostsManagedObject::getClassName());

		vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);
		if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0))
		{
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::snmpdConfigPush No such host v3 managed object");
			for (i = 0; i < MAX_AGTPTY_ENTRIES ; i++) {
				memset(agtcfgin.agtV3TrapEntries[i].targetAddr, 0x0,
					   TARGET_IP_ADDRESS_SIZE);
				strncpy((char *)agtcfgin.agtV3TrapEntries[i].targetAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
				agtcfgin.agtV3TrapEntries[i].agt_port = 162;
                agtcfgin.agtV3TrapEntries[i].informenabled = 0;
                agtcfgin.agtV3TrapEntries[i].severityLevel = None;
                convertStringtoEngineID((char*)"00:00:00:00:00:00:00:00:00", &manager_engineID[i]);
			}
		} else {
			resV3Size = pResultsV3Host->size();
			for (hc = 0; hc < resV3Size; hc ++) {
				V3HostsManagedObject *pV3HostManagedObject = dynamic_cast
					<V3HostsManagedObject *> ((*pResultsV3Host)[hc]);
				if (pV3HostManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR, 
						"ConfigHostIntWorker::snmpdConfigPush: pV3HostManagedObject dynamic_cast failed");
				}
				ipVal = pV3HostManagedObject->getHostUC();
				memset (agtcfgin.agtV3TrapEntries[hc].targetAddr, 0x0,
						TARGET_IP_ADDRESS_SIZE);
				string ipStr = ipVal.getHostValue();
				strncpy((char *)agtcfgin.agtV3TrapEntries[hc].targetAddr,
							ipStr.c_str(), ipStr.length());
                
				hostV3ObjId = pV3HostManagedObject->getUsername();
				int userObjIdSize = sizeof(userObjId);
				for (int id = 0; id < userObjIdSize; id ++) {
					if (hostV3ObjId == userObjId[id]) {
                        for (usmIdx = 0; usmIdx < MAX_USM_ENTRIES; usmIdx ++)
                        {
                            if (userName[id].compare((char *)agtcfgin.agtusmEntries[usmIdx].userName) == 0)
                                break;
                        }
						agtcfgin.agtV3TrapEntries[hc].usmIndex = usmIdx + 1;
						break;
					}
				}
                
				agtcfgin.agtV3TrapEntries[hc].agt_port = pV3HostManagedObject->getUdpPort();
                agtcfgin.agtV3TrapEntries[hc].informenabled = pV3HostManagedObject->getNotify();
                agtcfgin.agtV3TrapEntries[hc].severityLevel = pV3HostManagedObject->getSevLevel();
                engineId = pV3HostManagedObject->getEngineid();
                char engine_id[256];
                memset (engine_id, 0x0, 256);
                if (agtcfgin.agtV3TrapEntries[hc].informenabled == informs) {
                    strncpy(engine_id, engineId.c_str(), engineId.length());
                    convertStringtoEngineID(engine_id, &manager_engineID[hc]);
                } else {
                    convertStringtoEngineID((char*)"00:00:00:00:00:00:00:00:00", &manager_engineID[hc]);
                }
				delete pV3HostManagedObject;	
			}

			for (; hc < MAX_AGTPTY_ENTRIES; hc ++) {
				strncpy((char *)agtcfgin.agtV3TrapEntries[hc].targetAddr,
						"0.0.0.0", DEFAULT_IPV4_SIZE);
                convertStringtoEngineID((char*)"00:00:00:00:00:00:00:00:00", &manager_engineID[hc]);
                agtcfgin.agtV3TrapEntries[hc].informenabled = 0;
				agtcfgin.agtV3TrapEntries[hc].agt_port = 162;
                agtcfgin.agtV3TrapEntries[hc].severityLevel = None;
			}
		}
		delete syncQueryV3Host;
		delete pResultsV3Host;

		/*
		** Read contextMapping MO from DB and populate the backend deamon.
		** If no MO's found, clear context structure.
		** If size of MO result is less than default, clear the rest of locations.
		*/

		j = 0;
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContextMapping = new 
			WaveManagedObjectSynchronousQueryContext(
			ContextMappingManagedObject::getClassName());
		vector<WaveManagedObject *> *pContexts = querySynchronously (synchronousQueryContextMapping);

		if ((NULL == pContexts) || (pContexts->size() == 0)) {
			trace (TRACE_LEVEL_INFO,
				"ConfigHostIntWorker::snmpdConfigPush No Context MO");
			for (i = 0; i < MAX_CONTEXT_MAPPING_ENTRIES; i++) {
				memset (agtcfgin.agtContextMap[i].contextName, 0x0, 
						AGT_MAX_CONTEXT_SIZE);
				memset (agtcfgin.agtContextMap[i].vrfName, 0x0, 
						AGT_MAX_CONTEXT_SIZE);
			}
		} else {
			resSize = pContexts->size();
			trace (TRACE_LEVEL_INFO,
				string ("ConfigHostIntWorker::snmpdConfigPush Context resSize : ") +resSize);
			for (i = 0; i < resSize; i ++) {
		ContextMappingManagedObject *pContextManagedObject = dynamic_cast<ContextMappingManagedObject *> ((*pContexts)[i]);
				if (pContextManagedObject == NULL) {
					trace (TRACE_LEVEL_ERROR, 
						"ConfigHostIntWorker::snmpdConfigPush: pContextManagedObject dynamic_cast failed");
				}

				contextName[i] = pContextManagedObject->getContextName();
                vrfName[i] = pContextManagedObject->getVrfName();

				delete pContextManagedObject;
			}
			
			tSize = (resSize>MAX_CONTEXT_MAPPING_ENTRIES) ? MAX_CONTEXT_MAPPING_ENTRIES : resSize;
			for (i = 0; i < tSize; i ++) {
				memset (agtcfgin.agtContextMap[j].contextName, 0x0,
                        AGT_MAX_CONTEXT_SIZE);
				strncpy(agtcfgin.agtContextMap[j].contextName, contextName[i].c_str(), 
						contextName[i].length());

                memset (agtcfgin.agtContextMap[j].vrfName, 0x0,
                        AGT_MAX_CONTEXT_SIZE);
				strncpy(agtcfgin.agtContextMap[j].vrfName, vrfName[i].c_str(), 
						vrfName[i].length());

					j++;
				}

			for (; j < MAX_CONTEXT_MAPPING_ENTRIES; j++) {
				memset (agtcfgin.agtContextMap[j].contextName, 0x0, 
						AGT_MAX_CONTEXT_SIZE);
				memset (agtcfgin.agtContextMap[j].vrfName, 0x0, 
						AGT_MAX_CONTEXT_SIZE);
			}
		}
		delete synchronousQueryContextMapping;
		delete pContexts;


			EngineIDLocalManagedObject *pSnmpLocEngIDMO = NULL;
            LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
            vector<WaveManagedObject *> *pResultsEngID = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, EngineIDLocalManagedObject::getClassName());

            if ((NULL == pResultsEngID) || (pResultsEngID->size() != 1))    {
            trace (TRACE_LEVEL_INFO, "creating new entry in db");
            if ((pResultsEngID == NULL) || (pResultsEngID->size() == 0))
                trace (TRACE_LEVEL_INFO, "Zero engineID instances found.");

			char DefEngineId[LOCALENGINEID] = "0:0:0:0:0:0:0:0:0:0:0:0:0";	

				memset (agtcfgin.localEngineId, 0x0, LOCALENGINEID);
				strncpy ((char *)agtcfgin.localEngineId, DefEngineId, LOCALENGINEID);

            }
          else if (1 == pResultsEngID->size())
            {
			pSnmpLocEngIDMO = dynamic_cast<EngineIDLocalManagedObject *> ((*pResultsEngID)[0]);
			string EngineID = pSnmpLocEngIDMO->getEngineID();

                memset (agtcfgin.localEngineId, 0x0, LOCALENGINEID);
                strncpy((char *)agtcfgin.localEngineId, EngineID.c_str(), LOCALENGINEID);

		}
            if (pResultsEngID)
                delete pResultsEngID;
            if (pSnmpLocEngIDMO)
                delete pSnmpLocEngIDMO;


		/* Final push to SNMPd */
        rc = snmpManagerEngineIdsSet(&(manager_engineID[0]));
		ret = agtcfgXMSet(&agtcfgin);
		if (ret == -1) {
			trace (TRACE_LEVEL_ERROR, "ConfigHostIntWorker::snmpdConfigPush Set failed");
			return (-1);
		}

		if (rc == -1) {
			trace (TRACE_LEVEL_ERROR, "ConfigHostIntWorker::snmpdConfigPush Engine ID Set failed");
			return (-1);
		}

		return (0);
	}

	void ConfigHostIntWorker::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases) {
		WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::boot");
		int status = WAVE_MESSAGE_SUCCESS, ret = 0;
        if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) ||
			(WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason)) {
			trace (TRACE_LEVEL_INFO, string("ConfigHostIntWorker::boot reason code ") +bootReason);
			ret = snmpdConfigPush();
            if (ret == -1)
                status = WAVE_MESSAGE_SUCCESS;
		}
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	    pWaveAsynchronousContextForBootPhases->callback ();

	}

	/*
	** This function is the FSS stage handler for SNMPd.
	** Called from: SNMPd FSS handler each time SNMPd cold starts.
	** Inputs: 		Stage number (3. SNMPD_CONFIG_PUSH) and service Type (FCSW)
	** Functions:	Reads MOs from database and populates the backend to make sure sync.
	*/

	void  ConfigHostIntWorker::externalStateSynchronization (
			WaveAsynchronousContextForExternalStateSynchronization 
			*pWaveAsynchronousContextForExternalStateSynchronization)
    {
		int ret = 0, status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_INFO, "ConfigHostIntWorker::externalStateSynchronization");
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 3)
			&& (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
			trace (TRACE_LEVEL_INFO, "Entering External State Sync");
			ret = snmpdConfigPush();
			if (ret == -1) {
				status = WAVE_MESSAGE_SUCCESS;
				trace (TRACE_LEVEL_INFO, 
					"ConfigHostIntWorker::externalStateSynchronization: snmpdConfigPush failed");
			}
        }
	    pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
	    pWaveAsynchronousContextForExternalStateSynchronization->callback ();

    }

	/*
	** This function is the local message handler for adding a new host to SNMPd.
	** Called from: Global Message handler of add host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, finds an empty entry (0.0.0.0)
	** Uses this entry to populate the new host.
	** Only 6 hosts of each version can be configured.
	*/

    void  ConfigHostIntWorker::SnmpServerIntSnmpConfigAddHostIntMessageHandler( SnmpServerIntSnmpConfigAddHostIntMessage *pSnmpServerIntSnmpConfigAddHostIntMessage)
	{
		if (pSnmpServerIntSnmpConfigAddHostIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Add Host Global:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		trace (TRACE_LEVEL_INFO, "Host Local message handler Add: Coming here");
		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1;
		unsigned int i = 0;
		string ipStr, ipVal;
		string commStrVal = pSnmpServerIntSnmpConfigAddHostIntMessage->getCommunity();
		Version versionVal = pSnmpServerIntSnmpConfigAddHostIntMessage->getVersion();
		HostUC ipAddr = pSnmpServerIntSnmpConfigAddHostIntMessage->getHostUC();
		int udpPort = pSnmpServerIntSnmpConfigAddHostIntMessage->getUdpPort();
		int sevLevel = pSnmpServerIntSnmpConfigAddHostIntMessage->getSevLevel();
		
		ipStr.assign("0.0.0.0");
		ipVal = ipAddr.getHostValue();
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1)
		trace (TRACE_LEVEL_INFO, "Host Local message handler ADD: GET success" );

		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
			if (versionVal == v1) {
				if ((ipVal.compare((char *)agtcfgin.agtptyEntries[i].agt_tAddr) == 0) &&
    	            (commStrVal.compare((char *)agtcfgin.agtptyEntries[i].nv_commuStr) == 0)) {
                    found = 0;
    			   break;
				}
			} else if (versionVal == v2c) {
				if ((ipVal.compare((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr) == 0) &&
                    (commStrVal.compare((char *)agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c) == 0)) {
                    found = 0;
                   break;
                } 
			}
		}

		if (found != 0) {

			for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
				if (versionVal == v1) {
					if (ipStr.compare((char *)agtcfgin.agtptyEntries[i].agt_tAddr) == 0) {
						trace (TRACE_LEVEL_INFO, string("Add this") +i);
						memset (agtcfgin.agtptyEntries[i].agt_tAddr, 0x0, TARGET_IP_ADDRESS_SIZE);
						memset (agtcfgin.agtptyEntries[i].nv_commuStr, 0x0, AGT_MAX_SECRET_SIZE);
						strncpy((char *)agtcfgin.agtptyEntries[i].agt_tAddr,
							ipVal.c_str(), ipVal.length());
						strncpy((char *)agtcfgin.agtptyEntries[i].nv_commuStr,
								commStrVal.c_str(), commStrVal.length());
						agtcfgin.agtptyEntries[i].agt_port = udpPort;
						agtcfgin.agtptyEntries[i].agt_trapSeverityLevel = sevLevel;
						found = 1;
						Sret = agtcfgXMSet(&agtcfgin);
						break;
					}
				} else if (versionVal == v2c) {
                	if (ipStr.compare((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr) == 0) {
						trace (TRACE_LEVEL_INFO, string("Add this") +i);
						memset (agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr, 0x0, TARGET_IP_ADDRESS_SIZE);
	                    memset (agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c, 0x0, AGT_MAX_SECRET_SIZE);
						strncpy((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr,
								ipVal.c_str(), ipVal.length());
						strncpy((char *)agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c,
								commStrVal.c_str(), commStrVal.length());
						agtcfgin.agtptyv2cEntries[i].agtv2c_port = udpPort;
						agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel = sevLevel;
						Sret = agtcfgXMSet(&agtcfgin);
						found = 1;
						break;
					}
				}
			}
		}

		if (found == -1) {
			trace (TRACE_LEVEL_INFO, "Host Local message handler ADD Host: No empty entry");
			pSnmpServerIntSnmpConfigAddHostIntMessage->setCompletionStatus(WRC_SNMP_NUM_HOST_EXCEEDED_ERROR);
		} else if (found == 1 && Sret == -1) {
			trace (TRACE_LEVEL_INFO, "Host Local message handler ADD: SET failed" );
			pSnmpServerIntSnmpConfigAddHostIntMessage->setCompletionStatus(WRC_SNMP_SET_HOST_FAILED);
		} else {
			pSnmpServerIntSnmpConfigAddHostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		}
		reply(pSnmpServerIntSnmpConfigAddHostIntMessage);
	}

	/*
	** This function is the local message handler for changing attributes of host to SNMPd.
	** Called from: Global Message handler of change host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, determines if the host is v1 or v2c.
	** Finds an empty entry (0.0.0.0) in corresponding structure and populates it.
	** If a port has to be changed and and entry match has been found in v1 then only port is updated.
	*/

    void  ConfigHostIntWorker::SnmpServerIntSnmpConfigChangeHostIntMessageHandler( SnmpServerIntSnmpConfigChangeHostIntMessage *pSnmpServerIntSnmpConfigChangeHostIntMessage)
	{
		
		trace (TRACE_LEVEL_INFO, "Host Local message handler Change: Coming here");
		if (pSnmpServerIntSnmpConfigChangeHostIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Change Host Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1;
		int i = 0, j = 0, portNum = 0;
		string ipStr, ipCheck;
		ipCheck.assign ("0.0.0.0");
		string commStrVal = pSnmpServerIntSnmpConfigChangeHostIntMessage->getCommunity();
		Version versionVal = pSnmpServerIntSnmpConfigChangeHostIntMessage->getVersion();
		HostUC ipAddr = pSnmpServerIntSnmpConfigChangeHostIntMessage->getHostUC();
		int udpPort = pSnmpServerIntSnmpConfigChangeHostIntMessage->getUdpPort();
		int sevLevel = pSnmpServerIntSnmpConfigChangeHostIntMessage->getSevLevel();
		
		ipStr = ipAddr.getHostValue();
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1)
		trace (TRACE_LEVEL_INFO, "Host Local message handler CHANGE: GET success" +commStrVal);

		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
			if ((ipStr.compare((char *)agtcfgin.agtptyEntries[i].agt_tAddr) == 0 ) 
				&& (commStrVal.compare((char *)agtcfgin.agtptyEntries[i].nv_commuStr) == 0)) {
				trace (TRACE_LEVEL_INFO, "Host CHANGE: Got v1 match" + commStrVal);
				if (versionVal == v1) {
					agtcfgin.agtptyEntries[i].agt_port = udpPort;
					agtcfgin.agtptyEntries[i].agt_trapSeverityLevel = sevLevel;
					found = 1;
					Sret = agtcfgXMSet(&agtcfgin);
					break;
				} else if (versionVal == v2c) {
					for (j = 0; j < MAX_AGTPTYV2_ENTRIES; j++) {
						if (ipCheck.compare((char *)agtcfgin.agtptyv2cEntries[j].agtv2c_tAddr) == 0) {
							found = 1;
							trace (TRACE_LEVEL_INFO, "Host CHANGE: Got empty v2c entry");
							memset (agtcfgin.agtptyv2cEntries[j].agtv2c_tAddr,
									0x0, TARGET_IP_ADDRESS_SIZE);
		                    memset (agtcfgin.agtptyv2cEntries[j].nv_commuStrv2c,
									0x0, AGT_MAX_SECRET_SIZE);
							strncpy ((char *)agtcfgin.agtptyv2cEntries[j].agtv2c_tAddr,
										(char *)agtcfgin.agtptyEntries[i].agt_tAddr,
										sizeof (agtcfgin.agtptyEntries[i].agt_tAddr));
							strncpy ((char *)agtcfgin.agtptyv2cEntries[j].nv_commuStrv2c,
										(char *)agtcfgin.agtptyEntries[i].nv_commuStr,
										sizeof (agtcfgin.agtptyEntries[i].nv_commuStr));
							portNum = agtcfgin.agtptyEntries[i].agt_port;	
							agtcfgin.agtptyv2cEntries[j].agtv2c_port = portNum;
							agtcfgin.agtptyv2cEntries[j].agtv2c_trapSeverityLevel = 
										agtcfgin.agtptyEntries[i].agt_trapSeverityLevel;
							Sret = agtcfgXMSet(&agtcfgin);
							strncpy((char *)agtcfgin.agtptyEntries[i].agt_tAddr,
									"0.0.0.0", DEFAULT_IPV4_SIZE);
							agtcfgin.agtptyEntries[i].agt_port = 162;
							agtcfgin.agtptyEntries[i].agt_trapSeverityLevel = None;
							Sret = agtcfgXMSet(&agtcfgin);
							break;
						}
					}
				}
			} else if ((ipStr.compare((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr) == 0) 
				&& (commStrVal.compare((char *)agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c) == 0)) {
				trace (TRACE_LEVEL_INFO, " Host CHANGE: Got v2c match" +commStrVal);
				if (versionVal == v2c) {
					agtcfgin.agtptyv2cEntries[i].agtv2c_port = udpPort;
					agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel = sevLevel;
					found = 1;
					Sret = agtcfgXMSet(&agtcfgin);
					break;
				} else if (versionVal == v1) {
					for (j = 0; j < MAX_AGTPTY_ENTRIES; j++) {
						if (ipCheck.compare((char *)agtcfgin.agtptyEntries[j].agt_tAddr) == 0) {
							found = 1;
							trace (TRACE_LEVEL_INFO, " Host CHANGE: Got empty v1 entry");
							memset (agtcfgin.agtptyEntries[j].agt_tAddr,
                                    0x0, TARGET_IP_ADDRESS_SIZE);
                            memset (agtcfgin.agtptyEntries[j].nv_commuStr,
                                    0x0, AGT_MAX_SECRET_SIZE);
							strncpy ((char *)agtcfgin.agtptyEntries[j].agt_tAddr,
										(char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr,
										sizeof (agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr));
							strncpy ((char *)agtcfgin.agtptyEntries[j].nv_commuStr,
										(char *)agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c,
										sizeof (agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c));
							agtcfgin.agtptyEntries[j].agt_port = agtcfgin.agtptyv2cEntries[i].agtv2c_port;
							agtcfgin.agtptyEntries[j].agt_trapSeverityLevel =
											agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel;
							Sret = agtcfgXMSet(&agtcfgin);
							strncpy((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr,
									"0.0.0.0", DEFAULT_IPV4_SIZE);
							agtcfgin.agtptyv2cEntries[i].agtv2c_port = 162;
							agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel = None;
							Sret = agtcfgXMSet(&agtcfgin);
							break;
						}
					}
				}
			}
		}
		
		if (found == -1) {
			trace (TRACE_LEVEL_INFO, " Host Local message handler CHANGE: No empty entry" );
			pSnmpServerIntSnmpConfigChangeHostIntMessage->setCompletionStatus(WRC_SNMP_NUM_HOST_EXCEEDED_ERROR);
		} else if (Sret == -1) {
			trace (TRACE_LEVEL_INFO, " Host Local message handler CHANGE: Change failed" );
			pSnmpServerIntSnmpConfigChangeHostIntMessage->setCompletionStatus(WRC_SNMP_SET_HOST_FAILED);
		} else pSnmpServerIntSnmpConfigChangeHostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

		reply(pSnmpServerIntSnmpConfigChangeHostIntMessage);

	}
	/*
	** This function is the local message handler for deleting host from SNMPd.
	** Called from: Global Message handler of delete host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, determines. Find the match and populate
	** that entry with empty entry (0.0.0.0).
	*/

    void  ConfigHostIntWorker::SnmpServerIntSnmpConfigDelHostIntMessageHandler( SnmpServerIntSnmpConfigDelHostIntMessage *pSnmpServerIntSnmpConfigDelHostIntMessage)
    {
		trace (TRACE_LEVEL_INFO, " Host Local message handler Del: Coming here");
		if (pSnmpServerIntSnmpConfigDelHostIntMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Del Host Local:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		agtcfg_t agtcfgin;
		int ret = -1, found = 0, Sret = -1;
		unsigned int i = 0;
		string ipStr;
		string commStrVal = pSnmpServerIntSnmpConfigDelHostIntMessage->getCommunity();
		HostUC ipAddr = pSnmpServerIntSnmpConfigDelHostIntMessage->getHostUC();
		ipStr = ipAddr.getHostValue();
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1)
		trace (TRACE_LEVEL_INFO, " Host Local message handler REMOVE: GET success");

		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
				if ((ipStr.compare((char *)agtcfgin.agtptyEntries[i].agt_tAddr) == 0) 
				&& (commStrVal.compare((char *)agtcfgin.agtptyEntries[i].nv_commuStr) == 0)) {
					strncpy((char *)agtcfgin.agtptyEntries[i].agt_tAddr,
							"0.0.0.0", DEFAULT_IPV4_SIZE);
					agtcfgin.agtptyEntries[i].agt_port = 162;
					agtcfgin.agtptyEntries[i].agt_trapSeverityLevel = None;
					found = 1;
					break;
				}
				if ((ipStr.compare((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr) == 0) 
				&& (commStrVal.compare((char *)agtcfgin.agtptyv2cEntries[i].nv_commuStrv2c) == 0)) {
					strncpy((char *)agtcfgin.agtptyv2cEntries[i].agtv2c_tAddr,
							"0.0.0.0", DEFAULT_IPV4_SIZE);
					agtcfgin.agtptyv2cEntries[i].agtv2c_port = 162;
					agtcfgin.agtptyv2cEntries[i].agtv2c_trapSeverityLevel = None;
					found = 1;
					break;
				}
		}
		if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
		
			if (Sret == -1) {
				trace (TRACE_LEVEL_INFO, "Host Local message handler DELETE: SET Failed" );
				pSnmpServerIntSnmpConfigDelHostIntMessage->setCompletionStatus(WRC_SNMP_SET_HOST_FAILED);
			} else {
				pSnmpServerIntSnmpConfigDelHostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			}
		}  else {
			pSnmpServerIntSnmpConfigDelHostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		} 
		
		reply(pSnmpServerIntSnmpConfigDelHostIntMessage);

    }

void  ConfigHostIntWorker::SnmpServerIntConfigureDefaultsMessageHandler( SnmpServerIntConfigureDefaultsMessage *pSnmpServerIntConfigureDefaultsMessage)
        {
            trace (TRACE_LEVEL_INFO, "SnmpServerInitConfigureDefaultsMessageHandler: Start");
                if (pSnmpServerIntConfigureDefaultsMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "SnmpServerInitConfigureDefaultsMessageHandler:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        	}   

		int ret = snmpdConfigPush();
		if (ret == -1)
			trace (TRACE_LEVEL_ERROR, "Failed to sync DB with backend");
            trace (TRACE_LEVEL_INFO, "SnmpServerInitConfigureDefaultsMessageHandler: End");
		pSnmpServerIntConfigureDefaultsMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		reply(pSnmpServerIntConfigureDefaultsMessage);
	}


}
