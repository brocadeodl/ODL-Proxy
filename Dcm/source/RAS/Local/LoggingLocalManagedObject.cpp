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
 *   Author : chenj                                                     *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
// for operational data
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
// for composition
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"

#include "RAS/Local/RASLocalObjectManager.h"
//#include "RAS.h"
#include "vcs.h"
#include "raslog/raslogd.h" /* syslogip */
#include "RASTypes.h"

using namespace std;

namespace DcmNs
{

    LoggingLocalManagedObject::LoggingLocalManagedObject (RASLocalObjectManager *pRASLocalObjectManager)
        : PrismElement  (pRASLocalObjectManager),
        PrismPersistableObject (LoggingLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRASLocalObjectManager),
	DcmManagedObject (pRASLocalObjectManager),
	WaveLocalManagedObjectBase(this),
        DcmLocalManagedObjectBase(this),
	DcmLocalManagedObject (pRASLocalObjectManager),
	m_isAutoSupport(false),
	m_chassisName("chassis"),
	m_hostName("sw0")
    {
    }

LoggingLocalManagedObject::~LoggingLocalManagedObject ()
{
}

string  LoggingLocalManagedObject::getClassName()
{
    return ("LoggingLocalManagedObject");
}

void  LoggingLocalManagedObject::setupAttributesForPersistence()
{
        DcmLocalManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_switchId, "switchid",
				ras_rbridge_id));
        // addPersistableAttribute (new AttributeString(&m_raslog, "raslog", RAS_raslog));
	addPersistableAttribute (new AttributeBool (&m_isAutoSupport, "autoupload", ras_autoupload));
	addPersistableAttribute (new AttributeString(&m_hostName, "host_name", ras_host_name));
	addPersistableAttribute (new AttributeString(&m_chassisName, "chassis_name", ras_chassis_name));
}

void  LoggingLocalManagedObject::setupAttributesForCreate()
{
        DcmLocalManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeString(&m_switchId,
				"switchid", ras_rbridge_id));
        // addPersistableAttributeForCreate  (new AttributeString(&m_raslog,"raslog", RAS_raslog));
	addPersistableAttributeForCreate  (new AttributeBool(&m_isAutoSupport, "autoupload", ras_autoupload));
	addPersistableAttributeForCreate  (new AttributeString(&m_hostName, "host_name", ras_host_name));
	addPersistableAttributeForCreate  (new AttributeString(&m_chassisName, "chassis_name", ras_chassis_name));

	vector<string > keyName;
	keyName.push_back ("switchid");
	setUserDefinedKeyCombination (keyName);
}

void LoggingLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    // This example code simply returns.
    // In real implementation, one does IPC to backed daemons (OR whatever) to fetch the operational data and set the corresponding fields in the Managed Object.
    trace (TRACE_LEVEL_DEBUG, " LoggingLocalManagedObject::loadOperationalData");

    vector<string> operationalFields = pWaveManagedObjectLoadOperationalDataContext->getOperationalDataFields ();
    UI32 numberOfOperationalFields = operationalFields.size ();
    UI32 i                         = 0;

    // Fill All Operational Fields
    if (0 == numberOfOperationalFields)
    {
#if 0
	// get syslog IP
	vector<IpV4Address>  syslogIps;
	syslog_ipaddr_t syslogAddr;
	struct in_addr ipAddr;

	// IpV4Address ip;
	memset(&syslogAddr, 0, sizeof (syslog_ipaddr_t));
	if (syslogdIpGetNew(&syslogAddr) == 0) {
	    for (i = 0; i < (UI32)syslogAddr.count; i++) {
		if (0 < inet_pton(AF_INET, syslogAddr.addr[i], &ipAddr)) {
			// ip.fromString(syslogAddr.addr[i]);
		  syslogIps.push_back(string((char *)syslogAddr.addr[i]));
		}
	    }
	}
	setSyslogIp(syslogIps);
#endif

    } else {
        for (i = 0; i < numberOfOperationalFields; i++) {
#if 0
	 if (!operationalFields[i].compare ("ipaddress")) {
	  vector<IpV4Address>  syslogIps;
 	  for (uint j=0;j < 4; j++) {
	    IpV4Address ip (string ("") + j + string (".") + j + string (".") + j + string (".") + j);
	    syslogIps.push_back(string((char *)&ip));
	  }
	    setSyslogIp(syslogIps);

	 }
#endif
	}
    }

    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}


void  LoggingLocalManagedObject::setSwitchId(const string &switchId)
{
    m_switchId  =  switchId;
}

string  LoggingLocalManagedObject::getSwitchId() const
{
        return (m_switchId);
}

void  LoggingLocalManagedObject::setRaslog(const string &raslog)
{
    m_raslog  =  raslog;
}

string  LoggingLocalManagedObject::getRaslog() const
{
        return (m_raslog);
}

void  LoggingLocalManagedObject::setAutoSupport(bool support)
{
    m_isAutoSupport  =  support;
}

bool  LoggingLocalManagedObject::getAutoSupport() const
{
    return (m_isAutoSupport);
}

void  LoggingLocalManagedObject::setChassisName(const string &name)
{
    m_chassisName  =  name;
}

string  LoggingLocalManagedObject::getChassisName() const
{
    return (m_chassisName);
}


void  LoggingLocalManagedObject::setHostName(const string &name)
{
    m_hostName  =  name;
}

string  LoggingLocalManagedObject::getHostName() const
{
    return (m_hostName);
}
}
