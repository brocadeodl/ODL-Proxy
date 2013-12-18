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

#include "Framework/ServiceInterface/ApplicationServiceUtils.h"
#include "Framework/ServiceInterface/ApplicationServiceRepository.h"
#include "Framework/ServiceInterface/ApplicationService.h"
#include "Framework/ServiceInterface/ApplicationLocalService.h"
#include "Framework/ServiceInterface/ApplicationServiceMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

ApplicationServiceSendContext::ApplicationServiceSendContext (ApplicationServiceCallback pApplicationServiceCallback, void *pApplicationServiceContext)
    : m_pApplicationServiceCallback (pApplicationServiceCallback),
      m_pApplicationServiceContext  (pApplicationServiceContext)
{
}

ApplicationServiceSendContext::~ApplicationServiceSendContext ()
{
}

ApplicationServiceCallback  ApplicationServiceSendContext::getPApplicationServiceCallback ()
{
    return (m_pApplicationServiceCallback);
}

void *ApplicationServiceSendContext::getPApplicationServiceContext ()
{
    return (m_pApplicationServiceContext);
}

ApplicationServiceMessageHandlerContext::ApplicationServiceMessageHandlerContext (WaveObjectManager *pWaveObjectManager, PrismMessage *pPrismMessage)
    : m_pWaveObjectManager (pWaveObjectManager),
     m_pPrismMessage        (pPrismMessage)
{
}

ApplicationServiceMessageHandlerContext::~ApplicationServiceMessageHandlerContext ()
{
}

WaveObjectManager *ApplicationServiceMessageHandlerContext::getPWaveObjectManager ()
{
    return (m_pWaveObjectManager);
}

PrismMessage *ApplicationServiceMessageHandlerContext::getPPrismMessage ()
{
    return (m_pPrismMessage);
}

PrismServiceId ApplicationServiceUtils::getPrismServiceIdForApplicationServiceId (const UI32 &applicationServiceId)
{
    PrismServiceId                applicationServicePrismServiceId = 0;
    ApplicationServiceRepository *pApplicationServiceRepository    = NULL;

    ApplicationServiceHelper::lock ();

    pApplicationServiceRepository = ApplicationServiceRepository::getInstance ();

    if (true == (pApplicationServiceRepository->isAnApplicationLocalService (applicationServiceId)))
    {
        ApplicationLocalService *pApplicationLocalService = pApplicationServiceRepository->getApplicationLocalService (applicationServiceId);

        prismAssert (NULL != pApplicationLocalService, __FILE__, __LINE__);

        applicationServicePrismServiceId = pApplicationLocalService->getServiceId ();
    }
    else
    {
        ApplicationService *pApplicationService = pApplicationServiceRepository->getApplicationService (applicationServiceId);

        prismAssert (NULL != pApplicationService, __FILE__, __LINE__);

        applicationServicePrismServiceId = pApplicationService->getServiceId ();
    }

    ApplicationServiceHelper::unlock ();

    return (applicationServicePrismServiceId);
}

ResourceId ApplicationServiceUtils::sendToApplicationService (const PrismServiceId &sendingApplicationPrismServiceId, void *pPayLoad, const UI32 &payLoadLength, const PrismServiceId &receivingApplicationprismServiceId, const LocationId &prismLocationId, ApplicationServiceCallback pApplicationServiceCallback, void *pApplicationContext)
{
    ApplicationServiceMessage *pApplicationServiceMessage = new ApplicationServiceMessage (getPrismServiceIdForApplicationServiceId (receivingApplicationprismServiceId));
    ResourceId                 status                     = WAVE_MESSAGE_ERROR;

    if (NULL != pPayLoad)
    {
        pApplicationServiceMessage->addBuffer (APPLICATION_SERVICE_GENERIC_MESSAGE_INPUT_BUFFER1, payLoadLength, pPayLoad, false);
    }

    ApplicationServiceRepository *pApplicationServiceRepository = NULL;

    ApplicationServiceHelper::lock ();

    pApplicationServiceRepository = ApplicationServiceRepository::getInstance ();

    prismAssert (NULL != pApplicationServiceRepository, __FILE__, __LINE__);

    ApplicationServiceSendContext *pApplicationServiceSendContext = new ApplicationServiceSendContext (pApplicationServiceCallback, pApplicationContext);

    prismAssert (NULL != pApplicationServiceSendContext, __FILE__, __LINE__);

    if (true == (pApplicationServiceRepository->isAnApplicationLocalService (sendingApplicationPrismServiceId)))
    {
        ApplicationLocalService *pApplicationLocalService = pApplicationServiceRepository->getApplicationLocalService (sendingApplicationPrismServiceId);

        prismAssert (NULL != pApplicationLocalService, __FILE__, __LINE__);

        status = pApplicationLocalService->send (pApplicationServiceMessage, reinterpret_cast<PrismMessageResponseHandler> (&ApplicationLocalService::applicationLocalServiceMessageCallback), pApplicationServiceSendContext, 0, prismLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("ApplicationServiceUtils::sendToApplicationService : Could not send message to Application Service : ") + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        ApplicationService *pApplicationService = pApplicationServiceRepository->getApplicationService (sendingApplicationPrismServiceId);

        prismAssert (NULL != pApplicationService, __FILE__, __LINE__);

        status = pApplicationService->send (pApplicationServiceMessage, reinterpret_cast<PrismMessageResponseHandler> (&ApplicationService::applicationServiceMessageCallback), pApplicationServiceSendContext, 0, prismLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("ApplicationServiceUtils::sendToApplicationService : Could not send message to Application Service : ") + FrameworkToolKit::localize (status));
        }
    }

    ApplicationServiceHelper::unlock ();

    return (status);
}

void ApplicationServiceUtils::replyToApplicationService (void *pOutputPayLoad, const UI32 outputPayLoadLength, void *pPrismContext)
{
    ApplicationServiceMessageHandlerContext *pApplicationServiceMessageHandlerContext = reinterpret_cast<ApplicationServiceMessageHandlerContext *> (pPrismContext);

    prismAssert (NULL != pApplicationServiceMessageHandlerContext, __FILE__, __LINE__);

    WaveObjectManager *pWaveObjectManager = pApplicationServiceMessageHandlerContext->getPWaveObjectManager ();
    PrismMessage       *pPrismMessage       = pApplicationServiceMessageHandlerContext->getPPrismMessage ();

    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);
    prismAssert (NULL != pPrismMessage,       __FILE__, __LINE__);

    if (NULL != pOutputPayLoad)
    {
        pPrismMessage->addBuffer (APPLICATION_SERVICE_GENERIC_MESSAGE_OUTPUT_BUFFER1, outputPayLoadLength, pOutputPayLoad, false);
    }

    pPrismMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    pWaveObjectManager->reply (pPrismMessage);
}

}
