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

/********************************************************************
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                           *
 *   Author : krangara                                              *
 ********************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "FspfService/Global/FspfServiceGlobalObjectManager.h"
#include "FspfService/Local/BcastPriorityLocalManagedObject.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

FspfServiceGlobalObjectManager::FspfServiceGlobalObjectManager ()
    : WaveObjectManager (getClassName ())
{

}

FspfServiceGlobalObjectManager::~FspfServiceGlobalObjectManager ()
{

}

string FspfServiceGlobalObjectManager::getClassName ()
{
    return ("FspfServiceGlobalObjectManager");
}

FspfServiceGlobalObjectManager *FspfServiceGlobalObjectManager::getInstance ()
{
    static FspfServiceGlobalObjectManager *pFspfServiceGlobalObjectManager = NULL;

    if (NULL == pFspfServiceGlobalObjectManager)
    {
        pFspfServiceGlobalObjectManager = new FspfServiceGlobalObjectManager ();
        WaveNs::prismAssert (NULL != pFspfServiceGlobalObjectManager, __FILE__, __LINE__);
    }

    return (pFspfServiceGlobalObjectManager);
}

PrismServiceId FspfServiceGlobalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FspfServiceGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string ("FspfServiceGlobalObjectManager::failover reason") + FrameworkToolKit::localize(failoverReason));

    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
    {
        UI32 noOfFailedLocations = failedLocationIds.size ();


        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
            vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], BcastPriorityLocalManagedObject::getClassName ());

            trace (TRACE_LEVEL_INFO, string ("FspfServiceGlobalObjectManager::failover for Location Id ") + failedLocationIds[i]);

            startTransaction ();

            UI32 sizeOfManagedObjects = pWaveManagedObjects->size ();

            for (UI32 j = 0; j < sizeOfManagedObjects; j++)
            {
                delete (*pWaveManagedObjects)[j];
            }

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "FspfServiceGlobalObjectManager::failover can not commit local managed object delete to database ");
                prismAssert (false, __FILE__, __LINE__);
            }

            pWaveManagedObjects->clear ();

            delete pWaveManagedObjects;
        }
    }
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}

}

