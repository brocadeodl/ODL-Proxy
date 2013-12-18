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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalFcoeVlanMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalFcoeVlanMessage::NsmGlobalFcoeVlanMessage ()
        : PrismMessage (NsmGlobalObjectManager::getPrismServiceId (), NSMGLOBALFCOEVLAN)
    {
		m_cmdCode = 0;
		m_new_vlan = 1;
		m_old_vlan = 1;
		m_vlanIfIndex = 0;
    }
    NsmGlobalFcoeVlanMessage::NsmGlobalFcoeVlanMessage (const UI32 &cmd_code, 
                              const UI32 &old_vlan, 
                              const UI32 &new_vlan, 
                              const string &vlanName, 
                              const UI32 &vlanIfIndex, 
                              const string &vlan_desc)
        : PrismMessage (NsmGlobalObjectManager::getPrismServiceId (), NSMGLOBALFCOEVLAN),
        m_cmdCode    (cmd_code),
        m_old_vlan    (old_vlan),
        m_new_vlan    (new_vlan),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (vlan_desc)
    {
    }

    NsmGlobalFcoeVlanMessage::~NsmGlobalFcoeVlanMessage ()
    {
    }

    void  NsmGlobalFcoeVlanMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_old_vlan,"OldVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_new_vlan,"NewVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmGlobalFcoeVlanMessage::setOldVlan(const UI32 &old_vlan)
    {
        m_old_vlan =  old_vlan;
    }

    UI32  NsmGlobalFcoeVlanMessage::getOldVlan() const
    {
        return (m_old_vlan);
    }

    void  NsmGlobalFcoeVlanMessage::setNewVlan(const UI32 &new_vlan)
    {
        m_new_vlan =  new_vlan;
    }

    UI32  NsmGlobalFcoeVlanMessage::getNewVlan() const
    {
        return (m_new_vlan);
    }

    void  NsmGlobalFcoeVlanMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmGlobalFcoeVlanMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmGlobalFcoeVlanMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmGlobalFcoeVlanMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmGlobalFcoeVlanMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalFcoeVlanMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalFcoeVlanMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmGlobalFcoeVlanMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
