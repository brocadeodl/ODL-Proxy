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
*   Author : bkesanam                                                     *
**************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"

#include "RAS/Local/RASNodeSpecificLocalObjectManager.h"
#include "RAS/Local/RASTypes.h"
#include "vcs.h"
#include "RAS/Local/RASNodeSpecificLocalManagedObject.h"
#include "RAS/Local/RASNodeSpecificConfigureMessage.h"
#include "RAS/Global/RASManagedObject.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmResourceIds.h"

#include <iostream>
#include <string>
// #include <fstream>
#include <errno.h> 
#include <raslog/raslogd.h>
#include <raslog/raslogm.h>
#include <raslog/raslog.h>
#include <raslog/raslog_ras.h>
#include <em/em_if.h>
#include <hasm/ls.h>
#include <hasm/hasm.h>

using namespace std;

namespace DcmNs
{
#if 0 	
	RASNodeSpecificLocalObjectManager::RASNodeSpecificLocalObjectManager ()
		: WaveLocalObjectManager  (getClassName ())
	{
		DcmShell::addToWaveShell ();

		// to support failover, cluster node removing
		associateWithVirtualWaveObjectManager (RASObjectManager::getInstance ());

		// push RASNodeSpecificLocalManagedObject to DCM DB
		RASNodeSpecificLocalManagedObject RasNodeSpecificLocalManagedObject (this);
		RasNodeSpecificLocalManagedObject.setupOrm ();
		addManagedClass (RASNodeSpecificLocalManagedObject::getClassName ());

		addOperationMap (RASLOGGING,
				reinterpret_cast<PrismMessageHandler> (&RASNodeSpecificLocalObjectManager::RASNodeSpecificConfigureMessageHandler));
	}

	RASNodeSpecificLocalObjectManager::~RASNodeSpecificLocalObjectManager ()
	{

	}

	string  RASNodeSpecificLocalObjectManager::getClassName()
	{
		return ("RASNodeSpecificLocalObjectManager");
	}

	RASNodeSpecificLocalObjectManager *RASNodeSpecificLocalObjectManager:: getInstance ()
	{
		static RASNodeSpecificLocalObjectManager *pRASNodeSpecificLocalObjectManager = new  RASNodeSpecificLocalObjectManager ();

		WaveNs::prismAssert (NULL !=  pRASNodeSpecificLocalObjectManager, __FILE__, __LINE__);

		return (pRASNodeSpecificLocalObjectManager);
	}

	PrismServiceId  RASNodeSpecificLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *RASNodeSpecificLocalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		trace (TRACE_LEVEL_INFO, string ("RASNodeSpecificLocalOM::createMessageInstance opcode=") + operationCode);
		switch (operationCode) {
			case RASLOGGING :
				pPrismMessage = new RASNodeSpecificConfigureMessage();
				break;

			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject  *RASNodeSpecificLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::createManagedObjectInstance=" + managedClassName);

		// support single Object Manager with multiple Manage Object
		if ((RASNodeSpecificLocalManagedObject::getClassName ()) == managedClassName) {
			pWaveManagedObject = new RASNodeSpecificLocalManagedObject (this);

		} else {
			trace (TRACE_LEVEL_FATAL, "RASNodeSpecificLocalOM::createManagedObjectInstance : Unknown Managed Class Name : " +
					managedClassName);
		}

		return (pWaveManagedObject);
	}

	void RASNodeSpecificLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		ResourceId   status    = FRAMEWORK_ERROR;

