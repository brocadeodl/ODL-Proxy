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

#ifndef IPPOLICYLOCALMANAGEDOBJECTS_H
#define IPPOLICYLOCALMANAGEDOBJECTS_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "IpPolicy/Framework/PbrVrfManagedObject.h"
#include "IpPolicy/Framework/PbrNexthopAddrManagedObject.h"
#include "IpPolicy/Framework/PbrGlobalNexthopManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpPolicyLocalObjectManager;
    class IpPolicyLocalWorker;
	class PbrVrfManagedObject;
	class PbrNexthopAddrManagedObject;
	class PbrGlobalNexthopManagedObject;

// ************* RouteMapLocalManagedObject ************
    class RouteMapLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
            friend class IpPolicyLocalWorker;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();

        public:
            RouteMapLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
            virtual    ~RouteMapLocalManagedObject ();
            static string getClassName();
            void setInstance(const UI16 &instance);
            UI16 getInstance()  const;

            void setAction(const PolicyAction &action);
            PolicyAction getAction()  const;

            void setMatch_intf_eth(const vector<string> &match_intf_eth);
            void unsetMatch_intf_eth_selective(const vector<string> &match_intf_eth);
            void unsetMatch_intf_eth();
            vector<string> getMatch_intf_eth()  const;

            void setMatch_intf_gi(const vector<string> &match_intf_gi);
            void unsetMatch_intf_gi_selective(const vector<string> &match_intf_gi);
            void unsetMatch_intf_gi();
            vector<string> getMatch_intf_gi()  const;

            void setMatch_intf_fo(const vector<string> &match_intf_fo);
            void unsetMatch_intf_fo_selective(const vector<string> &match_intf_fo);
            void unsetMatch_intf_fo();
            vector<string> getMatch_intf_fo()  const;

            void setMatch_intf_vlan(const vector<UI32> &match_intf_vlan);
            void unsetMatch_intf_vlan_selective(const vector<UI32> &match_intf_vlan);
            void unsetMatch_intf_vlan();
            vector<UI32> getMatch_intf_vlan()  const;

            void setMatch_intf_po(const vector<UI32> &match_intf_po);
            void unsetMatch_intf_po_selective(const vector<UI32> &match_intf_po);
            void unsetMatch_intf_po();
            vector<UI32> getMatch_intf_po()  const;

            void setMatch_intf_ve(const vector<UI32> &match_intf_ve);
            void unsetMatch_intf_ve_selective(const vector<UI32> &match_intf_ve);
            void unsetMatch_intf_ve();
            vector<UI32> getMatch_intf_ve()  const;

            void setMatch_intf_loopback(const vector<UI32> &match_intf_loopback);
            void unsetMatch_intf_loopback_selective(const vector<UI32> &match_intf_loopback);
            void unsetMatch_intf_loopback();
            vector<UI32> getMatch_intf_loopback()  const;

            void setMatch_addr_prefix(const string &match_addr_prefix);
            void unsetMatch_addr_prefix();
            string getMatch_addr_prefix()  const;

            void setMatch_nh_prefix(const string &match_nh_prefix);
            void unsetMatch_nh_prefix();
            string getMatch_nh_prefix()  const;

            void setMatch_addr_acl(const string &match_addr_acl);
            void unsetMatch_addr_acl();
            string getMatch_addr_acl()  const;

