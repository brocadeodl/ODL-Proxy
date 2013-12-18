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
 *   Author : James Chen                                                   *
 **************************************************************************/


#include "RAS/Local/RASLocalSSThread.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/TraceUtils.h"

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "DcmCore/DcmToolKit.h"
#include <ha/ham_api.h>
#include <utils/slot.h>

#include <utils/support.h>
#include <raslog/raslog.h>
#include <raslog/raslog_ss.h>

namespace DcmNs
{

	RASLocalSSThread::RASLocalSSThread(WaveObjectManager *pWaveObjectManager, RASSupportMessage *pRASSupportMessage)
		: WaveWorker(pWaveObjectManager, false),
		PrismPosixThread(),
		m_pRASSupportMessage(pRASSupportMessage)
	{
	}	

	RASLocalSSThread::~RASLocalSSThread()
	{
	}

	WaveThreadStatus RASLocalSSThread::start()
	{
		trace (TRACE_LEVEL_DEBUG, "RASLocalSSThread::start");

    	// setup the sequence 
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalSSThread::SSCLI),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalSSThread::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalSSThread::prismSynchronousLinearSequencerFailedStep)
		};

		// put MSG to SSLocalMessagingContext
		RASLocalMessagingContext *pRASLocalMessagingContext =
			new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (m_pRASSupportMessage), this, sequencerSteps,
			sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
		pRASLocalMessagingContext->execute ();
		delete (this);
		return (WAVE_THREAD_SUCCESS);
	}

	ResourceId RASLocalSSThread::SSCLI(RASLocalMessagingContext *pRASLocalMessagingContext)
	{

		ss_progress_t ss;
		char *reason;

		// MSG from Actionpoint worker to carry data
		RASSupportMessage *pRASSupportMessage = reinterpret_cast<RASSupportMessage *> (pRASLocalMessagingContext->getPPrismMessage ());

		trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::WaveClientContext in RASSupportMessage = ") +
		pRASSupportMessage->getWaveClientOriginatingLocationId () + " " +
		pRASSupportMessage->getWaveNativeClientId () + " " +
		pRASSupportMessage->getWaveUserClientId());

		string data = "";
		char cmdline[512];
		int rc = 0;
		int rbridge_id = DcmToolKit::getLocalMappedId();
		string cmd = pRASSupportMessage->getCmd();
		trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::fosCLI=") + cmd);

		if (strstr(cmd.c_str(), "supportsave")) {
			raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, SS_2000, rbridge_id);
		}
		// if netconf

		if (pRASSupportMessage->getNetConf() == true) {
			snprintf(cmdline, sizeof (cmdline), "%s 2>/dev/null &", (char *)cmd.data());
			tracePrintf(TRACE_LEVEL_DEBUG, true, true, "SSLocalOM::NetConf SupportSave cmd = %s\n", cmdline);
			rc = system(cmdline);
			if ((WEXITSTATUS(rc) == 0)) {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, SS_2001, rbridge_id);
			} else {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, SS_2002, rbridge_id);
			}
			return ((WEXITSTATUS(rc) != 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
		}

		/*	By default all RAS show commands come in with sync flag set.
			This means that the command output is not sent to the CLI session
			but saved in a data buffer and sent back to be displayed by the
			respective wysereaoperation handler. */

		if (pRASSupportMessage->getSync()) {
			data = syncDataOutputHandler(pRASLocalMessagingContext);
			pRASSupportMessage->setData(data);
		} else {
			asyncDataOutputHandler(pRASLocalMessagingContext);
			ss_get_status(0, &ss);
			if(ss.percentage < 100) {
				if (ss.msg != NULL) {
					reason = strrchr(ss.msg, ':');
					if (reason != NULL) {
						reason++;
						pRASSupportMessage->setData(string(reason));
						trace (TRACE_LEVEL_INFO, string ("SSLocalOM:: SS failure reason retrieved: ") +
							string(ss.msg));
					}
				}
				trace (TRACE_LEVEL_INFO, "SSLocalOM:: Returning Wave_MESSAGE_ERROR");
				return (WAVE_MESSAGE_ERROR);
			}
		}
		return (WAVE_MESSAGE_SUCCESS);
	}


	string RASLocalSSThread::syncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext)
	{
		RASSupportMessage *pRASSupportMessage = reinterpret_cast<RASSupportMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
		string data = "";
		char cmdline[512];
		FILE	*cmd_op = NULL;
		char	cmd_output[513];
		int rc = 0, this_slot = 0;
		char slot_string[10];
		string slotstr = "";
		string cmd = pRASSupportMessage->getCmd();
		int localId = DcmToolKit::getLocalMappedId();
		int raslog = 0;

		trace (TRACE_LEVEL_INFO, string ("SSLocalOM::Into syncDataOutputHandler"));

		snprintf(cmdline, 512, "%s 2>/dev/null", (char *)cmd.data());

		if (strstr(cmdline, "supportsave")) {
			trace (TRACE_LEVEL_INFO, string ("SSLocalOM::Into syncDataOutputHandler"
				"cmdline: ") + cmdline);
			raslog = 1;
		}

		if ((cmd_op = popen(cmdline, "r")) != NULL) {
			if (pRASSupportMessage->getMappedId() != 0)
				data.append(string("Rbridge-id: ") + localId + string("\n"));

			// display CLI start
			if (!(pRASSupportMessage->getCLIStart()).empty()) {
				data.append(pRASSupportMessage->getCLIStart() + "\n" );
			}

			if ((strstr(cmdline, "coreshow")) ||
				(strstr(cmdline, "coreffdc"))) {
				if ((this_slot = getThisCPSlot()) != 0) {
					snprintf(slot_string, 10, "%s", slot_no2s(this_slot));
					slotstr = slot_string;
					trace (TRACE_LEVEL_INFO, string("slotString: ") + slotstr);
					 data.append(string("#") + slotstr + string(":\n"));
				}
			}

			while (fgets(cmd_output, 512, cmd_op)) {
				data.append( cmd_output );
			}

			if ((rc = pclose(cmd_op)) < 0) {
				trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::fosCLI pclose fail:") +
					cmdline + string (" rc=") + rc + string (" errno=") + -errno);
				data.append( string( "1. Command Fail rc=" ) + rc + "\n" );
				if (raslog) {
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2002, localId);
				}
			}

			// display CLI completion
			if (!(pRASSupportMessage->getCLIEnd()).empty()) {
				data.append(pRASSupportMessage->getCLIEnd() + "\n" );
			}
			if (raslog) {
				if (strcasestr(cmd_output, "Copy support completed") != NULL) {
					trace (TRACE_LEVEL_INFO, string ("SSLocalOM::Into syncDataOutputHandler success"));
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2001, localId);
				} else {
					trace (TRACE_LEVEL_INFO, string ("SSLocalOM::Into syncDataOutputHandler fail"));
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2002, localId);
				}
			}
		} else {
			trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::fosCLI popen fail:") +
				cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			data.append( string( "2. Command Fail rc=" ) + rc + "\n" );
			if (raslog) {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, SS_2002, localId);
			}
		}

		// Escape `%` in the output buffer as maapi_cli_vprintf function is invoked
		// which behaves like printf & expects arguments which are
		// not present in these cases. So replace `%` with `%%`
		string escapeWith = "%";
		DcmToolKit::insertEscapeCharInBuffer(data, '%', escapeWith);

		data.append(string("\n"));
		trace (TRACE_LEVEL_INFO, string("Returning data from sync datahandler:") + data);
		cmd = "";
		if (strlen(cmdline) != 0) {
			memset(cmdline, 0, strlen(cmdline));
		}
		return (data);
	}

	void RASLocalSSThread::asyncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext)
	{
		RASSupportMessage *pRASSupportMessage = reinterpret_cast<RASSupportMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
		WaveClientSessionContext waveClientSessionContext = pRASSupportMessage->getWaveClientSessionContext ();
		string data = "";
		string cmd = pRASSupportMessage->getCmd();
		char cmdline[512];
		char rbd_str[128] = "";
		FILE	*cmd_op = NULL;
		char	cmd_output[513];
		int rc = 0;
		int localId = DcmToolKit::getLocalMappedId();
		int raslog = 0;

		trace(TRACE_LEVEL_DEBUG, string ("SSLocalOM::Into asyncDataOutputHandler"));

		if (pRASSupportMessage->getMappedId() != 0)
			snprintf(rbd_str, 128, "Rbridge-id :%d ", localId);

		snprintf(cmdline, 512, "%s 2>/dev/null", (char *)cmd.data());
		if (strstr(cmdline, "supportsave")) {
			raslog = 1;
		}

		if ((cmd_op = popen(cmdline, "r")) != NULL) {
			// display CLI start 
			if (!(pRASSupportMessage->getCLIStart()).empty()) {
				printfToWaveClientSession (waveClientSessionContext, "%s%s\n",
					rbd_str, pRASSupportMessage->getCLIStart().c_str());
			}

			while (fgets(cmd_output, 512, cmd_op)) {
				printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, cmd_output);
			}

			if ((rc = pclose(cmd_op)) < 0) {
				trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::fosCLI pclose fail:") +
					cmdline + string (" rc=") + rc + string (" errno=") + -errno);
				printfToWaveClientSession (waveClientSessionContext, "%s%s", rbd_str, "Command Fail\n");
				if (raslog) {
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2002, localId);
				}
			}

			// display CLI completion
			if (!(pRASSupportMessage->getCLIEnd()).empty()) {
				printfToWaveClientSession (waveClientSessionContext, "%s%s\n",
					rbd_str, pRASSupportMessage->getCLIEnd().c_str());
			}

			if (strcasestr(cmd_output, "Copy support completed") != NULL) {
				trace(TRACE_LEVEL_INFO, string ("SSLocalOM::Into asyncDataOutputHandler success"));
				if (raslog) {
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2001, localId);
				}
			} else {
				trace(TRACE_LEVEL_INFO, string ("SSLocalOM::Into asyncDataOutputHandler fail"));
				if (raslog) {
					raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS,
						SS_2002, localId);
				}
			}
		} else {
			trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::fosCLI popen fail:") +
				cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			printfToWaveClientSession (waveClientSessionContext, "%s%s",
				rbd_str, "Command Fail\n");
			if (raslog) {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, SS_2001, localId);
			}
		}

		cmd = "";
		if (strlen(cmdline) != 0) {
			memset(cmdline, 0, strlen(cmdline));
		}
	}
}

