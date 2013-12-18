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

#include "ThresholdMonitor/Local/SecConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"
namespace DcmNs
{
	SecConfigLocalManagedObject::SecConfigLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecConfigLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		}
	SecConfigLocalManagedObject::SecConfigLocalManagedObject(ThresholdMonitorLocalObjectManager
			*pThresholdMonitorLocalObjectManager, const string &applyvalue,const bool &pause)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager), 
		m_applyvalue	(applyvalue),
		m_pause	(pause)
		{
		}
	SecConfigLocalManagedObject::~SecConfigLocalManagedObject ()
	{
	}

	string SecConfigLocalManagedObject::getClassName()
	{
		return ("SecConfigLocalManagedObject");
	}
	
	void SecConfigLocalManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeStringUC(&m_applyvalue,true,"applyvalue",threshold_monitor_apply));
		addPersistableAttribute (new AttributeBool(&m_pause,"pause",threshold_monitor_pause));
	}
	
	void SecConfigLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeStringUC(&m_applyvalue,true,"applyvalue",threshold_monitor_apply));
		addPersistableAttributeForCreate (new AttributeBool(&m_pause,"pause",threshold_monitor_pause));
	}
	void SecConfigLocalManagedObject::setApplyValue(const string &applyvalue, bool userConfig)
	{
		m_applyvalue.setStringValue(applyvalue);
		m_applyvalue.setIsUserConfigured(userConfig);
	}
	string SecConfigLocalManagedObject::getApplyValue() const
	{
		return (m_applyvalue.getStringValue());
	}
	void SecConfigLocalManagedObject::setPause(const bool &pause)
	{
		m_pause	= pause;
	}
	bool SecConfigLocalManagedObject::getPause()	const
	{
		return (m_pause);
	}
}
