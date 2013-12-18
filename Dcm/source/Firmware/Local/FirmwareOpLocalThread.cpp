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
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Firmware/Local/FirmwareOpLocalThread.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "Firmware/Local/FirmwareOpTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/TraceUtils.h"
#include "hasm/sus.h"
#include "hasm/hasm.h"
#include <config/config_sw.h>
#include <config/resource.h>

#define RASCLI_FILE "/var/log/firmwareresult.data"

namespace DcmNs
{

FirmwareOpLocalThread::FirmwareOpLocalThread(WaveObjectManager *pWaveObjectManager, FirmwareOpActionMessage *pFirmwareOpActionMessage, NetconfFirmwareDownloadMessage *pNetconfMessage, UI32 msg_type)
	: WaveWorker(pWaveObjectManager, false),
	PrismPosixThread(),
	m_pFirmwareOpActionMessage(pFirmwareOpActionMessage),
	m_pNetconfMessage(pNetconfMessage),
	m_msg_type(msg_type)
{
}

FirmwareOpLocalThread::~FirmwareOpLocalThread()
{
}

WaveThreadStatus FirmwareOpLocalThread::start()
{

	switch (m_msg_type) {

	case FIRMWAREOPACTION: {
        FILE    *cmd_file;
		char	msg[MAX_SANITY_ERR_SIZE]="";
        char    cmd_output[512];
		size_t	sanity;
		size_t	pwd_pos;
        int     rc;
        int     fwsync;

		WaveClientSessionContext waveClientSessionContext =
					m_pFirmwareOpActionMessage->getWaveClientSessionContext();

		string userCommand = m_pFirmwareOpActionMessage->getCommand();
		
		/* remove password before logging the message */
		if ((userCommand.find("firmwaredownload") != string::npos) &&
				((userCommand.find("scp") != string::npos) ||
				 (userCommand.find("ftp") != string::npos) ||
				 (userCommand.find("sftp") != string::npos))) {
			if ((pwd_pos = userCommand.find_last_of(",")) != string::npos) {
				trace(TRACE_LEVEL_INFO, "FirmwareOpLocalThread::start:get command: "
						+ userCommand.substr(0, pwd_pos));
			}
		} else {
			trace(TRACE_LEVEL_INFO, "FirmwareOpLocalThread::start: get command: "
				+ userCommand);
		}

		sanity = userCommand.find("-S");

		if (sanity == string::npos) {
			if (userCommand.find("autosync") != string::npos) {
				if (userCommand.find("enable") != string::npos) {
					trace(TRACE_LEVEL_INFO, string("FirmwareOpLocalThread::"
						"start: firmware.sync enable"));
					fwsync = 1;
					configSet("firmware.sync", ResourceInt, &fwsync);
					printfToWaveClientSession (waveClientSessionContext,
						"%s", "Automatic firmware sync enabled.\n");
				} else {
					trace(TRACE_LEVEL_INFO, string("FirmwareOpLocalThread::"
						"start: firmware.sync disable"));
					fwsync = 0;
					configSet("firmware.sync", ResourceInt, &fwsync);
					printfToWaveClientSession (waveClientSessionContext,
						"%s", "Automatic firmware sync disabled.\n");
				}

				configGet("firmware.sync", ResourceInt, &fwsync);
				trace(TRACE_LEVEL_INFO, string(
					"FirmwareOpLocalThread::start: firmware.sync=") + fwsync);

			} else
        	if ((cmd_file = popen((char *)userCommand.data(), "r")) != NULL) {

            	while (fgets(cmd_output, 512, cmd_file)) {
                	printfToWaveClientSession(waveClientSessionContext, "%s",
						cmd_output);
            	}
            	m_pFirmwareOpActionMessage->setResult(string ("\n"));
            	if ((rc = pclose(cmd_file)) < 0) {
                	trace(TRACE_LEVEL_ERROR, string("FirmwareOpLocalThread::"
						"start: pclose failed - CMD = ") + userCommand.data() +
						string(" rc=") + rc + string (" errno=") + -errno);
                	printfToWaveClientSession(waveClientSessionContext, "%s",
						"Command Failed\n");
            	}

        	} else {

            	trace(TRACE_LEVEL_ERROR, string("FirmwareOpLocalThread::start:"
					" popen failed - CMD =  ") + userCommand.data() +
					string(" errno=") + -errno);
            	printfToWaveClientSession(waveClientSessionContext, "%s",
					"Command Failed\n");
            	m_pFirmwareOpActionMessage->setResult(string("Command "
					"execution failed.\n"));

        	}

		} else {			/* Sanity command */

        	printfToWaveClientSession (waveClientSessionContext, "Performing"
				" system sanity check...\n");
			rc = system(userCommand.c_str());
			rc = WEXITSTATUS(rc);
			if (rc == 255)	/* -1 is translated to (255<<8) by system() */
				rc = -1;	/* now translated back to -1 */
			m_pFirmwareOpActionMessage->setReturnCode(rc);
			sus_sanity_status_get(msg, MAX_SANITY_ERR_SIZE);
			m_pFirmwareOpActionMessage->setResult(string(msg));
		}
		userCommand = "";

		m_pFirmwareOpActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	    reply (m_pFirmwareOpActionMessage);
		trace (TRACE_LEVEL_INFO, "FirmwareOpLocalThread::start: replied");
	break;
	}

	case NETCONF_FWDL: {

        int rc = 0;
		string cmdline_fwdl, cmdline_opt;

		trace(TRACE_LEVEL_INFO, "NetconfFirmwareDownloadMessageHandler "
			"entering..");

		if (m_pNetconfMessage->m_option & NETCONFFWDL_LOCAL_NOACTIVATE) {
			/*
			 * Use option "a" for performing noactive fwdl. Netconf will not
			 * have reboot request but "b" is required for pizzabox to make hasm to use
			 * prot_single_cp_reboot[]. Only then firmware activate will
			 * reboot the pizzabox.
			 */
			cmdline_opt += "-a ";
			if (ham_is_dual() == 0)
				cmdline_opt += "-sb ";
		} else {
			if (ham_is_dual() == 0) {
				cmdline_opt += "-s ";
				if (m_pNetconfMessage->m_reboot)
					cmdline_opt += "-b ";
			}
		}

		if (m_pNetconfMessage->m_protocol == "usb") {
			cmdline_opt += "-U ";
			cmdline_opt += m_pNetconfMessage->m_file;
		} else {
			if (m_pNetconfMessage->m_protocol == "ftp") {
				cmdline_opt += "-p ftp ";
			} else if (m_pNetconfMessage->m_protocol == "sftp") {
				cmdline_opt += "-p sftp ";
			} else {
				cmdline_opt += "-p scp ";
			}

			cmdline_opt += m_pNetconfMessage->m_host;
			cmdline_opt += ",";
			cmdline_opt += m_pNetconfMessage->m_user;
			cmdline_opt += ",";
			cmdline_opt += m_pNetconfMessage->m_file;
			cmdline_opt += ",";
			cmdline_opt += m_pNetconfMessage->m_password;
		}

		/* Perform sanity check alone in Pizzabox or Dual MM */
		cmdline_fwdl = "firmwaredownload -SV ";
		cmdline_fwdl += cmdline_opt;

		trace(TRACE_LEVEL_DEBUG, "NetconfFirmwareDownloadMessageHandler: "
			"Sanity check : cmdline = " + cmdline_fwdl);
		rc = system(cmdline_fwdl.c_str());

		if ((WEXITSTATUS(rc) != 0) && (WEXITSTATUS(rc) != 1)) {
			char errmsg[MAX_SANITY_ERR_SIZE];
			if (0 == sus_sanity_status_get(errmsg, MAX_SANITY_ERR_SIZE))
				m_pNetconfMessage->m_errmsg = errmsg;
			else
				m_pNetconfMessage->m_errmsg = "Failed for unknown reason";

			trace(TRACE_LEVEL_INFO, "NetconfFirmwareDownloadMessageHandler : "
				"Sanity check return " + WEXITSTATUS(rc));
			m_pNetconfMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
		} else {
			/* Perform the Pizzabox or Dual MM firmwaredownload */
			cmdline_fwdl = "firmwaredownload -V ";
			cmdline_fwdl += cmdline_opt;
			cmdline_fwdl += " &"; // run as a background process
			trace(TRACE_LEVEL_DEBUG, "NetconfFirmwareDownloadMessageHandler: "
				"Fwdl cmdline = " + cmdline_fwdl);
			rc = system(cmdline_fwdl.c_str());
			if (WEXITSTATUS(rc) != 0) {
				trace(TRACE_LEVEL_WARN, "NetconfFirmwareDownloadMessage"
					"Handler: Fwdl return " + WEXITSTATUS(rc));
                m_pNetconfMessage->m_errmsg = "Firmwaredownload command couldn't be "
											"initated";
                m_pNetconfMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
            } else {
                m_pNetconfMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            }
        }
		cmdline_fwdl = "";
		cmdline_opt = "";

        reply (m_pNetconfMessage);
        trace (TRACE_LEVEL_INFO, "NetconfFirmwareDownloadMessageHandler exit");

	break;
	}

	default:
		trace (TRACE_LEVEL_ERROR,"Invalid message type received.");
	break;

	}

	delete (this);
	return (WAVE_THREAD_SUCCESS);
}

}
