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
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigSystemGroupIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetContactIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetLocationIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetSysDescrMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "snmp/snmplib.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    ConfigSystemGroupIntWorker::ConfigSystemGroupIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETCONTACTINT, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetContactIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETLOCATIONINT, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetLocationIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETSYSDESCR, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetSysDescrIntMessageHandler));
    }

    ConfigSystemGroupIntWorker::~ConfigSystemGroupIntWorker ()
    {
    }

    PrismMessage  *ConfigSystemGroupIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGSETCONTACTINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetContactIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGSETLOCATIONINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetLocationIntMessage ();
                break;
			case SNMPSERVERINTSNMPCONFIGSETSYSDESCR	:
                pPrismMessage = new SnmpServerIntSnmpConfigSetSysDescrMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigSystemGroupIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetContactIntMessageHandler( SnmpServerIntSnmpConfigSetContactIntMessage *pSnmpServerIntSnmpConfigSetContactIntMessage)
    {
        trace (TRACE_LEVEL_INFO, "Set contact Local message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetContactIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Contact Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }
        agtcfg_t    agtcfgin;
        string contactStr = pSnmpServerIntSnmpConfigSetContactIntMessage->getContact();
        int gRet = -1, sRet = -1;
        gRet = agtcfgXMGet(&agtcfgin); 
        if (gRet == -1) {
            trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
            pSnmpServerIntSnmpConfigSetContactIntMessage->setCompletionStatus(WRC_SNMP_GET_SYSGROUP_FAILED);
            reply (pSnmpServerIntSnmpConfigSetContactIntMessage);
			return;
		}
		memset (agtcfgin.sysContact, 0x0, M2DISPLAYSTRSIZE);
		strncpy( (char *)agtcfgin.sysContact, contactStr.c_str(), contactStr.length());

		sRet = agtcfgXMSet(&agtcfgin);
		if (sRet == -1) {
			trace (TRACE_LEVEL_INFO, "Set contact Local messagehandler SET: SET failed");
            pSnmpServerIntSnmpConfigSetContactIntMessage->setCompletionStatus
											(WRC_SNMP_SET_SYSGROUP_FAILED);
		} else {
			trace (TRACE_LEVEL_INFO, "Set Contact Local messagehandler SET: SET Success");
			pSnmpServerIntSnmpConfigSetContactIntMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		}
    
	    reply (pSnmpServerIntSnmpConfigSetContactIntMessage);
    }

    void  ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetLocationIntMessageHandler( SnmpServerIntSnmpConfigSetLocationIntMessage *pSnmpServerIntSnmpConfigSetLocationIntMessage)
    {
        trace (TRACE_LEVEL_INFO, "Set Location Local message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetLocationIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Location Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

        agtcfg_t    agtcfgin;
        string locationStr = pSnmpServerIntSnmpConfigSetLocationIntMessage->getLocation();
        int gRet = -1, sRet = -1;
        gRet = agtcfgXMGet(&agtcfgin); 
        if (gRet == -1) {
            trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
            pSnmpServerIntSnmpConfigSetLocationIntMessage->setCompletionStatus(WRC_SNMP_GET_SYSGROUP_FAILED);
            reply (pSnmpServerIntSnmpConfigSetLocationIntMessage);
			return;
		}

		memset (agtcfgin.sysLocation, 0x0, M2DISPLAYSTRSIZE);
		strncpy( (char *)agtcfgin.sysLocation, locationStr.c_str(), locationStr.length());

		sRet = agtcfgXMSet(&agtcfgin);
		if (sRet == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler SET: SET failed");
            pSnmpServerIntSnmpConfigSetLocationIntMessage->setCompletionStatus
										(WRC_SNMP_GET_SYSGROUP_FAILED);
		} else {
			trace (TRACE_LEVEL_INFO, "Set Location Local message handler SET: SET success");
			pSnmpServerIntSnmpConfigSetLocationIntMessage->setCompletionStatus(
												WAVE_MESSAGE_SUCCESS);
		}
        reply (pSnmpServerIntSnmpConfigSetLocationIntMessage);

    }

    void ConfigSystemGroupIntWorker::SnmpServerIntSnmpConfigSetSysDescrIntMessageHandler( SnmpServerIntSnmpConfigSetSysDescrMessage *pSnmpServerIntSnmpConfigSetSysDescrIntMessage)
    {
        trace (TRACE_LEVEL_INFO, "Set System Description Local message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetSysDescrIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set SysDescription Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

        agtcfg_t    agtcfgin;
        string sysDescrStr = pSnmpServerIntSnmpConfigSetSysDescrIntMessage->getSysDescr();
        int gRet = -1, sRet = -1;
        gRet = agtcfgXMGet(&agtcfgin); 
        if (gRet == -1) {
            trace (TRACE_LEVEL_INFO, "Local message handler SET: GET failed");
            pSnmpServerIntSnmpConfigSetSysDescrIntMessage->setCompletionStatus(WRC_SNMP_GET_SYSGROUP_FAILED);
            reply (pSnmpServerIntSnmpConfigSetSysDescrIntMessage);
			return;
		}

		memset (agtcfgin.sysDescr, 0x0, M2DISPLAYSTRSIZE);
		strncpy( (char *)agtcfgin.sysDescr, sysDescrStr.c_str(), sysDescrStr.length());

		sRet = agtcfgXMSet(&agtcfgin);
		if (sRet == -1) {
			trace (TRACE_LEVEL_INFO, "Local message handler SET: SET failed");
            pSnmpServerIntSnmpConfigSetSysDescrIntMessage->setCompletionStatus
										(WRC_SNMP_GET_SYSGROUP_FAILED);
		} else {
			trace (TRACE_LEVEL_INFO, "Set System Description Local message handler SET: SET success");
			pSnmpServerIntSnmpConfigSetSysDescrIntMessage->setCompletionStatus(
												WAVE_MESSAGE_SUCCESS);
		}
        reply (pSnmpServerIntSnmpConfigSetSysDescrIntMessage);

    }
}
