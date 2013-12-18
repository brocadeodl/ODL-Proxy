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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef PHYINTFLOCALMANAGEDOBJECT_H
#define PHYINTFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObjectBase.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"
#include "Nsm/Local/NativeVlanIntfConfLocalManagedObject.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonStatisticsLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonHistoryLocalManagedObject.h"
#include "Nsm/Local/Ssm/NsmStormControlIntfConfLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

#include "Nsm/Local/NsmInterfaceVRFConfigManagedObjectBase.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class Dot1xPortConfigLocalManagedObject;
    class LldpIntfConfLocalManagedObject;
    class NativeVlanIntfConfLocalManagedObject;
	class RmonHistoryLocalManagedObject;
	class RmonStatisticsLocalManagedObject;
	class NsmStormControlIntfConfLocalManagedObject;
	
	class QosCosToTailDropManagedObject;
	class RspanMacAddressVlanClassificationMO;
	class RspanMacGroupVlanClassificationMO;
	class RspanGVlanTrunkPortManagedObject;
	class RspanGVlanTrunkPortRemoveManagedObject;

    class PhyIntfLocalManagedObject : virtual public PoPhyIntfManagedObject, virtual public DcmLocalManagedObjectBase, virtual public NsmInterfaceVRFConfigManagedObjectBase
	{    
		private:
		protected:
			virtual void setupAttributesForPersistence ();
			virtual void setupAttributesForCreate      ();
            virtual void setupKeys                     ();

		public:
            PhyIntfLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager);
            PhyIntfLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager,const string &id,const ObjectId &poId,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed, const WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> &lldpIntfConfig, const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig, const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &Portrole,const bool &openflow);

			PhyIntfLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager,const string &id,const ObjectId &poId,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed, const WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> &lldpIntfConfig, const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig, const vector<WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > &bumConfig, const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &Portrole,const bool &openflow);
			virtual    ~PhyIntfLocalManagedObject ();
			static string getClassName();
			void setId(const string &id);
			string getId()  const;
			void setPoId(const ObjectId &poId);
			ObjectId getPoId()  const;
			void setLacpTimeout(const LacpTimeout &lacpTimeout);
			LacpTimeout getLacpTimeout()  const;
			void setLacpPortPriority(const UI32 &lacpPortPriority);
			UI32 getLacpPortPriority()  const;
			void setLacpDefaultUp(const bool &lacpDefaultUp);
			bool getLacpDefaultUp()  const;
			void setLineSpeed(const LineSpeed &lineSpeed);
			LineSpeed getLineSpeed()  const;

			void setLldpIntfConfig(const WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> &lldpIntfConfig);
			WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> getLldpIntfConfig()  const;
			void setNativeVlanIntfConfig(const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig);
			WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> getNativeVlanIntfConfig()  const;
			void setPoMode(const UI32 &poMode);
			UI32 getPoMode()  const;
			void setPoType(const UI32 &poType);
			UI32 getPoType()  const;
			void setDestinationmirrorportEnable(const bool &enable);
			bool getDestinationmirrorportEnable();
			void setPhyType(InterfaceType type);
			InterfaceType getPhyType()  const;
			void setEnable(const bool &enable);
			bool getEnable()  const;
			void setVepa(const bool &enable);
			bool getVepa()  const;
			void setEld(const bool &eld);
			bool getEld()  const;
			void setEldPrio(const UI32 &eldprio);
			UI32 getEldPrio()  const;
			void setEldVlan(const UI32 &eldvlan);
			UI32 getEldVlan()  const;
			void setPollingInterval(const UI32 &pollingInterval);
			UI32 getPollingInterval()  const;
			void setSampleRate(const UI32 &sampleRate);
			UI32 getSampleRate()  const;
			void setIsPollingIntervalSet(const bool &isPollingIntervalSet);
			bool getIsPollingIntervalSet() const;
			void setPortrole(const bool &Portrole);
			bool getPortrole() const;
			void setIsSampleRateSet(const bool &isSampleRateSet);
			bool getIsSampleRateSet() const;
			void setIslCapability(const UI32 &islCapability);
			UI32 getIslCapability() const;
			void setDot1xIntfConfig(const WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> &Dot1xIntfConfig);
			void setPortSecConfg(const WaveManagedObjectPointer<PortSecurityLocalManagedObject> &PortSecConfg);
			WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> getDot1xIntfConfig();
			WaveManagedObjectPointer<PortSecurityLocalManagedObject>  getPortSecConfg();
			WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> getFcoeport() const;
			void setFcoeport(const WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>);
			void setVlanClassifierActivate(const vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> > &vlanClassifierActivate);
			vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> > getVlanClassifierActivate()  const;
			void addVlanClassifierActivate(VlanClassifierActivateLocalManagedObject *VlanClassifierActivateMO);
			int deleteVlanClassifierActivatedGroup(const UI32 &groupId);
			void deleteAllVlanClassifierActivatedGroup();

			void addRmonStats(RmonStatisticsLocalManagedObject *rmonStatsMO);
			SI32 deleteRmonStats(const SI32 &rmonStats);
			void addRmonHistory(RmonHistoryLocalManagedObject *rmonHistoryMO);
			SI32 deleteRmonHistory(const SI32 &rmonHistory);
			void setRmonStats(const vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > &rmonStats);
			vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > getRmonStats()  const;
			void setRmonHistory(const vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > &rmonHistory);
			vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > getRmonHistory()  const;

			void setCosTailDropConfig(const WaveManagedObjectPointer<QosCosToTailDropManagedObject> &cosToTailDrop);
            WaveManagedObjectPointer<QosCosToTailDropManagedObject> getCosTailDropConfig()  const;

			void setDistance(const LongDistance &distance);
			LongDistance getDistance()  const;
			void setVlanidList(const vector<UI32> &vlanidList);
			vector<UI32> getVlanidList()  const;
			vector<ObjectId> getVlanVector()  const;
			ResourceId addVlanObjectId(ObjectId &vlanOid);
			ResourceId deleteVlanObjectId(ObjectId &vlanOid);
			void setIfType(const UI32 &ifType);
			UI32 getIfType()  const;

            /* Openflow */
            void setOpenflow(const bool &openflow);
            bool getOpenflow() const;

            /* Storm Control */
			void setBumConfig(const vector<WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > &bumConfig);
            vector<WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > getBumConfig() const;

			void addBumControlManagedObjectToVector(NsmStormControlIntfConfLocalManagedObject *BumControlMO);
			int delBumControlManagedObjectFromVector(const UI32 &protocolType);
			// Now the data members

            // Udld
            bool getUdldEnable () const;
            void setUdldEnable (bool udldEnable);
            ObjectId getUdldGlobalConfigOid () const;
            void setUdldGlobalConfigOid (const ObjectId &oid);

            bool getNbrDiscoveryDisable () const;
            void setNbrDiscoveryDisable (const bool &discoveryFlag);
            //Rspan vlan range
            void setRspanVlanRangeAdd(const UI32Range &vlanRangeAdd);
            UI32Range getRspanVlanRangeAdd()  const;
            void setAccessRspanVlan(const UI32 &accessRspanVlan);
            UI32 getAccessRspanVlan()  const;
   			void deleteRspanMacAddressVlanClassificationAll();
   			void deleteRspanMacGroupVlanClassificationAll();
   			void deleteRspanCtagVlanClassificationAll();
			vector<WaveManagedObjectPointer<RspanMacAddressVlanClassificationMO> > getMacAddressRspanVlanClassification() const;
			vector<WaveManagedObjectPointer<RspanMacGroupVlanClassificationMO> > getMacGroupRspanVlanClassification() const;
			vector<WaveManagedObjectPointer<RspanGVlanTrunkPortManagedObject> > getCtagRspanVlanClassification() const;

		private:
		protected:
		public:
			string  m_id;
			ObjectId  m_poId;
			LacpTimeout  m_lacpTimeout;
			UI32 m_lacpPortPriority;
			bool m_lacpDefaultUp;
			LineSpeed  m_lineSpeed;
			WaveManagedObjectPointer<LldpIntfConfLocalManagedObject>  m_lldpIntfConfig;
			WaveManagedObjectPointer<QosCosToTailDropManagedObject> m_qosCosTailDropThresholdConfig;
			WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject>  m_nativeVlanIntfConfig;
			UI32  m_poType;
			UI32  m_poMode;
			bool m_destinationMirrorportEnable;
			bool m_enable;
			UI32 m_pollingInterval;
			UI32 m_sampleRate;
			UI32 m_eld_prio_val;
			UI32  m_ifType;
			UI32 m_eld_vlan;
			bool m_vepa;
			bool m_eld;
			vector<ObjectId>  m_vlanid;
			vector<UI32>  m_vlanidList;
