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

#include "HttpInterface/HttpRequest.h"
#include <vector>
#include <Framework/Utils/FrameworkToolKit.h>
#include "HttpToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Utils/ServerStreamingSocket.h"

#include <stdlib.h>
#include <strings.h>
#include <errno.h>

namespace WaveNs
{

string HttpRequest::s_authorizationString = "Authorization";
string HttpRequest::s_contentLengthString = "Content-Length";
string HttpRequest::s_contentTypeString   = "Content-Type";
string HttpRequest::s_hostString          = "Host";

HttpRequest::HttpRequest (string& httpRequest, ServerStreamingSocket* pServerStreamingSocket)
    : m_waveHttpInterfaceMethod (WAVE_HTTP_INTERFACE_METHOD_UNKNOWN),
      m_contentLength           (0),
      m_contentType             (WAVE_HTTP_CONTENT_TYPE_UNKNOWN),
      m_isContentReadIncomplete (false),
      m_pServerStreamingSocket  (pServerStreamingSocket)
{
    // First check if we have an empty line in the request - which is a delimiter indicating the end of header.
    // Otherwise keep reading from the socket till we receive such line.

    string::size_type emptyLinePosition          = string::npos;
    UI32              consecutiveZeroSizeReturns = 0;

    while (string::npos == (emptyLinePosition = (httpRequest.find ("\r\n\r\n"))))
    {
        string tempString;
        bool   isSuccesful = false;

        errno = 0;

        isSuccesful = (*pServerStreamingSocket) >> tempString;

        if (ENOENT == errno)
        {
            return;
        }
        else
        {
            if (0 == (tempString.size ()))
            {
                consecutiveZeroSizeReturns++;
            }
            else
            {
                consecutiveZeroSizeReturns = 0;
            }
        }

        if (100 < consecutiveZeroSizeReturns)
        {
            return;
        }

        if (true == isSuccesful)
        {
            httpRequest += tempString;
        }
    }

    // First check if we have the entire request.
    // Otherwise read the remaining from wire.

          UI32               requestLength                 = httpRequest.length ();
          string::size_type  contentLengthPosition         = httpRequest.find (s_contentLengthString);
          string::size_type  nextEndOfLinePosition         = 0;
          string::size_type  contentBeginPosition          = 0;
          UI32               lengthOfRemainingDataToBeRead = 0;
    const char              *pTempRequestCharArray         = NULL;

    if (string::npos != contentLengthPosition)
    {
        pTempRequestCharArray = httpRequest.c_str ();

        m_contentLength = strtoul (pTempRequestCharArray + (contentLengthPosition + s_contentLengthString.length () + 2), NULL, 10);

        nextEndOfLinePosition = httpRequest.find ("\r\n\r\n", contentLengthPosition);

        if (string::npos != nextEndOfLinePosition)
        {
            contentBeginPosition  = nextEndOfLinePosition + 4;
        }
    }

    if (0 != contentBeginPosition)
    {
        if ((UINT_MAX - m_contentLength) >= contentBeginPosition)
        {
            if ((contentBeginPosition + m_contentLength) > requestLength)
            {
                lengthOfRemainingDataToBeRead = (contentBeginPosition + m_contentLength) - requestLength;
            }
        }
    }

    trace (TRACE_LEVEL_DEVEL, string ("HttpRequest::HttpRequest : Content Begin                          : ") + contentBeginPosition);
    trace (TRACE_LEVEL_DEVEL, string ("HttpRequest::HttpRequest : Content Length                         : ") + m_contentLength);
    trace (TRACE_LEVEL_DEVEL, string ("HttpRequest::HttpRequest : Request Length                         : ") + requestLength);
    trace (TRACE_LEVEL_DEVEL, string ("HttpRequest::HttpRequest : Length of Remaining Content to be Read : ") + lengthOfRemainingDataToBeRead);

    if (0 < lengthOfRemainingDataToBeRead)
    {
        if (NULL != pServerStreamingSocket)
        {
            FixedSizeBuffer fixedSizeBuffer   (lengthOfRemainingDataToBeRead);
            bool            isSuccesful     = false;

            isSuccesful = (*pServerStreamingSocket) >> fixedSizeBuffer;

            if (true == isSuccesful)
            {
                string tempString;

                fixedSizeBuffer.toString (tempString);

                httpRequest += tempString;

                setIsContentReadIncomplete (false);
            }
            else
            {
                setIsContentReadIncomplete (true);
            }
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    // Now proceed to process the request in its entirety.

    string            httpRequestFromWire = httpRequest;
    string::size_type position            = 0;
    UI32              headerEndLineIndex  = 0;
    UI32              lineLength          = 0;

    vector<string> httpRequestLines;

    tokenize (httpRequestFromWire, httpRequestLines, '\n');

    UI32 numberOfHttpRequestLines = httpRequestLines.size ();
    UI32 i                        = 0;
    UI32 j                        = 0;

    if (0 < numberOfHttpRequestLines)
    {
        // Get the Method.

        string requestLine = httpRequestLines[0];

        vector<string> requestLineTokens;

        tokenize (requestLine, requestLineTokens, ' ');

        UI32 numberOfRequetLineTokens = requestLineTokens.size ();

        if (0 < numberOfRequetLineTokens)
        {
            m_waveHttpInterfaceMethod = HttpToolKit::getHttpInterfaceMethodFromString (requestLineTokens[0]);
        }

        if (1 < numberOfRequetLineTokens)
        {
            m_uri = requestLineTokens[1];

            tokenize (m_uri, m_uriTokens,'/');
        }

        // Eliminate the \r characters at the end of each line if exists.

        for (i = 0; i < numberOfHttpRequestLines; i++)
        {
            lineLength = (httpRequestLines[i]).length ();
            position   = (httpRequestLines[i]).find ('\r');

            if (position == (lineLength - 1))
            {
                (httpRequestLines[i]).erase (position, 1);
            }
        }

        for (i = 0; i < numberOfHttpRequestLines; i++)
        {
            //trace (TRACE_LEVEL_DEVEL, httpRequestLines[i] + "\r");

            // First empty line signals the end of the message header.

            if ("" == httpRequestLines[i])
            {
                headerEndLineIndex = i;

                break;
            }

            // Get Host

            if (0 == (strncasecmp ((httpRequestLines[i]).c_str (), s_hostString.c_str (), s_hostString.length ())))
            {
                vector<string> hostTokens;

                tokenize (httpRequestLines[i], hostTokens, ' ');

                if (2 <= (hostTokens.size ()))
                {
                    m_host = hostTokens[1];
                }
            }

            // Get Authrozation

            if (0 == (strncasecmp ((httpRequestLines[i]).c_str (), s_authorizationString.c_str (), s_authorizationString.length ())))
            {
                vector<string> authorizationTokens;

                tokenize (httpRequestLines[i], authorizationTokens, ' ');

                if (3 <= (authorizationTokens.size ()))
                {
                    m_authorization = authorizationTokens[2];
                }
            }

            // Get Content Length

            if (0 == (strncasecmp ((httpRequestLines[i]).c_str (), s_contentLengthString.c_str (), s_contentLengthString.length ())))
            {
                vector<string> contentLengthTokens;

                tokenize (httpRequestLines[i], contentLengthTokens, ' ');

                if (2 <= (contentLengthTokens.size ()))
                {
                    m_contentLength = strtoul (contentLengthTokens[1].c_str (), NULL, 10);
                }
            }

            // Get Content Type

            if (0 == (strncasecmp ((httpRequestLines[i]).c_str (), s_contentTypeString.c_str (), s_contentTypeString.length ())))
            {
                vector<string> contentTypeTokens;
                string         contentTypeString;

                tokenize (httpRequestLines[i], contentTypeTokens, ' ');

                if (3 <= (contentTypeTokens.size ()))
                {
                    contentTypeString = contentTypeTokens[1];
                    contentTypeString.erase (contentTypeString.begin () + (contentTypeString.length () - 1));

                    m_contentType = HttpToolKit::getHttpContentTypeFromString (contentTypeString);

                    if (WAVE_HTTP_CONTENT_TYPE_MULTIPART_FORM_DATA == m_contentType)
                    {
                        m_contentBoundary = contentTypeTokens[2];

                        m_contentBoundary.replace (0, m_contentBoundary.find ('=') + 1, string (""));
                    }
                }
                else if (2 <= (contentTypeTokens.size ()))
                {
                    m_contentType = HttpToolKit::getHttpContentTypeFromString (contentTypeTokens[1]);
                }
            }
        }
    }

    // Fix the URI.  Then URL Decode it.

    position = m_uri.find ('?');

    if (string::npos != position)
    {
        vector<string> uriTokens;
        UI32           numberOfUriTokens = 0;

        StringUtils::tokenize (m_uri, uriTokens, '?');

        numberOfUriTokens = uriTokens.size ();

        if (1 <= numberOfUriTokens)
        {
            m_uri = uriTokens[0];

            HttpToolKit::decodeUrl (m_uri);

            for (i = 1; i < numberOfUriTokens; i++)
            {
                vector<string> uriEntityTokens;
                UI32           numberOfUriEntityTokens = 0;

                StringUtils::tokenize (uriTokens[i], uriEntityTokens, '&');

                numberOfUriEntityTokens = uriEntityTokens.size ();

                for (j = 0; j < numberOfUriEntityTokens; j++)
                {
                    vector<string> uriEntityNameValueTokens;
                    UI32           numberOfUriEntityNameValueTokens = 0;

                    StringUtils::tokenize (uriEntityTokens[j], uriEntityNameValueTokens, '=');

                    numberOfUriEntityNameValueTokens = uriEntityNameValueTokens.size ();

                    if (2 <= numberOfUriEntityNameValueTokens)
                    {
                        string entityName   = uriEntityNameValueTokens[0];
                        string fileName     = "";
                        string entityValue  = uriEntityNameValueTokens[1];

                        HttpToolKit::decodeUrl (entityName);
                        HttpToolKit::decodeUrl (fileName);
                        HttpToolKit::decodeUrl (entityValue);

                        m_entityNames.push_back     (entityName);
                        m_entityFileNames.push_back (fileName);
                        m_entityValues.push_back    (entityValue);

                        m_entitiesMap[entityName] = entityValue;
                    }
                }
            }
        }
    }
    else
    {
        HttpToolKit::decodeUrl (m_uri);
    }

    // Get Content

    if (0 != m_contentLength)
    {
        UI32 totalHttpRequestLength = httpRequest.length ();

        if ((totalHttpRequestLength > m_contentLength) && (0 != contentBeginPosition))
        {
            //m_content.assign (httpRequest, totalHttpRequestLength - m_contentLength, m_contentLength);
            m_content.assign (httpRequest, contentBeginPosition, totalHttpRequestLength - contentBeginPosition);
        }

        if (WAVE_HTTP_CONTENT_TYPE_MULTIPART_FORM_DATA == m_contentType)
        {
            prismAssert (0 != (m_contentBoundary.length ()), __FILE__, __LINE__);

            string entityName;
            string fileName;
            string entityValue;
            string effectiveContentBoundaryString      = string ("--") + m_contentBoundary;
            string finalEffectiveContentBoundaryString = effectiveContentBoundaryString + string ("--");
            bool   startAddingToEntityValue            = false;

            for (i = headerEndLineIndex + 1; i < numberOfHttpRequestLines; i++)
            {
                if (0 == ((httpRequestLines[i]).find (effectiveContentBoundaryString)))
                {
                    if ("" != entityName)
                    {
                        m_entityNames.push_back     (entityName);
                        m_entityFileNames.push_back (fileName);
                        m_entityValues.push_back    (entityValue);

                        m_entitiesMap[entityName] = entityValue;

                        entityName               = "";
                        fileName                 = "";
                        entityValue              = "";
                        startAddingToEntityValue = false;
                    }
                }
                else
                {
                    if ("" == httpRequestLines[i])
                    {
                        if (false == startAddingToEntityValue)
                        {
                            startAddingToEntityValue = true;
                        }
                        else
                        {
                            if ("" == entityValue)
                            {
                                entityValue += httpRequestLines[i];
                            }
                            else
                            {
                                entityValue += "\n" + httpRequestLines[i];
                            }
                        }
                    }
                    else
                    {
                        if (true == startAddingToEntityValue)
                        {
                            if ("" == entityValue)
                            {
                                entityValue += httpRequestLines[i];
                            }
                            else
                            {
                                entityValue += "\n" + httpRequestLines[i];
                            }
                        }
                        else
                        {
                            if ("" == entityName)
                            {
                                vector<string> entityNameLineTokens;
                                UI32           numberOfEntityLineTokens = 0;

                                tokenize (httpRequestLines[i], entityNameLineTokens, ';');

                                numberOfEntityLineTokens = entityNameLineTokens.size ();

                                if (2 <= numberOfEntityLineTokens)
                                {
                                    vector<string> entityNameTokens;
                                    UI32           numberOfEntityNameTokens = 0;

                                    tokenize (entityNameLineTokens[1], entityNameTokens,'=');
                                    numberOfEntityNameTokens = entityNameTokens.size ();

                                    if (2 <= numberOfEntityNameTokens)
                                    {
                                        entityName = entityNameTokens[1];

                                        StringUtils::removeAllInstancesOfInputChar (entityName, '"');
                                    }
                                }

                                if (3 <= numberOfEntityLineTokens)
                                {
                                    vector<string> fileNameTokens;
                                    UI32           numberOfFileNameTokens = 0;

                                    tokenize (entityNameLineTokens[2], fileNameTokens,'=');
                                    numberOfFileNameTokens = fileNameTokens.size ();

                                    if (2 <= numberOfFileNameTokens)
                                    {
                                        fileName = fileNameTokens[1];

                                        StringUtils::removeAllInstancesOfInputChar (fileName, '"');
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (WAVE_HTTP_CONTENT_TYPE_APPLICATION_FORM_URLENCODED == m_contentType)
        {
            vector<string> contentTokens;
            UI32           numberOfContentTokens = 0;

            StringUtils::tokenize (m_content, contentTokens, '&');

            numberOfContentTokens = contentTokens.size ();

            for (i = 0; i < numberOfContentTokens; i++)
            {
                vector<string> contentNameValueTokens;
                UI32           numberOfContentNameValueTokens = 0;

                StringUtils::tokenize (contentTokens[i], contentNameValueTokens, '=');

                numberOfContentNameValueTokens = contentNameValueTokens.size ();

                if (2 <= numberOfContentNameValueTokens)
                {
                    string entityName   = contentNameValueTokens[0];
                    string fileName     = "";
                    string entityValue  = contentNameValueTokens[1];

                    HttpToolKit::decodeUrl (entityName);
                    HttpToolKit::decodeUrl (fileName);
                    HttpToolKit::decodeUrl (entityValue);

                    m_entityNames.push_back     (entityName);
                    m_entityFileNames.push_back (fileName);
                    m_entityValues.push_back    (entityValue);

                    m_entitiesMap[entityName] = entityValue;
                }
            }
        }
    }
}

HttpRequest::~HttpRequest()
{
}

WaveHttpInterfaceMethod HttpRequest::getWaveHttpInterfaceMethod () const
{
    return (m_waveHttpInterfaceMethod);
}

void HttpRequest::setWaveHttpInterfaceMethod (const WaveHttpInterfaceMethod& waveHttpInterfaceMethod)
{
    m_waveHttpInterfaceMethod = waveHttpInterfaceMethod;
}

string HttpRequest::getUri () const
{
    return (m_uri);
}

void HttpRequest::setUri (const string& uri)
{
    m_uri = uri;
}

vector<string> HttpRequest::getUriTokens () const
{
    return (m_uriTokens);
}

void HttpRequest::setUriTokens (const vector< string >& uriTokens)
{
    m_uriTokens = uriTokens;
}

string HttpRequest::getAuthorizarion() const
{
    return (m_authorization);
}

void HttpRequest::setAuthorization(const string& authorization)
{
    m_authorization = authorization;
}

UI32 HttpRequest::getContentLength () const
{
    return (m_contentLength);
}

void HttpRequest::setContentLength (const UI32& contentLength)
{
    m_contentLength = contentLength;
}

WaveHttpContentType HttpRequest::getContentType () const
{
    return (m_contentType);
}

void HttpRequest::setContentType (const WaveHttpContentType& contentType)
{
    m_contentType = contentType;
}

string HttpRequest::getContentBoundary () const
{
    return (m_contentBoundary);
}

void HttpRequest::setContentBoundary (const string& contentBoundry)
{
    m_contentBoundary = contentBoundry;
}

string HttpRequest::getContent () const
{
    return (m_content);
}

void HttpRequest::setContent (const string& content)
{
    m_content = content;
}

UI32 HttpRequest::getNumberOfEntities () const
{
    const UI32 numberOfEntityNames     = m_entityNames.size ();
    const UI32 numberOfEntityFileNames = m_entityFileNames.size ();
    const UI32 numberOfEntityValues    = m_entityValues.size ();

    prismAssert (numberOfEntityNames == numberOfEntityValues,    __FILE__, __LINE__);
    prismAssert (numberOfEntityNames == numberOfEntityFileNames, __FILE__, __LINE__);

    return (m_entityNames.size ());
}

void HttpRequest::getEntityAtIndex(const WaveNs::UI32& index, string& entityName, string& entityValue, string &entityFileName)
{
    UI32 numberOfEntities = getNumberOfEntities ();

    if (index < numberOfEntities)
    {
        entityName     = m_entityNames[index];
        entityValue    = m_entityValues[index];
        entityFileName = m_entityFileNames[index];
    }
}

void HttpRequest::getEntityValueByName (const string &entitityName, string &entityValue) const
{
    map<string, string>::const_iterator element    = m_entitiesMap.find (entitityName);
    map<string, string>::const_iterator endElement = m_entitiesMap.end  ();

    if (endElement != element)
    {
        entityValue = element->second;
    }
    else
    {
        entityValue = "";
    }
}

bool HttpRequest::isAKnownEntity (const string &entitityName) const
{
    map<string, string>::const_iterator element    = m_entitiesMap.find (entitityName);
    map<string, string>::const_iterator endElement = m_entitiesMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

ServerStreamingSocket *HttpRequest::getPServerStreamingSocket () const
{
    return (m_pServerStreamingSocket);
}

string HttpRequest::getHost () const
{
    return (m_host);
}

void HttpRequest::setHost (const string &host)
{
    m_host = host;
}

bool HttpRequest::getIsContentReadIncomplete () const
{
    return (m_isContentReadIncomplete);
}

void HttpRequest::setIsContentReadIncomplete (bool isContentReadIncomplete)
{
    m_isContentReadIncomplete = isContentReadIncomplete;
}

}
