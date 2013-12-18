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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef LSYSGLOBALTYPESH_H
#define LSYSGLOBALTYPESH_H
#define L2SYS_PLUGIN_GLOBAL_VERSION	0

namespace DcmNs
{

    typedef enum
    {
        L2SYSGLOBALUPDATEL2SYSAGINGTIMEOUT = 0 ,
        L2SYSGLOBALUPDATEL2SYSGLOBALSTATICMAC,
        L2SYSGLOBALUPDATEL2SYSGLOBALCLEAR
    }L2sysGlobal;

	#define L2SYS_DCM_MAC_AGING_DEFAULT			300
}
#endif                                            //LSYSGLOBALTYPESH_H
