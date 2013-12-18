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

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "Framework/Types/Types.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class ServerStreamingSocket;

class HttpRequest
{
    private :
    protected :
    public :
                                 HttpRequest                (string &httpRequest, ServerStreamingSocket *pServerStreamingSocket = NULL);
                                ~HttpRequest                ();

        WaveHttpInterfaceMethod  getWaveHttpInterfaceMethod () const;
        void                     setWaveHttpInterfaceMethod (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod);

        string                   getUri                     () const;
        void                     setUri                     (const string &uri);

        vector<string>           getUriTokens               () const;
        void                     setUriTokens               (const vector<string> &uriTokens);

        string                   getAuthorizarion           () const;
        void                     setAuthorization           (const string &authorization);

        UI32                     getContentLength           () const;
        void                     setContentLength           (const UI32 &contentLength);

        WaveHttpContentType      getContentType             () const;
        void                     setContentType             (const WaveHttpContentType &contentType);

        string                   getContent                 () const;
        void                     setContent                 (const string &content);

        string                   getContentBoundary         () const;
        void                     setContentBoundary         (const string &contentBoundry);

        UI32                     getNumberOfEntities        () const;
        void                     getEntityAtIndex           (const UI32 &index, string &entityName, string &entityValue, string &entityFileName);

        void                     getEntityValueByName       (const string &entitityName, string &entityValue) const;
        bool                     isAKnownEntity             (const string &entitityName) const;

        ServerStreamingSocket   *getPServerStreamingSocket  () const;

        string                   getHost                    () const;
        void                     setHost                    (const string &host);

        bool                     getIsContentReadIncomplete () const;
        void                     setIsContentReadIncomplete (bool isContentReadIncomplete);

    // Now the data members

    private :
               WaveHttpInterfaceMethod m_waveHttpInterfaceMethod;
               string                  m_uri;
               vector<string>          m_uriTokens;
               string                  m_authorization;
               UI32                    m_contentLength;
               WaveHttpContentType     m_contentType;
               string                  m_contentBoundary;
               string                  m_content;
               bool                    m_isContentReadIncomplete;
               vector<string>          m_entityNames;
               vector<string>          m_entityFileNames;
               vector<string>          m_entityValues;
               map<string, string>     m_entitiesMap;
               string                  m_host;

               ServerStreamingSocket  *m_pServerStreamingSocket;

        static string                  s_authorizationString;
        static string                  s_contentLengthString;
        static string                  s_contentTypeString;
        static string                  s_hostString;

    protected :
    public :
};

}

#endif // HTTPREQUEST_H
