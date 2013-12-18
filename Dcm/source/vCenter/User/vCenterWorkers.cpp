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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sharis                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include <sys/stat.h>

#include "vCenter/Framework/vCenterObjectManager.h"
#include "vCenter/Framework/vCenterMessages.h"
#include "vCenter/Framework/vCenterLocalMessages.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "vCenter/Framework/vCenterTypes.h"
#include "vCenter/User/vCenterWorkers.h"
#include "vCenter/User/vCenterToolkit.h"
#include "vCenter/User/vCenterPluginDebug.h"

#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/EventEngineMessages.h"
#include "discover/Framework/EventEngineManagementInterfaceMessages.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverManagedObjects.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "discover/User/discoverDcmDef.h"
#include "discover/User/discoverLocalWorkers.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "correlator/VcenterDiff.h"
#include "vCenter/User/vCenterDcmDef.h"

#include <iostream>
#include <fstream>
using std::ifstream;

#include <signal.h>
#include <raslog/raslog.h>
#include <raslog/raslog_vcenter.h>
#include "vCenter/User/vCenterDcmDef.h"

#include "vcs.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include "vCenter/User/vCenterPluginDebug.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/utils/FileUtils.h"

#define DBFILE          ((char *) "/tmp/dc.db")
#define DBFILE_NEW      ((char *) "/tmp/dc.db.new")
#define PER_DBFILE      ((char *) "/var/tmp/dc.db")
#define PER_DBFILE_NEW  ((char *) "/var/tmp/dc.db.new")
//#define DBFILE_ORIG     ((char *) "/fabos/webtools/htdocs/ws/config/dc.db")

namespace DcmNs {

VcenterWorker::VcenterWorker(vCenterObjectManager *pvCenterObjectManager) :
		WaveWorker(pvCenterObjectManager) {
	VcenterManagedObject VcenterManagedObject(pvCenterObjectManager);
	VcenterManagedObject.setupOrm();
	addManagedClass(VcenterManagedObject::getClassName(), this);
	addOperationMap(
			VCENTERCREATEVCENTER,
			reinterpret_cast<PrismMessageHandler>(&VcenterWorker::vCenterCreateVcenterMessageHandler));
	addOperationMap(
			VCENTERUPDATEVCENTER,
			reinterpret_cast<PrismMessageHandler>(&VcenterWorker::vCenterUpdateVcenterMessageHandler));
	addOperationMap(
			VCENTERDELETEVCENTER,
			reinterpret_cast<PrismMessageHandler>(&VcenterWorker::vCenterDeleteVcenterMessageHandler));
}

VcenterWorker::~VcenterWorker() {
}

PrismMessage *VcenterWorker::createMessageInstance(const UI32 &operationCode) {
	PrismMessage *pPrismMessage = NULL;

	trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
	switch (operationCode) {

	case VCENTERCREATEVCENTER:
		pPrismMessage = new vCenterCreateVcenterMessage();
		break;
	case VCENTERUPDATEVCENTER:
		pPrismMessage = new vCenterUpdateVcenterMessage();
		break;
	case VCENTERDELETEVCENTER:
		pPrismMessage = new vCenterDeleteVcenterMessage();
		break;
	default:
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
}

WaveManagedObject *VcenterWorker::createManagedObjectInstance(
		const string &managedClassName) {
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((VcenterManagedObject::getClassName()) == managedClassName) {
		pWaveManagedObject = new VcenterManagedObject(
				dynamic_cast<vCenterObjectManager *>(getPWaveObjectManager()));
	} else {
		trace(
				TRACE_LEVEL_FATAL,
				"VcenterWorker::createManagedObjectInstance : Unknown Managed Class Name : "
						+ managedClassName);
	}
	return (pWaveManagedObject);
}

void VcenterWorker::vCenterCreateVcenterMessageHandler(
		vCenterCreateVcenterMessage *pvCenterCreateVcenterMessage) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	PrismLinearSequencerStep sequencerSteps[] =
			{
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::doGlobalConfigStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerStartTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::createManagedObject),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerCommitTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerSucceededStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerFailedStep) };

	PrismLinearSequencerContext *pPrismLinearSequencerContext =
			new PrismLinearSequencerContext(pvCenterCreateVcenterMessage, this,
					sequencerSteps,
					sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
	pPrismLinearSequencerContext->holdAll();
	pPrismLinearSequencerContext->start();
}

