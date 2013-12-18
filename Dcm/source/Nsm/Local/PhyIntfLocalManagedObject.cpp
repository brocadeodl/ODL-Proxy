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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/NativeVlanIntfConfLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Nsm/Local/Ssm/NsmStormControlIntfConfLocalManagedObject.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Local/Rmon/RmonStatisticsLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonHistoryLocalManagedObject.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/NsmInterfaceVRFConfigManagedObjectBase.h"
#include "Udld/Global/UdldGlobalConfigManagedObject.h"

#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"

#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include <algorithm>

namespace DcmNs
{

    PhyIntfLocalManagedObject::PhyIntfLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager)
        :PrismElement  (pWaveObjectManager),
        PrismPersistableObject (PhyIntfLocalManagedObject::getClassName (), PoPhyIntfManagedObject::getClassName ()),
        WaveManagedObject          (pWaveObjectManager),
        DcmManagedObject           (pWaveObjectManager),
        IntfManagedObject          (pWaveObjectManager),
        PoPhyIntfManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase  (this),
        CommonManagedObjectBase (this),
        NsmInterfaceVRFConfigManagedObjectBase (this),
		m_destinationMirrorportEnable 	(false),	
        m_enable                   (SFLOW_DEFAULT_ENABLE),
        m_pollingInterval          (SFLOW_OUT_OF_RANGE_VALUE),
        m_sampleRate               (SFLOW_OUT_OF_RANGE_VALUE),
        m_isPollingIntervalSet     (SFLOW_DEFAULT_ENABLE),
        m_isSampleRateSet          (SFLOW_DEFAULT_ENABLE),
		m_portrole                 (false),
        m_udldEnable               (false)
        {
			m_vepa = false;
			m_nbrDiscoveryDisable = false;
			m_lacpTimeout = LACP_TIMEOUT_DEFAULT;
			m_lacpPortPriority = DCM_LACP_DEFAULT_PORT_PRIORITY;
			m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
			m_lineSpeed = LINE_SPEED_AUTO;
			m_poMode = 0;
			m_poType = 0;
			m_islCapability = 0;
			m_phy_type = IF_TYPE_TE;
			m_eld_prio_val = 128;
            m_vlanidList.clear();
            m_vlanid.clear();
			m_ifType = IF_TYPE_INVALID;
			m_accessRspanvlan = 1;
			m_rspanMacAddrVlanClassification.clear();
			m_rspanMacGroupVlanClassification.clear();
            m_openflow = NSM_OPENFLOW_DEF_STATUS;
        }

    PhyIntfLocalManagedObject::PhyIntfLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager,const string &id,const ObjectId &poId,const LacpTimeout &lacpTimeout,const UI32 &lacpPortPriority, const LineSpeed &lineSpeed,const WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> &lldpIntfConfig, const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig, const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &Portrole,const bool &openflow)
		:PrismElement  (pWaveObjectManager),
		PrismPersistableObject (PhyIntfLocalManagedObject::getClassName (), PoPhyIntfManagedObject::getClassName ()),
		WaveManagedObject          (pWaveObjectManager),
		DcmManagedObject           (pWaveObjectManager),
		IntfManagedObject          (pWaveObjectManager),
		PoPhyIntfManagedObject (pWaveObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase  (this),
        CommonManagedObjectBase (this),
        NsmInterfaceVRFConfigManagedObjectBase (this),
		m_id    (id),
		m_poId    (poId),
		m_lacpTimeout    (lacpTimeout),
		m_lacpPortPriority  (lacpPortPriority),
		m_lineSpeed    (lineSpeed),
		m_lldpIntfConfig    (lldpIntfConfig),
		m_nativeVlanIntfConfig    (nativeVlanIntfConfig),
		m_destinationMirrorportEnable   (false),
		m_sampleRate    (sampleRate),
        m_isPollingIntervalSet     (SFLOW_DEFAULT_ENABLE),
        m_isSampleRateSet          (SFLOW_DEFAULT_ENABLE),
        m_portrole (Portrole),
        m_udldEnable (false),
        m_openflow (openflow)
    {
			m_nbrDiscoveryDisable = false;
			m_phy_type = IF_TYPE_TE;
			m_eld_prio_val = 128;
            m_vlanidList.clear();
            m_vlanid.clear();
			m_ifType = IF_TYPE_INVALID;
			m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
			m_accessRspanvlan = 1;
			m_rspanMacAddrVlanClassification.clear();
			m_rspanMacGroupVlanClassification.clear();
	}

    PhyIntfLocalManagedObject::~PhyIntfLocalManagedObject ()
    {
    }

    string  PhyIntfLocalManagedObject::getClassName()
    {
        return ("PhyIntfLocalManagedObject");
    }

    void  PhyIntfLocalManagedObject::setupAttributesForPersistence()
    {
		UI32 eld_prio_default = 128;
		UI32 distanceDefault = (UI32)LONG_DISTANCE_200;
	  	UI32 poModeTypeDefault = 0;
	  	UI32 lacpTimeoutDefault = LACP_TIMEOUT_DEFAULT;
		UI32 lacpPortPriorityDefault = DCM_LACP_DEFAULT_PORT_PRIORITY;
		UI32 sflowDefault = SFLOW_OUT_OF_RANGE_VALUE;
		const UI32Range defaultData("");
		UI32 rspan_access_vlan_default = 1;

        DcmLocalManagedObjectBase::setupAttributesForPersistence();
		PoPhyIntfManagedObject::setupAttributesForPersistence();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeString(&m_id,"id"));
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_poId,"poId", PoIntfManagedObject::getClassName (),true));

		addPersistableAttribute (new AttributeEnum((UI32 * )(&m_lacpTimeout),lacpTimeoutDefault,"lacpTimeout",lacp_timeout));
	 	addPersistableAttribute (new AttributeUI32((UI32 *)(&m_lacpPortPriority),lacpPortPriorityDefault,"lacpPortPriority",lacp_std_port_priority));
	 	addPersistableAttribute (new AttributeBool((bool * )(&m_lacpDefaultUp),"lacpDefaultUp",lacp_default_up));

	 	addPersistableAttribute (new AttributeEnum((UI32 * )(&m_lineSpeed),"lineSpeed",brocade_interface_speed));
		addPersistableAttribute (new AttributeManagedObjectComposition<LldpIntfConfLocalManagedObject>(&m_lldpIntfConfig,"lldpIntfConfig", LldpIntfConfLocalManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectComposition<QosCosToTailDropManagedObject>(&m_qosCosTailDropThresholdConfig, "qosCosTailDropThresholdConfig", QosCosToTailDropManagedObject::getClassName (), true));
		addPersistableAttribute (new AttributeManagedObjectComposition<NativeVlanIntfConfLocalManagedObject>(&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", NativeVlanIntfConfLocalManagedObject::getClassName (), true));
		addPersistableAttribute (new AttributeManagedObjectComposition<PortSecurityLocalManagedObject>(&m_PortSecConfg,"portSecurity", PortSecurityLocalManagedObject::getClassName (), true));
		
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_poMode), poModeTypeDefault, "poMode",brocade_interface_mode));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_poType), poModeTypeDefault, "poType",brocade_interface_type));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_phy_type), "phyType"));
		addPersistableAttribute (new AttributeBool((bool * )(&m_enable),"enable",sflow_enable));
		addPersistableAttribute (new AttributeBool((bool * )(&m_vepa),"vepa_enable",brocade_interface_vepa_enable));
		addPersistableAttribute (new AttributeUI32((UI32 * )(&m_pollingInterval), sflowDefault, "pollingInterval",sflow_polling_interval));
		addPersistableAttribute (new AttributeUI32((UI32 * )(&m_sampleRate), sflowDefault, "sampleRate",sflow_sample_rate));
		addPersistableAttribute (new AttributeManagedObjectComposition<FcoeportAttrLocalManagedObject>(&m_fcoeport, "fcoeport", FcoeportAttrLocalManagedObject::getClassName(), true));
		addPersistableAttribute (new AttributeManagedObjectComposition<Dot1xPortConfigLocalManagedObject>(&m_Dot1xIntfConfig,"Dot1xIntfConfig", Dot1xPortConfigLocalManagedObject::getClassName (), true));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<VlanClassifierActivateLocalManagedObject>(&m_vlanClassifierActivate,"vlanClassifierActivate", getClassName (), getObjectId (), VlanClassifierActivateLocalManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RmonStatisticsLocalManagedObject>(&m_rmonStats,"rmonStats", getClassName (), getObjectId (), RmonStatisticsLocalManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RmonHistoryLocalManagedObject>(&m_rmonHistory,"rmonHistory", getClassName (), getObjectId (), RmonHistoryLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeUI32((UI32 * )(&m_ifType), "ifType"));

		setUserTagForAttribute ("id", brocade_interface_name);
		setUserTagForAttribute ("poId", brocade_interface_port_int);
		addPersistableAttribute (new AttributeBool((bool * )(&m_isPollingIntervalSet),"isPollingIntervalSet"));
		addPersistableAttribute (new AttributeBool((bool * )(&m_isSampleRateSet),"isSampleRateSet"));
		addPersistableAttribute (new AttributeBool((bool * )(&m_portrole),"Portrole"));
		addPersistableAttribute (new AttributeBool((bool * )(&m_nbrDiscoveryDisable), "nbrDiscoveryDisable", fcoe_disable));
        addPersistableAttribute (new AttributeUI32((UI32 * )(&m_islCapability), "islCapability"));
		addPersistableAttribute (new AttributeEnum((UI32 * )(&m_distance),distanceDefault, "Distance",brocade_interface_long_distance_isl));
        /* Eld */
		addPersistableAttribute (new AttributeUI32((UI32 * )(&m_eld_prio_val), eld_prio_default, "eldprio",brocade_interface_eldprio));
        addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_vlanid,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));

        setUserTagForAttribute("vlanid", brocade_interface_eldvlan);
        /* Storm Control */
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<NsmStormControlIntfConfLocalManagedObject>(&m_bumConfig, "bumConfig",  getClassName (), getObjectId (), NsmStormControlIntfConfLocalManagedObject::getClassName()));
		/* Span Destination port enabled flag */
		addPersistableAttribute (new AttributeBool((bool * )(&m_destinationMirrorportEnable), "isdestinationMirrorportEnable"));

        // Udld
        addPersistableAttribute(new AttributeBool(&m_udldEnable, "udldEnable", udld_udld_enable));
        addPersistableAttribute(new AttributeObjectIdAssociation(&m_udldOid, "udldGlobalAssoc", UdldGlobalConfigManagedObject::getClassName(), true));

        //Rspan
        addPersistableAttribute (new AttributeUI32Range(&m_rspanVlanRangeAdd,defaultData,"rspan_trunkvlanrange",brocade_interface_add_rspan_trunk_vlan));
        addPersistableAttribute (new AttributeUI32Range(&m_rspanVlanRangeRemove,defaultData,"rspan_trunkvlanrangeremove",brocade_interface_remove_rspan_trunk_vlan));
        addPersistableAttribute (new AttributeUI32((UI32 * )(&m_accessRspanvlan),rspan_access_vlan_default,"rspan_accessvlan",brocade_interface_rspan_access_vlan));

		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RspanMacAddressVlanClassificationMO>(&m_rspanMacAddrVlanClassification, "rspan_macaddress_vlan_classififcation", getClassName (), getObjectId (), RspanMacAddressVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RspanMacGroupVlanClassificationMO>(&m_rspanMacGroupVlanClassification, "rspan_macgroup_vlan_classififcation", getClassName (), getObjectId (), RspanMacGroupVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RspanGVlanTrunkPortManagedObject>(&m_rspanGvlanTrunk, "rspan_gvlan_trunk", getClassName (), getObjectId (), RspanGVlanTrunkPortManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<RspanGVlanTrunkPortRemoveManagedObject>(&m_rspanGvlanTrunkRemove, "rspan_gvlan_trunk_remove", getClassName (), getObjectId (), RspanGVlanTrunkPortRemoveManagedObject::getClassName ()));


       /* Openflow */
        addPersistableAttribute (new AttributeBool(&m_openflow,"openflow_enable",brocade_interface_openflow_enable));

	}

    void  PhyIntfLocalManagedObject::setupAttributesForCreate()
    {
		UI32 eld_prio_default = 128;
	  	UI32 poModeTypeDefault = 0;
	  	UI32 lacpTimeoutDefault = LACP_TIMEOUT_DEFAULT;
		UI32 lacpPortPriorityDefault = DCM_LACP_DEFAULT_PORT_PRIORITY;
		UI32 sflowDefault = SFLOW_OUT_OF_RANGE_VALUE;
		UI32 distanceDefault = (UI32)LONG_DISTANCE_200;
		UI32 rspan_access_vlan_default = 1;

        DcmLocalManagedObjectBase::setupAttributesForCreate();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForCreate();
		addPersistableAttributeForCreate  (new AttributeString(&m_id,"id"));
		addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_poId,"poId", PoIntfManagedObject::getClassName (),true));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 *) (&m_lacpTimeout),lacpTimeoutDefault,"lacpTimeout",lacp_timeout));
		addPersistableAttributeForCreate (new AttributeUI32((UI32 *) (&m_lacpPortPriority),lacpPortPriorityDefault,"lacpPortPriority",lacp_std_port_priority));
		addPersistableAttributeForCreate (new AttributeBool((bool * )(&m_lacpDefaultUp),"lacpDefaultUp",lacp_default_up));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 *) (&m_lineSpeed),"lineSpeed",brocade_interface_speed));
		addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<LldpIntfConfLocalManagedObject>(&m_lldpIntfConfig,"lldpIntfConfig", LldpIntfConfLocalManagedObject::getClassName ()));
		addPersistableAttributeForCreate (new AttributeManagedObjectComposition<QosCosToTailDropManagedObject>(&m_qosCosTailDropThresholdConfig, "qosCosTailDropThresholdConfig", QosCosToTailDropManagedObject::getClassName (), true));
		addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<NativeVlanIntfConfLocalManagedObject>(&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", NativeVlanIntfConfLocalManagedObject::getClassName (), true));
		addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<PortSecurityLocalManagedObject>(&m_PortSecConfg,"portSecurity", PortSecurityLocalManagedObject::getClassName (), true));
	
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_poMode), poModeTypeDefault, "poMode",brocade_interface_mode));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_poType), poModeTypeDefault, "poType",brocade_interface_type));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_phy_type), "phyType"));
		addPersistableAttributeForCreate (new AttributeBool((bool *) (&m_enable),"enable",sflow_enable));
		addPersistableAttributeForCreate (new AttributeBool((bool *) (&m_vepa),"vepa_enable",brocade_interface_vepa_enable));
		addPersistableAttributeForCreate (new AttributeUI32((UI32 *) (&m_pollingInterval), sflowDefault, "pollingInterval",sflow_polling_interval));
		addPersistableAttributeForCreate (new AttributeUI32((UI32 *) (&m_sampleRate), sflowDefault, "sampleRate",sflow_sample_rate));
		addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<Dot1xPortConfigLocalManagedObject>(&m_Dot1xIntfConfig,"Dot1xIntfConfig", Dot1xPortConfigLocalManagedObject::getClassName (), true));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VlanClassifierActivateLocalManagedObject>(&m_vlanClassifierActivate,"vlanClassifierActivate", getClassName (), getObjectId (), VlanClassifierActivateLocalManagedObject::getClassName ()));


		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<RmonStatisticsLocalManagedObject>(&m_rmonStats,"rmonStats", getClassName (), getObjectId (), RmonStatisticsLocalManagedObject::getClassName ()));
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<RmonHistoryLocalManagedObject>(&m_rmonHistory,"rmonHistory", getClassName (), getObjectId (), RmonHistoryLocalManagedObject::getClassName ()));
		addPersistableAttributeForCreate (new AttributeManagedObjectComposition<FcoeportAttrLocalManagedObject>(&m_fcoeport, "fcoeport", FcoeportAttrLocalManagedObject::getClassName(), true));

		addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_isPollingIntervalSet),"isPollingIntervalSet"));
		addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_isSampleRateSet),"isSampleRateSet"));
		addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_portrole),"Portrole"));
		addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_nbrDiscoveryDisable), "nbrDiscoveryDisable", fcoe_disable));
		addPersistableAttributeForCreate(new AttributeUI32((UI32 * )(&m_islCapability),"islCapability"));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 * )(&m_distance), distanceDefault, "Distance",brocade_interface_long_distance_isl));
		addPersistableAttributeForCreate(new AttributeUI32((UI32 * )(&m_ifType),"ifType"));

        /* Eld */
		addPersistableAttributeForCreate (new AttributeUI32((UI32 *) (&m_eld_prio_val), eld_prio_default, "eldprio",brocade_interface_eldprio));
        addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_vlanid,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));
        /* Storm Control */
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NsmStormControlIntfConfLocalManagedObject>(&m_bumConfig, "bumConfig", getClassName (), getObjectId (), NsmStormControlIntfConfLocalManagedObject::getClassName()));
		/* Span Destination port enabled flag */
		addPersistableAttributeForCreate (new AttributeBool((bool * )(&m_destinationMirrorportEnable), "isdestinationMirrorportEnable"));

        // Udld
        addPersistableAttributeForCreate(new AttributeBool(&m_udldEnable, "udldEnable", udld_udld_enable));
        addPersistableAttributeForCreate(new AttributeObjectIdAssociation(&m_udldOid, "udldGlobalAssoc", UdldGlobalConfigManagedObject::getClassName(), true));

        //Rspan
        addPersistableAttributeForCreate (new AttributeUI32Range(&m_rspanVlanRangeAdd,"rspan_trunkvlanrange",brocade_interface_add_rspan_trunk_vlan));
        addPersistableAttributeForCreate (new AttributeUI32Range(&m_rspanVlanRangeRemove,"rspan_trunkvlanrangeremove",brocade_interface_remove_rspan_trunk_vlan));
        addPersistableAttributeForCreate (new AttributeUI32((UI32 *)(&m_accessRspanvlan),rspan_access_vlan_default,"rspan_accessvlan",brocade_interface_rspan_access_vlan));

        //Rspan Gvlan
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<RspanMacAddressVlanClassificationMO>(&m_rspanMacAddrVlanClassification, "rspan_macaddress_vlan_classififcation", getClassName (), getObjectId (), RspanMacAddressVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<RspanMacGroupVlanClassificationMO>(&m_rspanMacGroupVlanClassification, "rspan_macgroup_vlan_classififcation", getClassName (), getObjectId (), RspanMacGroupVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<RspanGVlanTrunkPortManagedObject>(&m_rspanGvlanTrunk, "rspan_gvlan_trunk", getClassName (), getObjectId (), RspanGVlanTrunkPortManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<RspanGVlanTrunkPortRemoveManagedObject>(&m_rspanGvlanTrunkRemove, "rspan_gvlan_trunk_remove", getClassName (), getObjectId (), RspanGVlanTrunkPortRemoveManagedObject::getClassName ()));


       /* Openflow */
       addPersistableAttributeForCreate  (new AttributeBool(&m_openflow,"openflow_enable",brocade_interface_openflow_enable));

    }

    void PhyIntfLocalManagedObject::setupKeys ()
    {
		vector<string> userDefinedKeyCombination;

		userDefinedKeyCombination.push_back ("id");

		setUserDefinedKeyCombination (userDefinedKeyCombination);        
    }

    static bool profileVlanSort(ObjectId obj1, ObjectId obj2)
    {
        return obj1 < obj2;
    }

    ResourceId PhyIntfLocalManagedObject::addVlanObjectId(ObjectId &vlanOid)
    {

        vector<ObjectId>::iterator vlanid;

        if (find(m_vlanid.begin(), m_vlanid.end(), vlanOid) != m_vlanid.end() ) {
            trace (TRACE_LEVEL_ERROR, "InstanceBaseManagedObject::addVlanObjectId: Vlan is already member of this instance  \n");
            return WAVE_MESSAGE_ERROR;
        }
            trace (TRACE_LEVEL_INFO, "InstanceBaseManagedObject::addVlanObjectId\n");

        m_vlanid.push_back(vlanOid);
//      std::sort(m_vlanid.begin(), m_vlanid.end());

        stable_sort(m_vlanid.begin(), m_vlanid.end(), profileVlanSort);
        return WAVE_MESSAGE_SUCCESS;
    }
    ResourceId PhyIntfLocalManagedObject::deleteVlanObjectId(ObjectId &vlanOid)
    {
        vector<ObjectId>::iterator result;
        if (vlanOid == ObjectId()) {
            m_vlanid.clear();
            trace (TRACE_LEVEL_DEBUG, "InstanceBaseManagedObject::deleteVlanObjectId: vlanOid == ObjectId() \n");
            return WAVE_MESSAGE_SUCCESS;
        }

        if (find(m_vlanid.begin(), m_vlanid.end(), vlanOid) == m_vlanid.end() ) {
            trace (TRACE_LEVEL_ERROR, "InstanceBaseManagedObject::deleteVlanObjectId: Vlan is not present for this instance \n");
            return WAVE_MESSAGE_ERROR;
        }

        if ((result = find(m_vlanid.begin(), m_vlanid.end(), vlanOid)) != m_vlanid.end()) {
            m_vlanid.erase(result);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void  PhyIntfLocalManagedObject::setEldPrio(const UI32 &eldprio)
    {
        m_eld_prio_val = eldprio;
    }

    UI32  PhyIntfLocalManagedObject::getEldPrio() const
    {
        return (m_eld_prio_val);
    }

    void  PhyIntfLocalManagedObject::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

    UI32 PhyIntfLocalManagedObject::getIfType() const
    {
        return (m_ifType);
    }

    void  PhyIntfLocalManagedObject::setVlanidList(const vector<UI32> &vlanidList)
    {
		unsigned int i = 0;
		m_vlanidList  =  vlanidList;
		for (i = 0; i < m_vlanidList.size(); i++)
		{
			trace (TRACE_LEVEL_INFO, string ("\t setVlanidList(): i = ") + i + string("VLAN_ID = ") + m_vlanidList[i]);
			printf("setVlanidList():i = %d, VLAN_ID IS %d\n", i, m_vlanidList[i]);
		}
    }

    vector<ObjectId>  PhyIntfLocalManagedObject::getVlanVector() const
    {
        return (m_vlanid);
    }

    vector<UI32>  PhyIntfLocalManagedObject::getVlanidList() const
    {
        return (m_vlanidList);
    }
	
    void  PhyIntfLocalManagedObject::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PhyIntfLocalManagedObject::getId() const
    {
        return (m_id);
    }

    void  PhyIntfLocalManagedObject::setPoId(const ObjectId &poId)
    {
        m_poId  =  poId;
    }

    ObjectId  PhyIntfLocalManagedObject::getPoId() const
    {
        return (m_poId);
    }

    void  PhyIntfLocalManagedObject::setLacpTimeout(const LacpTimeout &lacpTimeout)
    {
        m_lacpTimeout  =  lacpTimeout;
    }

    void  PhyIntfLocalManagedObject::setLacpDefaultUp(const bool &lacpDefaultUp)
    {
        m_lacpDefaultUp =  lacpDefaultUp;
    }

    bool PhyIntfLocalManagedObject::getLacpDefaultUp() const
    {
        return (m_lacpDefaultUp);
    }

    LacpTimeout  PhyIntfLocalManagedObject::getLacpTimeout() const
    {
        return (m_lacpTimeout);
    }

	void  PhyIntfLocalManagedObject::setLacpPortPriority(const UI32 &lacpPortPriority)
	{
		m_lacpPortPriority =  lacpPortPriority;
	}

	UI32 PhyIntfLocalManagedObject::getLacpPortPriority() const
	{
		return (m_lacpPortPriority);
	}
    
	void  PhyIntfLocalManagedObject::setLineSpeed (const LineSpeed &lineSpeed)
    {
        m_lineSpeed  =  lineSpeed;
    }

    LineSpeed  PhyIntfLocalManagedObject::getLineSpeed() const
    {
        return (m_lineSpeed);
    }

    void  PhyIntfLocalManagedObject::setLldpIntfConfig(const WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> &lldpIntfConfig)
    {
        m_lldpIntfConfig  =  lldpIntfConfig;
    }

    WaveManagedObjectPointer<LldpIntfConfLocalManagedObject>  PhyIntfLocalManagedObject::getLldpIntfConfig() const
    {
        return (m_lldpIntfConfig);
    }
 
	void PhyIntfLocalManagedObject::setCosTailDropConfig(const WaveManagedObjectPointer<QosCosToTailDropManagedObject>
			&qosCosTailDropThresholdConfig)
	{
		m_qosCosTailDropThresholdConfig = qosCosTailDropThresholdConfig;
	}

	WaveManagedObjectPointer<QosCosToTailDropManagedObject> PhyIntfLocalManagedObject::getCosTailDropConfig() const
	{
		return (m_qosCosTailDropThresholdConfig);
	}
 
	void  PhyIntfLocalManagedObject::setNativeVlanIntfConfig(const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig)
    {
        m_nativeVlanIntfConfig  =  nativeVlanIntfConfig;
    }

    WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject>  PhyIntfLocalManagedObject::getNativeVlanIntfConfig() const
    {
        return (m_nativeVlanIntfConfig);
    }

    void  PhyIntfLocalManagedObject::setPoMode(const UI32 &poMode)
    {
        m_poMode = poMode;
    }

    UI32  PhyIntfLocalManagedObject::getPoMode() const
    {
        return (m_poMode);
    }

	void PhyIntfLocalManagedObject::setDestinationmirrorportEnable(const bool &enable) {
		m_destinationMirrorportEnable = enable;
	}
	 
    bool PhyIntfLocalManagedObject::getDestinationmirrorportEnable() {
		return (m_destinationMirrorportEnable);
	}

    void  PhyIntfLocalManagedObject::setPoType(const UI32 &poType)
    {
        m_poType = poType;
    }

    UI32  PhyIntfLocalManagedObject::getPoType() const
    {
        return (m_poType);
    }

    void  PhyIntfLocalManagedObject::setPhyType(InterfaceType type)
        {
            m_phy_type = type;
        }

    InterfaceType  PhyIntfLocalManagedObject::getPhyType() const
    {
            return (m_phy_type);
    }

    void  PhyIntfLocalManagedObject::setEnable(const bool &enable)
    {
        m_enable = enable ;
    }

    bool PhyIntfLocalManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  PhyIntfLocalManagedObject::setVepa(const bool &enable)
    {
        m_vepa = enable ;
    }

    bool PhyIntfLocalManagedObject::getVepa() const
    {
        return (m_vepa);
    }

    void  PhyIntfLocalManagedObject::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval = pollingInterval;
    }

    UI32 PhyIntfLocalManagedObject::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  PhyIntfLocalManagedObject::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate = sampleRate ;
    }

    UI32 PhyIntfLocalManagedObject::getSampleRate() const
    {
    return (m_sampleRate);
    }

    void  PhyIntfLocalManagedObject::setIsPollingIntervalSet(const bool &isPollingIntervalSet)
    {
        m_isPollingIntervalSet = isPollingIntervalSet;
    }

    bool PhyIntfLocalManagedObject::getIsPollingIntervalSet() const
    {
        return (m_isPollingIntervalSet);
    }

    void  PhyIntfLocalManagedObject::setIsSampleRateSet(const bool &isSampleRateSet)
    {
        m_isSampleRateSet = isSampleRateSet;
    }

    void  PhyIntfLocalManagedObject::setPortrole(const bool &Portrole)
    {
        m_portrole = Portrole;
    }

    bool PhyIntfLocalManagedObject::getPortrole() const
    {
        return (m_portrole);
    }

    void  PhyIntfLocalManagedObject::setNbrDiscoveryDisable (const bool &discoveryFlag)
    {
        m_nbrDiscoveryDisable = discoveryFlag;
    }

    bool PhyIntfLocalManagedObject::getNbrDiscoveryDisable () const
    {
        return m_nbrDiscoveryDisable;
    }

    bool PhyIntfLocalManagedObject::getIsSampleRateSet() const
    {
        return (m_isSampleRateSet);
    }

    void  PhyIntfLocalManagedObject::setIslCapability(const UI32 &islCapability)
    {
        m_islCapability = islCapability;
    }

    UI32 PhyIntfLocalManagedObject::getIslCapability() const
    {
        return (m_islCapability);
    }

	void  PhyIntfLocalManagedObject::setRmonStats(const vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> > &rmonStats)
	{
		m_rmonStats  =  rmonStats;
	}

	vector<WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> >  PhyIntfLocalManagedObject::getRmonStats() const
	{
		return (m_rmonStats);
	}
	
	void  PhyIntfLocalManagedObject::setRmonHistory(const vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > &rmonHistory)
	{
		m_rmonHistory  =  rmonHistory;
	}

	vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> >  PhyIntfLocalManagedObject::getRmonHistory() const
	{
		return (m_rmonHistory);
	}
	
	void  PhyIntfLocalManagedObject::setPortSecConfg(const WaveManagedObjectPointer<PortSecurityLocalManagedObject> &PortSecConfg)
	{
		m_PortSecConfg  =  PortSecConfg;
	}

	WaveManagedObjectPointer<PortSecurityLocalManagedObject>  PhyIntfLocalManagedObject::getPortSecConfg()
	{
		return (m_PortSecConfg);
	}

	
    void  PhyIntfLocalManagedObject::setDot1xIntfConfig(const WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> &Dot1xIntfConfig)
    {
        m_Dot1xIntfConfig  =  Dot1xIntfConfig;
    }

    WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject>  PhyIntfLocalManagedObject::getDot1xIntfConfig()
    {
        return (m_Dot1xIntfConfig);
    }

	void  PhyIntfLocalManagedObject::setDistance (const LongDistance &distance)
    {
        m_distance  =  distance;
    }

    LongDistance  PhyIntfLocalManagedObject::getDistance() const
    {
        return (m_distance);
    }

     void  PhyIntfLocalManagedObject::setVlanClassifierActivate(const vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> > &vlanClassifierActivate)
    {
        m_vlanClassifierActivate  =  vlanClassifierActivate;
    }

    vector<WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject> >  PhyIntfLocalManagedObject::getVlanClassifierActivate() const
    {
        return (m_vlanClassifierActivate);
    }

    void PhyIntfLocalManagedObject::addVlanClassifierActivate(VlanClassifierActivateLocalManagedObject *VlanClassifierActivateMO)
    {
        WaveManagedObjectPointer<VlanClassifierActivateLocalManagedObject > VlanClassifierActivate (VlanClassifierActivateMO);
        m_vlanClassifierActivate.push_back(VlanClassifierActivate);

    }
    int PhyIntfLocalManagedObject::deleteVlanClassifierActivatedGroup(const UI32 &groupId)
    {
		NSM_PLUG_DBG("Entering ...");
		size_t vecSize = m_vlanClassifierActivate.size();
        unsigned int i;
        for (i = 0; i < vecSize; i++) {
            if ((m_vlanClassifierActivate[i]->getGroupId()) == groupId) {
				NSM_PLUG_DBG_S(string("vlan classifier provisioning of group ") + m_vlanClassifierActivate[i]->getGroupId() + string(" deleted"));
                m_vlanClassifierActivate.erase(m_vlanClassifierActivate.begin() + i);
                return (0);
            }
        }
        return (0);
    }
	
	void PhyIntfLocalManagedObject::deleteAllVlanClassifierActivatedGroup()
	{
		m_vlanClassifierActivate.clear();
	}

    void PhyIntfLocalManagedObject::setFcoeport(const WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> fcoeport)
    {
        m_fcoeport = fcoeport;
    }

    WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> PhyIntfLocalManagedObject::getFcoeport() const
    {
        return m_fcoeport;
    }

    void PhyIntfLocalManagedObject::setBumConfig(const vector <WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > &bumConfig)
    {
        m_bumConfig = bumConfig;
    }

    vector <WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> >PhyIntfLocalManagedObject::getBumConfig() const
    {
        return(m_bumConfig);
    }

	void PhyIntfLocalManagedObject::addBumControlManagedObjectToVector(NsmStormControlIntfConfLocalManagedObject *BumControlMO)
	{
        WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> bumControlMO(BumControlMO);
		m_bumConfig.push_back(bumControlMO);
	}

	int PhyIntfLocalManagedObject::delBumControlManagedObjectFromVector(const UI32 &protocolType)
	{
		unsigned int i;
		for (i = 0; i < m_bumConfig.size(); i++)
		{
			if (m_bumConfig[i]->getProtocolType() == protocolType)
			{
				m_bumConfig.erase(m_bumConfig.begin() + i);
				return (1);
			}
		}
		return 0;
	}
			
     
	void PhyIntfLocalManagedObject::addRmonStats(RmonStatisticsLocalManagedObject *rmonStatsMO)
    {
        WaveManagedObjectPointer<RmonStatisticsLocalManagedObject> statsEntry (rmonStatsMO);
        m_rmonStats.push_back(statsEntry);
    }

    SI32 PhyIntfLocalManagedObject::deleteRmonStats(const SI32 &rmonStatsIndex)
    {
        unsigned int i;

        for (i = 0; i < m_rmonStats.size(); i++) {
            if ( m_rmonStats[i]->getStatsIndex() == rmonStatsIndex){
                trace(TRACE_LEVEL_DEBUG, string("Rmon stats deleted:") + rmonStatsIndex);
                m_rmonStats.erase(m_rmonStats.begin() + i);
                return (0);
            }
        }
        return (1);
    }

	void PhyIntfLocalManagedObject::addRmonHistory(RmonHistoryLocalManagedObject *rmonHistoryMO)
    {
        WaveManagedObjectPointer<RmonHistoryLocalManagedObject> historyEntry (rmonHistoryMO);
        m_rmonHistory.push_back(historyEntry);
    }

    SI32 PhyIntfLocalManagedObject::deleteRmonHistory(const SI32 &rmonHistoryIndex)
    {
        unsigned int i;

        for (i = 0; i < m_rmonHistory.size(); i++) {
            if ( m_rmonHistory[i]->getHistoryIndex() == rmonHistoryIndex){
                trace(TRACE_LEVEL_DEBUG, string("Rmon history deleted:") + rmonHistoryIndex);
                m_rmonHistory.erase(m_rmonHistory.begin() + i);
                return (0);
            }
        }
        return (1);
    }


    bool PhyIntfLocalManagedObject::getUdldEnable () const
    {
        return m_udldEnable;
    }

    void PhyIntfLocalManagedObject::setUdldEnable (bool udldEnable)
    {
        m_udldEnable = udldEnable;
    }

    ObjectId PhyIntfLocalManagedObject::getUdldGlobalConfigOid () const
    {
        return m_udldOid;
    }

    void PhyIntfLocalManagedObject::setUdldGlobalConfigOid (const ObjectId &oid)
    {
        m_udldOid = oid;
    }

    void  PhyIntfLocalManagedObject::setRspanVlanRangeAdd(const UI32Range &vlanRangeAdd)
    {
    	m_rspanVlanRangeAdd  =  vlanRangeAdd;
    }

    UI32Range  PhyIntfLocalManagedObject::getRspanVlanRangeAdd() const
    {
        return (m_rspanVlanRangeAdd);
    }

    void  PhyIntfLocalManagedObject::setAccessRspanVlan(const UI32 &accessRspanVlan)
    {
    	m_accessRspanvlan  =  accessRspanVlan;
    }

    UI32 PhyIntfLocalManagedObject::getAccessRspanVlan() const
    {
        return (m_accessRspanvlan);
    }

    void PhyIntfLocalManagedObject::deleteRspanMacAddressVlanClassificationAll()
    {
    	m_rspanMacAddrVlanClassification.clear();
    }

    void PhyIntfLocalManagedObject::deleteRspanMacGroupVlanClassificationAll()
    {
 		m_rspanMacGroupVlanClassification.clear();
    }

    void PhyIntfLocalManagedObject::deleteRspanCtagVlanClassificationAll()
    {
    	m_rspanGvlanTrunk.clear();
    }

	vector<WaveManagedObjectPointer<RspanMacAddressVlanClassificationMO> > PhyIntfLocalManagedObject::getMacAddressRspanVlanClassification() const
	{
		return m_rspanMacAddrVlanClassification;
	}

	vector<WaveManagedObjectPointer<RspanMacGroupVlanClassificationMO> > PhyIntfLocalManagedObject::getMacGroupRspanVlanClassification() const
	{
		return m_rspanMacGroupVlanClassification;
	}

	vector<WaveManagedObjectPointer<RspanGVlanTrunkPortManagedObject> > PhyIntfLocalManagedObject::getCtagRspanVlanClassification() const
	{
		return m_rspanGvlanTrunk;
	}

    void PhyIntfLocalManagedObject::setOpenflow(const bool &openflow)
    {
        m_openflow  =  openflow;
    }

    bool  PhyIntfLocalManagedObject::getOpenflow() const
    {
        return (m_openflow);
    }

}
