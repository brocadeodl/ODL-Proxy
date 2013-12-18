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
 *   Author : bkesanam                                                     *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "RAS/Local/RASNodeSpecificLocalManagedObject.h"
// for operational data
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
// for composition
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"

#include "RAS/Local/RASNodeSpecificLocalObjectManager.h"
#include "RAS/Local/RASLocalObjectManager.h"
#include "vcs.h"
#include "raslog/raslogd.h" /* syslogip */
#include "RASTypes.h"

using namespace std;

namespace DcmNs
{

RASNodeSpecificLocalManagedObject::RASNodeSpecificLocalManagedObject (RASLocalObjectManager *pRASLocalObjectManager)
		: PrismElement  (pRASLocalObjectManager),
		PrismPersistableObject (RASNodeSpecificLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pRASLocalObjectManager),
		DcmManagedObject (pRASLocalObjectManager),
		WaveLocalManagedObjectBase(this),
		DcmLocalManagedObjectBase(this),
		DcmLocalManagedObject (pRASLocalObjectManager),
		m_chassisName("chassis"),
		m_hostName("sw0")
{
}

RASNodeSpecificLocalManagedObject::~RASNodeSpecificLocalManagedObject ()
{
}

string  RASNodeSpecificLocalManagedObject::getClassName()
{
	return ("RASNodeSpecificLocalManagedObject");
}

void  RASNodeSpecificLocalManagedObject::setupAttributesForPersistence()
{
	DcmLocalManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_hostName, "host_name", ras_host_name));
	addPersistableAttribute (new AttributeString(&m_chassisName, "chassis_name", ras_chassis_name));
}

void  RASNodeSpecificLocalManagedObject::setupAttributesForCreate()
{
	DcmLocalManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeString(&m_hostName, "host_name", ras_host_name));
	addPersistableAttributeForCreate  (new AttributeString(&m_chassisName, "chassis_name", ras_chassis_name));
}

void RASNodeSpecificLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
	// This example code simply returns.
	// In real implementation, one does IPC to backed daemons (OR whatever) to fetch the operational data and set
	// the corresponding fields in the Managed Object.
	trace (TRACE_LEVEL_INFO, " RASNodeSpecificLocalManagedObject::loadOperationalData");

	pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	pWaveManagedObjectLoadOperationalDataContext->callback ();
}

void  RASNodeSpecificLocalManagedObject::setChassisName(const string &name)
{
	    m_chassisName  =  name;
}

string  RASNodeSpecificLocalManagedObject::getChassisName() const
{
	    return (m_chassisName);
}

void  RASNodeSpecificLocalManagedObject::setHostName(const string &name)
{
	    m_hostName  =  name;
}

string  RASNodeSpecificLocalManagedObject::getHostName() const
{
	    return (m_hostName);
}

} // namespace DcmNs
