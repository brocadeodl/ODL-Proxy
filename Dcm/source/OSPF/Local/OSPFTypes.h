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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFTYPESH_H
#define OSPFTYPESH_H
namespace DcmNs
{

    #define OSPF_PLUGIN_LOCAL_VERSION 0
    typedef enum
    {
		OSPFROUTERCONFIGLOCAL =0,
		OSPFAREALOCAL,
		OSPFPERMITDENYCONFIGLOCAL,
        OSPFVIRTUALLINKLOCAL,
		OSPFAREARANGELOCAL
    }OSPF;

    typedef enum
    {
        OSPF_FAILURE = -1,
        OSPF_SUCCESS
    }OSPFReturnCode;

}
#endif                                            //OSPFTYPESH_H
