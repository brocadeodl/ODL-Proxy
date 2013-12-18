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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdateLacpConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmUpdateLacpConfigSPMessage::NsmUpdateLacpConfigSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                                                        NSMUPDATELACPCONFIGSP)
    {
		m_cmdCode = 0;
        m_dot1xenable = false;
        m_nativevlanDisable = true;
        m_systemPriority = 32768;
		m_dot1xtesttimeout = 10;
    }

    NsmUpdateLacpConfigSPMessage::NsmUpdateLacpConfigSPMessage (
                              const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable, const UI8 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                                                      NSMUPDATELACPCONFIGSP),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_nativevlanDisable    (nativevlanDisable),
        m_cmdCode    (cmdCode)
    {
    }

    NsmUpdateLacpConfigSPMessage::NsmUpdateLacpConfigSPMessage (
                              const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable, const UI32 dot1xtesttimeout, const UI8 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                                                      NSMUPDATELACPCONFIGSP),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_nativevlanDisable    (nativevlanDisable),
        m_dot1xtesttimeout    (dot1xtesttimeout),
        m_cmdCode    (cmdCode)
    {
    }

    NsmUpdateLacpConfigSPMessage::~NsmUpdateLacpConfigSPMessage ()
    {
    }

    void  NsmUpdateLacpConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_systemPriority,
                                                            "systemPriority"));
        addSerializableAttribute (new AttributeBool(&m_dot1xenable,
                                                            "dot1xenable"));
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
														"nativevlanDisable"));
        addSerializableAttribute (new AttributeUI32(&m_dot1xtesttimeout,
                                                            "dot1xtesttimeout"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode, "cmdCode"));
    }

    void  NsmUpdateLacpConfigSPMessage::setSystemPriority(const UI32 
                                                               &systemPriority)
    {
        m_systemPriority  =  systemPriority;
    }

    UI32  NsmUpdateLacpConfigSPMessage::getSystemPriority() const
    {
        return (m_systemPriority);
    }

    void  NsmUpdateLacpConfigSPMessage::setDot1xenable(const bool 
                                                               &dot1xenable)
    {
        m_dot1xenable  =  dot1xenable;
    }

    bool  NsmUpdateLacpConfigSPMessage::getDot1xenable() const
    {
        return (m_dot1xenable);
    }
	
	void  NsmUpdateLacpConfigSPMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmUpdateLacpConfigSPMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }

    void  NsmUpdateLacpConfigSPMessage::setDot1xtesttimeout(const UI32 
                                                               &dot1xtesttimeout)
    {
        m_dot1xtesttimeout  =  dot1xtesttimeout;
    }

    UI32  NsmUpdateLacpConfigSPMessage::getDot1xtesttimeout() const
    {
        return (m_dot1xtesttimeout);
    }
	

    void  NsmUpdateLacpConfigSPMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  NsmUpdateLacpConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
