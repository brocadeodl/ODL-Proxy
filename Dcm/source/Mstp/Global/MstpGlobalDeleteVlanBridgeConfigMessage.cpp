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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Mstp/Global/MstpGlobalDeleteVlanBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"

namespace DcmNs
{

    MstpGlobalDeleteVlanBridgeConfigMessage::MstpGlobalDeleteVlanBridgeConfigMessage ()
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALDELETEVLANBRIDGECONFIG)
    { 
      m_id=0;
    }

    MstpGlobalDeleteVlanBridgeConfigMessage::MstpGlobalDeleteVlanBridgeConfigMessage (UI32 id, const ObjectId &oid)
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALDELETEVLANBRIDGECONFIG),
        m_id (id), m_oid (oid)
    {
    }

    MstpGlobalDeleteVlanBridgeConfigMessage::~MstpGlobalDeleteVlanBridgeConfigMessage ()
    {
    }

    void  MstpGlobalDeleteVlanBridgeConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeObjectId(&m_oid, "oid"));
    }

    UI32  MstpGlobalDeleteVlanBridgeConfigMessage::getId() const
    {
        return (m_id);
    }

    ObjectId MstpGlobalDeleteVlanBridgeConfigMessage::getObjectId () const
    {
        return m_oid;
    }

}
