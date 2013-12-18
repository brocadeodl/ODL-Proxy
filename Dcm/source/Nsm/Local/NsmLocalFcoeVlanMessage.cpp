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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalFcoeVlanMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/NsmLocalObjectManager.h"

namespace DcmNs
{

    NsmLocalFcoeVlanMessage::NsmLocalFcoeVlanMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALFCOEVLAN)
    {
		m_cmdCode = 0;
		m_new_vlan = 1;
		m_old_vlan = 1;
		m_vlanIfIndex = 0;
    }
    NsmLocalFcoeVlanMessage::NsmLocalFcoeVlanMessage (const UI32 &cmd_code, const UI32 &old_vlan, const UI32 &new_vlan, const string &vlanName, const UI32 &vlanIfIndex, const string &desc)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALFCOEVLAN),
        m_cmdCode    (cmd_code),
        m_old_vlan    (old_vlan),
        m_new_vlan    (new_vlan),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc)
    {
    }

    NsmLocalFcoeVlanMessage::~NsmLocalFcoeVlanMessage ()
    {
    }

    void  NsmLocalFcoeVlanMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_old_vlan,"OldVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_new_vlan,"NewVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmLocalFcoeVlanMessage::setOldVlan(const UI32 &old_vlan)
    {
        m_old_vlan =  old_vlan;
    }

    UI32  NsmLocalFcoeVlanMessage::getOldVlan() const
    {
        return (m_old_vlan);
    }

    void  NsmLocalFcoeVlanMessage::setNewVlan(const UI32 &new_vlan)
    {
        m_new_vlan =  new_vlan;
    }

    UI32  NsmLocalFcoeVlanMessage::getNewVlan() const
    {
        return (m_new_vlan);
    }

    void  NsmLocalFcoeVlanMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmLocalFcoeVlanMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmLocalFcoeVlanMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmLocalFcoeVlanMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmLocalFcoeVlanMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmLocalFcoeVlanMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmLocalFcoeVlanMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalFcoeVlanMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
