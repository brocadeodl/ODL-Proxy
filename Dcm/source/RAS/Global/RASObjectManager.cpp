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
 *   Author : James Chen                                               *
 ***************************************************************************/
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Types/IpVxAddress.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
// #include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Global/RASObjectManager.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASSupportMessage.h"
#include "RAS/Global/RASManagedObject.h"
#include "RAS/Global/RASSupportClusterMessage.h"
#include "AAA/Global/AAACrypto.h"
// #include "ClientInterface/DceWaveClientMessage.h"
//#include "ClientInterface/RAS/RASClientMessage.h"
#include "Shell/ShellPrism.h"
#include "vcs.h"		
//#include "RAS.h"		// Module name of RAS.yang

namespace DcmNs
{


RASObjectManager::RASObjectManager () : WaveObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());

    RASManagedObject rasManagedObject (this);
    rasManagedObject.setupOrm ();
    addManagedClass (RASManagedObject::getClassName ());
	
    RASGlobalConfigManagedObject RASGlobalConfig (this);
    RASGlobalConfig.setupOrm ();
    addManagedClass (RASGlobalConfigManagedObject::getClassName ());

    RASLOGConfigureManagedObject RASLOGConfigure (this);
    RASLOGConfigure.setupOrm ();
    addManagedClass (RASLOGConfigureManagedObject::getClassName ());

    RASMODConfigureManagedObject RASMODConfigure (this);
    RASMODConfigure.setupOrm ();
    addManagedClass (RASMODConfigureManagedObject::getClassName ());

	RASAutoUploadManagedObject RASAutoUpload (this);
	RASAutoUpload.setupOrm ();
	addManagedClass (RASAutoUploadManagedObject::getClassName ());

    RASAuditManagedObject RASAudit (this);
    RASAudit.setupOrm ();
    addManagedClass (RASAuditManagedObject::getClassName ());

	RASSysFcManagedObject RASsys (this);
	RASsys.setupOrm ();
	addManagedClass (RASSysFcManagedObject::getClassName ());


    // register to receive RAS_COPY_SUPPORT msg
    // addOperationMap (RAS_COPY_SUPPORT, reinterpret_cast<PrismMessageHandler> (&RASObjectManager::RASSupportSaveMessageAsyncHandler));
    addOperationMap (RAS_CLUSTER_CFG, reinterpret_cast<PrismMessageHandler> (&RASObjectManager::RASClusterCfgMessageHandler));
    addOperationMap (RAS_FOSCLI, reinterpret_cast<PrismMessageHandler> (&RASObjectManager::RASFoscliMessageHandler));
    //*addOperationMap (RAS_VCSLOGGING, reinterpret_cast<PrismMessageHandler> (&RASObjectManager::RASClientMessageHandler));
    addOperationMap (RAS_NETCONF, reinterpret_cast<PrismMessageHandler> (&RASObjectManager::RASClusterNetConfMessageHandler));
}

RASObjectManager::~RASObjectManager ()
{
}

string RASObjectManager::getClassName ()
{
    return ("RASObjectManager");
}

RASObjectManager *RASObjectManager::getInstance ()
{
    static RASObjectManager *pRASObjectManager = new RASObjectManager ();

    WaveNs::prismAssert (NULL != pRASObjectManager, __FILE__, __LINE__);

    return (pRASObjectManager);
}

PrismServiceId RASObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void RASObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, string ("RASOM::failover failover reason") +
	   FrameworkToolKit::localize(failoverReason));

    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason) {
        UI32 noOfFailedLocations = failedLocationIds.size ();

        for (UI32 i = 0; i < noOfFailedLocations; i++) {
            vector<WaveManagedObject *> *pWaveManagedObjects =
		    querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i],
						LoggingLocalManagedObject::getClassName ());

            trace (TRACE_LEVEL_INFO, string ("RASOM::failover for Location Id ") +
		   failedLocationIds[i]);

            startTransaction ();

            UI32 sizeOfManagedObjects = pWaveManagedObjects->size ();

            for (UI32 j = 0; j < sizeOfManagedObjects; j++) {
                delete (*pWaveManagedObjects)[j];
            }   

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "RASOM::failover can not commit local managed object delete to database ");
                prismAssert (false, __FILE__, __LINE__);
            }

            pWaveManagedObjects->clear ();

            delete pWaveManagedObjects;
        }
    }
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}



vector<WaveManagedObject *> *RASObjectManager::DoSynchronousQuery (const string className)
{
	vector<WaveManagedObject *> *pResults = querySynchronously (className);
	prismAssert (NULL != pResults, __FILE__, __LINE__);
	return pResults;
}


WaveManagedObject *RASObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if (RASManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new RASManagedObject (this);
    } else if (RASAutoUploadManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new RASAutoUploadManagedObject (this);
    } else if (RASLOGConfigureManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new RASLOGConfigureManagedObject (this);
    } else if (RASMODConfigureManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new RASMODConfigureManagedObject (this);
    } else if (RASAuditManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new RASAuditManagedObject (this);
	} else if (RASSysFcManagedObject::getClassName() == managedClassName) {
		pWaveManagedObject = new RASSysFcManagedObject (this);
	} else if (RASGlobalConfigManagedObject::getClassName() == managedClassName) {
		pWaveManagedObject = new RASGlobalConfigManagedObject (this);
    } else {
        trace (TRACE_LEVEL_ERROR, "RASObjectManager::createManagedObjectInstance : unsupported managed class name : " + managedClassName);
    }

    return pWaveManagedObject;
}

PrismMessage *RASObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
 
   tracePrintf(TRACE_LEVEL_DEBUG,
                  "RASOM::createMessageInstance opcode=%d", operationCode);

    switch (operationCode)
    {
        case RAS_COPY_SUPPORT :
	    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::createMessageInstance:: RAS_COPY_SUPPORT");
            pPrismMessage = new RASClusterMessage ();
            break;
        case RAS_CLUSTER_CFG :
	    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::createMessageInstance:: RAS_COPY_SUPPORT");
            pPrismMessage = new RASClusterMessage ();
            break;
        //case RAS_VCSLOGGING :
	    //trace (TRACE_LEVEL_DEBUG, "RASObjectManager::createMessageInstance:RAS_VCSLOGGING");
        //    pPrismMessage = new RASClientMessage ();
        //    break;
        case RAS_NETCONF:
	    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::createMessageInstance:RAS_NETCONF");
            pPrismMessage = new RASClusterNetConfMessage();
        case RAS_FOSCLI :
		    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::createMessageInstance:: RAS_FOSCLI");
            pPrismMessage = new RASClusterMessage ();
            break;
        default :
            tracePrintf(TRACE_LEVEL_INFO,
                  "RASObjectManager::createMessageInstance Invalid opcode=%d", operationCode);
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void RASObjectManager::install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	// RASManagedObject *pRASMO = NULL;	
	RASAuditManagedObject *pRASAMO_SEC = NULL, *pRASAMO_CONF = NULL, *pRASAMO_FW = NULL;	
	RASSysFcManagedObject *pRASSysFc = NULL;
	RASGlobalConfigManagedObject *pRASGlobalConfig = NULL;
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    /****************************************************************************
     * Create managed object and commit to database
     ***************************************************************************/
    trace (TRACE_LEVEL_INFO, "RASObjectManager::install: Installing RAS MO DB");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    WaveNs::trace (TRACE_LEVEL_INFO, string ("RASObjectManager::install:boot=") + bootReason);

    /* Node is out-of-the-box or is rebooting after net-install is performed */
    /* 0xC0002, 0xC000B */
    if ((WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason) || 
	(WAVE_BOOT_FIRST_TIME_BOOT == bootReason)) {	
 
        startTransaction ();

	if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason) {
		pRASAMO_SEC = new RASAuditManagedObject (this, SECURITY);
		pRASAMO_CONF = new RASAuditManagedObject (this, CONFIGURATION);
		pRASAMO_FW = new RASAuditManagedObject (this, FIRMWARE);
	}

	/* set local7 as default syslog facility */
	pRASSysFc = new RASSysFcManagedObject (this, RAS_LOG_LOCAL7);
	pRASGlobalConfig = new RASGlobalConfigManagedObject (this);
	status = commitTransaction ();

	if (FRAMEWORK_SUCCESS == status) {
            trace (TRACE_LEVEL_INFO, "RASObjectManager::install: Installing RAS DB complete");
	    status = WAVE_MESSAGE_SUCCESS;

	} else {
            trace (TRACE_LEVEL_FATAL, "RASObjectManager::install: Failed to install RAS DB");
	    prismAssert (false, __FILE__, __LINE__);
	}

	if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason) {
		delete pRASAMO_SEC;
		delete pRASAMO_CONF;
		delete pRASAMO_FW;
	}

	delete pRASSysFc;
	delete pRASGlobalConfig;
    }

    if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) {
        trace(TRACE_LEVEL_DEBUG, "RASObjectManager::install: secondary configure boot");
	// Global OM install will skip second time boot, need push syslogdIp from Local OM
	// get syslogIp from Global MO and push them to syslogd
	status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

 /************************************************************************
  * Cluster Cfg
  ************************************************************************/

void RASObjectManager::RASClusterCfgMessageHandler (RASClusterMessage *pRASClusterMessage)
{

    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::RASClusterCfgMessageHandler : Entering ...");

     PrismLinearSequencerStep sequencerSteps[] =
    {

	reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::cfgMessageHandler),
        reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerFailedStep),
    };

    RASMessagingContext *pRASMessagingContext = new RASMessagingContext (pRASClusterMessage, this,
			    sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRASMessagingContext->setPMessage (pRASClusterMessage);

    pRASMessagingContext->start ();
}

