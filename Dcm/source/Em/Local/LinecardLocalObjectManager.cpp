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
 *   Author : hungn                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Em/Local/LinecardLocalObjectManager.h"
#include "ClientInterface/Lccli/LccliClientMessage.h"
#include "Em/Local/LinecardPowerSetMessage.h"
#include "Em/Local/SlotShowInfoMessage.h"
#include "Em/Local/LinecardTypes.h"
#include "Em/Local/LinecardCmdMessage.h"

#include "switch/switch.h"
#include "em/em_if.h"
#include "utils/slot.h"

/* For Blade Disable/Enable APIs - fbladeEnable/fbladeDisable */
#include <fkio/fkiolib.h>

namespace DcmNs
{

	LinecardLocalObjectManager::LinecardLocalObjectManager ()
		: WaveLocalObjectManager  (getClassName ())
	{
		addOperationMap (LINECARDPOWERSET, reinterpret_cast<PrismMessageHandler> (&LinecardLocalObjectManager::LinecardPowerSetMessageHandler));
	    addOperationMap (SLOTSHOWINFO, reinterpret_cast<PrismMessageHandler> (&LinecardLocalObjectManager::SlotShowInfoMessageHandler));
		addOperationMap (LINECARDCMD, reinterpret_cast<PrismMessageHandler> (&LinecardLocalObjectManager::LinecardCmdMessageHandler));

	}

	LinecardLocalObjectManager::~LinecardLocalObjectManager ()
	{
	}

	LinecardLocalObjectManager  *LinecardLocalObjectManager::getInstance()
	{
		static LinecardLocalObjectManager *pLinecardLocalObjectManager = new LinecardLocalObjectManager ();

		WaveNs::prismAssert (NULL != pLinecardLocalObjectManager, __FILE__, __LINE__);

		return (pLinecardLocalObjectManager);
	}

	string  LinecardLocalObjectManager::getClassName()
	{
		return ("Linecard");
	}

	PrismServiceId  LinecardLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *LinecardLocalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			case LINECARDPOWERSET :
				pPrismMessage = new LinecardPowerSetMessage ();
				break;
			case SLOTSHOWINFO:
				pPrismMessage = new SlotShowInfoMessage ();
				break;
			case LINECARDCMD:
				pPrismMessage = new LinecardCmdMessage ();
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
	bool LinecardLocalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
	{
		switch (operationCode) {
				case LINECARDPOWERSET :
				case SLOTSHOWINFO:
				case LINECARDCMD:
					return (true);
			default:
				return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
		}
	}

	WaveManagedObject  *LinecardLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
	}

/*****************************************************
*  Turn on/off linecard  API
******************************************************/

	ResourceId LinecardLocalObjectManager::LinecardPowerSet(PrismSynchronousLinearSequencerContext *pLinecardPowerSetMessageContext)
	{
		int rval, maxSlot;
		unsigned int mode;
		int lcnum, slotnum,  id = SYS_SLOT_LC;
		LinecardPowerSetMessage *pLinecardPowerSetMessage =
		 reinterpret_cast<LinecardPowerSetMessage *> (pLinecardPowerSetMessageContext->getPPrismMessage ());

		trace (TRACE_LEVEL_INFO, "************* Linecard Plugin: Power-set entering...");

		/* not support for pizza box */
		maxSlot = chassisNumSlots();	
		if (maxSlot == 1) {
			trace(TRACE_LEVEL_INFO, "linecard plugin: not support on pizza box\n"); 	
			pLinecardPowerSetMessage->setResult(EMNOTSUPPORTED);
			return (WAVE_MESSAGE_SUCCESS);
		}	
		lcnum = pLinecardPowerSetMessage->getSlotnum();
		mode = pLinecardPowerSetMessage->getMode();
        slotnum = slot_id2no(id, lcnum);
		if (mode == 1) {			 
			trace (TRACE_LEVEL_INFO, string("***Linecard Plugin: Power-set : calling em_SlotOn slot = ") + slotnum);
			rval = em_SlotPowerOn(0, slotnum);
		} else if (mode == 0){			
			trace (TRACE_LEVEL_INFO, string("***Linecard Plugin: Power-set :calling em_SlotOff slot = ") + slotnum);
			rval = em_SlotPowerOff(0, slotnum);
		} else if (mode == 2){/* Reusing Power-set calls for blade enable/disable */
			trace (TRACE_LEVEL_INFO, string("***Linecard Plugin: Power-set :calling fbladeEnable slot = ") + slotnum);
			rval = fbladeEnable(slotnum);
		} else if (mode == 3){/* Reusing Power-set calls for blade enable/disable */
			trace (TRACE_LEVEL_INFO, string("***Linecard Plugin: Power-set :calling fbladeDisable slot = ") + slotnum);
			rval = fbladeDisable(slotnum);
		}
		trace(TRACE_LEVEL_INFO,
		string("*****Linecard Plugin: Power-set: rval = ") + rval);
		pLinecardPowerSetMessage->setResult(rval);
		return (WAVE_MESSAGE_SUCCESS);
	}

	void  LinecardLocalObjectManager::LinecardPowerSetMessageHandler( LinecardPowerSetMessage *pLinecardPowerSetMessage)
	{
	// Validations and Queries should go here

		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
	// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
	// Programming Protocol Daemons goes here
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::LinecardPowerSet),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
		};

		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLinecardPowerSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismSynchronousLinearSequencerContext->execute ();
	}

