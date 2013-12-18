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

/************************************************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          	*
 *   All rights reserved.                                               *
 *   Author : Sachin Mishra			   			*
 ************************************************************************/

#include "ThresholdMonitor/Local/SecLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"
namespace DcmNs
{
	SecLocalManagedObject::SecLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		}

	SecLocalManagedObject::SecLocalManagedObject(ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager,
			const UI32 &areaname, const UI32 &timebase,
			const SI32 &highthresh, const SI32 &lowthresh, const SI32 &buffer,
			const BitMap &abovehighthreshaction, const BitMap &abovelowthreshaction,
			const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SecLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager),
		m_areaname      (areaname),
		m_timebase (timebase),
		m_highthresh (highthresh),
		m_lowthresh  (lowthresh),
		m_buffer (buffer),
		m_abovehighthreshaction  (abovehighthreshaction),
		m_abovelowthreshaction   (abovelowthreshaction),
		m_belowhighthreshaction  (belowhighthreshaction),
		m_belowlowthreshaction   (belowlowthreshaction)
		{
		}

	SecLocalManagedObject::~SecLocalManagedObject ()
	{
	}

	string SecLocalManagedObject::getClassName()
	{
		return ("SecLocalManagedObject");
	}

	/* TR000401639 : Removing support for above lowthresh-action, maintaining as dummy field for future */	
	void SecLocalManagedObject::setupAttributesForPersistence()
	{
                DcmLocalManagedObject::setupAttributesForPersistence ();
                addPersistableAttribute (new AttributeEnumUC(&m_areaname,true,"areaname"));
                addPersistableAttribute (new AttributeEnum(&m_timebase, "timebase", threshold_monitor_timebase));
                addPersistableAttribute (new AttributeSI32(&m_highthresh,"highthresh", threshold_monitor_sec_high_threshold));
                addPersistableAttribute (new AttributeSI32(&m_lowthresh,"lowthresh", threshold_monitor_sec_low_threshold));
                addPersistableAttribute (new AttributeSI32(&m_buffer,"buffer", threshold_monitor_sec_buffer));
                addPersistableAttribute (new AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction", threshold_monitor_sec_above_highthresh_action));
                addPersistableAttribute (new AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction"));
                addPersistableAttribute (new AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction", threshold_monitor_sec_below_highthresh_action));
                addPersistableAttribute (new AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction", threshold_monitor_sec_below_lowthresh_action));
				setUserTagForAttribute ("areaname",	threshold_monitor_sec_area_value);
	}
	
	void SecLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
                addPersistableAttributeForCreate (new
						AttributeEnumUC(&m_areaname, true, "areaname"));
                addPersistableAttributeForCreate (new AttributeEnum(&m_timebase, "timebase", threshold_monitor_timebase));
                addPersistableAttributeForCreate (new AttributeSI32(&m_highthresh,"highthresh", threshold_monitor_sec_high_threshold));
                addPersistableAttributeForCreate (new AttributeSI32(&m_lowthresh,"lowthresh", threshold_monitor_sec_low_threshold));
                addPersistableAttributeForCreate (new AttributeSI32(&m_buffer,"buffer", threshold_monitor_sec_buffer));
                addPersistableAttributeForCreate (new AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction", threshold_monitor_sec_above_highthresh_action));
                addPersistableAttributeForCreate (new AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction"));       
                addPersistableAttributeForCreate (new AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction", threshold_monitor_sec_below_highthresh_action));
                addPersistableAttributeForCreate (new AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction", threshold_monitor_sec_below_lowthresh_action));
	}
	void SecLocalManagedObject::setupKeys()
	{
		vector<string > keyName;
		keyName.push_back ("areaname");
		setUserDefinedKeyCombination (keyName);
	}
	void SecLocalManagedObject::setAreaName(const UI32 &areaname, bool userConfig)
	{
			m_areaname.setUI32Value(areaname);
			m_areaname.setIsUserConfigured(userConfig);
	}
	UI32 SecLocalManagedObject::getAreaName() const
	{
			return (m_areaname.getUI32Value());
	}
	void SecLocalManagedObject::setTimebase(const UI32 &timebase)
	{
			m_timebase = timebase;
	}
	UI32 SecLocalManagedObject::getTimebase() const
	{
			return (m_timebase);
	}
	void SecLocalManagedObject::setHighThresh(const SI32 &highthresh)
	{
			m_highthresh = highthresh;
	}
	SI32 SecLocalManagedObject::getHighThresh() const
	{
			return (m_highthresh);
	}
	void SecLocalManagedObject::setLowThresh(const SI32 &lowthresh)
	{
			m_lowthresh = lowthresh;
	}
	SI32 SecLocalManagedObject::getLowThresh() const
	{
			return (m_lowthresh);
	}
	void SecLocalManagedObject::setBuffer(const SI32 &buffer)
	{
			m_buffer = buffer;
	}
	SI32 SecLocalManagedObject::getBuffer() const
	{
			return (m_buffer);
	}
	void SecLocalManagedObject::setAboveHighThreshAction(const
					BitMap &abovehighthreshaction)
	{
			m_abovehighthreshaction = abovehighthreshaction;
	}
	BitMap SecLocalManagedObject::getAboveHighThreshAction() const
	{
			return (m_abovehighthreshaction);
	}
	void SecLocalManagedObject::setAboveLowThreshAction(const
					BitMap &abovelowthreshaction)
	{
			m_abovelowthreshaction = abovelowthreshaction;
	}
	BitMap SecLocalManagedObject::getAboveLowThreshAction() const
	{
			return (m_abovelowthreshaction);
	}
	void SecLocalManagedObject::setBelowHighThreshAction(const
					BitMap &belowhighthreshaction)
	{
			m_belowhighthreshaction = belowhighthreshaction;
	}
	BitMap SecLocalManagedObject::getBelowHighThreshAction() const
	{
			return (m_belowhighthreshaction);
	}
	void SecLocalManagedObject::setBelowLowThreshAction(const
					BitMap &belowlowthreshaction)
	{
			m_belowlowthreshaction = belowlowthreshaction;
	}
	BitMap SecLocalManagedObject::getBelowLowThreshAction() const
	{
			return (m_belowlowthreshaction);
	}
}
