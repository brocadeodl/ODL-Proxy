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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PoClassMapManagedObject.h"
#include "Policer/Global/PolicerPriorityMapManagedObject.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "vcs.h"

namespace DcmNs
{

    PolicyClassMapManagedObject::PolicyClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PolicyClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager)
    {
    }

    PolicyClassMapManagedObject::PolicyClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &policyClassMapName)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PolicyClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager),
        m_policyClassMapName    (policyClassMapName)
    {
    }

    PolicyClassMapManagedObject::~PolicyClassMapManagedObject ()
    {
    }

    string  PolicyClassMapManagedObject::getClassName()
    {
        return ("PolicyClassMapManagedObject");
    }

    void  PolicyClassMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyClassMapName,"policyClassMapName", policer_po_name));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<PoClassMapManagedObject>(&m_poManObj,"poManObj", getClassName (), getObjectId (), PoClassMapManagedObject::getClassName ()));
	}
    void  PolicyClassMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyClassMapName,"policyClassMapName",policer_po_name));
		addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PoClassMapManagedObject>(&m_poManObj,"poManObj", getClassName (), getObjectId (), PoClassMapManagedObject::getClassName ()));	    
		vector<string > keyName;
        keyName.push_back ("policyClassMapName");
		setUserDefinedKeyCombination (keyName);
    }


    void  PolicyClassMapManagedObject::setPolicyClassMapName(const string &policyClassMapName)
    {
        m_policyClassMapName  =  policyClassMapName;
    }

    string  PolicyClassMapManagedObject::getPolicyClassMapName() const
    {
        return (m_policyClassMapName);
    }

	void PolicyClassMapManagedObject::setPoManObj(const vector<WaveManagedObjectPointer<PoClassMapManagedObject> > &poManObj)
	{
		m_poManObj = poManObj;
	}

	vector<WaveManagedObjectPointer<PoClassMapManagedObject> > PolicyClassMapManagedObject::getPoManObj() const
	{
		return m_poManObj;
	}

	PoClassMapManagedObject *PolicyClassMapManagedObject::getPoManObjByObjectId(const ObjectId &classMapId)
	{
		unsigned int i;

        for (i = 0; i < m_poManObj.size(); i++) {
            if (m_poManObj[i]->getClassMapId() == classMapId ){
               return (m_poManObj[i].operator-> ());
			}
        }
		
		return NULL;
    }


	void PolicyClassMapManagedObject::addPoManObj(PoClassMapManagedObject *poManObj)
	{
		WaveManagedObjectPointer<PoClassMapManagedObject> poEntry (poManObj);
		m_poManObj.push_back(poEntry);
	}

	SI32 PolicyClassMapManagedObject::deletePoManObj(const ObjectId &classMapId)
	{
		unsigned int i;

        for (i = 0; i < m_poManObj.size(); i++) {
            if (m_poManObj[i]->getClassMapId() == classMapId ){
                trace(TRACE_LEVEL_INFO, string("poClass deleted"));
                m_poManObj.erase(m_poManObj.begin() + i);
                return (0);
            }
        }

        return (1);
	}
}
