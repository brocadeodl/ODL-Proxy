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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : pasu                                                         *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Layer3/NsmPhyIntfVrfConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmPhyIntfVrfConfigMessage::NsmPhyIntfVrfConfigMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMPHYINTFVRFCONF)
    {
		m_cmdCode = 0;
    }

    NsmPhyIntfVrfConfigMessage::NsmPhyIntfVrfConfigMessage (const string &ifName, const UI32 &ifType, const UI32 &cmdCode,const string &vrfName)
		: ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMPHYINTFVRFCONF),
		m_ifName    (ifName),
        m_vrfName   (vrfName),
		m_cmdCode   (cmdCode),
        m_ifType    (ifType)
		{
        }

    NsmPhyIntfVrfConfigMessage::~NsmPhyIntfVrfConfigMessage ()
    {
    }

    void  NsmPhyIntfVrfConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }
     void  NsmPhyIntfVrfConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

	UI32  NsmPhyIntfVrfConfigMessage::getIfType() const
	{
		return (m_ifType);
	}

    void  NsmPhyIntfVrfConfigMessage::setVrfName(const string &vrfName)
    {
        m_vrfName  =  vrfName;
    }

    string  NsmPhyIntfVrfConfigMessage::getVrfName() const
    {
        return (m_vrfName);
    }

    void  NsmPhyIntfVrfConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmPhyIntfVrfConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmPhyIntfVrfConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmPhyIntfVrfConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
}
