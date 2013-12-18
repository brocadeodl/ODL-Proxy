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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMGLOBALTYPESH_H
#define APPMGLOBALTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        APPMGLOBALAPPMPROFILE = 0 ,
        APPMGLOBALAPPMVLAN,
        APPMGLOBALAPPMQOS,
        APPMGLOBALAPPMFCOE,
        APPMGLOBALAPPMSECURITY,
		APPMGLOBALAPPMPROFILEBULK  ,
		VCENTERGETALLPORTPROFILES,
		VCENTERGETPORTPROFILE,
        APPMGLOBALPORTPROFILEDOMAIN
    }APPMGlobal;

	
	typedef enum
	{
		APPM_FAILURE = -1,
		APPM_SUCCESS,
		APPM_NOOPERATION
	}AppmReturnCode;
}
#endif                                            //APPMGLOBALTYPESH_H
