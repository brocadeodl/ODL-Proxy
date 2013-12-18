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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/Postboot/PrismPostbootAgent.h"
#include "Framework/Postboot/PrismPostbootAgentContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Framework/Core/PostBootStartedEvent.h"
#include "Framework/Core/PostBootCompletedEvent.h"
#include "../postboot/PostBoot.h"

namespace WaveNs
{

// read this from the xml generated file
static map <UI32, vector <postbootPass> > registrations; 

PrismPostbootAgent::PrismPostbootAgent (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager, false)
{
}

PrismPostbootAgent::PrismPostbootAgent (WaveObjectManager *pWaveObjectManager, const UI32 &event, const UI32 &parameter, const UI32 &recoveryType)
    : WaveWorker (pWaveObjectManager, false),
     m_eventId    (event),
     m_parameter  (parameter),
     m_recoveryType (recoveryType)
{
}

PrismPostbootAgent::~PrismPostbootAgent ()
{
}

ResourceId PrismPostbootAgent::execute ()
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::mergeStaticRegistrationsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::notifyPostbootStartedStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::sendPostbootPassStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::notifyPostbootCompletedStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismPostbootAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismPostbootAgentContext *pPrismPostbootAgentContext = new PrismPostbootAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    trace(TRACE_LEVEL_DEBUG, string("PrismPostbootAgent::execute Triggering execution of sequencerSteps"));
    ResourceId status = pPrismPostbootAgentContext->execute ();

    return (status);
}

