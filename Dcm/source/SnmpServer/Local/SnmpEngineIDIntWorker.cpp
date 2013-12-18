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
 *   Author : skumaras                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "DcmResourceIdEnums.h"

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpEngineIDIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetEngineIDIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveEngineIDIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Local/EngineIDLocalManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "Framework/Types/HostUC.h"
#include "snmp/snmplib.h"
namespace DcmNs
{

    SnmpEngineIDIntWorker::SnmpEngineIDIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {

		EngineIDLocalManagedObject EngineIDLocalMO (pSnmpServerIntLocalObjectManager);
		EngineIDLocalMO.setupOrm ();
		addManagedClass (EngineIDLocalManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETENGINEIDINT, reinterpret_cast<PrismMessageHandler> (&SnmpEngineIDIntWorker::SnmpServerIntSnmpConfigSetEngineIDIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGREMOVEENGINEIDINT, reinterpret_cast<PrismMessageHandler> (&SnmpEngineIDIntWorker::SnmpServerIntSnmpConfigRemoveEngineIDIntMessageHandler));
    }

    SnmpEngineIDIntWorker::~SnmpEngineIDIntWorker ()
    {
    }

    PrismMessage  *SnmpEngineIDIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGSETENGINEIDINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetEngineIDIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGREMOVEENGINEIDINT :
                pPrismMessage = new SnmpServerIntSnmpConfigRemoveEngineIDIntMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }


