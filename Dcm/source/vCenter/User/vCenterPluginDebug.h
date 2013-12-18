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
 *   Author : Mobin Mohan   												*
 ***************************************************************************/

#ifndef VCENTER_PLUGIN_DEBUG_H
#define VCENTER_PLUGIN_DEBUG_H

#define VCENTER_PLUG_ASSERT(exp) \
        prismAssert ((exp), __FILE__, __LINE__);

#define VCENTER_PLUG_ASSERT_PTR(exp) \
        prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define VCENTER_PLUG_DBG_FUNC_BEG() \
    {\
      char vcenter_cpw_debug_buffer[512]; \
      snprintf(vcenter_cpw_debug_buffer, 512,\
               "%s:%s:%d: Entered Function", __FILE__, __FUNCTION__, __LINE__);\
      trace(TRACE_LEVEL_DEVEL, string(vcenter_cpw_debug_buffer));\
    }

#define VCENTER_PLUG_DBG_FUNC_END() \
    {\
      char vcenter_cpw_debug_buffer[512]; \
      snprintf(vcenter_cpw_debug_buffer, 512,\
               "%s:%s:%d: Exited Function", __FILE__, __FUNCTION__, __LINE__);\
      trace(TRACE_LEVEL_DEVEL, string(vcenter_cpw_debug_buffer));\
    }

#define VCENTER_PLUG_DBG(msg) \
    { \
        char vcenter_cpw_debug_buffer[512]; \
        snprintf(vcenter_cpw_debug_buffer, 512,\
                 "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(vcenter_cpw_debug_buffer) + msg); \
    }

#define VCENTER_PLUG_DBG_ERR(msg) \
    { \
        char vcenter_cpw_debug_buffer[512]; \
        snprintf(vcenter_cpw_debug_buffer, 512,\
                 "ERROR: %s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(vcenter_cpw_debug_buffer) + msg); \
    }

#define VCENTER_PLUG_DBG_SUC(msg) \
    { \
        char vcenter_cpw_debug_buffer[512]; \
        snprintf(vcenter_cpw_debug_buffer, 512,\
                 "SUCCESS: %s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(vcenter_cpw_debug_buffer) + msg); \
    }
#endif

