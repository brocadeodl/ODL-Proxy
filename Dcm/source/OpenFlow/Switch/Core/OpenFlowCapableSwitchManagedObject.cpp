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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowCapableSwitchManagedObject.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Attributes/Attributes.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowCapableSwitchManagedObject::OpenFlowCapableSwitchManagedObject (WaveObjectManager *pWaveObjectManager)
: PrismElement           (pWaveObjectManager),
  PrismPersistableObject (OpenFlowCapableSwitchManagedObject::getClassName (), DcmManagedObject::getClassName ()),
  WaveManagedObject      (pWaveObjectManager),
  DcmManagedObject       (pWaveObjectManager)
{
}

OpenFlowCapableSwitchManagedObject::~OpenFlowCapableSwitchManagedObject ()
{
}

string OpenFlowCapableSwitchManagedObject::getClassName ()
{
    return ("OpenFlowCapableSwitchManagedObject");
}

void OpenFlowCapableSwitchManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_openFlowSwitchName, "openFlowSwitchName"));
    addPersistableAttribute (new AttributeUri    (&m_openFlowSwitchUri,  "openFlowSwitchUri" ));
}

void OpenFlowCapableSwitchManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_openFlowSwitchName, "openFlowSwitchName"));
    addPersistableAttributeForCreate (new AttributeUri    (&m_openFlowSwitchUri,  "openFlowSwitchUri" ));
}

string OpenFlowCapableSwitchManagedObject::getOpenFlowSwitchName () const
{
    return (m_openFlowSwitchName);
}

void OpenFlowCapableSwitchManagedObject::setOpenFlowSwitchName (const string &openFlowSwitchName)
{
    m_openFlowSwitchName = openFlowSwitchName;
}

Uri OpenFlowCapableSwitchManagedObject::getOpenFlowSwitchUri () const
{
    return (m_openFlowSwitchUri);
}

void OpenFlowCapableSwitchManagedObject::setOpenFlowSwitchUri (const Uri &openFlowSwitchUri)
{
    m_openFlowSwitchUri = openFlowSwitchUri;
}

}
