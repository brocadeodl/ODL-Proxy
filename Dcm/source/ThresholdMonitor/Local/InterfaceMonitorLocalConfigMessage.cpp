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
  *    Copyright (C) 2005-2011 Brocade Communications Systems, Inc.        *
  *    All rights reserved.                                                *
  *    Author : Priya Ahuja												   *
   **************************************************************************/

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/InterfaceMonitorLocalConfigMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{
	InterfaceMonitorLocalConfigMessage::InterfaceMonitorLocalConfigMessage() 
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), INTERFACEMONITORLOCALCONFIG)
	{
	}
	InterfaceMonitorLocalConfigMessage::InterfaceMonitorLocalConfigMessage(const
			string
			&policyName, const UI32 &interfacetypename, const UI32 &areaName, const
			SI32 &highthresh, const SI32 &lowthresh, const SI32 &bufferthresh,
			const BitMap &abovehighthreshaction, const BitMap &abovelowthreshaction,
			const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction, const UI32 &timebase)
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), INTERFACEMONITORLOCALCONFIG),
		m_policyName   (policyName),
		m_interfacetypename  (interfacetypename),
		m_areaName	   (areaName),
		m_highthresh	(highthresh),
		m_lowthresh     (lowthresh),
		m_bufferthresh  (bufferthresh),
		m_abovehighthreshaction (abovehighthreshaction),
		m_abovelowthreshaction  (abovelowthreshaction),
		m_belowhighthreshaction (belowhighthreshaction),
		m_belowlowthreshaction  (belowlowthreshaction),
		m_timeBase	   (timebase)
		{
		}
	InterfaceMonitorLocalConfigMessage::~InterfaceMonitorLocalConfigMessage ()
	{
	}

	void InterfaceMonitorLocalConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
		addSerializableAttribute (new AttributeUI32(&m_interfacetypename, "interfacetypename"));
		addSerializableAttribute (new AttributeUI32(&m_areaName, "areaName"));
		addSerializableAttribute (new AttributeSI32(&m_highthresh, "highthresh"));
		addSerializableAttribute (new AttributeSI32(&m_lowthresh, "lowthresh"));
		addSerializableAttribute (new AttributeSI32(&m_bufferthresh, "bufferthresh"));
		addSerializableAttribute (new AttributeBitMap(&m_abovehighthreshaction,
					"abovehighthreshaction"));
		addSerializableAttribute (new AttributeBitMap(&m_abovelowthreshaction,
					"abovelowthreshaction"));
		addSerializableAttribute (new AttributeBitMap(&m_belowhighthreshaction,
					"belowhighthreshaction"));
		addSerializableAttribute (new AttributeBitMap(&m_belowlowthreshaction,
					"belowlowthreshaction"));
		addSerializableAttribute (new AttributeUI32(&m_timeBase, "timeBase"));
	}
	void InterfaceMonitorLocalConfigMessage::setPolicyName(const string &policyName)
	{
		m_policyName = policyName;
	}
	string InterfaceMonitorLocalConfigMessage::getPolicyName() const
	{
		return (m_policyName);
	}
	void InterfaceMonitorLocalConfigMessage::setInterfaceTypeName(const UI32 &interfacetypename)
	{
		m_interfacetypename = interfacetypename;
	}
	UI32 InterfaceMonitorLocalConfigMessage::getInterfaceTypeName() const
	{
		return (m_interfacetypename);
	}
	void InterfaceMonitorLocalConfigMessage::setAreaName(const UI32 &areaName)
	{
		m_areaName = areaName;
	}
	UI32 InterfaceMonitorLocalConfigMessage::getAreaName() const
	{
		return (m_areaName);
	}
	void InterfaceMonitorLocalConfigMessage::setHighThresh(const SI32 &highthresh)
	{
		m_highthresh = highthresh;	
	}		
	SI32 InterfaceMonitorLocalConfigMessage::getHighThresh() const
	{
		return (m_highthresh);
	}
	void InterfaceMonitorLocalConfigMessage::setLowThresh(const SI32 &lowthresh)
	{
		m_lowthresh = lowthresh;	
	}		
	SI32 InterfaceMonitorLocalConfigMessage::getLowThresh() const
	{
		return (m_lowthresh);
	}
	void InterfaceMonitorLocalConfigMessage::setBufferThresh(const SI32 &bufferthresh)
	{
		m_bufferthresh = bufferthresh;	
	}		
	SI32 InterfaceMonitorLocalConfigMessage::getBufferThresh() const
	{
		return (m_bufferthresh);
	}
	void InterfaceMonitorLocalConfigMessage::setAboveHighThreshAction(const
			BitMap &abovehighthreshaction)
	{
		m_abovehighthreshaction = abovehighthreshaction;
	}
	BitMap InterfaceMonitorLocalConfigMessage::getAboveHighThreshAction() const
	{
		return (m_abovehighthreshaction);
	}
	void InterfaceMonitorLocalConfigMessage::setAboveLowThreshAction(const
			BitMap &abovelowthreshaction)
	{
		m_abovelowthreshaction = abovelowthreshaction;
	}
	BitMap InterfaceMonitorLocalConfigMessage::getAboveLowThreshAction() const
	{
		return (m_abovelowthreshaction);
	}
	void InterfaceMonitorLocalConfigMessage::setBelowHighThreshAction(const
			BitMap &belowhighthreshaction)
	{
		m_belowhighthreshaction = belowhighthreshaction;
	}
	BitMap InterfaceMonitorLocalConfigMessage::getBelowHighThreshAction() const
	{
		return (m_belowhighthreshaction);	
	}
	void InterfaceMonitorLocalConfigMessage::setBelowLowThreshAction(const
			BitMap &belowlowthreshaction)
	{
		m_belowlowthreshaction = belowlowthreshaction;
	}
	BitMap InterfaceMonitorLocalConfigMessage::getBelowLowThreshAction() const
	{
		return (m_belowlowthreshaction);
	}
	void InterfaceMonitorLocalConfigMessage::setTimeBase(const UI32 &timeBase)
	{
		m_timeBase = timeBase;
	}
	UI32 InterfaceMonitorLocalConfigMessage::getTimeBase() const
	{
		return (m_timeBase);
	}

}
