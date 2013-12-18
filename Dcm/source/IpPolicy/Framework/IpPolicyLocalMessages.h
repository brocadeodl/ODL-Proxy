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

#ifndef IPPOLICYLOCALMESSAGES_H
#define IPPOLICYLOCALMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

// ************* IpPolicyCreateRouteMapMessage ************
    class IpPolicyRouteMapMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpPolicyRouteMapMessage ();
            virtual    ~IpPolicyRouteMapMessage ();
            void setName(const string &name);
            string getName()  const;
            bool getNameFlag()  const;
            void setInstance(const UI16 &instance);
            UI16 getInstance()  const;
            bool getInstanceFlag()  const;
            void setAction(const PolicyAction &action);
            PolicyAction getAction()  const;
            bool getActionFlag()  const;
            void setMatch_intf_eth(const vector<string> &match_intf_eth);
            vector<string> getMatch_intf_eth()  const;
            bool getMatch_intf_ethFlag()  const;
            void setMatch_intf_gi(const vector<string> &match_intf_gi);
            vector<string> getMatch_intf_gi()  const;
            bool getMatch_intf_giFlag()  const;
            void setMatch_intf_fo(const vector<string> &match_intf_fo);
            vector<string> getMatch_intf_fo()  const;
            bool getMatch_intf_foFlag()  const;
            void setMatch_intf_vlan(const vector<UI32> &match_intf_vlan);
            vector<UI32> getMatch_intf_vlan()  const;
            bool getMatch_intf_vlanFlag()  const;
            void setMatch_intf_po(const vector<UI32> &match_intf_po);
            vector<UI32> getMatch_intf_po()  const;
            bool getMatch_intf_poFlag()  const;
            void setMatch_intf_ve(const vector<UI32> &match_intf_ve);
            vector<UI32> getMatch_intf_ve()  const;
            bool getMatch_intf_veFlag()  const;
            void setMatch_intf_loopback(const vector<UI32> &match_intf_loopback);
            vector<UI32> getMatch_intf_loopback()  const;
            bool getMatch_intf_loopbackFlag()  const;
            void setMatch_addr_prefix(const string &match_addr_prefix);
            string getMatch_addr_prefix()  const;
            bool getMatch_addr_prefixFlag()  const;
            void setMatch_nh_prefix(const string &match_nh_prefix);
            string getMatch_nh_prefix()  const;
            bool getMatch_nh_prefixFlag()  const;

            void setMatch_addr_acl(const string &match_addr_acl);
            string getMatch_addr_acl()  const;
            bool getMatch_addr_aclFlag()  const;

            void setMatch_nullZero(const bool &nullZero);
            bool getMatch_nullZero()  const;
            bool getMatch_nullZeroFlag()  const;

