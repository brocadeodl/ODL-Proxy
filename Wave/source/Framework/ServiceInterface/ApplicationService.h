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

#ifndef APPLICATIONSERVICE_H
#define APPLICATIONSERVICE_H

#include "Framework/Utils/PrismMutex.h"
#include "Framework/ServiceInterface/ApplicationServiceRepository.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ServiceInterface/ApplicationServiceHelper.h"

namespace WaveNs
{

class ApplicationServiceMessage;

class ApplicationService : public WaveObjectManager
{
    private :
                              ApplicationService                (const string &applicationServiceName, const UI32 &applicationServiceId);

        virtual PrismMessage *createMessageInstance             (const UI32 &operationCode);

                void          applicationServiceMessageHandler  (ApplicationServiceMessage *pApplicationServiceMessage);
                void          applicationServiceMessageCallback (FrameworkStatus frameworkStatus, ApplicationServiceMessage *pApplicationServiceMessage, void *pContext);

    protected :
    public :
        virtual       ~ApplicationService                       ();

        static  UI32  registerNewApplicationService             (const string &newApplicationServiceName);
        static  void  createNewApplicationServices              ();
        static  void  addMessageHandler                         (const UI32 &applicationServiceId, const UI32 &operationCode, ApplicationServiceMessageHandler handler);

                void  addMessageHandlerToThisApplicationService (const UI32 &operationCode, ApplicationServiceMessageHandler handler);

    // Now the data members

    private :
        static ApplicationServiceRepository                *m_pApplicationServiceRepository;

               UI32                                         m_applicationServiceId;
               map<UI32, ApplicationServiceMessageHandler>  m_applicaionServiceMessageHandlersMap;

    protected :
    public :

    friend class ApplicationServiceRepository;
    friend class ApplicationServiceUtils;
};

}

#endif // APPLICATIONSERVICE_H
