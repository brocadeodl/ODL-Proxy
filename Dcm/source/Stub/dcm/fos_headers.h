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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Scott Busch                                                   *
 ***************************************************************************/


#ifndef __FOS_HEADERS_H__
#define __FOS_HEADERS_H__

/*
 * Needed by Stub version of fabric.h but not in the FOS version.
 */
#include "DcmCore/DcmToolKit.h"

extern "C" {

#include "iosp/asp.h"

#include "sys/fablog/fabmod.h"
#include "sys/fabos/fabobj.h"
#include "sys/fabos/if.h"
#include "sys/fabos/fabsys.h"
#include "sys/fabos/fabctrl.h"
#include "sys/fc/fcph.h"
#include "sys/fc/fspf.h"
#include "sysmod/syscomm.h"

#include "fabric/getid.h"
#include "fabric/fabric.h"
#include "switch/scn.h"
#include "switch/switch.h"
#include "switch/port.h"

#include "raslog/raslog.h"
#include "raslog/raslogm.h"
#include "raslog/raslog_dcm.h"
#include "raslog/raslog_sulb.h"

#include "auth/authipc.h"
#include "em/em_if.h"
#include "fabos/version.h"
#include "fc/fspf.h"
#include "fc/wwn.h"
#include "ns/ns_common.h"
#include "ns/public.h"
#include "ess/ess.h"
#include "fabos/license.h"
#include "fspf/public.h"
#include "security/public.h"
#include "thresh/fwdPub.h"
#include "thresh/fwd.h"
#include "zone/public.h"
#include "zone/zone.h"
#include "zone/zone_err.h"
#include "zone/zone_dstru.h"
#include "zone/ad_ns_private.h"
#include "hasm/hasm.h"

#include "utils/swInst.h"
#include "utils/cpuinfo.h"
#include "utils/slot.h"
#include "utils/mqueue.h"
#include "utils/libdef.h" /* do not include header file after this */
}

#endif

