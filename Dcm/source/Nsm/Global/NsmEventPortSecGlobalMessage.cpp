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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmEventPortSecGlobalMessage.h"

namespace DcmNs
{

    NsmEventPortSecGlobalMessage::NsmEventPortSecGlobalMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName(), NSMPORTSECURITYPOSTICKYMACMSG)
    {
    }

    NsmEventPortSecGlobalMessage::~NsmEventPortSecGlobalMessage ()
    {
    }

    void  NsmEventPortSecGlobalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  NsmEventPortSecGlobalMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmEventPortSecGlobalMessage::getOpCode() const
    {
        return (m_opCode);
    }
}