/*****************************************************
*  Slotshow  API
******************************************************/

	ResourceId LinecardLocalObjectManager::SlotShowInfo(PrismSynchronousLinearSequencerContext *pSlotShowInfoMessageContext)
	{
		int rc, slotCount = 0, bopen = 0;
		unsigned int slotnum, cmdcode;
		char command[512];
		FILE *fd = NULL;
		string output = "";
		char cmd_output[513];
		SlotShowInfoMessage *pSlotShowInfoMessage =
		 reinterpret_cast<SlotShowInfoMessage *> (pSlotShowInfoMessageContext->getPPrismMessage ());

		trace (TRACE_LEVEL_INFO, "***** Slotshow Plugin:  entering...");
		slotnum = pSlotShowInfoMessage->getSlotnum();
		cmdcode = pSlotShowInfoMessage->getCmdCode();
		trace (TRACE_LEVEL_INFO, string("***slotshow Plugin: slot  =  ") +
							slotnum + string ("cmdcode = ") + cmdcode);
		rc = em_get_slot_count(&slotCount);
		if (rc != 0) {
			output.append("can not get slot infor.\n");
			pSlotShowInfoMessage->setOutput(output);
			trace (TRACE_LEVEL_INFO,
				string("*** Slotshow Plugin: fail to get slot %d ") + cmdcode);
			return (WAVE_MESSAGE_SUCCESS);
		}
		if (slotCount == 1) {
			output.append("%%Error: Not supported on this platform.\n");
			pSlotShowInfoMessage->setOutput(output);
			trace (TRACE_LEVEL_INFO,
				string("*** Slotshow Plugin: unsupported platform %d ") + cmdcode);
			return (WAVE_MESSAGE_SUCCESS);
		}

		/* individual slot num is not valid */
		if (slotnum != MAX_SLOT + 1) {
			output.append("Invalid identifier number\n");
			pSlotShowInfoMessage->setOutput(output);
			trace (TRACE_LEVEL_INFO,
			 string("*** Slotshow Plugin: unsupported cma %d ") + cmdcode);
			return (WAVE_MESSAGE_SUCCESS);
		}

		switch (cmdcode) {

		case SLOT_SHOW_LINECARD:
			snprintf(command, 512, "/fabos/sbin/lcshow -nos");
			bopen = 1;
			break;
		case SLOT_SHOW_SFM:
			snprintf(command, 512, "/fabos/sbin/sfmshow -nos");
			bopen = 1;
			break;
		case SLOT_SHOW_MM:
			snprintf(command, 512, "/fabos/sbin/mmshow -nos");
			bopen = 1;
			break;
		case SLOT_SHOW_ALL:
			snprintf(command, 512, "/fabos/bin/slotShow -nos");
			bopen = 1;
			break;
		default:
			output.append("Invalid command\n");
			bopen = 0;
			break;
		}
		if (bopen == 0) {
			pSlotShowInfoMessage->setOutput(output);
			trace (TRACE_LEVEL_INFO, string("*** Slotshow Plugin: unsupported cmd %d exit...") + cmdcode);
			return (WAVE_MESSAGE_SUCCESS);
		}

		if ((fd = popen(command, "r")) != NULL) {
			while (fgets(cmd_output, 512, fd)) {
				trace (TRACE_LEVEL_INFO, string("sendSlotshowtoEm:"
										"In fgets : ") + cmd_output);
				output.append(cmd_output);
			}
			if ((rc = pclose(fd)) < 0) {
				trace (TRACE_LEVEL_INFO, string("sendSlotShowtoem:: "
					"pclose failed - CMD = ") + command + string (" rc=") +
											rc );
				output.append("Command execution Failed");
			}
		} else {
			trace (TRACE_LEVEL_INFO, string("sendSlotShowToEm: "
				"popen failed - CMD =  ") + command);
			output.append("Command execution failed.\n");
		}
		pSlotShowInfoMessage->setOutput(output);
		return (WAVE_MESSAGE_SUCCESS);
	}

	void  LinecardLocalObjectManager::SlotShowInfoMessageHandler(SlotShowInfoMessage *pSlotShowInfoMessage)
	{
	// Validations and Queries should go here

		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
	// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
	// Programming Protocol Daemons goes here
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::SlotShowInfo),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
		};

		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSlotShowInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismSynchronousLinearSequencerContext->execute ();
	}

