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

#include "ClientInterface/Nsm/NsmClientUpdateLacpConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmClientUpdateLacpConfigMessage::NsmClientUpdateLacpConfigMessage ()
        : DcmManagementInterfaceMessage (NSMGLOBALLACPCONFIG)
    {
    }

    NsmClientUpdateLacpConfigMessage::NsmClientUpdateLacpConfigMessage (
                                const UI32 &systemPriority,bool dot1xenable, const UI8 &cmdCode)
        : DcmManagementInterfaceMessage (NSMGLOBALLACPCONFIG),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_cmdCode    (cmdCode)
    {
    }

    NsmClientUpdateLacpConfigMessage::NsmClientUpdateLacpConfigMessage (
                                const UI32 &systemPriority,bool dot1xenable, const UI32 dot1xtesttimeout, const UI8 &cmdCode)
        : DcmManagementInterfaceMessage (NSMGLOBALLACPCONFIG),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_dot1xtesttimeout    (dot1xtesttimeout),
        m_cmdCode    (cmdCode)
    {
    }

    NsmClientUpdateLacpConfigMessage::~NsmClientUpdateLacpConfigMessage ()
    {
    }

    void  NsmClientUpdateLacpConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_systemPriority,
                                                            "systemPriority"));
        addSerializableAttribute (new AttributeBool(&m_dot1xenable,
                                                            "dot1xenable"));
        addSerializableAttribute (new AttributeUI32(&m_dot1xtesttimeout,
                                                            "dot1xtesttimeout"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
    }

    void  NsmClientUpdateLacpConfigMessage::setSystemPriority(const UI32 
                                                            &systemPriority)
    {
        m_systemPriority  =  systemPriority;
    }

    UI32  NsmClientUpdateLacpConfigMessage::getSystemPriority() const
    {
        return (m_systemPriority);
    }

    void  NsmClientUpdateLacpConfigMessage::setDot1xenable(const bool 
                                                            &dot1xenable)
    {
        m_dot1xenable  =  dot1xenable;
    }

    bool  NsmClientUpdateLacpConfigMessage::getDot1xenable() const
    {
        return (m_dot1xenable);
    }

    void  NsmClientUpdateLacpConfigMessage::setDot1xtesttimeout(const UI32 
                                                            &dot1xtesttimeout)
    {
        m_dot1xtesttimeout  =  dot1xtesttimeout;
    }

    UI32  NsmClientUpdateLacpConfigMessage::getDot1xtesttimeout() const
    {
        return (m_dot1xtesttimeout);
    }


    void  NsmClientUpdateLacpConfigMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  NsmClientUpdateLacpConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	const void *NsmClientUpdateLacpConfigMessage::getCStructureForInputs ()
    {
        LacpGlbUpdConfMsg_t *pInput = new LacpGlbUpdConfMsg_t;

        pInput->SystemPriority = m_systemPriority;
        pInput->dot1xenable = m_dot1xenable;
        pInput->dot1xtesttimeout = m_dot1xtesttimeout;
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void NsmClientUpdateLacpConfigMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}

}
