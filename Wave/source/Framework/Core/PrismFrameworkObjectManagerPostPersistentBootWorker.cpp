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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManagerPostPersistentBootWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/Wave.h"
#include "Framework/Core/WaveConfigurationIntentRepository.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WavePostBootEvent.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
#include "Framework/Postboot/PrismPostbootAgent.h"
#include "Framework/Postboot/PrismPostbootAgentThread.h"
#include "Framework/Postboot/PrismPostPersistentBootWorkerClusterContext.h"    
#include "Framework/Core/ConfigReplayStartedEvent.h"
#include "Framework/Core/ConfigReplayCompletedEvent.h"
#include "Framework/Core/LastConfigReplayCompletedEvent.h"
#include "Framework/Core/NodeReadyForAllCommandsEvent.h"
#include "Framework/ConfigReplayEnd/ConfigReplayEndAgent.h"
#include "Framework/Persistence/PersistenceObjectManagerMessages.h"
#include "Framework/Attributes/AttributeResourceId.h"

#include <cerrno>

namespace WaveNs
{

PrismFrameworkObjectManagerPostPersistentBootWorker::PrismFrameworkObjectManagerPostPersistentBootWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_EXECUTE_POST_PERSISTENT_BOOT_PASSES, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_EXECUTE_POST_PERSISTENT_BOOT_SERVICE_AND_WAVE_SLOTS, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootServiceAndWaveSlotsMessageHandler));

    addEventType (FRAMEWORK_OBJECT_MANAGER_LAST_CONFIG_REPLAY_COMPLETED_EVENT);
}

PrismFrameworkObjectManagerPostPersistentBootWorker::~PrismFrameworkObjectManagerPostPersistentBootWorker ()
{
}

void PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler (PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage)
{
    bool        isPostbootRequired                  = pPrismPostPersistenceBootMessage->getIsPostbootRequired ();
    bool        executeConfigReplaySequence         = pPrismPostPersistenceBootMessage->getExecuteConfigReplaySequence ();
    bool        executeNodeReadySequence            = pPrismPostPersistenceBootMessage->getExecuteNodeReadySequence ();
    bool        isBannerOutputToClientsRequired     = pPrismPostPersistenceBootMessage->getIsBannerOutputToClientsRequired ();
    ResourceId  recoveryType                        = pPrismPostPersistenceBootMessage->getRecoveryType ();
    bool        isAllCommandReadyEvent              = false;
    bool        skipReply                           = false;
    bool        fileReplay                          = false;
    bool        isActivationCompleteEvent           = false;

    PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    
    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    bool isConfigFileReplayGoingToBeDone = false;
    bool isConfigReplayCompleteEventLogged = false;

    if (isPostbootRequired)
    {
        switch (pPrismPostPersistenceBootMessage->getEventId())
        {
            case POSTBOOT_EVENT_SERVICE_READY:
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker:: POSTBOOT_EVENT_SERVICE_READY recved");

                trace (TRACE_LEVEL_PERF_START, "Global Postboot.");

                pPrismFrameworkObjectManager->setActivationCompleted (true);
                pPrismFrameworkObjectManager->setPostBootStarted(true);
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: PostBootStarted");

                updateHardwareSynchronizationState (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_SYNCHRONIZING, FrameworkToolKit::getThisLocationId ());

                if (true == executeConfigReplaySequence)
                {
                    // We will only replay the startup file in cold boot or cold recovery without sync dump ever complete
                    if ((false == FrameworkToolKit::getIsStartupValid ()) && (pPrismFrameworkObjectManager->getStandbySyncState () == OUT_OF_SYNC))
                    {
                        if (false == FrameworkToolKit::getStartupFileName ().empty ())
                        {
                            isConfigFileReplayGoingToBeDone = true;
                            FrameworkToolKit::setIsConfigFileReplayGoingToBeDone (isConfigFileReplayGoingToBeDone);
                            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: isConfigFileReplayGoingToBeDone set to true.");
                        }
                    }
                }

                triggerPostBootPassTable (POSTBOOT_GLOBAL_CONFIG, 0, pPrismPostPersistenceBootMessage);
                trace (TRACE_LEVEL_PERF_END, "Global Postboot.");
                break;
            }
            case POSTBOOT_EVENT_LINECARD_READY:
                trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerPostPersistentBootWorker:: POSTBOOT_EVENT_LINECARD_READY recved") + pPrismPostPersistenceBootMessage->getParameter() );

                if (true == FrameworkToolKit::getSecondaryClusterFormationFlag ())
                {
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: secondaryClusterFormationFlag is set to true.");
                    postponeMessageHandling (pPrismPostPersistenceBootMessage);
                    return;
                }

                triggerPostBootPassTable (POSTBOOT_LINECARD_CONFIG, pPrismPostPersistenceBootMessage->getParameter(),pPrismPostPersistenceBootMessage);
                skipReply = true;
                break;

            case POSTBOOT_EVENT_SYSTEM_READY:
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker:: POSTBOOT_EVENT_SYSTEM_READY recved");

                if (true == FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ())
                {
                    isConfigFileReplayGoingToBeDone = false;
                    FrameworkToolKit::setIsConfigFileReplayGoingToBeDone (isConfigFileReplayGoingToBeDone);
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: isConfigFileReplayGoingToBeDone set to false.");
                }

                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: PostBootCompleted");

                pPrismFrameworkObjectManager->setPostBootCompleted(true);

                if (true == executeConfigReplaySequence)
                {
                    pPrismFrameworkObjectManager->setConfigReplayStarted(true);
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: ConfigReplayStarted");
                      trace (TRACE_LEVEL_PERF_START, "Config Replay.");

                    // Temporarily disable the last config replay for LC mode to resolve node segamentation issue
                    if ((WAVE_HA_WARM_RECOVERY == recoveryType) && (false == Wave::clusterEnabledCheck ()))
                    {
                        // Replay the last configuration in the warm recovery case for any configuration that was in-progress during an HA failover.

                        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: LastConfigReplay Started.");
                        
                        ResourceId lastConfigReplayStatus = triggerLastConfigReplay ();

                        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: LastConfigReplay Completed. Status : ") + FrameworkToolKit::localize (lastConfigReplayStatus));
                    }
                    // We will only replay the startup file in cold boot or cold recovery without sync dump ever complete
                    else if ((false == FrameworkToolKit::getIsStartupValid ()) && (pPrismFrameworkObjectManager->getStandbySyncState () == OUT_OF_SYNC))
                    {
                        ///////////////////////////////////////////////////////////////////////
                        // Trigger config replay in standalone mode for regular file. 
                        // Trigger config replay in any mode for default file. 
                        ///////////////////////////////////////////////////////////////////////

                        if (false == FrameworkToolKit::getStartupFileName ().empty ())
                        {
                            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler : triggerStartupFileReplay is being called");
                            ConfigReplayStartedEvent* pConfigReplayStartedEvent = new ConfigReplayStartedEvent(true);
                            broadcast (pConfigReplayStartedEvent);
                            fileReplay = true;
                            status = triggerStartupFileReplay ();
                        }
                        else
                        {
                            // In general, startupFile name is -
                            //     FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + s_startupFileName
                            //     Where, s_startupFileName = "startupFile"
                            // So, FrameworkToolKit::getStartupFileName ().empty () will not be true.
                            //
                            // However, if it is allowed to be empty, then, even though line-by-line config-replay is not done,
                            // (for consistency required/requested by FrameworkToolKit::getRewriteStartupSchemaOnNodeReady ()) -
                            //     if FrameworkToolKit::getRewriteStartupSchemaOnNodeReady () returns true,
                            //     setupStartupAfterConfigReplay function should be called.

                            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler : startupFile name is empty.");
                        }
                    }
                    else if (true == FrameworkToolKit::getRewriteStartupSchemaOnNodeReady ())
                    {
                        // i.e. actual line-by-line config-replay is not done.
                        // but, during boot-up process, some service has updated running-schema,
                        // and has requested to sync startup-schema with running-schema.
                        // So, copy running-schema to startup-schema, and mark startup-schem valid, save cfg etc.
                        // Note : following function is also called at the end in triggerStartupFileReplay () function.

                        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler : FrameworkToolKit::getRewriteStartupSchemaOnNodeReady returned TRUE.");

                        status = setupStartupAfterConfigReplay ();
                    }

                    if (fileReplay == false)
                    {
                        ConfigReplayStartedEvent* pConfigReplayStartedEvent = new ConfigReplayStartedEvent(false);
                        broadcast (pConfigReplayStartedEvent);
                    }

                    pPrismFrameworkObjectManager->setConfigReplayCompleted(true);
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: ConfigReplayCompleted");
                    ConfigReplayCompletedEvent* pConfigReplayCompletedEvent = new ConfigReplayCompletedEvent(recoveryType);
                    broadcast (pConfigReplayCompletedEvent);
                    isConfigReplayCompleteEventLogged = true;
                    trace (TRACE_LEVEL_PERF_END, "Config Replay.");
                }

                isAllCommandReadyEvent = true;
                break;
        }
    }
    else if ((isPostbootRequired == false) && (executeConfigReplaySequence == false))
    {
		// This is the case of Ccmd which need neitehr postboot nor config replay
        switch (pPrismPostPersistenceBootMessage->getEventId())
        {
            case POSTBOOT_EVENT_SERVICE_READY:
                isActivationCompleteEvent = true;
                break;

           case POSTBOOT_EVENT_SYSTEM_READY:
                isAllCommandReadyEvent = true;
                break;
        }
    }

    if (true == executeNodeReadySequence)
    { 
        if (isAllCommandReadyEvent == true)
        {
            pPrismFrameworkObjectManager->setNodeReadyForAllCommands(true);
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler:NodeReadyForAllCommands");
            NodeReadyForAllCommandsEvent* pNodeReadyForAllCommandsEvent = new NodeReadyForAllCommandsEvent(recoveryType);
            broadcast (pNodeReadyForAllCommandsEvent);

            if (isBannerOutputToClientsRequired)
            {
                //Send a message to all waveclient sessions 

                WaveClientSessionContext tempWaveClientSessionContext;
                tempWaveClientSessionContext.setWaveClientOriginatingLocationId(FrameworkToolKit::getThisLocationId());
                pPrismFrameworkObjectManager->printfToAllWaveClientSessions(tempWaveClientSessionContext,"%s%s%s%s","\n","**********************************************************************\n","System initialization is complete. NOS is ready to handle all commands\n","**********************************************************************\n"); 

                // To avoid configReplayComplete event being broadcast twice and trigger plugin duplicate the handling and other related issues 
                if (isConfigReplayCompleteEventLogged == false)
                {
                    ConfigReplayCompletedEvent* pConfigReplayCompletedEvent = new ConfigReplayCompletedEvent(recoveryType);
                    broadcast (pConfigReplayCompletedEvent);
                }
            }
        }
        else if (isActivationCompleteEvent == true)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootMessageHandler: ActivationComplete");
            pPrismFrameworkObjectManager->setActivationCompleted (true);
        }
	}
    if (skipReply == false)
    {
        pPrismPostPersistenceBootMessage->setCompletionStatus (status);
        reply (pPrismPostPersistenceBootMessage);
    }
}

