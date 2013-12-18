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

/* *************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Firmware/Global/NodeAutoUpgradeManagedObject.h"
#include "Firmware/Global/NodeAutoUpgradeConfiguration.h"
#include "Firmware/Global/FirmwareToolKit.h"
#include "Firmware/Global/FirmwareTypes.h"

#include "hasm/sus.h"

namespace DcmNs
{

void FirmwareToolKit::getNodeAutoUpgradeConfigData(NodeAutoUpgradeConfiguration *pUpgradeConfigData)
{

	vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously(NodeAutoUpgradeManagedObject::getClassName());

	NodeAutoUpgradeManagedObject *pUpgradeMO =
		dynamic_cast<NodeAutoUpgradeManagedObject *> ((*pResults)[0]);;
	prismAssert (NULL != pUpgradeMO, __FILE__, __LINE__);

	pUpgradeConfigData->setStatus(pUpgradeMO->getStatus());
	pUpgradeConfigData->setFirmwareVersion(pUpgradeMO->getFirmwareVersion());
	pUpgradeConfigData->setHost(pUpgradeMO->getIpAddress());
	pUpgradeConfigData->setPath(pUpgradeMO->getPath());
	pUpgradeConfigData->setProtocol(pUpgradeMO->getProtocol());
	pUpgradeConfigData->setUsername(pUpgradeMO->getUsername());
	pUpgradeConfigData->setPassword(pUpgradeMO->getPassword());
	pUpgradeConfigData->setMsgVersion(pUpgradeMO->getMsgVersion());
	pUpgradeConfigData->setFlags(pUpgradeMO->getFlags());

	if (pUpgradeMO)
		delete pUpgradeMO;

}

int FirmwareToolKit::getUpgradeStatus()
{
	return (sus_fwdl_state());
}

void FirmwareToolKit::checkPrincipalVersionVersusConfiguredVersion(NodeAutoUpgradeConfiguration *pUpgradeConfigData)
{
	string	firmware_version;
	UI32	flags;
	fwversion_t current_version;

		/* Get Current Version */
		memset(&current_version, 0, sizeof(fwversion_t));
		version_get(NULL, (char *)"base", &current_version);
		firmware_version = pUpgradeConfigData->getFirmwareVersion();

		/* Verify with the configured Version */
		if (strcmp(current_version.fw_verstr, firmware_version.c_str()) != 0) {
			trace (TRACE_LEVEL_INFO, "FirmwareToolKit::checkPrincipalVersionVersusConfiguredVersion:"
				" Principal version & Configured Version are different.");
			flags = pUpgradeConfigData->getFlags() | NA_DIFF_VERSION;
			pUpgradeConfigData->setFlags(flags);
		}
}

/*ResourceId FirmwareToolKit::setNodeAutoUpgradeConfigData(NodeAutoUpgradeConfiguration *pUpgradeConfigData)
{

	ResourceId	status = WAVE_MESSAGE_ERROR;

	vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously(NodeAutoUpgradeManagedObject::getClassName());

	NodeAutoUpgradeManagedObject *pUpgradeMO =
		dynamic_cast<NodeAutoUpgradeManagedObject *> ((*pResults)[0]);;
	prismAssert (NULL != pUpgradeMO, __FILE__, __LINE__);

	pUpgradeMO->setStatus(pUpgradeConfigData->getStatus());
	pUpgradeMO->setFirmwareVersion(pUpgradeConfigData->getFirmwareVersion());
	pUpgradeMO->setIpAddress(pUpgradeConfigData->getHost());
	pUpgradeMO->setPath(pUpgradeConfigData->getPath());
	pUpgradeMO->setProtocol(pUpgradeConfigData->getProtocol());
	pUpgradeMO->setUsername(pUpgradeConfigData->getUsername());
	pUpgradeMO->setPassword(pUpgradeConfigData->getPassword());
	pUpgradeMO->setMsgVersion(pUpgradeConfigData->getMsgVersion());
	pUpgradeMO->setFlags(pUpgradeConfigData->getFlags());

	startTransaction();
	WaveObjectManager::updateWaveManagedObject(pUpgradeMO);
	status = WaveObjectManager::commitTransaction();

	if (pUpgradeMO)
		delete pUpgradeMO;

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return status;
}*/




}
