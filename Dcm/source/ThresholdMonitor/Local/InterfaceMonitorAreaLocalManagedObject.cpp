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
 *   Author : Priya Ahuja												   *
 **************************************************************************/

#include "ThresholdMonitor/Local/InterfaceMonitorAreaLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"
namespace DcmNs
{
	InterfaceMonitorAreaLocalManagedObject::InterfaceMonitorAreaLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (InterfaceMonitorAreaLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		}
	InterfaceMonitorAreaLocalManagedObject::InterfaceMonitorAreaLocalManagedObject(ThresholdMonitorLocalObjectManager
			*pThresholdMonitorLocalObjectManager, const UI32
			&interfacetypename, const UI32 &areaname, const
			SI32 &highthresh, const SI32 &lowthresh, const SI32 &buffer, const
			BitMap &abovehighthreshaction,const BitMap &abovelowthreshaction,
			const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction, const UI32 &timebase)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (InterfaceMonitorAreaLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager), 
		m_interfacetypename  (interfacetypename),
		m_areaname	(areaname),
		m_highthresh (highthresh),
		m_lowthresh  (lowthresh),
		m_buffer  (buffer),
		m_abovehighthreshaction  (abovehighthreshaction),
		m_abovelowthreshaction   (abovelowthreshaction),
		m_belowhighthreshaction  (belowhighthreshaction),
		m_belowlowthreshaction  (belowlowthreshaction),
		m_timebase (timebase)
		{
		}
	InterfaceMonitorAreaLocalManagedObject::~InterfaceMonitorAreaLocalManagedObject ()
	{
	}

	string InterfaceMonitorAreaLocalManagedObject::getClassName()
	{
		return ("InterfaceMonitorAreaLocalManagedObject");
	}
	
	void InterfaceMonitorAreaLocalManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeEnumUC(&m_interfacetypename,true,"interfacetypename"));
		addPersistableAttribute (new AttributeEnumUC(&m_areaname,true,"areaname"));
		addPersistableAttribute (new AttributeSI32(&m_highthresh,"highthresh",threshold_monitor_high_threshold));
		addPersistableAttribute (new AttributeSI32(&m_lowthresh,"lowthresh",threshold_monitor_low_threshold));
		addPersistableAttribute (new AttributeSI32(&m_buffer,"buffer",threshold_monitor_buffer));	
		addPersistableAttribute (new AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction",threshold_monitor_above_highthresh_action));
		addPersistableAttribute (new AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction",threshold_monitor_above_lowthresh_action));
		addPersistableAttribute (new AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction",threshold_monitor_below_highthresh_action));
		addPersistableAttribute (new AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction",threshold_monitor_below_lowthresh_action));
		addPersistableAttribute (new AttributeEnum(&m_timebase,"timebase",threshold_monitor_timebase_value));
		setUserTagForAttribute ("interfacetypename", threshold_monitor_type);
		setUserTagForAttribute ("areaname", threshold_monitor_area_value);
	}
	
	void InterfaceMonitorAreaLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new
				AttributeEnumUC(&m_interfacetypename,true,"interfacetypename"));
		addPersistableAttributeForCreate (new AttributeEnumUC(&m_areaname,true,"areaname"));
		addPersistableAttributeForCreate (new AttributeSI32(&m_highthresh,"highthresh",threshold_monitor_high_threshold));
		addPersistableAttributeForCreate (new AttributeSI32(&m_lowthresh,"lowthresh",threshold_monitor_low_threshold));
		addPersistableAttributeForCreate (new AttributeSI32(&m_buffer,"buffer",threshold_monitor_buffer));	
	
		addPersistableAttributeForCreate (new AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction",threshold_monitor_above_highthresh_action));
		addPersistableAttributeForCreate (new AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction",threshold_monitor_above_lowthresh_action));		
		addPersistableAttributeForCreate (new AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction",threshold_monitor_below_highthresh_action));
		addPersistableAttributeForCreate (new AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction",threshold_monitor_below_lowthresh_action));
		addPersistableAttributeForCreate (new AttributeEnum(&m_timebase,"timebase",threshold_monitor_timebase_value));
	}
	void InterfaceMonitorAreaLocalManagedObject::setupKeys()
	{
		vector<string > keyName;
		keyName.push_back ("interfacetypename");
		keyName.push_back ("areaname");
		setUserDefinedKeyCombination (keyName);
	}
	void InterfaceMonitorAreaLocalManagedObject::setInterfacetypeName(const
			UI32 &interfacetypename, bool userConfig)
	{
		m_interfacetypename.setUI32Value(interfacetypename);
		m_interfacetypename.setIsUserConfigured(userConfig);
	}
	UI32 InterfaceMonitorAreaLocalManagedObject::getInterfacetypeName() const
	{
		return (m_interfacetypename.getUI32Value());
	}

	void InterfaceMonitorAreaLocalManagedObject::setAreaName(const UI32
			&areaname, bool userConfig)
	{
		m_areaname.setUI32Value(areaname);
		m_areaname.setIsUserConfigured(userConfig);
	}
	UI32 InterfaceMonitorAreaLocalManagedObject::getAreaName() const
	{
		return (m_areaname.getUI32Value());
	}
	void InterfaceMonitorAreaLocalManagedObject::setHighThresh(const SI32 &highthresh)
	{
		m_highthresh = highthresh;
	}
	SI32 InterfaceMonitorAreaLocalManagedObject::getHighThresh() const
	{
		return (m_highthresh);
	}

	void InterfaceMonitorAreaLocalManagedObject::setLowThresh(const SI32 &lowthresh)
	{
		m_lowthresh = lowthresh;
	}
	SI32 InterfaceMonitorAreaLocalManagedObject::getLowThresh() const
	{
		return (m_lowthresh);
	}

	void InterfaceMonitorAreaLocalManagedObject::setBuffer(const SI32 &buffer)
	{
		m_buffer = buffer;
	}
	SI32 InterfaceMonitorAreaLocalManagedObject::getBuffer() const
	{
		return (m_buffer);
	}
	void InterfaceMonitorAreaLocalManagedObject::setAboveHighThreshAction(const
			BitMap &abovehighthreshaction)
	{
		m_abovehighthreshaction = abovehighthreshaction;
	}
	BitMap InterfaceMonitorAreaLocalManagedObject::getAboveHighThreshAction() const
	{
		return (m_abovehighthreshaction);
	}
        void InterfaceMonitorAreaLocalManagedObject::setAboveLowThreshAction(const
			BitMap &abovelowthreshaction)
	{
		m_abovelowthreshaction = abovelowthreshaction;
	}
	BitMap InterfaceMonitorAreaLocalManagedObject::getAboveLowThreshAction() const
	{
		return (m_abovelowthreshaction);
	}
	void InterfaceMonitorAreaLocalManagedObject::setBelowHighThreshAction(const
			BitMap &belowhighthreshaction)
	{
		m_belowhighthreshaction = belowhighthreshaction;
	}
	BitMap InterfaceMonitorAreaLocalManagedObject::getBelowHighThreshAction() const
	{
		return (m_belowhighthreshaction);	
	}
	void InterfaceMonitorAreaLocalManagedObject::setBelowLowThreshAction(const
			BitMap &belowlowthreshaction)
	{
		m_belowlowthreshaction = belowlowthreshaction;
	}
	BitMap InterfaceMonitorAreaLocalManagedObject::getBelowLowThreshAction() const
	{
		return (m_belowlowthreshaction);
	}
	void InterfaceMonitorAreaLocalManagedObject::setTimeBase(const UI32 &timebase)
	{
		m_timebase = timebase;
	}
	UI32 InterfaceMonitorAreaLocalManagedObject::getTimeBase() const
	{
		return (m_timebase);
	}

}
