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

#ifndef PVLAN_UTILS_H
#define PVLAN_UTILS_H

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Utils/DceContext.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
  class VlanIntfManagedObject;

  class PVlanUtils: public WaveObjectManagerToolKit
  {
    private:
    public:
	  static bool isPrimaryVlan(UI32 primVId);
	  static bool isSecondaryVlanRange(UI32Range vlanRange);
	  static bool isPrimaryVlanRange(UI32Range vlanRange);
	  static bool isSecondaryVlan(UI32 vlanId);
	  static bool isIsolatedVlanCountMore(UI32Range vlanRange);
	  static bool isPVlanIdPresent(UI32Range vlanRange, UI32 primVId);
	  static bool doesAssociationExists(UI32 primVId, UI32 secVId, DceContext *ctxt = NULL);
	  static bool doesAssociationExists(UI32 primVId, UI32Range secVlanRange, DceContext *ctxt = NULL);
	  static bool isSecVlanPartOfDomain(UI32 primVId, UI32Range secVlanRange, DceContext *ctxt = NULL);
	  static bool isDomainPresentOnPromiscuous(vector<WaveManagedObjectPointer<PVlanMappingManagedObject> >  pvlanMapping, UI32 primVId);
	  static bool isPVlan(UI32 primVId);
	  static bool isVeConfigured(UI32 primVId,  DceContext *ctxt = NULL, VlanIntfManagedObject *pVlanMO = NULL);
	  static bool isStaticMacConfigured(UI32 vlanId);
	  static bool isL2AclConfigured(UI32 vlanId, DceContext *ctxt = NULL);
	  static bool areMemberPortsConfigured(UI32 vlanId);
	  static bool areMemberPOsConfigured(UI32 vlanId);
	  static bool isMSTPInstancePresent(UI32 vlanId, DceContext *ctxt = NULL, VlanIntfManagedObject *pVlanMO = NULL);
	  static bool isClassifierPresentOnVlan(UI32 vlanId, DceContext *ctxt = NULL, VlanIntfManagedObject *pVlanMO = NULL);
	  static bool isClassifierPresentOnIntf(const string ifName, bool isGlobal);
	  static bool isPrivateVlanMode(string ifName);
	  static ObjectId getVlanObjectId(UI32 vlanId, DceContext *ctxt);
	  static UI32Range getSecPVlanRange (UI32Range msgVlanrange, UI32Range moVlanrange, PVlanMappingOper oper);
	  static UI32Range getSecPVlanRangeFromDb (UI32 primVId, DceContext *ctxt = NULL);
	  static bool doesSecPVlanRangeExceedMax (UI32 primVId, UI32Range msgVlanrange, DceContext *ctxt = NULL);
	  static bool isNativeVlanPresentInList (UI32Range vlanRange, UI32 nativeVlanId);
	  static UI32 getPVlanDomainCount(void);
	  static UI32 getVlanIdInPVlanTrunkAssociationCount(UI32 vlanId, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
	  static UI32 getVlanAsPrimaryInPVlanMappingCount(UI32 vlanId, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
  };
}

#endif


