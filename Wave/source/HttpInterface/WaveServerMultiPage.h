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

#ifndef WAVESERVERMULTIPAGE_H
#define WAVESERVERMULTIPAGE_H

#include "WaveServerPage.h"

namespace WaveNs
{

class WaveServerMultiPage;
class HttpInterfaceReceiverObjectManager;

typedef void (WaveServerMultiPage::* WaveServerMultiPageRequestHandler) (const HttpRequest &httpRequest);

class WaveServerMultiPage : public WaveServerPage
{
    private :
                bool                              isAKnownWaveServerMultiPageRequestHandlerForGet    (const string &path) const;
                bool                              isAKnownWaveServerMultiPageRequestHandlerForPost   (const string &path) const;
                bool                              isAKnownWaveServerMultiPageRequestHandlerForPut    (const string &path) const;
                bool                              isAKnownWaveServerMultiPageRequestHandlerForDelete (const string &path) const;

                WaveServerMultiPageRequestHandler getWaveServerMultiPageRequestHandlerForGet         (const string &path);
                WaveServerMultiPageRequestHandler getWaveServerMultiPageRequestHandlerForPost        (const string &path);
                WaveServerMultiPageRequestHandler getWaveServerMultiPageRequestHandlerForPut         (const string &path);
                WaveServerMultiPageRequestHandler getWaveServerMultiPageRequestHandlerForDelete      (const string &path);

        virtual void                              get                                                (const HttpRequest &httpRequest);
        virtual void                              post                                               (const HttpRequest &httpRequest);
        virtual void                              put                                                (const HttpRequest &httpRequest);
        virtual void                              deleteMethod                                       (const HttpRequest &httpRequest);

    protected :
                void addWaveServerMultiPageRequestHandlerForGet    (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler);
                void addWaveServerMultiPageRequestHandlerForPost   (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler);
                void addWaveServerMultiPageRequestHandlerForPut    (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler);
                void addWaveServerMultiPageRequestHandlerForDelete (const string &path, WaveServerMultiPageRequestHandler pWaveServerMultiPageRequestHandler);

        virtual void getListOfOptionsForGet                        (vector<string> &optionsForGet) const;
        virtual void getListOfOptionsForPost                       (vector<string> &optionsForPost) const;
        virtual void getListOfOptionsForPut                        (vector<string> &optionsForPut) const;
        virtual void getListOfOptionsForDelete                     (vector<string> &optionsForPut) const;

        virtual void wildCardRequestHandlerForGet                  (const HttpRequest &httpRequest);
        virtual void wildCardRequestHandlerForPost                 (const HttpRequest &httpRequest);
        virtual void wildCardRequestHandlerForPut                  (const HttpRequest &httpRequest);
        virtual void wildCardRequestHandlerForDelete               (const HttpRequest &httpRequest);

        virtual bool callWildCardRequestHandlerAtTop               () const;

    public :
                 WaveServerMultiPage (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path);
        virtual ~WaveServerMultiPage ();

    // Now the data members

    private :
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForGet;
        vector<string>                                 m_multiPageOptionsForGet;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForHead;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForPut;
        vector<string>                                 m_multiPageOptionsForPut;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForPost;
        vector<string>                                 m_multiPageOptionsForPost;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForDelete;
        vector<string>                                 m_multiPageOptionsForDelete;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForOptions;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForConnect;
        map<string, WaveServerMultiPageRequestHandler> m_multiPageRequestHandlersForTrace;

    protected :
    public :
};

}

#endif // WAVESERVERMULTIPAGE_H
