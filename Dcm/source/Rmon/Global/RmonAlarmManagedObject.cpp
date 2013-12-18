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

#include "Rmon/Global/RmonAlarmManagedObject.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    RmonAlarmManagedObject::RmonAlarmManagedObject (RmonGlobalObjectManager *pRmonGlobalObjectManager) : PrismElement  (pRmonGlobalObjectManager),
        PrismPersistableObject (RmonAlarmManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pRmonGlobalObjectManager),
        DcmManagedObject (pRmonGlobalObjectManager)
    {
		m_defaultData = 0;
    }

    RmonAlarmManagedObject::RmonAlarmManagedObject (RmonGlobalObjectManager *pRmonGlobalObjectManager,const SI32 &alarmIndex,const string &snmpOid,const UI32 &interval,const UI32 &sampleType,const UI32 &risingThreshold,const SI32 &risingEventId,const UI32 &fallingThreshold,const SI32 &fallingEventId,const string &owner, const string &ifName, const SI32 &statsIndex) : PrismElement  (pRmonGlobalObjectManager),
        PrismPersistableObject (RmonAlarmManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pRmonGlobalObjectManager),
        DcmManagedObject (pRmonGlobalObjectManager),
        m_alarmIndex    (alarmIndex),
        m_snmpOid    (snmpOid),
        m_interval    (interval),
        m_sampleType (sampleType),
        m_risingThreshold    (risingThreshold),
        m_risingEventId    (risingEventId),
        m_fallingThreshold    (fallingThreshold),
        m_fallingEventId    (fallingEventId),
        m_owner    (owner),
		m_defaultData (0),
		m_ifName	(ifName),
		m_statsIndex	(statsIndex)
    {
    }

    RmonAlarmManagedObject::~RmonAlarmManagedObject ()
    {
    }

    string  RmonAlarmManagedObject::getClassName()
    {
        return ("RmonAlarmManagedObject");
    }

    void  RmonAlarmManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
        addPersistableAttribute (new AttributeString(&m_snmpOid,"snmpOid", rmon_snmp_oid));
        addPersistableAttribute (new AttributeUI32(&m_interval,"interval", rmon_alarm_interval));
        addPersistableAttribute (new AttributeEnum(&m_sampleType,"sampleType", rmon_alarm_sample));
        addPersistableAttribute (new AttributeUI32(&m_risingThreshold,"risingThreshold", rmon_alarm_rising_threshold));
        addPersistableAttribute (new AttributeSI32(&m_risingEventId,"risingEventId", rmon_alarm_rising_event_index));
        addPersistableAttribute (new AttributeUI32(&m_fallingThreshold,"fallingThreshold", rmon_alarm_falling_threshold));
        addPersistableAttribute (new AttributeSI32(&m_fallingEventId, m_defaultData, "fallingEventId", rmon_alarm_falling_event_index));
        addPersistableAttribute (new AttributeString(&m_owner,"owner", rmon_alarm_owner));
		addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
		addPersistableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
		setUserTagForAttribute("alarmIndex", rmon_alarm_index);
    }

    void  RmonAlarmManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
        addPersistableAttributeForCreate  (new AttributeString(&m_snmpOid,"snmpOid", rmon_snmp_oid));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_interval,"interval", rmon_alarm_interval));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_sampleType,"sampleType", rmon_alarm_sample));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_risingThreshold,"risingThreshold", rmon_alarm_rising_threshold));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_risingEventId,"risingEventId", rmon_alarm_rising_event_index));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_fallingThreshold,"fallingThreshold", rmon_alarm_falling_threshold));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_fallingEventId,m_defaultData,"fallingEventId", rmon_alarm_falling_event_index));
        addPersistableAttributeForCreate  (new AttributeString(&m_owner,"owner", rmon_alarm_owner));
		addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_statsIndex,"statsIndex"));

		vector<string > keyName;
        keyName.push_back ("alarmIndex");
        setUserDefinedKeyCombination (keyName);
    }

    void  RmonAlarmManagedObject::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonAlarmManagedObject::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

    void  RmonAlarmManagedObject::setSnmpOid(const string &snmpOid)
    {
        m_snmpOid  =  snmpOid;
    }

    string  RmonAlarmManagedObject::getSnmpOid() const
    {
        return (m_snmpOid);
    }

    void  RmonAlarmManagedObject::setInterval(const UI32 &interval)
    {
        m_interval  =  interval;
    }

    UI32  RmonAlarmManagedObject::getInterval() const
    {
        return (m_interval);
    }

	void  RmonAlarmManagedObject::setSampleType(const UI32 &sampleType)
    {
        m_sampleType =  sampleType;
    }

    UI32 RmonAlarmManagedObject::getSampleType() const
    {
        return (m_sampleType);
    }

    void  RmonAlarmManagedObject::setRisingThreshold(const UI32 &risingThreshold)
    {
        m_risingThreshold  =  risingThreshold;
    }

    UI32  RmonAlarmManagedObject::getRisingThreshold() const
    {
        return (m_risingThreshold);
    }

    void  RmonAlarmManagedObject::setRisingEventId(const SI32 &risingEventId)
    {
        m_risingEventId  =  risingEventId;
    }

    SI32  RmonAlarmManagedObject::getRisingEventId() const
    {
        return (m_risingEventId);
    }

    void  RmonAlarmManagedObject::setFallingThreshold(const UI32 &fallingThreshold)
    {
        m_fallingThreshold  =  fallingThreshold;
    }

    UI32  RmonAlarmManagedObject::getFallingThreshold() const
    {
        return (m_fallingThreshold);
    }

    void  RmonAlarmManagedObject::setFallingEventId(const SI32 &fallingEventId)
    {
        m_fallingEventId  =  fallingEventId;
    }

    SI32  RmonAlarmManagedObject::getFallingEventId() const
    {
        return (m_fallingEventId);
    }

    void  RmonAlarmManagedObject::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonAlarmManagedObject::getOwner() const
    {
        return (m_owner);
    }
    void  RmonAlarmManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonAlarmManagedObject::getIfName() const
    {
        return (m_ifName);
    }

	void  RmonAlarmManagedObject::setStatsIndex(const SI32 &statsIndex)
    {
        m_statsIndex  =  statsIndex;
    }

	SI32 RmonAlarmManagedObject::getStatsIndex() const
    {
        return (m_statsIndex);
    }
}
