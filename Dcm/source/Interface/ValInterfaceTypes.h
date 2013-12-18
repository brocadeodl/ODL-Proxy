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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VALINTERFACETYPES_H
#define VALINTERFACETYPES_H

namespace DcmNs
{

typedef enum
{
    VAL_INTERFACE_CONFIGURE_INTERFACE = 0,
    VAL_INTERFACE_GET_INTERFACE,
    VAL_INTERFACE_CONFIGURE_INTERFACE_CHILDREN,
    VAL_INTERFACE_CONFIGURE_ASSOC_VLAN,
    VAL_INTERFACE_INSTALL_DEMO_OBJECTS,
    VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE
} ValInterfaceTypes;

typedef enum
{
    TCP,
    UDP,
    SMTP,
    SNMP,
    IP,
    ETHERNET,
    PROTO_MAX
} Protocol;

typedef enum
{
    DCM_DEBUG,
    DCM_INFO,
    DCM_WARNING,
    DCM_ERROR,
    DCM_ERRORLEVEL_MAX
} ErrorLevel;

typedef enum
{
    WAVE,
    DCM,
    PLATFORM,
    PLUGIN
} VirgoElement;

}

#endif // VALINTERFACETYPES_H
