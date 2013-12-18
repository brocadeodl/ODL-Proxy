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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ServiceInterface/ApplicationLocalService.h"
#include "Framework/ServiceInterface/ApplicationServiceMessages.h"
#include "Framework/ServiceInterface/ApplicationServiceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

ApplicationServiceRepository *ApplicationLocalService::m_pApplicationServiceRepository = NULL;

ApplicationLocalService::ApplicationLocalService (const string &applicationLocalServiceName, const UI32 &applicationLocalServiceId)
    : WaveLocalObjectManager (applicationLocalServiceName),
      m_applicationLocalServiceId (applicationLocalServiceId)
{
    addOperationMap (WAVE_OBJECT_MANAGER_ANY_OPCODE, reinterpret_cast<PrismMessageHandler> (&ApplicationLocalService::applicationLocalServiceMessageHandler));

    map<UI32, ApplicationServiceMessageHandler> *pHandlersMap = ApplicationServiceHelper::getHandlersMapForApplicationService (m_applicationLocalServiceId);

    if (NULL != pHandlersMap)
    {
        map<UI32, ApplicationServiceMessageHandler>::iterator element    = pHandlersMap->begin ();
        map<UI32, ApplicationServiceMessageHandler>::iterator endElement = pHandlersMap->end ();

        while (element != endElement)
        {
            addMessageHandlerToThisApplicationLocalService (element->first, element->second);

            element++;
        }
    }
}

ApplicationLocalService::~ApplicationLocalService ()
{
}

PrismMessage *ApplicationLocalService::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case APPLICATION_SERVICE_GENERIC_MESSAGE:
            pPrismMessage = new ApplicationServiceMessage ();
            break;
        default:
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

UI32 ApplicationLocalService::registerNewApplicationLocalService (const string &newApplicationLocalServiceName)
{
    UI32 newApplicationLocalServiceId = 0;

    if (NULL == m_pApplicationServiceRepository)
    {
        m_pApplicationServiceRepository = ApplicationServiceRepository::getInstance ();
    }

    ApplicationServiceHelper::lock ();

    newApplicationLocalServiceId = ApplicationServiceHelper::getNextApplicationServiceId ();

    m_pApplicationServiceRepository->addNewApplicationLocalService (newApplicationLocalServiceId, newApplicationLocalServiceName);

    ApplicationServiceHelper::unlock ();

    return (newApplicationLocalServiceId);
}

void ApplicationLocalService::createNewApplicationLocalServices ()
{
    ApplicationServiceHelper::lock ();

    if (NULL != m_pApplicationServiceRepository)
    {
        m_pApplicationServiceRepository->createApplicationLocalServices ();
    }

    ApplicationServiceHelper::unlock ();
}

void ApplicationLocalService::addMessageHandler (const UI32 &applicationServiceId, const UI32 &operationCode, ApplicationServiceMessageHandler handler)
{
    ApplicationServiceHelper::lock ();

    ApplicationServiceHelper::addMessageHandler (applicationServiceId, operationCode, handler);

    ApplicationServiceHelper::unlock ();
}

void ApplicationLocalService::addMessageHandlerToThisApplicationLocalService (const UI32 &operationCode, ApplicationServiceMessageHandler handler)
{
    map<UI32, ApplicationServiceMessageHandler>::iterator element    = m_applicaionLocalServiceMessageHandlersMap.find (operationCode);
    map<UI32, ApplicationServiceMessageHandler>::iterator endElement = m_applicaionLocalServiceMessageHandlersMap.end ();

    if (element != endElement)
    {
        trace (TRACE_LEVEL_FATAL, "ApplicationLocalService::addMessageHandlerToThisApplicationLocalService : Handler already registered for operation code : " + operationCode);
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_applicaionLocalServiceMessageHandlersMap[operationCode] = handler;
    }
}

void ApplicationLocalService::applicationLocalServiceMessageHandler (ApplicationServiceMessage *pApplicationServiceMessage)
{
    UI32  operationCode = pApplicationServiceMessage->getOperationCode ();
    void *pPayLoad      = NULL;
    UI32  payLoadLength = 0;

    pPayLoad = pApplicationServiceMessage->getInputBuffer1 (payLoadLength);

    ApplicationServiceMessageHandler pMessageHandler = m_applicaionLocalServiceMessageHandlersMap[operationCode];

    if (NULL == pMessageHandler)
    {
        trace (TRACE_LEVEL_FATAL, "ApplicationLocalService::applicationServiceMessageHandler : No Handler registered for operation code : " + operationCode);
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        ApplicationServiceMessageHandlerContext *pApplicationServiceMessageHandlerContext = new  ApplicationServiceMessageHandlerContext (this, pApplicationServiceMessage);

        prismAssert (NULL != pApplicationServiceMessageHandlerContext, __FILE__, __LINE__);

        (*pMessageHandler) (pPayLoad, payLoadLength, pApplicationServiceMessageHandlerContext);
    }
}

void ApplicationLocalService::applicationLocalServiceMessageCallback (FrameworkStatus frameworkStatus, ApplicationServiceMessage *pApplicationServiceMessage, void *pContext)
{
    ApplicationServiceSendContext *pApplicationServiceSendContext = reinterpret_cast<ApplicationServiceSendContext *> (pContext);
    void                          *pOutputPayLoad                 = NULL;
    UI32                           outputPayLoadLength            = 0;
    ResourceId                     messageStatus                  = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("ApplicationService::applicationLocalServiceMessageCallback : Application Message failed, Framework Status : ") + frameworkStatus);
    }

    if (NULL != pApplicationServiceMessage)
    {
        pOutputPayLoad = pApplicationServiceMessage->transferBufferToUser (APPLICATION_SERVICE_GENERIC_MESSAGE_OUTPUT_BUFFER1, outputPayLoadLength);
        messageStatus  = pApplicationServiceMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("ApplicationService::applicationLocalServiceMessageCallback : Message sent to Application Service returned error : ") + messageStatus);
        }

        delete pApplicationServiceMessage;
    }

    prismAssert (NULL != pApplicationServiceSendContext, __FILE__, __LINE__);

    ApplicationServiceCallback  pApplicationServiceCallback = pApplicationServiceSendContext->getPApplicationServiceCallback ();
    void                       *pApplicationServiceContext  = pApplicationServiceSendContext->getPApplicationServiceContext ();

    delete pApplicationServiceSendContext;

    if (NULL != pApplicationServiceCallback)
    {
        (*pApplicationServiceCallback) (pOutputPayLoad, outputPayLoadLength, pApplicationServiceContext);
    }
}

}
