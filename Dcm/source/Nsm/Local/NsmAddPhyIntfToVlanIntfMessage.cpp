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
#include "Nsm/Local/NsmAddPhyIntfToVlanIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmAddPhyIntfToVlanIntfMessage::NsmAddPhyIntfToVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMADDPHYINTFTOVLANINTF)
    {
		m_swIfIndex = 0;
		m_tagStatus = 0;
    }

    NsmAddPhyIntfToVlanIntfMessage::NsmAddPhyIntfToVlanIntfMessage (const UI64 &swIfIndex,const string &vlanName,const UI32 &tagStatus)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMADDPHYINTFTOVLANINTF),
        m_swIfIndex    (swIfIndex),
        m_vlanName    (vlanName),
        m_tagStatus    (tagStatus)
    {
    }

    NsmAddPhyIntfToVlanIntfMessage::~NsmAddPhyIntfToVlanIntfMessage ()
    {
    }

    void  NsmAddPhyIntfToVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_tagStatus,"tagStatus"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmAddPhyIntfToVlanIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmAddPhyIntfToVlanIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmAddPhyIntfToVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmAddPhyIntfToVlanIntfMessage::getVlanName() const
    {
        return ("m_vlanName");
    }

    void  NsmAddPhyIntfToVlanIntfMessage::setTagStatus(const UI32 &tagStatus)
    {
        m_tagStatus  =  tagStatus;
    }

    UI32  NsmAddPhyIntfToVlanIntfMessage::getTagStatus() const
    {
        return (m_tagStatus);
    }

}
