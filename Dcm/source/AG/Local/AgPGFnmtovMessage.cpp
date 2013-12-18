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
#include "AG/Local/AgPGFnmtovMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPGFnmtovMessage::AgPGFnmtovMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_FNMTOV_MESSAGE)
    {
    }

    AgPGFnmtovMessage::AgPGFnmtovMessage (const UI32 &Fnmtovalue)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_FNMTOV_MESSAGE),
        m_Fnmtovalue(Fnmtovalue)
    {
    }

    AgPGFnmtovMessage::~AgPGFnmtovMessage ()
    {
    }

    void  AgPGFnmtovMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_Fnmtovalue, "fnmtovalue"));
    }

    void  AgPGFnmtovMessage::setFnmtoValue(const UI32 &Fnmtovalue)
    {
        m_Fnmtovalue =  Fnmtovalue;
    }

    UI32  AgPGFnmtovMessage::getFnmtoValue() const
    {
        return (m_Fnmtovalue);
    }


}