ResourceId PrismPostbootAgent::getListOfEnabledServicesStep (PrismPostbootAgentContext *pPrismPostbootAgentContext)
{
    unsigned int    i = 0;

    trace(TRACE_LEVEL_INFO, string("PrismPostbootAgent::getListOfEnabledServicesStep Entered"));

    vector<PrismServiceId> &enabledServices = pPrismPostbootAgentContext->getEnabledServices ();

    WaveObjectManager::getListOfEnabledServices (enabledServices);

    trace(TRACE_LEVEL_DEBUG, string("Enabled services are:"));

    for (i=0; i<enabledServices.size(); i++)
    {
        trace(TRACE_LEVEL_DEBUG, string(" ") + enabledServices[i]);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void PrismPostbootAgent::populateRegistrations (map <UI32, vector <postbootPass> > registrationsTable)
{
    registrations = registrationsTable;
    return;
}

// Merge global postboot pass table with the one maintined by wave
// Wave framework provides APIs also to insert elements in this table. So,
// if any plugin uses those APIs, then also we are safe

ResourceId PrismPostbootAgent::mergeStaticRegistrationsStep (PrismPostbootAgentContext *pPrismPostbootAgentContext)
{
    trace(TRACE_LEVEL_INFO, string("PrismPostbootAgent::mergeStaticRegistrationsStep Entered"));

    m_postbootTable = registrations;
    printRegistrationTable();

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismPostbootAgent::notifyPostbootStartedStep (PrismPostbootAgentContext *pPrismPostbootAgentContext)
{
    trace(TRACE_LEVEL_DEVEL, string("PrismPostbootAgent::notifyPostbootStartedStep : Entered"));

    ResourceId  status  = WAVE_MESSAGE_ERROR;

    PostBootStartedEvent * pPostBootStartedEvent = new PostBootStartedEvent(m_eventId, m_parameter);
    ResourceId postboostStartBrocadcastStatus = broadcast(pPostBootStartedEvent);

    if (WAVE_MESSAGE_SUCCESS == postboostStartBrocadcastStatus)
    {
        status  = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;   
    }
    
    return (status);
}

ResourceId PrismPostbootAgent::sendPostbootPassStep (PrismPostbootAgentContext *pPrismPostbootAgentContext)
{
    vector<PrismServiceId> &enabledServices = pPrismPostbootAgentContext->getEnabledServices ();
    UI8                     passNum              = 0;
    UI8                     serviceIdIndex       = 0;
    UI8                     checkIdIndex         = 0;
    UI8                     enabledIndex         = 0;
    SI8                     serviceFound         = 0;
    PrismServiceId          serviceId;
    string                  passName;
    UI32                    recoveryTypeForThisService;

    trace(TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep Entered"));

    if ( m_postbootTable.size() == 0 ) 
    {
        trace(TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: Wave pass table is empty. No messages to be sent!"));
        return (WAVE_MESSAGE_SUCCESS);
    }

    // !! hack !! until HA support is ready for L3 config replay for warm recovery
    //string  envVar;
    //envVar = FrameworkToolKit::getProcessInitialWorkingDirectory() + "/triggerPostbootOnWarmRecovery";
    //trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: opening file:") + envVar);
    //ifstream tmpPostbootTriggerFile(envVar.c_str());
    
    // For all the pass rows in the table
    for (passNum = 0; passNum < m_postbootTable[m_eventId].size(); passNum++)
    {
        passName = (m_postbootTable[m_eventId])[passNum].passName;
        trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent:: postboot table:") + m_eventId + string(" pass:") + passNum + string(" name:") + passName);

        // verify if the #services present in the vector == #entries in recovery vector
        if (((m_postbootTable[m_eventId])[passNum].serviceId).size() != ((m_postbootTable[m_eventId])[passNum].recoveryType).size())
        {
            trace (TRACE_LEVEL_FATAL, string("PrismPostbootAgent::sendPostbootPassStep: #entries in serviceId vector != #entries in recoveryType vector!!. Check the postboot cpp generation process"));
            prismAssert(false, __FILE__, __LINE__);
        }

        for (serviceIdIndex = 0; serviceIdIndex < ((m_postbootTable[m_eventId])[passNum].serviceId).size(); serviceIdIndex++)
        {
            serviceId = (m_postbootTable[m_eventId])[passNum].serviceId[serviceIdIndex];

            // skip holes in the pass row
            if (serviceId == 0) // reserved ID & a hole
            {
                trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: Index:") + serviceIdIndex + string(" in pass:") + passNum + string(" is empty. Skipping"));
                continue;
            }

            // if the recovery type for this boot is WARM and for this postboot entry is not WARM, skip it
            recoveryTypeForThisService = (m_postbootTable[m_eventId])[passNum].recoveryType[serviceIdIndex];

            if ( WAVE_HA_WARM_RECOVERY == m_recoveryType )
            {
                // note that the same serviceIdIndex is applicable for recoveryType vector as well
                if ( !(recoveryTypeForThisService & WAVE_RECOVERY_TYPE_WARM) )
                {
                    trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: boot recovery type is WARM, but for service ") + FrameworkToolKit::getServiceNameById (serviceId) + string(" with passNum:") + passNum + string(" passName:") + passName + string(" recovery type is not warm as per postboot xml. Hence skipping"));
                    continue;
                }

                // !! hack !! demand a file ~/triggerPostbootOnWarmRecovery to be created for postboot support for warm recovery now
                //if (!tmpPostbootTriggerFile)
                //{
                //    trace (TRACE_LEVEL_ERROR, string("PrismPostbootAgent::sendPostbootPassStep: boot recovery type is WARM, but file:") + envVar + string(" is not present. Create it if you want WARM recovery support for postboot now"));
                //    continue;
                //}
            }
            else if ( WAVE_HA_COLD_RECOVERY == m_recoveryType )
            {
                if ( !(recoveryTypeForThisService & WAVE_RECOVERY_TYPE_COLD) )
                {
                    trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: boot recovery type is COLD, but for service ") + FrameworkToolKit::getServiceNameById (serviceId) + string(" with passNum:") + passNum + string(" passName:") + passName + string(" recovery type is not cold as per postboot xml. Hence skipping"));
                    continue;
                }
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, string("PrismPostbootAgent::sendPostbootPassStep: boot recovery type is UNKNOWN!! ") + m_recoveryType +string(" hence skipping") );
                continue;
            }

            // check if this service ID is repeated in the pass row
            for (checkIdIndex = serviceIdIndex+1; checkIdIndex < ((m_postbootTable[m_eventId])[passNum].serviceId).size(); checkIdIndex++)
            {
                if (serviceId == (m_postbootTable[m_eventId])[passNum].serviceId[checkIdIndex])
                {
                    trace (TRACE_LEVEL_FATAL, string("PrismPostbootAgent::sendPostbootPassStep: Duplicate serviceID:") + serviceId + string(" not allowed in same pass!"));
                    prismAssert(false, __FILE__, __LINE__);
                }
            }
            
            // first check if the service id in postboot table is actually enabled
            serviceFound = 0;
            for (enabledIndex = 0; enabledIndex < enabledServices.size(); enabledIndex++)
            {
                if (serviceId == enabledServices[enabledIndex])
                {
                    serviceFound = 1;
                    break;
                }
            }
            if (serviceFound == 0)
            {
                trace (TRACE_LEVEL_WARN, string("PrismPostbootAgent::sendPostbootPassStep: ServiceId:") + serviceId + string(" is not enabled!"));
                continue;
            }
            
            if (false == requiresPostboot(serviceId))
            {
                trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: Postboot required is false for ServiceId:") + serviceId);
                continue;
            }

            // Create a message with the serviceId & the pass number. This message will be sent to the plugins

            trace (TRACE_LEVEL_INFO, string("PrismPostbootAgent::sendPostbootPassStep: Sending postboot message to serviceId:") + FrameworkToolKit::getServiceNameById (serviceId) + string(" with passNum:") + passNum + string(" passName:") + passName + string(" parameter:") + m_parameter + string(" recoveryType:") + m_recoveryType);
            PrismPostbootObjectManagerMessage prismPostbootObjectManagerMessage (serviceId, passNum, passName, m_parameter, m_recoveryType);

            ResourceId status = sendSynchronously (&prismPostbootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::sendPostbootPassStep : Could not Postboot a service : " + FrameworkToolKit::getServiceNameById (serviceId));
                prismAssert (false, __FILE__, __LINE__);
                return (status);
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != prismPostbootObjectManagerMessage.getCompletionStatus ())
                {
                    trace (TRACE_LEVEL_FATAL, string("PrismPostbootAgent::sendPostbootPassStep: Failed to send message"));
                }
                else
                {
                    trace (TRACE_LEVEL_DEBUG, string("PrismPostbootAgent::sendPostbootPassStep: Message sent successfully"));
                }
            }
        }
    }
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismPostbootAgent::notifyPostbootCompletedStep (PrismPostbootAgentContext *pPrismPostbootAgentContext)
{
    trace(TRACE_LEVEL_DEVEL, string("PrismPostbootAgent::notifyPostbootCompletedStep :  Entered"));

    ResourceId  status  = WAVE_MESSAGE_ERROR;

    PostBootCompletedEvent * pPostBootCompletedEvent = new PostBootCompletedEvent(m_eventId, m_parameter);
    ResourceId postboostCompleteBrocadcastStatus = broadcast(pPostBootCompletedEvent);

    if (WAVE_MESSAGE_SUCCESS == postboostCompleteBrocadcastStatus)
    {
        status  = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;   
    }

    return (status);
}

bool PrismPostbootAgent::requiresPostboot (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void PrismPostbootAgent::printRegistrationTable (void)
{
    UI8                     tableNum             = 0;
    UI8                     passNum              = 0;
    UI8                     serviceIdIndex       = 0;
    PrismServiceId          serviceId;

    cout << endl;
    cout << "==============================================" << endl;
    cout << "PrismPostbootAgent::printRegistrationTable =>" << endl;
    cout << "==============================================";

    map<UI32, vector <postbootPass> >::iterator it;

  // show content:
    for ( it = m_postbootTable.begin() ; it != m_postbootTable.end(); it++ )
    {
        tableNum = (UI8)(it->first);
        cout << endl << "For table" << (int)tableNum << " ";
 
        for (passNum = 0; passNum < m_postbootTable[tableNum].size(); passNum++)
        {
            cout << endl << "For pass:" << (int)passNum << "name:" << (m_postbootTable[tableNum])[passNum].passName;
        cout << "ServiceIDs are: ";

            for (serviceIdIndex = 0; serviceIdIndex < ((m_postbootTable[tableNum])[passNum].serviceId).size(); serviceIdIndex++)
            {
                serviceId = (m_postbootTable[tableNum])[passNum].serviceId[serviceIdIndex];
            cout << serviceId << " ";
            }
        }
    }
    cout << endl;
    cout << "==============================================" << endl;
}

}
