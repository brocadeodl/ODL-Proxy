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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Cluster/Local/WaveHaNode.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace WaveNs
{

WaveHaNode::WaveHaNode (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (WaveHaNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (pWaveObjectManager)
{
    setOwnerWaveNodeObjectId (getObjectId ());
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

WaveHaNode::WaveHaNode (WaveObjectManager *pWaveObjectManager, const WaveHaNodeRole &role, const string &internalIpAddress, const string &serialNumber)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (WaveHaNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (pWaveObjectManager),
      m_haRole                   (role),
      m_internalIpAddress        (internalIpAddress),
      m_serialNumber             (serialNumber)
{
    setOwnerWaveNodeObjectId (getObjectId ());
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

WaveHaNode::WaveHaNode (WaveObjectManager *pWaveObjectManager, const WaveHaNodeRole &role)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (WaveHaNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (pWaveObjectManager),
      m_haRole                   (role),
      m_internalIpAddress        (""),
      m_serialNumber             ("")
{
    setOwnerWaveNodeObjectId (getObjectId ());
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

WaveHaNode::~WaveHaNode ()
{
}

string WaveHaNode::getClassName ()
{
    return ("WaveHaNode");
}

void WaveHaNode::setupAttributesForPersistence ()
{
    WaveLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeEnum       ((UI32 *) (&m_haRole),       "haRole"));
    addPersistableAttribute (new AttributeString     (&m_internalIpAddress,  "internalIpAddress"));
    addPersistableAttribute (new AttributeString     (&m_serialNumber,  "serialNumber"));
}

void WaveHaNode::setupAttributesForCreate ()
{
    WaveLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeEnum       ((UI32 *) (&m_haRole),       "haRole"));
    addPersistableAttributeForCreate (new AttributeString     (&m_internalIpAddress,  "internalIpAddress"));
    addPersistableAttributeForCreate (new AttributeString     (&m_serialNumber,  "serialNumber"));
}

string WaveHaNode::getInternalIpAddress () const
{
    return (m_internalIpAddress);
}

void WaveHaNode::setInternalIpAddress (const string &ipAddress)
{
    m_internalIpAddress = ipAddress;
}

string WaveHaNode::getSerialNumber () const
{
    return (m_serialNumber);
}

void WaveHaNode::setSerialNumber (const string &serialNumber)
{
    m_serialNumber = serialNumber;
}

WaveHaNodeRole WaveHaNode::getHaRole () const
{
    return (m_haRole);
}

void WaveHaNode::setHaRole (const WaveHaNodeRole &role)
{
    m_haRole = role;
}

}
