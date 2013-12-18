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
 *   Author : lhu                                                          *
 **************************************************************************/

#include <iostream>
#include <string>
#include <fstream>

#include "Firmware/Local/FirmwareShowLocalObjectManager.h"
#include "Firmware/Local/FirmwareShowActionMessage.h"
#include "Firmware/Local/FirmwareShowTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmShell/DcmShell.h"

#include <fabos/version.h>
#include <hasm/smi.h>
#include <hasm/hasm.h>
#include <hasm/sus.h>
#include <hasm/node.h>
#include <ha/ham_api.h>
#include <utils/node_id.h>
#include <config/config_sw.h>
#include <config/resource.h>

namespace DcmNs
{

    FirmwareShowLocalObjectManager::FirmwareShowLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FIRMWARESHOWACTION, reinterpret_cast<PrismMessageHandler> (&FirmwareShowLocalObjectManager::FirmwareShowActionMessageHandler));
    }

    FirmwareShowLocalObjectManager::~FirmwareShowLocalObjectManager ()
    {
    }

    FirmwareShowLocalObjectManager  *FirmwareShowLocalObjectManager::getInstance()
    {
        static FirmwareShowLocalObjectManager *pFirmwareShowLocalObjectManager = new FirmwareShowLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFirmwareShowLocalObjectManager, __FILE__, __LINE__);

        return (pFirmwareShowLocalObjectManager);
    }

    string  FirmwareShowLocalObjectManager::getClassName()
    {
        return ("Firmware Show Local Manager");
    }

    PrismServiceId  FirmwareShowLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FirmwareShowLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FIRMWARESHOWACTION :
                pPrismMessage = new FirmwareShowActionMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FirmwareShowLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

