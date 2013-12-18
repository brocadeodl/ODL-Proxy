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

#include "Firmware/Local/FirmwareClusterLocalThread.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "Firmware/Local/FirmwareClusterTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/TraceUtils.h"
#include "hasm/sus.h"
#include "hasm/hasm.h"
#include "utils/node_id.h"
#include <raslog/raslog.h>
#include <raslog/raslog_sulb.h>

#define FWDL_CLUSTER_FILE "/var/log/firmwareresult.data"
#define MAX_SANITY_ERR_SIZE 4096

namespace DcmNs
{

FirmwareClusterLocalThread::FirmwareClusterLocalThread(WaveObjectManager *pWaveObjectManager, FirmwareClusterLocalMessage *pFirmwareClusterLocalMessage, NetconfStatusMessage *pNetconfStatusMessage, UI32 msg_type)
	: WaveWorker(pWaveObjectManager, false),
	PrismPosixThread(),
	m_pFirmwareClusterLocalMessage(pFirmwareClusterLocalMessage),
	m_pNetconfStatusMessage(pNetconfStatusMessage),
	m_msg_type(msg_type)
{
}

FirmwareClusterLocalThread::~FirmwareClusterLocalThread()
{
}

WaveThreadStatus FirmwareClusterLocalThread::start()
{

    switch (m_msg_type) {

    case FIRMWARECLUSTERLOCAL: {

		UI32		rbridgeId = 0;
		UI32		state, next_state, local_node_ret = 0, flag;
		int			rc = 0, reply_back = 0;
		ResourceId 	status = WAVE_MESSAGE_SUCCESS;
		string		cmdline = "", user = "", pass = "";
		string		host = "", path = "", proto = "";
		//FILE		*cmd_file, *output_file;
		//char		cmd_output[512];
		char		msg[MAX_SANITY_ERR_SIZE]= "none";

		trace (TRACE_LEVEL_DEBUG, "FCLMessage::FCLMessageHandler: Entering");
		state = m_pFirmwareClusterLocalMessage->getState();
		rbridgeId = m_pFirmwareClusterLocalMessage->getRbridgeId();
		flag = m_pFirmwareClusterLocalMessage->getFlag();
		WaveNs::trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
			"FCLMessageHandler: State = ") + state);

		switch (state) {
			case SANITY_CHECK:
				trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Start Sanity check for rbridge-id = ")
					+ rbridgeId);

				cmdline = "/fabos/sbin/firmwaredownload -VS ";

				user = m_pFirmwareClusterLocalMessage->getUser();
				pass = m_pFirmwareClusterLocalMessage->getPassword();
				host = m_pFirmwareClusterLocalMessage->getHost();
				path = m_pFirmwareClusterLocalMessage->getPath();
				proto = m_pFirmwareClusterLocalMessage->getProto();
				flag = m_pFirmwareClusterLocalMessage->getFlag();

				if (ham_is_dual() == 0)
					cmdline += "-s ";
				if (flag & FW_CLUSTER_USB)
					cmdline += "-U ";

				cmdline = cmdline + "-p " + proto + " " + host + "," +
							user + "," + path + "," + pass;

				WaveNs::trace(TRACE_LEVEL_INFO, string("FCLocalMessage::"
					"FCLMessageHandler: cmd = ") + cmdline);
				next_state = SANITY_CHECK_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
			break;

			case INSTALL:
				trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Install firmware for rbridge-id = ")
					+ rbridgeId);

				cmdline = "/fabos/sbin/firmwaredownload -aV ";

				user = m_pFirmwareClusterLocalMessage->getUser();
				pass = m_pFirmwareClusterLocalMessage->getPassword();
				host = m_pFirmwareClusterLocalMessage->getHost();
				path = m_pFirmwareClusterLocalMessage->getPath();
				proto = m_pFirmwareClusterLocalMessage->getProto();
				flag = m_pFirmwareClusterLocalMessage->getFlag();

				if (ham_is_dual() == 0)
					cmdline += "-sb ";

				if (flag & FW_CLUSTER_USB)
					cmdline += "-U ";

				cmdline = cmdline + "-p " + proto + " " + host + "," +
							user + "," + path + "," + pass;

				WaveNs::trace(TRACE_LEVEL_INFO, string("FCLocalMessage::"
					"FCLMessageHandler: cmd = ") + cmdline);
				next_state = INSTALL_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
			break;