//            void setMatch_precedence(const WellKnownPrecedence &wellKnownPrecedence);
//            void unsetMatch_precedence();
//            WellKnownPrecedence getMatch_wellKnownPrecedence()  const;

            void setMatch_dscpPrecedence(const DscpPrecedence &dscpPrecedence);
            void unsetMatch_dscpPrecedence();
            DscpPrecedence getMatch_dscpPrecedence()  const;

            void setMatch_nullZero(const bool &nullZero);
            void unsetMatch_nullZero();
            bool getMatch_nullZero()  const;

            void setMatch_next_hop_address(const UI32 &match_next_hop_address);
            void unsetMatch_next_hop_address();
            UI32 getMatch_next_hop_address()  const;

            void setMatch_metric(const UI32& match_metric);
            void unsetMatch_metric();
            UI32 getMatch_metric()  const;

            void setMatch_route_type(const PolicyRouteType &match_route_type);
            void unsetMatch_route_type();
            PolicyRouteType getMatch_route_type()  const;

            void setMatch_route_tag(const vector<UI32> &match_route_tag);
            void unsetMatch_route_tag();
            vector<UI32> getMatch_route_tag()  const;

            void setMatch_rs_prefix(const string &match_rs_prefix);
            void unsetMatch_rs_prefix();
            string getMatch_rs_prefix()  const;

            void setMatch_route_aspath(const string &match_route_aspath);
            void unsetMatch_route_aspath();
            string getMatch_route_aspath()  const;

            void setMatch_bgp_route_type(const BgpProtocolRouteType &match_bgp_route_type);
            void unsetMatch_bgp_route_type();
            BgpProtocolRouteType getMatch_bgp_route_type()  const;

            void setMatch_protocol_bgp(const bool &match_protocol_bgp);
            void unsetMatch_protocol_bgp();
            bool getMatch_protocol_bgp()  const;

            void setSet_distance(const UI32 &set_distance);
            void unsetSet_distance();
            UI32 getSet_distance()  const;

            void setSet_route_metric_type_external(const bool &set_route_metric_type_external);
            void unsetSet_route_metric_type_external();
            bool getSet_route_metric_type_external()  const;

            void setSet_route_metric_type_internal(const bool &set_route_metric_type_internal);
            void unsetSet_route_metric_type_internal();
            bool getSet_route_metric_type_internal()  const;

            void setSet_route_metric_type_type_1(const bool &set_route_metric_type_type_1);
            void unsetSet_route_metric_type_type_1();
            bool getSet_route_metric_type_type_1()  const;

            void setSet_route_metric_type_type_2(const bool &set_route_metric_type_type_2);
            void unsetSet_route_metric_type_type_2();
            bool getSet_route_metric_type_type_2()  const;

            void setSet_metric_action(const PolicyMetricAction &set_metric_action);
            void unsetSet_metric_action();
            PolicyMetricAction getSet_metric_action()  const;

            void setSet_metric(const UI32 &set_metric);
            void unsetSet_metric();
            UI32 getSet_metric()  const;

            void setSet_route_type(const PolicyRouteType &set_route_type);
            void unsetSet_route_type();
            PolicyRouteType getSet_route_type()  const;

            void setSet_route_tag(const UI32 &set_route_tag);
            void unsetSet_route_tag();
            UI32 getSet_route_tag()  const;

            void setSet_route_local_preference(const UI32 &set_route_local_preference);
            void unsetSet_route_local_preference();
            UI32 getSet_route_local_preference()  const;

            void setSet_route_weight(const UI32 &set_route_weight);
            void unsetSet_route_weight();
            UI32 getSet_route_weight()  const;

        	void  setRoute_continue_no_val(const bool &route_continue_no_val);
        	void  unsetRoute_continue_no_val();
        	bool  getRoute_continue_no_val() const;

            void setRoute_continue(const UI32 &route_continue);
            void unsetRoute_continue();
            UI32 getRoute_continue()  const;

            void setSet_route_dampening_half_life(const UI32 &set_route_dampening_half_life);
            void unsetSet_route_dampening_half_life();
            UI32 getSet_route_dampening_half_life()  const;

            void setSet_route_dampening_reuse(const UI32 &set_route_dampening_reuse);
            void unsetSet_route_dampening_reuse();
            UI32 getSet_route_dampening_reuse()  const;

            void setSet_route_dampening_suppress(const UI32 &set_route_dampening_suppress);
            void unsetSet_route_dampening_suppress();
            UI32 getSet_route_dampening_suppress()  const;

            void setSet_route_dampening_max_suppress(const UI32 &set_route_dampening_max_suppress);
            void unsetSet_route_dampening_max_suppress();
            UI32 getSet_route_dampening_max_suppress()  const;

            void setSet_route_aspath_prepend(const string &set_route_aspath_prepend);
            void unsetSet_route_aspath_prepend();
            string getSet_route_aspath_prepend()  const;

            void setSet_route_aspath_tag(const bool &set_route_aspath_tag);
            void unsetSet_route_aspath_tag();
            bool getSet_route_aspath_tag()  const;

            void setMatch_route_community_list(const string &match_route_community_list);
            void unsetMatch_route_community_list();
            string getMatch_route_community_list()  const;

            void setSet_route_origin_igp(const bool &set_route_origin_igp);
            void unsetSet_route_origin_igp();
            bool getSet_route_origin_igp()  const;

            void setSet_route_origin_incomplete(const bool &set_route_origin_incomplete);
            void unsetSet_route_origin_incomplete();
            bool getSet_route_origin_incomplete()  const;

            void setSet_route_community_access_list(const string &set_route_community_access_list);
            void unsetSet_route_community_access_list();
            string getSet_route_community_access_list()  const;
            bool getSet_route_community_acl_delete() const;

            void setSet_route_automatic_tag(const bool &set_route_automatic_tag);
            void unsetSet_route_automatic_tag();
            bool getSet_route_automatic_tag()  const;

            void setSet_route_dampening(const bool &set_route_dampening);
            void unsetSet_route_dampening();
            bool getSet_route_dampening()  const;

            void setSet_next_hop_peer_address(const bool &set_next_hop_peer_address);
            void unsetSet_next_hop_peer_address();
            bool getSet_next_hop_peer_address()  const;

            void setSet_route_community_expr(const string &set_route_community_expr);
            void unsetSet_route_community_expr();
            string getSet_route_community_expr()  const;

            void setSet_route_extcommunity_expr(const string &set_route_extcommunity_expr);
            void unsetSet_route_extcommunity_expr();
            string getSet_route_extcommunity_expr()  const;

            void setSet_route_extcommunity_soo_expr(const string &set_route_extcommunity_soo_expr);
            void unsetSet_route_extcommunity_soo_expr();
            string getSet_route_extcommunity_soo_expr()  const;

            void addPbrVrfMOConfToVector(PbrVrfManagedObject *pbrVrfMO);
            void delPbrVrfMOConfFromVector(string vrfName, IpV4Address nexthopAddr);
            void setPbrVrfConf(const vector<WaveManagedObjectPointer<PbrVrfManagedObject> > &pbrVrfConf);
            vector<WaveManagedObjectPointer<PbrVrfManagedObject> > getPbrVrfConf()  const;

            void addPbrNexthopMOConfToVector(PbrNexthopAddrManagedObject *pbrNexthopMO);
            void delPbrNexthopMOConfFromVector(IpV4Address nexthopAddr);
            void setPbrNexthopConf(const vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject> > &pbrNexthopConf);
            vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject> > getPbrNexthopConf()  const;

            void addPbrGlobalNexthopMOConfToVector(PbrGlobalNexthopManagedObject *pbrNexthopMO);
            void delPbrGlobalNexthopMOConfFromVector(IpV4Address nexthopAddr);
            void setGlobalPbrNexthopConf(const vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> > &pbrNexthopConf);
            vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> > getPbrGlobalNexthopConf()  const;