void RASObjectManager::RASFoscliMessageHandler(RASClusterMessage *pRASClusterMessage)
{

    trace (TRACE_LEVEL_DEBUG, "RASObjectManager::RASFoscliMessageHandler: Entering ...");

     PrismLinearSequencerStep sequencerSteps[] =
    {

	reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::foscliMessageHandler),
        reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerFailedStep),
    };

    RASMessagingContext *pRASMessagingContext = new RASMessagingContext (pRASClusterMessage, this,
			    sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRASMessagingContext->setPMessage (pRASClusterMessage);

    pRASMessagingContext->start ();
}

void RASObjectManager::foscliMessageHandler(RASMessagingContext *pRASMessagingContext)
{
   ResourceId status = WAVE_MESSAGE_ERROR;
   ResourceId messageStatus = WAVE_MESSAGE_ERROR;
   const ObjectId  pOID;
   RASLoggingMessage *pMessage = NULL;
	string result = "";
   UI16 mappedId = 0;
   string data;
   // MSG from Actionpoint worker to carry data
   RASClusterMessage *pRASClusterMessage = dynamic_cast<RASClusterMessage *> (pRASMessagingContext->getPPrismMessage ());

    WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::Entering foscliMessageHandler"));
    // get cluser info
    vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
    UI32 nodesInCluster = pResultsClusterInfo->size ();
    WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::getClusterInfo: Getting information for : ")+ nodesInCluster + " switches");

    pRASMessagingContext->setNumberOfFailures (0);
    if (false == pRASClusterMessage->getSync())
    ++(*pRASMessagingContext);

    UI32 i;
    for (i = 0; i < nodesInCluster; i++) {

    // check if "clear logging all"    
        if (pRASClusterMessage->getMappedId() == 0) {
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
         (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
        mappedId = pVcsNodeLocalManagedObject->getMappedId ();

    } else {
            // none all option  
            mappedId = pRASClusterMessage->getMappedId();
    }
        LocationId locationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
        tracePrintf(TRACE_LEVEL_DEBUG,
                "RASObjectManager::foscliAsync: Send msg to "
            "mappedId=%d locationId=%d", mappedId, locationId);

    pMessage = new RASLoggingMessage (pOID);
    pMessage->setMappedId(mappedId);

    // setup foscli cmd
        WaveNs::trace (TRACE_LEVEL_DEBUG, "RASObjectManager::foscliAsync: CLI in ClusterMessage = " +
                pRASClusterMessage->getCmd());
    pMessage->setCmd(pRASClusterMessage->getCmd());
        pMessage->setNetConf(pRASClusterMessage->getNetConf());
    pMessage->setCLIStart(pRASClusterMessage->getCLIStart());
    pMessage->setCLIEnd(pRASClusterMessage->getCLIEnd());
        pMessage->setCLIOutput(pRASClusterMessage->getCLIOutput());
        pMessage->setSync(pRASClusterMessage->getSync());

    trace (TRACE_LEVEL_DEBUG, string ("RASObjectManager::WaveClientContext= ") +
        pRASClusterMessage->getWaveClientOriginatingLocationId () + " " +
            pRASClusterMessage->getWaveNativeClientId () + " " + pRASClusterMessage->getWaveUserClientId());

    WaveClientSessionContext waveClientSessionContext = pRASClusterMessage->getWaveClientSessionContext ();
    pMessage->setWaveClientOriginatingLocationId(pRASClusterMessage->getWaveClientOriginatingLocationId ());
    pMessage->setWaveNativeClientId(pRASClusterMessage->getWaveNativeClientId ());
    pMessage->setWaveUserClientId(pRASClusterMessage->getWaveUserClientId());

        WaveNs::trace (TRACE_LEVEL_DEBUG, "RASObjectManager::foscliAsync: CLI in LocalMessage = " +
            pMessage->getCmd());

        if (pRASClusterMessage->getSync()) {
            status = sendSynchronously(pMessage, locationId);
        } else {
            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler>
                (&RASObjectManager::foscliAsyncMessageCallback),
               pRASMessagingContext, pRASClusterMessage->getCLITimeout(), locationId);
        }

    if (WAVE_MESSAGE_SUCCESS != status) {
            pRASMessagingContext->incrementNumberOfFailures ();
        trace (TRACE_LEVEL_INFO, string ("RASObjectManager::foscliAsync: Sending a message to [") +
            PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) +
                    " service] failed.");
		result += (string ("Failed to send message to rbridge-id: ") + mappedId + string("\n\n"));
		data.append(result);
        trace (TRACE_LEVEL_ERROR, "RASObjectManager::foscliAsync : Error in sending message to get this node  : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
        delete pMessage;
    } else {
            if (pRASClusterMessage->getSync()) {
                ResourceId completionStatus = pMessage->getCompletionStatus();
                if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                    pRASMessagingContext->incrementNumberOfFailures();
                    trace (TRACE_LEVEL_ERROR, string("RASObjectManager::foscliAsync : getCompletionStatus Failed for Rbridge ID: ") + mappedId + string("\n"));
					result = (string("The comand failed for rbridge-id: ") + mappedId + string("\n\n"));
					data.append(result);
                    delete pMessage;
                } else {
                    trace (TRACE_LEVEL_ERROR, "RASObjectManager::foscliAsync : Appending data");
                    data.append(pMessage->getData());
                }
            } else {
                ++(*pRASMessagingContext);
            }
    }

    // clear logging for local switch, stop here
    if (pRASClusterMessage->getMappedId() != 0)
        break;
    }
    if (pRASClusterMessage->getSync()) {
        pRASClusterMessage->setData(data);
    } else {
        --(*pRASMessagingContext);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ?
                       WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void  RASObjectManager::foscliAsyncMessageCallback (FrameworkStatus frameworkStatus,
   RASLoggingMessage *pMessage, RASMessagingContext *pRASMessagingContext)
{
    --(*pRASMessagingContext);

    // get Original request msg
    RASClusterMessage *pRASClusterMessage = dynamic_cast<RASClusterMessage *> (pRASMessagingContext->getPMessage ());

    if (FRAMEWORK_SUCCESS == frameworkStatus) {
        prismAssert (pMessage, __FILE__, __LINE__);

	WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASObjectManager::foscliAsyncMSGCB: MID=") + pMessage->getMappedId());

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
	    WaveNs::trace (TRACE_LEVEL_INFO, string ("RASObjectManager::foscliAsyncMSGCB: MID=") +
			   pMessage->getMappedId() + string ("Completion Failure"));
            pRASMessagingContext->incrementNumberOfFailures ();

        } else {
	    // add returned result
	    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASObjectManager::foscliAsyncMSGCB: MID=") +
			   pMessage->getMappedId() + string (" Success"));

	    pRASClusterMessage->setData(pMessage->getData());
	}
    } else {

	WaveNs::trace (TRACE_LEVEL_INFO, string ("RASObjectManager::foscliAsyncMSGCB:FRAMEWROK Error") + frameworkStatus);

	pRASClusterMessage->setData("Failure");
        pRASMessagingContext->incrementNumberOfFailures ();
    }
    if (NULL != pMessage)
        delete pMessage;

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void RASObjectManager::cfgMessageHandler(RASMessagingContext *pRASMessagingContext)
{
		WaveNs::trace (TRACE_LEVEL_DEBUG, "RASObjectManager::cfgMessageHandler: Entering");
		ResourceId status = WAVE_MESSAGE_ERROR;
		ResourceId messageStatus = WAVE_MESSAGE_ERROR;
		RASLoggingMessage *pMessage = NULL;
		UI32 i = 0, j = 0, flag = 0;
		int found = 0;
		bool addOp = false, delOp = false, modOp = false;
		bool is_enabled = false;
		bool syslog_attr = false, msg_disable = false, mod_disable = false;
		IpVxAddress hostip;
		string user, dir, pass, data, password, msgId, modId, msgId_db, proto;
		UI32 supportData = 0;
		IpVxAddress user_ip;
		UI32 log_severity = DCM_RASLOG_DEFAULT;
		UI32 log_severity_db = DCM_RASLOG_DEFAULT;
		UI32 audit_class = 0;
		RASSysFacilityTypes sys_facility;
		UI32 isSingleton = 0;
		Severity console = DCM_RASLOG_INFO;
		bool ffdc = true;

		RASManagedObject *pRASManagedObject = NULL;
		RASGlobalConfigManagedObject *pRASGlobalConfigManagedObject = NULL;
		RASAutoUploadManagedObject *pRASAutoUploadManagedObject = NULL;
		RASLOGConfigureManagedObject *pRASLOGConfigureManagedObject = NULL;
		RASMODConfigureManagedObject *pRASMODConfigureManagedObject = NULL;
		RASAuditManagedObject *pRASAuditManagedObject = NULL;
		RASSysFcManagedObject *pRASSysFcManagedObject = NULL;

		pRASMessagingContext->setNumberOfFailures (0);
		++(*pRASMessagingContext);

		RASClusterMessage *pRASClusterMessage = dynamic_cast<RASClusterMessage *> (pRASMessagingContext->getPMessage ());
		prismAssert (pRASClusterMessage, __FILE__, __LINE__);

		UI32 xmlTag = pRASClusterMessage->getWyserUserTag();
		vector<WaveManagedObject *> *pResults = NULL;
		vector<WaveManagedObject *> *pRemove = NULL;
		vector<WaveManagedObject *> *pMod = NULL;
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;

		if (xmlTag == ras_syslog_server || xmlTag == ras_port || xmlTag == ras_secure) {
				user_ip = pRASClusterMessage->getSyslogIp();
				pResults = querySynchronously (RASManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASManagedObject = dynamic_cast<RASManagedObject *> ((*pResults)[j]);
						if (user_ip == pRASManagedObject->getSyslogIp()) {
								found = 1;
                                break;
                        }
				}
		} else if (xmlTag == ras_enable) {
				is_enabled = pRASClusterMessage->getAutoSupport();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
//						if (true == pRASAutoUploadManagedObject->getAutoSupport())
								found = 1;
				}
		} else if (xmlTag == ras_autoupload_param) {
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
								found = 1;
				}
		} else if (xmlTag == ras_hostip) {
				hostip = pRASClusterMessage->getHostIp();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
						if (hostip == pRASAutoUploadManagedObject->getHostIp())
								found = 1;
				}
		} else if (xmlTag == ras_username) {
				user = pRASClusterMessage->getUser();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
						if (user == pRASAutoUploadManagedObject->getUser())
								found = 1;
				}
		} else if (xmlTag == ras_protocol) {
				proto = pRASClusterMessage->getProtocol();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
                prismAssert (pResults, __FILE__, __LINE__);		
				 for (j = 0; j < pResults->size (); j++) {
                        pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
                        if (proto == pRASAutoUploadManagedObject->getProtocol())
                                found = 1; 
                }
		} else if (xmlTag == ras_directory) {
				dir = pRASClusterMessage->getDirectory();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
						if (dir == pRASAutoUploadManagedObject->getDirectory())
								found = 1;
				}
		} else if (xmlTag == ras_password) {
				pass = pRASClusterMessage->getPassword();
				pResults = querySynchronously (RASAutoUploadManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[j]);
						password = pRASAutoUploadManagedObject->getPassword();
						if (password.length() != 0) {
							password = AAACrypto::decryptPassword(password);
							if (password.length() == 0) {
								tracePrintf(TRACE_LEVEL_ERROR, "RASObjectManager::getRASManagedObject Password Decryption failed");
							}
						}
						if (pass == password)
								found = 1;
				}
		} else if (xmlTag == ras_severity) {
				log_severity = pRASClusterMessage->getSeverity();
				msgId = pRASClusterMessage->getMsgId();
				pResults = querySynchronously (RASLOGConfigureManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[j]);
						if ((strcmp(msgId.c_str(), (pRASLOGConfigureManagedObject->getMsgId()).c_str()) == 0) || (DCM_RASLOG_DEFAULT == log_severity)) {
							log_severity_db = pRASLOGConfigureManagedObject->getSeverity();
							found = 1;
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::log_severity_db=") +
									log_severity_db);
						}
				}
		} else if (xmlTag == ras_syslog) {
				syslog_attr = pRASClusterMessage->getSyslogEnable();
				msgId = pRASClusterMessage->getMsgId();
				pResults = querySynchronously (RASLOGConfigureManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[j]);
						if (strcmp(msgId.c_str(), (pRASLOGConfigureManagedObject->getMsgId()).c_str()) == 0) {
							flag = 1;
							if (!(strcmp((pRASClusterMessage->getCmd()).c_str(),"add") == 0)) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::not add command"));
								found = 1;
							}
							msgId_db = pRASLOGConfigureManagedObject->getMsgId();
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::msgId_db=") + msgId_db);
						}
				}
		} else if (xmlTag == ras_message) {
				msg_disable = pRASClusterMessage->getMessageDisable();
				msgId = pRASClusterMessage->getMsgId();
				pResults = querySynchronously (RASLOGConfigureManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[j]);
						if (strcmp(msgId.c_str(), (pRASLOGConfigureManagedObject->getMsgId()).c_str()) == 0) {
							flag = 1;
							if (!(strcmp((pRASClusterMessage->getCmd()).c_str(),"add") == 0)) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::not add command"));
								found = 1;
							}
							msgId_db = pRASLOGConfigureManagedObject->getMsgId();
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::msgId_db=") + msgId_db);
						}
				}
		} else if (xmlTag == ras_module) {
				mod_disable = pRASClusterMessage->getModuleDisable();
				modId = pRASClusterMessage->getModId();
				pResults = querySynchronously (RASMODConfigureManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASMODConfigureManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {	
						pRASMODConfigureManagedObject = dynamic_cast<RASMODConfigureManagedObject *> ((*pResults)[j]);
						if (strcmp(modId.c_str(), (pRASMODConfigureManagedObject->getModId()).c_str()) == 0)
								found = 1;
				}
		 } else if (xmlTag == ras_auditlog) {
				audit_class = pRASClusterMessage->getAuditLogclass();
				pResults = querySynchronously (RASAuditManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASAuditManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASAuditManagedObject = dynamic_cast<RASAuditManagedObject *> ((*pResults)[j]);
						if (audit_class == pRASAuditManagedObject->getAuditLogclass())
								found = 1;
				}
		} else if (xmlTag == ras_syslog_facility) {
				isSingleton = 1;
				sys_facility = pRASClusterMessage->getSysLogfacility();
				pResults = querySynchronously (RASSysFcManagedObject::getClassName());
				synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASSysFcManagedObject::getClassName());
				prismAssert (pResults, __FILE__, __LINE__);
				for (j = 0; j < pResults->size (); j++) {
						pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pResults)[0]);
						if (sys_facility == (RASSysFacilityTypes) pRASSysFcManagedObject->getSysFacility())
								found = 1;
				}
		} else if (xmlTag == ras_ffdc) {
			ffdc = pRASClusterMessage->getBoolData();
			pResults = querySynchronously (RASGlobalConfigManagedObject::getClassName());
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASGlobalConfigManagedObject::getClassName());
			prismAssert (pResults, __FILE__, __LINE__);
			if (pResults->size () > 0) {
				pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pResults)[0]);
					found = 1;
			}
		} else if (xmlTag == ras_console) {
			console = pRASClusterMessage->getEnumData();
			pResults = querySynchronously (RASGlobalConfigManagedObject::getClassName());
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(RASGlobalConfigManagedObject::getClassName());
			prismAssert (pResults, __FILE__, __LINE__);
			if (pResults->size () > 0) {
				pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pResults)[0]);
					found = 1;
			}
		}
		tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject sz=%d found=%d",
						pResults->size(), found);

		if ((strcmp((pRASClusterMessage->getCmd()).c_str(),"delete") == 0) && found) {
				if (xmlTag == ras_syslog_server) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject delete=%s",
										(user_ip).toString().c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeIpVxAddress (user_ip, "ipaddress"));
				} else if (xmlTag == ras_enable) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject delete enable = %d", is_enabled);
						is_enabled = pRASAutoUploadManagedObject->getAutoSupport();
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject delete enable1 = %d", is_enabled);
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeBool (is_enabled, "autosupport"));
				} else if (xmlTag == ras_autoupload_param) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete Params");
						hostip = pRASAutoUploadManagedObject->getHostIp();
						user = pRASAutoUploadManagedObject->getUser();
						dir = pRASAutoUploadManagedObject->getDirectory();
						pass = pRASAutoUploadManagedObject->getPassword();
						proto = pRASAutoUploadManagedObject->getProtocol();						
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeIpVxAddress (hostip, "hostip"));
						synchronousQueryContext->addAndAttribute (new AttributeString (user, "username"));
						synchronousQueryContext->addAndAttribute (new AttributeString (dir, "directory"));
						synchronousQueryContext->addAndAttribute (new AttributeString (pass, "password"));
						synchronousQueryContext->addAndAttribute (new AttributeString (proto, "protocol"));
				} else if (xmlTag == ras_hostip) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete=%s",
										(hostip).toString().c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeIpVxAddress (hostip, "hostip"));
				} else if (xmlTag == ras_username) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete=%s",
										user.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (user, "username"));
				} else if (xmlTag == ras_protocol) {
						 tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete=%s",
                                        proto.c_str());
						delOp = true;
                        synchronousQueryContext->addAndAttribute (new AttributeString (proto, "protocol"));
				} else if (xmlTag == ras_directory) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete=%s",
										dir.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (dir, "directory"));
				} else if (xmlTag == ras_password) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAutoUploadManagedObject delete=%s",
										pass.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (pass, "password"));
				} else if (xmlTag == ras_severity) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGCManagedObject delete=%s",
										msgId.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (msgId, "msgId"));
				} else if (xmlTag == ras_syslog) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGCManagedObject delete=%s",
										msgId.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (msgId, "msgId"));
				} else if (xmlTag == ras_message) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGCManagedObject delete=%s",
										msgId.c_str());
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (msgId, "msgId"));
				} else if (xmlTag == ras_module) {
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeString (modId, "modId"));
				} else if (xmlTag == ras_auditlog) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject delete=%d", audit_class);
						delOp = true;
						synchronousQueryContext->addAndAttribute (new AttributeEnum (audit_class, "auditlogclass"));
				}
				if (!(xmlTag == ras_enable)) {
					pRemove = querySynchronously (synchronousQueryContext);
				if ((NULL == pRemove) || (pRemove->size() == 0))
				{
						if (xmlTag == ras_syslog_server)
								trace (TRACE_LEVEL_FATAL, "syslog ip not found");
						else if (xmlTag == ras_enable)
								trace (TRACE_LEVEL_FATAL, "autoupload enable not found");
						else if (xmlTag == ras_autoupload_param)
								trace (TRACE_LEVEL_FATAL, "autoupload param not found");
						else if (xmlTag == ras_hostip) 
								trace (TRACE_LEVEL_FATAL, "hostip not found");
						else if (xmlTag == ras_username) 
								trace (TRACE_LEVEL_FATAL, "user not found");
						else if (xmlTag == ras_protocol) 
								trace (TRACE_LEVEL_FATAL, "proto not found");
						else if (xmlTag == ras_directory)
								trace (TRACE_LEVEL_FATAL, "dir not found");
						else if (xmlTag == ras_password)
								trace (TRACE_LEVEL_FATAL, "pass not found");
						else if (xmlTag == ras_severity)
								trace (TRACE_LEVEL_FATAL, "set raslog severity not found");
						else if (xmlTag == ras_syslog)
								trace (TRACE_LEVEL_FATAL, "syslog attr not found");
						else if (xmlTag == ras_message)
								trace (TRACE_LEVEL_FATAL, "message config not found");
						else if (xmlTag == ras_module)
								trace (TRACE_LEVEL_FATAL, "module config not found");
						else if (xmlTag == ras_auditlog)
								trace (TRACE_LEVEL_FATAL, "auditlog class not found");
						else if (xmlTag == ras_syslog_facility)
								trace (TRACE_LEVEL_FATAL, "syslog facility not found");
						if (pRemove != NULL)
								WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pRemove);
						if (pResults)
								WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
						delete synchronousQueryContext;
						--(*pRASMessagingContext);
						pRASMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
						return;
				}
			}
		} else if ((strcmp((pRASClusterMessage->getCmd()).c_str(),"add") == 0) && !found) {
				if (xmlTag == ras_syslog_server)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject add=%s",
										user_ip.toString().c_str());
				else if (xmlTag == ras_enable)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add bool=%d", is_enabled);
				else if (xmlTag == ras_hostip)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add =%s",
										hostip.toString().c_str());
				else if (xmlTag == ras_username)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add =%s",
										user.c_str());
				else if (xmlTag == ras_protocol)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add =%s",
                                        proto.c_str());
				else if (xmlTag == ras_directory) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add =%s",
										dir.c_str());
				else if (xmlTag == ras_password)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject add =%s",
										pass.c_str());
				else if (xmlTag == ras_severity)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGCManagedObject add/change sev=%d",
										log_severity);
				else if (xmlTag == ras_syslog) {
						if (flag) 
							synchronousQueryContext->addAndAttribute (new AttributeString (msgId_db, "msgId"));
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject addsyslgattr=%d", syslog_attr);
				}
				else if (xmlTag == ras_message) {
						if (flag) 
							synchronousQueryContext->addAndAttribute (new AttributeString (msgId_db, "msgId"));
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject add message conf=%d", msg_disable);
				}
				else if (xmlTag == ras_module)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject add mod conf=%d", mod_disable);
				else if (xmlTag == ras_auditlog)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject add_class=%d", audit_class);
				else if (xmlTag == ras_syslog_facility)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject add_sys_fc=%d", sys_facility);
				else if (xmlTag == ras_console)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASGlobalConfigManagedObject ffdc=%d", ffdc);
				else if (xmlTag == ras_ffdc)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASGlobalConfigManagedObject console=%d", console);
				addOp = true;
				if (flag) 
					modOp = true;
		} else if ((strcmp((pRASClusterMessage->getCmd()).c_str(),"update") == 0)) {
				if (xmlTag == ras_syslog_facility) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_sys_fc=%d", sys_facility);
				} else if (xmlTag == ras_port) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_sys_port");
                } else if (xmlTag == ras_secure) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_sys_secure_mode");
				}
				if (xmlTag == ras_enable)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASAUManagedObject update bool=%d", is_enabled);
				if (xmlTag == ras_hostip) {				
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_hostip=%s", hostip.toString().c_str());
				}
				if (xmlTag == ras_username)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_user=%s", user.c_str());
				if (xmlTag == ras_protocol)
                        tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_proto=%s", proto.c_str());
				if (xmlTag == ras_directory)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_dir=%s", dir.c_str());
				if (xmlTag == ras_password) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject update_pass=%s", pass.c_str());				
				if (xmlTag == ras_ffdc) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject ffdc=%d", ffdc);				
				if (xmlTag == ras_console) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject console=%d", console);				
				if (xmlTag == ras_syslog)
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject update_syslog attr=%d",
							syslog_attr);
				if (xmlTag == ras_severity) {
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject update sev=%d",
							log_severity_db);
						synchronousQueryContext->addAndAttribute (new AttributeEnum (log_severity_db, "severity"));
						synchronousQueryContext->addAndAttribute (new AttributeString (msgId, "msgId"));
				}
				if (xmlTag == ras_message) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject update_msg_disable=%d",
							msg_disable);
				if (xmlTag == ras_module) 
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASLOGManagedObject update_mod_disable=%d",
							mod_disable);
				modOp = true;
		}

		/* if we find MO lets use pMod to update */
		if ((pResults->size () != 0) && (xmlTag == ras_syslog_facility) && modOp) {
				pMod = querySynchronously (synchronousQueryContext);
		}
		if ((pResults->size () != 0) && ((xmlTag == ras_hostip) || (xmlTag == ras_username) || (xmlTag == ras_protocol) ||(xmlTag == ras_directory) || (xmlTag == ras_password)) && modOp) {
				pMod = querySynchronously (synchronousQueryContext);

		}
		if ((pResults->size () != 0) && (xmlTag == ras_enable) && modOp) {
				pMod = querySynchronously (synchronousQueryContext);
		}
		if ((pResults->size () != 0) && ((xmlTag == ras_ffdc) ||  (xmlTag == ras_console)) && modOp) {
				pMod = querySynchronously (synchronousQueryContext);
		}
		if ((pResults->size () != 0) && ((xmlTag == ras_severity) || (xmlTag == ras_syslog) || (xmlTag == ras_message) || (xmlTag == ras_module)) && modOp) {
				pMod = querySynchronously (synchronousQueryContext);
		}

		startTransaction ();
		if (delOp) {
				if (xmlTag == ras_syslog_server) {
						pRASManagedObject = dynamic_cast<RASManagedObject *> ((*pRemove)[0]);
						delete pRASManagedObject;
				} else if (xmlTag == ras_enable) {
						pRASAutoUploadManagedObject->setAutoSupport(pRASClusterMessage->getAutoSupport());
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_autoupload_param) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pRemove)[0]);
						is_enabled = pRASAutoUploadManagedObject->getAutoSupport();
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::is_enabled=%d", is_enabled);
						delete pRASAutoUploadManagedObject;
				} else if ((xmlTag == ras_hostip) || (xmlTag == ras_username) || (xmlTag == ras_protocol) || (xmlTag == ras_directory) || (xmlTag == ras_password)) {
						pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pRemove)[0]);
						delete pRASAutoUploadManagedObject;
				} else if (xmlTag == ras_auditlog) {
						pRASAuditManagedObject = dynamic_cast<RASAuditManagedObject *> ((*pRemove)[0]);
						delete pRASAuditManagedObject;
				} else if (xmlTag == ras_severity) {
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pRemove)[0]);
						if ((pRASLOGConfigureManagedObject->getSyslogEnable()) || (pRASLOGConfigureManagedObject->getMessageDisable())) {
							pMod = querySynchronously (synchronousQueryContext);
							pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
							pRASLOGConfigureManagedObject->setSeverity(pRASClusterMessage->getSeverity());
							pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
							updateWaveManagedObject (pRASLOGConfigureManagedObject);
						}
						else  {
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::delete"));
							delete pRASLOGConfigureManagedObject;
						}
				} else if (xmlTag == ras_syslog) {
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pRemove)[0]);
						WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::disable val=") + pRASLOGConfigureManagedObject->getMessageDisable());
						if ((DCM_RASLOG_DEFAULT != pRASLOGConfigureManagedObject->getSeverity()) || (pRASLOGConfigureManagedObject->getMessageDisable())) {
							pMod = querySynchronously (synchronousQueryContext);
							pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
							pRASLOGConfigureManagedObject->setSyslogEnable(pRASClusterMessage->getSyslogEnable());
							pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
							updateWaveManagedObject (pRASLOGConfigureManagedObject);
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::Update"));
						} else {
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::delete"));
							delete pRASLOGConfigureManagedObject;
						}
				} else if (xmlTag == ras_message) {
						pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pRemove)[0]);
						if ((DCM_RASLOG_DEFAULT != pRASLOGConfigureManagedObject->getSeverity()) || (pRASLOGConfigureManagedObject->getSyslogEnable())) {
							pMod = querySynchronously (synchronousQueryContext);
							pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
							pRASLOGConfigureManagedObject->setMessageDisable(pRASClusterMessage->getMessageDisable());
							pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
							updateWaveManagedObject (pRASLOGConfigureManagedObject);
						} else  {
							WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::delete"));
							delete pRASLOGConfigureManagedObject;
						}
				} else if (xmlTag == ras_module) {
						pRASMODConfigureManagedObject = dynamic_cast<RASMODConfigureManagedObject *> ((*pRemove)[0]);
						delete pRASMODConfigureManagedObject;
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::done deleting");
				} else if (xmlTag == ras_syslog_facility) {
						pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pRemove)[0]);
						delete pRASSysFcManagedObject;
				}
		} else if (addOp || modOp) {
				if (xmlTag == ras_syslog_server && addOp) {
						pRASManagedObject = new RASManagedObject (this);
						pRASManagedObject->setSyslogIp(user_ip);
						pRASManagedObject->setSyslogPort(pRASClusterMessage->getSyslogPort());
						pRASManagedObject->setSyslogSecureMode(pRASClusterMessage->getSyslogSecureMode());
						trace (TRACE_LEVEL_INFO, string ("addOp getSyslogSecureMode=") +
							pRASClusterMessage->getSyslogSecureMode()
							+ string ("setSyslogPort=") + pRASClusterMessage->getSyslogPort());
						updateWaveManagedObject (pRASManagedObject);
				} else if (xmlTag == ras_port && modOp) {
						pRASManagedObject->setSyslogIp(user_ip);
						pRASManagedObject->setSyslogPort(pRASClusterMessage->getSyslogPort());
						trace (TRACE_LEVEL_INFO, string("modOp getSyslogPort=") +
							pRASClusterMessage->getSyslogPort());
						updateWaveManagedObject (pRASManagedObject);
				} else if (xmlTag == ras_secure && modOp) {
						pRASManagedObject->setSyslogIp(user_ip);
						pRASManagedObject->setSyslogSecureMode(pRASClusterMessage->getSyslogSecureMode());
						pRASManagedObject->setSyslogPort(pRASClusterMessage->getSyslogPort());
						trace (TRACE_LEVEL_INFO, string("modOp getSyslogSecureMode=") +
							pRASClusterMessage->getSyslogSecureMode() + string(" port ") +
							pRASClusterMessage->getSyslogPort());
						updateWaveManagedObject (pRASManagedObject);
				} else if (xmlTag == ras_enable) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}					
						WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::enable = ") + pRASClusterMessage->getAutoSupport());
						pRASAutoUploadManagedObject->setAutoSupport(pRASClusterMessage->getAutoSupport());
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_hostip) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}					
						pRASAutoUploadManagedObject->setHostIp(pRASClusterMessage->getHostIp());
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_username) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}					
						pRASAutoUploadManagedObject->setUser(pRASClusterMessage->getUser());
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_protocol) {
						 /* if entry is present update else create new */
                        if (pResults->size() != 0) { 
                                pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
                                /* incase of update don't delete the obj */
                        } else {
                                pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
                                /* if new delete obj later */
                                addOp = true;
                        }     
                        pRASAutoUploadManagedObject->setProtocol(pRASClusterMessage->getProtocol());
                        updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_directory) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}					
						pRASAutoUploadManagedObject->setDirectory(pRASClusterMessage->getDirectory());
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				 } else if (xmlTag == ras_password) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASAutoUploadManagedObject = new RASAutoUploadManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}
						pass = pRASClusterMessage->getPassword();
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::Pass=%s", pass.c_str());
						pass = AAACrypto::encryptPassword(pass);
		                if (pass.length() == 0) {
		                    tracePrintf(TRACE_LEVEL_ERROR, "RASObjectManager::getRASManagedObject Pass Encryption failed");
						}
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::PassEn=%s", pass.c_str());
						pRASAutoUploadManagedObject->setPassword(pass);
						updateWaveManagedObject (pRASAutoUploadManagedObject);
				} else if (xmlTag == ras_severity) {
						/* if entry is present update else create new */
						if (modOp) {
								pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::UPDATE"));
						} else if(addOp) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::ADD"));
								pRASLOGConfigureManagedObject = new RASLOGConfigureManagedObject (this);
								/* if new delete obj later */
						}
						pRASLOGConfigureManagedObject->setSeverity(pRASClusterMessage->getSeverity());
						pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
						updateWaveManagedObject (pRASLOGConfigureManagedObject);
				} else if (xmlTag == ras_syslog) {
						/* if entry is present update else create new */
						if (modOp) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::UPDATE"));
								pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else if (addOp) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::ADD"));
								pRASLOGConfigureManagedObject = new RASLOGConfigureManagedObject (this);
								/* if new delete obj later */
						}
						pRASLOGConfigureManagedObject->setSyslogEnable(pRASClusterMessage->getSyslogEnable());
						pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
						updateWaveManagedObject (pRASLOGConfigureManagedObject);
				} else if (xmlTag == ras_message) {
						/* if entry is present update else create new */
						if (modOp) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::UPDATE"));
								pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else if (addOp){
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::ADD"));
								pRASLOGConfigureManagedObject = new RASLOGConfigureManagedObject (this);
								/* if new delete obj later */
						}					
						pRASLOGConfigureManagedObject->setMessageDisable(pRASClusterMessage->getMessageDisable());
						pRASLOGConfigureManagedObject->setMsgId(pRASClusterMessage->getMsgId());
						updateWaveManagedObject (pRASLOGConfigureManagedObject);
				} else if (xmlTag == ras_module) {
						/* if entry is present update else create new */
						if (modOp) {
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::UPDATE"));
								pRASMODConfigureManagedObject = dynamic_cast<RASMODConfigureManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else if (addOp){
								WaveNs::trace (TRACE_LEVEL_DEBUG, string("RASObjectManager::ADD"));
								pRASMODConfigureManagedObject = new RASMODConfigureManagedObject (this);
								/* if new delete obj later */
						}					
						pRASMODConfigureManagedObject->setModuleDisable(pRASClusterMessage->getModuleDisable());
						pRASMODConfigureManagedObject->setModId(pRASClusterMessage->getModId());
						updateWaveManagedObject (pRASMODConfigureManagedObject);
				} else if (xmlTag == ras_auditlog) {
						pRASAuditManagedObject = new RASAuditManagedObject (this);
						pRASAuditManagedObject->setAuditLogclass(audit_class);
						updateWaveManagedObject (pRASAuditManagedObject);
				} else if (xmlTag == ras_syslog_facility) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
								pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pMod)[0]);
								/* incase of update don't delete the obj */
						} else {
								pRASSysFcManagedObject = new RASSysFcManagedObject (this);
								/* if new delete obj later */
								addOp = true;
						}
						pRASSysFcManagedObject->setSysFacility(sys_facility);
						updateWaveManagedObject (pRASSysFcManagedObject);
				} else if (xmlTag == ras_ffdc) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
							pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pMod)[0]);
						} else {
							pRASGlobalConfigManagedObject = new RASGlobalConfigManagedObject (this);
							/* if new delete obj later */
							addOp = true;
						}
						pRASGlobalConfigManagedObject->setFFDC(ffdc);
						updateWaveManagedObject (pRASGlobalConfigManagedObject);
				} else if (xmlTag == ras_console) {
						/* if entry is present update else create new */
						if (pResults->size() != 0) {
							pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pMod)[0]);
						} else {
							pRASGlobalConfigManagedObject = new RASGlobalConfigManagedObject (this);
							/* if new delete obj later */
							addOp = true;
						}
						pRASGlobalConfigManagedObject->setConsole(console);
						updateWaveManagedObject (pRASGlobalConfigManagedObject);
						tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectManager:: console %d",
							pRASGlobalConfigManagedObject->getConsole());
				}
		}
			WaveNs::tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager:: start commitTransaction");
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status) {
            trace (TRACE_LEVEL_INFO, "RASObjectManager::install: Installing DB complete");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_FATAL, "RASObjectManager::install: Failed to install DB");
			status = WAVE_MESSAGE_ERROR;
		}

		if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		if (delOp) {
				if (pRemove != NULL)
					delete pRemove;
		}
		if (addOp) {
				if (xmlTag == ras_syslog_server)
						delete pRASManagedObject;
				if (xmlTag == ras_enable)
						delete pRASAutoUploadManagedObject;
				if ((xmlTag == ras_hostip) || (xmlTag == ras_username) || (xmlTag == ras_directory) || (xmlTag == ras_password) || (xmlTag == ras_protocol))
						delete pRASAutoUploadManagedObject;
				if (xmlTag == ras_auditlog)
						delete pRASAuditManagedObject;
				if (xmlTag == ras_severity)
						delete pRASLOGConfigureManagedObject;
				if (xmlTag == ras_syslog)
						delete pRASLOGConfigureManagedObject;
				if (xmlTag == ras_message)
						delete pRASLOGConfigureManagedObject;
				if (xmlTag == ras_module)
						delete pRASMODConfigureManagedObject;
				if (xmlTag == ras_autoupload_param)
						delete pRASAutoUploadManagedObject;
				if (xmlTag == ras_syslog_facility)
						delete pRASSysFcManagedObject;
				if (xmlTag == ras_ffdc)
						delete pRASGlobalConfigManagedObject;
				if (xmlTag == ras_console)
						delete pRASGlobalConfigManagedObject;
		}

	if(modOp) {
		/* don't delete obj in case of update */
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMod);
	}

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		trace (TRACE_LEVEL_DEBUG, "RASOM:cfgMessageHandler:Successfully Cluster-Wide committed the Interface Configuration");
		status = WAVE_MESSAGE_SUCCESS;
	}
	else
	{
		trace (TRACE_LEVEL_FATAL, "RASOM:cfgMessageHandler: Commiting the Interface configuration failed.  Status : " 
				+ FrameworkToolKit::localize (status));
		--(*pRASMessagingContext);
		pRASMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
		return;
	}

	// get cluser info vector
	vector<LocationId> fullyConnectedLocationIdVector;
	FrameworkToolKit::getFullyConnectedLocations (fullyConnectedLocationIdVector);
	fullyConnectedLocationIdVector.push_back(FrameworkToolKit::getThisLocationId());
