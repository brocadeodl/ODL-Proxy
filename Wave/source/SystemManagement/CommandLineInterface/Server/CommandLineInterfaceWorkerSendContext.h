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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef COMMANDLINEINTERFACEWORKERSENDCONTEXT_H
#define COMMANDLINEINTERFACEWORKERSENDCONTEXT_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverWorkerThread.h"

namespace WaveNs
{

class CommandLineInterfaceWorkerSendContext
{
    private :
    protected :
    public :
                                                                                CommandLineInterfaceWorkerSendContext (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext);
                                                                               ~CommandLineInterfaceWorkerSendContext ();

        FrameworkStatus                                                         getFrameworkStatus                    ();
        PrismMessage                                                           *getPPrismMessage                      ();
        PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread  getPrismMessageResponseHandler        ();
        void                                                                   *getPPrismMessageContext               ();

    // Now the Data Members

    private :
        FrameworkStatus                                                         m_frameworkStatus;
        PrismMessage                                                           *m_pPrismMessage;
        PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread  m_prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread;
        void                                                                   *m_pPrismMessageContext;

    protected :
    public :
};

}

#endif // COMMANDLINEINTERFACEWORKERSENDCONTEXT_H
