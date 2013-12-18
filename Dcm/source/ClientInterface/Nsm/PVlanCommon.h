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

typedef struct PVlanTrunkAssocMsg{
#define PVLAN_TRUNK__UNKNOWN_ACTION 0
#define PVLAN_TRUNK_ASSOC_PHY 1
#define PVLAN_TRUNK_ASSOC_PHY_DEL 2
#define PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM 3
#define PVLAN_TRUNK_ASSOC_PHY_DEL_ALL 4
#define PVLAN_TRUNK_ASSOC_PO 5
#define PVLAN_TRUNK_ASSOC_PO_DEL 6
#define PVLAN_TRUNK_ASSOC_PO_DEL_PRIM 7
#define PVLAN_TRUNK_ASSOC_PO_DEL_ALL 8

   int PortId;
   char PortName[32];
   unsigned long ifIndex;
   unsigned int ifType;
   int primaryVlanId;
   int secondaryVlanId;
   int opcode;
}PVlanTrunkAssocMsg_t;


typedef struct PVlanMappingMsg{
#define PVLAN_MAPPING__UNKNOWN_ACTION 0
#define PVLAN_MAPPING_PHY 1
#define PVLAN_MAPPING_PHY_DEL_PRIM 2
#define PVLAN_MAPPING_PHY_DEL_ALL 3
#define PVLAN_MAPPING_PO 4
#define PVLAN_MAPPING_PO_DEL_PRIM 5
#define PVLAN_MAPPING_PO_DEL_ALL 6

   int PortId;
   char PortName[32];
   unsigned long ifIndex;
   unsigned int ifType;
   int primaryVlanId;
   char *secVlanRange;
   int oper;
   int opcode;
}PVlanMappingMsg_t;
