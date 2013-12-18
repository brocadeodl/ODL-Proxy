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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "discover/Framework/discoverManagedObjects.h"
#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverTypes.h"

namespace DcmNs
{

// ************* DiscoverManagedObject ************

    DiscoverManagedObject::DiscoverManagedObject (DiscoverObjectManager *pDiscoverObjectManager)
        : PrismElement  (pDiscoverObjectManager),
        PrismPersistableObject (DiscoverManagedObject::getClassName (),
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pDiscoverObjectManager),
        DcmManagedObject (pDiscoverObjectManager)
    {
    }


    DiscoverManagedObject::~DiscoverManagedObject ()
    {
    }

    string  DiscoverManagedObject::getClassName()
    {
        return ("DiscoverManagedObject");
    }

    void  DiscoverManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_id,"id"));
        addPersistableAttribute (new AttributeUI32(&m_state,"state"));
        addPersistableAttribute (new AttributeUI32(&m_startTS,"startTS"));
        addPersistableAttribute (new AttributeUI32(&m_discStartTS,"discStartTS"));
        addPersistableAttribute (new AttributeUI32(&m_discEndTS,"discEndTS"));
        addPersistableAttribute (new AttributeUI32(&m_ppStartTS,"ppStartTS"));
        addPersistableAttribute (new AttributeUI32(&m_ppEndTS,"ppEndTS"));
        addPersistableAttribute (new AttributeUI32(&m_exitCode,"exitCode"));
        addPersistableAttribute (new AttributeUI32(&m_endTS,"endTS"));

    }

    void  DiscoverManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_state,"state"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_startTS,"startTS"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_discStartTS,"discStartTS"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_discEndTS,"discEndTS"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_ppStartTS,"ppStartTS"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_ppEndTS,"ppEndTS"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_exitCode,"exitCode"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_endTS,"endTS"));

        vector<string > keyName;
        keyName.push_back ("id");
        setUserDefinedKeyCombination (keyName);

}

    void  DiscoverManagedObject::setId(const string &id)
    {
        m_id = id;
    }

    string  DiscoverManagedObject::getId() const
    {
        return (m_id);
    }

    void  DiscoverManagedObject::setState(const UI32 &state)
    {
        m_state = state;
    }
    
    UI32  DiscoverManagedObject::getState() const
    {
        return (m_state);
    }

    void  DiscoverManagedObject::setStartTS(const UI32 &startTS)
    {
        m_startTS = startTS;
    }
    
    UI32  DiscoverManagedObject::getStartTS() const
    {
        return (m_startTS);
    }

    void  DiscoverManagedObject::setDiscStartTS(const UI32 &discStartTS)
    {
        m_discStartTS = discStartTS;
    }

    UI32  DiscoverManagedObject::getDiscStartTS() const
    {
        return (m_discStartTS);
    }
    
    void  DiscoverManagedObject::setDiscEndTS(const UI32 &discEndTS)
    {
        m_discEndTS = discEndTS;
    }

    UI32  DiscoverManagedObject::getDiscEndTS() const
    {
        return (m_discEndTS);
    }

    void  DiscoverManagedObject::setPpStartTS(const UI32 &ppStartTS)
    {
        m_ppStartTS = ppStartTS;
    }

    UI32  DiscoverManagedObject::getPpStartTS() const
    {
        return (m_ppStartTS);
    }
    
    void  DiscoverManagedObject::setPpEndTS(const UI32 &ppEndTS)
    {
        m_ppEndTS = ppEndTS;
    }

    UI32  DiscoverManagedObject::getPpEndTS() const
    {
        return (m_ppEndTS);
    }
    
    void  DiscoverManagedObject::setExitCode(const UI32 &exitCode)
    {
        m_exitCode = exitCode;
    }

    UI32  DiscoverManagedObject::getExitCode() const
    {
        return (m_exitCode);
    }

    void  DiscoverManagedObject::setEndTS(const UI32 &endTS)
    {
        m_endTS = endTS;
    }

    UI32  DiscoverManagedObject::getEndTS() const
    {
        return (m_endTS);
    }

}
