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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef TERMINALTYPESH_H
#define TERMINALTYPESH_H

namespace DcmNs
{
	typedef enum
    {
        TERMINALSESSIONCFG = 1
    }Terminal_msg;

	typedef enum
    {
		SESSION_NONE = 0,
        SESSION_CONSOLE = 1,
		SESSION_VTY = 2,
		SESSION_ALL = 255 
    }Session_Id;

	typedef enum
    {
		TERMINALCFG_INVALID = 0,
        TERMINALCFG_CREATE,
		TERMINALCFG_UPDATE,
		TERMINALCFG_REMOVE,
		TERMINALCFG_READ
    }Terminal_cfg;

	typedef enum
	{
		VALUE_NOCHANGE = 0,
		VALUE_UPDATED,
		VALUE_DEFAULT,
		VALUE_READ
	}LeafValue_flag;

#define	TERMINAL_DEFAULT_EXECTIMEOUT	10	//10 min 
#define TERMINAL_SESSION_NOT_EXIST		100
#define TERMINAL_SESSION_CFG_FAIL		101

}
#endif                                            //TERMINALTYPESH_H