/*****************************************************
*  Linecard CMD  API
******************************************************/

	ResourceId LinecardLocalObjectManager::LinecardCmd(PrismSynchronousLinearSequencerContext *pLinecardCmdMessageContext)
	{
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
		unsigned int slotnum;
		string cmd, output = "";
		UI32 size = 0;
		void *data;
		char *outputstr;
		lccli_output_msg *omsg = NULL;
		LccliMsg *client_message = NULL;
		LinecardCmdMessage *pLinecardCmdMessage =
			reinterpret_cast<LinecardCmdMessage *> (pLinecardCmdMessageContext->getPPrismMessage ());


		trace (TRACE_LEVEL_INFO, "***** LinecardCMd Plugin:  entering...");
		slotnum = pLinecardCmdMessage->getSlotnum();
		cmd = pLinecardCmdMessage->getCmd();
		trace (TRACE_LEVEL_INFO, string("***LinecardCmd Plugin: slot  =  ") + slotnum + string ("cmd = ") + cmd);


		client_message = new LccliMsg("", 0);
		client_message->setCliCode (LCCLI_MSG_CODE);
		client_message->setArg (cmd);
		status = sendSynchronouslyToWaveClient(LCCLI_CLIENT_NAME, client_message, slotnum);
		if (status != WAVE_MESSAGE_SUCCESS) {
			trace (TRACE_LEVEL_INFO, "***** LinecardCMd Plugin:  send fail...");
			pLinecardCmdMessage->setResult(LINECARD_CMD_FAIL_TO_SEND);
			pLinecardCmdMessage->setOutput(output);
			delete(client_message);
			return (WAVE_MESSAGE_SUCCESS);
		}


		if(client_message->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			trace (TRACE_LEVEL_ERROR,
				  string ("Linecardcmd: pluggin r:send to clien incomplete status=") +
						  client_message->getCompletionStatus());
			pLinecardCmdMessage->setResult(LINECARD_CMD_CLIENT_INCOMPLETE);
			pLinecardCmdMessage->setOutput(output);
			delete(client_message);
			return (WAVE_MESSAGE_SUCCESS);
		}

		/* get data pointer which setup in LCCLIMsg::loadOutputsFromCStructure() */
		client_message->getDuplicateInterfaceOutputBuffer(LCCLI_MSG_CODE, size, data);
		if (size == 0) {
			trace(TRACE_LEVEL_INFO, "LinecardCmd: pluggin no data size = 0");
			pLinecardCmdMessage->setResult(LINECARD_CMD_SUCCESS_NO_OUTPUT);
			pLinecardCmdMessage->setOutput(output);
			delete(client_message);
			return (WAVE_MESSAGE_SUCCESS);
		}

		omsg = (lccli_output_msg *)data;
		if ((omsg == NULL) || (omsg->out_buf_len == 0)) {
			trace(TRACE_LEVEL_INFO, "LinecardCmd: pluggin no data buffer size is 0");
			pLinecardCmdMessage->setResult(LINECARD_CMD_SUCCESS_NO_OUTPUT);
			pLinecardCmdMessage->setOutput(output);
			delete(client_message);
			return (WAVE_MESSAGE_SUCCESS);
		}
		pLinecardCmdMessage->setResult(LINECARD_CMD_SUCCESS_WITH_OUTPUT);
		outputstr = (( char *)data + sizeof(lccli_output_msg_t));
		output.append(outputstr);
		pLinecardCmdMessage->setOutput(output);
		delete(client_message);
		trace(TRACE_LEVEL_INFO, string("LinecardCmd: pluggin done datasizen %d ") + omsg->out_buf_len);
		return (WAVE_MESSAGE_SUCCESS);
	}

	void  LinecardLocalObjectManager::LinecardCmdMessageHandler(LinecardCmdMessage *pLinecardCmdMessage)
	{
	// Validations and Queries should go here

		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
	// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
	// Programming Protocol Daemons goes here
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::LinecardCmd),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LinecardLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
		};

		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLinecardCmdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismSynchronousLinearSequencerContext->execute ();
	}


}
