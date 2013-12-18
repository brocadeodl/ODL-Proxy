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

#include "HttpInterface/HttpInterfacePutWorker.h"
#include "HttpToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <Framework/Utils/Integer.h>
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include <algorithm>
#include "WaveServerPage.h"
#include "WaveServerPageDirectory.h"
#include "Framework/Utils/TraceUtils.h"

using namespace std;

namespace WaveNs
{

HttpInterfacePutWorker::HttpInterfacePutWorker (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager)
    : HttpInterfaceMethodWorker (pHttpInterfaceReceiverObjectManager, WAVE_HTTP_INTERFACE_METHOD_PUT)
{
}

HttpInterfacePutWorker::~HttpInterfacePutWorker ()
{
}

void HttpInterfacePutWorker::process (const HttpRequest &httpRequest)
{
    string                 uri                    = httpRequest.getUri                         ();
    WaveServerPage        *pWaveServerPage        = WaveServerPageDirectory::getWaveServerPage (uri);
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    if (NULL == pWaveServerPage)
    {
        string httpResponse;

        trace (TRACE_LEVEL_DEBUG, "HttpInterfacePutWorker::process : There is no page registered with the given path \"" + uri + "\"");

        HttpToolKit::getNotFoundErrorString (httpResponse);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "HttpInterfacePutWorker::process : Serving post for \"" + uri + "\"");

        pWaveServerPage->put (httpRequest);
    }
}

}
