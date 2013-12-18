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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef FIRMWAREOPTYPESH_H
#define FIRMWAREOPTYPESH_H

namespace DcmNs
{

	#define MAX_SANITY_ERR_SIZE 4096

    typedef enum
    {
        FIRMWAREOPACTION = 0,
        NETCONF_FWDL = 1,
		NODEAUTOUPGRADE = 2
    }FirmwareOp;

	typedef enum
	{
		NETCONFFWDL_LOCAL_NOACTIVATE = 1
	}NetconfFwdl_options;

}
#endif                                            //FIRMWAREOPTYPESH_H