		startTransaction ();
		RASNodeSpecificLocalManagedObject *pRasNodeSpecificLocalManagedObj = new RASNodeSpecificLocalManagedObject (this);
		pRasNodeSpecificLocalManagedObj->setFFDC(true);
		pRasNodeSpecificLocalManagedObj->setConsole(DCM_RASLOG_INFO);

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status) {
			trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalObjectManager::Install: Installed node specific MO.");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_FATAL, "RASNodeSpecificLocalObjectManager::Install: Failed to install node specific MO.");
		}

		delete (pRasNodeSpecificLocalManagedObj);
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	void RASNodeSpecificLocalObjectManager::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::postboot: Entered postBoot");

		/* sync host-name, ffdc and console filter */
		loggingBootReplay();

		status = WAVE_MESSAGE_SUCCESS;

		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
		pWaveAsynchronousContextForPostBootPhase->callback ();

		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::postboot: Exiting Boot");
	}

	void RASNodeSpecificLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::listenForEvents");

		listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION,
				reinterpret_cast<PrismEventHandler> (&RASNodeSpecificLocalObjectManager::bootCompleteForThisLocationEventHandler));

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	void RASNodeSpecificLocalObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
	{
		reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
	}

	// MSG handler to set RAS/System/Support config data
	void  RASNodeSpecificLocalObjectManager::RASNodeSpecificConfigureMessageHandler(RASNodeSpecificConfigureMessage *pRASNodeSpecificConfigureMessage)
	{
		// Validations and Queries should go here
		ObjectId  OID = pRASNodeSpecificConfigureMessage->getObjectId ();
		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler");
		char cmdline[512];
		string Data;

		RASNodeSpecificLocalManagedObject *pRasNodeManagedObj	=	NULL;
		WaveManagedObject	*pWaveManagedObject	=	NULL;
		ResourceId	status	=	WAVE_MESSAGE_SUCCESS;
		FILE	*cmd_op	=	NULL;
		char cmd_output[513];
		int	rc	=	0;

		WaveClientSessionContext waveClientSessionContext	=	pRASNodeSpecificConfigureMessage->getWaveClientSessionContext ();
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

		// Find MO from OID/KeyName
		pWaveManagedObject = queryManagedObject (OID);
		if (NULL == pWaveManagedObject) {
			trace (TRACE_LEVEL_ERROR, "RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler : No MO Found:");
			pRASNodeSpecificConfigureMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
			reply (pRASNodeSpecificConfigureMessage);
			return;
		}

		pRasNodeManagedObj = dynamic_cast<RASNodeSpecificLocalManagedObject *> (pWaveManagedObject);

		prismAssert (NULL != pRasNodeManagedObj, __FILE__, __LINE__);

		// For cfg set request, how about for remove ??
		switch (pRASNodeSpecificConfigureMessage->getWyserUserTag()) {
			case ras_ffdc:
				trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler : Set ffdc = " +
					pRASNodeSpecificConfigureMessage->getBoolData());
				if (pRASNodeSpecificConfigureMessage->getBoolData()) {
					snprintf(cmdline, 512, "/fabos/sbin/supportffdc --enable 2>/dev/null");
				} else { 
					snprintf(cmdline, 512, "/fabos/sbin/supportffdc --disable 2>/dev/null");
				}

				pRasNodeManagedObj->setFFDC(pRASNodeSpecificConfigureMessage->getBoolData());
				break;
			case ras_console:
				trace (TRACE_LEVEL_DEBUG, "RASNSLocalOM::RASNodeSpecificConfigureMessageHandler : Set console = " +
					pRASNodeSpecificConfigureMessage->getEnumData());
				pRasNodeManagedObj->setConsole((Severity)pRASNodeSpecificConfigureMessage->getEnumData());
				switch (pRasNodeManagedObj->getConsole()) {
					case DCM_RASLOG_INFO:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v info 2>/dev/null");
						break;
					case DCM_RASLOG_WARNING:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v warning 2>/dev/null");
						break;
					case DCM_RASLOG_ERROR:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v error 2>/dev/null");
						break;
					case DCM_RASLOG_CRITICAL:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v critical 2>/dev/null");
						break;
					default:
						trace (TRACE_LEVEL_ERROR, "RASNSLocalOM::RASNodeSpecificConfigureMessageHandler : unknown console type");
						break;
				}
				break;		  
			default:
				trace (TRACE_LEVEL_ERROR, string ("RASWyserEaConfigurationWorker::RASNodeSpecificConfigureMessageHandler : Unsupported tag=") +
				pRASNodeSpecificConfigureMessage->getWyserUserTag());
				break;
		}

		if ((cmd_op = popen(cmdline, "r")) != NULL) {
			while (fgets(cmd_output, 512, cmd_op)) {
				if (pRASNodeSpecificConfigureMessage->getCLIOutput())
					trace (TRACE_LEVEL_INFO, string ("switch ") + pRASNodeSpecificConfigureMessage->getMappedId() +
							string(": ") + cmd_output);
				if (locationRole == LOCATION_STAND_ALONE) {
					printfToWaveClientSession (waveClientSessionContext, "%s",
							cmd_output);
				} else {
					printfToWaveClientSession (waveClientSessionContext, "switch %d: %s",
						pRASNodeSpecificConfigureMessage->getMappedId(), cmd_output);
				}
			}
			if ((rc = pclose(cmd_op)) != 0) {
				trace (TRACE_LEVEL_INFO, string ("RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler pclose fail:") +
					cmdline + string (" rc=") + rc + string (" errno=") + -errno);
				pRASNodeSpecificConfigureMessage->setCompletionStatus (status);
				reply (pRASNodeSpecificConfigureMessage);
				if (pRasNodeManagedObj)
				    delete pRasNodeManagedObj;
				return;
			}
		} else {
			trace (TRACE_LEVEL_INFO, string ("RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler popen fail:") +
				cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s","Command Failed\n");
				pRASNodeSpecificConfigureMessage->setCompletionStatus (status);
				reply (pRASNodeSpecificConfigureMessage);
				if (pRasNodeManagedObj)
		    		delete pRasNodeManagedObj;
					return;
			} else {
				printfToWaveClientSession (waveClientSessionContext, "switch %d: %s",
				pRASNodeSpecificConfigureMessage->getMappedId(), "Command Failed\n");
				pRASNodeSpecificConfigureMessage->setCompletionStatus (status);
				reply (pRASNodeSpecificConfigureMessage);
				if (pRasNodeManagedObj)
				    delete pRasNodeManagedObj;
				return;
			}
		}

		startTransaction ();
		updateWaveManagedObject (pRasNodeManagedObj);
		status = commitTransaction ();

		if (FRAMEWORK_SUCCESS == status) {
			trace (TRACE_LEVEL_INFO,
					"RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler : Successfully committed the Logging Configuration");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_INFO,
				"RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler : Commiting the Interface configuration failed.  Status : " +
				FrameworkToolKit::localize (status));
		}

		delete  pRasNodeManagedObj;

		pRASNodeSpecificConfigureMessage->setCompletionStatus (status);
		reply (pRASNodeSpecificConfigureMessage);
	}

	void RASNodeSpecificLocalObjectManager::loggingBootReplay()
	{
		RASNodeSpecificLocalManagedObject *pRasNodeManagedObj = NULL;
		string cmd_data;
		bool get_ffdc;
		char cmd_op[512];

		trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalObjectManager::loggingBootReplay: Entered....");

		int localId = DcmToolKit::getLocalMappedId();
		string id = string ("") + localId;

		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(RASNodeSpecificLocalManagedObject::getClassName());
		//syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		trace (TRACE_LEVEL_INFO, string ("RASNodeSpecificLocalObjectManager::loggingBootReplay: find switchid=") + id);
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
		delete syncQueryContext;

		if ((NULL == pResults) || (pResults->size() == 0)) {
			tracePrintf(TRACE_LEVEL_INFO, true, true, "RASNodeSpecificLocalOM::loggingBootReplay no Logging MO 0\n");
			return ;
		}

		pRasNodeManagedObj = dynamic_cast<RASNodeSpecificLocalManagedObject *> ((*pResults)[0]);
		if (pRasNodeManagedObj == NULL) {
			tracePrintf(TRACE_LEVEL_INFO, true, true, "RASNodeSpecificLocalOM::loggingBootReplay no Logging MO 1\n");
			return ;	
		}

		switch (pRasNodeManagedObj->getConsole()) {
			case DCM_RASLOG_INFO:
				snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v info 2>/dev/null");
				break;
			case DCM_RASLOG_WARNING:
				snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v warning 2>/dev/null");
				break;
			case DCM_RASLOG_ERROR:
				snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v error 2>/dev/null");
				break;
			case DCM_RASLOG_CRITICAL:
				snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v critical 2>/dev/null");
				break;
			default:
				trace (TRACE_LEVEL_ERROR, "RASNodeSpecificLocalOM::RASNodeSpecificConfigureMessageHandler : unknown console type");
				break;
		}
		trace (TRACE_LEVEL_INFO, string ("RASNodeSpecificLocalObjectManager::loggingBootReplay: Set console=") +
				pRasNodeManagedObj->getConsole());
		/* system(cmd_op); */

		if ((get_ffdc = pRasNodeManagedObj->getFFDC())) {
			system("/fabos/sbin/supportffdc --enable >/dev/null 2>&1");
			trace (TRACE_LEVEL_INFO, "RASNodeSpecificLocalObjectManager::loggingBootReplay: FFDC enabled");
		} else {
			system("/fabos/sbin/supportffdc --disable >/dev/null 2>&1");
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

		return;
	}
#endif	
}
