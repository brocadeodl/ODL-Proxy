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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Interface/NDeepComposition/InterfaceNDeepCompositionChildLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionSkippedGChildLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"

#include "interface_with_ndeepcomposition.h"

namespace DcmNs
{

////////////////////////////////////////////////////////////////////////////////////
/// NDeep Composition Child
///////////////////////////////////////////////////////////////////

InterfaceNDeepCompositionChildLocalManagedObject::InterfaceNDeepCompositionChildLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceNDeepCompositionChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceNDeepCompositionChildLocalManagedObject::InterfaceNDeepCompositionChildLocalManagedObject (WaveObjectManager *pWaveObjectManager, SI32 &id, bool allow)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceNDeepCompositionChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_rule_id                  (id),
      m_allow                    (allow)
{
}

InterfaceNDeepCompositionChildLocalManagedObject::~InterfaceNDeepCompositionChildLocalManagedObject ()
{
}

string InterfaceNDeepCompositionChildLocalManagedObject::getClassName ()
{
    return ("InterfaceNDeepCompositionChildLocalManagedObject");
}

void InterfaceNDeepCompositionChildLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeSI32          (&m_rule_id,          "rule_id",         interface_ndeepcomposition_rule_id));
    addPersistableAttribute (new AttributeBool          (&m_allow,            "allow",           interface_ndeepcomposition_isAllowed));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<InterfaceNDeepCompSkippedGChildLocalManagedObject> (&m_nDeepGChildren, "nDeepGChildren", getClassName (), getObjectId (), InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeManagedObjectComposition<InterfaceNDeepCompSkippedGChildLocalManagedObject> (&m_nDeepGChild, "nDeepGChild", InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName (), true));
}

void InterfaceNDeepCompositionChildLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeSI32          (&m_rule_id,           "rule_id",           interface_ndeepcomposition_rule_id));
    addPersistableAttributeForCreate (new AttributeBool          (&m_allow,           "allow",           interface_ndeepcomposition_isAllowed));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<InterfaceNDeepCompSkippedGChildLocalManagedObject> (&m_nDeepGChildren, "nDeepGChildren", getClassName (), getObjectId (), InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition<InterfaceNDeepCompSkippedGChildLocalManagedObject> (&m_nDeepGChild, "nDeepGChild", InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName (), true));

    vector<string > keyName;
    keyName.push_back ("rule_id");
    setUserDefinedKeyCombination (keyName);
}

SI32 InterfaceNDeepCompositionChildLocalManagedObject::getId () const
{
    return (m_rule_id);
}

void InterfaceNDeepCompositionChildLocalManagedObject::setId (const SI32 &rule_id)
{
    m_rule_id = rule_id;
}

bool InterfaceNDeepCompositionChildLocalManagedObject::getIsAllowed () const
{
    return (m_allow);
}

void InterfaceNDeepCompositionChildLocalManagedObject::setIsAllowed (const bool &allow)
{
    m_allow = allow;
}

vector<WaveManagedObjectPointer<InterfaceNDeepCompSkippedGChildLocalManagedObject> > InterfaceNDeepCompositionChildLocalManagedObject::getNDeepGChildren () const
{
    return (m_nDeepGChildren);
}

void InterfaceNDeepCompositionChildLocalManagedObject::setNDeepGChildren (vector<WaveManagedObjectPointer<InterfaceNDeepCompSkippedGChildLocalManagedObject> > nDeepGChildren)
{
    m_nDeepGChildren = nDeepGChildren;
}

WaveManagedObjectPointer<InterfaceNDeepCompSkippedGChildLocalManagedObject> InterfaceNDeepCompositionChildLocalManagedObject::getNDeepGChild () const
{
    return (m_nDeepGChild);
}

void InterfaceNDeepCompositionChildLocalManagedObject::setNDeepGChild (WaveManagedObjectPointer<InterfaceNDeepCompSkippedGChildLocalManagedObject> nDeepGChild)
{
    m_nDeepGChild = nDeepGChild;
}

}
