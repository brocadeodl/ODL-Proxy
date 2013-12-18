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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ram Kumar Gandhi                                                  *
 **************************************************************************/

#include "Nsm/Global/ReservedVlanManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    ReservedVlanManagedObject::ReservedVlanManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
          PrismPersistableObject (ReservedVlanManagedObject::getClassName (), 
          DcmManagedObject::getClassName ()),
          WaveManagedObject (pNsmGlobalObjectManager),
          DcmManagedObject (pNsmGlobalObjectManager)
    {
        m_start = UI32UC(3863);
        m_end   = UI32UC(4090);
    }

    ReservedVlanManagedObject::ReservedVlanManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &start, const UI32 &end)
        : PrismElement  (pNsmGlobalObjectManager),
          PrismPersistableObject (ReservedVlanManagedObject::getClassName (), 
          DcmManagedObject::getClassName ()),
          WaveManagedObject (pNsmGlobalObjectManager),
          DcmManagedObject (pNsmGlobalObjectManager)
    {     
          m_start = UI32UC(start);
          m_end   = UI32UC(end);
 
    }

    ReservedVlanManagedObject::~ReservedVlanManagedObject ()
    {
    }

    string  ReservedVlanManagedObject::getClassName()
    {
        return ("ReservedVlanManagedObject");
    }

    void  ReservedVlanManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        
        addPersistableAttribute (new AttributeUI32UC (&m_start, false, "reservedVlanStart"));
        addPersistableAttribute (new AttributeUI32UC (&m_end, false, "reservedVlanEnd"));
        
      	setUserTagForAttribute ("reservedVlanStart", brocade_interface_reserved_vlan_start);
	    	setUserTagForAttribute ("reservedVlanEnd"  , brocade_interface_reserved_vlan_end);
    }

    void  ReservedVlanManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        
        addPersistableAttributeForCreate  (new AttributeUI32UC (&m_start, false, "reservedVlanStart"));
        addPersistableAttributeForCreate  (new AttributeUI32UC (&m_end, false, "reservedVlanEnd"));
    }

    void  ReservedVlanManagedObject::setStart(const UI32 &start)
    {
        m_start.setUI32Value(start);
        m_start.setIsUserConfigured(true);
    }

    UI32  ReservedVlanManagedObject::getStart() const
    {
        return (m_start.getUI32Value());
    }
 
     void  ReservedVlanManagedObject::setEnd(const UI32 &end)
    {
        m_end.setUI32Value(end);
        m_end.setIsUserConfigured(true);
    }

    UI32  ReservedVlanManagedObject::getEnd() const
    {
        return (m_end.getUI32Value());
    }
}
