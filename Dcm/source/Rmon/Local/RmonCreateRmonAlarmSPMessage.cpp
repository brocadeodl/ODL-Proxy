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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Rmon/Local/RmonCreateRmonAlarmSPMessage.h"
#include "Rmon/Local/RmonTypes.h"

namespace DcmNs
{

    RmonCreateRmonAlarmSPMessage::RmonCreateRmonAlarmSPMessage ()
        : PrismMessage (RmonLocalObjectManager::getPrismServiceId(),RMONCREATERMONALARMSP)
    {
    }

    RmonCreateRmonAlarmSPMessage::RmonCreateRmonAlarmSPMessage (const SI32 &alarmIndex,const string &snmpOid,const UI32 &interval,const UI32
		&sampleType,const UI32 &risingThreshold,const SI32 &risingEventId,const UI32 &fallingThreshold,const SI32 &fallingEventId,const string &owner)
        : PrismMessage(RmonLocalObjectManager::getPrismServiceId(),RMONCREATERMONALARMSP),
        m_alarmIndex    (alarmIndex),
        m_snmpOid    (snmpOid),
        m_interval    (interval),
		m_sampleType (sampleType),
        m_risingThreshold    (risingThreshold),
        m_risingEventId    (risingEventId),
        m_fallingThreshold    (fallingThreshold),
        m_fallingEventId    (fallingEventId),
        m_owner    (owner)
    {
    }

    RmonCreateRmonAlarmSPMessage::~RmonCreateRmonAlarmSPMessage ()
    {
    }

    void  RmonCreateRmonAlarmSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
        addSerializableAttribute (new AttributeString(&m_snmpOid,"snmpOid"));
        addSerializableAttribute (new AttributeUI32(&m_interval,"interval"));
		addSerializableAttribute (new AttributeUI32(&m_sampleType,"sampleType"));
        addSerializableAttribute (new AttributeUI32(&m_risingThreshold,"risingThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_risingEventId,"risingEventId"));
        addSerializableAttribute (new AttributeUI32(&m_fallingThreshold,"fallingThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_fallingEventId,"fallingEventId"));
        addSerializableAttribute (new AttributeString(&m_owner,"owner"));
    }

    void  RmonCreateRmonAlarmSPMessage::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonCreateRmonAlarmSPMessage::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

    void  RmonCreateRmonAlarmSPMessage::setSnmpOid(const string &snmpOid)
    {
        m_snmpOid  =  snmpOid;
    }

    string  RmonCreateRmonAlarmSPMessage::getSnmpOid() const
    {
        return (m_snmpOid);
    }

    void  RmonCreateRmonAlarmSPMessage::setInterval(const UI32 &interval)
    {
        m_interval  =  interval;
    }

    UI32  RmonCreateRmonAlarmSPMessage::getInterval() const
    {
        return (m_interval);
    }

	void  RmonCreateRmonAlarmSPMessage::setSampleType(const UI32 &sampleType)
    {
        m_sampleType =  sampleType;
    }

    UI32 RmonCreateRmonAlarmSPMessage::getSampleType() const
    {
        return (m_sampleType);
    }

    void  RmonCreateRmonAlarmSPMessage::setRisingThreshold(const UI32 &risingThreshold)
    {
        m_risingThreshold  =  risingThreshold;
    }

    UI32  RmonCreateRmonAlarmSPMessage::getRisingThreshold() const
    {
        return (m_risingThreshold);
    }

    void  RmonCreateRmonAlarmSPMessage::setRisingEventId(const SI32 &risingEventId)
    {
        m_risingEventId  =  risingEventId;
    }

    SI32  RmonCreateRmonAlarmSPMessage::getRisingEventId() const
    {
        return (m_risingEventId);
    }

    void  RmonCreateRmonAlarmSPMessage::setFallingThreshold(const UI32 &fallingThreshold)
    {
        m_fallingThreshold  =  fallingThreshold;
    }

    UI32  RmonCreateRmonAlarmSPMessage::getFallingThreshold() const
    {
        return (m_fallingThreshold);
    }

    void  RmonCreateRmonAlarmSPMessage::setFallingEventId(const SI32 &fallingEventId)
    {
        m_fallingEventId  =  fallingEventId;
    }

    SI32  RmonCreateRmonAlarmSPMessage::getFallingEventId() const
    {
        return (m_fallingEventId);
    }

    void  RmonCreateRmonAlarmSPMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonCreateRmonAlarmSPMessage::getOwner() const
    {
        return (m_owner);
    }

}
