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

#ifndef APPLICATIONSERVICEHELPER_H
#define APPLICATIONSERVICEHELPER_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/ServiceInterface/ApplicationServiceInterface.h"
#include <map>

namespace WaveNs
{

class PrismMessage;

typedef PrismApplicationServiceMessageHandler ApplicationServiceMessageHandler;

class ApplicationServiceHelper
{
    private :
    protected :
    public :
        static void lock   ();
        static void unlock ();

        static UI32                                         getNextApplicationServiceId         ();
        static void                                         addMessageHandler                   (const UI32 &applicationServiceId, const UI32 &operationCode, ApplicationServiceMessageHandler handler);
        static map<UI32, ApplicationServiceMessageHandler> *getHandlersMapForApplicationService (const UI32 &applicationServiceId);

    // Now the data members

    private :
        static PrismMutex                                               m_applicationServiceHelperMutex;
        static UI32                                                     m_nextApplicationServiceId;
        static map<UI32, map<UI32, ApplicationServiceMessageHandler> *> m_allApplicationServiceMessageHandlersMap;

    protected :
    public :
};

}

#endif // APPLICATIONSERVICEHELPER_H