//            void setMatch_precedence(WellKnownPrecedence wellKnownPrecedence);
//            WellKnownPrecedence getMatch_precedence()  const;
//            bool getMatch_precedenceFlag()  const;

            void setMatch_dscpPrecedence(DscpPrecedence dscpPrecedence);
            DscpPrecedence getMatch_dscpPrecedence() const;
            bool getMatch_dscpPrecedenceFlag() const;

            void setMatch_next_hop_address(const UI32 &match_next_hop_address);
            UI32 getMatch_next_hop_address()  const;
            bool getMatch_next_hop_addressFlag()  const;
            void setMatch_metric(const UI32& match_metric);
            UI32 getMatch_metric()  const;
            bool getMatch_metricFlag()  const;
            void setMatch_route_type(const PolicyRouteType &match_route_type);
            PolicyRouteType getMatch_route_type()  const;
            bool getMatch_route_typeFlag()  const;
            void setMatch_bgp_route_type(const BgpProtocolRouteType &match_bgp_route_type);
            BgpProtocolRouteType getMatch_bgp_route_type()  const;
            bool getMatch_bgp_route_typeFlag()  const;
            void setMatch_protocol_bgp(const bool &match_protocol_bgp);
            bool getMatch_protocol_bgp()  const;
            bool getMatch_protocol_bgpFlag()  const;
            void setMatch_route_tag(const vector<UI32> &match_route_tag);
            vector<UI32> getMatch_route_tag()  const;
            bool getMatch_route_tagFlag()  const;
            void setMatch_route_aspath(const string &match_route_aspath);
            string getMatch_route_aspath()  const;
            bool getMatch_route_aspathFlag()  const;
            void setSet_distance(const UI32 &set_distance);
            UI32 getSet_distance()  const;
            bool getSet_distanceFlag()  const;
            void setSet_nh_ip(const IpV4Address &set_nh_ip);
            IpV4Address getSet_nh_ip()  const;
            bool getSet_nh_ipFlag()  const;
            void setSet_next_hop_peer_address(const bool &set_next_hop_peer_address);
            bool getSet_next_hop_peer_address()  const;
            bool getSet_next_hop_peer_addressFlag()  const;
            void setSet_route_metric_type_external(const bool &set_route_metric_type_external);
            bool getSet_route_metric_type_external()  const;
            bool getSet_route_metric_type_externalFlag()  const;
            void setSet_route_metric_type_internal(const bool &set_route_metric_type_internal);
            bool getSet_route_metric_type_internal()  const;
            bool getSet_route_metric_type_internalFlag()  const;
            void setSet_route_metric_type_type_1(const bool &set_route_metric_type_type_1);
            bool getSet_route_metric_type_type_1()  const;
            bool getSet_route_metric_type_type_1Flag()  const;
            void setSet_route_metric_type_type_2(const bool &set_route_metric_type_type_2);
            bool getSet_route_metric_type_type_2()  const;
            bool getSet_route_metric_type_type_2Flag()  const;
            void setSet_metric_action(const PolicyMetricAction &set_metric_action);
            PolicyMetricAction getSet_metric_action()  const;
            bool getSet_metric_actionFlag()  const;
            void setSet_metric(const UI32 &set_metric);
            UI32 getSet_metric()  const;
            bool getSet_metricFlag()  const;
            void setSet_route_type(const PolicyRouteType &set_route_type);
            PolicyRouteType getSet_route_type()  const;
            bool getSet_route_typeFlag()  const;
            void setSet_route_tag(const UI32 &set_route_tag);
            UI32 getSet_route_tag()  const;
            bool getSet_route_tagFlag()  const;
            void setSet_route_local_preference(const UI32 &set_route_local_preference);
            UI32 getSet_route_local_preference()  const;
            bool getSet_route_local_preferenceFlag()  const;
            void setSet_route_weight(const UI32 &set_route_weight);
            UI32 getSet_route_weight()  const;
            bool getSet_route_weightFlag()  const;
            void setRoute_continue(const UI32 &route_continue);
            UI32 getRoute_continue()  const;
            bool getRoute_continueFlag()  const;
            void setRoute_continueFlag(const bool &route_continue_flag);
            void setMatch_rs_prefix(const string &match_rs_prefix);
            string getMatch_rs_prefix()  const;
            bool getMatch_rs_prefixFlag()  const;
            void setSet_route_aspath_prepend(const string &set_route_aspath_prepend);
            string getSet_route_aspath_prepend()  const;
            bool getSet_route_aspath_prependFlag()  const;
            void setSet_route_aspath_tag(const bool &set_route_aspath_tag);
            bool getSet_route_aspath_tag()  const;
            bool getSet_route_aspath_tagFlag()  const;
            void setMatch_route_community_list(const string &Match_route_community_list);
            string getMatch_route_community_list()  const;
            bool getMatch_route_community_listFlag()  const;
            void setMatch_route_community_exact_match(const bool &Match_route_community_exact_match);
            bool getMatch_route_community_exact_match()  const;
            bool getMatch_route_community_exact_matchFlag()  const;
            void setSet_route_origin_igp(const bool &set_route_origin_igp);
            bool getSet_route_origin_igp()  const;
            bool getSet_route_origin_igpFlag()  const;
            void setSet_route_origin_incomplete(const bool &set_route_origin_incomplete);
            bool getSet_route_origin_incomplete()  const;
            bool getSet_route_origin_incompleteFlag()  const;
            void setSet_route_automatic_tag(const bool &set_route_automatic_tag);
            bool getSet_route_automatic_tag()  const;
            bool getSet_route_automatic_tagFlag()  const;
            void setSet_route_dampening(const bool &set_route_dampening);
            bool getSet_route_dampening()  const;
            bool getSet_route_dampeningFlag()  const;
            void setSet_route_community_access_list(const string &set_route_community_access_list);
            string getSet_route_community_access_list()  const;
            bool getSet_route_community_access_listFlag()  const;
            void setSet_route_community_acl_delete(const bool &set_route_community_acl_delete);
            bool getSet_route_community_acl_delete()  const;
            bool getSet_route_community_acl_deleteFlag()  const;
            void setSet_route_dampening_half_life(const UI32 &set_route_dampening_half_life);
            UI32 getSet_route_dampening_half_life()  const;
            bool getSet_route_dampening_half_lifeFlag()  const;
            void setSet_route_dampening_reuse(const UI32 &set_route_dampening_reuse);
            UI32 getSet_route_dampening_reuse()  const;
            bool getSet_route_dampening_reuseFlag()  const;
            void setSet_route_dampening_suppress(const UI32 &set_route_dampening_suppress);
            UI32 getSet_route_dampening_suppress()  const;
            bool getSet_route_dampening_suppressFlag()  const;
            void setSet_route_dampening_max_suppress(const UI32 &set_route_dampening_max_suppress);
            UI32 getSet_route_dampening_max_suppress()  const;
            bool getSet_route_dampening_max_suppressFlag()  const;
            void setSet_route_community_expr(const string &set_route_community_expr);
            string getSet_route_community_expr()  const;
            bool getSet_route_community_exprFlag()  const;
            void setSet_route_extcommunity_expr(const string &set_route_extcommunity_expr);
            string getSet_route_extcommunity_expr()  const;
            bool getSet_route_extcommunity_exprFlag()  const;
            void setSet_route_extcommunity_soo_expr(const string &set_route_extcommunity_soo_expr);
            string getSet_route_extcommunity_soo_expr()  const;
            bool getSet_route_extcommunity_soo_exprFlag()  const;
            IpPolicyOpCode opCode() const { return m_op_code;};
            void opCode(IpPolicyOpCode op_code) {m_op_code = op_code;};
            const string& get_backend_err() const { return m_backend_err;}
            void set_backend_err(const string& err) { m_backend_err = err;}
            void setIfTypeToDeleteAllIntfs(UI8 ifType);
            UI8 getIfTypeToDeleteAllIntfs() const;

