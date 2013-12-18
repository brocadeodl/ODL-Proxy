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

/*
 * Debug.h
 *
 *  Created on: Apr 1, 2012
 *      Author: rmadapur
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define VIC_TRACE_INFO TRACE_LEVEL_INFO
#define VIC_TRACE_DEBUG TRACE_LEVEL_DEBUG
#define VIC_TRACE_ERROR TRACE_LEVEL_ERROR
#define VIC_CUR_LEVEL VIC_TRACE_ERROR
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include <stdarg.h>
#include "Utils/DceClusterUtils.h"

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#define VCENTER_RASLOG_PARAMS __FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS



#define VIC_PLUG_LOG(level,fmt, ...) tracePrintf(level,true,false,"%s:%s:%d:"fmt,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__)


using namespace std;
namespace DcmNs {

    class Debug {
        public:
            Debug();


            static  void VIC_PLUG_TRACE(TraceLevel level, const char *fmt,
                    ...);
            virtual ~Debug();
    };

} /* namespace DCM */
#endif /* DEBUG_H_ */

