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

#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworksWaveServerMultiPageForRestXml.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/Integer.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkMessage.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationDeleteNetworkMessage.h"
#include "HttpInterface/HttpToolKit.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Types/UI32Range.h"
#include "HttpInterface/WaveServerPageDirectory.h"

namespace NspNs
{

NspNetworksWaveServerMultiPageForRestXml::NspNetworksWaveServerMultiPageForRestXml (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path)
: WaveServerMultiPage (pHttpInterfaceReceiverObjectManager, path)
{
    WaveServerPageDirectory::registerServerPage ("/rest/slm/tenants/*/networks/*", this);
    WaveServerPageDirectory::registerServerPage ("/rest/slm/tenants/*/networks/*/detail", this);

    setPageHeading ("Network Configuration (REST/XML)");

    addWaveServerMultiPageRequestHandlerForGet ("List",   reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworksWaveServerMultiPageForRestXml::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("detail", reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworksWaveServerMultiPageForRestXml::getForDetail));
    addWaveServerMultiPageRequestHandlerForGet ("Create", reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworksWaveServerMultiPageForRestXml::getForCreate));
}

NspNetworksWaveServerMultiPageForRestXml::~NspNetworksWaveServerMultiPageForRestXml ()
{
}

void NspNetworksWaveServerMultiPageForRestXml::getForListByRangeAndFields (const HttpRequest &httpRequest, const bool &filterByRange)
{
    WaveManagedObjectSynchronousQueryContext queryContext (NetworkManagedObject::getClassName ());
    string                                   pathForThisPage     = getPath ();
    vector<string>                           uriTokens;
    string                                   rangeString         = "all";
    UI32                                     i                   = 0;
    string                                   uri                 = httpRequest.getUri ();
    UI32                                     numberOfUriTokens   = 0;
    bool                                     detailForNetwork    = false;

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

                    if ((i + 2) < numberOfUriTokens)
                    {
                        if ("detail" == uriTokens[i + 2])
                        {
                            detailForNetwork = true;
                        }
                    }
                }
            }

            if (("all" != rangeString) && ("*" != rangeString) && ("detail" != rangeString))
            {
                queryContext.addAndAttribute (new AttributeString (rangeString, "uuid"));
            }
        }
    }

    queryContext.addOrderField ("name");

           vector<WaveManagedObject *> *pQueryResults           = querySynchronously (&queryContext);
           UI32                         numberOfNetworks        = 0;
           NetworkManagedObject        *pNetworkManagedObject   = NULL;
           UI32                         index                   = 0;
           string                       restRowData;
    static vector<string>               restAttributeNames;

    if (0 == (restAttributeNames.size ()))
    {
        restAttributeNames.push_back ("uuid");
        restAttributeNames.push_back ("name");
        restAttributeNames.push_back ("networkId");
    }

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    numberOfNetworks = pQueryResults->size ();

    string                 httpResponseString;
    string                 xmlBody;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    xmlBody += "<networks>\r\n";

    if (0 < numberOfNetworks)
    {
        for (index = 0; index < numberOfNetworks; index++)
        {
            pNetworkManagedObject = dynamic_cast<NetworkManagedObject *> ((*pQueryResults)[index]);

            prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

            if (("detail" == rangeString) || (true == detailForNetwork))
            {
                pNetworkManagedObject->getRestRowData (restRowData);

                xmlBody += restRowData;
#if 0
                NetworkSegmentManagedObject *pNetworkSegmentManagedObject = pNetworkManagedObject->getNetworkSegementAtIndex (0);

                if (NULL != pNetworkSegmentManagedObject)
                {
                    UI32 numberOfNetworkInterfaces = pNetworkSegmentManagedObject->getNumberOfNetworkInterfaces ();
                    UI32 j                         = 0;

                    if (0 != numberOfNetworkInterfaces)
                    {
                        xmlBody += "    <ports>";
                    }

                    for (j = 0; j < numberOfNetworkInterfaces; j++)
                    {
                        NetworkInterfaceManagedObject *pNetworkInterfaceManagedObject = pNetworkSegmentManagedObject->getNetworkInterfaceAtIndex (j);

                        if (NULL != pNetworkInterfaceManagedObject)
                        {
                            pNetworkInterfaceManagedObject->getRestRowData (restRowData);

                            xmlBody += "    " + restRowData;
                        }
                    }

                    if (0 != numberOfNetworkInterfaces)
                    {
                        xmlBody += "    </ports>";
                    }
                }
#endif
            }
            else
            {
                pNetworkManagedObject->getRestRowData (restRowData, restAttributeNames);

                xmlBody += restRowData;
            }
        }
    }

    xmlBody += "</networks>\r\n";

    HttpToolKit::getOkStringForGetForRestXml (httpResponseString, xmlBody);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);
}