void VcenterWorker::nodeAddedEventHandler(
		const WaveNewNodesAddedEvent* &pEvent) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	UI32 i = 0, count=0;
	string vnames;
	LocationId locationid;

	/* This check can be removed once the file serice issue(jay is working) is fixed
	 */

	vCenterToolkit::getNumberVcenterConfigFromMo(count);

	if (count == 0) {
		VCENTER_PLUG_DBG(string("Vcenter config missing, No operation"));
		/* Event reply to framework */
		reply(reinterpret_cast<const PrismEvent *&>(pEvent));
		return;
	}

	ResourceId distributionMode = DcmToolKit::getDistributionMode();
	DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
	if (distributionMode == CCM_ONLY) {
		VCENTER_PLUG_DBG(
				string("Cleanup the existing config and sync with mother cluster pp configs"));
		dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
	} else {
		dmsg->setLocalScriptNames(DISC_COPY_SCRIPTS);
	}
	dmsg->setCmdcode(DISC_CMD_NODE_ADD);

	for (i = 0; i < pEvent->getNumberOfNewLocations(); i++) {
		locationid = pEvent->getLocationIdAtIndex(i);
		VCENTER_PLUG_DBG(string("Location Id ") + locationid);
		dmsg->setLocationIds(locationid);
	}

	VCENTER_PLUG_DBG(
			string("Async msg send from vcenter plugin to discover plugin"));
	sendOneWay(dmsg);
	/* Event reply to framework */
	reply(reinterpret_cast<const PrismEvent *&>(pEvent));
}

void VcenterWorker::primaryChangedEventHandler(
		const PrimaryChangedEvent* &pEvent) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	string vnames;
	const LocationId newPrimaryLocation = pEvent->getNewPrimaryLocationId();

	VCENTER_PLUG_DBG(
			string("New primary has been elected in the cluster LocationId") + newPrimaryLocation);
	/* Event reply to framework */
	reply(reinterpret_cast<const PrismEvent *&>(pEvent));

	vector<VcenterManagedObject *> vcenterList =
			vCenterToolkit::getVcenterConfigObjects();

	for (size_t i = 0; i < vcenterList.size(); i++) {

		VcenterManagedObject* pVcenterManagedObject = vcenterList.at(i);

		if ((pVcenterManagedObject->getActivate() == true)) {
			vnames = pVcenterManagedObject->getId();
			VCENTER_PLUG_DBG(string("Config available and activated in MO"
			"for vcenter ") + vnames);

			/* start discovery, if vcenter is configured and activated*/
			DiscoverDiscoverMessage *dmsg_d = new DiscoverDiscoverMessage();
			dmsg_d->setVcenter(vnames);
			dmsg_d->setUrl(pVcenterManagedObject->getIpaddress());
			dmsg_d->setUsername(pVcenterManagedObject->getUsername());
			dmsg_d->setPassword(pVcenterManagedObject->getPassword());
			dmsg_d->setCmdcode(DISC_CMD_DISCOVER_ALL_VCENTER);
			dmsg_d->setMsgType(DISC_MSG_TYPE_CREATE);

			sendOneWay(dmsg_d);
			VCENTER_PLUG_DBG(
					string("Async msg send  for first time discovery from vcenter plugin to discover plugin"));
		}

		/* Release Memory */
		delete pVcenterManagedObject;
	}

}

void VcenterWorker::listenForEvents(
		WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	listenForEvent(
			PrismFrameworkObjectManager::getPrismServiceId(),
			FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT,
			reinterpret_cast<PrismEventHandler>(&VcenterWorker::startDiscoveryOnBootup));

	listenForEvent(
			PrismFrameworkObjectManager::getPrismServiceId(),
			FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT,
			reinterpret_cast<PrismEventHandler>(&VcenterWorker::nodeAddedEventHandler));

	listenForEvent(
			PrismFrameworkObjectManager::getPrismServiceId(),
			FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_EVENT,
			reinterpret_cast<PrismEventHandler>(&VcenterWorker::primaryChangedEventHandler));

	pWaveAsynchronousContextForBootPhases->setCompletionStatus(
			WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForBootPhases->callback();
}

/* This code works for only single vcenter */
void VcenterWorker::createFreshConfigFile(VcenterManagedObject *pMO,
		LocationId locationid) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int ret;

	/* local message */
	vCenterVcenterMessage *m = new vCenterVcenterMessage();
	if (pMO != NULL) {
		m->setId(pMO->getId());
		m->setUsername(pMO->getUsername());
		m->setPassword(pMO->getPassword());
		m->setIpaddress(pMO->getIpaddress());
		m->setActivate(pMO->getActivate());
	}

	ret = sendSynchronously(m, locationid);
	if (ret == WAVE_MESSAGE_SUCCESS) {
		status = m->getCompletionStatus();
		if (status == WAVE_MESSAGE_SUCCESS) {
			VCENTER_PLUG_DBG(string("config file created"));
		} else {
			VCENTER_PLUG_DBG(
					string("config file creation failed Reason: ") + FrameworkToolKit::localize(m->getCompletionStatus()));
		}
	} else {
		VCENTER_PLUG_DBG(string("config file creation failed"));
	}
	delete m;

}