void PrismFrameworkObjectManagerPostPersistentBootWorker::postPersistentBootServiceAndWaveSlotsMessageHandler (PrismPostPersistenceBootServiceAndWaveSlotsMessage *pPrismPostPersistenceBootServiceAndWaveSlotsMessage)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext = new PrismPostPersistentBootWorkerClusterContext (0, 0, WAVE_HA_COLD_RECOVERY);
    prismAssert (NULL != pPostPersistentBootWorkerClusterContext, __FILE__, __LINE__);

    status = triggerPostBootPassTableForAll (pPostPersistentBootWorkerClusterContext);

    delete (pPostPersistentBootWorkerClusterContext);

    pPrismPostPersistenceBootServiceAndWaveSlotsMessage->setCompletionStatus (status);
    reply (pPrismPostPersistenceBootServiceAndWaveSlotsMessage);
}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::copyRunningToStart ()
{
    DatabaseObjectManagerExecuteCopySchemaMessage message (OrmRepository::getWaveCurrentSchema (), OrmRepository::getWaveStartSchema ());

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::copyRunningToStart");

    ResourceId status = sendSynchronously (&message);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::copyRunningToStart : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::copyRunningToStart : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::copyRunningToStart : Completion Status : " + FrameworkToolKit::localize (status));
        }
    }

    return (status);
}

