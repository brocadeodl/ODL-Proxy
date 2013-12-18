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

#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    IpadmLocalManagedObject::IpadmLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager)
        : PrismElement  (pIpadmLocalObjectManager),
        PrismPersistableObject (IpadmLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpadmLocalObjectManager),
        DcmManagedObject (pIpadmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpadmLocalObjectManager)
    {
    }
    IpadmLocalManagedObject::IpadmLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const string &id,const string &IPv4Address,const IpV4Address &IPv4Gateway, const bool &dhcp, const string &Ipv6Address, const bool &autoconfig, const LinkSpeed &linkSpeed, const string &oper_ip_address, const IpV4Address &oper_gateway, const vector<string> &oper_vector_ipv6_address, const vector<string>  &oper_vector_ipv6_gateways, const string &oper_speed, const string &oper_config_speed, const bool &dhcpv6,
		const bool &unreachable, const bool &echoreply, const unsigned int &ratelimiting, const bool &v6unreachable, const bool &v6echoreply, const unsigned int &v6ratelimiting)
        : PrismElement  (pIpadmLocalObjectManager),
        PrismPersistableObject (IpadmLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpadmLocalObjectManager),
        DcmManagedObject (pIpadmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpadmLocalObjectManager),
		m_id (id),
        m_IPv4Address    (IPv4Address),
        m_IPv4Gateway    (IPv4Gateway),
		m_dhcp		(dhcp),
		m_Ipv6Address (Ipv6Address),
		m_autoconfig (autoconfig),
		m_link_speed    (linkSpeed),
		m_oper_ip_address (oper_ip_address),
		m_oper_gateway (oper_gateway),
		m_oper_vector_ipv6_address (oper_vector_ipv6_address),
		m_oper_vector_ipv6_gateways (oper_vector_ipv6_gateways),
		m_oper_speed (oper_speed),
		m_oper_config_speed (oper_config_speed),
		m_dhcpv6 (dhcpv6),
		m_unreachable(unreachable),
		m_echoreply(echoreply),
		m_ratelimiting(ratelimiting),
		m_v6unreachable(v6unreachable),
		m_v6echoreply(v6echoreply),
		m_v6ratelimiting(v6ratelimiting)
    {
    }

    IpadmLocalManagedObject::~IpadmLocalManagedObject ()
    {
    }

    string  IpadmLocalManagedObject::getClassName()
    {
        return ("IpadmLocalManagedObject");
    }

    void  IpadmLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_id,"switchid", brocade_interface_name));
        addPersistableAttribute (new AttributeString(&m_IPv4Address,true,"","IPv4Address", brocade_interface_ip_address));
        addPersistableAttribute (new AttributeIpV4Address(&m_IPv4Gateway,"IPv4Gateway", brocade_interface_gateway_address));
		addPersistableAttribute (new AttributeBool(&m_dhcp,"dhcp", brocade_interface_dhcp));
        addPersistableAttribute (new AttributeString(&m_Ipv6Address,true,"","Ipv6Address", brocade_interface_ipv6_global_address));
		addPersistableAttribute (new AttributeBool(&m_autoconfig,"autoconfig", brocade_interface_autoconfig));
	addPersistableAttribute (new AttributeEnum((UI32 * )(&m_link_speed), (UI32)M_LINK_SPEED_AUTO, "LinkSpeed",brocade_interface_speed));
		
        addPersistableAttribute (new AttributeString(&m_oper_ip_address,"oper_ip_address", brocade_interface_oper_ip_address, true));
        addPersistableAttribute (new AttributeIpV4Address(&m_oper_gateway,"oper_gateway", brocade_interface_oper_gateway, true));
        addPersistableAttribute (new AttributeStringVector(&m_oper_vector_ipv6_address,"oper_vector_ipv6_address", brocade_interface_ipv6_address, true));
        addPersistableAttribute (new AttributeStringVector(&m_oper_vector_ipv6_gateways,"oper_vector_ipv6_gateways", brocade_interface_ipv6_gateways, true));
        addPersistableAttribute (new AttributeString(&m_oper_speed,"oper_speed", brocade_interface_actual, true));
        addPersistableAttribute (new AttributeString(&m_oper_config_speed,"oper_config_speed", brocade_interface_configured, true));
		addPersistableAttribute (new AttributeBool(&m_dhcpv6,"dhcpv6", brocade_interface_dhcpv6));
		addPersistableAttribute (new AttributeBool(&m_unreachable,"unreachable", brocade_interface_unreachable));
		addPersistableAttribute (new AttributeBool(&m_echoreply,"echoreply", brocade_interface_echo_reply));
		addPersistableAttribute (new AttributeUI32(&m_ratelimiting,1000, "ratelimiting", brocade_interface_rate_limiting));
		addPersistableAttribute (new AttributeBool(&m_v6unreachable,"v6unreachable", brocade_interface_v6_unreachable));
		addPersistableAttribute (new AttributeBool(&m_v6echoreply,"v6echoreply", brocade_interface_v6_echo_reply));
		addPersistableAttribute (new AttributeUI32(&m_v6ratelimiting,1000, "v6ratelimiting", brocade_interface_v6_rate_limiting));
    }

    void  IpadmLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeString(&m_id,"switchid", brocade_interface_name));
        addPersistableAttributeForCreate (new AttributeString(&m_IPv4Address,"IPv4Address", brocade_interface_ip_address));
        addPersistableAttributeForCreate (new AttributeIpV4Address(&m_IPv4Gateway,"IPv4Gateway", brocade_interface_gateway_address));
		addPersistableAttributeForCreate (new AttributeBool(&m_dhcp,"dhcp", brocade_interface_dhcp));
        addPersistableAttributeForCreate (new AttributeString(&m_Ipv6Address,"Ipv6Address", brocade_interface_ipv6_global_address));
		addPersistableAttributeForCreate (new AttributeBool(&m_autoconfig,"autoconfig", brocade_interface_autoconfig));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *) (&m_link_speed),"LinkSpeed",brocade_interface_speed));

        addPersistableAttributeForCreate (new AttributeString(&m_oper_ip_address,"oper_ip_address", brocade_interface_oper_ip_address, true));
        addPersistableAttributeForCreate (new AttributeIpV4Address(&m_oper_gateway,"oper_gateway", brocade_interface_oper_gateway, true));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_oper_vector_ipv6_address,"oper_vector_ipv6_address", brocade_interface_ipv6_address, true));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_oper_vector_ipv6_gateways,"oper_vector_ipv6_gateways", brocade_interface_ipv6_gateways, true));
        addPersistableAttributeForCreate (new AttributeString(&m_oper_speed,"oper_speed", brocade_interface_actual, true));
        addPersistableAttributeForCreate (new AttributeString(&m_oper_config_speed,"oper_config_speed", brocade_interface_configured, true));
		addPersistableAttributeForCreate (new AttributeBool(&m_dhcpv6,"dhcpv6", brocade_interface_dhcpv6));
		addPersistableAttributeForCreate (new AttributeBool(&m_unreachable,"unreachable", brocade_interface_unreachable));
		addPersistableAttributeForCreate (new AttributeBool(&m_echoreply,"echoreply", brocade_interface_echo_reply));
		addPersistableAttributeForCreate (new AttributeUI32(&m_ratelimiting,1000, "ratelimiting", brocade_interface_rate_limiting));
		addPersistableAttributeForCreate (new AttributeBool(&m_v6unreachable,"v6unreachable", brocade_interface_v6_unreachable));
		addPersistableAttributeForCreate (new AttributeBool(&m_v6echoreply,"v6echoreply", brocade_interface_v6_echo_reply));
		addPersistableAttributeForCreate (new AttributeUI32(&m_v6ratelimiting,1000, "v6ratelimiting", brocade_interface_v6_rate_limiting));
   
		vector<string > keyName;
		keyName.push_back ("switchid");
		setUserDefinedKeyCombination (keyName);
	}

	void IpadmLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
	{
		vector<string> operationalFields         = pWaveManagedObjectLoadOperationalDataContext->getOperationalDataFields ();
		UI32           numberOfOperationalFields = operationalFields.size ();

		trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : numberOfOperationalFields = ") + numberOfOperationalFields + string("switchid ->") +m_id);
		string current_ip;
		IpV4Address current_gateway;

		unsigned int i;
		int j;
		int idx = 0;
		unsigned int flags;
		int nentry = 0;
		int nentrygw = 0;
		char ipbuf[INET6_ADDRSTRLEN];
		char prefixbuf[8];
		string str;
		string ipv4_address;
		string ipv4_gateway;
		vector<string> current_ipv6_vector_address;
		vector<string> current_ipv6_vector_gateway;
		string speed = "Unknown";
		string config_speed = "Unknown";
		
		string str_switchid;
		size_t pos;
		int cp_num;

		pos = m_id.find("/");
		str_switchid = m_id.substr(pos+1);
		cp_num = atoi(str_switchid.c_str());
		cp_num -= 1;
		if (cp_num < 0) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalManagedObject::loadOperationalData : cp_num invalid: ") + cp_num);
			cp_num = 0;
		}

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;
		
		int rc = 0;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalManagedObject::loadOperationalData :ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("IpadmLocalManagedObject::loadOperationalData :ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalManagedObject::loadOperationalData :ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);

		/* Extract IPV4 address */
		if (pizza)
			idx = locate_addr_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET, 0);
		else 
			idx = locate_addr_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET, 0);
		if (idx < 0) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalManagedObject::loadOperationalData :could not locate IPv4 address for ethernet interface"));
		} 
		else {
			string add_type;
			inet_ntop(AF_INET, &(SOCKADDR_INP(current.local_addrs[idx])->sin_addr), ipbuf, sizeof(ipbuf));
			sprintf(prefixbuf, "/%d", current.local_prefixes[idx]);
			flags = current.local_flags[idx];
			if((flags & IPADM_ADDR_DHCP) && (current.dhcp_flag & IPADM_DHCP_ENABLE)) {
				add_type = "dhcp ";
			}
			else { 
				add_type = "static ";
			}
			str = add_type;
			str.append(ipbuf);
			str.append(prefixbuf);
		        ipv4_address.assign(str);	
		}

		/* Extract IPv4 gateway */
		idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, 0, AF_INET, 0);
		if (idx < 0) {
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData :could not locate IPv4 gateway for ethernet interface"));
		} 
		else {
			inet_ntop(AF_INET, &(SOCKADDR_INP(current.gw_addrs[idx])->sin_addr), ipbuf, sizeof(ipbuf));
			ipv4_gateway.assign(ipbuf);
		}

		/* Extract IPv6 addresses */
		nentry = 0;
		for (j = 0; j < current.num_locals; j++) {
			if (pizza)
				idx = locate_addr_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6, nentry);
			else 
				idx = locate_addr_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6, nentry);
			if (idx < 0) {
				if(nentry == 0) {
					trace (TRACE_LEVEL_ERROR, string ("IpadmLocalManagedObject::loadOperationalData :could not locate IPv6 address for ethernet interface"));
				}
				break;
			}

			if(current.local_flags[idx] & IPADM_ADDR_NONE) {
				nentry++;
				continue;
			}

			inet_ntop(AF_INET6, &(SOCKADDR_IN6P(current.local_addrs[idx])->sin6_addr), ipbuf, sizeof(ipbuf));
			sprintf(prefixbuf, "/%d ", current.local_prefixes[idx]);
			
			string state;
			string type;

			flags = current.local_flags[idx];
			if (flags & IPADM_ADDR_TENTATIVE) {
				state = "tentative";
			} else if (flags & IPADM_ADDR_DEPRECATED) {
				state = "deprecated";
			} else {
				state = "preferred";
			}

			if(flags & IPADM_ADDR_AUTO_STATELESS) {
				type = "stateless ";
			}
			else if((flags & IPADM_ADDR_DHCPV6) && (current.dhcp_flag & IPADM_DHCPV6_ENABLE)) {
				type = "dhcpv6 ";
			}
			else {
				type = "static ";
			}
			
			str = type;
			str.append(ipbuf);
			str.append(prefixbuf);
			str += state;

			current_ipv6_vector_address.push_back(str);
			nentry++;
		}

		/* Extract IPv6 gateways */
		for (j = 0; j < current.num_locals; j++) {
			idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, cp_num, AF_INET6, nentrygw);
			if (idx < 0) {
				if(nentrygw == 0) {
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData :could not locate IPv6 gateways for ethernet interface"));
				}
				break;
			}

			if(current.gw_flags[idx] & IPADM_ADDR_NONE) {
				nentrygw++;
				continue;
			}

			inet_ntop(AF_INET6, &(SOCKADDR_IN6P(current.gw_addrs[idx])->sin6_addr), ipbuf, sizeof(ipbuf));
			str.assign(ipbuf);
			current_ipv6_vector_gateway.push_back(str);
			nentrygw++;
		}

		/* Execute the ifmodeshow script */
		FILE* fd = popen("/fabos/cliexec/ifmodeshow eth0", "r");
		char buf[256];
		if (fd) {
			fgets(buf, 256, fd);
			fclose(fd);

			buf[255] = '\0';
			trace (TRACE_LEVEL_INFO, string("IpadmLocalManagedObject::loadOperationalData : ethmode = ") + buf);

			char* p;
			string duplex;

			if ((p = strstr(buf, "link ok")) == NULL) {
				speed.assign("Link Down");
			}

			if ((p = strstr(buf, "nego"))) {
				config_speed.assign("Auto");
			}
			else {
				if ((p = strstr(buf, "100 Mbit"))) {
					config_speed.assign("100 Mbit");
				}
				else if ((p = strstr(buf, "10 Mbit"))) {
					config_speed.assign("10 Mbit");
				}
				else {
					config_speed.assign("Unknown");
				}
			}

			if (speed.compare("Link Down") != 0) {
				if ((p = strstr(buf, "10 Mbit"))) {
					speed.assign("10 Mbit");
				}
				else if ((p = strstr(buf, "100 Mbit"))) {
					speed.assign("100 Mbit");
				}
				else if ((p = strstr(buf, "10baseT"))) {
					speed.assign("10baseT");
				}
				else if ((p = strstr(buf, "100baseT"))) {
					speed.assign("100baseT");
				}
				else if ((p = strstr(buf, "1000baseT"))) {
					speed.assign("1000baseT");
				}
				else {
					speed.assign("Unknown");
				}
			}

			if ((p = strstr(buf, "full duplex"))) {
				duplex.assign(", Duplex: Full");
			}
			else if ((p = strstr(buf, "half duplex"))) {
				duplex.assign(", Duplex: Half");
			}
			else if ((p = strstr(buf, "T-FD"))) {
				duplex.assign(", Duplex: Full");
			}
			else if ((p = strstr(buf, "T-HD"))) {
				duplex.assign(", Duplex: Half");
			}
			else if ((p = strstr(buf, "Tx-FD"))) {
				duplex.assign(", Duplex: Full");
			}
			else if ((p = strstr(buf, "Tx-HD"))) {
				duplex.assign(", Duplex: Half");
			}
			else {
				duplex.assign(", Duplex: Half");
			}

			if(config_speed.compare("Auto") != 0) {
				config_speed.append(duplex);
			}

			if (speed.compare("Link Down") != 0) {
				speed.append(duplex);
			}
		}

		if (!numberOfOperationalFields) {
			set_oper_ip_address(ipv4_address);
			set_oper_gateway(ipv4_gateway);
			if (nentry != 0)
				set_oper_vector_ipv6_address(current_ipv6_vector_address);
			if (nentrygw != 0)
				set_oper_vector_ipv6_gateway(current_ipv6_vector_gateway);
			set_oper_speed(speed);
			set_oper_config_speed(config_speed);

		}
		else {
			for (i = 0; i < numberOfOperationalFields; i++) {
				if (!operationalFields[i].compare ("oper_ip_address")) {
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting IPv4 address!"));
					set_oper_ip_address(ipv4_address);
				}

				if (!operationalFields[i].compare ("oper_gateway")) {
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting IPv4 gateway!"));
					set_oper_gateway(ipv4_gateway);
				}

				if (!operationalFields[i].compare ("oper_vector_ipv6_address")) {

					if (nentry != 0)
						set_oper_vector_ipv6_address(current_ipv6_vector_address);
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting IPv6 addresses!"));	
				}
	
				if (!operationalFields[i].compare ("oper_vector_ipv6_gateways")) {
					if (nentrygw != 0)
						set_oper_vector_ipv6_gateway(current_ipv6_vector_gateway);
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting IPv6 gateways!"));	
				}	
				if (!operationalFields[i].compare ("oper_speed")) {
					set_oper_speed(speed);
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting speed"));	
				}	
				if (!operationalFields[i].compare ("oper_config_speed")) {
					set_oper_config_speed(config_speed);
					trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::loadOperationalData : extracting config_speed"));	
				}	
			}
		}