//	vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
//	prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
//	UI32 nodesInCluster = pResultsClusterInfo->size ();
	UI32 numberOfFullyConnectedNodes = fullyConnectedLocationIdVector.size ();
	trace (TRACE_LEVEL_INFO, string("RASObjectManager::getClusterInfo: Getting information for : ")+ numberOfFullyConnectedNodes + " switches");

	for (i = 0; i < numberOfFullyConnectedNodes; i++) {
		WaveNs::trace (TRACE_LEVEL_INFO, "RASOM::cfgMessageHandler:Sending message to SW RASLocalObjectManager");
/*		VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
			(dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i])); 
		const UI32 mappedId = DcmToolKit::getMappedIdFromLocationId (mappedId); */
		LocationId locationId = fullyConnectedLocationIdVector[i];
		const UI32 mappedId = DcmToolKit::getMappedIdFromLocationId(locationId);
		ObjectId pOID = DcmToolKit::getVcsNodeObjectIdFromMappedId(mappedId);
		WaveNs::tracePrintf(TRACE_LEVEL_INFO, "RASOM:cfgMessageHandler: Sending message to MID=%d LID=0x%x",
				mappedId, locationId);

		if (xmlTag == ras_syslog_server || xmlTag == ras_port || xmlTag == ras_secure)
			pMessage = new RASLoggingMessage (pOID, xmlTag);
		else if (xmlTag == ras_enable) {
			is_enabled = pRASClusterMessage->getAutoSupport();
			pMessage = new RASLoggingMessage (pOID,is_enabled, supportData, xmlTag);
		}
		else if (xmlTag == ras_autoupload_param)
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_hostip)
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_username)
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_protocol)
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_directory)
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_password)		
			pMessage = new RASLoggingMessage (pOID, data, supportData, xmlTag);
		else if (xmlTag == ras_severity) {
			log_severity = pRASClusterMessage->getSeverity();
			msgId = pRASClusterMessage->getMsgId();
			pMessage = new RASLoggingMessage (pOID, log_severity, msgId, xmlTag);
		}
		else if (xmlTag == ras_syslog) {
			syslog_attr = pRASClusterMessage->getSyslogEnable();
			msgId = pRASClusterMessage->getMsgId();
			pMessage = new RASLoggingMessage (pOID, xmlTag, syslog_attr, msgId);
		} else if (xmlTag == ras_message) {
			msg_disable = pRASClusterMessage->getMessageDisable();
			msgId = pRASClusterMessage->getMsgId();
			pMessage = new RASLoggingMessage (pOID, msg_disable, msgId, xmlTag);
		} else if (xmlTag == ras_module) {
			mod_disable = pRASClusterMessage->getModuleDisable();
			modId = pRASClusterMessage->getModId();
			pMessage = new RASLoggingMessage (pOID, modId, mod_disable, xmlTag);
		}
		else if (xmlTag == ras_auditlog)
			pMessage = new RASLoggingMessage (pOID, audit_class, xmlTag);
		else if (xmlTag == ras_syslog_facility)
			pMessage = new RASLoggingMessage (pOID, sys_facility, isSingleton, xmlTag);
		else if (xmlTag == ras_ffdc)
			pMessage = new RASLoggingMessage (pOID, ffdc, ffdc, xmlTag);
		else if (xmlTag == ras_console)
			pMessage = new RASLoggingMessage (pOID, console, xmlTag);
				pMessage->setMappedId(mappedId);
				if (delOp)
						pMessage->setBoolData(true);
				else if (addOp || modOp)
						pMessage->setBoolData(false);
				if (xmlTag == ras_syslog_server)
						pMessage->setSyslogIp(pRASClusterMessage->getSyslogIp());
				else if (xmlTag == ras_enable)
						pMessage->setAutoSupport(pRASClusterMessage->getAutoSupport());
				else if (xmlTag == ras_hostip)
						pMessage->setHostIp(pRASClusterMessage->getHostIp());
				else if (xmlTag == ras_username)
						pMessage->setUser(pRASClusterMessage->getUser());
				else if (xmlTag == ras_protocol)
						pMessage->setProtocol(pRASClusterMessage->getProtocol());
				else if (xmlTag == ras_directory)
						pMessage->setDirectory(pRASClusterMessage->getDirectory());
				else if (xmlTag == ras_password) {
						pass = pRASClusterMessage->getPassword();
						tracePrintf(TRACE_LEVEL_INFO, "RASObjectManager::getRASManagedObject Pass=%s", pass.c_str());
						pMessage->setPassword(pass);
				}
				else if (xmlTag == ras_severity) {
						pMessage->setSeverity(pRASClusterMessage->getSeverity());
						pMessage->setMsgId(pRASClusterMessage->getMsgId());
				}
				else if (xmlTag == ras_syslog) {
						pMessage->setSyslogEnable(pRASClusterMessage->getSyslogEnable());
						pMessage->setMsgId(pRASClusterMessage->getMsgId());
				}
				else if (xmlTag == ras_message) {
						pMessage->setMessageDisable(pRASClusterMessage->getMessageDisable());
						pMessage->setMsgId(pRASClusterMessage->getMsgId());
				}
				else if (xmlTag == ras_module) {
						pMessage->setModuleDisable(pRASClusterMessage->getModuleDisable());
						pMessage->setModId(pRASClusterMessage->getModId());
				}
				else if (xmlTag == ras_auditlog)
						pMessage->setAuditLogclass(pRASClusterMessage->getAuditLogclass());
				else if (xmlTag == ras_syslog_facility)
						pMessage->setSysLogfacility(pRASClusterMessage->getSysLogfacility());
				else if (xmlTag == ras_ffdc)
						pMessage->setFFDC(pRASClusterMessage->getBoolData());
				else if (xmlTag == ras_console)
						pMessage->setEnumData(pRASClusterMessage->getEnumData());
				status = WAVE_MESSAGE_SUCCESS;

				if (WAVE_MESSAGE_SUCCESS == status) {	  
						pMessage->setWaveClientOriginatingLocationId(pRASClusterMessage->getWaveClientOriginatingLocationId ());
						pMessage->setWaveNativeClientId(pRASClusterMessage->getWaveNativeClientId ());
						pMessage->setWaveUserClientId(pRASClusterMessage->getWaveUserClientId());

						status = send (pMessage,
										reinterpret_cast<PrismMessageResponseHandler> (&RASObjectManager::cfgMessageCallback),
										pRASMessagingContext, 10000, locationId);

						if (WAVE_MESSAGE_SUCCESS != status) {
								pRASMessagingContext->incrementNumberOfFailures ();
								trace (TRACE_LEVEL_INFO, string ("RASObjectManager::cfgMessageHandler: Sending a message to [") 
												+ PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
								trace (TRACE_LEVEL_ERROR, "RASObjectManager::cfgMessageHandler: Error in sending message to get this node  : Status : \"" 
												+ FrameworkToolKit::localize (messageStatus) + "\"");
								delete pMessage;
						} else {
								++(*pRASMessagingContext);
						}
				}
		}

