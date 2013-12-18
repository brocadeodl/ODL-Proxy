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

#include "ThresholdMonitor/Local/InterfaceMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "ThresholdMonitor/Local/InterfaceMonitorAreaLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "vcs.h"
namespace DcmNs
{
	InterfaceMonitorTypeLocalManagedObject::InterfaceMonitorTypeLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (InterfaceMonitorTypeLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		}
	InterfaceMonitorTypeLocalManagedObject::InterfaceMonitorTypeLocalManagedObject(ThresholdMonitorLocalObjectManager
			*pThresholdMonitorLocalObjectManager, const string  &policyName, const vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > &areaThresholdAlertMap)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (InterfaceMonitorTypeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager), 
		m_policyName    (policyName),
		m_areaThresholdAlertMap  (areaThresholdAlertMap)	
		{
		}
	InterfaceMonitorTypeLocalManagedObject::~InterfaceMonitorTypeLocalManagedObject ()
	{
	}

	string InterfaceMonitorTypeLocalManagedObject::getClassName()
	{
		return ("InterfaceMonitorTypeLocalManagedObject");
	}
	
	void InterfaceMonitorTypeLocalManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeStringUC(&m_policyName,true,"policyName", threshold_monitor_policy_name));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<InterfaceMonitorAreaLocalManagedObject>(&m_areaThresholdAlertMap,"areaThresholdAlertMap", getClassName (), getObjectId (), InterfaceMonitorAreaLocalManagedObject::getClassName ()));					
	}
	
	void InterfaceMonitorTypeLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new
				AttributeStringUC(&m_policyName,true,"policyName", threshold_monitor_policy_name));
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<InterfaceMonitorAreaLocalManagedObject>(&m_areaThresholdAlertMap,"areaThresholdAlertMap",getClassName (), getObjectId (), InterfaceMonitorAreaLocalManagedObject::getClassName ()));					

		 vector<string > keyName;
		 keyName.push_back ("policyName");
		 setUserDefinedKeyCombination (keyName);
	}
	void InterfaceMonitorTypeLocalManagedObject::setpolicyName(const string
			&policyName, bool userConfig)
	{
		m_policyName.setStringValue(policyName);
		m_policyName.setIsUserConfigured(userConfig);
	}
	string InterfaceMonitorTypeLocalManagedObject::getpolicyName() const
	{
		return (m_policyName.getStringValue());
	} 
	void InterfaceMonitorTypeLocalManagedObject::setAreaThresholdAlertMap(const vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > &areaThresholdAlertMap)
	{
		m_areaThresholdAlertMap = areaThresholdAlertMap;
	}
	vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > InterfaceMonitorTypeLocalManagedObject::getAreaThresholdAlertMap() const
	{
		return (m_areaThresholdAlertMap);
	}
	void InterfaceMonitorTypeLocalManagedObject::addAreaThreshAlertMap(InterfaceMonitorAreaLocalManagedObject *areaMap)
	{
		WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> areaThreshAlert (areaMap);
		m_areaThresholdAlertMap.push_back(areaThreshAlert);
	}
}
