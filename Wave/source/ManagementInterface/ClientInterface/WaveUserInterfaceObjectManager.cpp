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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveClientDebug.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestTypes.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerTypes.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerMessages.h"
#include "ManagementInterface/ClientInterface/UnifiedClientUpdateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCreateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientDeleteMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientPostbootMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientNotifyMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientGetDataFromClientMessage.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Core/Wave.h"
#include "Framework/Core/WaveClient.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

namespace WaveNs
{

ManagementInterfaceMessageHandlerForClient                   WaveUserInterfaceObjectManager::m_managementInterfaceMessageHandlerForClient;
ManagementClientInterfaceToLoadCStructureOutput              WaveUserInterfaceObjectManager::m_managementClientInterfaceToLoadCStructureOuput;
ManagementInterfaceMessageInstantiatorForClient              WaveUserInterfaceObjectManager::m_managementInterfaceMessageInstantiatorForClient;
map<UI32, ManagementInterfaceMessageHandlerForCStyleClient>  WaveUserInterfaceObjectManager::m_managementInterfaceMessageHandlerMapForCStyleClient;
PrismMutex                                                   WaveUserInterfaceObjectManager::m_managementInterfaceMessageHandlerMapForCStyleClientMutex;
map<UI32, ManagementInterfaceMessageCallbackHandlerAtClient> WaveUserInterfaceObjectManager::m_managementInterfaceMessageCallbackHandlerAtCStyleClient;
PrismMutex                                                   WaveUserInterfaceObjectManager::m_managementInterfaceMessageCallbackHandlerAtCStyleClientMutex;
map<string, ManagementInterfaceMessageInstantiatorForClient> WaveUserInterfaceObjectManager::m_managementInterfaceMessageInstantiatorForClients;
PrismMutex                                                   WaveUserInterfaceObjectManager::m_managementInterfaceMessageInstantiatorForClientsMutex;
string                                                       WaveUserInterfaceObjectManager::m_ttyName;

WaveUserInterfaceObjectManager::WaveUserInterfaceObjectManager ()
    : WaveObjectManager (getPrismServiceName ())
{
	setTraceLevel (TRACE_LEVEL_INFO);

    addDebugFunction ((ShellCmdFunction) (&WaveClientDebug::debugConnect), "connecttoserver");
    addDebugFunction ((ShellCmdFunction) (&WaveClientDebug::debugClose),   "closeserver");
    addOperationMap (WAVE_UPDATE_CLIENT_STATUS, reinterpret_cast<PrismMessageHandler> (&WaveUserInterfaceObjectManager::updateClientStatusFromServerMessageHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&WaveUserInterfaceObjectManager::genericManagementInterfaceMessageHandler));

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);

    m_ttyName = FrameworkToolKit::getCurrentTtyName ();
}

WaveUserInterfaceObjectManager::~WaveUserInterfaceObjectManager ()
{
}

WaveUserInterfaceObjectManager *WaveUserInterfaceObjectManager:: getInstance ()
{
    static WaveUserInterfaceObjectManager *pWaveUserInterfaceObjectManager = new WaveUserInterfaceObjectManager ();

    WaveNs::prismAssert (NULL != pWaveUserInterfaceObjectManager, __FILE__, __LINE__);

    return (pWaveUserInterfaceObjectManager);
}

PrismServiceId WaveUserInterfaceObjectManager:: getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string WaveUserInterfaceObjectManager::getPrismServiceName ()
{
    return ("Wave User Interface");
}

PrismMessage *WaveUserInterfaceObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {

        case UNIFIEDCLIENTUPDATE:
            pPrismMessage = new UnifiedClientUpdateMessage ();
            break;
        case UNIFIEDCLIENTCREATE:
            pPrismMessage = new UnifiedClientCreateMessage ();
            break;
        case UNIFIEDCLIENTDELETE:
            pPrismMessage = new UnifiedClientDeleteMessage ();
            break;
        case UNIFIEDCLIENTPOSTBOOT:
            pPrismMessage = new UnifiedClientPostbootMessage ();
            break;
        case  UNIFIEDCLIENTNOTIFY:
            pPrismMessage = new UnifiedClientNotifyMessage ();
            break;
        case UNIFIEDCLIENTGETDATA:
            pPrismMessage = new UnifiedClientGetDataFromClientMessage();
            break;
        case WAVE_UPDATE_CLIENT_STATUS:
             pPrismMessage = new WaveUpdateClientStatusMessage( );
             break;
        default :
            pPrismMessage = NULL;
    }

    if (!pPrismMessage)
    {
        if (NULL == m_managementInterfaceMessageInstantiatorForClient)
        {
            trace (TRACE_LEVEL_FATAL, "WaveUserInterfaceObjectManager::createMessageInstance : User defined Management Interface Message Instantiator has NOT been set.");
            trace (TRACE_LEVEL_FATAL, "WaveUserInterfaceObjectManager::createMessageInstance : Please use WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient to set it.");

            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            pPrismMessage = (*m_managementInterfaceMessageInstantiatorForClient) (operationCode);

            if (NULL == pPrismMessage)
            {
                trace (TRACE_LEVEL_FATAL, "WaveUserInterfaceObjectManager::createMessageInstance : User defined Management Interface Message Instantitor returned NULL.");

                prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            }
        }
    }

    return (pPrismMessage);
}

