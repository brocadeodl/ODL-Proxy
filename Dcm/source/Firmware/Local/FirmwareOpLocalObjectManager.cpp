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
 *   Author : lhu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "Firmware/Global/NodeAutoUpgradeManagedObject.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Firmware/Local/FirmwareOpLocalObjectManager.h"
#include "Firmware/Local/FirmwareOpActionMessage.h"
#include "Firmware/Local/FirmwareOpTypes.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "Firmware/Local/FirmwareOpLocalThread.h"
#include "Firmware/Global/FirmwareToolKit.h"
#include "AAA/Global/AAACrypto.h"
#include <string>
#include "raslog/raslog.h"
#include "raslog/raslog_sulb.h"
#include "errno.h"
#include "hasm/hasm.h"
#include "hasm/sus.h"
#include "fabos/version.h"
#include "raslog/raslog_dad.h"
extern "C" {
#include <config/config_sw.h>
#include <config/resource.h>
#include <ha/ham_api.h>
#include <ha/pdm_api.h>
}

namespace DcmNs
{

#define PROTO_USB       0
#define PROTO_FTP       1
#define PROTO_SCP       2
#define PROTO_SFTP      3

    FirmwareOpLocalObjectManager::FirmwareOpLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FIRMWAREOPACTION, reinterpret_cast<PrismMessageHandler> (&FirmwareOpLocalObjectManager::FirmwareOpActionMessageHandler));
        addOperationMap (NETCONF_FWDL, reinterpret_cast<PrismMessageHandler> (&FirmwareOpLocalObjectManager::NetconfFirmwareDownloadMessageHandler));
        addOperationMap (NODEAUTOUPGRADE, reinterpret_cast<PrismMessageHandler> (&FirmwareOpLocalObjectManager::FirmwareOpAutoUpgradeMessageHandler));
		dadTimerHandle = dadState = 0;
    }

    FirmwareOpLocalObjectManager::~FirmwareOpLocalObjectManager ()
    {
    }

    FirmwareOpLocalObjectManager  *FirmwareOpLocalObjectManager::getInstance()
    {
        static FirmwareOpLocalObjectManager *pFirmwareOpLocalObjectManager = new FirmwareOpLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFirmwareOpLocalObjectManager, __FILE__, __LINE__);

        return (pFirmwareOpLocalObjectManager);
    }

    string  FirmwareOpLocalObjectManager::getClassName()
    {
        return ("Firmware Operation Local Manager");
    }

    PrismServiceId  FirmwareOpLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

	void FirmwareOpLocalObjectManager::domainValidEventHandler (const VcsNodeDomainValidEvent *&pEvent)
	{
    #if 0
		system("date >> /var/log/dafd.log");
		FILE *fp=fopen("/var/log/dafd.log", "a");
		fprintf(fp, "domainid : %d,%d\n", pEvent->getDomainId(), pEvent->getPrincipalDomainId());
		fclose(fp);
    #endif
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}


	void FirmwareOpLocalObjectManager::syncDumpStartEventHandler(const VcsNodeFabosSyncStartEvent* &pEvent)
	{
		if (dadState == 1) {
			dadState = 2;		
			dhcpUpdateDadState(dadState);
		}
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	void FirmwareOpLocalObjectManager::configReplayCompletedEventHandler(const ConfigReplayCompletedEvent* &pEvent)
	{

		if (dadState == 1) {
			dadState = 5; 		
			dhcpUpdateDadState(dadState);
		}
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	#define TIMER_INTERVAL (1000) /* 1 seconds */
	#define SYNC_TIMEOUT (300) /* 60*5 = 300 seconds */
	#define STARTUP_TIMEOUT (5400) /* = 540*10 seconds same as reboot timeout for firwmare commit */
	#define TOUCH_DAFD "echo pass > /tmp/dafd"
	void FirmwareOpLocalObjectManager::dhcpAutomaticFirmwareDownload(void)
	{
		int fwdl_state = sus_fwdl_state(); /* last firmwaredownload status */
		const char *vcsmode;
		int	vcsid, rbridgeid;
		plist_hdr_t fwheader;
		int fwver_offset;
		ResourceId distmode = DcmToolKit::getThisVcsNodeClusterType();
		int mode = 0; /* 0 - pizza box, 1 - single MM, 2 - dual MM*/;
		int rc = 0;
		
		if (access("/tmp/dafd", F_OK) != -1)
			return;

		system(TOUCH_DAFD);

		/* check chassis or not */
		if (ham_is_slotbased()) {
			if (ham_is_dual()) {
				if (ham_is_active()) {
					mode = 2; /* dualMM active */
				} else {
					system("touch /tmp/dualMMbad");
					return; /* dualMM standby or ha failover */
				}
			} else
				mode = 1; /* singleMM */
		}
#if 0
		/* check MC auto-upgrade enabled or not */
		if (distmode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) {
			WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
			NodeAutoUpgradeManagedObject *pUpgradeManagedObject = NULL;
			vector<WaveManagedObject *> *pResults = NULL;
			synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(NodeAutoUpgradeManagedObject::getClassName());
			if (synchronousQueryContext)
				pResults = querySynchronously (synchronousQueryContext);
			if (pResults && pResults->size() > 0)
				pUpgradeManagedObject = dynamic_cast<NodeAutoUpgradeManagedObject *> ((*pResults)[0]);
			if (pUpgradeManagedObject)
				rc = (pUpgradeManagedObject->getStatus() == true);
			else
				rc = 0;
			if (synchronousQueryContext)
				delete synchronousQueryContext;
			if (pResults)
				delete pResults;	
			if (rc) {
				fwdl_state = 10;
			}
		}
#endif
		system("echo `date` >> /var/log/dad.log");
		char cmd[100] = "[ -x /fabos/libexec/dhcp_fwdl ] && /fabos/libexec/dhcp_fwdl "; /* space for para */
		switch(fwdl_state) {
			case 0:
			case 2:
				/* check DAFD enabled on DHCP server or not */
				if (system("[ -f /tmp/dhcp_zerocfg ] && grep -q ^ftp /tmp/dhcp_zerocfg")) {
					system("echo `date` >> /var/log/dad.log");
					system("echo DHCP Auto-deployment failed during DHCP process. >> /var/log/dad.log");
					raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1302);
					rc = -1;
					break;
				}

				/* check mode */
				vcsmode = ((distmode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT ) ? "MC" : ((distmode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC) ? "FC" : "SA"));
				vcsid = (vcsmode ? DcmToolKit::getVcsId() : 0);
				rbridgeid = (vcsmode ? DcmToolKit::getThisVcsNodeDomainId() : 0);
				fwver_offset = (char *)(&fwheader.plh_release.r_version) - (char *)(&fwheader);
				sprintf(&cmd[strlen(cmd)], "%d %s %d %d %d %d", mode, vcsmode, vcsid, rbridgeid, fwver_offset, (int)sizeof(r_version_t));
				rc = system(cmd) >> 8;
				switch (rc) {
					case 0:
						raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1300);
						break;
					case 6:
						raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1308);
						system("echo DHCP Auto-deployment failed due to same firmware. >> /var/log/dad.log");
						break;
					default:
						raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1306);
						system("echo DHCP Auto-deployment failed in sanity check. >> /var/log/dad.log");
				}
				break;
			case 1:
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1303);
				rc = -2;
				break;
