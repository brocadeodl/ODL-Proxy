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
 *   Author : chenj                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "RAS/Local/RASLocalObjectManager.h"
#include "RAS/Local/RASTypes.h"
//#include "RAS.h"
#include "vcs.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASSupportMessage.h"
#include "RAS/Global/RASManagedObject.h"
//#include "ClientInterface/RAS/RASClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmResourceIds.h"
#include "Framework/ObjectModel/WaveSendToClientsContext.h"
#include "AAA/Global/AAACrypto.h"

#include "RAS/Local/RASLocalSSThread.h"

#include <iostream>
#include <string>
// #include <fstream>
#include <errno.h> 
#include <unistd.h>
#include <raslog/raslogd.h>
#include <raslog/raslogm.h>
#include <raslog/raslog.h>
#include <raslog/raslog_ras.h>
#include <em/em_if.h>
#include <utils/slot.h>
#include <hasm/ls.h>
#include <hasm/smi.h>
#include <hasm/hasm.h>
#include <ha/ham_api.h>
#include <sysmod/syscomm.h>
#include <hil/hil.h>
#include <raslog/raslog.h>
#include <raslog/raslog_ss.h>
#include "dcm/fos_headers.h"

#define CP_IF_ADDR 0x20

using namespace std;

namespace DcmNs
{   
    RASLocalObjectManager::RASLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
    DcmShell::addToWaveShell ();

    // to support failover, cluster node removing
    associateWithVirtualWaveObjectManager (RASObjectManager::getInstance ());

    // push LoggingLocalManagedObject to DCM DB
    LoggingLocalManagedObject LoggingLocal (this);
    LoggingLocal.setupOrm ();
    addManagedClass (LoggingLocalManagedObject::getClassName ());

    addOperationMap (RASLOGGING, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::RASLoggingMessageHandler));
    addOperationMap (RASGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::RASGlobalConfigMessageHandler));
//    addOperationMap (RAS_VCSLOGGING_LOCAL, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::VCSMessageHandler));
    addOperationMap (FOSCLI, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::fosCLIMessageHandler));
	addOperationMap (RASMANCLI, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::rasmanCLIMessageHandler));
    addOperationMap (RAS_NETCONF, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::netConfMessageHandler));
    addOperationMap (IPVXCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::ipVxMessageHandler));
    addOperationMap (AUDITCLASSCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::auditLogMessageHandler));
    addOperationMap (SUPPORTSAVESTATUS, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::SSStatusMessageHandler));
    addOperationMap (RASLCCLI, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::RASLCCLIMessageHandler));
    addOperationMap (SYSFACILITYCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::sysLogMessageHandler));
	addOperationMap (AUTOUPLOADCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::autoUploadMessageHandler));
	addOperationMap (AUTOUPLOADCFGPARAM, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::autoUploadParamMessageHandler));
	addOperationMap (RASLOGSEVCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::raslogSeverityCfgMessageHandler));
	addOperationMap (RASLOGSYSLOGCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::raslogSyslogCfgMessageHandler));
	addOperationMap (RASLOGMSGCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::raslogMsgCfgMessageHandler));
	addOperationMap (RASLOGMODCFG, reinterpret_cast<PrismMessageHandler> (&RASLocalObjectManager::raslogModCfgMessageHandler));
    WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient ("RAS", &RASLocalObjectManager::createManagementInterfaceMessageInstance);
	PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
}

RASLocalObjectManager::~RASLocalObjectManager ()
{
}


