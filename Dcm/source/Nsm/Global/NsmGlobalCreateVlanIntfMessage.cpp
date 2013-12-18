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

#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"

namespace DcmNs
{

    NsmGlobalCreateVlanIntfMessage::NsmGlobalCreateVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEVLANINTF)
    {
		m_type = NSM_VLAN_INTF_TYPE_DEFAULT;
		m_vlanIfIndex = 0;
        m_bulkVlanConfig = false;
        m_RangeVlanStr = "";
    }

    NsmGlobalCreateVlanIntfMessage::NsmGlobalCreateVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEVLANINTF),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc)
    {
	m_type = NSM_VLAN_INTF_TYPE_DEFAULT;
        m_bulkVlanConfig = false;
        m_RangeVlanStr = "";
    }


    NsmGlobalCreateVlanIntfMessage::NsmGlobalCreateVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc, UI32 type)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEVLANINTF),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc),
        m_type    (type)
    {
        m_bulkVlanConfig = false;
        m_RangeVlanStr = "";
    }

    NsmGlobalCreateVlanIntfMessage::~NsmGlobalCreateVlanIntfMessage ()
    {
    }

    void  NsmGlobalCreateVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_desc,"descr"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
        addSerializableAttribute (new AttributeUI32(&m_type,"type"));
        addSerializableAttribute (new AttributeBool(&m_bulkVlanConfig,"vlanBulkConfig"));
        addSerializableAttribute (new AttributeString(&m_RangeVlanStr,"vlanRangeStr"));
        addSerializableAttribute (new AttributeUI32Vector(&failedVlanIds,"failedVlanIds"));
        addSerializableAttribute (new AttributeStringVector(&successfulRbridgesRange,"successfulRbridgesRange"));
        addSerializableAttribute (new AttributeStringVectorVector(&failStatusRbridgeMapping,"failStatusRbridgeMapping"));
    }

    void  NsmGlobalCreateVlanIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalCreateVlanIntfMessage::getVlanId() const
    {
        return m_vlanId;
    }

    void  NsmGlobalCreateVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmGlobalCreateVlanIntfMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmGlobalCreateVlanIntfMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmGlobalCreateVlanIntfMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmGlobalCreateVlanIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalCreateVlanIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalCreateVlanIntfMessage::setType(const UI32 &type)
    {
        m_type  =  type;
    }

    UI32  NsmGlobalCreateVlanIntfMessage::getType() const
    {
        return (m_type);
    }

    string  NsmGlobalCreateVlanIntfMessage::getRangeVlanStr() const
    {
        return m_RangeVlanStr;
    }

    void NsmGlobalCreateVlanIntfMessage::setRangeVlanStr(const string &RangeVlanStr)
    {
        m_RangeVlanStr = RangeVlanStr;
    }

    bool NsmGlobalCreateVlanIntfMessage::isBulkVlanConfig() const
    {
        return m_bulkVlanConfig;
    }

    void NsmGlobalCreateVlanIntfMessage::setIsBulkVlanConfig(const bool &isBulk)
    {
        m_bulkVlanConfig = isBulk;
    }

    void NsmGlobalCreateVlanIntfMessage::updateFailureStatus(const UI32 &vlanId, vector<string> &statusRbridge, const string &successRbridgesRangeStr) {
        failedVlanIds.push_back(vlanId);
        failStatusRbridgeMapping.push_back(statusRbridge);
        successfulRbridgesRange.push_back(successRbridgesRangeStr);
    }

    map<UI32, map<SI32, string> > NsmGlobalCreateVlanIntfMessage::getFailureStatusRbridgeMappings() {
        map<UI32, map<SI32, string> > resultMap;
        for (UI32 i = 0; i < failedVlanIds.size(); i++) {
            UI32 failedVlanId = failedVlanIds[i];
            vector<string> statusRbridge = failStatusRbridgeMapping[i];

            for (UI32 z = 0; z < statusRbridge.size(); z++) {
                string statusRbridgeRangeStr = statusRbridge[z];
                vector<string> token;
                tokenize (statusRbridgeRangeStr, token, '#');
                SI32 errorCode = (SI32) atoi((token[0]).c_str());

                (resultMap[failedVlanId])[errorCode] = token[1];
            }
        }

        return resultMap;
    }
    map<UI32, string> NsmGlobalCreateVlanIntfMessage::getSuccessfulRbridgesRange() {
        map<UI32, string> resultMap;
        for (UI32 i = 0; i < failedVlanIds.size(); i++) {
            UI32 failedVlanId = failedVlanIds[i];
            resultMap[failedVlanId] = successfulRbridgesRange[i];
        }
        return resultMap;
    }
}
