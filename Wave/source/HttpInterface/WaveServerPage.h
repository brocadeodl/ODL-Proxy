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

#ifndef WAVESERVERPAGE_H
#define WAVESERVERPAGE_H
#include <Framework/Attributes/Attribute.h>
#include "HttpRequest.h"
#include <Framework/ObjectModel/WaveWorker.h>

namespace WaveNs
{

class HttpInterfaceReceiverObjectManager;

class WaveServerPage : public WaveWorker
{

    private :
    protected :
                void setPageHeading            (const string &pageHeading);
        virtual void getListOfOptionsForGet    (vector<string> &optionsForGet) const;
        virtual void getListOfOptionsForPost   (vector<string> &optionsForPost) const;
        virtual void getListOfOptionsForPut    (vector<string> &optionsForPost) const;
        virtual void getListOfOptionsForDelete (vector<string> &optionsForDelete) const;

    public :
                       WaveServerPage                        (HttpInterfaceReceiverObjectManager *pHttpInterfaceReceiverObjectManager, const string &path);
        virtual       ~WaveServerPage                        ();

        virtual void   get                                   (const HttpRequest &httpRequest);
        virtual void   head                                  (const HttpRequest &httpRequest);
        virtual void   post                                  (const HttpRequest &httpRequest);
        virtual void   put                                   (const HttpRequest &httpRequest);
        virtual void   deleteMethod                          (const HttpRequest &httpRequest);
        virtual void   options                               (const HttpRequest &httpRequest);
        virtual void   traceX                                (const HttpRequest &httpRequest);
        virtual void   connect                               (const HttpRequest &httpRequest);

                string getPageHeading                        () const;

                void   getWaveServerPagePrePortionForGet     (string &waveServerPagePrePortionForGet);
                void   getWaveServerPagePostPortionForGet    (string &waveServerPagePostPortionForGet);

                void   getWaveServerPagePrePortionForPost    (string &waveServerPagePrePortionForPost);
                void   getWaveServerPagePostPortionForPost   (string &waveServerPagePostPortionForPost);

                void   getWaveServerPagePrePortionForPut     (string &waveServerPagePrePortionForPost);
                void   getWaveServerPagePostPortionForPut    (string &waveServerPagePostPortionForPost);

                void   getWaveServerPagePrePortionForDelete  (string &waveServerPagePrePortionForDelete);
                void   getWaveServerPagePostPortionForDelete (string &waveServerPagePostPortionForDelete);

                string getPath                               () const;


    // Now the data members

    private :
        string m_path;
        string m_pageHeading;

    protected :
    public :
};

}

#endif // WAVESERVERPAGE_H

