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

#include "ThresholdMonitor/Local/SfpMonitorAreaLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"
namespace DcmNs
{
	SfpMonitorAreaLocalManagedObject::SfpMonitorAreaLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SfpMonitorAreaLocalManagedObject::getClassName (),DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pThresholdMonitorLocalObjectManager),
		DcmManagedObject (pThresholdMonitorLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
		{
		//	m_sfptypename.setIsUserConfigured(false);
		//	m_areaname.setIsUserConfigured(false);
		}
	SfpMonitorAreaLocalManagedObject::SfpMonitorAreaLocalManagedObject(ThresholdMonitorLocalObjectManager
			*pThresholdMonitorLocalObjectManager, const UI32 &sfptypename, const UI32 &areaname, const SI32 &highthresh, const SI32 &lowthresh, const SI32 &buffer, const BitMap &abovehighthreshaction,const BitMap &abovelowthreshaction,const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction)
		: PrismElement  (pThresholdMonitorLocalObjectManager),
		PrismPersistableObject (SfpMonitorAreaLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()), 
		WaveManagedObject (pThresholdMonitorLocalObjectManager), 
		DcmManagedObject (pThresholdMonitorLocalObjectManager), 
		WaveLocalManagedObjectBase (this), 
		DcmLocalManagedObjectBase (this), 
		DcmLocalManagedObject (pThresholdMonitorLocalObjectManager), 
		m_sfptypename   (sfptypename),
		m_areaname	(areaname),
		m_highthresh (highthresh),
		m_lowthresh  (lowthresh),
		m_buffer  (buffer),
		m_abovehighthreshaction  (abovehighthreshaction),
		m_abovelowthreshaction   (abovelowthreshaction),
		m_belowhighthreshaction  (belowhighthreshaction),
		m_belowlowthreshaction  (belowlowthreshaction)
		{
		}
	SfpMonitorAreaLocalManagedObject::~SfpMonitorAreaLocalManagedObject ()
	{
	}

	string SfpMonitorAreaLocalManagedObject::getClassName()
	{
		return ("SfpMonitorAreaLocalManagedObject");
	}
	/* TR000401639 : Removing support for above lowthresh-action, maintaining as dummy field for future */	
	void SfpMonitorAreaLocalManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeEnumUC(&m_sfptypename,true,"sfptypename"));
		addPersistableAttribute (new
				AttributeEnumUC(&m_areaname,true,"areaname"));
		addPersistableAttribute (new AttributeSI32(&m_highthresh,"highthresh",threshold_monitor_high_threshold));
		addPersistableAttribute (new AttributeSI32(&m_lowthresh,"lowthresh",threshold_monitor_low_threshold));
		addPersistableAttribute (new AttributeSI32(&m_buffer,"buffer",threshold_monitor_buffer));	
		addPersistableAttribute (new
				AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction",threshold_monitor_above_highthresh_action));
		addPersistableAttribute (new
				AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction"));
		addPersistableAttribute (new
				AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction",threshold_monitor_below_highthresh_action));
		addPersistableAttribute (new
				AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction",threshold_monitor_below_lowthresh_action));
		setUserTagForAttribute ("sfptypename", threshold_monitor_type);
		setUserTagForAttribute ("areaname", threshold_monitor_area_value);
	}
	
	void SfpMonitorAreaLocalManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeEnumUC(&m_sfptypename,
					true,"sfptypename"));
		addPersistableAttributeForCreate (new AttributeEnumUC(&m_areaname,
					true,"areaname"));
		addPersistableAttributeForCreate (new
				AttributeSI32(&m_highthresh,"highthresh",threshold_monitor_high_threshold));
		addPersistableAttributeForCreate (new
				AttributeSI32(&m_lowthresh,"lowthresh",threshold_monitor_low_threshold));
		addPersistableAttributeForCreate (new AttributeSI32(&m_buffer,"buffer",threshold_monitor_buffer));	
	
		addPersistableAttributeForCreate (new AttributeBitMap(&m_abovehighthreshaction,"abovehighthreshaction",threshold_monitor_above_highthresh_action));
		addPersistableAttributeForCreate (new AttributeBitMap(&m_abovelowthreshaction,"abovelowthreshaction"));		
		addPersistableAttributeForCreate (new AttributeBitMap(&m_belowhighthreshaction,"belowhighthreshaction",threshold_monitor_below_highthresh_action));
		addPersistableAttributeForCreate (new AttributeBitMap(&m_belowlowthreshaction,"belowlowthreshaction",threshold_monitor_below_lowthresh_action));
	}
	void SfpMonitorAreaLocalManagedObject::setupKeys()
	{
		vector<string > keyName;
		keyName.push_back ("sfptypename");
		keyName.push_back ("areaname");
		setUserDefinedKeyCombination (keyName);
	}
	void SfpMonitorAreaLocalManagedObject::setSfptypeName(const UI32
			&sfptypename, bool userConfig)
	{
		m_sfptypename.setUI32Value(sfptypename);
		m_sfptypename.setIsUserConfigured(userConfig);
	}
	UI32 SfpMonitorAreaLocalManagedObject::getSfptypeName() const
	{
		return (m_sfptypename.getUI32Value());
	}

	void SfpMonitorAreaLocalManagedObject::setAreaName(const UI32 &areaname,
			bool userConfig)
	{
		m_areaname.setUI32Value(areaname);
		m_areaname.setIsUserConfigured(userConfig);
	}
	UI32 SfpMonitorAreaLocalManagedObject::getAreaName() const
	{
		return (m_areaname.getUI32Value());
	}
	void SfpMonitorAreaLocalManagedObject::setHighThresh(const SI32 &highthresh)
	{
		m_highthresh = highthresh;
	}
	SI32 SfpMonitorAreaLocalManagedObject::getHighThresh() const
	{
		return (m_highthresh);
	}

	void SfpMonitorAreaLocalManagedObject::setLowThresh(const SI32 &lowthresh)
	{
		m_lowthresh = lowthresh;
	}
	SI32 SfpMonitorAreaLocalManagedObject::getLowThresh() const
	{
		return (m_lowthresh);
	}

	void SfpMonitorAreaLocalManagedObject::setBuffer(const SI32 &buffer)
	{
		m_buffer = buffer;
	}
	SI32 SfpMonitorAreaLocalManagedObject::getBuffer() const
	{
		return (m_buffer);
	}
	void SfpMonitorAreaLocalManagedObject::setAboveHighThreshAction(const
			BitMap &abovehighthreshaction)
	{
		m_abovehighthreshaction = abovehighthreshaction;
	}
	BitMap SfpMonitorAreaLocalManagedObject::getAboveHighThreshAction() const
	{
		return (m_abovehighthreshaction);
	}
        void SfpMonitorAreaLocalManagedObject::setAboveLowThreshAction(const
			BitMap &abovelowthreshaction)
	{
		m_abovelowthreshaction = abovelowthreshaction;
	}
	BitMap SfpMonitorAreaLocalManagedObject::getAboveLowThreshAction() const
	{
		return (m_abovelowthreshaction);
	}
	void SfpMonitorAreaLocalManagedObject::setBelowHighThreshAction(const
			BitMap &belowhighthreshaction)
	{
		m_belowhighthreshaction = belowhighthreshaction;
	}
	BitMap SfpMonitorAreaLocalManagedObject::getBelowHighThreshAction() const
	{
		return (m_belowhighthreshaction);	
	}
	void SfpMonitorAreaLocalManagedObject::setBelowLowThreshAction(const
			BitMap &belowlowthreshaction)
	{
		m_belowlowthreshaction = belowlowthreshaction;
	}
	BitMap SfpMonitorAreaLocalManagedObject::getBelowLowThreshAction() const
	{
		return (m_belowlowthreshaction);
	}
}
