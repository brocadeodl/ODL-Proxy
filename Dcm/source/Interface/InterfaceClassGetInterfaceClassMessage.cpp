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
#include "Interface/InterfaceClassGetInterfaceClassMessage.h"
#include "Interface/InterfaceClassTypes.h"

namespace DcmNs
{

    InterfaceClassGetInterfaceClassMessage::InterfaceClassGetInterfaceClassMessage ()
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECLASSGETINTERFACECLASS)
    {
    }

    InterfaceClassGetInterfaceClassMessage::InterfaceClassGetInterfaceClassMessage (const SI32 &vlanClass,const SI32 &ethernetClass)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECLASSGETINTERFACECLASS),
        m_vlanClass    (vlanClass),
        m_ethernetClass    (ethernetClass)
    {
    }

    InterfaceClassGetInterfaceClassMessage::~InterfaceClassGetInterfaceClassMessage ()
    {
    }

    void  InterfaceClassGetInterfaceClassMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_vlanClass,"vlanClass"));
        addSerializableAttribute (new AttributeSI32(&m_ethernetClass,"ethernetClass"));
    }

    void  InterfaceClassGetInterfaceClassMessage::setVlanClass(const SI32 &vlanClass)
    {
        m_vlanClass  =  vlanClass;
    }

    SI32  InterfaceClassGetInterfaceClassMessage::getVlanClass() const
    {
        return (m_vlanClass);
    }

    void  InterfaceClassGetInterfaceClassMessage::setEthernetClass(const SI32 &ethernetClass)
    {
        m_ethernetClass  =  ethernetClass;
    }

    SI32  InterfaceClassGetInterfaceClassMessage::getEthernetClass() const
    {
        return (m_ethernetClass);
    }

}
