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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef APPLICATIONSERVICEUTILS_H
#define APPLICATIONSERVICEUTILS_H

#include "Framework/Types/Types.h"
#include "Framework/ServiceInterface/ApplicationServiceHelper.h"
#include "Framework/ServiceInterface/ApplicationServiceInterface.h"

namespace WaveNs
{

class WaveObjectManager;
class PrismMessage;

typedef PrismApplicationServiceCallback ApplicationServiceCallback;

class ApplicationServiceSendContext
{
    private :
    protected :
    public :
                                    ApplicationServiceSendContext (ApplicationServiceCallback pApplicationServiceCallback, void *pApplicationServiceContext);
                                   ~ApplicationServiceSendContext ();

        ApplicationServiceCallback  getPApplicationServiceCallback ();
        void                       *getPApplicationServiceContext ();

    // Now the data members

    private :
        ApplicationServiceCallback  m_pApplicationServiceCallback;
        void                       *m_pApplicationServiceContext;

    protected :
    public :
};

class ApplicationServiceUtils
{
    private :
    protected :
    public :
        static PrismServiceId getPrismServiceIdForApplicationServiceId (const UI32 &applicationServiceId);
        static ResourceId     sendToApplicationService                 (const PrismServiceId &sendingApplicationServicePrismServiceId, void *pPayLoad, const UI32 &payLoadLength, const PrismServiceId &prismServiceId, const LocationId &prismLocationId, ApplicationServiceCallback pApplicationServiceCallback, void *pApplicationContext);
        static void           replyToApplicationService                (void *pOutputPayLoad, const UI32 outputPayLoadLength, void *pPrismContext);

    // Now the data members

    private :
    protected :
    public :
};

class ApplicationServiceMessageHandlerContext
{
    private :
    protected :
    public :
                            ApplicationServiceMessageHandlerContext (WaveObjectManager *pWaveObjectManager, PrismMessage *pPrismMessage);
                           ~ApplicationServiceMessageHandlerContext ();

        WaveObjectManager *getPWaveObjectManager                  ();
        PrismMessage       *getPPrismMessage                        ();

    // Now the data members

    private :
        WaveObjectManager *m_pWaveObjectManager;
        PrismMessage       *m_pPrismMessage;

    protected :
    public :
};

}

#endif // APPLICATIONSERVICEUTILS_H
