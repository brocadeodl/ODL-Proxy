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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMTYPESH_H
#define PIMTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        PIMBSRCANDIDATECFG = 0 ,
        PIMRPCANDIDATECFG,
        PIMSTATICRPCFG,
        PIMRTRINST,
        PIMMULTRTRCFG,
		PIMRPCANDIDATEPREFIXCFG
    }Pim;

    typedef enum {
        PIM_SPT_VAL_USR_DEF = -1,
        PIM_SPT_VAL_INFINITY = 0
    } PimSptVal;
    
    typedef enum {
        PIM_INTF_TYPE_UNKWN = -1,
        PIM_INTF_TYPE_TE,
        PIM_INTF_TYPE_PO,
        PIM_INTF_TYPE_VE
    } PimIntfType;

	typedef enum {
		PIM_PREFIX_ACTION_UNKWN = -1,
        PIM_PREFIX_ACTION_ADD = 1,
        PIM_PREFIX_ACTION_DELETE = 2
    } PimPrefixActionType;
}
#endif                                            //PIMTYPESH_H