ManagementInterfaceMessage *RASLocalObjectManager::createManagementInterfaceMessageInstance (const UI32 &operationCode)
{
    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

    switch (operationCode) {
        case LCCLI_MSG:
            pManagementInterfaceMessage = new LCCLIMsg();
	    WaveNs::trace (TRACE_LEVEL_INFO, string ("RASMsgInstantiator : LCCLI"));
            break;
        default:
            WaveNs::trace (TRACE_LEVEL_INFO, string ("RASMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pManagementInterfaceMessage);
}

RASLocalObjectManager *RASLocalObjectManager:: getInstance ()
{
    static RASLocalObjectManager *pRASLocalObjectManager = new  RASLocalObjectManager ();

    WaveNs::prismAssert (NULL !=  pRASLocalObjectManager, __FILE__, __LINE__);

    return (pRASLocalObjectManager);
}

PrismServiceId  RASLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

PrismMessage  *RASLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::createMessageInstance opcode=") + operationCode);
        switch (operationCode)
        {
//	case RAS_VCSLOGGING_LOCAL :
  //              pPrismMessage = new RASClientMessage(operationCode);
    //            break;

            case RASLOGGING :
                pPrismMessage = new RASLoggingMessage;
                break;
            case RASGLOBALCONFIG :
                pPrismMessage = new RASLoggingMessage;
                break;
	        case FOSCLI :
                pPrismMessage = new RASLoggingMessage;
                break;
	case IPVXCFG:
		        pPrismMessage = new RASLoggingMessage;
                break;
	        case AUDITCLASSCFG:
		        pPrismMessage = new RASLoggingMessage;
                break;
	        case SUPPORTSAVESTATUS:
		        pPrismMessage = new RASLoggingMessage;
                break;
	        case SYSFACILITYCFG:
		        pPrismMessage = new RASLoggingMessage;
                break;
			case AUTOUPLOADCFG:
				pPrismMessage = new RASLoggingMessage;
				break;
			case AUTOUPLOADCFGPARAM:
				pPrismMessage = new RASLoggingMessage;
				break;
			case RASLOGSEVCFG:
				pPrismMessage = new RASLoggingMessage;
				break;
			case RASLOGSYSLOGCFG:
				pPrismMessage = new RASLoggingMessage;
				break;
			case RASLOGMSGCFG:
				pPrismMessage = new RASLoggingMessage;
				break;
			case RASLOGMODCFG:
				pPrismMessage = new RASLoggingMessage;
				break;

        case RAS_NETCONF:
            pPrismMessage = new RASNetConfMessage;
            break;

	case RASLCCLI:
		pPrismMessage = new RASLCCLIMsg;
                break;
	case RASMANCLI:
		pPrismMessage = new RASRasmanMessage;
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

WaveManagedObject  *RASLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    // support single Object Manager with multiple Manage Object
    if ((LoggingLocalManagedObject::getClassName ()) == managedClassName) {
        pWaveManagedObject = new LoggingLocalManagedObject (this);
    } else {
        trace (TRACE_LEVEL_FATAL, "RASLocalOM::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

void RASLocalObjectManager::externalStateSynchronization(WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{
	UI32 fssStage = pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber();
	ResourceId serviceType = pWaveAsynchronousContextForExternalStateSynchronization->getServiceType();
	int returnValue = 0;
	string cmd_data;
	char cmd_op[512];
	LoggingLocalManagedObject *pLogging = NULL;
	int localId = DcmToolKit::getLocalMappedId();
	string id = string ("") + localId;

	trace(TRACE_LEVEL_INFO, string("RASLocalOM::externalStateSynchronization entering..., fssStage=") + fssStage + string(" Service Type = ") + serviceType + string("FCSW=") + FCSW + string("FSS_RECOV_ACTIVE=") + FSS_RECOV_ACTIVE);

	if ((fssStage == FSS_RECOV_ACTIVE) && (serviceType == FCSW)) {
		trace (TRACE_LEVEL_INFO, "RASLocalOM::externalStateSynchronization : checking for startup config type");

		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
				new WaveManagedObjectSynchronousQueryContext(LoggingLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
		delete syncQueryContext;

		if ((NULL == pResults) || (pResults->size() == 0)) {
			tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::externalStateSynchronization no Logging MO 0\n");
		} else {
			pLogging = dynamic_cast<LoggingLocalManagedObject *> ((*pResults)[0]);
			if (pLogging == NULL) {
				tracePrintf(TRACE_LEVEL_ERROR, true, true, "RASLocalOM::loggingBootReplay no Logging MO 1\n");
			} else {
				trace (TRACE_LEVEL_INFO, string ("RASLOM::externalStateSynchronization:SID=") + pLogging->getSwitchId());
				cmd_data = pLogging->getHostName();
				trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::externalStateSynchronization: host_name=") + cmd_data);
				snprintf(cmd_op, 512, "/fabos/bin/switchname %s >/dev/null 2>&1", (char *)cmd_data.c_str());
				returnValue = system(cmd_op);
				trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::externalStateSynchronization:sysret =") + returnValue);
				delete pLogging;
			}
		}
	}

	pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForExternalStateSynchronization->callback();
}

void RASLocalObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	int returnValue = 0;
	string cmd_data;
	char cmd_op[512];
	LoggingLocalManagedObject *pLogging = NULL;
	int localId = DcmToolKit::getLocalMappedId();
	string id = string ("") + localId;

	trace (TRACE_LEVEL_INFO, "RASLocalObjectManager::haboot : Entering ...");
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

	trace(TRACE_LEVEL_INFO, string("RAS Local 'haboot' Service called with reason - ") + FrameworkToolKit::localize(bootReason));

	if ((bootReason == WAVE_BOOT_HASTANDBY_BOOT) ||
		(bootReason == WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT))
	{
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
				new WaveManagedObjectSynchronousQueryContext(LoggingLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
		delete syncQueryContext;

		if ((NULL == pResults) || (pResults->size() == 0)) {
			tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::haboot no Logging MO 0\n");
		} else {
			pLogging = dynamic_cast<LoggingLocalManagedObject *> ((*pResults)[0]);
			if (pLogging == NULL) {
				tracePrintf(TRACE_LEVEL_ERROR, true, true, "RASLocalOM::haboot no Logging MO 1\n");
			} else {
				trace (TRACE_LEVEL_INFO, string ("RASLOM::haboot:SID=") + pLogging->getSwitchId());
				cmd_data = pLogging->getHostName();
				trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::haboot: host_name=") + cmd_data);
				snprintf(cmd_op, 512, "/fabos/bin/switchname %s >/dev/null 2>&1", (char *)cmd_data.c_str());
				returnValue = system(cmd_op);
				trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::haboot:sysret =") + returnValue);
				delete pLogging;
			}
		}
	}
	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void RASLocalObjectManager::boot(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	int returnValue = 0;
	string cmd_data;
	char cmd_op[512];
	LoggingLocalManagedObject *pLogging = NULL;
	int localId = DcmToolKit::getLocalMappedId();
	string id = string ("") + localId;

	trace (TRACE_LEVEL_INFO, "RASLocalObjectManager boot Entering");
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

	trace(TRACE_LEVEL_INFO, string("RAS Local 'boot' Service called with reason - ") + FrameworkToolKit::localize(bootReason));
	if ((bootReason == WAVE_BOOT_HASTANDBY_BOOT) ||
		(bootReason == WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT))
	{
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
				new WaveManagedObjectSynchronousQueryContext(LoggingLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
		delete syncQueryContext;

		if ((NULL == pResults) || (pResults->size() == 0)) {
			tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::boot no Logging MO 0\n");
		} else {
			pLogging = dynamic_cast<LoggingLocalManagedObject *> ((*pResults)[0]);
			if (pLogging == NULL) {
				tracePrintf(TRACE_LEVEL_ERROR, true, true, "RASLocalOM::boot no Logging MO 1\n");
			} else {
				trace (TRACE_LEVEL_INFO, string ("RASLOM::boot:SID=") + pLogging->getSwitchId());
				cmd_data = pLogging->getHostName();
				trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::boot: host_name=") + cmd_data);
				snprintf(cmd_op, 512, "/fabos/bin/switchname %s >/dev/null 2>&1", (char *)cmd_data.c_str());
				returnValue = system(cmd_op);
				trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::boot:sysret =") + returnValue);
				delete pLogging;
			}
		}
	}

	pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	pWaveAsynchronousContextForBootPhases->callback ();
}

void RASLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    /* add all MO here */
    LoggingLocalManagedObject  *pLogging=NULL;
    int localId = DcmToolKit::getLocalMappedId();
    int swbd = get_swbd();
    int needInit = 0;
	char    chassisname[32];
	string chassisnamestr;
	sysModHandle_t  *pSysModHandle = NULL;
	int sysmod_stat;
	UI32 stat = 1;

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("RASLocalOM::install:boot reason=")
		 + bootReason + string (" MID=") + localId + string (" swbd=") + swbd);

    if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || 
	(WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)) {
	trace(TRACE_LEVEL_INFO, string ("RASLocalOM::install first boot SID=") + localId);
	needInit = 1;    

    // get called once after "none VCS -> VCS"
    // search local MO from DB which proprogated from princiapl. If MO is not find, it means this node
    // is a newly jointed node, creat MO base on its localID. 	
    } else if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) {
	string id = string ("") + localId;
        trace(TRACE_LEVEL_INFO, string ("RASLocalOM::install secondary boot SID=") + id);
	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(LoggingLocalManagedObject::getClassName());
	syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
	delete syncQueryContext;

	if ((NULL == pResults) || (pResults->size() == 0)) {
	   trace(TRACE_LEVEL_INFO, string ("RASLocalOM::install secondary boot no MO SID=") + id);
	   // Need creat LoggingLocalManagedObject for this node if principal node DB does not have it yet.
	   needInit = 1;

	} else {
	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
    } else {
        trace(TRACE_LEVEL_INFO, string ("RASLocalOM::install: boot reason=%d, do nothing") +
		    bootReason);
	status = WAVE_MESSAGE_SUCCESS;
    }

    if (needInit) {

	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::initialize"));
        /****************************************************************************
         * Create managed object and commit to database
         ***************************************************************************/   
        startTransaction ();

	pLogging = new LoggingLocalManagedObject (this);

//Chassis name from the backend
	WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::backend"));
	sysmod_stat = sysModInit (&pSysModHandle);
	if ((sysmod_stat != 0) || (pSysModHandle == NULL))
		trace (TRACE_LEVEL_ERROR, string ("RASLocalOM::sysModInit failed"));

	stat = hilGetSwName(pSysModHandle, CP_IF_ADDR, 1, &chassisname);
	if (stat != 0) {
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM:: hilGetSwName failed"));
		pLogging->setChassisName("chassis");
	} else {
		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::chassisname=") + chassisname);
		chassisnamestr = chassisname;
		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::chassisnamestr=") + chassisnamestr);

		pLogging->setChassisName(chassisnamestr);
	}

	pLogging->setHostName("sw0");
	// m_isACopy (false),

	// setup key name to mappedId to associate this with MO, setName() is in DcmLocalManagedObject()
	// pLogging->setName(string ("") + localId);
	pLogging->setSwitchId(string ("") + localId);
	// loggings.push_back(pLogging);
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::install: BRID=") + localId + string (" swbd=") + swbd);

	status = commitTransaction ();

	if (FRAMEWORK_SUCCESS == status) {	
	    trace (TRACE_LEVEL_INFO, "RASLocalOM::install: Installing RAS DB complete");

	    status = WAVE_MESSAGE_SUCCESS;
	} else {
            trace (TRACE_LEVEL_FATAL, "RASLocalOM::install: Failed to install RAS DB");
	    prismAssert (false, __FILE__, __LINE__);
	}	
	delete pLogging;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void RASLocalObjectManager::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
{
	int status = WAVE_MESSAGE_SUCCESS;
	// WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
	SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::postboot pass = ") + PassNum);

	// sync up syslog ip with backend
//	ipVxSecondBoot();
	generateSyslogNGConfig();
	
	/* sync autosupport with backend */
	autoUploadBoot();
	
	/* sync raslog message configurations with backend */
	raslogCfgBoot();
	/* sync ras module configurations with backend */
	rasmodCfgBoot();
	/* sync the auditcfg with backend */
	auditClass();

	/* sync syslog facility config */
//	syslogFacility();

	/* sync chassis-name, host-name, ffdc and console filter */
	loggingBootReplay();

	status = WAVE_MESSAGE_SUCCESS;

	pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
	pWaveAsynchronousContextForPostBootPhase->callback ();

	trace (TRACE_LEVEL_INFO, "RASLocalOM::postboot: Exiting Boot");
}

void RASLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "RASLocalOM::listenForEvents");

    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&RASLocalObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void RASLocalObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
}

// MSG handler to set RAS/System/Support config data
void  RASLocalObjectManager::RASLoggingMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
    // Validations and Queries should go here
    ObjectId  OID = pRASLoggingMessage->getObjectId ();
    trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler");
    char cmdline[512];
    string Data;

    LoggingLocalManagedObject *pLogging          = NULL;
    WaveManagedObject         *pWaveManagedObject  = NULL;
    ResourceId                status              = WAVE_MESSAGE_SUCCESS;
    FILE *cmd_op = NULL;
    char cmd_output[513];
    int rc = 0;

    WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();
    LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

    // Find MO from OID/KeyName
    pWaveManagedObject = queryManagedObject (OID);
    if (NULL == pWaveManagedObject)
    {
	trace (TRACE_LEVEL_ERROR, "RASLocalOM::RASLoggingMessageHandler : No MO Found:");
        pRASLoggingMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pRASLoggingMessage);
        return;
    }

    pLogging = dynamic_cast<LoggingLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pLogging, __FILE__, __LINE__);

    // For cfg set request, how about for remove ??
    switch (pRASLoggingMessage->getWyserUserTag()) {
	case ras_raslog:
	trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler : Set raslog = " +
	       pRASLoggingMessage->getData());
        pLogging->setRaslog(pRASLoggingMessage->getData());
	break;
	case ras_autoupload:
	trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler : Set support auto = " +
	       pRASLoggingMessage->getBoolData());
        pLogging->setAutoSupport(pRASLoggingMessage->getBoolData());
	break;
        case ras_chassis_name:
	  Data = pRASLoggingMessage->getData();
	  trace (TRACE_LEVEL_INFO, string ("RASLocalOM::RASLoggingMessageHandler:set system chassis_name=") + Data);
	  pLogging->setChassisName(Data);
	  snprintf(cmdline, 512, "/fabos/bin/chassisname %s 2>/dev/null", (char *)Data.data());


	  status = WAVE_MESSAGE_SUCCESS;
	  break;

        case ras_host_name:
	  Data = pRASLoggingMessage->getData();
	  trace (TRACE_LEVEL_INFO, string ("RASLocalOM::RASLoggingMessageHandler:set system host_name=") + Data);
	  pLogging->setHostName(Data);
	  snprintf(cmdline, 512, "/fabos/bin/switchname %s 2>/dev/null", (char *)Data.data());
	  // JPC need handle error return

	  status = WAVE_MESSAGE_SUCCESS;
	  break;
        default:
        trace (TRACE_LEVEL_ERROR, string ("RASWyserEaConfigurationWorker::RASLoggingMessageHandler : Unsupported tag=") +
	       pRASLoggingMessage->getWyserUserTag());
        break;
    }

   if ((cmd_op = popen(cmdline, "r")) != NULL) {
        while (fgets(cmd_output, 512, cmd_op)) {
            if (pRASLoggingMessage->getCLIOutput())
                        trace (TRACE_LEVEL_INFO, string ("switch ") + pRASLoggingMessage->getMappedId() + string(": ") + cmd_output);
                if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			} else {
				printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s", pRASLoggingMessage->getMappedId(), cmd_output);
                }
        }
        if ((rc = pclose(cmd_op)) != 0) {
            trace (TRACE_LEVEL_INFO, string ("RASLocalOM::RASLoggingMessageHandler pclose fail:") +
                   cmdline + string (" rc=") + rc + string (" errno=") + -errno);
        		pRASLoggingMessage->setCompletionStatus (status);
        		reply (pRASLoggingMessage);
			if (pLogging)
			    delete pLogging;
			return;
        }
   } else {
       trace (TRACE_LEVEL_INFO, string ("RASLocalOM::RASLoggingMessageHandler popen fail:") +
                   cmdline + string (" rc=") + rc + string (" errno=") + -errno);
        if (locationRole == LOCATION_STAND_ALONE) {
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
        	pRASLoggingMessage->setCompletionStatus (status);
        	reply (pRASLoggingMessage);
		if (pLogging)
		    delete pLogging;
		return;
        } else {
			printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s", pRASLoggingMessage->getMappedId(), "Command Failed\n");
        	pRASLoggingMessage->setCompletionStatus (status);
        	reply (pRASLoggingMessage);
		if (pLogging)
		    delete pLogging;
		return;
        }
   }

    startTransaction ();
    trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler : Start updating Configuration");
    updateWaveManagedObject (pLogging);
    status = commitTransaction ();

  if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler : Successfully committed the Logging Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "RASLocalOM::RASLoggingMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete  pLogging;

    pRASLoggingMessage->setCompletionStatus (status);
    reply (pRASLoggingMessage);
}

#if 0
// VCS MSG handler
/*
void  RASLocalObjectManager::VCSMessageHandler(RASClientMessage *pRASClientMessage)
{
    // Validations and Queries should go here
    tracePrintf(TRACE_LEVEL_DEBUG, "RASLocalOM::VCSMessageHandler RLID=%d SLID=%d",
	    DcmToolKit::getLocalMappedId(), pRASClientMessage->getMappedId());

    // UI32 mappedId =  DcmToolKit::getLocalMappedId();
    // if (pRASClientMessage->getMappedId() ==  mappedId)
    //	return;

    // setup the sequence 
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
	// If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in RASLocalMessagingContext
	// reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::queryLocalManagedObjectFromMappedId),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::VCSRASLog),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    // put MSG to RASLocalMessagingContext
    RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASClientMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pRASLocalMessagingContext->execute ();
}

ResourceId RASLocalObjectManager::VCSRASLog(RASLocalMessagingContext *pRASLocalMessagingContext)
{
    vcslog_t *pvcslog = new vcslog_t;
    rasevt_t *prasevt = new rasevt_t;
    rls_msg_t *msg = &pvcslog->vcsmsg;

    RASClientMessage *pRASClientMessage = reinterpret_cast<RASClientMessage *> (pRASLocalMessagingContext->getPPrismMessage ());

    tracePrintf(TRACE_LEVEL_DEBUG, "RASLocalOM::RASClientMessage:"
		"swname=%s, VER=0x%x MAGIC=0x%x",
	pRASClientMessage->m_swname.c_str(), pRASClientMessage->m_hdr_ver,
 	pRASClientMessage->m_hdr_magic);

   pRASClientMessage->setCStructure(pvcslog);

   RLS_SET_FLAGS(msg,(RLS_GET_FLAGS(msg) | RASEVT_FRCLOG | RASEVT_IGNORE_FFDC));
   RLS_SET_IQDROP(msg, 0);

   tracePrintf(TRACE_LEVEL_DEBUG, "RASLocalOM::VCSRASLog:"
	"swname=%s, VER=0x%x MAGIC=0x%x OID=0x%x FLG=0x%x",
       RLS_GET_SWNAME(msg), RLS_GET_VER(msg), RLS_GET_MAGIC(msg), RLS_GET_OBJID(msg),
	       RLS_GET_FLAGS(msg));

   prasevt->hdr = msg->hdr;
   prasevt->evt = msg->evt;

   if (raslog_msg(prasevt)) {
	   tracePrintf(TRACE_LEVEL_INFO, "RASLocalOM::fail to call raslog_msg");
   }

   delete pvcslog;
   delete prasevt;

   return (WAVE_MESSAGE_SUCCESS);

}
*/
#endif
// MSG handler for Actionpoint FOSCLI
void RASLocalObjectManager::fosCLIMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
    trace (TRACE_LEVEL_DEBUG, "RASLocalOM::fosCLIMessageHandler");
    
    // setup the sequence 
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
	// If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in RASLocalMessagingContext
	// reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::queryLocalManagedObjectFromMappedId),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::fosCLI),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    // put MSG to RASLocalMessagingContext
    RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::rasmanCLIMessageHandler(RASRasmanMessage *pRASRasmanMessage)
{
    trace (TRACE_LEVEL_DEBUG, "RASLocalOM::rasmanCLIMessageHandler");

    // setup the sequence 
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::rasmanCLI),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    // put MSG to RASLocalMessagingContext
    RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASRasmanMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pRASLocalMessagingContext->execute ();
}

// Get max port information
static int
RASLOM_get_max_port()
{
    int rc;
    sysModHandle_t *sysmodh = NULL;
    fabSysParam_t system_param;

    /* extract platform-level system parameters */
    rc = sysModInit(&sysmodh);
    if ((rc != 0) || (sysmodh == NULL)) {
        trace(TRACE_LEVEL_WARN, "RASLOM_get_max_port: Fabsys system initialization failed");
        return (-1);
    }

    rc = sysCtrlGetSystemParameters(sysmodh, &system_param);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "RASLOM_get_max_port: Fabsys system parameter retrieval failed");
        sysModClose(&sysmodh);
        return (-1);
    }

    sysModClose(&sysmodh);
    return (system_param.max_ge_ports_per_system);
}

