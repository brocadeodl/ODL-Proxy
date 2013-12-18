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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rhkumar                                                      *
 **************************************************************************/
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    AgNPortMapLocalManagedObject::AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgNPortMapLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager)
        {
            trace (TRACE_LEVEL_DEBUG, "AgNPortMapLocalManagedObject Constructor 1...");
        }

     AgNPortMapLocalManagedObject:: AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager, const string &agNPortNb)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgNPortMapLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager),
        m_agNPortNb    (agNPortNb)
    {
        m_agFailover = false;
        m_agFailback = false;
    }

     AgNPortMapLocalManagedObject:: AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
        const string &agNPortNb, const vector<string> &agNFPortTopo, const bool &agFailover, const bool &agFailback)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgNPortMapLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager),
        m_agNPortNb    (agNPortNb),
        m_agNFPortTopo (agNFPortTopo),
        m_agFailover   (agFailover),
        m_agFailback   (agFailback)
    {
    }

    AgNPortMapLocalManagedObject::~AgNPortMapLocalManagedObject ()
    {
    }

    string  AgNPortMapLocalManagedObject::getClassName()
    {
        return ("AgNPortMapLocalManagedObject");
    }

    void AgNPortMapLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("agNPortNb");
        setUserDefinedKeyCombination (keyName);
    }

    void  AgNPortMapLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_agNPortNb, "agNPortNb", ag_agNPortNb));
        addPersistableAttribute (new AttributeStringVector(&m_agNFPortTopo, "agNFPortTopo", ag_map_fcoe));
        addPersistableAttribute (new AttributeStringVector(&m_agStaticNFPortTopo, "agStaticNFPortTopo", ag_staticmap_fcoe));
        addPersistableAttribute (new AttributeBool(&m_agFailover, "agFailover", ag_failoverenable));
        addPersistableAttribute (new AttributeBool(&m_agFailback, "agFailback", ag_failbackenable));
    }

    void  AgNPortMapLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeString(&m_agNPortNb, "agNPortNb", ag_agNPortNb));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_agNFPortTopo, "agNFPortTopo", ag_map_fcoe));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_agStaticNFPortTopo, "agStaticNFPortTopo", ag_staticmap_fcoe));
        addPersistableAttributeForCreate (new AttributeBool(&m_agFailover, "agFailover" , ag_failoverenable));
        addPersistableAttributeForCreate (new AttributeBool(&m_agFailback, "agFailback" , ag_failbackenable));
    }

    void AgNPortMapLocalManagedObject::setNPortNb(const string &NPortNb)
    {
        m_agNPortNb = NPortNb; 
    }

    string AgNPortMapLocalManagedObject::getNPortNb() const
    {
        return m_agNPortNb;
    }

    void AgNPortMapLocalManagedObject::setStaticNFPortTopo(const vector<string> &StaticNFPortTopo)
    {
        m_agStaticNFPortTopo = StaticNFPortTopo; 
    }

    vector<string> AgNPortMapLocalManagedObject::getStaticNFPortTopo() const
    {
        return (m_agStaticNFPortTopo);
    }

    void AgNPortMapLocalManagedObject::setNFPortTopo(const vector<string> &NFPortTopo)
    {
        m_agNFPortTopo = NFPortTopo; 
    }

    vector<string> AgNPortMapLocalManagedObject::getNFPortTopo() const
    {
        return (m_agNFPortTopo);
    }

    void AgNPortMapLocalManagedObject::setFailover(const bool &agFailover)
    {
        m_agFailover = agFailover;
    }

    bool AgNPortMapLocalManagedObject::getFailover() const
    {
        return(m_agFailover);
    }

    void AgNPortMapLocalManagedObject::setFailback(const bool &agFailback)
    {
        m_agFailback = agFailback;
    }

    bool AgNPortMapLocalManagedObject::getFailback() const
    {
        return(m_agFailback);
    }
}
