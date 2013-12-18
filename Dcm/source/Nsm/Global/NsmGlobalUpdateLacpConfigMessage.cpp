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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalUpdateLacpConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdateLacpConfigMessage::NsmGlobalUpdateLacpConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),
													NSMGLOBALUPDATELACPCONFIG)
    {
		m_cmdCode = 0;
		m_dot1xenable = false;
		m_nativevlanDisable = true;
		m_systemPriority = 32768;
		m_dot1xtesttimeout = 10;

    }

    NsmGlobalUpdateLacpConfigMessage::NsmGlobalUpdateLacpConfigMessage (
								const UI32 &systemPriority,bool dot1xenable, bool nativevlanDisable, const UI8 &cmdCode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),
			NSMGLOBALUPDATELACPCONFIG),  m_systemPriority    (systemPriority),
		    m_dot1xenable (dot1xenable),
        	m_nativevlanDisable    (nativevlanDisable),
        	m_dot1xtesttimeout (10),
        	m_cmdCode    (cmdCode)
    {
    }

    NsmGlobalUpdateLacpConfigMessage::NsmGlobalUpdateLacpConfigMessage (
								const UI32 &systemPriority,bool dot1xenable, bool nativevlanDisable, const UI32 dot1xtesttimeout, const UI8 &cmdCode)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),
			NSMGLOBALUPDATELACPCONFIG),  m_systemPriority    (systemPriority),
		    m_dot1xenable (dot1xenable),
        	m_nativevlanDisable    (nativevlanDisable),
			m_dot1xtesttimeout (dot1xtesttimeout),
        	m_cmdCode    (cmdCode)
    {
    }

    NsmGlobalUpdateLacpConfigMessage::~NsmGlobalUpdateLacpConfigMessage ()
    {
    }

    void  NsmGlobalUpdateLacpConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_systemPriority,
														"systemPriority"));
        addSerializableAttribute (new AttributeBool(&m_dot1xenable,
														"dot1xenable"));
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
														"nativevlanDisable"));
        addSerializableAttribute (new AttributeUI32(&m_dot1xtesttimeout,
														"dot1xtesttimeout"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
    }

    void  NsmGlobalUpdateLacpConfigMessage::setSystemPriority(const UI32 
															&systemPriority)
    {
        m_systemPriority  =  systemPriority;
    }

    UI32  NsmGlobalUpdateLacpConfigMessage::getSystemPriority() const
    {
        return (m_systemPriority);
    }

    void  NsmGlobalUpdateLacpConfigMessage::setDot1xenable(const bool 
		&dot1xenable)
    {
        m_dot1xenable  =  dot1xenable;
    }

    bool  NsmGlobalUpdateLacpConfigMessage::getDot1xenable() const
    {
        return (m_dot1xenable);
    }
    
	void  NsmGlobalUpdateLacpConfigMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmGlobalUpdateLacpConfigMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }

    void  NsmGlobalUpdateLacpConfigMessage::setDot1xtesttimeout(const UI32 
		&dot1xtesttimeout)
    {
        m_dot1xtesttimeout  =  dot1xtesttimeout;
    }

    UI32  NsmGlobalUpdateLacpConfigMessage::getDot1xtesttimeout() const
    {
        return (m_dot1xtesttimeout);
    }
    

    void  NsmGlobalUpdateLacpConfigMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  NsmGlobalUpdateLacpConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