// Get Chassis Info
static int
getChassisInfo(void **buffer, UI32 *bufsize)
{
    int rc, objectCount = 0;
    chassisShowOut_t    *pOut = NULL;
    emChassisInfo_t     *pChassisInfo = NULL;
    emIndex_t           emCount;

    rc = em_GetChassisObjectCount(&objectCount);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "RASLOM:getChassisInfo: Unable to retrieve chassis object count\n");
        return (rc);
    }
    tracePrintf(TRACE_LEVEL_DEBUG, true, true, "RASLocalOM::getChassisInfo: objectCount = %d\n", objectCount);

    /* Allocated memory for getting chassisinfo */
    *bufsize = sizeof (chassisShowOut_t) + (objectCount * sizeof(emChassisInfo_t));
    pOut = (chassisShowOut_t *) malloc(*bufsize);
    if (pOut == NULL) {
        trace(TRACE_LEVEL_WARN, "RASLOM:getChassisInfo: Unable to allocated memory for getting Chassis Info\n");
        return (-1);
    }

    memset(pOut, '\0', *bufsize);

    pChassisInfo = (emChassisInfo_t*)((char*)pOut + sizeof (chassisShowOut_t));
    /* Initialize emCount */
    emCount.start = 0;
    emCount.end = objectCount;

    rc = em_GetChassisInfo(pChassisInfo, &emCount);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "RASLOM:getChassisInfo: Unable to retrieve chassis information\n");
        free (pOut);
        return (rc);
    }

    if (getThisCPSlot() == 0) {
        pOut->ha_state = 0;
    } else {
        if ((pOut->ha_state = ham_get_state()) < 0) {
            pOut->ha_state = 0;
        }

        /* Get slot port information */
        for (int i = 0; i < SMI_SLOTS; i++) {
            pbmp_t  pbmp;

            /* ETH Ports */
            if (has_get_slport(i, 0, SMI_MAX_PORTS, -1, PROT_ETH, &pbmp) < 0) {
                continue;
            } else {
                /* Count number of ports in the bitmap */
                int count = 0;
                for (int j = 0; j < SMI_MAX_PORTS; j++) {
                    if (PBMP_ISSET(&pbmp, j)) {
                        count++;
                    }
                }
                pOut->slotETHPortCnt[i] = count;
            }
        }
    }

    /* Get slot type and Id of all slots */
    for (int i = 0; i < objectCount; i++) {
        if (pChassisInfo[i].sysObject == SYS_HW_SLOT) {
            if (pChassisInfo[i].unitNum < SMI_SLOTS) {
                slot_no2id(pChassisInfo[i].unitNum,
                    &(pOut->slotType[pChassisInfo[i].unitNum]),
                    &(pOut->slotId[pChassisInfo[i].unitNum]));
            }
        }
    }


	pOut->maxport   = MAX_PHY_PORT;     /* max physical port */
    pOut->bd        = ham_get_swbd();   /* bd number */
    pOut->frucnt    = objectCount;      /* fru count */
    pOut->maxport   = RASLOM_get_max_port();
    if ((int)pOut->maxport == -1) {
        trace(TRACE_LEVEL_WARN, "RASLOM:getChassisInfo: Unable to retrieve max port count\n");
        free (pOut);
        return (-1);
    } else {
        trace(TRACE_LEVEL_INFO, string("RASLOM:getChassisInfo: frucnt = ") + objectCount);
    }

    *buffer = pOut;

    return (rc);
}

// Get Sensor Info
static int
getSensorInfo(void **buffer, UI32 *bufsize)
{
    int rc = 0;
    int totalSensorCnt = 0;
    int tempSensorCnt = 0, tempSensorIdx = 0;
    int fanSensorCnt = 0, fanSensorIdx = 0;
    emSensor_t  *pSensor = NULL;

    rc = em_GetChassisTempSensorCount(&tempSensorCnt);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "Unable to retrieve temperature sensor count\n");
        return (rc);
    }
    totalSensorCnt += tempSensorCnt;
    fanSensorIdx = tempSensorCnt;
    tracePrintf(TRACE_LEVEL_DEBUG, true, true, "RASLocalOM::getSensorInfo: tempSensorCnt = %d\n", tempSensorCnt);


    rc = em_GetChassisFanCount(&fanSensorCnt);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "Unable to retrieve fan count\n");
        return (rc);
    }
    totalSensorCnt += fanSensorCnt;

    tracePrintf(TRACE_LEVEL_DEBUG, true, true, "RASLocalOM::getSensorInfo: fanSensorCnt = %d\n", fanSensorCnt);
    tracePrintf(TRACE_LEVEL_DEBUG, true, true, "RASLocalOM::getSensorInfo: tempSensorIdx = %d, fanSensorIdx = %d\n", tempSensorIdx, fanSensorIdx);

    // allocate memory for getting sensor information
    pSensor = (emSensor_t*) calloc(totalSensorCnt, sizeof(emSensor_t));
    if (pSensor == NULL) {
        trace(TRACE_LEVEL_WARN, "Unable to allocated memory for getting Sensor Info\n");
        return (-1);
    }

    // retrieve temp sensor information
    rc = em_GetChassisTempSensor(&pSensor[tempSensorIdx], tempSensorCnt);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "Unable to retrieve temp Sensor info\n");
        free (pSensor);
        return (rc);
    }

    // retrieve fan sensor information
    rc = em_GetChassisFanRpm(&pSensor[fanSensorIdx], fanSensorCnt);
    if (rc != 0) {
        trace(TRACE_LEVEL_WARN, "Unable to retrieve fan Sensor info\n");
        free (pSensor);
        return (rc);
    }

    // send the data back
    *buffer = pSensor;
    *bufsize = totalSensorCnt * sizeof(emSensor_t);

    return (rc);
}

//System Uptime handler
static int
getSysUptime(void **buffer, UI32 *bufsize) {
	char cmdline[512];
	FILE *cmd_op = NULL;

	snprintf(cmdline, 512, "cat /proc/uptime | cut -d ' ' -f 1");
	WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::getSysUptime cmdline=") + cmdline);
	*buffer = malloc(512);
	if (*buffer == NULL) {
		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::netConfMessageHandler memory alloc fail!"));
		return (-1);
	}
	if ((cmd_op = popen(cmdline, "r")) != NULL) {
		while(fgets((char *)*buffer, 512, cmd_op)) {
			tracePrintf(TRACE_LEVEL_INFO, "RASLocalOM::getSysUptime buffer= %s", *buffer);
		}
	} else {
		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::getSysUptime popen fail"));
		free(*buffer);
		return (-1);
	}

	*bufsize = sizeof(*buffer);
	WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::getSysUptime bufsize sizeof = ") + *bufsize);
	tracePrintf(TRACE_LEVEL_INFO, "RASLocalOM::getSysUptime buffer= %s", *buffer);
	return (0);
}
			

// MSG handler for NetConf Messages
void
RASLocalObjectManager::netConfMessageHandler(RASNetConfMessage *pMessage)
{
    void *buffer = NULL;
    UI32 bufsize = 0;
    int  rc = 0;

    trace (TRACE_LEVEL_DEBUG, "RASLocalOM::netConfMessageHandler: Enter");
    tracePrintf(TRACE_LEVEL_DEBUG, true, true,
            "RASLocalOM::netConfMessageHandler: cmd=%d SC=%x SES=%x",
            pMessage->getCmd(), NC_SHOW_CHASSIS, NC_SHOW_ENV_SENSOR);

    switch(pMessage->getCmd()) {
    case NC_SHOW_CHASSIS:
        trace (TRACE_LEVEL_DEBUG, "RASLocalOM::netConfMessageHandler: calling getChassisInfo");
        rc = getChassisInfo(&buffer, &bufsize);
        break;

    case NC_SHOW_ENV_SENSOR:
        trace (TRACE_LEVEL_DEBUG, "RASLocalOM::netConfMessageHandler: calling getSensorInfo");
        rc = getSensorInfo(&buffer, &bufsize);
        break;
	
	case NC_SHOW_SYS_UPTIME:
		trace (TRACE_LEVEL_INFO, "RASLocalOM::netConfMessageHandler: calling system uptime");
		rc = getSysUptime(&buffer, &bufsize);
		tracePrintf(TRACE_LEVEL_INFO, "RASLocalOM::netConfMessageHandler buffer = %s", buffer);
		WaveNs::trace (TRACE_LEVEL_INFO, string ("RASLocalOM::netConfMessageHandler = bufsize") + bufsize);
		break;

    default:
        rc = -1;
        trace (TRACE_LEVEL_DEBUG, "RASLocalOM::netConfMessageHandler Invalid command");
        break;
    }

    if (rc == 0) {
        pMessage->addBuffer(pMessage->getReceiverLocationId(), bufsize, buffer, false);
        pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        trace (TRACE_LEVEL_INFO, "RASLocalOM::netConfMessageHandler: SUCCESS");
        free(buffer);
    } else {
        pMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
        trace (TRACE_LEVEL_ERROR, "RASLocalOM::netConfMessageHandler: FAILURE");
    }

    trace (TRACE_LEVEL_DEBUG, "RASLocalOM::netConfMessageHandler: Exit");
    reply (pMessage);
}

string RASLocalObjectManager::syncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();
	string data = "";
	char cmdline[512];
	FILE	*cmd_op = NULL;
	char	cmd_output[513];
	int rc = 0, this_slot = 0;
	char slot_string[10];
	string slotstr = "";
	string cmd = pRASLoggingMessage->getCmd();

	trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::Into syncDataOutputHandler"));
	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

	snprintf(cmdline, 512, "%s 2>/dev/null", (char *)cmd.data());

	if ((cmd_op = popen(cmdline, "r")) != NULL) {
		if (locationRole != LOCATION_STAND_ALONE)
			data.append(string("Rbridge-id: ") + pRASLoggingMessage->getMappedId() + string("\n"));

		// display CLI start
		if (!(pRASLoggingMessage->getCLIStart()).empty()) {
			data.append( pRASLoggingMessage->getCLIStart() + "\n" );
		}

		if ((strstr(cmdline, "coreshow")) ||
			(strstr(cmdline, "coreffdc"))) {
			if ((this_slot = getThisCPSlot()) != 0) {
				snprintf(slot_string, 10, "%s", slot_no2s(this_slot));
				slotstr = slot_string;
				trace (TRACE_LEVEL_INFO, string("slotString: ") + slotstr);
				data.append(string("#") + slotstr + string(":\n"));
			}
		}

		while (fgets(cmd_output, 512, cmd_op)) {
			data.append( cmd_output );
		}
		if ((rc = pclose(cmd_op)) < 0) {
			trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI pclose fail:") +
					cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			data.append( string( "1. Command Fail rc=" ) + rc + "\n" );
		}
		// display CLI completion
		if (!(pRASLoggingMessage->getCLIEnd()).empty()) {
			data.append( pRASLoggingMessage->getCLIEnd() + "\n" );
		}
	} else {
		trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI popen fail:") +
				cmdline + string (" rc=") + rc + string (" errno=") + -errno);
		data.append( string( "2. Command Fail rc=" ) + rc + "\n" );
	}

	// Escape `%` in the output buffer as maapi_cli_vprintf function is invoked
	// which behaves like printf & expects arguments which are
	// not present in these cases. So replace `%` with `%%`
	string escapeWith = "%";
	DcmToolKit::insertEscapeCharInBuffer(data, '%', escapeWith);

	data.append(string("\n"));

	return (data);
}

