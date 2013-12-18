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

#include <stdio.h>
#include <unistd.h>

 
enum VlanClassifierProto {
    VLAN_CLASSIFIER_PROTO_UNKNOWN = 0,
    VLAN_CLASSIFIER_PROTO_ARP = 2054,
    VLAN_CLASSIFIER_PROTO_IP = 2048 ,
    VLAN_CLASSIFIER_PROTO_IPV6 = 34525
};

enum VlanClassifierEncap {
    VLAN_CLASSIFIER_ENCAP_UNKNOWN = 0,
    VLAN_CLASSIFIER_ENCAP_ETHV2 ,
    VLAN_CLASSIFIER_ENCAP_LLC_SNAP ,
    VLAN_CLASSIFIER_ENCAP_LLC_NO_SNAP
};

typedef struct VlanClassifierCreateRuleMsg{
   int ruleId; 
   int protoVal;
   int encap;
   char mac[15];
   unsigned int opcode;
}VlanClassifierCreateRuleMsg_t;
typedef enum {
        UNKNOWN_VLAN_CLASSIFIER = 0,
        CREATE_MAC_VLAN_CLASSIFIER ,
        CREATE_PROTO_VLAN_CLASSIFIER
}vlanClassifierOpcode;


typedef struct VlanClassifierCreateGroupMsg{
   int ruleId;
   int groupId;
   int operation;
}VlanClassifierCreateGroupMsg_t;

typedef struct VlanClassifierActivateGroupMsg{
#define VLAN_CLASSIFER_UNKNOWN_ACTION 0
#define VLAN_CLASSIFIER_PO_ACTIVATE 1 
#define VLAN_CLASSIFIER_PO_DEACTIVATE 2 
#define VLAN_CLASSIFIER_TE_ACTIVATE 3 
#define VLAN_CLASSIFIER_TE_DEACTIVATE 4 
#define VLAN_CLASSIFIER_TE_REMOVE_MO 5 
#define VLAN_CLASSIFIER_PO_REMOVE_MO 6 
   int PortId;
   char PortName[32];
   unsigned long long ifIndex;
   int groupid;
   int vlanid;
   int opcode; 	
}VlanClassifierActivateGroupMsg_t;


 
