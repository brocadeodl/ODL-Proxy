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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#include "VcsFabric/Global/VcsSwitchSerialNumberMappedIdManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#if 0
namespace DcmNs
{

using namespace std;

VcsSwitchSerialNumberMappedIdManagedObject::VcsSwitchSerialNumberMappedIdManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement           (pWaveObjectManager),
      PrismPersistableObject (VcsSwitchSerialNumberMappedIdManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pWaveObjectManager),
      DcmManagedObject       (pWaveObjectManager)
{
    setEmptyNeededOnPersistentBoot (false);
}

VcsSwitchSerialNumberMappedIdManagedObject::~VcsSwitchSerialNumberMappedIdManagedObject ()
{
}

void VcsSwitchSerialNumberMappedIdManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_switchSerialNumber, "switchSerialNumber"));
    addPersistableAttribute (new AttributeUI32 (&m_mappedId, "mappedId"));
    addPersistableAttribute (new AttributeBool (&m_switchActive, "switchActive"));
    addPersistableAttribute (new AttributeString (&m_switchIpAddress, "switchIpAddress"));
    addPersistableAttribute (new AttributeUI32 (&m_switchPortId, "switchPortId"));
    addPersistableAttribute (new AttributeString (&m_switchMgmtIpAddress, "switchMgmtIpAddress"));
    addPersistableAttribute (new AttributeUI32 (&m_switchMgmtPortId, "switchMgmtPortId"));
    addPersistableAttribute (new AttributeBool (&m_switchDeleteAfterFailover, "switchDeleteAfterFailover"));

}

void VcsSwitchSerialNumberMappedIdManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_switchSerialNumber, "switchSerialNumber"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_mappedId, "mappedId"));
    addPersistableAttributeForCreate (new AttributeBool (&m_switchActive, "switchActive"));
    addPersistableAttributeForCreate (new AttributeString (&m_switchIpAddress, "switchIpAddress"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_switchPortId, "switchPortId"));
    addPersistableAttributeForCreate (new AttributeString (&m_switchMgmtIpAddress, "switchMgmtIpAddress"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_switchMgmtPortId, "switchMgmtPortId"));
    addPersistableAttributeForCreate (new AttributeBool (&m_switchDeleteAfterFailover, "switchDeleteAfterFailover"));
}

string VcsSwitchSerialNumberMappedIdManagedObject::getClassName ()
{
    return ("VcsSwitchSerialNumberMappedIdManagedObject");
}

bool VcsSwitchSerialNumberMappedIdManagedObject::getSwitchDeleteAfterFailoverUseSNInSNDB (const string &switchSerialNumber)
{
    trace (TRACE_LEVEL_INFO, "VcsSwitchSerialNumberMappedIdManagedObject::getSwitchDeleteAfterFailoverUseSNInSNDB : Entering ...");
    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
    bool retValue = 0;

    // check if serial number to deactivate exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
        retValue = (pVcsSwitchSerialNumberMappedIdManagedObject->m_switchDeleteAfterFailover & pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive);
        trace (TRACE_LEVEL_INFO, string("VcsSwitchSerialNumberMappedIdManagedObject::getSwitchDeleteAfterFailoverUseSNInSNDB : delete = ") + pVcsSwitchSerialNumberMappedIdManagedObject->m_switchDeleteAfterFailover
            + " active = " + pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive + " sn = " + pVcsSwitchSerialNumberMappedIdManagedObject->m_switchSerialNumber);
    }
    pVcsFabObjectManager->deleteObjects (pResultsForSN);
    return (retValue);
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::setSwitchDeleteAfterFailoverUseSNInSNDB (const string &switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::setSwitchDeleteAfterFailoverUseSNInSNDB : Entering ...");
    ResourceId status = WAVE_MESSAGE_ERROR;
    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

    // check if serial number to deactivate exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
        startTransaction ();
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
        pVcsSwitchSerialNumberMappedIdManagedObject->m_switchDeleteAfterFailover = true;
        updateWaveManagedObject (pVcsSwitchSerialNumberMappedIdManagedObject);
        status = commitTransaction ();
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsSwitchSerialNumberMappedIdManagedObject::setSwitchDeleteAfterFailoverUseSNInSNDB : Set flag for Serial Number :" + switchSerialNumber);
            return VCS_SUCCESS;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::setSwitchDeleteAfterFailoverUseSNInSNDB : Failed to to set flag for SerialNumber :" + switchSerialNumber);
            return ADD_SWITCH_SERIAL_NUMBER_FAILED;
        }
    }
    else // switch serial number not in database
    {
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
        return SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB;
    }
}


