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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Firmware/Local/FirmwareClusterLocalMessage.h"
#include "Firmware/Global/FirmwareObjectManager.h"
#include "Firmware/Global/FirmwareManagedObject.h"
#include "Firmware/Global/FirmwareDownloadStatusManagedObject.h"
#include "Firmware/Global/FirmwareClusterMessage.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

#include <stdarg.h>
#include <raslog/raslog.h>
#include <raslog/raslog_sulb.h>

namespace DcmNs
{

    FirmwareObjectManager::FirmwareObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        FirmwareManagedObject    FirmwareManagedObject    (this);
		FirmwareDownloadStatusManagedObject FirmwareDownloadStatusManagedObject(this);
        FirmwareManagedObject.setupOrm ();
		FirmwareDownloadStatusManagedObject.setupOrm();
        glog_handle = glog_init(GLOG_MODID_FWDL_GLOBAL_OM, GLOG_O_CREAT, GLOG_FILESIZE_DEFAULT);
        if (NULL == glog_handle) {
		    WaveNs::trace(TRACE_LEVEL_WARN, string("Global FirmwareOM::Global FirmwareOM: Generic logger initialization failed"));
            /* continue */
        } else {
            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::FirmwareObjectManager: Firmware Object Manager Instantiated.");
        }

        addManagedClass (FirmwareManagedObject::getClassName ());
		addManagedClass (FirmwareDownloadStatusManagedObject::getClassName ());
        addOperationMap (FIRMWARECLUSTER, reinterpret_cast<PrismMessageHandler> (&FirmwareObjectManager::FirmwareClusterMessageHandler));
		addOperationMap (FIRMWAREACTIVATE, reinterpret_cast<PrismMessageHandler> (&FirmwareObjectManager::FirmwareActivateMessageHandler));
		cliTimerHandle = 0;
		nodeActivateRebooted = 0;
    }

    FirmwareObjectManager::~FirmwareObjectManager ()
    {
        glog_exit(glog_handle);
        glog_handle = NULL;
    }

    void
    FirmwareObjectManager::fgom_log(int level, const char *fmt, ...)
    {
        char    buf[1024];
        va_list args;
        WaveNs::TraceLevel tlevel;

        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        switch(level) {
        case GLOG_CRIT:
            tlevel = TRACE_LEVEL_FATAL;
            break;
        case GLOG_ERR:
            tlevel = TRACE_LEVEL_ERROR;
            break;
        case GLOG_WARNING:
            tlevel = TRACE_LEVEL_WARN;
            break;
        case GLOG_INFO:
            tlevel = TRACE_LEVEL_INFO;
            break;
        case GLOG_DEBUG:
            tlevel = TRACE_LEVEL_DEBUG;
            break;
        default:
            tlevel = TRACE_LEVEL_INFO;
            break;
        }

        if (glog_handle)
            glog(glog_handle, level, "%s", buf);
        WaveNs::trace(tlevel, string(buf));
    }

    FirmwareObjectManager  *FirmwareObjectManager::getInstance()
    {
        static FirmwareObjectManager *pFirmwareObjectManager = new FirmwareObjectManager ();

        WaveNs::prismAssert (NULL != pFirmwareObjectManager, __FILE__, __LINE__);

        return (pFirmwareObjectManager);
    }

    string  FirmwareObjectManager::getClassName()
    {
        return ("Firmware Cluster Global Manager");
    }

    PrismServiceId  FirmwareObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FirmwareObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FIRMWARECLUSTER :
                pPrismMessage = new FirmwareClusterMessage ();
                break;
			case FIRMWAREACTIVATE :
				pPrismMessage = new FirmwareClusterMessage();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FirmwareObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::createManagedObjectInstance : Entry");
        if ((FirmwareManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FirmwareManagedObject(this);
        }
		else if ((FirmwareDownloadStatusManagedObject::getClassName()) == managedClassName)
		{
			pWaveManagedObject = new FirmwareDownloadStatusManagedObject(this);
		}
        else
        {
            WaveNs::trace(TRACE_LEVEL_ERROR, string("Global FirmwareOM::createManagedObjectInstance : Unknown Managed Class Name : ") + managedClassName.c_str());
        }
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::createManagedObjectInstance : Exit");
        return (pWaveManagedObject);
    }

	vector<WaveManagedObject *> *FirmwareObjectManager::DoSynchronousQuery (const string className)
	{
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::DoSynchronousQuery: Entry");
		vector<WaveManagedObject *> *pResults = querySynchronously (className);
		prismAssert (NULL != pResults, __FILE__, __LINE__);
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::DoSynchronousQuery: Exit");
		return pResults;
	}

    void  FirmwareObjectManager::FirmwareClusterMessageHandler(FirmwareClusterMessage *pFirmwareClusterMessage)
    {

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareClusterMessageHandler: Entry");
	UI32 flag = pFirmwareClusterMessage->getFlag();
	PrismLinearSequencerContext *pFirmwareContext = NULL;

	PrismLinearSequencerStep sequencerSteps1[] =
       	{
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::SanityCheckHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerFailedStep)
		};

	PrismLinearSequencerStep sequencerSteps2[] =
       	{
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::FirmwareInstallHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::RebootHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerFailedStep)
		};


	if (flag & FW_CLUSTER_SANITY) {

		fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareClusterMessageHandler: CLUSTER_SANITY operation initiated");
		pFirmwareContext = new PrismLinearSequencerContext (pFirmwareClusterMessage, this, sequencerSteps1, sizeof (sequencerSteps1) /sizeof (sequencerSteps1[0]));

	} else {

		fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareClusterMessageHandler: CLUSTER_INSTALL operation initiated");
		pFirmwareContext = new PrismLinearSequencerContext (pFirmwareClusterMessage, this, sequencerSteps2, sizeof (sequencerSteps2) /sizeof (sequencerSteps2[0]));

	}
	
		pFirmwareContext->setPPrismMessage (pFirmwareClusterMessage);
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareClusterMessageHandler: Exit");
        pFirmwareContext->start ();

    }

	/*
	 * Use the same sequencer for Firmware Activate and Firmware Recover
	*/
	void  FirmwareObjectManager::FirmwareActivateMessageHandler(FirmwareClusterMessage *pFirmwareClusterMessage) {

		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareActivateMessageHandler: Entry");
		PrismLinearSequencerStep sequencerSteps[] = {
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::FirmwareActivateHandler),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep>(&FirmwareObjectManager::prismLinearSequencerFailedStep)
		};

		PrismLinearSequencerContext *pFirmwareContext = new PrismLinearSequencerContext (pFirmwareClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pFirmwareContext->setPPrismMessage (pFirmwareClusterMessage);
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareActivateMessageHandler: Exit");
		pFirmwareContext->start ();
	}


    void  FirmwareObjectManager::SanityCheckHandler(PrismLinearSequencerContext *pFirmwareContext)
{

	UI32			switchId = 0;
	UI32			nodesInCluster, i, j, entire_cluster = 1;
	vector<UI32>	rbridgeIdsVector, rbridgeIdsVector_temp;
	ResourceId		status = WAVE_MESSAGE_ERROR;
	FirmwareClusterLocalMessage *pMessage = NULL;
	FirmwareDownloadStatusManagedObject *pFirmwareDownloadStatusManagedObject = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::SanityCheckHandler: Entry");
	pFirmwareContext->setNumberOfFailures(0);
	++(*pFirmwareContext);

	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

	vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
    nodesInCluster = pResultsClusterInfo->size();

	rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();
	if ((rbridgeIdsVector.size() > 0) && (rbridgeIdsVector[0] != 0)) {
		nodesInCluster = rbridgeIdsVector.size();
		entire_cluster = 0;
	}

	/*
	* The FirmwareClusterMessage needs to return the RID, return code and
	* mesg of nodes in the cluster back to the Gateway. The RID and the
	* corresponding entries cannot be mapped if the user had given "all"
	* for rbridge_id in the command prompt. Hence Clear the rbridgeid contents
	* in this message. Form the list later in the CallBack when the nodes reply.
	*/
    rbridgeIdsVector_temp.clear();
    pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector_temp);

	/*
	* Clear the Firmwaredownload status MO for the nodes that are about to be upgraded.
	*/
	pResults = querySynchronously (FirmwareDownloadStatusManagedObject::getClassName());
	if (pResults) {
		for (i = 0; i < nodesInCluster; i++) {
			if (entire_cluster == 1) {
				VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  = (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
				switchId = pVcsNodeLocalManagedObject->getMappedId();
			} else {
				switchId = rbridgeIdsVector[i];
			}

			for (j = 0; j < pResults->size(); j++) {
				if (!((*pResults)[j]))
					continue;
				pFirmwareDownloadStatusManagedObject = dynamic_cast<FirmwareDownloadStatusManagedObject *> ((*pResults)[j]);
				if (pFirmwareDownloadStatusManagedObject && (pFirmwareDownloadStatusManagedObject->getRbridge_Id_Local() == switchId)) {
					WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::SanityCheckHandler.Remove MO. Rid ") +
						pFirmwareDownloadStatusManagedObject->getRbridge_Id_Local() + string("status ") + pFirmwareDownloadStatusManagedObject->getStatus());
					startTransaction();
					delete pFirmwareDownloadStatusManagedObject;
					commitTransaction();
					(*pResults)[j] = NULL;
				}
			}
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pResults = NULL;
	}

	fgom_log(GLOG_INFO, "Global FirmwareOM::SanityCheckHandler: # of switches in cluster = %d", nodesInCluster);

    for (i = 0; i < nodesInCluster; i++) {

        if (entire_cluster == 1) {
	    	VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  = (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
	    	switchId = pVcsNodeLocalManagedObject->getMappedId();

		} else {
			switchId = rbridgeIdsVector[i];
		}

		LocationId locationId = DcmToolKit::getLocationIdFromMappedId(switchId);

		pMessage = new FirmwareClusterLocalMessage( SANITY_CHECK,
			pFirmwareClusterMessage->getUser(),
			pFirmwareClusterMessage->getPassword(),
			pFirmwareClusterMessage->getHost(),
			pFirmwareClusterMessage->getPath(),
			pFirmwareClusterMessage->getProto(),
			pFirmwareClusterMessage->getFlag(),
			switchId);

		status = send (pMessage,
			reinterpret_cast<PrismMessageResponseHandler> (&FirmwareObjectManager::SanityCheckCallback),
			pFirmwareContext, 300000, locationId);

		if (WAVE_MESSAGE_SUCCESS != status) {
			pFirmwareContext->incrementNumberOfFailures();
			updateFirmwareDownloadStatusMO(switchId, SANITY_REQUEST, FrameworkToolKit::localize(status));
			fgom_log(GLOG_ERR, "Global FirmwareOM::SanityCheckHandler: Error in sending message to node %d", switchId);
			delete pMessage;
		} else {
			WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::SanityCheckHandler: Message sent to local node ") + switchId);
			updateFirmwareDownloadStatusMO(switchId, SANITY_REQUEST, string("Success."));
			++(*pFirmwareContext);
		}
	}

	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::SanityCheckHandler: Exit");

	--(*pFirmwareContext);
	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

	void  FirmwareObjectManager::SanityCheckCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext)
{
	--(*pFirmwareContext);

	UI32 switchId = 0;
	int local_node_ret;
	string local_node_mesg, status_str, local_firmware_version;
	vector<UI32> rbridgeIdsVector;
	vector<int> cluster_nodes_ret;
	vector<string> cluster_nodes_mesg;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::SanityCheckCallback: Entry");

	if (FRAMEWORK_SUCCESS == frameworkStatus) {

		WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::SanityCheckCallback: FRAMEWORK_SUCCESS.");

		switchId = pMessage->getRbridgeId();
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus())) {

			fgom_log(GLOG_INFO, "Global FirmwareOM::SanityCheckCallback: Request failed for %d", switchId);
			pFirmwareContext->incrementNumberOfFailures();
			local_node_ret = 255;
			local_node_mesg = FrameworkToolKit::localize (pMessage->getCompletionStatus());

		} else {

			WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::SanityCheckCallback: Response received from ") + switchId);

			if (pMessage->getState() == SANITY_CHECK_COMPLETE) {
				fgom_log(GLOG_INFO, "Global FirmwareOM::SanityCheckCallback: Sanity Check completed for %d", switchId);
			} else {
				fgom_log(GLOG_INFO, "Global FirmwareOM::SanityCheckCallback: Sanity Check failed for %d", switchId);
				pFirmwareContext->incrementNumberOfFailures();

			}

			local_node_ret = pMessage->getLocalNodeRet();
			local_node_mesg = pMessage->getLocalNodeMesg();
			local_firmware_version = pMessage->getFirmwareVersion();
		}

		FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

		/*
		 * Update the Firmware Cluster Message with the RID, return
		 * code and message of the node that has replied. The Gateway
		 * will read the Cluster message and retrieve the entire info
		 * of all nodes.
		*/

		rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();
		rbridgeIdsVector.push_back(switchId);
		pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector);

		cluster_nodes_ret = pFirmwareClusterMessage->getClusterNodesRet();
		cluster_nodes_ret.push_back(local_node_ret);
		pFirmwareClusterMessage->setClusterNodesRet(cluster_nodes_ret);

		cluster_nodes_mesg = pFirmwareClusterMessage->getClusterNodeMesg();
		cluster_nodes_mesg.push_back(local_node_mesg);
		pFirmwareClusterMessage->setClusterNodeMesg(cluster_nodes_mesg);

		pFirmwareClusterMessage->setFirmwareVersion(local_firmware_version);

		if (local_node_ret == 0) {
			status_str = "disruptive.";
		} else if (local_node_ret == 1) {
			status_str = "uses ISSU protocol, non-disruptive.";
		} else {
			status_str = local_node_mesg;
		}
		updateFirmwareDownloadStatusMO(switchId, SANITY_REPLY, status_str);

		WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::SanityCheckCallback: RID ") + switchId + string(" Ret ") + local_node_ret + string(" Mesg ")
			+ local_node_mesg.c_str());

	}

	if (pMessage)
		delete pMessage;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::SanityCheckCallback: Exit");

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

    void  FirmwareObjectManager::FirmwareInstallHandler(PrismLinearSequencerContext *pFirmwareContext)
{

	UI32 switchId = 0;
	UI32 nodesInCluster = 0, i;
	ResourceId status = WAVE_MESSAGE_ERROR;
	FirmwareClusterLocalMessage *pMessage = NULL;
	vector<UI32> rbridgeIdsVector, rbridgeIdsVector_temp;
	UI32 entire_cluster = 1, first_rbridgeids_entry = 1;
	char rbridgeids[256];

	++(*pFirmwareContext);
	pFirmwareContext->setNumberOfFailures(0);
  
	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareInstallHandler: Entry");

	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

    vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
    nodesInCluster = pResultsClusterInfo->size();
	vector<WaveManagedObject *> *pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults != NULL, __FILE__, __LINE__);

	rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();

	rbridgeIdsVector_temp.clear();
	pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector_temp);

	if ((rbridgeIdsVector.size() > 0) && (rbridgeIdsVector[0] != 0)) {
		nodesInCluster = rbridgeIdsVector.size();
		entire_cluster = 0;
	}

	fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallHandler: Number of switches to start install = %d", nodesInCluster);

   	for (i = 0; i < nodesInCluster; i++) {
		FirmwareManagedObject *pFirmwareManagedObject = NULL;
		int FirmwareManagedObjectExist = 0;	

		if (entire_cluster == 1) {
			VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
					(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
			switchId = pVcsNodeLocalManagedObject->getMappedId();
		} else {
			switchId = rbridgeIdsVector[i];
		}
		
		LocationId locationId = DcmToolKit::getLocationIdFromMappedId(switchId);

		pMessage = new FirmwareClusterLocalMessage(INSTALL,
			pFirmwareClusterMessage->getUser(),
			pFirmwareClusterMessage->getPassword(),
			pFirmwareClusterMessage->getHost(),
			pFirmwareClusterMessage->getPath(),
			pFirmwareClusterMessage->getProto(),
			pFirmwareClusterMessage->getFlag(),
			switchId);

		status = send (pMessage,
			reinterpret_cast<PrismMessageResponseHandler> (&FirmwareObjectManager::FirmwareInstallCallback),
			pFirmwareContext, 3900000, locationId);

		if (WAVE_MESSAGE_SUCCESS != status) {
			pFirmwareContext->incrementNumberOfFailures ();
	        fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallHandler: Error in sending message to node %d", switchId);
			updateFirmwareDownloadStatusMO(switchId, INSTALL_REQUEST, FrameworkToolKit::localize(status));
			delete pMessage;
		} else {
	        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareInstallHandler: Install message sent to node ") + switchId);
			updateFirmwareDownloadStatusMO(switchId, INSTALL_REQUEST, string("Success."));

			++(*pFirmwareContext);
			startTransaction();
			for (int j = 0; j < (int)pResults->size(); j++) {
				pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
				if (switchId == pFirmwareManagedObject->getRbridge_Id()) {
					FirmwareManagedObjectExist = 1;
					break;
				}
			}

			if (!FirmwareManagedObjectExist) {
				pFirmwareManagedObject = new FirmwareManagedObject(this);

	            WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareInstallHandler: Creating New Managed Object for rbridge-id ") + switchId);
			}
			updateWaveManagedObject(pFirmwareManagedObject);
			pFirmwareManagedObject->setState(INSTALL);
			pFirmwareManagedObject->setCliState(CLI_STATUS_UNBLOCK);
			pFirmwareManagedObject->setRbridge_Id(switchId);
			status = commitTransaction();

			if (status == FRAMEWORK_SUCCESS) {
//				++(*pFirmwareContext);
				WaveNs::trace (TRACE_LEVEL_INFO, string("Global FirmwareOM::Firmware"
                            "InstallHandler: Install state updated for ") + switchId);
			} else {
				pFirmwareContext->incrementNumberOfFailures ();
				WaveNs::trace (TRACE_LEVEL_INFO, string("Global FirmwareOM::Firmware"
                             "InstallHandler: Install state update failed for : ") + switchId);
			}
			if (first_rbridgeids_entry) {
				snprintf(rbridgeids, 256, "%d",	switchId);
				WaveNs::trace (TRACE_LEVEL_INFO, string("Global FirmwareOM::Firmware"
                            "InstallHandler:1st entry") + rbridgeids + string("switchId") + switchId);
				first_rbridgeids_entry = 0;
			} else {
				char temp[256];
				WaveNs::trace (TRACE_LEVEL_INFO, string("Global FirmwareOM::Firmware"
                            "InstallHandler: rbridgeids ") + rbridgeids + string("switchId") + switchId);
				strcpy(temp, rbridgeids);
				snprintf(rbridgeids, 256,"%s, %d", temp, switchId);	
			}
		}
	}

	if (!first_rbridgeids_entry) {
	    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareInstallHandler: RASLOG: rbridges ") + rbridgeids);
		raslog_ext(__FILE__,__FUNCTION__,__LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG,
			SULB_1111, rbridgeids);
	}

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	if (pResultsClusterInfo)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareInstallHandler: Exit");
	--(*pFirmwareContext);

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

	void  FirmwareObjectManager::FirmwareInstallCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext)
{
	--(*pFirmwareContext);

	UI32 switchId = 0;
	UI32 i, found = 0;
	ResourceId status = WAVE_MESSAGE_ERROR;
	vector<UI32>	rbridgeIdsVector;
	vector<int>     cluster_nodes_ret;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareInstallCallback: Entry");
	vector<WaveManagedObject *> *pResults = DoSynchronousQuery(FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);

	if (FRAMEWORK_SUCCESS == frameworkStatus) {

		fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallCallback: FRAMEWORK_SUCCESS.");

		switchId = pMessage->getRbridgeId();
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {

			fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallCallback: Request failed for %d", switchId);
			updateFirmwareDownloadStatusMO(switchId, INSTALL_REPLY, FrameworkToolKit::localize (pMessage->getCompletionStatus()));
			pFirmwareContext->incrementNumberOfFailures();

			rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();
			rbridgeIdsVector.push_back(switchId);
			pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector);

			cluster_nodes_ret = pFirmwareClusterMessage->getClusterNodesRet();
			cluster_nodes_ret.push_back(-1);
			pFirmwareClusterMessage->setClusterNodesRet(cluster_nodes_ret);

		} else {
			WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareInstallCallback: Response received successfully for ") + switchId);

			for (i = 0; i < pResults->size(); i++) {
				pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[i]);
				if (switchId == pFirmwareManagedObject->getRbridge_Id()) {
					found = 1;
					break;
				}
			}

			if (pMessage->getState() == INSTALL_COMPLETE) {

				if (found && pFirmwareManagedObject) {
					startTransaction();
					updateWaveManagedObject(pFirmwareManagedObject);
					pFirmwareManagedObject->setState(INSTALL_COMPLETE);
					status = commitTransaction();

					if (status == FRAMEWORK_SUCCESS) {
						raslog_ext(__FILE__,__FUNCTION__,__LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG,
							SULB_1112, switchId);
			            fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallCallback: Install completed for %d", switchId);
					} else {
						pFirmwareContext->incrementNumberOfFailures();
			            WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareInstallCallback: Transaction failed for ") + switchId);
					}
					updateFirmwareDownloadStatusMO(switchId, INSTALL_REPLY, string("Success."));
				} else {
			        fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallCallback: Managed object NOT found");
				}

			} else {
				pFirmwareContext->incrementNumberOfFailures();
			    fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareInstallCallback: Install failed for %d", switchId);
				updateFirmwareDownloadStatusMO(switchId, INSTALL_REPLY, string("Install failed for the node."));
			}
		}
	}

	if (pResults) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	    WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::FirmwareInstallCallback: pResults deleted");
    }


	if (pMessage) {
		delete pMessage;
	    WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::FirmwareInstallCallback: Message deleted");
    }


	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareInstallCallback: Exit");

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}


    void  FirmwareObjectManager::RebootHandler(PrismLinearSequencerContext *pFirmwareContext)
{

	UI32 switchId = 0, local_mappedId = 0;
	UI32 nodesInCluster, i, primary_switchId = 0, flag = 0;
	int primary_switchId_index = -1;
	ResourceId status = WAVE_MESSAGE_ERROR;
	FirmwareClusterLocalMessage *pMessage = NULL;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::RebootHandler: Entry");
	pFirmwareContext->setNumberOfFailures(0);

	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

	flag = pFirmwareClusterMessage->getFlag();

	vector<WaveManagedObject *> *pResults = DoSynchronousQuery(FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);

	nodesInCluster = pResults->size();
	local_mappedId = DcmToolKit::getLocalMappedId();

	fgom_log(GLOG_INFO, "Global FirmwareOM::RebootHandler: Number of switches in cluster = %d", nodesInCluster);

    for (i = 0; i <= nodesInCluster; i++) {

		if (i == nodesInCluster) {
			switchId = primary_switchId;
			if (primary_switchId_index != -1)
				pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[primary_switchId_index]);
		} else {
			pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[i]);
			switchId = pFirmwareManagedObject->getRbridge_Id();
			if (local_mappedId == switchId) {
				primary_switchId = switchId;
				primary_switchId_index = i;
				continue;
			}
		}

		if (switchId <= 0)
			continue;

		if (pFirmwareManagedObject->getState() != INSTALL_COMPLETE)
			continue;

		LocationId locationId = DcmToolKit::getLocationIdFromMappedId(switchId);

		status = WAVE_MESSAGE_SUCCESS;
		if (flag & FW_CLUSTER_ACTIVATE) {
	        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootHandler: About to send reboot message to node ") + switchId);
			pMessage = new FirmwareClusterLocalMessage ();
			pMessage->setState(ACTIVATE);
			pMessage->setRbridgeId(switchId);
			status = send (pMessage,
				reinterpret_cast<PrismMessageResponseHandler> (&FirmwareObjectManager::RebootCallback), pFirmwareContext, 30000, locationId);

		} else {
	        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootHandler: Skipping in sending the mesg to node ") + switchId);
		}

		if (WAVE_MESSAGE_SUCCESS != status) {
			pFirmwareContext->incrementNumberOfFailures ();
	        fgom_log(GLOG_ERR, "Global FirmwareOM::RebootHandler: Error in sending the mesg to node %d", switchId);
			updateFirmwareDownloadStatusMO(switchId, ACTIVATE_REQUEST, FrameworkToolKit::localize(status));
			delete pMessage;

		} else {
			if (flag & FW_CLUSTER_ACTIVATE) {
	            fgom_log(GLOG_INFO, "Global FirmwareOM::RebootHandler: Reboot mesg sent to node %d", switchId);
				updateFirmwareDownloadStatusMO(switchId, ACTIVATE_REQUEST, string("Success."));

				++(*pFirmwareContext);
				startTransaction();
				updateWaveManagedObject(pFirmwareManagedObject);
				pFirmwareManagedObject->setState(ACTIVATE);
				pFirmwareManagedObject->setCliState(CLI_STATUS_BLOCK);
				pFirmwareManagedObject->setRbridge_Id(switchId);
				status = commitTransaction();

				if (status == FRAMEWORK_SUCCESS) {
//					++(*pFirmwareContext);
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootHandler:state updated for ") + switchId);
				} else {
					pFirmwareContext->incrementNumberOfFailures ();
					WaveNs::trace (TRACE_LEVEL_INFO,
						string("Global FirmwareOM::RebootHandler: "
						"state update failed for : ") + switchId);
				}
			} else {
                startTransaction ();
				delete pFirmwareManagedObject;
				status = commitTransaction ();
				if (status == FRAMEWORK_SUCCESS) {
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootHandler:removed MO for rbridge-id ") + switchId);
				} else {
					pFirmwareContext->incrementNumberOfFailures ();
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootHandler:remove MO failed for rbridge-id ") + switchId);
				}
			}
		}
	}

	if (flag & FW_CLUSTER_ACTIVATE) {
        WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::RebootHandler: start timer.");
		if (cliTimerHandle != 0) {
			deleteTimer (cliTimerHandle);
			cliTimerHandle = 0;
		}
		startTimer (cliTimerHandle, 900000, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareObjectManager::cliTimerHandler));

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	} else {
		pResults->clear();
	}

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::RebootHandler: Exit");

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

	void  FirmwareObjectManager::RebootCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext)
{
	--(*pFirmwareContext);

	UI32 switchId = 0;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::RebootCallback: Entry");

	if (FRAMEWORK_SUCCESS == frameworkStatus) {
	    WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::RebootCallback: FRAMEWORK_SUCCESS.");

		switchId = pMessage->getRbridgeId();
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
	        fgom_log(GLOG_INFO, "Global FirmwareOM::RebootCallback: Request failed for %d", switchId);
			pFirmwareContext->incrementNumberOfFailures ();
		} else {

	       WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::RebootCallback: Response received for ") + switchId);
		}

	}

	if (NULL != pMessage)
		delete pMessage;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::RebootCallback: Exit");

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

