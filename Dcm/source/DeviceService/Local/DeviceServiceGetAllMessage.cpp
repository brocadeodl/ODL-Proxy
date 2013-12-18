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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "DeviceService/Local/DeviceServiceLocalObjectManager.h"
#include "DeviceService/Local/DeviceServiceGetAllMessage.h"
#include "DeviceService/Local/DeviceServiceTypes.h"

namespace DcmNs
{

    DeviceServiceGetAllMessage::DeviceServiceGetAllMessage ()
        : PrismMessage (DeviceServiceLocalObjectManager::getPrismServiceId (), DEVICESERVICEGETALL)
    {
    }

    DeviceServiceGetAllMessage::DeviceServiceGetAllMessage (const UI32 &domain_id,									const UI32 &request_type,						const vector<UI32> &port_id, 
															const vector<UI32> &port_type,							const vector<UI32> &class_of_service,			const vector<WorldWideName> &port_wwn_name, 
															const vector<WorldWideName> &node_wwn_name,				const vector<UI32> &state_change_registration,	const vector<UI32> &fc4s_supported, 
															const vector<string> &port_symbolic_name,				const vector<string> &node_symbolic_name,		const vector<WorldWideName> &fabric_port_wwn_name, 
															const vector<WorldWideName> &permanent_port_wwn_name,	const vector<UI32> &port_index,					const vector<UI32> &shared_area, 
															const vector<UI32> &frame_redirection,					const vector<string> &inq_string,				const vector<UI32> &partial_entry, 
															const vector<UI32> &device_type,						const vector<IpV6Address> &device_ip_addr,		const vector<UI32> &device_ipa, 
															const vector<IpV6Address> &device_ip_port,				const vector<UI32> &is_connected_via_ag,		const vector<UI32> &is_real,
															const vector<UI32> &is_cascaded,						const vector<UI32> &is_ag_base_device)
        : PrismMessage (DeviceServiceLocalObjectManager::getPrismServiceId (), DEVICESERVICEGETALL),
        m_domain_id    (domain_id),
        m_request_type    (request_type),
        m_port_id    (port_id),
        m_port_type    (port_type),
        m_class_of_service    (class_of_service),
        m_port_wwn_name    (port_wwn_name),
        m_node_wwn_name    (node_wwn_name),
        m_state_change_registration    (state_change_registration),
        m_fc4s_supported    (fc4s_supported),
        m_port_symbolic_name    (port_symbolic_name),
        m_node_symbolic_name    (node_symbolic_name),
        m_fabric_port_wwn_name    (fabric_port_wwn_name),
        m_permanent_port_wwn_name    (permanent_port_wwn_name),
        m_port_index    (port_index),
        m_shared_area    (shared_area),
        m_frame_redirection    (frame_redirection),
        m_inq_string    (inq_string),
        m_partial_entry    (partial_entry),
        m_device_type    (device_type),
        m_device_ip_addr    (device_ip_addr),
        m_device_ipa    (device_ipa),
        m_device_ip_port    (device_ip_port),
		m_is_connected_via_ag	(is_connected_via_ag),
		m_is_real	(is_real),
		m_is_cascaded	(is_cascaded),
		m_is_ag_base_device	(is_ag_base_device)
    {
    }

    DeviceServiceGetAllMessage::~DeviceServiceGetAllMessage ()
    {
    }