string PrismFrameworkObjectManagerPostPersistentBootWorker::createReplayString (const string &path, const string &params, const string &file)
{
    //ofstream loadfile ("/etc/fabos/Dcmd/loadconfig.sh");

    string contents = path + " " + params + " " + file;

//    string contents = path + " " + params + " << EOF\n";
//    contents += "configure terminal\n";
 //   contents += "load merge " + file + "\n";
 //   contents += "EOF\n";

    //loadfile << contents;
    //loadfile.close ();

    return contents;
}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForAll ( PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext )
{
    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    ResourceId recoveryType = pPostPersistentBootWorkerClusterContext->getRecoveryType();

    //Trigger postboot for Service Ready
    pPostPersistentBootWorkerClusterContext->initializeContext ( POSTBOOT_GLOBAL_CONFIG, 0, recoveryType );
    status = triggerPostBootPassTableForEvent ( pPostPersistentBootWorkerClusterContext ); 

    if ( WAVE_MESSAGE_SUCCESS != status )
    {   
        trace (TRACE_LEVEL_FATAL, string("PrismFrameworkObjectManager::triggerPostBootPassTableForAll : postboot for Global-Config failed with status -") + FrameworkToolKit::localize ( status ) );
        return( status );
    }

    //Trigger postboot for Slots
    BitMap linecardReadySlotIdBitmap;
    UI32 maxLinecards;

    Wave::getLinecardReadySlotIdBitMap ( linecardReadySlotIdBitmap, maxLinecards );

    if ( 1 <= maxLinecards  )
    {   
        PrismMutex postbootMutex;
        PrismCondition postbootSynchronizingCondition (&postbootMutex);

        pPostPersistentBootWorkerClusterContext->initializeContext ( POSTBOOT_LINECARD_CONFIG, 0, recoveryType, NULL, &postbootMutex, &postbootSynchronizingCondition );
        UI32 postbootTriggeredlinecards = 0;

        // Acquire lock so that any new threads created for linecard postboot 
        // cannot signal until this thread calls wait
        postbootMutex.lock();

        for ( UI32 i = 0 ; i < maxLinecards ; i++ )
        {
            if ( linecardReadySlotIdBitmap[i] )
            {
                pPostPersistentBootWorkerClusterContext->setParameter ( i );
                status  = triggerPostBootPassTableForEvent ( pPostPersistentBootWorkerClusterContext );

                if ( WAVE_MESSAGE_SUCCESS != status )
                {   
                    trace (TRACE_LEVEL_FATAL, string("PrismFrameworkObjectManager::triggerPostBootPassTableForAll : postboot for Linecard failed for Slot ") + i + ("with status -") + FrameworkToolKit::localize ( status ) );
                    // Make sure to release lock for all error cases
                    postbootMutex.unlock();
                    return ( status );
                }
                else
                {
                    postbootTriggeredlinecards++;
                }
            }
        }

        if ( 1 <= postbootTriggeredlinecards )
        {
            // if we have triggered for atleast single linecard then wait here
            postbootSynchronizingCondition.wait();
        }

        postbootMutex.unlock();
    }
    
    return ( status );
}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent ( const PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext )
{
    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : Entered"));

    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    UI32 eventId = pPostPersistentBootWorkerClusterContext->getEventId();
    UI32 parameter = pPostPersistentBootWorkerClusterContext->getParameter();
    UI32 recoveryType = pPostPersistentBootWorkerClusterContext->getRecoveryType();

    if (POSTBOOT_LINECARD_CONFIG == eventId)
    {   
        trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : starting Postboot thread for POSTBOOT_LINECARD_CONFIG"));

        PrismPostbootAgentThreadContext *pPrismPostbootAgentThreadContext = new PrismPostbootAgentThreadContext ();
        prismAssert (NULL != pPrismPostbootAgentThreadContext, __FILE__, __LINE__);

        PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage = NULL;
        pPrismPostPersistenceBootMessage = pPostPersistentBootWorkerClusterContext->getPostbootMessagePointer();

        if ( NULL != pPrismPostPersistenceBootMessage )
        {
            // Since message is not null, we can reply 
            pPrismPostbootAgentThreadContext->setPostbootMessagePointer ( pPrismPostPersistenceBootMessage );
        }
        else if ( NULL != pPostPersistentBootWorkerClusterContext->getPostbootMutex() )
        {
            // Since message is null, we can signal the waiting thread    
            pPrismPostbootAgentThreadContext->setPostbootMutex ( pPostPersistentBootWorkerClusterContext->getPostbootMutex() );
            pPrismPostbootAgentThreadContext->setPostbootSynchronizingCondition ( pPostPersistentBootWorkerClusterContext->getPostbootSynchronizingCondition() );
        }
        
        PrismPostbootAgentThread *pPrismPostbootAgentThread = new PrismPostbootAgentThread(m_pWaveObjectManager, eventId, parameter, recoveryType, pPrismPostbootAgentThreadContext);
        WaveThreadStatus threadStatus = pPrismPostbootAgentThread->run ();

        if (WAVE_THREAD_SUCCESS != threadStatus)
        {   
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : PrismPostbootAgent thread failed with status:" + FrameworkToolKit::localize (threadStatus));
            status = threadStatus ;
        }
        else
        {   
            trace (TRACE_LEVEL_SUCCESS, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : PrismPostbootAgent thread status success");
        }
    }
    else
    {   
        PrismPostbootAgent  *pPrismPostbootAgent = new PrismPostbootAgent(m_pWaveObjectManager, eventId, parameter, recoveryType);
        prismAssert (NULL != pPrismPostbootAgent, __FILE__, __LINE__);
        status = pPrismPostbootAgent->execute();

        if ( status != WAVE_MESSAGE_SUCCESS )
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : PostbootAgent failed with status:" + FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTableForEvent : PostbootAgent Completed : " + FrameworkToolKit::localize (status));
        }
        delete (pPrismPostbootAgent);
    }    
    
    return (status);

}

void PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTable (const UI32 &eventId, const UI32 &parameter, PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage)
{
    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTable : send for Triggering POSTBOOT TABLE"));

    PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext = new PrismPostPersistentBootWorkerClusterContext ( eventId, parameter, pPrismPostPersistenceBootMessage->getRecoveryType(), pPrismPostPersistenceBootMessage );

    status = triggerPostBootPassTableForEvent ( pPostPersistentBootWorkerClusterContext );
    delete (pPostPersistentBootWorkerClusterContext);

    if ( status != WAVE_MESSAGE_SUCCESS )
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerPostBootPassTable : Postboot failed with status:" + FrameworkToolKit::localize (status));
    }

}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // create link to startup in /var/config/vcs/scripts directory
    string configDir = string (ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/");
    string linkName = string("bootTimeReplayFile");
    string replayString = string("ln -s " + FrameworkToolKit::getStartupFileName () + " " + configDir + linkName);
      
    int ret = system (replayString.c_str ());

    trace (TRACE_LEVEL_INFO,   string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : ") + replayString + " ret = " + WEXITSTATUS(ret));

    if (-1 == ret)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Could not execute " + replayString);
        prismAssert (false, __FILE__, __LINE__);
        return WAVE_MESSAGE_ERROR;
    }

    //replayString = string("export ROLE_ID=root\nexport HOME=/root\nexport LOGNAME=root\nexport PWD=/root\n/usr/WyserEa/bin/WyserEaCli -C << EOF\ncopy flash://" + linkName + " running-config") + string ("\nEOF\n");
    replayString = string("export ROLE_ID=root\nexport HOME=/root\nexport LOGNAME=root\nexport PWD=/root\n/fabos/libexec/ConfigCmd.Linux.powerpc --copy flash://" + linkName + " running-config") + string ("\nEOF\n");

    errno = 0;
    ret = system (replayString.c_str ());
    int errnovalue = errno;

    trace (TRACE_LEVEL_INFO,   string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : ") + replayString + " ret = " + WEXITSTATUS(ret));

    // on x86 simulator, /usr/WyserEa/bin/WyserEaCli does not exist. Do not assert if that is the case.
    if ((0 != ret) && (127 != (WEXITSTATUS(ret))))
    {
        if (256 == ret)
        {
            trace (TRACE_LEVEL_INFO,   string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Detected simulator environment and running it without root privileges."));

            // On simulator environments where private x86 machines are setuip with correct WyserEa directories, it will return EPERM.  So, ignore it and return it.

            //replayString = string("/usr/WyserEa/bin/WyserEaCli -C << EOF\ncopy flash://" + linkName + " running-config") + string ("\nEOF\n");
            replayString = string("/vobs/projects/springboard/fabos/src/vcs/dcm/application/ConfigCmd.Linux.i686 --copy flash://" + linkName + " running-config") + string ("\nEOF\n");
            ret = system (replayString.c_str ());

            trace (TRACE_LEVEL_INFO,   string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : ") + replayString + " ret = " + WEXITSTATUS(ret));
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Could not execute " + replayString + string(" errno = ") + errnovalue);
            prismAssert (false, __FILE__, __LINE__);
            return WAVE_MESSAGE_ERROR;
        }
    }

    // remove the previously created link
    replayString = string("rm -f " + configDir + linkName);

    ret = system (replayString.c_str ());

    trace (TRACE_LEVEL_INFO,   string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : ") + replayString + " ret = " + WEXITSTATUS(ret));

    if (-1 == ret)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Could not execute " + replayString);
        prismAssert (false, __FILE__, __LINE__);
        return WAVE_MESSAGE_ERROR;
    }


    /* Call the virtual function ConfigReplayEnd so that the plugins get to merge the configuration based on their internal requirements */
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Start the Config Replay End .... ");

    ConfigReplayEndAgent *pPrismConfigReplayEnd = new ConfigReplayEndAgent (m_pWaveObjectManager);

    pPrismConfigReplayEnd->execute ();

    delete pPrismConfigReplayEnd;

    //  Copy running to start

    status = setupStartupAfterConfigReplay ();

    return status;
}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::setupStartupAfterConfigReplay ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // Is the following check necessary? Or should running-schema be copied to startup-schema even when cluster is enabled ?

    bool clusterEnabled = Wave::clusterEnabledCheck ();

    if (false == clusterEnabled)
    {
        resetXPathStringsTimestampsInPersistenceObjectManager ();

        status = copyRunningToStart ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR,   "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Could not copy running to start");
            prismAssert (false, __FILE__, __LINE__);
            return WAVE_MESSAGE_ERROR;
        }
    }

    //  Set startup file to valid
    FrameworkToolKit::setIsStartupValid (true);

    //  Clear startup file name 
    FrameworkToolKit::setStartupFileName ("");

    // Save persistent configuration
    status = FrameworkToolKit::savePrismConfiguration ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR,   "PrismFrameworkObjectManagerPostPersistentBootWorker::triggerStartupFileReplay : Could not save configuration");
        prismAssert (false, __FILE__, __LINE__);
        return WAVE_MESSAGE_ERROR;
    }

    return status;
}

