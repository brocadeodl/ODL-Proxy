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
 *   Author : vtummala                                                     *
 **************************************************************************/

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceSetLocalDomainIdMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceSetLocalDomainIdMessage::FabricServiceSetLocalDomainIdMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICESETLOCALDOMAINID)
    {
    }

    FabricServiceSetLocalDomainIdMessage::FabricServiceSetLocalDomainIdMessage (const SI32 &domain_id)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICESETLOCALDOMAINID),
        m_domain_id    (domain_id)
    {
    }

    FabricServiceSetLocalDomainIdMessage::~FabricServiceSetLocalDomainIdMessage ()
    {
    }

    void  FabricServiceSetLocalDomainIdMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_domain_id,"domain_id"));
    }

    void  FabricServiceSetLocalDomainIdMessage::setDomain_id(const SI32 &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    SI32  FabricServiceSetLocalDomainIdMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

}
