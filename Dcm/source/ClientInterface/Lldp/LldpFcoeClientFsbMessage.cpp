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

#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Lldp/LldpFcoeClientFsbMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{   

    LldpFcoeClientFsbMessage::LldpFcoeClientFsbMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_FSB_MODE)
    {
    }

    LldpFcoeClientFsbMessage::LldpFcoeClientFsbMessage (const bool &fsb_enable)
        : DcmManagementInterfaceMessage (MSG_LLDP_FSB_MODE),
        m_fsb_enable    (fsb_enable)
    {
    }

    LldpFcoeClientFsbMessage::~LldpFcoeClientFsbMessage ()
    {
    }

    void  LldpFcoeClientFsbMessage::setupAttributesForSerialization()
    {   
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_fsb_enable,"fsb_enable"));
    }

    bool  LldpFcoeClientFsbMessage::getFsbMode() const
    {   
        return (m_fsb_enable);
    }

    const void * LldpFcoeClientFsbMessage::getCStructureForInputs()
    {   
        lldp_fsb_mode_t *msg_p = new lldp_fsb_mode_t;

        msg_p->fsb_enable = m_fsb_enable;

        return msg_p;
    }
}