void VcenterWorker::cleanupDBfiles() {
	string vname;
	std::set<string> vcenterSetInDcm;

	vector<VcenterManagedObject *> vcenterList =
			vCenterToolkit::getVcenterConfigObjects();

	for (size_t i = 0; i < vcenterList.size(); i++) {

		VcenterManagedObject* pVcenterManagedObject = vcenterList.at(i);
		// Only Activated vCenters we need to worry about
		if (pVcenterManagedObject->getActivate() == true) {
			vname = pVcenterManagedObject->getId();
			vcenterSetInDcm.insert(vname);
		}
		delete pVcenterManagedObject;
	}

	std::set<string> vcenterSetInDir;
	//Fetch list of vcenters as on this nodes filesystem.
	FileUtils::fetchvCenterNamesFromFileSystem(MC_VCENTER_DIR, vcenterSetInDir);

	std::set<string>::iterator vcenterListInDirItr;
	for (vcenterListInDirItr = vcenterSetInDir.begin();
			vcenterListInDirItr != vcenterSetInDir.end();
			vcenterListInDirItr++) {

		string vcenterName = (*vcenterListInDirItr).c_str();

		if (std::find(vcenterSetInDcm.begin(), vcenterSetInDcm.end(),
				vcenterName) != vcenterSetInDcm.end()) {
			VIC_PLUG_LOG(VIC_TRACE_INFO,
					"Found vCenter during boot time discovery in DCM DB %s \n",
					vcenterName.c_str());
		} else {
			//Delete those vCenters that are not going to part of the cluster
			VIC_PLUG_LOG(
					VIC_TRACE_INFO,
					"Not found vCenter during boot time discovery in DCM DB %s \n",
					vcenterName.c_str());
			FileUtils::deleteVcenterPersistentDBFiles(vcenterName);
		}
	}

	//Fetch list of vcenters as on this nodes /mnt filesystem.
	FileUtils::fetchvCenterNamesFromFileSystem(MNT_MSC_VCENTER_DIR,
			vcenterSetInDir);

	for (vcenterListInDirItr = vcenterSetInDir.begin();
			vcenterListInDirItr != vcenterSetInDir.end();
			vcenterListInDirItr++) {

		string vcenterName = (*vcenterListInDirItr).c_str();

		if (std::find(vcenterSetInDcm.begin(), vcenterSetInDcm.end(),
				vcenterName) != vcenterSetInDcm.end()) {
			VIC_PLUG_LOG(VIC_TRACE_INFO,
					"Found vCenter during boot time discovery in DCM DB %s \n",
					vcenterName.c_str());
		} else {
			//Delete those vCenters that are not going to part of the cluster
			VIC_PLUG_LOG(
					VIC_TRACE_INFO,
					"Not found vCenter during boot time discovery in DCM DB %s \n",
					vcenterName.c_str());
			FileUtils::deleteVcenterPersistentDBFiles(vcenterName);
		}
	}
	return;
}

