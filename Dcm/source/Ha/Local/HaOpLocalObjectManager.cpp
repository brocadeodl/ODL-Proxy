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
 *   October, 2011                                                         *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "Ha/Local/HaOpLocalObjectManager.h"
#include "Ha/Local/HaOpActionMessage.h"
#include "Ha/Local/HaShowLocalMessage.h"
#include "Ha/Local/HaOpTypes.h"

#include <string>
#include "errno.h"

namespace DcmNs
{

	HaOpLocalObjectManager::HaOpLocalObjectManager ()
		: WaveLocalObjectManager  (getClassName ())
	{
		addOperationMap (HAOPACTION, reinterpret_cast<PrismMessageHandler> (&HaOpLocalObjectManager::HaOpActionMessageHandler));
		addOperationMap (HASHOWACTION, reinterpret_cast<PrismMessageHandler> (&HaOpLocalObjectManager::HaShowLocalMessageHandler));
	}

	HaOpLocalObjectManager::~HaOpLocalObjectManager ()
	{
	}

	HaOpLocalObjectManager  *HaOpLocalObjectManager::getInstance()
	{
		static HaOpLocalObjectManager *pHaOpLocalObjectManager = new HaOpLocalObjectManager ();

		WaveNs::prismAssert (NULL != pHaOpLocalObjectManager, __FILE__, __LINE__);

		return (pHaOpLocalObjectManager);
	}

	string  HaOpLocalObjectManager::getClassName()
	{
		return ("Ha Operation Local Manager");
	}

	PrismServiceId  HaOpLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *HaOpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			case HAOPACTION :
				pPrismMessage = new HaOpActionMessage ();
				break;
			case HASHOWACTION :
				trace (TRACE_LEVEL_INFO, "HaOpLocalObjectManager::createMessageInstance  HaShowLocalMessage create..");
				pPrismMessage = new HaShowLocalMessage ();
				break;
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

/*****************************************************
 To allow certain operations BEFORE Plugin Service
 gets Enabled during cluster formation

 Note:
  Please be judicious in allowing such operations.
  These operations should access backend daemons ONLY.
  NO DCM DATABASE related operations should EVER BE
  allowed BEFORE the Plugin Service gets enabled!!!
  This is critical towards maintaining Db integrity.

******************************************************/
	bool HaOpLocalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
	{
		switch (operationCode) {
				case HAOPACTION :
					return (true);
				case HASHOWACTION :
					return (true);
			default:
				return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
		}
	}

	WaveManagedObject  *HaOpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
	}

#define HACLI_FILE "/var/log/haresult.data"
#define CLUSTER_MESSAGE 1
	void  HaOpLocalObjectManager::HaOpActionMessageHandler( HaOpActionMessage *pHaOpActionMessage)
	{
		FILE    *cmd_file;
		int     rc, ChassisReboot = 0;
		char    cmd_output[512];
		string result = "";
		UI32 rbridgeId = 0;
		UI32 flag = 0;

		WaveClientSessionContext waveClientSessionContext = pHaOpActionMessage->getWaveClientSessionContext();
		trace (TRACE_LEVEL_INFO, "HaOpActionMessageHandler enter..");

		string userCommand = pHaOpActionMessage->getCommand();
		rbridgeId = pHaOpActionMessage->getRbridgeId();
		flag = pHaOpActionMessage->getFlag();
		trace (TRACE_LEVEL_INFO, string("HaOpActionMessageHandler Flag ") + flag);
		if (strstr((char *)userCommand.data(), "chassisReboot") && (flag == CLUSTER_MESSAGE)) {
			trace (TRACE_LEVEL_INFO, "HaOpActionMessageHandler ChassisReboot");
			ChassisReboot = 1;
			pHaOpActionMessage->setRbridgeId(rbridgeId);
			pHaOpActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pHaOpActionMessage);
			trace (TRACE_LEVEL_INFO, "HaOpActionMessageHandler replied");
			sleep(5);
			trace (TRACE_LEVEL_INFO, "HaOpActionMessageHandler 5 sec sleep done");
		}

		trace (TRACE_LEVEL_INFO, string("HaOpActionMessageHandler get command: ") +
			userCommand + string(", rbridgeId: ") + rbridgeId);

		if ((cmd_file = popen((char *)userCommand.data(), "r")) != NULL) {

			while (fgets(cmd_output, 512, cmd_file)) {
				trace (TRACE_LEVEL_INFO, string("FOAMessageHandler: In fgets : ") + cmd_output);
				result.append(cmd_output);
//				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
				// fflush(cmd_file);
			}

			result.append(string("\n"));
			pHaOpActionMessage->setResult(result);
			if ((rc = pclose(cmd_file)) < 0) {

				trace (TRACE_LEVEL_INFO, string("FOAMessageHandler: pclose failed - CMD = ") + userCommand.data() + string (" rc=") + rc + string (" errno=") + -errno);
				printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			}

		} else {

			trace (TRACE_LEVEL_INFO, string("FOAMessageHandler: popen failed - CMD =  ") + userCommand.data() + string (" errno=") + -errno);
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			pHaOpActionMessage->setResult(string ("Command execution failed.\n"));
		}
		printfToWaveClientSessionOver(waveClientSessionContext);
		if (!ChassisReboot) {
			pHaOpActionMessage->setRbridgeId(rbridgeId);
			pHaOpActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply (pHaOpActionMessage);
			trace (TRACE_LEVEL_INFO, "HaOpActionMessageHandler replied");
		}
	}
