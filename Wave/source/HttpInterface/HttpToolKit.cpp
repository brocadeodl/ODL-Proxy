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

#include "HttpInterface/HttpToolKit.h"
#include <Framework/Utils/StringUtils.h>

namespace WaveNs
{

WaveHttpInterfaceMethod HttpToolKit::getHttpInterfaceMethod(const string& httpRequest)
{
    vector<string> httpRequestWords;
    WaveHttpInterfaceMethod waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_UNKNOWN;

    tokenize (httpRequest, httpRequestWords, ' ');

    if (0 < (httpRequestWords.size ()))
    {
        waveHttpInterfaceMethod = getHttpInterfaceMethodFromString (httpRequestWords[0]);
    }

    return (waveHttpInterfaceMethod);
}

WaveHttpInterfaceMethod HttpToolKit::getHttpInterfaceMethodFromString (const string& httpRequestMethodString)
{
    WaveHttpInterfaceMethod waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_UNKNOWN;

    if ("GET" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_GET;
    }
    else if ("HEAD" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_HEAD;
    }
    else if ("PUT" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_PUT;
    }
    else if ("POST" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_POST;
    }
    else if ("DELETE" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_DELETE;
    }
    else if ("OPTIONS" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_OPTIONS;
    }
    else if ("TRACE" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_TRACE;
    }
    else if ("CONNECT" == httpRequestMethodString)
    {
        waveHttpInterfaceMethod = WAVE_HTTP_INTERFACE_METHOD_CONNECT;
    }

    return (waveHttpInterfaceMethod);
}

WaveHttpContentType HttpToolKit::getHttpContentTypeFromString (const string& contentTypeString)
{
    WaveHttpContentType httpContentType = WAVE_HTTP_CONTENT_TYPE_UNKNOWN;

    if ("text/plain" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_TEXT_PLAIN;
    }
    else if ("text/html" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_TEXT_HTML;
    }
    else if ("text/xml" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_TEXT_XML;
    }
    else if ("application/octet-stream" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_APPLICATION_OCTET_STREAM;
    }
    else if ("application/atom+xml" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_APPLICATION_ATOM_XML;
    }
    else if ("application/x-www-form-urlencoded" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_APPLICATION_FORM_URLENCODED;
    }
    else if ("application/json" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_APPLICATION_JSON;
    }
    else if ("application/xhtml+xml" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_APPLICATION_XHTML_XML;
    }
    else if ("multipart/form-data" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_MULTIPART_FORM_DATA;
    }
    else if ("image/gif" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_IMAGE_GIF;
    }
    else if ("image/png" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_IMAGE_PNG;
    }
    else if ("image/jpeg" == contentTypeString)
    {
        httpContentType = WAVE_HTTP_CONTENT_TYPE_IMAGE_JPEG;
    }

    return (httpContentType);
}

void HttpToolKit::addHeader (string& httpResponseString)
{
    httpResponseString += "<H1>\r\n";
    httpResponseString += "<P STYLE=\"COLOR:RGB(255, 0, 0); TEXT-ALIGN:CENTER\">Wave World Wide Web Server (W4S)</P>\r\n";
    httpResponseString += "</H1>\r\n";
    httpResponseString += "<P STYLE=\"COLOR:RGB(200, 200, 200); TEXT-ALIGN:CENTER\"><SUP>V 0.0.1 TB.PS 23.01</SUP></P>\r\n";
}

void HttpToolKit::addFooter (string& httpResponseString)
{
    httpResponseString += "<P STYLE=\"COLOR:RGB(200, 200, 200)\"><EM><SUB>Wave World Wide Web Server (W4S) TB.PS 23.01. (C) Vidyasagara R. Guntaka et al.</SUB></EM></P>\r\n";
}

void HttpToolKit::getFrontPage (string& frontPageString)
{
    frontPageString  = "HTTP/1.1 200 OK\r\n";
    frontPageString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    frontPageString += "Content-Type: text/html\r\n";
    frontPageString += "\r\n";
    frontPageString += "<HTML>\r\n";

    frontPageString += "<HEAD>\r\n";

    frontPageString += "<STYLE>\r\n";

    frontPageString += "A:VISITED\r\n";
    frontPageString += "{\r\n";
    frontPageString += "COLOR:RGB(255, 0 ,0);\r\n";
    frontPageString += "TEXT-DECORATION:NONE;\r\n";
    frontPageString += "}\r\n";

    frontPageString += "A:LINK\r\n";
    frontPageString += "{\r\n";
    frontPageString += "COLOR:RGB(0, 0 ,255);\r\n";
    frontPageString += "TEXT-DECORATION:NONE;\r\n";
    frontPageString += "}\r\n";

    frontPageString += "A:HOVER\r\n";
    frontPageString += "{\r\n";
    frontPageString += "COLOR:RGB(0, 255 ,0)\r\n";
    frontPageString += "}\r\n";

    frontPageString += "A:ACTIVE\r\n";
    frontPageString += "{\r\n";
    frontPageString += "COLOR:RGB(255, 255 ,0)\r\n";
    frontPageString += "}\r\n";

    frontPageString += "</STYLE>\r\n";

    frontPageString += "</HEAD>\r\n";

    frontPageString += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    frontPageString += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    frontPageString += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    frontPageString += "<A>\r\n";
    frontPageString += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    frontPageString += "</A>\r\n";
    frontPageString += "</DIV>\r\n";

    frontPageString += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    frontPageString += "<A HREF=\"/\">Home</A>\r\n";
    frontPageString += " | \r\n";
    frontPageString += "<A HREF=\"/\">About</A>\r\n";
    frontPageString += " | \r\n";
    frontPageString += "<A HREF=\"/\">Contact Us</A>\r\n";
    frontPageString += "</DIV>\r\n";

    frontPageString += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    frontPageString += "<UL>\r\n";
    frontPageString += "<LI><A HREF=\"/\">Trace</A></BR></LI>\r\n";
    frontPageString += "<LI><A HREF=\"/\">Service</A></BR></LI>\r\n";
    frontPageString += "<LI><A HREF=\"/mylink\">Regression</A></BR></LI>\r\n";
    frontPageString += "</UL>\r\n";

    frontPageString += "<HR/>\r\n";

    frontPageString += "</DIV>\r\n";

    frontPageString += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
    frontPageString += "<P>This is content</P>\r\n";
    frontPageString += "</DIV>\r\n";

    frontPageString += "</DIV>\r\n";
    frontPageString += "</BODY>\r\n";
    frontPageString += "</HTML>\r\n";
}

void HttpToolKit::getNotImplementedErrorString (string& notImplementedErrorString)
{
    notImplementedErrorString  = "HTTP/1.1 501 Not Implemented\r\n";
    notImplementedErrorString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    notImplementedErrorString += "Content-Type: text/html\r\n";
    notImplementedErrorString += "\r\n";
    notImplementedErrorString += "<HTML>\r\n";
    notImplementedErrorString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    notImplementedErrorString += "<H2>Method Not Implemented</H2>\r\n";
    notImplementedErrorString += "<BODY>\r\n";
    notImplementedErrorString += "<P STYLE=\"COLOR:RED\">Wave World Wide Web Server (W4S) Currently does not support the requested Method.</P>\r\n";
    addFooter (notImplementedErrorString);
    notImplementedErrorString += "</BODY>\r\n";
    notImplementedErrorString += "</HTML>\r\n";
}

void HttpToolKit::getMethodNotAllowedErrorString(string& methodNotAllowedErrorString)
{
    methodNotAllowedErrorString  = "HTTP/1.1 405 Method Not Allowed\r\n";
    methodNotAllowedErrorString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    methodNotAllowedErrorString += "Allow: GET, HEAD, PUT, POST, TRACE, OPTIONS\r\n";
    methodNotAllowedErrorString += "Content-Type: text/html\r\n";
    methodNotAllowedErrorString += "\r\n";
    methodNotAllowedErrorString += "<HTML>\r\n";
    methodNotAllowedErrorString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    methodNotAllowedErrorString += "<H2>Method Not Allowed</H2>\r\n";
    methodNotAllowedErrorString += "<BODY>\r\n";
    methodNotAllowedErrorString += "<P STYLE=\"COLOR:RED\">Wave World Wide Web Server (W4S) Currently does not allow the requested Method.</P>\r\n";
    addFooter (methodNotAllowedErrorString);
    methodNotAllowedErrorString += "</BODY>\r\n";
    methodNotAllowedErrorString += "</HTML>\r\n";
}

void HttpToolKit::getNotFoundErrorString (string& notFoundErrorString)
{
    notFoundErrorString  = "HTTP/1.1 404 Not Found\r\n";
    notFoundErrorString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    notFoundErrorString += "Content-Type: text/html\r\n";
    notFoundErrorString += "\r\n";
    notFoundErrorString += "<HTML>\r\n";
    notFoundErrorString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    notFoundErrorString += "<H2>Resource Not Found</H2>\r\n";
    notFoundErrorString += "<BODY>\r\n";
    notFoundErrorString += "<P STYLE=\"COLOR:RED\">Wave World Wide Web Server (W4S) could not find the requested resource.</P>\r\n";
    addFooter (notFoundErrorString);
    notFoundErrorString += "</BODY>\r\n";
    notFoundErrorString += "</HTML>\r\n";
}

void HttpToolKit::getUnauthorizedString(string& unAuthorizedString)
{
    unAuthorizedString  = "HTTP/1.1 401 Unauthorized\r\n";
    unAuthorizedString += "WWW-Authenticate: Basic realm=\"Wave\"\r\n";
    unAuthorizedString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    unAuthorizedString += "Content-Type: text/html\r\n";
    unAuthorizedString += "\r\n";
    unAuthorizedString += "<HTML>\r\n";
    unAuthorizedString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    unAuthorizedString += "<H2>Unauthorized</H2>\r\n";
    unAuthorizedString += "<BODY>\r\n";
    unAuthorizedString += "<P STYLE=\"COLOR:RED\">You are required to provide authorization credentials to access the requested resource.</P>\r\n";
    addFooter (unAuthorizedString);
    unAuthorizedString += "</BODY>\r\n";
    unAuthorizedString += "</HTML>\r\n";
}

void HttpToolKit::getBadRequestString (string& badRequestString, const string &reason)
{
    badRequestString  = "HTTP/1.1 400 Bad Request\r\n";
    badRequestString += "WWW-Authenticate: Basic realm=\"Wave\"\r\n";
    badRequestString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    badRequestString += "Content-Type: text/html\r\n";
    badRequestString += "\r\n";
    badRequestString += "<HTML>\r\n";
    badRequestString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    badRequestString += "<H2>Bad Request</H2>\r\n";
    badRequestString += "<BODY>\r\n";
    badRequestString += "<P STYLE=\"COLOR:RED\">" + reason + "</P>\r\n";
    addFooter (badRequestString);
    badRequestString += "</BODY>\r\n";
    badRequestString += "</HTML>\r\n";
}

void HttpToolKit::getCreatedString (string &createdString, const string &createdUri)
{
    createdString  = "HTTP/1.1 201 Created\r\n";
    createdString += "WWW-Authenticate: Basic realm=\"Wave\"\r\n";
    createdString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    createdString += "Content-Type: text/html\r\n";
    createdString += "Location: " + createdUri + "\r\n";
    createdString += "\r\n";
    createdString += "<HTML>\r\n";
    createdString += "<TITLE>Wave World Wide Web Server (W4S) v0.0.1</TITLE>\r\n";
    createdString += "<H2>Resource Successfully Created</H2>\r\n";
    createdString += "<BODY>\r\n";
    createdString += "<P STYLE=\"COLOR:BLACK\">The resource has been successfully created.</P>\r\n";
    createdString += "<P STYLE=\"COLOR:BLACK\">Newly Created Resource Location : " + createdUri + "</P>\r\n";
    addFooter (createdString);
    createdString += "</BODY>\r\n";
    createdString += "</HTML>\r\n";
}

void HttpToolKit::getCreatedStringForRestXml (string &createdString, const string &createdUri, const string &xmlBody)
{
    createdString  = "HTTP/1.1 201 Created\r\n";
    createdString += "WWW-Authenticate: Basic realm=\"Wave\"\r\n";
    createdString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    createdString += "Content-Type: text/xml\r\n";
    createdString += "Location: " + createdUri + "\r\n";
    createdString += "\r\n";
    createdString += xmlBody;
}

void HttpToolKit::getDeletedStringForRestXml (string &deletedString, const string &xmlBody)
{
    deletedString  = "HTTP/1.1 204 Deleted\r\n";
    deletedString += "WWW-Authenticate: Basic realm=\"Wave\"\r\n";
    deletedString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    deletedString += "Content-Type: text/xml\r\n";
    deletedString += "\r\n";
    deletedString += xmlBody;
}

void HttpToolKit::getOkStringForGetForRestXml (string &okString, const string &xmlBody)
{
    okString  = "HTTP/1.1 200 OK\r\n";
    okString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    okString += "Content-Type: text/xml\r\n";
    okString += "\r\n";
    okString += xmlBody;
}

void HttpToolKit::getNoContentStringForPutForRestXml (string &noContentString)
{
    noContentString  = "HTTP/1.1 204 No Content\r\n";
    noContentString += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    noContentString += "Content-Type: text/html\r\n";
    noContentString += "\r\n";
}

void HttpToolKit::decodeUrl (string& url)
{
          string::size_type position = 0;
    const string::size_type length   = url.length ();

    StringUtils::replaceAllInstancesOfInputCharWith (url, '+', ' ');

    while (string::npos != (position = (url.find ('%', position + 1))))
    {
        if (length > (position + 2))
        {
            string tempString;
            char   c           = 0;

            tempString.insert (0, url, position + 1, 2);

            c = (char) strtoul (tempString.c_str (), NULL, 16);

            url.replace (position, 3, 1, c);
        }
    }
}

void HttpToolKit::getWaveServerPagePrePortion (string& waveServerPagePrePortion)
{
    waveServerPagePrePortion  = "HTTP/1.1 200 OK\r\n";
    waveServerPagePrePortion += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    waveServerPagePrePortion += "Content-Type: text/html\r\n";
    waveServerPagePrePortion += "\r\n";
    waveServerPagePrePortion += "<HTML>\r\n";

    waveServerPagePrePortion += "<HEAD>\r\n";

    waveServerPagePrePortion += "<STYLE>\r\n";

    waveServerPagePrePortion += "A:VISITED\r\n";
    waveServerPagePrePortion += "{\r\n";
    waveServerPagePrePortion += "COLOR:RGB(255, 0 ,0);\r\n";
    waveServerPagePrePortion += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortion += "}\r\n";

    waveServerPagePrePortion += "A:LINK\r\n";
    waveServerPagePrePortion += "{\r\n";
    waveServerPagePrePortion += "COLOR:RGB(0, 0 ,255);\r\n";
    waveServerPagePrePortion += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortion += "}\r\n";

    waveServerPagePrePortion += "A:HOVER\r\n";
    waveServerPagePrePortion += "{\r\n";
    waveServerPagePrePortion += "COLOR:RGB(0, 255 ,0)\r\n";
    waveServerPagePrePortion += "}\r\n";

    waveServerPagePrePortion += "A:ACTIVE\r\n";
    waveServerPagePrePortion += "{\r\n";
    waveServerPagePrePortion += "COLOR:RGB(255, 255 ,0)\r\n";
    waveServerPagePrePortion += "}\r\n";

    waveServerPagePrePortion += "</STYLE>\r\n";

    waveServerPagePrePortion += "</HEAD>\r\n";

    waveServerPagePrePortion += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    waveServerPagePrePortion += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    waveServerPagePrePortion += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    waveServerPagePrePortion += "<A>\r\n";
    waveServerPagePrePortion += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    waveServerPagePrePortion += "</A>\r\n";
    waveServerPagePrePortion += "</DIV>\r\n";

    waveServerPagePrePortion += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    waveServerPagePrePortion += "<A HREF=\"/\">Home</A>\r\n";
    waveServerPagePrePortion += " | \r\n";
    waveServerPagePrePortion += "<A HREF=\"/\">About</A>\r\n";
    waveServerPagePrePortion += " | \r\n";
    waveServerPagePrePortion += "<A HREF=\"/\">Contact Us</A>\r\n";
    waveServerPagePrePortion += "</DIV>\r\n";

    waveServerPagePrePortion += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    waveServerPagePrePortion += "<UL>\r\n";
    waveServerPagePrePortion += "<LI><A HREF=\"/\">Trace</A></BR></LI>\r\n";
    waveServerPagePrePortion += "<LI><A HREF=\"/\">Service</A></BR></LI>\r\n";
    waveServerPagePrePortion += "<LI><A HREF=\"/mylink\">Regression</A></BR></LI>\r\n";
    waveServerPagePrePortion += "</UL>\r\n";

    waveServerPagePrePortion += "<HR/>\r\n";

    waveServerPagePrePortion += "</DIV>\r\n";

    waveServerPagePrePortion += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
}

void HttpToolKit::getWaveServerPagePostPortion (string& waveServerPagePostPortion)
{
    waveServerPagePostPortion += "</DIV>\r\n";

    waveServerPagePostPortion += "</DIV>\r\n";
    waveServerPagePostPortion += "</BODY>\r\n";
    waveServerPagePostPortion += "</HTML>\r\n";
}


}