    void  DeviceServiceGetAllMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeUI32(&m_request_type,"request_type"));
        addSerializableAttribute (new AttributeUI32Vector(&m_port_id,"port_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_port_type,"port_type"));
        addSerializableAttribute (new AttributeUI32Vector(&m_class_of_service,"class_of_service"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_port_wwn_name,"port_wwn_name"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_node_wwn_name,"node_wwn_name"));
        addSerializableAttribute (new AttributeUI32Vector(&m_state_change_registration,"state_change_registration"));
        addSerializableAttribute (new AttributeUI32Vector(&m_fc4s_supported,"fc4s_supported"));
        addSerializableAttribute (new AttributeStringVector(&m_port_symbolic_name,"port_symbolic_name"));
        addSerializableAttribute (new AttributeStringVector(&m_node_symbolic_name,"node_symbolic_name"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_fabric_port_wwn_name,"fabric_port_wwn_name"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_permanent_port_wwn_name,"permanent_port_wwn_name"));
        addSerializableAttribute (new AttributeUI32Vector(&m_port_index,"port_index"));
        addSerializableAttribute (new AttributeUI32Vector(&m_shared_area,"shared_area"));
        addSerializableAttribute (new AttributeUI32Vector(&m_frame_redirection,"frame_redirection"));
        addSerializableAttribute (new AttributeStringVector(&m_inq_string,"inq_string"));
        addSerializableAttribute (new AttributeUI32Vector(&m_partial_entry,"partial_entry"));
        addSerializableAttribute (new AttributeUI32Vector(&m_device_type,"device_type"));
        addSerializableAttribute (new AttributeIpV6AddressVector(&m_device_ip_addr,"device_ip_addr"));
        addSerializableAttribute (new AttributeUI32Vector(&m_device_ipa,"device_ipa"));
        addSerializableAttribute (new AttributeIpV6AddressVector(&m_device_ip_port,"device_ip_port"));
		addSerializableAttribute (new AttributeUI32Vector(&m_is_connected_via_ag,"is_connected_via_ag"));
		addSerializableAttribute (new AttributeUI32Vector(&m_is_real,"is_real"));
		addSerializableAttribute (new AttributeUI32Vector(&m_is_cascaded,"is_cascaded"));
		addSerializableAttribute (new AttributeUI32Vector(&m_is_ag_base_device,"is_ag_base_device"));
    }

    void  DeviceServiceGetAllMessage::setDomain_id(const UI32 &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    UI32  DeviceServiceGetAllMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  DeviceServiceGetAllMessage::setRequest_type(const UI32 &request_type)
    {
        m_request_type  =  request_type;
    }

    UI32  DeviceServiceGetAllMessage::getRequest_type() const
    {
        return (m_request_type);
    }

    void  DeviceServiceGetAllMessage::setPort_id(const vector<UI32> &port_id)
    {
        m_port_id  =  port_id;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getPort_id() const
    {
        return (m_port_id);
    }

    void  DeviceServiceGetAllMessage::setPort_type(const vector<UI32> &port_type)
    {
        m_port_type  =  port_type;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getPort_type() const
    {
        return (m_port_type);
    }

    void  DeviceServiceGetAllMessage::setClass_of_service(const vector<UI32> &class_of_service)
    {
        m_class_of_service  =  class_of_service;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getClass_of_service() const
    {
        return (m_class_of_service);
    }

    void  DeviceServiceGetAllMessage::setPort_wwn_name(const vector<WorldWideName> &port_wwn_name)
    {
        m_port_wwn_name  =  port_wwn_name;
    }

    vector<WorldWideName>  DeviceServiceGetAllMessage::getPort_wwn_name() const
    {
        return (m_port_wwn_name);
    }

    void  DeviceServiceGetAllMessage::setNode_wwn_name(const vector<WorldWideName> &node_wwn_name)
    {
        m_node_wwn_name  =  node_wwn_name;
    }

    vector<WorldWideName>  DeviceServiceGetAllMessage::getNode_wwn_name() const
    {
        return (m_node_wwn_name);
    }

    void  DeviceServiceGetAllMessage::setState_change_registration(const vector<UI32> &state_change_registration)
    {
        m_state_change_registration  =  state_change_registration;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getState_change_registration() const
    {
        return (m_state_change_registration);
    }

    void  DeviceServiceGetAllMessage::setFc4s_supported(const vector<UI32> &fc4s_supported)
    {
        m_fc4s_supported  =  fc4s_supported;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getFc4s_supported() const
    {
        return (m_fc4s_supported);
    }

    void  DeviceServiceGetAllMessage::setPort_symbolic_name(const vector<string> &port_symbolic_name)
    {
        m_port_symbolic_name  =  port_symbolic_name;
    }

    vector<string>  DeviceServiceGetAllMessage::getPort_symbolic_name() const
    {
        return (m_port_symbolic_name);
    }

    void  DeviceServiceGetAllMessage::setNode_symbolic_name(const vector<string> &node_symbolic_name)
    {
        m_node_symbolic_name  =  node_symbolic_name;
    }

    vector<string>  DeviceServiceGetAllMessage::getNode_symbolic_name() const
    {
        return (m_node_symbolic_name);
    }

    void  DeviceServiceGetAllMessage::setFabric_port_wwn_name(const vector<WorldWideName> &fabric_port_wwn_name)
    {
        m_fabric_port_wwn_name  =  fabric_port_wwn_name;
    }

    vector<WorldWideName>  DeviceServiceGetAllMessage::getFabric_port_wwn_name() const
    {
        return (m_fabric_port_wwn_name);
    }

    void  DeviceServiceGetAllMessage::setPermanent_port_wwn_name(const vector<WorldWideName> &permanent_port_wwn_name)
    {
        m_permanent_port_wwn_name  =  permanent_port_wwn_name;
    }

    vector<WorldWideName>  DeviceServiceGetAllMessage::getPermanent_port_wwn_name() const
    {
        return (m_permanent_port_wwn_name);
    }

    void  DeviceServiceGetAllMessage::setPort_index(const vector<UI32> &port_index)
    {
        m_port_index  =  port_index;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getPort_index() const
    {
        return (m_port_index);
    }

    void  DeviceServiceGetAllMessage::setShared_area(const vector<UI32> &shared_area)
    {
        m_shared_area  =  shared_area;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getShared_area() const
    {
        return (m_shared_area);
    }

    void  DeviceServiceGetAllMessage::setFrame_redirection(const vector<UI32> &frame_redirection)
    {
        m_frame_redirection  =  frame_redirection;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getFrame_redirection() const
    {
        return (m_frame_redirection);
    }

    void  DeviceServiceGetAllMessage::setInq_string(const vector<string> &inq_string)
    {
        m_inq_string  =  inq_string;
    }

    vector<string>  DeviceServiceGetAllMessage::getInq_string() const
    {
        return (m_inq_string);
    }

    void  DeviceServiceGetAllMessage::setPartial_entry(const vector<UI32> &partial_entry)
    {
        m_partial_entry  =  partial_entry;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getPartial_entry() const
    {
        return (m_partial_entry);
    }

    void  DeviceServiceGetAllMessage::setDevice_type(const vector<UI32> &device_type)
    {
        m_device_type  =  device_type;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getDevice_type() const
    {
        return (m_device_type);
    }

    void  DeviceServiceGetAllMessage::setDevice_ip_addr(const vector<IpV6Address> &device_ip_addr)
    {
        m_device_ip_addr  =  device_ip_addr;
    }

    vector<IpV6Address>  DeviceServiceGetAllMessage::getDevice_ip_addr() const
    {
        return (m_device_ip_addr);
    }

    void  DeviceServiceGetAllMessage::setDevice_ipa(const vector<UI32> &device_ipa)
    {
        m_device_ipa  =  device_ipa;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getDevice_ipa() const
    {
        return (m_device_ipa);
    }

    void  DeviceServiceGetAllMessage::setDevice_ip_port(const vector<IpV6Address> &device_ip_port)
    {
        m_device_ip_port  =  device_ip_port;
    }

    vector<IpV6Address>  DeviceServiceGetAllMessage::getDevice_ip_port() const
    {
        return (m_device_ip_port);
    }

	void  DeviceServiceGetAllMessage::setIs_connected_via_ag(const vector<UI32> &is_connected_via_ag)
    {
        m_is_connected_via_ag  =  is_connected_via_ag;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getIs_connected_via_ag() const
    {
        return (m_is_connected_via_ag);
    }

	void  DeviceServiceGetAllMessage::setIs_real(const vector<UI32> &is_real)
    {
        m_is_real  =  is_real;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getIs_real() const
    {
        return (m_is_real);
    }

	void  DeviceServiceGetAllMessage::setIs_cascaded(const vector<UI32> &is_cascaded)
    {
        m_is_cascaded  =  is_cascaded;
    }

    vector<UI32>  DeviceServiceGetAllMessage::getIs_cascaded() const
    {
        return (m_is_cascaded);
    }

	void DeviceServiceGetAllMessage::setIs_ag_base_device(const vector<UI32> &is_ag_base_device)
	{
		m_is_ag_base_device = is_ag_base_device;
	}

	vector<UI32> DeviceServiceGetAllMessage::getIs_ag_base_device() const
	{
		return (m_is_ag_base_device);
	} 
}
