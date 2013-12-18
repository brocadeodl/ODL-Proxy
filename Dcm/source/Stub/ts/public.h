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

#ifdef __cplusplus
extern "C"
{
#endif

#define TS_DEFAULT_CLKSVR           "LOCL"
#define TS_TOKEN_SEPERATOR          ";"
#define TS_MAX_CLKSVRS              8
#define TS_MAX_CLKSVR_LEN           32
#define TS_MAX_CLKSVR_LEN_EXT       257

#define TS_OK						0
#define TS_ERR_GENERAL                      -1
#define TS_ERR_MAKEIU                       -2
#define TS_ERR_MCAST                        -3
#define TS_ERR_TYPE                         -4
#define TS_ERR_DATA                         -5
#define TS_ERR_NULLPTR                      -6
#define TS_ERR_POSTMSG                      -7
#define TS_ERR_STAMP                        -8
#define TS_ERR_INVALID_NODE_NAME            -9
#define TS_ERR_LOW_MEMORY                   -10
#define TS_ERR_CONFIG_SET                   -11
#define TS_ERR_CONFIG_GET                   -12
#define TS_ERR_CONFIG_UPDT                  -13
#define TS_ERR_MAX_CLKSVRS_LT               -14
#define TS_ERR_MAX_CLKSVR_LEN_LT            -15
#define TS_ERR_OPN_NOT_AUTHORIZED           -16
#define TS_ERR_IPC                          -17
#define TS_ERR_IPC_DATA                     -18
#define TS_ERR_VERIFY_FAILED                -19
#define TS_ERR_INVALID_CHAR                 -20
#define TS_ERR_INVALID_NTPSVR               -21
#define TS_ERR_MQWRITE                      -22
#define TS_ERR_INVALID_TZ_NAME              -23
#define TS_ERR_SFOS_ENABLED                 -24
#define TS_ERR_MAX_NTP_SVRS                 -25
#define TS_ERR_NTP_QUERY_FAILED             -26
#define TS_ERR_LOCL_LIST                    -27
#define TS_ERR_DUP_LIST                     -28
#define TS_ERR_INVALID_ENTRIES              -29
#define TS_ERR_SFOS_IPV6_DNS				-30

static char ntp_svr_list[80] = {0};
static char clock_timezone_name[80] = {0};

static inline void tsSetSystemTZName(char *name)
{
	strcpy(clock_timezone_name, name);
	printf("setTsTimeZoneName successful\n");
}

static inline char *getTsTimeZoneName(char **name)
{
	*name = (char *)malloc(80);
	strcpy(*name, clock_timezone_name);
	return *name;
}

#define tsClockServer(x) 0; printf("tsClockServer successful\n");

static inline int tsGetClockServer(char **svrList)
{
	static char *ntp_active_svr;
	int i;

	ntp_active_svr = (char *)malloc(80);
	for (i = 0; i < 80; i++)
	{
		if (('\0' == ntp_svr_list[i]) || (TS_TOKEN_SEPERATOR[0] == ntp_svr_list[i]))
			break;
		ntp_active_svr[i] = ntp_svr_list[i];
	}
	*svrList = ntp_active_svr;
	return 0;
}

static inline int tsSetLocalClock(char *datetime)
{
	printf("tsSetLocalClock %s\n", datetime);
	return 0;
}

static inline void getTsTimeZone(int *tzho, int *tzmo)
{
	*tzho = *tzmo = 0;
}

// #define tsSetClockServer(svrList) 0; printf("tsSetClockServer successful\n");
#define tsConfigureClockServer(svrList, flag) 0; if (NULL != svrList) printf (" "); printf("tsConfigureClockServer successful\n");

/* Denotes the key format supported in /etc/ntp.keys */
typedef enum {
    KEY_DES = 0, /* Key format stored as S indicating DES format */
    KEY_NTP, /* Key format stored as N indicating NTP standard */
    KEY_ASCII, /* Key format stored as A indicating ASCII standard */
    KEY_MD5 /* Key format stored as M indicating MD5 */
}ntpKeyType_e;

/* This structure is used for storing index, key type and key information */
typedef struct {
    int             index;  
    ntpKeyType_e    keyType;
    char            *key;   
}keyInfo_t;

/* This structure is used to map the server with index information */
typedef struct {
    char        server[TS_MAX_CLKSVR_LEN_EXT];
    keyInfo_t   keyinfo;
}serverKeyInfo;

#define tsSetClockServerAuthKey(flag, serverKey) 0; printf ("%p : ", (void *) serverKey); printf("tsSetClockServerAuthKey successful\n"); 

#ifdef __cplusplus
}
#endif

