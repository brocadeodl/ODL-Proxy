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
 *   Author : mmahajan                                                     *
 **************************************************************************/

#include "TrillOam/Local/TrillOamLocalObjectManager.h"
#include "TrillOam/Local/TrillOamTrillOamPingRequestMessage.h"
#include "TrillOam/Local/TrillOamTypes.h"

namespace DcmNs
{

    TrillOamTrillOamPingRequestMessage::TrillOamTrillOamPingRequestMessage ()
        : ManagementInterfaceMessage (TrillOamLocalObjectManager::getClassName (),TRILLOAMTRILLOAMPINGREQUEST)
    {
    }

    TrillOamTrillOamPingRequestMessage::TrillOamTrillOamPingRequestMessage (const MacAddress &srcmac,const MacAddress &destmac,const UI32 &vlanid)
        : ManagementInterfaceMessage (TrillOamLocalObjectManager::getClassName (),TRILLOAMTRILLOAMPINGREQUEST),
        m_srcmac    (srcmac),
        m_destmac    (destmac),
        m_vlanid    (vlanid)
    {
    }

    TrillOamTrillOamPingRequestMessage::~TrillOamTrillOamPingRequestMessage ()
    {
    }

    void  TrillOamTrillOamPingRequestMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeMacAddress(&m_srcmac,"srcmac"));
        addSerializableAttribute (new AttributeMacAddress(&m_destmac,"destmac"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
    }

    void  TrillOamTrillOamPingRequestMessage::setSrcmac(const MacAddress &srcmac)
    {
        m_srcmac  =  srcmac;
    }

    MacAddress  TrillOamTrillOamPingRequestMessage::getSrcmac() const
    {
        return (m_srcmac);
    }

    void  TrillOamTrillOamPingRequestMessage::setDestmac(const MacAddress &destmac)
    {
        m_destmac  =  destmac;
    }

    MacAddress  TrillOamTrillOamPingRequestMessage::getDestmac() const
    {
        return (m_destmac);
    }

    void  TrillOamTrillOamPingRequestMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  TrillOamTrillOamPingRequestMessage::getVlanid() const
    {
        return (m_vlanid);
    }

}
