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
 *   Author : suw                                                     *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

namespace DcmNs
{

// ************* RouteMapLocalManagedObject ************

    RouteMapLocalManagedObject::RouteMapLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
        : PrismElement  (pIpPolicyLocalObjectManager),
        PrismPersistableObject (RouteMapLocalManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpPolicyLocalObjectManager),
        DcmManagedObject (pIpPolicyLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpPolicyLocalObjectManager)
    {
	  	m_pbrVrfMO.clear();
		m_pbrNexthopAddrMO.clear();
        m_set_route_dampening_half_life = 0;
        m_set_route_dampening_reuse = 0;
        m_set_route_dampening_suppress = 0;
        m_set_route_dampening_max_suppress = 0;

    }

    RouteMapLocalManagedObject::~RouteMapLocalManagedObject ()
    {
    }

    string  RouteMapLocalManagedObject::getClassName()
    {
        return ("RouteMapLocalManagedObject");
    }

    void  RouteMapLocalManagedObject::setupAttributesForPersistence()
    {
        vector<UI32> defaultUI32Vec;
        defaultUI32Vec.clear();
        
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI16(&m_instance,"instance"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
//		addPersistableAttribute (new AttributeEnum((UI32*)&m_well_known_precedence,"well_known_precedence"));
//		addPersistableAttribute (new AttributeEnum((UI32*)&m_dscp_precedence,"dscp_precedence"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_next_hop_peer_address,true,"set_next_hop_peer_address"));
		addPersistableAttribute (new AttributeEnumUC(&m_dscp_precedence,true,"dscp_precedence"));
        addPersistableAttribute (new AttributeStringVectorUC(&m_match_intf_eth,true,"match_intf_eth"));
        addPersistableAttribute (new AttributeStringVectorUC(&m_match_intf_gi,true,"match_intf_gi"));
        addPersistableAttribute (new AttributeStringVectorUC(&m_match_intf_fo,true,"match_intf_fo"));
        addPersistableAttribute (new AttributeUI32VectorUC(&m_match_intf_vlan,true,true,defaultUI32Vec,"match_intf_vlan"));
        addPersistableAttribute (new AttributeUI32VectorUC(&m_match_intf_po,true,true,defaultUI32Vec,"match_intf_po"));
        addPersistableAttribute (new AttributeUI32VectorUC(&m_match_intf_ve,true,true,defaultUI32Vec,"match_intf_ve"));
        addPersistableAttribute (new AttributeUI32VectorUC(&m_match_intf_loopback,true,true,defaultUI32Vec,"match_intf_loopback"));
        addPersistableAttribute (new AttributeStringUC(&m_match_addr_prefix,true,"match_addr_prefix"));
        addPersistableAttribute (new AttributeStringUC(&m_match_nh_prefix,true,"match_nh_prefix"));
		addPersistableAttribute (new AttributeStringUC(&m_match_addr_acl,true,"match_addr_acl"));
		addPersistableAttribute (new AttributeBoolUC(&m_match_nullZero,true,"match_nullZero"));
        addPersistableAttribute (new AttributeUI32UC(&m_match_next_hop_address,true,"match_next_hop_address"));
        addPersistableAttribute (new AttributeUI32UC(&m_match_metric,true,"match_metric"));
        addPersistableAttribute (new AttributeEnumUC(&m_match_route_type,true,"match_route_type"));
        addPersistableAttribute (new AttributeUI32VectorUC(&m_match_route_tag,true,true,defaultUI32Vec,"match_route_tag"));
        addPersistableAttribute (new AttributeStringUC(&m_match_rs_prefix,true,"match_rs_prefix"));
        addPersistableAttribute (new AttributeStringUC(&m_match_route_aspath,true,"match_route_aspath"));
        addPersistableAttribute (new AttributeEnumUC(&m_match_bgp_route_type,true,"match_bgp_route_type"));
        addPersistableAttribute (new AttributeBoolUC(&m_match_protocol_bgp,true,"match_protocol_bgp"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_distance,true,"set_distance"));
 		addPersistableAttribute (new AttributeBoolUC(&m_set_route_metric_type_external,true,"set_route_metric_type_external"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_metric_type_internal,true,"set_route_metric_type_internal"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_metric_type_type_1,true,"set_route_metric_type_type_1"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_metric_type_type_2,true,"set_route_metric_type_type_2"));        
        addPersistableAttribute (new AttributeEnumUC(&m_set_metric_action,true,"set_metric_action"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_metric,true,"set_metric"));
        addPersistableAttribute (new AttributeEnumUC(&m_set_route_type,true,"set_route_type"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_tag,true,"set_route_tag"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_local_preference,true,"set_route_local_preference"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_weight,true,"set_route_weight"));
        addPersistableAttribute (new AttributeStringUC(&m_set_route_aspath_prepend,true,"set_route_aspath_prepend"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_aspath_tag,true,"set_route_aspath_tag"));
        addPersistableAttribute (new AttributeStringUC(&m_match_route_community_list,true,"match_route_community_list"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_origin_igp,true,"set_route_origin_igp"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_origin_incomplete,true,"set_route_origin_incomplete"));
        addPersistableAttribute (new AttributeStringUC(&m_set_route_community_access_list,true,"set_route_community_access_list"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_community_acl_delete,true,"set_route_community_acl_delete"));
        addPersistableAttribute (new AttributeBoolUC(&m_set_route_automatic_tag,true,"set_route_automatic_tag"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_dampening_half_life,true,"set_route_dampening_half_life"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_dampening_reuse,true,"set_route_dampening_reuse"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_dampening_suppress,true,"set_route_dampening_suppress"));
        addPersistableAttribute (new AttributeUI32UC(&m_set_route_dampening_max_suppress,true,"set_route_dampening_max_suppress"));
        addPersistableAttribute (new AttributeUI32UC(&m_route_continue,true,"route_continue"));
        addPersistableAttribute (new AttributeBoolUC(&m_route_continue_no_val,true,"route_continue_no_val"));
        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<PbrVrfManagedObject>(
            &m_pbrVrfMO, "pbrVrfMO", getClassName(), getObjectId(), PbrVrfManagedObject::getClassName()));
        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<PbrNexthopAddrManagedObject>(
            &m_pbrNexthopAddrMO, "pbrNexthopMO", getClassName(), getObjectId(), PbrNexthopAddrManagedObject::getClassName()));
        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<PbrGlobalNexthopManagedObject>(
            &m_pbrGlobalNexthopAddrMO, "pbrGlobalNexthopMO", getClassName(), getObjectId(), PbrGlobalNexthopManagedObject::getClassName()));
        addPersistableAttribute (new AttributeStringUC(&m_set_route_community_expr,true,"set_route_community_expr"));

        setUserTagForAttribute ("name", ip_policy_name);
        setUserTagForAttribute ("instance", ip_policy_instance);
        setUserTagForAttribute("action", ip_policy_action_rm);
        setUserTagForAttribute("set_next_hop_peer_address", ip_policy_peer_address);
        setUserTagForAttribute("match_intf_eth", ip_policy_tengigabitethernet_rmm);
        setUserTagForAttribute("match_intf_gi", ip_policy_gigabitethernet_rmm);
        setUserTagForAttribute("match_intf_fo", ip_policy_fortygigabitethernet_rmm);
        setUserTagForAttribute("match_intf_vlan", ip_policy_vlan_rmm);
        setUserTagForAttribute("match_intf_po", ip_policy_port_channel_rmm);
        setUserTagForAttribute("match_intf_ve", ip_policy_ve);
        setUserTagForAttribute("match_intf_loopback", ip_policy_loopback);
        setUserTagForAttribute("match_addr_prefix", ip_policy_prefix_list_rmm);
        setUserTagForAttribute("match_nh_prefix", ip_policy_prefix_list_rmm_n);
		setUserTagForAttribute("match_nullZero", ip_policy_null0);
		setUserTagForAttribute("dscp_precedence", ip_policy_dscp_rms);
		setUserTagForAttribute("match_addr_acl", ip_policy_acl_rmm);
        setUserTagForAttribute("match_next_hop_address", ip_policy_next_hop_filter_val);
        setUserTagForAttribute("match_metric", ip_policy_metric_rmm);
        setUserTagForAttribute("match_route_type", ip_policy_route_type_rmm);
        setUserTagForAttribute("match_route_tag", ip_policy_tag_rmm);
        setUserTagForAttribute("match_rs_prefix", ip_policy_prefix_list_rmrs);
        setUserTagForAttribute("match_route_aspath", ip_policy_as_path_access_list_name);
        setUserTagForAttribute("match_bgp_route_type", ip_policy_bgp_route_type);
        setUserTagForAttribute("match_protocol_bgp", ip_policy_protocol_bgp);
        setUserTagForAttribute("set_distance", ip_policy_dist_rms);
   		setUserTagForAttribute("set_route_metric_type_external", ip_policy_external);
        setUserTagForAttribute("set_route_metric_type_internal", ip_policy_internal);
        setUserTagForAttribute("set_route_metric_type_type_1", ip_policy_type_1);
        setUserTagForAttribute("set_route_metric_type_type_2", ip_policy_type_2);
        setUserTagForAttribute("set_metric_action", ip_policy_delta_rms);
        setUserTagForAttribute("set_metric", ip_policy_metric_rms);
        setUserTagForAttribute("set_route_type", ip_policy_route_type_rms);
        setUserTagForAttribute("set_route_tag", ip_policy_tag_rms);
        setUserTagForAttribute("set_route_local_preference", ip_policy_local_preference_value);
        setUserTagForAttribute("set_route_weight", ip_policy_weight_value);
        setUserTagForAttribute("set_route_aspath_prepend", ip_policy_prepend);
        setUserTagForAttribute("set_route_aspath_tag", ip_policy_aspath_tag);
        setUserTagForAttribute("match_route_community_list", ip_policy_community_access_list_name);
        setUserTagForAttribute("set_route_origin_igp", ip_policy_origin_igp);
        setUserTagForAttribute("set_route_origin_incomplete", ip_policy_origin_incomplete);
        setUserTagForAttribute("set_route_community_access_list", ip_policy_comm_list_name);
        setUserTagForAttribute("set_route_community_acl_delete", ip_policy_match_comm_delete);
        setUserTagForAttribute("set_route_automatic_tag", ip_policy_tag_empty);
        setUserTagForAttribute("set_route_dampening_half_life", ip_policy_half_life);
        setUserTagForAttribute("set_route_dampening_reuse", ip_policy_reuse);
        setUserTagForAttribute("set_route_dampening_suppress", ip_policy_suppress);
        setUserTagForAttribute("set_route_dampening_max_suppress", ip_policy_max_suppress_time);
        setUserTagForAttribute("set_route_community_expr", ip_policy_set_community_expr);
        setUserTagForAttribute("route_continue", ip_policy_continue_val);
        setUserTagForAttribute("route_continue_no_val", ip_policy_continue);
    }

    void  RouteMapLocalManagedObject::setupAttributesForCreate()
    {
        vector<UI32> defaultUI32Vec;
        defaultUI32Vec.clear();
        
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI16(&m_instance,"instance"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_action,"action"));
//		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_well_known_precedence,"well_known_precedence"));
//		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_dscp_precedence,"dscp_precedence"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_next_hop_peer_address,true,"set_next_hop_peer_address"));
		addPersistableAttributeForCreate  (new AttributeEnumUC(&m_dscp_precedence,true, "dscp_precedence"));
        addPersistableAttributeForCreate  (new AttributeStringVectorUC(&m_match_intf_eth,true,"match_intf_eth"));
        addPersistableAttributeForCreate  (new AttributeStringVectorUC(&m_match_intf_gi,true,"match_intf_gi"));
        addPersistableAttributeForCreate  (new AttributeStringVectorUC(&m_match_intf_fo,true,"match_intf_fo"));
        addPersistableAttributeForCreate  (new AttributeUI32VectorUC(&m_match_intf_vlan,true,true,defaultUI32Vec,"match_intf_vlan"));
        addPersistableAttributeForCreate  (new AttributeUI32VectorUC(&m_match_intf_po,true,true,defaultUI32Vec,"match_intf_po"));
        addPersistableAttributeForCreate  (new AttributeUI32VectorUC(&m_match_intf_ve,true,true,defaultUI32Vec,"match_intf_ve"));
        addPersistableAttributeForCreate  (new AttributeUI32VectorUC(&m_match_intf_loopback,true,true,defaultUI32Vec,"match_intf_loopback"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_addr_prefix,true,"match_addr_prefix"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_nh_prefix,true,"match_nh_prefix"));
		addPersistableAttributeForCreate  (new AttributeBoolUC(&m_match_nullZero,true,"match_nullZero"));
		addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_addr_acl,true,"match_addr_acl"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_match_next_hop_address,true,"match_next_hop_address"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_match_metric,true,"match_metric"));
        addPersistableAttributeForCreate  (new AttributeEnumUC(&m_match_route_type,true, "match_route_type"));
        addPersistableAttributeForCreate  (new AttributeUI32VectorUC(&m_match_route_tag,true,true,defaultUI32Vec,"match_route_tag"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_rs_prefix,true,"match_rs_prefix"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_route_aspath,true,"match_route_aspath"));
        addPersistableAttributeForCreate  (new AttributeEnumUC(&m_match_bgp_route_type,true, "match_bgp_route_type"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_match_protocol_bgp,true,"match_protocol_bgp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_distance,true,"set_distance"));
		addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_metric_type_external,true,"set_route_metric_type_external"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_metric_type_internal,true,"set_route_metric_type_internal"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_metric_type_type_1,true,"set_route_metric_type_type_1"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_metric_type_type_2,true,"set_route_metric_type_type_2"));
        addPersistableAttributeForCreate  (new AttributeEnumUC(&m_set_metric_action, true, "set_metric_action"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_metric,true, "set_metric"));
        addPersistableAttributeForCreate  (new AttributeEnumUC(&m_set_route_type,true, "set_route_type"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_tag,true,"set_route_tag"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_local_preference,true,"set_route_local_preference"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_weight,true,"set_route_weight"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_set_route_aspath_prepend,true,"set_route_aspath_prepend"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_aspath_tag,true,"set_route_aspath_tag"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_match_route_community_list,true,"match_route_community_list"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_origin_igp,true,"set_route_origin_igp"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_origin_incomplete,true,"set_route_origin_incomplete"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_set_route_community_access_list,true,"set_route_community_access_list"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_community_acl_delete,true,"set_route_community_acl_delete"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_automatic_tag,true,"set_route_automatic_tag"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_set_route_dampening,true,"set_route_dampening"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_dampening_half_life,true,"set_route_dampening_half_life"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_dampening_reuse,true,"set_route_dampening_reuse"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_dampening_suppress,true,"set_route_dampening_suppress"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_set_route_dampening_max_suppress,true,"set_route_dampening_max_suppress"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_route_continue,true,"route_continue"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_route_continue_no_val,true,"route_continue_no_val"));
        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<PbrVrfManagedObject>(
            &m_pbrVrfMO, "pbrVrfMO", getClassName(), getObjectId(), PbrVrfManagedObject::getClassName())); 
        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<PbrNexthopAddrManagedObject>(
            &m_pbrNexthopAddrMO, "pbrNexthopMO", getClassName(), getObjectId(), PbrNexthopAddrManagedObject::getClassName())); 
        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<PbrGlobalNexthopManagedObject>(
            &m_pbrGlobalNexthopAddrMO, "pbrGlobalNexthopMO", getClassName(), getObjectId(), PbrGlobalNexthopManagedObject::getClassName()));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_set_route_community_expr,true,"set_route_community_expr"));
    }

    void  RouteMapLocalManagedObject::setupKeys() {
        vector<string> keyName;
        keyName.push_back("name");
        keyName.push_back("action");
        keyName.push_back("instance");
        setUserDefinedKeyCombination (keyName);
    }

    void  RouteMapLocalManagedObject::setInstance(const UI16 &instance)
    {
        m_instance = instance;
    }

    UI16  RouteMapLocalManagedObject::getInstance() const
    {
        return (m_instance);
    }

    void  RouteMapLocalManagedObject::setAction(const PolicyAction &action)
    {
        m_action = action;
    }

    PolicyAction  RouteMapLocalManagedObject::getAction() const
    {
        return (m_action);
    }

    void  RouteMapLocalManagedObject::setMatch_intf_eth(const vector<string> &match_intf_eth)
    {
        m_match_intf_eth = match_intf_eth;
        m_match_intf_eth.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_eth_selective(const vector<string> &match_intf_eth)
    {
        vector<string> intf1 = match_intf_eth;
        vector<string> intf2 = m_match_intf_eth.getStringVector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_eth_selective: intf1: ") + intf1[i] + string(" intf2: ") + intf2[j] );  
                if(intf1[i] == intf2[j]) {
                    //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_eth_selective: Erasing.."));
                    intf2.erase(intf2.begin() + j);
                    break;
                }    
            }    
        }
       
        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_eth.setIsUserConfigured(false);
        else
            m_match_intf_eth.setStringVector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_eth()
    {
        m_match_intf_eth.setIsUserConfigured(false);
    }

    vector<string>  RouteMapLocalManagedObject::getMatch_intf_eth() const
    {
        if (m_match_intf_eth.getIsUserConfigured())
            return (m_match_intf_eth.getStringVector());

        return vector<string>();
    }


    void  RouteMapLocalManagedObject::setMatch_intf_gi(const vector<string> &match_intf_gi)
    {
        m_match_intf_gi = match_intf_gi;
        m_match_intf_gi.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_gi_selective(const vector<string> &match_intf_gi)
    {
        vector<string> intf1 = match_intf_gi;
        vector<string> intf2 = m_match_intf_gi.getStringVector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_gi_selective: intf1: ") + intf1[i] + string(" intf2: ") + intf2[j] );  
                if(intf1[i] == intf2[j]) {
                    //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_gi_selective: Erasing.."));
                    intf2.erase(intf2.begin() + j);
                    break;
                }    
            }    
        }
       
        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_gi.setIsUserConfigured(false);
        else
            m_match_intf_gi.setStringVector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_gi()
    {
        m_match_intf_gi.setIsUserConfigured(false);
    }

    vector<string>  RouteMapLocalManagedObject::getMatch_intf_gi() const
    {
        if (m_match_intf_gi.getIsUserConfigured())
            return (m_match_intf_gi.getStringVector());

        return vector<string>();
    }

    void  RouteMapLocalManagedObject::setMatch_intf_fo(const vector<string> &match_intf_fo)
    {
        m_match_intf_fo = match_intf_fo;
        m_match_intf_fo.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_fo_selective(const vector<string> &match_intf_fo)
    {
        vector<string> intf1 = match_intf_fo;
        vector<string> intf2 = m_match_intf_fo.getStringVector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_fo_selective: intf1: ") + intf1[i] + string(" intf2: ") + intf2[j] );  
                if(intf1[i] == intf2[j]) {
                    //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_fo_selective: Erasing.."));
                    intf2.erase(intf2.begin() + j);
                    break;
                }    
            }    
        }
       
        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_fo.setIsUserConfigured(false);
        else
            m_match_intf_fo.setStringVector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_fo()
    {
        m_match_intf_fo.setIsUserConfigured(false);
    }

    vector<string>  RouteMapLocalManagedObject::getMatch_intf_fo() const
    {
        if (m_match_intf_fo.getIsUserConfigured())
            return (m_match_intf_fo.getStringVector());

        return vector<string>();
    }

    void  RouteMapLocalManagedObject::setMatch_intf_vlan(const vector<UI32> &match_intf_vlan)
    {
        m_match_intf_vlan = match_intf_vlan;
        m_match_intf_vlan.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_vlan_selective(const vector<UI32> &match_intf_vlan)
    {
        vector<UI32> intf1 = match_intf_vlan;
        vector<UI32> intf2 = m_match_intf_vlan.getUI32Vector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_vlan_selective: intf1: ") + intf1[i] + string(" intf2: ") + intf2[j] );  
                if(intf1[i] == intf2[j]) {
                    //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_vlan_selective: Erasing.."));
                    intf2.erase(intf2.begin() + j);
                    break;
                }    
            }    
        }
       
        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_vlan.setIsUserConfigured(false);
        else
            m_match_intf_vlan.setUI32Vector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_vlan()
    {
        m_match_intf_vlan.setIsUserConfigured(false);
    }

    vector<UI32>  RouteMapLocalManagedObject::getMatch_intf_vlan() const
    {
        if (m_match_intf_vlan.getIsUserConfigured())
            return (m_match_intf_vlan.getUI32Vector());

        return vector<UI32>();
    }

    void  RouteMapLocalManagedObject::setMatch_intf_po(const vector<UI32> &match_intf_po)
    {
        m_match_intf_po = match_intf_po;
        m_match_intf_po.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_po_selective(const vector<UI32> &match_intf_po)
    {
        vector<UI32> intf1 = match_intf_po;
        vector<UI32> intf2 = m_match_intf_po.getUI32Vector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_po_selective: intf1: ") + intf1[i] + string(" intf2: ") + intf2[j] );  
                if(intf1[i] == intf2[j]) {
                    //trace(TRACE_LEVEL_INFO, string("unsetMatch_intf_po_selective: Erasing.."));
                    intf2.erase(intf2.begin() + j);
                    break;
                }    
            }    
        }
       
        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_po.setIsUserConfigured(false);
        else
            m_match_intf_po.setUI32Vector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_po()
    {
        m_match_intf_po.setIsUserConfigured(false);
    }

    vector<UI32>  RouteMapLocalManagedObject::getMatch_intf_po() const
    {
        if (m_match_intf_po.getIsUserConfigured())
            return (m_match_intf_po.getUI32Vector());
        return vector<UI32>();
    }

    void  RouteMapLocalManagedObject::setMatch_intf_ve(const vector<UI32> &match_intf_ve)
    {
        m_match_intf_ve = match_intf_ve;
        m_match_intf_ve.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_ve_selective(const vector<UI32> &match_intf_ve)
    {
        vector<UI32> intf1 = match_intf_ve;
        vector<UI32> intf2 = m_match_intf_ve.getUI32Vector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                if(intf1[i] == intf2[j]) {
                    intf2.erase(intf2.begin() + j);
                    break;
                }
            }
        }

        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_ve.setIsUserConfigured(false);
        else
            m_match_intf_ve.setUI32Vector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_ve()
    {
        m_match_intf_ve.setIsUserConfigured(false);
    }

    vector<UI32>  RouteMapLocalManagedObject::getMatch_intf_ve() const
    {
        if (m_match_intf_ve.getIsUserConfigured())
            return (m_match_intf_ve.getUI32Vector());

        return vector<UI32>();
    }

    void  RouteMapLocalManagedObject::setMatch_intf_loopback(const vector<UI32> &match_intf_loopback)
    {
        m_match_intf_loopback = match_intf_loopback;
        m_match_intf_loopback.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_loopback_selective(const vector<UI32> &match_intf_loopback)
    {
        vector<UI32> intf1 = match_intf_loopback;
        vector<UI32> intf2 = m_match_intf_loopback.getUI32Vector();
        for (UI32 i = 0; i < intf1.size(); i++) {
            for (UI32 j = 0; j < intf2.size(); j++) {
                if(intf1[i] == intf2[j]) {
                    intf2.erase(intf2.begin() + j);
                    break;
                }
            }
        }

        if( (!intf1.size()) || (!intf2.size()) )
            m_match_intf_loopback.setIsUserConfigured(false);
        else
            m_match_intf_loopback.setUI32Vector(intf2);
    }

    void  RouteMapLocalManagedObject::unsetMatch_intf_loopback()
    {
        m_match_intf_loopback.setIsUserConfigured(false);
    }

    vector<UI32>  RouteMapLocalManagedObject::getMatch_intf_loopback() const
    {
        if (m_match_intf_loopback.getIsUserConfigured())
            return (m_match_intf_loopback.getUI32Vector());

        return vector<UI32>();
    }

    void  RouteMapLocalManagedObject::setMatch_addr_prefix(const string &match_addr_prefix)
    {
        m_match_addr_prefix.setStringValue(match_addr_prefix);
        m_match_addr_prefix.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_addr_prefix()
    {
        m_match_addr_prefix.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_addr_prefix() const
    {
        return (m_match_addr_prefix.getStringValue());
    }

    void  RouteMapLocalManagedObject::setMatch_nh_prefix(const string &match_nh_prefix)
    {
        m_match_nh_prefix.setStringValue(match_nh_prefix);
        m_match_nh_prefix.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_nh_prefix()
    {
        m_match_nh_prefix.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_nh_prefix() const
    {
        return (m_match_nh_prefix.getStringValue());
    }

    void  RouteMapLocalManagedObject::setMatch_addr_acl(const string &match_addr_acl)
    {
        m_match_addr_acl.setStringValue(match_addr_acl);
        m_match_addr_acl.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_addr_acl()
    {
        m_match_addr_acl.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_addr_acl() const
    {
        return (m_match_addr_acl.getStringValue());
    }

/*
    void RouteMapLocalManagedObject::setMatch_precedence(const WellKnownPrecedence &wellKnownPrecedence) {
        m_well_known_precedence = wellKnownPrecedence;
    }

    void RouteMapLocalManagedObject::unsetMatch_precedence() {
        m_well_known_precedence = defaultVal;
    }

    WellKnownPrecedence RouteMapLocalManagedObject::getMatch_wellKnownPrecedence()  const {
        return (m_well_known_precedence);
    }
	*/

    void RouteMapLocalManagedObject::setMatch_dscpPrecedence(const DscpPrecedence &dscpPrecedence) {
        //m_dscp_precedence = dscpPrecedence;
		m_dscp_precedence.setUI32Value((UI32)dscpPrecedence);
		m_dscp_precedence.setIsUserConfigured(true);
    }

    void RouteMapLocalManagedObject::unsetMatch_dscpPrecedence() {
        //m_dscp_precedence = defaultValDscp;
		m_dscp_precedence.setIsUserConfigured(false);
    }

    DscpPrecedence RouteMapLocalManagedObject::getMatch_dscpPrecedence()  const {
        return (DscpPrecedence)(m_dscp_precedence.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_next_hop_peer_address(const bool &set_next_hop_peer_address)
    {
        //unsetSet_nh_ip();
        m_set_next_hop_peer_address.setBoolValue(set_next_hop_peer_address);
        m_set_next_hop_peer_address.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_next_hop_peer_address()
    {
        m_set_next_hop_peer_address.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_next_hop_peer_address() const
    {
        return (m_set_next_hop_peer_address.getBoolValue());
    }

    void RouteMapLocalManagedObject::setMatch_nullZero(const bool &nullZero) {
        m_match_nullZero.setBoolValue(nullZero);
        m_match_nullZero.setIsUserConfigured(true);
    }

    void RouteMapLocalManagedObject::unsetMatch_nullZero() {
        m_match_nullZero.setIsUserConfigured(false);
    }

    bool RouteMapLocalManagedObject::getMatch_nullZero()  const {
        return (m_match_nullZero.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setMatch_next_hop_address(const UI32 &match_next_hop_address)
    {
        m_match_next_hop_address.setUI32Value(match_next_hop_address);
        m_match_next_hop_address.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_next_hop_address()
    {
        m_match_next_hop_address.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getMatch_next_hop_address() const
    {
        return (m_match_next_hop_address.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setMatch_metric(const UI32 &match_metric)
    {
        m_match_metric.setUI32Value(match_metric);
        m_match_metric.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_metric()
    {
        m_match_metric.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getMatch_metric() const
    {
        return (m_match_metric.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setMatch_route_type(const PolicyRouteType &match_route_type)
    {
        m_match_route_type.setUI32Value((UI32)match_route_type);
        m_match_route_type.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_route_type()
    {
        m_match_route_type.setIsUserConfigured(false);
    }

    PolicyRouteType  RouteMapLocalManagedObject::getMatch_route_type() const
    {
        return (PolicyRouteType)(m_match_route_type.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setMatch_bgp_route_type(const BgpProtocolRouteType &match_bgp_route_type)
    {
        m_match_bgp_route_type.setUI32Value((UI32)match_bgp_route_type);
        m_match_bgp_route_type.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_bgp_route_type()
    {
        m_match_bgp_route_type.setIsUserConfigured(false);
    }

    BgpProtocolRouteType  RouteMapLocalManagedObject::getMatch_bgp_route_type() const
    {
        return (BgpProtocolRouteType)(m_match_bgp_route_type.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setMatch_protocol_bgp(const bool &match_protocol_bgp)
    {
        m_match_protocol_bgp.setBoolValue(match_protocol_bgp);
        m_match_protocol_bgp.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_protocol_bgp()
    {
        m_match_protocol_bgp.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getMatch_protocol_bgp() const
    {
        return m_match_protocol_bgp.getBoolValue();
    }


    void  RouteMapLocalManagedObject::setMatch_route_tag(const vector<UI32> &match_route_tag)
    {
        m_match_route_tag = match_route_tag;
        m_match_route_tag.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_route_tag()
    {
        m_match_route_tag.setIsUserConfigured(false);
    }

    vector<UI32>  RouteMapLocalManagedObject::getMatch_route_tag() const
    {
        return (m_match_route_tag.getUI32Vector());
    }

    void  RouteMapLocalManagedObject::setMatch_rs_prefix(const string &match_rs_prefix)
    {
        m_match_rs_prefix.setStringValue(match_rs_prefix);
        m_match_rs_prefix.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_rs_prefix()
    {
        m_match_rs_prefix.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_rs_prefix() const
    {
        return (m_match_rs_prefix.getStringValue());
    }

    void  RouteMapLocalManagedObject::setMatch_route_aspath(const string &match_route_aspath)
    {
        m_match_route_aspath = match_route_aspath;
        m_match_route_aspath.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_route_aspath()
    {
        m_match_route_aspath.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_route_aspath() const
    {
    	return m_match_route_aspath.getStringValue();
    }

    void  RouteMapLocalManagedObject::setSet_distance(const UI32 &set_distance)
    {
        m_set_distance.setUI32Value(set_distance);
        m_set_distance.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_distance()
    {
        m_set_distance.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_distance() const
    {
        return (m_set_distance.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_metric_type_external(const bool &set_route_metric_type_external)
    {
        m_set_route_metric_type_external.setBoolValue(set_route_metric_type_external);
        m_set_route_metric_type_external.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_metric_type_external()
    {
        m_set_route_metric_type_external.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_route_metric_type_external() const
    {
        return (m_set_route_metric_type_external.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_metric_type_internal(const bool &set_route_metric_type_internal)
    {
        m_set_route_metric_type_internal.setBoolValue(set_route_metric_type_internal);
        m_set_route_metric_type_internal.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_metric_type_internal()
    {
        m_set_route_metric_type_internal.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_route_metric_type_internal() const
    {
        return (m_set_route_metric_type_internal.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_metric_type_type_1(const bool &set_route_metric_type_type_1)
    {
        m_set_route_metric_type_type_1.setBoolValue(set_route_metric_type_type_1);
        m_set_route_metric_type_type_1.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_metric_type_type_1()
    {
        m_set_route_metric_type_type_1.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_route_metric_type_type_1() const
    {
        return (m_set_route_metric_type_type_1.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_metric_type_type_2(const bool &set_route_metric_type_type_2)
    {
        m_set_route_metric_type_type_2.setBoolValue(set_route_metric_type_type_2);
        m_set_route_metric_type_type_2.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_metric_type_type_2()
    {
        m_set_route_metric_type_type_2.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_route_metric_type_type_2() const
    {
        return (m_set_route_metric_type_type_2.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setSet_metric_action(const PolicyMetricAction &set_metric_action)
    {
        m_set_metric_action.setUI32Value((UI32)set_metric_action);
        m_set_metric_action.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_metric_action()
    {
        m_set_metric_action.setIsUserConfigured(false);
    }

    PolicyMetricAction  RouteMapLocalManagedObject::getSet_metric_action() const
    {
        return (PolicyMetricAction)(m_set_metric_action.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_metric(const UI32 &set_metric)
    {
        m_set_metric.setUI32Value(set_metric);
        m_set_metric.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_metric()
    {
        m_set_metric.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_metric() const
    {
        return (m_set_metric.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_type(const PolicyRouteType &set_route_type)
    {
        m_set_route_type.setUI32Value((UI32)set_route_type);
        m_set_route_type.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_type()
    {
        m_set_route_type.setIsUserConfigured(false);
    }

    PolicyRouteType  RouteMapLocalManagedObject::getSet_route_type() const
    {
        return (PolicyRouteType)(m_set_route_type.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_tag(const UI32 &set_route_tag)
    {
        m_set_route_tag.setUI32Value(set_route_tag);
        m_set_route_tag.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_tag()
    {
        m_set_route_tag.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_tag() const
    {
        return (m_set_route_tag.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_local_preference(const UI32 &set_route_local_preference)
    {
        m_set_route_local_preference.setUI32Value(set_route_local_preference);
        m_set_route_local_preference.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_local_preference()
    {
        m_set_route_local_preference.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_local_preference() const
    {
        return (m_set_route_local_preference.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_weight(const UI32 &set_route_weight)
    {
        m_set_route_weight.setUI32Value(set_route_weight);
        m_set_route_weight.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_weight()
    {
        m_set_route_weight.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_weight() const
    {
        return (m_set_route_weight.getUI32Value());
    }

	void  RouteMapLocalManagedObject::setRoute_continue(const UI32 &route_continue)
	{
			m_route_continue.setUI32Value(route_continue);
			m_route_continue.setIsUserConfigured(true);
	}

	void  RouteMapLocalManagedObject::unsetRoute_continue()
	{
		m_route_continue.setIsUserConfigured(false);
	}

	UI32  RouteMapLocalManagedObject::getRoute_continue() const
	{
		return (m_route_continue.getUI32Value());
	}

	void  RouteMapLocalManagedObject::setRoute_continue_no_val(const bool &route_continue_no_val)
	{
			m_route_continue_no_val.setBoolValue(route_continue_no_val);
			m_route_continue_no_val.setIsUserConfigured(true);
	}

	void  RouteMapLocalManagedObject::unsetRoute_continue_no_val()
	{
		m_route_continue_no_val.setIsUserConfigured(false);
	}

	bool  RouteMapLocalManagedObject::getRoute_continue_no_val() const
	{
		return (m_route_continue_no_val.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setSet_route_aspath_prepend(const string &set_route_aspath_prepend)
    {
    	unsetSet_route_aspath_tag();
        m_set_route_aspath_prepend.setStringValue(set_route_aspath_prepend);
        m_set_route_aspath_prepend.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_aspath_prepend()
    {
        m_set_route_aspath_prepend.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getSet_route_aspath_prepend() const
    {
        return (m_set_route_aspath_prepend.getStringValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_aspath_tag(const bool &set_route_aspath_tag)
	{
    	unsetSet_route_aspath_prepend();
		m_set_route_aspath_tag.setBoolValue(set_route_aspath_tag);
		m_set_route_aspath_tag.setIsUserConfigured(true);
	}

    void  RouteMapLocalManagedObject::unsetSet_route_aspath_tag()
	{
    	m_set_route_aspath_tag.setIsUserConfigured(false);
	}

    bool  RouteMapLocalManagedObject::getSet_route_aspath_tag() const
	{
    	return (m_set_route_aspath_tag.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setMatch_route_community_list(const string &match_route_community_list)
    {
        m_match_route_community_list.setStringValue(match_route_community_list);
        m_match_route_community_list.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetMatch_route_community_list()
    {
        m_match_route_community_list.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getMatch_route_community_list() const
    {
        return (m_match_route_community_list.getStringValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_origin_igp(const bool &set_route_origin_igp)
    {
    	unsetSet_route_origin_incomplete();
        m_set_route_origin_igp.setBoolValue(set_route_origin_igp);
        m_set_route_origin_igp.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_origin_igp()
    {
        m_set_route_origin_igp.setIsUserConfigured(false);
    }

    bool  RouteMapLocalManagedObject::getSet_route_origin_igp() const
    {
        return (m_set_route_origin_igp.getBoolValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_origin_incomplete(const bool &set_route_origin_incomplete)
	{
    	unsetSet_route_origin_igp();
		m_set_route_origin_incomplete.setBoolValue(set_route_origin_incomplete);
		m_set_route_origin_incomplete.setIsUserConfigured(true);
	}

    void  RouteMapLocalManagedObject::unsetSet_route_origin_incomplete()
	{
    	m_set_route_origin_incomplete.setIsUserConfigured(false);
	}

    bool  RouteMapLocalManagedObject::getSet_route_origin_incomplete() const
	{
    	return (m_set_route_origin_incomplete.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setSet_route_community_access_list(const string &set_route_community_access_list)
    {
        m_set_route_community_access_list.setStringValue(set_route_community_access_list);
        m_set_route_community_access_list.setIsUserConfigured(true);
        m_set_route_community_acl_delete.setBoolValue(true);
        m_set_route_community_acl_delete.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_community_access_list()
    {
		m_set_route_community_acl_delete.setIsUserConfigured(false);
        m_set_route_community_access_list.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getSet_route_community_access_list() const
    {
        return (m_set_route_community_access_list.getStringValue());
    }

    bool  RouteMapLocalManagedObject::getSet_route_community_acl_delete() const
	{
    	return (m_set_route_community_acl_delete.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setSet_route_community_expr(const string &set_route_community_expr)
    {
        m_set_route_community_expr.setStringValue(set_route_community_expr);
        m_set_route_community_expr.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_community_expr()
    {
        m_set_route_community_expr.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getSet_route_community_expr() const
    {
        return (m_set_route_community_expr.getStringValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_extcommunity_expr(const string &set_route_extcommunity_expr)
    {
        m_set_route_extcommunity_expr.setStringValue(set_route_extcommunity_expr);
        m_set_route_extcommunity_expr.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_extcommunity_expr()
    {
        m_set_route_extcommunity_expr.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getSet_route_extcommunity_expr() const
    {
        return (m_set_route_extcommunity_expr.getStringValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_extcommunity_soo_expr(const string &set_route_extcommunity_soo_expr)
    {
        m_set_route_extcommunity_soo_expr.setStringValue(set_route_extcommunity_soo_expr);
        m_set_route_extcommunity_soo_expr.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_extcommunity_soo_expr()
    {
        m_set_route_extcommunity_soo_expr.setIsUserConfigured(false);
    }

    string  RouteMapLocalManagedObject::getSet_route_extcommunity_soo_expr() const
    {
        return (m_set_route_extcommunity_soo_expr.getStringValue());
    }

    void  RouteMapLocalManagedObject::setSet_route_automatic_tag(const bool &set_route_automatic_tag)
	{
		m_set_route_automatic_tag.setBoolValue(set_route_automatic_tag);
		m_set_route_automatic_tag.setIsUserConfigured(true);
	}

    void  RouteMapLocalManagedObject::unsetSet_route_automatic_tag()
	{
    	m_set_route_automatic_tag.setIsUserConfigured(false);
	}

    bool  RouteMapLocalManagedObject::getSet_route_automatic_tag() const
	{
    	return (m_set_route_automatic_tag.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setSet_route_dampening(const bool &set_route_dampening)
	{
		m_set_route_dampening.setBoolValue(set_route_dampening);
		m_set_route_dampening.setIsUserConfigured(true);
	}

    void  RouteMapLocalManagedObject::unsetSet_route_dampening()
	{
    	m_set_route_dampening.setIsUserConfigured(false);
    	m_set_route_dampening_half_life.setIsUserConfigured(false);
        m_set_route_dampening_reuse.setIsUserConfigured(false);
        m_set_route_dampening_max_suppress.setIsUserConfigured(false);
        m_set_route_dampening_max_suppress.setIsUserConfigured(false);
	}

    bool  RouteMapLocalManagedObject::getSet_route_dampening() const
	{
    	return (m_set_route_dampening.getBoolValue());
	}

    void  RouteMapLocalManagedObject::setSet_route_dampening_half_life(const UI32 &set_route_dampening_half_life)
    {
        m_set_route_dampening_half_life.setUI32Value(set_route_dampening_half_life);
        m_set_route_dampening_half_life.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_dampening_half_life()
    {
        m_set_route_dampening_half_life.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_dampening_half_life() const
    {
        return (m_set_route_dampening_half_life.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_dampening_reuse(const UI32 &set_route_dampening_reuse)
    {
        m_set_route_dampening_reuse.setUI32Value(set_route_dampening_reuse);
        m_set_route_dampening_reuse.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_dampening_reuse()
    {
        m_set_route_dampening_reuse.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_dampening_reuse() const
    {
        return (m_set_route_dampening_reuse.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_dampening_suppress(const UI32 &set_route_dampening_suppress)
    {
        m_set_route_dampening_suppress.setUI32Value(set_route_dampening_suppress);
        m_set_route_dampening_suppress.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_dampening_suppress()
    {
        m_set_route_dampening_suppress.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_dampening_suppress() const
    {
        return (m_set_route_dampening_suppress.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setSet_route_dampening_max_suppress(const UI32 &set_route_dampening_max_suppress)
    {
        m_set_route_dampening_max_suppress.setUI32Value(set_route_dampening_max_suppress);
        m_set_route_dampening_max_suppress.setIsUserConfigured(true);
    }

    void  RouteMapLocalManagedObject::unsetSet_route_dampening_max_suppress()
    {
        m_set_route_dampening_max_suppress.setIsUserConfigured(false);
    }

    UI32  RouteMapLocalManagedObject::getSet_route_dampening_max_suppress() const
    {
        return (m_set_route_dampening_max_suppress.getUI32Value());
    }

    void  RouteMapLocalManagedObject::setPbrVrfConf(const vector<WaveManagedObjectPointer<PbrVrfManagedObject> > &pbrVrfConf)
    {
        m_pbrVrfMO  =  pbrVrfConf;
    }

    vector<WaveManagedObjectPointer<PbrVrfManagedObject> >  RouteMapLocalManagedObject::getPbrVrfConf() const
    {
        return (m_pbrVrfMO);
    }

    void RouteMapLocalManagedObject::addPbrVrfMOConfToVector(PbrVrfManagedObject *pbrVrfMO)
    {
        WaveManagedObjectPointer<PbrVrfManagedObject> pbrVrf (pbrVrfMO);
        m_pbrVrfMO.push_back(pbrVrf);
    }


    void RouteMapLocalManagedObject::delPbrVrfMOConfFromVector(string vrfName, IpV4Address nexthopAddr)
    {
        trace(TRACE_LEVEL_INFO, 
              string ("Entering RouteMapLocalManagedObject::delPbrVrfMOConfFromVector"));
        for (UI32 i = 0; i < m_pbrVrfMO.size(); i++) {
            if (((m_pbrVrfMO[i])->getVrfNameMO() == vrfName)
                && ((m_pbrVrfMO[i])->getSet_next_hop_address() == nexthopAddr)) {
                trace (TRACE_LEVEL_INFO, string ("RouteMapLocalManagedObject::delPbrVrfMOConfFromVector:deleting ")
                    + string("vrfName= ") + vrfName + string("NextHopAddr= ") + nexthopAddr.toString());
                m_pbrVrfMO.erase (m_pbrVrfMO.begin() + i);
                break;
            }
        }
        return;
    }

    void  RouteMapLocalManagedObject::setPbrNexthopConf(const vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject > > &pbrNexthopConf)
    {
          m_pbrNexthopAddrMO =  pbrNexthopConf;
    }

    vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject> >  RouteMapLocalManagedObject::getPbrNexthopConf() const
    {
        return (m_pbrNexthopAddrMO);
    }

    void RouteMapLocalManagedObject::addPbrNexthopMOConfToVector(PbrNexthopAddrManagedObject *pbrNexthopMO)
    {
        WaveManagedObjectPointer<PbrNexthopAddrManagedObject> pbrNexthop (pbrNexthopMO);
        m_pbrNexthopAddrMO.push_back(pbrNexthop);
    }


    void RouteMapLocalManagedObject::delPbrNexthopMOConfFromVector(IpV4Address nexthopAddr)
    {
        trace(TRACE_LEVEL_INFO,
              string ("Entering RouteMapLocalManagedObject::delPbrNexthopMOConfFromVector"));
        for (UI32 i = 0; i < m_pbrNexthopAddrMO.size(); i++) {
            if (((m_pbrNexthopAddrMO[i])->getSet_next_hop_address() == nexthopAddr)) {
                trace (TRACE_LEVEL_INFO, string ("RouteMapLocalManagedObject::delPbrNexthopMOConfFromVector:deleting ")
                    + string("NextHopAddr= ") + nexthopAddr.toString());
                m_pbrNexthopAddrMO.erase (m_pbrNexthopAddrMO.begin() + i);
                break;
            }
        }
        return;
    }

    void RouteMapLocalManagedObject::addPbrGlobalNexthopMOConfToVector(PbrGlobalNexthopManagedObject *pbrGlobalNexthopMO)
    {
        WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> pbrGlobalNexthop (pbrGlobalNexthopMO);
        m_pbrGlobalNexthopAddrMO.push_back(pbrGlobalNexthop);
    }

    void RouteMapLocalManagedObject::delPbrGlobalNexthopMOConfFromVector(IpV4Address nexthopAddr)
    {
        trace(TRACE_LEVEL_INFO,
              string ("Entering RouteMapLocalManagedObject::delPbrGlobalNexthopMOConfFromVector"));
        for (UI32 i = 0; i < m_pbrGlobalNexthopAddrMO.size(); i++) {
            if (((m_pbrGlobalNexthopAddrMO[i])->getSet_next_hop_address() == nexthopAddr)) {
                trace (TRACE_LEVEL_INFO, string ("RouteMapLocalManagedObject::delPbrGlobalNexthopMOConfFromVector:deleting ")
                    + string("NextHopAddr= ") + nexthopAddr.toString());
                m_pbrGlobalNexthopAddrMO.erase (m_pbrGlobalNexthopAddrMO.begin() + i);
                break;
            }
        }
        return;
    }

    void  RouteMapLocalManagedObject::setGlobalPbrNexthopConf(const vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject > > &pbrGlobalNexthopConf)
    {
          m_pbrGlobalNexthopAddrMO =  pbrGlobalNexthopConf;
    }

    vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> >  RouteMapLocalManagedObject::getPbrGlobalNexthopConf() const
    {
        return (m_pbrGlobalNexthopAddrMO);
    }

// ************* IpPrefixLocalManagedObject ************

    IpPrefixLocalManagedObject::IpPrefixLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
        : PrismElement  (pIpPolicyLocalObjectManager),
        PrismPersistableObject (IpPrefixLocalManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpPolicyLocalObjectManager),
        DcmManagedObject (pIpPolicyLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpPolicyLocalObjectManager)
    {
    }

    IpPrefixLocalManagedObject::IpPrefixLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const string &name,
        const UI32 &seq,
        const UI16 &instance,
        const PolicyAction &action,
        const IpV4AddressNetworkMask &ip_prefix,
        const UI16 &ge,
        const UI16 &le)
        : PrismElement  (pIpPolicyLocalObjectManager),
        PrismPersistableObject (IpPrefixLocalManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpPolicyLocalObjectManager),
        DcmManagedObject (pIpPolicyLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpPolicyLocalObjectManager),
        m_seq       (seq),
        m_instance    (instance),
        m_action    (action),
        m_ip_prefix    (ip_prefix),
        m_ge    (ge),
        m_le    (le)
    {
    }

    IpPrefixLocalManagedObject::~IpPrefixLocalManagedObject ()
    {
    }

    string  IpPrefixLocalManagedObject::getClassName()
    {
        return ("IpPrefixLocalManagedObject");
    }

    void  IpPrefixLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum(&m_seq,"Seq"));
        addPersistableAttribute (new AttributeUI16(&m_instance,"instance"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
        addPersistableAttribute (new AttributeIpV4AddressNetworkMask(&m_ip_prefix,"ip_prefix"));
        addPersistableAttribute (new AttributeUI32UC(&m_ge,true, "ge"));
        addPersistableAttribute (new AttributeUI32UC(&m_le,true,"le"));

        setUserTagForAttribute ("name", ip_policy_name);
        setUserTagForAttribute ("Seq", ip_policy_seq_keyword);
        setUserTagForAttribute ("instance", ip_policy_instance);
        setUserTagForAttribute("action", ip_policy_action_ipp);
        setUserTagForAttribute("ip_prefix", ip_policy_prefix_ipp);
        setUserTagForAttribute("ge", ip_policy_ge_ipp);
        setUserTagForAttribute("le", ip_policy_le_ipp);
    }

    void  IpPrefixLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum(&m_seq,"Seq"));  
        addPersistableAttributeForCreate  (new AttributeUI16(&m_instance,"instance"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_action,"action"));
        addPersistableAttributeForCreate  (new AttributeIpV4AddressNetworkMask(&m_ip_prefix,"ip_prefix"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_ge, true, "ge"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_le, true, "le"));

    }

    void  IpPrefixLocalManagedObject::setupKeys() {
        vector<string> keyName;
        keyName.push_back("name");
        keyName.push_back("Seq");
        keyName.push_back("instance");
        setUserDefinedKeyCombination (keyName);
    }

    void  IpPrefixLocalManagedObject::setInstance(const UI16 &instance)
    {
        m_seq = 1;
        m_instance = instance;
    }

    UI16  IpPrefixLocalManagedObject::getInstance() const
    {
        return (m_instance);
    }

    void  IpPrefixLocalManagedObject::setAction(const PolicyAction &action)
    {
        m_action = action;
    }

    PolicyAction  IpPrefixLocalManagedObject::getAction() const
    {
        return (m_action);
    }

    void  IpPrefixLocalManagedObject::setIp_prefix(const IpV4AddressNetworkMask &ip_prefix)
    {
        m_ip_prefix = ip_prefix;
    }

    IpV4AddressNetworkMask  IpPrefixLocalManagedObject::getIp_prefix() const
    {
        return (m_ip_prefix);
    }

    void  IpPrefixLocalManagedObject::setGe(UI32 ge)
    {
        m_ge.setUI32Value(ge);
        m_ge.setIsUserConfigured(true);
    }

    void  IpPrefixLocalManagedObject::unsetGe()
    {
        m_ge.setIsUserConfigured(false);
    }

    UI32  IpPrefixLocalManagedObject::getGe() const
    {
        return (m_ge.getUI32Value());
    }

    void  IpPrefixLocalManagedObject::setLe(UI32 le)
    {
        m_le.setUI32Value(le);
        m_le.setIsUserConfigured(true);
    }

    void  IpPrefixLocalManagedObject::unsetLe()
    {
        m_le.setIsUserConfigured(false);
    }

    UI32  IpPrefixLocalManagedObject::getLe() const
    {
        return (m_le.getUI32Value());
    }
}