void RASLocalObjectManager::asyncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();
	string data = "";
	string cmd = pRASLoggingMessage->getCmd();
	char rbd_str[128] = "";
	char cmdline[512];
	FILE	*cmd_op = NULL;
	char	cmd_output[513];
	int rc = 0;

	trace(TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::Into asyncDataOutputHandler"));
	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
	if (locationRole != LOCATION_STAND_ALONE)
		snprintf(rbd_str, 128, "Rbridge-id :%d ", pRASLoggingMessage->getMappedId());


	snprintf(cmdline, 512, "%s 2>/dev/null", (char *)cmd.data());

	if ((cmd_op = popen(cmdline, "r")) != NULL) {
		// display CLI start 
		if (!(pRASLoggingMessage->getCLIStart()).empty()) {
			printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, pRASLoggingMessage->getCLIStart().c_str());
		}
		while (fgets(cmd_output, 512, cmd_op)) {
			printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, cmd_output);
		}
		if ((rc = pclose(cmd_op)) < 0) {
			trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI pclose fail:") +
					cmdline + string (" rc=") + rc + string (" errno=") + -errno);
			printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, "Command Fail\n");
		}

		// display CLI completion
		if (!(pRASLoggingMessage->getCLIEnd()).empty()) {
			printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, pRASLoggingMessage->getCLIEnd().c_str());
		}
	} else {
		trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI popen fail:") +
				cmdline + string (" rc=") + rc + string (" errno=") + -errno);
		printfToWaveClientSession (waveClientSessionContext, "%s%s\n", rbd_str, "Command Fail\n");
	}
}

void RASLocalObjectManager::asyncRASMANDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext)
{
    RASRasmanMessage *pRASRasmanMessage = reinterpret_cast<RASRasmanMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
    WaveClientSessionContext waveClientSessionContext = pRASRasmanMessage->getWaveClientSessionContext ();
    string data = "";
    string cmd = pRASRasmanMessage->getCmd();
    char cmdline[512];
    FILE    *cmd_op = NULL;
    char    cmd_output[513];
    int rc = 0;

    trace(TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::Into asyncRASMANDataOutputHandler cmd: ") + cmd);
    snprintf(cmdline, 512, "%s", (char *)cmd.data());

    if ((cmd_op = popen(cmdline, "r")) != NULL) {
        while (fgets(cmd_output, 512, cmd_op)) {
			if (strlen(cmd_output) > 1) {
                 printfToWaveClientSession (waveClientSessionContext, "%s\n", cmd_output);
			}
        }
        if ((rc = pclose(cmd_op)) < 0) {
            trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI pclose fail:") +
                    cmdline + string (" rc=") + rc + string (" errno=") + -errno);
            printfToWaveClientSession (waveClientSessionContext, "%s\n", "Command Fail\n");
        }

    } else {
        trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI popen fail:") +
                cmdline + string (" rc=") + rc + string (" errno=") + -errno);
        printfToWaveClientSession (waveClientSessionContext, "%s\n", "Command Fail\n");
    }
}

ResourceId RASLocalObjectManager::fosCLI(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   // MSG from Actionpoint worker to carry data
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());

   WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::WaveClientContext in RASLoggingMessage = ") +
		pRASLoggingMessage->getWaveClientOriginatingLocationId () + " " +
		pRASLoggingMessage->getWaveNativeClientId () + " " +
		pRASLoggingMessage->getWaveUserClientId());

	string data = "";

	string cmd = pRASLoggingMessage->getCmd();
	trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::fosCLI=") + cmd);

	/* By default all RAS show commands come in with sync flag set.
	   This means that the command output is not sent to the CLI session
	   but saved in a data buffer and sent back to be displayed by the
	   respective wysereaoperation handler. */

	if (pRASLoggingMessage->getSync()) {
		data = syncDataOutputHandler(pRASLocalMessagingContext);
		pRASLoggingMessage->setData(data);
	} else {
		asyncDataOutputHandler(pRASLocalMessagingContext);
	}

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::rasmanCLI(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   // MSG from Actionpoint worker to carry data
   RASRasmanMessage *pRASRasmanMessage= reinterpret_cast<RASRasmanMessage *> (pRASLocalMessagingContext->getPPrismMessage ());

   WaveNs::trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::WaveClientContext in RASRasmanMessage = ") +
        pRASRasmanMessage->getWaveClientOriginatingLocationId () + " " +
        pRASRasmanMessage->getWaveNativeClientId () + " " +
        pRASRasmanMessage->getWaveUserClientId());

    string data = "";

    string cmd = pRASRasmanMessage->getCmd();
    trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::rasmanCLI=") + cmd);

	WaveNs::trace (TRACE_LEVEL_DEBUG, "RASLocalOM::WaveClientContext in RASMANMessage Async "); 
    asyncRASMANDataOutputHandler(pRASLocalMessagingContext);
    return (WAVE_MESSAGE_SUCCESS);
}
// This function generates the syslog-ng config based on parameters present in
// the Dcmd database and restarts syslog-ng daemon.
ResourceId RASLocalObjectManager::generateSyslogNGConfig(void)
{
    vector<WaveManagedObject *> *pSyslogServers = NULL;
	RASManagedObject *pRASManagedObject = NULL;
	RASSysFcManagedObject *pRASSysFcManagedObject = NULL;
	int j, rc = 0;
	FILE *fp = NULL;
	char cmd[512];

	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig Temp copied"));
	rc = system("/bin/cp /etc/syslog-ng/syslog-ng.template /etc/syslog-ng/syslog-ng.tmp");
    if (WEXITSTATUS(rc) != 0) {
    	return (WAVE_MESSAGE_ERROR);
    }

	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig Open tmp file"));
	fp = fopen("/etc/syslog-ng/syslog-ng.tmp", "a+");
	if (fp == NULL) {
		return (WAVE_MESSAGE_ERROR);
	}

	/* Syslog facility */
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig Facility"));
	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASSysFcManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
	delete syncQueryContext;
	UI32 sys_log = 0;
	int size = 0;
	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::generateSyslogNGConfig No MO found\n");
		sys_log = RAS_LOG_LOCAL7;
	}

	size = pResults->size();
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig:Facility add/modify"));
	if (size != 0) {
		pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pResults)[0]);
		if (pRASSysFcManagedObject != NULL) {
			sys_log = pRASSysFcManagedObject->getSysFacility();
		}
	}
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	fprintf(fp, "filter f_remote { facility(local%d); };\n", sys_log);

	/* IPaddress/ port/ secure/ */
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig IPadress/port/secure"));
    pSyslogServers = querySynchronously (RASManagedObject::getClassName());
	prismAssert (pSyslogServers, __FILE__, __LINE__);
    for (j = 0; j < (int)pSyslogServers->size(); j++) {
		pRASManagedObject = dynamic_cast<RASManagedObject *> ((*pSyslogServers)[j]);
		IpVxAddress user_ip = pRASManagedObject->getSyslogIp();
		if (!(pRASManagedObject->getSyslogSecureMode())) {
			if (user_ip.isValidIpV4Address(user_ip.toString())) {
				fprintf(fp, "destination df_remote_%d {udp(\"%s\");};\n", j, (user_ip).toString().c_str());	
			} else if (user_ip.isValidIpV6Address(user_ip.toString())) {
				fprintf(fp, "destination df_remote_%d {udp6(\"%s\");};\n", j, (user_ip).toString().c_str());
			}
		} else {
            bool isValidIp = false;
			if (user_ip.isValidIpV4Address(user_ip.toString())) {
                isValidIp = true;
				fprintf(fp, "destination df_remote_%d {tcp", j);
			} else if (user_ip.isValidIpV6Address(user_ip.toString())) {
                isValidIp = true;
				fprintf(fp, "destination df_remote_%d {tcp6", j);
			}
            if (isValidIp) {
				fprintf(fp, "(\"%s\" port(%d) "
                        "tls(ca_dir(\"/etc/syslog-ng/ca.d\") "
                        "cipher_suite(\"AES128-SHA:AES256-SHA:"
                        "DHE-RSA-AES128-SHA:DHE-RSA-AES256-SHA\")));};\n",
                        (user_ip).toString().c_str(),
                        pRASManagedObject->getSyslogPort());
            }
		}
	}

	for (j = 0; j < (int)pSyslogServers->size (); j++) {
		fprintf(fp, "log { source(s_all); filter(f_remote); destination(df_remote_%d);};\n", j);
	}

	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig updated"));
	if (fp)
		fclose(fp);
	if (pSyslogServers)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSyslogServers);

	rc = system("/bin/cp /etc/syslog-ng/syslog-ng.tmp /etc/syslog-ng/syslog-ng.conf");
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig copied from tmp to conf rc") + WEXITSTATUS(rc) );

	//Updating the backend syslog facility
	sprintf(cmd, "/fabos/sbin/syslogdfacility -l %d", sys_log);
	rc = system(cmd);
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig update backend facilty rc") + WEXITSTATUS(rc) );
	
	rc = system("/bin/rm /etc/syslog-ng/syslog-ng.tmp");
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig removed tmp rc") + WEXITSTATUS(rc));
	rc = system("/etc/rc.d/init.d/syslog restart >> /var/log/syslog.log 2>&1");
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::generateSyslogNGConfig syslog restart rc") + WEXITSTATUS(rc));
	if (WEXITSTATUS(rc) != 0) {
		return (WAVE_MESSAGE_ERROR);
	}
    return (WAVE_MESSAGE_SUCCESS);
}

// MSG handler for ipVx cfg
void RASLocalObjectManager::ipVxMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
	trace (TRACE_LEVEL_DEBUG, "RASLocalOM::ipVxMessageHandler");
	// setup the sequence 
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	// If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in RASLocalMessagingContext
	// reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::queryLocalManagedObjectFromMappedId),
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::generateSyslogNGConfig),
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	};

	// put MSG to RASLocalMessagingContext
	RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
	// If MappedId is needed to get associated its MO
	// pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
	pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::auditLogMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
     trace (TRACE_LEVEL_INFO, "RASLocalOM::auditLogMessageHandler");

     // setup the sequence 
     PrismSynchronousLinearSequencerStep sequencerSteps[] =
     {
	// If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in RASLocalMessagingContext
	// reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::queryLocalManagedObjectFromMappedId),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::auditLog),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
     };

      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::RASGlobalConfigMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::GlobalConfig msg handler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::GlobalConfig),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::sysLogMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
	trace (TRACE_LEVEL_INFO, "RASLocalOM::sysLog msg handler");
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::generateSyslogNGConfig),
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
	reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	};
	// put MSG to RASLocalMessagingContext
	RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
	// If MappedId is needed to get associated its MO
	// pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
	pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::autoUploadMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::autoSupport msg handler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::autoUpload),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::autoUploadParamMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::autoSupportParam msg handler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::autoUploadParam),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::raslogSeverityCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::raslogCfgSeverity msg handler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::raslogSevCfg),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::raslogSyslogCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::raslogSyslogCfgMessageMessageHandler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::raslogSyslogCfg),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::raslogMsgCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::raslogMsgCfgMessageMessageHandler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::raslogMsgCfg),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

void RASLocalObjectManager::raslogModCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage)
{
      trace (TRACE_LEVEL_INFO, "RASLocalOM::raslogModCfgMessageMessageHandler");
      PrismSynchronousLinearSequencerStep sequencerSteps[] =
      {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::raslogModCfg),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
      };
      // put MSG to RASLocalMessagingContext
      RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASLoggingMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
      // If MappedId is needed to get associated its MO
      // pRASLocalMessagingContext->setMappedId (pRASLoggingMessage->getMappedId());
      pRASLocalMessagingContext->execute ();
}

ResourceId RASLocalObjectManager::ipVx(RASLocalMessagingContext *pRASLocalMessagingContext)
{

   // MSG from Actionpoint worker to carry data
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
   WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

   string              strIpAddr;
   IpVxAddress ipVxadrs = pRASLoggingMessage->getSyslogIp();
   int rc = 0;

	if (ipVxadrs.getAddressFamily() == AF_INET) {
   		trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::cfgMessageHandler: IPV4=")
		+ ipVxadrs.toString());
	} else {
		trace (TRACE_LEVEL_DEBUG, string("RASLocalOM::cfgMessageHandler:IPV6=")
		+ ipVxadrs.toString());
	}
   if (pRASLoggingMessage->getBoolData()) {
   	trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::cfgMessageHandler: remove"));
	strIpAddr = ipVxadrs.toString();
	syslogdIpRemove((char *)strIpAddr.c_str());
   } else {
   	trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::cfgMessageHandler: add"));
   	strIpAddr = ipVxadrs.toString();
   	rc = syslogdIpAdd((char *)strIpAddr.c_str());
   	if (rc != RAS_SYSLOG_SUCESS) {
		if (ipVxadrs.getAddressFamily() == AF_INET) {
		trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::ipV4 fail:") +
		string (" rc=") + rc + string (" errno=") + -errno);
		} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::ipV6 fail:") +
		string (" rc=") + rc + string (" errno=") + -errno);
		}
		printfToWaveClientSession (waveClientSessionContext, "%s", "Command Fail\n");
   	}
   }