#if 0
		/* icmp control */
		fd = fopen("/proc/sys/net/ipv4/conf/eth0/ip_unreachable", "r");
		if (fd) {
			fgets(buf, 25, fd);
			fclose(fd);
			setUnreachable (buf[0] == '1');
		} else
			setUnreachable (true);
			
		fd = fopen("/proc/sys/net/ipv4/conf/eth0/icmp_echo_ignore", "r");
		if (fd) {
			fgets(buf, 25, fd);
			fclose(fd);
			setEchoreply(buf[0] == '0');
		} else
			setEchoreply(true);
			
		fd = fopen("/proc/sys/net/ipv4/icmp_ratelimit", "r");
		if (fd) {
			unsigned int ratelimiting;
			fscanf(fd, "%u", &ratelimiting);
			fclose(fd);
			setRatelimiting(ratelimiting);
		} else
			setRatelimiting(1000);
			
		/* icmpv6 control */
		fd = fopen("/proc/sys/net/ipv6/conf/eth0/ip_unreach", "r");
		if (fd) {
			fgets(buf, 25, fd);
			fclose(fd);
			setV6Unreachable (buf[0] == '1');
		} else
			setV6Unreachable (true);
			
		fd = fopen("/proc/sys/net/ipv6/conf/eth0/echo_ignore", "r");
		if (fd) {
			fgets(buf, 25, fd);
			fclose(fd);
			setEchoreply(buf[0] == '0');
		} else
			setEchoreply(true);
			
		fd = fopen("/proc/sys/net/ipv6/icmp/ratelimit", "r");
		if (fd) {
			unsigned int ratelimiting;
			fscanf(fd, "%u", &ratelimiting);
			fclose(fd);
			setRatelimiting(ratelimiting);
		} else
			setRatelimiting(1000);
