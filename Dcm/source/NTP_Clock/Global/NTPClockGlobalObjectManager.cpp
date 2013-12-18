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

#include "DcmCore/DcmToolKit.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmResourceIdEnums.h"

#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPServerManagedObject.h"
#include "NTP_Clock/Global/NTPClockGlobalAddServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalChangeServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalRemoveServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalGetServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalSetClockMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalGetClockMessage.h"
// #include "NTP_Clock/Global/TimeZoneManagedObject.h"
#include "NTP_Clock/Global/NTPClockGlobalSetZoneMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"
#include "NTP_Clock/Global/timezone-list.h"
#include "NTP_Clock/Local/NTPClockSetServersMessage.h"
#include "NTP_Clock/Local/NTPClockGetActiveServerMessage.h"
#include "NTP_Clock/Local/NTPClockSetSwitchClockMessage.h"
#include "NTP_Clock/Local/NTPClockSetSwitchZoneMessage.h"
#include "NTP_Clock/Local/NTPClockGetSwitchClockMessage.h"
#include "NTP_Clock/Global/NTPKeyManagedObject.h"
#include "NTP_Clock/Global/NTPClockGlobalAddKeyMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalRemoveKeyMessage.h"
#include "NTP_Clock/Local/NTPClockSetServerKeyMessage.h"

extern "C"
{
#include "ts/public.h"
}

namespace DcmNs
{

    NTPClockGlobalObjectManager::NTPClockGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        NTPServerManagedObject    NTPServerManagedObject    (this);
        NTPServerManagedObject.setupOrm ();
        addManagedClass (NTPServerManagedObject::getClassName ());

        NTPKeyManagedObject    NTPKeyManagedObject    (this);
        NTPKeyManagedObject.setupOrm ();
        addManagedClass (NTPKeyManagedObject::getClassName ());