void NspNetworksWaveServerMultiPageForRestXml::getForList (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworksWaveServerMultiPageForRestXml::getForDetail (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForGet (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost (const HttpRequest &httpRequest)
{
    bool       isAKnownEntity = httpRequest.isAKnownEntity ("name");
    ResourceId status         = WAVE_MESSAGE_ERROR;

    if (true == isAKnownEntity)
    {
        string networkName;
        string networkIdString;
        UI32   networkId        = 0;

        httpRequest.getEntityValueByName ("name", networkName);
        httpRequest.getEntityValueByName ("vlan", networkIdString);

        if ("" != networkIdString)
        {
            networkId = strtoul (networkIdString.c_str (), NULL, 10);
        }

        NetworkConfigurationCreateNetworkMessage message   (networkName, networkId);

        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = message.getCompletionStatus ();
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Failed to create the Network with name \"" + networkName + "\".  Status : " + FrameworkToolKit::localize (status));

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
            string                 networkUuid            = message.getNetworkUuid ();
            string                 newUri                 = "http://" + (httpRequest.getHost ()) + "/rest/slm/tenants/X/networks/" + networkUuid;
            string                 xmlBody;

            networkId = message.getNetworkId ();

            trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Successfully Created the Network with name \"" + networkName + "\"");

            xmlBody += "<network id=\"" + networkUuid + "\" vlan=\"" + networkId + "\"/>\r\n";

            HttpToolKit::getCreatedStringForRestXml (httpResponse, newUri, xmlBody);

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

        trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Bad Request with the given path \"" + uri + "\"");

        HttpToolKit::getBadRequestString (httpResponse);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
}

void NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForDelete (const HttpRequest &httpRequest)
{
    string                                   pathForThisPage     = getPath ();
    vector<string>                           uriTokens;
    string                                   networkUuid         = "all";
    UI32                                     i                   = 0;
    string                                   uri                 = httpRequest.getUri ();
    UI32                                     numberOfUriTokens   = 0;

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
                    networkUuid = uriTokens[i + 1];
                }
            }
        }
    }

    trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForDelete : Deleting network with UUID : " + networkUuid);

    NetworkConfigurationDeleteNetworkMessage message;
    ResourceId                               status   = WAVE_MESSAGE_SUCCESS;

    message.setNetworkUuid (networkUuid);

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = message.getCompletionStatus ();
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForDelete : Failed to delete the Network with UUID \"" + networkUuid + "\".  Status : " + FrameworkToolKit::localize (status));

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
        string                 httpResponse;
        ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();
        string                 xmlBody;

        trace (TRACE_LEVEL_DEBUG, "NspNetworksWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Successfully Deleted the Network with name \"" + networkUuid + "\"");

        HttpToolKit::getDeletedStringForRestXml (httpResponse, xmlBody);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
}

void NspNetworksWaveServerMultiPageForRestXml::getForCreate (const HttpRequest &httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForGet (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + getPageHeading() + "</H2>\r\n";
    httpResponseString += "<H3 STYLE=\"COLOR:RGB(0, 0, 255);TEXT-ALIGN:CENTER;\">Create Network</H3>\r\n";

    httpResponseString += "<FORM ACTION=\"/rest/slm/tenants/X/networks/Create\" METHOD=\"POST\">\r\n";
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

bool NspNetworksWaveServerMultiPageForRestXml::callWildCardRequestHandlerAtTop () const
{
    return (true);
}

}