ResourceId PrismFrameworkObjectManagerPostPersistentBootWorker::triggerLastConfigReplay ()
{
    UI32        numberOfConfigurationIntents    = WaveConfigurationIntentRepository::getConfigurationIntentRepositorySize ();
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerLastConfigReplay : Number of configuration intents found : ") + numberOfConfigurationIntents + ".");

    if (1 <= numberOfConfigurationIntents)
    {
        vector<UI32>        configurationIntentVector;
        map<UI32, string>   configurationIntentMap;
        string              serializedConfigurationIntentMessage    = "";
        PrismMessage       *pConfigurationIntentMessage             = NULL;
        PrismServiceId      configurationIntentServiceCode          = 0;
        UI32                configurationIntentOperationCode        = 0;
        ResourceId          overallStatus                           = WAVE_MESSAGE_SUCCESS;

        WaveConfigurationIntentRepository::getConfigurationIntentRepository (configurationIntentVector, configurationIntentMap);

        WaveConfigurationIntentRepository::clearConfigurationIntentRepository ();

        // The size of the configuration intent repository should remain the same

        prismAssert (numberOfConfigurationIntents == configurationIntentVector.size (), __FILE__, __LINE__);

        // We have the capability to replay multiple configuration intents, but purposefully we will only replay the last config intent.
        // The index of the last configuration intent will be used here.  It's okay to subtract one from the index here since we are guaranteed at least one intent at this point.

        UI32 lastConfigurationIntentIndex = numberOfConfigurationIntents - 1;
        
        serializedConfigurationIntentMessage = configurationIntentMap[configurationIntentVector[lastConfigurationIntentIndex]];

        pConfigurationIntentMessage = PrismMessage::createAndLoadFromSerializedData2 (serializedConfigurationIntentMessage);
        prismAssert (NULL != pConfigurationIntentMessage, __FILE__, __LINE__);

        pConfigurationIntentMessage->setIsALastConfigReplay (true);

        configurationIntentServiceCode = pConfigurationIntentMessage->getServiceCode ();
        configurationIntentOperationCode = pConfigurationIntentMessage->getOperationCode ();

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::triggerLastConfigReplay : Executing last config replay. Message handler service code : ") + configurationIntentServiceCode + " (" + FrameworkToolKit::getServiceNameById (configurationIntentServiceCode) + "), operation code : " + configurationIntentOperationCode + ", serialized intent message : " + serializedConfigurationIntentMessage + ", serialized intent message size : " + serializedConfigurationIntentMessage.size ());

        status = sendSynchronously (pConfigurationIntentMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            overallStatus = status;
        }

        delete pConfigurationIntentMessage;
        pConfigurationIntentMessage = NULL;

        // Generate a single RASLOG for the last configuration replay using event.

        LastConfigReplayCompletedEvent *plastConfigReplayCompletedEvent = new LastConfigReplayCompletedEvent (overallStatus);
        prismAssert (NULL != plastConfigReplayCompletedEvent, __FILE__, __LINE__);

        broadcast (plastConfigReplayCompletedEvent);
    }
    else if (0 == numberOfConfigurationIntents)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        // All cases covered above.
    }

    return (status);
}

