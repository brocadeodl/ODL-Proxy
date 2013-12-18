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
#include "Nsm/Local/NsmCreatePhyIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{

    NsmCreatePhyIntfMessage::NsmCreatePhyIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMCREATEPHYINTF)
    {
		m_mtu = NSM_MTU_DEF_VAL; 
		m_swIfIndex = 0;
        m_portrole = false;
        m_islCapability = 0;
    }

    NsmCreatePhyIntfMessage::NsmCreatePhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu,const string &desc)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMCREATEPHYINTF),
        m_ifName    (ifName),
        m_swIfIndex    (swIfIndex),
        m_mtu    (mtu),
        m_desc    (desc)
    {
        m_portrole = false;
        m_islCapability = 0;
    }

    NsmCreatePhyIntfMessage::~NsmCreatePhyIntfMessage ()
    {
    }

    void  NsmCreatePhyIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeBool((bool * )(&m_portrole),"Portrole"));
        addSerializableAttribute (new AttributeUI32(&m_islCapability,"islCapability"));
        
    }

    void  NsmCreatePhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmCreatePhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmCreatePhyIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmCreatePhyIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmCreatePhyIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmCreatePhyIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmCreatePhyIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmCreatePhyIntfMessage::getDesc() const
    {
        return (m_desc);
    }
    void  NsmCreatePhyIntfMessage::setPortrole(const bool &Portrole)
    {
        m_portrole = Portrole;
    }

    bool NsmCreatePhyIntfMessage::getPortrole() const
    {
        return (m_portrole);
    }

    void  NsmCreatePhyIntfMessage::setIslCapability(const UI32 &islCapability)
    {
        m_islCapability = islCapability;
    }

    UI32 NsmCreatePhyIntfMessage::getIslCapability() const
    {
        return (m_islCapability);
    }
}
