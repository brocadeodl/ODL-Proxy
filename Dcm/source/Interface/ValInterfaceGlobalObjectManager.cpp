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
 *   Author : Himanshu Varshney                                       *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include <Interface/ValInterfaceGlobalObjectManager.h>
#include <Interface/InterfaceLocalManagedObject.h>
#include "Interface/Composition/InterfaceCompositionLocalManagedObject.h"
#include "Interface/Association/InterfaceAssociationLocalManagedObject.h"
#include <Interface/InterfaceNodeSpecificLocalManagedObject.h>
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Interface/ContainerInList/InterfaceContainerInListLocalManagedObject.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionLocalManagedObject.h"

namespace DcmNs
{

ValInterfaceGlobalObjectManager::ValInterfaceGlobalObjectManager ()
    : WaveObjectManager (getClassName ())
{

}

ValInterfaceGlobalObjectManager::~ValInterfaceGlobalObjectManager ()
{

}

string ValInterfaceGlobalObjectManager::getClassName ()
{
    return ("ValInterfaceGlobalObjectManager");
}

ValInterfaceGlobalObjectManager *ValInterfaceGlobalObjectManager::getInstance ()
{
    static ValInterfaceGlobalObjectManager *pValInterfaceGlobalObjectManager = NULL;

    if (NULL == pValInterfaceGlobalObjectManager)
    {
        pValInterfaceGlobalObjectManager = new ValInterfaceGlobalObjectManager ();
        WaveNs::prismAssert (NULL != pValInterfaceGlobalObjectManager, __FILE__, __LINE__);
    }

    return (pValInterfaceGlobalObjectManager);
}

PrismServiceId ValInterfaceGlobalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void ValInterfaceGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string ("ValInterfaceGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));

    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
    {
        UI32 noOfFailedLocations = failedLocationIds.size ();


        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
            vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceLocalManagedObject::getClassName ());
            
            vector<WaveManagedObject *> *pWaveManagedObjectsForNodeSpecific = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceNodeSpecificLocalManagedObject::getClassName ());

            vector<WaveManagedObject *> *pWaveManagedObjectsForAssociation = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], DcmInterfaceAssociationLocalManagedObject::getClassName ());
            
            vector<WaveManagedObject *> *pWaveManagedObjectsForComposition = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceCompositionLocalManagedObject::getClassName ());
            
            vector<WaveManagedObject *> *pWaveManagedObjectsForContainer   = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceContainerInListLocalManagedObject::getClassName ());

            vector<WaveManagedObject *> *pWaveManagedObjectsForNDeepComposition = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceNDeepCompositionLocalManagedObject::getClassName ());

            trace (TRACE_LEVEL_INFO, string ("ValInterfaceGlobalObjectManager::failover for Location Id ") + failedLocationIds[i]);

            startTransaction ();

            UI32 sizeOfManagedObjects                   = pWaveManagedObjects->size ();
            UI32 sizeOfManagedObjectsForNodeSpecific    = pWaveManagedObjectsForNodeSpecific->size ();
            UI32 sizeOfManagedObjectsForAssociation     = pWaveManagedObjectsForAssociation->size ();
            UI32 sizeOfManagedObjectsForComposition     = pWaveManagedObjectsForComposition->size ();
            UI32 sizeOfManagedObjectsForContainer       = pWaveManagedObjectsForContainer->size ();
            UI32 sizeOfManagedObjectsForNDeepComposition = pWaveManagedObjectsForNDeepComposition->size ();
            
            for (UI32 j = 0; j < sizeOfManagedObjects; j++)
            {
                delete (*pWaveManagedObjects)[j];
            }   
            
            for (UI32 j = 0; j < sizeOfManagedObjectsForNodeSpecific; j++)
            {
                delete (*pWaveManagedObjectsForNodeSpecific)[j];
            }   

            for (UI32 j = 0; j < sizeOfManagedObjectsForAssociation; j++)
            {
                delete (*pWaveManagedObjectsForAssociation)[j];
            }

            for (UI32 j = 0; j < sizeOfManagedObjectsForComposition; j++)
            {
                delete (*pWaveManagedObjectsForComposition)[j];
            }

            for (UI32 j = 0; j < sizeOfManagedObjectsForContainer; j++)
            {
                delete (*pWaveManagedObjectsForContainer)[j];
            }

            for (UI32 j = 0; j < sizeOfManagedObjectsForNDeepComposition; j++)
            {
                delete (*pWaveManagedObjectsForNDeepComposition)[j];
            }

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "ValInterfaceGlobalObjectManager::failover can not commit local managed object delete to database ");
                prismAssert (false, __FILE__, __LINE__);
            }

            pWaveManagedObjects->clear ();
            pWaveManagedObjectsForNodeSpecific->clear ();
            pWaveManagedObjectsForAssociation->clear ();
            pWaveManagedObjectsForComposition->clear ();
            pWaveManagedObjectsForContainer->clear ();
            pWaveManagedObjectsForNDeepComposition->clear ();

            delete pWaveManagedObjects;
            delete pWaveManagedObjectsForNodeSpecific;
            delete pWaveManagedObjectsForAssociation;
            delete pWaveManagedObjectsForComposition;
            delete pWaveManagedObjectsForContainer;
            delete pWaveManagedObjectsForNDeepComposition;
        }
    }
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}

}
