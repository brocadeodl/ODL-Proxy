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

#ifndef QOS_UTILS_H
#define QOS_UTILS_H

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
  class QosUtils: public WaveObjectManagerToolKit
  {
    private:
    public:
      static ResourceId vcsQosSanity(SI32 oper, string mapName, bool apply, int cosMutationBitmap, int reservePriorityBitmap, int defaultCoS);
      static ResourceId vcsL3QosSanity(SI32 oper, string mapName, bool apply, int reservePriorityBitmap, SI32 markToValue);
      static bool checkMarkListCompatibilityWithCeeMap (vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector,
	      int reservePriorityBitmap);
	  static ResourceId getReservePriorityBitMapFromCeeMap(int &reservePriorityBitMap);
	  static ResourceId getQosMapObjectIdByMapName(string cosMapName, ObjectId &cosMapObjectId, QosMapType mapType,
		  PrismLinearSequencerContext *pPrismLinearSequencerContext);
	  static ResourceId getQosMapNameByObjectId(string &mapName,
			ObjectId mapObjectId, QosMapType mapType,
			PrismLinearSequencerContext *pPrismLinearSequencerContext);
	  static bool isInterfaceQosNonCeeConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	  static bool isInterfaceQosNonCeeConfigured(PoIntfManagedObject *pPoIntfManagedObject);
	  static bool isInterfaceQosMLSConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	  static bool isInterfaceQosMLSConfigured(PoIntfManagedObject *pPoIntfManagedObject);
	  static bool isInterfaceQosCeeConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	  static bool isInterfaceQosCeeConfigured(PoIntfManagedObject *pPoIntfManagedObject);
	  static bool isInterfaceFcoeConfigured (PoIntfManagedObject *pPoIntfManagedObject);
	  static bool isInterfaceFcoeConfigured (PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	  static bool isInterfaceMQCConfigured(PoIntfManagedObject *pPoIntfManagedObject);
	  static bool isInterfaceMQCConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
  	  static bool doesMapExist(QosMapType mapType, string mapName, ResourceId &status);
  	  static ResourceId isMacACLReferredByAnyClassMap(const string &policyMapName);
  	  static ResourceId isCeeMapSetToDefault();
  	  static ResourceId isAutoNasEnabled();

  };

	class PriorityGroupInfo 
	{
		public:
			string  pgId;
            SI32  weight;
            SI32  pfc;

            PriorityGroupInfo(string in_pgid, SI32 in_weight, SI32 in_pfc) {
				pgId = in_pgid;
				weight = in_weight;
				pfc = in_pfc;
			};
            PriorityGroupInfo() {
			};
    };

	class PriorityGroupCompare
	{
		public:
			bool operator()(const PriorityGroupInfo pg1, const PriorityGroupInfo pg2) const
			{
				if (pg1.pgId != pg2.pgId) {
					return ((double)atof(pg1.pgId.c_str()) < (double)atof(pg2.pgId.c_str())) ? true: false;
				} else if (pg1.weight != pg2.weight) {
					return (pg1.weight < pg2.weight ? true: false);
				} else if (pg1.pfc != pg2.pfc ){
					return (pg1.pfc < pg2.pfc? true: false);
				} else {
					return false;
				}
			};
	};
}

#endif  //QOS_UTILS_H
