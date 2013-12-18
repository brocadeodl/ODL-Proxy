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
#include "Nsm/Local/Layer3/NsmVeIntfVrfConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmVeIntfVrfConfigMessage::NsmVeIntfVrfConfigMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMVEINTFVRFCONF)
    {
		m_cmdCode = 0;
    }

    NsmVeIntfVrfConfigMessage::NsmVeIntfVrfConfigMessage (const UI32 &ifId, const UI32 &cmdCode,const string &vrfName)
		: ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMVEINTFVRFCONF),
		m_ifId    (ifId),
        m_vrfName   (vrfName),
		m_cmdCode   (cmdCode)
		{
        }

    NsmVeIntfVrfConfigMessage::~NsmVeIntfVrfConfigMessage ()
    {
    }

    void  NsmVeIntfVrfConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }

    void  NsmVeIntfVrfConfigMessage::setVrfName(const string &vrfName)
    {
        m_vrfName  =  vrfName;
    }

    string  NsmVeIntfVrfConfigMessage::getVrfName() const
    {
        return (m_vrfName);
    }

    void  NsmVeIntfVrfConfigMessage::setIfId(const UI32 &ifId)
    {
        m_ifId  =  ifId;
    }

    UI32 NsmVeIntfVrfConfigMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  NsmVeIntfVrfConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmVeIntfVrfConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
}
