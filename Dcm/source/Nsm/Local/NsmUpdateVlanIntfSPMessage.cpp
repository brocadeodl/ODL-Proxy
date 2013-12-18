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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdateVlanIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeUI32Range.h"

namespace DcmNs
{

    NsmUpdateVlanIntfSPMessage::NsmUpdateVlanIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVLANINTFSP)
    {
		m_cmdCode = 0;
		m_flag = false;
		m_mtu = NSM_IP_MTU_DEF_VAL; 
		m_vlanIfIndex = 0;
		m_pvlanType = PVLAN_TYPE_UNKNOWN;
		m_pVlanSecAssocRange = UI32Range("");
		m_replayInProgress = false;
    }

    NsmUpdateVlanIntfSPMessage::NsmUpdateVlanIntfSPMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc,const bool &flag,const UI32 &mtu,const UI32 &cmdCode,const pvlanTypeEnum &pvlanType)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVLANINTFSP),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc),
        m_flag    (flag),
        m_pvlanType    (pvlanType),
        m_mtu        (mtu),
        m_cmdCode    (cmdCode)
    {
    	m_remoteSpanEnabled = false;
    	m_replayInProgress = false;
    }

    NsmUpdateVlanIntfSPMessage::~NsmUpdateVlanIntfSPMessage ()
    {
    }

    void  NsmUpdateVlanIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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
        addSerializableAttribute (new AttributeBool(&m_replayInProgress,"replayInProgress"));
    }

    void  NsmUpdateVlanIntfSPMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmUpdateVlanIntfSPMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmUpdateVlanIntfSPMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmUpdateVlanIntfSPMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmUpdateVlanIntfSPMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmUpdateVlanIntfSPMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmUpdateVlanIntfSPMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmUpdateVlanIntfSPMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmUpdateVlanIntfSPMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmUpdateVlanIntfSPMessage::getFlag() const
    {
        return (m_flag);
    }
    void  NsmUpdateVlanIntfSPMessage::setPvlanType(const pvlanTypeEnum &pvlanType)
    {
        m_pvlanType  =  pvlanType;
    }

    pvlanTypeEnum NsmUpdateVlanIntfSPMessage::getPvlanType() const
    {
        return (m_pvlanType);
    }

    void NsmUpdateVlanIntfSPMessage::setPvlanSecAssocRange(const UI32Range &pVlanSecAssocRange)
    	{
    	m_pVlanSecAssocRange = pVlanSecAssocRange;
    	}

    UI32Range NsmUpdateVlanIntfSPMessage::getPvlanSecAssocRange() const
    	{
    		return (m_pVlanSecAssocRange);
    	}

    void  NsmUpdateVlanIntfSPMessage::setMtu(const UI32 &mtu)
    {   
        m_mtu  =  mtu;
    }

    UI32  NsmUpdateVlanIntfSPMessage::getMtu() const
    {   
        return (m_mtu);
    }

    void  NsmUpdateVlanIntfSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdateVlanIntfSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdateVlanIntfSPMessage::setRemoteSpanEnabled(const bool &flag)
    {
    	m_remoteSpanEnabled  =  flag;
    }

    bool  NsmUpdateVlanIntfSPMessage::getRemoteSpanEnabled() const
    {
        return (m_remoteSpanEnabled);
    }

    bool NsmUpdateVlanIntfSPMessage::isReplayInProgress() const
    {
        return m_replayInProgress;
    }

    void NsmUpdateVlanIntfSPMessage::setReplayInProgress(const bool &replayInProgress)
    {
        m_replayInProgress = replayInProgress;
    }

}
