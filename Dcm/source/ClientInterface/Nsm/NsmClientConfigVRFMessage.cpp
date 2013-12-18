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
 *   Author : pvaish                                                       *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include <arpa/inet.h>

namespace DcmNs
{

    NsmClientConfigVRFMessage::NsmClientConfigVRFMessage ()
        : DcmManagementInterfaceMessage (MSG_VRF_INT)
    {
    }

    NsmClientConfigVRFMessage::NsmClientConfigVRFMessage (const string &vrfName,const string &ifName,const UI32 &opcode)
        : DcmManagementInterfaceMessage (MSG_VRF_INT),
        m_vrfName	(vrfName),
        m_ifName    (ifName),
        m_opcode    (opcode)
    {
    }

    NsmClientConfigVRFMessage::~NsmClientConfigVRFMessage ()
    {
    }

    void  NsmClientConfigVRFMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_opcode, "opcode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeString(&m_rd,"rd"));
        //addSerializableAttribute (new AttributeBool(&m_status, "status"));
    }

    void  NsmClientConfigVRFMessage::setVrfName(const string &vrfName)
    {
        m_vrfName  =  vrfName;
    }

    void  NsmClientConfigVRFMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmClientConfigVRFMessage::getVrfName() const
    {
        return (m_vrfName);
    }

    string  NsmClientConfigVRFMessage::getIfName() const
    {
        return (m_ifName);
    }

    void NsmClientConfigVRFMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

    UI32  NsmClientConfigVRFMessage::getIfType() const
    {
        return (m_ifType);
    }

    void NsmClientConfigVRFMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode = opcode;
    }

    UI32  NsmClientConfigVRFMessage::getOpcode() const
    {
        return (m_opcode);
    }


    void  NsmClientConfigVRFMessage::setRd(const string &rd)
    {
        m_rd  =  rd;
    }

    string  NsmClientConfigVRFMessage::getRd( ) const
    {
        return m_rd;
    }
	
    const void *NsmClientConfigVRFMessage::getCStructureForInputs ()
    {
       	nsm_vrf_global_config_msg_t            *msg_p;

        msg_p = (nsm_vrf_global_config_msg_t *)calloc(1, sizeof(nsm_vrf_global_config_msg_t));

        msg_p->opcode = m_opcode;
        strncpy(msg_p->vrf_name, m_vrfName.c_str (), NSM_INTF_NAME_SIZE);

	    if( NSM_MSG_VRF_BIND_INTF == m_opcode || NSM_MSG_VRF_UNBIND_INTF == m_opcode) {
       		strncpy(msg_p->if_name, m_ifName.c_str (), NSM_INTF_NAME_SIZE);
            msg_p->if_type = m_ifType;
	    } 

        return msg_p;
    	
    }

    void NsmClientConfigVRFMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
