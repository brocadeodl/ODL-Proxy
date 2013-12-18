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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#include "ClientInterface/Fcoe/FcoeClientFsbMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{   

    FcoeClientFsbMessage::FcoeClientFsbMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FSB_CONFIG)
    {
    }

    FcoeClientFsbMessage::FcoeClientFsbMessage (const bool &enable)  
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FSB_CONFIG),
        m_enable    (enable)
    {
    }

    FcoeClientFsbMessage::~FcoeClientFsbMessage ()
    {
    }

    void  FcoeClientFsbMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
    }

    void  FcoeClientFsbMessage::setMode(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  FcoeClientFsbMessage::getMode() const
    {
        return (m_enable);
    }

    const void * FcoeClientFsbMessage::getCStructureForInputs()
    {
        fcoe_fsb_msg_t *msg_p = new fcoe_fsb_msg_t;

        msg_p->enable = m_enable?1:0;

        return msg_p;
    }
}
