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

#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    DscpToCosMapManagedObject::DscpToCosMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpToCosMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    DscpToCosMapManagedObject::DscpToCosMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &dscpToCosMapName,const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpToCosMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_dscpToCosMapName    (dscpToCosMapName),
        m_dscpMarkList    (dscpMarkList)
    {
    }

    DscpToCosMapManagedObject::~DscpToCosMapManagedObject ()
    {
    }

    string  DscpToCosMapManagedObject::getClassName()
    {
        return ("DscpToCosMapManagedObject");
    }

    void  DscpToCosMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_dscpToCosMapName,"dscpToCosMapName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<DscpMarkListManagedObject>(&m_dscpMarkList,"dscpMarkList", getClassName (), getObjectId (), DscpMarkListManagedObject::getClassName ()));

		setUserTagForAttribute("dscpToCosMapName", qos_dscp_cos_map_name);
		setUserTagForAttribute("dscpMarkList", qos_mark);
    }

    void  DscpToCosMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_dscpToCosMapName,"dscpToCosMapName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<DscpMarkListManagedObject>(&m_dscpMarkList,"dscpMarkList", getClassName (), getObjectId (), DscpMarkListManagedObject::getClassName ()));

        vector<string > keyName;
        keyName.push_back ("dscpToCosMapName");
        setUserDefinedKeyCombination (keyName);
    }

    void  DscpToCosMapManagedObject::setDscpToCosMapName(const string &dscpToCosMapName)
    {
        m_dscpToCosMapName  =  dscpToCosMapName;
    }

    string  DscpToCosMapManagedObject::getDscpToCosMapName() const
    {
        return (m_dscpToCosMapName);
    }

    void  DscpToCosMapManagedObject::setDscpMarkList(const vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > &dscpMarkList)
    {
        m_dscpMarkList  =  dscpMarkList;
    }

    vector<WaveManagedObjectPointer<DscpMarkListManagedObject> >  DscpToCosMapManagedObject::getDscpMarkList() const
    {
        return (m_dscpMarkList);
    }

	void DscpToCosMapManagedObject::addDscpMarkList(DscpMarkListManagedObject *pDscpMarkListManagedObject)
	{
		WaveManagedObjectPointer<DscpMarkListManagedObject> markListEntry(pDscpMarkListManagedObject);
		m_dscpMarkList.push_back(markListEntry);
	}

	SI32 DscpToCosMapManagedObject::delDscpMarkList(const string &markList)
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

	ObjectId DscpToCosMapManagedObject::getDscpMarkListObjectId(const string &markList)
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
