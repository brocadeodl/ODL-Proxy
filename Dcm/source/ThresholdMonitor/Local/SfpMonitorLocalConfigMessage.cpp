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
  *    Author : Ramya Rangarajan										   *
   **************************************************************************/

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/SfpMonitorLocalConfigMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{
	SfpMonitorLocalConfigMessage::SfpMonitorLocalConfigMessage() 
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SFPMONITORLOCALCONFIG)
	{
	}
	SfpMonitorLocalConfigMessage::SfpMonitorLocalConfigMessage(const string
			&policyName, const UI32 &sfptypename, const UI32 &areaName, const
			SI32 &highthresh, const SI32 &lowthresh, const SI32 &bufferthresh,
			const BitMap &abovehighthreshaction, const BitMap &abovelowthreshaction,
			const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction)
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SFPMONITORLOCALCONFIG),
		m_policyName   (policyName),
		m_sfptypename  (sfptypename),
		m_areaName	   (areaName),
		m_highthresh	(highthresh),
		m_lowthresh     (lowthresh),
		m_bufferthresh  (bufferthresh),
		m_abovehighthreshaction (abovehighthreshaction),
		m_abovelowthreshaction  (abovelowthreshaction),
		m_belowhighthreshaction (belowhighthreshaction),
		m_belowlowthreshaction  (belowlowthreshaction)
		{
		}
	SfpMonitorLocalConfigMessage::~SfpMonitorLocalConfigMessage ()
	{
	}

	void SfpMonitorLocalConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
		addSerializableAttribute (new AttributeUI32(&m_sfptypename, "sfptypename"));
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
	}
	void SfpMonitorLocalConfigMessage::setPolicyName(const string &policyName)
	{
		m_policyName = policyName;
	}
	string SfpMonitorLocalConfigMessage::getPolicyName() const
	{
		return (m_policyName);
	}
	void SfpMonitorLocalConfigMessage::setSfpTypeName(const UI32 &sfptypename)
	{
		m_sfptypename = sfptypename;
	}
	UI32 SfpMonitorLocalConfigMessage::getSfpTypeName() const
	{
		return (m_sfptypename);
	}
	void SfpMonitorLocalConfigMessage::setAreaName(const UI32 &areaName)
	{
		m_areaName = areaName;
	}
	UI32 SfpMonitorLocalConfigMessage::getAreaName() const
	{
		return (m_areaName);
	}
	void SfpMonitorLocalConfigMessage::setHighThresh(const SI32 &highthresh)
	{
		m_highthresh = highthresh;	
	}		
	SI32 SfpMonitorLocalConfigMessage::getHighThresh() const
	{
		return (m_highthresh);
	}
	void SfpMonitorLocalConfigMessage::setLowThresh(const SI32 &lowthresh)
	{
		m_lowthresh = lowthresh;	
	}		
	SI32 SfpMonitorLocalConfigMessage::getLowThresh() const
	{
		return (m_lowthresh);
	}
	void SfpMonitorLocalConfigMessage::setBufferThresh(const SI32 &bufferthresh)
	{
		m_bufferthresh = bufferthresh;	
	}		
	SI32 SfpMonitorLocalConfigMessage::getBufferThresh() const
	{
		return (m_bufferthresh);
	}
	void SfpMonitorLocalConfigMessage::setAboveHighThreshAction(const
			BitMap &abovehighthreshaction)
	{
		m_abovehighthreshaction = abovehighthreshaction;
	}
	BitMap SfpMonitorLocalConfigMessage::getAboveHighThreshAction() const
	{
		return (m_abovehighthreshaction);
	}
	void SfpMonitorLocalConfigMessage::setAboveLowThreshAction(const
			BitMap &abovelowthreshaction)
	{
		m_abovelowthreshaction = abovelowthreshaction;
	}
	BitMap SfpMonitorLocalConfigMessage::getAboveLowThreshAction() const
	{
		return (m_abovelowthreshaction);
	}
	void SfpMonitorLocalConfigMessage::setBelowHighThreshAction(const
			BitMap &belowhighthreshaction)
	{
		m_belowhighthreshaction = belowhighthreshaction;
	}
	BitMap SfpMonitorLocalConfigMessage::getBelowHighThreshAction() const
	{
		return (m_belowhighthreshaction);	
	}
	void SfpMonitorLocalConfigMessage::setBelowLowThreshAction(const
			BitMap &belowlowthreshaction)
	{
		m_belowlowthreshaction = belowlowthreshaction;
	}
	BitMap SfpMonitorLocalConfigMessage::getBelowLowThreshAction() const
	{
		return (m_belowlowthreshaction);
	}
}