#if 0			
			case 10:
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1307);
				system("echo `date`, DHCP Auto-deployment failed due to auto-upgrade on>> /var/log/dafd.log");
				rc = -1;
				break;
#endif
			default:
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1304);
				system("echo DHCP Auto-deployment failed due to last firmware session failed. >> /var/log/dad.log");
				rc = -1;
		}
		if (rc) {
			system("echo `date`, DHCP Auto-deployment failed >> /var/log/dafd.log");
		} else {
			system("echo `date`, DHCP Auto-deployment started firmwaredownload >> /var/log/dafd.log");
			system("echo DHCP Auto-deployment started firmwaredownload. >> /var/log/dad.log");
			dadState = 6;			
		}
	}
	
	void FirmwareOpLocalObjectManager::dhcpDeleteTimer(void)
	{
	    if (dadTimerHandle)
			deleteTimer (dadTimerHandle);
		dadTimerHandle = 0;
		return;
	}

	void FirmwareOpLocalObjectManager::dhcpUpdateDadState(int dadStatus)
	{
		int status = dadStatus;
		char dadStateStr[80];
		int rc = -1;
		const char *fargs[3] = {"/var/log/dad.log", "/var/log/dafd.log", NULL};
	
		sprintf(dadStateStr, "echo `date`, update DAD state to %d... >> /var/log/dafd.log", status);
		system(dadStateStr);
			
		rc = configSet("dafd.status", ResourceInt, &status);
		if (rc)
			system("echo `date`, !!!DAD state configSet failed  ... >> /var/log/dafd.log");
		rc = configUpdate();
		if (rc)
			system("echo `date`, !!!DAD state configUpdate failed  ... >> /var/log/dafd.log");
		configSave((char**)fargs);
		return;
	}
	
	void FirmwareOpLocalObjectManager::dadTimerHandler (TimerHandle &timerHandle)
	{
		static unsigned int timeout = 0;
		int					rc = -1;
		char			 	dadStateStr[80] = "";		
		int 				status = 0;
		const char 			*fargs[2] = {"/var/log/dafd.log", NULL};
		
		dhcpDeleteTimer();
		switch (dadState) {
		case 0:
			configInit();			
			if(configGet("dafd.status", ResourceInt, &status) < 0) {
				status = 0;
				system("echo `date`, dadTimerHandler: dadState 0: configGet failed >> /var/log/dafd.log");
				configGet("dafd.status", ResourceInt, &status);
			}
			sprintf(dadStateStr, "echo `date`, dadTimerHandler: dadState 0:status is %d >> /var/log/dafd.log", status);
			system(dadStateStr);
			if (status == 0) {
					system("echo `date`, DHCP Auto-deployment is disabled >> /var/log/dafd.log");
			} else if (status == 1) {
				system("echo `date`, DAD started timer in init for dad enable >> /var/log/dafd.log");
				startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
				dadState = 1;
			} else if (status == 6) {
				system("echo `date`, DAD started timer in init for firmwarecommit>> /var/log/dafd.log");
				startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
				dadState = 6;
			} else {
				sprintf(dadStateStr, "echo `date`, dadTimerHandler: dadState 0:configGet unexpected state %d >> /var/log/dafd.log", dadState);
				system(dadStateStr);
				dadState = 0;
				dhcpUpdateDadState(dadState);
				system("echo `date`>> /var/log/dad.log");
				system("echo DHCP Auto-deployment failed due to unexpected state.>> /var/log/dad.log");
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1311);
			}
			configSave((char**)fargs);
			break;
		case 1: /* start up */
			if (timeout++ < STARTUP_TIMEOUT) {
				startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
			} else {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1311);
				system("echo `date`, waiting for dad trigger time out ... >> /var/log/dafd.log");
				system("echo `date`>> /var/log/dad.log");
				system("echo `date`, DHCP Auto-deployment failed to start.>> /var/log/dad.log");
				dadState = 0;
				dhcpUpdateDadState(dadState);
			}
			break;
		case 2: /* dual-MM sync dump start */
			system("echo `date`, configreplay finished >> /var/log/dafd.log");
			dadState = 3;	
			dhcpUpdateDadState(dadState);
			startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
			break;
		case 3: /* wait for dual-MM in-sync */
			startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
			system("echo `date`, start polling sync dump.. >> /var/log/dafd.log");
			timeout = 0;
			dadState = 4;			
			dhcpUpdateDadState(dadState);
			break;
		case 4: /* dual-MM in sync */
			if(++timeout < SYNC_TIMEOUT) {
				startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
				if (ham_get_state() & HAM_STATE_SYNCHRONIZED) {
					system("echo `date`, dual CP sync dump complete >> /var/log/dafd.log");
					dadState = 5;				
					dhcpUpdateDadState(dadState);
				}					
			} else {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1301);
				system("echo `date`, dual CP sync dump timeout ... >> /var/log/dafd.log");
				system("echo `date`>> /var/log/dad.log");
				system("echo DHCP Auto-deployment failed to start in dual MM.>> /var/log/dad.log");
				dadState = 0;
				dhcpUpdateDadState(dadState);
			}
			break;
		case 5: /* trigger dad */
			system("echo `date`, trigger dad >> /var/log/dafd.log");			
			dhcpAutomaticFirmwareDownload();
			if (dadState == 6) {	
				dhcpUpdateDadState(dadState);
				timeout = 0;
				startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
				system("echo `date`, start timer polling firmwaredownload status... >> /var/log/dafd.log");
			} else {
				dadState = 0;
				dhcpUpdateDadState(dadState);
			}
			break;
		case 6: /* wait for firmwaredownload commit */
			if (++timeout < STARTUP_TIMEOUT) {
				rc = sus_fwdl_state();
				if ((rc == 0) || (rc == 3)) {
					if (rc == 0) {
						system("echo `date`, firmwaredownload trigged by DAD succeeded ... >> /var/log/dafd.log");
						system("echo `date`>> /var/log/dad.log");
						system("echo DHCP Auto-deployment succeeded>> /var/log/dad.log");
						raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1310);
					}
					else {
						system("echo `date`, firmwaredownload trigged by DAD failed ... >> /var/log/dafd.log");
						system("echo `date`>> /var/log/dad.log");
						system("echo DHCP Auto-deployment firmwaredownload failed.>> /var/log/dad.log");
						raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1311);
					}
					dadState = 0;
					dhcpUpdateDadState(dadState);
				}
				else {
					startTimer (dadTimerHandle, TIMER_INTERVAL, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
				}					
			} else {
				raslog_ext(__FILE__, __FUNCTION__, __LINE__,RASLOG_OID_INVALID, RASLOG_FRCLOG, DAD_1311);
				system("echo `date`, waiting for dad triggered dad fwdl commit time out ... >> /var/log/dafd.log");
				system("echo `date`>> /var/log/dad.log");
				system("echo `date`, DHCP Auto-deployment failed due to fwdl commit timeout.>> /var/log/dad.log");
				dadState = 0;
				dhcpUpdateDadState(dadState);
			}
			break;
		default:
			system("echo `date`, error: unexpected dadState, delete timer ... >> /var/log/dafd.log");
			dadState = 0;
			dhcpUpdateDadState(dadState);
		}
	}

	void FirmwareOpLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType()) {
			//system("[ -f /etc/zerocfg.delay ] && sleep $[`cat /etc/zerocfg.delay`]");
			//system("[ -f /mnt/etc/zerocfg.delay ] && sleep $[`cat /mnt/etc/zerocfg.delay`]");
			/* wait for principal node come up first */
			listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_VALID, reinterpret_cast<PrismEventHandler> (&FirmwareOpLocalObjectManager::domainValidEventHandler));
		}

		dadState = 0;
		if (ham_is_dual()) {
			if (ham_is_active()) {
				listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYNC_START, reinterpret_cast<PrismEventHandler> (&FirmwareOpLocalObjectManager::syncDumpStartEventHandler));
				startTimer (dadTimerHandle, 40000, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
			}
			else
				system(TOUCH_DAFD);
		} else {					
			listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&FirmwareOpLocalObjectManager::configReplayCompletedEventHandler));
			startTimer (dadTimerHandle, 40000, reinterpret_cast<PrismTimerExpirationHandler> (&FirmwareOpLocalObjectManager::dadTimerHandler));
		}

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

    PrismMessage  *FirmwareOpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FIRMWAREOPACTION :
                pPrismMessage = new FirmwareOpActionMessage();
                break;
            case NETCONF_FWDL:
                pPrismMessage = new NetconfFirmwareDownloadMessage();
                break;
            case NODEAUTOUPGRADE :
                pPrismMessage = new FirmwareOpAutoUpgradeMessage();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FirmwareOpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  FirmwareOpLocalObjectManager::FirmwareOpActionMessageHandler( FirmwareOpActionMessage *pFirmwareOpActionMessage)
    {
		NetconfFirmwareDownloadMessage *pNetconfMessage = NULL;

		FirmwareOpLocalThread *pFirmwareOpLocalThread = new FirmwareOpLocalThread(m_pWaveObjectManager,	pFirmwareOpActionMessage, pNetconfMessage, FIRMWAREOPACTION);
		WaveThreadStatus threadStatus = pFirmwareOpLocalThread->run();

		if (WAVE_THREAD_SUCCESS != threadStatus) {
			trace(TRACE_LEVEL_ERROR, "FirmwareOpLocalOM::FirmwareOpAction"
				"MessageHandler: Local thread failed with status: " +
				FrameworkToolKit::localize(threadStatus));
		} else {
			trace(TRACE_LEVEL_SUCCESS, "FirmwareOpLocalOM::FirmwareOpAction"
				"MessageHandler: Local thread started successfully");
		}
    }

    void
    FirmwareOpLocalObjectManager::NetconfFirmwareDownloadMessageHandler(NetconfFirmwareDownloadMessage *pMessage)
    {
		FirmwareOpActionMessage *pFirmwareOpActionMessage = NULL;

		FirmwareOpLocalThread *pFirmwareOpLocalThread = new FirmwareOpLocalThread(m_pWaveObjectManager, pFirmwareOpActionMessage, pMessage, NETCONF_FWDL);
		WaveThreadStatus threadStatus = pFirmwareOpLocalThread->run();

		if (WAVE_THREAD_SUCCESS != threadStatus) {
			trace(TRACE_LEVEL_ERROR, "FirmwareOpLocalOM::NetconfFirmware"
				"Download MessageHandler: Local thread failed with status: " +
				FrameworkToolKit::localize(threadStatus));
		} else {
			trace(TRACE_LEVEL_SUCCESS, "FirmwareOpLocalOM::NetconfFirmware"
				"DownloadMessageHandler: Local thread started successfully");
		}

    }


