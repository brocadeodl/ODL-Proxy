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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLldpInternalMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmLldpInternalMessage::NsmLldpInternalMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLLDPINTERNAL)
    {
		m_cmdcode = 0;
		m_mgmtFlags = 0;
		m_msgType = 0;
    }

    NsmLldpInternalMessage::NsmLldpInternalMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &profile)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLLDPINTERNAL),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_profile    (profile)
    {
    }

    NsmLldpInternalMessage::~NsmLldpInternalMessage ()
    {
    }

    void  NsmLldpInternalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_profile,"profile"));
    }

    void  NsmLldpInternalMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  NsmLldpInternalMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  NsmLldpInternalMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  NsmLldpInternalMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  NsmLldpInternalMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  NsmLldpInternalMessage::getMsgType() const
    {
        return (m_msgType);
    }


    void  NsmLldpInternalMessage::setProfile(const string &profile)
    {
        m_profile  =  profile;
    }

    string  NsmLldpInternalMessage::getProfile() const
    {
        return (m_profile);
    }

}
