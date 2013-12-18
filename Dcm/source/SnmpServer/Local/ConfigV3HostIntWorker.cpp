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

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigV3HostIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigChangeV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/V3HostsManagedObject.h"
#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include <snmp/snmplib.h>
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Types/HostUC.h"
#define DEFAULT_ENGINEID {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 9}
#define DEFAULT_IPV4_SIZE 8
namespace DcmNs
{

    ConfigV3HostIntWorker::ConfigV3HostIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGADDV3HOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostIntWorker::SnmpServerIntSnmpConfigAddV3HostIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGDELV3HOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostIntWorker::SnmpServerIntSnmpConfigDelV3HostIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINT, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostIntWorker::SnmpServerIntSnmpConfigChangeV3HostIntMessageHandler));
    }

    ConfigV3HostIntWorker::~ConfigV3HostIntWorker ()
    {
    }

    PrismMessage  *ConfigV3HostIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGADDV3HOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigAddV3HostIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGDELV3HOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigDelV3HostIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigChangeV3HostIntMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigV3HostIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
	
	/*
	** This function is the local message handler for adding a new V3host to SNMPd.
	** Called from: Global Message handler of add V3host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, finds an empty entry (0.0.0.0)
	** Uses this entry to populate the new V3host.
	** Only 6 V3hosts of each version can be configured.
	*/

    void  ConfigV3HostIntWorker::SnmpServerIntSnmpConfigAddV3HostIntMessageHandler( SnmpServerIntSnmpConfigAddV3HostIntMessage *pSnmpServerIntSnmpConfigAddV3HostIntMessage)
	{
		if (pSnmpServerIntSnmpConfigAddV3HostIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Add V3Host Global:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		trace (TRACE_LEVEL_INFO, "V3Host Local message handler Add: Coming here");
		agtcfg_t agtcfgin;
        fixed_OctetString mEngineID[6] = {
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID
        };

        mEngineID[0].length = 9;

		int ret = -1, Sret = -1, found = -1, mRet = -1;
		int i = 0, j = 0;
		u_int idx = 0;
		//int rc = 0;
		string ipStr, ipVal;
		string userName = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getUsername();
		string engineId = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getEngineid();
		NotifyType notifyType = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getNotify();
		HostUC ipAddr = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getHostUC();
		int udpPort = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getUdpPort();
		SevLevel sevLevel = pSnmpServerIntSnmpConfigAddV3HostIntMessage->getSevLevel();
		char engine_id[256];
        memset (engine_id, 0x0, 256);
        strncpy(engine_id, engineId.c_str(), engineId.length());
		ipStr.assign("0.0.0.0");
		ipVal = ipAddr.getHostValue();
		ret = agtcfgXMGet(&agtcfgin);
        mRet = snmpManagerEngineIdsGet(&(mEngineID[0]));
		if ((ret == -1) || (mRet == -1) ) {
    		trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: GET error" );
            pSnmpServerIntSnmpConfigAddV3HostIntMessage->setCompletionStatus
                (WRC_SNMP_V3_GET_HOST_FAILED);
            reply(pSnmpServerIntSnmpConfigAddV3HostIntMessage);
            return; 
        }

        for (j = 0; j < MAX_USM_ENTRIES; j++) {
            if (userName.compare ((char *) agtcfgin.agtusmEntries[j].userName) == 0 ) {
                break;
            }
        }

        if (j == MAX_USM_ENTRIES) {
     		trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: No username error");
            pSnmpServerIntSnmpConfigAddV3HostIntMessage->setCompletionStatus
                (WRC_SNMP_V3_INVALID_USERNAME_ERROR);
            reply(pSnmpServerIntSnmpConfigAddV3HostIntMessage);
            return;           
        }
        
		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
			idx = j;
			idx++;
			if ((ipVal.compare((char *)agtcfgin.agtV3TrapEntries[i].targetAddr) == 0) &&
			( idx == agtcfgin.agtV3TrapEntries[i].usmIndex) ) {
				found = 0;
			trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: Data Already exists in backend" );
				break;
            }
		}

		if (found != 0) {
			for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) { 
				if (ipStr.compare((char *)agtcfgin.agtV3TrapEntries[i].targetAddr) == 0) {
    	            trace (TRACE_LEVEL_INFO, string("Add this") +i);
        	        memset (agtcfgin.agtV3TrapEntries[i].targetAddr, 0x0, TARGET_IP_ADDRESS_SIZE);
            	    strncpy((char *)agtcfgin.agtV3TrapEntries[i].targetAddr,
                	        ipVal.c_str(), ipVal.length());
	                agtcfgin.agtV3TrapEntries[i].usmIndex = j + 1;
//                strcpy(agtcfgin.agtV3TrapEntries[i].agt_authPrivSecret, agtcfgin.agtusmEntries[j].userName);
    	            agtcfgin.agtV3TrapEntries[i].agt_port = udpPort;
        	        agtcfgin.agtV3TrapEntries[i].severityLevel = sevLevel;
            	    agtcfgin.agtV3TrapEntries[i].informenabled = notifyType;
                	if (notifyType == informs) {
                    	trace (TRACE_LEVEL_INFO, string("Message Engine Id:") +engine_id);
	                    convertStringtoEngineID(engine_id, &mEngineID[i]);
    	                int rc = snmpManagerEngineIdsSet(&(mEngineID[0]));
        		        if (0 != rc)
                	    {
                    	    printf ("rc is non zero");
	                    }
                	} 
	                found = 1;
    	            Sret = agtcfgXMSet(&agtcfgin);
        	        break;
            	}
			}
		}

		if (found == -1) {
			trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD V3Host: No empty entry");
			pSnmpServerIntSnmpConfigAddV3HostIntMessage->setCompletionStatus(WRC_SNMP_V3_NUM_HOST_EXCEEDED_ERROR);
		} else if (found == 1 && Sret == -1) {
			trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: SET failed" );
			pSnmpServerIntSnmpConfigAddV3HostIntMessage->setCompletionStatus(WRC_SNMP_V3_SET_HOST_FAILED);
		} else {
			pSnmpServerIntSnmpConfigAddV3HostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		}
		reply(pSnmpServerIntSnmpConfigAddV3HostIntMessage);
	}

	/*
	** This function is the local message handler for changing attributes of V3host to SNMPd.
	** Called from: Global Message handler of change V3host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, determines if the V3host is v1 or v2c.
	** Finds an empty entry (0.0.0.0) in corresponding structure and populates it.
	** If a port has to be changed and and entry match has been found in v1 then only port is updated.
	*/

    void  ConfigV3HostIntWorker::SnmpServerIntSnmpConfigChangeV3HostIntMessageHandler( SnmpServerIntSnmpConfigChangeV3HostIntMessage *pSnmpServerIntSnmpConfigChangeV3HostIntMessage)
	{
		trace (TRACE_LEVEL_INFO, "V3Host Local message handler Change: Coming here");
		if (pSnmpServerIntSnmpConfigChangeV3HostIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Change V3Host Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t agtcfgin;
        fixed_OctetString manager_engineID[6] = {
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID
        };

        /* To get rid of X86 unused variable warning */
        manager_engineID[0].length = 9;

		int ret = -1, Sret = -1, found = -1, mRet = -1;
		int i = 0, rc = 0;
        unsigned int j = 0;
		string ipStr, ipCheck;
		ipCheck.assign ("0.0.0.0");
		string userName = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getUsername();
		NotifyType notifyType = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getNotify();
		HostUC ipAddr = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getHostUC();
		int udpPort = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getUdpPort();
        string engineId = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getEngineid();
		SevLevel sevLevel = pSnmpServerIntSnmpConfigChangeV3HostIntMessage->getSevLevel();
		char engine_id[256];
        memset (engine_id, 0x0, 256);
        strncpy(engine_id, engineId.c_str(), engineId.length());
        
		ipStr = ipAddr.getHostValue();

		ret = agtcfgXMGet(&agtcfgin);

        mRet = snmpManagerEngineIdsGet(&(manager_engineID[0]));
 
        if ((ret == -1) || (mRet == -1) ) {
            pSnmpServerIntSnmpConfigChangeV3HostIntMessage->setCompletionStatus
                    (WRC_SNMP_V3_GET_HOST_FAILED);
            reply(pSnmpServerIntSnmpConfigChangeV3HostIntMessage);
            return;
        }
    
        for (j = 0; j < MAX_USM_ENTRIES; j++) {
            if (userName.compare ((char *) agtcfgin.agtusmEntries[j].userName) == 0 ) {
               break;
            }
        }
        if (j == MAX_USM_ENTRIES) {
      		trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: No username error");
            pSnmpServerIntSnmpConfigChangeV3HostIntMessage->setCompletionStatus
                    (WRC_SNMP_V3_INVALID_USERNAME_ERROR);
            reply(pSnmpServerIntSnmpConfigChangeV3HostIntMessage);
            return;           
        }
 
		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
			if ((ipStr.compare((char *)agtcfgin.agtV3TrapEntries[i].targetAddr) == 0 ) 
				&& (agtcfgin.agtV3TrapEntries[i].usmIndex == j + 1)) {
                trace (TRACE_LEVEL_INFO, "V3Host CHANGE: Got match" + userName);
				agtcfgin.agtV3TrapEntries[i].agt_port = udpPort;
                agtcfgin.agtV3TrapEntries[i].informenabled = notifyType;
                agtcfgin.agtV3TrapEntries[i].severityLevel = sevLevel;
                if (notifyType == informs) {
	            convertStringtoEngineID(engine_id, &manager_engineID[i]);
                rc = snmpManagerEngineIdsSet(&(manager_engineID[0]));
                trace (TRACE_LEVEL_INFO, string("V3Host Change: Engineid set") +rc);
                }
 		    	found = 1;
				Sret = agtcfgXMSet(&agtcfgin);
				break;
			} 
		}
		
		if (found == -1) {
			trace (TRACE_LEVEL_INFO, " V3Host Local message handler CHANGE: No empty entry" );
			pSnmpServerIntSnmpConfigChangeV3HostIntMessage->setCompletionStatus(WRC_SNMP_V3_NUM_HOST_EXCEEDED_ERROR);
		} else if (Sret == -1) {
			trace (TRACE_LEVEL_INFO, " V3Host Local message handler CHANGE: Change failed" );
			pSnmpServerIntSnmpConfigChangeV3HostIntMessage->setCompletionStatus(WRC_SNMP_V3_SET_HOST_FAILED);
		} else 
            pSnmpServerIntSnmpConfigChangeV3HostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

		reply(pSnmpServerIntSnmpConfigChangeV3HostIntMessage);
	}
	/*
	** This function is the local message handler for deleting V3host from SNMPd.
	** Called from: Global Message handler of delete V3host.
	** Inputs: 		IP, Community and optional version and port
	** Functions:	Gets current agtcfg structure from SNMPd, determines. Find the match and populate
	** that entry with empty entry (0.0.0.0).
	*/

    void  ConfigV3HostIntWorker::SnmpServerIntSnmpConfigDelV3HostIntMessageHandler( SnmpServerIntSnmpConfigDelV3HostIntMessage *pSnmpServerIntSnmpConfigDelV3HostIntMessage)
    {
		trace (TRACE_LEVEL_INFO, " V3Host Local message handler Del: Coming here");
		if (pSnmpServerIntSnmpConfigDelV3HostIntMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Del V3Host Local:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		agtcfg_t agtcfgin;
        fixed_OctetString manager_engineID[6] = {
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID,
            DEFAULT_ENGINEID
        };

        /* To get rid of X86 unused variable warning */
        manager_engineID[0].length = 9;

		int ret = -1, found = 0, Sret = 0, rc = 0, mRet = -1;
		unsigned int i = 0, j = 0;
		string ipStr;
		string userName = pSnmpServerIntSnmpConfigDelV3HostIntMessage->getUsername();
		HostUC ipAddr = pSnmpServerIntSnmpConfigDelV3HostIntMessage->getHostUC();
		ipStr = ipAddr.getHostValue();

		ret = agtcfgXMGet(&agtcfgin);
        mRet = snmpManagerEngineIdsGet(&(manager_engineID[0]));
        
		if ((ret == -1) || (mRet == -1) ) {
    		trace (TRACE_LEVEL_INFO, " V3Host Local message handler REMOVE: GET error");
            pSnmpServerIntSnmpConfigDelV3HostIntMessage->setCompletionStatus
                (WRC_SNMP_V3_GET_HOST_FAILED);
            reply(pSnmpServerIntSnmpConfigDelV3HostIntMessage);
            return;
        }
        
        for (j = 0; j < MAX_USM_ENTRIES; j++) {
            if (userName.compare ((char *) agtcfgin.agtusmEntries[j].userName) == 0 ) {
                break;
            }
        }
         if (j == MAX_USM_ENTRIES) {
     		trace (TRACE_LEVEL_INFO, "V3Host Local message handler ADD: No username error");
            pSnmpServerIntSnmpConfigDelV3HostIntMessage->setCompletionStatus
                (WRC_SNMP_V3_INVALID_USERNAME_ERROR);
            reply(pSnmpServerIntSnmpConfigDelV3HostIntMessage);
            return;           
        }
 
		for (i = 0; i < MAX_AGTPTY_ENTRIES; i++) {
				if ((ipStr.compare((char *)agtcfgin.agtV3TrapEntries[i].targetAddr) == 0) 
				&& (agtcfgin.agtV3TrapEntries[i].usmIndex == j + 1)) {
					strncpy((char *)agtcfgin.agtV3TrapEntries[i].targetAddr,
							"0.0.0.0", DEFAULT_IPV4_SIZE);
					agtcfgin.agtV3TrapEntries[i].agt_port = 162;
                    agtcfgin.agtV3TrapEntries[i].informenabled = traps;
                    agtcfgin.agtV3TrapEntries[i].severityLevel = None;
                    convertStringtoEngineID((char*)"00:00:00:00:00:00:00:00:00", &manager_engineID[i]);    
					found = 1;
				}		
		}
		if (found == 1) {
            rc = snmpManagerEngineIdsSet(&(manager_engineID[0]));
			Sret = agtcfgXMSet(&agtcfgin);
		}
		
		if ((Sret == -1) || (rc == -1) ) {
			trace (TRACE_LEVEL_INFO, "V3Host Local message handler DELETE: SET Failed" );
			pSnmpServerIntSnmpConfigDelV3HostIntMessage->setCompletionStatus(WRC_SNMP_V3_SET_HOST_FAILED);
		} else {
			pSnmpServerIntSnmpConfigDelV3HostIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		}
		reply(pSnmpServerIntSnmpConfigDelV3HostIntMessage);

    }

}
