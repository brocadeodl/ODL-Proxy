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

#include "Nsm/Global/NsmGlobalDeleteVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeleteVlanIntfMessage::NsmGlobalDeleteVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANINTF)
    {
		m_vlanIfIndex = 0;
		m_isChangeVlanList = false;
    }

    NsmGlobalDeleteVlanIntfMessage::NsmGlobalDeleteVlanIntfMessage (const UI32 &vlanId,const UI32 &vlanIfIndex)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANINTF),
        m_vlanId    (vlanId),
		m_isChangeVlanList(false),
        m_vlanIfIndex    (vlanIfIndex)
    {
    }

    NsmGlobalDeleteVlanIntfMessage::~NsmGlobalDeleteVlanIntfMessage ()
    {
    }

    void  NsmGlobalDeleteVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeBool(&m_isChangeVlanList,"isChangeVlanList"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
    }

    void  NsmGlobalDeleteVlanIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalDeleteVlanIntfMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmGlobalDeleteVlanIntfMessage::setIsChangeAPPMVlanList(const bool &isChangeVlanList)
    {
        m_isChangeVlanList  =  isChangeVlanList;
    }

    bool  NsmGlobalDeleteVlanIntfMessage::getIsChangeAPPMVlanList() const
    {
        return (m_isChangeVlanList);
    }

    void  NsmGlobalDeleteVlanIntfMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmGlobalDeleteVlanIntfMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

}
