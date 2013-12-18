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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"

namespace DcmNs
{

VcsClusterManagedObject::VcsClusterManagedObject (VcsFabObjectManager *pVcsFabObjectManager)
    : PrismElement                      (pVcsFabObjectManager),
      PrismPersistableObject            (VcsClusterManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject                 (pVcsFabObjectManager),
      DcmManagedObject                  (pVcsFabObjectManager),
      m_vcsClusterId                    (0),
      m_principalSwitchDomainId         (0)
{
    setClusterFormationTime ();
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
	clearClusterGUID ();
}

VcsClusterManagedObject::VcsClusterManagedObject (VcsFabObjectManager *pVcsFabObjectManager, const UI32 &vcsClusterId)
    : PrismElement                      (pVcsFabObjectManager),
      PrismPersistableObject            (VcsClusterManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject                 (pVcsFabObjectManager),
      DcmManagedObject                  (pVcsFabObjectManager),
      m_vcsClusterId                    (vcsClusterId),
      m_principalSwitchDomainId         (0)
{
    setClusterFormationTime ();
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
	clearClusterGUID ();
}

VcsClusterManagedObject::~VcsClusterManagedObject ()
{
}

void VcsClusterManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_switchObjectIds, "switchObjectIds", getClassName (), getObjectId (), VcsNodeLocalManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeUI32 (&m_vcsClusterId, "vcsClusterId"));
    addPersistableAttribute (new AttributeString (&m_licenseString, "licenseString"));
    addPersistableAttribute (new AttributeObjectId (&m_principalSwitchObjectId, "principalSwitchObjectId"));
    addPersistableAttribute (new AttributeUI32 (&m_principalSwitchDomainId, "principalSwitchDomainId"));
    addPersistableAttribute (new AttributeString (&m_clusterFormationTimeStamp, "clusterFormationTime"));
    addPersistableAttribute (new AttributeUUID (&m_clusterGUID, "clusterGUID", 0, false));
//	time_t           m_clusterFormationTimeStamp;
}

void VcsClusterManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_switchObjectIds, "switchObjectIds", getClassName (), getObjectId (), VcsNodeLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_vcsClusterId, "vcsClusterId"));
    addPersistableAttributeForCreate (new AttributeString (&m_licenseString, "licenseString"));
    addPersistableAttributeForCreate (new AttributeObjectId (&m_principalSwitchObjectId, "principalSwitchObjectId"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_principalSwitchDomainId, "principalSwitchDomainId"));
    addPersistableAttributeForCreate (new AttributeString (&m_clusterFormationTimeStamp, "clusterFormationTime"));
    addPersistableAttributeForCreate (new AttributeUUID (&m_clusterGUID, "clusterGUID", 0, false));
}

string VcsClusterManagedObject::getClassName ()
{
    return ("VcsClusterManagedObject");
}

void VcsClusterManagedObject::addSwitchObjectId  (const ObjectId &switchObjectId)
{
	m_switchObjectIds.push_back(switchObjectId);
}

UI32 VcsClusterManagedObject::removeSwitchObjectId (const ObjectId switchObjectId)
{
    UI32 i = 0;
	UI32 nSwitches = m_switchObjectIds.size ();

	if (0 == nSwitches)
	{
	    return 	NO_SWITCHES_IN_VCS_CLUSTER;
	}
 
    for (i = 0; i < nSwitches; i++)
    {
       if (switchObjectId == m_switchObjectIds[i])   
       {
           m_switchObjectIds.erase(m_switchObjectIds.begin()+i);

		   break;
       }
    }
    if (i < nSwitches)
	{
	    return VCS_SUCCESS;
	}
	else
	{
	    return SWITCH_NOT_IN_VCS_MANAGED_OBJECT_DB;
	}
}

vector<ObjectId>  VcsClusterManagedObject::getSwitchObjectIds  () const
{
    return m_switchObjectIds;
}

UI32 VcsClusterManagedObject::getVcsClusterId               ()
{
	return m_vcsClusterId;
}

void VcsClusterManagedObject::setVcsClusterId  (const UI32 vcsClusterId)
{
	m_vcsClusterId = vcsClusterId;
}

string  VcsClusterManagedObject::getClusterName               ()
{
	return getName();
}

void VcsClusterManagedObject::setClusterName  (const string clusterName)
{
	setName   (clusterName);
}

ObjectId  VcsClusterManagedObject::getPrincipalSwitchObjectId       ()
{
	return m_principalSwitchObjectId;
}

void VcsClusterManagedObject::setPrincipalSwitchObjectId       (const ObjectId switchObjectId)
{
	m_principalSwitchObjectId = switchObjectId;
}

UI32 VcsClusterManagedObject::getPrincipalSwitchDomainId ()
{
    return m_principalSwitchDomainId;
}

void VcsClusterManagedObject::setPrincipalSwitchDomainId (UI32 domainId)
{
    m_principalSwitchDomainId = domainId;
}

UI32 VcsClusterManagedObject::getSwitchCount()
{
	return m_switchObjectIds.size ();
}

bool VcsClusterManagedObject::isObjectIdInClusterMO	(const ObjectId switchObjectId)
{
    UI32 objCount = m_switchObjectIds.size ();
	UI32 i;
	for (i = 0; i < objCount; i++)
	{
		if (m_switchObjectIds[i] == switchObjectId) return true;
	}
	return false;
}

void VcsClusterManagedObject::setClusterFormationTime ()
{
    DateTime *pDateTime = new DateTime ();
    prismAssert (NULL != pDateTime, __FILE__, __LINE__);
    m_clusterFormationTimeStamp = pDateTime->toString ();
    delete pDateTime;
}

string VcsClusterManagedObject::getClusterFormationTime ()
{
    return m_clusterFormationTimeStamp;
}

void VcsClusterManagedObject::setClusterGUID (const Uuid &clusterGUID)
{
	m_clusterGUID = clusterGUID;
}

Uuid VcsClusterManagedObject::getClusterGUID ()
{
	return m_clusterGUID;
}

void VcsClusterManagedObject::clearClusterGUID ()
{
	m_clusterGUID.clear();
}

}
