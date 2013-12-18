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
 * Copyright (c) 1996-2011 Brocade Communications Systems, Inc.
 * All rights  reserved.
 *
 * This is the global include file of FabOS Version library
 * that provides get/set functionality for fabOS version information.
 *
 */

#ifndef __CPUINFO_H__
#define __CPUINFO_H__

#ifdef  __cplusplus
extern  "C" {
#endif

#define TMP_FW_MAX_APP_NAME_LEN        9
#define TMP_FW_MAX_APPS                12
#define TMP_FW_MAX_VERSION_LEN         64
#define TMP_FW_MAX_STR_LEN             128
#define TMP_FW_MAX_FULL_VERSION_LEN    128

#define TMP_FW_NAME            "Network Operating System Software"
#define TMP_FW_SHORT_NAME      "NOS"
#define TMP_FW_UNKNOWN_NAME    "Unknown"
#define TMP_FW_COPY_RIGHT_INFO "Copyright (c) 1995-2011 Brocade Communications Systems, Inc."

typedef struct cpu_info {
    char    vendor[64];
    char    chipset[64];
    char    proc_memory[64];
} cpu_info_t;

typedef struct _fw_app_show_info_s
{
    char app[TMP_FW_MAX_APPS];
    char primary[TMP_FW_MAX_FULL_VERSION_LEN];
    char secondary[TMP_FW_MAX_FULL_VERSION_LEN];
} fw_app_show_info_t;

typedef struct _fw_show_info_s
{
    char name[TMP_FW_MAX_STR_LEN];
    char version[TMP_FW_MAX_VERSION_LEN];
    char copy_right_info[TMP_FW_MAX_STR_LEN];
    char time_info[TMP_FW_MAX_STR_LEN];
    char full_version[TMP_FW_MAX_FULL_VERSION_LEN];
    cpu_info_t cpu_info;
    unsigned int app_num;
    fw_app_show_info_t apps[TMP_FW_MAX_APPS];
} fw_version_info_t;

#if 0
static fw_version_info_t tmp_fw_info =
{ TMP_FW_NAME, "2.1.0", TMP_FW_COPY_RIGHT_INFO, "20:55:22 Feb 24, 2011",
  "v2.1.0nos_simulator_110219_0500",
  {"Freescale Semiconductor", "8548E", "2000 MB"},
  1, { {"NOS", "v2.1.0nos_simulator_110219_0500",
       "v2.1.0nos_simulator_110219_0500"} } };
#endif

inline int get_fw_version_info(fw_version_info_t  *p_info)
{
    strncpy(p_info->name, TMP_FW_NAME, sizeof (p_info->name));
    strncpy(p_info->version, "2.1.0", sizeof (p_info->version));
    strncpy(p_info->copy_right_info, TMP_FW_COPY_RIGHT_INFO,
            sizeof (p_info->version));
    strncpy(p_info->time_info, "20:55:22 Feb 24, 2011",
            sizeof (p_info->time_info));
    strncpy(p_info->full_version, "v2.1.0nos_simulator_110219_0500",
            sizeof (p_info->full_version));
    strncpy(p_info->cpu_info.vendor, "Freescale Semiconductor",
            sizeof (p_info->cpu_info.vendor));
    strncpy(p_info->cpu_info.chipset, "8548E",
            sizeof (p_info->cpu_info.chipset));
    strncpy(p_info->cpu_info.proc_memory, "2000 MB",
            sizeof (p_info->cpu_info.proc_memory));
    p_info->app_num = 1;
    strncpy(p_info->apps[0].app, "NOS", sizeof (p_info->apps[0].app));
    strncpy(p_info->apps[0].primary, "v2.1.0nos_simulator_110219_0500",
            sizeof (p_info->apps[0].primary));
    strncpy(p_info->apps[0].secondary, "Unknown",
           sizeof (p_info->apps[0].secondary));
    return (0);
}
inline int get_cpuinfo(cpu_info_t  *cin)
{
	return (0);
}

#ifdef  __cplusplus
}
#endif

#endif /* __CPUINFO_H__ */