//		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);

		--(*pRASMessagingContext);
		pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void  RASObjectManager::cfgMessageCallback (FrameworkStatus frameworkStatus,
   RASLoggingMessage *pMessage, RASMessagingContext *pRASMessagingContext)
{
    --(*pRASMessagingContext);

    // get Original request msg
    RASClusterMessage *pRASClusterMessage = dynamic_cast<RASClusterMessage *> (pRASMessagingContext->getPMessage ());

    if (FRAMEWORK_SUCCESS == frameworkStatus) {
        prismAssert (pMessage, __FILE__, __LINE__);

	WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASObjectManager::cfgMessageCallback: MID=") + pMessage->getMappedId());

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
	    WaveNs::trace (TRACE_LEVEL_INFO, string ("RASObjectManager::cfgMessageCallback: MID=") +
			   pMessage->getMappedId() + string ("Completion Failure"));

		WaveClientSessionContext waveClientSessionContext = pRASClusterMessage->getWaveClientSessionContext ();
		printfToWaveClientSession (waveClientSessionContext, "%s %d.\n", "Configuration Change failed for Rbridge ID: ", pMessage->getMappedId());
	    // pRASClusterMessage->setData("Failure");
            pRASMessagingContext->incrementNumberOfFailures ();

        } else {
	    // add returned result
	    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASObjectManager::cfgMessageCallback: MID=") +
			   pMessage->getMappedId() + string (" Success"));

	    // pRASClusterMessage->setData(string ("RASObjectManager::cfgMessageCallback: switchid=") +
	    //	   pMessage->getMappedId() + string (" Success\n"));

	    pRASClusterMessage->setData(pMessage->getData());
	}
    } else {

	WaveNs::trace (TRACE_LEVEL_ERROR, string ("RASOM::cfgMessageCallback FRAMEWROK Error") + frameworkStatus);

        pRASMessagingContext->incrementNumberOfFailures ();
    }
    if (NULL != pMessage)
        delete pMessage;

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

