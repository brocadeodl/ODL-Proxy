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

#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpDeleteVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"

namespace DcmNs
{

    MstpDeleteVlanBridgeConfigMessage::MstpDeleteVlanBridgeConfigMessage ()
        : ManagementInterfaceMessage (MstpLocalObjectManager::getClassName (),MSTPDELETEVLANBRIDGECONFIG)
    {
      m_id=0;
    }

    MstpDeleteVlanBridgeConfigMessage::MstpDeleteVlanBridgeConfigMessage (const UI32 &id)
        : ManagementInterfaceMessage (MstpLocalObjectManager::getClassName (),MSTPDELETEVLANBRIDGECONFIG),
        m_id    (id)
    {
    }

    MstpDeleteVlanBridgeConfigMessage::~MstpDeleteVlanBridgeConfigMessage ()
    {
    }

    void  MstpDeleteVlanBridgeConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
    }

    void  MstpDeleteVlanBridgeConfigMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  MstpDeleteVlanBridgeConfigMessage::getId() const
    {
        return (m_id);
    }

}