void VcenterWorker::startDiscoveryOnBootup(
		const NodeReadyForAllCommandsEvent* &pEvent) {

	VCENTER_PLUG_DBG_FUNC_BEG();VCENTER_PLUG_DBG(
			string("Node is ready to handle config commands.."));
	string vname;

	if (FileUtils::fileexists(PER_DBFILE)) {
		remove(PER_DBFILE); //delete legacy DB
		remove(PER_DBFILE_NEW);
	}
	VcenterDiff::deleteLegacyPortProfiles();

	/*
	 * compares DB vCenter DB files with DCM vCenter objects and cleanups
	 * unwanted DB files.
	 */
	cleanupDBfiles();

	vector<VcenterManagedObject *> vcenterList =
			vCenterToolkit::getVcenterConfigObjects();

	for (size_t j = 0; j < vcenterList.size(); j++) {

		VcenterManagedObject* pVcenterManagedObject = vcenterList.at(j);
		if ((pVcenterManagedObject->getActivate() != true))
			continue; //reconcile only for activated vCenter

		vname = pVcenterManagedObject->getId();
		string per_vcenterDB = "/var/tmp/vCenter/" + vname + ".db";

		if (FileUtils::fileexists(per_vcenterDB.c_str())) {
			trace(TRACE_LEVEL_INFO,
					string("File EXISTS will reconcile: ") + vname.c_str());
			//reconcile for this DB
			VcenterDiff::reconcile(vname);
		}
	}

	/* Event reply to framework */
	reply(reinterpret_cast<const PrismEvent *&>(pEvent));

	for (size_t i = 0; i < vcenterList.size(); i++) {

		VcenterManagedObject* pVcenterManagedObject = vcenterList.at(i);
		if ((pVcenterManagedObject->getActivate() != true))
			continue; //send discovery msg only for activated vCenter

		VCENTER_PLUG_DBG(
				string("MO ignore value:") + pVcenterManagedObject->getIgnoreValue());
		vname = pVcenterManagedObject->getId();

		if ((pVcenterManagedObject->getActivate() == true)) {
			VCENTER_PLUG_DBG(string("Config available and activated in MO"
			"for vcenter ") + vname);

			/* start discovery, if vcenter is configured and activated*/
			DiscoverDiscoverMessage *dmsg_d = new DiscoverDiscoverMessage();
			dmsg_d->setVcenter(vname);
			dmsg_d->setUrl(pVcenterManagedObject->getIpaddress());
			dmsg_d->setUsername(pVcenterManagedObject->getUsername());
			dmsg_d->setPassword(pVcenterManagedObject->getPassword());
			dmsg_d->setIgnoreValue(pVcenterManagedObject->getIgnoreValue());
			dmsg_d->setAlways(pVcenterManagedObject->getAlways());
			dmsg_d->setCmdcode(DISC_CMD_DISCOVER_ALL_VCENTER_ON_NODE_STATRUP);
			dmsg_d->setMsgType(DISC_MSG_TYPE_CREATE);

			sendOneWay(dmsg_d);
			VCENTER_PLUG_DBG(
					string("Async msg send  for first time discovery from vcenter plugin to discover plugin"));
		}

		/* Release Memory */
		delete pVcenterManagedObject;
	}

}

/* void VcenterWorker::startDiscoveryOnBootup(
 const NodeReadyForAllCommandsEvent* &pEvent) {
 VCENTER_PLUG_DBG_FUNC_BEG();
 VCENTER_PLUG_DBG(string("Node is ready to handle config commands.."));
 string vnames;


 // Event reply to framework
 reply(reinterpret_cast<const PrismEvent *&>(pEvent));

 // Recreate the config.php file with current running-config
 VcenterManagedObject* pVcenterManagedObject = getVcenterConfigFromMo("");
 LocationId locationid = NsmUtils::getLocalLocationId();
 createFreshConfigFile(pVcenterManagedObject, locationid);

 if (pVcenterManagedObject != NULL) {
 vnames = pVcenterManagedObject->getId();

 } else {
 VCENTER_PLUG_DBG(string("Vcenter config not available in MO"));
 cleanPersistedDBFiles();
 return;
 }

 DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();

 dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
 dmsg->setCmdcode(DISC_CMD_SYNC_PERSISTED_WITH_RUNNING);
 dmsg->setMsgType(DISC_MSG_TYPE_UPDATE);

 if (locationid) {

 dmsg->setLocationIds(locationid);
 }

 sendOneWay(dmsg);
 VCENTER_PLUG_DBG(
 string("Async msg for DB sync send from vcenter plugin to discover plugin"));

 if ((pVcenterManagedObject->getActivate() == true)) {
 VCENTER_PLUG_DBG(string("Config available and activated in MO"
 "for vcenter ") + vnames);


 VCENTER_PLUG_DBG(string("MO ignore value:") + pVcenterManagedObject->getIgnoreValue());

 // start discovery, if vcenter is configured and activated
 DiscoverDiscoverMessage *dmsg_d = new DiscoverDiscoverMessage();
 dmsg_d->setVcenter(vnames);
 dmsg_d->setUrl(pVcenterManagedObject->getIpaddress());
 dmsg_d->setUsername(pVcenterManagedObject->getUsername());
 dmsg_d->setPassword(pVcenterManagedObject->getPassword());
 dmsg_d->setIgnoreValue(pVcenterManagedObject->getIgnoreValue());
 dmsg_d->setAlways(pVcenterManagedObject->getAlways());
 dmsg_d->setGlobalScriptNames(DISC_GLOBAL_PLUGIN_SCRIPTS);
 dmsg_d->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
 dmsg_d->setCmdcode(DISC_CMD_DISCOVER_ALL_VCENTER);
 dmsg_d->setMsgType(DISC_MSG_TYPE_CREATE);

 sendOneWay(dmsg_d);
 VCENTER_PLUG_DBG(
 string("Async msg send  for first time discovery from vcenter plugin to discover plugin"));

 EventEngineMessage *eMsg = new EventEngineMessage();
 eMsg->setVcenter(vnames);
 eMsg->setUrl(pVcenterManagedObject->getIpaddress());
 eMsg->setUsername(pVcenterManagedObject->getUsername());
 eMsg->setPassword(pVcenterManagedObject->getPassword());
 eMsg->setCmdcode(EVENT_ENGINE_START);

 sendOneWay(eMsg);
 VCENTER_PLUG_DBG(
 string("Async msg send  Event Engine Start from vcenter plugin to discover plugin"));
 }

 // Release Memory
 delete pVcenterManagedObject;

 }*/