/************************************************************************
  * DCM Client Message Handler
  ***********************************************************************/
/*
void RASObjectManager::RASClientMessageHandler (RASClientMessage *pRASClientMessage)
{
    tracePrintf(TRACE_LEVEL_DEBUG, "RASOM:RASClientMessageHandler VER= 0x%x MAGIC=0x%x ",
		 pRASClientMessage->m_hdr_ver, pRASClientMessage->m_hdr_magic);

    PrismLinearSequencerStep sequencerSteps[] = {
       reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::RASVCSMessage),
       reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerSucceededStep),
       reinterpret_cast<PrismLinearSequencerStep> (&RASObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pRASMessagingContext = new PrismLinearSequencerContext (pRASClientMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRASMessagingContext->start ();
    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:RASClientMessageHandler exit");
}


void RASObjectManager::RASVCSMessage(PrismLinearSequencerContext *pRASMessagingContext)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    const ObjectId	pOID;   

    tracePrintf(TRACE_LEVEL_DEBUG, "RASOM:RASVCSMessage");

    RASClientMessage *pRASClientMessage = dynamic_cast<RASClientMessage *> (pRASMessagingContext->getPPrismMessage ());

#define RASEVT_MAGIC	   0x12
#define	RASEVT_VER_STORAGE  5
    if ((pRASClientMessage->m_hdr_ver != RASEVT_VER_STORAGE) || 
	    (pRASClientMessage->m_hdr_magic != RASEVT_MAGIC)) {
	     tracePrintf(TRACE_LEVEL_DEBUG, "RASOM:RASClientMessageH fail VER=0x%x 0x%x MAGIC=0x%x 0x%x",
		 RASEVT_VER_STORAGE, pRASClientMessage->m_hdr_ver,
		RASEVT_MAGIC, pRASClientMessage->m_hdr_magic);
	     // RASClientMessage *test = NULL;
	     //tracePrintf(TRACE_LEVEL_DEBUG, "RASOM:RASClientMessageH fail VER=0x%x ",
	     //	 test->m_hdr_ver);
         pRASMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
	 return;
    }

    // get cluser info vector
    vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
    UI32 nodesInCluster = pResultsClusterInfo->size ();
    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:RASVCSMessage: %d nodes SMID=%d",
		nodesInCluster, pRASClientMessage->getMappedId());

    pRASMessagingContext->setNumberOfFailures (0);
    ++(*pRASMessagingContext);
    UI32 i;
    for (i = 0; i < nodesInCluster; i++) {
	VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
		 (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));

	LocationId locationId = DcmToolKit::getLocationIdFromMappedId (pVcsNodeLocalManagedObject->getMappedId ());
	tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:VCSMSG:LocationId=0x%x", locationId);

	// skip the switch which initiate the logging
	if (pVcsNodeLocalManagedObject->getMappedId () == pRASClientMessage->getMappedId()) {
		continue;
	}
	const UI32	opcode = RAS_VCSLOGGING_LOCAL;
	RASClientMessage *pMessage = new RASClientMessage (opcode);
	prismAssert (pMessage, __FILE__, __LINE__);

	pMessage->setAll(*pRASClientMessage);

	// no timeout
	status = send (pMessage,
            reinterpret_cast<PrismMessageResponseHandler> (&RASObjectManager::RASVCSMessageCallback),
			   pRASMessagingContext, 0, locationId);
	if (WAVE_MESSAGE_SUCCESS != status) {
            pRASMessagingContext->incrementNumberOfFailures ();
	    tracePrintf(TRACE_LEVEL_ERROR, "RASObjectM:VCSMSG:Error in sending message to LocationId=0x%x",
		locationId);
	    delete pMessage;	  
	} else {
	    ++(*pRASMessagingContext);
	}
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);
    --(*pRASMessagingContext);

    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:VCSMSG:Pending=0x%x",
		pRASMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep());

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ?
					   WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void  RASObjectManager::RASVCSMessageCallback (FrameworkStatus frameworkStatus,
   RASClientMessage *pMessage, PrismLinearSequencerContext *pRASMessagingContext)
{
    --(*pRASMessagingContext);

    // get Original request msg
    RASClientMessage *pRASClientMessage = dynamic_cast<RASClientMessage *> (pRASMessagingContext->getPPrismMessage ());

    if (FRAMEWORK_SUCCESS == frameworkStatus) {

        prismAssert (pMessage, __FILE__, __LINE__);

	tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectManager::RASVCSMessageCB: SMID=%d LocalMID=%d",
		pMessage->getMappedId(), DcmToolKit::getLocalMappedId());

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
	    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectManager::RASVCSMessageCallback:"
			" Completion Failure");
	    pRASClientMessage->setData(string ("switchid:") +
			   DcmToolKit::getLocalMappedId() + string (" Fail\n"));
            pRASMessagingContext->incrementNumberOfFailures ();

        } else {
	    // add returned result
	    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:RASVCSMessageCB: LMID=%d Success",
			DcmToolKit::getLocalMappedId());

	    pRASClientMessage->setData(string ("switchid:") + DcmToolKit::getLocalMappedId()
				       + string (" Success\n"));
	    pRASClientMessage->setData(pMessage->getData());
	}

    } else {

	WaveNs::trace (TRACE_LEVEL_INFO, string ("RASOM::copySupportAsyncMSGCB:FRAMEWROK Error") + frameworkStatus);

	pRASClientMessage->setData(string ("switchid:") +
			   DcmToolKit::getLocalMappedId() + string (" Fail\n"));
        pRASMessagingContext->incrementNumberOfFailures ();
    }

    if (NULL != pMessage)
        delete pMessage;

    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:VCSMSG:CB PendingCB=0x%x",
		pRASMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep());

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ?
					   WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
*/
void
RASObjectManager::RASClusterNetConfMessageHandler(RASClusterNetConfMessage *pRASClusterNetConfMessage)
{
    /* Create message to be sent to local object managers */
    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:RASCNetConfHdlr:cmd=0x%x", pRASClusterNetConfMessage->getCmd());
    RASNetConfMessage *pMsg = new RASNetConfMessage(pRASClusterNetConfMessage->getCmd());
    prismAssert(NULL != pMsg, __FILE__, __LINE__);

    /* Create a send to cluster context */
    WaveSendToClusterContext *pCtx = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&RASObjectManager::RASClusterNetConfMessageCallback), pRASClusterNetConfMessage);
    prismAssert(NULL != pCtx, __FILE__, __LINE__);

    /* send to wave cluster */
    tracePrintf(TRACE_LEVEL_DEBUG, "RASObjectM:RASCNetConfHdlr:LocalOM cmd=0x%x", pMsg->getCmd());
    pCtx->setPPrismMessageForPhase1(pMsg);
    sendToWaveCluster(pCtx);
}

