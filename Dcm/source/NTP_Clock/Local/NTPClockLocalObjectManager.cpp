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
 *   Author : noku                                                     *
 **************************************************************************/

#include <sys/time.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

#include "DcmResourceIdEnums.h"

#include "NTP_Clock/Global/NTPServerManagedObject.h"
#include "NTP_Clock/Global/NTPKeyManagedObject.h"
// #include "NTP_Clock/Global/TimeZoneManagedObject.h"
#include "NTP_Clock/Local/NTPClockLocalObjectManager.h"
#include "NTP_Clock/Local/NTPClockSetSwitchClockMessage.h"
#include "NTP_Clock/Local/NTPClockSetSwitchZoneMessage.h"
#include "NTP_Clock/Local/NTPClockGetSwitchClockMessage.h"
#include "NTP_Clock/Local/NTPClockGetActiveServerMessage.h"
#include "NTP_Clock/Local/NTPClockSetServersMessage.h"
#include "NTP_Clock/Local/NTPClockSetServerKeyMessage.h"
#include "NTP_Clock/Local/NTPClockTypes.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"
#include "NTP_Clock/Global/timezone-list.h"

extern "C"
{
#include "ts/public.h"
}

namespace DcmNs
{

    NTPClockLocalObjectManager::NTPClockLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (NTPCLOCKSETSWITCHCLOCK, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler));
        addOperationMap (NTPCLOCKSETSWITCHZONE, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler));
        addOperationMap (NTPCLOCKGETSWITCHCLOCK, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockGetSwitchClockMessageHandler));
        addOperationMap (NTPCLOCKGETACTIVESERVER, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler));
        addOperationMap (NTPCLOCKSETSERVERS, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockSetServersMessageHandler));
        addOperationMap (NTPCLOCKSETSERVERKEY, reinterpret_cast<PrismMessageHandler> (&NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler));

         PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    NTPClockLocalObjectManager::~NTPClockLocalObjectManager ()
    {
    }

    NTPClockLocalObjectManager  *NTPClockLocalObjectManager::getInstance()
    {
        static NTPClockLocalObjectManager *pNTPClockLocalObjectManager = new NTPClockLocalObjectManager ();

        WaveNs::prismAssert (NULL != pNTPClockLocalObjectManager, __FILE__, __LINE__);

        return (pNTPClockLocalObjectManager);
    }

    string  NTPClockLocalObjectManager::getClassName()
    {
        return ("NTPClock");
    }

    PrismServiceId  NTPClockLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *NTPClockLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NTPCLOCKSETSWITCHCLOCK :
                pPrismMessage = new NTPClockSetSwitchClockMessage ();
                break;
            case NTPCLOCKSETSWITCHZONE :
                pPrismMessage = new NTPClockSetSwitchZoneMessage ();
                break;
            case NTPCLOCKGETSWITCHCLOCK :
                pPrismMessage = new NTPClockGetSwitchClockMessage ();
                break;
            case NTPCLOCKGETACTIVESERVER :
                pPrismMessage = new NTPClockGetActiveServerMessage ();
                break;
            case NTPCLOCKSETSERVERS :
                pPrismMessage = new NTPClockSetServersMessage ();
                break;
            case NTPCLOCKSETSERVERKEY :
                pPrismMessage = new NTPClockSetServerKeyMessage ();
                break;	
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NTPClockLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId NTPClockLocalObjectManager::translateTsStatusCode(const int &tsStatus)
    {
        switch (tsStatus)
        {
            case TS_OK:
                return WAVE_MESSAGE_SUCCESS;
                break;
            case TS_ERR_CONFIG_GET:
                return NTPCLOCK_TIME_ERROR;
                break;
            case TS_ERR_DUP_LIST:
                return NTPCLOCK_SERVER_ALREADY_IN_LIST;
                break;
            case TS_ERR_INVALID_NTPSVR:
                return NTPCLOCK_INVALID_SERVER;
                break;
            case TS_ERR_MAX_NTP_SVRS:
            case TS_ERR_MAX_CLKSVRS_LT:
                return NTPCLOCK_MAX_SERVERS;
                break;
            case TS_ERR_NULLPTR:
                return NTPCLOCK_NULLPTR_ERROR;
                break;
            case TS_ERR_LOW_MEMORY:
                return NTPCLOCK_LOWMEM_ERROR;
                break;
            case TS_ERR_MAX_CLKSVR_LEN_LT:
                return NTPCLOCK_MAX_SVRLEN_ERROR;
                break;
            case TS_ERR_IPC:
                return NTPCLOCK_IPC_ERROR;
                break;
            case TS_ERR_VERIFY_FAILED:
                return NTPCLOCK_VERIFY_ERROR;
                break;
            case TS_ERR_INVALID_CHAR:
                return NTPCLOCK_INVALID_CHAR;
                break;
            case TS_ERR_INVALID_ENTRIES:
                return NTPCLOCK_INVALID_NTP_ERROR;
                break;
            case TS_ERR_SFOS_ENABLED:
            case TS_ERR_SFOS_IPV6_DNS:
                return NTPCLOCK_SFOS_ERROR;
                break;
            case TS_ERR_NTP_QUERY_FAILED:
                return NTPCLOCK_QUERY_FAILED;
                break;
            case TS_ERR_LOCL_LIST:
                return NTPCLOCK_LOCL_ERROR;
                break;
            case TS_ERR_CONFIG_UPDT:
            case TS_ERR_CONFIG_SET:
                return NTPCLOCK_CONFIG_SET_ERROR;
                break;
            case TS_ERR_DATA:
                return NTPCLOCK_KEY_READ_ERROR;
                break;
            case TS_ERR_GENERAL:
                return NTPCLOCK_KEY_WRITE_ERROR;
                break;
            default:
                return NTPCLOCK_SETSERVERS_ERROR;
                break;
        }
    }

    ResourceId NTPClockLocalObjectManager::pushBackend()
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		// TR000317625: removing timezone managed object
		// push timezone and NTP server list to backend
		// string timezoneStr;
		string serverlist, server, KeyValue;
		char *p_svrlist, *p_server, *p_KeyValue;
		int ts_status; 

		serverKeyInfo serverKey;

