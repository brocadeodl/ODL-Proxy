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


/*
 ******************************************************************************
 * Copyright (c) 2010 Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * Module name: RIP
 *
 * Description:
 *  This header file contains defines and structure definition of IPC messages
 *  used between RIP and other modules (IFM, RTM etc).
 *****************************************************************************
 */
#ifndef _RIP_IPC_H_
#define _RIP_IPC_H_

#include "dce_ipc_common.h"

#define RIP_SERVER_NAME	"IPCRIP"
#define RIP_INSTANCE		MYSWITCH
#define RIP_IPC_INSTANCE DCE_IPC_INSTANCE
#define RIP_NODE			MYNODE
#define RIP_MAX_MSG_SIZE	6
#define RIP_IPC_TIMEOUT	5

/* Initilization using basic timeout value */
#define RIP_SET_IPC_TIMEOUT(timeout)		\
do {										\
	(timeout).tv_sec = RIP_IPC_TIMEOUT;	\
	(timeout).tv_usec = 0;					\
} while (0)

// OS_CDECLS_BEGIN


/* RIP IPC call backs */
enum {
	CB_RIP_MIN = 0,

	CB_RIP_PKT_RCVD,
	CB_RIP_POLICY,

	CB_RIP_MAX
};

// OS_CDECLS_END

#endif /* _RIP_IPC_H_ */