void
RASObjectManager::RASClusterNetConfMessageCallback(WaveSendToClusterContext *pCtx)
{
    char switchIdStr[16];

    trace (TRACE_LEVEL_DEVEL, "RASObjectManager::RASClusterNetConfMessageCallback : Entering ...");

    RASClusterNetConfMessage *pMsg = (RASClusterNetConfMessage*) pCtx->getPCallerContext();

    vector<LocationId> locationsVector = pCtx->getLocationsToSendToForPhase1();
    ResourceId status = pCtx->getCompletionStatus();

    if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status) {
        trace(TRACE_LEVEL_WARN, "RASObjectManager::RASClusterNetConfMessageCallback error on all nodes - " + FrameworkToolKit::localize(status));
        delete (pCtx->getPPrismMessageForPhase1());
        delete (pCtx);
        pMsg->setCompletionStatus(status);
        reply(pMsg);
        return;
    }

    // loop through each location and get the responses.
    for (unsigned int i = 0; i < locationsVector.size(); i++) {
        RASNetConfMessage *pNCMsg = (RASNetConfMessage*) pCtx->getResultingMessageForPhase1(locationsVector[i]);

        ResourceId lStatus = pNCMsg->getCompletionStatus();
        if (WAVE_MESSAGE_SUCCESS == lStatus) {
            void *pBuf;
            UI32 bufsize;

            pBuf = pNCMsg->transferBufferToUser(locationsVector[i], bufsize);
            prismAssert(NULL != pBuf, __FILE__, __LINE__);
            pMsg->addBuffer(locationsVector[i], bufsize, pBuf, true);
        } else {
            /* for now print a trace message */
            /* need infrastructure to provide a way to do error handling */
            sprintf(switchIdStr, "%d", DcmToolKit::getMappedIdFromLocationId(locationsVector[i]));
            trace(TRACE_LEVEL_WARN, "RASObjectManager::RASClusterNetConfMessageCallback: Message to switch " + string(switchIdStr) + " failed");
            status = CLUSTER_ERROR_1001;
        }
    }

    delete (pCtx->getPPrismMessageForPhase1());
    delete (pCtx);
    pMsg->setCompletionStatus(status);
    trace (TRACE_LEVEL_DEVEL, "RASObjectManager::RASClusterNetConfMessageCallback : Exiting ...");

    reply(pMsg);
}

