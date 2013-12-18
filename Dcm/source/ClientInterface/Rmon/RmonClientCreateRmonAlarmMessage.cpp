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

#include "ClientInterface/Rmon/RmonClientCreateRmonAlarmMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/rmon_common.h"

namespace DcmNs
{

    RmonClientCreateRmonAlarmMessage::RmonClientCreateRmonAlarmMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTCREATERMONALARM)
    {
    }

    RmonClientCreateRmonAlarmMessage::RmonClientCreateRmonAlarmMessage (const SI32 &alarmIndex,const string &snmpOid,const UI32 &interval,const UI32
		&sampleType,const UI32 &risingThreshold,const SI32 &risingEventId,const UI32 &fallingThreshold,const SI32 &fallingEventId,const string &owner)
        : DcmManagementInterfaceMessage (RMONCLIENTCREATERMONALARM),
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

    RmonClientCreateRmonAlarmMessage::~RmonClientCreateRmonAlarmMessage ()
    {
    }

    void  RmonClientCreateRmonAlarmMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  RmonClientCreateRmonAlarmMessage::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonClientCreateRmonAlarmMessage::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

    void  RmonClientCreateRmonAlarmMessage::setSnmpOid(const string &snmpOid)
    {
        m_snmpOid  =  snmpOid;
    }

    string  RmonClientCreateRmonAlarmMessage::getSnmpOid() const
    {
        return (m_snmpOid);
    }

    void  RmonClientCreateRmonAlarmMessage::setInterval(const UI32 &interval)
    {
        m_interval  =  interval;
    }

    UI32  RmonClientCreateRmonAlarmMessage::getInterval() const
    {
        return (m_interval);
    }

	void  RmonClientCreateRmonAlarmMessage::setSampleType(const UI32 &sampleType)
    {
        m_sampleType =  sampleType;
    }

    UI32 RmonClientCreateRmonAlarmMessage::getSampleType() const
    {
        return (m_sampleType);
    }

    void  RmonClientCreateRmonAlarmMessage::setRisingThreshold(const UI32 &risingThreshold)
    {
        m_risingThreshold  =  risingThreshold;
    }

    UI32  RmonClientCreateRmonAlarmMessage::getRisingThreshold() const
    {
        return (m_risingThreshold);
    }

    void  RmonClientCreateRmonAlarmMessage::setRisingEventId(const SI32 &risingEventId)
    {
        m_risingEventId  =  risingEventId;
    }

    SI32  RmonClientCreateRmonAlarmMessage::getRisingEventId() const
    {
        return (m_risingEventId);
    }

    void  RmonClientCreateRmonAlarmMessage::setFallingThreshold(const UI32 &fallingThreshold)
    {
        m_fallingThreshold  =  fallingThreshold;
    }

    UI32  RmonClientCreateRmonAlarmMessage::getFallingThreshold() const
    {
        return (m_fallingThreshold);
    }

    void  RmonClientCreateRmonAlarmMessage::setFallingEventId(const SI32 &fallingEventId)
    {
        m_fallingEventId  =  fallingEventId;
    }

    SI32  RmonClientCreateRmonAlarmMessage::getFallingEventId() const
    {
        return (m_fallingEventId);
    }

    void  RmonClientCreateRmonAlarmMessage::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonClientCreateRmonAlarmMessage::getOwner() const
    {
        return (m_owner);
    }

	const void *RmonClientCreateRmonAlarmMessage::getCStructureForInputs ()
    {
        rmon_alarm_message_t *pInput = (rmon_alarm_message_t*)malloc(sizeof(rmon_alarm_message_t));
        pInput->alarm_index = m_alarmIndex;
        pInput->interval = m_interval;
        pInput->sample_type = m_sampleType;
        pInput->rising_event = m_risingEventId;
        pInput->rising_threshold = m_risingThreshold;
        pInput->falling_event = m_fallingEventId;
        pInput->falling_threshold = m_fallingThreshold;
		strncpy(pInput->owner, m_owner.c_str(), RMON_MAX_WORD_LEN);
		pInput->owner[RMON_MAX_WORD_LEN] = '\0';
		strncpy(pInput->snmp_oid, m_snmpOid.c_str(), RMON_MAX_ALARM_OID_LEN);
		pInput->snmp_oid[RMON_MAX_ALARM_OID_LEN] = '\0';
        return (pInput);
    }

    void RmonClientCreateRmonAlarmMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }


}