			case REBOOT:
				trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Reboot switch for rbridge-id = ")
					+ rbridgeId);
				/*reboot_delay = m_pFirmwareClusterLocalMessage->getReboot_delay();
				if (reboot_delay > 0) {
					WaveNs::trace (TRACE_LEVEL_INFO, string("FCLocalMessage::"
						"FCLMessageHandler: Sleeping for ") + reboot_delay);
					sleep(reboot_delay*60);
				}*/
				cmdline = "/fabos/cliexec/reboot -s -r FirmwareDownload";
				//cmdline = "";
				next_state = REBOOT_INCOMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
			break;

			case RECOVER_COMMIT:
				trace (TRACE_LEVEL_INFO, "FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Commit to recover sec. partition.");
				cmdline = "/fabos/sbin/firmwarecommit";
				status = WAVE_MESSAGE_ERROR;
				m_pFirmwareClusterLocalMessage->setCompletionStatus (status);
				reply (m_pFirmwareClusterLocalMessage);
				reply_back = 1;
			break;

			case ACTIVATE_SANITY:
				local_node_ret = sus_sanity_activate();
				trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Firmware Activate Sanity. sts ") + local_node_ret);
				next_state = ACTIVATE_SANITY_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
				m_pFirmwareClusterLocalMessage->setLocalNodeRet(local_node_ret);
				m_pFirmwareClusterLocalMessage->setLocalFwdlStatus(sus_fwdl_state());
				trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Firmware Activate Status ") +
					m_pFirmwareClusterLocalMessage->getLocalFwdlStatus());
			break; 

			case ACTIVATE:
				trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Firmware Activate rbridgeId ") + rbridgeId);
				next_state = ACTIVATE_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
				cmdline = "/fabos/sbin/firmwareactivate";
			break;

			case RECOVER_SANITY:
				rc = sus_sanity_activate();
				if ((sus_state_get(NID_SELF()) == 1) && ((rc == 1) || (rc == 2)))
					local_node_ret = 1;
				trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Firmware Recover Sanity. sts ") + local_node_ret);
				next_state = RECOVER_SANITY_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
				rc = 0;
				m_pFirmwareClusterLocalMessage->setLocalNodeRet(local_node_ret);
			break;

			case RECOVER:
				trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Firmware Recover rbridgeId ") + rbridgeId);
				next_state = RECOVER_COMPLETE;
				status = WAVE_MESSAGE_SUCCESS;
				cmdline = "/fabos/sbin/firmwareabort -a";
			break;

			default:
				trace (TRACE_LEVEL_INFO, "FirmwareClusterLocalMessage::"
					"FCLMessageHandler: Unsupported state.");
				status = WAVE_MESSAGE_ERROR;
			break;
		}

	/*if ((cmd_file = popen((char *)cmdline.data(), "r")) != NULL) {

		trace (TRACE_LEVEL_INFO, "FrmwareClusterLocalMessage::FCLMessageHandler"
			": Command started.");
		output_file = fopen(FWDL_CLUSTER_FILE, "w");
		while (fgets(cmd_output, 512, cmd_file)) {
			fprintf(output_file, "%s\n", cmd_output);
		}
		fclose(output_file);

		if ((rc = pclose(cmd_file)) < 0) {

			trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
				"FCLMessageHandler: pclose() failed. errno = ") + errno);
		}

		m_pFirmwareClusterLocalMessage->setState(next_state);

		WaveNs::trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
			"FCLMessageHandler: Next state = ") + next_state);
		trace (TRACE_LEVEL_INFO, "FirmwareClusterLocalMessage:FCLMessageHandler"
			": Command completed.");*/

		if (cmdline.length() != 0)
			rc = system((char *)cmdline.data());
		if ((rc = WEXITSTATUS(rc)) < 0) {

			trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
				"FCLMessageHandler: system() cmd failed. rc = ") + rc);
			trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
				"FCLMessageHandler: cmd failure errno = ") + errno);
			status = WAVE_MESSAGE_SUCCESS;

			m_pFirmwareClusterLocalMessage->setState(state);
			m_pFirmwareClusterLocalMessage->setRbridgeId(rbridgeId);
	
		} else {
			// For INSTALL, we need to find out the real status
			// in case of error, reset the state to INSTALL so
			// global plug-in can handle it
			if (state == INSTALL) {
				if (sus_fwdl_state() == 3) {
					WaveNs::trace(TRACE_LEVEL_INFO, string("FirmwareCluster"
						"LocalMessage:: install failed"));
					next_state = state;
				} else if (!(flag & FW_CLUSTER_ACTIVATE)) {
					raslog_ext(__FILE__, __FUNCTION__, __LINE__,
					RASLOG_OID_INVALID, RASLOG_FRCLOG, SULB_1114, rbridgeId);
				}
			}
			m_pFirmwareClusterLocalMessage->setState(next_state);
			m_pFirmwareClusterLocalMessage->setRbridgeId(rbridgeId);
			WaveNs::trace (TRACE_LEVEL_INFO, string("FirmwareClusterLocal"
				"Message::FCLMessageHandler: Next state = ") + next_state);
			trace (TRACE_LEVEL_INFO, "FirmwareClusterLocalMessage:FCL"
				"MessageHandler: Command completed.");
		}

		m_pFirmwareClusterLocalMessage->setCompletionStatus(status);
		if (state == SANITY_CHECK) {
			m_pFirmwareClusterLocalMessage->setLocalNodeRet(rc);
			sus_sanity_status_get(msg, MAX_SANITY_ERR_SIZE);
			if (strcmp(msg, "\n") == 0)
				strcpy(msg, "none");
			m_pFirmwareClusterLocalMessage->setLocalNodeMesg(string(msg));
			m_pFirmwareClusterLocalMessage->setLocalFwdlStatus(sus_fwdl_state());

			fwversion_t current_version;
			memset(&current_version, 0, sizeof(fwversion_t));
			version_get(NULL, (char *)"base", &current_version);
			m_pFirmwareClusterLocalMessage->setFirmwareVersion(current_version.fw_verstr);

			trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
				"FCLMessageHandler SANITY_CHECK : rc ") + rc +
				string(" mesg ") + msg);
		}

		trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalMessage::"
			"FCLMessageHandler: rbridgeID = ") + rbridgeId);
		trace(TRACE_LEVEL_INFO, "FirmwareClusterLocalMessage::FCLMessage"
			"Handler: Exiting...");

		if (reply_back == 0)
			reply (m_pFirmwareClusterLocalMessage);
	break;
	}

    case NETCONFSTATUS: {

		int 				i, count = 0;
		char 				msg[512], msg_time[64];
		struct tm 			*ptm;
		sus_status_t		sus_status;
		vector<UI32>		index, messageId;
		vector<string>		date_time, message, bladeName;
		ResourceId			status = WAVE_MESSAGE_ERROR;

		trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalThread::"
			"NetconfStatusMessageHandler: In NETCONFSTATUS."));

		m_pNetconfStatusMessage->setFwdlState(sus_fwdl_state());

		memset(&sus_status, 0, sizeof (sus_status_t));
		if (sus_status_get(0, &sus_status) == 0) {
			count = sus_status.ss_count;
			m_pNetconfStatusMessage->setNumOfEntries(count);
			for (i = 1; i <= count; i++) {

				memset(&sus_status, 0, sizeof (sus_status_t));
				msg[0] = '\0';

				if (sus_status_get(i, &sus_status) != 0)
					continue;

				if (sus_status_map(sus_status.ss_scode,
						sus_status.ss_fcode, msg) != 0)
					continue;

				index.push_back(i);
				m_pNetconfStatusMessage->setIndex(index);
				messageId.push_back(sus_status.ss_fcode);
				m_pNetconfStatusMessage->setMessageId(messageId);
				if ((ptm = localtime(&sus_status.ss_time))) {
					strftime(msg_time, 64, "%Y-%m-%d/%H:%M:%S", ptm);
					date_time.push_back(msg_time);
					m_pNetconfStatusMessage->setDateAndTime(date_time);
				}
				message.push_back(msg);
				m_pNetconfStatusMessage->setMessage(message);
				bladeName.push_back(get_node_str(sus_status.ss_node));
				m_pNetconfStatusMessage->setBladeName(bladeName);
			}
			status = WAVE_MESSAGE_SUCCESS;
		}
		trace(TRACE_LEVEL_INFO, string("FirmwareClusterLocalThread::"
			"NetconfStatusMessageHandler: Status complete for ") +
			m_pNetconfStatusMessage->getRbridgeId());

		m_pNetconfStatusMessage->setCompletionStatus(status);
		reply (m_pNetconfStatusMessage);
	break;
	}

	default:
		trace (TRACE_LEVEL_ERROR,"Invalid message type received.");
	break;

	} /*switch msg_type end*/

	delete (this);
	return (WAVE_THREAD_SUCCESS);
}


}
