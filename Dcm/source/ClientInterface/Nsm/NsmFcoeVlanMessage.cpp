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

#include "ClientInterface/Nsm/NsmFcoeVlanMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmFcoeVlanMessage::NsmFcoeVlanMessage ()
        : DcmManagementInterfaceMessage (NSMFCOEVLAN)
    {
    }

    NsmFcoeVlanMessage::NsmFcoeVlanMessage (const UI32 &cmdCode, const UI32 &old_vlan,const UI32 &new_vlan, 
                                            const string &vlanName,const UI32 &vlanIfIndex,const string &desc)
        : DcmManagementInterfaceMessage (NSMFCOEVLAN),
        m_cmdCode    (cmdCode),
        m_old_vlan    (old_vlan),
        m_new_vlan (new_vlan),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc)
    {
    }

    NsmFcoeVlanMessage::~NsmFcoeVlanMessage ()
    {
    }

    void  NsmFcoeVlanMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_old_vlan,"old_vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
        addSerializableAttribute (new AttributeUI32(&m_new_vlan,"new_vlan"));
    }

    void  NsmFcoeVlanMessage::setOldVlan(const UI32 &old_vlan)
    {
        m_old_vlan = old_vlan;
    }

    UI32  NsmFcoeVlanMessage::getOldVlan() const
    {
        return (m_old_vlan);
    }
    void  NsmFcoeVlanMessage::setNewVlan(const UI32 &new_vlan)
    {
        m_new_vlan = new_vlan;
    }

    UI32  NsmFcoeVlanMessage::getNewVlan() const
    {
        return (m_new_vlan);
    }

    void  NsmFcoeVlanMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmFcoeVlanMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmFcoeVlanMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmFcoeVlanMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmFcoeVlanMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmFcoeVlanMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmFcoeVlanMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmFcoeVlanMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void *NsmFcoeVlanMessage::getCStructureForInputs ()
    {
        NsmFcoeVlanMsg_t *pInput = new NsmFcoeVlanMsg_t;

        strncpy ((char *)pInput->vlanName, m_vlanName.c_str (), MAX_STR);
        strncpy ((char *)pInput->desc, m_desc.c_str (), MAX_STR);
        pInput->old_vlan = m_old_vlan;
        pInput->new_vlan = m_new_vlan;
        pInput->vlanIfIndex = m_vlanIfIndex;
        pInput->cmd_code     = m_cmdCode;
        return (pInput);
    }

	void NsmFcoeVlanMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
