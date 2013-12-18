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

#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "VRRP/Local/VRRPUpdateVRRPConfigMessage.h"
#include "VRRP/Local/VRRPTypes.h"

namespace DcmNs
{

    VRRPUpdateVRRPConfigMessage::VRRPUpdateVRRPConfigMessage ()
        : ManagementInterfaceMessage (VRRPLocalObjectManager::getClassName (),VRRPUPDATEVRRPCONFIG)
    {
    }

    VRRPUpdateVRRPConfigMessage::VRRPUpdateVRRPConfigMessage (const bool &config_disable, const bool &vrrp_enable, const bool &vrrpe_enable)
        : ManagementInterfaceMessage (VRRPLocalObjectManager::getClassName (),VRRPUPDATEVRRPCONFIG),
		m_config_disable (config_disable),
        m_vrrp_enable    (vrrp_enable),
        m_vrrpe_enable    (vrrpe_enable)
    {
    }

    VRRPUpdateVRRPConfigMessage::~VRRPUpdateVRRPConfigMessage ()
    {
    }

    void  VRRPUpdateVRRPConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_config_disable,"config_disable"));
        addSerializableAttribute (new AttributeBool(&m_vrrp_enable,"vrrp_enable"));
        addSerializableAttribute (new AttributeBool(&m_vrrpe_enable,"vrrpe_enable"));
    }

    void  VRRPUpdateVRRPConfigMessage::setConfigDisable(const bool &config_disable)
    {
        m_config_disable  =  config_disable;
    }

    bool  VRRPUpdateVRRPConfigMessage::getConfigDisable() const
    {
        return (m_config_disable);
    }

    void  VRRPUpdateVRRPConfigMessage::setVrrpEnable(const bool &vrrp_enable)
    {
        m_vrrp_enable  =  vrrp_enable;
    }

    bool  VRRPUpdateVRRPConfigMessage::getVrrpEnable() const
    {
        return (m_vrrp_enable);
    }

    void  VRRPUpdateVRRPConfigMessage::setVrrpeEnable(const bool &vrrpe_enable)
    {
        m_vrrpe_enable  =  vrrpe_enable;
    }

    bool  VRRPUpdateVRRPConfigMessage::getVrrpeEnable() const
    {
        return (m_vrrpe_enable);
    }

}