#define SHOW_FILE "/var/log/firmware_show.data"

    static int nc_get_firmware_version(ncFwInfo_t *pFwInfo)
    {
        int rc = 0;
        int nids[MAX_NODES];
        int  i, num = 0;

        memset(pFwInfo, 0, sizeof (ncFwInfo_t));

        pFwInfo->switch_id = DcmToolKit::getLocalMappedId();
        strcpy(pFwInfo->os_name, TMP_FW_NAME);
        strcpy(pFwInfo->os_copyright, TMP_FW_COPY_RIGHT_INFO);

        /* cpu info - TODO */
		cpu_info_t cin;
		if (get_cpuinfo(&cin)) {
			return (-1);
		}
		strcpy(pFwInfo->cpu_info.vendor, cin.vendor);
		strcpy(pFwInfo->cpu_info.chipset, cin.chipset);
		strcpy(pFwInfo->cpu_info.proc_memory, cin.proc_memory);							

        if (getThisCPSlot() == 0) {
            /* Pizza box */
            fwversion_t pver, sver;

            pFwInfo->isChassis = 0;
            memset(&pver, 0, sizeof(fwversion_t));
            memset(&sver, 0, sizeof(fwversion_t));

            if (version_get((const char*) PRI_VERSION_FILE, (char*)"base", &pver)) {
                return (-1);
            }

            if (version_get((const char*)MNT_VERSION_FILE, (char*)"base", &sver)) {
                return (-1);
            }

            /* Use primary version for OS Version */
            snprintf(pFwInfo->os_version, TMP_FW_MAX_VERSION_LEN, "%d.%d.%d",
                            pver.fw_major, pver.fw_minor, pver.fw_patch);
            strcpy(pFwInfo->os_build_time, pver.fw_bdate);
            strcpy(pFwInfo->os_verstr, pver.fw_verstr);

            pFwInfo->node_cnt = 1;
            pFwInfo->fw_nodeinfo[0].slot_no = 0;
            pFwInfo->fw_nodeinfo[0].node_inst_no = 1;
            pFwInfo->fw_nodeinfo[0].node_type = TYPE_CP;
            strcpy(pFwInfo->fw_nodeinfo[0].fw_app_info.app, "NOS");
            strcpy(pFwInfo->fw_nodeinfo[0].fw_app_info.primary, pver.fw_verstr);
            strcpy(pFwInfo->fw_nodeinfo[0].fw_app_info.secondary, sver.fw_verstr);
        } else {
            /* chassis */
            pFwInfo->isChassis = 1;
            memset(&nids, 0, sizeof (nids));
            num = has_get_nodeset(nids, HAS_NSET_ALL);
            pFwInfo->node_cnt = num;
            for (i = 0; i < num; i++) {
                int nid, slot, node_type;
                sus_vers_t vers;
                sus_node_t nin;
                version_array_t *pvers, *svers;
                fwversion_t *pver, *sver;

                nid = nids[i];

                /* if you can't get version information - skip */
                if (sus_version_get(nid, &vers) != 0 || sus_node_get(nid, &nin) != 0)
                    continue;

                slot = get_node_slot(nid);
                node_type = get_node_type(nid);

                pvers = SUS_VERS_PRI(&vers);
                svers = SUS_VERS_SEC(&vers);

                pver = VERSION_FIND(pvers, (char*)"base");
                sver = VERSION_FIND(svers, (char*)"base");

                /* fill the structure */
                pFwInfo->fw_nodeinfo[i].slot_no = slot;
                pFwInfo->fw_nodeinfo[i].node_inst_no = get_node_inst(nid);
                pFwInfo->fw_nodeinfo[i].node_type = get_node_type(nid);
                strcpy(pFwInfo->fw_nodeinfo[i].fw_app_info.app, "NOS");
                strcpy(pFwInfo->fw_nodeinfo[i].fw_app_info.primary, pver->fw_verstr);
                strcpy(pFwInfo->fw_nodeinfo[i].fw_app_info.secondary, sver->fw_verstr);

                /* If CP and Active - fill certain names */
                if (nin.sn_hamst & HAM_STATE_ACTIVE && node_type == TYPE_CP) {
                    snprintf(pFwInfo->os_version, TMP_FW_MAX_VERSION_LEN, "%d.%d.%d",
                            pver->fw_major, pver->fw_minor, pver->fw_patch);
                    strcpy(pFwInfo->os_build_time, pver->fw_bdate);
                    strcpy(pFwInfo->os_verstr, pver->fw_verstr);
                    pFwInfo->fw_nodeinfo[i].is_active_cp = true;
                } else {
                    pFwInfo->fw_nodeinfo[i].is_active_cp = false;
                }
            }
        }

        return (rc);
    }

    void  FirmwareShowLocalObjectManager::FirmwareShowActionMessageHandler( FirmwareShowActionMessage *pFirmwareShowActionMessage)
    {
        string fName("FirmwareShowLocalObjectManager::FirmwareShowActionMessageHandler");
        string result = "";
        char cmdline[512];
        ifstream in;
        int rc = 0;

		trace (TRACE_LEVEL_INFO, "FirmwareShowActionMessageHandler enter..");

        string fosCommand = pFirmwareShowActionMessage->getCommand();

		trace (TRACE_LEVEL_INFO, string("FirmwareShowActionMessageHandler get command: ") + fosCommand.data());

		// get the firmware info by using API
		if ((fosCommand.compare("firmwareshow") == 0) && pFirmwareShowActionMessage->getApiFlag()) {
			ncFwInfo_t fwInfo;
            ResourceId  status = WAVE_MESSAGE_SUCCESS;

			trace (TRACE_LEVEL_INFO, fName + string(": Using the API for ") + fosCommand);

            /* get firmware information */
            if (nc_get_firmware_version(&fwInfo) < 0) {
			    trace (TRACE_LEVEL_ERROR, fName + string(": Failed to retrieve firmware version information"));
                status = WAVE_MESSAGE_ERROR;
            } else {
                /* transfer the buffer */
			    pFirmwareShowActionMessage->transferOutputBuffer(&fwInfo, (UI32)sizeof (ncFwInfo_t), (UI32) FW_SHOW_MSG_ID);
            }

			pFirmwareShowActionMessage->setCompletionStatus(status);
			reply (pFirmwareShowActionMessage);
			return;

		} else if (fosCommand.find("autosync") != string::npos) {
			int fwsync;
            ResourceId status = WAVE_MESSAGE_SUCCESS;

			if (fosCommand.find("have_option_status") != string::npos) {
				if (configGet("firmware.sync", ResourceInt, &fwsync) == 0) {
					trace (TRACE_LEVEL_INFO, fName + string(":firmware.sync=") + fwsync);
					if (fwsync == 1) {
						result = "Automatic firmware sync enabled.\n";
					} else {
						result = "Automatic firmware sync disabled.\n";
					}
				} else {
					result = "Failed to get automatic firmware sync.\n";
			    	trace (TRACE_LEVEL_ERROR, fName + string(": configGet() failed.\n"));
                	status = WAVE_MESSAGE_ERROR;
				}
			} else {
				result = "Failed to provide argument.\n";
			    trace (TRACE_LEVEL_ERROR, fName + string(": Failed to find arguments"));
                status = WAVE_MESSAGE_ERROR;
			}
			pFirmwareShowActionMessage->setResult(result);
			pFirmwareShowActionMessage->setCompletionStatus(status);
			reply (pFirmwareShowActionMessage);
			return;
		}

		snprintf(cmdline, 512, "%s > %s 2>/dev/null", (char *)fosCommand.data(), SHOW_FILE);

		cout << cmdline << endl;

		rc = system(cmdline);

		if (WEXITSTATUS(rc) != 0) {
			trace (TRACE_LEVEL_INFO, string ("FirmwareShowActionMessageHandler::Command execution failed: ") + cmdline);
			result = "Command execution failed.\n";
		} else {
		
			in.open(SHOW_FILE);
			if ( ! in ) {
				trace (TRACE_LEVEL_INFO, string ("FirmwareShowActionMessageHandler::fosCLI fail:") + cmdline);
				result = "Fail to execute ";
				result += fosCommand;
				result += '\n';
			} else {
				string str;
				getline(in, str);
				while ( in ) {
					result = result + str + '\n';
					trace (TRACE_LEVEL_DEBUG, string ("fosCLI=") + str);
					getline(in, str, in.widen ('\n'));
				}
				in.close();
			}
		}

		pFirmwareShowActionMessage->setResult(result);

		pFirmwareShowActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		
	    reply (pFirmwareShowActionMessage);
		trace (TRACE_LEVEL_INFO, "FirmwareShowActionMessageHandler replied");
    }

}