//			UI32Range  m_vlanid;
			InterfaceType m_phy_type;

			WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> m_fcoeport;

			WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject>  m_Dot1xIntfConfig;
			WaveManagedObjectPointer<PortSecurityLocalManagedObject>  m_PortSecConfg;
			vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> >  m_vlanClassifierActivate;
			bool m_isPollingIntervalSet;
			bool m_isSampleRateSet;
			bool m_portrole;
			bool m_nbrDiscoveryDisable;
			LongDistance m_distance;
			UI32 m_islCapability;
			vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > m_rmonStats;
			vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > m_rmonHistory;

			vector<WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > m_bumConfig;

            // Udld
            ObjectId m_udldOid; // OID of UdldGlobalConfigManagedObject; required by UP
            bool     m_udldEnable;

            UI32Range  m_rspanVlanRangeAdd;
            UI32 m_accessRspanvlan;
            UI32Range  m_rspanVlanRangeRemove;

			vector<WaveManagedObjectPointer<RspanMacAddressVlanClassificationMO> > m_rspanMacAddrVlanClassification;
			vector<WaveManagedObjectPointer<RspanMacGroupVlanClassificationMO> > m_rspanMacGroupVlanClassification;
			vector<WaveManagedObjectPointer<RspanGVlanTrunkPortManagedObject> > m_rspanGvlanTrunk;
			vector<WaveManagedObjectPointer<RspanGVlanTrunkPortRemoveManagedObject> > m_rspanGvlanTrunkRemove;
           /* Openflow */
           bool m_openflow;
	};
}
#endif                                            //PHYINTFLOCALMANAGEDOBJECT_H