        // TR000317625: removing timezone managed object
        // TimeZoneManagedObject    TimeZoneManagedObject    (this);
        // TimeZoneManagedObject.setupOrm ();
        // addManagedClass (TimeZoneManagedObject::getClassName ());
        addOperationMap (NTPCLOCKGLOBALADDSERVER, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalAddServerMessageHandler));
        addOperationMap (NTPCLOCKGLOBALCHANGESERVER, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalChangeServerMessageHandler));
        addOperationMap (NTPCLOCKGLOBALREMOVESERVER, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalRemoveServerMessageHandler));
        addOperationMap (NTPCLOCKGLOBALGETSERVER, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalGetServerMessageHandler));
        addOperationMap (NTPCLOCKGLOBALSETCLOCK, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalSetClockMessageHandler));
        addOperationMap (NTPCLOCKGLOBALGETCLOCK, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalGetClockMessageHandler));
        addOperationMap (NTPCLOCKGLOBALSETZONE, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler));
        addOperationMap (NTPCLOCKGLOBALADDKEY, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessageHandler));
        addOperationMap (NTPCLOCKGLOBALREMOVEKEY, reinterpret_cast<PrismMessageHandler> (&NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler));
    }

    NTPClockGlobalObjectManager::~NTPClockGlobalObjectManager ()
    {
    }

    NTPClockGlobalObjectManager  *NTPClockGlobalObjectManager::getInstance()
    {
        static NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager = new NTPClockGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pNTPClockGlobalObjectManager, __FILE__, __LINE__);

        return (pNTPClockGlobalObjectManager);
    }

    string  NTPClockGlobalObjectManager::getClassName()
    {
        return ("NTPClockGlobal");
    }

    PrismServiceId  NTPClockGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *NTPClockGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NTPCLOCKGLOBALADDSERVER :
                pPrismMessage = new NTPClockGlobalAddServerMessage ();
                break;
            case NTPCLOCKGLOBALCHANGESERVER :
                pPrismMessage = new NTPClockGlobalChangeServerMessage ();
                break;
            case NTPCLOCKGLOBALREMOVESERVER :
                pPrismMessage = new NTPClockGlobalRemoveServerMessage ();
                break;
            case NTPCLOCKGLOBALGETSERVER :
                pPrismMessage = new NTPClockGlobalGetServerMessage ();
                break;
            case NTPCLOCKGLOBALSETCLOCK :
                pPrismMessage = new NTPClockGlobalSetClockMessage ();
                break;
            case NTPCLOCKGLOBALGETCLOCK :
                pPrismMessage = new NTPClockGlobalGetClockMessage ();
                break;
            case NTPCLOCKGLOBALSETZONE :
                pPrismMessage = new NTPClockGlobalSetZoneMessage ();
                break;
            case NTPCLOCKGLOBALADDKEY :
                pPrismMessage = new NTPClockGlobalAddKeyMessage ();
                break;
            case NTPCLOCKGLOBALREMOVEKEY :
                pPrismMessage = new NTPClockGlobalRemoveKeyMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NTPClockGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((NTPServerManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NTPServerManagedObject(this);
        }
        else if ((NTPKeyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NTPKeyManagedObject(this);
        }
        // TR000317625: removing timezone managed object
        // else if ((TimeZoneManagedObject::getClassName ()) == managedClassName)
        // {
        //     pWaveManagedObject = new TimeZoneManagedObject(this);
        // }
        else
        {
            trace (TRACE_LEVEL_FATAL, string("NTPClockGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : ") + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void NTPClockGlobalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
#if 0
        // TR000317625: removing timezone managed object
        TimeZoneManagedObject *pZoneMO;
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

        if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason))
        { // out of the box first boot, set global db to default
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::install() reason WAVE_BOOT_FIRST_TIME_BOOT"));
            /* Commit default to the Wave Database */
            startTransaction ();
            pZoneMO = new TimeZoneManagedObject(this, getDefaultTimezone());
            status = commitTransaction ();
            delete pZoneMO;
            if (FRAMEWORK_SUCCESS == status)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
#endif

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalAddServerMessageHandler( NTPClockGlobalAddServerMessage *pNTPClockGlobalAddServerMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpVxAddress serverIp;
        UI32 Key;
        string Key_Value, server;
        string /* prevlist, */ serverlist;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalAddServerMessageHandler Enter."));

        // Validations and Queries should go here
        if (pNTPClockGlobalAddServerMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        serverIp = pNTPClockGlobalAddServerMessage->getIpAddr();
        Key      = pNTPClockGlobalAddServerMessage->getKey();

        holdAll();

        // get the list of servers from the managed object and compile into a string
        serverlist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
        syncQuery->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added server
            /* prevlist = TS_DEFAULT_CLKSVR; */
            pNTPClockGlobalAddServerMessage->setIndex(0);
            trace (TRACE_LEVEL_INFO, string("NTPClockAddServerMessageHandler: server list has one entry"));
        }
        else if (TS_MAX_CLKSVRS == pResults->size ())
        {
            // already have max number of configured ntp servers
            status = NTPCLOCK_MAX_SERVERS;
            trace (TRACE_LEVEL_ERROR, serverIp.toString() + FrameworkToolKit::localize (status));
            added = false;
        }
        else
        {
            unsigned int i;

            // accumulate list into string; append new server; and send to locals
            for (i = 0; i < pResults->size(); i++)
            {
                NTPServerManagedObject *pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults)[i]);
                IpVxAddress tempIp = pMO->getServer();
                UI32 tempKey       = pMO->getKey();

                if (tempIp == serverIp)
                {
                    if(tempKey == Key)
                    {
					if (pNTPClockGlobalAddServerMessage->getIsALastConfigReplay() == true) {
						trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalAddServerMessage getIsALastConfigReplay already exits");
						status = WAVE_MESSAGE_SUCCESS;
					} else {
    	                // server is already in list.  set status
	                    status = NTPCLOCK_SERVER_ALREADY_IN_LIST;
					}
                    trace (TRACE_LEVEL_ERROR, serverIp.toString() + FrameworkToolKit::localize (status));
                    added = false;
                    break;
                    }
                }
                serverlist += tempIp.toString();
                /* prevlist = serverlist; */
                // add token between ip addresses in list
                serverlist += TS_TOKEN_SEPERATOR;
            }

            pNTPClockGlobalAddServerMessage->setIndex(pResults->size());
            // cleanup pResults vectors
            for (i = 0; i < pResults->size(); i++)
            {
                delete (*pResults)[i];
            }
        }
        // append new server to end of list
        serverlist += serverIp.toString();

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;
        // key association checking
        if ((false != added) && (Key != 0))
        {
            WaveManagedObjectSynchronousQueryContext *synQueryContext = new WaveManagedObjectSynchronousQueryContext(NTPKeyManagedObject::getClassName());
            synQueryContext->addAndAttribute (new AttributeUI32 (Key, "Key_ID"));
            vector<WaveManagedObject *> *pRes = querySynchronously (synQueryContext);
            if ((NULL == pRes) || (pRes->size() == 0))
            {       
                trace (TRACE_LEVEL_ERROR, "Associated Key is not present");
                status = NTPCLOCK_KEY_ASSOCIATION_ERROR;
                added = false;
            }
            else
            {
                NTPKeyManagedObject *pNTPKeyManagedObject = dynamic_cast<NTPKeyManagedObject *> ((*pRes)[0]);
                trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalAddServerMessage: Associated Key exits");			
                Key_Value = pNTPKeyManagedObject->getKey_Value();
                delete pNTPKeyManagedObject;
            }

            delete pRes;
            delete synQueryContext;
        }

        if (false == added)
        {
        // too many servers or server already in list or Associated Key is not present.  return status
            pNTPClockGlobalAddServerMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalAddServerMessage);
            unholdAll();
            return;
        }

        // send setservers list to cluster
        // NTPClockSetServersMessage *pMessage = new NTPClockSetServersMessage (serverlist);
        server = serverIp.toString();
        if (Key == 0)
        {
            // no authentication
            Key_Value = NTPSERVER_NO_KEY;
        }

        NTPClockSetServerKeyMessage *pMessage = new NTPClockSetServerKeyMessage (serverlist, server, Key, Key_Value);
        /* NTPClockSetServersMessage *pMessageRevert = new NTPClockSetServersMessage (prevlist); */

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::addServerSTCCallback), pNTPClockGlobalAddServerMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        /* pWaveSendToClusterContext->setPPrismMessageForPhase2(pMessageRevert); */

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::addServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::addServerSTCCallback"));

        NTPClockGlobalAddServerMessage *pOrigMsg = (NTPClockGlobalAddServerMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId stcc_status = pWaveSendToClusterContext->getCompletionStatus(), ret_status = WAVE_MESSAGE_SUCCESS;

        if (WAVE_MESSAGE_SUCCESS == stcc_status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::addServerSTCCallback : SUCCESS on local plugins. Updating DB"));

            UI32 index = pOrigMsg->getIndex();
            IpVxAddress serverIp = pOrigMsg->getIpAddr();
            UI32 Key = pOrigMsg->getKey();

            // add the server to the managed object
            startTransaction();
            NTPServerManagedObject *pNTPServerManagedObject = dynamic_cast<NTPServerManagedObject *>(createManagedObjectInstance(NTPServerManagedObject::getClassName()));
            pNTPServerManagedObject->setIndex(index);
            pNTPServerManagedObject->setServer(serverIp);
            pNTPServerManagedObject->setKey(Key);
            ret_status = commitTransaction();
            delete pNTPServerManagedObject;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::addServerSTCCallback Added Server ") + serverIp.toString());
                ret_status = WAVE_MESSAGE_SUCCESS;
                if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
                {
                    ret_status = CLUSTER_ERROR_1001;
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::addServerSTCCallback: failed adding server to DB"));
                ret_status = NTPCLOCK_ADDSERVER_ERROR;
            }
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) || 
			(WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status) || (CLUSTER_ERROR_1001 == ret_status))
        { // first print cluster status, then loop through locations
            string strIds;

            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::addServerSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (ret_status));

            ResourceId lStatus = traceSTCErrors(pWaveSendToClusterContext, string("NTPClockGlobalObjectManager::addServerSTCCallback"), strIds);
            if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status)
            { // if failed on all nodes, lStatus might be relevent
                ret_status = lStatus;
            }
            else
            {
                pOrigMsg->setMessageString(strIds);
            }
        }

        NTPClockSetServerKeyMessage *pSetMsg = (NTPClockSetServerKeyMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        /* NTPClockSetServersMessage *pRevertMsg = (NTPClockSetServersMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase2(); */

        delete(pSetMsg);
        /* delete(pRevertMsg); */
        delete pWaveSendToClusterContext;

        unholdAll();
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::addServerSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalGetClockMessageHandler( NTPClockGlobalGetClockMessage *pNTPClockGlobalGetClockMessage)
    {
        UI32 numVectors;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalGetClockMessageHandler Entering"));

        // Validations and Queries should go here
        if (pNTPClockGlobalGetClockMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        // send getswitchclock message to single location or to cluster
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::getClockSTCCallback), pNTPClockGlobalGetClockMessage);

        numVectors = pNTPClockGlobalGetClockMessage->getNumVectors();
        if (1 == numVectors)
        { // mappedId is specified, sending only to one switch
            vector<LocationId> locationsVector;
            UI32 mappedId;

            mappedId = pNTPClockGlobalGetClockMessage->getMappedId();
            if (LOCAL_SWITCH_ID == mappedId)
            {
                locationsVector.push_back(FrameworkToolKit::getThisLocationId());
            }
            else
            {
                locationsVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
                if (0 == locationsVector[0]) // invalid mappedId
                {
                    char swidCstr[16];
                    sprintf(swidCstr, "%d", mappedId);
                    trace (TRACE_LEVEL_INFO, string("NTPClockGlobalGetClockMessage invalid switchid") + string(swidCstr));
                    pNTPClockGlobalGetClockMessage->setCompletionStatus(NTPCLOCK_INVALID_MAPPED_ID);
                    reply (pNTPClockGlobalGetClockMessage);
                    delete pWaveSendToClusterContext;
                    return;
                }
            }
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locationsVector);
        }

        NTPClockGetSwitchClockMessage *pMessage = new NTPClockGetSwitchClockMessage ();
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::getClockSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        ResourceId status;

        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getClockSTCCallback"));

        NTPClockGlobalGetClockMessage *pOrigMsg = (NTPClockGlobalGetClockMessage *)pWaveSendToClusterContext->getPCallerContext();
        pOrigMsg->clearNumVectors();

        status = pWaveSendToClusterContext->getCompletionStatus();
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getClockSTCCallback : SUCCESS"));
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::getClockSTCCallback: failed on sendToCluster with error - ") + FrameworkToolKit::localize (status));
        }

        UI32 mappedId;
        vector<UI32> switchIds;
        string formattedSwitchIds;
        char swidCstr[16];
        switchIds.clear();
        // get location ids for return status or data
        vector<LocationId> locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        for (unsigned int i = 0; i < locationsVector.size(); i++)
        { // loop through each location and get msg rsp pointer for location
            NTPClockGetSwitchClockMessage *pLocMsg = (NTPClockGetSwitchClockMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[i]);
            mappedId = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);
            sprintf(swidCstr, "%d", mappedId);

            ResourceId lStatus = pLocMsg->getCompletionStatus();
            if (WAVE_MESSAGE_SUCCESS == lStatus)
            {
                DateTime localTime = pLocMsg->getClockDateTime();
                string zoneStr = pLocMsg->getClockTimeZone();
                pOrigMsg->setVectorClockDateTime(localTime, mappedId, zoneStr);
            }
            else
            { // trace error
                trace (TRACE_LEVEL_WARN, string("NTPClockGlobalObjectManager::getClockSTCCallback : FAILURE on switchid ") + string(swidCstr) + string(" with error - ") + FrameworkToolKit::localize (lStatus));
                switchIds.push_back(mappedId);
                if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
                { // if failed on all nodes, lStatus might be relevent
                    status = lStatus;
                }
                else
                {
                    status = CLUSTER_ERROR_1001;
                }
            }

            // don't need to free pLocMsg
        }

        if (0 != switchIds.size())
        {
            formattedSwitchIds = DcmToolKit::getFormattedSwitchIDs(switchIds);
            pOrigMsg->setMessageString(formattedSwitchIds);
        }

        NTPClockGetSwitchClockMessage *pGetMsg = (NTPClockGetSwitchClockMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pGetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getClockSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalGetServerMessageHandler( NTPClockGlobalGetServerMessage *pNTPClockGlobalGetServerMessage)
    {
        UI32 numVectors;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalGetServerMessageHandler Entering"));

        // Validations and Queries should go here
        if (pNTPClockGlobalGetServerMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        // send getactiveserver message to single location or to cluster
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::getServerSTCCallback), pNTPClockGlobalGetServerMessage);

        numVectors = pNTPClockGlobalGetServerMessage->getNumVectors();

        if (1 == numVectors)
        { // mappedId is specified, sending only to one switch
            vector<LocationId> locationsVector;
            UI32 mappedId;

            mappedId = pNTPClockGlobalGetServerMessage->getMappedId();
            if (LOCAL_SWITCH_ID == mappedId)
            {
                locationsVector.push_back(FrameworkToolKit::getThisLocationId());
            }
            else
            {
                locationsVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
                if (0 == locationsVector[0]) // invalid mappedId
                {
                    char swidCstr[16];
                    sprintf(swidCstr, "%d", mappedId);
                    trace (TRACE_LEVEL_INFO, string("NTPClockGlobalGetServerMessage invalid switchid") + string(swidCstr));
                    pNTPClockGlobalGetServerMessage->setCompletionStatus(NTPCLOCK_INVALID_MAPPED_ID);
                    reply (pNTPClockGlobalGetServerMessage);
                    delete pWaveSendToClusterContext;
                    return;
                }
            }
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locationsVector);
        }

        NTPClockGetActiveServerMessage *pMessage = new NTPClockGetActiveServerMessage ();
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::getServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        ResourceId status;

        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getServerSTCCallback"));

        NTPClockGlobalGetServerMessage *pOrigMsg = (NTPClockGlobalGetServerMessage *)pWaveSendToClusterContext->getPCallerContext();
        pOrigMsg->clearNumVectors();

        status = pWaveSendToClusterContext->getCompletionStatus();
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getServerSTCCallback : SUCCESS"));
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::getServerSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (status));
        }

        ResourceId lStatus;
        UI32 mappedId;
        vector<UI32> switchIds;
        string formattedSwitchIds;
        switchIds.clear();
        char swidCstr[16];
        // get location ids for return status or data
        vector<LocationId> locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        for (unsigned int i = 0; i < locationsVector.size(); i++)
        { // loop through each location and get msg rsp pointer for location
            NTPClockGetActiveServerMessage *pLocMsg = (NTPClockGetActiveServerMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[i]);    
            mappedId = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);
            sprintf(swidCstr, "%d", mappedId);

            lStatus = pLocMsg->getCompletionStatus();
            if (WAVE_MESSAGE_SUCCESS == lStatus)
            {
                IpVxAddress serverIp = pLocMsg->getIpAddr();
                pOrigMsg->setVectorActiveServer(serverIp, mappedId);
            }
            else
            { // trace error
                trace (TRACE_LEVEL_WARN, string("NTPClockGlobalObjectManager::getServerSTCCallback : FAILURE on switchid ") + string(swidCstr) + string(" with error - ") + FrameworkToolKit::localize (lStatus));
                switchIds.push_back(mappedId);
                if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
                { // if failed on all nodes, lStatus might be relevent
                    status = lStatus;
                }
                else
                {
                    status = CLUSTER_ERROR_1001;
                }
            }

            // don't need to free pLocMsg
        }

        if (0 != switchIds.size())
        {
            formattedSwitchIds = DcmToolKit::getFormattedSwitchIDs(switchIds);
            pOrigMsg->setMessageString(formattedSwitchIds);
        }

        NTPClockGetActiveServerMessage *pGetMsg = (NTPClockGetActiveServerMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pGetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::getServerSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalRemoveServerMessageHandler( NTPClockGlobalRemoveServerMessage *pNTPClockGlobalRemoveServerMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpVxAddress serverIp;
        /* string prevlist; */
        string serverlist;
        bool removed = false;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveServerMessageHandler Enter."));

        // Validations and Queries should go here
        if (pNTPClockGlobalRemoveServerMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        serverIp = pNTPClockGlobalRemoveServerMessage->getIpAddr();

        holdAll();
 
        // get the list of servers from the managed object and compile into a string
        /* prevlist.clear(); */
        serverlist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
        syncQuery->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. nothing to delete
            trace (TRACE_LEVEL_ERROR, string("NTPClockRemoveServerMessageHandler: server list is empty"));
        }
        else
        {
            // accumulate list into string
            for (unsigned int i = 0, num_svrs = 0; i < pResults->size(); i++)
            {
                NTPServerManagedObject *pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults)[i]);
                IpVxAddress tempIp = pMO->getServer();
                if (tempIp == serverIp)
                {
                    // found server to remove
                    removed = true;
                }
                else
                {
                    serverlist = serverlist + ((num_svrs > 0) ? TS_TOKEN_SEPERATOR : "") + tempIp.toString();
                    num_svrs++;
                }
                /* prevlist = prevlist + ((i > 0) ? TS_TOKEN_SEPERATOR : "") + tempIp.toString(); */
                delete (*pResults)[i];
            }
        }

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;

        if (false == removed)
        {
            unholdAll();
            // server not in list. return status
			if (pNTPClockGlobalRemoveServerMessage->getIsALastConfigReplay() == true) {
				trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalRemoveServerMessage getIsALastConfigReplay already removed");
				status = WAVE_MESSAGE_SUCCESS;
			} else {
	            status = NTPCLOCK_SERVER_NOT_IN_LIST;
			}
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveServerMessageHandler:NTPClockGlobalRemoveServerMessageHandler server not found. ") + serverIp.toString());
            pNTPClockGlobalRemoveServerMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalRemoveServerMessage);
            return;
        }

        // if list empty, don't set as empty list, set to "LOCL"
        if (serverlist.empty())
        {
            serverlist = TS_DEFAULT_CLKSVR;
        }

        // send setservers msg to cluster
        NTPClockSetServersMessage *pMessage = new NTPClockSetServersMessage (serverlist);
        /* NTPClockSetServersMessage *pMessageRevert = new NTPClockSetServersMessage (prevlist); */

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::removeServerSTCCallback), pNTPClockGlobalRemoveServerMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        /* pWaveSendToClusterContext->setPPrismMessageForPhase2(pMessageRevert); */

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::removeServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::removeServerSTCCallback"));

        NTPClockGlobalRemoveServerMessage *pOrigMsg = (NTPClockGlobalRemoveServerMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId stcc_status = pWaveSendToClusterContext->getCompletionStatus(), ret_status = WAVE_MESSAGE_SUCCESS;

        if (WAVE_MESSAGE_SUCCESS == stcc_status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::removeServerSTCCallback : SUCCESS on local plugins. Updating DB"));

            IpVxAddress serverIp = pOrigMsg->getIpAddr();

            // Find and Remove the server from the managed object
            WaveManagedObjectSynchronousQueryContext *syncQueryAttr = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
            syncQueryAttr->addOrderField("index", true);
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryAttr);
            if (NULL != pResults)
            {
                if (0 == pResults->size ())
                {
                    // no records. internal error
                    ret_status = NTPCLOCK_INTERNAL_ERROR;
                }
                else
                {
                    // look for server and update indexes that follow it
                    NTPServerManagedObject *pMO;
                    UI32 foundServer = pResults->size() + 1;
                    for (UI32 i = 0; i < pResults->size (); i++)
                    {
                        pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults)[i]);
                        // look for serverIp and remove it
                        if (serverIp == pMO->getServer())
                        {
                            foundServer = i;
                            startTransaction();
                        }

                        if (foundServer < i)
                        {
                            updateWaveManagedObject (pMO);
                            pMO->setIndex(i - 1);
                        }
                    }

                    if (foundServer < pResults->size())
                    {
                        // remove from NTPServerManagedObject
                        pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults)[foundServer]);
                        delete pMO;
                        ret_status = commitTransaction();
                        /* Defect 457783 - Deleting all pResults other than
                           foundServer to fix memory leak */
                        for (UI32 i = 0; i < pResults->size(); i++)
                        {
                            if (i != foundServer)
                            {
                                delete (*pResults)[i];
                            }
                        }
                    }
                    else
                    {
                        // not found. internal error
                        ret_status = NTPCLOCK_INTERNAL_ERROR;
                    }
                }
                pResults->clear ();
                delete pResults;
            }
            else
            {
                // query error
                ret_status = NTPCLOCK_INTERNAL_ERROR;
            }
            delete syncQueryAttr;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("removeServerSTCCallback: Removed Server ") + serverIp.toString());
                ret_status = WAVE_MESSAGE_SUCCESS;
                if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
                {
                    ret_status = CLUSTER_ERROR_1001;
                }
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("removeServerSTCCallback: MO failure removing Server"));
                ret_status = NTPCLOCK_REMOVESERVER_ERROR;
            }
        }


        if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status || CLUSTER_ERROR_1001 == ret_status)
        {
            string strIds;

            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::removeServerSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (ret_status));

            ResourceId lStatus = traceSTCErrors(pWaveSendToClusterContext, string("NTPClockGlobalObjectManager::removeServerSTCCallback"), strIds);
            if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status)
            { // if failed on all nodes, lStatus might be relevent
                ret_status = lStatus;
            }
            else
            {
                pOrigMsg->setMessageString(strIds);
            }
        }

        NTPClockSetServersMessage *pSetMsg = (NTPClockSetServersMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        /* NTPClockSetServersMessage *pRevertMsg = (NTPClockSetServersMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase2(); */

        delete(pSetMsg);
        /* delete(pRevertMsg); */
        delete pWaveSendToClusterContext;

        unholdAll();
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::removeServerSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalSetClockMessageHandler( NTPClockGlobalSetClockMessage *pNTPClockGlobalSetClockMessage)
    {
        DateTime setTime;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalSetClockMessageHandler Entering"));

        // Validations and Queries should go here
        if (pNTPClockGlobalSetClockMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        setTime = pNTPClockGlobalSetClockMessage->getClockDateTime();

        // send set time msg to cluster
        NTPClockSetSwitchClockMessage *pMessage = new NTPClockSetSwitchClockMessage (setTime);
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::setClockSTCCallback), pNTPClockGlobalSetClockMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::setClockSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setClockSTCCallback"));

        NTPClockGlobalSetClockMessage *pOrigMsg = (NTPClockGlobalSetClockMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setclockSTCCallback : SUCCESS"));
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
            {
                status = CLUSTER_ERROR_1001;
            }
        }
        else if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
        { // if failed on all nodes, lStatus might be relevent
            trace (TRACE_LEVEL_ERROR,
                string("NTPClockGlobalObjectManager::setClockSTCCallback: failed on sendToCluster with Framework error - ")
                + FrameworkToolKit::localize (status));
        }

        if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status || CLUSTER_ERROR_1001 == status)
        {
            string strIds;

            ResourceId lStatus = traceSTCErrors(pWaveSendToClusterContext, string("NTPClockGlobalObjectManager::setClockSTCCallback"), strIds);
            if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
            { // if failed on all nodes, lStatus might be relevent
                status = lStatus;
            }
            else
            {
                pOrigMsg->setMessageString(strIds);
            }
        }

        NTPClockSetSwitchClockMessage *pSetMsg = (NTPClockSetSwitchClockMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        pOrigMsg->setCompletionStatus(status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler( NTPClockGlobalSetZoneMessage *pNTPClockGlobalSetZoneMessage)
    {
        // ResourceId status = WAVE_MESSAGE_SUCCESS;
        // TimeZoneManagedObject *pTimeZoneMO;
        string zoneStr; //, prevStr;
        UI32 zoneEnum;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler Entering"));

        // Validations and Queries should go here
        if (pNTPClockGlobalSetZoneMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        zoneEnum = pNTPClockGlobalSetZoneMessage->getTimezone();
        // trace (TRACE_LEVEL_INFO, "NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler zoneEnum" + zoneEnum);
        zoneStr = pNTPClockGlobalSetZoneMessage->convertZoneEnumToStr(zoneEnum);
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler zoneStr ") + zoneStr);

#if 0
        // TR000317625: removing timezone managed object
        // get timezone managed object, for reverting in case of error during distribution
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(TimeZoneManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // not found. set prevStr to default
            trace (TRACE_LEVEL_WARN, string("NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler no default timezone in the database"));
            prevStr = getDefaultTimezone();
            startTransaction();
            pTimeZoneMO = dynamic_cast<TimeZoneManagedObject *>(createManagedObjectInstance(TimeZoneManagedObject::getClassName()));
            pTimeZoneMO->setTimezonestring(prevStr);
            commitTransaction();
        }
        else if (1 == pResults->size ())
        {
            // found. recast it and update it's value
            pTimeZoneMO = dynamic_cast<TimeZoneManagedObject *> ((*pResults)[0]);
            prevStr = pTimeZoneMO->getTimezonestring();
        }
        else
        {
            // should only be one
            status = NTPCLOCK_INTERNAL_ERROR;
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::NTPClockGlobalSetZoneMessageHandler DB Error, too many entries"));
            pNTPClockGlobalSetZoneMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalSetZoneMessage);
            pResults->clear();
            delete pResults;
            return;
        }

        if (pResults)
        {
            pResults->clear();
            delete pResults;
        }
        delete syncQuery;
#endif

        // send set timezone msg to cluster
        NTPClockSetSwitchZoneMessage *pMessage = new NTPClockSetSwitchZoneMessage (zoneStr);
        // NTPClockSetSwitchZoneMessage *pMessageRevert = new NTPClockSetSwitchZoneMessage (prevStr);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::setZoneSTCCallback), pNTPClockGlobalSetZoneMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        // pWaveSendToClusterContext->setPPrismMessageForPhase2(pMessageRevert);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::setZoneSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setZoneSTCCallback"));

        NTPClockGlobalSetZoneMessage *pOrigMsg = (NTPClockGlobalSetZoneMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setZoneSTCCallback : SUCCESS on local plugins. Updating DB"));

            UI32 zoneEnum = pOrigMsg->getTimezone();
            string zoneStr = pOrigMsg->convertZoneEnumToStr(zoneEnum);

#if 0
            // TR000317625: removing timezone managed object
            // update the global DB for timezone
            // Find and update the timezone managed object
            WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(TimeZoneManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
            if ((NULL == pResults) || (0 == pResults->size ()))
            { // not found. internal error since must always be 1
                trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::setZoneSTCCallback : DB does not contain timezonemanagedobject"));

                // create one
                startTransaction ();
                TimeZoneManagedObject *pZoneMO = new TimeZoneManagedObject(this, zoneStr);
                status = commitTransaction ();
                delete pZoneMO;
                if (FRAMEWORK_SUCCESS == status)
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (1 < pResults->size ())
            { // internal error, should only have one
                trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::setZoneSTCCallback : DB contains too many timezonemanagedobject"));
                status = NTPCLOCK_INTERNAL_ERROR;

            }
            else
               { // found. update value to new zone
                TimeZoneManagedObject *pZoneMO = dynamic_cast<TimeZoneManagedObject *> ((*pResults)[0]);
                startTransaction();
                updateWaveManagedObject(pZoneMO);
                pZoneMO->setTimezonestring(zoneStr);
                status = commitTransaction();
                delete pZoneMO;
            }

            pResults->clear ();
            delete pResults;
            delete syncQuery;

            if (FRAMEWORK_SUCCESS == status)
            {
                trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setZoneSTCCallback timezone is ") + zoneStr);
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::setZoneSTCCallback: failed setting zone to DB, ") + zoneStr);
                status = NTPCLOCK_ZONE_ERROR;
            }
#endif

            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
            {
                status = CLUSTER_ERROR_1001;
            }
        }

        if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status || CLUSTER_ERROR_1001 == status)
        {
            string strIds;
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::setZoneSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (status));

            ResourceId lStatus = traceSTCErrors(pWaveSendToClusterContext, string("NTPClockGlobalObjectManager::setZoneSTCCallback"), strIds);
            
            if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
            { // if failed on all nodes, lStatus might be relevent
                status = lStatus;
            }
            else
            {
                pOrigMsg->setMessageString(strIds);
            }
        }

        NTPClockSetSwitchZoneMessage *pSetMsg = (NTPClockSetSwitchZoneMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        // NTPClockSetSwitchZoneMessage *pRevertMsg = (NTPClockSetSwitchZoneMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase2();

        delete(pSetMsg);
        // delete(pRevertMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::setZoneSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(status);
        reply (pOrigMsg);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessageHandler( NTPClockGlobalAddKeyMessage *pNTPClockGlobalAddKeyMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 Key_ID;
        string Key_Value;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessageHandler Enter."));

        // Validations and Queries should go here
        if (pNTPClockGlobalAddKeyMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        Key_ID = pNTPClockGlobalAddKeyMessage->getKey_ID();
        Key_Value = pNTPClockGlobalAddKeyMessage->getKey_Value();

        holdAll();

        // get the list of keys from the managed object
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(NTPKeyManagedObject::getClassName());
        syncQuery->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added key
            pNTPClockGlobalAddKeyMessage->setIndex(0);
            trace (TRACE_LEVEL_INFO, string("NTPClockAddKeyMessageHandler: Key list has one entry"));
        }
        else
        {
            unsigned int i;

            // append new key;
            for (i = 0; i < pResults->size(); i++)
            {
                NTPKeyManagedObject *pMO = dynamic_cast<NTPKeyManagedObject *> ((*pResults)[i]);
                UI32 tempKey_ID = pMO->getKey_ID();
                if (tempKey_ID == Key_ID)
                {
                    if (pNTPClockGlobalAddKeyMessage->getIsALastConfigReplay() == true) {
                        trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalAddKeyMessage getIsALastConfigReplay already exits");
                        status = WAVE_MESSAGE_SUCCESS;
                    } else {
                        // Key is already in list.  set status
                        status = NTPCLOCK_KEY_ALREADY_IN_LIST;
                    }
                    trace (TRACE_LEVEL_ERROR, FrameworkToolKit::localize (status));
                    added = false;
                    break;
                }
            }

            pNTPClockGlobalAddKeyMessage->setIndex(pResults->size());
            // cleanup pResults vectors            
            for (i = 0; i < pResults->size(); i++)
            {
                delete (*pResults)[i];
            }
        }

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;

        if (false == added)
        {
            // Key already in list.  return status
            pNTPClockGlobalAddKeyMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalAddKeyMessage);
            unholdAll();
            return;
        }
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessage:Updating DB"));

        UI32 index = pNTPClockGlobalAddKeyMessage->getIndex();

        // add the key to  managed object
        startTransaction();
        NTPKeyManagedObject *pNTPKeyManagedObject = dynamic_cast<NTPKeyManagedObject *>(createManagedObjectInstance(NTPKeyManagedObject::getClassName()));
        pNTPKeyManagedObject->setIndex(index);
        pNTPKeyManagedObject->setKey_ID(Key_ID);
        pNTPKeyManagedObject->setKey_Value(Key_Value);
        status = commitTransaction();
        delete pNTPKeyManagedObject;

        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessage:Added Key"));
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessage: failed adding key to DB"));
            status = NTPCLOCK_ADDKEY_ERROR;
        }

        unholdAll();
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalAddKeyMessage:Exiting"));
        pNTPClockGlobalAddKeyMessage->setCompletionStatus(status);
        reply (pNTPClockGlobalAddKeyMessage);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler( NTPClockGlobalRemoveKeyMessage *pNTPClockGlobalRemoveKeyMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 Key_ID;
        bool removed = false;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler Enter."));

        // Validations and Queries should go here
        if (pNTPClockGlobalRemoveKeyMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        Key_ID = pNTPClockGlobalRemoveKeyMessage->getKey_ID();

        holdAll();

        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(NTPKeyManagedObject::getClassName());
        syncQuery->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. nothing to delete
            trace (TRACE_LEVEL_ERROR, string("NTPClockRemoveKeyMessageHandler: Key list is empty"));
        }
        else
        {
            for (unsigned int i = 0; i < pResults->size(); i++)
            {
                NTPKeyManagedObject *pMO = dynamic_cast<NTPKeyManagedObject *> ((*pResults)[i]);
                UI32 tempKey_ID = pMO->getKey_ID();
                if (tempKey_ID == Key_ID)
                {
                    // found key to remove
                    removed = true;
                }
            }
        }
        // check the key is associated with any existing server
        if (true == removed)
        {
            WaveManagedObjectSynchronousQueryContext *synQueryContext = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
            synQueryContext->addAndAttribute (new AttributeUI32 (Key_ID, "Key"));
            vector<WaveManagedObject *> *pRes = querySynchronously (synQueryContext);
            if ((NULL == pRes) || (pRes->size() == 0))
            { 
                trace (TRACE_LEVEL_ERROR, "Key is not associated with server");
            }
            else
            {
                 status = NTPCLOCK_KEY_DISASSOCIATION_ERROR;
                 removed =false;
            }

            delete pRes;
            delete synQueryContext;
        }

        if (false == removed)
        {
            if (NULL != pResults) {
                pResults->clear ();
                delete pResults;
            }
            delete syncQuery;
            unholdAll();
            if (pNTPClockGlobalRemoveKeyMessage->getIsALastConfigReplay() == true) {
                trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalRemoveKeyMessage getIsALastConfigReplay already removed");
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                if (status != NTPCLOCK_KEY_DISASSOCIATION_ERROR) {
	                // key not in list. return status
    	            status = NTPCLOCK_KEY_NOT_IN_LIST;
                }
            }
            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler key not found. "));
            pNTPClockGlobalRemoveKeyMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalRemoveKeyMessage);
            return;
        }

        // find and remove the key from the managed object
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler : Updating DB"));

        // look for key and update indexes that follow it
        NTPKeyManagedObject *pMO;
        UI32 foundKey = pResults->size() + 1;
        for (UI32 i = 0; i < pResults->size (); i++)
        {
            pMO = dynamic_cast<NTPKeyManagedObject *> ((*pResults)[i]);
            // look for Key and remove it
            if (Key_ID == pMO->getKey_ID())
            {
               foundKey = i;
               startTransaction();
            }
            if (foundKey < i)
            {
               updateWaveManagedObject (pMO);
               pMO->setIndex(i - 1);
            }
        }

        if (foundKey < pResults->size())
        {
            // remove from NTPKeyManagedObject
            pMO = dynamic_cast<NTPKeyManagedObject *> ((*pResults)[foundKey]);
            delete pMO;
            status = commitTransaction();
        }
        else
        {
            // not found. internal error
            status = NTPCLOCK_INTERNAL_ERROR;
        }
        pResults->clear ();
        delete pResults;

        delete syncQuery;
        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalRemoveKeyMessageHandler: Removed Key "));
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalRemoveKeyMessageHandler: MO failure removing Key"));
            status = NTPCLOCK_REMOVEKEY_ERROR;
        }

        unholdAll();
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalRemoveKeyMessageHandler Exiting"));
        pNTPClockGlobalRemoveKeyMessage->setCompletionStatus(status);
        reply (pNTPClockGlobalRemoveKeyMessage);
    }

    void  NTPClockGlobalObjectManager::NTPClockGlobalChangeServerMessageHandler( NTPClockGlobalChangeServerMessage *pNTPClockGlobalChangeServerMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpVxAddress serverIp;
        UI32 Key;
        string serverlist, server, Key_Value;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::NTPClockGlobalChangeServerMessageHandler Enter."));

        // Validations and Queries should go here
        if (pNTPClockGlobalChangeServerMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        serverIp = pNTPClockGlobalChangeServerMessage->getIpAddr();
        Key      = pNTPClockGlobalChangeServerMessage->getKey();

        holdAll();

        // get the list of servers from the managed object and compile into a string
        serverlist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
        syncQuery->addOrderField("index", true);
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added server
            pNTPClockGlobalChangeServerMessage->setIndex(0);
            trace (TRACE_LEVEL_INFO, string("NTPClockChangeServerMessageHandler: server list has one entry"));
        }
        else if (TS_MAX_CLKSVRS == pResults->size ())
        {
            // already have max number of configured ntp servers
            status = NTPCLOCK_MAX_SERVERS;
            trace (TRACE_LEVEL_ERROR, serverIp.toString() + FrameworkToolKit::localize (status));
            added = false;
        }
        else
        {
            unsigned int i;

            // accumulate list into string; append new server; and send to locals
            for (i = 0; i < pResults->size(); i++)
            {
                NTPServerManagedObject *pMO = dynamic_cast<NTPServerManagedObject *> ((*pResults)[i]);
                IpVxAddress tempIp = pMO->getServer();
                UI32 tempKey       = pMO->getKey();

                if ((tempIp == serverIp))
                {
                    if(tempKey == Key)
                    {
                        if (pNTPClockGlobalChangeServerMessage->getIsALastConfigReplay() == true) {
                            trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalChangeServerMessage getIsALastConfigReplay already exits");
                            status = WAVE_MESSAGE_SUCCESS;
                        } else {
                            // server is already in list.  set status
                            status = NTPCLOCK_SERVER_ALREADY_IN_LIST;
                        }
                    	trace (TRACE_LEVEL_ERROR, serverIp.toString() + FrameworkToolKit::localize (status));
                    	added = false;
                    	break;
                    }
                }
                serverlist += tempIp.toString();
                serverlist += TS_TOKEN_SEPERATOR;
            }

            pNTPClockGlobalChangeServerMessage->setIndex(pResults->size());
            // cleanup pResults vectors
            for (i = 0; i < pResults->size(); i++)
            {
                delete (*pResults)[i];
            }
        }

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;
        // key association checking
        if ((false != added) && (Key != 0))
        {
            WaveManagedObjectSynchronousQueryContext *synQueryContext = new WaveManagedObjectSynchronousQueryContext(NTPKeyManagedObject::getClassName());
            synQueryContext->addAndAttribute (new AttributeUI32 (Key, "Key_ID"));
            vector<WaveManagedObject *> *pRes = querySynchronously (synQueryContext);
            if ((NULL == pRes) || (pRes->size() == 0))
            { 
                trace (TRACE_LEVEL_ERROR, "Associated Key is not present");
                status = NTPCLOCK_KEY_ASSOCIATION_ERROR;
                added = false;
            }
            else
            {
                NTPKeyManagedObject *pNTPKeyManagedObject = dynamic_cast<NTPKeyManagedObject *> ((*pRes)[0]);
                trace (TRACE_LEVEL_DEBUG, "pNTPClockGlobalChangeServerMessage: Associated Key exits");
                Key_Value = pNTPKeyManagedObject->getKey_Value();
                delete pNTPKeyManagedObject;
            }

            delete pRes;
            delete synQueryContext;
        }

        if (false == added)
        {
         // too many servers or server already in list or associated Key is not in list.  return status
            pNTPClockGlobalChangeServerMessage->setCompletionStatus(status);
            reply (pNTPClockGlobalChangeServerMessage);
            unholdAll();
            return;
        }

        // send setservers list and key to cluster
        server = serverIp.toString();
        if (Key == 0)
        {
            // no authentication
            Key_Value = NTPSERVER_NO_KEY;
        }
        NTPClockSetServerKeyMessage *pMessage = new NTPClockSetServerKeyMessage (serverlist, server, Key, Key_Value);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&NTPClockGlobalObjectManager::changeServerSTCCallback), pNTPClockGlobalChangeServerMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void NTPClockGlobalObjectManager::changeServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::changeServerSTCCallback"));

        NTPClockGlobalChangeServerMessage *pOrigMsg = (NTPClockGlobalChangeServerMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId stcc_status = pWaveSendToClusterContext->getCompletionStatus(), ret_status = WAVE_MESSAGE_SUCCESS;

        if (WAVE_MESSAGE_SUCCESS == stcc_status)
        {
            trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::changeServerSTCCallback : SUCCESS on local plugins. Updating DB"));

            UI32 index;
            IpVxAddress serverIp = pOrigMsg->getIpAddr();
            UI32 Key = pOrigMsg->getKey();

            /* Get the server */
            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(NTPServerManagedObject::getClassName());
            synchronousQueryContext->addAndAttribute (new AttributeIpVxAddress (serverIp, "server"));
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such host managed object");
                pOrigMsg->setCompletionStatus(NTPCLOCK_ADDSERVER_ERROR);
                NTPClockSetServerKeyMessage *pSetMsg = (NTPClockSetServerKeyMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
                delete(pSetMsg);
                reply (pOrigMsg);
                delete pWaveSendToClusterContext;
                delete synchronousQueryContext;
                return;
            }

            NTPServerManagedObject *pNTPServerManagedObject = dynamic_cast<NTPServerManagedObject *> ((*pResults)[0]);
            index = pNTPServerManagedObject->getIndex();
            delete pResults;
            delete synchronousQueryContext;

            // add the server to the managed object
            startTransaction();
            updateWaveManagedObject (pNTPServerManagedObject);
            pNTPServerManagedObject->setIndex(index);
            pNTPServerManagedObject->setServer(serverIp);
            pNTPServerManagedObject->setKey(Key);
            ret_status = commitTransaction();
            delete pNTPServerManagedObject;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::changeServerSTCCallback Changed Server ") + serverIp.toString());
                ret_status = WAVE_MESSAGE_SUCCESS;
                if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
                {
                    ret_status = CLUSTER_ERROR_1001;
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::changeServerSTCCallback: failed changing server to DB"));
                ret_status = NTPCLOCK_ADDSERVER_ERROR;
            }
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
            (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status) || (CLUSTER_ERROR_1001 == ret_status))
        { // first print cluster status, then loop through locations
            string strIds;

            trace (TRACE_LEVEL_ERROR, string("NTPClockGlobalObjectManager::changeServerSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (ret_status));

            ResourceId lStatus = traceSTCErrors(pWaveSendToClusterContext, string("NTPClockGlobalObjectManager::changeServerSTCCallback"), strIds);
            if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == stcc_status)
            { // if failed on all nodes, lStatus might be relevent
                ret_status = lStatus;
            }
            else
            {
                pOrigMsg->setMessageString(strIds);
            }
        }

        NTPClockSetServerKeyMessage *pSetMsg = (NTPClockSetServerKeyMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        unholdAll();
        trace (TRACE_LEVEL_INFO, string("NTPClockGlobalObjectManager::changeServerSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

    ResourceId NTPClockGlobalObjectManager::traceSTCErrors(WaveSendToClusterContext *pWaveSTCContext, const string &callerName, string &formattedIds)
    {
        // get location ids for return status to find out why first failed
        ResourceId status = WAVE_MESSAGE_SUCCESS, lStatus = WAVE_MESSAGE_SUCCESS;
        vector<UI32> switchIds;
        vector<LocationId> locationsVector = pWaveSTCContext->getLocationsToSendToForPhase1();
        switchIds.clear();
        for (unsigned int i = 0; i < locationsVector.size(); i++)
        {
            lStatus = pWaveSTCContext->getSendStatusForPhase1(locationsVector[i]);
            if (WAVE_MESSAGE_SUCCESS == lStatus)
                lStatus = pWaveSTCContext->getFrameworkStatusForPhase1(locationsVector[i]);
            if (FRAMEWORK_SUCCESS == lStatus)
                lStatus = pWaveSTCContext->getCompletionStatusForPhase1(locationsVector[i]);

            if (WAVE_MESSAGE_SUCCESS != lStatus && FRAMEWORK_SUCCESS != lStatus)
            {
                UI32 mappedId = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);
                char idCstr[16];
                sprintf(idCstr, "%d", mappedId);
                trace (TRACE_LEVEL_WARN, callerName + string(" : FAILURE on local switchid ") + string(idCstr) + string(" plugin with error - ") + FrameworkToolKit::localize (lStatus));
                switchIds.push_back(mappedId);
                status = lStatus;
            }
        }
        trace (TRACE_LEVEL_ERROR, callerName + string(" : FAILURE on local plugins with error - ") + FrameworkToolKit::localize (status));

        formattedIds = DcmToolKit::getFormattedSwitchIDs(switchIds);

        // get location ids for return status to find out revert status
        vector<LocationId> location2Vector = pWaveSTCContext->getLocationsToSendToForPhase2();
        for (unsigned int i = 0; i < location2Vector.size(); i++)
        {
            lStatus = pWaveSTCContext->getSendStatusForPhase2(location2Vector[i]);
            if (WAVE_MESSAGE_SUCCESS == lStatus)
                lStatus = pWaveSTCContext->getFrameworkStatusForPhase2(location2Vector[i]);
            if (FRAMEWORK_SUCCESS == lStatus)
                lStatus = pWaveSTCContext->getCompletionStatusForPhase2(location2Vector[i]);

            if (WAVE_MESSAGE_SUCCESS != lStatus && FRAMEWORK_SUCCESS != lStatus)
            {
                char idCstr[16];
                sprintf(idCstr, "%d", DcmToolKit::getMappedIdFromLocationId(location2Vector[i]));
                trace (TRACE_LEVEL_WARN, callerName + string(" : FAILURE on revert, switchid ") + string(idCstr) + string(" is inconsistent to rest of cluster with error - ") + FrameworkToolKit::localize (lStatus));
            }
        }

        return status;
    }
}