void WaveUserInterfaceObjectManager::setManagementInterfaceMessageHandlerForClient (ManagementInterfaceMessageHandlerForClient managementInterfaceMessageHandlerForClient)
{
    m_managementInterfaceMessageHandlerForClient = managementInterfaceMessageHandlerForClient;
}

void WaveUserInterfaceObjectManager::setClientHandlerToLoadOutputForCStructure (ManagementClientInterfaceToLoadCStructureOutput managementInterfaceMessageHandlerForClient)
{
    m_managementClientInterfaceToLoadCStructureOuput = managementInterfaceMessageHandlerForClient;
}

void WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (ManagementInterfaceMessageInstantiatorForClient managementInterfaceMessageInstantiatorForClient)
{
    m_managementInterfaceMessageInstantiatorForClient = managementInterfaceMessageInstantiatorForClient;
}

void WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (const string &clientName, ManagementInterfaceMessageInstantiatorForClient managementInterfaceMessageInstantiatorForClient)
{
    m_managementInterfaceMessageInstantiatorForClientsMutex.lock ();

    map<string, ManagementInterfaceMessageInstantiatorForClient>::const_iterator element   = m_managementInterfaceMessageInstantiatorForClients.find (clientName);
    map<string, ManagementInterfaceMessageInstantiatorForClient>::const_iterator endElement = m_managementInterfaceMessageInstantiatorForClients.end ();

    if (element != endElement)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient : Message Innstantiator for Client \'" + clientName + "\' has already been set.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    m_managementInterfaceMessageInstantiatorForClients[clientName] = managementInterfaceMessageInstantiatorForClient;

    m_managementInterfaceMessageInstantiatorForClientsMutex.unlock ();
}

ManagementInterfaceMessageInstantiatorForClient WaveUserInterfaceObjectManager::getManagementInterfaceMessageInstantiatorForClient (const string &clientName)
{
    m_managementInterfaceMessageInstantiatorForClientsMutex.lock ();

    ManagementInterfaceMessageInstantiatorForClient managementInterfaceMessageInstantiatorForClient = NULL;

    map<string, ManagementInterfaceMessageInstantiatorForClient>::const_iterator element   = m_managementInterfaceMessageInstantiatorForClients.find (clientName);
    map<string, ManagementInterfaceMessageInstantiatorForClient>::const_iterator endElement = m_managementInterfaceMessageInstantiatorForClients.end ();

    if (element == endElement)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient : Message Innstantiator for Client \'" + clientName + "\' has not been set.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    managementInterfaceMessageInstantiatorForClient = m_managementInterfaceMessageInstantiatorForClients[clientName];

    m_managementInterfaceMessageInstantiatorForClientsMutex.unlock ();

    return (managementInterfaceMessageInstantiatorForClient);
}

void WaveUserInterfaceObjectManager::replyToWaveServer (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    (WaveUserInterfaceObjectManager::getInstance ())->reply (pManagementInterfaceMessage);
}

WaveMessageStatus WaveUserInterfaceObjectManager::sendToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, ManagementInterfaceMessageCallbackHandlerAtClient messageCallback, void *pInputContext, UI32 timeOutInMilliSeconds, LocationId locationId, PrismElement *pPrismMessageSender)
{
    pManagementInterfaceMessage->setServiceCode (WaveClientTransportObjectManager::getPrismServiceId ());
    pManagementInterfaceMessage->setServerId (waveServerId);
    pManagementInterfaceMessage->setTtyName (m_ttyName);

    addMessageCallbackHandlerAtClient (pManagementInterfaceMessage->getMessageId (), messageCallback);

    return (send (pManagementInterfaceMessage, reinterpret_cast<PrismMessageResponseHandler> (&WaveUserInterfaceObjectManager::sendToWaveServerCallback), pInputContext, timeOutInMilliSeconds, locationId, pPrismMessageSender));
}

