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
 *   Author : krangara                                                     *
 **************************************************************************/

#include "FspfService/Local/FspfServiceLocalObjectManager.h"
#include "FspfService/Local/FspfServiceUpdateBcastPriorityInfoMessage.h"
#include "FspfService/Local/FspfServiceTypes.h"

namespace DcmNs
{

    FspfServiceUpdateBcastPriorityInfoMessage::FspfServiceUpdateBcastPriorityInfoMessage ()
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEUPDATEBCASTPRIORITYINFO)
    {
    }

    FspfServiceUpdateBcastPriorityInfoMessage::FspfServiceUpdateBcastPriorityInfoMessage (const UI32 &switchId,const UI32 &bcastPriority)
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEUPDATEBCASTPRIORITYINFO),
        m_switchId    (switchId),
        m_bcastPriority    (bcastPriority)
    {
    }

    FspfServiceUpdateBcastPriorityInfoMessage::~FspfServiceUpdateBcastPriorityInfoMessage ()
    {
    }

    void  FspfServiceUpdateBcastPriorityInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_switchId,"switchId"));
        addSerializableAttribute (new AttributeUI32(&m_bcastPriority,"bcastPriority"));
    }

    void  FspfServiceUpdateBcastPriorityInfoMessage::setSwitchId(const UI32 &switchId)
    {
        m_switchId  =  switchId;
    }

    UI32  FspfServiceUpdateBcastPriorityInfoMessage::getSwitchId() const
    {
        return (m_switchId);
    }

    void  FspfServiceUpdateBcastPriorityInfoMessage::setBcastPriority(const UI32 &bcastPriority)
    {
        m_bcastPriority  =  bcastPriority;
    }

    UI32  FspfServiceUpdateBcastPriorityInfoMessage::getBcastPriority() const
    {
        return (m_bcastPriority);
    }

}
