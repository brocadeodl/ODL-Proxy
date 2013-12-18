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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/NDeepComposition/InterfaceNDeepCompositionLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionChildLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionGChildLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "interface_with_ndeepcomposition.h"

namespace DcmNs
{

InterfaceNDeepCompositionLocalManagedObject::InterfaceNDeepCompositionLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceNDeepCompositionLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceNDeepCompositionLocalManagedObject::~InterfaceNDeepCompositionLocalManagedObject ()
{
}

string InterfaceNDeepCompositionLocalManagedObject::getClassName ()
{
    return ("InterfaceNDeepCompositionLocalManagedObject");
}

void InterfaceNDeepCompositionLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeManagedObjectVectorComposition<InterfaceNDeepCompositionChildLocalManagedObject> (&m_nDeepchildren, "nDeepchildren", getClassName (), getObjectId (), InterfaceNDeepCompositionChildLocalManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeManagedObjectComposition<InterfaceNDeepCompositionChildLocalManagedObject> (&m_nDeepchild, "nDeepchild", InterfaceNDeepCompositionChildLocalManagedObject::getClassName (), true));

    setUserTagForAttribute ("name", interface_ndeepcomposition_name);
}

void InterfaceNDeepCompositionLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<InterfaceNDeepCompositionChildLocalManagedObject> (&m_nDeepchildren, "nDeepchildren", getClassName (), getObjectId (), InterfaceNDeepCompositionChildLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition<InterfaceNDeepCompositionChildLocalManagedObject> (&m_nDeepchild, "nDeepchild", InterfaceNDeepCompositionChildLocalManagedObject::getClassName (), true));

    vector<string > keyName;
    keyName.push_back ("name");
    setUserDefinedKeyCombination (keyName);
}

void InterfaceNDeepCompositionLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}


vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > InterfaceNDeepCompositionLocalManagedObject::getNDeepChildren () const
{
    return (m_nDeepchildren);
}

void InterfaceNDeepCompositionLocalManagedObject::setNDeepChildren (vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > children)
{
    m_nDeepchildren = children;
}

WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> InterfaceNDeepCompositionLocalManagedObject::getNDeepChild () const
{
    return (m_nDeepchild);
}

void InterfaceNDeepCompositionLocalManagedObject::setNDeepChild (WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> child)
{
    m_nDeepchild = child;
}
}
