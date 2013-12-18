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

#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworkInterfacesWaveServerMultiPageForRestXml.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/Integer.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceMessage.h"
#include "HttpInterface/HttpToolKit.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Types/UI32Range.h"
#include "HttpInterface/WaveServerPageDirectory.h"
#include "Framework/Types/MacAddress.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceMessage.h"

namespace NspNs
{

NspNetworkInterfacesWaveServerMultiPageForRestXml::NspNetworkInterfacesWaveServerMultiPageForRestXml (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path)
: WaveServerMultiPage (pHttpInterfaceReceiverObjectManager, path)
{
    WaveServerPageDirectory::registerServerPage ("/rest/slm/tenants/*/networks/*/ports/*", this);
    WaveServerPageDirectory::registerServerPage ("/rest/slm/tenants/*/networks/*/ports/*/detail", this);
    WaveServerPageDirectory::registerServerPage ("/rest/slm/tenants/*/networks/*/ports/*/attachment", this);

    setPageHeading ("Network Configuration (REST/XML)");

    addWaveServerMultiPageRequestHandlerForGet ("List",   reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworkInterfacesWaveServerMultiPageForRestXml::getForList));
    addWaveServerMultiPageRequestHandlerForGet ("detail", reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworkInterfacesWaveServerMultiPageForRestXml::getForDetail));
    addWaveServerMultiPageRequestHandlerForGet ("Create", reinterpret_cast<WaveServerMultiPageRequestHandler> (&NspNetworkInterfacesWaveServerMultiPageForRestXml::getForCreate));
}

