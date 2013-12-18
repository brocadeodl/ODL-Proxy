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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalDeleteVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeleteVlanClassifierGroupMessage::NsmGlobalDeleteVlanClassifierGroupMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERGROUP)
    {
		m_groupId = 0;
    }

    NsmGlobalDeleteVlanClassifierGroupMessage::NsmGlobalDeleteVlanClassifierGroupMessage (const UI32 &groupId)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEVLANCLASSIFIERGROUP),
        m_groupId    (groupId)
    {
    }

    NsmGlobalDeleteVlanClassifierGroupMessage::~NsmGlobalDeleteVlanClassifierGroupMessage ()
    {
    }

    void  NsmGlobalDeleteVlanClassifierGroupMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_groupId,"groupId"));
    }

    void  NsmGlobalDeleteVlanClassifierGroupMessage::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  NsmGlobalDeleteVlanClassifierGroupMessage::getGroupId() const
    {
        return (m_groupId);
    }

}
