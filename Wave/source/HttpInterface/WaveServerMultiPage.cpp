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

#include "HttpInterface/WaveServerMultiPage.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "HttpInterface/WaveServerPageDirectory.h"
#include "HttpInterface/HttpToolKit.h"
#include "Framework/Utils/ServerStreamingSocket.h"

namespace WaveNs
{

WaveServerMultiPage::WaveServerMultiPage (HttpInterfaceReceiverObjectManager *pHttpInterfaceReceiverObjectManager, const string &path)
    : WaveServerPage (pHttpInterfaceReceiverObjectManager, path)
{
}

WaveServerMultiPage::~WaveServerMultiPage ()
{
}

bool WaveServerMultiPage::isAKnownWaveServerMultiPageRequestHandlerForGet (const string &path) const
{
    map<string, WaveServerMultiPageRequestHandler>::const_iterator element    = m_multiPageRequestHandlersForGet.find (path);
    map<string, WaveServerMultiPageRequestHandler>::const_iterator endElement = m_multiPageRequestHandlersForGet.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool WaveServerMultiPage::isAKnownWaveServerMultiPageRequestHandlerForPost (const string &path) const
{
    map<string, WaveServerMultiPageRequestHandler>::const_iterator element    = m_multiPageRequestHandlersForPost.find (path);
    map<string, WaveServerMultiPageRequestHandler>::const_iterator endElement = m_multiPageRequestHandlersForPost.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool WaveServerMultiPage::isAKnownWaveServerMultiPageRequestHandlerForPut (const string &path) const
{
    map<string, WaveServerMultiPageRequestHandler>::const_iterator element    = m_multiPageRequestHandlersForPut.find (path);
    map<string, WaveServerMultiPageRequestHandler>::const_iterator endElement = m_multiPageRequestHandlersForPut.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool WaveServerMultiPage::isAKnownWaveServerMultiPageRequestHandlerForDelete (const string &path) const
{
    map<string, WaveServerMultiPageRequestHandler>::const_iterator element    = m_multiPageRequestHandlersForDelete.find (path);
    map<string, WaveServerMultiPageRequestHandler>::const_iterator endElement = m_multiPageRequestHandlersForDelete.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForGet (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler)
{
    if (false == (isAKnownWaveServerMultiPageRequestHandlerForGet (path)))
    {
        m_multiPageRequestHandlersForGet[path] = pWaveServerMultiPageRequestHandler;
        m_multiPageOptionsForGet.push_back (path);

        WaveServerPageDirectory::registerServerPage ((getPath ()) + "/" + path, this);
    }
    else
    {
       trace (TRACE_LEVEL_FATAL, " WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForGet : An option already exists with the path \"" + path + "\"");
       prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForPost (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler)
{
    if (false == (isAKnownWaveServerMultiPageRequestHandlerForPost (path)))
    {
        m_multiPageRequestHandlersForPost[path] = pWaveServerMultiPageRequestHandler;
        m_multiPageOptionsForPost.push_back (path);

        //WaveServerPageDirectory::registerServerPage ((getPath ()) + "/" + path, this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, " WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForPost : An option already exists with the path \"" + path + "\"");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForPut (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler)
{
    if (false == (isAKnownWaveServerMultiPageRequestHandlerForPut (path)))
    {
        m_multiPageRequestHandlersForPut[path] = pWaveServerMultiPageRequestHandler;
        m_multiPageOptionsForPut.push_back (path);

        //WaveServerPageDirectory::registerServerPage ((getPath ()) + "/" + path, this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, " WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForPut : An option already exists with the path \"" + path + "\"");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForDelete (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler)
{
    if (false == (isAKnownWaveServerMultiPageRequestHandlerForDelete (path)))
    {
        m_multiPageRequestHandlersForDelete[path] = pWaveServerMultiPageRequestHandler;
        m_multiPageOptionsForDelete.push_back (path);

        //WaveServerPageDirectory::registerServerPage ((getPath ()) + "/" + path, this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, " WaveServerMultiPage::addWaveServerMultiPageRequestHandlerForDelete : An option already exists with the path \"" + path + "\"");
        prismAssert (false, __FILE__, __LINE__);
    }
}

WaveServerMultiPageRequestHandler WaveServerMultiPage::getWaveServerMultiPageRequestHandlerForGet (const string &path)
{
     WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = NULL;

    if (true == (isAKnownWaveServerMultiPageRequestHandlerForGet (path)))
    {
        pWaveServerMultiPageRequestHandler = m_multiPageRequestHandlersForGet[path];
    }

    return (pWaveServerMultiPageRequestHandler);
}

WaveServerMultiPageRequestHandler WaveServerMultiPage::getWaveServerMultiPageRequestHandlerForPost (const string &path)
{
    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = NULL;

    if (true == (isAKnownWaveServerMultiPageRequestHandlerForPost (path)))
    {
        pWaveServerMultiPageRequestHandler = m_multiPageRequestHandlersForPost[path];
    }

    return (pWaveServerMultiPageRequestHandler);
}

WaveServerMultiPageRequestHandler WaveServerMultiPage::getWaveServerMultiPageRequestHandlerForPut (const string &path)
{
    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = NULL;

    if (true == (isAKnownWaveServerMultiPageRequestHandlerForPut (path)))
    {
        pWaveServerMultiPageRequestHandler = m_multiPageRequestHandlersForPut[path];
    }

    return (pWaveServerMultiPageRequestHandler);
}

WaveServerMultiPageRequestHandler WaveServerMultiPage::getWaveServerMultiPageRequestHandlerForDelete (const string &path)
{
    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = NULL;

    if (true == (isAKnownWaveServerMultiPageRequestHandlerForDelete (path)))
    {
        pWaveServerMultiPageRequestHandler = m_multiPageRequestHandlersForDelete[path];
    }

    return (pWaveServerMultiPageRequestHandler);
}

void WaveServerMultiPage::getListOfOptionsForGet (vector<string> &optionsForGet) const
{
    optionsForGet = m_multiPageOptionsForGet;
}

void WaveServerMultiPage::getListOfOptionsForPost (vector<string> &optionsForPost) const
{
    optionsForPost = m_multiPageOptionsForPost;
}

void WaveServerMultiPage::getListOfOptionsForPut (vector<string> &optionsForPut) const
{
    optionsForPut = m_multiPageOptionsForPut;
}

void WaveServerMultiPage::getListOfOptionsForDelete (vector<string> &optionsForDelete) const
{
    optionsForDelete = m_multiPageOptionsForDelete;
}

void WaveServerMultiPage::get (const HttpRequest &httpRequest)
{
    const string path                                  = getPath                         ();
    const string uri                                   = httpRequest.getUri              ();
    const bool   isWildCardRequestHandlerRequiredAtTop = callWildCardRequestHandlerAtTop ();

    if (path == uri)
    {
        if (true == isWildCardRequestHandlerRequiredAtTop)
        {
            wildCardRequestHandlerForGet (httpRequest);
        }
        else
        {
            WaveServerPage::get (httpRequest);
        }
    }
    else
    {
        string            adjustedPath         = path + "/";
        string            adjustedUri          = uri;
        UI32              lengthOfAdjustedPath = adjustedPath.length ();
        string::size_type position             = uri.find (adjustedPath, 0);

        if (string::npos != position)
        {
            adjustedUri.replace (0, lengthOfAdjustedPath, "");
        }

        trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::get : Adjusted URI : \"" + adjustedUri + "\"");

        WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = getWaveServerMultiPageRequestHandlerForGet (adjustedUri);

        if (NULL != pWaveServerMultiPageRequestHandler)
        {
            (this->*pWaveServerMultiPageRequestHandler) (httpRequest);
        }
        else
        {
            wildCardRequestHandlerForGet (httpRequest);
        }
    }
}

void WaveServerMultiPage::post (const HttpRequest &httpRequest)
{
    const string path = getPath            ();
    const string uri  = httpRequest.getUri ();

    string            adjustedPath         = path + "/";
    string            adjustedUri          = uri;
    UI32              lengthOfAdjustedPath = adjustedPath.length ();
    string::size_type position             = uri.find (adjustedPath, 0);

    if (string::npos != position)
    {
        adjustedUri.replace (0, lengthOfAdjustedPath, "");
    }

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::post : Adjusted URI : \"" + adjustedUri + "\"");

    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = getWaveServerMultiPageRequestHandlerForPost (adjustedUri);

    if (NULL != pWaveServerMultiPageRequestHandler)
    {
        (this->*pWaveServerMultiPageRequestHandler) (httpRequest);
    }
    else
    {
        wildCardRequestHandlerForPost (httpRequest);
    }
}

void WaveServerMultiPage::put (const HttpRequest &httpRequest)
{
    const string path = getPath            ();
    const string uri  = httpRequest.getUri ();

    string            adjustedPath         = path + "/";
    string            adjustedUri          = uri;
    UI32              lengthOfAdjustedPath = adjustedPath.length ();
    string::size_type position             = uri.find (adjustedPath, 0);

    if (string::npos != position)
    {
        adjustedUri.replace (0, lengthOfAdjustedPath, "");
    }

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::put : Adjusted URI : \"" + adjustedUri + "\"");

    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = getWaveServerMultiPageRequestHandlerForPut (adjustedUri);

    if (NULL != pWaveServerMultiPageRequestHandler)
    {
        (this->*pWaveServerMultiPageRequestHandler) (httpRequest);
    }
    else
    {
        wildCardRequestHandlerForPut (httpRequest);
    }
}

void WaveServerMultiPage::deleteMethod (const HttpRequest &httpRequest)
{
    const string path = getPath            ();
    const string uri  = httpRequest.getUri ();

    string            adjustedPath         = path + "/";
    string            adjustedUri          = uri;
    UI32              lengthOfAdjustedPath = adjustedPath.length ();
    string::size_type position             = uri.find (adjustedPath, 0);

    if (string::npos != position)
    {
        adjustedUri.replace (0, lengthOfAdjustedPath, "");
    }

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::deleteMethod : Adjusted URI : \"" + adjustedUri + "\"");

    WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler = getWaveServerMultiPageRequestHandlerForDelete (adjustedUri);

    if (NULL != pWaveServerMultiPageRequestHandler)
    {
        (this->*pWaveServerMultiPageRequestHandler) (httpRequest);
    }
    else
    {
        wildCardRequestHandlerForDelete (httpRequest);
    }
}

void WaveServerMultiPage::wildCardRequestHandlerForGet (const HttpRequest &httpRequest)
{
    string                 uri                    = httpRequest.getUri ();
    string                 httpResponse;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::wildCardRequestHandlerForGet : There is no page implemented with the given path \"" + uri + "\"");

    HttpToolKit::getNotFoundErrorString (httpResponse);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponse;
    }
}

void WaveServerMultiPage::wildCardRequestHandlerForPost (const HttpRequest &httpRequest)
{
    string                 uri                    = httpRequest.getUri ();
    string                 httpResponse;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::wildCardRequestHandlerForPost : There is no page implemented with the given path \"" + uri + "\"");

    HttpToolKit::getNotFoundErrorString (httpResponse);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponse;
    }
}

void WaveServerMultiPage::wildCardRequestHandlerForPut (const HttpRequest &httpRequest)
{
    string                 uri                    = httpRequest.getUri ();
    string                 httpResponse;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::wildCardRequestHandlerForPut : There is no page implemented with the given path \"" + uri + "\"");

    HttpToolKit::getNotFoundErrorString (httpResponse);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponse;
    }
}

void WaveServerMultiPage::wildCardRequestHandlerForDelete (const HttpRequest &httpRequest)
{
    string                 uri                    = httpRequest.getUri ();
    string                 httpResponse;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket ();

    trace (TRACE_LEVEL_DEBUG, "WaveServerMultiPage::wildCardRequestHandlerForDelete : There is no page implemented with the given path \"" + uri + "\"");

    HttpToolKit::getNotFoundErrorString (httpResponse);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponse;
    }
}

bool WaveServerMultiPage::callWildCardRequestHandlerAtTop () const
{
    return (false);
}

}