NspNetworkInterfacesWaveServerMultiPageForRestXml::~NspNetworkInterfacesWaveServerMultiPageForRestXml ()
{
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::getForListByRangeAndFields (const HttpRequest &httpRequest, const bool &filterByRange)
{
    WaveManagedObjectSynchronousQueryContext queryContext (VnicManagedObject::getClassName ());
    string                                   pathForThisPage     = getPath ();
    vector<string>                           uriTokens;
    string                                   rangeString         = "all";
    UI32                                     i                   = 0;
    string                                   uri                 = httpRequest.getUri ();
    UI32                                     numberOfUriTokens   = 0;
    bool                                     detailForNetwork    = false;
    string                                   networkUuid;

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
                        networkUuid = uriTokens[i + 1];
                    }
                }
                else if ("ports" == uriTokens[i])
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

            queryContext.addAndAttribute (new AttributeString (networkUuid, "networkUuid"));
        }
    }

    queryContext.addOrderField ("uuid");

           vector<WaveManagedObject *>   *pQueryResults      = querySynchronously (&queryContext);
           UI32                           numberOfNetworks   = 0;
           VnicManagedObject             *pVnicManagedObject = NULL;
           UI32                          index               = 0;
           string                        restRowData;
    static vector<string>                restAttributeNames;

    if (0 == (restAttributeNames.size ()))
    {
        restAttributeNames.push_back ("uuid");
    }

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    numberOfNetworks = pQueryResults->size ();

    string                 httpResponseString;
    string                 xmlBody;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    xmlBody += "<ports>\r\n";

    if (0 < numberOfNetworks)
    {
        for (index = 0; index < numberOfNetworks; index++)
        {
            pVnicManagedObject = dynamic_cast<VnicManagedObject *> ((*pQueryResults)[index]);

            prismAssert (NULL != pVnicManagedObject, __FILE__, __LINE__);

            if (("detail" == rangeString) || (true == detailForNetwork))
            {
                pVnicManagedObject->getRestRowData (restRowData);
            }
            else
            {
                pVnicManagedObject->getRestRowData (restRowData, restAttributeNames);
            }

            xmlBody += restRowData;
        }
    }

    xmlBody += "</ports>\r\n";

    HttpToolKit::getOkStringForGetForRestXml (httpResponseString, xmlBody);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::getForList (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::getForDetail (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForGet (const HttpRequest &httpRequest)
{
    getForListByRangeAndFields (httpRequest);
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost (const HttpRequest &httpRequest)
{
    bool           isAKnownEntity    = httpRequest.isAKnownEntity ("state");
    ResourceId     status            = WAVE_MESSAGE_ERROR;
    string         uri               = httpRequest.getUri ();
    vector<string> uriTokens;
    UI32           numberOfUriTokens = 0;
    string         networkUuid;
    UI32           i                 = 0;


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

    if (true == isAKnownEntity)
    {
        string portStatus;

        httpRequest.getEntityValueByName ("state", portStatus);

        NetworkInterfaceStatus                     networkinterfaceStatus = NETWORK_INTERFACE_ACTIVE;
        NetworkConfigurationCreateInterfaceMessage message                  (networkUuid, networkinterfaceStatus);

        if ("ACTIVE" == portStatus)
        {
            networkinterfaceStatus = NETWORK_INTERFACE_ACTIVE;
        }
        else
        {
            networkinterfaceStatus = NETWORK_INTERFACE_INACTIVE;
        }

        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = message.getCompletionStatus ();
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Failed to create a port on the Network with uuid \"" + networkUuid + "\".  Status : " + FrameworkToolKit::localize (status));

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
            string                 interfaceUuid          = message.getInterfaceUuid ();
            string                 newUri                 = "http://" + (httpRequest.getHost ()) + "/rest/slm/tenants/X/networks/" + networkUuid + "/ports/" + interfaceUuid;
            string                 xmlBody;

            trace (TRACE_LEVEL_DEBUG, "NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Successfully Created the Port At \"" + newUri + "\"");

            xmlBody += "<port id=\"" + interfaceUuid + "\"/>\r\n";

            HttpToolKit::getCreatedStringForRestXml (httpResponse, newUri, xmlBody);

            if (NULL != pServerStreamingSocket)
            {
                (*pServerStreamingSocket) << httpResponse;
            }
        }
    }
    else
    {
        string                 httpResponse;
        ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

        trace (TRACE_LEVEL_DEBUG, "NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPost : Bad Request with the given path \"" + uri + "\"");

        HttpToolKit::getBadRequestString (httpResponse);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPut (const HttpRequest &httpRequest)
{
    bool           isAKnownEntity    = httpRequest.isAKnownEntity ("id");
    ResourceId     status            = WAVE_MESSAGE_ERROR;
    string         uri               = httpRequest.getUri ();
    vector<string> uriTokens;
    UI32           numberOfUriTokens = 0;
    string         networkUuid;
    string         portUuid;
    UI32           i                 = 0;


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
            else if ("ports" == uriTokens[i])
            {
                if ((i + 1) < numberOfUriTokens)
                {
                    portUuid = uriTokens[i + 1];
                }
            }
        }
    }

    if (true == isAKnownEntity)
    {
        string     macAddressInStringFromat;
        MacAddress macAddress;
        string     targetHost;

        httpRequest.getEntityValueByName ("id",         macAddressInStringFromat);
        httpRequest.getEntityValueByName ("targethost", targetHost);

        if (true == (MacAddress::isValidMacAddressString (macAddressInStringFromat)))
        {
            macAddress.fromString (macAddressInStringFromat);

            NetworkConfigurationAttachVirtualInterfaceMessage message (networkUuid, portUuid, macAddress, targetHost);

            status  = sendSynchronously (&message);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                status = message.getCompletionStatus ();
            }
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPut : Failed to attach on the Network with uuid \"" + networkUuid + "\", Port uuid \"" + portUuid + "\", MAC \"" + macAddressInStringFromat + "\". Status : " + FrameworkToolKit::localize (status));

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

            HttpToolKit::getNoContentStringForPutForRestXml (httpResponse);

            if (NULL != pServerStreamingSocket)
            {
                (*pServerStreamingSocket) << httpResponse;
            }
        }
    }
    else
    {
        string                 httpResponse;
        ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

        trace (TRACE_LEVEL_DEBUG, "NspNetworkInterfacesWaveServerMultiPageForRestXml::wildCardRequestHandlerForPut : Bad Request with the given path \"" + uri + "\"");

        HttpToolKit::getBadRequestString (httpResponse);

        if (NULL != pServerStreamingSocket)
        {
            (*pServerStreamingSocket) << httpResponse;
        }
    }
}

void NspNetworkInterfacesWaveServerMultiPageForRestXml::getForCreate (const HttpRequest &httpRequest)
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

bool NspNetworkInterfacesWaveServerMultiPageForRestXml::callWildCardRequestHandlerAtTop () const
{
    return (true);
}

}