#if 0
       snprintf(cmdline, 512, "/fabos/sbin/syslogdipadd %s 2>/dev/null", (char *)strIpAddr.c_str());
       tracePrintf(TRACE_LEVEL_DEBUG, true, true, "RASLocalOM::cfgMessageHandler: cmd=%s",
			     cmdline);

       if ((cmd_op = popen(cmdline, "r")) != NULL) {
	   while (fgets(cmd_output, 512, cmd_op)) {
	       printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
	   }
	   if ((rc = pclose(cmd_op)) < 0) {
	       trace (TRACE_LEVEL_INFO, string ("RASLocalOM::ipV4 fail:") +
		   cmdline + string (" rc=") + rc + string (" errno=") + -errno);
	       printfToWaveClientSession (waveClientSessionContext, "%s", "Command Fail\n");
	   }
       } else {
	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::ipV4 popen fail:") +
		   cmdline + string (" rc=") + rc + string (" errno=") + -errno);
	   printfToWaveClientSession (waveClientSessionContext,
		  "Fail to configure switch:%d syslog IP:%s\n",
		   pRASLoggingMessage->getMappedId(), strIpAddr.c_str());
       }
#endif

   pRASLoggingMessage->setData("");

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::auditLog(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
   WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

   RASAuditManagedObject *pRASAuditManagedObject = NULL;
   string cmd = "/fabos/sbin/auditcfg --class ";
   UI32 audit_class = 0;
   int i = 0, size = 0, sysret = -1;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASAuditManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::auditLog No MO\n");
	   /* if no MO found set the backend config to dummy */
	   cmd += "7"; /* DUMMY */
   }
   size = pResults->size();

   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::auditLog: add/modify"));
   for (i = 0; i < size; i++) {
	pRASAuditManagedObject = dynamic_cast<RASAuditManagedObject *> ((*pResults)[i]);
	if (pRASAuditManagedObject != NULL) {
   	  audit_class = pRASAuditManagedObject->getAuditLogclass();
	  /* match the case in RAS.yang */
	  switch (audit_class) {
		case 1:
			cmd += "2,"; /* SECURITY */
             	break;
             	case 2:
			cmd += "6,"; /* DCMCFG */
             	break;
             	case 3:
			cmd += "4,"; /* FIRMWARE */
             	break;
             	case 10:
			cmd += "2,3,4";
             	break;
             	default:
                	trace (TRACE_LEVEL_INFO, "RASLocalOM::auditLog : unknown flag");
	     	break;
          }
	}
   }
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::auditLog: cmd") + cmd.c_str());
   sysret = system(cmd.c_str());
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::auditLog: cmd return value: ")+ sysret);

   pRASLoggingMessage->setData("");
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::GlobalConfig(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

	RASGlobalConfigManagedObject *pRASGlobalConfigManagedObject = NULL;
	int size = 0, sysret = -1;
	char cmdline[512];
	Severity console = DCM_RASLOG_INFO;
	bool ffdc = 1;

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASGlobalConfigManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

	delete syncQueryContext;

	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::GlobalConfig No MO found\n");
	}

	size = pResults->size();

	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::GlobalConfig get config"));
	if (size != 0) {
		pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pResults)[0]);
		if (pRASGlobalConfigManagedObject != NULL) {
			console = (Severity) pRASGlobalConfigManagedObject->getConsole();
			ffdc = pRASGlobalConfigManagedObject->getFFDC();
		}
	}

	switch (pRASLoggingMessage->getWyserUserTag()) {
		case ras_ffdc:
	    		trace (TRACE_LEVEL_INFO, "RASLocalOM::GlobalConfig : Set ffdc = " +
					ffdc);
				if (ffdc) {
					snprintf(cmdline, 512, "/fabos/sbin/supportffdc --enable 2>/dev/null");
				} else {
				snprintf(cmdline, 512, "/fabos/sbin/supportffdc --disable 2>/dev/null");
				}
				break;
		case ras_console:
				trace (TRACE_LEVEL_INFO, "RASLocalOM::GlobalConfig : Set console = " +
					console);
				switch (console) {
					case DCM_RASLOG_INFO:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v info 2>/dev/null");
						break;
					case DCM_RASLOG_WARNING:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v warning 2>/dev/null");
						break;
					case DCM_RASLOG_ERROR:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v error 2>/dev/null");
						break;
					case DCM_RASLOG_CRITICAL:
						snprintf(cmdline, 512, "/fabos/sbin/errfilterset -d console -v critical 2>/dev/null");
						break;
					default:
						trace (TRACE_LEVEL_ERROR, "RASLocalOM::GlobalConfig : unknown console type");
						break;
				}
				break;
	}

	sysret = system(cmdline);
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::GlobalConfig: cmd=") + cmdline+ string(" sysret:") + sysret);

	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::sysLog(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
   WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

   RASSysFcManagedObject *pRASSysFcManagedObject = NULL;
   char cmd[64];
   UI32 sys_log = 0;
   int size = 0, sysret = -1;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASSysFcManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::sysLog No MO found\n");
	sys_log = RAS_LOG_LOCAL7;
   }
   size = pResults->size();

   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::sysLog: add/modify"));
   if (size != 0) {
	pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pResults)[0]);
	if (pRASSysFcManagedObject != NULL) {
   	  sys_log = pRASSysFcManagedObject->getSysFacility();
	}
   }

   sprintf(cmd, "/fabos/sbin/syslogdfacility -l %d", sys_log);
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::sysLog: cmd=") + cmd);
   sysret = system(cmd);
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::sysLog: sysret=") + sysret);	
   pRASLoggingMessage->setData("");
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::autoUpload(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
   WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

   RASAutoUploadManagedObject *pRASAutoUploadManagedObject = NULL;
   char cmd[512];
   bool is_enabled = false, operation = false, auto_support = false;
   int size = 0, sysret = -1;
   operation = pRASLoggingMessage->getBoolData();
   auto_support = pRASLoggingMessage->getAutoSupport();

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoSupport No MO found\n");
   }
   size = pResults->size();

   if (size != 0) {
	pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[0]);
	if (pRASAutoUploadManagedObject != NULL) {
   	  is_enabled = pRASAutoUploadManagedObject->getAutoSupport();
	  tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoSupport is_enabled=%d\n", is_enabled);
	
	}
   }

   if (!operation && auto_support) {	
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoSupport enable");
	   sprintf(cmd, "/fabos/cliexec/supportftp -e 2>/dev/null");
	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autoSupport: cmd=") + cmd);
	   sysret = system(cmd);
	   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::autoSupport: sysret=") + sysret);	
	} else if (operation && !auto_support) {
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoSupport disable");
	   sprintf(cmd, "/fabos/cliexec/supportftp -d 2>/dev/null");
       trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autoSupport: cmd=") + cmd);
	   sysret = system(cmd);
	   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::autoSupport: sysret=") + sysret);	
    }	   		

   
   pRASLoggingMessage->setData("");
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::autoUploadParam(RASLocalMessagingContext *pRASLocalMessagingContext)
{
   RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
   WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

   RASAutoUploadManagedObject *pRASAutoUploadManagedObject = NULL;
   char cmd[512];
   IpVxAddress ipaddr;
   string ipaddr_str, user, dir, pass, proto;
   int size = 0, sysret = -1;
   bool option = false;
   UI32 tag = 0;
   option = pRASLoggingMessage->getBoolData();
   tag = pRASLoggingMessage->getWyserUserTag();

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoUploadParam No MO found\n");
   }
   size = pResults->size();

   if (size != 0) {
	pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[0]);
	if (pRASAutoUploadManagedObject != NULL) {
	  ipaddr = pRASAutoUploadManagedObject->getHostIp();
	  ipaddr_str = ipaddr.toString();
	  user = pRASAutoUploadManagedObject->getUser();
	  dir = pRASAutoUploadManagedObject->getDirectory();
	  pass = pRASAutoUploadManagedObject->getPassword();
	  proto = pRASAutoUploadManagedObject->getProtocol();	  
	tracePrintf(TRACE_LEVEL_ERROR, "RASLocalObjectManager:: Pass1= %s", pass.c_str());
	  if (pass.length() != 0) {
		  pass = AAACrypto::decryptPassword(pass);
		  if (pass.length() == 0) {
			  tracePrintf(TRACE_LEVEL_ERROR, "RASLocalObjectManager:: Password Decryption failed");
	  	  }	
	  }		
	}
   }
	tracePrintf(TRACE_LEVEL_ERROR, "RASLocalObjectManager:: Option =%d", option);
	tracePrintf(TRACE_LEVEL_ERROR, "RASLocalObjectManager:: Pass= %s", pass.c_str());
	  if ((tag == ras_autoupload_param) && option) {
		sprintf(cmd, "/fabos/cliexec/supportftp -R");
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autouploadparam: cmd=") + cmd);
		sysret = system(cmd);
	}
	else if(!option) {
		if ((ipaddr_str.length() != 0) && (user.length() != 0) && (proto.length() != 0) && (dir.length() != 0) && (pass.length() != 0)) {
		   sprintf(cmd, "/fabos/cliexec/supportftp -s -h %s -u %s -d %s -p %s -l %s 2>/dev/null", ipaddr_str.c_str(), user.c_str(), dir.c_str(), pass.c_str(), proto.c_str());
		   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autouploadparam: cmd=") + cmd);
		  sysret = system(cmd);
		}
	}
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::autouploadparam: sysret=") + sysret); 
   pRASLoggingMessage->setData("");
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::raslogSevCfg(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	char cmd[1024];
	char cmd_output[1024];
	FILE *cmd_fp = NULL;
	string msgid;
	int size = 0;
	int rc = 0;
	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

	RASLOGConfigureManagedObject *pRASLOGConfigureManagedObject = NULL;

	msgid = pRASLoggingMessage->getMsgId();
	trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogCfgSeverity : msgid=") + msgid);
	

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

	delete syncQueryContext;

	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
	trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogCfgSeverity : location role=" ) + locationRole);

	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::raslogCfgSeverity No MO found\n");
	}
	size = pResults->size();

	if (size != 0) {
		pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[0]);
		if (pRASLOGConfigureManagedObject != NULL) {
			trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogCfgSeverity : Set severity =") + pRASLOGConfigureManagedObject->getSeverity());
		}
	}
	switch (pRASLoggingMessage->getSeverity()) {
		case DCM_RASLOG_INFO:
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity info 2>/dev/null", msgid.c_str());
			break;
		case DCM_RASLOG_WARNING:
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity warning 2>/dev/null", msgid.c_str());
			break;
		case DCM_RASLOG_ERROR:
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity error 2>/dev/null", msgid.c_str());
			break;
		case DCM_RASLOG_CRITICAL:
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity critical 2>/dev/null", msgid.c_str());
			break;
		case DCM_RASLOG_DEFAULT:
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity default 2>/dev/null", msgid.c_str());
			break;
		default:
			trace (TRACE_LEVEL_ERROR, "RASLocalOM:raslogCfgSeverity unknown severity");
			break;
	}
	trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgSeverity: cmd=") + cmd);
	
	if ((cmd_fp = popen(cmd, "r")) != NULL) {
		while (fgets(cmd_output, 1024, cmd_fp)) {
			if (pRASLoggingMessage->getCLIOutput())
				trace (TRACE_LEVEL_INFO, string ("switch ") + pRASLoggingMessage->getMappedId() +
					pRASLoggingMessage->getMappedId() + string(": ") + cmd_output);
			if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			} else {
				printfToWaveClientSession (waveClientSessionContext, "switch %d: %s", pRASLoggingMessage->getMappedId(),
						cmd_output);
			}
		}
		if ((rc = pclose(cmd_fp)) != 0) {
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgSeverity pclose fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgSeverity popen fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
		if (locationRole == LOCATION_STAND_ALONE) {
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		} else {
			printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s",
					pRASLoggingMessage->getMappedId(), "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	}
  	
	pRASLoggingMessage->setData("");
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::raslogSyslogCfg(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	char cmd[1024];
	char cmd_output[1024];
	FILE *cmd_fp = NULL;
	string msgid;
	int size = 0, rc = 0;

	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

	RASLOGConfigureManagedObject *pRASLOGConfigureManagedObject = NULL;
	msgid = pRASLoggingMessage->getMsgId();
		

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

	delete syncQueryContext;

	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
	trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogSyslogCfg: location role=" ) + locationRole);

	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::raslogMsgCfgNo MO found\n");
	}
	size = pResults->size();

	if (size != 0) {
		pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[0]);
		if (pRASLOGConfigureManagedObject != NULL) {
			trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogSyslogCfg: Get syslog attribute=") + pRASLOGConfigureManagedObject->getSyslogEnable());
		}
	}

	if (pRASLoggingMessage->getSyslogEnable()) {
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --enable -syslog %s 2>/dev/null", msgid.c_str());
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogSyslogCfg: cmd=") + cmd);
	} else if (!(pRASLoggingMessage->getSyslogEnable())) {
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --disable -syslog %s 2>/dev/null", msgid.c_str());
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogSyslogCfg: cmd=") + cmd);
	}
	
	if ((cmd_fp = popen(cmd, "r")) != NULL) {
		while (fgets(cmd_output, 1024, cmd_fp)) {
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogSyslogCfg: ONE"));
			if (pRASLoggingMessage->getCLIOutput())
				trace (TRACE_LEVEL_INFO, string ("switch ") + pRASLoggingMessage->getMappedId() +
					pRASLoggingMessage->getMappedId() + string(": ") + cmd_output);
			if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			} else {
				printfToWaveClientSession (waveClientSessionContext, "switch %d: %s", pRASLoggingMessage->getMappedId(),
						cmd_output);
			}
		}
		if ((rc = pclose(cmd_fp)) != 0) {
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogSyslogCfg : pclosefail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogSyslogCfgpopen fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
		if (locationRole == LOCATION_STAND_ALONE) {
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		} else {
			printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s",
					pRASLoggingMessage->getMappedId(), "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	}
	
	pRASLoggingMessage->setData("");
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return (WAVE_MESSAGE_SUCCESS);
}

ResourceId RASLocalObjectManager::raslogMsgCfg(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	char cmd[1024];
	char cmd_output[1024];
	FILE *cmd_fp = NULL;
	string msgid;
	int size = 0, rc = 0;

	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

	RASLOGConfigureManagedObject *pRASLOGConfigureManagedObject = NULL;

	msgid = pRASLoggingMessage->getMsgId();

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

	delete syncQueryContext;

	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
	trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogMsgCfg: location role=" ) + locationRole);

	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::raslogMsgCfgNo MO found\n");
	}
	size = pResults->size();

	if (size != 0) {
		pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[0]);
		if (pRASLOGConfigureManagedObject != NULL) {
			trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogMsgCfg: Set message attribute=") + pRASLOGConfigureManagedObject->getMessageDisable());
		}
	}

	if (pRASLoggingMessage->getMessageDisable()) {
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --disable -log %s 2>/dev/null", msgid.c_str());
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogMsgCfg: cmd=") + cmd);
	} else if (!(pRASLoggingMessage->getMessageDisable())) {
			snprintf(cmd, 1024, "/fabos/sbin/rasadmin --enable -log %s 2>/dev/null", msgid.c_str());
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogMsgCfg: cmd=") + cmd);
	}
	
	if ((cmd_fp = popen(cmd, "r")) != NULL) {
		while (fgets(cmd_output, 1024, cmd_fp)) {
			if (pRASLoggingMessage->getCLIOutput())
				trace (TRACE_LEVEL_INFO, string ("switch ") + pRASLoggingMessage->getMappedId() +
					pRASLoggingMessage->getMappedId() + string(": ") + cmd_output);
			if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			} else {
				printfToWaveClientSession (waveClientSessionContext, "switch %d: %s", pRASLoggingMessage->getMappedId(),
						cmd_output);
			}
		}
		if ((rc = pclose(cmd_fp)) != 0) {
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogMsgCfgpclose fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogMsgCfg popen fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
		if (locationRole == LOCATION_STAND_ALONE) {
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		} else {
			printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s",
					pRASLoggingMessage->getMappedId(), "Command Failed\n");
			if (pRASLOGConfigureManagedObject)
				delete pRASLOGConfigureManagedObject;
			return (rc);
		}
	}
  	
	pRASLoggingMessage->setData("");
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return (WAVE_MESSAGE_SUCCESS);
}