UI32 VcsSwitchSerialNumberMappedIdManagedObject::reActivateSwitchSerialNumberInSNDB (const string &switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::reActivateSwitchSerialNumber : Entering ...");
    ResourceId status = WAVE_MESSAGE_ERROR;
    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

    // check if serial number to add already exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    { // switch serial number exists and is active
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
        if (pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive)
        {
            pVcsFabObjectManager->deleteObjects (pResultsForSN);
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::reActivateSwitchSerialNumberToObject : Serial Number already in VCS cluster :" + switchSerialNumber);
            return SERIAL_NUMBER_ALREADY_IN_MAPPED_ID_SNDB;
        }
        else // switch serial number exists and is inactive so activate it
        {
            startTransaction ();
            pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive = true;
            updateWaveManagedObject (pVcsSwitchSerialNumberMappedIdManagedObject);
            status = commitTransaction ();
            pVcsFabObjectManager->deleteObjects (pResultsForSN);
            if (FRAMEWORK_SUCCESS == status)
            {
                WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsSwitchSerialNumberMappedIdManagedObject::reActivateSwitchSerialNumberToObject : Serial Number re-activated in database :" + switchSerialNumber);
                return VCS_SUCCESS;
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::reActivateSwitchSerialNumberToObject : Failed to re-activate existing record for SerialNumber :" + switchSerialNumber);
                return ADD_SWITCH_SERIAL_NUMBER_FAILED;
            }
        }
    }
    else // switch serial number not in database
    {
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
		return SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB;
    }
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchSerialNumberInSNDB  (const string &switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchSerialNumberFromSNDB : Entering ...");
    ResourceId status = WAVE_MESSAGE_ERROR;
    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

    // check if serial number to deactivate exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
        startTransaction ();
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
        pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive = false;
        updateWaveManagedObject (pVcsSwitchSerialNumberMappedIdManagedObject);
        status = commitTransaction ();
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchSerialNumberFromSNDB : Serial Number re-activated in database :" + switchSerialNumber);
            return VCS_SUCCESS;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchSerialNumberFromSNDB : Failed to re-activate existing record for SerialNumber :" + switchSerialNumber);
            return ADD_SWITCH_SERIAL_NUMBER_FAILED;
        }
    }
    else // switch serial number not in database
    {
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
        return SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB;
    }
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchSerialNumberFromSNDB  (const string &switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchSerialNumberFromSNDB : Entering ...");
    ResourceId status = WAVE_MESSAGE_ERROR;

    // check if serial number to remove exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
		WaveNs::trace (TRACE_LEVEL_INFO, "VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchSerialNumberFromSNDB removing SN "+switchSerialNumber);
        startTransaction ();
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
		delete pVcsSwitchSerialNumberMappedIdManagedObject;
        status = commitTransaction ();
		delete pResultsForSN;
        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchSerialNumberFromSNDB : Successfully removed SN from SNDB :" + switchSerialNumber);
            return VCS_SUCCESS;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchSerialNumberFromSNDB : Failed to remove existing record for SerialNumber :" + switchSerialNumber);
            return ADD_SWITCH_SERIAL_NUMBER_FAILED;
        }
    }
    else // switch serial number not in database
    {
		WaveNs::trace (TRACE_LEVEL_INFO, "VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchSerialNumberFromSNDB no record found for SN: "+switchSerialNumber);
		delete pResultsForSN;
        return SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB;
    }
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB  (UI32 mappedId)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB : Entering ...");
    ResourceId status = WAVE_MESSAGE_ERROR;

    // check if mappedId to remove exists
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "mappedId"));
    vector<WaveManagedObject *> *pResultsForMappedId = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForMappedId, __FILE__, __LINE__);
    UI32 count = pResultsForMappedId->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
		WaveNs::trace (TRACE_LEVEL_INFO, string("VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB removing mappedId ") + mappedId);
        startTransaction ();
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForMappedId)[0]);
		delete pVcsSwitchSerialNumberMappedIdManagedObject;
        status = commitTransaction ();
		delete pResultsForMappedId;
        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, string ("VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB : Successfully removed mappedId = ")+mappedId+"  from SNDB");
            return VCS_SUCCESS;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB : Failed to remove existing record for mappedId :") + mappedId);
            return ADD_SWITCH_SERIAL_NUMBER_FAILED;
        }
    }
    else // switch mappedId not in database
    {
		WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsSwitchSerialNumberMappedIdManagedObject::removeSwitchMappedIdFromSNDB no record found for mappedId = ") + mappedId);
		delete pResultsForMappedId;
        return MAPPED_ID_NOT_IN_MAPPED_ID_SNDB;
    }
}