// Now the data members

        private:
            IpPolicyOpCode m_op_code;
            string  m_name;
            UI16  m_instance;
            PolicyAction  m_action;
//			WellKnownPrecedence m_well_known_precedence;
			DscpPrecedence m_dscp_precedence;
            vector<string>  m_match_intf_eth;
            vector<string>  m_match_intf_gi;
            vector<string>  m_match_intf_fo;
            vector<UI32>  m_match_intf_vlan;
            vector<UI32>  m_match_intf_po;
            vector<UI32>  m_match_intf_ve;
            vector<UI32>  m_match_intf_loopback;
            string  m_match_addr_prefix;
            string  m_match_nh_prefix;
			string  m_match_addr_acl;
			bool m_nullZero;
            UI32  m_match_next_hop_address;
            UI32    m_match_metric;
            PolicyRouteType  m_match_route_type;
            vector<UI32>  m_match_route_tag;
            string  m_match_route_aspath;
            BgpProtocolRouteType  m_match_bgp_route_type;
            bool m_match_protocol_bgp;
            UI32  m_set_distance;
            IpV4Address  m_set_nh_ip;
            PolicyMetricAction  m_set_metric_action;
            UI32  m_set_metric;
            PolicyRouteType  m_set_route_type;
            UI32  m_set_route_tag;
            UI32  m_set_route_local_preference;
            UI32  m_set_route_weight;
            UI32  m_route_continue;
            UI32  m_set_route_dampening_half_life;
            UI32  m_set_route_dampening_reuse;
            UI32  m_set_route_dampening_suppress;
            UI32  m_set_route_dampening_max_suppress;
            string  m_match_rs_prefix;
            string m_set_route_aspath_prepend;
			string m_set_route_community_access_list;
			string m_set_route_community_expr;
			string m_set_route_extcommunity_expr;
			string m_set_route_extcommunity_soo_expr;
			bool m_set_route_community_acl_delete;
			bool m_set_route_automatic_tag;
            bool m_set_route_aspath_tag;
            string m_match_route_community_list;
            bool m_match_route_community_exact_match;
            bool m_set_route_origin_igp;
            bool m_set_route_origin_incomplete;
            bool m_set_next_hop_peer_address;
            bool m_set_route_metric_type_external;
			bool m_set_route_metric_type_internal;
			bool m_set_route_metric_type_type_1;
			bool m_set_route_metric_type_type_2;
            bool m_name_flag;
            bool m_instance_flag;
            bool m_action_flag;
            bool m_match_intf_eth_flag;
            bool m_match_intf_gi_flag;
            bool m_match_intf_fo_flag;
            bool m_match_intf_vlan_flag;
            bool m_match_intf_po_flag;
            bool m_match_intf_ve_flag;
            bool m_match_intf_loopback_flag;
            bool m_match_addr_prefix_flag;
            bool m_match_nh_prefix_flag;
			bool m_match_addr_acl_flag;
			bool m_nullZero_flag;
