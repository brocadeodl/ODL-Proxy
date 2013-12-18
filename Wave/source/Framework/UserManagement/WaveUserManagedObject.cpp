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

#include "Framework/UserManagement/WaveUserManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/UserManagement/WaveGroupManagedObject.h"

namespace WaveNs
{

WaveUserManagedObject::WaveUserManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement           (pWaveObjectManager),
      PrismPersistableObject (WaveUserManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject      (pWaveObjectManager),
      m_userId               (0),
      m_isActive             (false)
{
}

WaveUserManagedObject::WaveUserManagedObject (WaveObjectManager *pWaveObjectManager, const string &name)
    : PrismElement           (pWaveObjectManager),
      PrismPersistableObject (WaveUserManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject      (pWaveObjectManager),
      m_userId               (0),
      m_isActive             (false)
{
    setName (name);
}

WaveUserManagedObject::~WaveUserManagedObject ()
{
}

string WaveUserManagedObject::getClassName ()
{
    return ("WaveUserManagedObject");
}

void WaveUserManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32                      (&m_userId,                   "userId"));
    addPersistableAttribute (new AttributeObjectIdAssociation       (&m_groupObjectId,            "groupObjectId",            WaveGroupManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_associatedGroupObjectIds, "associatedGroupObjectIds", getClassName (), getObjectId (), WaveGroupManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeString                    (&m_encryptedPassword,        "encryptedPassword"));
    addPersistableAttribute (new AttributeBool                      (&m_isActive,                 "isActive"));
    addPersistableAttribute (new AttributeString                    (&m_homeDirectory,            "homeDirectory"));
}

void WaveUserManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32                      (&m_userId,                   "userId"));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation       (&m_groupObjectId,            "groupObjectId",            WaveGroupManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_associatedGroupObjectIds, "associatedGroupObjectIds", getClassName (), getObjectId (), WaveGroupManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeString                    (&m_encryptedPassword,        "encryptedPassword"));
    addPersistableAttributeForCreate (new AttributeBool                      (&m_isActive,                 "isActive"));
    addPersistableAttributeForCreate (new AttributeString                    (&m_homeDirectory,            "homeDirectory"));
}

UI32 WaveUserManagedObject::getUserId () const
{
    return (m_userId);
}

void WaveUserManagedObject::setUserId (const UI32 &userId)
{
    m_userId = userId;
}

ObjectId WaveUserManagedObject::getGroupObjectId () const
{
    return (m_groupObjectId);
}

void WaveUserManagedObject::setGroupObjectId (const ObjectId &groupObjectId)
{
    m_groupObjectId = groupObjectId;
}

string WaveUserManagedObject::getEncryptedPassword () const
{
    return (m_encryptedPassword);
}

void WaveUserManagedObject::setEncryptedPassword (const string &encryptedPassword)
{
    m_encryptedPassword = encryptedPassword;
}

bool WaveUserManagedObject::getIsActive () const
{
    return (m_isActive);
}

void WaveUserManagedObject::setIsActive (const bool &isActive)
{
    m_isActive = isActive;
}

string WaveUserManagedObject::getHomeDirectory () const
{
    return (m_homeDirectory);
}

void WaveUserManagedObject::setHomeDirectory (const string &homeDirectory)
{
    m_homeDirectory = homeDirectory;
}

}
