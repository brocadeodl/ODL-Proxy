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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmDot1xUpdateConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmDot1xUpdateConfigSPMessage::NsmDot1xUpdateConfigSPMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDOT1XUPDATECONFIGSP)
    {
		m_Enable = false;
    }

    NsmDot1xUpdateConfigSPMessage::NsmDot1xUpdateConfigSPMessage (const bool &Enable)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDOT1XUPDATECONFIGSP),
        m_Enable    (Enable)
    {
    }

    NsmDot1xUpdateConfigSPMessage::~NsmDot1xUpdateConfigSPMessage ()
    {
    }

    void  NsmDot1xUpdateConfigSPMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_Enable,"Enable"));
    }

    void  NsmDot1xUpdateConfigSPMessage::setEnable(const bool &Enable)
    {
        m_Enable  =  Enable;
    }

    bool  NsmDot1xUpdateConfigSPMessage::getEnable() const
    {
        return (m_Enable);
    }

}