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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "L2sys/Global/L2sysGlobalUpdateL2sysAgingTimeoutMessage.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Global/L2sysGlobalTypes.h"

namespace DcmNs
{

    L2sysGlobalUpdateL2sysAgingTimeoutMessage::L2sysGlobalUpdateL2sysAgingTimeoutMessage ()
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName (),L2SYSGLOBALUPDATEL2SYSAGINGTIMEOUT),
        m_AgingTimeout    (L2SYS_DCM_MAC_AGING_DEFAULT)
    {
    }

    L2sysGlobalUpdateL2sysAgingTimeoutMessage::L2sysGlobalUpdateL2sysAgingTimeoutMessage (const UI32 &AgingTimeout)
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName (),L2SYSGLOBALUPDATEL2SYSAGINGTIMEOUT),
        m_AgingTimeout    (AgingTimeout)
    {
    }

    L2sysGlobalUpdateL2sysAgingTimeoutMessage::~L2sysGlobalUpdateL2sysAgingTimeoutMessage ()
    {
    }

    void  L2sysGlobalUpdateL2sysAgingTimeoutMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_AgingTimeout,"AgingTimeout"));
    }

    void  L2sysGlobalUpdateL2sysAgingTimeoutMessage::setAgingTimeout(const UI32 &AgingTimeout)
    {
        m_AgingTimeout  =  AgingTimeout;
    }

    UI32  L2sysGlobalUpdateL2sysAgingTimeoutMessage::getAgingTimeout() const
    {
        return (m_AgingTimeout);
    }

}
