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

#include "Framework/ServiceInterface/Test/PrismNativeTest1Service.h"
#include "Framework/ServiceInterface/Test/PrismNativeTest1ServiceMessages.h"

namespace WaveNs
{

PrismNativeTest1Service::PrismNativeTest1Service ()
    : WaveObjectManager ("PrismNativeTest1Service")
{
   addOperationMap (WAVE_NATIVE_TEST1_SERVICE_MESSAGE1, reinterpret_cast<PrismMessageHandler> (&PrismNativeTest1Service::prismNativeTest1ServiceMessage1Handler));
}

PrismNativeTest1Service::~PrismNativeTest1Service ()
{
}

PrismNativeTest1Service *PrismNativeTest1Service::getInstance ()
{
    static PrismNativeTest1Service *pPrismNativeTest1Service = NULL;

    if (NULL == pPrismNativeTest1Service)
    {
        pPrismNativeTest1Service = new PrismNativeTest1Service ();
        WaveNs::prismAssert (NULL != pPrismNativeTest1Service, __FILE__, __LINE__);
    }

    return (pPrismNativeTest1Service);
}

PrismServiceId PrismNativeTest1Service::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void PrismNativeTest1Service::prismNativeTest1ServiceMessage1Handler (PrismNativeTest1ServiceMessage1 *pMsg)
{
    trace (TRACE_LEVEL_DEVEL , "Prism Native Test1 Messge Received");

    static UI32 numberOfReceivedMessagesSoFar = 0;

    numberOfReceivedMessagesSoFar++;

    if (0 == (numberOfReceivedMessagesSoFar % 1000))
    {
        cout << "PrismNativeTest1Service::prismNativeTest1ServiceMessage1Handler : Received " <<  numberOfReceivedMessagesSoFar << " Messages. \r";
        fflush (stdout);
    }

    if (100000 == numberOfReceivedMessagesSoFar)
    {
        numberOfReceivedMessagesSoFar = 0;
    }

    pMsg->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMsg);
}

}
