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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Sanjeev Joshi 												*
 ***************************************************************************/

#ifndef LLDP_PLUGIN_DEBUG_H
#define LLDP_PLUGIN_DEBUG_H

#define LLDP_PLUG_ASSERT(exp) \
        prismAssert ((exp), __FILE__, __LINE__);

#define LLDP_PLUG_ASSERT_PTR(exp) \
        prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define LLDP_PLUG_DBG_TRACE() \
    {\
      char lldp_cpw_debug_buffer[512]; \
      snprintf(lldp_cpw_debug_buffer, 512,\
               "%s:%s:%d: Entered Function", __FILE__, __FUNCTION__, __LINE__);\
      trace(TRACE_LEVEL_INFO, __FUNCTION__);\
    }

#define LLDP_PLUG_DBG(msg) \
    { \
        char lldp_cpw_debug_buffer[512]; \
        snprintf(lldp_cpw_debug_buffer, 512,\
                 "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(lldp_cpw_debug_buffer) + msg); \
    }

#define LLDP_PLUG_DBG_ERR(msg) \
    { \
        char lldp_cpw_debug_buffer[512]; \
        snprintf(lldp_cpw_debug_buffer, 512,\
                 "ERROR: %s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(lldp_cpw_debug_buffer) + msg); \
    }

#define LLDP_PLUG_DBG_S(msg) \
    { \
        char lldp_cpw_debug_buffer[512]; \
        snprintf(lldp_cpw_debug_buffer, 512,\
                 "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        trace(TRACE_LEVEL_INFO, string(lldp_cpw_debug_buffer) + msg); \
    }
#endif