#endif			
		pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveManagedObjectLoadOperationalDataContext->callback ();

	}

	void  IpadmLocalManagedObject::setUnreachable(const bool unreachable)
	{
		m_unreachable  =  unreachable;
	}

	bool  IpadmLocalManagedObject::getUnreachable() const
	{
		return (m_unreachable);
	}

	void  IpadmLocalManagedObject::setEchoreply(const bool echoreply)
	{
		m_echoreply  =  echoreply;
	}

	bool  IpadmLocalManagedObject::getEchoreply() const
	{
		return (m_echoreply);
	}

	void  IpadmLocalManagedObject::setRatelimiting(const unsigned int ratelimiting)
	{
		m_ratelimiting  =  ratelimiting;
	}

	unsigned int IpadmLocalManagedObject::getRatelimiting() const
	{
		return (m_ratelimiting);
	}

	void  IpadmLocalManagedObject::setV6Unreachable(const bool unreachable)
	{
		m_v6unreachable  =  unreachable;
	}

	bool  IpadmLocalManagedObject::getV6Unreachable() const
	{
		return (m_v6unreachable);
	}

	void  IpadmLocalManagedObject::setV6Echoreply(const bool echoreply)
	{
		m_v6echoreply  =  echoreply;
	}

	bool  IpadmLocalManagedObject::getV6Echoreply() const
	{
		return (m_v6echoreply);
	}

	void  IpadmLocalManagedObject::setV6Ratelimiting(const unsigned int ratelimiting)
	{
		m_v6ratelimiting  =  ratelimiting;
	}

	unsigned int IpadmLocalManagedObject::getV6Ratelimiting() const
	{
		return (m_v6ratelimiting);
	}

	void  IpadmLocalManagedObject::setId(const string &id)
	{
		m_id  =  id;
	}

	string  IpadmLocalManagedObject::getId() const
	{
		return (m_id);
	}

    void  IpadmLocalManagedObject::setIPv4Address(const string &IPv4Address)
    {
        m_IPv4Address  =  IPv4Address;
    }

    string IpadmLocalManagedObject::getIPv4Address() const
    {
        return (m_IPv4Address);
    }

    void  IpadmLocalManagedObject::setIPv4Gateway(const IpV4Address &IPv4Gateway)
    {
        m_IPv4Gateway  =  IPv4Gateway;
    }

    IpV4Address  IpadmLocalManagedObject::getIPv4Gateway() const
    {
        return (m_IPv4Gateway);
    }

    void  IpadmLocalManagedObject::setDhcp(const bool &dhcp)
    {
        m_dhcp  =  dhcp;
    }

    bool IpadmLocalManagedObject::getIpv6Dhcp() const
    {
        return (m_dhcpv6);
    }
    void  IpadmLocalManagedObject::setIpv6Dhcp(const bool &dhcp)
    {
        m_dhcpv6  =  dhcp;
    }

    bool IpadmLocalManagedObject::getDhcp() const
    {
        return (m_dhcp);
    }

    void  IpadmLocalManagedObject::setIPv6Address(const string &Ipv6Address)
    {
        m_Ipv6Address = Ipv6Address ;
    }

    string IpadmLocalManagedObject::getIPv6Address() const
    {
        return (m_Ipv6Address);
    }

    void  IpadmLocalManagedObject::setAutoconfig(const bool &autoconfig)
    {
        m_autoconfig  =  autoconfig;
    }

    bool IpadmLocalManagedObject::getAutoconfig() const
    {
        return (m_autoconfig);
    }

    void  IpadmLocalManagedObject::set_link_speed(const LinkSpeed &speed)
    {
        m_link_speed  =  speed;
    }

    LinkSpeed IpadmLocalManagedObject::get_link_speed() const
    {
        return (m_link_speed);
    }

	void IpadmLocalManagedObject::set_oper_ip_address(const string &oper_ip_address) 
	{
		m_oper_ip_address = oper_ip_address;
	}
	
	string IpadmLocalManagedObject::get_oper_ip_address()  const
	{
		return (m_oper_ip_address);
	}

	void IpadmLocalManagedObject::set_oper_gateway(const IpV4Address &oper_gateway) 
	{
		m_oper_gateway = oper_gateway;
	}
	
	IpV4Address IpadmLocalManagedObject::get_oper_gateway()  const
	{
		return (m_oper_gateway);
	}

	void IpadmLocalManagedObject::set_oper_vector_ipv6_address(const vector<string> &oper_vector_ipv6_address) 
	{
		m_oper_vector_ipv6_address = oper_vector_ipv6_address;
	}
	
	vector<string> IpadmLocalManagedObject::get_oper_vector_ipv6_address()  const
	{
		return (m_oper_vector_ipv6_address);
	}

	void IpadmLocalManagedObject::set_oper_vector_ipv6_gateway(const vector<string> &oper_vector_ipv6_gateways) 
	{
		m_oper_vector_ipv6_gateways = oper_vector_ipv6_gateways;
	}
	
	vector<string> IpadmLocalManagedObject::get_oper_vector_ipv6_gateways()  const
	{
		return (m_oper_vector_ipv6_gateways);
	}

	void IpadmLocalManagedObject::set_oper_speed(const string &oper_speed) 
	{
		m_oper_speed = oper_speed;
	}
	
	string IpadmLocalManagedObject::get_oper_speed()  const
	{
		return (m_oper_speed);
	}

	void IpadmLocalManagedObject::set_oper_config_speed(const string &oper_config_speed) 
	{
		m_oper_config_speed = oper_config_speed;
	}
	
	string IpadmLocalManagedObject::get_oper_config_speed()  const
	{
		return (m_oper_config_speed);
	}
	/*
	 * this function searches the get/set at p_getset for a local interface of
	 * the specified entity, interface type, entity instance and address family
	 * and returns the local address table index containing the specified
	 * local interface.
	 *
	 * returns: >= 0 => valid local index
	 *	<0 => some error occurred (specified interface not found)
	 */

	int IpadmLocalManagedObject::locate_addr_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family, int nentry)
	{
		int i;
		int count = 0;

		for (i = 0; i < p_getset->num_locals; ++i) {
			ipadm_local_addr_id_t *p = &(p_getset->local_addr_ids[i]);
			//	trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::locate_addr_idx : ") + p->addr_family);
			if ((p->entity_type == entity) &&
				(p->entity_instance == entity_instance) &&
				(p->interface_type == interface) &&
				(p->addr_family == addr_family)) {
				// Only worry about interface matching for non-blades
				if ((entity != BLADE_ENTITY) ||
					(p->interface_instance == interface_instance)) {
					if (count == nentry)
						return (i);
					else
						count++;
				}
			}
		}
		return (-1);
	}

	/*
	 * this function searches the get/set at p_getset for a gateway of
	 * the specified instance and returns the gateway
	 * address table index containing the specified gateway.
	 *
	 * returns: >= 0 => valid gateway index
	 *	<0 => some error occurred (specified gateway not found)
	 */

	int IpadmLocalManagedObject::locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family, int nentry)
	{
		int i;
		int count = 0;

		for (i = 0; i < p_getset->num_gws; ++i) {
			//	trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::locate_addr_idx : ") + p_getset->gw_addr_ids[i].addr_family);
			if ((p_getset->gw_addr_ids[i].host_instance == host_instance) &&
				(p_getset->gw_addr_ids[i].host_type == host_type) &&
				(p_getset->gw_addr_ids[i].addr_family == addr_family)) {
					if (count == nentry)
						return (i);
					else
						count++;
			}
		}
		return (-1);
	}

}