void FirmwareObjectManager::prismLinearSequencerFailedStep (PrismLinearSequencerContext *pFirmwareContext)
{

	UI32 nodesInCluster, i, current_state, rbridge_id;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::prismLinearSequencerFailedStep: Entry");

	vector<WaveManagedObject *> *pResults = querySynchronously (FirmwareManagedObject::getClassName());

	nodesInCluster = pResults->size();

	for (i = 0; i < nodesInCluster; i++) {

		pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[i]);
		current_state = pFirmwareManagedObject->getState();
		rbridge_id = pFirmwareManagedObject->getRbridge_Id();

		if (current_state == SANITY_CHECK) {
	        fgom_log(GLOG_INFO, "Global FirmwareOM::prismLinearSequencerFailedStep: Node %d failed at SANITY_CHECK step.", rbridge_id);
		} else if ( current_state == INSTALL ) {
	        fgom_log(GLOG_INFO, "Global FirmwareOM::prismLinearSequencerFailedStep: Node %d failed at INSTALL step.", rbridge_id);
		}

		if (pFirmwareManagedObject) {
	       	WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::prismLinearSequencerFailedStep: Managed Object deleted for Rbridge-id ") + rbridge_id);
			startTransaction();
			delete pFirmwareManagedObject;
			status = commitTransaction ();

			if (status == FRAMEWORK_SUCCESS) {
	            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::prismLinearSequencerFailedStep: Commit transaction completed.");
			} else {
	            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::prismLinearSequencerFailedStep: Commit transaction failed.");
			}
		}


	}

	if (pResults)
		pResults->clear();

	PrismMessage             *pPrismMessage             = pFirmwareContext->getPPrismMessage ();
	PrismAsynchronousContext *pPrismAsynchronousContext = pFirmwareContext->getPPrismAsynchronousContext ();

	if (true == (pFirmwareContext->getIsHoldAllRequested ())) {
		pFirmwareContext->unholdAll ();
	}

	if (true == (pFirmwareContext->getIsTransactionStartedByMe ())) {
		pFirmwareContext->setIsTransactionStartedByMe (false);
		rollbackTransaction ();
	}

	if (NULL != pPrismMessage) {
		pPrismMessage->setCompletionStatus (pFirmwareContext->getCompletionStatus ());
		reply (pPrismMessage);
	} else if (NULL != pPrismAsynchronousContext) {
		pPrismAsynchronousContext->setCompletionStatus (pFirmwareContext->getCompletionStatus ());
		pPrismAsynchronousContext->callback ();
	}

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::prismLinearSequencerFailedStep: Exit");

	delete pFirmwareContext;
}

void FirmwareObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	char rbridgeids[256];
	int failed = 0, numNodeBlocked = 0;
	UI32 local_mappedId = DcmToolKit::getLocalMappedId();

	rbridgeids[0] = '\0';

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::failover: Entry");

	FrameworkObjectManagerFailoverReason	failoverReason	=	pFailoverAsynchronousContext->getfailoverReason ();
	vector<LocationId>		failedLocationIds				=	pFailoverAsynchronousContext->getfailedLocationIds ();

	fgom_log(GLOG_INFO, "Global FirmwareOM::failover: reason - %s", FrameworkToolKit::localize(failoverReason).c_str());

	vector<WaveManagedObject *> *pResults = NULL;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);

	for (int j = 0; j < (int) pResults->size(); j++) {
		pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: failover Rbridge ID ") + pFirmwareManagedObject->getRbridge_Id() + string(" State ") +
                pFirmwareManagedObject->getState());

		if ((pFirmwareManagedObject->getState() == INSTALL) ||
					(pFirmwareManagedObject->getState() == INSTALL_COMPLETE)) {

			if (!(pFailoverAsynchronousContext->getIsPrincipalChangedWithThisFailover())) {
				fgom_log(GLOG_INFO, "Global FirmwareOM::failover: Skip deletion of %d since the "
					"principal had not changed.", pFirmwareManagedObject->getRbridge_Id());
				continue;
			}

			failed = 1;
            WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: failover"
                    " during Firmware download for rbridgeID ") +
                    pFirmwareManagedObject->getRbridge_Id());
				if (rbridgeids[0] == '\0') {
					snprintf(rbridgeids, 256, "%d", pFirmwareManagedObject->getRbridge_Id());
				} else {
					char temp[256];
					strcpy(temp, rbridgeids);
					snprintf(rbridgeids, 256,"%s, %d", temp,
						pFirmwareManagedObject->getRbridge_Id()); 
				}

			/* Removing the MO */
			startTransaction();
			delete pFirmwareManagedObject;
			(*pResults)[j] = NULL;
			status = commitTransaction();
			if (FRAMEWORK_SUCCESS != status) {
                WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::failover: MO delete couldn't be committed");
		    }
		} else if (pFirmwareManagedObject->getCliState() == CLI_STATUS_BLOCK) {

			if ((pFirmwareManagedObject->getRbridge_Id() == local_mappedId) &&
				nodeActivateRebooted) {
				/*
				 * This node has just rebooted and become a principal
				 * node, reset its blocking state.
				 */
                WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: update MO of rbridge ") + pFirmwareManagedObject->getRbridge_Id());
				startTransaction();
				updateWaveManagedObject(pFirmwareManagedObject);
				pFirmwareManagedObject->setState(ACTIVATE_COMPLETE);
				pFirmwareManagedObject->setCliState(CLI_STATUS_UNBLOCK);
				status = commitTransaction();
				if (FRAMEWORK_SUCCESS != status) {
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: Failed to update MO of rbridge ") + pFirmwareManagedObject->getRbridge_Id());
				}
			} else {
				/*  This rbridge-id is in blocking state */
                WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: block on rbridge ") + pFirmwareManagedObject->getRbridge_Id());
				numNodeBlocked++;
			}
		}
	}

	if (failed) {
		WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::failover: failure raslog - rbridges ") + rbridgeids);
		raslog_ext(__FILE__,__FUNCTION__,__LINE__,RASLOG_OID_INVALID,
			RASLOG_FRCLOG, SULB_1113, rbridgeids);
	}

	/*
	 * If some nodes are in blocking state, start a timer to monior
	 * them. If this principal node was just booted up, the timer
	 * should have been started in configCompletedEventHandler().
	 */
	if (numNodeBlocked) {
		if (cliTimerHandle == 0) {
	        WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::failover: start timer");
			startTimer (cliTimerHandle, 900000,
                    reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareObjectManager::cliTimerHandler));
		}
	}

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::failover: Exit");
	pFailoverAsynchronousContext->setCompletionStatus (status);
	pFailoverAsynchronousContext->callback ();
}

