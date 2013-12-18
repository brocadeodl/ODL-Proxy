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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/CfgMemberManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    CfgManagedObject::CfgManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (CfgManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    CfgManagedObject::CfgManagedObject (ZoneObjectManager *pZoneObjectManager,const string &cfgName,const vector<WaveManagedObjectPointer<CfgMemberManagedObject> > &cfgMembers)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (CfgManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_cfgName    (cfgName),
        m_cfgMembers    (cfgMembers)
    {
    }

    CfgManagedObject::~CfgManagedObject ()
    {
    }

    string  CfgManagedObject::getClassName()
    {
        return ("CfgManagedObject");
    }

    void  CfgManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_cfgName,"cfgName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<CfgMemberManagedObject>(&m_cfgMembers,"cfgMembers", getClassName (), getObjectId (), CfgMemberManagedObject::getClassName ()));

	    setUserTagForAttribute ("cfgName",brocade_zone_cfg_name);
    }

    void  CfgManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_cfgName,"cfgName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<CfgMemberManagedObject>(&m_cfgMembers,"cfgMembers", getClassName (), getObjectId (), CfgMemberManagedObject::getClassName ()));

	    setDisableValidations ("cfgMembers", true);

	    vector<string> keyName;
	    keyName.push_back ("cfgName");
	    setUserDefinedKeyCombination (keyName);
    }

    void  CfgManagedObject::setCfgName(const string &cfgName)
    {
        m_cfgName  =  cfgName;
    }

    string  CfgManagedObject::getCfgName() const
    {
        return (m_cfgName);
    }

    void  CfgManagedObject::setCfgMembers(const vector<WaveManagedObjectPointer<CfgMemberManagedObject> > &cfgMembers)
    {
        m_cfgMembers  =  cfgMembers;
    }

    vector<WaveManagedObjectPointer<CfgMemberManagedObject> >  CfgManagedObject::getCfgMembers() const
    {
        return (m_cfgMembers);
    }

    void CfgManagedObject::addCfgMembers(CfgMemberManagedObject *cfgMember)
	{
//		ExtMacAclRuleManagedObject *extRule = dynamic_cast<ExtMacAclRuleManagedObject *>(ruleMO);
//		WaveManagedObjectPointer<ExtMacAclRuleManagedObject> eRule (extRule);
		m_cfgMembers.push_back(cfgMember);
		
	}

	int CfgManagedObject::removeCfgMembers(const string cfgEntry)
	{
		unsigned int i;

		for (i = 0; i < m_cfgMembers.size(); i++) {
			if (m_cfgMembers[i]->getCfgEntry() == cfgEntry) {
				trace(TRACE_LEVEL_INFO, "CfgManagedObject::removeCfgMembers: Found cfgEntry");
				m_cfgMembers.erase(m_cfgMembers.begin() + i);
				return (0);
			}
		}

		return (1);
	}

	int CfgManagedObject::numOfLeftCfgMembers(const string cfgEntry,
	    unsigned int *maxListIndex)
	{
		unsigned int i;
	    int num_of_left = 0;

	    if (maxListIndex) {
	        *maxListIndex = 0;
	    }

		for (i = 0; i < m_cfgMembers.size(); i++) {
			if (m_cfgMembers[i]->getCfgEntry() == cfgEntry) {
				trace(TRACE_LEVEL_INFO, "CfgManagedObject::removeCfgMembers: Found cfgEntry");
			} else {
	            if (maxListIndex) {
	                if (m_cfgMembers[i]->getListIndex() > *maxListIndex) {
	                    *maxListIndex = m_cfgMembers[i]->getListIndex();
	                }
	            }
	            num_of_left++;
	        }
		}

		return (num_of_left);
	}
}
