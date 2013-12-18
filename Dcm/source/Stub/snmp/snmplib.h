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
 *   Author : Priya Ahuja                                                  *
 ***************************************************************************/

#ifndef __SNMP_SNMPLIB_H__
#define __SNMP_SNMPLIB_H__

#ifdef __cplusplus
extern "C"
{
#endif
#define MAX_COMMUSTR_SIZE       64
#define NV_MAX_PTYNAME  24
#define TARGET_IP_ADDRESS_SIZE  256
#define MIN_M2DISPLAYSTRSIZE 4
#define M2DISPLAYSTRSIZE 256
#define AGT_MAX_SECRET_SIZE     64
#define AGT_MAX_CONTEXT_SIZE    32
#define AGT_MAX_PASSWD_SIZE     60
#define AGT_MAX_PSWD_SIZE     32
#define MAX_AGTPTY_ENTRIES          6
#define MAX_USM_ENTRIES         6
#define MAX_TRAPTARGET_ENTRIES          6
#define MAX_COMMUNITY_ENTRIES  256 
#define MAX_CONTEXT_MAPPING_ENTRIES 256
#define DEFAULT_COMMUNITY_ENTRIES    6
#define MAX_AGTPTYV2_ENTRIES      6
#define AUTH_PROTOCOL_MD5       1
#define AUTH_PROTOCOL_SHA       2
#define NO_AUTH_PROTOCOL        3
#define PRIV_PROTOCOL_DES       1
#define NO_PRIV_PROTOCOL        2
#define PRIV_PROTOCOL_3DES    3
#define PRIV_PROTOCOL_AES_128   4
#define PRIV_PROTOCOL_AES_192   5
#define PRIV_PROTOCOL_AES_256   6
#define AGT_MAX_CONTEXT_SIZE       32
#define MAX_CONTEXT_MAPPING_ENTRIES 256
#define DEFAULT_COMMUNITY_ENTRIES    6

    /* default values for some MIB-II system group objects */
#define DEFAULT_SYSLOCATION "End User Premise."
#define DEFAULT_SYSCONTACT  "Field Support."
#define DEFAULT_SYSDESCRPTION "Brocade VDX Switch."
#define MAX_USERNAME_SIZE     32

#define LOCALENGINEID 36
typedef struct {
    unsigned char octet_array[32];
    int length;
} fixed_OctetString;
                    
typedef struct octetStr{
    unsigned char  *octet_ptr;
    int        length;
}octetString;

typedef struct {
    char communitystr[AGT_MAX_SECRET_SIZE];
    int  access;            /* RO/RW */
} agtcommunity_t;

typedef struct {
    char  agt_ptyname[NV_MAX_PTYNAME];
	char nv_commuStr[NV_MAX_PTYNAME];
    char  agt_tAddr[TARGET_IP_ADDRESS_SIZE];
    u_int   agt_port;                       /* UDP port */
	u_int   agt_trapSeverityLevel; /* Trap Seveity Level */
} agtpty_t;

typedef struct agtptyv2c {
        u_char  agtv2c_ptyname[NV_MAX_PTYNAME];    /* party name */
        u_char  agtv2c_tAddr[TARGET_IP_ADDRESS_SIZE];                  /* IP address */
        u_int   agtv2c_port;                       /* UDP port */
	    u_char  nv_commuStrv2c[NV_MAX_PTYNAME];
		u_int   agtv2c_trapSeverityLevel; /* Trap Seveity Level */
}agtptyv2c_t ;

typedef struct v3trapTargetEntry {
    u_int targetIndex; /* index for this table entry */
    u_int usmIndex; /* user name from usm table will be used */
    u_char targetAddr[TARGET_IP_ADDRESS_SIZE];
    u_int severityLevel;
    u_int agt_port;
    u_int informenabled;
} agtv3trapTargetEntry_t ;
                                    
typedef struct agt_usmEntry {
    u_int  usmIndex;
    u_int  userAccessLevel; /* 1-RW or 2-RO */
    u_char  userName[MAX_USERNAME_SIZE + 1];
    u_char  agt_authSecret[AGT_MAX_PASSWD_SIZE + 1];
    u_char  agt_privSecret[AGT_MAX_PASSWD_SIZE + 1];
    u_int authProtocol;
    u_int privProtocol;
    u_int isInformsEnabled;
	u_int agt_encpswdflag;
} agtusmEntry_t ;
typedef struct contextMapping {
    char contextName[AGT_MAX_CONTEXT_SIZE];
    char vrfName[AGT_MAX_CONTEXT_SIZE];
    char protoInstance[AGT_MAX_CONTEXT_SIZE];
    u_int rbridgeId;
    int storageType;
    int rowStatus;
}agtContextMapping_t;
                                        
typedef struct {
	u_char           sysContact[M2DISPLAYSTRSIZE];
	u_char           sysLocation[M2DISPLAYSTRSIZE];
	u_char           sysDescr[M2DISPLAYSTRSIZE];
	agtcommunity_t  agtcommunityentries[MAX_COMMUNITY_ENTRIES];
	agtpty_t agtptyEntries[MAX_AGTPTY_ENTRIES];
	agtptyv2c_t agtptyv2cEntries[MAX_AGTPTYV2_ENTRIES];
	agtusmEntry_t  agtusmEntries[MAX_AGTPTY_ENTRIES];
    agtv3trapTargetEntry_t agtV3TrapEntries[MAX_TRAPTARGET_ENTRIES];
	agtContextMapping_t  agtContextMap[MAX_CONTEXT_MAPPING_ENTRIES];
	/* local Engine Id */
	u_char			localEngineId[LOCALENGINEID];
} agtcfg_t ;
/*
agtcfg_t agtcfg = {
	{
	{"Secret C0de" , 1},
	{"OrigEquipMfr", 1},
	{"private", 1},
	{"public", 0},
	{"common", 0},
	{"FibreChannel", 0}
	}
};
*/
#define agtcfgXMSet(x) 0; printf("SET successful\n")
#define agtcfgXMGet(x) 0; printf("GET successful\n")
#define snmpManagerEngineIdsGet(x) 0;if (x) {if (9 == (x)->length) printf (" ");} printf("Engine ID GET successful\n")
#define snmpManagerEngineIdsSet(x) 0;if (x) {if (9 == (x)->length) printf (" ");} printf("Engine ID SET successful\n")
#define convertStringtoEngineID(y, x) ;printf ("Conversion of EngineID successful\n")


#define encryptPasswd(x, y) 0; printf("Encrypted successfully with encryption type : %d\n", x)

/*
int agtcfgXMSet(agtcfg_t *cfgNew);
int agtcfgXMSet(agtcfg_t *cfgNew)
{
    printf("!!!!---snmp Comm Change OK ---!!!!!\n%s*%d\n", cfgNew->agtcommunityentries[0].communitystr, cfgNew->agtcommunityentries[0].access);
    return (0);
}

int
agtcfgXMGet(agtcfg_t *cfg);

int
agtcfgXMGet(agtcfg_t *cfg) {
	cfg = &agtcfg;
	return (0);
}
*/
#ifdef __cplusplus
}
#endif

#endif // __PUBLIC_H__

