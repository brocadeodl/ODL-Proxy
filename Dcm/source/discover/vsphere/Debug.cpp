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
 * Debug.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: rmadapur
 */

#include "Debug.h"
#include <stdarg.h>
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs {

    Debug::Debug() {
        // TODO Auto-generated constructor stub

    }

    Debug::~Debug() {
        // TODO Auto-generated destructor stub
    }
    void Debug::VIC_PLUG_TRACE(TraceLevel level, const char *fmt, ...) {
       // if (level >= VIC_CUR_LEVEL) {
            char *ptr;
            va_list arg;
            va_start(arg, fmt);
            vasprintf(&ptr, fmt, arg);
            tracePrintf(level,ptr);
            free(ptr);
            va_end(arg);
       // }
    }

} /* namespace DCM */
