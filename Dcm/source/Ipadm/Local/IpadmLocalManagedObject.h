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
 *   Author : cshah                                                     *
 **************************************************************************/

#ifndef IPVCONFIGLOCALMANAGEDOBJECT_H
#define IPVCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Ipadm/Local/IpadmTypes.h"

#include <ha/ipadm.h>

using namespace WaveNs;

namespace DcmNs
{

    class IpadmLocalObjectManager;
    class IpadmLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void loadOperationalData(WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);
        public:
            IpadmLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager);
            IpadmLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const string &id,const string &IPv4Address,const IpV4Address &IPv4Gateway, const bool &dhcp, const string &Ipv6Address, const bool &autoconfig, const LinkSpeed &linkSpeed, const string &oper_ip_address, const IpV4Address &oper_gateway, const vector<string>  &oper_vector_ipv6_address, const vector<string>  &oper_vector_ipv6_gateways, const string &oper_speed, const string &oper_config_speed, const bool &dhcpv6,
				const bool &unreachable, const bool &echoreply, const unsigned int &ratelimiting, const bool &v6unreachable, const bool &v6echoreply, const unsigned int &v6ratelimiting);
            virtual    ~IpadmLocalManagedObject ();
			static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setIPv4Address(const string &IPv4Address);
            string getIPv4Address()  const;
            void setIPv4Gateway(const IpV4Address &IPv4Gateway);
            IpV4Address getIPv4Gateway()  const;
			void setDhcp(const bool &dhcp);
			bool getIpv6Dhcp() const;
			void setIpv6Dhcp(const bool &dhcp);
			bool getDhcp() const;
			void setIPv6Address(const string &IPv6Address);
			string getIPv6Address()  const;
			void setAutoconfig(const bool &autoconfig);
			bool getAutoconfig() const;
			
            void set_oper_ip_address(const string &oper_ip_address);
            string get_oper_ip_address()  const;
			void set_oper_gateway(const IpV4Address &oper_gateway);
			IpV4Address get_oper_gateway()  const;
            void set_oper_speed(const string &oper_speed);
            string get_oper_speed()  const;
            void set_oper_config_speed(const string &oper_config_speed);
            string get_oper_config_speed()  const;

			void set_oper_vector_ipv6_address(const vector<string> &oper_vector_ipv6_address);
			vector<string> get_oper_vector_ipv6_address()  const;
			void set_oper_vector_ipv6_gateway(const vector<string> &oper_vector_ipv6_gateways);
			vector<string> get_oper_vector_ipv6_gateways()  const;
			
			int locate_addr_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family, int nentry);
			int locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family, int nentry);
            		void set_link_speed(const LinkSpeed &speed);
            		LinkSpeed get_link_speed()  const;
// Now the data members

			void	setUnreachable(const bool unreachable);
			bool	getUnreachable(void) const ;
			void	setEchoreply(const bool echoreply);
			bool	getEchoreply(void) const ;
			void	setRatelimiting(const unsigned int ratelimiting);
			unsigned int getRatelimiting(void) const ;
			void	setV6Unreachable(const bool unreachable);
			bool	getV6Unreachable(void) const ;
			void	setV6Echoreply(const bool echoreply);
			bool	getV6Echoreply(void) const ;
			void	setV6Ratelimiting(const unsigned int ratelimiting);
			unsigned int getV6Ratelimiting(void) const ;
        private:
			// define these for operational data
			ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
			struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
			addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
			struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
			addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
			ipadm_ip_get_set_t current; 

        protected:
        public:
			string 					m_id;
            string		  			m_IPv4Address;
            IpV4Address  			m_IPv4Gateway;
			bool 					m_dhcp;
			string					m_Ipv6Address;
			bool					m_autoconfig;
			LinkSpeed 			m_link_speed;
			
			string		 			m_oper_ip_address;
			IpV4Address 			m_oper_gateway;
			vector<string>		m_oper_vector_ipv6_address;
			vector<string>		m_oper_vector_ipv6_gateways;
			string			m_oper_speed;
			string			m_oper_config_speed;
			bool					m_dhcpv6;
			/* icmp control */
			bool			m_unreachable;
			bool			m_echoreply;
			unsigned int	m_ratelimiting;
			bool			m_v6unreachable;
			bool			m_v6echoreply;
			unsigned int	m_v6ratelimiting;

    };
}
#endif                                            //IPVCONFIGLOCALMANAGEDOBJECT_H
