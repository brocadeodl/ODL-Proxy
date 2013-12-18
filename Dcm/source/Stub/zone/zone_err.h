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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __ZONEZONE_ERR_H__
#define __ZONEZONE_ERR_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	/* Generic error messages */

	AD_SUCCESS 					= 1,	/* SUCCESS status */
	AD_FAILURE 					= 2,	/* FAILURE status */

	/* Zone errors */
	ZN_ERR_OK					= 0,

	ZN_ERR_INVALID_KEY			= -1,
	ZN_ERR_TRANS_IN_PROGRESS	= -3,
	ZN_ERR_DUPLICATE_KEY		= -4,
	ZN_ERR_NO_MEMORY			= -7,
	ZN_ERR_DUPLICATE_ENTRY      = -8,
	ZN_ERR_NO_TRANS             = -9,

	ZN_ERR_NO_EFF_CFG			= -50, /* No effective CFG found */
	ZN_ERR_INVALID_VALUE		= -99,
	TI_ZONE_PORTLIST_MISSING	= -100,

	/* AD Validate/Lookup/Allocate */
	INVALID_AD_NAME			= -101,	/* AD name invalid */
	INVALID_ALIAS_NAME		= -102,	/* ALIAS name invalid */
	AD_NUM_OUT_OF_RANGE		= -103,	/* AD number out of range */
	AD_NAME_SZ_TOO_LONG		= -104,	/* AD name is too long */
	AD_NAME_INVALID_CHAR	= -105,
	AD_OP_NOT_PERMITTED		= -106,
	NO_FREE_AD				= -107,
	AD_EXIST				= -108,
	AD_NON_EXIST			= -109,
	AD_BAD_PARM				= -110,
	AD_BAD_MBR_TYPE			= -111,
	AD_INVALID_MBR			= -112,
	AD_INVALID_TYPE			= -113,
	AD_NO_MEMORY			= -114,
	INVALID_OP_ON_AD0		= -115,
	INVALID_OP_ON_AD255		= -116,
	AD_NAME_NUM_MISMATCH	= -117,
	AD_UNINITIALIZED		= -118,
	AD_SW_MBR_DOM_INVALID	= -119, /* Invalid Domain ID in Switch member */
	                                /* List */
	AD_SW_MBR_DOM_LKUP_FAIL	= -120, /* AD Switch member's Domain ID to WWN */
	                                /* lookup failure */
	AD_MBR_ALREADY_EXISTS	= -121, /* AD member already exists */
	AD_MBR_NOT_PRESENT		= -122, /* AD member not present */
	AD_DUPLICATE_MBR		= -123, /* Duplicate members */
	AD_LOOKUP_FAILURE 		= -123,
	/* AD CLI/API Specific  */
	AD_NO_ZONE_LICENSE		= -201,
	AD_BAD_CALLER_TYPE		= -202,
	AD_ACCT_INFO_NOT_AVL	= -202,
	AD_SEL_NO_PERM			= -203,
	AD_SHO_NO_PERM			= -204,
	AD_NO_PERM				= -205,
	AD_FAB_UNSTABLE			= -206,
	AD_FILE_OP_FAILED		= -207,

	/* IPC Error codes */
	AD_IPC_SUCCESS			= -301,
	AD_IPC_DECODE_FAIL		= -302,
	AD_IPC_EMPTY_BUFF		= -303,
	AD_IPC_SENDRCV_FAIL		= -304,
	AD_IPC_ENCODE_FAIL		= -305,
	AD_IPC_ERROR			= -306,

	/* AD CLI/API Status */
	AD_ERR_ENV_VAL			= -401,	/* Environment variable error	*/
	INVALID_AD_NUM 			= -402,	/* AD Number is not valid	*/
	OPERATION_NOT_PERMITTED = -403,
	AD_FABRIC_BUSY 			= -404,	/* Fabric is busy */
	AD_INVALID_USER_NAME 	= -405,	/* Username unknown */
	AD_INFO 				= -406, /* No error messages */
	AD_ERROR 				= -407,	/* When the err msg is handled in the */
									/* backend */
	AD_DEFZONE_COMBO_ERR 	= -408, /* AD config allowed only in defzone */
									 /* noaccess mode */
	AD_NO_CHANGE_NO_SAVE 	= -409, /* Nothing to save in ad --save path */
	AD_DELETE_SAVE_ERR  	= -410, /* deleted AD has to be applied directly. */
	                                /* Just ad --save of a deleted AD is not */
	                                /* allowed */
	AD_DEACTV_FAIL_EFF_ZN_CFG
	                        = -411, /* AD cannot be deactivated bcos Eff Zone */
	                                /* Cfg exists */
	AD_DELETE_FAIL_ZNCFG	= -412, /* AD cannot be deleted bcos Zone CFG */
	                                /* exists within an AD */
	AD_REMOVE_FAIL_ZNCFG	= -413, /* Last member cannot be removed bcos */
	                                /* it casues AD delete & AD has Zone CFG */
	AD_TRANS_IN_PROGRESS	= -414, /* Another AD transaction already in */
	                                /* progress */
	STANDBY_AD_UNAWARE  	= -415,	/* Standby AD is running AD unaware fw */

	AD_ZNDB_FULL  			= -416, /* Zone database can not accomodate */
	                                /* this transaction, max size reached */

	AD_NO_SUPPORT_INTEROP	= -417, /* Operation not supported for the */
	                                /* current interopmode */

	/* Zone Validate Error Codes */
	ZV_PARAM_ERR			= -451,
	ZV_PATT_NONEX			= -452,
	ZV_FILE_OP_FAILED		= -453,
	ZV_NOMEM				= -455,
	ZV_ERR_ZONED			= -456,

	/* Defzone error code */
	DEFZONE_ERR_DUP			= -470,	/* Defzone requested mode is already set */
	NO_ACC_TO_ALL_ACC		= -471, /* Mode change allacc->noacc disallowed */
	DEFZONE_INVALID_VAL		= -472,	/* Cfgdload bad value for key "defzone" */
	DEFZONE_AD_CONFIGURED	= -473,	/* Can't change mode with AD configured */
	DEFZONE_RD_CONFIGURED	= -474,	/* Can't change mode with RD zones in */
	                                /* fabric */

	/* Group Copy error codes */
	ZN_MBR_EXIST			= -481,
	ZN_DUPLICATE_NAME		= -482,
	ZN_NON_EXIST			= -483,
	ZN_INVALID_NAME			= -484,
	ZN_UNEXPECTED_ERR		= -485,
	AD_INTEROP_COMBO_ERR	= -486, /* can't configure ADs in interop mode */
	DEFZONE_INTEROP_COMBO_ERR	= -487, /* can't configure defzone in interop */
	                                    /* mode */
	DEFZONE_AD_TRANS_IN_PROG	= -488, /* can't configure defzone while */
	                                    /* AD255 transaction in progress */
	ZN_ERR_NO_COPY_SPECIAL		= -489, /* Special Zone (TI/RD/QOS) "copy" */
	                                    /* not allowed */
	AD_DEACTV_CFG_ENABLE_FAIL	= -490, /* can't cfgEnable due to deactivated */
	                                   /* AD - Defined configuration Inactive */
	DEFZONE_ERR_RD_ZONES		= -491,
	ZN_ERR_NO_RENAME_SPECIAL	= -492, /* Special Zone "rename" not allowed */

	/* NOS Zone Plugin error codes */
	ZN_ERR_EMPTY_ZONE_OBJECT	= -501,	/* An empty zone object exists in the cfg trying to be enabled */
	ZN_ERR_CFG_ENABLE_INVALID_CFGNAME	= -502, /* CfgEnable failed due to invalid cfgName specified */
	ZN_ERR_CFG_ALIAS_MEMBER		= -503, /* Cfg contains Alias member, which is not permitted */
	ZN_ERR_MULTI_MEMBER			= -504  /* Multi-member removal not supported for the specified command */

} ad_err_num_t;

#ifdef __cplusplus
}
#endif

using namespace WaveNs;

namespace DcmNs
{

}

#endif // __ZONEZONE_ERR_H__
