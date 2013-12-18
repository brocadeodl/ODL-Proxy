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

/*
*	CPP file to store the firmware download sanity results as MO.
*	This file has the necessary get/set methods for the sanity results.
*
*/

#include "Firmware/Global/FirmwareDownloadStatusManagedObject.h"
#include "Firmware/Global/FirmwareObjectManager.h"

namespace DcmNs
{

	FirmwareDownloadStatusManagedObject::FirmwareDownloadStatusManagedObject (FirmwareObjectManager *pFirmwareObjectManager)
		: PrismElement  (pFirmwareObjectManager),
		PrismPersistableObject (FirmwareDownloadStatusManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pFirmwareObjectManager),
		DcmManagedObject (pFirmwareObjectManager)
	{
	}

	FirmwareDownloadStatusManagedObject::~FirmwareDownloadStatusManagedObject()
	{
	}

	string FirmwareDownloadStatusManagedObject::getClassName()
	{
		return("FirmwareDownloadStatusManagedObject");
	}

	void FirmwareDownloadStatusManagedObject::setupAttributesForPersistence()
	{
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_rbridge_id_local, "rbridge_id_local"));
		addPersistableAttribute (new AttributeSI32(&m_status, "status"));
		addPersistableAttribute (new AttributeString(&m_mesg, "mesg"));
		addPersistableAttribute (new AttributeUI32(&m_rbridge_id_principal, "rbridge_id_principal"));
		addPersistableAttribute (new AttributeString(&m_time, "time"));
	}

	void FirmwareDownloadStatusManagedObject::setupAttributesForCreate()
	{
		DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_rbridge_id_local, "rbridge_id_local"));
		addPersistableAttributeForCreate (new AttributeSI32(&m_status, "status"));
		addPersistableAttributeForCreate (new AttributeString(&m_mesg, "mesg"));
		addPersistableAttributeForCreate (new AttributeUI32(&m_rbridge_id_principal, "rbridge_id_principal"));
		addPersistableAttributeForCreate (new AttributeString(&m_time, "time"));
	}

	void FirmwareDownloadStatusManagedObject::setRbridge_Id_Local(const UI32 &rbridge_id)
	{
		m_rbridge_id_local  =  rbridge_id;
	}

	UI32 FirmwareDownloadStatusManagedObject::getRbridge_Id_Local() const
	{
		return (m_rbridge_id_local);
	}

	void FirmwareDownloadStatusManagedObject::setStatus(const int &status)
	{
		m_status = status;
	}

	int FirmwareDownloadStatusManagedObject::getStatus() const
	{
		return (m_status);
	}

	void FirmwareDownloadStatusManagedObject::setMesg(const string &mesg)
	{
		m_mesg = mesg;
	}

	string FirmwareDownloadStatusManagedObject::getMesg() const
	{
		return (m_mesg);
	}

	void FirmwareDownloadStatusManagedObject::setRbridge_Id_Principal(const UI32 &rbridge_id)
	{
		m_rbridge_id_principal  =  rbridge_id;
	}

	UI32 FirmwareDownloadStatusManagedObject::getRbridge_Id_Principal() const
	{
		return (m_rbridge_id_principal);
	}

	void FirmwareDownloadStatusManagedObject::setTime(const string &time)
	{
		m_time = time;
	}

	string FirmwareDownloadStatusManagedObject::getTime() const
	{
		return (m_time);
	}
}