void WaveUserInterfaceObjectManager::sendToWaveServerCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, void *pInputContext)
{
    SI32 callbackStatus = frameworkStatus;

    if (NULL != pManagementInterfaceMessage)
    {
		bool isLastReply	= pManagementInterfaceMessage->getIsLastReply ();
    
        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            callbackStatus = pManagementInterfaceMessage->getCompletionStatus ();
        }

        UI32 orignalMessageID = pManagementInterfaceMessage->getWaveClientMessageId ();

        ManagementInterfaceMessageCallbackHandlerAtClient handler = getMessageCallbackHandlerAtClient (orignalMessageID, isLastReply);

        void *outputContext = m_managementClientInterfaceToLoadCStructureOuput (pManagementInterfaceMessage);

        prismAssert (NULL != outputContext, __FILE__, __LINE__);
    
        handler (callbackStatus, outputContext, pInputContext);

        if (true == isLastReply)
        {
            delete pManagementInterfaceMessage;
        } 
    }
}

WaveMessageStatus WaveUserInterfaceObjectManager::sendOneWayToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId)
{
    pManagementInterfaceMessage->setServiceCode (WaveClientTransportObjectManager::getPrismServiceId ());
    pManagementInterfaceMessage->setServerId (waveServerId);
    pManagementInterfaceMessage->setTtyName (m_ttyName);

    return (sendOneWay (pManagementInterfaceMessage, locationId));
}

WaveMessageStatus WaveUserInterfaceObjectManager::sendSynchronouslyToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId)
{
    pManagementInterfaceMessage->setServiceCode (WaveClientTransportObjectManager::getPrismServiceId ());
    pManagementInterfaceMessage->setServerId (waveServerId);
    pManagementInterfaceMessage->setTtyName (m_ttyName);

    return (sendSynchronously (pManagementInterfaceMessage, locationId));
}