/*
	void  HaOpLocalObjectManager::HaShowLocalMessageHandler(HaShowLocalMessage *pHaShowLocalMessage)
	{
		PrismLinearSequencerContext *pHaShowLocalContext = NULL;
		PrismLinearSequencerStep sequencerSteps[] = {
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpLocalObjectManager::HaShowLocalHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpLocalObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&HaOpLocalObjectManager::prismLinearSequencerFailedStep),
		};
		pHaShowLocalContext = new PrismLinearSequencerContext (pHaShowLocalMessage, this,
				sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pHaShowLocalContext->setPPrismMessage (pHaShowLocalMessage);
		pHaShowLocalContext->start ();
	}
*/
	void HaOpLocalObjectManager::HaShowLocalMessageHandler(HaShowLocalMessage *pHaShowLocalMessage)
	{
		string data = "";
		char cmdline[512];
		FILE    *cmd_op = NULL;
		char    cmd_output[513];
		int rc = 0;

		string cmd = pHaShowLocalMessage->getCmd();
		trace (TRACE_LEVEL_DEBUG, string ("HaOpLocalObjectManager::HaShowLocalHandler"));
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
		snprintf(cmdline, 512, "%s 2>/dev/null", (char *)cmd.data());
		if ((cmd_op = popen(cmdline, "r")) != NULL) {
			if (locationRole != LOCATION_STAND_ALONE)
				data.append(string("Rbridge-id: ") + pHaShowLocalMessage->getMappedId() + string("\n"));

			while (fgets(cmd_output, 512, cmd_op)) {
				data.append( cmd_output );
			}
			trace (TRACE_LEVEL_DEBUG, string ("HaOpLocalObjectManager::HaShowLocalHandler: data = ") + data);
			if ((rc = pclose(cmd_op)) < 0) {
				trace (TRACE_LEVEL_DEBUG, string ("HaOpLocalObjectManager::HaShowLocalHandler pclose fail:") +
						cmdline + string (" rc=") + rc + string (" errno=") + -errno);
				data.append( string( "1. Command Fail rc=" ) + rc + "\n" );
			}
		} else {
			 trace (TRACE_LEVEL_DEBUG, string ("HaOpLocalObjectManager::HaShowLocalHandler popen fail:") +
					 cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			 data.append( string( "2. Command Fail rc=" ) + rc + "\n" );
		}
		// Escape `%` in the output buffer as maapi_cli_vprintf function is invoked
		// which behaves like printf & expects arguments which are
		// not present in these cases. So replace `%` with `%%`
		string escapeWith = "%";
		DcmToolKit::insertEscapeCharInBuffer(data, '%', escapeWith);

		data.append(string("\n"));
		pHaShowLocalMessage->setData(data);
		pHaShowLocalMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		reply (pHaShowLocalMessage);
		trace (TRACE_LEVEL_DEBUG, string ("HaOpLocalObjectManager::HaShowLocalHandler replied"));
	}
}
