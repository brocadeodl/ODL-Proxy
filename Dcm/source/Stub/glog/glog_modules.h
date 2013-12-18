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
 * Copyright (c) 2000-2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * glog_modules.h
 *
 * Generic Logger modules definition
 */
#ifndef __GLOG_MODULES_H__
#define __GLOG_MODULES_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definition of application modules.
 */
typedef enum {
	GLOG_MODID_DEFAULT,		/* 0:Default module id used by Logger */
	GLOG_MODID_GLOGTEST,		/* 1:Logger tester module */
	GLOG_MODID_EM,			/* 2:EM daemon */
	GLOG_MODID_FW_HIST,		/*3: Firmware download history module */
	GLOG_MODID_CLI_HIST,		/*4: CLI history module */
	GLOG_MODID_FWDL_GLOBAL_OM,	/*5: Firmware download global object manager */

	GLOG_MODID_MAX_ID		/* XX:THIS SHALL BE THE LAST ONE */
} glog_module_id_t;

#define IS_VALID_MOD_ID(x) ((((x) < 0) || ((x) >= GLOG_MODID_MAX_ID)) ? 0 : 1)

/*
 * CF storage space control.
 * Set limitation of geneic logger module id.
 */
#define GLOG_MODID_LIMIT	25
#if (GLOG_MODID_MAX_ID >= GLOG_MODID_LIMIT)
#error The number of generic logger module is over the limitation.
#error Check glog_module_id_t defines.
#endif

/*
 * Generic logger module description structure
 */
#define GLOG_MODULE_NAME_LEN	16
#define GLOG_MODULE_DESC_LEN	64
typedef struct glog_module_desc {
	glog_module_id_t	md_modid;
	char			md_name[GLOG_MODULE_NAME_LEN];
	char			md_desc[GLOG_MODULE_DESC_LEN];
	int			md_flags;
} glog_module_desc_t;

#ifdef __cplusplus
}
#endif

#endif /* __GLOG_MODULES_H__ */
