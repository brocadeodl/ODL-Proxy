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

#ifndef __HAM_API_H__
#define	__HAM_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Max length for time string returned by ctime */
#define HAM_TIME_STR_LEN	128

/* Max length for reason string in hamStat */
#define HAM_REASON_LEN		128

/*
 * hamHealth status
 *
 *		CP_HEALTHY: CP is up (if remote CP, this means heartbeat is up)
 *		CP_DEGRADED: Currently not used
 *		CP_FAILED: CP is down (if remote CP, this means heartbeat is down)
 *
 */
typedef enum {
	HAM_HMON_STOPPED = 0,
	HAM_CP_HEALTHY,
	HAM_CP_FAILED,
	HAM_NOT_SYNC,
	HAM_CP_NOT_AVAILABLE,
	HAM_CP_MAX
} hamHealth_t;

#define	BUF_SIZE	128
typedef struct hamStat {
	hamHealth_t	hamHealth;
	int	hamValid;	/* The rest of the fields is only valid if hamValid is 1 */
	int	hamState;
	int	hamCPNum;
	int	hamSlotNum;
	char	hamTakenOverTime[HAM_TIME_STR_LEN];
	char	hamTakenOverReasonText[HAM_REASON_LEN];
	char	hamBootUpReasonText[HAM_REASON_LEN];
	char	hamCPStatusString[HAM_REASON_LEN];
	char	fabosVersion[BUF_SIZE];
	char	cp_status_local[BUF_SIZE];
	char	cp_status_remote[BUF_SIZE];
	char	ha_status[BUF_SIZE];
} hamStat_t;

typedef enum reboot_opt {
		REBOOT_AS_MASTER    = 1,
		REBOOT_AS_SLAVE     = 2,
		REBOOT_QUICK        = 3
} reboot_opt_t;

static inline void reboot_system(reboot_opt_t *pOption)
{

}

static inline int getThisCPSlot(void)
{
    return (1);
}

static inline void get_ha_status(int state, hamStat_t *hamStat)
{
    snprintf(hamStat->ha_status, BUF_SIZE, "HA enabled, Heartbeat Up, HA State synchronized");
}

#ifdef __cplusplus
}
#endif

#endif
//EOF