// SSObjectManager implementation Start
SSObjectManager::SSObjectManager () : WaveObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());

    // register to receive RAS_COPY_SUPPORT msg
    addOperationMap (RAS_COPY_SUPPORT, reinterpret_cast<PrismMessageHandler> (&SSObjectManager::RASSupportSaveMessageAsyncHandler));
}

SSObjectManager::~SSObjectManager ()
{
}

string SSObjectManager::getClassName ()
{
    return ("SSObjectManager");
}

SSObjectManager *SSObjectManager::getInstance ()
{
    static SSObjectManager *pSSObjectManager = new SSObjectManager ();

    WaveNs::prismAssert (NULL != pSSObjectManager, __FILE__, __LINE__);

    return (pSSObjectManager);
}

PrismServiceId SSObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void SSObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, string ("SSObjectManager::failover failover reason") +
	   FrameworkToolKit::localize(failoverReason));

    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}


vector<WaveManagedObject *> *SSObjectManager::DoSynchronousQuery (const string className)
{
	vector<WaveManagedObject *> *pResults = querySynchronously (className);
	prismAssert (NULL != pResults, __FILE__, __LINE__);
	return pResults;
}


WaveManagedObject *SSObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    return NULL;
}

PrismMessage *SSObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
 
   tracePrintf(TRACE_LEVEL_DEBUG,
                  "SSObjectManager::createMessageInstance opcode=%d", operationCode);

    switch (operationCode)
    {
        case RAS_COPY_SUPPORT :
	    trace (TRACE_LEVEL_DEBUG, "SSObjectManager::createMessageInstance:: RAS_COPY_SUPPORT");
            pPrismMessage = new RASSupportClusterMessage ();
            break;
        default :
            tracePrintf(TRACE_LEVEL_INFO,
                  "SSObjectManager::createMessageInstance Invalid opcode=%d", operationCode);
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

/************************************************************************
  * Async copy support
  ***********************************************************************/
void SSObjectManager::RASSupportSaveMessageAsyncHandler (RASSupportClusterMessage *pRASSupportClusterMessage)
{
     PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&SSObjectManager::copySupportAsync),
        reinterpret_cast<PrismLinearSequencerStep> (&SSObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&SSObjectManager::prismLinearSequencerFailedStep),
    };

	PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
	(pRASSupportClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

	pPrismLinearSequencerContext->start ();
	return;
}

