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
#include "Mstp/Local/MstpCreateBridgeConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"

namespace DcmNs
{

    MstpCreateBridgeConfigMessage::MstpCreateBridgeConfigMessage ()
        : ManagementInterfaceMessage (MstpLocalObjectManager::getClassName (),MSTPCREATEBRIDGECONFIG)
    {
      m_mode=0;
    }

    MstpCreateBridgeConfigMessage::MstpCreateBridgeConfigMessage (const UI32 &mode)
        : ManagementInterfaceMessage (MstpLocalObjectManager::getClassName (),MSTPCREATEBRIDGECONFIG),
        m_mode    (mode)
    {
    }

    MstpCreateBridgeConfigMessage::~MstpCreateBridgeConfigMessage ()
    {
    }

    void  MstpCreateBridgeConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
    }

    void  MstpCreateBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpCreateBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

}
