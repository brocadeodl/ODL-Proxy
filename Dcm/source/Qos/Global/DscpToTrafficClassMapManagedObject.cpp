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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nbellari                                                     *
 **************************************************************************/

#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    DscpToTrafficClassMapManagedObject::DscpToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpToTrafficClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    DscpToTrafficClassMapManagedObject::DscpToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &dscpToTrafficClassMapName,const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpToTrafficClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_dscpToTrafficClassMapName    (dscpToTrafficClassMapName),
        m_dscpMarkList    (dscpMarkList)
    {
    }

    DscpToTrafficClassMapManagedObject::~DscpToTrafficClassMapManagedObject ()
    {
    }

    string  DscpToTrafficClassMapManagedObject::getClassName()
    {
        return ("DscpToTrafficClassMapManagedObject");
    }

    void  DscpToTrafficClassMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<DscpMarkListManagedObject>(&m_dscpMarkList,"dscpMarkList", getClassName (), getObjectId (), DscpMarkListManagedObject::getClassName ()));

		setUserTagForAttribute("dscpToTrafficClassMapName", qos_dscp_traffic_class_map_name);
		setUserTagForAttribute("dscpMarkList", qos_mark);
    }

    void  DscpToTrafficClassMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<DscpMarkListManagedObject>(&m_dscpMarkList,"dscpMarkList", getClassName (), getObjectId (), DscpMarkListManagedObject::getClassName ()));

        vector<string > keyName;
        keyName.push_back ("dscpToTrafficClassMapName");
        setUserDefinedKeyCombination (keyName);
    }

    void  DscpToTrafficClassMapManagedObject::setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  DscpToTrafficClassMapManagedObject::getDscpToTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

    void  DscpToTrafficClassMapManagedObject::setDscpMarkList(const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList)
    {
        m_dscpMarkList  =  dscpMarkList;
    }

    vector<WaveManagedObjectPointer<DscpMarkListManagedObject> >  DscpToTrafficClassMapManagedObject::getDscpMarkList() const
    {
        return (m_dscpMarkList);
    }

	void DscpToTrafficClassMapManagedObject::addDscpMarkList(DscpMarkListManagedObject *pDscpMarkListManagedObject)
	{
		WaveManagedObjectPointer<DscpMarkListManagedObject> markListEntry(pDscpMarkListManagedObject);
		m_dscpMarkList.push_back(markListEntry);
	}

	SI32 DscpToTrafficClassMapManagedObject::delDscpMarkList(const string &markList)
	{
		uint32_t i;
		
		for (i=0; i<m_dscpMarkList.size(); i++) {
			if (m_dscpMarkList[i]->getMarkList() == markList) {
				trace(TRACE_LEVEL_INFO, string("markList ") + markList + string(" deleted"));
				m_dscpMarkList.erase(m_dscpMarkList.begin() + i);
				return (0);
			}
		}

		return (1);
	}

	ObjectId DscpToTrafficClassMapManagedObject::getDscpMarkListObjectId(const string &markList)
	{
		uint32_t i;
		
		for (i=0; i<m_dscpMarkList.size(); i++) {
			if (m_dscpMarkList[i]->getMarkList() == markList) {
				trace(TRACE_LEVEL_INFO, string("markList ") + markList + string(" object id"));
				return (m_dscpMarkList[i]->getObjectId());
			}
		}

		return (ObjectId::NullObjectId);
	}
}
