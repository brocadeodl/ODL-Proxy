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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : krangara                                                     *
 **************************************************************************/

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceDisableIslsToNeighborMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceDisableIslsToNeighborMessage::FabricServiceDisableIslsToNeighborMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEDISABLEISLSTONEIGHBOR)
    {
    }

    FabricServiceDisableIslsToNeighborMessage::FabricServiceDisableIslsToNeighborMessage (const WorldWideName &switch_wwn,const SI32 &reason_code)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEDISABLEISLSTONEIGHBOR),
        m_switch_wwn    (switch_wwn),
        m_reason_code    (reason_code)
    {
    }

    FabricServiceDisableIslsToNeighborMessage::~FabricServiceDisableIslsToNeighborMessage ()
    {
    }

    void  FabricServiceDisableIslsToNeighborMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeWorldWideName(&m_switch_wwn,"switch_wwn"));
        addSerializableAttribute (new AttributeSI32(&m_reason_code,"reason_code"));
    }

    void  FabricServiceDisableIslsToNeighborMessage::setSwitch_wwn(const WorldWideName &switch_wwn)
    {
        m_switch_wwn  =  switch_wwn;
    }

    WorldWideName  FabricServiceDisableIslsToNeighborMessage::getSwitch_wwn() const
    {
        return (m_switch_wwn);
    }

    void  FabricServiceDisableIslsToNeighborMessage::setReason_code(const SI32 &reason_code)
    {
        m_reason_code  =  reason_code;
    }

    SI32  FabricServiceDisableIslsToNeighborMessage::getReason_code() const
    {
        return (m_reason_code);
    }

}
