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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sharis                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "vCenter/Framework/vCenterLocalObjectManager.h"
#include "vCenter/User/vCenterLocalWorkers.h"
#include "vCenter/Framework/vCenterLocalMessages.h"
#include "vCenter/Framework/vCenterLocalTypes.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "vCenter/User/vCenterPluginDebug.h"
#include "discover/User/discoverDcmDef.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "discover/utils/FileUtils.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

#include <iostream>
#include <fstream>

namespace DcmNs {

VcenterLocalWorker::VcenterLocalWorker(
		vCenterLocalObjectManager *pvCenterLocalObjectManager) :
		WaveWorker(pvCenterLocalObjectManager) {
	addOperationMap(
			VCENTERVCENTER,
			reinterpret_cast<PrismMessageHandler>(&VcenterLocalWorker::vCenterVcenterMessageHandler));
}

VcenterLocalWorker::~VcenterLocalWorker() {
}

PrismMessage *VcenterLocalWorker::createMessageInstance(
		const UI32 &operationCode) {
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode) {

	case VCENTERVCENTER:
		pPrismMessage = new vCenterVcenterMessage();
		break;
	default:
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject *VcenterLocalWorker::createManagedObjectInstance(
		const string &managedClassName) {
	return NULL;
}

void VcenterLocalWorker::listenForEvents(
		WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases) {

	listenForEvent(
			PersistenceLocalObjectManager::getPrismServiceId(),
			STARTUP_SCHEMA_CHANGE_EVENT,
			reinterpret_cast<PrismEventHandler>(&VcenterLocalWorker::persistDataFiles));

	listenForEvent(
			PrismFrameworkObjectManager::getPrismServiceId(),
			FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT,
			reinterpret_cast<PrismEventHandler>(&VcenterLocalWorker::handleNodeReadyForCommands));

	pWaveAsynchronousContextForBootPhases->setCompletionStatus(
			WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForBootPhases->callback();
}

VcenterManagedObject*
VcenterLocalWorker::getVcenterConfigFromMo ()
{
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VcenterManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    VcenterManagedObject* pMO = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
        } else if (1 == numberOfResults) {
            pMO = dynamic_cast<VcenterManagedObject*>((*pResults)[0]);
        } else {
            /* Multiple Vcenter is not supported hence release the
             *  memory and return NULL
             */
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    return pMO;
}

void VcenterLocalWorker::externalStateSynchronization() {

	trace(TRACE_LEVEL_INFO,
			"Check to enable VCENTER HasmSetMvCFeature in node ready stage");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	ValClientSynchronousConnection *pValClientSynchronousConnection;

	UI32 numberOfResults = pResults->size();
	if (numberOfResults > 0) {
		trace(TRACE_LEVEL_INFO,
				"Enable VCENTER HasmSetMvCFeature in node ready stage");
		pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
		pValClientSynchronousConnection->HasmSetMvCFeature(true);
	}
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

}

void VcenterLocalWorker::handleNodeReadyForCommands(
		const NodeReadyForAllCommandsEvent* &pEvent) {
	string vnames;
	VCENTER_PLUG_DBG(string("Node is ready to handle config commands.."));

	externalStateSynchronization();

	/* Event reply to framework */
	reply(reinterpret_cast<const PrismEvent *&>(pEvent));

	VcenterManagedObject* pVcenterManagedObject = getVcenterConfigFromMo();
	if (pVcenterManagedObject != NULL) {
		vnames = pVcenterManagedObject->getId();
	} else {
		VCENTER_PLUG_DBG(string("Vcenter config not available in MO"));
		return;
	}

	ResourceId mode = DcmToolKit::getThisVcsNodeClusterType();
	LocationRole role = FrameworkToolKit::getThisLocationRole();
	ResourceId dmode = DcmToolKit::getDistributionMode();

	if ((dmode == CCM_ONLY) && (mode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT)
			&& (role == LOCATION_SECONDARY)) {
		VCENTER_PLUG_DBG(string("FC Cluster non-Primary node"));

		DiscoverLocalDiscoverMessage *m = new DiscoverLocalDiscoverMessage();

		m->setVcenter(vnames);
		m->setScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
		m->setCmdcode(DISC_CMD_DISCOVER_ALL_VCENTER);
		m->setMsgType(DISC_MSG_TYPE_UPDATE);

		sendOneWay(m);

		VCENTER_PLUG_DBG(string("Async msg sent for pp create"));
	}

	/* Release Memory */
	delete pVcenterManagedObject;
}

void VcenterLocalWorker::persistDataFiles(
		const StartupSchemaChangeEvent* &pEvent) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
	VCENTER_PLUG_DBG(string("Node has changed Startup Configuration."));

	const SchemaType fromSchema = pEvent->getFromSchema();
	const SchemaType toSchema = pEvent->getToSchema();

	/* Event reply to framework */
	reply(reinterpret_cast<const PrismEvent *&>(pEvent));

	if (!(FileUtils::isvCenterInManagementCluster())) //for MC case file are already there
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
				VcenterManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously(
				&syncQueryCtxt);
		string vname;

		if (toSchema == STARTUP_SCHEMA) {
			trace(TRACE_LEVEL_INFO, string("toSchema==STARTUP_SCHEMA"));
			if (fromSchema == RUNNING_SCHEMA) {
				trace(TRACE_LEVEL_INFO, string("fromSchema==RUNNING_SCHEMA"));
				if (NULL != pResults) {
					UI32 numberOfResults = pResults->size();
					for (UI32 i = 0; i < numberOfResults; i++) {
						VcenterManagedObject* pMO =
								dynamic_cast<VcenterManagedObject*>((*pResults)[i]);
						vname = pMO->getId();
						string vcenterDB = FileUtils::getDBLocation(vname);
						string per_vcenterDB =
								FileUtils::getPersistentDBLocation(vname);
						if (FileUtils::fileexists(vcenterDB.c_str()))
							FileUtils::copyfile(vcenterDB.c_str(),
									per_vcenterDB.c_str());
					}
					trace(TRACE_LEVEL_INFO,
							string("Copied vcenter files: ") + numberOfResults);
				}
			} else {
				//remove persistent file
				trace(TRACE_LEVEL_INFO, string("Removing VCENTER"));
				if (NULL != pResults) {
					UI32 numberOfResults = pResults->size();
					for (UI32 i = 0; i < numberOfResults; i++) {
						VcenterManagedObject* pMO =
								dynamic_cast<VcenterManagedObject*>((*pResults)[i]);
						vname = pMO->getId();
						string per_vcenterDB =
								FileUtils::getPersistentDBLocation(vname);
						if (FileUtils::fileexists(per_vcenterDB.c_str())) {
							FileUtils::remove(per_vcenterDB.c_str());
							trace(
									TRACE_LEVEL_INFO,
									string("Removed vcenter file: ")
											+ vname.c_str());
						}
					}

				}
			}
		}
		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	} else { // MC cluster case
		// If startup is copied anything other than running config clean up the DB files

		if ((toSchema == STARTUP_SCHEMA) && (fromSchema != RUNNING_SCHEMA)) {
			// Remove all files in DB directory path
			FileUtils::removeDBFiles(MC_VCENTER_DIR);
			FileUtils::removeDBFiles(MNT_MSC_VCENTER_DIR);
		}
	}

}

void VcenterLocalWorker::vCenterVcenterMessageHandler(
		vCenterVcenterMessage *pvCenterVcenterMessage) {
	ValClientSynchronousConnection *pValClientSynchronousConnection;
	trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

	PrismLinearSequencerStep sequencerSteps[] =
			{
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterLocalWorker::doLocalConfigStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterLocalWorker::prismLinearSequencerStartTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterLocalWorker::prismLinearSequencerCommitTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterLocalWorker::prismLinearSequencerSucceededStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterLocalWorker::prismLinearSequencerFailedStep) };

