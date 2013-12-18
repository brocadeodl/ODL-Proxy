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
 *   Author : Charanjith Kunduru                                           *
 ***************************************************************************/

#ifndef __SECURITY_IPFILTER_PUBLIC_H__
#define __SECURITY_IPFILTER_PUBLIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define IPF_IPV4 0
#define IPF_IPV6 1
#define IPF_MAX_NUM_RULES 512 
#define IPF_ACCEPT 0
#define IPF_DROP 1
#define	IPF_DEFAULT_POLICY_NAME "default_ipv4"
#define	IPF_DEFAULT6_POLICY_NAME "default_ipv6"
#define	FALSE 1
#define	TRUE 0
#define IPF_NAME_LEN 48
#define IPF_MAX_PORTNUM 65536
#define INET6_ADDRSTRLEN 46
#define IPF_IP6_ADDR_PREFIX_LEN (INET6_ADDRSTRLEN+4)
#define IPF_MAX_DSCP_LEN    50

typedef long ipf_txn_hdl_t;
typedef struct {
	/* Name of the policy table */
	char				name[IPF_NAME_LEN];

	/* Policy type. Value must be from ipfPolicyType_e */
	u_char				pType;

	/* State of the policy . Value must be from  ipfPolicyState_e */
	u_char				pState;

	/* Number of rules in that policy */
    int					numRules;
} ipfExtnPolicy_t;

/* structure for IP filter policy rules with externally exposed parameters */
typedef struct {
	/* Action to be performed. Value must be from ipfActionType_e */
	u_char				action;

	/* protocol number */
	u_short				proto_num;

	/* Source ipaddress/prefix, long enough for IPv6 */
	char				saddrPref[IPF_IP6_ADDR_PREFIX_LEN];

	/* Destination Port (First port in multiport) */
	u_short				dPort;

	/* End port no. in destination port range */
	u_short 			dEndport;

	/* Rule Position */
	int					rule_num;

	/* Type of traffic */
	u_short				type;

	/* Destination ipaddress/prefix, long enough for IPv6 */
	char				daddrPref[IPF_IP6_ADDR_PREFIX_LEN];

	/* Source Port (First port in multiport) */
	u_short             sPort;

	/* End port no. in source port range */
	u_short             sEndport;

	/* DSCP/TOS/Precedence value */
	char                preceDscpTos[IPF_MAX_DSCP_LEN];

	/* urg */
	int             urg;

	/* ack flag */
	int             ack;

	/* push */
	int             psh;

	/* fin flag */
	int             fin;

	/* rst flag */
	int             rst;

	/* sync flag */
	int             syn;

	/* neq sport */     
	int             neqSportFlag;

	/* neq dport */     
	int             neqDportFlag;
} ipfExtnRule_t;

#define ipfPolicyCreate(a,b) 0; cout << "Creating ACL policy is successful\n";
#define ipfPolicyRuleAdd(a,b,c,d,e,f,g) 0; cout << "Adding the rule to ACL policy is successful\n";
#define ipfPolicyRuleAddNos(a, b, c) 0; cout << "Adding optimized rule to ACL policy is successful\n";
#define ipfPolicyRuleDelete(a,b) 0; cout << "Deleting the rule from ACL policy is successful\n";
#define ipfPolicySave(a) ; cout << "Saving the ACL policy is successful\n";
#define ipfPolicyActivate(a) 0; cout << "Activating the ACL policy is successful\n";
#define ipfPolicyDelete(a) 0; cout << "Deleting the ACL policy is successful\n";
#define ipfActivePolicyGet(a, b) 0; cout << "getting the ACL policy is successful\n";
#define ipfPolicyGet(a,b) 0; cout << "getting the ACL policy is successful\n";
#define ipfPolicyRuleGet(a,b,c) 0; cout << "getting the Rules in the ACL policy is successful\n" << b << endl;
#define ipfTransBegin() 1; cout << "Transaction is started for  ACL policy DB\n";
#define ipfTransAbort(a) ; cout << "Transaction is aborted\n";
#define ipfTransEnd(a) ; cout << "Transaction is ended for  ACL policy DB\n";
#define findActiveInterface() 0; cout << "The Active MM\n";

#ifdef __cplusplus
}
#endif

#endif // __SECURITY_IPFILTER_PUBLIC_H__


