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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigUserIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "snmp/snmplib.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    ConfigUserIntWorker::ConfigUserIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETUSERINT, reinterpret_cast<PrismMessageHandler> (&ConfigUserIntWorker::SnmpServerIntSnmpConfigSetUserIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGREMOVEUSERINT, reinterpret_cast<PrismMessageHandler> (&ConfigUserIntWorker::SnmpServerIntSnmpConfigRemoveUserIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGADDUSERINT, reinterpret_cast<PrismMessageHandler> (&ConfigUserIntWorker::SnmpServerIntSnmpConfigAddUserIntMessageHandler));
    }

    ConfigUserIntWorker::~ConfigUserIntWorker ()
    {
    }

    PrismMessage  *ConfigUserIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGSETUSERINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetUserIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGREMOVEUSERINT :
                pPrismMessage = new SnmpServerIntSnmpConfigRemoveUserIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGADDUSERINT :
                pPrismMessage = new SnmpServerIntSnmpConfigAddUserIntMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigUserIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  ConfigUserIntWorker::SnmpServerIntSnmpConfigAddUserIntMessageHandler( SnmpServerIntSnmpConfigAddUserIntMessage *pSnmpServerIntSnmpConfigAddUserIntMessage)
    {
		if (pSnmpServerIntSnmpConfigAddUserIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Add User Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		trace (TRACE_LEVEL_INFO, "User Local message handler Add: Coming here");
		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1, startIdx = 0, endIdx = 0;
		int i = 0;
		string userName = pSnmpServerIntSnmpConfigAddUserIntMessage->getUsername();
        string groupType = pSnmpServerIntSnmpConfigAddUserIntMessage->getGroupname();
		AuthType authType = pSnmpServerIntSnmpConfigAddUserIntMessage->getAuth();
		string authPswd = pSnmpServerIntSnmpConfigAddUserIntMessage->getAuthpswd();
		PrivType privType = pSnmpServerIntSnmpConfigAddUserIntMessage->getPriv();
		string privPswd = pSnmpServerIntSnmpConfigAddUserIntMessage->getPrivpswd();
		bool encpswdflag = pSnmpServerIntSnmpConfigAddUserIntMessage->getEncpswdflag();

		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
		    trace (TRACE_LEVEL_INFO, "User Local message handler ADD: GET error" );
    		pSnmpServerIntSnmpConfigAddUserIntMessage->setCompletionStatus 
								(WRC_SNMP_GET_USER_FAILED);
		    reply(pSnmpServerIntSnmpConfigAddUserIntMessage);
            return;
        }

		if (groupType.compare("snmpadmin") == 0) {
            startIdx = 0;
            endIdx = 3;
        } else if (groupType.compare("snmpuser") == 0) {
            startIdx = 3;
            endIdx = 6;
        }
            
        for (i = startIdx; i < endIdx; i++) {
			if ((userName.compare((char *)agtcfgin.agtusmEntries[i].userName)) == 0) {
				found = 0;
				break;
            } 
		}
		if (found != 0) {
			for (i = startIdx; i < endIdx; i++) {
				if (strlen ((const char *)agtcfgin.agtusmEntries[i].userName) == 0) {
   		            trace (TRACE_LEVEL_INFO, string ("Add this") +userName);
        	        trace (TRACE_LEVEL_INFO, string ("Priv Type") +privType);
            	    trace (TRACE_LEVEL_INFO, string ("Def Priv Type") +NO_PRIV_PROTOCOL);
                	strncpy((char *)agtcfgin.agtusmEntries[i].userName,
                        userName.c_str(), userName.length());
	                agtcfgin.agtusmEntries[i].userName[userName.length()] = '\0';
    	            agtcfgin.agtusmEntries[i].agt_authSecret[0] = '\0';
                	if (authType != AuthNone) {
                    	memset (agtcfgin.agtusmEntries[i].agt_authSecret, 0x0, AGT_MAX_PASSWD_SIZE);
	                    strncpy((char *)agtcfgin.agtusmEntries[i].agt_authSecret,
							authPswd.c_str(), authPswd.length());
                		agtcfgin.agtusmEntries[i].agt_encpswdflag 	= encpswdflag;
    	            }
        	        agtcfgin.agtusmEntries[i].authProtocol = authType+1;
            	    switch (privType) {
                	    case PrivNone:
                    	    agtcfgin.agtusmEntries[i].privProtocol = NO_PRIV_PROTOCOL;
                        	break;
    	                case DES:
	                        agtcfgin.agtusmEntries[i].privProtocol = PRIV_PROTOCOL_DES;
        	                break;
            	        case AES128:
                	        agtcfgin.agtusmEntries[i].privProtocol = PRIV_PROTOCOL_AES_128;
                    	    break;
                	}
	                agtcfgin.agtusmEntries[i].agt_privSecret[0] = '\0';
    	           if ( privType != PrivNone) {
        	            memset (agtcfgin.agtusmEntries[i].agt_privSecret, 0x0, AGT_MAX_PASSWD_SIZE);
            	        strncpy ((char *)agtcfgin.agtusmEntries[i].agt_privSecret,
						privPswd.c_str(), privPswd.length());
                	}
	                found = 1;
    	            break;
            	}
        	}
		}

        if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_FATAL, "Local message handler ADD: SET failed");
				pSnmpServerIntSnmpConfigAddUserIntMessage->setCompletionStatus 
								(WRC_SNMP_SET_USER_FAILED);
			} else {
				pSnmpServerIntSnmpConfigAddUserIntMessage->setCompletionStatus 
										(WAVE_MESSAGE_SUCCESS);
			}
		} else if (found == -1) {
			trace (TRACE_LEVEL_FATAL, "Local message handler ADD User: User exceed");
            pSnmpServerIntSnmpConfigAddUserIntMessage->setCompletionStatus 
								(WRC_SNMP_NUM_USER_EXCEEDED_ERROR);
		} else if (found == 0) {
			trace (TRACE_LEVEL_FATAL, "Local message handler ADD User: Already exists in backend");
			pSnmpServerIntSnmpConfigAddUserIntMessage->setCompletionStatus
                                        (WAVE_MESSAGE_SUCCESS);
		} 

		reply(pSnmpServerIntSnmpConfigAddUserIntMessage);
    }

    void  ConfigUserIntWorker::SnmpServerIntSnmpConfigRemoveUserIntMessageHandler( SnmpServerIntSnmpConfigRemoveUserIntMessage *pSnmpServerIntSnmpConfigRemoveUserIntMessage)
    {

		if (pSnmpServerIntSnmpConfigRemoveUserIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Del User Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		string userName = pSnmpServerIntSnmpConfigRemoveUserIntMessage->getUsername();
		trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: Coming here" +userName);

		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1;
		unsigned int i = 0;
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: GET failed");
			pSnmpServerIntSnmpConfigRemoveUserIntMessage->setCompletionStatus 
									(WRC_SNMP_GET_USER_FAILED);
			reply (pSnmpServerIntSnmpConfigRemoveUserIntMessage);
			return;
		}

		for (i = 0; i < MAX_USM_ENTRIES; i++) {
			if (userName.compare((char *)agtcfgin.agtusmEntries[i].userName) == 0) {
				trace (TRACE_LEVEL_INFO, "Deleted this" +userName);
				agtcfgin.agtusmEntries[i].userName[0] = '\0';
				agtcfgin.agtusmEntries[i].agt_encpswdflag   = 0;
				memset (agtcfgin.agtusmEntries[i].userName, 0x0, MAX_USERNAME_SIZE);
	            agtcfgin.agtusmEntries[i].authProtocol = NO_AUTH_PROTOCOL;
                agtcfgin.agtusmEntries[i].privProtocol = NO_PRIV_PROTOCOL;
                agtcfgin.agtusmEntries[i].agt_privSecret[0] = '\0';
                agtcfgin.agtusmEntries[i].agt_authSecret[0] = '\0';
 	    		found = 1;
				break;
			}
		}
		
		if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: SET failed");
				pSnmpServerIntSnmpConfigRemoveUserIntMessage->setCompletionStatus 
										(WRC_SNMP_SET_USER_FAILED);
			} else {
				pSnmpServerIntSnmpConfigRemoveUserIntMessage->setCompletionStatus 
											(WAVE_MESSAGE_SUCCESS);
			}
		} else if (found == -1) {
			trace (TRACE_LEVEL_FATAL, "Local message handler REMOVE: Data not exists in Backend");
			pSnmpServerIntSnmpConfigRemoveUserIntMessage->setCompletionStatus
                                            (WAVE_MESSAGE_SUCCESS);
		} 
		reply (pSnmpServerIntSnmpConfigRemoveUserIntMessage);
   }

    void  ConfigUserIntWorker::SnmpServerIntSnmpConfigSetUserIntMessageHandler( SnmpServerIntSnmpConfigSetUserIntMessage *pSnmpServerIntSnmpConfigSetUserIntMessage)
    {
		trace (TRACE_LEVEL_INFO, "Local message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetUserIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Change User Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t    agtcfgin;
		string userName = pSnmpServerIntSnmpConfigSetUserIntMessage->getUsername();
        string groupType = pSnmpServerIntSnmpConfigSetUserIntMessage->getGroupname();
		AuthType authType = pSnmpServerIntSnmpConfigSetUserIntMessage->getAuth();
		string authPswd = pSnmpServerIntSnmpConfigSetUserIntMessage->getAuthpswd();
		PrivType privType = pSnmpServerIntSnmpConfigSetUserIntMessage->getPriv();
		string privPswd = pSnmpServerIntSnmpConfigSetUserIntMessage->getPrivpswd();
		bool encpswdflag = pSnmpServerIntSnmpConfigSetUserIntMessage->getEncpswdflag();

		int ret = -1, i = 0, idx = -1, Sret = -1, addSuccess = -1;
        int found = -1;
        int addIdx = 0;
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
			pSnmpServerIntSnmpConfigSetUserIntMessage->setCompletionStatus(
							WRC_SNMP_GET_USER_FAILED);
			reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
			return;
		}

		for (i = 0; i < MAX_USM_ENTRIES; i++) {
			if (userName.compare((char *)agtcfgin.agtusmEntries[i].userName) == 0) {
				idx = i;
				trace (TRACE_LEVEL_INFO, "Change this" 
						+userName);
				break;
			}
		}

		if ((groupType.compare("snmpadmin") == 0) && (idx > 2) ) {
			for (i = 0; i < 3; i ++) {
				if (agtcfgin.agtusmEntries[i].userName[0] == '\0') {
					trace (TRACE_LEVEL_INFO, "Add this"+userName);
                    addIdx = i;
					addSuccess = 1;
					break;
				}	
			}
		} else if ((groupType.compare("snmpuser") == 0) && (idx < 3) ) {
			for (i = 3; i < 6; i ++) {
				if (agtcfgin.agtusmEntries[i].userName[0] == '\0') {
					trace (TRACE_LEVEL_INFO, "Add this" +userName);
                    addIdx = i;
                    addSuccess = 1;
					break;
				}
			}
		} else {
            for (i = 0; i < MAX_USM_ENTRIES; i++) {
                if (userName.compare ((char *)agtcfgin.agtusmEntries[i].userName) == 0) {
                    agtcfgin.agtusmEntries[i].agt_authSecret[0] = '\0';
					agtcfgin.agtusmEntries[i].agt_encpswdflag   = encpswdflag;
                    if (authType != AuthNone) {    
                        memset (agtcfgin.agtusmEntries[i].agt_authSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                        strncpy((char *)agtcfgin.agtusmEntries[i].agt_authSecret,
								authPswd.c_str(), authPswd.length());
                    }
                    agtcfgin.agtusmEntries[i].authProtocol = authType + 1;
                    switch (privType) {
                        case PrivNone:
                            agtcfgin.agtusmEntries[i].privProtocol = NO_PRIV_PROTOCOL;
                            break;
                        case DES:
                            agtcfgin.agtusmEntries[i].privProtocol = PRIV_PROTOCOL_DES;
                            break;
                        case AES128:
                            agtcfgin.agtusmEntries[i].privProtocol = PRIV_PROTOCOL_AES_128;
                            break;
                    }
                    agtcfgin.agtusmEntries[i].agt_privSecret[0] = '\0';
                    if ( privType != PrivNone) {
                        memset ((char *)agtcfgin.agtusmEntries[i].agt_privSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                        strncpy ((char *)agtcfgin.agtusmEntries[i].agt_privSecret,
								privPswd.c_str(), privPswd.length());
                    }
                    found = 1;
        			Sret = agtcfgXMSet(&agtcfgin);
		        	if (Sret == -1) {
				        trace (TRACE_LEVEL_ERROR, "SNMP Local Set MessageHandler - failed");
        				pSnmpServerIntSnmpConfigSetUserIntMessage ->setCompletionStatus
		        							(WRC_SNMP_SET_USER_FAILED);
				        reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
        				return;
		        	}
                    break;
                }
            }
        }

		if ((idx != -1) && (addSuccess == 1)) {
			agtcfgin.agtusmEntries[idx].userName[0] = '\0';
			memset (agtcfgin.agtusmEntries[idx].userName, 0x0, MAX_USERNAME_SIZE);
			agtcfgin.agtusmEntries[i].agt_encpswdflag   = 0;
            agtcfgin.agtusmEntries[idx].agt_authSecret[0] = '\0';
            agtcfgin.agtusmEntries[idx].agt_privSecret[0] = '\0';
	        agtcfgin.agtusmEntries[idx].authProtocol = NO_AUTH_PROTOCOL;
            agtcfgin.agtusmEntries[idx].privProtocol = NO_PRIV_PROTOCOL;
 	    	Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_ERROR, "SNMP Local Set MessageHandler - failed");
				pSnmpServerIntSnmpConfigSetUserIntMessage ->setCompletionStatus
									(WRC_SNMP_SET_USER_FAILED);
				reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
				return;
			}
            ret = agtcfgXMGet(&agtcfgin);
            if (ret == -1) {
                trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
                pSnmpServerIntSnmpConfigSetUserIntMessage->setCompletionStatus(
                                WRC_SNMP_GET_USER_FAILED);
                reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
                return;
            }

            strncpy((char *)agtcfgin.agtusmEntries[addIdx].userName,
                    userName.c_str(), userName.length());
            agtcfgin.agtusmEntries[addIdx].userName[userName.length()] = '\0';
            agtcfgin.agtusmEntries[addIdx].agt_authSecret[0] = '\0';
			agtcfgin.agtusmEntries[addIdx].agt_encpswdflag   = encpswdflag;
            if (authType != AuthNone) {    
                memset (agtcfgin.agtusmEntries[addIdx].agt_authSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                strncpy((char *)agtcfgin.agtusmEntries[addIdx].agt_authSecret,
						authPswd.c_str(), authPswd.length());
            }
            agtcfgin.agtusmEntries[addIdx].authProtocol = authType + 1;
            switch (privType) {
                case PrivNone:
                    agtcfgin.agtusmEntries[addIdx].privProtocol = NO_PRIV_PROTOCOL;
                    break;
                case DES:
                    agtcfgin.agtusmEntries[addIdx].privProtocol = PRIV_PROTOCOL_DES;
                    break;
                case AES128:
                    agtcfgin.agtusmEntries[addIdx].privProtocol = PRIV_PROTOCOL_AES_128;
                    break;
            }
            agtcfgin.agtusmEntries[addIdx].agt_privSecret[0] = '\0';
            if ( privType != PrivNone) {
                memset ((char *)agtcfgin.agtusmEntries[addIdx].agt_privSecret, 0x0, AGT_MAX_PASSWD_SIZE);
                strncpy ((char *)agtcfgin.agtusmEntries[addIdx].agt_privSecret,
						privPswd.c_str(), privPswd.length());
            }

            Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_ERROR, "SNMP Local Set MessageHandler - failed");
				pSnmpServerIntSnmpConfigSetUserIntMessage ->setCompletionStatus
									(WRC_SNMP_SET_USER_FAILED);
				reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
				return;
			}

		}

 		if ((addSuccess != 1) && (found != 1) ){
			pSnmpServerIntSnmpConfigSetUserIntMessage ->setCompletionStatus 
							(WRC_SNMP_NUM_USER_EXCEEDED_ERROR);
		} else {
			pSnmpServerIntSnmpConfigSetUserIntMessage ->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		}
		
		reply (pSnmpServerIntSnmpConfigSetUserIntMessage);
    }

}