	trace(TRACE_LEVEL_INFO, string("Inside local worker MsgHandler:"));
	PrismLinearSequencerContext *pPrismLinearSequencerContext =
			new PrismLinearSequencerContext(pvCenterVcenterMessage, this,
					sequencerSteps,
					sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

	// eanble vCenter feature through HASM
	pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
	prismAssert(NULL != pValClientSynchronousConnection, __FILE__, __LINE__);
	if (pvCenterVcenterMessage->getDeleteFlag() == false) {
		pValClientSynchronousConnection->HasmSetMvCFeature(true);
		trace(TRACE_LEVEL_INFO, string("Setting HASMMvCFeature"));
	} else {
		pValClientSynchronousConnection->HasmSetMvCFeature(false);
		trace(TRACE_LEVEL_INFO, string("Unsetting HASMMvCFeature"));
	}

	pPrismLinearSequencerContext->start();
}

void VcenterLocalWorker::doLocalConfigStep(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

	vCenterVcenterMessage *pmsg =
			dynamic_cast<vCenterVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	createConfigPhpFile(pmsg);

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	return;
}

ResourceId VcenterLocalWorker::createConfigPhpFile(vCenterVcenterMessage *m) {
	trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

	string ipaddr, username, password, id, discover;
	ofstream outFile;
	const char *outputFile = "/fabos/webtools/htdocs/ws/config/config.php";

	id = m->getId();
	ipaddr = m->getIpaddress();
	username = m->getUsername();
	password = m->getPassword();

	m->getActivate() ? discover = "discover" : discover = "";

	outFile.open(outputFile, ios::out);

	outFile << "<?php\n\n";

	outFile << "$vcenter  = \"" << id << "\";\n";
	outFile << "$url      = \"" << ipaddr << "\";\n"; // XXX
	outFile << "$user     = \"" << username << "\";\n";
	outFile << "$pass     = \"" << password << "\";\n";
	outFile << "$activate = \"" << discover << "\";\n";
	outFile << "\n?>\n";

	outFile.close();

	return WAVE_MESSAGE_SUCCESS;
}
}