void VcenterWorker::cleanPersistedDBFiles() {
	if (FileUtils::fileexists(PER_DBFILE)
			|| (FileUtils::fileexists(PER_DBFILE_NEW))) {

		//copy default-db to DBFILE
		FileUtils::copyfile(DBFILE_ORIG, DBFILE);
		FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW);

		DiscoverLocalDiscoverMessage *dmsg = new DiscoverLocalDiscoverMessage();

		dmsg->setScriptNames(DISC_PPCLEAR_SCRIPTS);
		dmsg->setCmdcode(DISC_CMD_DISCOVER_VCENTER);

		VCENTER_PLUG_DBG(string("Executing pp-clear"));
		sendOneWay(dmsg);
	}

}

void VcenterWorker::vCenterDeleteVcenterMessageHandler(
		vCenterDeleteVcenterMessage *pvCenterDeleteVcenterMessage) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	PrismLinearSequencerStep sequencerSteps[] =
			{
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::doGlobalDeconfigStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::ppDeconfigStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerStartTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::deleteManagedObject),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerCommitTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerSucceededStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerFailedStep) };

	PrismLinearSequencerContext *pPrismLinearSequencerContext =
			new PrismLinearSequencerContext(pvCenterDeleteVcenterMessage, this,
					sequencerSteps,
					sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
	pPrismLinearSequencerContext->holdAll();
	pPrismLinearSequencerContext->start();
}

void VcenterWorker::vCenterUpdateVcenterMessageHandler(
		vCenterUpdateVcenterMessage *pvCenterUpdateVcenterMessage) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	PrismLinearSequencerStep sequencerSteps[] =
			{
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::doGlobalUpdateStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerStartTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::updateManagedObject),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerCommitTransactionStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::sendMsgToDiscoverPlugin),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerSucceededStep),
					reinterpret_cast<PrismLinearSequencerStep>(&VcenterWorker::prismLinearSequencerFailedStep) };

	PrismLinearSequencerContext *pPrismLinearSequencerContext =
			new PrismLinearSequencerContext(pvCenterUpdateVcenterMessage, this,
					sequencerSteps,
					sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

	pPrismLinearSequencerContext->holdAll();
	pPrismLinearSequencerContext->start();
}

