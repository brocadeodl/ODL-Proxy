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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSM_L3_UTILS_H 
#define NSM_L3_UTILS_H 

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"

using namespace WaveNs;
using namespace std;


namespace DcmNs
{
    class PhyIntfLocalManagedObject;
    class VlanIntfManagedObject;
    class PoIntfManagedObject;
    class NsmL3ReadyManagedObject;
    class NsmIpAddrConfigManagedObject;
    class NsmIpAddrManagedObject;
    class NsmNodePoManagedObject;
    class NsmIntfSviManagedObject;
    class NsmIntfLoopbackManagedObject;

    class NsmL3Util
    {
        private:

        public:

/* Don't delete MO pointers returned from the following functions */
/* "fromLocal" means caller function is an instance of LocalObjectManager */ 
/* "fromGlobal" means caller function is an instance of LocalObjectManager */ 
/* PhyIntf related functions are checked against "fromLocal". */
/* PoIntf and VlanIntf related functions are checked against "fromGlobal". */
/* User may use "CheckIpAddrReady" functions for IpAddress dependant protocols. */
/* User may use "CheckL3Ready" functions for IpAddress independant L3 protocols. */

            static PhyIntfLocalManagedObject* PhyIfNameGetPhyIfLocalMO(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true,
                const vector <string> *sFields = NULL);
            static NsmL3ReadyManagedObject* PhyIfNameGetL3ReadyMO(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true, const vector <string> *sFields = NULL);
            static NsmIpAddrConfigManagedObject* PhyIfNameGetIpAddrCfgMO(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true, const vector <string> *sFields = NULL);
            static NsmIpConfigManagedObject* PhyIfNameGetIpConfigMO(
                string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true);
            static NsmIpAddrManagedObject* PhyIfNameGetPrimaryIpAddrMO(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL, 
                bool fromLocal = true);
            static bool PhyIfNameCheckL3Ready(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true);
            static bool PhyIfNameCheckIpAddrReady(string ifName, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true);
            static PoIntfManagedObject* PoIdGetPoIfMO(UI32 poId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true,  const vector <string> *sFields = NULL);
            static NsmNodePoManagedObject* PoIdGetNodePoMO(UI32 poId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static NsmL3ReadyManagedObject* PoIdGetL3ReadyMO(UI32 poId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmL3ReadyManagedObject* PoMoGetL3ReadyMO(PoIntfManagedObject* poIntfMO,
                PrismSynchronousLinearSequencerContext  *pContext = NULL);
            static NsmIpAddrConfigManagedObject* PoIdGetIpAddrCfgMO(UI32 poId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmIpConfigManagedObject* PoIdGetIpConfigMO(UI32 poId, 
                const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmIpAddrManagedObject* PoIdGetPrimaryIpAddrMO(UI32 poId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool PoIdCheckNodePo(UI32 poId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool PoIdCheckL3Ready(UI32 poId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool PoIdCheckIpAddrReady(UI32 poId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);

            static VlanIntfManagedObject* VlanIdGetVlanIfMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmIntfSviManagedObject* VlanIdGetIntfSviMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmIntfSviManagedObject* GetIntfSviMO(VlanIntfManagedObject *vlanMo, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmL3ReadyManagedObject* VlanIdGetL3ReadyMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true, const vector <string> *sFields = NULL);
            static NsmL3ReadyManagedObject* LoopbackIdGetL3ReadyMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL);
            static NsmIpAddrConfigManagedObject* VlanIdGetIpAddrCfgMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static NsmIpConfigManagedObject* VlanIdGetIpConfigMO(UI32 vlanId, 
                const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static NsmIpAddrManagedObject* VlanIdGetPrimaryIpAddrMO(UI32 vlanId, const SI32 &mapId, 
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool VlanIdCheckIntfSvi(UI32 vlanId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool VlanIdCheckL3Ready(UI32 vlanId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static bool VlanIdCheckIpAddrReady(UI32 vlanId, const SI32 &mapId,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromGlobal = true);
            static void NodePoIfGetMOs(vector<NsmNodePoManagedObject*>& mos,
                const SI32 mapID = 0); 
            static void NodeVlanIfGetMOs(vector<NsmIntfSviManagedObject*>& mos,
                const LocationId &locId = 0);
            static void GetIntfLoopbackMOs(
                vector<NsmIntfLoopbackManagedObject*>& mos, 
                const LocationId &locId = 0);
            static NsmL3ReadyManagedObject* PhyIfGetL3ReadyMO(PhyIntfLocalManagedObject *pPhyIntfMo = NULL,
                PrismSynchronousLinearSequencerContext  *pContext = NULL,
                bool fromLocal = true);
            static NsmIntfLoopbackManagedObject* loIdGetLoopbackIfMO (UI32 loId,
                const SI32 &mapId, const vector <string> *sFields = NULL);

    };

} // DcmNs

#endif // NSM_L3_UTIL_H
