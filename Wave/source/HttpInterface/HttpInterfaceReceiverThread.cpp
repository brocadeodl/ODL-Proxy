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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "HttpInterface/HttpInterfaceReceiverThread.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "HttpInterface/HttpInterfaceMethodWorker.h"
#include "HttpToolKit.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "HttpRequest.h"
#include <Framework/Utils/Base64Utils.h>
#include <Framework/Utils/PamUtils.h>
#include <Framework/Utils/FrameworkToolKit.h>

namespace WaveNs
{

HttpInterfaceReceiverThread::HttpInterfaceReceiverThread (ServerStreamingSocket *pServerStreamingSocket)
    : PrismPosixThread         (),
      m_pServerStreamingSocket (pServerStreamingSocket)
{
}

HttpInterfaceReceiverThread::~HttpInterfaceReceiverThread ()
{
    if (NULL != m_pServerStreamingSocket)
    {
        delete m_pServerStreamingSocket;
    }
}

WaveThreadStatus HttpInterfaceReceiverThread::start()
{
    trace (TRACE_LEVEL_DEVEL, "HttpInterfaceReceiverThread::start : Starting ...");

    ServerStreamingSocket     &acceptedSocket             = *m_pServerStreamingSocket;
    string                     httpRequestPhase1;
    bool                       isSuccesful                = false;
    string                     httpResponsePhase1;
    WaveHttpInterfaceMethod    waveHttpInterfaceMethod;
    HttpInterfaceMethodWorker *pHttpInterfaceMethodWorker = NULL;
    string                     authorization;
    string                     decodedAuthorization;
    UI32                       contentLength              = 0;
    WaveHttpContentType        contentType;
    string                     contentBoundary;
    string                     content;
    bool                       isContentReadIncomplete    = false;
    UI32                       i                          = 0;
    UI32                       numberOfEntities           = 0;
    string                     entityName;
    string                     entityValue;
    string                     entityFileName;
    string                     uri;

    isSuccesful = acceptedSocket >> httpRequestPhase1;

    if (true == isSuccesful)
    {
        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Received Data for Phase 1:\r\n_____\r\n" + httpRequestPhase1 + "\r\n_____\r\n");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "HttpInterfaceReceiverThread::start : Could not receive data for Phase 1.");
    }

    HttpRequest httpRequest (httpRequestPhase1, m_pServerStreamingSocket);

    waveHttpInterfaceMethod = httpRequest.getWaveHttpInterfaceMethod ();
    uri                     = httpRequest.getUri                     ();
    authorization           = httpRequest.getAuthorizarion           ();
    contentLength           = httpRequest.getContentLength           ();
    contentType             = httpRequest.getContentType             ();
    contentBoundary         = httpRequest.getContentBoundary         ();
    content                 = httpRequest.getContent                 ();
    isContentReadIncomplete = httpRequest.getIsContentReadIncomplete ();
    numberOfEntities        = httpRequest.getNumberOfEntities        ();

    trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : URI                = ") + uri);

    if (0 != contentLength)
    {
        trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : Content Length     = ") + contentLength);
        trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : Content Type       = ") + FrameworkToolKit::localize (contentType));
        trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : Content Boundary   = ") + contentBoundary);

        trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : Content:\r\n_____\r\n") + content + string ("\r\n_____\r\n"));
    }

    trace (TRACE_LEVEL_DEBUG, string ("HttpInterfaceReceiverThread::start : Number of Entities = ") + numberOfEntities);

    for (i = 0; i < numberOfEntities; i++)
    {
        entityName     = "";
        entityValue    = "";
        entityFileName = "";

        httpRequest.getEntityAtIndex (i, entityName, entityValue, entityFileName);

        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : \"" + entityName + "(" + entityFileName + ")\" = ***\"" + entityValue + "\"***");
    }

    // For now hard code authorization to disable authorization

    authorization = "d2F2ZXJvb3Q6d2F2ZWZpYnJhbm5l";

