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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Authors : Mobin Mohan                                                 *
 **************************************************************************/

#include "vcs.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Utils/TraceUtils.h"
#include "Lldp/Global/CdpGlobalConfManagedObject.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"

namespace DcmNs
{

    CdpGlobalConfManagedObject::CdpGlobalConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (CdpGlobalConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager)
    {
        m_protoEnable = false;
        
		trace (TRACE_LEVEL_INFO," CdpGlobalConfManagedObject::Constructor"
			                                          "Entering..");
    }

    CdpGlobalConfManagedObject::CdpGlobalConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const bool &protoEnable)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (CdpGlobalConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager),
        m_protoEnable    (protoEnable)
    {
        m_protoEnable = false;

		trace (TRACE_LEVEL_INFO," CdpGlobalConfManagedObject::Constructor"
			                                                      "Entering..");
    }

    CdpGlobalConfManagedObject::~CdpGlobalConfManagedObject ()
    {
        trace (TRACE_LEVEL_INFO," CdpGlobalConfManagedObject::Destructor"
			                                                      "Entering..");
    }

    string  CdpGlobalConfManagedObject::getClassName()
    {
        return ("CdpGlobalConfManagedObject");
    }

    void  CdpGlobalConfManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_protoEnable,"protoEnable",cdp_cdp));
    }

    void  CdpGlobalConfManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_protoEnable,"protoEnable",cdp_cdp));
    }

    void  CdpGlobalConfManagedObject::setProtoEnable(const bool &protoEnable)
    {
        m_protoEnable  =  protoEnable;
    }

    bool  CdpGlobalConfManagedObject::getProtoEnable() const
    {
        return (m_protoEnable);
    }
    
}
