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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Interface/Association/InterfaceAssociationLocalManagedObject.h"
#include "Framework/Attributes/Attributes.h"

#include "interface_with_association.h"

namespace DcmNs
{

DcmInterfaceAssociationLocalManagedObject::DcmInterfaceAssociationLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (DcmInterfaceAssociationLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

DcmInterfaceAssociationLocalManagedObject::~DcmInterfaceAssociationLocalManagedObject ()
{
}

string DcmInterfaceAssociationLocalManagedObject::getClassName ()
{
    return ("DcmInterfaceAssociationLocalManagedObject");
}

void DcmInterfaceAssociationLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute  (new AttributeObjectIdAssociation(&m_vlanId,"vlanId", InterfaceSampleVlanLocalManagedObject::getClassName ()));
    addPersistableAttribute  (new AttributeObjectIdAssociation(&m_objectId,"nullObjectId", InterfaceSampleVlanLocalManagedObject::getClassName (), true));
    addPersistableAttribute  (new AttributeObjectIdVectorAssociation(&m_vlanIdList,"vlanIdList", getClassName (), getObjectId (), InterfaceSampleVlanLocalManagedObject::getClassName ()));
    addPersistableAttribute  (new AttributeObjectIdVectorAssociation(&m_vlanIdList2,"vlanIdList2", getClassName (), getObjectId (), InterfaceSampleVlanLocalManagedObject::getClassName (), 0, false, true));

    setUserTagForAttribute ("name", DcmInterfaceAssociation_name);
    setUserTagForAttribute ("vlanId", DcmInterfaceAssociation_vlanId);
    setUserTagForAttribute ("nullObjectId", DcmInterfaceAssociation_nullObjectId);
    setUserTagForAttribute ("vlanIdList", DcmInterfaceAssociation_vlanIdList);
    setUserTagForAttribute ("vlanIdList2", DcmInterfaceAssociation_vlanIdList2);
}

void DcmInterfaceAssociationLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_vlanId,"vlanId", InterfaceSampleVlanLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_objectId,"nullObjectId", InterfaceSampleVlanLocalManagedObject::getClassName (), true));
    addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_vlanIdList,"vlanIdList", getClassName (), getObjectId (), InterfaceSampleVlanLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_vlanIdList2,"vlanIdList2", getClassName (), getObjectId (), InterfaceSampleVlanLocalManagedObject::getClassName (), 0, false, true));

    vector<string > keyName;
    keyName.push_back ("name");
    setUserDefinedKeyCombination (keyName);
}

void DcmInterfaceAssociationLocalManagedObject::setVlanId (const ObjectId &vlanId)
{
    m_vlanId = vlanId;
}

ObjectId DcmInterfaceAssociationLocalManagedObject::getVlanId ()
{
    return (m_vlanId);
}

void DcmInterfaceAssociationLocalManagedObject::setObjectId (const ObjectId &objectId)
{
    m_objectId = objectId;
}

void DcmInterfaceAssociationLocalManagedObject::setVlanIdVector (const vector<ObjectId> &vlanIdList)
{
    m_vlanIdList = vlanIdList;
}

vector<ObjectId> DcmInterfaceAssociationLocalManagedObject::getVlanIdVector ()
{
    return (m_vlanIdList);
}

void DcmInterfaceAssociationLocalManagedObject::setVlanIdVector2 (const vector<ObjectId> &vlanIdList)
{
    m_vlanIdList2 = vlanIdList;
}

vector<ObjectId> DcmInterfaceAssociationLocalManagedObject::getVlanIdVector2 ()
{
    return (m_vlanIdList2);
}
////////////////////////////////////////////////////////////////////////////////////
/// Association Child
///////////////////////////////////////////////////////////////////

InterfaceSampleVlanLocalManagedObject::InterfaceSampleVlanLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceSampleVlanLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceSampleVlanLocalManagedObject::InterfaceSampleVlanLocalManagedObject (WaveObjectManager *pWaveObjectManager, UI32 &id)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceSampleVlanLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_id                  (id)
{
}

InterfaceSampleVlanLocalManagedObject::~InterfaceSampleVlanLocalManagedObject ()
{
}

string InterfaceSampleVlanLocalManagedObject::getClassName ()
{
    return ("InterfaceSampleVlanLocalManagedObject");
}

void InterfaceSampleVlanLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32          (&m_id,           "id",           DcmInterfaceAssociation_id));

}

void InterfaceSampleVlanLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32          (&m_id,           "id",           DcmInterfaceAssociation_id));

    vector<string > keyName;
    keyName.push_back ("id");
    trace (TRACE_LEVEL_INFO, "InterfaceSampleVlanLocalManagedObject::setupAttributesForCreate -> setUserDefinedKeyCombination ----------->");
    setUserDefinedKeyCombination (keyName);
}

UI32 InterfaceSampleVlanLocalManagedObject::getId () const
{
    return (m_id);
}

void InterfaceSampleVlanLocalManagedObject::setId (const UI32 &id)
{
    m_id = id;
}

}
