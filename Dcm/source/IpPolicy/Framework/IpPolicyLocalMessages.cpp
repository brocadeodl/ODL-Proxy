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

#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalMessages.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

// ************* IpPolicyRouteMapMessage ************

    IpPolicyRouteMapMessage::IpPolicyRouteMapMessage ()
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYROUTEMAP)
    {
        m_name_flag = false;
        m_instance_flag = false;
        m_action_flag = false;
        m_match_intf_eth_flag = false;
		m_match_intf_gi_flag = false;
		m_match_intf_fo_flag = false;
        m_match_intf_vlan_flag = false;
        m_match_intf_ve_flag = false;
        m_match_intf_loopback_flag = false;
        m_match_intf_po_flag = false;
        m_match_addr_prefix_flag = false;
        m_match_nh_prefix_flag = false;
        m_match_addr_acl_flag = false;
        m_nullZero_flag = false;
//        m_precedence_flag = false;
        m_dscp_precedence_flag = false;
        m_match_next_hop_address_flag = false;
        m_match_metric_flag = false;
        m_match_route_type_flag = false;
        m_match_route_tag_flag = false;
        m_match_rs_prefix_flag = false;
        m_match_route_aspath_flag = false;
        m_match_bgp_route_type_flag = false;
        m_match_protocol_bgp_flag = false;
        m_set_distance_flag = false;
        m_set_nh_ip_flag = false;
        m_set_metric_action_flag = false;
        m_set_metric_flag = false;
        m_set_route_type_flag = false;
        m_set_route_tag_flag = false;
        m_set_route_local_preference_flag = false;
        m_set_route_weight_flag = false;
        m_route_continue_flag = false;
        m_set_route_aspath_prepend_flag = false;
        m_set_route_aspath_tag_flag = false;
        m_match_route_community_list_flag = false;
        m_match_route_community_exact_match_flag = false;
        m_set_route_origin_igp_flag = false;
        m_set_route_origin_incomplete_flag = false;
        m_set_next_hop_peer_address_flag = false;
        m_set_route_metric_type_external_flag = false;
		m_set_route_metric_type_internal_flag = false;
		m_set_route_metric_type_type_1_flag = false;
		m_set_route_metric_type_type_2_flag = false;
        m_set_route_community_access_list_flag = false;
        m_set_route_community_acl_delete_flag = false;
        m_set_route_automatic_tag_flag = false;
        m_set_route_dampening_flag = false;
        m_set_route_dampening_half_life_flag = false;
        m_set_route_dampening_reuse_flag = false;
        m_set_route_dampening_suppress_flag = false;
        m_set_route_dampening_max_suppress_flag = false;
        m_set_route_community_expr_flag = false;
        m_set_route_extcommunity_expr_flag = false;
        m_set_route_extcommunity_soo_expr_flag = false;
        m_deleteAllIntfOfType = IF_TYPE_INVALID;
    }


    IpPolicyRouteMapMessage::~IpPolicyRouteMapMessage ()
    {
    }

    void  IpPolicyRouteMapMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI16(&m_instance,"instance"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
        addSerializableAttribute (new AttributeStringVector(&m_match_intf_eth,"match_intf_eth"));
        addSerializableAttribute (new AttributeStringVector(&m_match_intf_gi,"match_intf_gi"));
        addSerializableAttribute (new AttributeStringVector(&m_match_intf_fo,"match_intf_fo"));
        addSerializableAttribute (new AttributeUI32Vector(&m_match_intf_vlan,"match_intf_vlan"));
        addSerializableAttribute (new AttributeUI32Vector(&m_match_intf_po,"match_intf_po"));
        addSerializableAttribute (new AttributeUI32Vector(&m_match_intf_ve,"match_intf_ve"));
        addSerializableAttribute (new AttributeUI32Vector(&m_match_intf_loopback,"match_intf_loopback"));
        addSerializableAttribute (new AttributeString(&m_match_addr_prefix,"match_addr_prefix"));
		addSerializableAttribute (new AttributeString(&m_match_addr_acl,"match_addr_acl"));
        addSerializableAttribute (new AttributeString(&m_match_nh_prefix,"match_nh_prefix"));
//        addSerializableAttribute (new AttributeEnum((UI32*)&m_well_known_precedence,"wellKnownPrecedence"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_dscp_precedence,"dscpPrecedence"));
        addSerializableAttribute (new AttributeBool(&m_nullZero,"nullZero"));
        addSerializableAttribute (new AttributeUI32(&m_match_next_hop_address,"match_next_hop_address"));
        addSerializableAttribute (new AttributeUI32(&m_match_metric,"match_metric"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_match_route_type,"match_route_type"));
        addSerializableAttribute (new AttributeUI32Vector(&m_match_route_tag,"match_route_tag"));
        addSerializableAttribute (new AttributeString(&m_match_rs_prefix,"match_rs_prefix"));
        addSerializableAttribute (new AttributeString(&m_match_route_aspath,"match_route_aspath"));
        addSerializableAttribute (new AttributeBool(&m_match_protocol_bgp,"match_protocol_bgp"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_match_bgp_route_type,"match_bgp_route_type"));
        addSerializableAttribute (new AttributeUI32(&m_set_distance,"set_distance"));
        addSerializableAttribute (new AttributeIpV4Address(&m_set_nh_ip,"set_nh_ip"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_set_metric_action,"set_metric_action"));
        addSerializableAttribute (new AttributeUI32(&m_set_metric,"set_metric"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_set_route_type,"set_route_type"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_tag,"set_route_tag"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_local_preference,"set_route_local_preference"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_weight,"set_route_weight"));
        addSerializableAttribute (new AttributeUI32(&m_route_continue,"route_continue"));
        addSerializableAttribute (new AttributeString(&m_set_route_aspath_prepend,"set_route_aspath_prepend"));
        addSerializableAttribute (new AttributeBool(&m_set_route_aspath_tag,"set_route_aspath_tag"));
        addSerializableAttribute (new AttributeString(&m_match_route_community_list,"match_route_community_list"));
        addSerializableAttribute (new AttributeBool(&m_match_route_community_exact_match,"match_route_community_exact_match"));
        addSerializableAttribute (new AttributeBool(&m_set_route_origin_igp,"set_route_origin_igp"));
        addSerializableAttribute (new AttributeBool(&m_set_route_origin_incomplete,"set_route_origin_incomplete"));
        addSerializableAttribute (new AttributeBool(&m_set_next_hop_peer_address,"set_next_hop_peer_address"));
        addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_external,"set_route_metric_type_external"));
		addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_internal,"set_route_metric_type_internal"));
		addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_type_1,"set_route_metric_type_type_1"));
		addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_type_2,"set_route_metric_type_type_2"));
        addSerializableAttribute (new AttributeString(&m_set_route_community_access_list,"set_route_community_access_list"));
        addSerializableAttribute (new AttributeBool(&m_set_route_community_acl_delete,"set_route_community_acl_delete"));
        addSerializableAttribute (new AttributeBool(&m_set_route_automatic_tag,"set_route_automatic_tag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_dampening,"set_route_dampening"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_dampening_half_life,"set_route_dampening_half_life"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_dampening_reuse,"set_route_dampening_reuse"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_dampening_suppress,"set_route_dampening_suppress"));
        addSerializableAttribute (new AttributeUI32(&m_set_route_dampening_max_suppress,"set_route_dampening_max_suppress"));
        addSerializableAttribute (new AttributeString(&m_set_route_community_expr,"set_route_community_expr"));
        addSerializableAttribute (new AttributeString(&m_set_route_extcommunity_expr,"set_route_extcommunity_expr"));
        addSerializableAttribute (new AttributeString(&m_set_route_extcommunity_soo_expr,"set_route_extcommunity_soo_expr"));
       	addSerializableAttribute (new AttributeBool(&m_name_flag,"name_flag"));
       	addSerializableAttribute (new AttributeBool(&m_instance_flag,"instance_flag"));
       	addSerializableAttribute (new AttributeBool(&m_action_flag,"action_flag"));
       	addSerializableAttribute (new AttributeBool(&m_match_intf_eth_flag,"match_intf_eth_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_intf_gi_flag,"match_intf_gi_flag"));
 		addSerializableAttribute (new AttributeBool(&m_match_intf_fo_flag,"match_intf_fo_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_intf_vlan_flag,"match_intf_vlan_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_intf_po_flag,"match_intf_po_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_intf_ve_flag,"match_intf_ve_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_intf_loopback_flag,"match_intf_loopback_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_addr_prefix_flag,"match_addr_prefix_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_nh_prefix_flag,"match_nh_prefix_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_addr_acl_flag,"match_addr_acl_flag"));
        addSerializableAttribute (new AttributeBool(&m_nullZero_flag,"nullZeroFlag"));
//        addSerializableAttribute (new AttributeBool(&m_precedence_flag,"precedenceFlag"));
        addSerializableAttribute (new AttributeBool(&m_dscp_precedence_flag,"dscpPrecedenceFlag"));
        addSerializableAttribute (new AttributeBool(&m_match_next_hop_address_flag,"match_next_hop_address_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_metric_flag,"match_metrici_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_route_type_flag,"match_route_type_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_route_tag_flag,"match_route_tag_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_route_aspath_flag,"match_route_aspath_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_bgp_route_type_flag,"match_bgp_route_type_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_protocol_bgp_flag,"match_protocol_bgp_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_distance_flag,"set_distance_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_nh_ip_flag,"set_nh_ip_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_next_hop_peer_address_flag,"set_next_hop_peer_address_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_external_flag,"set_route_metric_type_external_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_internal_flag,"set_route_metric_type_internal_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_type_1_flag,"set_route_metric_type_type_1_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_metric_type_type_2_flag,"set_route_metric_type_type_2_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_metric_action_flag,"set_metric_action_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_metric_flag,"set_metric_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_type_flag,"set_route_type_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_tag_flag,"set_route_tag_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_local_preference_flag,"set_route_local_preference_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_weight_flag,"set_route_weight_flag"));
        addSerializableAttribute (new AttributeBool(&m_route_continue_flag,"route_continue_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_aspath_prepend_flag,"set_route_aspath_prepend_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_aspath_tag_flag,"set_route_aspath_tag_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_automatic_tag_flag,"set_route_automatic_tag_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_dampening_half_life_flag,"set_route_dampening_half_life_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_dampening_reuse_flag,"set_route_dampening_reuse_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_dampening_suppress_flag,"set_route_dampening_suppress_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_dampening_max_suppress_flag,"set_route_dampening_max_suppress_flag"));

        addSerializableAttribute (new AttributeBool(&m_match_rs_prefix_flag,"match_rs_prefix_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_route_community_list_flag,"match_route_community_list_flag"));
        addSerializableAttribute (new AttributeBool(&m_match_route_community_exact_match_flag,"match_route_community_exact_match_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_origin_igp_flag,"set_route_origin_igp_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_origin_incomplete_flag,"set_route_origin_incomplete_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_community_expr_flag,"set_route_community_expr_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_community_acl_delete_flag,"set_route_community_acl_delete_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_community_access_list_flag,"set_route_community_access_list_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_extcommunity_expr_flag,"set_route_extcommunity_expr_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_route_extcommunity_soo_expr_flag,"set_route_extcommunity_soo_expr_flag"));
        addSerializableAttribute (new AttributeString(&m_backend_err,"backend_err"));
        addSerializableAttribute (new AttributeUI8(&m_deleteAllIntfOfType, "deleteAllIntfOfType"));
    }

    void  IpPolicyRouteMapMessage::setName(const string &name)
    {
        m_name = name;
        m_name_flag = true;
    }

    string  IpPolicyRouteMapMessage::getName() const
    {
        return (m_name);
    }

    bool  IpPolicyRouteMapMessage::getNameFlag() const
    {
        return (m_name_flag);
    }

    void  IpPolicyRouteMapMessage::setInstance(const UI16 &instance)
    {
        m_instance = instance;
        m_instance_flag = true;
    }

    UI16  IpPolicyRouteMapMessage::getInstance() const
    {
        return (m_instance);
    }

    bool  IpPolicyRouteMapMessage::getInstanceFlag() const
    {
        return (m_instance_flag);
    }

    void  IpPolicyRouteMapMessage::setAction(const PolicyAction &action)
    {
        m_action = action;
        m_action_flag = true;
    }

    PolicyAction  IpPolicyRouteMapMessage::getAction() const
    {
        return (m_action);
    }

    bool  IpPolicyRouteMapMessage::getActionFlag() const
    {
        return (m_action_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_eth(const vector<string> &match_intf_eth)
    {
        m_match_intf_eth = match_intf_eth;
        m_match_intf_eth_flag = true;
    }

    vector<string>  IpPolicyRouteMapMessage::getMatch_intf_eth() const
    {
        return (m_match_intf_eth);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_ethFlag() const
    {
        return (m_match_intf_eth_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_gi(const vector<string> &match_intf_gi)
    {
        m_match_intf_gi = match_intf_gi;
        m_match_intf_gi_flag = true;
    }

    vector<string>  IpPolicyRouteMapMessage::getMatch_intf_gi() const
    {
        return (m_match_intf_gi);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_giFlag() const
    {
        return (m_match_intf_gi_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_fo(const vector<string> &match_intf_fo)
    {
        m_match_intf_fo = match_intf_fo;
        m_match_intf_fo_flag = true;
    }

    vector<string>  IpPolicyRouteMapMessage::getMatch_intf_fo() const
    {
        return (m_match_intf_fo);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_foFlag() const
    {
        return (m_match_intf_fo_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_vlan(const vector<UI32> &match_intf_vlan)
    {
        m_match_intf_vlan = match_intf_vlan;
        m_match_intf_vlan_flag = true;
    }

    vector<UI32>  IpPolicyRouteMapMessage::getMatch_intf_vlan() const
    {
        return (m_match_intf_vlan);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_vlanFlag() const
    {
        return (m_match_intf_vlan_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_po(const vector<UI32> &match_intf_po)
    {
        m_match_intf_po = match_intf_po;
        m_match_intf_po_flag = true;
    }

    vector<UI32>  IpPolicyRouteMapMessage::getMatch_intf_po() const
    {
        return (m_match_intf_po);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_poFlag() const
    {
        return (m_match_intf_po_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_ve(const vector<UI32> &match_intf_ve)
    {
        m_match_intf_ve = match_intf_ve;
        m_match_intf_ve_flag = true;
    }

    vector<UI32>  IpPolicyRouteMapMessage::getMatch_intf_ve() const
    {
        return (m_match_intf_ve);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_veFlag() const
    {
        return (m_match_intf_ve_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_intf_loopback(const vector<UI32> &match_intf_loopback)
    {
        m_match_intf_loopback = match_intf_loopback;
        m_match_intf_loopback_flag = true;
    }

    vector<UI32>  IpPolicyRouteMapMessage::getMatch_intf_loopback() const
    {
        return (m_match_intf_loopback);
    }

    bool  IpPolicyRouteMapMessage::getMatch_intf_loopbackFlag() const
    {
        return (m_match_intf_loopback_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_addr_prefix(const string &match_addr_prefix)
    {
        m_match_addr_prefix = match_addr_prefix;
        m_match_addr_prefix_flag = true;
    }

    void  IpPolicyRouteMapMessage::setMatch_addr_acl(const string &match_addr_acl)
    {
        m_match_addr_acl = match_addr_acl;
        m_match_addr_acl_flag = true;
    }

    string  IpPolicyRouteMapMessage::getMatch_addr_prefix() const
    {
        return (m_match_addr_prefix);
    }

    bool  IpPolicyRouteMapMessage::getMatch_addr_prefixFlag() const
    {
        return (m_match_addr_prefix_flag);
    }

    bool  IpPolicyRouteMapMessage::getMatch_addr_aclFlag() const
    {
        return (m_match_addr_acl_flag);
    }

    string  IpPolicyRouteMapMessage::getMatch_addr_acl() const
    {
        return (m_match_addr_acl);
    }

    void  IpPolicyRouteMapMessage::setMatch_nh_prefix(const string &match_nh_prefix)
    {
        m_match_nh_prefix = match_nh_prefix;
        m_match_nh_prefix_flag = true;
    }

    string  IpPolicyRouteMapMessage::getMatch_nh_prefix() const
    {
        return (m_match_nh_prefix);
    }

    bool  IpPolicyRouteMapMessage::getMatch_nh_prefixFlag() const
    {
        return (m_match_nh_prefix_flag);
    }

      void IpPolicyRouteMapMessage::setMatch_nullZero(const bool &nullZero) {
          m_nullZero = nullZero;
          m_nullZero_flag = true;
      }

      bool IpPolicyRouteMapMessage::getMatch_nullZero()  const {
          return (m_nullZero);
      }

      bool IpPolicyRouteMapMessage::getMatch_nullZeroFlag()  const {
          return (m_nullZero_flag);
      }

	  /*
      void IpPolicyRouteMapMessage::setMatch_precedence(WellKnownPrecedence wellKnownPrecedence) {
          m_well_known_precedence = wellKnownPrecedence;
          m_precedence_flag = true;
      }

      WellKnownPrecedence IpPolicyRouteMapMessage::getMatch_precedence()  const {
          return (m_well_known_precedence);
      }

      bool IpPolicyRouteMapMessage::getMatch_precedenceFlag()  const {
          return m_precedence_flag;
      }
	  */

      void IpPolicyRouteMapMessage::setMatch_dscpPrecedence(DscpPrecedence dscpPrecedence) {
          m_dscp_precedence = dscpPrecedence;
          m_dscp_precedence_flag = true;
      }

      DscpPrecedence IpPolicyRouteMapMessage::getMatch_dscpPrecedence()  const {
          return (m_dscp_precedence);
      }

      bool IpPolicyRouteMapMessage::getMatch_dscpPrecedenceFlag()  const {
          return m_dscp_precedence_flag;
      }


    void  IpPolicyRouteMapMessage::setMatch_next_hop_address(const UI32 &match_next_hop_address)
    {
        m_match_next_hop_address = match_next_hop_address;
        m_match_next_hop_address_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getMatch_next_hop_address() const
    {
        return (m_match_next_hop_address);
    }

    bool  IpPolicyRouteMapMessage::getMatch_next_hop_addressFlag() const
    {
        return (m_match_next_hop_address_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_metric(const UI32& match_metric)
    {
        m_match_metric = match_metric;
        m_match_metric_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getMatch_metric() const
    {
        return (m_match_metric);
    }

    bool  IpPolicyRouteMapMessage::getMatch_metricFlag() const
    {
        return (m_match_metric_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_route_type(const PolicyRouteType &match_route_type)
    {
        m_match_route_type = match_route_type;
        m_match_route_type_flag = true;
    }

    PolicyRouteType  IpPolicyRouteMapMessage::getMatch_route_type() const
    {
        return (m_match_route_type);
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_typeFlag() const
    {
        return (m_match_route_type_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_bgp_route_type(const BgpProtocolRouteType &match_bgp_route_type)
    {
    	setMatch_protocol_bgp(true);
        m_match_bgp_route_type = match_bgp_route_type;
        m_match_bgp_route_type_flag = true;
    }

    BgpProtocolRouteType  IpPolicyRouteMapMessage::getMatch_bgp_route_type() const
    {
        return (m_match_bgp_route_type);
    }

    bool  IpPolicyRouteMapMessage::getMatch_bgp_route_typeFlag() const
    {
        return (m_match_bgp_route_type_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_protocol_bgp(const bool &match_protocol_bgp)
    {
        m_match_protocol_bgp = match_protocol_bgp;
        m_match_protocol_bgp_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getMatch_protocol_bgp() const
    {
        return (m_match_protocol_bgp);
    }

    bool  IpPolicyRouteMapMessage::getMatch_protocol_bgpFlag() const
    {
        return (m_match_protocol_bgp_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_route_tag(const vector<UI32> &match_route_tag)
    {
        m_match_route_tag = match_route_tag;
        m_match_route_tag_flag = true;
    }

    vector<UI32>  IpPolicyRouteMapMessage::getMatch_route_tag() const
    {
        return (m_match_route_tag);
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_tagFlag() const
    {
        return (m_match_route_tag_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_rs_prefix(const string &match_rs_prefix)
    {
        m_match_rs_prefix = match_rs_prefix;
        m_match_rs_prefix_flag = true;
    }

    string  IpPolicyRouteMapMessage::getMatch_rs_prefix() const
    {
        return (m_match_rs_prefix);
    }

    bool  IpPolicyRouteMapMessage::getMatch_rs_prefixFlag() const
    {
        return (m_match_rs_prefix_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_route_aspath(const string &match_route_aspath)
    {
        m_match_route_aspath = match_route_aspath;
        m_match_route_aspath_flag = true;
    }

    string  IpPolicyRouteMapMessage::getMatch_route_aspath() const
    {
        return m_match_route_aspath;
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_aspathFlag() const
    {
        return (m_match_route_aspath_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_distance(const UI32 &set_distance)
    {
        m_set_distance = set_distance;
        m_set_distance_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_distance() const
    {
        return (m_set_distance);
    }

    bool  IpPolicyRouteMapMessage::getSet_distanceFlag() const
    {
        return (m_set_distance_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_nh_ip(const IpV4Address &set_nh_ip)
    {
        m_set_nh_ip = set_nh_ip;
        m_set_nh_ip_flag = true;
    }

    IpV4Address  IpPolicyRouteMapMessage::getSet_nh_ip() const
    {
        return (m_set_nh_ip);
    }

    bool  IpPolicyRouteMapMessage::getSet_nh_ipFlag() const
    {
        return (m_set_nh_ip_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_metric_action(const PolicyMetricAction &set_metric_action)
    {
        m_set_metric_action = set_metric_action;
        m_set_metric_action_flag = true;
    }

    PolicyMetricAction  IpPolicyRouteMapMessage::getSet_metric_action() const
    {
        return (m_set_metric_action);
    }

    bool  IpPolicyRouteMapMessage::getSet_metric_actionFlag() const
    {
        return (m_set_metric_action_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_metric(const UI32 &set_metric)
    {
        m_set_metric = set_metric;
        m_set_metric_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_metric() const
    {
        return (m_set_metric);
    }

    bool  IpPolicyRouteMapMessage::getSet_metricFlag() const
    {
        return (m_set_metric_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_type(const PolicyRouteType &set_route_type)
    {
        m_set_route_type = set_route_type;
        m_set_route_type_flag = true;
    }

    PolicyRouteType  IpPolicyRouteMapMessage::getSet_route_type() const
    {
        return (m_set_route_type);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_typeFlag() const
    {
        return (m_set_route_type_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_tag(const UI32 &set_route_tag)
    {
        m_set_route_tag = set_route_tag;
        m_set_route_tag_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_tag() const
    {
        return (m_set_route_tag);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_tagFlag() const
    {
        return (m_set_route_tag_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_local_preference(const UI32 &set_route_local_preference)
    {
        m_set_route_local_preference = set_route_local_preference;
        m_set_route_local_preference_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_local_preference() const
    {
        return (m_set_route_local_preference);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_local_preferenceFlag() const
    {
        return (m_set_route_local_preference_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_weight(const UI32 &set_route_weight)
    {
        m_set_route_weight = set_route_weight;
        m_set_route_weight_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_weight() const
    {
        return (m_set_route_weight);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_weightFlag() const
    {
        return (m_set_route_weight_flag);
    }

    void  IpPolicyRouteMapMessage::setRoute_continue(const UI32 &route_continue)
    {
        m_route_continue = route_continue;
        m_route_continue_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getRoute_continue() const
    {
        return (m_route_continue);
    }

    bool  IpPolicyRouteMapMessage::getRoute_continueFlag() const
    {
        return (m_route_continue_flag);
    }

    void  IpPolicyRouteMapMessage::setRoute_continueFlag(const bool &route_continue_flag)
    {
    	m_route_continue_flag = route_continue_flag;
    }

    void  IpPolicyRouteMapMessage::setSet_route_dampening_half_life(const UI32 &set_route_dampening_half_life)
    {
        m_set_route_dampening_half_life = set_route_dampening_half_life;
        m_set_route_dampening_half_life_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_dampening_half_life() const
    {
        return (m_set_route_dampening_half_life);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_dampening_half_lifeFlag() const
    {
        return (m_set_route_dampening_half_life_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_dampening_reuse(const UI32 &set_route_dampening_reuse)
    {
        m_set_route_dampening_reuse = set_route_dampening_reuse;
        m_set_route_dampening_reuse_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_dampening_reuse() const
    {
        return (m_set_route_dampening_reuse);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_dampening_reuseFlag() const
    {
        return (m_set_route_dampening_reuse_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_dampening_suppress(const UI32 &set_route_dampening_suppress)
    {
        m_set_route_dampening_suppress = set_route_dampening_suppress;
        m_set_route_dampening_suppress_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_dampening_suppress() const
    {
        return (m_set_route_dampening_suppress);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_dampening_suppressFlag() const
    {
        return (m_set_route_dampening_suppress_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_dampening_max_suppress(const UI32 &set_route_dampening_max_suppress)
    {
        m_set_route_dampening_max_suppress = set_route_dampening_max_suppress;
        m_set_route_dampening_max_suppress_flag = true;
    }

    UI32  IpPolicyRouteMapMessage::getSet_route_dampening_max_suppress() const
    {
        return (m_set_route_dampening_max_suppress);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_dampening_max_suppressFlag() const
    {
        return (m_set_route_dampening_max_suppress_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_aspath_prepend(const string &set_route_aspath_prepend)
    {
        m_set_route_aspath_prepend = set_route_aspath_prepend;
        m_set_route_aspath_prepend_flag = true;
    }

    string  IpPolicyRouteMapMessage::getSet_route_aspath_prepend() const
    {
        return (m_set_route_aspath_prepend);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_aspath_prependFlag() const
    {
        return (m_set_route_aspath_prepend_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_aspath_tag(const bool &set_route_aspath_tag)
    {
        m_set_route_aspath_tag = set_route_aspath_tag;
        m_set_route_aspath_tag_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_aspath_tag() const
    {
        return (m_set_route_aspath_tag);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_aspath_tagFlag() const
    {
        return (m_set_route_aspath_tag_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_route_community_list(const string &match_route_community_list)
    {
        m_match_route_community_list = match_route_community_list;
        m_match_route_community_list_flag = true;
    }

    string  IpPolicyRouteMapMessage::getMatch_route_community_list() const
    {
        return (m_match_route_community_list);
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_community_listFlag() const
    {
        return (m_match_route_community_list_flag);
    }

    void  IpPolicyRouteMapMessage::setMatch_route_community_exact_match(const bool &match_route_community_exact_match)
    {
        m_match_route_community_exact_match = match_route_community_exact_match;
        m_match_route_community_exact_match_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_community_exact_match() const
    {
        return (m_match_route_community_exact_match);
    }

    bool  IpPolicyRouteMapMessage::getMatch_route_community_exact_matchFlag() const
    {
        return (m_match_route_community_exact_match_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_origin_igp(const bool &set_route_origin_igp)
    {
        m_set_route_origin_igp = set_route_origin_igp;
        m_set_route_origin_igp_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_origin_igp() const
    {
        return (m_set_route_origin_igp);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_origin_igpFlag() const
    {
        return (m_set_route_origin_igp_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_origin_incomplete(const bool &set_route_origin_incomplete)
    {
        m_set_route_origin_incomplete = set_route_origin_incomplete;
        m_set_route_origin_incomplete_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_origin_incomplete() const
    {
        return (m_set_route_origin_incomplete);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_origin_incompleteFlag() const
    {
        return (m_set_route_origin_incomplete_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_next_hop_peer_address(const bool &set_next_hop_peer_address)
    {
        m_set_next_hop_peer_address = set_next_hop_peer_address;
        m_set_next_hop_peer_address_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_next_hop_peer_address() const
    {
        return (m_set_next_hop_peer_address);
    }

    bool  IpPolicyRouteMapMessage::getSet_next_hop_peer_addressFlag() const
    {
        return (m_set_next_hop_peer_address_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_metric_type_external(const bool &set_route_metric_type_external)
    {
        m_set_route_metric_type_external = set_route_metric_type_external;
        m_set_route_metric_type_external_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_external() const
    {
        return (m_set_route_metric_type_external);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_externalFlag() const
    {
        return (m_set_route_metric_type_external_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_metric_type_internal(const bool &set_route_metric_type_internal)
    {
        m_set_route_metric_type_internal = set_route_metric_type_internal;
        m_set_route_metric_type_internal_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_internal() const
    {
        return (m_set_route_metric_type_internal);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_internalFlag() const
    {
        return (m_set_route_metric_type_internal_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_metric_type_type_1(const bool &set_route_metric_type_type_1)
    {
        m_set_route_metric_type_type_1 = set_route_metric_type_type_1;
        m_set_route_metric_type_type_1_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_type_1() const
    {
        return (m_set_route_metric_type_type_1);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_type_1Flag() const
    {
        return (m_set_route_metric_type_type_1_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_metric_type_type_2(const bool &set_route_metric_type_type_2)
    {
        m_set_route_metric_type_type_2 = set_route_metric_type_type_2;
        m_set_route_metric_type_type_2_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_type_2() const
    {
        return (m_set_route_metric_type_type_2);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_metric_type_type_2Flag() const
    {
        return (m_set_route_metric_type_type_2_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_community_access_list(const string &set_route_community_access_list)
    {
        m_set_route_community_access_list = set_route_community_access_list;
        m_set_route_community_access_list_flag = true;
    }

    string  IpPolicyRouteMapMessage::getSet_route_community_access_list() const
    {
        return (m_set_route_community_access_list);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_community_access_listFlag() const
    {
        return (m_set_route_community_access_list_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_community_expr(const string &set_route_community_expr)
    {
        m_set_route_community_expr = set_route_community_expr;
        m_set_route_community_expr_flag = true;
    }

    string  IpPolicyRouteMapMessage::getSet_route_community_expr() const
    {
        return (m_set_route_community_expr);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_community_exprFlag() const
    {
        return (m_set_route_community_expr_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_extcommunity_expr(const string &set_route_extcommunity_expr)
    {
        m_set_route_extcommunity_expr = set_route_extcommunity_expr;
        m_set_route_extcommunity_expr_flag = true;
    }

    string  IpPolicyRouteMapMessage::getSet_route_extcommunity_expr() const
    {
        return (m_set_route_extcommunity_expr);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_extcommunity_exprFlag() const
    {
        return (m_set_route_extcommunity_expr_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_extcommunity_soo_expr(const string &set_route_extcommunity_soo_expr)
    {
        m_set_route_extcommunity_soo_expr = set_route_extcommunity_soo_expr;
        m_set_route_extcommunity_soo_expr_flag = true;
    }

    string  IpPolicyRouteMapMessage::getSet_route_extcommunity_soo_expr() const
    {
        return (m_set_route_extcommunity_soo_expr);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_extcommunity_soo_exprFlag() const
    {
        return (m_set_route_extcommunity_soo_expr_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_community_acl_delete(const bool &set_route_community_acl_delete)
    {
        m_set_route_community_acl_delete = set_route_community_acl_delete;
        m_set_route_community_acl_delete_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_community_acl_delete() const
    {
        return (m_set_route_community_acl_delete);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_community_acl_deleteFlag() const
    {
        return (m_set_route_community_acl_delete_flag);
    }

    void  IpPolicyRouteMapMessage::setSet_route_automatic_tag(const bool &set_route_automatic_tag)
    {
        m_set_route_automatic_tag = set_route_automatic_tag;
        m_set_route_automatic_tag_flag = true;
    }

    bool  IpPolicyRouteMapMessage::getSet_route_automatic_tag() const
    {
        return (m_set_route_automatic_tag);
    }

    bool  IpPolicyRouteMapMessage::getSet_route_automatic_tagFlag() const
    {
        return (m_set_route_automatic_tag_flag);
    }

	void  IpPolicyRouteMapMessage::setSet_route_dampening(const bool &set_route_dampening)
	{
	   m_set_route_dampening = set_route_dampening;
	   m_set_route_dampening_flag = true;
	}

	bool  IpPolicyRouteMapMessage::getSet_route_dampening() const
	{
	   return (m_set_route_dampening);
	}

	bool  IpPolicyRouteMapMessage::getSet_route_dampeningFlag() const
	{
	   return (m_set_route_dampening_flag);
	}

    void IpPolicyRouteMapMessage::setIfTypeToDeleteAllIntfs(UI8 ifType) {
    	m_deleteAllIntfOfType = ifType;
    }

    UI8 IpPolicyRouteMapMessage::getIfTypeToDeleteAllIntfs() const {
    	return m_deleteAllIntfOfType;
    }

// ************* IpPolicyIpPrefixMessage ************

    IpPolicyIpPrefixMessage::IpPolicyIpPrefixMessage ()
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYIPPREFIX)
    {
        m_name_flag = false;
        m_instance_flag = false;
        m_action_flag = false;
        m_ip_prefix_flag = false;
        m_ge_flag = false;
        m_le_flag = false;
    }

    IpPolicyIpPrefixMessage::IpPolicyIpPrefixMessage (const string &name,
        const UI16 &instance,
        const PolicyAction &action,
        const IpV4AddressNetworkMask &ip_prefix,
        const UI16 &ge,
        const UI16 &le)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYIPPREFIX),
        m_name    (name),
        m_instance    (instance),
        m_action    (action),
        m_ip_prefix    (ip_prefix),
        m_ge    (ge),
        m_le    (le)
    {
        m_name_flag = true;
        m_instance_flag = true;
        m_action_flag = true;
        m_ip_prefix_flag = true;
        m_ge_flag = true;
        m_le_flag = true;
    }

    IpPolicyIpPrefixMessage::~IpPolicyIpPrefixMessage ()
    {
    }

    void  IpPolicyIpPrefixMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI16(&m_instance,"instance"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_ip_prefix,"ip_prefix"));
        addSerializableAttribute (new AttributeUI16(&m_ge,"ge"));
        addSerializableAttribute (new AttributeUI16(&m_le,"le"));
        addSerializableAttribute (new AttributeString(&m_backend_err,"backend_err"));
        addSerializableAttribute (new AttributeBool(&m_name_flag,"name_flag"));
        addSerializableAttribute (new AttributeBool(&m_instance_flag,"instance_flag"));
        addSerializableAttribute (new AttributeBool(&m_action_flag,"action_flag"));
        addSerializableAttribute (new AttributeBool(&m_ip_prefix_flag,"ip_prefix_flag"));
        addSerializableAttribute (new AttributeBool(&m_ge_flag,"ge_flag"));
        addSerializableAttribute (new AttributeBool(&m_le_flag,"le_flag"));
    }

    void  IpPolicyIpPrefixMessage::setName(const string &name)
    {
        m_name = name;
        m_name_flag = true;
    }

    string  IpPolicyIpPrefixMessage::getName() const
    {
        return (m_name);
    }

    bool  IpPolicyIpPrefixMessage::getNameFlag() const
    {
        return (m_name_flag);
    }

    void  IpPolicyIpPrefixMessage::setInstance(const UI16 &instance)
    {
        m_instance = instance;
        m_instance_flag = true;
    }

    UI16  IpPolicyIpPrefixMessage::getInstance() const
    {
        return (m_instance);
    }

    bool  IpPolicyIpPrefixMessage::getInstanceFlag() const
    {
        return (m_instance_flag);
    }

    void  IpPolicyIpPrefixMessage::setAction(const PolicyAction &action)
    {
        m_action = action;
        m_action_flag = true;
    }

    PolicyAction  IpPolicyIpPrefixMessage::getAction() const
    {
        return (m_action);
    }

    bool  IpPolicyIpPrefixMessage::getActionFlag() const
    {
        return (m_action_flag);
    }

    void  IpPolicyIpPrefixMessage::setIp_prefix(const IpV4AddressNetworkMask &ip_prefix)
    {
        m_ip_prefix = ip_prefix;
        m_ip_prefix_flag = true;
    }

    IpV4AddressNetworkMask  IpPolicyIpPrefixMessage::getIp_prefix() const
    {
        return (m_ip_prefix);
    }

    bool  IpPolicyIpPrefixMessage::getIp_prefixFlag() const
    {
        return (m_ip_prefix_flag);
    }

    void  IpPolicyIpPrefixMessage::setGe(const UI16 &ge)
    {
        m_ge = ge;
        m_ge_flag = true;
    }

    UI16  IpPolicyIpPrefixMessage::getGe() const
    {
        return (m_ge);
    }

    bool  IpPolicyIpPrefixMessage::getGeFlag() const
    {
        return (m_ge_flag);
    }

    void  IpPolicyIpPrefixMessage::setLe(const UI16 &le)
    {
        m_le = le;
        m_le_flag = true;
    }

    UI16  IpPolicyIpPrefixMessage::getLe() const
    {
        return (m_le);
    }

    bool  IpPolicyIpPrefixMessage::getLeFlag() const
    {
        return (m_le_flag);
    }
}
