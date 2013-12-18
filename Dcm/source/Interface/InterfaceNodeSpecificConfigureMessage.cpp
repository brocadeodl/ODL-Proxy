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
#include "Interface/InterfaceNodeSpecificConfigureMessage.h"
#include "Interface/InterfaceNodeSpecificTypes.h"

namespace DcmNs
{

    InterfaceNodeSpecificConfigureMessage::InterfaceNodeSpecificConfigureMessage ()
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACENODESPECIFICCONFIGUREINTERFACE)
    {
    }

    InterfaceNodeSpecificConfigureMessage::InterfaceNodeSpecificConfigureMessage (const SI32 &nsVlan,const SI32 &nsEthernet)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACENODESPECIFICCONFIGUREINTERFACE),
        m_nsVlan    (nsVlan),
        m_nsEthernet    (nsEthernet)
    {
    }
    
    InterfaceNodeSpecificConfigureMessage::~InterfaceNodeSpecificConfigureMessage ()
    {
    }

    void  InterfaceNodeSpecificConfigureMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_nsVlan,"nsVlan"));
        addSerializableAttribute (new AttributeSI32(&m_nsEthernet,"nsEthernet"));
    }
    
    void  InterfaceNodeSpecificConfigureMessage::setNsVlan(const SI32 &nsVlan)
    {
        m_nsVlan  =  nsVlan;
    }

    SI32  InterfaceNodeSpecificConfigureMessage::getNsVlan() const
    {
        return (m_nsVlan);
    }

    void  InterfaceNodeSpecificConfigureMessage::setNsEthernet(const SI32 &nsEthernet)
    {
        m_nsEthernet  =  nsEthernet;
    }

    SI32  InterfaceNodeSpecificConfigureMessage::getNsEthernet() const
    {
        return (m_nsEthernet);
    }


}