SNDBObjectStates VcsSwitchSerialNumberMappedIdManagedObject::getSNDBObjectStateForSN (const string switchSerialNumber, UI32 &mappedId)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::getSNDBObjectState : Entering ...");
	WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
	UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count, __FILE__, __LINE__);
 	VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

	if (0 == count)
	{
		pVcsFabObjectManager->deleteObjects (pResultsForSN);
		mappedId = 0;
		return NOT_IN_SNDB;
	}
	VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = 
		dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
    mappedId = pVcsSwitchSerialNumberMappedIdManagedObject->m_mappedId;
	if (pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive)
	{
		pVcsFabObjectManager->deleteObjects (pResultsForSN);
		return IN_SNDB_ACTIVE;
	}
	else
	{
	    if (!pVcsSwitchSerialNumberMappedIdManagedObject->m_switchDeleteAfterFailover) {
    		pVcsFabObjectManager->deleteObjects (pResultsForSN);
    		return IN_SNDB_INACTIVE;
        } else {
            /* 
                         - Rejoin before failover complete : No delete is needed, so reset the flag
                         - Failover complete before any other operation : delete is need, so the objec will be gone. No hurt to reset flag
                      */
            setSwitchDeleteAfterFailoverInSNDB(false);
            pVcsFabObjectManager->deleteObjects (pResultsForSN);
    		return IN_SNDB_DELETE_PENDING;
        }
	}
}


UI32 VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB (const UI32 mappedId)
{

    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : Entering ...");
	WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "mappedId"));
    vector<WaveManagedObject *> *pResultsForMappedId = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForMappedId, __FILE__, __LINE__);

    UI32 count = pResultsForMappedId->size ();
	WaveNs::trace (TRACE_LEVEL_INFO, string("VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : de-activating mappedID = ") + mappedId + " record count = "+count);

    prismAssert (1 >= count, __FILE__, __LINE__);
 	VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

	if (1 == count)
	{
        ResourceId status = WAVE_MESSAGE_ERROR;
        VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForMappedId)[0]);
		string switchSerialNumber =     pVcsSwitchSerialNumberMappedIdManagedObject->m_switchSerialNumber;
	    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : starting transaction"));

		startTransaction ();
        pVcsSwitchSerialNumberMappedIdManagedObject->m_switchActive = false;
        updateWaveManagedObject (pVcsSwitchSerialNumberMappedIdManagedObject);
        status = commitTransaction ();

	    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : completed transaction"));

        pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
        if (FRAMEWORK_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : Serial Number de-activated in database :" + switchSerialNumber);
            return VCS_SUCCESS;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsSwitchSerialNumberMappedIdManagedObject::deActivateSwitchMappedIdFromSNDB : Failed to de-activate existing record for SerialNumber :" + switchSerialNumber);
            return DEACTIVATE_SWITCH_SERIAL_NUMBER_FAILED;
        }
	}
    pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
    return     SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB;
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::getMappedIdForThisSerialNumber  (const string switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::getMappedIdForThisSerialNumber : Entering ...");
    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();

    // get record for this serial number
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (switchSerialNumber, "switchSerialNumber"));
    vector<WaveManagedObject *> *pResultsForSN = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForSN, __FILE__, __LINE__);
    UI32 count = pResultsForSN->size ();
    prismAssert (1 >= count,__FILE__, __LINE__);
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

    if (1 == count)
    {
        pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForSN)[0]);
		UI32 mappedId = pVcsSwitchSerialNumberMappedIdManagedObject->m_mappedId;
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
		return mappedId;
    }
    else // switch serial number not in database
    {
        pVcsFabObjectManager->deleteObjects (pResultsForSN);
        return 0;
    }
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::getSerialNumberForthisMappedId      (const UI32 mappedId, string &switchSerialNumber)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::getSwitchIpAddressFromMappedId : Entering ...");
	WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "mappedId"));
    vector<WaveManagedObject *> *pResultsForMappedId = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForMappedId, __FILE__, __LINE__);
    UI32 count = pResultsForMappedId->size ();
 	VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

	switch (count)
	{
		case 0:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("SNDB object not found for mappedId: ")+mappedId);
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return     SWITCH_NOT_IN_MAPPED_ID_SNDB;
		case 1:
			pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForMappedId)[0]);
			switchSerialNumber =  pVcsSwitchSerialNumberMappedIdManagedObject->m_switchSerialNumber;		
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return VCS_SUCCESS;
		default:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("More than one SNDB objects for mappedId: ")+mappedId+" count = "+count);
	       return     MULTIPLE_SWITCHES_WITH_SAME_MAPPED_ID_FOUND;
	}
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchSerialNumber  (string switchSerialNumber)
{
	m_switchSerialNumber = switchSerialNumber;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setMappedId  (UI32 mappedId)
{
	m_mappedId = mappedId;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchActiveOrInactive  (bool activeOrInactive)
{
	m_switchActive = activeOrInactive;
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::getMappedId ()
{
    return m_mappedId;
}
bool VcsSwitchSerialNumberMappedIdManagedObject::getSwitchActiveOrInactive  ()
{
	return m_switchActive;
}

string VcsSwitchSerialNumberMappedIdManagedObject::getSwitchIpAddressFromSNDB ()
{
	return m_switchIpAddress;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchIpAddressInSNDB (const string &switchIpAddress)
{
	m_switchIpAddress = switchIpAddress;
}
UI32 VcsSwitchSerialNumberMappedIdManagedObject::getSwitchPortFromSNDB ()
{
	return m_switchPortId;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchPortInSNDB (const UI32 &switchPortId)
{
	m_switchPortId = switchPortId;
}

string VcsSwitchSerialNumberMappedIdManagedObject::getSwitchMgmtIpAddressFromSNDB ()
{
	return m_switchMgmtIpAddress;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchMgmtIpAddressInSNDB (const string &switchMgmtIpAddress)
{
	m_switchMgmtIpAddress = switchMgmtIpAddress;
}
UI32 VcsSwitchSerialNumberMappedIdManagedObject::getSwitchMgmtPortFromSNDB ()
{
	return m_switchMgmtPortId;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchMgmtPortInSNDB (const UI32 &switchMgmtPortId)
{
	m_switchMgmtPortId = switchMgmtPortId;
}

bool VcsSwitchSerialNumberMappedIdManagedObject::getSwitchDeleteAfterFailoverFromSNDB ()
{
	return m_switchDeleteAfterFailover;
}

void VcsSwitchSerialNumberMappedIdManagedObject::setSwitchDeleteAfterFailoverInSNDB (const bool &switchDeleteAfterFailover)
{
	m_switchDeleteAfterFailover = switchDeleteAfterFailover;
}



UI32 VcsSwitchSerialNumberMappedIdManagedObject::getSwitchIpAddressFromMappedId (const UI32 mappedId, string &switchIpAddress)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::getSwitchIpAddressFromMappedId : Entering ...");
	WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "mappedId"));
    vector<WaveManagedObject *> *pResultsForMappedId = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForMappedId, __FILE__, __LINE__);
    UI32 count = pResultsForMappedId->size ();
 	VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

	switch (count)
	{
		case 0:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("SNDB object not found for mappedId: ")+mappedId);
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return     SWITCH_NOT_IN_MAPPED_ID_SNDB;
		case 1:
			pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForMappedId)[0]);
			switchIpAddress =  pVcsSwitchSerialNumberMappedIdManagedObject->m_switchIpAddress;		
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return VCS_SUCCESS;
		default:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("More than one SNDB objects for mappedId: ")+mappedId+" count = "+count);
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return     MULTIPLE_SWITCHES_WITH_SAME_MAPPED_ID_FOUND;
	}	
}

