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

#ifndef FIRMWARETYPESH_H
#define FIRMWARETYPESH_H

namespace DcmNs
{

#define FW_CLUSTER_SANITY			0x1
#define FW_CLUSTER_ACTIVATE			0x2
#define	FW_ACTIVATE_SANITY			0x4
#define	FW_ACTIVATE					0x8
#define FW_CLUSTER_USB				0x10
#define	FW_RECOVER_SANITY			0x20
#define FW_RECOVER					0x40

    typedef enum
    {
        FIRMWARECLUSTER = 0,
		FIRMWAREACTIVATE
    }Firmware;

	typedef enum
	{
		SANITY_CHECK = 0,
		SANITY_CHECK_COMPLETE,
		INSTALL,
		INSTALL_COMPLETE,
		REBOOT,
		REBOOT_INCOMPLETE,
		RECOVER_COMMIT,
		ACTIVATE_SANITY,
		ACTIVATE_SANITY_COMPLETE,
		ACTIVATE,
		ACTIVATE_REBOOTED,
		ACTIVATE_COMPLETE,
		RECOVER_SANITY,
		RECOVER_SANITY_COMPLETE,
		RECOVER,
		RECOVER_COMPLETE,
		CLI_STATUS_UNBLOCK,
        CLI_STATUS_BLOCK
	}FirmwareStates;

	typedef enum
	{
		INIT = 0,
		SANITY_REQUEST,
		SANITY_REPLY,
		INSTALL_REQUEST,
		INSTALL_REPLY,
		ACTIVATE_REQUEST,
		NODE_JOIN,
		NODE_JOIN_FAILURE
	}LogicalChassisFwdlStatus;
}

#endif		//FIRMWARETYPESH_H

#ifndef NODEAUTOUPGRADETYPESH_H
#define NODEAUTOUPGRADETYPESH_H


namespace DcmNs
{

	#define NODEAUTOUPGRADE_MSG_VER		"1.0.0"

	#define NA_OPT_ENABLE				0x1
	#define	NA_OPT_HOST					0x2
	#define	NA_OPT_PATH					0x4
	#define	NA_OPT_PROTOCOL				0x8
	#define	NA_OPT_USERNAME				0x10
	#define	NA_OPT_PASSWORD				0x20
	#define	NA_DIFF_VERSION				0x40

	#define USB		0
	#define FTP		1
	#define	SCP		2

	typedef enum
	{
		NODEAUTOUPGRADECONFIG = 0
	} NodeAutoUpgrade;

}
#endif		//NODEAUTOUPGRADETYPESH_H


#ifndef FIRMWARESTATUSTYPESH_H
#define FIRMWARESTATUSTYPESH_H

namespace DcmNs
{
	typedef enum
	{
		FIRMWARENETCONFSTATUS = 0
	} FirmwareStatus;

}
#endif      //FIRMWARESTATUSTYPESH_H
