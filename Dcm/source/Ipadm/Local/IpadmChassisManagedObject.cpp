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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : cshah													 *
 **************************************************************************/
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Ipadm/Local/IpadmChassisManagedObject.h"
#include "Ipadm/Local/IpadmChassisObjectManager.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "vcs.h"

namespace DcmNs
{

#if 0
	IpadmChassisManagedObject::IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager)
		: PrismElement  (pIpadmChassisObjectManager),
		PrismPersistableObject (IpadmChassisManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObjectBase (this),
		DcmLocalManagedObject (pIpadmChassisObjectManager)
	{
	}
#endif
	    IpadmChassisManagedObject::IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager)
		: PrismElement  (pIpadmChassisObjectManager),
		PrismPersistableObject (IpadmChassisManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pIpadmChassisObjectManager),
		DcmManagedObject (pIpadmChassisObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pIpadmChassisObjectManager)
	    {
	    }

	    IpadmChassisManagedObject::IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager,const string &VipAddress, const string &VipV6Address, const string &oper_Vip_address, const vector<string> &oper_vector_VipV6_address)
		: PrismElement  (pIpadmChassisObjectManager),
		PrismPersistableObject (IpadmChassisManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pIpadmChassisObjectManager),
		DcmManagedObject (pIpadmChassisObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pIpadmChassisObjectManager),
		m_VipAddress	(VipAddress),
                m_VipV6Address (VipV6Address),
		m_oper_Vip_address (oper_Vip_address),
                m_oper_vector_VipV6_address (oper_vector_VipV6_address)

	    {
	    }