// Now the data members

        private:
            UI16  m_instance;
            PolicyAction  m_action;
//			WellKnownPrecedence m_well_known_precedence;
//			DscpPrecedence m_dscp_precedence;
			UI32UC m_dscp_precedence;
            StringVectorUC  m_match_intf_eth;
            StringVectorUC  m_match_intf_gi;
			BoolUC  m_set_next_hop_peer_address;
            StringVectorUC  m_match_intf_fo;
            UI32VectorUC  m_match_intf_vlan;
            UI32VectorUC  m_match_intf_po;
            UI32VectorUC  m_match_intf_ve;
            UI32VectorUC  m_match_intf_loopback;
            StringUC  m_match_addr_prefix;
            StringUC  m_match_nh_prefix;
            UI32UC  m_match_next_hop_address;
            UI32UC  m_match_metric;
			StringUC  m_match_addr_acl;
			BoolUC  m_match_nullZero;
            UI32UC  m_match_route_type;
            UI32VectorUC  m_match_route_tag;
            StringUC  m_match_rs_prefix;
            StringUC  m_match_route_aspath;
            UI32UC  m_match_bgp_route_type;
            BoolUC  m_match_protocol_bgp;
            UI32UC  m_set_distance;
            UI32UC  m_set_metric_action;
            UI32UC  m_set_metric;
            UI32UC  m_set_route_type;
            UI32UC  m_set_route_tag;
            UI32UC  m_set_route_local_preference;
            UI32UC  m_set_route_weight;
            UI32UC  m_set_route_dampening_half_life;
            UI32UC  m_set_route_dampening_reuse;
            UI32UC  m_set_route_dampening_suppress;
            UI32UC  m_set_route_dampening_max_suppress;
            StringUC  m_set_route_aspath_prepend;
            BoolUC  m_set_route_aspath_tag;
            StringUC  m_match_route_community_list;
            BoolUC  m_set_route_origin_igp;
            BoolUC  m_set_route_origin_incomplete;
			BoolUC  m_set_route_metric_type_external;
			BoolUC  m_set_route_metric_type_internal;
			BoolUC  m_set_route_metric_type_type_1;
			BoolUC  m_set_route_metric_type_type_2;
            StringUC  m_set_route_community_access_list;
            BoolUC  m_set_route_community_acl_delete;
            BoolUC  m_set_route_automatic_tag;
            BoolUC  m_set_route_dampening;
            UI32UC  m_route_continue;
            BoolUC  m_route_continue_no_val;
			vector<WaveManagedObjectPointer<PbrVrfManagedObject> >  m_pbrVrfMO;
			vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject> > m_pbrNexthopAddrMO;
			vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> > m_pbrGlobalNexthopAddrMO;
            StringUC  m_set_route_community_expr;
            StringUC  m_set_route_extcommunity_expr;
            StringUC  m_set_route_extcommunity_soo_expr;
    };

