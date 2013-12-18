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

#ifndef NSPNETWORKSWAVESERVERMULTIPAGE_H
#define NSPNETWORKSWAVESERVERMULTIPAGE_H

#include <HttpInterface/WaveServerMultiPage.h>
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"

using namespace WaveNs;

namespace NspNs
{

class NspNetworksWaveServerMultiPage : public WaveServerMultiPage
{
    private :
                void getForListByRangeAndFields      (const HttpRequest &httpRequest, const bool &filterByRange = true);
                void getForList                      (const HttpRequest &httpRequest);
                void getForCreate                    (const HttpRequest &httpRequest);

        virtual void wildCardRequestHandlerForGet    (const HttpRequest &httpRequest);
        virtual void wildCardRequestHandlerForPost   (const HttpRequest &httpRequest);
        virtual bool callWildCardRequestHandlerAtTop () const;

    protected :
    public :
                 NspNetworksWaveServerMultiPage (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const string& path);
        virtual ~NspNetworksWaveServerMultiPage ();

        // Now the data members

    private :
    protected :
    public :
};

}

#endif // NSPNETWORKSWAVESERVERMULTIPAGE_H