void SSObjectManager::copySupportAsyncCallback (WaveSendToClusterContext *ctx_p) {
	PrismLinearSequencerContext     *seq_ctx_p  = NULL;
	RASSupportClusterMessage		*req_msg_p  = NULL;
	RASSupportMessage		  *resp_msg_p = NULL,
							*sw_resp_p  = NULL;
	UI32 i = 0;
	UI32 displayClusterWideStatus = 0;
	string data;

	seq_ctx_p   = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
	req_msg_p   = dynamic_cast<RASSupportClusterMessage *> (seq_ctx_p->getPPrismMessage ());
	resp_msg_p  = dynamic_cast<RASSupportMessage *> (ctx_p->getPPrismMessageForPhase1());

	vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
	trace(TRACE_LEVEL_INFO, string("Response processing: Num resp:") + locations.size());
	for (i = 0; i < locations.size(); i++) {
		if (i ==0)
			trace(TRACE_LEVEL_INFO, string("locations:") + locations[i]);
		else
			trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
	}

	if (!req_msg_p->getSync()) {
		trace(TRACE_LEVEL_INFO,string("SSObjectManager::copySupportAsyncCallback, set clusterwide status == 1"));
		displayClusterWideStatus = 1;
		WaveClientSessionContext waveClientSessionContext = req_msg_p->getWaveClientSessionContext ();
		printfToWaveClientSession (waveClientSessionContext, "%s\n", "VCS Copy Support Status:");
		printfToWaveClientSession (waveClientSessionContext, "%s\n", "#################################");
	}

	for (i = 0; i < locations.size(); i++) {
		sw_resp_p = dynamic_cast<RASSupportMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
		prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);

		if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
			trace(TRACE_LEVEL_INFO,string("SSObjectManager::copySupportAsyncCallback WAVE_MESSAGE_SUCCESS"));
			if (req_msg_p->getSync()) {
				trace (TRACE_LEVEL_ERROR, "SSObjectManager::copySupportAsync : Appending data");
				data.append(sw_resp_p->getData());
				trace (TRACE_LEVEL_ERROR, string("SSObjectManager::copySupportAsync: data received:") +	sw_resp_p->getData());
				req_msg_p->setData(data);
			} else if (displayClusterWideStatus == 1) {
				WaveClientSessionContext waveClientSessionContext = req_msg_p->getWaveClientSessionContext ();
				printfToWaveClientSession (waveClientSessionContext, "%s %d: %s\n", "Rbridge-ID", 
					DcmToolKit::getMappedIdFromLocationId(locations[i]), "Success.");
			}
		} else if (WAVE_MESSAGE_ERROR == sw_resp_p->getCompletionStatus()) {
			trace(TRACE_LEVEL_INFO,string("SSObjectManager::copySupportAsyncCallback WAVE_MESSAGE_FOS_ERROR"));
			if (displayClusterWideStatus == 1) {
				WaveClientSessionContext waveClientSessionContext = req_msg_p->getWaveClientSessionContext ();
				data = sw_resp_p->getData();
				if (data.length() > 1) {
					printfToWaveClientSession (waveClientSessionContext, "%s %d: %s%s", "Rbridge-ID",
						DcmToolKit::getMappedIdFromLocationId(locations[i]), "Failed, Reason:", data.c_str());
				} else {
					printfToWaveClientSession (waveClientSessionContext, "%s %d: %s\n", "Rbridge-ID",
						DcmToolKit::getMappedIdFromLocationId(locations[i]), "Failed.");
				}
			}
		} else {
			ResourceId clusterCompletionStatus = sw_resp_p->getCompletionStatus();
			trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
				+ locations[i] + string("returned error: ")
				+ FrameworkToolKit::localize(clusterCompletionStatus));
			if (displayClusterWideStatus == 1) {
				WaveClientSessionContext waveClientSessionContext = req_msg_p->getWaveClientSessionContext ();
					printfToWaveClientSession (waveClientSessionContext, "%s %d: %s %s\n", "Rbridge-ID",
						DcmToolKit::getMappedIdFromLocationId(locations[i]), "Failed, Reason: ",
						FrameworkToolKit::localize(clusterCompletionStatus).c_str());
			}
		}
	}

	req_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
	delete resp_msg_p;
	delete ctx_p;
	return;
}

void SSObjectManager::copySupportAsync(PrismLinearSequencerContext *seq_ctx_p)
{
    trace (TRACE_LEVEL_DEBUG, "SSObjectManager::copySupportAsync: Entering");
    const ObjectId	pOID;
    RASSupportMessage *pMessage = NULL;
    //UI16 mappedId = 0;
	UI32 rid = 0;
	WaveSendToClusterContext *send_ctx_p = NULL;
	vector<UI32> locations;
	vector<UI32> rbridgeIds;
    RASSupportClusterMessage *pRASSupportClusterMessage = dynamic_cast<RASSupportClusterMessage *> (seq_ctx_p->getPPrismMessage ());

	rbridgeIds = pRASSupportClusterMessage->getRbridgeIds();
	trace(TRACE_LEVEL_INFO, string("SSObjectManager::copySupportAsync:rbridgeIds size is ") + rbridgeIds.size());
	for (rid = 0; rid < rbridgeIds.size(); rid++) {
            trace(TRACE_LEVEL_INFO, string(
                "SSObjectManager::copySupportAsync: Send msg to "
                "mappedId=")+ rbridgeIds[rid]);
    }
	//mappedId = pRASSupportClusterMessage->getMappedId();
	pMessage = new RASSupportMessage (pOID, true);
//	pMessage->setMappedId(mappedId);

	// setup foscli cmd
		WaveNs::trace (TRACE_LEVEL_DEBUG, "SSObjectManager::CopySupportAsync: CLI in ClusterMessage = " +
				pRASSupportClusterMessage->getCmd());
	pMessage->setCmd(pRASSupportClusterMessage->getCmd());
        pMessage->setNetConf(pRASSupportClusterMessage->getNetConf());
	pMessage->setCLIStart(pRASSupportClusterMessage->getCLIStart());
	pMessage->setCLIEnd(pRASSupportClusterMessage->getCLIEnd());
		pMessage->setCLIOutput(pRASSupportClusterMessage->getCLIOutput());
		pMessage->setSync(pRASSupportClusterMessage->getSync());

	trace (TRACE_LEVEL_DEBUG, string ("SSObjectManager::WaveClientContext= ") +
		pRASSupportClusterMessage->getWaveClientOriginatingLocationId () + " " +
			pRASSupportClusterMessage->getWaveNativeClientId () + " " + pRASSupportClusterMessage->getWaveUserClientId());

	WaveClientSessionContext waveClientSessionContext = pRASSupportClusterMessage->getWaveClientSessionContext ();
	pMessage->setWaveClientOriginatingLocationId(pRASSupportClusterMessage->getWaveClientOriginatingLocationId ());
	pMessage->setWaveNativeClientId(pRASSupportClusterMessage->getWaveNativeClientId ());
	pMessage->setWaveUserClientId(pRASSupportClusterMessage->getWaveUserClientId());

	WaveNs::trace (TRACE_LEVEL_DEBUG, "SSObjectManager::CopySupportAsync: CLI in LocalMessage = " +
		pMessage->getCmd());
	send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast <PrismAsynchronousCallback>(&SSObjectManager::copySupportAsyncCallback), seq_ctx_p);
	if (!send_ctx_p)
		return;
	send_ctx_p->setPPrismMessageForPhase1(pMessage);
	send_ctx_p->setPartialSuccessFlag(true);

	if ((rbridgeIds.size() == 1) && (rbridgeIds[0] == 0)) {
//		 Differentiate between Clusterwide SS and Local Node SS in LocalOM 
		pMessage->setMappedId(DcmToolKit::getLocalMappedId());
		trace(TRACE_LEVEL_INFO, "Send to entire Cluster");
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
		if(!locations.empty()) {
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		} else {
			trace(TRACE_LEVEL_INFO, "Location ID empty");
		}
		sendToWaveCluster(send_ctx_p);
	} else {
		/* Set a non Zero value so that Local OM recognizes its a Non Local Op */
		pMessage->setMappedId(1);
		trace(TRACE_LEVEL_INFO, "Send to particular node");
		for (rid = 0; rid < rbridgeIds.size(); rid++) {
			locations.push_back(DcmToolKit::getLocationIdFromMappedId(rbridgeIds[rid]));
			trace(TRACE_LEVEL_INFO, string(
				"SSObjectManager::copySupportAsync: Send msg to "
				"mappedId=")+ rbridgeIds[rid] +string(" locationId=") + locations[rid]);
		}
		if(!locations.empty()) {
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		} else {
			trace(TRACE_LEVEL_INFO, "Location ID empty");
		}
		sendToWaveCluster(send_ctx_p);
	}

	return;
}

void  SSObjectManager::copySupportAsyncMessageCallback (FrameworkStatus frameworkStatus,
   RASSupportMessage *pMessage, RASMessagingContext *pRASMessagingContext)
{
    --(*pRASMessagingContext);

    // get Original request msg
    RASSupportClusterMessage *pRASSupportClusterMessage = dynamic_cast<RASSupportClusterMessage *> (pRASMessagingContext->getPMessage ());

    if (FRAMEWORK_SUCCESS == frameworkStatus) {
        prismAssert (pMessage, __FILE__, __LINE__);

	WaveNs::trace (TRACE_LEVEL_DEBUG, string ("SSObjectManager::copySupportAsyncMSGCB: MID=") + pMessage->getMappedId());

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
	    WaveNs::trace (TRACE_LEVEL_INFO, string ("SSObjectManager::copySupportAsyncMSGCB: MID=") +
			   pMessage->getMappedId() + string ("Completion Failure"));
	    pRASSupportClusterMessage->setData("Failure");
            pRASMessagingContext->incrementNumberOfFailures ();

        } else {
	    // add returned result
	    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("SSObjectManager::copySupportAsyncMSGCB: MID=") +
			   pMessage->getMappedId() + string (" Success"));

	    pRASSupportClusterMessage->setData(string ("SSObjectManager::copySupportAsyncMSGCB: switchid=") +
			   pMessage->getMappedId() + string (" Success\n"));

	    pRASSupportClusterMessage->setData(pMessage->getData());
	}
    } else {

	WaveNs::trace (TRACE_LEVEL_INFO, string ("SSObjectManager::copySupportAsyncMSGCB:FRAMEWROK Error") + frameworkStatus);

	pRASSupportClusterMessage->setData("Failure");
        pRASMessagingContext->incrementNumberOfFailures ();
    }
    if (NULL != pMessage)
        delete pMessage;

    pRASMessagingContext->executeNextStep (((pRASMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

// SSObjectManager implementation End
}