void VcenterWorker::createManagedObject(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	VcenterManagedObject *mo = NULL;
	vCenterCreateVcenterMessage *pmsg =
			dynamic_cast<vCenterCreateVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	string id = pmsg->getId();

	mo = new VcenterManagedObject(
			dynamic_cast<vCenterObjectManager*>(getPWaveObjectManager()));
	;

	if (mo) {
		mo->setId(pmsg->getId());
		pmsg->getIpaddressFlag() ?
				mo->setIpaddress(pmsg->getIpaddress()) : mo->setIpaddress("");
		pmsg->getUsernameFlag() ?
				mo->setUsername(pmsg->getUsername()) : mo->setUsername("");
		pmsg->getPasswordFlag() ?
				mo->setPassword(pmsg->getPassword()) : mo->setPassword("");
		/* Generate vCenter config addition raslog */
		raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
				RASLOG_NOFLAGS, VC_1000, id.c_str());
		/* mark the MO for GC */
		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(mo);
	}

	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void VcenterWorker::updateManagedObject(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	VcenterManagedObject *mo = NULL;
	vCenterUpdateVcenterMessage *pmsg =
			dynamic_cast<vCenterUpdateVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	string id = pmsg->getId();

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(id, "id"));
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	vector<WaveManagedObject *>::iterator it;

	VCENTER_PLUG_DBG(string("UPDATE MO"));
	for (it = pResults->begin(); it != pResults->end(); it++) {
		mo = dynamic_cast<VcenterManagedObject *>(*it);
		if (!id.compare(mo->getId())) {
			VCENTER_PLUG_DBG(string("FOUND MO"));
			break;
		}
	}

	if (mo == NULL) {
		pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
				*pResults);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
		return;
	}

	if (pmsg->getIpaddressFlag())
		mo->setIpaddress(pmsg->getIpaddress());
	if (pmsg->getUsernameFlag())
		mo->setUsername(pmsg->getUsername());
	if (pmsg->getPasswordFlag())
		mo->setPassword(pmsg->getPassword());

	/* Generate vCenter config change raslog */
	if (pmsg->getIpaddressFlag() || pmsg->getUsernameFlag()
			|| pmsg->getPasswordFlag())
		raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
				RASLOG_NOFLAGS, VC_1001, id.c_str());

	if (pmsg->getActivateFlag()) {
		//pPrismLinearSequencerContext->executeNextStep(status);
		//return;
		mo->setActivate(pmsg->getActivate());
		/* Generate vCenter config activate/deactivate raslog */
		if (pmsg->getActivate())
			raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
					RASLOG_NOFLAGS, VC_1003, id.c_str());
		else
			raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
					RASLOG_NOFLAGS, VC_1004, id.c_str());
	}

	if (pmsg->getIntervalFlag()) {
		UI32 interval = pmsg->getInterval();
		mo->setInterval(interval);
		raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
				RASLOG_NOFLAGS, VC_1006, id.c_str(), interval);
	}

	if (pmsg->getIgnoreValueFlag()) {
		UI32 ignore_value = pmsg->getIgnoreValue();
		mo->setIgnoreValue(ignore_value);
		mo->setAlways(false);
		raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
				RASLOG_NOFLAGS, VC_1007, id.c_str(), ignore_value);
	}

	if (pmsg->getAlwaysFlag()) {
		bool always = pmsg->getAlways();
		mo->setAlways(always);
		mo->setIgnoreValue(DISCOVER_IGNORE_COUNT);
		raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
				RASLOG_NOFLAGS, VC_1010, id.c_str());
	}

	updateWaveManagedObject(mo);

	/* mark the MOs for GC */
	pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
			*pResults);

	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void VcenterWorker::deleteManagedObject(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	VcenterManagedObject *mo = NULL;
	vCenterDeleteVcenterMessage *pmsg =
			dynamic_cast<vCenterDeleteVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	string id = pmsg->getId();

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(id, "id"));
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	vector<WaveManagedObject *>::iterator it;

	VCENTER_PLUG_DBG(string("DELETE MO"));
	for (it = pResults->begin(); it != pResults->end(); it++) {
		mo = dynamic_cast<VcenterManagedObject *>(*it);
		if (!id.compare(mo->getId())) {
			VCENTER_PLUG_DBG(string("FOUND MO"));
			break;
		}
	}

	delete (mo);
	pResults->clear();
	delete pResults;
	/* Generate vCenter config deletion raslog */
	raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
			RASLOG_NOFLAGS, VC_1002, id.c_str());

	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void VcenterWorker::doGlobalConfigStep(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	vCenterCreateVcenterMessage *pmsg =
			dynamic_cast<vCenterCreateVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	WaveSendToClusterContext *send_ctx_p;
	UI32 count = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(pmsg->getId(), "id"));
	status = querySynchronouslyForCount(&syncQueryCtxt, count);
	if ((WAVE_MESSAGE_SUCCESS == status) && (count >= 1)) {

		VCENTER_PLUG_DBG(string("vCenter already configured"));
		pPrismLinearSequencerContext->executeNextStep(
				WRC_VCENTER_COMMON_ERR_ALREADY_EXISTS);
		return;
	}

	/* local message */
	vCenterVcenterMessage *m = new vCenterVcenterMessage();

	m->setId(pmsg->getId());
	m->setUsername(pmsg->getUsername());
	m->setPassword(pmsg->getPassword());
	m->setIpaddress(pmsg->getIpaddress());
	m->setActivate(pmsg->getActivate());

	send_ctx_p =
			new WaveSendToClusterContext(
					this,
					reinterpret_cast<PrismAsynchronousCallback>(&VcenterWorker::GlobalSendToClusterCallback),
					pPrismLinearSequencerContext);

	prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

	send_ctx_p->setPPrismMessageForPhase1(m);
	VCENTER_PLUG_DBG(string("Sending to Cluster: "));
	sendToWaveCluster(send_ctx_p);
	return;
}

