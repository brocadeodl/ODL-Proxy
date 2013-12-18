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
 *   Author : krao                                                     *
 **************************************************************************/

#include "ClientInterface/VRRP/VRRPGlobalConfigSPMessage.h"
#include "VRRP/Global/VRRPGlobalObjectManager.h"
#include "VRRP/Global/VRRPGlobalTypes.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"

namespace DcmNs
{

    VRRPGlobalConfigSPMessage::VRRPGlobalConfigSPMessage ()
        : DcmManagementInterfaceMessage (VRRPGLOBALUPDATEVRRPGLOBALCONFIG)
    {
    }

    VRRPGlobalConfigSPMessage::VRRPGlobalConfigSPMessage (const bool &config_disable, const bool &vrrp_enable, const bool &vrrpe_enable)
        : DcmManagementInterfaceMessage (VRRPGLOBALUPDATEVRRPGLOBALCONFIG),
		m_config_disable (config_disable),
        m_vrrp_enable    (vrrp_enable),
        m_vrrpe_enable    (vrrpe_enable)
    {
    }

    VRRPGlobalConfigSPMessage::~VRRPGlobalConfigSPMessage ()
    {
    }

    void  VRRPGlobalConfigSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_config_disable,"config_disable"));
        addSerializableAttribute (new AttributeBool(&m_vrrp_enable,"vrrp_enable"));
        addSerializableAttribute (new AttributeBool(&m_vrrpe_enable,"vrrpe_enable"));
    }

    void  VRRPGlobalConfigSPMessage::setConfigDisable(const bool &config_disable)
    {
        m_config_disable  =  config_disable;
    }

    bool  VRRPGlobalConfigSPMessage::getConfigDisable() const
    {
        return (m_config_disable);
    }

    void  VRRPGlobalConfigSPMessage::setVrrpEnable(const bool &vrrp_enable)
    {
        m_vrrp_enable  =  vrrp_enable;
    }

    bool  VRRPGlobalConfigSPMessage::getVrrpEnable() const
    {
        return (m_vrrp_enable);
    }

    void  VRRPGlobalConfigSPMessage::setVrrpeEnable(const bool &vrrpe_enable)
    {
        m_vrrpe_enable  =  vrrpe_enable;
    }

    bool  VRRPGlobalConfigSPMessage::getVrrpeEnable() const
    {
        return (m_vrrpe_enable);
    }

    const void *VRRPGlobalConfigSPMessage::getCStructureForInputs ()
    {
        vrrp_global_config_msg_t *pInput = (vrrp_global_config_msg_t *) malloc (sizeof (struct vrrp_global_config_msg_));
    
		pInput->config_disable = m_config_disable;
        pInput->vrrp_config = m_vrrp_enable;
        pInput->vrrpe_config = m_vrrpe_enable;

        return (pInput);
    }


    void VRRPGlobalConfigSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