// ************* IpPrefixLocalManagedObject ************
    class IpPrefixLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        friend class IpPolicyLocalWorker;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            IpPrefixLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
            IpPrefixLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const string &name,
                const UI32 &seq,
                const UI16 &instance,
                const PolicyAction &action,
                const IpV4AddressNetworkMask &ip_prefix,
                const UI16 &ge,
                const UI16 &le);
            virtual    ~IpPrefixLocalManagedObject ();
            static string getClassName();
            void setInstance(const UI16 &instance);
            UI16 getInstance()  const;
            void setAction(const PolicyAction &action);
            PolicyAction getAction()  const;
            void setIp_prefix(const IpV4AddressNetworkMask &ip_prefix);
            IpV4AddressNetworkMask getIp_prefix()  const;

            void setGe(UI32 ge);
            void unsetGe();
            UI32 getGe()  const;

            void setLe(UI32 le);
            void unsetLe();
            UI32 getLe()  const;


// Now the data members

        private:
            UI32  m_seq;  
            UI16  m_instance;
            PolicyAction  m_action;
            IpV4AddressNetworkMask  m_ip_prefix;
            UI32UC  m_ge;
            UI32UC  m_le;
    };

}
#endif                                            // IPPOLICYLOCALMANAGEDOBJECTS_H
