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
#include "Nsm/Local/NsmCreateVlanIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmCreateVlanIntfSPMessage::NsmCreateVlanIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMCREATEVLANINTFSP)
    {
		m_vlanIfIndex = 0;
        m_bulkVlanConfig = false;
        m_RangeVlanStr = "";
    }

    NsmCreateVlanIntfSPMessage::NsmCreateVlanIntfSPMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMCREATEVLANINTFSP),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc)
    {
        m_bulkVlanConfig = false;
        m_RangeVlanStr = "";
    }

    NsmCreateVlanIntfSPMessage::~NsmCreateVlanIntfSPMessage ()
    {
    }

    void  NsmCreateVlanIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
        addSerializableAttribute (new AttributeBool(&m_bulkVlanConfig,"bulkVlanConfig"));
        addSerializableAttribute (new AttributeString(&m_RangeVlanStr,"RangeVlanStr"));
        addSerializableAttribute (new AttributeUI32Vector(&m_failedVlanIds,"failedVlanIds"));
        addSerializableAttribute (new AttributeSI32Vector(&m_failureStatus,"failureStatus"));
    }

    void  NsmCreateVlanIntfSPMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmCreateVlanIntfSPMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmCreateVlanIntfSPMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmCreateVlanIntfSPMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmCreateVlanIntfSPMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmCreateVlanIntfSPMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmCreateVlanIntfSPMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmCreateVlanIntfSPMessage::getDesc() const
    {
        return (m_desc);
    }

    bool NsmCreateVlanIntfSPMessage::isBulkVlanConfig() const
    {
        return m_bulkVlanConfig;
    }

    void NsmCreateVlanIntfSPMessage::setIsBulkVlanConfig(const bool &isBulk)
    {
        m_bulkVlanConfig = isBulk;
    }

    void  NsmCreateVlanIntfSPMessage::setRangeVlanStr(const string &RangeVlanStr)
    {
        m_RangeVlanStr =  RangeVlanStr;
    }

    string  NsmCreateVlanIntfSPMessage::getRangeVlanStr() const
    {
        return (m_RangeVlanStr);
    }

    void  NsmCreateVlanIntfSPMessage::updateFailureStatus(const UI32 &vlanId, const SI32 &status)
    {
        m_failedVlanIds.push_back(vlanId);
        m_failureStatus.push_back(status);
    }

    vector<UI32> NsmCreateVlanIntfSPMessage::getFailedVlanIds() {
        return m_failedVlanIds;
    }

    vector<SI32> NsmCreateVlanIntfSPMessage::getFailureStatus() {
        return m_failureStatus;
    }
}
