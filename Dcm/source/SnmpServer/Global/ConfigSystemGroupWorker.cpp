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
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"


#include "SnmpServer/Global/ConfigSystemGroupWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetContactMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetContactIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetLocationIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetSysDescrMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetLocationMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetSysDescrMessage.h"
#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "snmp/snmplib.h"

#include "AAA/Global/AAASequencerContexts.h"
namespace DcmNs
{

    ConfigSystemGroupWorker::ConfigSystemGroupWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        SystemGroupManagedObject    SystemGroupManagedObject    (pSnmpServerObjectManager);
        SystemGroupManagedObject.setupOrm ();
        addManagedClass (SystemGroupManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGSETCONTACT, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupWorker::SnmpServerSnmpConfigSetContactMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGSETLOCATION, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupWorker::SnmpServerSnmpConfigSetLocationMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGSETSYSDESCR, reinterpret_cast<PrismMessageHandler> (&ConfigSystemGroupWorker::SnmpServerSnmpConfigSetSysDescrMessageHandler));
    }

    ConfigSystemGroupWorker::~ConfigSystemGroupWorker ()
    {
    }

    PrismMessage  *ConfigSystemGroupWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGSETCONTACT :
                pPrismMessage = new SnmpServerSnmpConfigSetContactMessage ();
                break;
            case SNMPSERVERSNMPCONFIGSETLOCATION :
                pPrismMessage = new SnmpServerSnmpConfigSetLocationMessage ();
                break;
            case SNMPSERVERSNMPCONFIGSETSYSDESCR:
                pPrismMessage = new SnmpServerSnmpConfigSetSysDescrMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigSystemGroupWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SystemGroupManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SystemGroupManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ConfigSystemGroupWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	/* 
	**	This function is the global message handler to set contact string for System Group.
	**  Called from: System Group WyserEaConfiguration Worker.
	**  Inputs: 	 Contact string length 1 to 16.
	**  Functions:	 Sends message to local handlers to set backend system group objects.
					 On success commits to database.
	**  Errors :	 Returns GET/SET failed or invalid contact string length.
	**  On SUCCESS:	 Returns WAVE_MESSAGE_SUCCESS.
	*/

    void  ConfigSystemGroupWorker::SnmpServerSnmpConfigSetContactMessageHandler( SnmpServerSnmpConfigSetContactMessage *pSnmpServerSnmpConfigSetContactMessage)
    {
        trace (TRACE_LEVEL_INFO, "System Group Worker::Set Contact Msg Handler:Entering ");

		if (pSnmpServerSnmpConfigSetContactMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Contact:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
	
		vector<UI32> locations;
        string contactStr      = pSnmpServerSnmpConfigSetContactMessage->getContact();
		if ((contactStr.length() < MIN_M2DISPLAYSTRSIZE) || 
			(contactStr.length() > M2DISPLAYSTRSIZE -1)) {
			pSnmpServerSnmpConfigSetContactMessage->setCompletionStatus (WRC_SNMP_INVALID_CONTACT_LENGTH);
            reply (pSnmpServerSnmpConfigSetContactMessage);
            return;
		}

		SnmpServerIntSnmpConfigSetContactIntMessage *pMessage =
					new SnmpServerIntSnmpConfigSetContactIntMessage (contactStr);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigSystemGroupWorker::setContactSTCCB), pSnmpServerSnmpConfigSetContactMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

	void ConfigSystemGroupWorker::setContactSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetContactMessage *pSnmpServerSnmpConfigSetContactMessage = 
                                (SnmpServerSnmpConfigSetContactMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS; 
		string     contactStr   = pSnmpServerSnmpConfigSetContactMessage->getContact();
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting snmp contact operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting snmp contact operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delCommSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        { 
            SystemGroupManagedObject *pSysGroupMO = NULL;

            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(SystemGroupManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +contactStr);
                pSnmpServerSnmpConfigSetContactMessage->setCompletionStatus
                                    (WRC_SNMP_SYSTEMGROUP_DOESNT_EXIST);

				// clean up allocated messages
				SnmpServerIntSnmpConfigSetContactIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetContactIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetContactMessage);
                delete synchronousQueryContext;
                return;
            }

            pSysGroupMO = dynamic_cast<SystemGroupManagedObject *> ((*pResults)[0]);
            delete pResults;
            delete synchronousQueryContext;
            startTransaction ();
            updateWaveManagedObject (pSysGroupMO);
            if (contactStr.length() != 0)
                pSysGroupMO->setContact(contactStr);
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {             
                trace (TRACE_LEVEL_INFO, 
                        "SNMP System Group SET: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {             
                trace (TRACE_LEVEL_FATAL, 
                        "SNMP System Group SET:Commiting configuration failed.Status : " 
                        + FrameworkToolKit::localize (fstatus));
            }
            delete pSysGroupMO;
        }


        // clean up allocated messages
        SnmpServerIntSnmpConfigSetContactIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetContactIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigSetContactMessage->setCompletionStatus (status);
        pSnmpServerSnmpConfigSetContactMessage->setMessageString (errorMessage);
        reply (pSnmpServerSnmpConfigSetContactMessage);
    }

	/* 
	**	This function is the global message handler to set location string for System Group.
	**  Called from: System Group WyserEaConfiguration Worker.
	**  Inputs: 	 Location string length 1 to 16.
	**  Functions:	 Sends message to local handlers to set backend system group objects.
					 On success commits to database.
	**  Errors :	 Returns GET/SET failed or invalid location string length.
	**  On SUCCESS:	 Returns WAVE_MESSAGE_SUCCESS.
	*/

    void  ConfigSystemGroupWorker::SnmpServerSnmpConfigSetLocationMessageHandler( SnmpServerSnmpConfigSetLocationMessage *pSnmpServerSnmpConfigSetLocationMessage)
    {
        trace (TRACE_LEVEL_INFO, "System Group Worker::Set Location Msg Handler:Entering ");

		if (pSnmpServerSnmpConfigSetLocationMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Location:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
        string locationStr      = pSnmpServerSnmpConfigSetLocationMessage->getLocation();

		if ((locationStr.length() < MIN_M2DISPLAYSTRSIZE) || 
			(locationStr.length() > M2DISPLAYSTRSIZE -1)) {
			pSnmpServerSnmpConfigSetLocationMessage->setCompletionStatus (WRC_SNMP_INVALID_LOCATION_LENGTH);
            reply (pSnmpServerSnmpConfigSetLocationMessage);
            return;
		}

		SnmpServerIntSnmpConfigSetLocationIntMessage *pMessage =
					new SnmpServerIntSnmpConfigSetLocationIntMessage (locationStr);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));


        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigSystemGroupWorker::setLocationSTCCB), pSnmpServerSnmpConfigSetLocationMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
 
    }
    void ConfigSystemGroupWorker::setLocationSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetLocationMessage *pSnmpServerSnmpConfigSetLocationMessage = 
                                (SnmpServerSnmpConfigSetLocationMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed     = false;
        ResourceId status        = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus       = WAVE_MESSAGE_SUCCESS; 
		string     locationStr   = pSnmpServerSnmpConfigSetLocationMessage->getLocation();
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting snmp location operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting snmp location operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delCommSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        { 
            SystemGroupManagedObject *pSysGroupMO = NULL;
            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext =             
                new WaveManagedObjectSynchronousQueryContext(SystemGroupManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +locationStr);
                pSnmpServerSnmpConfigSetLocationMessage->setCompletionStatus
                                    (WRC_SNMP_SYSTEMGROUP_DOESNT_EXIST);
				
				// clean up allocated messages
				SnmpServerIntSnmpConfigSetLocationIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetLocationIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetLocationMessage);
                delete synchronousQueryContext;
                return;
            }		

            pSysGroupMO = dynamic_cast<SystemGroupManagedObject *> ((*pResults)[0]);
            delete pResults;
            delete synchronousQueryContext;
            startTransaction ();
            updateWaveManagedObject (pSysGroupMO);
            if (locationStr.length() != 0)
                pSysGroupMO->setLocation(locationStr);
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {             
                trace (TRACE_LEVEL_INFO, "SNMP SystemGroup Loc SET: SuccessfulCluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, 
                        "SNMP System Group Location SET:Commiting failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }

            delete pSysGroupMO;
        }


        // clean up allocated messages
        SnmpServerIntSnmpConfigSetLocationIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetLocationIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigSetLocationMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigSetLocationMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigSetLocationMessage);
    }

	/* 
	**	This function is the global message handler to set sytem description string for System Group.
	**  Called from: System Group WyserEaConfiguration Worker.
	**  Inputs: 	 system description string length 1 to 256.
	**  Functions:	 Sends message to local handlers to set backend system group objects.
					 On success commits to database.
	**  Errors :	 Returns GET/SET failed or invalid system description string length.
	**  On SUCCESS:	 Returns WAVE_MESSAGE_SUCCESS.
	*/

    void
