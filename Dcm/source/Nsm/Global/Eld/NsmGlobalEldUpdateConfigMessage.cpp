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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "Nsm/Global/Eld/NsmGlobalEldUpdateConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalEldUpdateConfigMessage::NsmGlobalEldUpdateConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALELDUPDATECONFIG)
    {
		m_Enable = false;
    }

    NsmGlobalEldUpdateConfigMessage::NsmGlobalEldUpdateConfigMessage (const bool &Enable)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALELDUPDATECONFIG),
        m_Enable    (Enable)
    {
    }

    NsmGlobalEldUpdateConfigMessage::~NsmGlobalEldUpdateConfigMessage ()
    {
    }

    void  NsmGlobalEldUpdateConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_Enable,"Enable"));
        addSerializableAttribute (new AttributeUI32(&m_rxlim,"rxlim"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmGlobalEldUpdateConfigMessage::setEnable(const bool &Enable)
    {
        m_Enable  =  Enable;
    }

	bool  NsmGlobalEldUpdateConfigMessage::getEnable() const
	{
		return (m_Enable);
	}

	void  NsmGlobalEldUpdateConfigMessage::setRxlim(const UI32 &rxlim)
	{
		m_rxlim  =  rxlim;
	}

	UI32  NsmGlobalEldUpdateConfigMessage::getRxlim() const
	{
		return (m_rxlim);
	}

	void  NsmGlobalEldUpdateConfigMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32  NsmGlobalEldUpdateConfigMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

}