void WaveUserInterfaceObjectManager::genericManagementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    ResourceId                                        status              = WAVE_MGMT_CLIENT_INTF_CLIENT_NO_MESSAGE_HANDLER;
    /*UI32                                              operationCode       = pManagementInterfaceMessage->getOperationCode ();
    bool                                              isFrameworkOpCode   = false;
    const void                                       *pInput              = NULL;
    UnifiedClientUpdateMessage                       *updateMessage       = NULL;
    UnifiedClientCreateMessage                       *createMessage       = NULL;
    UnifiedClientDeleteMessage                       *deleteMessage       = NULL;
    UnifiedClientPostbootMessage                     *postbootMessage     = NULL;
    UnifiedClientNotifyMessage                       *notifyMessage       = NULL;
    UnifiedClientGetDataFromClientMessage            *getDataMessage      = NULL;

    switch (operationCode)
    {

        case UNIFIEDCLIENTUPDATE:
            {
                isFrameworkOpCode = true;
                updateMessage = dynamic_cast<UnifiedClientUpdateMessage *> ( pManagementInterfaceMessage);
                prismAssert (NULL != updateMessage, __FILE__, __LINE__);
                pInput = updateMessage->getCStructureForInputs ();
            }
            break;
        case UNIFIEDCLIENTCREATE:
            {
                isFrameworkOpCode = true;
                createMessage = dynamic_cast<UnifiedClientCreateMessage *> (pManagementInterfaceMessage);
                prismAssert (NULL != createMessage, __FILE__, __LINE__);
                pInput = createMessage->getCStructureForInputs ();
            }
            break;
        case UNIFIEDCLIENTDELETE:
            {
                isFrameworkOpCode = true;
                deleteMessage = dynamic_cast<UnifiedClientDeleteMessage *> (pManagementInterfaceMessage);
                prismAssert (NULL != deleteMessage, __FILE__, __LINE__);
                pInput = deleteMessage->getCStructureForInputs ();
            }
            break;
        case UNIFIEDCLIENTPOSTBOOT:
            {
                isFrameworkOpCode = true;
                postbootMessage = dynamic_cast<UnifiedClientPostbootMessage *> (pManagementInterfaceMessage);
                prismAssert (NULL != postbootMessage, __FILE__, __LINE__);
                pInput = postbootMessage->getCStructureForInputs ();
            }
            break;
        case UNIFIEDCLIENTNOTIFY:
            {
                isFrameworkOpCode = true;
                notifyMessage = dynamic_cast<UnifiedClientNotifyMessage *> (pManagementInterfaceMessage);
                prismAssert (NULL != notifyMessage, __FILE__, __LINE__);
                pInput = notifyMessage->getCStructureForInputs ();
            }
            break;
        case UNIFIEDCLIENTGETDATA:
            {   
                isFrameworkOpCode = true;
                getDataMessage = dynamic_cast<UnifiedClientGetDataFromClientMessage *>(pManagementInterfaceMessage);
                prismAssert (NULL != getDataMessage, __FILE__, __LINE__);
                pInput = getDataMessage->getCStructureForInputs ();
            }
            break;
        default :
            isFrameworkOpCode = false;
    }

    if (isFrameworkOpCode)
    {
        ManagementInterfaceMessageHandlerForCStyleClient  handler       = WaveUserInterfaceObjectManager::getMessageHandlerForCStyleApplication (operationCode);
        SI32                                              handlerStatus = 0;
        void                                             *pOutput       = NULL;

        if (NULL != handler)
        {
            trace (TRACE_LEVEL_DEVEL, string ("WaveUserInterfaceObjectManager::genericManagementInterfaceMessageHandler : operationCode=") + operationCode);

            handlerStatus = (handler) (pInput, &pOutput);

            trace (TRACE_LEVEL_DEVEL, string ("WaveUserInterfaceObjectManager::genericManagementInterfaceMessageHandler : handler execution complete"));
            
            fflush(stdout);

            switch (operationCode)
            {
                case UNIFIEDCLIENTUPDATE:
                    updateMessage->deleteCStructureForInputs (pInput);
                    updateMessage->loadOutputsFromCStructure(pOutput);
                    break;
                case UNIFIEDCLIENTCREATE:
                    createMessage->deleteCStructureForInputs (pInput);
                    createMessage->loadOutputsFromCStructure(pOutput);
                    break;
                case UNIFIEDCLIENTDELETE:
                    deleteMessage->deleteCStructureForInputs (pInput);
                    deleteMessage->loadOutputsFromCStructure(pOutput);
                    break;
                case UNIFIEDCLIENTPOSTBOOT:
                    postbootMessage->deleteCStructureForInputs (pInput);
                    break;
                case UNIFIEDCLIENTNOTIFY:
                    notifyMessage->deleteCStructureForInputs (pInput);
                    break;
                case UNIFIEDCLIENTGETDATA:
                    getDataMessage->deleteCStructureForInputs (pInput);
                    getDataMessage->loadOutputsFromCStructure(pOutput);
                    break;
            }

            if ((handlerStatus == WAVE_MESSAGE_SUCCESS) || (!handlerStatus))
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                 status = handlerStatus;
            }

            pManagementInterfaceMessage->setCompletionStatus (status);
            reply (pManagementInterfaceMessage);

        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("WaveUserInterfaceObjectManager::genericManagementInterfaceMessageHandler : Handler not registered. Please register the handler for the operationCode ") + operationCode);
            prismAssert (false , __FILE__, __LINE__);
        }
    }
    else
    {
        if (NULL != m_managementInterfaceMessageHandlerForClient)
        {
            status = (*m_managementInterfaceMessageHandlerForClient) (pManagementInterfaceMessage);
        }

        if (WAVE_MGMT_CLIENT_INTF_CLIENT_ASYNC_REPSONSE == status)
        {
            return;
        }
        else
        {
            pManagementInterfaceMessage->setCompletionStatus (status);
            reply (pManagementInterfaceMessage);
        }
    }*/

    if (NULL != m_managementInterfaceMessageHandlerForClient)
    {
        status = (*m_managementInterfaceMessageHandlerForClient) (pManagementInterfaceMessage);
    }

    if (WAVE_MGMT_CLIENT_INTF_CLIENT_ASYNC_REPSONSE == status)
    {
        return;
    }
    else
    {
        pManagementInterfaceMessage->setCompletionStatus (status);
        reply (pManagementInterfaceMessage);
    }

    
}

void WaveUserInterfaceObjectManager::updateClientStatusFromServerMessageHandler ( WaveUpdateClientStatusMessage *pWaveUpdateClientStatusMessage )
{
    SI32  status = 0;
    status = WaveClient::updateClientStatusFunction( pWaveUpdateClientStatusMessage );
    if (0 != status)
    {
        pWaveUpdateClientStatusMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("WaveUserInterfaceObjectManager::updateClientStatusFromServerMessageHandler : Update error#  ") + status );
    }
    else
    {
        pWaveUpdateClientStatusMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    reply(pWaveUpdateClientStatusMessage);
}


