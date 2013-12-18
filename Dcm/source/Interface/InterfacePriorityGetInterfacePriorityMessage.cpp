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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/InterfacePriorityGetInterfacePriorityMessage.h"
#include "Interface/InterfacePriorityTypes.h"

namespace DcmNs
{

    InterfacePriorityGetInterfacePriorityMessage::InterfacePriorityGetInterfacePriorityMessage ()
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEPRIORITYGETINTERFACEPRIORITY)
    {
    }

    InterfacePriorityGetInterfacePriorityMessage::InterfacePriorityGetInterfacePriorityMessage (const SI32 &vlanPriority,const SI32 &ethernetPriority)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEPRIORITYGETINTERFACEPRIORITY),
        m_vlanPriority    (vlanPriority),
        m_ethernetPriority    (ethernetPriority)
    {
    }

    InterfacePriorityGetInterfacePriorityMessage::~InterfacePriorityGetInterfacePriorityMessage ()
    {
    }

    void  InterfacePriorityGetInterfacePriorityMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_vlanPriority,"vlanPriority"));
        addSerializableAttribute (new AttributeSI32(&m_ethernetPriority,"ethernetPriority"));
    }

    void  InterfacePriorityGetInterfacePriorityMessage::setVlanPriority(const SI32 &vlanPriority)
    {
        m_vlanPriority  =  vlanPriority;
    }

    SI32  InterfacePriorityGetInterfacePriorityMessage::getVlanPriority() const
    {
        return (m_vlanPriority);
    }

    void  InterfacePriorityGetInterfacePriorityMessage::setEthernetPriority(const SI32 &ethernetPriority)
    {
        m_ethernetPriority  =  ethernetPriority;
    }

    SI32  InterfacePriorityGetInterfacePriorityMessage::getEthernetPriority() const
    {
        return (m_ethernetPriority);
    }

}
