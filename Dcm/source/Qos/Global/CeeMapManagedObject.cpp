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

#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    CeeMapManagedObject::CeeMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CeeMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    CeeMapManagedObject::CeeMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &ceeMapName,const SI32 &precedence,const vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &priorityGroup,const WaveManagedObjectPointer<PriorityMapManagedObject> &priorityMap)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CeeMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_ceeMapName    (ceeMapName),
        m_precedence    (precedence),
        m_priorityGroup    (priorityGroup),
        m_priorityMap    (priorityMap)
    {
    }

    CeeMapManagedObject::~CeeMapManagedObject ()
    {
    }

    string  CeeMapManagedObject::getClassName()
    {
        return ("CeeMapManagedObject");
    }

    void  CeeMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addPersistableAttribute (new AttributeSI32(&m_precedence,"precedence"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<PriorityGroupManagedObject>(&m_priorityGroup,"priorityGroup", getClassName (), getObjectId (), PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectComposition<PriorityMapManagedObject>(&m_priorityMap,"priorityMap", PriorityMapManagedObject::getClassName (), true));
		addPersistableAttribute (new AttributeSI32(&m_remapFabricPriority, "remapFabricPriority"));
        addPersistableAttribute (new AttributeSI32(&m_remapLosslessPriority, "remapLosslessPriority"));

        setUserTagForAttribute("ceeMapName", qos_name);
		setUserTagForAttribute("precedence", brocade_cee_map_precedence);
        setUserTagForAttribute("priorityGroup", brocade_cee_map_priority_group_table);
        setUserTagForAttribute("priorityMap", brocade_cee_map_priority_table);
		setUserTagForAttribute("remapFabricPriority", brocade_cee_map_fabric_remapped_priority);
		setUserTagForAttribute("remapLosslessPriority",brocade_cee_map_lossless_remapped_priority);
    }

    void  CeeMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_precedence,"precedence"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PriorityGroupManagedObject>(&m_priorityGroup,"priorityGroup", getClassName (), getObjectId (), PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<PriorityMapManagedObject>(&m_priorityMap,"priorityMap", PriorityMapManagedObject::getClassName (), true));
		addPersistableAttributeForCreate (new AttributeSI32(&m_remapFabricPriority, "remapFabricPriority"));
		addPersistableAttributeForCreate (new AttributeSI32(&m_remapLosslessPriority, "remapLosslessPriority"));

        vector<string > keyName;
        keyName.push_back ("ceeMapName");
        setUserDefinedKeyCombination (keyName);
    }

    void  CeeMapManagedObject::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  CeeMapManagedObject::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  CeeMapManagedObject::setPrecedence(const SI32 &precedence)
    {
        m_precedence  =  precedence;
    }

    SI32  CeeMapManagedObject::getPrecedence() const
    {
        return (m_precedence);
    }

    void  CeeMapManagedObject::setPriorityGroup(const vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &priorityGroup)
    {
        m_priorityGroup  =  priorityGroup;
    }

    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> >  CeeMapManagedObject::getPriorityGroup() const
    {
        return (m_priorityGroup);
    }

    void  CeeMapManagedObject::setPriorityMap(const WaveManagedObjectPointer<PriorityMapManagedObject> &priorityMap)
    {
        m_priorityMap  =  priorityMap;
    }

    WaveManagedObjectPointer<PriorityMapManagedObject>  CeeMapManagedObject::getPriorityMap() const
    {
        return (m_priorityMap);
    }

    void  CeeMapManagedObject::setRemapFabricPriority(const SI32 &remapFabricPriority)
    {
        m_remapFabricPriority  =  remapFabricPriority;
    }

    SI32  CeeMapManagedObject::getRemapFabricPriority() const
    {
        return (m_remapFabricPriority);
    }

    void  CeeMapManagedObject::setRemapLosslessPriority(const SI32 &remapLosslessPriority)
    {
        m_remapLosslessPriority  =  remapLosslessPriority;
    }

    SI32  CeeMapManagedObject::getRemapLosslessPriority() const
    {
        return (m_remapLosslessPriority);
    }

	int CeeMapManagedObject::delPriorityGroup(const string &pgid) 
	{
		unsigned int i=0;

		for (i = 0; i < m_priorityGroup.size(); i++) {
			if (m_priorityGroup[i]->getPgId() == pgid) {
				trace(TRACE_LEVEL_INFO, string("CeeMapManagedObject::delPriorityGroup: Found PGID ") + pgid);
				m_priorityGroup.erase(m_priorityGroup.begin() + i);
				return (0);
			}
		}
		return (1);
	}

	void CeeMapManagedObject::addPriorityGroup(PriorityGroupManagedObject *pPriorityGroupManagedObject)
	{
		WaveManagedObjectPointer<PriorityGroupManagedObject> p(pPriorityGroupManagedObject);
		m_priorityGroup.push_back(p);
	}

	void CeeMapManagedObject::updatePriorityMap(const int &cos, const ObjectId &PriorityGroup)
	{
		switch (cos) {
		case 0:
			m_priorityMap->setCos0ToPriorityGrouMap(PriorityGroup);
			break;
        case 1:
            m_priorityMap->setCos1ToPriorityGrouMap(PriorityGroup);
            break;
        case 2:
            m_priorityMap->setCos2ToPriorityGrouMap(PriorityGroup);
            break;
        case 3:
            m_priorityMap->setCos3ToPriorityGrouMap(PriorityGroup);
            break;
        case 4:
            m_priorityMap->setCos4ToPriorityGrouMap(PriorityGroup);
            break;
        case 5:
            m_priorityMap->setCos5ToPriorityGrouMap(PriorityGroup);
            break;
        case 6:
            m_priorityMap->setCos6ToPriorityGrouMap(PriorityGroup);
            break;
        case 7:
            m_priorityMap->setCos7ToPriorityGrouMap(PriorityGroup);
            break;
		default:
			break;
		}
	}

	WaveManagedObjectPointer<PriorityGroupManagedObject>* CeeMapManagedObject::findPriorityGroupByName(const string &pgId)
	{
        UI32 i;
        for (i = 0; i < m_priorityGroup.size(); i++) {
            if (m_priorityGroup[i]->getPgId() == pgId)
                return (&(m_priorityGroup[i]));
        }
        return (NULL);
    }
}