#if 0
	IpadmChassisManagedObject::IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager,const string &VipAddress)
		: PrismElement  (pIpadmChassisObjectManager),
		PrismPersistableObject (IpadmChassisManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObject (pIpadmChassisObjectManager),
		m_VipAddress	(VipAddress)
	{
	}
#endif


	IpadmChassisManagedObject::~IpadmChassisManagedObject ()
	{
	}

	string  IpadmChassisManagedObject::getClassName()
	{
		return ("IpadmChassisManagedObject");
	}

	void  IpadmChassisManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_VipAddress,true,"","VipAddress", chassis_virtual_ip));
		addPersistableAttribute (new AttributeString(&m_VipV6Address,true,"","VipV6Address", chassis_virtual_ipv6));
		addPersistableAttribute (new AttributeString(&m_oper_Vip_address,"oper_Vip_address", chassis_virtual_oper_Vip_address, true));
                addPersistableAttribute (new AttributeStringVector(&m_oper_vector_VipV6_address,"oper_vector_VipV6_address", chassis_virtual_oper_VipV6_address, true));
	}

	void  IpadmChassisManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeString(&m_VipAddress,"VipAddress", chassis_virtual_ip));
		addPersistableAttributeForCreate  (new AttributeString(&m_VipV6Address,"VipV6Address", chassis_virtual_ipv6));
		addPersistableAttributeForCreate  (new AttributeString(&m_oper_Vip_address,"oper_Vip_address", chassis_virtual_oper_Vip_address, true));
                addPersistableAttributeForCreate  (new AttributeStringVector(&m_oper_vector_VipV6_address,"oper_vector_VipV6_address", chassis_virtual_oper_VipV6_address, true));
	}

	void  IpadmChassisManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
	{
		vector<string> operationalFields         = pWaveManagedObjectLoadOperationalDataContext->getOperationalDataFields ();
		UI32           numberOfOperationalFields = operationalFields.size ();
		trace (TRACE_LEVEL_INFO, string ("IpadmChassisManagedObject::loadOperationalData : numberOfOperationalFields = ") + numberOfOperationalFields);
		string current_ip;
		string ipv4_address;
		char ipbuf[INET6_ADDRSTRLEN];
		char ipbuf6[INET6_ADDRSTRLEN];
		string str;
		char prefixbuf[8];
		char prefixbuf6[8];
		vector<string> current_ipv6_vector_address;
		unsigned int flags;
		ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
		struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
		addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
		struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
		addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];


		ipadm_ip_get_set_t current = {
			0, /* num_locals */
			local_addr_ids,
			local_addrs,
			local_prefixes,
			local_flags,
			0, /* num_gws */
			gw_addr_ids,
			gw_addrs,
			gw_flags,
			0 /* dhcp_flag */
		};

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		unsigned int i;
		int j;
		int nentry =0;
		int idx;
		string str_prefix, str_ipaddr;
		string str_prefix6, str_ipaddr6;

		string ipv6_address ("fe80::21e:bff:febf:47d4/64");
		int rc = 0;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData :ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("IpadmChassisManagedObject::loadOperationalData :ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}

		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData :ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		if (pizza)
			trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData : Not applicable for pizza box "));

		else {
			// Extract IPv4 address
			idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
			if (idx < 0) {
				trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData :could not locate IPv4 address for ethernet interface"));
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
			int eth_idx6 = -1;
			nentry = 0;
			for (j = 0; j < current.num_locals; j++) {

				eth_idx6 =  locate_addr_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6, nentry);
				if (eth_idx6 < 0) {
					if(nentry == 0) {
						trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData :could not locate IPv6 address for ethernet interface"));
					}
					break;
				}
				trace (TRACE_LEVEL_ERROR, string ("IpadmChassisManagedObject::loadOperationalData :eth_idx6")+ eth_idx6);

				if(current.local_flags[eth_idx6] & IPADM_ADDR_NONE) {
					nentry++;
					continue;
				}

				const char *rcc =inet_ntop(AF_INET6, &(SOCKADDR_IN6P(current.local_addrs[eth_idx6])->sin6_addr), ipbuf6, sizeof(ipbuf6));
				if(!rcc)
					trace (TRACE_LEVEL_ERROR, string ("inet_ntop() failed "));
				sprintf(prefixbuf6, "/%d ", current.local_prefixes[eth_idx6]);
				trace (TRACE_LEVEL_INFO, string("IpadmChassisLocalMangaedObject:  ipv6 address") + ipbuf6);
				string state,str6;
				string type;

				flags = current.local_flags[eth_idx6];
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

				str6 = type;
				str6.append(ipbuf6);
				str6.append(prefixbuf6);
				str6 += state;
				trace (TRACE_LEVEL_INFO, string("IpadmChassisLocalMangaedObject:  ipv6 address/prefix") + str6);
				current_ipv6_vector_address.push_back(str6);
				nentry++;
			}
		}
		if (!numberOfOperationalFields) {
			set_oper_Vip_address(ipv4_address);
			if (nentry != 0)
				set_oper_vector_VipV6_address(current_ipv6_vector_address);
		}
		else {
			for (i = 0; i < numberOfOperationalFields; i++) {
				if (!operationalFields[i].compare ("oper_Vip_address")) {
					trace (TRACE_LEVEL_INFO, string ("IpadmChassisManagedObject::loadOperationalData : extracting IPv4 address!"));
					set_oper_Vip_address(ipv4_address);
				}
				if (!operationalFields[i].compare ("oper_vector_VipV6_address")) {

					if (nentry != 0)
						set_oper_vector_VipV6_address(current_ipv6_vector_address);
					trace (TRACE_LEVEL_INFO, string ("IpadmChassisManagedObject::loadOperationalData : extracting IPv6 addresses!"));
				}

			}
		}
		pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveManagedObjectLoadOperationalDataContext->callback ();
	}

	int IpadmChassisManagedObject::locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family)
	{
		int i;

		for (i = 0; i < p_getset->num_locals; ++i) {
			ipadm_local_addr_id_t *p = &(p_getset->local_addr_ids[i]);
			if ((p->entity_type == entity) &&
					(p->entity_instance == entity_instance) &&
					(p->interface_type == interface) &&
					(p->addr_family == addr_family)) {
				// Only worry about interface matching for non-blades
				if ((entity != BLADE_ENTITY) ||
						(p->interface_instance == interface_instance))
					return (i);
			}
		}
		return (-1);
	}

	/*
	 * this function searches the get/set at p_getset for a local interface of
	 * the specified entity, interface type, entity instance and address family
	 * and returns the local address table index containing the specified
	 * local interface.
	 *
	 * returns: >= 0 => valid local index
	 *      <0 => some error occurred (specified interface not found)
	 */

	int IpadmChassisManagedObject::locate_addr_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family, int nentry)
	{
		int i;
		int count = 0;

		for (i = 0; i < p_getset->num_locals; ++i) {
			ipadm_local_addr_id_t *p = &(p_getset->local_addr_ids[i]);
			//      trace (TRACE_LEVEL_INFO, string ("IpadmLocalManagedObject::locate_addr_idx : ") + p->addr_family);
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
	
	void  IpadmChassisManagedObject::setVipAddress(const string &VipAddress)
	{
		m_VipAddress  =  VipAddress;
	}

	string  IpadmChassisManagedObject::getVipAddress() const
	{
		return (m_VipAddress);
	}

	void  IpadmChassisManagedObject::setVipV6Address(const string &VipV6Address)
	{
		m_VipV6Address  =  VipV6Address;
	}

	string  IpadmChassisManagedObject::getVipV6Address() const
	{
		return (m_VipV6Address);
	}
	void IpadmChassisManagedObject::set_oper_Vip_address(const string &oper_Vip_address)
	{
		m_oper_Vip_address = oper_Vip_address;
	}

	void IpadmChassisManagedObject::set_oper_vector_VipV6_address(const vector<string> &oper_vector_VipV6_address)
	{
		m_oper_vector_VipV6_address = oper_vector_VipV6_address;
	}

	vector<string> IpadmChassisManagedObject::get_oper_vector_VipV6_address()  const
	{
		return (m_oper_vector_VipV6_address);
	}
	string IpadmChassisManagedObject::get_oper_Vip_address()  const
	{
		return (m_oper_Vip_address);
	}

}