/*
 * FirmwareActivateHandler is being used by both Firmware Activate and Firmware Recover.
 * This function receives the cluster message from the Gateway and then sends the
 * cluter local message to each node. The response from the node will be received by the callback.
*/
	void FirmwareObjectManager::FirmwareActivateHandler(PrismLinearSequencerContext *pFirmwareContext)
{

	UI32 switchId = 0, local_mappedId = 0, entire_cluster = 0;
	UI32 flag = 0, j = 0;
	vector<UI32> rbridgeIdsVector, rbridgeIdsVector_temp;
	ResourceId status = WAVE_MESSAGE_ERROR;
	FirmwareClusterLocalMessage *pMessage = NULL;
	vector<WaveManagedObject *> *pResultsClusterInfo = NULL;
	bool activate_self = false;
	int rid_self = -1, rids = 0, i;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;

	pFirmwareContext->setNumberOfFailures(0);
	++(*pFirmwareContext);

	vector<WaveManagedObject *> *pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults != NULL, __FILE__, __LINE__);

	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());
	flag = pFirmwareClusterMessage->getFlag();

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM: FirmwareActivateHandler: Entry");
	local_mappedId = DcmToolKit::getLocalMappedId();

	rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();

	/*
		The FirmwareClusterMessage needs to return the RID of nodes in the cluster
		back to the Gateway. The RIDs will not be known if the user had given "all"
		for rbridge_id in the command prompt. Hence Clear the rbridgeid contents
		in this message. Form the list later in the CallBack when the nodes reply.
	*/
	rbridgeIdsVector_temp.clear();
	pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector_temp);

	pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());

	if ((rbridgeIdsVector.size() == 1) && (rbridgeIdsVector[0] == 0)) {
		/* User has selected 0. Get the total no of rbridgeids */
		rids = pResultsClusterInfo->size();
		entire_cluster = 1;
	} else {
		rids = rbridgeIdsVector.size();
	}

	WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM: FirmwareActivateHandler: # of switches in cluster = ") + rids + string(" local_map_id= ") + local_mappedId);

	do {

		for (i = 0; i < rids; i++) {

			if (activate_self == true) {
				i = rid_self; /* Run the loop once only for the Local node id*/
				rids = -1; /* Need to exit the for loop once the message is sent to self */
				rid_self = -1; /* Need to exit the outer while loop */
	            fgom_log(GLOG_INFO, "Global FirmwareOM: FirmwareActivateHandler: Activate/Recover self");
			}

	       	WaveNs::trace(TRACE_LEVEL_DEBUG, string("Global FirmwareOM: FirmwareActivateHandler: i = ") + i);

			if (entire_cluster == 0) {
				for (j = 0 ; j < pResultsClusterInfo->size(); j++) {
	                WaveNs::trace(TRACE_LEVEL_DEBUG, string("Global FirmwareOM: FirmwareActivateHandler: j = ") + j);
					VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
						(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[j]));
					if (pVcsNodeLocalManagedObject->getMappedId() == rbridgeIdsVector[i]) {
                        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM: FirmwareActivateHandler: Found switch ID ") + rbridgeIdsVector[i]);
						switchId = rbridgeIdsVector[i];
						break;
					}
				}
				if (j >= pResultsClusterInfo->size()) {
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM: FirmwareActivateHandler: Invalid RID ") + rbridgeIdsVector[i] + string(" Skip."));
					continue;
				}
			} else {
				VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
					(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
				switchId = pVcsNodeLocalManagedObject->getMappedId();
			}

			if ((local_mappedId == switchId) && (activate_self == false) && ((flag & FW_ACTIVATE) || (flag & FW_RECOVER))) {
				rid_self = i; /* Assign the local id to rid_self */
				WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM: FirmwareActivateHandler: Trying to activate/recover self. Skip and Continue\n.");
				continue;
			}

			LocationId locationId = DcmToolKit::getLocationIdFromMappedId(switchId);
	        fgom_log(GLOG_INFO, "Global FirmwareOM: FirmwareActivateHandler: switchId %d", switchId);
			pMessage = new FirmwareClusterLocalMessage ();

			if (flag & FW_ACTIVATE_SANITY) {
				pMessage->setState(ACTIVATE_SANITY);
			} else if (flag & FW_ACTIVATE) {
				pMessage->setState(ACTIVATE);
			} else if (flag & FW_RECOVER_SANITY) {
				pMessage->setState(RECOVER_SANITY);
			} else if (flag & FW_RECOVER) {
				pMessage->setState(RECOVER);
			}

			pMessage->setRbridgeId(switchId);

			status = send (pMessage,
				reinterpret_cast<PrismMessageResponseHandler> (&FirmwareObjectManager::FirmwareActivateCallback), pFirmwareContext, 30000, locationId);

            fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareActivateHandler: Activate message sent to %d", switchId);

			if (WAVE_MESSAGE_SUCCESS != status) {
				pFirmwareContext->incrementNumberOfFailures();
                fgom_log(GLOG_ERR, "Global FirmwareOM::FirmwareActivateHandler: Error in sending message to node %d", switchId);
				if (flag & FW_ACTIVATE) {
					updateFirmwareDownloadStatusMO(switchId, ACTIVATE_REQUEST, FrameworkToolKit::localize(status));
				}
				delete pMessage;
			} else {
                WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateHandler: Message sent to local node ") + switchId);
				if (flag & FW_ACTIVATE) {
					updateFirmwareDownloadStatusMO(switchId, ACTIVATE_REQUEST, string("Success."));
				}
				++(*pFirmwareContext);

				if (flag & FW_ACTIVATE) {
					int FirmwareManagedObjectExist = 0;	

					startTransaction();
					for (j = 0; j < pResults->size(); j++) {
						pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
						if (switchId == pFirmwareManagedObject->getRbridge_Id()) {
							FirmwareManagedObjectExist = 1;
							break;
						}
					}

					if (!FirmwareManagedObjectExist) {
						pFirmwareManagedObject = new FirmwareManagedObject(this);
                        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateHandler: Creating MO for rbridge-id ") + switchId);
					}
					updateWaveManagedObject(pFirmwareManagedObject);
					pFirmwareManagedObject->setState(ACTIVATE);
					pFirmwareManagedObject->setCliState(CLI_STATUS_BLOCK);
					pFirmwareManagedObject->setRbridge_Id(switchId);
					status = commitTransaction();

					if (status == FRAMEWORK_SUCCESS) {
                        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateHandler: state updated for rbridge-id ") + switchId);
					} else {
						pFirmwareContext->incrementNumberOfFailures ();
                        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateHandler: state update failed for rbridge-id ") + switchId);
					}
				}
	        }
	    }

		if (rid_self != -1) {
			activate_self = true;
		} else {
			break;
		}

	} while(true);

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	if (pResultsClusterInfo)
		pResultsClusterInfo->clear();

	// To monitor the nodes that started firmwareactivate
    WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::FirmwareActivateHandler: start timer.");
	if (cliTimerHandle != 0) {
		deleteTimer (cliTimerHandle);
		cliTimerHandle = 0;
	}
	startTimer (cliTimerHandle, 900000, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareObjectManager::cliTimerHandler));

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareActivateHandler: Exit");

	--(*pFirmwareContext);
	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

	void FirmwareObjectManager::FirmwareActivateCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext)
{

	--(*pFirmwareContext);

	UI32 switchId = 0, state = 0, local_node_ret = 0, local_fwdl_status;
	vector<UI32>	rbridgeIdsVector;
	vector<int>		cluster_nodes_ret;
	vector<int>		cluster_fwdl_status;

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareActivateCallback: Entry");
	FirmwareClusterMessage *pFirmwareClusterMessage = dynamic_cast<FirmwareClusterMessage *> (pFirmwareContext->getPPrismMessage());

	if (FRAMEWORK_SUCCESS == frameworkStatus) {

        fgom_log(GLOG_INFO, "Global FirmwareOM::FirmwareActivateCallback: FRAMEWORK_SUCCESS.");

		switchId = pMessage->getRbridgeId();
		if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
            WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateCallback: Message receive failure for rbridge ") + switchId);

			rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();
			rbridgeIdsVector.push_back(switchId);
			pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector);

			cluster_nodes_ret = pFirmwareClusterMessage->getClusterNodesRet();
			cluster_nodes_ret.push_back(-1);
			pFirmwareClusterMessage->setClusterNodesRet(cluster_nodes_ret);

			pFirmwareContext->incrementNumberOfFailures ();
		} else {
			state = pMessage->getState();
			local_node_ret = pMessage->getLocalNodeRet();
            WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateCallback: Message received successfully for rbridge ") + switchId);

			switch (state) {
				case ACTIVATE_SANITY_COMPLETE :
				case RECOVER_SANITY_COMPLETE :
                    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::FirmwareActivateCallback: ret ") + local_node_ret);

					rbridgeIdsVector = pFirmwareClusterMessage->getRbridgeIds();
					rbridgeIdsVector.push_back(switchId);
					pFirmwareClusterMessage->setRbridgeIds(rbridgeIdsVector);

					cluster_nodes_ret = pFirmwareClusterMessage->getClusterNodesRet();
					cluster_nodes_ret.push_back(local_node_ret);
					pFirmwareClusterMessage->setClusterNodesRet(cluster_nodes_ret);

					local_fwdl_status = pMessage->getLocalFwdlStatus();
					cluster_fwdl_status = pFirmwareClusterMessage->getClusterFwdlStatus();
					cluster_fwdl_status.push_back(local_fwdl_status);
					pFirmwareClusterMessage->setClusterFwdlStatus(cluster_fwdl_status);
					break;
				default :
					break;
			}
		}
	}

	if (NULL != pMessage)
		delete pMessage;

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::FirmwareActivateCallback: Exit");

	pFirmwareContext->executeNextStep (((pFirmwareContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}


void FirmwareObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::listenForEvents: Entry");

	listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&FirmwareObjectManager::configReplayCompletedEventHandler));

	listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&FirmwareObjectManager::nodeAddedEventHandler));

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::listenForEvents: Exit");
	pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void FirmwareObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	int j;
	int numNodeBlocked = 0;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::boot: Entry");
	/* Get all of nodes that are blocking */
	pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);

	/*
	 * Set nodeActivateRebooted to 1 to indicate that this
	 * node has rebooted.
	 */
	nodeActivateRebooted = 1;

	/* Find the nodes that are blocking */
	for (j = 0; j < (int) pResults->size(); j++) {
		pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::boot: Rbridge Id - ") + pFirmwareManagedObject->getRbridge_Id() + string(" State - ") + pFirmwareManagedObject->getState());

		if (pFirmwareManagedObject->getCliState() == CLI_STATUS_BLOCK) {
            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::boot: blocked");
			numNodeBlocked++;
		}
	}

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	/*
	 * For nodes that are in blocking state, their state will be
	 * reset after they join the cluster. Start a timer to monitor
	 * them in case they don't join.
	 */
	if (numNodeBlocked) {
		if (cliTimerHandle == 0) {
            fgom_log(GLOG_CRIT, "Global FirmwareOM::boot: start timer");
			startTimer (cliTimerHandle, 900000, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareObjectManager::cliTimerHandler));
		}
	}

    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::boot: Exit");
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}


void FirmwareObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::shutdown: Entry");
	if (cliTimerHandle != 0) {
        fgom_log(GLOG_INFO, "Global FirmwareOM::shutdown: deleting timer");
		deleteTimer (cliTimerHandle);
		cliTimerHandle = 0;
	}
    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::shutdown: Exit");
	pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForShutDownPhases->callback ();
}

void FirmwareObjectManager::configReplayCompletedEventHandler(const WaveNewNodesAddedEvent* &pEvent)
{
    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::configReplayCompleted: Entry");
    WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::configReplayCompleted: Exit");
	reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void FirmwareObjectManager::nodeAddedEventHandler(const WaveNewNodesAddedEvent* &pEvent)
{
	UI32 i;
	int j;
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	int numNodeBlocked = 0;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	/* Get all of nodes that are in blocking state */
	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::AddEventHandler: Entry");
	pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);
	for (j = 0; j < (int) pResults->size(); j++) {
		pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
		if (pFirmwareManagedObject->getCliState() == CLI_STATUS_BLOCK) {
	        WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::AddEventHandler: not Done");
			numNodeBlocked++;
		}
	}

	/* Return if there are not nodes in blocking state */
	if (!numNodeBlocked) {
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	    fgom_log(GLOG_INFO, "Global FirmwareOM::AddEventHandler: No nodes in blocked state. Exit");
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
		return;
	}

	for(i = 0; i < pEvent->getNumberOfNewLocations(); i++) {
		LocationId eventLocationId = pEvent->getLocationIdAtIndex(i);
	    fgom_log(GLOG_INFO, "Global FirmwareOM::AddEventHandler: node %d", eventLocationId);

		/*
		 * Find the nodes that have joined or re-joined the cluster and
		 * reset their blocking state
		 */
		for (j = 0; j < (int) pResults->size(); j++) {
			pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
			LocationId thisLocationId = DcmToolKit::getLocationIdFromMappedId(pFirmwareManagedObject->getRbridge_Id());

	        WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::AddEventHandler: Rbridge Id ") + pFirmwareManagedObject->getRbridge_Id() 
			+ string(" State - ") + pFirmwareManagedObject->getState());

			if ((thisLocationId == eventLocationId) && nodeActivateRebooted &&
				(pFirmwareManagedObject->getCliState() == CLI_STATUS_BLOCK)) {

	            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::AddEventHandler: updating MO");
				startTransaction();
				updateWaveManagedObject(pFirmwareManagedObject);
				pFirmwareManagedObject->setState(ACTIVATE_COMPLETE);
				pFirmwareManagedObject->setCliState(CLI_STATUS_UNBLOCK);
				status = commitTransaction();
				if (FRAMEWORK_SUCCESS != status) {
	                WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::AddEventHandler: update MO failed");
				}
				--numNodeBlocked;
				updateFirmwareDownloadStatusMO(pFirmwareManagedObject->getRbridge_Id(), NODE_JOIN, string(""));
			}
		}
	}

	/*
	 * If no nodes are in blocking state, remove all MO's
	 * and unblock CLIs.
	 */
	if (!numNodeBlocked) {
	    WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::AddEventHandler: Done.  Removiing all MOs");
		for (j = 0; j < (int) pResults->size(); j++) {
			pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);
			startTransaction();
			delete	pFirmwareManagedObject;
			(*pResults)[j] = NULL;
			status = commitTransaction();
			if (FRAMEWORK_SUCCESS != status) {
	            WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::AddEventHandler: remove MO failed");
			}
		}

		if (cliTimerHandle != 0) {
			deleteTimer (cliTimerHandle);
			cliTimerHandle = 0;
		}
	}

	if (pResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::AddEventHandler: Exit");
	reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void FirmwareObjectManager::cliTimerHandler (TimerHandle &timerHandle)
{
	int j = 0;
	ResourceId status = WAVE_MESSAGE_ERROR;
	FirmwareManagedObject *pFirmwareManagedObject = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::TimerHandler: Entry");

	/*
	 * Get all of nodes that are running firmwareactivate,
	 * delete their MO's, and unblock CLIs
	 */
	pResults = querySynchronously (FirmwareManagedObject::getClassName());
	prismAssert (pResults, __FILE__, __LINE__);

	for (j = 0; j < (int) pResults->size(); j++) {
		pFirmwareManagedObject = dynamic_cast<FirmwareManagedObject *> ((*pResults)[j]);

	    WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::TimerHandler: Rbridge Id ") + pFirmwareManagedObject->getRbridge_Id() +
		string(", State ") + pFirmwareManagedObject->getState() + string(", CliState ") + pFirmwareManagedObject->getCliState());

		if (pFirmwareManagedObject->getState() == ACTIVATE) {
			/*
			 * Firmware MO will set the state as ACTIVATE prior to reboot.
			 * As the timer had expired, check for the MO that still have
			 * the state as ACTIVATE. These should be the nodes that havent
			 * joined the cluster after activate.
			 */
			updateFirmwareDownloadStatusMO(pFirmwareManagedObject->getRbridge_Id(), NODE_JOIN_FAILURE, string(""));
		}

        startTransaction ();
		delete pFirmwareManagedObject;
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS != status) {
	        WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareOM::TimerHandler: failed to delete MO");
		}
	}

	if (pResults)
		pResults->clear();

	/* delete the timer */
	deleteTimer (cliTimerHandle);
	cliTimerHandle = 0;

	WaveNs::trace(TRACE_LEVEL_DEBUG, "Global FirmwareOM::TimerHandler: Exit");
}

