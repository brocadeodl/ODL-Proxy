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

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceWorkerSendContext.h"

namespace WaveNs
{

CommandLineInterfaceWorkerSendContext::CommandLineInterfaceWorkerSendContext (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread, void *pPrismMessageContext)
    : m_frameworkStatus                                                        (frameworkStatus),
      m_pPrismMessage                                                          (pPrismMessage),
      m_prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread (prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread),
      m_pPrismMessageContext                                                   (pPrismMessageContext)
{
}

CommandLineInterfaceWorkerSendContext::~CommandLineInterfaceWorkerSendContext ()
{
}

FrameworkStatus CommandLineInterfaceWorkerSendContext::getFrameworkStatus ()
{
    return (m_frameworkStatus);
}

PrismMessage *CommandLineInterfaceWorkerSendContext::getPPrismMessage ()
{
    return (m_pPrismMessage);
}

PrismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread CommandLineInterfaceWorkerSendContext::getPrismMessageResponseHandler ()
{
    return (m_prismMessageResponseHandlerForCommandLineInterfaceReceiverWorkerThread);
}

void *CommandLineInterfaceWorkerSendContext::getPPrismMessageContext ()
{
    return (m_pPrismMessageContext);
}

}
