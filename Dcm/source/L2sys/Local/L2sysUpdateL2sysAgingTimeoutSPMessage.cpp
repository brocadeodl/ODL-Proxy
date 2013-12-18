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

#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysAgingTimeoutSPMessage.h"
#include "L2sys/Local/L2sysTypes.h"
#include "L2sys/Global/L2sysGlobalTypes.h"

namespace DcmNs
{

    L2sysUpdateL2sysAgingTimeoutSPMessage::L2sysUpdateL2sysAgingTimeoutSPMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSAGINGTIMEOUTSP),
        m_AgingTimeout    (L2SYS_DCM_MAC_AGING_DEFAULT)
    {
    }

    L2sysUpdateL2sysAgingTimeoutSPMessage::L2sysUpdateL2sysAgingTimeoutSPMessage (const UI32 &AgingTimeout)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSAGINGTIMEOUTSP),
        m_AgingTimeout    (AgingTimeout)
    {
    }

    L2sysUpdateL2sysAgingTimeoutSPMessage::~L2sysUpdateL2sysAgingTimeoutSPMessage ()
    {
    }

    void  L2sysUpdateL2sysAgingTimeoutSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_AgingTimeout,"AgingTimeout"));
    }

    void  L2sysUpdateL2sysAgingTimeoutSPMessage::setAgingTimeout(const UI32 &AgingTimeout)
    {
        m_AgingTimeout  =  AgingTimeout;
    }

    UI32  L2sysUpdateL2sysAgingTimeoutSPMessage::getAgingTimeout() const
    {
        return (m_AgingTimeout);
    }

}