//			bool m_precedence_flag;
			bool m_dscp_precedence_flag;
            bool m_match_next_hop_address_flag;
            bool m_match_metric_flag;
            bool m_match_route_type_flag;
            bool m_match_route_tag_flag;
            bool m_match_route_aspath_flag;
            bool m_match_bgp_route_type_flag;
            bool m_match_protocol_bgp_flag;
            bool m_set_distance_flag;
            bool m_set_nh_ip_flag;
            bool m_set_metric_action_flag;
            bool m_set_metric_flag;
            bool m_set_route_type_flag;
            bool m_set_route_tag_flag;
            bool m_set_route_local_preference_flag;
            bool m_set_route_weight_flag;
            bool m_route_continue_flag;
            bool m_match_rs_prefix_flag;
            bool m_set_route_aspath_prepend_flag;
            bool m_set_route_aspath_tag_flag;
            bool m_match_route_community_list_flag;
            bool m_match_route_community_exact_match_flag;
            bool m_set_route_origin_igp_flag;
            bool m_set_route_origin_incomplete_flag;
            bool m_set_next_hop_peer_address_flag;
            bool m_set_route_metric_type_external_flag;
			bool m_set_route_metric_type_internal_flag;
			bool m_set_route_metric_type_type_1_flag;
			bool m_set_route_metric_type_type_2_flag;
			bool m_set_route_community_access_list_flag;
			bool m_set_route_community_acl_delete_flag;
            bool m_set_route_automatic_tag_flag;
            bool m_set_route_dampening;
            bool m_set_route_dampening_flag;
            bool m_set_route_dampening_half_life_flag;
            bool m_set_route_dampening_reuse_flag;
            bool m_set_route_dampening_suppress_flag;
            bool m_set_route_dampening_max_suppress_flag;
            bool m_set_route_community_expr_flag;
            bool m_set_route_extcommunity_expr_flag;
            bool m_set_route_extcommunity_soo_expr_flag;

            UI8 m_deleteAllIntfOfType;
            string m_backend_err;
    };


// ************* IpPolicyCreateIpPrefixMessage ************
    class IpPolicyIpPrefixMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpPolicyIpPrefixMessage ();
            IpPolicyIpPrefixMessage (const string &name,
                const UI16 &instance,
                const PolicyAction &action,
                const IpV4AddressNetworkMask &ip_prefix,
                const UI16 &ge,
                const UI16 &le);
            virtual    ~IpPolicyIpPrefixMessage ();
            void setName(const string &name);
            string getName()  const;
            bool getNameFlag()  const;
            void setInstance(const UI16 &instance);
            UI16 getInstance()  const;
            bool getInstanceFlag()  const;
            void setAction(const PolicyAction &action);
            PolicyAction getAction()  const;
            bool getActionFlag()  const;
            void setIp_prefix(const IpV4AddressNetworkMask &ip_prefix);
            IpV4AddressNetworkMask getIp_prefix()  const;
            bool getIp_prefixFlag()  const;
            void setGe(const UI16 &ge);
            UI16 getGe()  const;
            bool getGeFlag()  const;
            void setLe(const UI16 &le);
            UI16 getLe()  const;
            bool getLeFlag()  const;
            IpPolicyOpCode opCode() const { return m_op_code;};
            void opCode(IpPolicyOpCode op_code) {m_op_code = op_code;};
            const string& get_backend_err() const { return m_backend_err;}
            void set_backend_err(const string& err) { m_backend_err = err;}

// Now the data members

        private:
            IpPolicyOpCode m_op_code;
            string  m_name;
            UI16  m_instance;
            PolicyAction  m_action;
            IpV4AddressNetworkMask  m_ip_prefix;
            UI16  m_ge;
            UI16  m_le;
            bool m_name_flag;
            bool m_instance_flag;
            bool m_action_flag;
            bool m_ip_prefix_flag;
            bool m_ge_flag;
            bool m_le_flag;
            string m_backend_err;

    };
}
#endif                                            // IPPOLICYLOCALMESSAGES_H
