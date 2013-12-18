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

#include "VRRP/Global/VRRPGlobalUpdateVRRPGlobalConfigMessage.h"
#include "VRRP/Global/VRRPGlobalObjectManager.h"
#include "VRRP/Global/VRRPGlobalTypes.h"

namespace DcmNs
{

    VRRPGlobalUpdateVRRPGlobalConfigMessage::VRRPGlobalUpdateVRRPGlobalConfigMessage ()
        : ManagementInterfaceMessage (VRRPGlobalObjectManager::getClassName (),VRRPGLOBALUPDATEVRRPGLOBALCONFIG)
    {
    }

    VRRPGlobalUpdateVRRPGlobalConfigMessage::VRRPGlobalUpdateVRRPGlobalConfigMessage (const bool &config_disable, const bool &enable,const bool &vrrpe_enable)
        : ManagementInterfaceMessage (VRRPGlobalObjectManager::getClassName (),VRRPGLOBALUPDATEVRRPGLOBALCONFIG),
		m_config_disable (config_disable),
        m_vrrp_enable    (enable),
        m_vrrpe_enable  (vrrpe_enable)
    {
    }

    VRRPGlobalUpdateVRRPGlobalConfigMessage::~VRRPGlobalUpdateVRRPGlobalConfigMessage ()
    {
    }

    void  VRRPGlobalUpdateVRRPGlobalConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_config_disable,"config_disable"));
        addSerializableAttribute (new AttributeBool(&m_vrrp_enable,"vrrp_enable"));
        addSerializableAttribute (new AttributeBool(&m_vrrpe_enable,"vrrpe_enable"));
    }

    void  VRRPGlobalUpdateVRRPGlobalConfigMessage::setConfigDisable(const bool &config_disable)
    {
        m_config_disable  =  config_disable;
    }

    bool  VRRPGlobalUpdateVRRPGlobalConfigMessage::getConfigDisable() const
    {
        return (m_config_disable);
    }

    void  VRRPGlobalUpdateVRRPGlobalConfigMessage::setVrrpEnable(const bool &enable)
    {
        m_vrrp_enable  =  enable;
    }

    bool  VRRPGlobalUpdateVRRPGlobalConfigMessage::getVrrpEnable() const
    {
        return (m_vrrp_enable);
    }

    void  VRRPGlobalUpdateVRRPGlobalConfigMessage::setVrrpeEnable(const bool &vrrpe_enable)
    {
        m_vrrpe_enable  =  vrrpe_enable;
    }

    bool  VRRPGlobalUpdateVRRPGlobalConfigMessage::getVrrpeEnable() const
    {
        return (m_vrrpe_enable);
    }

}
