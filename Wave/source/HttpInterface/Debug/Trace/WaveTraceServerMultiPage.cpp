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

#include "HttpInterface/Debug/Trace/WaveTraceServerMultiPage.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <Framework/Utils/Integer.h>

namespace WaveNs
{

WaveTraceServerMultiPage::WaveTraceServerMultiPage (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path)
    : WaveServerMultiPage (pHttpInterfaceReceiverObjectManager, path)
{
    setPageHeading ("Trace Controls");

    addWaveServerMultiPageRequestHandlerForGet ("List",                reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Set Level",           reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Set Level For All",   reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Reset Level For All", reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Legend",              reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Help",                reinterpret_cast<WaveServerMultiPageRequestHandler> (&WaveTraceServerMultiPage::getForList));
}

WaveTraceServerMultiPage::~WaveTraceServerMultiPage ()
{
}

void WaveTraceServerMultiPage::getForList (const HttpRequest &httpRequest)
{
    vector<TraceClientId> traceClientIdsVector;
    vector<TraceLevel>    traceLevelsVector;
    UI32                  nServices             = 0;
    UI32                  index                 = 0;
    string                serviceName;
    string                traceLevelName;

    TraceObjectManager::getClientsInformationDirectly (traceClientIdsVector, traceLevelsVector);

    nServices = traceClientIdsVector.size ();

    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForGet (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + getPageHeading() + "</H2>\r\n";
    httpResponseString += "<H3 STYLE=\"COLOR:RGB(0, 0, 255);TEXT-ALIGN:CENTER;\">List</H3>\r\n";

    if (0 < nServices)
    {
        httpResponseString += "<TABLE BORDER=0 \">\r\n";

        httpResponseString += "<TH>#</TH>\r\n";
        httpResponseString += "<TH>Service Name</TH>\r\n";
        httpResponseString += "<TH>Trace Level</TH>\r\n";

        for (index = 0; index < nServices; index++)
        {
            TraceClientId traceClientId = traceClientIdsVector[index];
            TraceLevel    traceLevel    = traceLevelsVector[index];

            serviceName    = FrameworkToolKit::getTraceClientNameById (traceClientId);
            traceLevelName = FrameworkToolKit::localize               (traceLevel);

            if (0 == (index % 2))
            {
                httpResponseString += "<TR STYLE=\"BACKGROUND-COLOR:RGB(200, 200, 200);\">\r\n";
            }
            else
            {
                httpResponseString += "<TR STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255);\">\r\n";
            }

            httpResponseString += "<TD>" + ((Integer (index + 1)).toString ()) + "</TD>\r\n";
            httpResponseString += "<TD>" + serviceName                         + "</TD>\r\n";
            httpResponseString += "<TD>" + traceLevelName                      + "</TD>\r\n";

            httpResponseString += "</TR>\r\n";
        }

        httpResponseString += "</TABLE>\r\n";
    }

    getWaveServerPagePostPortionForGet (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }

}

}
