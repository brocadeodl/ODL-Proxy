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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
//#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
//#include "Framework/Attributes/AttributeManagedObjectComposition.h"
//#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "vcs.h"

namespace DcmNs
{

    PriorityGroupManagedObject::PriorityGroupManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (PriorityGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    PriorityGroupManagedObject::PriorityGroupManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &pgId,const SI32 &weight,const SI32 &pfc, const string &ceeMapName)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (PriorityGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_pgId    (pgId),
        m_weight    (weight),
        m_pfc    (pfc),
		m_ceeMapName	(ceeMapName)
    {
    }

    PriorityGroupManagedObject::~PriorityGroupManagedObject ()
    {
    }

    string  PriorityGroupManagedObject::getClassName()
    {
        return ("PriorityGroupManagedObject");
    }

    void  PriorityGroupManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_pgId,"pgId"));
        addPersistableAttribute (new AttributeSI32(&m_weight,"weight"));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_pfc),"pfc"));
        addPersistableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));

        setUserTagForAttribute("pgId", brocade_cee_map_PGID);
        setUserTagForAttribute("weight", brocade_cee_map_weight);
        setUserTagForAttribute("pfc", brocade_cee_map_pfc);
        setUserTagForAttribute("ceeMapName", brocade_cee_map_cee_map);
    }

    void  PriorityGroupManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_pgId,"pgId"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_weight,"weight"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_pfc),"pfc"));
        addPersistableAttributeForCreate  (new AttributeString(&m_ceeMapName,"ceeMapName"));

        vector<string > keyName;
        keyName.push_back ("pgId");
        setUserDefinedKeyCombination (keyName);
    }

    void  PriorityGroupManagedObject::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  PriorityGroupManagedObject::getPgId() const
    {
        return (m_pgId);
    }

    void  PriorityGroupManagedObject::setWeight(const SI32 &weight)
    {
        m_weight  =  weight;
    }

    SI32  PriorityGroupManagedObject::getWeight() const
    {
        return (m_weight);
    }

    void  PriorityGroupManagedObject::setPfc(const SI32 &pfc)
    {
        m_pfc  =  pfc;
    }

    SI32  PriorityGroupManagedObject::getPfc() const
    {
        return (m_pfc);
    }

	void  PriorityGroupManagedObject::setCeeMapName(const string &ceeMapName)
	{
		m_ceeMapName = ceeMapName;
	}

    string  PriorityGroupManagedObject::getCeeMapName() const
    {
        return (m_ceeMapName);
    }


}
