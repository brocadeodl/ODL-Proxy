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
#include "Nsm/Local/NsmAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmAddPhyIntfToPoIntfMessage::NsmAddPhyIntfToPoIntfMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMADDPHYINTFTOPOINTF)
    {

		m_cmdCode = 0;
		m_poIndex = 0;
		m_poLacpSystemIdDomainId = 0;
		m_poMode = 0;
		m_poType = 0;
		m_replayPoConfigs = false;
		m_swIfIndex = 0;
		m_ifType = IF_TYPE_INVALID;
		m_vlanidList.clear();
    }

    NsmAddPhyIntfToPoIntfMessage::NsmAddPhyIntfToPoIntfMessage (const string &ifName,const UI64 &swIfIndex,const string &poName,const UI32 &poIndex,const UI32 &mode,const UI32 &type,const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId,const bool &replayPoConfigs,const UI32 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMADDPHYINTFTOPOINTF),
        m_ifName    (ifName),
        m_swIfIndex    (swIfIndex),
        m_poName    (poName),
        m_poIndex    (poIndex),
        m_poMode    (mode),
        m_poType    (type),
		m_cmdCode    (cmdCode),
        m_poLacpSystemId    (poLacpSystemId),
        m_poLacpSystemIdDomainId    (poLacpSystemIdDomainId),
		m_replayPoConfigs (replayPoConfigs)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    NsmAddPhyIntfToPoIntfMessage::~NsmAddPhyIntfToPoIntfMessage ()
    {
    }

    void  NsmAddPhyIntfToPoIntfMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addSerializableAttribute (new AttributeUI32(&m_poIndex,"poIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addSerializableAttribute (new AttributeBool((bool * )(&m_replayPoConfigs),"replayPoConfigs"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_prio,"prio"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
	}
	void  NsmAddPhyIntfToPoIntfMessage::setPrio(const UI32 &prio)
	{
		m_prio  =  prio;
	}

	UI32  NsmAddPhyIntfToPoIntfMessage::getPrio() const
	{
		return (m_prio); 
	}

	void  NsmAddPhyIntfToPoIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
	{
		m_vlanidList  =  vlanidList;
	}

	vector<UI32>  NsmAddPhyIntfToPoIntfMessage::getVlanidList() const
	{
		return (m_vlanidList);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmAddPhyIntfToPoIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmAddPhyIntfToPoIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmAddPhyIntfToPoIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoIndex(const UI32 &poIndex)
    {
        m_poIndex  =  poIndex;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getPoIndex() const
    {
        return (m_poIndex);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoMode(const UI32 &mode)
    {
        m_poMode  =  mode;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoType(const UI32 &type)
    {
        m_poType  =  type;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmAddPhyIntfToPoIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setReplayPoConfigs(const bool &replayPoConfigs)
    {
        m_replayPoConfigs  =  replayPoConfigs;
    }

    bool  NsmAddPhyIntfToPoIntfMessage::getReplayPoConfigs() const
    {
        return (m_replayPoConfigs);
    }

    void  NsmAddPhyIntfToPoIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmAddPhyIntfToPoIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }


}