ResourceId RASLocalObjectManager::raslogModCfg(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	char cmd[1024];
	char cmd_output[1024];
	FILE *cmd_fp = NULL;
	string modid;
	int size = 0;
	int rc = 0;
	RASLoggingMessage *pRASLoggingMessage = reinterpret_cast<RASLoggingMessage *> (pRASLocalMessagingContext->getPPrismMessage ());
	WaveClientSessionContext waveClientSessionContext = pRASLoggingMessage->getWaveClientSessionContext ();

	RASMODConfigureManagedObject *pRASMODConfigureManagedObject = NULL;

	modid = pRASLoggingMessage->getModId();
	
	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASMODConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

	delete syncQueryContext;

	LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();
	trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogModCfg: location role=" ) + locationRole);

	if ((NULL == pResults) || (pResults->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::raslogModCfg No MO found\n");
	}
	size = pResults->size();

	if (size != 0) {
		pRASMODConfigureManagedObject = dynamic_cast<RASMODConfigureManagedObject *> ((*pResults)[0]);
		if (pRASMODConfigureManagedObject != NULL) {
			trace (TRACE_LEVEL_INFO, string("RASLocalOM::raslogModCfg: Set module attribute=") + pRASMODConfigureManagedObject->getModuleDisable());
		}
	}

	if (pRASLoggingMessage->getModuleDisable()) {
		snprintf(cmd, 1024, "/fabos/sbin/rasadmin --disable -module %s 2>/dev/null", modid.c_str());
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogModCfg: cmd=") + cmd);
	} else if (!(pRASLoggingMessage->getModuleDisable())) {
		snprintf(cmd, 1024, "/fabos/sbin/rasadmin --enable -module %s 2>/dev/null", modid.c_str());
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogModCfg: cmd=") + cmd);
	}
	
	if ((cmd_fp = popen(cmd, "r")) != NULL) {
		while (fgets(cmd_output, 1024, cmd_fp)) {
			if (pRASLoggingMessage->getCLIOutput())
				trace (TRACE_LEVEL_INFO, string ("switch ") + pRASLoggingMessage->getMappedId() +
					pRASLoggingMessage->getMappedId() + string(": ") + cmd_output);
			if (locationRole == LOCATION_STAND_ALONE) {
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			} else {
				printfToWaveClientSession (waveClientSessionContext, "switch %d: %s", pRASLoggingMessage->getMappedId(),
						cmd_output);
			}
		}
		if ((rc = pclose(cmd_fp)) != 0) {
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogModCfgpclose fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
			if (pRASMODConfigureManagedObject)
				delete pRASMODConfigureManagedObject;
			return (rc);
		}
	} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogModCfg popen fail:") + cmd + string ("rc=")
					+ rc + string ("errno=") + -errno);
		if (locationRole == LOCATION_STAND_ALONE) {
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			if (pRASMODConfigureManagedObject)
				delete pRASMODConfigureManagedObject;
			return (rc);
		} else {
			printfToWaveClientSession (waveClientSessionContext, "Rbridge-id %d: %s",
					pRASLoggingMessage->getMappedId(), "Command Failed\n");
			if (pRASMODConfigureManagedObject)
				delete pRASMODConfigureManagedObject;
			return (rc);
		}
	}
  	
	pRASLoggingMessage->setData("");
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	return (WAVE_MESSAGE_SUCCESS);
}


