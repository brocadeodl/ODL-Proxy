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
 *   Author : Mobin Mohan                                              	   *
 **************************************************************************/

#include "Lldp/Global/LldpGlobalProfileGetObjectIdMessage.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"

namespace DcmNs
{

    LldpGlobalProfileGetObjectIdMessage::LldpGlobalProfileGetObjectIdMessage ()
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALPROFILEGETOBJECTID)
    {
    }

    LldpGlobalProfileGetObjectIdMessage::LldpGlobalProfileGetObjectIdMessage (const string &profileName)
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALPROFILEGETOBJECTID),
        m_profileName    (profileName)
    {
    }

    LldpGlobalProfileGetObjectIdMessage::~LldpGlobalProfileGetObjectIdMessage ()
    {
    }

    void  LldpGlobalProfileGetObjectIdMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeObjectId(&m_profileObjId,"profileObjId"));
    }

    void  LldpGlobalProfileGetObjectIdMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  LldpGlobalProfileGetObjectIdMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  LldpGlobalProfileGetObjectIdMessage::setProfileObjId(const ObjectId &profileId)
    {
        m_profileObjId  =  profileId;
    }

    ObjectId  LldpGlobalProfileGetObjectIdMessage::getProfileObjId() const
    {
        return (m_profileObjId);
    }

}