void VcenterWorker::doGlobalUpdateStep(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	VcenterManagedObject *mo = NULL;
	WaveSendToClusterContext *send_ctx_p;
	vCenterUpdateVcenterMessage *pmsg =
			dynamic_cast<vCenterUpdateVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	string id = pmsg->getId();

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(id, "id"));
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	vector<WaveManagedObject *>::iterator it;

	for (it = pResults->begin(); it != pResults->end(); it++) {
		mo = dynamic_cast<VcenterManagedObject *>(*it);
		if (id.compare(mo->getId())) {
			VCENTER_PLUG_DBG(string("FOUND MO FOR: ") + mo->getId());
			break;
		}
	}

	/* local message */
	vCenterVcenterMessage *m = new vCenterVcenterMessage();
	m->setId(mo->getId());
	m->setUsername(mo->getUsername());
	m->setPassword(mo->getPassword());
	m->setIpaddress(mo->getIpaddress());
	m->setActivate(mo->getActivate());

	/* Free memory */
	delete mo;
	pResults->clear();
	delete pResults;

	if (pmsg->getIpaddressFlag())
		m->setIpaddress(pmsg->getIpaddress());
	if (pmsg->getUsernameFlag())
		m->setUsername(pmsg->getUsername());
	if (pmsg->getPasswordFlag())
		m->setPassword(pmsg->getPassword());
	if (pmsg->getActivateFlag())
		m->setActivate(pmsg->getActivate());

	send_ctx_p =
			new WaveSendToClusterContext(
					this,
					reinterpret_cast<PrismAsynchronousCallback>(&VcenterWorker::GlobalSendToClusterCallback),
					pPrismLinearSequencerContext);

	prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	VCENTER_PLUG_DBG(string("Sending to Cluster: "));
	sendToWaveCluster(send_ctx_p);
	return;
}

/* Initiate a discover on activating vCenter config */
void VcenterWorker::sendMsgToDiscoverPlugin(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	vCenterUpdateVcenterMessage *pmsg =
			dynamic_cast<vCenterUpdateVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	VcenterManagedObject *mo = NULL;
	if ((pmsg->getActivateFlag()) && (pmsg->getActivate())) {
		string id = pmsg->getId();
		mo = vCenterToolkit::getVcenterConfigFromMo(id);

		if (mo) {
			DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
			dmsg->setVcenter(pmsg->getId());
			dmsg->setUrl(mo->getIpaddress());
			dmsg->setUsername(mo->getUsername());
			dmsg->setPassword(mo->getPassword());
			dmsg->setIgnoreValue(mo->getIgnoreValue());
			dmsg->setAlways(mo->getAlways());
			dmsg->setGlobalScriptNames(DISC_GLOBAL_PLUGIN_SCRIPTS);
			dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
			dmsg->setCmdcode(DISC_CMD_DISCOVER_VCENTER);
			dmsg->setMsgType(DISC_MSG_TYPE_CREATE);

			/* Free memory */
			//delete mo;
			VCENTER_PLUG_DBG(
					string("Async msg send from vcenter plugin to discover plugin"));
			status = sendSynchronously(dmsg);
			if (status == WAVE_MESSAGE_SUCCESS) {
				status = dmsg->getCompletionStatus();
				if (status == WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS) {
					pmsg->setCompletionStatus(
							WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS);
					startTransaction();
					mo->setActivate(false);
					updateWaveManagedObject(mo);
					if (commitTransaction() != FRAMEWORK_SUCCESS) {
						status = WAVE_MESSAGE_ERROR;
					}
					delete mo;
					delete dmsg;
					pPrismLinearSequencerContext->executeNextStep(
							WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS);
					return;
				}
			}
			delete mo;
			delete dmsg;
		}
	}
	//User is trying to deactivate, set statemachine as deactivated and do
	//not stop event/discovery engine
	if ((pmsg->getActivateFlag()) && (pmsg->getActivate() == false)) {

		DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
		VCENTER_PLUG_DBG(string("De-activate vCenter"));
		dmsg->setVcenter(pmsg->getId());
		dmsg->setCmdcode(DISC_CMD_DISCOVER_VCENTER);
		dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
		dmsg->setMsgType(DISC_MSG_TYPE_DEACTIVATE);
		status = sendSynchronously(dmsg);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return;
	}

	if (pmsg->getIntervalFlag()) {
		VCENTER_PLUG_DBG(string("Restart Timer on Timer interval change"));
		DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
		dmsg->setVcenter(pmsg->getId());
		dmsg->setCmdcode(DISC_CMD_DISCOVER_TIMER);
		VCENTER_PLUG_DBG(
				string("Async msg send from vcenter plugin to discover plugin"));
		sendOneWay(dmsg);
	}

	pPrismLinearSequencerContext->executeNextStep(status);
}

