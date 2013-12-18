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
  *    Copyright (C) 2012 Brocade Communications Systems, Inc.        	*
  *    All rights reserved.                                             *
  *    Author : Sachin Mishra					   	*
   **********************************************************************/

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/SecLocalMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{
	SecLocalMessage::SecLocalMessage() 
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECLOCALCONFIG)
	{
	}
	SecLocalMessage::SecLocalMessage(const string &policy, const UI32 &area, const UI32 &timebase,
                                const SI32 &highthresh,const SI32 &lowthresh,const SI32 &buffer,
                                const BitMap &abovehighthreshaction, const BitMap &abovelowthreshaction,
                                const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction)
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECLOCALCONFIG),
		m_policy	(policy),
		m_area		(area),
		m_timebase	(timebase),
		m_highthresh	(highthresh),
		m_lowthresh     (lowthresh),
		m_buffer	(buffer),
                m_abovehighthreshaction (abovehighthreshaction),
                m_abovelowthreshaction  (abovelowthreshaction),
                m_belowhighthreshaction (belowhighthreshaction),
                m_belowlowthreshaction  (belowlowthreshaction)
	
		{
		}
	SecLocalMessage::~SecLocalMessage ()
	{
	}

	void SecLocalMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_policy, "policy"));
		addSerializableAttribute (new AttributeUI32(&m_area, "areaName"));
		addSerializableAttribute (new AttributeUI32(&m_timebase, "timebase"));
		addSerializableAttribute (new AttributeSI32(&m_highthresh, "highthresh"));
		addSerializableAttribute (new AttributeSI32(&m_lowthresh, "lowthresh"));
		addSerializableAttribute (new AttributeSI32(&m_buffer, "buffer"));
                addSerializableAttribute (new AttributeBitMap(&m_abovehighthreshaction,
                                        "abovehighthreshaction"));
                addSerializableAttribute (new AttributeBitMap(&m_abovelowthreshaction,
                                        "abovelowthreshaction"));
                addSerializableAttribute (new AttributeBitMap(&m_belowhighthreshaction,
                                        "belowhighthreshaction"));
                addSerializableAttribute (new AttributeBitMap(&m_belowlowthreshaction,
                                        "belowlowthreshaction"));
	}
	void SecLocalMessage::setPolicyName(const string &policy)
	{
		m_policy = policy;
	}
	string SecLocalMessage::getPolicyName() const
	{
		return (m_policy);
	}
	void SecLocalMessage::setAreaName(const UI32 &area)
	{
		m_area = area;
	}
	UI32 SecLocalMessage::getAreaName() const
	{
		return (m_area);
	}
	void SecLocalMessage::setTimebase(const UI32 &timebase)
	{
		m_timebase = timebase;
	}
	UI32 SecLocalMessage::getTimebase() const
	{
		return (m_timebase);
	}
	void SecLocalMessage::setHighThresh(const SI32 &highthresh)
	{
		m_highthresh = highthresh;	
	}		
	SI32 SecLocalMessage::getHighThresh() const
	{
		return (m_highthresh);
	}
	void SecLocalMessage::setLowThresh(const SI32 &lowthresh)
	{
		m_lowthresh = lowthresh;	
	}		
	SI32 SecLocalMessage::getLowThresh() const
	{
		return (m_lowthresh);
	}
	void SecLocalMessage::setBuffer(const SI32 &buffer)
	{
		m_buffer = buffer;	
	}		
	SI32 SecLocalMessage::getBuffer() const
	{
		return (m_buffer);
	}
        void SecLocalMessage::setAboveHighThreshAction(const
                        BitMap &abovehighthreshaction)
        {
                m_abovehighthreshaction = abovehighthreshaction;
        }
        BitMap SecLocalMessage::getAboveHighThreshAction() const
        {
                return (m_abovehighthreshaction);
        }
        void SecLocalMessage::setAboveLowThreshAction(const
                        BitMap &abovelowthreshaction)
        {
                m_abovelowthreshaction = abovelowthreshaction;
        }
        BitMap SecLocalMessage::getAboveLowThreshAction() const
        {
                return (m_abovelowthreshaction);
        }
        void SecLocalMessage::setBelowHighThreshAction(const
                        BitMap &belowhighthreshaction)
        {
                m_belowhighthreshaction = belowhighthreshaction;
        }
        BitMap SecLocalMessage::getBelowHighThreshAction() const
        {
                return (m_belowhighthreshaction);
        }
        void SecLocalMessage::setBelowLowThreshAction(const
                        BitMap &belowlowthreshaction)
        {
                m_belowlowthreshaction = belowlowthreshaction;
        }
        BitMap SecLocalMessage::getBelowLowThreshAction() const
        {
                return (m_belowlowthreshaction);
        }
}