void RASLocalObjectManager::ipVxSecondBoot()
{
   string              strIpAddr;
   int rc = 0;
   IpVxAddress db_ip;
   int size = 0, i = 0;

   RASManagedObject *pRASManagedObject = NULL;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   // remove all syslog IP
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::ipVxSecondBoot reset syslog IP"));

   syslogdIpClear();

   if ((NULL == pResults) || (pResults->size() == 0)) {
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::ipVxSecondBoot No MO\n");
	   return ;
   }
   size = pResults->size();

   for (i = 0; i < size; i++) {
     pRASManagedObject = dynamic_cast<RASManagedObject *> ((*pResults)[i]);
     if (pRASManagedObject != NULL) {
       db_ip = pRASManagedObject->getSyslogIp();
       strIpAddr = db_ip.toString();
           tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::ipVxSecondBoot set syslog IP %s\n", strIpAddr.c_str());
       rc = syslogdIpAdd((char *)strIpAddr.c_str());
       if (rc != RAS_SYSLOG_SUCESS) {
	 	tracePrintf(TRACE_LEVEL_ERROR, true, true, "RASLocalOM::ipVxSecondBoot rc=%d error=%d\n", rc, -errno);
       }
     } else {
	        tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::ipVxSecondBoot No syslog IP found\n");	
     }
   }

   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::autoUploadBoot()
{
   bool is_enabled = false;
   IpVxAddress ipaddr;
   string ipaddr_str, user, dir, pass, proto;
   int size = 0, i = 0, sysret = -1;
   char cmd[512];

   RASAutoUploadManagedObject *pRASAutoUploadManagedObject = NULL;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASAutoUploadManagedObject::getClassName());
//	syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
//	trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::autoUploadBoot: find switchid=") + id);
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;


   if ((NULL == pResults) || (pResults->size() == 0)) {
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoUploadBoot No MO\n");
		sprintf(cmd, "/fabos/cliexec/supportftp -R");
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::Updating backend"));
		trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autouploadparam cmd=") + cmd);
		sysret = system(cmd);
	   return ;
   }
   size = pResults->size();

   for (i = 0; i < size; i++) {
     pRASAutoUploadManagedObject = dynamic_cast<RASAutoUploadManagedObject *> ((*pResults)[i]);
     if (pRASAutoUploadManagedObject != NULL) {
       is_enabled = pRASAutoUploadManagedObject->getAutoSupport();
       tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoUploadBoot set autosupport %d\n", is_enabled);
	   ipaddr = pRASAutoUploadManagedObject->getHostIp();
	   ipaddr_str = ipaddr.toString();
	   user = pRASAutoUploadManagedObject->getUser();
	   proto = pRASAutoUploadManagedObject->getProtocol();
	   dir = pRASAutoUploadManagedObject->getDirectory();
	   pass = pRASAutoUploadManagedObject->getPassword();
 	   pass = AAACrypto::decryptPassword(pass);
	   if (pass.length() == 0) {
		   tracePrintf(TRACE_LEVEL_ERROR, "RASLocalObjectManager:autoUploadBoot Password Decryption failed");
	   }	

			if ((ipaddr_str.length() != 0) && (user.length() != 0) && (dir.length() != 0) && (pass.length() != 0)) {
				sprintf(cmd, "/fabos/cliexec/supportftp -s -h %s -u %s -d %s -p %s -l %s 2>/dev/null", ipaddr_str.c_str(), user.c_str(), dir.c_str(), pass.c_str(), proto.c_str());
				trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autouploadparam cmd=") + cmd);
				sysret = system(cmd);
			} else { 
				sprintf(cmd, "/fabos/cliexec/supportftp -R");
				trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autouploadparam cmd=") + cmd);
				sysret = system(cmd);
			}
		if (is_enabled) {
			sprintf(cmd, "/fabos/cliexec/supportftp -e 2>/dev/null");
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autoupload enable cmd=") + cmd);
			sysret = system(cmd);
		} else if (!is_enabled) {
			sprintf(cmd, "/fabos/cliexec/supportftp -d 2>/dev/null");
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::autoupload enable cmd=") + cmd);
			sysret = system(cmd);
		}
     } else {
	        tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::autoUploadBoot No autosupport entry found\n");	
     }
   }
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::autoupload enable sysret=") + sysret);		
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::raslogCfgBoot()
{
	UI32 log_sev;
	string msgid;
	bool sys_attr = false, msg_attr = false;
	int size = 0, i, sysret = -1;
	char cmd[512];
 

   RASLOGConfigureManagedObject *pRASLOGConfigureManagedObject = NULL;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASLOGConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot "));

   if ((NULL == pResults) || (pResults->size() == 0)) {
	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot No MO found"));
	   return ;
   }
   size = pResults->size();
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot size=") + size);

	for (i = 0; i < size; i++) {
		pRASLOGConfigureManagedObject = dynamic_cast<RASLOGConfigureManagedObject *> ((*pResults)[i]);
		if (pRASLOGConfigureManagedObject != NULL) {
			msgid = pRASLOGConfigureManagedObject->getMsgId();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot msgid=") + msgid);

			//severity
			log_sev = pRASLOGConfigureManagedObject->getSeverity();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot log_sev=") + log_sev);
			//syslog
			sys_attr = pRASLOGConfigureManagedObject->getSyslogEnable();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot set syslog attr=") + sys_attr);
			//message
			msg_attr = pRASLOGConfigureManagedObject->getMessageDisable();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot set msg attr=") + msg_attr);
			
			switch (log_sev) {
				case DCM_RASLOG_INFO:
					snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity info 2>/dev/null", msgid.c_str());
					break;
				case DCM_RASLOG_WARNING:
					snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity warning 2>/dev/null", msgid.c_str());
					break;
				case DCM_RASLOG_ERROR:
					snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity error 2>/dev/null", msgid.c_str());
					break;
				case DCM_RASLOG_CRITICAL:
					snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity critical 2>/dev/null", msgid.c_str());
					break;
				case DCM_RASLOG_DEFAULT:
					snprintf(cmd, 1024, "/fabos/sbin/rasadmin --set -log %s -severity default 2>/dev/null", msgid.c_str());
					break;
				default:
					trace (TRACE_LEVEL_ERROR, "RASLocalOM:raslogCfgSeverity unknown severity");
					break;
			}
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot severity set cmd=") + cmd);

			if (sys_attr) {
				snprintf(cmd, 1024, "/fabos/sbin/rasadmin --enable -syslog %s 2>/dev/null", msgid.c_str());
				trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot syslog enable cmd=") + cmd);
				sysret = system(cmd);
			}
			if (msg_attr) {
				snprintf(cmd, 1024, "/fabos/sbin/rasadmin --disable -log %s 2>/dev/null", msgid.c_str());
				trace (TRACE_LEVEL_INFO, string ("RASLocalOM::raslogCfgBoot cmd=") + cmd);
				sysret = system(cmd);
			}
     	} else {
	        tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::raslogCfgBoot No MO entry found\n");	
     	}
   }
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::raslogCfgBoot sysret=") + sysret);	
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::rasmodCfgBoot()
{
	string modid;
	bool mod_attr = false;
	int  size = 0, i, sysret = -1;
	char cmd[512];
 

   RASMODConfigureManagedObject *pRASMODConfigureManagedObject = NULL;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASMODConfigureManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot"));
   if ((NULL == pResults) || (pResults->size() == 0)) {
   	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot No MO found"));
	   return ;
   }

   size = pResults->size();
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot size=") + size);

	for (i = 0; i < size; i++) {
		pRASMODConfigureManagedObject = dynamic_cast<RASMODConfigureManagedObject *> ((*pResults)[i]);
	 	if (pRASMODConfigureManagedObject != NULL) {
			modid = pRASMODConfigureManagedObject->getModId();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot modid=") + modid);

			//module
			mod_attr = pRASMODConfigureManagedObject->getModuleDisable();
			trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot set mod attr=") + mod_attr);
			
			if (mod_attr) {
				snprintf(cmd, 1024, "/fabos/sbin/rasadmin --disable -module %s 2>/dev/null", modid.c_str());
				trace (TRACE_LEVEL_INFO, string ("RASLocalOM::rasmodCfgBoot cmd=") + cmd);
				sysret = system(cmd);
				trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::rasmodCfgBoot sysret=") + sysret);
			}
		 } else {
	        tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::rasmodCfgBoot No MO entry found\n");	
		 }
	} 
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::syslogFacility()
{
   RASSysFcManagedObject *pRASSysFcManagedObject = NULL;
   char cmd[64];
   UI32 sys_log = 0;
   int size = 0, sysret = -1;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASSysFcManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::syslogFacility No MO found\n");
	sys_log = RAS_LOG_LOCAL7;
   }
   size = pResults->size();

   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::syslogFacility: set"));
   if (size != 0) {
	pRASSysFcManagedObject = dynamic_cast<RASSysFcManagedObject *> ((*pResults)[0]);
	if (pRASSysFcManagedObject != NULL) {
   	  sys_log = pRASSysFcManagedObject->getSysFacility();
	}
   }

   sprintf(cmd, "/fabos/sbin/syslogdfacility -l %d", sys_log);
   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::syslogFacility: cmd=") + cmd);
   sysret = system(cmd);
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::syslogFacility: sysret=") + sysret);	
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::auditClass()
{
   RASAuditManagedObject *pRASAuditManagedObject = NULL;
   string cmd = "/fabos/sbin/auditcfg --class ";
   int size = 0, i = 0, sysret = -1;

   WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(RASAuditManagedObject::getClassName());
   vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);

   delete syncQueryContext;

   if ((NULL == pResults) || (pResults->size() == 0)) {
	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::auditClass No MO"));
	   /* if no MO found set the backend config to dummy */
	   cmd += "7";
   }
   size = pResults->size();

   for (i = 0; i < size; i++) {
   	pRASAuditManagedObject = dynamic_cast<RASAuditManagedObject *> ((*pResults)[i]);
   	if (pRASAuditManagedObject != NULL) {
   	   UI32 audit_class = pRASAuditManagedObject->getAuditLogclass();
   	   trace (TRACE_LEVEL_INFO, string ("RASLocalOM::auditClass class=%d") + audit_class);

	    /* match the case in RAS.yang */
	    switch (audit_class) {
		case 1:
	   		cmd += "2,"; /* SECURITY */
		break;
		case 2:
	   		cmd += "6,"; /* DCMCFG */
		break;
		case 3:
	   		cmd += "4,"; /* FIRMWARE */
		break;
		case 10:
	   		cmd += "2,3,4";
		break;
		default:
	   		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::auditClass Invalid class\n");
		break;
   	   }
	}
   }
   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::auditClass cmd=%s\n", cmd.c_str());
   /* Enable the auditlog filters */
   sysret= system("/fabos/sbin/auditcfg --enable");
   sysret= system(cmd.c_str());
   trace (TRACE_LEVEL_DEBUG, string ("RASLocalOM::auditClass: sysret: ")+ sysret);		
   WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
}

void RASLocalObjectManager::loggingBootReplay() {
	
	LoggingLocalManagedObject *pLogging = NULL;
	string cmd_data;
	char cmd_op[512];
	int rbrid, sysret = -1, size = 0;
	Severity console = DCM_RASLOG_INFO;
	bool ffdc = 1;
	RASGlobalConfigManagedObject *pRASGlobalConfigManagedObject = NULL;

	trace (TRACE_LEVEL_INFO, "RASLocalObjectManager::loggingBootReplay: Entered....");

	int localId = DcmToolKit::getLocalMappedId();
	string id = string ("") + localId;

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(LoggingLocalManagedObject::getClassName());
	syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
	trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::loggingBootReplay: find switchid=") + id);
	vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContext);
	delete syncQueryContext;

	if ((NULL == pResults) || (pResults->size() == 0)) {
	   tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalOM::loggingBootReplay no Logging MO 0\n");
	   return ;
	}

	pLogging = dynamic_cast<LoggingLocalManagedObject *> ((*pResults)[0]);
	if (pLogging == NULL) {
	    tracePrintf(TRACE_LEVEL_ERROR, true, true, "RASLocalOM::loggingBootReplay no Logging MO 1\n");
	    return ;	
	}
	
	WaveManagedObjectSynchronousQueryContext *syncQueryContext1 =
        new WaveManagedObjectSynchronousQueryContext(RASGlobalConfigManagedObject::getClassName());
    vector<WaveManagedObject *> *pLog = querySynchronously (syncQueryContext1);
      
    delete syncQueryContext1;
	
	 if ((NULL == pLog) || (pLog->size() == 0)) {
		tracePrintf(TRACE_LEVEL_INFO, true, true, "RASLocalObjectManager::loggingBootReplay No MO found\n");
		return;
	}
	
	size = pLog->size();

    trace (TRACE_LEVEL_INFO, string ("RASLocalOM::loggingBootReplay get config"));
    if (size != 0) {
        pRASGlobalConfigManagedObject = dynamic_cast<RASGlobalConfigManagedObject *> ((*pLog)[0]);
        if (pRASGlobalConfigManagedObject != NULL) {
            console = (Severity) pRASGlobalConfigManagedObject->getConsole();
            ffdc = pRASGlobalConfigManagedObject->getFFDC();
        }   
    }

	trace (TRACE_LEVEL_INFO, string ("RASLOM::loggingBootReplay:SID=") +
		       pLogging->getSwitchId());
	cmd_data = pLogging->getChassisName();
	trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::loggingBootReplay: chassis_name=") + cmd_data);
		snprintf(cmd_op, 512, "/fabos/bin/chassisname %s >/dev/null 2>&1", (char *)cmd_data.c_str());
	sysret = system(cmd_op);
	trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::loggingBootReplay: sysret =") + sysret);
	if(!raslogm_set_swname( -1, (char *)cmd_data.c_str())) {
		trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::loggingBootReplay: chassisname sent to raslogd"));
	} else {
		trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::loggingBootReplay: chassisname send failed"));
	}

	cmd_data = pLogging->getHostName();
	trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::loggingBootReplay: host_name=") + cmd_data);
	snprintf(cmd_op, 512, "/fabos/bin/switchname %s >/dev/null 2>&1", (char *)cmd_data.c_str());
	sysret = system(cmd_op);
	trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::loggingBootReplay:sysret =") + sysret);


	rbrid = DcmToolKit::isNodeWithRbridgeIdInCluster(DcmToolKit::getLocalMappedId());

	if (rbrid > 0) {
		snprintf(cmd_op, 512, "/sbin/errlogutest -a5 -l %d >/dev/null 2>&1", DcmToolKit::getThisVcsNodeDomainId());
	    trace (TRACE_LEVEL_INFO, string ("RASLocalOM::setup raslogd VCS RID: cmd=") + cmd_op);
	   sysret = system(cmd_op);
	  trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::loggingBootReplay:sysret =") + sysret);	
	} else {
	    trace (TRACE_LEVEL_INFO, string ("RASLocalOM::not setup raslogd VCS RID: None VCS node RID=") + rbrid);
	}

	// Console severity setting
	trace (TRACE_LEVEL_INFO, "RASLocalOM::loggingBootReplay  Set console = " + console);
	switch (console) {
		case DCM_RASLOG_INFO:
			snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v info 2>/dev/null");
			break;
		case DCM_RASLOG_WARNING:
			snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v warning 2>/dev/null");
			break;
		case DCM_RASLOG_ERROR:
			snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v error 2>/dev/null");
			break;
		case DCM_RASLOG_CRITICAL:
			snprintf(cmd_op, 512, "/fabos/sbin/errfilterset -d console -v critical 2>/dev/null");
			break;
		default:
			trace (TRACE_LEVEL_ERROR, "RASLocalOM::loggingBootReplay : unknown console type");
			break;
	}
	sysret = system(cmd_op);
	trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::loggingBootReplay:sysret =") + sysret);

	// FFDC
	trace (TRACE_LEVEL_INFO, "RASLocalOM::loggingBootReplay : Set ffdc = " + ffdc);
	if (ffdc) {
		snprintf(cmd_op, 512, "/fabos/sbin/supportffdc --enable 2>/dev/null");
	} else {
		snprintf(cmd_op, 512, "/fabos/sbin/supportffdc --disable 2>/dev/null");
	}

	sysret = system(cmd_op);
	trace (TRACE_LEVEL_DEBUG, string ("RASLocalObjectManager::loggingBootReplay:sysret =") + sysret);
	
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pLog);

	return;
}

void RASLocalObjectManager::SSStatusMessageHandler(
                            RASLoggingMessage *pRASLoggingMessage)
{
    const char * const pFname = "RASLocalObjectManager::SSStatusMessageHandler";
    ss_progress_t ss;
    
    trace (TRACE_LEVEL_INFO, string(pFname) + ": Entering...");

    // TBD: session id is always 0
    ss_get_status(0, &ss);

    pRASLoggingMessage->setSsStatus(ss.status);
    pRASLoggingMessage->setSsMsg(string(ss.msg));
    pRASLoggingMessage->setSsPercentageOfCompletion(ss.percentage);

    pRASLoggingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pRASLoggingMessage);
}

