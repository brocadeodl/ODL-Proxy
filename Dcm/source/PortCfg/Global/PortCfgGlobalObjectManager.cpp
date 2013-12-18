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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                             *
 *   Author : Murali Lachireddy                                       *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include <PortCfg/Global/PortCfgGlobalObjectManager.h>
#include "PortCfg/Local/PortFCCfgLocalManagedObject.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

PortCfgGlobalObjectManager::PortCfgGlobalObjectManager ()
    : WaveObjectManager (getClassName ())
{

}

PortCfgGlobalObjectManager::~PortCfgGlobalObjectManager ()
{

}

string PortCfgGlobalObjectManager::getClassName ()
{
    return ("PortCfgGlobalObjectManager");
}

PortCfgGlobalObjectManager *PortCfgGlobalObjectManager::getInstance ()
{
    static PortCfgGlobalObjectManager *pPortCfgGlobalObjectManager = NULL;

    if (NULL == pPortCfgGlobalObjectManager)
    {
        pPortCfgGlobalObjectManager = new PortCfgGlobalObjectManager ();
        WaveNs::prismAssert (NULL != pPortCfgGlobalObjectManager, __FILE__, __LINE__);
    }

    return (pPortCfgGlobalObjectManager);
}

PrismServiceId PortCfgGlobalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void PortCfgGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string ("PortCfgGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));

    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
    {
        UI32 noOfFailedLocations = failedLocationIds.size ();


        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
            vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], PortFCCfgLocalManagedObject::getClassName ());
            trace (TRACE_LEVEL_INFO, string ("PortCfgGlobalObjectManager::failover for Location Id ") + failedLocationIds[i]);

            startTransaction ();

            UI32 sizeOfManagedObjects                   = pWaveManagedObjects->size ();

            for (UI32 j = 0; j < sizeOfManagedObjects; j++)
            {
                delete (*pWaveManagedObjects)[j];
            }   
            
            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PortCfgGlobalObjectManager::failover can not commit local managed object delete to database ");
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
