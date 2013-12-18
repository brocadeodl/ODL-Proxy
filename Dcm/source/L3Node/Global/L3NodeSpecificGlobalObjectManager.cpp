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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pasu                                                     *
 **************************************************************************/


#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"

using namespace WaveNs;


/* 
 * NOTE: This OM is used only for handling failover for L3 Node specific
 * configuration. PLEASE DON'T USE THIS FOR ANY CONFIG PATH.
 */
namespace DcmNs{


L3NodeSpecificGlobalObjectManager::L3NodeSpecificGlobalObjectManager()
    :WaveObjectManager (L3NodeSpecificGlobalObjectManager::getClassName())

{
}

L3NodeSpecificGlobalObjectManager::~L3NodeSpecificGlobalObjectManager()
{
}

L3NodeSpecificGlobalObjectManager *L3NodeSpecificGlobalObjectManager::getInstance()
{
    static L3NodeSpecificGlobalObjectManager *s_pL3NodeSpecificGlobalObjectManager = new L3NodeSpecificGlobalObjectManager ();
    WaveNs::prismAssert (NULL != s_pL3NodeSpecificGlobalObjectManager, __FILE__, __LINE__);
    return (s_pL3NodeSpecificGlobalObjectManager);
}

string L3NodeSpecificGlobalObjectManager::getClassName()
{
    return ("L3NodeSpecificGlobalObjectManager");
}

PrismServiceId  L3NodeSpecificGlobalObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *L3NodeSpecificGlobalObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *L3NodeSpecificGlobalObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL ;
    return (pWaveManagedObject);
}


void L3NodeSpecificGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason  = pFailoverAsynchronousContext->getfailoverReason ();

    do
    {
        // 1. Check for controlled failover.

        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED != failoverReason)
        {
            break;
        }

        trace (TRACE_LEVEL_DEVEL, "L3NodeSpecificGlobalObjectManager::failover");

        // 2. Get vector of failed location ids.

        vector<LocationId>  failedLocationIds       = pFailoverAsynchronousContext->getfailedLocationIds ();
        UI32                numberOfFailedLocations = failedLocationIds.size ();

        if (0 < numberOfFailedLocations)
        {
            // 3. Get vector of class names of Owned Managed Objects.

            vector<string>      ownedManagedObjectClassNames;
            getAllOwnedManagedObjectClassNames (ownedManagedObjectClassNames);
            UI32 numberOfOwnedManagedObjectClassNames = ownedManagedObjectClassNames.size ();

            // 4. Start transaction.

            startTransaction ();    // [

            for (UI32 i = 0; i < numberOfOwnedManagedObjectClassNames; i++)
            {
                string managedObjectClassName = ownedManagedObjectClassNames[i];

                // 5. Skip if not a Local Managed Object.

                if (false == OrmRepository::isALocalManagedObject (managedObjectClassName))
                {
                    trace (TRACE_LEVEL_DEVEL, "L3NodeSpecificGlobalObjectManager::failover : [" + managedObjectClassName  + "] is not a Local Managed Object. Skipped.");

                    continue;
                }
                WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion (managedObjectClassName);

                // 6. For current class name, query and delete Managed Objects for all failed locations.

                for (UI32 j = 0; j < numberOfFailedLocations; j++)
                {
                    LocationId failedLocationId = failedLocationIds[j];
                    ObjectId locationIdObjectId = FrameworkToolKit::getObjectIdForLocationId (failedLocationId);
                    deleteCtxt->addOrAttribute (new AttributeObjectId (locationIdObjectId, "ownerWaveNodeObjectId"));

                }
                trace (TRACE_LEVEL_DEVEL, string ("L3NodeSpecificGlobalObjectManager::failover : deleting Managed Object Of Class [") + managedObjectClassName + string ("]"));
                deleteMultipleWaveManagedObjects (deleteCtxt);

            }

            // 7. Commit Transaction.

            commitTransaction ();
        }

    } while (0);

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();
}


}