	WaveManagedObject  *SnmpEngineIDIntWorker::createManagedObjectInstance (const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;

		if ((EngineIDLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new EngineIDLocalManagedObject (dynamic_cast<SnmpServerIntLocalObjectManager *>(getPWaveObjectManager()));
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, " SnmpEngineIDIntWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
 		}

		return (pWaveManagedObject);
	}


void  SnmpEngineIDIntWorker::SnmpServerIntSnmpConfigSetEngineIDIntMessageHandler( SnmpServerIntSnmpConfigSetEngineIDIntMessage *pSnmpServerIntSnmpConfigSetEngineIDIntMessage)
    {
        trace (TRACE_LEVEL_INFO, "Set Snmp Local Engine Id message handler: Coming here");
        if (pSnmpServerIntSnmpConfigSetEngineIDIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Snmp Local Engine Id:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

        agtcfg_t  agtcfgin;
        string EngineID = pSnmpServerIntSnmpConfigSetEngineIDIntMessage->getEngineID();
        int sRet = -1,ret = -1;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		ret = agtcfgXMGet(&agtcfgin);
		if (ret == -1)
		trace (TRACE_LEVEL_INFO, "SnmpEngineID Local message handler ADD: GET success" );

		memset (agtcfgin.localEngineId, 0x0, LOCALENGINEID);
        strncpy((char *)agtcfgin.localEngineId, EngineID.c_str(), LOCALENGINEID);

        sRet = agtcfgXMSet(&agtcfgin);
        if (sRet == -1) {
            trace (TRACE_LEVEL_INFO, "Local message handler SET: SET failed");
        } else {
            trace (TRACE_LEVEL_INFO, "Set EngineID Local message handler SET: SET success");
           	EngineIDLocalManagedObject *pSnmpLocEngIDMO = NULL;
            EngineIDLocalManagedObject *pEngineIDMO = NULL;
            LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
			vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, EngineIDLocalManagedObject::getClassName());

			if ((NULL == pResults) || (pResults->size() != 1))    {
            trace (TRACE_LEVEL_INFO, "creating new entry in db");
            if ((pResults == NULL) || (pResults->size() == 0))
                trace (TRACE_LEVEL_INFO, "Zero engineID instances found.");
            
			startTransaction();
            pEngineIDMO =
                new EngineIDLocalManagedObject(dynamic_cast<SnmpServerIntLocalObjectManager *>(getPWaveObjectManager()));
			pEngineIDMO->setEngineID(pSnmpServerIntSnmpConfigSetEngineIDIntMessage->getEngineID());
            status = commitTransaction();
            }
			else if (1 == pResults->size()) {
            startTransaction ();            
			pSnmpLocEngIDMO = dynamic_cast<EngineIDLocalManagedObject *> ((*pResults)[0]);
			trace (TRACE_LEVEL_ERROR, "calling update wavemanged obj" );
			pSnmpLocEngIDMO->setEngineID(pSnmpServerIntSnmpConfigSetEngineIDIntMessage->getEngineID());
            updateWaveManagedObject (pSnmpLocEngIDMO);
            status = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status) {
                trace (TRACE_LEVEL_INFO, "SNMP Local Engine ID SET: SuccessfulCluster-Wide commit");
                status = WAVE_MESSAGE_SUCCESS;
            }
            else {
                trace (TRACE_LEVEL_FATAL,
                        "SNMP Local Engine ID SET:Commiting failed.Status:"
                        + FrameworkToolKit::localize (status));
            }
            if (pResults)
                delete pResults;
			if (pEngineIDMO)
				delete pEngineIDMO;
			if (pSnmpLocEngIDMO)
        		delete pSnmpLocEngIDMO;
			}	       
        
		}   
        pSnmpServerIntSnmpConfigSetEngineIDIntMessage->setCompletionStatus (status);
        reply (pSnmpServerIntSnmpConfigSetEngineIDIntMessage);

    }
    void  SnmpEngineIDIntWorker::SnmpServerIntSnmpConfigRemoveEngineIDIntMessageHandler( SnmpServerIntSnmpConfigRemoveEngineIDIntMessage *pSnmpServerIntSnmpConfigRemoveEngineIDIntMessage)
    {
        trace (TRACE_LEVEL_INFO, " local EngineID Local message handler Del: Coming here");
        if (pSnmpServerIntSnmpConfigRemoveEngineIDIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Del local EngineID Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		ResourceId status = WAVE_MESSAGE_SUCCESS;
        agtcfg_t  agtcfgin;
        int sRet = -1,ret = -1;

        ret = agtcfgXMGet(&agtcfgin);
        if (ret == -1)
        trace (TRACE_LEVEL_INFO, "SnmpEngineID Local message handler REMOVE: GET success" );



		string EngineID = pSnmpServerIntSnmpConfigRemoveEngineIDIntMessage->getEngineID();
        memset (agtcfgin.localEngineId, 0x0, LOCALENGINEID);
        strncpy((char *)agtcfgin.localEngineId, EngineID.c_str(), LOCALENGINEID);
            
            sRet = agtcfgXMSet(&agtcfgin);

        if (sRet == -1) {
            trace (TRACE_LEVEL_INFO, "local EngineID Local message handler DELETE: SET Failed" );
        } else {

			EngineIDLocalManagedObject *pEngineIDLocalManagedObject = NULL;
			LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
			vector<WaveManagedObject *> *pResultsRollback = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, EngineIDLocalManagedObject::getClassName());      
 
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" );
            }

            startTransaction ();
            pEngineIDLocalManagedObject = dynamic_cast<EngineIDLocalManagedObject *> ((*pResultsRollback)[0]);
            trace (TRACE_LEVEL_ERROR, "calling update wavemanged obj" );
 
			if (pEngineIDLocalManagedObject)
            delete pEngineIDLocalManagedObject;
            status = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status)
            {
                trace (TRACE_LEVEL_INFO,
                        "SNMP EngineIDDelete MessageHandler: Successful Cluster-Wide commit EngineIDConfig");
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP EngineID Delete MessageHandler:Commiting Community config failed.Status:"
                        + FrameworkToolKit::localize (status));
            }
            if (pResultsRollback)
            delete pResultsRollback;
        }
        pSnmpServerIntSnmpConfigRemoveEngineIDIntMessage->setCompletionStatus (status);
        reply(pSnmpServerIntSnmpConfigRemoveEngineIDIntMessage);

    }
}