UI32 VcsSwitchSerialNumberMappedIdManagedObject::getSwitchPortIdFromMappedId (const UI32 mappedId, UI32 &switchPortId)
{
    trace (TRACE_LEVEL_DEBUG, "VcsSwitchSerialNumberMappedIdManagedObject::getSwitchIpAddressFromMappedId : Entering ...");
	WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (this->getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "mappedId"));
    vector<WaveManagedObject *> *pResultsForMappedId = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsForMappedId, __FILE__, __LINE__);
    UI32 count = pResultsForMappedId->size ();
 	VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
    VcsSwitchSerialNumberMappedIdManagedObject *pVcsSwitchSerialNumberMappedIdManagedObject = NULL;

	switch (count)
	{
		case 0:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("SNDB object not found for mappedId: ")+mappedId);
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return     SWITCH_NOT_IN_MAPPED_ID_SNDB;
		case 1:
			pVcsSwitchSerialNumberMappedIdManagedObject = dynamic_cast<VcsSwitchSerialNumberMappedIdManagedObject *> ((*pResultsForMappedId)[0]);
			switchPortId =  pVcsSwitchSerialNumberMappedIdManagedObject->m_switchPortId;		
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
			return VCS_SUCCESS;
		default:
			WaveNs::trace (TRACE_LEVEL_ERROR, string("More than one SNDB objects for mappedId: ")+mappedId+" count = "+count);
			pVcsFabObjectManager->deleteObjects (pResultsForMappedId);
	       return     MULTIPLE_SWITCHES_WITH_SAME_MAPPED_ID_FOUND;
	}
}


}
#endif
