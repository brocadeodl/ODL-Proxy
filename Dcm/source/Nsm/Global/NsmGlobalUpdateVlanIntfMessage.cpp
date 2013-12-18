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
 *   Author : skbalasu.                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeUI32Range.h"

namespace DcmNs
{

    NsmGlobalUpdateVlanIntfMessage::NsmGlobalUpdateVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVLANINTF)
    {
		 m_cmdCode = 0;
		 m_flag = false;
		 m_mtu = NSM_MTU_DEF_VAL;
		 m_pvlanType = PVLAN_TYPE_UNKNOWN;
		 m_secondaryVlanId = 1;
		 m_vlanIfIndex = 0;
		 m_pVlanSecAssocRange = UI32Range("");
		 m_remoteSpanEnabled = false;
		 
    }
    NsmGlobalUpdateVlanIntfMessage::NsmGlobalUpdateVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc,const bool &flag, const UI32 &mtu, const UI32 &cmdCode,const pvlanTypeEnum &pvlanType)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVLANINTF),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc),
        m_flag    (flag),
        m_pvlanType    (pvlanType),
        m_mtu        (mtu),
        m_cmdCode    (cmdCode)
    {
		m_secondaryVlanId = 1;
		m_remoteSpanEnabled = false;
    }

    NsmGlobalUpdateVlanIntfMessage::~NsmGlobalUpdateVlanIntfMessage ()
    {
    }

    void  NsmGlobalUpdateVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_pvlanType,"pvlanType"));
        addSerializableAttribute (new AttributeUI32Range(&m_pVlanSecAssocRange,"pVlanSecAssocRange"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeBool(&m_remoteSpanEnabled,"remoteSpanEnabled"));
    }

    void  NsmGlobalUpdateVlanIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalUpdateVlanIntfMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmGlobalUpdateVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmGlobalUpdateVlanIntfMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmGlobalUpdateVlanIntfMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmGlobalUpdateVlanIntfMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmGlobalUpdateVlanIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalUpdateVlanIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalUpdateVlanIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmGlobalUpdateVlanIntfMessage::getFlag() const
    {
        return (m_flag);
    }
    void  NsmGlobalUpdateVlanIntfMessage::setPvlanType(const pvlanTypeEnum &pvlanType)
    {
        m_pvlanType  =  pvlanType;
    }

    pvlanTypeEnum NsmGlobalUpdateVlanIntfMessage::getPvlanType() const
    {
        return (m_pvlanType);
    }

    void NsmGlobalUpdateVlanIntfMessage::setPvlanSecAssocRange(const UI32Range &pVlanSecAssocRange)
    	{
    	m_pVlanSecAssocRange = pVlanSecAssocRange;
    	}

    UI32Range NsmGlobalUpdateVlanIntfMessage::getPvlanSecAssocRange() const
    	{
    		return (m_pVlanSecAssocRange);
    	}

    void  NsmGlobalUpdateVlanIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmGlobalUpdateVlanIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmGlobalUpdateVlanIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmGlobalUpdateVlanIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void  NsmGlobalUpdateVlanIntfMessage::setRemoteSpanEnabled(const bool &flag)
    {
    	m_remoteSpanEnabled  =  flag;
    }

    bool  NsmGlobalUpdateVlanIntfMessage::getRemoteSpanEnabled() const
    {
        return (m_remoteSpanEnabled);
    }
}