void PrismFrameworkObjectManagerPostPersistentBootWorker::resetXPathStringsTimestampsInPersistenceObjectManager ()
{
    // About the issue -
    //
    // Whenever line-by-line config replay is done, in the end, all timestamps should be reset. (because, during config-replay, timestamps are not updated.)
    // This is done at the end of actual config-replay.
    //
    // But, in the following sequence -
    //      t0. create Vlan 20   (Timestamp of /interface-vlan/interface/vlan is t0) Lets assume that description leaf for this leaf has value "PQR"
    //      t1. copy running-config startup-config
    //      t2. change description of Vlan 20 to "ABC" (Timestamp of /interface-vlan/interface/vlan is t2)
    //      t3. reboot : Note : here, line-by-line config-replay does not happen.
    //      t4. startup-schema is copied over running-schema. Here, timestamp of /interface-vlan/interface/vlan should be reset to t4.
    //          Otherwise old value (t0) will be visible to northbound client(s)/manager(s).
    //
    // i.e. While doing copy startupschema to runningschema -
    //      Find the pathstrings for which timestamps in startup and running schema differ.
    //      Update timestamps for the pathstrings.                                          ...(A)
    //
    // This problem cannot be solved even if in pathString managed object, empty-needed-on-persistent-boot is set to false.
    // i.e. Persistence Object Manager still needs to do the operatons in (A) during copy of startupschema to runningschema.
    //
    // However, "copy startupschema to runningschema" does not come through PersistenceObjectManager.
    //
    //
    // This issue can be looked upon in another way -
    // Framwork should properly reset the lastUpdateTT for each of the managed objects for which lastUpdatedTT in running and startup schema's is different
    // i.e. from above example, Framework should reset the lastUpdateTT of Vlan 20 MO to t4. Otherwise, the last-modified-timestamps are incorrect.
    // When Framework does it for all Other managed Objects, then the timestamps of XPath Managed Objects will automatically get updated properly.
    // Currently, this is NOT happening.
    // So, in the sequence above, BNA will think that Vlan 20 still has description "ABC", but in fact, at t4, the description got changed to "PQR".
    //
    // i.e. BNA WILL GET OLD TIMESTAMP VALUES FROM XPATH/FEATURE_GROUP MANAGED OBJETS.
    //      Anyway, in such case, the timestamps will not match with those before reboot, which would be treated as config-change.

    PersistenceObjectManagerResetXPathStringsTimestampsMessage persistenceObjectManagerResetXPathStringsTimestampsMessage;

    ResourceId sendStatus = sendSynchronously (&persistenceObjectManagerResetXPathStringsTimestampsMessage);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::resetXPathStringsTimestampsInPersistenceObjectManager : could not send PersistenceObjectManagerResetXPathStringsTimestampsMessage. sendStatus [") + (FrameworkToolKit::localizeToSourceCodeEnum (sendStatus)) + string ("]"));
    }
    else
    {
        ResourceId completionStatus = persistenceObjectManagerResetXPathStringsTimestampsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::resetXPathStringsTimestampsInPersistenceObjectManager : PersistenceObjectManagerResetXPathStringsTimestampsMessage processing failed. completionStatus[") + (FrameworkToolKit::localizeToSourceCodeEnum (completionStatus)) + string ("]"));
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, string ("PrismFrameworkObjectManagerPostPersistentBootWorker::resetXPathStringsTimestampsInPersistenceObjectManager : Success."));
        }
    }
}

}
