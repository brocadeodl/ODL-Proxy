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
 *   Author : kghanta                                                     *
 **************************************************************************/

#include "AAA/Global/AAAGlobalDeleteRuleMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

    AAAGlobalDeleteRuleMessage::AAAGlobalDeleteRuleMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETERULE)
    {
    }

    AAAGlobalDeleteRuleMessage::AAAGlobalDeleteRuleMessage (const UI32 &index)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETERULE),
        m_index    (index)
    {
    }

    AAAGlobalDeleteRuleMessage::~AAAGlobalDeleteRuleMessage ()
    {
    }

    void  AAAGlobalDeleteRuleMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_index,"index"));
    }

    void  AAAGlobalDeleteRuleMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  AAAGlobalDeleteRuleMessage::getIndex() const
    {
        return (m_index);
    }

}