/*
*	Global Firmwaredownload Status MO to record the fwdl status in each node. These MO
*	will be retrieved by the Firmwareshow Actionpoint to display the status.
*/
void FirmwareObjectManager::updateFirmwareDownloadStatusMO (UI32 rid_local, int status, string mesg)
{

	time_t	fwdl_time;
	char	str_fwdl_time[40];
	FirmwareDownloadStatusManagedObject	*pFirmwareDownloadStatusManagedObject = NULL;

	WaveNs::trace(TRACE_LEVEL_INFO, string("Global FirmwareOM::updateFirmwareDownloadStatusMO: Local ") + rid_local + string(", Status ") + status);

	fwdl_time = time(0);
	ctime_r(&fwdl_time, str_fwdl_time);

	startTransaction();
	pFirmwareDownloadStatusManagedObject = new FirmwareDownloadStatusManagedObject(this);
	if (pFirmwareDownloadStatusManagedObject != NULL) {
		updateWaveManagedObject(pFirmwareDownloadStatusManagedObject);
		pFirmwareDownloadStatusManagedObject->setRbridge_Id_Local(rid_local);
		pFirmwareDownloadStatusManagedObject->setStatus(status);
		pFirmwareDownloadStatusManagedObject->setMesg(mesg);
		pFirmwareDownloadStatusManagedObject->setRbridge_Id_Principal(DcmToolKit::getLocalMappedId());
		pFirmwareDownloadStatusManagedObject->setTime(string(str_fwdl_time));
	}

	if (commitTransaction() != FRAMEWORK_SUCCESS)
		WaveNs::trace(TRACE_LEVEL_INFO, "Global FirmwareObjectManager::updateFirmwareDownloadStatusMO : Failed to create sanity MO.");

	delete pFirmwareDownloadStatusManagedObject;
}

}
