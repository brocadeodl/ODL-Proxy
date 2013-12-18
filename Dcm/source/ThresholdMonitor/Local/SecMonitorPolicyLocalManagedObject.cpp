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
 *   Author : Ramya Rangarajan											   *
 **************************************************************************/

#include "ThresholdMonitor/Local/SecMonitorPolicyLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "ThresholdMonitor/Local/SecLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "vcs.h"
namespace DcmNs
{
	SecMonitorPolicyLocalManagedObject::SecMonitorPolicyLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecMonitorPolicyLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		}
	SecMonitorPolicyLocalManagedObject::SecMonitorPolicyLocalManagedObject(ThresholdMonitorLocalObjectManager
			*pThresholdMonitorLocalObjectManager, const string &policyName, const vector<WaveManagedObjectPointer<SecLocalManagedObject> > &areaThresholdAlertMap)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecMonitorPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager), 
		m_policyName    (policyName),
		m_areaThresholdAlertMap  (areaThresholdAlertMap)	
		{
		}
	SecMonitorPolicyLocalManagedObject::~SecMonitorPolicyLocalManagedObject ()
	{
	}

	string SecMonitorPolicyLocalManagedObject::getClassName()
	{
		return ("SecMonitorPolicyLocalManagedObject");
	}
	
	void SecMonitorPolicyLocalManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new
				AttributeStringUC(&m_policyName,true,"policyName",
					threshold_monitor_sec_policy_name));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<SecLocalManagedObject>(&m_areaThresholdAlertMap,"areaThresholdAlertMap",
getClassName (), getObjectId (), SecLocalManagedObject::getClassName ()));					
	}
	
	void SecMonitorPolicyLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new
				AttributeStringUC(&m_policyName,true,"policyName",
					threshold_monitor_sec_policy_name));
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<SecLocalManagedObject>(&m_areaThresholdAlertMap,"areaThresholdAlertMap",getClassName (), getObjectId (), SecLocalManagedObject::getClassName ()));					

		 vector<string > keyName;
		 keyName.push_back ("policyName");
		 setUserDefinedKeyCombination (keyName);
	}
	void SecMonitorPolicyLocalManagedObject::setpolicyName(const string
			&policyName, bool userConfig)
	{
		m_policyName.setStringValue(policyName);
		m_policyName.setIsUserConfigured(userConfig);
	}
	string SecMonitorPolicyLocalManagedObject::getpolicyName() const
	{
		return (m_policyName.getStringValue());
	} 
	void SecMonitorPolicyLocalManagedObject::setAreaThresholdAlertMap(const vector<WaveManagedObjectPointer<SecLocalManagedObject> > &areaThresholdAlertMap)
	{
		m_areaThresholdAlertMap = areaThresholdAlertMap;
	}
	vector<WaveManagedObjectPointer<SecLocalManagedObject> > SecMonitorPolicyLocalManagedObject::getAreaThresholdAlertMap() const
	{
		return (m_areaThresholdAlertMap);
	}
	void SecMonitorPolicyLocalManagedObject::addAreaThreshAlertMap(SecLocalManagedObject *areaMap)
	{
		WaveManagedObjectPointer<SecLocalManagedObject> areaThreshAlert (areaMap);
		m_areaThresholdAlertMap.push_back(areaThreshAlert);
	}
}
