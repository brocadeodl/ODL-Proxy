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

#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"
/* Qos */
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "APPM/Global/AMPPProfileDomainCompositeManagedObject.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/PVlanTrunkAssocManagedObject.h"
#include "Nsm/Local/PVlanMappingManagedObject.h"

#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalUpdatePoStpConfigMessage.h"
#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"


namespace DcmNs
{

    PoPhyIntfManagedObject::PoPhyIntfManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (PoPhyIntfManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        IntfManagedObject (pWaveObjectManager)
        
    {
		m_portVlanXstpConfig.clear();
		m_portInstanceXstpConfig.clear();
        m_isAllowedVlanNone = false;
		m_flowControlTx = 3;
		m_flowControlRx = 3;
		m_qosTrust = false;
		m_qosDscpTrust = false;
        m_priorityTag = false;
		m_defaultCos = 0;
		m_isAllowedVlanAll = true;
		m_swModeFlag = false; 
		m_swMode = SW_MODE_UNKNOWN;
        m_ignore_split = true;
		m_isPortProfiled = false;
		m_pVlanModePromiscuous = false;
		m_pVlanModeHost = false;
		m_pVlanModeTrunkBas = false;
		m_pVlanModeTrunkPromis = false;
		m_pVlanModeTrunkHost = false;
		m_pVlanNativeVlanId = 0;
		m_pVlanIsAllowedVlanAll = false;
		m_pVlanIsAllowedVlanNone = false;
		m_pVlanHostPrimVlanId = 0;
		m_pVlanHostSecVlanId = 0;
		m_raGuard = false;
		m_macAddressVlanClassification.clear();
		m_macGroupVlanClassification.clear();
		m_ctagVlanClassification.clear();
		m_ctagVlanClassificationRemove.clear();
        m_associatedPortProfiles.clear();
    }

    PoPhyIntfManagedObject::PoPhyIntfManagedObject (WaveObjectManager *pWaveObjectManager,const bool &swModeFlag,const swModeEnum &swMode,const bool &isAllowedVlanAll,const UI32Range &vlanRangeAdd, const UI32Range &vlanRangeRemove, const UI32Range &vlanRangeExcept,  const vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > &portVlanXstpConfig, const vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > &portInstanceXstpConfig, const bool &isPortProfiled, const bool &pVlanModePromiscuous, const bool &pVlanModeHost, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll, const bool &pVlanIsAllowedVlanNone, const UI32Range &pVlanRangeAdd, const UI32Range &pVlanRangeRemove, const UI32Range &pVlanRangeExcept)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (PoPhyIntfManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        IntfManagedObject (pWaveObjectManager),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_pVlanModePromiscuous    (pVlanModePromiscuous),
        m_pVlanModeHost    (pVlanModeHost),
		m_pVlanNativeVlanId	(pVlanNativeVlanId),
		m_pVlanIsAllowedVlanAll (pVlanIsAllowedVlanAll), 
		m_pVlanIsAllowedVlanNone (pVlanIsAllowedVlanNone), 
		m_pVlanRangeAdd			(pVlanRangeAdd), 
		m_pVlanRangeRemove		(pVlanRangeRemove), 
		m_pVlanRangeExcept		(pVlanRangeExcept), 
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_vlanRangeAdd       (vlanRangeAdd),
        m_vlanRangeRemove    (vlanRangeRemove),
        m_vlanRangeExcept    (vlanRangeExcept),
		m_portVlanXstpConfig (portVlanXstpConfig),
		m_portInstanceXstpConfig (portInstanceXstpConfig),
		m_isPortProfiled (isPortProfiled)
    {
        m_isAllowedVlanNone = false;
		m_flowControlTx = 3;
		m_flowControlRx = 3;
		m_qosTrust = false;
		m_qosDscpTrust = false;
        m_priorityTag = false;
		m_defaultCos = 0;
        m_ignore_split = true;
        m_pVlanModeTrunkBas = false;
		m_pVlanModeTrunkPromis = false;
		m_pVlanModeTrunkHost = false;
		m_pVlanHostPrimVlanId = 0;
		m_pVlanHostSecVlanId = 0;
		m_raGuard = false;
		m_macAddressVlanClassification.clear();
		m_macGroupVlanClassification.clear();
		m_ctagVlanClassification.clear();
		m_ctagVlanClassificationRemove.clear();
		m_ctagPVlanClassification.clear();
		m_ctagPVlanClassificationRemove.clear();
    }

    PoPhyIntfManagedObject::~PoPhyIntfManagedObject ()
    {
    }

    string  PoPhyIntfManagedObject::getClassName()
    {
        return ("PoPhyIntfManagedObject");
    }

    void  PoPhyIntfManagedObject::setupAttributesForPersistence()
    {
	  	UI32 qosDefault = 3;
        UI32 swModeDefault = 0 ; // SW_MODE_UNKNOWN;
         const UI32Range defaultData("");
		UI32 vlanDefault = 0;
        IntfManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag",brocade_interface_switchport));
        addPersistableAttribute (new AttributeBool(&m_swModeFlag,"swBasic",brocade_interface_basic));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_swMode),swModeDefault, "swMode",brocade_interface_vlan_mode));
        addPersistableAttribute (new AttributeBool(&m_pVlanModePromiscuous,"pVlanModePromiscuous",brocade_interface_promiscuous));
        addPersistableAttribute (new AttributeBool(&m_pVlanModeHost,"pVlanModeHost",brocade_interface_host));
        addPersistableAttribute (new AttributeBool(&m_pVlanModeTrunkBas,"pVlanModeTrunkBas",brocade_interface_trunk_basic));
        addPersistableAttribute (new AttributeBool(&m_pVlanModeTrunkPromis,"pVlanModeTrunkPromis",brocade_interface_trunk_promiscuous));
        addPersistableAttribute (new AttributeBool(&m_pVlanModeTrunkHost,"pVlanModeTrunkHost",brocade_interface_trunk_host));
		addPersistableAttribute (new AttributeUI32(&m_pVlanNativeVlanId,vlanDefault,"pVlanNativeVlanId",brocade_interface_pvlanNativevlan));
        addPersistableAttribute (new AttributeBool(&m_pVlanIsAllowedVlanAll,"pVlanIsAllowedVlanAll",brocade_interface_pvlan_all));
        addPersistableAttribute (new AttributeBool(&m_pVlanIsAllowedVlanNone,"pVlanIsAllowedVlanNone",brocade_interface_pvlan_none));
		addPersistableAttribute (new AttributeUI32Range(&m_pVlanRangeAdd,defaultData,"pVlanRangeAdd",brocade_interface_pvlan_add));
		addPersistableAttribute (new AttributeUI32Range(&m_pVlanRangeRemove,defaultData,"pVlanRangeRemove",brocade_interface_pvlan_remove));
		addPersistableAttribute (new AttributeUI32Range(&m_pVlanRangeExcept,defaultData,"pVlanRangeExcept",brocade_interface_pvlan_except));
		addPersistableAttribute (new AttributeUI32(&m_pVlanHostPrimVlanId,vlanDefault,"pVlanHostPrimVlanId",brocade_interface_host_pri_pvlan));
		addPersistableAttribute (new AttributeUI32(&m_pVlanHostSecVlanId,vlanDefault,"pVlanHostSecVlanId",brocade_interface_host_sec_pvlan));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<PVlanTrunkAssocManagedObject>(&m_pVlanTrunkAssoc,"pVlanTrunkAssoc", getClassName (), getObjectId (), PVlanTrunkAssocManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<PVlanMappingManagedObject>(&m_pVlanMapping,"pVlanMapping", getClassName (), getObjectId (), PVlanMappingManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeBool(&m_ignore_split,"ignore_split",brocade_interface_ignore_split));
        addPersistableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll",brocade_interface_all));
        addPersistableAttribute (new AttributeBool(&m_isAllowedVlanNone,"isAllowedVlanNone",brocade_interface_none));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_accessVlanObjectId,"vlanObjectId", VlanIntfManagedObject::getClassName (),true));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeAdd,defaultData,"vlanRangeAdd",brocade_interface_add));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeRemove,defaultData,"vlanRangeRemove",brocade_interface_remove));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeExcept,defaultData,"vlanRangeExcept",brocade_interface_except));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_flowControlTx, qosDefault, "flowControlTx", qos_flowcontrol_tx));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_flowControlRx, qosDefault, "flowControlRx", qos_flowcontrol_rx));
        addPersistableAttribute (new AttributeBool(&m_qosTrust,"qosTrust", qos_trust_cos));
        addPersistableAttribute (new AttributeBool(&m_priorityTag,"priorityTag", qos_priority_tag_enable));
        addPersistableAttribute (new AttributeSI32(&m_defaultCos,"defaultCos", qos_default_cos));
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_qosCosMap,"qosCosMap", CosToCosMutationMapManagedObject::getClassName (), true, qos_cos_mutation));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_qosTrafficClassMap,"qosTrafficClassMap", CosToTrafficClassMapManagedObject::getClassName (), true, qos_cos_traffic_class));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_ceeMap,"ceeMap", CeeMapManagedObject::getClassName (), true, brocade_interface_cee));
        addPersistableAttribute (new AttributeBool(&m_qosDscpTrust,"qosDscpTrust", qos_trust_dscp));
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_qosDscpMap,"qosDscpMap", DscpToDscpMutationMapManagedObject::getClassName (), true, qos_dscp_mutation));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_qosDscpTrafficClassMap,"qosDscpTrafficClassMap", DscpToTrafficClassMapManagedObject::getClassName (), true, qos_dscp_traffic_class));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_qosDscpCosMap,"qosDscpCosMap",DscpToCosMapManagedObject::getClassName (), true, qos_dscp_cos));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<PfcFlowControlLocalManagedObject>(&m_pfcFlowControl,"pfcFlowControl", getClassName (), getObjectId (), PfcFlowControlLocalManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<QosCosToProfileIdManagedObject>(&m_cosProfile,"cosProfile", getClassName (), getObjectId (), QosCosToProfileIdManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<QosCosToTailDropManagedObject>(&m_cosTailDrop,"cosTailDrop", getClassName (), getObjectId (), QosCosToTailDropManagedObject::getClassName ()));
		

        setUserTagForAttribute ("vlanObjectId", brocade_interface_accessvlan);

        addPersistableAttribute (new AttributeManagedObjectVectorComposition<XstpPortVlanConfigManagedObject>(&m_portVlanXstpConfig,"portVlanXstp", getClassName (), getObjectId (), XstpPortVlanConfigManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<XstpPortInstanceConfigManagedObject>(&m_portInstanceXstpConfig,"portInstanceXstp", getClassName (), getObjectId (), XstpPortInstanceConfigManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectComposition<XstpPortConfigManagedObject>(&m_portStpConfig, "portStpConfig", XstpPortConfigManagedObject::getClassName (),true));
		addPersistableAttribute (new AttributeBool((bool * )(&m_isPortProfiled),"isPortProfiled",appm_port_profile_port));

        //QoS Policy
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_inputPolicyMapId,"inputPolicyMapId", PolicyClassMapManagedObject::getClassName (),true, policer_in));
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_outputPolicyMapId,"outputPolicyMapId", PolicyClassMapManagedObject::getClassName (),true, policer_out));
        addPersistableAttribute (new AttributeBool((bool * )(&m_raGuard), "raGuard", brocade_interface_ra_guard));
		addPersistableAttribute (new AttributeManagedObjectComposition<PortSecurityLocalManagedObject>(&m_portSecConfig,"portSec", PortSecurityLocalManagedObject::getClassName (), true));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<MacAddressVlanClassificationMO>(&m_macAddressVlanClassification, "macAddressVlanClassification", getClassName (), getObjectId (), MacAddressVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<MacGroupVlanClassificationMO>(&m_macGroupVlanClassification, "macGroupVlanClassification", getClassName (), getObjectId (), MacGroupVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<CtagVlanClassificationMO>(&m_ctagVlanClassification, "ctagVlanClassification", getClassName (), getObjectId (), CtagVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<CtagVlanClassificationRemoveMO>(&m_ctagVlanClassificationRemove, "ctagVlanClassificationRemove", getClassName (), getObjectId (), CtagVlanClassificationRemoveMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<CtagPVlanClassificationMO>(&m_ctagPVlanClassification, "ctagPVlanClassification", getClassName (), getObjectId (), CtagPVlanClassificationMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<CtagPVlanClassificationRemoveMO>(&m_ctagPVlanClassificationRemove, "ctagPVlanClassificationRemove", getClassName (), getObjectId (), CtagPVlanClassificationRemoveMO::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<AMPPProfileCompositeManagedObject>(&m_associatedPortProfiles, "associatedPortProfiles", getClassName (), getObjectId (), AMPPProfileCompositeManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectComposition<AMPPProfileDomainCompositeManagedObject>(&m_associatedPortProfileDomain, "associatedPortProfileDomain", AMPPProfileDomainCompositeManagedObject::getClassName (),true));		
		
    }

    void  PoPhyIntfManagedObject::setupAttributesForCreate()
    {
	  	UI32 qosDefault = 3;
        UI32 swModeDefault = 0; // SW_MODE_UNKNOWN;
        const UI32Range defaultData("");
		UI32 vlanDefault = 0;
        addPersistableAttributeForCreate  (new AttributeBool(&m_swModeFlag,"swModeFlag",brocade_interface_switchport));
        addPersistableAttributeForCreate (new AttributeBool(&m_swModeFlag,"swBasic",brocade_interface_basic));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_swMode),swModeDefault, "swMode",brocade_interface_vlan_mode));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanModePromiscuous,"pVlanModePromiscuous",brocade_interface_promiscuous));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanModeHost,"pVlanModeHost",brocade_interface_host));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanModeTrunkBas,"pVlanModeTrunkBas",brocade_interface_trunk_basic));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanModeTrunkPromis,"pVlanModeTrunkPromis",brocade_interface_trunk_promiscuous));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanModeTrunkHost,"pVlanModeTrunkHost",brocade_interface_trunk_host));
        addPersistableAttributeForCreate (new AttributeUI32(&m_pVlanNativeVlanId,vlanDefault,"pVlanNativeVlanId",brocade_interface_pvlanNativevlan));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanIsAllowedVlanAll,"pVlanIsAllowedVlanAll",brocade_interface_pvlan_all));
        addPersistableAttributeForCreate (new AttributeBool(&m_pVlanIsAllowedVlanNone,"pVlanIsAllowedVlanNone",brocade_interface_pvlan_none));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_pVlanRangeAdd,defaultData,"pVlanRangeAdd",brocade_interface_pvlan_add));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_pVlanRangeRemove,defaultData,"pVlanRangeRemove",brocade_interface_pvlan_remove));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_pVlanRangeExcept,defaultData,"pVlanRangeExcept",brocade_interface_pvlan_except));
		addPersistableAttributeForCreate (new AttributeUI32(&m_pVlanHostPrimVlanId,"pVlanHostPrimVlanId",brocade_interface_host_pri_pvlan));
		addPersistableAttributeForCreate (new AttributeUI32(&m_pVlanHostSecVlanId,"pVlanHostSecVlanId",brocade_interface_host_sec_pvlan));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PVlanTrunkAssocManagedObject>(&m_pVlanTrunkAssoc,"pVlanTrunkAssoc", getClassName (), getObjectId (), PVlanTrunkAssocManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PVlanMappingManagedObject>(&m_pVlanMapping,"pVlanMapping", getClassName (), getObjectId (), PVlanMappingManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_ignore_split, "ignore_split"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isAllowedVlanNone,"isAllowedVlanNone"));
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_accessVlanObjectId,"vlanObjectId", VlanIntfManagedObject::getClassName (),true));
        addPersistableAttributeForCreate (new AttributeUI32Range(&m_vlanRangeAdd,"vlanRangeAdd",brocade_interface_add));
        addPersistableAttributeForCreate (new AttributeUI32Range(&m_vlanRangeRemove,"vlanRangeRemove",brocade_interface_remove));
        addPersistableAttributeForCreate (new AttributeUI32Range(&m_vlanRangeExcept,"vlanRangeExcept",brocade_interface_except));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_flowControlTx, qosDefault, "flowControlTx", qos_flowcontrol_tx));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_flowControlRx, qosDefault, "flowControlRx", qos_flowcontrol_rx));
        addPersistableAttributeForCreate  (new AttributeBool(&m_qosTrust,"qosTrust", qos_trust_cos));
        addPersistableAttributeForCreate  (new AttributeBool(&m_priorityTag,"priorityTag", qos_priority_tag_enable));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_defaultCos,"defaultCos", qos_default_cos));
		 addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_qosCosMap,"qosCosMap", CosToCosMutationMapManagedObject::getClassName (), true, qos_cos_mutation));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_qosTrafficClassMap,"qosTrafficClassMap", CosToTrafficClassMapManagedObject::getClassName (), true, qos_cos_traffic_class));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_ceeMap,"ceeMap", CeeMapManagedObject::getClassName (), true, brocade_interface_cee));
        addPersistableAttributeForCreate  (new AttributeBool(&m_qosDscpTrust,"qosDscpTrust", qos_trust_dscp));
         addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_qosDscpMap,"qosDscpMap", DscpToDscpMutationMapManagedObject::getClassName (), true, qos_dscp_mutation));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_qosDscpTrafficClassMap,"qosDscpTrafficClassMap", DscpToTrafficClassMapManagedObject::getClassName (), true, qos_dscp_traffic_class));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_qosDscpCosMap,"qosDscpCosMap", DscpToCosMapManagedObject::getClassName (), true, qos_dscp_cos));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PfcFlowControlLocalManagedObject>(&m_pfcFlowControl,"pfcFlowControl", getClassName (), getObjectId (), PfcFlowControlLocalManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<QosCosToProfileIdManagedObject>(&m_cosProfile,"cosProfile", getClassName (), getObjectId (), QosCosToProfileIdManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<QosCosToTailDropManagedObject>(&m_cosTailDrop,"cosTailDrop", getClassName (), getObjectId (), QosCosToTailDropManagedObject::getClassName ()));


        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<XstpPortVlanConfigManagedObject>(&m_portVlanXstpConfig,"portVlanXstp", getClassName (), getObjectId (), XstpPortVlanConfigManagedObject::getClassName ()));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<XstpPortInstanceConfigManagedObject>(&m_portInstanceXstpConfig,"portInstanceXstp", getClassName (), getObjectId (), XstpPortInstanceConfigManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<XstpPortConfigManagedObject>(&m_portStpConfig,"portStpConfig", XstpPortConfigManagedObject::getClassName (),true));
        addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_isPortProfiled),"isPortProfiled",appm_port_profile_port));

        //QoS Policy Map
		addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_inputPolicyMapId,"inputPolicyMapId", PolicyClassMapManagedObject::getClassName (),true, policer_in));
		addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_outputPolicyMapId,"outputPolicyMapId", PolicyClassMapManagedObject::getClassName (),true, policer_out));

        addPersistableAttributeForCreate(new AttributeBool((bool * )(&m_raGuard), "raGuard", brocade_interface_ra_guard));
		addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<PortSecurityLocalManagedObject>(&m_portSecConfig,"portSec", PortSecurityLocalManagedObject::getClassName (), true));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<MacAddressVlanClassificationMO>(&m_macAddressVlanClassification, "macAddressVlanClassification", getClassName (), getObjectId (), MacAddressVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<MacGroupVlanClassificationMO>(&m_macGroupVlanClassification, "macGroupVlanClassification", getClassName (), getObjectId (), MacGroupVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<CtagVlanClassificationMO>(&m_ctagVlanClassification, "ctagVlanClassification", getClassName (), getObjectId (), CtagVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<CtagVlanClassificationRemoveMO>(&m_ctagVlanClassificationRemove, "ctagVlanClassificationRemove", getClassName (), getObjectId (), CtagVlanClassificationRemoveMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<CtagPVlanClassificationMO>(&m_ctagPVlanClassification, "ctagPVlanClassification", getClassName (), getObjectId (), CtagPVlanClassificationMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<CtagPVlanClassificationRemoveMO>(&m_ctagPVlanClassificationRemove, "ctagPVlanClassificationRemove", getClassName (), getObjectId (), CtagPVlanClassificationRemoveMO::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<AMPPProfileCompositeManagedObject>(&m_associatedPortProfiles, "associatedPortProfiles", getClassName (), getObjectId (), AMPPProfileCompositeManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<AMPPProfileDomainCompositeManagedObject>(&m_associatedPortProfileDomain, "associatedPortProfileDomain", AMPPProfileDomainCompositeManagedObject::getClassName (),true));				
    }

    void  PoPhyIntfManagedObject::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  PoPhyIntfManagedObject::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  PoPhyIntfManagedObject::setSwMode(const swModeEnum &swMode)
    {
        m_swMode  =  swMode;
    }

    swModeEnum  PoPhyIntfManagedObject::getSwMode() const
    {
        return (m_swMode);
    }

    void  PoPhyIntfManagedObject::setAccessVlanObjectId(const ObjectId &accessVlanObjectId)
    {
        m_accessVlanObjectId  =  accessVlanObjectId;
    }

    ObjectId  PoPhyIntfManagedObject::getAccessVlanObjectId() const
    {
        return (m_accessVlanObjectId);
    }
    void  PoPhyIntfManagedObject::setPvlanModePromiscuous(const bool &pVlanModePromiscuous)
    {
        m_pVlanModePromiscuous =  pVlanModePromiscuous;
    }

    bool PoPhyIntfManagedObject::getPvlanModePromiscuous() const
    {
        return (m_pVlanModePromiscuous);
    }

    void  PoPhyIntfManagedObject::setPvlanModeHost(const bool &pVlanModeHost)
    {
        m_pVlanModeHost =  pVlanModeHost;
    }

    bool PoPhyIntfManagedObject::getPvlanModeHost() const
    {
        return (m_pVlanModeHost);
    }

    void  PoPhyIntfManagedObject::setPvlanModeTrunkBas(const bool &pVlanModeTrunkBas)
    {
        m_pVlanModeTrunkBas =  pVlanModeTrunkBas;
    }

    bool PoPhyIntfManagedObject::getPvlanModeTrunkBas() const
    {
        return (m_pVlanModeTrunkBas);
    }

    void  PoPhyIntfManagedObject::setPvlanModeTrunkPromis(const bool &pVlanModeTrunkPromis)
    {
        m_pVlanModeTrunkPromis =  pVlanModeTrunkPromis;
    }

    bool PoPhyIntfManagedObject::getPvlanModeTrunkPromis() const
    {
        return (m_pVlanModeTrunkPromis);
    }

    void  PoPhyIntfManagedObject::setPvlanModeTrunkHost(const bool &pVlanModeTrunkHost)
    {
        m_pVlanModeTrunkHost =  pVlanModeTrunkHost;
    }

    bool PoPhyIntfManagedObject::getPvlanModeTrunkHost() const
    {
        return (m_pVlanModeTrunkHost);
    }

	void  PoPhyIntfManagedObject::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
    {
        m_pVlanNativeVlanId = pVlanNativeVlanId;
    }

   	UI32 PoPhyIntfManagedObject::getPvlanNativeVlanId() const
    {
        return (m_pVlanNativeVlanId);
    }
	
	void PoPhyIntfManagedObject::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
	{
		m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
	}

	bool PoPhyIntfManagedObject::getPvlanIsAllowedVlanAll() const
	{
		return (m_pVlanIsAllowedVlanAll);
	}
	
	void PoPhyIntfManagedObject::setPvlanIsAllowedVlanNone(const bool &pVlanIsAllowedVlanNone)
	{
		m_pVlanIsAllowedVlanNone = pVlanIsAllowedVlanNone;
	}

	bool PoPhyIntfManagedObject::getPvlanIsAllowedVlanNone() const
	{
		return (m_pVlanIsAllowedVlanNone);
	}

	void PoPhyIntfManagedObject::setPvlanRangeAdd(const UI32Range &pVlanRangeAdd)
	{
		m_pVlanRangeAdd = pVlanRangeAdd;
	}

	UI32Range PoPhyIntfManagedObject::getPvlanRangeAdd() const
	{
		return (m_pVlanRangeAdd);
	}

	void PoPhyIntfManagedObject::setPvlanRangeRemove(const UI32Range &pVlanRangeRemove)
	{
		m_pVlanRangeRemove = pVlanRangeRemove;
	}

	UI32Range PoPhyIntfManagedObject::getPvlanRangeRemove() const
	{
		return (m_pVlanRangeRemove);
	}

	void PoPhyIntfManagedObject::setPvlanRangeExcept(const UI32Range &pVlanRangeExcept)
	{
		m_pVlanRangeExcept = pVlanRangeExcept;
	}

	UI32Range PoPhyIntfManagedObject::getPvlanRangeExcept() const
	{
		return (m_pVlanRangeExcept);
	}

	void  PoPhyIntfManagedObject::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 PoPhyIntfManagedObject::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void  PoPhyIntfManagedObject::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 PoPhyIntfManagedObject::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }

    void  PoPhyIntfManagedObject::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  PoPhyIntfManagedObject::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  PoPhyIntfManagedObject::setIgnoreSplit(const bool &flag)
    {
        m_ignore_split  =  flag;
    }

    bool  PoPhyIntfManagedObject::getIgnoreSplit() const
    {
        return (m_ignore_split);
    }

    void  PoPhyIntfManagedObject::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  PoPhyIntfManagedObject::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }

    void  PoPhyIntfManagedObject::setVlanRangeAdd(const UI32Range &vlanRangeAdd)
    {
        m_vlanRangeAdd  =  vlanRangeAdd;
    }

    UI32Range  PoPhyIntfManagedObject::getVlanRangeAdd() const
    {
        return (m_vlanRangeAdd);
    }

    void  PoPhyIntfManagedObject::setVlanRangeRemove(const UI32Range &vlanRangeRemove)
    {
        m_vlanRangeRemove  =  vlanRangeRemove;
    }

    UI32Range  PoPhyIntfManagedObject::getVlanRangeRemove() const
    {
        return (m_vlanRangeRemove);
    }
	
    void  PoPhyIntfManagedObject::setVlanRangeExcept(const UI32Range &vlanRangeExcept)
    {
        m_vlanRangeExcept  =  vlanRangeExcept;
    }

    UI32Range  PoPhyIntfManagedObject::getVlanRangeExcept() const
    {
        return (m_vlanRangeExcept);
    }

	void  PoPhyIntfManagedObject::setFlowControlTx(const SI32 &flowControlTx)
    {
        m_flowControlTx  =  flowControlTx;
    }

    SI32  PoPhyIntfManagedObject::getFlowControlTx() const
    {
        return (m_flowControlTx);
    }

    void  PoPhyIntfManagedObject::setFlowControlRx(const SI32 &flowControlRx)
    {
        m_flowControlRx  =  flowControlRx;
    }
    SI32  PoPhyIntfManagedObject::getFlowControlRx() const
    {
        return (m_flowControlRx);
    }

    void  PoPhyIntfManagedObject::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }
    bool  PoPhyIntfManagedObject::getQosTrust() const
    {
        return (m_qosTrust);
    }

	void  PoPhyIntfManagedObject::setQosDscpTrust(const bool &qosDscpTrust)
    {
        m_qosDscpTrust  =  qosDscpTrust;
    }
    bool  PoPhyIntfManagedObject::getQosDscpTrust() const
    {
        return (m_qosDscpTrust);
    }

    void  PoPhyIntfManagedObject::setPriorityTag(const bool &priorityTag)
    {
        m_priorityTag  =  priorityTag;
    }
    bool  PoPhyIntfManagedObject::getPriorityTag() const
    {
        return (m_priorityTag);
    }

    void  PoPhyIntfManagedObject::setDefaultCos(const SI32 &defaultCos)
    {
        m_defaultCos  =  defaultCos;
    }

    SI32  PoPhyIntfManagedObject::getDefaultCos() const
    {
        return (m_defaultCos);
    }


    void  PoPhyIntfManagedObject::addPortVlanXstpConfig(XstpPortVlanConfigManagedObject* portVlanXstpConfig)
    {
        WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> objectPointer(portVlanXstpConfig);
        m_portVlanXstpConfig.push_back(objectPointer);
    }

    vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > PoPhyIntfManagedObject::getPortVlanXstpConfig() const
    {
        return (m_portVlanXstpConfig);
    }

    void  PoPhyIntfManagedObject::deletePortVlanXstpConfigAll()
	{
		m_portVlanXstpConfig.clear();
	}

    void  PoPhyIntfManagedObject::deletePortVlanXstpConfig(const ObjectId &vlanOid)
	{
        unsigned int i;

        for (i = 0; i < m_portVlanXstpConfig.size(); i++) {
            if (m_portVlanXstpConfig[i]->getVlanObjectId() == vlanOid) {
                trace(TRACE_LEVEL_INFO, string("DelPortVlanXstpConfig: Found "));
                m_portVlanXstpConfig.erase(m_portVlanXstpConfig.begin() + i);
                break;
            }
        }
        return;
	}

    bool  PoPhyIntfManagedObject::isInstanceAllow(ObjectId &oid, UI32 id)
    {
	    return (true);
    }
    void  PoPhyIntfManagedObject::addPortInstanceXstpConfig(XstpPortInstanceConfigManagedObject* portInstanceXstpConfig)
    {
        WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> objectPointer(portInstanceXstpConfig);
        m_portInstanceXstpConfig.push_back(objectPointer);
    }

    vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > PoPhyIntfManagedObject::getPortInstanceXstpConfig() const
    {
        return (m_portInstanceXstpConfig);
    }

    void  PoPhyIntfManagedObject::deletePortInstanceXstpConfigAll()
	{
		m_portInstanceXstpConfig.clear();
	}
     void  PoPhyIntfManagedObject::deletePortInstanceXstpConfig(const ObjectId &instanceOid)
     {
	     unsigned int i;

	     for (i = 0; i < m_portInstanceXstpConfig.size(); i++) {
		     if (m_portInstanceXstpConfig[i]->getInstanceId() == instanceOid) {
			     trace(TRACE_LEVEL_INFO, string("DelPortInstanceXstpConfig: Found "));
			     m_portInstanceXstpConfig.erase(m_portInstanceXstpConfig.begin() + i);
			     break;
		     }
	     }
	     return;
     }

	bool  PoPhyIntfManagedObject::isVlanAllow(ObjectId &oid, UI32 id)
	{
			return (true);
	}

    void  PoPhyIntfManagedObject::setPVlanTrunkAssoc(const vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > &pVlanTrunkAssoc)
   {
       m_pVlanTrunkAssoc  =  pVlanTrunkAssoc;
   }

   vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> >  PoPhyIntfManagedObject::getPVlanTrunkAssoc() const
   {
       return (m_pVlanTrunkAssoc);
   }

   void PoPhyIntfManagedObject::addPVlanTrunkAssoc(PVlanTrunkAssocManagedObject *pVlanTrunkAssocMO)
   {
       WaveManagedObjectPointer<PVlanTrunkAssocManagedObject > pVlanTrunkAssoc (pVlanTrunkAssocMO);
       m_pVlanTrunkAssoc.push_back(pVlanTrunkAssoc);

   }

   SI32 PoPhyIntfManagedObject::deletePVlanTrunkAssocPrimSec(const UI32 &primId, const UI32 &secId)
   {

       unsigned int i;

       for (i = 0; i < m_pVlanTrunkAssoc.size(); i++) {
           if ( m_pVlanTrunkAssoc[i]->getPrimaryVlanId() == primId && m_pVlanTrunkAssoc[i]->getSecondaryVlanId() == secId){
               trace(TRACE_LEVEL_DEBUG, string("trunk assoc deleted") + primId + secId);
               m_pVlanTrunkAssoc.erase(m_pVlanTrunkAssoc.begin() + i);
               return (0);
           }
       }

       return (1);

   }

   SI32 PoPhyIntfManagedObject::deletePVlanTrunkAssocPrim(const UI32 &primId)
   {

	   unsigned int count=0;
	   unsigned int size = m_pVlanTrunkAssoc.size();

       for (unsigned int i = 0; i < size; i++)
       {
    	   if ( m_pVlanTrunkAssoc[i-count]->getPrimaryVlanId() == primId){
               trace(TRACE_LEVEL_DEBUG, string("trunk assoc deleted for primary vlan") + primId + "count "+ count);
               m_pVlanTrunkAssoc.erase(m_pVlanTrunkAssoc.begin() + (i-count));
               count++;
    	   }
       }
       return (1);
   }

   SI32 PoPhyIntfManagedObject::deletePVlanTrunkAssocAll()
   {
	   trace(TRACE_LEVEL_DEBUG, string(" deletePVlanTrunkAssocAll  size: ") + m_pVlanTrunkAssoc.size());
	   m_pVlanTrunkAssoc.clear();
       return (1);
   }


   void  PoPhyIntfManagedObject::setPVlanMapping(const vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > &pVlanMapping)
  {
      m_pVlanMapping  =  pVlanMapping;
  }

  vector<WaveManagedObjectPointer<PVlanMappingManagedObject> >  PoPhyIntfManagedObject::getPVlanMapping() const  //PVLAN todo configReplay and showAction
  {
      return (m_pVlanMapping);
  }

  void PoPhyIntfManagedObject::addPVlanMapping(PVlanMappingManagedObject *pVlanMappingMO)
  {
      WaveManagedObjectPointer<PVlanMappingManagedObject> pVlanMapping (pVlanMappingMO);
      m_pVlanMapping.push_back(pVlanMapping);

  }

  SI32 PoPhyIntfManagedObject::deletePVlanMappingPrim(const UI32 &primId)
  {

	   unsigned int count=0;
	   unsigned int size = m_pVlanMapping.size();

      for (unsigned int i = 0; i < size; i++)
      {
   	   if ( m_pVlanMapping[i-count]->getPrimaryVlanId() == primId){
              trace(TRACE_LEVEL_DEBUG, string("mapping deleted for primary vlan") + primId + "count "+ count);
              m_pVlanMapping.erase(m_pVlanMapping.begin() + (i-count));
              count++;
   	   }
      }
      return (1);
  }

  SI32 PoPhyIntfManagedObject::deletePVlanMappingAll()
  {
      trace(TRACE_LEVEL_DEBUG, string("deletePVlanMappingAll size:  ") + m_pVlanMapping.size());
      m_pVlanMapping.clear();

      return (1);
  }

  void PoPhyIntfManagedObject::cleanUpPvlanAll()
  {
	  	trace(TRACE_LEVEL_INFO, string("cleanUpPvlanAll"));
		setPvlanModePromiscuous(false);
		setPvlanModeHost(false);
		setPvlanModeTrunkBas(false);
		setPvlanModeTrunkPromis(false);
		setPvlanModeTrunkHost(false);
		setPvlanNativeVlanId(0);
		setPvlanIsAllowedVlanAll(false);
		setPvlanIsAllowedVlanNone(false);
		setPvlanRangeAdd(UI32Range(""));
		setPvlanRangeRemove(UI32Range(""));
		setPvlanRangeExcept(UI32Range(""));
		setPvlanHostPrimVlanId(0);
		setPvlanHostSecVlanId(0);
		deletePVlanMappingAll();
		deletePVlanTrunkAssocAll();
  }

  void PoPhyIntfManagedObject::cleanUpAccessTrunkAll()
  {
	  trace(TRACE_LEVEL_INFO, string("cleanUpAccessTrunkAll"));
	  setSwMode(SW_MODE_UNKNOWN);

	  /* Delete trunk mode entries */
	  setIsAllowedVlanAll(false);
	  setIsAllowedVlanNone(false);
	  /* Delete access mode entries */
	  setAccessVlanObjectId(ObjectId::NullObjectId);
	  setVlanRangeAdd(UI32Range(""));
	  setVlanRangeRemove(UI32Range(""));
	  setVlanRangeExcept(UI32Range(""));
  }

	void  PoPhyIntfManagedObject::setQosCosMap(const ObjectId &qosCosMap)
    {
        m_qosCosMap  =  qosCosMap;
    }

    ObjectId  PoPhyIntfManagedObject::getQosCosMap() const
    {
        return (m_qosCosMap);
    }

    void  PoPhyIntfManagedObject::setQosTrafficClassMap(const ObjectId &qosTrafficClassMap)
    {
        m_qosTrafficClassMap  =  qosTrafficClassMap;
    }

    ObjectId  PoPhyIntfManagedObject::getQosTrafficClassMap() const
    {
        return (m_qosTrafficClassMap);
    }
    void  PoPhyIntfManagedObject::setCeeMap(const ObjectId &ceeMap)
    {
        m_ceeMap  =  ceeMap;
    }

    ObjectId  PoPhyIntfManagedObject::getCeeMap() const
    {
        return (m_ceeMap);
    }

	void  PoPhyIntfManagedObject::setQosDscpMap(const ObjectId &qosDscpMap)
    {
        m_qosDscpMap  =  qosDscpMap;
    }

    ObjectId  PoPhyIntfManagedObject::getQosDscpMap() const
    {
        return (m_qosDscpMap);
    }

    void  PoPhyIntfManagedObject::setQosDscpTrafficClassMap(const ObjectId &qosDscpTrafficClassMap)
    {
        m_qosDscpTrafficClassMap  =  qosDscpTrafficClassMap;
    }

    ObjectId  PoPhyIntfManagedObject::getQosDscpTrafficClassMap() const
    {
        return (m_qosDscpTrafficClassMap);
    }
    void  PoPhyIntfManagedObject::setQosDscpCosMap(const ObjectId &qosDscpCosMap)
    {
        m_qosDscpCosMap  =  qosDscpCosMap;
    }

    ObjectId  PoPhyIntfManagedObject::getQosDscpCosMap() const
    {
        return (m_qosDscpCosMap);
    }

    void  PoPhyIntfManagedObject::setPfcFlowControl(const vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > &pfcFlowControl)
    {
        m_pfcFlowControl  =  pfcFlowControl;
    }

    vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> >  PoPhyIntfManagedObject::getPfcFlowControl() const
    {
        return (m_pfcFlowControl);
    }

	void  PoPhyIntfManagedObject::setCosProfile(const vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > &cosProfile)
    {
        m_cosProfile  =  cosProfile;
    }

    vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> >  PoPhyIntfManagedObject::getCosProfile() const
    {
        return (m_cosProfile);
    }

	void  PoPhyIntfManagedObject::setCosTailDrop(const vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > &cosTailDrop)
    {
        m_cosTailDrop  =  cosTailDrop;
    }

    vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> >  PoPhyIntfManagedObject::getCosTailDrop() const
    {
        return (m_cosTailDrop);
    }



    void  PoPhyIntfManagedObject::setPortStpConfig( WaveManagedObjectPointer<XstpPortConfigManagedObject> pportStpConfig)
    {
	    m_portStpConfig  =  pportStpConfig;
    }

    void  PoPhyIntfManagedObject::updatePortStpConfig(XstpPortConfigManagedObject *pportStpConfig)
    {
	    WaveManagedObjectPointer<XstpPortConfigManagedObject > portStpConfig (pportStpConfig);
	    m_portStpConfig  =  pportStpConfig;
    }

    WaveManagedObjectPointer<XstpPortConfigManagedObject>  PoPhyIntfManagedObject::getPortStpConfig() const
    {
	    return (m_portStpConfig);
    }
	void PoPhyIntfManagedObject::addPfc(PfcFlowControlLocalManagedObject *pfcMO)
    {
        WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> pfcEntry (pfcMO);
        m_pfcFlowControl.push_back(pfcEntry);

    }
    SI32 PoPhyIntfManagedObject::deletePfc(const SI32 &pfcCos)
    {

        unsigned int i;

        for (i = 0; i < m_pfcFlowControl.size(); i++) {
            if ( m_pfcFlowControl[i]->getCos() == pfcCos ){
                trace(TRACE_LEVEL_INFO, string("PFC deleted") + pfcCos);
                m_pfcFlowControl.erase(m_pfcFlowControl.begin() + i);
                return (0);
            }
        }

        return (1);

    }

    SI32 PoPhyIntfManagedObject::deletePfcAll()
    {
        unsigned int i;
	unsigned int size = m_pfcFlowControl.size();

        for (i = 0; i < size; i++)
       	{
            trace(TRACE_LEVEL_INFO, string("PFCs deleted"));
            m_pfcFlowControl.erase(m_pfcFlowControl.begin() + i);
        }
        return (1);
    }
    
    SI32 PoPhyIntfManagedObject::getPfcFlowControlRow(const SI32 &pfcCos, UI32 &tx, UI32 &rx)
    {
        unsigned int i;

        for (i = 0; i < m_pfcFlowControl.size(); i++) {
            if ( m_pfcFlowControl[i]->getCos() == pfcCos ) {
			  	tx = m_pfcFlowControl[i]->getTx();
			  	rx = m_pfcFlowControl[i]->getRx();
                return (0);
            }
        }
        return (-1);
    }
	
	void PoPhyIntfManagedObject::addCosProfile(QosCosToProfileIdManagedObject *cosProfileMO)
    {
        WaveManagedObjectPointer<QosCosToProfileIdManagedObject> cosProfileEntry (cosProfileMO);
        m_cosProfile.push_back(cosProfileEntry);

    }
    SI32 PoPhyIntfManagedObject::deleteCosProfile(const UI32 &cos)
    {

        unsigned int i;

        for (i = 0; i < m_cosProfile.size(); i++) {
            if ( m_cosProfile[i]->getCos() == cos){
                trace(TRACE_LEVEL_INFO, string("cos profile deleted") + cos);
                m_cosProfile.erase(m_cosProfile.begin() + i);
                return (0);
            }
        }

        return (1);

    }

    SI32 PoPhyIntfManagedObject::deleteCosProfileAll()
    {
        unsigned int i;
    	unsigned int size = m_cosProfile.size();

        for (i = 0; i < size; i++)
        {
            trace(TRACE_LEVEL_INFO, string("cos Profile deleted"));
            m_cosProfile.erase(m_cosProfile.begin() + i);
        }
        return (1);
    }

    SI32 PoPhyIntfManagedObject::getCosProfileRow(const UI32 &cos, UI32 &red_profile)
    {
        unsigned int i;

        for (i = 0; i < m_cosProfile.size(); i++) {
            if ( m_cosProfile[i]->getCos() == cos ) {
                red_profile = m_cosProfile[i]->getRedProfile();
                return (0);
            }
        }
	return (-1);
    }
	
	void PoPhyIntfManagedObject::addCosTailDrop(QosCosToTailDropManagedObject *cosTailDropMO)
    {
        WaveManagedObjectPointer<QosCosToTailDropManagedObject> cosTailDropEntry (cosTailDropMO);
        m_cosTailDrop.push_back(cosTailDropEntry);

    }
    
	SI32 PoPhyIntfManagedObject::deleteCosTailDrop(const UI32 &cos)
    {

        unsigned int i;

        for (i = 0; i < m_cosTailDrop.size(); i++) {
#if 0	//need to fix compilation
            if ( m_cosTailDrop[i]->getCos() == cos){
                trace(TRACE_LEVEL_INFO, string("cos profile deleted") + cos);
                m_cosTailDrop.erase(m_cosTailDrop.begin() + i);
                return (0);
            }
#endif
        }

        return (1);

    }

    SI32 PoPhyIntfManagedObject::deleteCosTailDropAll()
    {
        unsigned int i;
    	unsigned int size = m_cosTailDrop.size();

        for (i = 0; i < size; i++)
        {
            trace(TRACE_LEVEL_INFO, string("cos Profile deleted"));
            m_cosTailDrop.erase(m_cosTailDrop.begin() + i);
        }
        return (1);
    }

    SI32 PoPhyIntfManagedObject::getCosTailDropRow(const UI32 &cos, UI32 &tailDrop)
    {
        unsigned int i;

        for (i = 0; i < m_cosTailDrop.size(); i++) {
#if 0
            if ( m_cosTailDrop[i]->getCos() == cos ) {
                tailDrop = m_cosTailDrop[i]->getTailDropThreshold();
                return (0);
            }
#endif
        }
	return (-1);
    }

    void  PoPhyIntfManagedObject::setIsPortProfiled(const bool &isPortProfiled)
    {
        m_isPortProfiled = isPortProfiled;
    }

    bool PoPhyIntfManagedObject::getIsPortProfiled() const
    {
        return (m_isPortProfiled);
    }

	ObjectId PoPhyIntfManagedObject::getInputPolicyMapId() const {
		return (m_inputPolicyMapId);
	}

	void PoPhyIntfManagedObject::setInputPolicyMapId(const ObjectId &inputPolicyMapId) {
        m_inputPolicyMapId = inputPolicyMapId;
    }

	ObjectId PoPhyIntfManagedObject::getOutputPolicyMapId() const {
        return (m_outputPolicyMapId);
    }

    void PoPhyIntfManagedObject::setOutputPolicyMapId(const ObjectId &outputPolicyMapId) {
        m_outputPolicyMapId = outputPolicyMapId;
    }

    void  PoPhyIntfManagedObject::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard;
    }

    bool PoPhyIntfManagedObject::getRaGuard() const
    {
        return (m_raGuard);
    }

	void  PoPhyIntfManagedObject::setPortSecConfig(const WaveManagedObjectPointer<PortSecurityLocalManagedObject> &PortSecConfig)
	{
		m_portSecConfig  =  PortSecConfig;
	}

	WaveManagedObjectPointer<PortSecurityLocalManagedObject>  PoPhyIntfManagedObject::getPortSecConfig() const
	{
		return (m_portSecConfig);
    }

   void PoPhyIntfManagedObject::deleteMacAddressVlanClassificationAll()
   {
		m_macAddressVlanClassification.clear();
   }

   void PoPhyIntfManagedObject::deleteMacGroupVlanClassificationAll()
   {
		m_macGroupVlanClassification.clear();
   }

   void PoPhyIntfManagedObject::deleteCtagVlanClassificationAll()
   {
		m_ctagVlanClassification.clear();
   }

   void PoPhyIntfManagedObject::deleteCtagPVlanClassificationAll()
   {
		m_ctagPVlanClassification.clear();
   }

    vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > PoPhyIntfManagedObject::getMacAddressVlanClassification() const
    {
        return (m_macAddressVlanClassification);
    }

    vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > PoPhyIntfManagedObject::getMacGroupVlanClassification() const
    {
        return (m_macGroupVlanClassification);
    }

    vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > PoPhyIntfManagedObject::getCtagVlanClassification() const
    {
        return (m_ctagVlanClassification);
    }

    vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > PoPhyIntfManagedObject::getCtagPVlanClassification() const
    {
        return (m_ctagPVlanClassification);
    }
   
    void PoPhyIntfManagedObject::addPortProfileToInterface
        (const WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> &portProfile) {
            m_associatedPortProfiles.push_back(portProfile);
    }

    void PoPhyIntfManagedObject::removePortProfileFromInterface
        (const string &portProfile) {

            for(unsigned int it = 0 ; it < m_associatedPortProfiles.size(); it++) {
                if(portProfile == (m_associatedPortProfiles[it]->getPortProfileName())){
                    m_associatedPortProfiles.erase(m_associatedPortProfiles.begin()+it);
                }
            }
    }

    vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > 
        PoPhyIntfManagedObject::getAssociatedPortProfiles() const {

            return m_associatedPortProfiles;
    }

    void PoPhyIntfManagedObject::setAssociatedPortProfiles
        (const vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > &associatedPortProfiles) {
            m_associatedPortProfiles = associatedPortProfiles;
    }
	
	void PoPhyIntfManagedObject::setAssociatedPortProfileDomain
		(const WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> &portProfileDomain) {
		m_associatedPortProfileDomain = portProfileDomain;
    }

	void PoPhyIntfManagedObject::updatePortProfileDomain
		(const WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> &portProfileDomain) {
	}

	WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject>
		PoPhyIntfManagedObject::getAssociatedPortProfileDomain() const {
		return m_associatedPortProfileDomain;
	}

    string PoPhyIntfManagedObject::getAssociatedPortProfileDomainName() const {
        //AMPPProfileDomainCompositeManagedObject* p = m_associatedPortProfileDomain.operator->();
        //string profileDomainName = m_associatedPortProfileDomain->getPortProfileDomainName();
        return "";//profileDomainName;
    }

    vector<string> PoPhyIntfManagedObject::getAssociatedPortProfileNames() const {
        vector<string> portProfiles;
        /*for(unsigned int it = 0 ; it < m_associatedPortProfiles.size(); it++) {
            string profileName = m_associatedPortProfiles[it]->getPortProfileName();
            portProfiles.push_back(profileName);
        }*/
        return portProfiles;
    }
}