SSLocalObjectManager::SSLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
    DcmShell::addToWaveShell ();

	associateWithVirtualWaveObjectManager (SSObjectManager::getInstance ());
    addOperationMap (SUPPORTSAVE, reinterpret_cast<PrismMessageHandler> (&SSLocalObjectManager::SSMessageHandler));   
}

SSLocalObjectManager::~SSLocalObjectManager ()
{
}

string  SSLocalObjectManager::getClassName()
{
    return ("SSLocalObjectManager");
}

SSLocalObjectManager *SSLocalObjectManager:: getInstance ()
{
    static SSLocalObjectManager *pSSLocalObjectManager = new  SSLocalObjectManager ();

    WaveNs::prismAssert (NULL !=  pSSLocalObjectManager, __FILE__, __LINE__);

    return (pSSLocalObjectManager);
}

PrismServiceId  SSLocalObjectManager::getPrismServiceId()
{
        return ((getInstance ())->getServiceId ());
}

PrismMessage  *SSLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
	trace (TRACE_LEVEL_DEBUG, string ("SSLocalOM::createMessageInstance opcode=") + operationCode);
        switch (operationCode)
        {
	case SUPPORTSAVE:
                pPrismMessage = new RASSupportMessage;
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

// MSG handler for Actionpoint SUPPORTSAVE
void SSLocalObjectManager::SSMessageHandler(RASSupportMessage *pRASSupportMessage)
{
    trace (TRACE_LEVEL_DEBUG, "SSLocalOM::SSCLIMessageHandler");
 
	RASLocalSSThread *pRASLocalSSThread = new RASLocalSSThread(m_pWaveObjectManager, pRASSupportMessage);
	WaveThreadStatus threadStatus =   pRASLocalSSThread->run();
	if (WAVE_THREAD_SUCCESS != threadStatus) {
		trace(TRACE_LEVEL_ERROR, "SSLocalObjectManager::SSMessageHandler:"
			" Local thread failed with status: " +
			FrameworkToolKit::localize(threadStatus));
	} else {
		trace(TRACE_LEVEL_SUCCESS, "SSLocalObjectManager::SSMessageHandler:"
			" Local thread started successfully");
	}
}
    RASLogLocalObjectManager::RASLogLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        trace (TRACE_LEVEL_INFO,
        "RASLogLocalObjectManager::RASLogLocalObjectManager: Entering...");
        addOperationMap (RASLOGSHOW, reinterpret_cast<PrismMessageHandler>
                        (&RASLogLocalObjectManager::RASLogShowMessageHandler));
    }

    RASLogLocalObjectManager::~RASLogLocalObjectManager ()
    {
    }

    RASLogLocalObjectManager  *RASLogLocalObjectManager::getInstance()
    {
        static RASLogLocalObjectManager *pRASLogLocalObjectManager =
                                         new RASLogLocalObjectManager ();

        WaveNs::prismAssert (NULL != pRASLogLocalObjectManager,
                              __FILE__, __LINE__);
        return (pRASLogLocalObjectManager);
    }

    string  RASLogLocalObjectManager::getClassName()
    {
        return ("RASLogLocalObjectManager");
    }

    PrismServiceId  RASLogLocalObjectManager::getPrismServiceId()    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *RASLogLocalObjectManager::createMessageInstance(
                                             const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        trace (TRACE_LEVEL_INFO,
        "RASLogLocalObjectManager::createMessageInstance: Entering...");

        switch (operationCode)
        {

            case RASLOGSHOW :
                pPrismMessage = new RASLogShowMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *RASLogLocalObjectManager::createManagedObjectInstance(
                            const string &managedClassName)
    {
        return NULL;
    }

    void  RASLogLocalObjectManager::RASLogShowMessageHandler(
                                    RASLogShowMessage*pRASLogShowMessage)
    {
        const char * const pFname = "RASLogLocalObjectManager::RASLogShowMessageHandler";
        trace (TRACE_LEVEL_INFO, string(pFname) + ": Entering...");

        UI32 i, j, tmp, num;
        int rc;
		int index_app = -1;
        //pRASLogShowMessage->setSwitchid("1"); // TBD

        raslog_return_msgs_t *pMsgs = NULL;
        raslog_filter_t filter;

        bzero(&filter, sizeof (filter));
        filter.severity    = RASLOG_SEVERITY_MAX;
        filter.type        = RASLOG_EXT_MSGS;
        filter.sw_instance = -1;

        pMsgs = (raslog_return_msgs_t *) calloc(1,
                                         sizeof (raslog_return_msgs_t));
        if (pMsgs == NULL)
        {
            trace (TRACE_LEVEL_ERROR, string(pFname) + ": calloc failed for " +
                   sizeof(raslog_return_msgs_t) + "bytes." );
            // TBD
            pRASLogShowMessage->setCompletionStatus (WRC_ENUM_RASReturnCodes);
            reply (pRASLogShowMessage);
            return;
        }

        /* get the error messages */
        rc = raslogm_get_log_app(pMsgs, filter, index_app);

        if (rc < 0)
        {
            trace (TRACE_LEVEL_ERROR, string(pFname) +
                   ": Corrupted error log.");
            free(pMsgs);
            pMsgs = NULL;
            // TBD
            pRASLogShowMessage->setCompletionStatus (WRC_ENUM_RASReturnCodes);
            reply (pRASLogShowMessage);
            return;
        }

        j = pMsgs->msg_num;
        if ( j <= 0 ) {
            num = 0; 
            trace (TRACE_LEVEL_INFO, string(pFname) + ": No messages");
        } else {
            /*
             * Select the minimum value from available events and requesting
             * number of events.
             *
             * Take 0 (zero) as all available entries.
             */
            tmp = (pRASLogShowMessage->getNumber_of_latest_events() == 0) ?
                    RASLOG_MSGS_PER_TYPE * RASLOG_NUM_TYPES :
			pRASLogShowMessage->getNumber_of_latest_events() - 1;
            num = (j < tmp) ? j : tmp;
        }

        pRASLogShowMessage->setNumber_of_entries(num);

        j -= num;
        for (i = 0; i < num; i++, j++)
        {
            netconfEvent_t evt;
            void *pData = (void *) &(pMsgs->msgs[j]);
            bzero(&evt, sizeof(evt));
            strncpy(evt.id,
             pRASLogShowMessage->getMessageId(pData).c_str(), sizeof(evt.id));
            evt.extSeqNum = pRASLogShowMessage->getIndex(pData);
            strncpy(evt.timeInfo,
                    pRASLogShowMessage->getDateAndTimeInfo(pData).c_str(),
                    sizeof(evt.timeInfo));
            evt.severity = pRASLogShowMessage->getSeverity(pData);
			evt.log_type = pRASLogShowMessage->getLogType(pData);
            strncpy(evt.descr,
             pRASLogShowMessage->getMessage(pData).c_str(), sizeof(evt.descr));
            strncpy(evt.name,
                    pRASLogShowMessage->getSwitchOrChassisName(pData),
                    sizeof(evt.name));
            evt.flag = pRASLogShowMessage->getMessageFlag(pData);

            // Add the recent RASLog event entry into buffer
            pRASLogShowMessage->transferOutputBuffer(&evt, sizeof(evt), i);
        }

        if (pMsgs)
        {
            free(pMsgs);
            pMsgs = NULL;
        }

        pRASLogShowMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pRASLogShowMessage);
}

void  RASLocalObjectManager::RASLCCLIMessageHandler(RASLCCLIMsg *pRASLCCLIMsg)
{
    trace (TRACE_LEVEL_DEBUG, " RASLocalOM::RASLCCLIMessageHandler for:lccli slot all xxx");
    
    // setup the sequence 
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&RASLocalObjectManager::RASLCCLISendToClients),
        reinterpret_cast<PrismLinearSequencerStep> (&RASLocalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&RASLocalObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pRASLocalMessagingContext = new PrismLinearSequencerContext (pRASLCCLIMsg, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRASLocalMessagingContext->holdAll ();
    pRASLocalMessagingContext->start ();
}

void RASLocalObjectManager::RASLCCLISendToClients(PrismLinearSequencerContext *pRASLocalMessagingContext)
{
    RASLCCLIMsg *pRASLCCLIMsg = reinterpret_cast<RASLCCLIMsg *> (pRASLocalMessagingContext->getPPrismMessage ());
    trace (TRACE_LEVEL_INFO, "RASLocalObjectManager::RASLCCLISendToClients: CLI=" + pRASLCCLIMsg->getArg());

    if (pRASLCCLIMsg->getArg().compare(0, 6, "ASSERT") == 0) {
	trace (TRACE_LEVEL_INFO, string ("Test WaveAssert to generate raslog message"));
	prismAssert (false, __FILE__, __LINE__);
    }

    LCCLIMsg *pLCCLIMsg = new LCCLIMsg (pRASLCCLIMsg->getArg(), LCCLI_MSG);

    WaveSendToClientsContext *pWaveSendToClientsContext = new WaveSendToClientsContext (this, reinterpret_cast<PrismAsynchronousCallback> (&RASLocalObjectManager::RASCLISendToClientsCallback), pRASLocalMessagingContext, "RAS");

    pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (pLCCLIMsg);

    sendToWaveClients (pWaveSendToClientsContext);
}

void RASLocalObjectManager::RASCLISendToClientsCallback (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::RASCLISendToClientsCallback"));  

    ResourceId   sendToClientsCompletionStatus = pWaveSendToClientsContext->getCompletionStatus ();
    PrismLinearSequencerContext *pRASLocalMessagingContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClientsContext->getPCallerContext ());

    prismAssert (NULL != pRASLocalMessagingContext, __FILE__, __LINE__);

    RASLCCLIMsg *pRASLCCLIMsg = reinterpret_cast<RASLCCLIMsg *> (pRASLocalMessagingContext->getPPrismMessage ());

    vector<SI32>              instancesToSendToForPhase1 = pWaveSendToClientsContext->getInstancesToSendToForPhase1 ();
    UI32                      numberOfInstancesToSendTo = instancesToSendToForPhase1.size ();
    UI32 i = 0;
    for (i = 0; i < numberOfInstancesToSendTo; i++)
    {
        WaveMessageStatus sendStatus = pWaveSendToClientsContext->getSendStatusForPhase1 (instancesToSendToForPhase1[i]);

        if (WAVE_MESSAGE_SUCCESS == sendStatus) {
            void *data;
            lccli_out_msg *omsg;
            UI32 size = 0;
 	    LCCLIMsg *pLCCLIMsg = reinterpret_cast<LCCLIMsg *> ( pWaveSendToClientsContext->getResultingMessageForPhase1 (instancesToSendToForPhase1[i]));

	    pLCCLIMsg->getDuplicateInterfaceOutputBuffer(LCCLI_MSG, size, data);

            if (size == 0) {
                trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::RASCLISendToClientsCallback instnace=") + instancesToSendToForPhase1[i] + string ("data len=0"));
		continue;
	    }

            omsg = (lccli_out_msg *)data;
            if ((omsg == 0) || (omsg->out_buf_len == 0)) {
                trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::RASCLISendToClientsCallback instnace=") + instancesToSendToForPhase1[i] + string ("data len=0"));
		continue;
	    }

	    // data = ((unsigned char *)data) + sizeof(lccli_out_msg_t);
            trace (TRACE_LEVEL_INFO, string ("RASLocalObjectManager::RASCLISendToClientsCallback instnace=") + instancesToSendToForPhase1[i] + string (" data len=") + omsg->out_buf_len);
	    // const char *dataPtr =  (const char *)((unsigned char *)data + sizeof(lccli_out_msg_t));

            pRASLCCLIMsg->addOutData(string (">>>>> Instance ") + instancesToSendToForPhase1[i] + string (" data <<<<<<\n"));
	    pRASLCCLIMsg->addOutData((const char *)((unsigned char *)data + sizeof(lccli_out_msg_t)));
	}
    }

    delete (pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ());
    delete pWaveSendToClientsContext;

    pRASLocalMessagingContext->executeNextStep (sendToClientsCompletionStatus);
    }

} // namespace DcmNs
