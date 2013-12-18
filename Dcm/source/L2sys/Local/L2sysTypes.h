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

#ifndef LSYSTYPESH_H
#define LSYSTYPESH_H

namespace DcmNs
{

/* These enums defined the L2sys Message buffer types that are passed between */
/* plug-ins.  The types defined here are registered with Dcmd through */
/* the call to function addOperation. */
    typedef enum
    {
        L2SYSUPDATEL2SYSGLOBALSTATICMACSP = 0 ,
        L2SYSUPDATEL2SYSAGINGTIMEOUTSP,
        L2SYSUPDATEL2SYSGLOBALCLEARSP,
        L2SYSSHOWMACPROFILE,
        L2SYSSHOWRBRIDGEMAC,
		L2SYSSHOWPORTSECURITY
    }L2sys;

}
#endif                                            //LSYSTYPESH_H