void WaveUserInterfaceObjectManager::addMessageHandlerForCStyleApplication (const UI32 &operationCode, ManagementInterfaceMessageHandlerForCStyleClient handler)
{
    m_managementInterfaceMessageHandlerMapForCStyleClientMutex.lock ();

    map<UI32, ManagementInterfaceMessageHandlerForCStyleClient>::iterator element    = m_managementInterfaceMessageHandlerMapForCStyleClient.find (operationCode);
    map<UI32, ManagementInterfaceMessageHandlerForCStyleClient>::iterator endElement = m_managementInterfaceMessageHandlerMapForCStyleClient.end  ();

    if (endElement == element)
    {
        m_managementInterfaceMessageHandlerMapForCStyleClient[operationCode] = handler;
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, string ("WaveUserInterfaceObjectManager::addMessageHandlerForCStyleApplication : Handler for Operation Code ") + operationCode + " already exists.");

        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    m_managementInterfaceMessageHandlerMapForCStyleClientMutex.unlock ();
}

ManagementInterfaceMessageHandlerForCStyleClient WaveUserInterfaceObjectManager::getMessageHandlerForCStyleApplication (const UI32 &operationCode)
{
    ManagementInterfaceMessageHandlerForCStyleClient handler = NULL;

    m_managementInterfaceMessageHandlerMapForCStyleClientMutex.lock ();

    map<UI32, ManagementInterfaceMessageHandlerForCStyleClient>::iterator element    = m_managementInterfaceMessageHandlerMapForCStyleClient.find (operationCode);
    map<UI32, ManagementInterfaceMessageHandlerForCStyleClient>::iterator endElement = m_managementInterfaceMessageHandlerMapForCStyleClient.end  ();

    if (endElement != element)
    {
        handler = m_managementInterfaceMessageHandlerMapForCStyleClient[operationCode];
    }

    m_managementInterfaceMessageHandlerMapForCStyleClientMutex.unlock ();

    return (handler);
}

void WaveUserInterfaceObjectManager::addMessageCallbackHandlerAtClient (UI32 messageId, ManagementInterfaceMessageCallbackHandlerAtClient messageCallback)
{
    m_managementInterfaceMessageCallbackHandlerAtCStyleClientMutex.lock ();

    map<UI32, ManagementInterfaceMessageCallbackHandlerAtClient>::iterator element    = m_managementInterfaceMessageCallbackHandlerAtCStyleClient.find (messageId);
    map<UI32, ManagementInterfaceMessageCallbackHandlerAtClient>::iterator endElement = m_managementInterfaceMessageCallbackHandlerAtCStyleClient.end  ();

    if (endElement == element)
    {
        m_managementInterfaceMessageCallbackHandlerAtCStyleClient [messageId] = messageCallback;
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, string ("WaveUserInterfaceObjectManager::addMessageCallbackHandlerAtClient: Handler for this message Id is already added once. You can not re-use same message pointer to send twice."));

        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    m_managementInterfaceMessageCallbackHandlerAtCStyleClientMutex.unlock ();
}

ManagementInterfaceMessageCallbackHandlerAtClient WaveUserInterfaceObjectManager::getMessageCallbackHandlerAtClient (const UI32 &messageId, const bool &isLastReply)
{
    ManagementInterfaceMessageCallbackHandlerAtClient handler = NULL;

    m_managementInterfaceMessageCallbackHandlerAtCStyleClientMutex.lock ();

    map<UI32, ManagementInterfaceMessageCallbackHandlerAtClient>::iterator element    = m_managementInterfaceMessageCallbackHandlerAtCStyleClient.find (messageId);
    map<UI32, ManagementInterfaceMessageCallbackHandlerAtClient>::iterator endElement = m_managementInterfaceMessageCallbackHandlerAtCStyleClient.end  ();

    if (endElement != element)
    {
        handler = m_managementInterfaceMessageCallbackHandlerAtCStyleClient [messageId];
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Last reply for this message already received");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == isLastReply)
    {
        m_managementInterfaceMessageCallbackHandlerAtCStyleClient.erase (element);
    }

    m_managementInterfaceMessageCallbackHandlerAtCStyleClientMutex.unlock ();

    return (handler);
}

}