void FirmwareOpLocalObjectManager::FirmwareOpAutoUpgradeMessageHandler(FirmwareOpAutoUpgradeMessage *pFirmwareOpAutoUpgradeMessage)
{

	trace (TRACE_LEVEL_INFO, "FirmwareOpAutoUpgradeMessageHandler enterng..");

	FILE		*cmd_file;
	int			rc, fwdl_state;
	UI32		proto;
	char		cmd_output[512], ver_file[256];
	string		fwdlCommand = "", sanityCommand = "";
	string		fw_version = "", protocol ="", host = "";
	string		username = "", path = "", password = "";
	fwversion_t pver;

	WaveClientSessionContext waveClientSessionContext =
				pFirmwareOpAutoUpgradeMessage->getWaveClientSessionContext();

	NodeAutoUpgradeConfiguration *pUpgradeConfigData =
						pFirmwareOpAutoUpgradeMessage->getUpgradeConfigData();

	pFirmwareOpAutoUpgradeMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

	fwdl_state = sus_fwdl_state();

	if (fwdl_state == 1) {
		trace(TRACE_LEVEL_INFO, string("FirmwareiOpAutoUpgradeMessageHandler: "
			"FWDL already in progress on this switch, auto-upgrade skipped."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_IN_PROGRESS);
		reply(pFirmwareOpAutoUpgradeMessage);
		return;
	}

	
	if (fwdl_state == 2) {
		trace(TRACE_LEVEL_INFO, string("FirmwareiOpAutoUpgradeMessageHandler: "
			"Firmware download completed on this switch."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_DOWNLOADED);
		reply(pFirmwareOpAutoUpgradeMessage);
		return;
	}

	if (fwdl_state == 3) {
		trace(TRACE_LEVEL_INFO, string("FirmwareiOpAutoUpgradeMessageHandler: "
			"Firmware download failed on this switch."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_FAILURE);
		reply(pFirmwareOpAutoUpgradeMessage);
		return;
	}

	if (pUpgradeConfigData->getStatus() != true) {
		trace(TRACE_LEVEL_INFO, string("FirmwareiOpAutoUpgradeMessageHandler: "
			"Node auto-upgrade feature disabled, no upgrade done."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_DISABLED);
		reply(pFirmwareOpAutoUpgradeMessage);
		return;
	}

	if (pUpgradeConfigData->getFlags() & NA_DIFF_VERSION) {
		trace(TRACE_LEVEL_INFO, string("FirmwareiOpAutoUpgradeMessageHandler: "
			"Principal version is different from the configured version"));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_PRIN_VER_DFIFF);
		reply(pFirmwareOpAutoUpgradeMessage);
		return;
	}
	strcpy(ver_file, PRI_VERSION_FILE);
	memset(&pver, 0, sizeof (fwversion_t));
	if (version_get((const char*) ver_file, (char*) "base", &pver)) {
		trace(TRACE_LEVEL_INFO, string("FirmwareOpAutoUpgradeMessageHandler:: "
									"failed to retrieve version info."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_FAILURE);
		reply (pFirmwareOpAutoUpgradeMessage);
		return;
	}

	fw_version = pUpgradeConfigData->getFirmwareVersion();

	if (strcmp(pver.fw_verstr, fw_version.c_str()) == 0) {
		trace(TRACE_LEVEL_INFO, string("FirmwareOpAutoUpgradeMessageHandler: "
			"Firmware versions identical, auto-upgrade not required"));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_NOT_REQUIRED);
		reply (pFirmwareOpAutoUpgradeMessage);
		return;
	}

	proto		= pUpgradeConfigData->getProtocol();
	host		= pUpgradeConfigData->getHost();
	username	= pUpgradeConfigData->getUsername();
	path		= pUpgradeConfigData->getPath();
	password	= pUpgradeConfigData->getPassword();
	if (password != "")
		password = AAACrypto::decryptPassword(password);

	if (ham_is_dual() == 0) {
		fwdlCommand = "/fabos/sbin/firmwaredownload -sVb ";
	} else {
		fwdlCommand = "/fabos/sbin/firmwaredownload -V ";
	}

	if (proto == PROTO_USB) {
		fwdlCommand = fwdlCommand + "-U " + path;
		sanityCommand = fwdlCommand + "-S " + "-U " + path;
	} else {
		if (proto == PROTO_FTP)
			protocol = "-p ftp";
		else if (proto == PROTO_SCP)
			protocol = "-p scp";
		else if (proto == PROTO_SFTP)
			protocol = "-p sftp";

		fwdlCommand = fwdlCommand + protocol + " " + host + "," +
						username + "," + path + "," + password;
		sanityCommand = fwdlCommand + "-S " + protocol + " " + host + "," +
						username + "," + path + "," + password;
	}

	trace(TRACE_LEVEL_INFO, string("FirmwareOPAutoUpgradeMessageHandler: get "
			"command: " + fwdlCommand));

	printfToWaveClientSession (waveClientSessionContext, "Performing"
		" system sanity check...\n");
	rc = system(sanityCommand.c_str());
	rc = WEXITSTATUS(rc);
	if (rc == 255) {
		/* -1 is translated to (255<<8) by system() */
		printfToWaveClientSession (waveClientSessionContext, "System sanity"
			"check failed.\n");
		trace(TRACE_LEVEL_INFO, string("FirmwareOpAutoUpgradeMessageHandler:: "
			"Firmware sanity check failed, auto-upgrade not done."));
		pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_SANITY_FAILED);
		reply (pFirmwareOpAutoUpgradeMessage);
		return;
	}

	trace(TRACE_LEVEL_INFO, string("FirmwareOpAutoUpgradeMessageHandler:: "
		"Node auto-upgrade started successfully on this node."));
	pFirmwareOpAutoUpgradeMessage->setStatus(NA_STS_START_SUCCESS);
	reply (pFirmwareOpAutoUpgradeMessage);

	trace(TRACE_LEVEL_INFO, string("FirmwareOpAutoUpgradeMessageHandler: "
		"Msg reply sent to VcsFabricLocal, continue with FWDL."));

		if ((cmd_file = popen((char *)fwdlCommand.data(), "r")) != NULL) {

			while (fgets(cmd_output, 512, cmd_file)) {
				trace(TRACE_LEVEL_DEBUG, string("FOAUMessageHandler: In "
					"fgets: ") + cmd_output);
				printfToWaveClientSession(waveClientSessionContext, "%s",
					cmd_output);
			}

			if ((rc = pclose(cmd_file)) < 0) {

				trace(TRACE_LEVEL_INFO, string("FOAUMessageHandler: pclose"
					"failed - CMD = ") + fwdlCommand.data() + string(" rc=") +
					rc + string (" errno=") + -errno);
				printfToWaveClientSession(waveClientSessionContext, "%s",
					"Command Failed\n");
			}

		} else {

			trace(TRACE_LEVEL_INFO, string("FOAUMessageHandler: popen failed"
				"- CMD =  ") + fwdlCommand.data() + string (" errno=") +
				-errno);
			printfToWaveClientSession(waveClientSessionContext, "%s",
				"Command Failed\n");
		}

		trace(TRACE_LEVEL_INFO, "FOAUMessageHandler: firmware download"
			"command failed or the switch did not reboot.");
		trace(TRACE_LEVEL_DEBUG, "FirmwareOpAutoUpgradeMsgHandler exiting");

}

}
