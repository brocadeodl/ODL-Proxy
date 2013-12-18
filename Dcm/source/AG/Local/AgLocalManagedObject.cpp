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
 *   Author : ranantha/rhkumar                                             *
 **************************************************************************/
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    AgLocalManagedObject::AgLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager)
        {
            trace (TRACE_LEVEL_DEBUG, "AgLocalManagedObject Constructor 1...");
        }

     AgLocalManagedObject:: AgLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager, 
        const vector<string> &policy, const bool &fportTrunkEnable)
        : PrismElement  (pAgLocalObjectManager),
        PrismPersistableObject (AgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAgLocalObjectManager),
        DcmManagedObject (pAgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAgLocalObjectManager),
        m_policy            (policy),
        m_fportTrunkEnable  (fportTrunkEnable)
    {
    }

    AgLocalManagedObject::~AgLocalManagedObject ()
    {
    }

    string  AgLocalManagedObject::getClassName()
    {
        return ("AgLocalManagedObject");
    }

    void  AgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeStringVector(&m_policy, "policy", ag_agPolicy));
        addPersistableAttribute (new AttributeBool(&m_fportTrunkEnable, "fportTrunkEnable"));
        addPersistableAttribute (new AttributeUI32(&m_fnmtovalue, "fnmtovalue", ag_fnmtovalue));
        addPersistableAttribute (new AttributeUI32(&m_relcountervalue, "reliabilityvalue", ag_reliabilitycountervalue));
    }

    void  AgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeStringVector(&m_policy, "policy", ag_agPolicy));
        addPersistableAttributeForCreate (new AttributeBool(&m_fportTrunkEnable, "fportTrunkEnable"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_fnmtovalue, "fnmtovalue", ag_fnmtovalue));
        addPersistableAttributeForCreate (new AttributeUI32(&m_relcountervalue, "reliabilityvalue", ag_reliabilitycountervalue));
    }

    UI32 AgLocalManagedObject::getRelCounterValue() const
    {
        return m_relcountervalue;
    }

    void AgLocalManagedObject::setRelCounterValue(UI32 &relcountervalue) 
    {
        m_relcountervalue = relcountervalue;
    }

    UI32 AgLocalManagedObject::getFnmtov() const
    {
        return m_fnmtovalue;
    }

    void AgLocalManagedObject::setFnmtov(UI32 &fnmtovalue) 
    {
        m_fnmtovalue = fnmtovalue;
    }

    void AgLocalManagedObject::setPolicy(vector<string> &policy)
    {
        m_policy = policy;
    }

    vector<string> AgLocalManagedObject::getPolicy() const
    {
        return m_policy;
    }

    void AgLocalManagedObject::setFportTrunkEnable(bool &fportTrunkEnable)
    {
        m_fportTrunkEnable = fportTrunkEnable;
    }

    bool AgLocalManagedObject::getFportTrunkEnable() const
    {
        return m_fportTrunkEnable;
    }
}
