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

#ifndef IPADM_H
#define IPADM_H

#ifdef __cplusplus
extern "C"
{
#endif

#define IPADM_ADDR_NONE             0x00000001
#define IPADM_MASK_NONE             0x00000002
#define IPADM_GRAT_ARP                          0x00000004
#define IPADM_ADDR_IPFC_EXISTS                  0x00000008
#define IPADM_ADDR_TENTATIVE                    0x04000000
#define IPADM_ADDR_DEPRECATED                   0x08000000
#define IPADM_ADDR_AUTO_STATELESS               0x10000000
#define IPADM_ADDR_DHCP                         0x20000000
#define IPADM_CHANGE_PENDING                    0x40000000
#define IPADM_CHANGE_COMPLETE                   0x80000000
#define IPADM_ADDR_DHCPV6           0x00000010
/* flags that are changable by the API caller */
#define IPADM_USER_FLAGS (IPADM_ADDR_NONE | IPADM_MASK_NONE)
#define IPADM_IPV6_FLAGS (IPADM_ADDR_TENTATIVE | IPADM_ADDR_DEPRECATED)

#define IPADM_ERR_DUP_ADDRESS 13

#define MAX_DOM_SIZE	64

/* types used to identify a local IP address */
typedef enum {
	ACTIVE_CP_HOST = 0,		/* ids which ever is the active CP at the time */
	SPECIFIC_CP_HOST,		/* ids a specific CP independently of HA state */
	BLADE_HOST,				/* intelligent blade */
	CLUSTER_HOST,
	UNDETERMINED_HOST = 0xffffffff	/* used internally by ipadmd */
} ipadm_host_t;

typedef enum {
	ETHERNET_INTERFACE = 0,	/* ids an external ethernet device */
	FC_IP_INTERFACE			/* ids an FC-IP device */
} ipadm_interface_t;

typedef struct {
    char domainName[MAX_DOM_SIZE];
    char nameServer1[MAX_DOM_SIZE];
    char nameServer2[MAX_DOM_SIZE];
} dns_t;

typedef enum {
	SWITCH_ENTITY = 0,
	CP_ENTITY,
	BLADE_ENTITY,
	UNDETERMINED_ENTITY = 0xffffffff	/* used internally by ipadmd */
} ipadm_entity_t;

typedef struct ipadm_local_addr_id_struct {
	ipadm_host_t host_type;			/* type of host */
	int host_instance;				/* instance of above host */
	ipadm_interface_t interface_type;	/* type of interface */
	int interface_instance;			/* instance of above interface */
	ipadm_entity_t entity_type;		/* type of logical entity */
	int entity_instance;			/* instance of above logical entity */
	int addr_family;				/* AF_INET or AF_INET6 */
	int tag;
} ipadm_local_addr_id_t;

/* type for storing a number of IPv4 address prefix bits */
typedef unsigned char ipv4_prefix_len_t;

typedef unsigned int addr_flags_t;

/* type used to identify a gateway IP address */
typedef struct ipadm_gw_addr_id_struct {
	ipadm_host_t host_type;			/* type of host */
	int host_instance;				/* instance of above host */
	int addr_family;				/* AF_INET or AF_INET6 */
	int tag;
} ipadm_gw_addr_id_t;

/* type used to get or set IP address values */
typedef struct ipadm_ip_get_set_struct {
	int num_locals;				/* # of elements in local addr table */ 
	ipadm_local_addr_id_t *local_addr_ids;	/* ptr to local addr table */
	struct sockaddr_storage *local_addrs;	/* ptr to array of local IP addrs */
	ipv4_prefix_len_t *local_prefixes;	/* ptr to array of IPv4 prefixes */
	addr_flags_t *local_flags;		/* ptr to array of local addr flags */
	int num_gws;					/* # of elements in gateway addr table */
	ipadm_gw_addr_id_t *gw_addr_ids;	/* ptr to gateway addr table */
	struct sockaddr_storage *gw_addrs;	/* ptr to array of gateway IP addrs */
	addr_flags_t *gw_flags;			/* ptr to array of gateway addr flags */
#define IPADM_DHCP_ENABLEABLE		0x80000000
#define IPADM_CHASSIS_MODEL			0x40000000
#define IPADM_AUTOCONFIG_ENABLE		0x00000002
#define IPADM_DHCP_ENABLE			0x00000001
#define IPADM_DHCPV6_ENABLE         0x00000004
	unsigned int dhcp_flag;					/* is DHCP enabled */
} ipadm_ip_get_set_t;

#define MAX_LOCAL_ADDR_TABLE_SIZE	4
#define MAX_GW_ADDR_TABLE_SIZE		4
#define SOCKADDR_INP(p) ((struct sockaddr_in *) &(p))
#define SOCKADDR_IN6P(p) ((struct sockaddr_in6 *) &(p))

/*
int ipAdmLocalAddrIdGet(ipadm_local_addr_id_t *pTable, int *pNumElements);
int ipAdmGwAddrIdGet(ipadm_gw_addr_id_t *pTable, int *pNumElements);
int ipAdmIpAddrGet(ipadm_ip_get_set_t *pGetSet);
int ipAdmIpAddrSet(ipadm_ip_get_set_t *pGetSet, int asyncFlag, unsigned int *pChangeNum);
int ipAdmPrefixToMask(u_int *pMask, ipv4_prefix_len_t prefix);
*/
static inline int ipAdmLocalAddrIdGet(ipadm_local_addr_id_t *pTable, int *pNumElements) {

#if 0
	pTable->host_type = ACTIVE_CP_HOST;			/* type of host */
	pTable->host_instance = 0;				/* instance of above host */
	pTable->interface_type = ETHERNET_INTERFACE;	/* type of interface */
	pTable->interface_instance = 1;			/* instance of above interface */
	pTable->entity_type = SWITCH_ENTITY;		/* type of logical entity */
	pTable->entity_instance = 0;			/* instance of above logical entity */
	pTable->addr_family = 2;				/* AF_INET or AF_INET6 */
	pTable->tag = 0;
#endif

	pTable[0].host_type = ACTIVE_CP_HOST;			/* type of host */
	pTable[0].host_instance = 0;				/* instance of above host */
	pTable[0].interface_type = ETHERNET_INTERFACE;	/* type of interface */
	pTable[0].interface_instance = 1;			/* instance of above interface */
	pTable[0].entity_type = SWITCH_ENTITY;		/* type of logical entity */
	pTable[0].entity_instance = 0;			/* instance of above logical entity */
	pTable[0].addr_family = 2;				/* AF_INET or AF_INET6 */
	pTable[0].tag = 0;

	pTable[1].host_type = ACTIVE_CP_HOST;			/* type of host */
	pTable[1].host_instance = 0;				/* instance of above host */
	pTable[1].interface_type = ETHERNET_INTERFACE;	/* type of interface */
	pTable[1].interface_instance = 1;			/* instance of above interface */
	pTable[1].entity_type = SWITCH_ENTITY;		/* type of logical entity */
	pTable[1].entity_instance = 0;			/* instance of above logical entity */
	pTable[1].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[1].tag = 0;

	pTable[2].host_type = ACTIVE_CP_HOST;			/* type of host */
	pTable[2].host_instance = 0;				/* instance of above host */
	pTable[2].interface_type = ETHERNET_INTERFACE;	/* type of interface */
	pTable[2].interface_instance = 1;			/* instance of above interface */
	pTable[2].entity_type = SWITCH_ENTITY;		/* type of logical entity */
	pTable[2].entity_instance = 0;			/* instance of above logical entity */
	pTable[2].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[2].tag = 0;

	pTable[3].host_type = ACTIVE_CP_HOST;			/* type of host */
	pTable[3].host_instance = 0;				/* instance of above host */
	pTable[3].interface_type = ETHERNET_INTERFACE;	/* type of interface */
	pTable[3].interface_instance = 1;			/* instance of above interface */
	pTable[3].entity_type = SWITCH_ENTITY;		/* type of logical entity */
	pTable[3].entity_instance = 0;			/* instance of above logical entity */
	pTable[3].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[3].tag = 0;


        *pNumElements = 4;
	return (0);
}

static inline int ipAdmGwAddrIdGet(ipadm_gw_addr_id_t *pTable, int *pNumElements) {

#if 0
	pTable->host_type = SPECIFIC_CP_HOST;			/* type of host */
	pTable->host_instance = 0;				/* instance of above host */
	pTable->addr_family = 2;				/* AF_INET or AF_INET6 */
	pTable->tag = 0;
#endif

	pTable[0].host_type = SPECIFIC_CP_HOST;			/* type of host */
	pTable[0].host_instance = 0;				/* instance of above host */
	pTable[0].addr_family = 2;				/* AF_INET or AF_INET6 */
	pTable[0].tag = 0;

	pTable[1].host_type = SPECIFIC_CP_HOST;			/* type of host */
	pTable[1].host_instance = 0;				/* instance of above host */
	pTable[1].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[1].tag = 0;

	pTable[2].host_type = SPECIFIC_CP_HOST;			/* type of host */
	pTable[2].host_instance = 0;				/* instance of above host */
	pTable[2].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[2].tag = 0;

	pTable[3].host_type = SPECIFIC_CP_HOST;			/* type of host */
	pTable[3].host_instance = 0;				/* instance of above host */
	pTable[3].addr_family = AF_INET6;				/* AF_INET or AF_INET6 */
	pTable[3].tag = 0;

        *pNumElements = 4;
	return (0);
}

static inline int ipAdmIpAddrGet(ipadm_ip_get_set_t *pGetSet) {
	pGetSet->num_locals = 4;
	pGetSet->num_gws = 4;
	SOCKADDR_INP(pGetSet->local_addrs[0])->sin_addr.s_addr = htonl(0xc0a80a05);
	SOCKADDR_INP(pGetSet->gw_addrs[0])->sin_addr.s_addr = htonl(0xc0a80a06);

    #if 0
	SOCKADDR_IN6P(pGetSet->local_addrs[1])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->local_addrs[1])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[1])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[1])->sin6_addr.in6_u.u6_addr32[3] = htonl(0x1);

	SOCKADDR_IN6P(pGetSet->local_addrs[2])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->local_addrs[2])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[2])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[2])->sin6_addr.in6_u.u6_addr32[3] = htonl(0x2);

	SOCKADDR_IN6P(pGetSet->local_addrs[3])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->local_addrs[3])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[3])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->local_addrs[3])->sin6_addr.in6_u.u6_addr32[3] = htonl(0x3);

	SOCKADDR_IN6P(pGetSet->gw_addrs[1])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->gw_addrs[1])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[1])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[1])->sin6_addr.in6_u.u6_addr32[3] = htonl(0xa);

	SOCKADDR_IN6P(pGetSet->gw_addrs[2])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->gw_addrs[2])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[2])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[2])->sin6_addr.in6_u.u6_addr32[3] = htonl(0xb);

	SOCKADDR_IN6P(pGetSet->gw_addrs[3])->sin6_addr.in6_u.u6_addr32[0] = htonl(0xfe800000);
	SOCKADDR_IN6P(pGetSet->gw_addrs[3])->sin6_addr.in6_u.u6_addr32[1] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[3])->sin6_addr.in6_u.u6_addr32[2] = 0x0;
	SOCKADDR_IN6P(pGetSet->gw_addrs[3])->sin6_addr.in6_u.u6_addr32[3] = htonl(0xc);
    #endif

	pGetSet->local_flags[0] = 0;
	pGetSet->local_flags[1] = IPADM_ADDR_AUTO_STATELESS | IPADM_ADDR_TENTATIVE;
	pGetSet->local_flags[2] = IPADM_ADDR_AUTO_STATELESS | IPADM_ADDR_DEPRECATED;
	pGetSet->local_flags[3] = 0;

	pGetSet->local_prefixes[0] = 24;
	pGetSet->local_prefixes[1] = 64;
	pGetSet->local_prefixes[2] = 72;
	pGetSet->local_prefixes[3] = 127;

	return (0);
}

//#define ipAdmIpAddrGet(x) 0; printf("In get")

static inline int ipAdmIpAddrSet(ipadm_ip_get_set_t *pGetSet, int asyncFlag, unsigned int *pChangeNum) {
	return (0);
}

static inline int ipAdmPrefixToMask(u_int *pMask, ipv4_prefix_len_t prefix) {
	return (0);
}

static inline int ipAdmAddrValid(char *buf, u_int *pAddr) {
        return (0);
}

static inline int domNsSet(dns_t *dns) {
	return (0);
}

static inline int domNsReset(void ) {
	return (0);
}

static inline int domNsGet(dns_t *dns) {
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif //IPADM_H
