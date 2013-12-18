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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject5.h"

namespace WaveNs
{

UpgradeTestManagedObject5::UpgradeTestManagedObject5 (WaveObjectManager *pWaveObjectManager)
        : PrismElement               (pWaveObjectManager),
        PrismPersistableObject     (UpgradeTestManagedObject5::getClassName (), UpgradeTestManagedObject4::getClassName ()),
        UpgradeTestManagedObject4    (pWaveObjectManager),
        WaveLocalManagedObjectBase (this)
{
}

UpgradeTestManagedObject5::~UpgradeTestManagedObject5 ()
{
}

string UpgradeTestManagedObject5::getClassName ()
{
    return ("UpgradeTestManagedObject5");
}

void UpgradeTestManagedObject5::setupAttributesForPersistence ()
{
    UpgradeTestManagedObject4::setupAttributesForPersistence ();
    WaveLocalManagedObjectBase::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_associationVector5, "associationVector5", getClassName (), getObjectId (), UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject5::setupAttributesForCreate ()
{
    WaveLocalManagedObjectBase::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_associationVector5, "associationVector5", getClassName (), getObjectId (), UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject5::setAssociationVector5 (const vector<ObjectId> &objectVector5)
{
    m_associationVector5 = objectVector5;
}
vector<ObjectId> UpgradeTestManagedObject5::getAssociationVector5 () const
{
    return (m_associationVector5);
}

bool UpgradeTestManagedObject5::operator == (const UpgradeTestManagedObject5 &rhs) const
{
    bool isEqual = UpgradeTestManagedObject4::operator == (rhs);

    if (false == isEqual)
    {
        return (false);
    }

    if (rhs.m_associationVector5 != m_associationVector5)
    {
        return (false);
    }

    return (true);
}

bool UpgradeTestManagedObject5::operator != (const UpgradeTestManagedObject5 &rhs) const
{
    return (!(operator == (rhs)));
}

}
