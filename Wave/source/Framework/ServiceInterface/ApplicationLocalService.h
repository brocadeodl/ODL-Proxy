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

#ifndef APPLICATIONLOCALSERVICE_H
#define APPLICATIONLOCALSERVICE_H

#include "Framework/Utils/PrismMutex.h"
#include "Framework/ServiceInterface/ApplicationServiceRepository.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ServiceInterface/ApplicationServiceHelper.h"

namespace WaveNs
{

class ApplicationServiceMessage;

class ApplicationLocalService : public WaveLocalObjectManager
{
    private :
                              ApplicationLocalService               (const string &applicationServiceName, const UI32 &applicationLocalServiceId);

        virtual PrismMessage *createMessageInstance                 (const UI32 &operationCode);

                void          applicationLocalServiceMessageHandler  (ApplicationServiceMessage *pApplicationServiceMessage);
                void          applicationLocalServiceMessageCallback (FrameworkStatus frameworkStatus, ApplicationServiceMessage *pApplicationServiceMessage, void *pContext);

    protected :
    public :
        virtual       ~ApplicationLocalService                        ();

        static  UI32   registerNewApplicationLocalService             (const string &newApplicationLocalServiceName);
        static  void   createNewApplicationLocalServices              ();
        static  void   addMessageHandler                              (const UI32 &applicationServiceId, const UI32 &operationCode, ApplicationServiceMessageHandler handler);

                void   addMessageHandlerToThisApplicationLocalService (const UI32 &operationCode, ApplicationServiceMessageHandler handler);

    // Now the data members

    private :
        static ApplicationServiceRepository                *m_pApplicationServiceRepository;

               UI32                                         m_applicationLocalServiceId;
               map<UI32, ApplicationServiceMessageHandler>  m_applicaionLocalServiceMessageHandlersMap;

    protected :
    public :

    friend class ApplicationServiceRepository;
    friend class ApplicationServiceUtils;
};

}

#endif // APPLICATIONLOCALSERVICE_H
