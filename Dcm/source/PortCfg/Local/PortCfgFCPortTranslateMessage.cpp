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
 *   Author : vbagavat & mlachire                                                   *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFCPortTranslateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFCPortTranslateMessage::PortCfgFCPortTranslateMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTTRANSLATE)
    {
    }

    PortCfgFCPortTranslateMessage::PortCfgFCPortTranslateMessage (const string &id,const SI32 &switchPort,const UI32 &rbridgeId)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTTRANSLATE),
        m_id    (id),
        m_switchPort    (switchPort),
        m_rbridgeId    (rbridgeId)
    {
    }

    PortCfgFCPortTranslateMessage::~PortCfgFCPortTranslateMessage ()
    {
    }

    void  PortCfgFCPortTranslateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeSI32(&m_switchPort,"switchPort"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
    }

    void  PortCfgFCPortTranslateMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgFCPortTranslateMessage::getId() const
    {
        return (m_id);
    }

    void  PortCfgFCPortTranslateMessage::setSwitchPort(const SI32 &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    SI32  PortCfgFCPortTranslateMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }

    void  PortCfgFCPortTranslateMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  PortCfgFCPortTranslateMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

}
