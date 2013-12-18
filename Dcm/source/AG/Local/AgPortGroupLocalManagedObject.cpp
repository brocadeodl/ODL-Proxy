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
#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    AgPortGroupLocalManagedObject::AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgPortGroupLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager)
        {
            trace (TRACE_LEVEL_DEBUG, "AgPortGroupLocalManagedObject Constructor 1...");
        }

     AgPortGroupLocalManagedObject:: AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager, const UI32 &agPgId, const string &agPgName, const vector<string> &agPgModes)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgPortGroupLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager),
        m_agPgId    (agPgId),
        m_agPgFlags (agPgModes),
        m_agPgName       (agPgName)
    {
    }


     AgPortGroupLocalManagedObject:: AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager, const UI32 &agPgId,
        const bool &agPgEnabled, const vector<string> &agPgFlags, const string &agPgName, const vector<string> &agPgNPortTopo, const vector<string> &agPgFPortTopo)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgPortGroupLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager),
        m_agPgId         (agPgId),
        m_agPgEnabled    (agPgEnabled),
        m_agPgFlags      (agPgFlags),
        m_agPgName       (agPgName),
        m_agPgNPortTopo  (agPgNPortTopo),
        m_agPgFPortTopo  (agPgFPortTopo)
    {
    }
    AgPortGroupLocalManagedObject::~AgPortGroupLocalManagedObject ()
    {
    }

    string  AgPortGroupLocalManagedObject::getClassName()
    {
        return ("AgPortGroupLocalManagedObject");
    }

    void AgPortGroupLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("agPgId");
        setUserDefinedKeyCombination (keyName);
    }

    void  AgPortGroupLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_agPgId, "agPgId", ag_pgid));
        addPersistableAttribute (new AttributeStringVector(&m_agPgFlags, "agPgFlags", ag_modes));
        addPersistableAttribute (new AttributeString(&m_agPgName, "agPgName", ag_rename));
        addPersistableAttribute (new AttributeStringVector(&m_agPgNPortTopo, "agPgNPortTopo", ag_nports));
    }

    void  AgPortGroupLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_agPgId, "agPgId", ag_pgid));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_agPgFlags, "agPgFlags", ag_modes));
        addPersistableAttributeForCreate (new AttributeString(&m_agPgName, "agPgName", ag_rename));
        addPersistableAttributeForCreate (new AttributeStringVector(&m_agPgNPortTopo, "agPgNPortTopo", ag_nports));
    }

    UI32 AgPortGroupLocalManagedObject::getPgId() const
    {
        return m_agPgId;
    }

    void AgPortGroupLocalManagedObject::setPgEnabled(bool &agPgEnabled)
    {
        m_agPgEnabled = agPgEnabled;
    }

    bool AgPortGroupLocalManagedObject::getPgEnabled() const
    {
        return m_agPgEnabled;
    }

    void AgPortGroupLocalManagedObject::setPgFlags(vector<string> &agPgFlags)
    {
        m_agPgFlags = agPgFlags;
    }

    vector<string> AgPortGroupLocalManagedObject::getPgFlags() const
    {
        return m_agPgFlags;
    }

    void AgPortGroupLocalManagedObject::setPgName(string &agPgName)
    {
        m_agPgName = agPgName;
    }

    string AgPortGroupLocalManagedObject::getPgName() const
    {
        return m_agPgName;
    }

    void AgPortGroupLocalManagedObject::setPgNPortTopo(vector<string> &agPgNPortTopo)
    {
        m_agPgNPortTopo = agPgNPortTopo;
    }

    vector<string> AgPortGroupLocalManagedObject::getPgNPortTopo() const
    {
        return m_agPgNPortTopo;
    }

    void AgPortGroupLocalManagedObject::setPgFPortTopo(vector<string> &agPgFPortTopo)
    {
        m_agPgFPortTopo = agPgFPortTopo;
    }

    vector<string> AgPortGroupLocalManagedObject::getPgFPortTopo() const
    {
        return m_agPgFPortTopo;
    }
}