void VcenterWorker::ppDeconfigStep(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	vCenterDeleteVcenterMessage *pmsg =
			dynamic_cast<vCenterDeleteVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
	dmsg->setVcenter(pmsg->getId());
	dmsg->setCmdcode(DISC_CMD_DISCOVER_VCENTER);
	dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
	dmsg->setMsgType(DISC_MSG_TYPE_DELETE);
	sendOneWay(dmsg);

	/*VcenterManagedObject* pVcenterManagedObjec = getVcenterConfigFromMo(pmsg->getId());
	 //If vCenter is activate then Stop EventEngine
	 if(pVcenterManagedObjec->getActivate()){
	 EventEngineMessage *eMsg = new EventEngineMessage();
	 eMsg->setVcenter(pmsg->getId());
	 eMsg->setCmdcode(EVENT_ENGINE_STOP);
	 }*/
	pPrismLinearSequencerContext->executeNextStep(status);
}

void VcenterWorker::doGlobalDeconfigStep(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {
	VCENTER_PLUG_DBG_FUNC_BEG();
	WaveSendToClusterContext *send_ctx_p;
	UI32 count = 0;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vCenterDeleteVcenterMessage *pmsg =
			dynamic_cast<vCenterDeleteVcenterMessage*>(pPrismLinearSequencerContext->getPPrismMessage());

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(pmsg->getId(), "id"));
	status = querySynchronouslyForCount(&syncQueryCtxt, count);

	VIC_PLUG_LOG(VIC_TRACE_INFO, "------------ count %d -----------", count);

	if ((WAVE_MESSAGE_SUCCESS == status) && (count == 0)) {

		VCENTER_PLUG_DBG(string("vCenter Not Present"));
		pPrismLinearSequencerContext->executeNextStep(
				WRC_VCENTER_COMMON_ERR_ALREADY_DELETE);
		return;
	}

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(
			VcenterManagedObject::getClassName());
	status = querySynchronouslyForCount(&syncQueryCtxt2, count);

	vCenterVcenterMessage *m = new vCenterVcenterMessage();

	if (count == 1) {
		VCENTER_PLUG_DBG(string("Setting Delete Flag True"));
		m->setDeleteFlag(true);
	}

	send_ctx_p =
			new WaveSendToClusterContext(
					this,
					reinterpret_cast<PrismAsynchronousCallback>(&VcenterWorker::GlobalSendToClusterCallback),
					pPrismLinearSequencerContext);
	prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	VCENTER_PLUG_DBG(string("Sending to Cluster"));
	sendToWaveCluster(send_ctx_p);
	return;
}

void VcenterWorker::GlobalSendToClusterCallback(
		WaveSendToClusterContext *ctx_p) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	LocationId locationid;
	ClusterGetFirstFailureLocation(ctx_p, locationid);

	PrismLinearSequencerContext * pPrismLinearSequencerContext;
	pPrismLinearSequencerContext =
			reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
	delete ctx_p->getPPrismMessageForPhase1();
	delete ctx_p;
	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	return;
}

void VcenterWorker::ClusterGetFirstFailureLocation(
		WaveSendToClusterContext *ctx_p, LocationId &locationid) {
	vector<LocationId> locationsToSendToForPhase1 =
			ctx_p->getLocationsToSendToForPhase1();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	for (unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++) {
		locationid = locationsToSendToForPhase1[i];
		VCENTER_PLUG_DBG(string("location id=") + locationid);
		status = ClusterGetPhase1StatusFromContextForLocation(ctx_p,
				locationid); /* XXX */
		if (status != WAVE_MESSAGE_SUCCESS)
			return;
	}
}
void VcenterWorker::createConfigPhpFile(vCenterVcenterMessage *m) {
	VCENTER_PLUG_DBG_FUNC_BEG();

	string s;
	ofstream outFile;
	const char *outputFile = "/fabos/webtools/htdocs/ws/config/config.php";

	outFile.open(outputFile, ios::out);
	if (outFile) {

		s = m->getIpaddress();
		if (!s.empty())
			outFile << "$ipaddr = " << s << ";\n";

		s = m->getUsername();
		if (!s.empty())
			outFile << "$username = " << s << ";\n";

		s = m->getPassword();
		if (!s.empty())
			outFile << "$password = " << s << ";\n";

		outFile.close();

	}
	return;
}
}