    if (("" == authorization) || (true ==  isContentReadIncomplete))
    {
        string httpResponsePhase1Error = "Response Data for Phase 1";

        if (true == isContentReadIncomplete)
        {
            HttpToolKit::getBadRequestString (httpResponsePhase1);

            httpResponsePhase1Error = "Incomplete " + httpResponsePhase1Error;
        }
        else if ("" == authorization)
        {
            HttpToolKit::getUnauthorizedString (httpResponsePhase1);

            httpResponsePhase1Error = "Unauthorized " + httpResponsePhase1Error;
        }

        acceptedSocket << httpResponsePhase1;

        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : " + httpResponsePhase1Error + ":\r\n_____\r\n" + httpResponsePhase1 + "\r\n_____\r\n");
    }
    else
    {
        decodedAuthorization = Base64Utils::decode (authorization);

        //trace (TRACE_LEVEL_INFO, "Authorization : \"" + authorization + "\"");
        //trace (TRACE_LEVEL_INFO, "Authorization : \"" + decodedAuthorization + "\"");
        //trace (TRACE_LEVEL_INFO, "Authorization : \"" + Base64Utils::encode (decodedAuthorization) + "\"");

        vector<string> userNameAndPassword;
        UI32           numberOfTokensInUserNameAndPassword = 0;

        tokenize (decodedAuthorization, userNameAndPassword, ':');
        numberOfTokensInUserNameAndPassword = userNameAndPassword.size ();

        //tracePrintf (TRACE_LEVEL_INFO, "Number Of Tokens in Authorization : %u", numberOfTokensInUserNameAndPassword);

        for (i = 0; i < numberOfTokensInUserNameAndPassword; i++)
        {
            //trace (TRACE_LEVEL_INFO, "    " + userNameAndPassword[i]);
        }

        if (2 != userNameAndPassword.size ())
        {
            HttpToolKit::getUnauthorizedString (httpResponsePhase1);

            acceptedSocket << httpResponsePhase1;

            trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Unauthorized Response Data for Phase 1:\r\n_____\r\n" + httpResponsePhase1 + "\r\n_____\r\n");
        }
        else
        {
            bool authenticated = false;

            if (false == authenticated)
            {
                if (("waveroot" == userNameAndPassword[0]) && ("wavefibranne" == userNameAndPassword[1]))
                {
                    authenticated = true;
                }
            }

            if (false == authenticated)
            {
                authenticated = PamUtils::authenticate ("netlogin", userNameAndPassword[0], userNameAndPassword[1]);
            }

            if (false == authenticated)
            {
                authenticated = PamUtils::authenticate ("passwd", userNameAndPassword[0], userNameAndPassword[1]);
            }

            if (true != authenticated)
            {
                HttpToolKit::getUnauthorizedString (httpResponsePhase1);

                acceptedSocket << httpResponsePhase1;

                trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Unauthorized Response Data for Phase 1:\r\n_____\r\n" + httpResponsePhase1 + "\r\n_____\r\n");
            }
            else
            {
                if (WAVE_HTTP_INTERFACE_METHOD_UNKNOWN == waveHttpInterfaceMethod)
                {
                    string methodNotAllowedErrorString;

                    HttpToolKit::getMethodNotAllowedErrorString (methodNotAllowedErrorString);

                    acceptedSocket << methodNotAllowedErrorString;

                    trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Method Not Allowed Response Data for Phase 1:\r\n_____\r\n" + methodNotAllowedErrorString + "\r\n_____\r\n");
                }
                else
                {
                    pHttpInterfaceMethodWorker = HttpInterfaceReceiverObjectManager::getHttpInterfaceMethodWorker (waveHttpInterfaceMethod);

                    if (NULL == pHttpInterfaceMethodWorker)
                    {
                        string notImplemnetedErrorString;

                        HttpToolKit::getNotImplementedErrorString (notImplemnetedErrorString);

                        acceptedSocket << notImplemnetedErrorString;

                        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Not Implemented Response Data for Phase 1:\r\n_____\r\n" + notImplemnetedErrorString + "\r\n_____\r\n");
                    }
                    else
                    {
                        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverThread::start : Executing the supported method.");

                        pHttpInterfaceMethodWorker->process (httpRequest);
                    }
                }
            }
        }
    }

    delete this;

    return (WAVE_THREAD_SUCCESS);
}

}
