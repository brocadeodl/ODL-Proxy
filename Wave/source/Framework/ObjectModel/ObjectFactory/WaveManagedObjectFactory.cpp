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

#include "Framework/ObjectModel/ObjectFactory/WaveManagedObjectFactory.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
namespace WaveNs
{

WaveManagedObject *WaveManagedObjectFactory::getWaveManagedObjectInstance (const PrismServiceId &prismServiceId, const  string &className)
{
    WaveObjectManager *pWaveObjectManager = getWaveObjectManagerForManagedObject (prismServiceId, className);
    WaveManagedObject *pWaveManagedObject = NULL;

    if (NULL != pWaveObjectManager)
    {
        pWaveManagedObject = pWaveObjectManager->createManagedObjectInstanceWrapper (className);
    }

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getMessageInstance : Could not create Managed Object instance for  Service : " + FrameworkToolKit::getServiceNameById (prismServiceId) + ", Class Name = " + className + ".");
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getMessageInstance : Have you implemented the createManagedObjectInstance on the object manager yet???.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pWaveManagedObject);
}

WaveObjectManager *WaveManagedObjectFactory::getWaveObjectManagerForManagedObject (const PrismServiceId &prismServiceId, const  string &className)
{
    PrismThread        *pPrismThread        = NULL;
    WaveObjectManager *pWaveObjectManager = NULL;

    pPrismThread = PrismThread::getPrismThreadForServiceId (prismServiceId);

    if (NULL != pPrismThread)
    {
        pWaveObjectManager = pPrismThread->getWaveObjectManagerForManagedClass (className);
    }

    return (pWaveObjectManager);
}

}
