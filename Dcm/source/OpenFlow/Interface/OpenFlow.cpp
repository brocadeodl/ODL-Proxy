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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Interface/OpenFlow.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

namespace OpenFlowNs
{
const UI32 OpenFlow::m_openFlow10Version                 = 0x01;
const UI32 OpenFlow::m_openFlow11Version                 = 0x02;
const UI32 OpenFlow::m_openFlow12Version                 = 0x03;
const UI32 OpenFlow::m_openFlow13Version                 = 0x04;
const UI32 OpenFlow::m_openFlowMaximumTableNameLength    = 32;
const UI32 OpenFlow::m_openFlowMaximumPortNameLength     = 16;
const UI32 OpenFlow::m_openFlowTcpPort                   = 6633;
const UI32 OpenFlow::m_openFlowSslPort                   = 6633;
const UI32 OpenFlow::m_openFlowEthernetAddressLength     = 6;
const UI32 OpenFlow::m_openFlowDescriptionStringLEngth   = 256;
const UI32 OpenFlow::m_openFlowSerialNumberLength        = 32;
}
