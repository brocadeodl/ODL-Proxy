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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Lldp/LldpFcoePriorityConfMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    LldpFcoePriorityConfMessage::LldpFcoePriorityConfMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_FCOE_PRIORITY_CONF)
    {
    }
    
    LldpFcoePriorityConfMessage::LldpFcoePriorityConfMessage (const UI32 &cmdcode, const UI32 &priority_bitmap)
        : DcmManagementInterfaceMessage (MSG_LLDP_FCOE_PRIORITY_CONF),
        m_cmdcode (cmdcode),
        m_priority_bitmap (priority_bitmap)
    {
    }

    LldpFcoePriorityConfMessage::~LldpFcoePriorityConfMessage ()
    {
    }

    void  LldpFcoePriorityConfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_priority_bitmap, "priority_bitmap"));
    }

	const void *LldpFcoePriorityConfMessage::getCStructureForInputs ()
    {
        lldp_fcoe_priority_conf_t   *msg_p = new lldp_fcoe_priority_conf_t;

        msg_p->cmdcode = m_cmdcode;
        msg_p->priority_bitmap = m_priority_bitmap;
        
        return msg_p;
    }

	void LldpFcoePriorityConfMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}

}
