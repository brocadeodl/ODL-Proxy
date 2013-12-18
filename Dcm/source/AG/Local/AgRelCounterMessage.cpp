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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : ranantha                                                    *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgRelCounterMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgRelCounterMessage::AgRelCounterMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_RELCOUNTER_MESSAGE)
    {
    }

    AgRelCounterMessage::AgRelCounterMessage (const UI32 &RelCounter, const UI32 &cmdCode)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_RELCOUNTER_MESSAGE),
        m_RelCounterValue(RelCounter),
        m_cmdCode(cmdCode)
    {
    }

    AgRelCounterMessage::~AgRelCounterMessage ()
    {
    }

    void  AgRelCounterMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_RelCounterValue, "reliabilityvalue"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "commmandcode"));
    }

    void  AgRelCounterMessage::setRelCounterValue(const UI32 &RelCounter)
    {
        m_RelCounterValue =  RelCounter;
    }

    UI32  AgRelCounterMessage::getRelCounterValue() const
    {
        return (m_RelCounterValue);
    }

    void  AgRelCounterMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode =  cmdCode;
    }

    UI32  AgRelCounterMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }


}