#if 0
		// TR000317625: removing timezone managed object
        // first push the timezone
        // query for TimeZoneManagedObject
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(TimeZoneManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if (NULL != pResults && 1 == pResults->size ())
        {
            // found. recast it and update it's value
            TimeZoneManagedObject *pZoneMO = dynamic_cast<TimeZoneManagedObject *> ((*pResults)[0]);
            timezoneStr = pZoneMO->getTimezonestring();
            delete pZoneMO;
        }
        else
        {
            // set to default
            timezoneStr = getDefaultTimezone();
        }

        if (NULL != pResults)
        {
            pResults->clear();
            delete pResults;
        }
        delete syncQuery;

        // tsSetSystemTZName returns void, so no error code handling
        tsSetSystemTZName((char *)timezoneStr.c_str());
#endif

        // second push the NTP server list
        WaveManagedObjectSynchronousQueryContext *syncQuery2 = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
        syncQuery2->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults2 = querySynchronously (syncQuery2);
        if ((NULL == pResults2) || (0 == pResults2->size ()))
        {
            // empty list. set to default
            serverlist = TS_DEFAULT_CLKSVR;
        }
        else
        { // accumulate list into string
            unsigned int i;
            for (i = 0; i < pResults2->size(); i++)
            {
                NTPServerManagedObject *pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults2)[i]);
                IpVxAddress tempIp = pMO->getServer();
                serverlist += tempIp.toString();
                // add token between ip addresses in list
                if ((i + 1) < pResults2->size())
                {
                    serverlist += TS_TOKEN_SEPERATOR;
                }
                delete (*pResults2)[i];
            }
        }

        if (NULL != pResults2)
        {
            delete pResults2;
        }
        delete syncQuery2;

        // set servers to backend
        p_svrlist = (char *)serverlist.c_str();
        ts_status = tsConfigureClockServer(p_svrlist, 1);         // ts_status = tsSetClockServer(serverlist.c_str());
        if (TS_OK == ts_status)
        {
            if (serverlist != TS_DEFAULT_CLKSVR) {

                WaveManagedObjectSynchronousQueryContext *syncQuery3 = new WaveManagedObjectSynchronousQueryContext(NTPKeyManagedObject::getClassName());
                syncQuery3->addOrderField("index", true);
                vector<WaveManagedObject *> *pResults3 = querySynchronously (syncQuery3);
                if ((NULL == pResults3) || (0 == pResults3->size ()))
                {
                    // empty list. nothing to do
                    trace (TRACE_LEVEL_DEBUG, string("NTPClockLocalObjectManager: Key list is empty"));
                }
                else
                {
                    WaveManagedObjectSynchronousQueryContext *syncQuery4 = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
                    syncQuery4->addOrderField("index", true);
                    vector<WaveManagedObject *> *pResults4 = querySynchronously (syncQuery4);
                    unsigned int i;
                    for (i = 0; i < pResults4->size(); i++)
                    {
                        NTPServerManagedObject *pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults4)[i]);
			            IpVxAddress tempIp = pMO->getServer();
                        server = tempIp.toString();
                        p_server = (char *)server.c_str();
                        strncpy(serverKey.server, p_server, sizeof(serverKey.server) - 1);

                        UI32 tempKey = pMO->getKey();
                        if (tempKey == 0)
                        {
                            serverKey.keyinfo.index = -1;
                            KeyValue = NTPSERVER_NO_KEY;
                            p_KeyValue = (char *)KeyValue.c_str();
                            serverKey.keyinfo.key = p_KeyValue;
                        }
                        else
                        {
           	                for (unsigned int j = 0; j < pResults3->size(); j++)
                            {
                                NTPKeyManagedObject *pKeyMO = dynamic_cast<NTPKeyManagedObject *> ((*pResults3)[j]);
                                UI32 Key_ID = pKeyMO->getKey_ID();
                                if (tempKey == Key_ID) 
                                {       
                                    serverKey.keyinfo.index = Key_ID;
                                    KeyValue = pKeyMO->getKey_Value();
                                    p_KeyValue = (char *)KeyValue.c_str();
                                    serverKey.keyinfo.key = p_KeyValue;
                                    break;
                                }
                             }
                        }
                         // only MD5;
                        serverKey.keyinfo.keyType = KEY_MD5;
                        // Synchronizing server and key with backend
                        ts_status  = tsSetClockServerAuthKey(1, &serverKey);
                        if (TS_OK == ts_status)
                        {
          	                trace (TRACE_LEVEL_DEBUG, "NTPClockLocalObjectManager::Setting Key OK.");
                        }
                        else
                        {
                            status = NTPCLOCK_KEY_READ_ERROR;
                            trace (TRACE_LEVEL_ERROR, "Synchronizing NTP config with backend failed for server:" + server);
                        }
                        delete (*pResults4)[i];
                    }
                    if (NULL != pResults4) {
                        delete pResults4;
                    }
                    delete syncQuery4;
                }
                if (NULL != pResults3)
                {
                    unsigned int k;
                    for (k = 0; k < pResults3->size(); k++)
                        delete (*pResults3)[k];
                    delete pResults3;
                }
                delete syncQuery3;
            }
            trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::boot OK.");
        }
        else
        {
            status = NTPCLOCK_SETSERVERS_ERROR;
        }

		if (WAVE_MESSAGE_SUCCESS != status)
		{
            trace (TRACE_LEVEL_ERROR, "NTPClockLocalObjectManager::pushBackend status" + FrameworkToolKit::localize (status));
			status = WAVE_MESSAGE_SUCCESS;
		}
		return status;
    }

    void NTPClockLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

        if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT != bootReason) &&
             (WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT != bootReason))
        { // only CONFIGURE needs to push data to backend.
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

		status = pushBackend();

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    void NTPClockLocalObjectManager::externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
    {
#define TSD_FSS_STAGE4 4
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        ResourceId svcType = pWaveAsynchronousContextForExternalStateSynchronization->getServiceType();
        UI32 fssStage = pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber();

        // do not need getServiceType() since TSD stage only in fcsw.svc
        if (FCSW != svcType || TSD_FSS_STAGE4 != fssStage)
        {
            pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
            pWaveAsynchronousContextForExternalStateSynchronization->callback ();
            return;
        }

        trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::externalStateSynchronization FCSW TSD_FSS_STAGE4");

        // TSD_FSS_STAGE4
        status = pushBackend();

        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();
    }

    void  NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler( NTPClockGetActiveServerMessage *pNTPClockGetActiveServerMessage)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int ts_status;
		char *p_svr = NULL;
		string svrStr;

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockGetActiveServerMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		// Programming Protocol Daemons goes here
		ts_status = tsGetClockServer(&p_svr);
		if (ts_status == TS_OK && p_svr)
		{
			svrStr = p_svr;
			if (0 == strcmp(TS_DEFAULT_CLKSVR, p_svr))
			{
				svrStr = TS_LOCALHOST_IPV4;
			}
			IpVxAddress *activeServer = new IpVxAddress (svrStr);
			pNTPClockGetActiveServerMessage->setIpAddr(*activeServer);
			trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler setting to " + activeServer->toString());
			delete activeServer;
			free (p_svr);
		}
		else
		{
			status = translateTsStatusCode(ts_status);
			trace (TRACE_LEVEL_ERROR, "NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler tsGetClockServer failed");
		}

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockGetActiveServerMessageHandler Exiting.");
		pNTPClockGetActiveServerMessage->setCompletionStatus(status);
		reply (pNTPClockGetActiveServerMessage);
    }

    void  NTPClockLocalObjectManager::NTPClockGetSwitchClockMessageHandler( NTPClockGetSwitchClockMessage *pNTPClockGetSwitchClockMessage)
    {
#define YEAR_OFFSET 1900
#define MONTH_OFFSET 1
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		char *p_zone = NULL;
		string			timeStr;
        struct timeval	localTimeval;
        struct tm		*p_localTm;
		DateTime		localDateTime;

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockGetSwitchClockMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockGetSwitchClockMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		if (0 == gettimeofday(&localTimeval, NULL))
		{
			p_localTm = localtime(&localTimeval.tv_sec);
			localDateTime.setAllValues(
				WaveNs::SI16(p_localTm->tm_year) + YEAR_OFFSET,
				WaveNs::UI8(p_localTm->tm_mon) + MONTH_OFFSET,
				WaveNs::UI8(p_localTm->tm_mday),WaveNs::UI8(p_localTm->tm_hour),
				WaveNs::UI8(p_localTm->tm_min),	WaveNs::UI8(p_localTm->tm_sec),
				WaveNs::UI32(0), WaveNs::SI8(0), WaveNs::SI8(0));

			pNTPClockGetSwitchClockMessage->setClockDateTime(localDateTime);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "NTPClockLocalObjectManager::NTPClockGetSwitchClockMessageHandler no gettimeofday.");
			status = NTPCLOCK_TIME_ERROR;
		}
		// getTsTimeZoneName only returns char *, so no error code handling
		getTsTimeZoneName(&p_zone);
		if (p_zone)
		{
			timeStr = p_zone;
		}
		else
		{
			// not set, use default
			timeStr = getDefaultTimezone();
		}
		pNTPClockGetSwitchClockMessage->setClockTimeZone(timeStr);
		free (p_zone);

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockGetSwitchClockMessageHandler Exit");
		pNTPClockGetSwitchClockMessage->setCompletionStatus(status);
		reply (pNTPClockGetSwitchClockMessage);
    }

    void  NTPClockLocalObjectManager::NTPClockSetServersMessageHandler( NTPClockSetServersMessage *pNTPClockSetServersMessage)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int ts_status;
		char *p_svrlist;
		string serverList;

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServersMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockSetServersMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		serverList = pNTPClockSetServersMessage->getIpAddrList();
		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServersMessageHandler setting to " + serverList);
		p_svrlist = (char *)serverList.c_str();
		ts_status = tsConfigureClockServer(p_svrlist, 1);
		if (TS_OK == ts_status)
		{
#ifdef NTP_LAST_CMD_SYNC_TEST
	        if ((pNTPClockSetServersMessage->getIsALastConfigReplay() ==false) &&
    	            (access( "/root/HA_Sec_Test", F_OK ) == 0)) {
	            trace (TRACE_LEVEL_DEBUG, "pNTPClockSetServersMessage getIsALastConfigReplay false");
    	        system ("/sbin/reboot -f");
	        }
#endif
			trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServersMessageHandler OK.");
		}
		else
		{
			status = translateTsStatusCode(ts_status);
			// in future, may send back current list
			// ts_status = tsGetClockServer(&p_svrlist);
			// serverList = p_svrlist;
			trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServersMessageHandler not set, current list " + serverList);
			// free (p_svrlist);
			// pNTPClockSetServersMessage->setIpAddrList(serverList);
		}

		pNTPClockSetServersMessage->setCompletionStatus(status);
		reply (pNTPClockSetServersMessage);
    }

    void  NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler( NTPClockSetSwitchClockMessage *pNTPClockSetSwitchClockMessage)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		DateTime		setDateTime;

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockSetSwitchClockMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		setDateTime = pNTPClockSetSwitchClockMessage->getClockDateTime();

		// Your configuration change code goes here
		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler setting date time " + setDateTime.toString());

		// Programming Protocol Daemons goes here
		char dateCmd[80];
		sprintf(dateCmd, "date %02d%02d%02d%02d%04d.%02d", setDateTime.getMonth(),
			setDateTime.getDay(), setDateTime.getHour(), setDateTime.getMin(),
			setDateTime.getYear(), setDateTime.getSec());
		trace (TRACE_LEVEL_INFO, dateCmd);
		// set the system clock and copy to the hardware clock
		if (0 != tsSetLocalClock(dateCmd))
		{
			trace (TRACE_LEVEL_ERROR, "NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler error saving to hwclock");
			status = NTPCLOCK_TIME_ERROR;
		}

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchClockMessageHandler Exit.");
		pNTPClockSetSwitchClockMessage->setCompletionStatus(status);
		reply (pNTPClockSetSwitchClockMessage);
    }

    void  NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler( NTPClockSetSwitchZoneMessage *pNTPClockSetSwitchZoneMessage)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string timeZone;

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockSetSwitchZoneMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		// set the local switch timezone (returns void)
		timeZone = pNTPClockSetSwitchZoneMessage->getTimezone();
		// trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler setting to " + timeZone);
		// tsSetSystemTZName returns void, so no error code handling
		tsSetSystemTZName((char *)timeZone.c_str());
        /* TR000370158: clock timezone not working, was introduced by the
           check-in of TR000362426 when they moved the tzset from the
           instantiation of DateTime object to only during system init.
           tzset updates the tzname local variable from either the TZ
           environment variable or /etc/localtime.  Since tzset was no
           longer being called, tzname becomes stale whenever the timezone
           gets changed.  tzname is used by localtime() function when
           getting the current clock time.  Calling tzset explicitly will
           update the tzname variable here, but changing it in the FOS
           will not update this.  Also, tzset here will only update Dcmd,
           while other system daemons may not update their timezones
           until reboot.
        */
        tzset();

		// get the timezone to compare with what was set
		char *p_zone = NULL;
		// getTsTimeZoneName only returns char *, so no error code handling
		getTsTimeZoneName(&p_zone);
		if (0 == strcmp(p_zone, timeZone.c_str()))
		{
			timeZone = p_zone;
			pNTPClockSetSwitchZoneMessage->setTimezone(timeZone);
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler error setting zone");
			status = NTPCLOCK_ZONE_ERROR;
		}
		free (p_zone);

		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetSwitchZoneMessageHandler Exit");
		pNTPClockSetSwitchZoneMessage->setCompletionStatus(status);
		reply (pNTPClockSetSwitchZoneMessage);
    }

    void  NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler( NTPClockSetServerKeyMessage *pNTPClockSetServerKeyMessage)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int ts_status, Key;
		char *p_svrlist, *p_server, *p_KeyValue;
		string serverList, server, KeyValue;
		serverKeyInfo serverKey;	

		trace (TRACE_LEVEL_DEBUG, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler Enter.");

		// Validations and Queries should go here
		if (pNTPClockSetServerKeyMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}

		serverList = pNTPClockSetServerKeyMessage->getIpAddrList();
		server = pNTPClockSetServerKeyMessage->getServer();
		Key = pNTPClockSetServerKeyMessage->getKey_ID();
		KeyValue = pNTPClockSetServerKeyMessage->getKey_Value();
		p_server = (char *)server.c_str();
		p_KeyValue = (char *)KeyValue.c_str();

		strncpy(serverKey.server, p_server, sizeof(serverKey.server) - 1);
		if (Key == 0)
		{
			// no authentication
			serverKey.keyinfo.index = -1;
		}
		else
		{
			serverKey.keyinfo.index = Key;
		}
		// only MD5;
		serverKey.keyinfo.keyType = KEY_MD5;  
		serverKey.keyinfo.key = p_KeyValue;
	
		char swidCstr[50];
		sprintf(swidCstr, "%s:%d:%s", p_server, Key, p_KeyValue);
		trace (TRACE_LEVEL_DEBUG, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler:status" + string(swidCstr));
		trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler setting to " + serverList);
		p_svrlist = (char *)serverList.c_str();
		ts_status = tsConfigureClockServer(p_svrlist, 1);
		if (TS_OK == ts_status)
		{
			ts_status  = tsSetClockServerAuthKey(1, &serverKey);
			if (TS_OK == ts_status)
			{
#ifdef NTP_LAST_CMD_SYNC_TEST
				if ((pNTPClockSetServerKeyMessage->getIsALastConfigReplay() ==false) &&
						(access( "/root/HA_Sec_Test", F_OK ) == 0)) {
					trace (TRACE_LEVEL_DEBUG, "pNTPClockSetServerKeyMessage getIsALastConfigReplay false");
					system ("/sbin/reboot -f");
				}
#endif
				trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler OK.");
			}
			else
			{
				status = translateTsStatusCode(ts_status);
				trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler: setting authentication key is failed");
				pNTPClockSetServerKeyMessage->setCompletionStatus(status);
				reply (pNTPClockSetServerKeyMessage);
				return;
			}
		}
		else
		{
			status = translateTsStatusCode(ts_status);
			trace (TRACE_LEVEL_INFO, "NTPClockLocalObjectManager::NTPClockSetServerKeyMessageHandler not set, current list " + serverList);
		}
		pNTPClockSetServerKeyMessage->setCompletionStatus(status);
		reply (pNTPClockSetServerKeyMessage);
    }

}
