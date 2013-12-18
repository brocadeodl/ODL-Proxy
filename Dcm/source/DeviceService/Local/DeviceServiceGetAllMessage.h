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

#ifndef DEVICESERVICEGETALLMESSAGE_H
#define DEVICESERVICEGETALLMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Types/IpV6Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class DeviceServiceGetAllMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DeviceServiceGetAllMessage ();
            DeviceServiceGetAllMessage (const UI32 &domain_id, 									const UI32 &request_type, 						const vector<UI32> &port_id,
										const vector<UI32> &port_type, 							const vector<UI32> &class_of_service, 			const vector<WorldWideName> &port_wwn_name,
										const vector<WorldWideName> &node_wwn_name, 			const vector<UI32> &state_change_registration,	const vector<UI32> &fc4s_supported,
										const vector<string> &port_symbolic_name, 				const vector<string> &node_symbolic_name, 		const vector<WorldWideName> &fabric_port_wwn_name,
										const vector<WorldWideName> &permanent_port_wwn_name,	const vector<UI32> &port_index, 				const vector<UI32> &shared_area, 
										const vector<UI32> &frame_redirection, 					const vector<string> &inq_string, 				const vector<UI32> &partial_entry, 
										const vector<UI32> &device_type, 						const vector<IpV6Address> &device_ip_addr, 		const vector<UI32> &device_ipa, 
										const vector<IpV6Address> &device_ip_port,				const vector<UI32> &is_connected_via_ag,		const vector<UI32> &is_real,
										const vector<UI32> &is_cascaded,                        const vector<UI32> &is_ag_base_device);
            virtual    ~DeviceServiceGetAllMessage ();
            void setDomain_id(const UI32 &domain_id);
            UI32 getDomain_id()  const;
            void setRequest_type(const UI32 &request_type);
            UI32 getRequest_type()  const;
            void setPort_id(const vector<UI32> &port_id);
            vector<UI32> getPort_id()  const;
            void setPort_type(const vector<UI32> &port_type);
            vector<UI32> getPort_type()  const;
            void setClass_of_service(const vector<UI32> &class_of_service);
            vector<UI32> getClass_of_service()  const;
            void setPort_wwn_name(const vector<WorldWideName> &port_wwn_name);
            vector<WorldWideName> getPort_wwn_name()  const;
            void setNode_wwn_name(const vector<WorldWideName> &node_wwn_name);
            vector<WorldWideName> getNode_wwn_name()  const;
            void setState_change_registration(const vector<UI32> &state_change_registration);
            vector<UI32> getState_change_registration()  const;
            void setFc4s_supported(const vector<UI32> &fc4s_supported);
            vector<UI32> getFc4s_supported()  const;
            void setPort_symbolic_name(const vector<string> &port_symbolic_name);
            vector<string> getPort_symbolic_name()  const;
            void setNode_symbolic_name(const vector<string> &node_symbolic_name);
            vector<string> getNode_symbolic_name()  const;
            void setFabric_port_wwn_name(const vector<WorldWideName> &fabric_port_wwn_name);
            vector<WorldWideName> getFabric_port_wwn_name()  const;
            void setPermanent_port_wwn_name(const vector<WorldWideName> &permanent_port_wwn_name);
            vector<WorldWideName> getPermanent_port_wwn_name()  const;
            void setPort_index(const vector<UI32> &port_index);
            vector<UI32> getPort_index()  const;
            void setShared_area(const vector<UI32> &shared_area);
            vector<UI32> getShared_area()  const;
            void setFrame_redirection(const vector<UI32> &frame_redirection);
            vector<UI32> getFrame_redirection()  const;
            void setInq_string(const vector<string> &inq_string);
            vector<string> getInq_string()  const;
            void setPartial_entry(const vector<UI32> &partial_entry);
            vector<UI32> getPartial_entry()  const;
            void setDevice_type(const vector<UI32> &device_type);
            vector<UI32> getDevice_type()  const;
            void setDevice_ip_addr(const vector<IpV6Address> &device_ip_addr);
            vector<IpV6Address> getDevice_ip_addr()  const;
            void setDevice_ipa(const vector<UI32> &device_ipa);
            vector<UI32> getDevice_ipa()  const;
            void setDevice_ip_port(const vector<IpV6Address> &device_ip_port);
            vector<IpV6Address> getDevice_ip_port()  const;
			void setIs_connected_via_ag(const vector<UI32> &is_connected_via_ag);
            vector<UI32> getIs_connected_via_ag()  const;
			void setIs_real(const vector<UI32> &is_real);
            vector<UI32> getIs_real()  const;
			void setIs_cascaded(const vector<UI32> &is_cascaded);
            vector<UI32> getIs_cascaded()  const;
			void setIs_ag_base_device(const vector<UI32> &is_ag_base_device);
            vector<UI32> getIs_ag_base_device()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_domain_id;
            UI32  m_request_type;
            vector<UI32>  m_port_id;
            vector<UI32>  m_port_type;
            vector<UI32>  m_class_of_service;
            vector<WorldWideName>  m_port_wwn_name;
            vector<WorldWideName>  m_node_wwn_name;
            vector<UI32>  m_state_change_registration;
            vector<UI32>  m_fc4s_supported;
            vector<string>  m_port_symbolic_name;
            vector<string>  m_node_symbolic_name;
            vector<WorldWideName>  m_fabric_port_wwn_name;
            vector<WorldWideName>  m_permanent_port_wwn_name;
            vector<UI32>  m_port_index;
            vector<UI32>  m_shared_area;
            vector<UI32>  m_frame_redirection;
            vector<string>  m_inq_string;
            vector<UI32>  m_partial_entry;
            vector<UI32>  m_device_type;
            vector<IpV6Address>  m_device_ip_addr;
            vector<UI32>  m_device_ipa;
            vector<IpV6Address>  m_device_ip_port;
			vector<UI32>  m_is_connected_via_ag;
			vector<UI32>  m_is_real;
			vector<UI32>  m_is_cascaded;
			vector<UI32>  m_is_ag_base_device;
            
            enum request_type_enum {
                get_all_type,
                get_detail_type,
                node_find_type,
				zone_member_type
            };
    };
}
#endif                                            //DEVICESERVICEGETALLMESSAGE_H
