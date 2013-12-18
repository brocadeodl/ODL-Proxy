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

#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworksWaveServerMultiPage.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/Integer.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkMessage.h"
#include "HttpInterface/HttpToolKit.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Types/UI32Range.h"
#include "HttpInterface/WaveServerPageDirectory.h"

namespace NspNs
{

NspNetworksWaveServerMultiPage::NspNetworksWaveServerMultiPage (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path)
    : WaveServerMultiPage (pHttpInterfaceReceiverObjectManager, path)
{
    WaveServerPageDirectory::registerServerPage("/tenants/*/networks/*", this);

    setPageHeading ("Network Configuration");

    addWaveServerMultiPageRequestHandlerForGet ("List",   reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworksWaveServerMultiPage::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("Create", reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworksWaveServerMultiPage::getForCreate));
}

NspNetworksWaveServerMultiPage::~NspNetworksWaveServerMultiPage ()
{
}

void NspNetworksWaveServerMultiPage::getForListByRangeAndFields (const HttpRequest &httpRequest, const bool &filterByRange)
{
    WaveManagedObjectSynchronousQueryContext queryContext          (NetworkManagedObject::getClassName ());
    string                                   pathForThisPage     = getPath ();
    vector<string>                           uriTokens;
    string                                   rangeString         = "all";
    UI32Range                                networkIdRange;
    UI32                                     networkId;
    vector<UI32>                             networkIds;
    UI32                                     numberOfNetworkIds  = 0;
    UI32                                     i                   = 0;
    string                                   uri                 = httpRequest.getUri ();
    UI32                                     numberOfUriTokens   = 0;

    if (true == filterByRange)
    {
        StringUtils::tokenize (uri, uriTokens, '/');
        numberOfUriTokens = uriTokens.size ();

        if (0 < numberOfUriTokens)
        {
            for (i = 0; i < numberOfUriTokens; i++)
            {
                if ("networks" == uriTokens[i])
                {
                    if ((i + 1) < numberOfUriTokens)
                    {
                        rangeString = uriTokens[i + 1];
                    }
                }
            }

            if (("all" != rangeString) && ("*" != rangeString))
            {
                if (true == (UI32Range::isAValidString (rangeString)))
                {
                    networkIdRange.fromString (rangeString);

                    networkIdRange.getUI32RangeVector (networkIds);
                }
                else
                {
                    networkId = strtoul (rangeString.c_str (), NULL, 10);

                    networkIds.push_back (networkId);
                }
            }
        }

        numberOfNetworkIds = networkIds.size ();

        if (0 < (networkIds.size ()))
        {
            for (i = 0; i < numberOfNetworkIds; i++)
            {
                queryContext.addOrAttribute (new AttributeUI32 (networkIds[i], "networkId"));
            }
        }
    }

    queryContext.addOrderField ("name");

    vector<WaveManagedObject *> *pQueryResults           = querySynchronously (&queryContext);
    UI32                         numberOfNetworks        = 0;
    NetworkManagedObject        *pNetworkManagedObject   = NULL;
    UI32                         index                   = 0;
    string                       tableHeaderNamesString;
    string                       tableRowDataString;

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    numberOfNetworks = pQueryResults->size ();

    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    getWaveServerPagePrePortionForGet (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + getPageHeading() + "</H2>\r\n";
    httpResponseString += "<H3 STYLE=\"COLOR:RGB(0, 0, 255);TEXT-ALIGN:CENTER;\">List</H3>\r\n";

    if (0 < numberOfNetworks)
    {
        httpResponseString += "<TABLE BORDER=0 \">\r\n";

        for (index = 0; index < numberOfNetworks; index++)
        {
            pNetworkManagedObject = dynamic_cast<NetworkManagedObject *> ((*pQueryResults)[index]);

            prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

            if (0 == index)
            {
                httpResponseString += "<TR>\r\n";

                httpResponseString += "<TH>#</TH>\r\n";

                pNetworkManagedObject->getHtmlTableHeaderNamesString (tableHeaderNamesString);

                httpResponseString += tableHeaderNamesString;

                httpResponseString += "</TR>\r\n";
            }

            if (0 == (index % 2))
            {
                httpResponseString += "<TR STYLE=\"BACKGROUND-COLOR:RGB(200, 200, 200);\">\r\n";
            }
            else
            {
                httpResponseString += "<TR STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255);\">\r\n";
            }

            httpResponseString += "    <TD>" + (Integer (index + 1)).toString () + "</TD>\r\n";

            pNetworkManagedObject->getHtmlTableRowDataString (tableRowDataString);

            httpResponseString += tableRowDataString;

            httpResponseString += "</TR>\r\n";
        }

        httpResponseString += "</TABLE>\r\n";
    }

    getWaveServerPagePostPortionForGet (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);
}

void NspNetworksWaveServerMultiPage::getForList (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworksWaveServerMultiPage::wildCardRequestHandlerForGet (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworksWaveServerMultiPage::wildCardRequestHandlerForPost (const HttpRequest &httpRequest)
{
    bool       isAKnownEntity = httpRequest.isAKnownEntity ("name");
    ResourceId status         = WAVE_MESSAGE_ERROR;

    if (true == isAKnownEntity)
    {
        string networkName;

        httpRequest.getEntityValueByName ("name", networkName);

        NetworkConfigurationCreateNetworkMessage message   (networkName);

        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = message.getCompletionStatus ();
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPage::NspNetworksWaveServerMultiPage : Failed to create the Network with name \"" + networkName + "\".  Status : " + FrameworkToolKit::localize (status));

            string                 uri                    = httpRequest.getUri ();
            string                 httpResponse;
            ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

            HttpToolKit::getBadRequestString (httpResponse, FrameworkToolKit::localize (status));

            if (NULL != pServerStreamingSocket)
            {
                (*pServerStreamingSocket) << httpResponse;
            }
        }
        else
        {
            string                 uri                    = httpRequest.getUri ();
            string                 httpResponse;
            ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();
            UI32                   networkId              = message.getNetworkId ();
            string                 newUri                 = "http://" + (httpRequest.getHost ()) + "/tenants/X/networks/" + (Integer (networkId)).toString ();

            trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPage::NspNetworksWaveServerMultiPage : Successfully Created the Network with name \"" + networkName + "\"");

            HttpToolKit::getCreatedString (httpResponse, newUri);

            if (NULL != pServerStreamingSocket)
            {
                (*pServerStreamingSocket) << httpResponse;
            }
        }
    }
    else
    {
        string                 uri                    = httpRequest.getUri ();
        string                 httpResponse;
        ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

        trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPage::NspNetworksWaveServerMultiPage : Bad Request with the given path \"" + uri + "\"");

        HttpToolKit::getBadRequestString (httpResponse);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
}

void NspNetworksWaveServerMultiPage::getForCreate (const HttpRequest &httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForGet (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + getPageHeading() + "</H2>\r\n";
    httpResponseString += "<H3 STYLE=\"COLOR:RGB(0, 0, 255);TEXT-ALIGN:CENTER;\">Create Network</H3>\r\n";

    httpResponseString += "<FORM ACTION=\"/tenants/X/networks/Create\" METHOD=\"POST\">\r\n";
    httpResponseString += "<P>\r\n";
    httpResponseString += "Enter Name of the Network to be created :\r\n";
    httpResponseString += "</P>\r\n";
    httpResponseString += "<P>\r\n";
    httpResponseString += "<INPUT TYPE=\"TEXT\"   NAME=\"name\" size=\"50\">\r\n";
    httpResponseString += "</P>\r\n";
    httpResponseString += "<P>\r\n";
    httpResponseString += "<INPUT TYPE=\"SUBMIT\" VALUE=\"Create Network\">\r\n";
    httpResponseString += "</P>\r\n";
    httpResponseString += "</FORM>\r\n";

    getWaveServerPagePostPortionForGet (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }
}

bool NspNetworksWaveServerMultiPage::callWildCardRequestHandlerAtTop () const
{
    return (true);
}

}