ConfigSystemGroupWorker::SnmpServerSnmpConfigSetSysDescrMessageHandler( SnmpServerSnmpConfigSetSysDescrMessage *pSnmpServerSnmpConfigSetSysDescrMessage)
    {
        trace (TRACE_LEVEL_INFO, "System Group Worker::Set System Description Msg Handler:Entering ");

		if (pSnmpServerSnmpConfigSetSysDescrMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set System Description :NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
        string sysDescrStr = pSnmpServerSnmpConfigSetSysDescrMessage->getSysDescr();

		if ((sysDescrStr.length() < MIN_M2DISPLAYSTRSIZE) || 
			(sysDescrStr.length() > M2DISPLAYSTRSIZE -1)) {
			pSnmpServerSnmpConfigSetSysDescrMessage->setCompletionStatus (WRC_SNMP_INVALID_SYSDESCR_LENGTH);
            reply (pSnmpServerSnmpConfigSetSysDescrMessage);
            return;
		}

		SnmpServerIntSnmpConfigSetSysDescrMessage *pMessage =
					new SnmpServerIntSnmpConfigSetSysDescrMessage (sysDescrStr);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigSystemGroupWorker::setsysDescrSTCCB), pSnmpServerSnmpConfigSetSysDescrMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
 
    }

    void ConfigSystemGroupWorker::setsysDescrSTCCB(WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetSysDescrMessage *pSnmpServerSnmpConfigSetSysDescrMessage = 
                                (SnmpServerSnmpConfigSetSysDescrMessage*)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed     = false;
        ResourceId status        = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus       = WAVE_MESSAGE_SUCCESS; 
		string     sysdescrStr   = pSnmpServerSnmpConfigSetSysDescrMessage->getSysDescr();
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting snmp System Description operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting snmp System Description operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delCommSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        { 
            SystemGroupManagedObject *pSysGroupMO = NULL;
            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext =             
                new WaveManagedObjectSynchronousQueryContext(SystemGroupManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +sysdescrStr);
                pSnmpServerSnmpConfigSetSysDescrMessage->setCompletionStatus
                                    (WRC_SNMP_SYSTEMGROUP_DOESNT_EXIST);
				// clean up allocated messages
				SnmpServerIntSnmpConfigSetSysDescrMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetSysDescrMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetSysDescrMessage);
                delete synchronousQueryContext;
                return;
            }		

            pSysGroupMO = dynamic_cast<SystemGroupManagedObject *> ((*pResults)[0]);
            delete pResults;
            delete synchronousQueryContext;
            startTransaction ();
            updateWaveManagedObject (pSysGroupMO);
            if (sysdescrStr.length() != 0)
                pSysGroupMO->setSysDescr(sysdescrStr);
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {             
                trace (TRACE_LEVEL_INFO, "SNMP SystemGroup Sys Description SET: SuccessfulCluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, 
                        "SNMP System Group Sys Description SET:Commiting failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }

            delete pSysGroupMO;
        }

        // clean up allocated messages
        SnmpServerIntSnmpConfigSetSysDescrMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetSysDescrMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigSetSysDescrMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigSetSysDescrMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigSetSysDescrMessage);
	}
}
