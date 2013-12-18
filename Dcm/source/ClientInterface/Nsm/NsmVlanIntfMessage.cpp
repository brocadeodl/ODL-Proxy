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

#include "ClientInterface/Nsm/NsmVlanIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeUI32Range.h"

namespace DcmNs
{

    NsmVlanIntfMessage::NsmVlanIntfMessage ()
        : NsmVlanProvisionMessage (NSMVLANINTF),
          m_remoteSpanEnabled(false),
          m_replayInProgress(false)
    {
        m_bulkCmdCode = 0;
    }

    NsmVlanIntfMessage::NsmVlanIntfMessage (const UI32 &bulkCmdCode)
        : NsmVlanProvisionMessage (NSMVLANINTF),
          m_remoteSpanEnabled(false),
          m_replayInProgress(false)
    {
        m_bulkCmdCode = 0;
        m_bulkCmdCode |= bulkCmdCode;
    }
    NsmVlanIntfMessage::NsmVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc,const bool &flag,const UI32 &cmdCode, const UI8 &pvlanType)
        : NsmVlanProvisionMessage (NSMVLANINTF),
        m_vlanId    (vlanId),
        m_vlanName    (vlanName),
        m_vlanIfIndex    (vlanIfIndex),
        m_desc    (desc),
        m_flag    (flag),
        m_cmdCode    (cmdCode),
        m_pvlanType    (pvlanType),
        m_remoteSpanEnabled(false),
        m_replayInProgress(false)
    {
        m_bulkCmdCode = 0;
    }

    NsmVlanIntfMessage::~NsmVlanIntfMessage ()
    {
    }

    void  NsmVlanIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_bulkCmdCode,"bulkCmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
        addSerializableAttribute (new AttributeUI8((&m_pvlanType),"pvlanType"));
        addSerializableAttribute (new AttributeUI32Range(&m_pVlanSecAssocRange,"pVlanSecAssocRange"));
        addSerializableAttribute (new AttributeBool(&m_remoteSpanEnabled,"remoteSpanEnabled"));
        addSerializableAttribute (new AttributeBool(&m_replayInProgress,"replayInProgress"));
    }

    void  NsmVlanIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmVlanIntfMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
        m_bulkCmdCode |= NSMUPDATE_VLAN_CREATE_CMD;
    }

    string  NsmVlanIntfMessage::getVlanName() const
    {
        return (m_vlanName);
    }

    void  NsmVlanIntfMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmVlanIntfMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmVlanIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
        m_bulkCmdCode |= NSMUPDATE_VLAN_DESC_CMD;
    }

    string  NsmVlanIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmVlanIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
        m_bulkCmdCode |= NSMUPDATE_VLAN_STATUS_CMD;
    }

    bool  NsmVlanIntfMessage::getFlag() const
    {
        return (m_flag);
    }

    void  NsmVlanIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmVlanIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmVlanIntfMessage::setBulkCmdCode(const UI32 &cmdCode)
    {
        m_bulkCmdCode  =  cmdCode;
    }

    UI32  NsmVlanIntfMessage::getBulkCmdCode() const
    {
        return (m_bulkCmdCode);
    }
	void  NsmVlanIntfMessage::setPvlanType(const UI8 &pvlanType)
    {
        m_pvlanType  =  pvlanType;
        m_bulkCmdCode |= NSMUPDATE_PVLAN_TYPE_CMD;
    }

    UI8  NsmVlanIntfMessage::getPvlanType() const
    {
        return (m_pvlanType);
    }

    void NsmVlanIntfMessage::setPvlanSecAssocRange(const UI32Range &pVlanSecAssocRange)
    	{
    	m_pVlanSecAssocRange = pVlanSecAssocRange;
        m_bulkCmdCode |= NSMUPDATE_PVLAN_SEC_ASSOC_RANGE_CMD;
    	}

    UI32Range NsmVlanIntfMessage::getPvlanSecAssocRange() const
    	{
    		return (m_pVlanSecAssocRange);
    	}

    void  NsmVlanIntfMessage::setRemoteSpanEnabled(const bool &flag)
    {
    	m_remoteSpanEnabled  =  flag;
    }

    bool  NsmVlanIntfMessage::getRemoteSpanEnabled() const
    {
        return (m_remoteSpanEnabled);
    }

    bool NsmVlanIntfMessage::isReplayInProgress() const
    {
        return m_replayInProgress;
    }

    void NsmVlanIntfMessage::setReplayInProgress(bool replayInProgress)
    {
        m_replayInProgress = replayInProgress;
    }

    const void *NsmVlanIntfMessage::getCStructureForInputs ()
    {
        NsmVlanIntfMsg_t *pInput = new NsmVlanIntfMsg_t;

        strncpy (pInput->vlanName, m_vlanName.c_str (), 64);
		pInput->vlanName[63] = '\0';
		pInput->vlanId = m_vlanId;
        strncpy (pInput->desc, m_desc.c_str (), 64);
		pInput->desc[63] = '\0';
        pInput->vlanIfIndex = m_vlanIfIndex;
        pInput->flag     = m_flag;
        pInput->cmdCode     = m_cmdCode;
        pInput->bulkCmdCode     = m_bulkCmdCode;
        pInput->pvlanType     = m_pvlanType;
        pInput->pVlanSecAssocRange = NULL;
        pInput->replayInProgress = 0;

        if (m_pVlanSecAssocRange.toString() != "")
        {
            char *vlanList = NULL;
            int vlSize = 0;

            vlanList = (char *)(m_pVlanSecAssocRange.toString()).c_str();
            vlSize = (m_pVlanSecAssocRange.toString()).size();

            if (vlanList) {
                pInput->pVlanSecAssocRange = (char *)malloc(vlSize + 1);
                strncpy (pInput->pVlanSecAssocRange, vlanList, vlSize);
                pInput->pVlanSecAssocRange[vlSize] = '\0';
            }
        }
        pInput->replayInProgress = m_replayInProgress;
        pInput->rspanEnabled = m_remoteSpanEnabled;
        return (pInput);
    }


}
