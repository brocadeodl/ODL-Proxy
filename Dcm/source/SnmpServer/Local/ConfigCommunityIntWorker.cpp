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
#include "SnmpServer/Local/ConfigCommunityIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetCommIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveCommIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddCommIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <snmp/snmplib.h>
#include "DcmResourceIds.h"
namespace DcmNs
{

    ConfigCommunityIntWorker::ConfigCommunityIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETCOMMINT, reinterpret_cast<PrismMessageHandler> (&ConfigCommunityIntWorker::SnmpServerIntSnmpConfigSetCommIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGREMOVECOMMINT, reinterpret_cast<PrismMessageHandler> (&ConfigCommunityIntWorker::SnmpServerIntSnmpConfigRemoveCommIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGADDCOMMINT, reinterpret_cast<PrismMessageHandler> (&ConfigCommunityIntWorker::SnmpServerIntSnmpConfigAddCommIntMessageHandler));
    }

    ConfigCommunityIntWorker::~ConfigCommunityIntWorker ()
    {
    }

    PrismMessage  *ConfigCommunityIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGSETCOMMINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetCommIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGREMOVECOMMINT :
                pPrismMessage = new SnmpServerIntSnmpConfigRemoveCommIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGADDCOMMINT :
                pPrismMessage = new SnmpServerIntSnmpConfigAddCommIntMessage ();
                break;
           default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigCommunityIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	/*
    ** This function is the local message handler to add new community string.
    ** Called from: Global message handler of add community.
    ** Inputs:      Community string and access (optional)
    ** Functions:   Finds an empty entry in the SNMPd agtcfg structure, populates it with new values.
	** If access is rw then first 3 index is searched, for ro last 3 is searched.
    ** Errors:      Return GET/SET errors if failed. Only 6 community strings can be populated.
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigCommunityIntWorker::SnmpServerIntSnmpConfigAddCommIntMessageHandler( SnmpServerIntSnmpConfigAddCommIntMessage *pSnmpServerIntSnmpConfigAddCommIntMessage)
    {
		trace (TRACE_LEVEL_INFO, "Local message handler Add: Coming here");

		if (pSnmpServerIntSnmpConfigAddCommIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Add Comm Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }
		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1;
		unsigned int i = 0;
		string commStrVal = pSnmpServerIntSnmpConfigAddCommIntMessage->getCommunity(); 
		Access accessVal = pSnmpServerIntSnmpConfigAddCommIntMessage->getAccess();
		ret = agtcfgXMGet(&agtcfgin);


		if (ret == -1) {
			trace (TRACE_LEVEL_INFO,"Local message handler ADD: GET failed");
			pSnmpServerIntSnmpConfigAddCommIntMessage->setCompletionStatus 
								(WRC_SNMP_GET_COMMUNITY_FAILED);
			reply(pSnmpServerIntSnmpConfigAddCommIntMessage);
			return;
		}

		for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
            if ((commStrVal.compare((char *) agtcfgin.agtcommunityentries[i].communitystr)==0)) {
                    found = 0;
                    break;
                }
        }
		if (found != 0) {
			for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
				if (strlen (agtcfgin.agtcommunityentries[i].communitystr) == 0) {
					trace (TRACE_LEVEL_INFO, string ("Add this") +commStrVal);
					strncpy(agtcfgin.agtcommunityentries[i].communitystr,
						commStrVal.c_str(), commStrVal.length());
					agtcfgin.agtcommunityentries[i].communitystr[commStrVal.length()] = '\0';
					agtcfgin.agtcommunityentries[i].access = accessVal;
					found = 1;
					break;
				}
			}
		}
		if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_FATAL, "Local message handler ADD: SET failed");
				pSnmpServerIntSnmpConfigAddCommIntMessage->setCompletionStatus 
								(WRC_SNMP_SET_COMMUNITY_FAILED);
			} else {
				pSnmpServerIntSnmpConfigAddCommIntMessage->setCompletionStatus 
										(WAVE_MESSAGE_SUCCESS);
			}
		} else if (found == 0) {
			trace (TRACE_LEVEL_FATAL, "Local message handler ADD: Data already exist in backend");
			pSnmpServerIntSnmpConfigAddCommIntMessage->setCompletionStatus
                                        (WAVE_MESSAGE_SUCCESS);
		} else if (found == -1) {
			trace (TRACE_LEVEL_FATAL, "Local message handler ADD Comm: Comm exceed");
            pSnmpServerIntSnmpConfigAddCommIntMessage->setCompletionStatus 
								(WRC_SNMP_NUM_COMMUNITY_EXCEEDED_ERROR);
		} 

		reply(pSnmpServerIntSnmpConfigAddCommIntMessage);
    }

	/*
    ** This function is the local message handler to remove community string.
    ** Called from: Global message handler of remove community.
    ** Inputs:      Community string and access (optional)
    ** Functions:   Finds the community inputted from SNMPd agtcfg structure, populates it with NULL.
    ** Errors:      Return GET/SET errors if failed..
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigCommunityIntWorker::SnmpServerIntSnmpConfigRemoveCommIntMessageHandler( SnmpServerIntSnmpConfigRemoveCommIntMessage *pSnmpServerIntSnmpConfigRemoveCommIntMessage)
	{
		if (pSnmpServerIntSnmpConfigRemoveCommIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Del Comm Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		string commStr = pSnmpServerIntSnmpConfigRemoveCommIntMessage->getCommunity();
		trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: Coming here" +commStr);

		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1;
		unsigned int i = 0;
	  	ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: GET failed");
			pSnmpServerIntSnmpConfigRemoveCommIntMessage->setCompletionStatus 
									(WRC_SNMP_GET_COMMUNITY_FAILED);
			reply (pSnmpServerIntSnmpConfigRemoveCommIntMessage);
			return;
		}

		for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
			if (commStr.compare(agtcfgin.agtcommunityentries[i].communitystr) == 0) {
				trace (TRACE_LEVEL_INFO, string ("Deleted this") +agtcfgin.agtcommunityentries[i].communitystr);
				agtcfgin.agtcommunityentries[i].communitystr[0] = '\0';
				memset (agtcfgin.agtcommunityentries[i].communitystr, 0x0, AGT_MAX_SECRET_SIZE);
				agtcfgin.agtcommunityentries[i].access = 0;
				found = 1;
				break;
			}
		}
		
		if (found == 1) {
            Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: SET failed");
				pSnmpServerIntSnmpConfigRemoveCommIntMessage->setCompletionStatus 
										(WRC_SNMP_SET_COMMUNITY_FAILED);
			} else {
				pSnmpServerIntSnmpConfigRemoveCommIntMessage->setCompletionStatus 
											(WAVE_MESSAGE_SUCCESS);
			}
		} else {
			trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: Data not exists in backend");
			pSnmpServerIntSnmpConfigRemoveCommIntMessage->setCompletionStatus
                                            (WAVE_MESSAGE_SUCCESS);
		} 
		reply (pSnmpServerIntSnmpConfigRemoveCommIntMessage);
	}

	/*
    ** This function is the local message handler to change access of community string.
    ** Called from: Global message handler of set community.
    ** Inputs:      Community string and access (optional)
    ** Functions:   Finds the community inputted from SNMPd agtcfg structure, copies it to correct 
	**				position and NULLS the previous location.
    ** Errors:      Return GET/SET errors if failed. Only 3 community strings 
	**				can be configured in each group
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigCommunityIntWorker::SnmpServerIntSnmpConfigSetCommIntMessageHandler( SnmpServerIntSnmpConfigSetCommIntMessage *pSnmpServerIntSnmpConfigSetCommIntMessage)
	{
		trace (TRACE_LEVEL_INFO, "Local message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetCommIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Change Comm Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t    agtcfgin;
		string commStr = pSnmpServerIntSnmpConfigSetCommIntMessage->getCommunity();
		Access accessVal = pSnmpServerIntSnmpConfigSetCommIntMessage->getAccess();
		int ret = -1, i = 0, idx = -1, Sret = -1;
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
			pSnmpServerIntSnmpConfigSetCommIntMessage->setCompletionStatus(
							WRC_SNMP_GET_COMMUNITY_FAILED);
			reply (pSnmpServerIntSnmpConfigSetCommIntMessage);
			return;
		}

		for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
			if (commStr.compare(agtcfgin.agtcommunityentries[i].communitystr) == 0) {
				idx = i;
				break;
			}
		}
		if (idx != -1) {
			if (accessVal != agtcfgin.agtcommunityentries[idx].access) {
				trace (TRACE_LEVEL_INFO, string ("Change this") 
						+agtcfgin.agtcommunityentries[i].communitystr);
				agtcfgin.agtcommunityentries[idx].access = accessVal;
				Sret = agtcfgXMSet(&agtcfgin);

				if (Sret == -1) {
					trace (TRACE_LEVEL_ERROR, "SNMP Local Set MessageHandler - failed");
					pSnmpServerIntSnmpConfigSetCommIntMessage ->setCompletionStatus
									(WRC_SNMP_SET_COMMUNITY_FAILED);
					reply (pSnmpServerIntSnmpConfigSetCommIntMessage);
					return;
				}
			}
			pSnmpServerIntSnmpConfigSetCommIntMessage ->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pSnmpServerIntSnmpConfigSetCommIntMessage);
		} else {
			pSnmpServerIntSnmpConfigSetCommIntMessage ->setCompletionStatus 
							(WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
			reply (pSnmpServerIntSnmpConfigSetCommIntMessage);
		} 
	
	}
}
