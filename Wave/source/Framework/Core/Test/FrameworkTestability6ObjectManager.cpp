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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.         *
 *   All rights reserved.                                                 *
 *   Author : Vidyasagara Reddy Guntaka                                 *
 ***************************************************************************/

#include <time.h>
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject1.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject2.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject3.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject4.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject5.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject6.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject7.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject8.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/Test/Upgrade/UpgradePersistenceTestContext.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManagerGetManagedObjects.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectRelationalMapping/OrmToolKit.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Regression/RegressionTestObjectManager.h"
#include "Framework/Core/Test/UpgradeDatabaseSchemaParser.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManagerBackupMessage.h"
#include "Framework/Database/DatabaseObjectManagerCleanPreparedTransactionsMessage.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"


namespace WaveNs
{

FrameworkTestability6ObjectManager::FrameworkTestability6ObjectManager ()
        : PrismTestObjectManager (getPrismServiceName ())
{
    UpgradeTestManagedObject1 upgradeTestManagedObject1 (this);
    UpgradeTestManagedObject2 upgradeTestManagedObject2 (this);
    UpgradeTestManagedObject3 upgradeTestManagedObject3 (this);
    UpgradeTestManagedObject4 upgradeTestManagedObject4 (this);
    UpgradeTestManagedObject5 upgradeTestManagedObject5 (this);
    UpgradeTestManagedObject6  upgradeTestManagedObject6  (this);
    UpgradeTestManagedObject7  upgradeTestManagedObject7  (this);
    UpgradeTestManagedObject8  upgradeTestManagedObject8  (this);

    upgradeTestManagedObject1.setupOrm ();
    upgradeTestManagedObject2.setupOrm ();
    upgradeTestManagedObject3.setupOrm ();
    upgradeTestManagedObject4.setupOrm ();
    upgradeTestManagedObject5.setupOrm ();
    upgradeTestManagedObject6.setupOrm ();
    upgradeTestManagedObject7.setupOrm ();
    upgradeTestManagedObject8.setupOrm ();

    addManagedClass (UpgradeTestManagedObject1::getClassName ());
    addManagedClass (UpgradeTestManagedObject2::getClassName ());
    addManagedClass (UpgradeTestManagedObject3::getClassName ());
    addManagedClass (UpgradeTestManagedObject4::getClassName ());
    addManagedClass (UpgradeTestManagedObject5::getClassName ());
    addManagedClass (UpgradeTestManagedObject6::getClassName ());
    addManagedClass (UpgradeTestManagedObject7::getClassName ());
    addManagedClass (UpgradeTestManagedObject8::getClassName ());

    addOperationMap (FRAMEWORK_TESTABILITY_MESSAGE11,               reinterpret_cast<PrismMessageHandler> (&FrameworkTestability6ObjectManager::frameworkTestabilityMessage11RequestHandler));

}

FrameworkTestability6ObjectManager::~FrameworkTestability6ObjectManager ()
{
}

FrameworkTestability6ObjectManager *FrameworkTestability6ObjectManager::getInstance ()
{
    static FrameworkTestability6ObjectManager *pFrameworkTestability6ObjectManager = new FrameworkTestability6ObjectManager ();

    WaveNs::prismAssert (NULL != pFrameworkTestability6ObjectManager, __FILE__, __LINE__);

    return (pFrameworkTestability6ObjectManager);
}

PrismServiceId FrameworkTestability6ObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string FrameworkTestability6ObjectManager::getPrismServiceName ()
{
    return ("Framework Testability 6");
}

void FrameworkTestability6ObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability6ObjectManager::initialize : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}


PrismMessage *FrameworkTestability6ObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
    case FRAMEWORK_TESTABILITY_4_GET_MANAGED_OBJECTS :
        pPrismMessage = new FrameworkTestability6ObjectManagerGetManagedObjects;
        break;

    case FRAMEWORK_TESTABILITY_MESSAGE9 :
        pPrismMessage = new FrameworkTestabilityMessage9;
        break;
    default :
        pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *FrameworkTestability6ObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((UpgradeTestManagedObject1::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject1 (this);
    }
    else if ((UpgradeTestManagedObject2::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject2 (this);
    }
    else if ((UpgradeTestManagedObject3::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject3 (this);
    }
    else if ((UpgradeTestManagedObject4::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject4 (this);
    }
    else if ((UpgradeTestManagedObject5::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject5 (this);
    }
    else if ((UpgradeTestManagedObject6::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject6 (this);
    }
    else if ((UpgradeTestManagedObject7::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject7 (this);
    }
    else if ((UpgradeTestManagedObject8::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new UpgradeTestManagedObject8 (this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}
void FrameworkTestability6ObjectManager::testRequestHandler(RegressionTestMessage *pMessage)
{

        PrismLinearSequencerStep sequencerSteps[] =
        {
/*          
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByAttributeTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByAttributeRangeTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeDerivationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToOneCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),
            */

            /*
                Tests the HA DB sync APIs
                Commented for now. Will be uncommented after all APIs are done.
            
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep),
            */

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::validatePrepareStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleTransactionTestStep),
            
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::backupDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::instrumentTheDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::restoreDatabaseStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllObjectsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::prismLinearSequencerFailedStep)
        };

        UpgradePersistenceTestContext *pUpgradePersistenceTestContext = new UpgradePersistenceTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        pUpgradePersistenceTestContext->setNumberOfObjectsToBeCreated (10);

        pUpgradePersistenceTestContext->start ();
}

void FrameworkTestability6ObjectManager::frameworkTestabilityMessage11RequestHandler (FrameworkTestabilityMessage11 *pFrameworkTestabilityMessage11)
{
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByAttributeTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByAttributeRangeTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeDerivationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyAssociationTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToOneCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionUpdateTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::largeOneToManyCompositionTransactionTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep),

            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&FrameworkTestability6ObjectManager::prismLinearSequencerFailedStep)
        };

        UpgradePersistenceTestContext *pUpgradePersistenceTestContext = new UpgradePersistenceTestContext (pFrameworkTestabilityMessage11, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        pUpgradePersistenceTestContext->setNumberOfObjectsToBeCreated (pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ());

        pUpgradePersistenceTestContext->start ();

}

void FrameworkTestability6ObjectManager::simpleTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;

    pUpgradePersistenceTestContext->setNumberOfObjectsToBeCreated (numberOfObjectsToCreate);

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Wave's Test Cases");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        delete pUpgradeTestManagedObject1;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject1::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap             = pUpgradePersistenceTestContext->getCreatedObjectsMap ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleQueryCallback : Simple Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleQueryCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject1 *pUpgradeTestManagedObject1        = reinterpret_cast<UpgradeTestManagedObject1 *> ((*pResults)[i]);
                ObjectId                 oid                              = pUpgradeTestManagedObject1->getObjectId  ();
                UI32                     integer1                        = pUpgradeTestManagedObject1->getInteger1  ();
                string                 message1                      = pUpgradeTestManagedObject1->getMessage1  ();
                ObjectId                 objectId1                      = pUpgradeTestManagedObject1->getObjectId1 ();

                UpgradeTestManagedObject1 *pUpgradeTestManagedObject1Expected = allCreatedObjectsMap[oid];
                ObjectId                 oidExpected                      = pUpgradeTestManagedObject1Expected->getObjectId  ();
                UI32                     integer1Expected                = pUpgradeTestManagedObject1Expected->getInteger1  ();
                string                 message1Expected              = pUpgradeTestManagedObject1Expected->getMessage1  ();
                ObjectId                 objectId1Expected              = pUpgradeTestManagedObject1Expected->getObjectId1 ();

                //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());

                if ((*pUpgradeTestManagedObject1Expected) != (*pUpgradeTestManagedObject1))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Query By ObjectIds Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject1::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleQueryTestCallback), pUpgradePersistenceTestContext);

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject1 *> &allCreatedObjects       = pUpgradePersistenceTestContext->getCreatedObjects ();
    UI32                               numberOfObjectsCreated   = allCreatedObjects.size ();
    UI32                               numberOfObjectsToQueryFor = numberOfObjectsCreated > 100 ? 100 : numberOfObjectsCreated;
    UI32                               i                         = 0;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        pWaveManagedObjectQueryContext->addObjectId ((allCreatedObjects[i])->getObjectId ());
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (numberOfObjectsToQueryFor);;

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleQueryByAttributeTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Query By ATtributes Test ...");

    FrameworkTestabilityMessage11   *pFrameworkTestabilityMessage11   = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    UI32                            numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject1::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleQueryTestCallback), pUpgradePersistenceTestContext);

    if (0 < numberOfObjectsToCreate)
    {
        pWaveManagedObjectQueryContext->addAndAttribute (new AttributeUI32 (1, "integer1"));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::simpleQueryByAttributeTestStep : There must be at least one Object created for this test step to run.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (1);

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleQueryByAttributeRangeTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Query By Range");

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (UpgradeTestManagedObject1::getClassName ());

    UI32Range   range32("1,3-11,15,21-50");
    string     fieldNameToQuery  = "integer1";
    vector<UI32> rangeCount;

    range32.getUI32RangeVector (rangeCount);


    synchronousQueryContext.setAttributeRangeForQuery (new AttributeUI32Range (range32.toString (), fieldNameToQuery));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    UI32 numberOfResults = 0;

    if (NULL != pResults)
    {
        numberOfResults = pResults->size ();

        trace (TRACE_LEVEL_INFO, string ("SimpleQueryByRange: number of Object found : ") + numberOfResults);
    }

    for (UI32 i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;


    WaveManagedObjectSynchronousQueryContext synchronousQueryContext1 (UpgradeTestManagedObject1::getClassName ());
    synchronousQueryContext1.setAttributeRangeForQuery (new AttributeUI32Range (range32.toString (), fieldNameToQuery));

    UI32 count = 0;

    // Test 1 Count on correct range integer with Query Context

    ResourceId queryStatus = querySynchronouslyForCount (&synchronousQueryContext1, count);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        trace (TRACE_LEVEL_ERROR, "Query Failed Reason : " + FrameworkToolKit::localize (queryStatus));
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("SimpleQueryByRange: querySynchronouslyForCount = ") + count);

    // Test 2 Count on Managed Object

    UI32 countUpgradeTestManagedObject1 = 0;

    queryStatus = querySynchronouslyForCount (UpgradeTestManagedObject1::getClassName (), countUpgradeTestManagedObject1);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        trace (TRACE_LEVEL_ERROR, "Query Failed Reason : " + FrameworkToolKit::localize (queryStatus));
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    trace (TRACE_LEVEL_INFO, string ("SimpleQueryByRange: total Managed object created = ") + countUpgradeTestManagedObject1);

    // Test 3 Trying to get Count on non integer field in Managed Object for Range

    string fieldNameToQuery1 = "message1";
    UI32 countUpgradeTestManagedObject2 = 0;

    queryStatus = querySynchronouslyForCount (UpgradeTestManagedObject1::getClassName (), fieldNameToQuery1, range32.toString (), countUpgradeTestManagedObject2);

    if (WAVE_MESSAGE_ERROR_COUNT_QUERY_NOT_SUPPORTED_ON_NON_INTEGER_RANGE != queryStatus)
    {
        trace (TRACE_LEVEL_ERROR, "Query Failed Reason : " + FrameworkToolKit::localize (queryStatus));
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    if ((count != numberOfResults) || (count != rangeCount.size ()) || (countUpgradeTestManagedObject1 != pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ()))
    {
        trace (TRACE_LEVEL_INFO, string ("Count = ") + pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ());

        trace (TRACE_LEVEL_ERROR, "SimpleQueryByRange: Test Failed");

        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
    else
    {
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Synchronous Query Test ...");
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<WaveManagedObject *>          *pResults           = querySynchronously (UpgradeTestManagedObject1::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = (pUpgradePersistenceTestContext->getCreatedObjects ()).size ();
    UI32       numberOfResults       = pResults->size ();
    UI32       i                       = 0;
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleSynchronousQueryTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1        = dynamic_cast<UpgradeTestManagedObject1 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject1, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject1->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject1->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject1->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject1->getObjectId1 ();

            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1Expected = allCreatedObjectsMap[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject1Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject1Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject1Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject1Expected->getObjectId1 ();

            //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());

            if ((*pUpgradeTestManagedObject1Expected) != (*pUpgradeTestManagedObject1))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Synchronous Query By ObjectIds Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects      = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap     = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    UI32                                     numberOfObjectsCreated = allCreatedObjects.size ();
    UI32                                     numberOfObjectsToQueryFor = numberOfObjectsCreated > 100 ? 100 : numberOfObjectsCreated;
    UI32                                     i                       = 0;
    vector<ObjectId>                         objectIds;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        objectIds.push_back ((allCreatedObjects[i])->getObjectId ());
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (UpgradeTestManagedObject1::getClassName (), objectIds);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = numberOfObjectsToQueryFor;
    UI32       numberOfResults       = pResults->size ();
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdsTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1        = dynamic_cast<UpgradeTestManagedObject1 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject1, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject1->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject1->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject1->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject1->getObjectId1 ();

            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1Expected = allCreatedObjectsMap[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject1Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject1Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject1Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject1Expected->getObjectId1 ();

            //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());

            if ((*pUpgradeTestManagedObject1Expected) != (*pUpgradeTestManagedObject1))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleSynchronousQueryByObjectIdTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Synchronous Query By ObjectId (One At a Time) Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects   = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap  = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    UI32                                     numberOfObjectsCreated = allCreatedObjects.size ();
    UI32                                     i                    = 0;
    ResourceId                             status                = WAVE_MESSAGE_SUCCESS;

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1        = dynamic_cast<UpgradeTestManagedObject1 *> (queryManagedObject ((allCreatedObjects[i])->getObjectId ()));

        prismAssert (NULL != pUpgradeTestManagedObject1, __FILE__, __LINE__);

        ObjectId                 oid                              = pUpgradeTestManagedObject1->getObjectId  ();
        UI32                     integer1                        = pUpgradeTestManagedObject1->getInteger1  ();
        string                 message1                      = pUpgradeTestManagedObject1->getMessage1  ();
        ObjectId                 objectId1                      = pUpgradeTestManagedObject1->getObjectId1 ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1Expected = allCreatedObjectsMap[oid];

        prismAssert (NULL != pUpgradeTestManagedObject1Expected, __FILE__, __LINE__);

        ObjectId                 oidExpected                      = pUpgradeTestManagedObject1Expected->getObjectId  ();
        UI32                     integer1Expected                = pUpgradeTestManagedObject1Expected->getInteger1  ();
        string                 message1Expected              = pUpgradeTestManagedObject1Expected->getMessage1  ();
        ObjectId                 objectId1Expected              = pUpgradeTestManagedObject1Expected->getObjectId1 ();

        if ((*pUpgradeTestManagedObject1Expected) != (*pUpgradeTestManagedObject1))
        {
            trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId ());
            trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId ());
            prismAssert (false, __FILE__, __LINE__);

            status = WAVE_MESSAGE_ERROR;
            break;
        }

        delete pUpgradeTestManagedObject1;
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Update Transaction Test ...");

    ResourceId                       status                     = WAVE_MESSAGE_SUCCESS;
    UI32                               i                             = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    UI32                               numberOfObjectsCreated       = allCreatedObjects.size ();

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        startTransaction ();

        updateWaveManagedObject (allCreatedObjects[i]);

        allCreatedObjects[i]->setInteger1 (1000 + i);
        allCreatedObjects[i]->setMessage1 ("Waves's Simple Update's Test Cases");
        allCreatedObjects[i]->setObjectId1 (ObjectId (1000 + i, 1000 + i));

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large Update Transaction Test ...");

    ResourceId                       status                     = WAVE_MESSAGE_SUCCESS;
    UI32                               i                             = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    UI32                               numberOfObjectsCreated       = allCreatedObjects.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        updateWaveManagedObject (allCreatedObjects[i]);

        allCreatedObjects[i]->setInteger1 (2000 + i);
        allCreatedObjects[i]->setMessage1 ("Test Simple Update");
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllObjectsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Objects Transaction Test ...");

    ResourceId                                  status                  = WAVE_MESSAGE_SUCCESS;

    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion  = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject1::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion2 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject2::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion3 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject3::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion4 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject4::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion5 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject5::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion6 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject6::getClassName ());
    WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion7 = new WaveManagedObjectSynchronousQueryContextForDeletion (UpgradeTestManagedObject7::getClassName ());

    startTransaction ();

    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion7);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion6);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion5);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion4);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion3);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion2);
    deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion);

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllObjectsTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects Transaction Test ...");

    ResourceId                              status               = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                   = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects    = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap   = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    UI32                                      numberOfObjectsCreated = allCreatedObjects.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        delete allCreatedObjects[i];
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjectsTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects.clear ();
    allCreatedObjectsMap.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large Transaction Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap          = pUpgradePersistenceTestContext->getCreatedObjectsMap ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjectsMap[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToAssociationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject2 *pUpgradeTestManagedObject2 = new UpgradeTestManagedObject2 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject2->setInteger2 (i);
        pUpgradeTestManagedObject2->setMessage2 ("Test");
        pUpgradeTestManagedObject2->setObjectId2 (ObjectId (i, i));
        pUpgradeTestManagedObject2->setAssociation2 (pUpgradeTestManagedObject1->getObjectId ());

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToAssociationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        delete pUpgradeTestManagedObject1;
        delete pUpgradeTestManagedObject2;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject2::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects2 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject2 *> &allCreatedObjects2Map           = pUpgradePersistenceTestContext->getCreatedObjects2Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestCallback : Simple One-One Association Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject2 *pUpgradeTestManagedObject2        = dynamic_cast<UpgradeTestManagedObject2 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject2, __FILE__, __LINE__);
                ObjectId                 oid                              = pUpgradeTestManagedObject2->getObjectId  ();
                UI32                     integer2                        = pUpgradeTestManagedObject2->getInteger2  ();
                string                 message2                      = pUpgradeTestManagedObject2->getMessage2  ();
                ObjectId                 objectId2                      = pUpgradeTestManagedObject2->getObjectId2 ();

                UpgradeTestManagedObject2 *pUpgradeTestManagedObject2Expected = allCreatedObjects2Map[oid];
                ObjectId                 oidExpected                      = pUpgradeTestManagedObject2Expected->getObjectId  ();
                UI32                     integer2Expected                = pUpgradeTestManagedObject2Expected->getInteger2  ();
                string                 message2Expected              = pUpgradeTestManagedObject2Expected->getMessage2  ();
                ObjectId                 objectId2Expected              = pUpgradeTestManagedObject2Expected->getObjectId2 ();

                //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());

                if ((*pUpgradeTestManagedObject2Expected) != (*pUpgradeTestManagedObject2))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer2Expected + string (", ") + message2Expected + string (", ") + objectId2Expected.getClassId () + string (", ") + objectId2Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Query By ObjectIds Test ...");

    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject2::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToOneAssociationQueryTestCallback), pUpgradePersistenceTestContext);

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject2 *> &allCreatedObjects2     = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    UI32                               numberOfObjectsCreated2   = allCreatedObjects2.size ();
    UI32                               numberOfObjectsToQueryFor = numberOfObjectsCreated2 > 100 ? 100 : numberOfObjectsCreated2;
    UI32                               i                         = 0;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        pWaveManagedObjectQueryContext->addObjectId ((allCreatedObjects2[i])->getObjectId ());
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (numberOfObjectsToQueryFor);

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Synchronous Query Test ...");

    map<ObjectId, UpgradeTestManagedObject2 *> &allCreatedObjects2Map = pUpgradePersistenceTestContext->getCreatedObjects2Map ();
    vector<WaveManagedObject *>          *pResults            = querySynchronously (UpgradeTestManagedObject2::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = (pUpgradePersistenceTestContext->getCreatedObjects2 ()).size ();
    UI32       numberOfResults       = pResults->size ();
    UI32       i                        = 0;
    ResourceId status                  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject2 *pUpgradeTestManagedObject2        = dynamic_cast<UpgradeTestManagedObject2 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject2, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject2->getObjectId  ();
            UI32                     integer2                        = pUpgradeTestManagedObject2->getInteger2  ();
            string                 message2                      = pUpgradeTestManagedObject2->getMessage2  ();
            ObjectId                 objectId2                      = pUpgradeTestManagedObject2->getObjectId2 ();

            UpgradeTestManagedObject2 *pUpgradeTestManagedObject2Expected = allCreatedObjects2Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject2Expected->getObjectId  ();
            UI32                     integer2Expected                = pUpgradeTestManagedObject2Expected->getInteger2  ();
            string                 message2Expected              = pUpgradeTestManagedObject2Expected->getMessage2  ();
            ObjectId                 objectId2Expected              = pUpgradeTestManagedObject2Expected->getObjectId2 ();

            //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());

            if ((*pUpgradeTestManagedObject2Expected) != (*pUpgradeTestManagedObject2))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer2Expected + string (", ") + message2Expected + string (", ") + objectId2Expected.getClassId () + string (", ") + objectId2Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Synchronous Query By ObjectIds Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject2 *>     &allCreatedObjects2    = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    map<ObjectId, UpgradeTestManagedObject2 *> &allCreatedObjects2Map   = pUpgradePersistenceTestContext->getCreatedObjects2Map ();
    UI32                                     numberOfObjectsCreated2   = allCreatedObjects2.size ();
    UI32                                     numberOfObjectsToQueryFor = numberOfObjectsCreated2 > 100 ? 100 : numberOfObjectsCreated2;
    UI32                                     i                       = 0;
    vector<ObjectId>                         objectIds;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        objectIds.push_back ((allCreatedObjects2[i])->getObjectId ());
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (UpgradeTestManagedObject2::getClassName (), objectIds);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = numberOfObjectsToQueryFor;
    UI32       numberOfResults       = pResults->size ();
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject2 *pUpgradeTestManagedObject2        = dynamic_cast<UpgradeTestManagedObject2 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject2, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject2->getObjectId  ();
            UI32                     integer2                        = pUpgradeTestManagedObject2->getInteger2  ();
            string                 message2                      = pUpgradeTestManagedObject2->getMessage2  ();
            ObjectId                 objectId2                      = pUpgradeTestManagedObject2->getObjectId2 ();

            UpgradeTestManagedObject2 *pUpgradeTestManagedObject2Expected = allCreatedObjects2Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject2Expected->getObjectId  ();
            UI32                     integer2Expected                = pUpgradeTestManagedObject2Expected->getInteger2  ();
            string                 message2Expected              = pUpgradeTestManagedObject2Expected->getMessage2  ();
            ObjectId                 objectId2Expected              = pUpgradeTestManagedObject2Expected->getObjectId2 ();

            //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());

            if ((*pUpgradeTestManagedObject2Expected) != (*pUpgradeTestManagedObject2))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer2Expected + string (", ") + message2Expected + string (", ") + objectId2Expected.getClassId () + string (", ") + objectId2Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects2 Transaction Test ...");

    ResourceId                              status                = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                    = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap    = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject2 *>     &allCreatedObjects2   = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    map<ObjectId, UpgradeTestManagedObject2 *> &allCreatedObjects2Map   = pUpgradePersistenceTestContext->getCreatedObjects2Map ();
    UI32                                      numberOfObjectsCreated  = allCreatedObjects.size ();
    UI32                                      numberOfObjectsCreated2 = allCreatedObjects2.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated2; i++)
    {
        delete allCreatedObjects2[i];
    }

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        delete allCreatedObjects[i];
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects2TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects2.clear ();
    allCreatedObjects2Map.clear ();
    allCreatedObjects.clear ();
    allCreatedObjectsMap.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToAssociationUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-One Association Update Test ...");

    ResourceId                       status              = WAVE_MESSAGE_SUCCESS;
    UI32                               i                      = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects    = pUpgradePersistenceTestContext->getCreatedObjects ();
    vector<UpgradeTestManagedObject2 *> &allCreatedObjects2  = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    UI32                               numberOfObjectsCreated = allCreatedObjects2.size ();

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = allCreatedObjects[i];
        UpgradeTestManagedObject2 *pUpgradeTestManagedObject2 = allCreatedObjects2[i];

        updateWaveManagedObject (pUpgradeTestManagedObject1);
        updateWaveManagedObject (pUpgradeTestManagedObject2);

        pUpgradeTestManagedObject1->setInteger1 (i + 21);
        pUpgradeTestManagedObject1->setMessage1 ("Test Update 21");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i + 100, i + 100));

        pUpgradeTestManagedObject2->setInteger2 (i + 22);
        pUpgradeTestManagedObject2->setMessage2 ("Test Update 22");
        pUpgradeTestManagedObject2->setObjectId2 (ObjectId (i + 1000, i + 100));
        pUpgradeTestManagedObject2->setAssociation2 (pUpgradeTestManagedObject1->getObjectId ());

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToAssociationUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToAssociationUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-One Association Update Test ...");

    ResourceId                       status              = WAVE_MESSAGE_SUCCESS;
    UI32                               i                      = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects    = pUpgradePersistenceTestContext->getCreatedObjects ();
    vector<UpgradeTestManagedObject2 *> &allCreatedObjects2  = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    UI32                               numberOfObjectsCreated = allCreatedObjects2.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = allCreatedObjects[i];
        UpgradeTestManagedObject2 *pUpgradeTestManagedObject2 = allCreatedObjects2[i];

        updateWaveManagedObject (pUpgradeTestManagedObject1);
        updateWaveManagedObject (pUpgradeTestManagedObject2);

        pUpgradeTestManagedObject1->setInteger1 (i + 210);
        pUpgradeTestManagedObject1->setMessage1 ("Test Update 210");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i + 210, i + 210));

        pUpgradeTestManagedObject2->setInteger2 (i + 220);
        pUpgradeTestManagedObject2->setMessage2 ("Test Update 220");
        pUpgradeTestManagedObject2->setObjectId2 (ObjectId (i + 1220, i + 1220));
        pUpgradeTestManagedObject2->setAssociation2 (pUpgradeTestManagedObject1->getObjectId ());
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeOneToAssociationUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToAssociationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-One Association Transaction Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap          = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject2 *>     &allCreatedObjects2         = pUpgradePersistenceTestContext->getCreatedObjects2 ();
    map<ObjectId, UpgradeTestManagedObject2 *> &allCreatedObjects2Map        = pUpgradePersistenceTestContext->getCreatedObjects2Map ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject2 *pUpgradeTestManagedObject2 = new UpgradeTestManagedObject2 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject2->setInteger2 (i);
        pUpgradeTestManagedObject2->setMessage2 ("Test");
        pUpgradeTestManagedObject2->setObjectId2 (ObjectId (i, i));
        pUpgradeTestManagedObject2->setAssociation2 (pUpgradeTestManagedObject1->getObjectId ());

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjectsMap[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        allCreatedObjects2.push_back (pUpgradeTestManagedObject2);
        allCreatedObjects2Map[pUpgradeTestManagedObject2->getObjectId ()] = pUpgradeTestManagedObject2;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeOneToAssociationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleDerivationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Derivation Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject3 *pUpgradeTestManagedObject3 = new UpgradeTestManagedObject3 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject3->setInteger1 (i);
        pUpgradeTestManagedObject3->setMessage1 ("Test 31");
        pUpgradeTestManagedObject3->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject3->setInteger3 (i);
        pUpgradeTestManagedObject3->setMessage3 ("Test 33");
        pUpgradeTestManagedObject3->setObjectId3 (ObjectId (i, i));
        pUpgradeTestManagedObject3->setAssociation3 (pUpgradeTestManagedObject1->getObjectId ());

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleDerivationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        delete pUpgradeTestManagedObject1;
        delete pUpgradeTestManagedObject3;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleDerivationQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Derivation Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject3::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects3 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleDerivationQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject3 *> &allCreatedObjects3Map           = pUpgradePersistenceTestContext->getCreatedObjects3Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleDerivationQueryTestCallback : Simple Derivation Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleDerivationQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject3 *pUpgradeTestManagedObject3        = dynamic_cast<UpgradeTestManagedObject3 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject3, __FILE__, __LINE__);
                ObjectId                 oid                              = pUpgradeTestManagedObject3->getObjectId  ();
                UI32                     integer1                        = pUpgradeTestManagedObject3->getInteger1  ();
                string                 message1                      = pUpgradeTestManagedObject3->getMessage1  ();
                ObjectId                 objectId1                      = pUpgradeTestManagedObject3->getObjectId1 ();
                UI32                     integer3                        = pUpgradeTestManagedObject3->getInteger3  ();
                string                 message3                      = pUpgradeTestManagedObject3->getMessage3  ();
                ObjectId                 objectId3                      = pUpgradeTestManagedObject3->getObjectId3 ();
                ObjectId                 association3                    = pUpgradeTestManagedObject3->getAssociation3 ();

                UpgradeTestManagedObject3 *pUpgradeTestManagedObject3Expected = allCreatedObjects3Map[oid];
                ObjectId                 oidExpected                      = pUpgradeTestManagedObject3Expected->getObjectId  ();
                UI32                     integer1Expected                = pUpgradeTestManagedObject3Expected->getInteger1  ();
                string                 message1Expected              = pUpgradeTestManagedObject3Expected->getMessage1  ();
                ObjectId                 objectId1Expected              = pUpgradeTestManagedObject3Expected->getObjectId1 ();
                UI32                     integer3Expected                = pUpgradeTestManagedObject3Expected->getInteger3  ();
                string                 message3Expected              = pUpgradeTestManagedObject3Expected->getMessage3  ();
                ObjectId                 objectId3Expected              = pUpgradeTestManagedObject3Expected->getObjectId3 ();
                ObjectId                 association3Expected            = pUpgradeTestManagedObject3Expected->getAssociation3 ();

                if ((*pUpgradeTestManagedObject3Expected) != (*pUpgradeTestManagedObject3))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer3 + string (", ") + message3 + string (", ") + objectId3.getClassId () + string (", ") + objectId3.getInstanceId () + string (", ") + association3.getClassId () + string (", ") + association3.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer3Expected + string (", ") + message3Expected + string (", ") + objectId3Expected.getClassId () + string (", ") + objectId3Expected.getInstanceId () + string (", ") + association3Expected.getClassId () + string (", ") + association3Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleDerivationQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Derivation Query By ObjectIds Test ...");

    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject3::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleDerivationQueryTestCallback), pUpgradePersistenceTestContext);

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject3 *> &allCreatedObjects3     = pUpgradePersistenceTestContext->getCreatedObjects3 ();
    UI32                               numberOfObjectsCreated3   = allCreatedObjects3.size ();
    UI32                               numberOfObjectsToQueryFor = numberOfObjectsCreated3 > 100 ? 100 : numberOfObjectsCreated3;
    UI32                               i                         = 0;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        pWaveManagedObjectQueryContext->addObjectId ((allCreatedObjects3[i])->getObjectId ());
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (numberOfObjectsToQueryFor);

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Derivation Synchronous Query Test ...");

    map<ObjectId, UpgradeTestManagedObject3 *> &allCreatedObjects3Map = pUpgradePersistenceTestContext->getCreatedObjects3Map ();
    vector<WaveManagedObject *>          *pResults            = querySynchronously (UpgradeTestManagedObject3::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = (pUpgradePersistenceTestContext->getCreatedObjects3 ()).size ();
    UI32       numberOfResults       = pResults->size ();
    UI32       i                        = 0;
    ResourceId status                  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject3 *pUpgradeTestManagedObject3        = dynamic_cast<UpgradeTestManagedObject3 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject3, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject3->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject3->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject3->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject3->getObjectId1 ();
            UI32                     integer3                        = pUpgradeTestManagedObject3->getInteger3  ();
            string                 message3                      = pUpgradeTestManagedObject3->getMessage3  ();
            ObjectId                 objectId3                      = pUpgradeTestManagedObject3->getObjectId3 ();
            ObjectId                 association3                    = pUpgradeTestManagedObject3->getAssociation3 ();

            UpgradeTestManagedObject3 *pUpgradeTestManagedObject3Expected = allCreatedObjects3Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject3Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject3Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject3Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject3Expected->getObjectId1 ();
            UI32                     integer3Expected                = pUpgradeTestManagedObject3Expected->getInteger3  ();
            string                 message3Expected              = pUpgradeTestManagedObject3Expected->getMessage3  ();
            ObjectId                 objectId3Expected              = pUpgradeTestManagedObject3Expected->getObjectId3 ();
            ObjectId                 association3Expected            = pUpgradeTestManagedObject3Expected->getAssociation3 ();

            if ((*pUpgradeTestManagedObject3Expected) != (*pUpgradeTestManagedObject3))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer3 + string (", ") + message3 + string (", ") + objectId3.getClassId () + string (", ") + objectId3.getInstanceId () + string (", ") + association3.getClassId () + string (", ") + association3.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer3Expected + string (", ") + message3Expected + string (", ") + objectId3Expected.getClassId () + string (", ") + objectId3Expected.getInstanceId () + string (", ") + association3Expected.getClassId () + string (", ") + association3Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple Derivation Synchronous Query By ObjectIds Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject3 *>     &allCreatedObjects3    = pUpgradePersistenceTestContext->getCreatedObjects3 ();
    map<ObjectId, UpgradeTestManagedObject3 *> &allCreatedObjects3Map   = pUpgradePersistenceTestContext->getCreatedObjects3Map ();
    UI32                                     numberOfObjectsCreated3   = allCreatedObjects3.size ();
    UI32                                     numberOfObjectsToQueryFor = numberOfObjectsCreated3 > 100 ? 100 : numberOfObjectsCreated3;
    UI32                                     i                       = 0;
    vector<ObjectId>                         objectIds;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        objectIds.push_back ((allCreatedObjects3[i])->getObjectId ());
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (UpgradeTestManagedObject3::getClassName (), objectIds);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = numberOfObjectsToQueryFor;
    UI32       numberOfResults       = pResults->size ();
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleDerivationSynchronousQueryByObjectIdsTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject3 *pUpgradeTestManagedObject3        = dynamic_cast<UpgradeTestManagedObject3 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject3, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject3->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject3->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject3->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject3->getObjectId1 ();
            UI32                     integer3                        = pUpgradeTestManagedObject3->getInteger3  ();
            string                 message3                      = pUpgradeTestManagedObject3->getMessage3  ();
            ObjectId                 objectId3                      = pUpgradeTestManagedObject3->getObjectId3 ();
            ObjectId                 association3                    = pUpgradeTestManagedObject3->getAssociation3 ();

            UpgradeTestManagedObject3 *pUpgradeTestManagedObject3Expected = allCreatedObjects3Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject3Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject3Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject3Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject3Expected->getObjectId1 ();
            UI32                     integer3Expected                = pUpgradeTestManagedObject3Expected->getInteger3  ();
            string                 message3Expected              = pUpgradeTestManagedObject3Expected->getMessage3  ();
            ObjectId                 objectId3Expected              = pUpgradeTestManagedObject3Expected->getObjectId3 ();
            ObjectId                 association3Expected            = pUpgradeTestManagedObject3Expected->getAssociation3 ();

            if ((*pUpgradeTestManagedObject3Expected) != (*pUpgradeTestManagedObject3))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer3 + string (", ") + message3 + string (", ") + objectId3.getClassId () + string (", ") + objectId3.getInstanceId () + string (", ") + association3.getClassId () + string (", ") + association3.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer3Expected + string (", ") + message3Expected + string (", ") + objectId3Expected.getClassId () + string (", ") + objectId3Expected.getInstanceId () + string (", ") + association3Expected.getClassId () + string (", ") + association3Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects3 Transaction Test ...");

    ResourceId                              status                = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                    = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map   = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject3 *>     &allCreatedObjects3   = pUpgradePersistenceTestContext->getCreatedObjects3 ();
    map<ObjectId, UpgradeTestManagedObject3 *> &allCreatedObjects3Map   = pUpgradePersistenceTestContext->getCreatedObjects3Map ();
    UI32                                      numberOfObjectsCreated  = allCreatedObjects.size ();
    UI32                                      numberOfObjectsCreated3 = allCreatedObjects3.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated3; i++)
    {
        delete allCreatedObjects3[i];
    }

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        delete allCreatedObjects[i];
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects3TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects3.clear ();
    allCreatedObjects3Map.clear ();
    allCreatedObjects.clear ();
    allCreatedObjects1Map.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeDerivationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large Derivation Transaction Test ...");

    FrameworkTestabilityMessage11     *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                       status                     = WAVE_MESSAGE_SUCCESS;
    UI32                               numberOfObjectsToCreate     = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                               i                             = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects    = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map  = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject3 *>     &allCreatedObjects3  = pUpgradePersistenceTestContext->getCreatedObjects3 ();
    map<ObjectId, UpgradeTestManagedObject3 *> &allCreatedObjects3Map  = pUpgradePersistenceTestContext->getCreatedObjects3Map ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject3 *pUpgradeTestManagedObject3 = new UpgradeTestManagedObject3 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject3->setInteger1 (i);
        pUpgradeTestManagedObject3->setMessage1 ("Test 31");
        pUpgradeTestManagedObject3->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject3->setInteger3 (i);
        pUpgradeTestManagedObject3->setMessage3 ("Test 33");
        pUpgradeTestManagedObject3->setObjectId3 (ObjectId (i, i));
        pUpgradeTestManagedObject3->setAssociation3 (pUpgradeTestManagedObject1->getObjectId ());

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        allCreatedObjects3.push_back (pUpgradeTestManagedObject3);
        allCreatedObjects3Map[pUpgradeTestManagedObject3->getObjectId ()] = pUpgradeTestManagedObject3;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeDerivationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Association Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<ObjectId>                          tempVector;

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject4 *pUpgradeTestManagedObject4 = new UpgradeTestManagedObject4 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject4->setInteger1 (i);
        pUpgradeTestManagedObject4->setMessage1 ("Test 41");
        pUpgradeTestManagedObject4->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject4->setInteger4 (i);
        pUpgradeTestManagedObject4->setMessage4 ("Test 44");
        pUpgradeTestManagedObject4->setObjectId4 (ObjectId (i, i));
        pUpgradeTestManagedObject4->setAssociation4 (pUpgradeTestManagedObject1->getObjectId ());

        tempVector.clear ();

        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));

        pUpgradeTestManagedObject4->setObjectIdVector4 (tempVector);

        tempVector.clear ();

        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());

        pUpgradeTestManagedObject4->setAssociationVector4 (tempVector);

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyAssociationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        delete pUpgradeTestManagedObject1;
        delete pUpgradeTestManagedObject4;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-Many Association Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject4::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects4 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject4 *> &allCreatedObjects4Map           = pUpgradePersistenceTestContext->getCreatedObjects4Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestCallback : Simple One-To-Many Association Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject4 *pUpgradeTestManagedObject4        = dynamic_cast<UpgradeTestManagedObject4 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject4, __FILE__, __LINE__);
                ObjectId                 oid                              = pUpgradeTestManagedObject4->getObjectId  ();
                UI32                     integer1                        = pUpgradeTestManagedObject4->getInteger1  ();
                string                 message1                      = pUpgradeTestManagedObject4->getMessage1  ();
                ObjectId                 objectId1                      = pUpgradeTestManagedObject4->getObjectId1 ();
                UI32                     integer4                        = pUpgradeTestManagedObject4->getInteger4  ();
                string                 message4                      = pUpgradeTestManagedObject4->getMessage4  ();
                ObjectId                 objectId4                      = pUpgradeTestManagedObject4->getObjectId4 ();
                ObjectId                 association4                    = pUpgradeTestManagedObject4->getAssociation4 ();

                UpgradeTestManagedObject4 *pUpgradeTestManagedObject4Expected = allCreatedObjects4Map[oid];
                ObjectId                 oidExpected                      = pUpgradeTestManagedObject4Expected->getObjectId  ();
                UI32                     integer1Expected                = pUpgradeTestManagedObject4Expected->getInteger1  ();
                string                 message1Expected              = pUpgradeTestManagedObject4Expected->getMessage1  ();
                ObjectId                 objectId1Expected              = pUpgradeTestManagedObject4Expected->getObjectId1 ();
                UI32                     integer4Expected                = pUpgradeTestManagedObject4Expected->getInteger4  ();
                string                 message4Expected              = pUpgradeTestManagedObject4Expected->getMessage4  ();
                ObjectId                 objectId4Expected              = pUpgradeTestManagedObject4Expected->getObjectId4 ();
                ObjectId                 association4Expected            = pUpgradeTestManagedObject4Expected->getAssociation4 ();

                if ((*pUpgradeTestManagedObject4Expected) != (*pUpgradeTestManagedObject4))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-Many Association Query By ObjectIds Test ...");

    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject4::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToManyAssociationQueryTestCallback), pUpgradePersistenceTestContext);

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject4 *> &allCreatedObjects4     = pUpgradePersistenceTestContext->getCreatedObjects4 ();
    UI32                               numberOfObjectsCreated4   = allCreatedObjects4.size ();
    UI32                               numberOfObjectsToQueryFor = numberOfObjectsCreated4 > 100 ? 100 : numberOfObjectsCreated4;
    UI32                               i                         = 0;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        pWaveManagedObjectQueryContext->addObjectId ((allCreatedObjects4[i])->getObjectId ());
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (numberOfObjectsToQueryFor);

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-Many Association Synchronous Query Test ...");

    map<ObjectId, UpgradeTestManagedObject4 *> &allCreatedObjects4Map = pUpgradePersistenceTestContext->getCreatedObjects4Map ();
    vector<WaveManagedObject *>          *pResults            = querySynchronously (UpgradeTestManagedObject4::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = (pUpgradePersistenceTestContext->getCreatedObjects4 ()).size ();
    UI32       numberOfResults       = pResults->size ();
    UI32       i                        = 0;
    ResourceId status                  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject4 *pUpgradeTestManagedObject4        = dynamic_cast<UpgradeTestManagedObject4 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject4, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject4->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject4->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject4->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject4->getObjectId1 ();
            UI32                     integer4                        = pUpgradeTestManagedObject4->getInteger4  ();
            string                 message4                      = pUpgradeTestManagedObject4->getMessage4  ();
            ObjectId                 objectId4                      = pUpgradeTestManagedObject4->getObjectId4 ();
            ObjectId                 association4                    = pUpgradeTestManagedObject4->getAssociation4 ();

            UpgradeTestManagedObject4 *pUpgradeTestManagedObject4Expected = allCreatedObjects4Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject4Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject4Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject4Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject4Expected->getObjectId1 ();
            UI32                     integer4Expected                = pUpgradeTestManagedObject4Expected->getInteger4  ();
            string                 message4Expected              = pUpgradeTestManagedObject4Expected->getMessage4  ();
            ObjectId                 objectId4Expected              = pUpgradeTestManagedObject4Expected->getObjectId4 ();
            ObjectId                 association4Expected            = pUpgradeTestManagedObject4Expected->getAssociation4 ();

            if ((*pUpgradeTestManagedObject4Expected) != (*pUpgradeTestManagedObject4))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-Many Association Synchronous Query By ObjectIds Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject4 *>     &allCreatedObjects4    = pUpgradePersistenceTestContext->getCreatedObjects4 ();
    map<ObjectId, UpgradeTestManagedObject4 *> &allCreatedObjects4Map   = pUpgradePersistenceTestContext->getCreatedObjects4Map ();
    UI32                                     numberOfObjectsCreated4   = allCreatedObjects4.size ();
    UI32                                     numberOfObjectsToQueryFor = numberOfObjectsCreated4 > 100 ? 100 : numberOfObjectsCreated4;
    UI32                                     i                       = 0;
    vector<ObjectId>                         objectIds;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        objectIds.push_back ((allCreatedObjects4[i])->getObjectId ());
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (UpgradeTestManagedObject4::getClassName (), objectIds);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = numberOfObjectsToQueryFor;
    UI32       numberOfResults       = pResults->size ();
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyAssociationSynchronousQueryByObjectIdsTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject4 *pUpgradeTestManagedObject4        = dynamic_cast<UpgradeTestManagedObject4 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject4, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject4->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject4->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject4->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject4->getObjectId1 ();
            UI32                     integer4                        = pUpgradeTestManagedObject4->getInteger4  ();
            string                 message4                      = pUpgradeTestManagedObject4->getMessage4  ();
            ObjectId                 objectId4                      = pUpgradeTestManagedObject4->getObjectId4 ();
            ObjectId                 association4                    = pUpgradeTestManagedObject4->getAssociation4 ();

            UpgradeTestManagedObject4 *pUpgradeTestManagedObject4Expected = allCreatedObjects4Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject4Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject4Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject4Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject4Expected->getObjectId1 ();
            UI32                     integer4Expected                = pUpgradeTestManagedObject4Expected->getInteger4  ();
            string                 message4Expected              = pUpgradeTestManagedObject4Expected->getMessage4  ();
            ObjectId                 objectId4Expected              = pUpgradeTestManagedObject4Expected->getObjectId4 ();
            ObjectId                 association4Expected            = pUpgradeTestManagedObject4Expected->getAssociation4 ();

            if ((*pUpgradeTestManagedObject4Expected) != (*pUpgradeTestManagedObject4))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyAssociationUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Association Update Test ...");

    ResourceId                       status               = WAVE_MESSAGE_SUCCESS;
    UI32                               i                       = 0;
    vector<UpgradeTestManagedObject1 *>  &allCreatedObjects   = pUpgradePersistenceTestContext->getCreatedObjects ();
    vector<UpgradeTestManagedObject4 *>  &allCreatedObjects4     = pUpgradePersistenceTestContext->getCreatedObjects4 ();
    UI32                               numberOfObjectsCreated4 = allCreatedObjects4.size ();
    vector<ObjectId>                   tempVector;

    for (i = 0; i < numberOfObjectsCreated4; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = allCreatedObjects[i];
        UpgradeTestManagedObject4 *pUpgradeTestManagedObject4 = allCreatedObjects4[i];

        prismAssert (NULL != pUpgradeTestManagedObject4, __FILE__, __LINE__);

        updateWaveManagedObject (pUpgradeTestManagedObject4);

        pUpgradeTestManagedObject4->setInteger1 (i);
        pUpgradeTestManagedObject4->setMessage1 ("Test 41 Update");
        pUpgradeTestManagedObject4->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject4->setInteger4 (i);
        pUpgradeTestManagedObject4->setMessage4 ("Test 44 Update");
        pUpgradeTestManagedObject4->setObjectId4 (ObjectId (i + 5000, i + 5000));
        pUpgradeTestManagedObject4->setAssociation4 (pUpgradeTestManagedObject1->getObjectId ());

        tempVector.clear ();

        tempVector.push_back (ObjectId (i + 100, i + 100));
        tempVector.push_back (ObjectId (i + 100, i + 100));
        tempVector.push_back (ObjectId (i + 100, i + 100));

        pUpgradeTestManagedObject4->setObjectIdVector4 (tempVector);

        tempVector.clear ();

        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());

        pUpgradeTestManagedObject4->setAssociationVector4 (tempVector);

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyAssociationUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects4 Transaction Test ...");

    ResourceId                       status               = WAVE_MESSAGE_SUCCESS;
    UI32                               i                       = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    vector<UpgradeTestManagedObject4 *> &allCreatedObjects4   = pUpgradePersistenceTestContext->getCreatedObjects4 ();
    UI32                               numberOfObjectsCreated  = allCreatedObjects.size ();
    UI32                               numberOfObjectsCreated4 = allCreatedObjects4.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated4; i++)
    {
        delete allCreatedObjects4[i];
    }

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        delete allCreatedObjects[i];
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects4TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects4.clear ();
    allCreatedObjects.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToManyAssociationTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-To-Many Association Transaction Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject4 *>     &allCreatedObjects4         = pUpgradePersistenceTestContext->getCreatedObjects4 ();
    map<ObjectId, UpgradeTestManagedObject4 *> &allCreatedObjects4Map        = pUpgradePersistenceTestContext->getCreatedObjects4Map ();
    vector<ObjectId>                          tempVector;

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject4 *pUpgradeTestManagedObject4 = new UpgradeTestManagedObject4 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject4->setInteger1 (i);
        pUpgradeTestManagedObject4->setMessage1 ("Test 41");
        pUpgradeTestManagedObject4->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject4->setInteger4 (i);
        pUpgradeTestManagedObject4->setMessage4 ("Test 44");
        pUpgradeTestManagedObject4->setObjectId4 (ObjectId (i, i));
        pUpgradeTestManagedObject4->setAssociation4 (pUpgradeTestManagedObject1->getObjectId ());

        tempVector.clear ();

        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));

        pUpgradeTestManagedObject4->setObjectIdVector4 (tempVector);

        tempVector.clear ();

        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());

        pUpgradeTestManagedObject4->setAssociationVector4 (tempVector);

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        allCreatedObjects4.push_back (pUpgradeTestManagedObject4);
        allCreatedObjects4Map[pUpgradeTestManagedObject4->getObjectId ()] = pUpgradeTestManagedObject4;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeOneToManyAssociationTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Muiltiple Associations In Hierarchy Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<ObjectId>                          tempVector;

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject5 *pUpgradeTestManagedObject5 = new UpgradeTestManagedObject5 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject5->setInteger1 (i);
        pUpgradeTestManagedObject5->setMessage1 ("Test 41");
        pUpgradeTestManagedObject5->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject5->setInteger4 (i);
        pUpgradeTestManagedObject5->setMessage4 ("Test 44");
        pUpgradeTestManagedObject5->setObjectId4 (ObjectId (i, i));
        pUpgradeTestManagedObject5->setAssociation4 (pUpgradeTestManagedObject1->getObjectId ());

        tempVector.clear ();

        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));

        pUpgradeTestManagedObject5->setObjectIdVector4 (tempVector);

        tempVector.clear ();

        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());

        pUpgradeTestManagedObject5->setAssociationVector4 (tempVector);

        pUpgradeTestManagedObject5->setAssociationVector5 (tempVector);

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        delete pUpgradeTestManagedObject1;
        delete pUpgradeTestManagedObject5;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Muiltiple Associations In Hierarchy Query Test ...");

    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject5::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects5 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject5 *> &allCreatedObjects5Map           = pUpgradePersistenceTestContext->getCreatedObjects5Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback : Simple One-To-Many Muiltiple Associations In Hierarchy Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject5 *pUpgradeTestManagedObject5        = dynamic_cast<UpgradeTestManagedObject5 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject5, __FILE__, __LINE__);
                ObjectId                 oid                              = pUpgradeTestManagedObject5->getObjectId  ();
                UI32                     integer1                        = pUpgradeTestManagedObject5->getInteger1  ();
                string                 message1                      = pUpgradeTestManagedObject5->getMessage1  ();
                ObjectId                 objectId1                      = pUpgradeTestManagedObject5->getObjectId1 ();
                UI32                     integer4                        = pUpgradeTestManagedObject5->getInteger4  ();
                string                 message4                      = pUpgradeTestManagedObject5->getMessage4  ();
                ObjectId                 objectId4                      = pUpgradeTestManagedObject5->getObjectId4 ();
                ObjectId                 association4                    = pUpgradeTestManagedObject5->getAssociation4 ();

                UpgradeTestManagedObject5 *pUpgradeTestManagedObject5Expected = allCreatedObjects5Map[oid];
                ObjectId                 oidExpected                      = pUpgradeTestManagedObject5Expected->getObjectId  ();
                UI32                     integer1Expected                = pUpgradeTestManagedObject5Expected->getInteger1  ();
                string                 message1Expected              = pUpgradeTestManagedObject5Expected->getMessage1  ();
                ObjectId                 objectId1Expected              = pUpgradeTestManagedObject5Expected->getObjectId1 ();
                UI32                     integer4Expected                = pUpgradeTestManagedObject5Expected->getInteger4  ();
                string                 message4Expected              = pUpgradeTestManagedObject5Expected->getMessage4  ();
                ObjectId                 objectId4Expected              = pUpgradeTestManagedObject5Expected->getObjectId4 ();
                ObjectId                 association4Expected            = pUpgradeTestManagedObject5Expected->getAssociation4 ();

                if ((*pUpgradeTestManagedObject5Expected) != (*pUpgradeTestManagedObject5))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Muiltiple Associations In Hierarchy Query By ObjectIds Test ...");

    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject5::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback), pUpgradePersistenceTestContext);

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject5 *> &allCreatedObjects5     = pUpgradePersistenceTestContext->getCreatedObjects5 ();
    UI32                               numberOfObjectsCreated5   = allCreatedObjects5.size ();
    UI32                               numberOfObjectsToQueryFor = numberOfObjectsCreated5 > 100 ? 100 : numberOfObjectsCreated5;
    UI32                               i                         = 0;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        pWaveManagedObjectQueryContext->addObjectId ((allCreatedObjects5[i])->getObjectId ());
    }

    pUpgradePersistenceTestContext->setNumberOfExpectedResults (numberOfObjectsToQueryFor);

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Muiltiple Associations In Hierarchy Synchronous Query Test ...");

    map<ObjectId, UpgradeTestManagedObject5 *> &allCreatedObjects5Map = pUpgradePersistenceTestContext->getCreatedObjects5Map ();
    vector<WaveManagedObject *>          *pResults            = querySynchronously (UpgradeTestManagedObject5::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = (pUpgradePersistenceTestContext->getCreatedObjects5 ()).size ();
    UI32       numberOfResults       = pResults->size ();
    UI32       i                        = 0;
    ResourceId status                  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject5 *pUpgradeTestManagedObject5        = dynamic_cast<UpgradeTestManagedObject5 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject5, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject5->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject5->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject5->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject5->getObjectId1 ();
            UI32                     integer4                        = pUpgradeTestManagedObject5->getInteger4  ();
            string                 message4                      = pUpgradeTestManagedObject5->getMessage4  ();
            ObjectId                 objectId4                      = pUpgradeTestManagedObject5->getObjectId4 ();
            ObjectId                 association4                    = pUpgradeTestManagedObject5->getAssociation4 ();

            UpgradeTestManagedObject5 *pUpgradeTestManagedObject5Expected = allCreatedObjects5Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject5Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject5Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject5Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject5Expected->getObjectId1 ();
            UI32                     integer4Expected                = pUpgradeTestManagedObject5Expected->getInteger4  ();
            string                 message4Expected              = pUpgradeTestManagedObject5Expected->getMessage4  ();
            ObjectId                 objectId4Expected              = pUpgradeTestManagedObject5Expected->getObjectId4 ();
            ObjectId                 association4Expected            = pUpgradeTestManagedObject5Expected->getAssociation4 ();

            if ((*pUpgradeTestManagedObject5Expected) != (*pUpgradeTestManagedObject5))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Muiltiple Associations In Hierarchy Synchronous Query By ObjectIds Test ...");

    // Now add the ObjectIds to the context so that the query gets only the requested objects.

    vector<UpgradeTestManagedObject5 *>     &allCreatedObjects5    = pUpgradePersistenceTestContext->getCreatedObjects5 ();
    map<ObjectId, UpgradeTestManagedObject5 *> &allCreatedObjects5Map   = pUpgradePersistenceTestContext->getCreatedObjects5Map ();
    UI32                                     numberOfObjectsCreated5   = allCreatedObjects5.size ();
    UI32                                     numberOfObjectsToQueryFor = numberOfObjectsCreated5 > 100 ? 100 : numberOfObjectsCreated5;
    UI32                                     i                       = 0;
    vector<ObjectId>                         objectIds;

    for (i = 0; i < numberOfObjectsToQueryFor; i++)
    {
        objectIds.push_back ((allCreatedObjects5[i])->getObjectId ());
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (UpgradeTestManagedObject5::getClassName (), objectIds);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32       numberOfExpectedResults = numberOfObjectsToQueryFor;
    UI32       numberOfResults       = pResults->size ();
    ResourceId status                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string (" Number of Results (synchronously obtained) = ") + numberOfResults);

    if (numberOfExpectedResults != numberOfResults)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            UpgradeTestManagedObject5 *pUpgradeTestManagedObject5        = dynamic_cast<UpgradeTestManagedObject5 *> ((*pResults)[i]);
            prismAssert (NULL != pUpgradeTestManagedObject5, __FILE__, __LINE__);
            ObjectId                 oid                              = pUpgradeTestManagedObject5->getObjectId  ();
            UI32                     integer1                        = pUpgradeTestManagedObject5->getInteger1  ();
            string                 message1                      = pUpgradeTestManagedObject5->getMessage1  ();
            ObjectId                 objectId1                      = pUpgradeTestManagedObject5->getObjectId1 ();
            UI32                     integer4                        = pUpgradeTestManagedObject5->getInteger4  ();
            string                 message4                      = pUpgradeTestManagedObject5->getMessage4  ();
            ObjectId                 objectId4                      = pUpgradeTestManagedObject5->getObjectId4 ();
            ObjectId                 association4                    = pUpgradeTestManagedObject5->getAssociation4 ();

            UpgradeTestManagedObject5 *pUpgradeTestManagedObject5Expected = allCreatedObjects5Map[oid];
            ObjectId                 oidExpected                      = pUpgradeTestManagedObject5Expected->getObjectId  ();
            UI32                     integer1Expected                = pUpgradeTestManagedObject5Expected->getInteger1  ();
            string                 message1Expected              = pUpgradeTestManagedObject5Expected->getMessage1  ();
            ObjectId                 objectId1Expected              = pUpgradeTestManagedObject5Expected->getObjectId1 ();
            UI32                     integer4Expected                = pUpgradeTestManagedObject5Expected->getInteger4  ();
            string                 message4Expected              = pUpgradeTestManagedObject5Expected->getMessage4  ();
            ObjectId                 objectId4Expected              = pUpgradeTestManagedObject5Expected->getObjectId4 ();
            ObjectId                 association4Expected            = pUpgradeTestManagedObject5Expected->getAssociation4 ();

            if ((*pUpgradeTestManagedObject5Expected) != (*pUpgradeTestManagedObject5))
            {
                trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer1 + string (", ") + message1 + string (", ") + objectId1.getClassId () + string (", ") + objectId1.getInstanceId () + string (", ") + integer4 + string (", ") + message4 + string (", ") + objectId4.getClassId () + string (", ") + objectId4.getInstanceId () + string (", ") + association4.getClassId () + string (", ") + association4.getInstanceId ());
                trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer1Expected + string (", ") + message1Expected + string (", ") + objectId1Expected.getClassId () + string (", ") + objectId1Expected.getInstanceId () + string (", ") + integer4Expected + string (", ") + message4Expected + string (", ") + objectId4Expected.getClassId () + string (", ") + objectId4Expected.getInstanceId () + string (", ") + association4Expected.getClassId () + string (", ") + association4Expected.getInstanceId ());
                prismAssert (false, __FILE__, __LINE__);

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
        }
    }

    for (i = 0; i < numberOfResults; i++)
    {
        delete ((*pResults)[i]);
    }

    delete pResults;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects5 Transaction Test ...");

    ResourceId                       status               = WAVE_MESSAGE_SUCCESS;
    UI32                               i                       = 0;
    vector<UpgradeTestManagedObject1 *> &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    vector<UpgradeTestManagedObject5 *> &allCreatedObjects5   = pUpgradePersistenceTestContext->getCreatedObjects5 ();
    UI32                               numberOfObjectsCreated  = allCreatedObjects.size ();
    UI32                               numberOfObjectsCreated5 = allCreatedObjects5.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated5; i++)
    {
        delete allCreatedObjects5[i];
    }

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        delete allCreatedObjects[i];
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects5TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects5.clear ();
    allCreatedObjects.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-To-Many Muiltiple Associations In Hierarchy Transaction Test ...");

    FrameworkTestabilityMessage11        *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject5 *>     &allCreatedObjects5         = pUpgradePersistenceTestContext->getCreatedObjects5 ();
    map<ObjectId, UpgradeTestManagedObject5 *> &allCreatedObjects5Map        = pUpgradePersistenceTestContext->getCreatedObjects5Map ();
    vector<ObjectId>                          tempVector;

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject5 *pUpgradeTestManagedObject5 = new UpgradeTestManagedObject5 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject5->setInteger1 (i);
        pUpgradeTestManagedObject5->setMessage1 ("Test 41");
        pUpgradeTestManagedObject5->setObjectId1 (ObjectId (i, i));
        pUpgradeTestManagedObject5->setInteger4 (i);
        pUpgradeTestManagedObject5->setMessage4 ("Test 44");
        pUpgradeTestManagedObject5->setObjectId4 (ObjectId (i, i));
        pUpgradeTestManagedObject5->setAssociation4 (pUpgradeTestManagedObject1->getObjectId ());

        tempVector.clear ();

        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));
        tempVector.push_back (ObjectId (i, i));

        pUpgradeTestManagedObject5->setObjectIdVector4 (tempVector);

        tempVector.clear ();

        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());
        tempVector.push_back (pUpgradeTestManagedObject1->getObjectId ());

        pUpgradeTestManagedObject5->setAssociationVector4 (tempVector);

        pUpgradeTestManagedObject5->setAssociationVector5 (tempVector);

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        allCreatedObjects5.push_back (pUpgradeTestManagedObject5);
        allCreatedObjects5Map[pUpgradeTestManagedObject5->getObjectId ()] = pUpgradeTestManagedObject5;

    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneCompositionTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-One Composition Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject6 *pUpgradeTestManagedObject6 = new UpgradeTestManagedObject6 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject6->setInteger6 (i);
        pUpgradeTestManagedObject6->setMessage6 ("Test");
        pUpgradeTestManagedObject6->setObjectId6 (ObjectId (i, i));
        pUpgradeTestManagedObject6->setComposition6 (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToOneCompositionTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        delete pUpgradeTestManagedObject6;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-One Composition Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject6::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects6 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject6 *>  &allCreatedObjects6Map           = pUpgradePersistenceTestContext->getCreatedObjects6Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestCallback : Simple One-One Composition Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToOneCompositionQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject6 *pUpgradeTestManagedObject6        = dynamic_cast<UpgradeTestManagedObject6 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject6, __FILE__, __LINE__);
                ObjectId                oid                          = pUpgradeTestManagedObject6->getObjectId  ();
                UI32                    integer6                        = pUpgradeTestManagedObject6->getInteger6  ();
                string                message6                      = pUpgradeTestManagedObject6->getMessage6  ();
                ObjectId                objectId6                      = pUpgradeTestManagedObject6->getObjectId6 ();

                UpgradeTestManagedObject6 *pUpgradeTestManagedObject6Expected = allCreatedObjects6Map[oid];
                ObjectId                oidExpected                  = pUpgradeTestManagedObject6Expected->getObjectId  ();
                UI32                    integer6Expected                = pUpgradeTestManagedObject6Expected->getInteger6  ();
                string                message6Expected              = pUpgradeTestManagedObject6Expected->getMessage6  ();
                ObjectId                objectId6Expected              = pUpgradeTestManagedObject6Expected->getObjectId6 ();

                //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());

                if ((*pUpgradeTestManagedObject6Expected) != (*pUpgradeTestManagedObject6))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer6 + string (", ") + message6 + string (", ") + objectId6.getClassId () + string (", ") + objectId6.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer6Expected + string (", ") + message6Expected + string (", ") + objectId6Expected.getClassId () + string (", ") + objectId6Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToOneCompositionUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-One Composition Update Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject6 *>      &allCreatedObjects6            = pUpgradePersistenceTestContext->getCreatedObjects6 ();

    allCreatedObjects.clear  ();
    allCreatedObjects1Map.clear ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        startTransaction ();

        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject6 *pUpgradeTestManagedObject6   = allCreatedObjects6[i];

        pUpgradeTestManagedObject1->setInteger1 (i * 2);
        pUpgradeTestManagedObject1->setMessage1 ("Test Update 1");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i * 2, i * 2));

        updateWaveManagedObject (pUpgradeTestManagedObject6);

        pUpgradeTestManagedObject6->setInteger6 (i);
        pUpgradeTestManagedObject6->setMessage6 ("Test Update 2");
        pUpgradeTestManagedObject6->setObjectId6 (ObjectId (i * 3, i * 3));
        pUpgradeTestManagedObject6->setComposition6 (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToOneCompositionUpdateTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects6 Transaction Test ...");

    ResourceId                              status                = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                    = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap    = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject6 *>      &allCreatedObjects6      = pUpgradePersistenceTestContext->getCreatedObjects6 ();
    map<ObjectId, UpgradeTestManagedObject6 *>  &allCreatedObjects6Map   = pUpgradePersistenceTestContext->getCreatedObjects6Map ();
    UI32                                      numberOfObjectsCreated6 = allCreatedObjects6.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated6; i++)
    {
        delete allCreatedObjects6[i];
    }

    // NOTICE: We must not delete the UpgradeTestManagedObject1 here since they are owned by the UpgradeTestManagedObject6.
    //       They are implicitly deleted when the owning UpgradeTestManagedObject6 are deleted.
    //       If you explicitly delete UpgradeTestManagedObject1 objects it will result in SIGSEGV!!!

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects6TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects6.clear ();
    allCreatedObjects6Map.clear ();
    allCreatedObjects.clear ();
    allCreatedObjectsMap.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToOneCompositionTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-To-One Composition Transaction Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject6 *>      &allCreatedObjects6            = pUpgradePersistenceTestContext->getCreatedObjects6 ();
    map<ObjectId, UpgradeTestManagedObject6 *>  &allCreatedObjects6Map       = pUpgradePersistenceTestContext->getCreatedObjects6Map ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate; i++)
    {
        UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);
        UpgradeTestManagedObject6 *pUpgradeTestManagedObject6 = new UpgradeTestManagedObject6 (this);

        pUpgradeTestManagedObject1->setInteger1 (i);
        pUpgradeTestManagedObject1->setMessage1 ("Test");
        pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i, i));

        pUpgradeTestManagedObject6->setInteger6 (i);
        pUpgradeTestManagedObject6->setMessage6 ("Test");
        pUpgradeTestManagedObject6->setObjectId6 (ObjectId (i, i));
        pUpgradeTestManagedObject6->setComposition6 (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

        allCreatedObjects.push_back (pUpgradeTestManagedObject1);
        allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        allCreatedObjects6.push_back (pUpgradeTestManagedObject6);
        allCreatedObjects6Map[pUpgradeTestManagedObject6->getObjectId ()] = pUpgradeTestManagedObject6;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::largeOneToOneCompositionTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Composition Transaction Test ...");

    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pUpgradePersistenceTestContext->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    UI32                                      j                          = 0;

    for (i = 0; i < numberOfObjectsToCreate / 10; i++)
    {
        startTransaction ();

        vector <WaveManagedObjectPointer<UpgradeTestManagedObject1> > tempVector;

        for (j = 0; j < 32; j++)
        {
            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);

            pUpgradeTestManagedObject1->setInteger1 (i * 32 + j);
            pUpgradeTestManagedObject1->setMessage1 ("Test");
            pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i * 32 + j, i * 32 + j));

            tempVector.push_back (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

        }

        UpgradeTestManagedObject7 *pUpgradeTestManagedObject7 = new UpgradeTestManagedObject7 (this);

        pUpgradeTestManagedObject7->setInteger7 (i);
        pUpgradeTestManagedObject7->setMessage7 ("Test");
        pUpgradeTestManagedObject7->setObjectId7 (ObjectId (i, i));
        pUpgradeTestManagedObject7->setComposition7 (tempVector);


        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        delete pUpgradeTestManagedObject7;

        if (0 == ((i + 1) % 100))
        {
            trace (TRACE_LEVEL_INFO, string ("  Finished ") + (i + 1) + string (" Simple One-To-Many Composition Transactions"));
        }
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Composition Query Test ...");
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = new WaveManagedObjectQueryContext (UpgradeTestManagedObject7::getClassName (), this, reinterpret_cast<PrismAsynchronousCallback> (&FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestCallback), pUpgradePersistenceTestContext);

    pUpgradePersistenceTestContext->setNumberOfExpectedResults ((pUpgradePersistenceTestContext->getCreatedObjects7 ()).size ());

    query (pWaveManagedObjectQueryContext);
}

void FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestCallback (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    UpgradePersistenceTestContext         *pUpgradePersistenceTestContext = reinterpret_cast<UpgradePersistenceTestContext *> (pWaveManagedObjectQueryContext->getPCallerContext ());
    ResourceId                              status                         = pWaveManagedObjectQueryContext->getCompletionStatus ();
    map<ObjectId, UpgradeTestManagedObject7 *>  &allCreatedObjects7Map           = pUpgradePersistenceTestContext->getCreatedObjects7Map ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestCallback : Simple One-Many Composition Query test failed. Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        vector<WaveManagedObject *> *pResults = pWaveManagedObjectQueryContext->getPResults ();

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfExpectedResults = pUpgradePersistenceTestContext->getNumberOfExpectedResults ();
        UI32 numberOfResults = pResults->size ();
        UI32 i             = 0;

        if (numberOfExpectedResults != numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("FrameworkTestability6ObjectManager::simpleOneToManyCompositionQueryTestCallback : We did not get expected number of results.  Expected = ") + numberOfExpectedResults + string (", Obtained =  ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, string (" Number of Results (asynchronously obtained) = ") + numberOfResults);

            for (i = 0; i < numberOfResults; i++)
            {
                UpgradeTestManagedObject7 *pUpgradeTestManagedObject7        = dynamic_cast<UpgradeTestManagedObject7 *> ((*pResults)[i]);
                prismAssert (NULL != pUpgradeTestManagedObject7, __FILE__, __LINE__);
                ObjectId                oid                          = pUpgradeTestManagedObject7->getObjectId  ();
                UI32                    integer7                        = pUpgradeTestManagedObject7->getInteger7  ();
                string                message7                      = pUpgradeTestManagedObject7->getMessage7  ();
                ObjectId                objectId7                      = pUpgradeTestManagedObject7->getObjectId7 ();

                UpgradeTestManagedObject7 *pUpgradeTestManagedObject7Expected = allCreatedObjects7Map[oid];
                ObjectId                oidExpected                  = pUpgradeTestManagedObject7Expected->getObjectId  ();
                UI32                    integer7Expected                = pUpgradeTestManagedObject7Expected->getInteger7  ();
                string                message7Expected              = pUpgradeTestManagedObject7Expected->getMessage7  ();
                ObjectId                objectId7Expected              = pUpgradeTestManagedObject7Expected->getObjectId7 ();

                //trace (TRACE_LEVEL_DEVEL, string ("       ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer2 + string (", ") + message2 + string (", ") + objectId2.getClassId () + string (", ") + objectId2.getInstanceId ());

                if ((*pUpgradeTestManagedObject7Expected) != (*pUpgradeTestManagedObject7))
                {
                    trace (TRACE_LEVEL_ERROR, string ("Got    : ") + oid.getClassId () + string (", ") + oid.getInstanceId () + string (", ") + integer7+ string (", ") + message7 + string (", ") + objectId7.getClassId () + string (", ") + objectId7.getInstanceId ());
                    trace (TRACE_LEVEL_ERROR, string ("Expected : ") + oidExpected.getClassId () + string (", ") + oidExpected.getInstanceId () + string (", ") + integer7Expected + string (", ") + message7Expected + string (", ") + objectId7Expected.getClassId () + string (", ") + objectId7Expected.getInstanceId ());
                    prismAssert (false, __FILE__, __LINE__);

                    status = WAVE_MESSAGE_ERROR;
                    break;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEVEL, " All Queried results were successfully compared against expected values.");
            }
        }
    }

    delete pWaveManagedObjectQueryContext;

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::simpleOneToManyCompositionUpdateTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Simple One-To-Many Composition Update Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    UI32                                      j                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject7 *>      &allCreatedObjects7            = pUpgradePersistenceTestContext->getCreatedObjects7 ();

    allCreatedObjects.clear  ();
    allCreatedObjects1Map.clear ();

    for (i = 0; i < numberOfObjectsToCreate / 10; i++)
    {
        startTransaction ();

        vector <WaveManagedObjectPointer<UpgradeTestManagedObject1> > tempVector;

        for (j = 0; j < 32; j++)
        {
            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);

            pUpgradeTestManagedObject1->setInteger1 ((i * 32 + j) + 100);
            pUpgradeTestManagedObject1->setMessage1 ("Test Update 1");
            pUpgradeTestManagedObject1->setObjectId1 (ObjectId ((i * 32 + j) * 2, (i * 32 + j) * 2));

            tempVector.push_back (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

            allCreatedObjects.push_back (pUpgradeTestManagedObject1);
            allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        }

        UpgradeTestManagedObject7 *pUpgradeTestManagedObject7 = allCreatedObjects7[i];

        updateWaveManagedObject (pUpgradeTestManagedObject7);

        pUpgradeTestManagedObject7->setInteger7 (i + i);
        pUpgradeTestManagedObject7->setMessage7 ("Test Update 2");
        pUpgradeTestManagedObject7->setObjectId7 (ObjectId (i + i, i + i));
        pUpgradeTestManagedObject7->setComposition7 (tempVector);

        status = commitTransaction ();

        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
            break;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (0 == ((i + 1) % 100))
        {
            trace (TRACE_LEVEL_INFO, string ("  Finished ") + (i + 1) + string (" Simple One-To-Many Composition Transactions"));
        }
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Delete All Created Objects7 Transaction Test ...");

    ResourceId                              status                = WAVE_MESSAGE_SUCCESS;
    UI32                                      i                    = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects     = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjectsMap    = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject7 *>      &allCreatedObjects7      = pUpgradePersistenceTestContext->getCreatedObjects7 ();
    map<ObjectId, UpgradeTestManagedObject7 *>  &allCreatedObjects7Map   = pUpgradePersistenceTestContext->getCreatedObjects7Map ();
    UI32                                      numberOfObjectsCreated7 = allCreatedObjects7.size ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsCreated7; i++)
    {
        delete allCreatedObjects7[i];
    }

    // NOTICE: We must not delete the UpgradeTestManagedObject1 here since they are owned by the UpgradeTestManagedObject6.
    //       They are implicitly deleted when the owning UpgradeTestManagedObject6 are deleted.
    //       If you explicitly delete UpgradeTestManagedObject1 objects it will result in SIGSEGV!!!

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::deleteAllCreatedObjects7TestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    allCreatedObjects7.clear ();
    allCreatedObjects7Map.clear ();
    allCreatedObjects.clear ();
    allCreatedObjectsMap.clear ();

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::largeOneToManyCompositionTransactionTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    trace (TRACE_LEVEL_INFO, "Large One-To-Many Composition Transaction Test ...");

    FrameworkTestabilityMessage11            *pFrameworkTestabilityMessage11 = reinterpret_cast<FrameworkTestabilityMessage11 *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId                              status                      = WAVE_MESSAGE_SUCCESS;
    UI32                                      numberOfObjectsToCreate      = pFrameworkTestabilityMessage11->getNumberOfObjectsToBeCreated ();
    UI32                                      i                          = 0;
    UI32                                      j                          = 0;
    vector<UpgradeTestManagedObject1 *>     &allCreatedObjects           = pUpgradePersistenceTestContext->getCreatedObjects ();
    map<ObjectId, UpgradeTestManagedObject1 *> &allCreatedObjects1Map        = pUpgradePersistenceTestContext->getCreatedObjectsMap ();
    vector<UpgradeTestManagedObject7 *>      &allCreatedObjects7            = pUpgradePersistenceTestContext->getCreatedObjects7 ();
    map<ObjectId, UpgradeTestManagedObject7 *>  &allCreatedObjects7Map       = pUpgradePersistenceTestContext->getCreatedObjects7Map ();

    startTransaction ();

    for (i = 0; i < numberOfObjectsToCreate / 10; i++)
    {
        vector <WaveManagedObjectPointer<UpgradeTestManagedObject1> > tempVector;

        for (j = 0; j < 32; j++)
        {
            UpgradeTestManagedObject1 *pUpgradeTestManagedObject1 = new UpgradeTestManagedObject1 (this);

            pUpgradeTestManagedObject1->setInteger1 (i * 32 + j);
            pUpgradeTestManagedObject1->setMessage1 ("Test");
            pUpgradeTestManagedObject1->setObjectId1 (ObjectId (i * 32 + j, i * 32 + j));

            tempVector.push_back (WaveManagedObjectPointer<UpgradeTestManagedObject1> (pUpgradeTestManagedObject1));

            allCreatedObjects.push_back (pUpgradeTestManagedObject1);
            allCreatedObjects1Map[pUpgradeTestManagedObject1->getObjectId ()] = pUpgradeTestManagedObject1;
        }

        UpgradeTestManagedObject7 *pUpgradeTestManagedObject7 = new UpgradeTestManagedObject7 (this);

        pUpgradeTestManagedObject7->setInteger7 (i);
        pUpgradeTestManagedObject7->setMessage7 ("Test");
        pUpgradeTestManagedObject7->setObjectId7 (ObjectId (i, i));
        pUpgradeTestManagedObject7->setComposition7 (tempVector);

        allCreatedObjects7.push_back (pUpgradeTestManagedObject7);
        allCreatedObjects7Map[pUpgradeTestManagedObject7->getObjectId ()] = pUpgradeTestManagedObject7;
    }

    status = commitTransaction ();

    if (status != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkTestability6ObjectManager::simpleOneToManyCompositionTransactionTestStep : Transaction failed.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pUpgradePersistenceTestContext->executeNextStep (status);
}

void FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep(UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    ResourceId  status  =   WAVE_MESSAGE_SUCCESS;
    trace (TRACE_LEVEL_INFO, string ("FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep start"));

    string upgradeMOName                        =  OrmRepository::getSchemaUpgradeTableName ();

    DatabaseSchema dbSchemaBeforeModify, dbSchemaAfterDeleteMO, dbSchemaAfterRestore, dbSchemaAfterModifyMO, dbSchemaAfterDBConversion;
    OrmToolKit::getDatabaseSchema(dbSchemaBeforeModify);

    string sqlToDeleteAnMOFromUpgradeTable        = string ("DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + " WHERE classname = 'PrismTestManagedObject5';\n"
                                                        + "DELETE FROM " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + " WHERE classname = 'PrismTestManagedObject5';\n");

    bool st = backupAndAlterUpgradeMo (sqlToDeleteAnMOFromUpgradeTable);
    if (false == st)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep : failed to backup and alter the upgradeMO");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    OrmToolKit::getDatabaseSchema(dbSchemaAfterDeleteMO);

    if (dbSchemaBeforeModify == dbSchemaAfterDeleteMO)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: FAILED: dbSchemaBeforeModify == dbSchemaAfterDeleteMO .");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: PASSED: dbSchemaBeforeModify != dbSchemaAfterDeleteMO .");
    }

    if (dbSchemaAfterDeleteMO.isConvertibleTo (dbSchemaBeforeModify))
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: FAILED: dbSchemaAfterDeleteMO->isConvertibleTo (dbSchemaBeforeModify) .");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: PASSED: dbSchemaAfterDeleteMO->isNotConvertibleTo (dbSchemaBeforeModify) .");
    }

    st = restoreUpgradeMo ();
    if (false == st)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep : failed to restore the upgradeMO");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    string sqlToAddFieldInMOInUpgradeTable = string (
                      "UPDATE " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName
    + " SET fieldnames = concat(fieldnames,'11#newColumnId'), fieldtypes = concat(fieldtypes, '24#WAVE_ATTRIBUTE_TYPE_UI32'), expandedfieldtypes = concat(expandedfieldtypes, '36#WAVE_ATTRIBUTE_TYPE_UI32|integer|0|0')  WHERE classname = 'PrismTestManagedObject5';\n"
                    + " UPDATE " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName
    + " SET fieldnames = concat(fieldnames,'11#newColumnId'), fieldtypes = concat(fieldtypes, '24#WAVE_ATTRIBUTE_TYPE_UI32'), expandedfieldtypes = concat(expandedfieldtypes, '36#WAVE_ATTRIBUTE_TYPE_UI32|integer|0|0')  WHERE classname = 'PrismTestManagedObject5';\n"
                    + " ALTER TABLE " + OrmRepository::getWaveCurrentSchema () + ".PrismTestManagedObject5 ADD COLUMN newColumnId integer;\n" 
                    + " ALTER TABLE " + OrmRepository::getWaveStartSchema () + ".PrismTestManagedObject5 ADD COLUMN newColumnId integer;"); 

    st = backupAndAlterUpgradeMo (sqlToAddFieldInMOInUpgradeTable);
    if (false == st)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep : failed to backup and alter the upgradeMO");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    OrmToolKit::getDatabaseSchema(dbSchemaAfterModifyMO);

    if (dbSchemaBeforeModify == dbSchemaAfterModifyMO)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: FAILED: dbSchemaBeforeModify == dbSchemaAfterModifyMO .");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: PASSED: dbSchemaBeforeModify != dbSchemaAfterModifyMO .");
    }

    if (dbSchemaAfterModifyMO.isConvertibleTo (dbSchemaBeforeModify))
    {
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: PASSED: dbSchemaAfterModifyMO->isConvertibleTo (dbSchemaBeforeModify) .");

        status = PrismFrameworkObjectManager::getInstance ()->upgradeDatabase ();
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: FAILED: dbSchemaAfterModifyMO->isNotConvertibleTo (dbSchemaBeforeModify) .");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    OrmToolKit::getDatabaseSchema(dbSchemaAfterDBConversion);

    st = restoreUpgradeMo ();
    if (false == st)
    {
        trace (TRACE_LEVEL_WARN, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep : failed to restore the upgradeMO");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }

    OrmToolKit::getDatabaseSchema(dbSchemaAfterRestore);

    if (dbSchemaAfterDBConversion == dbSchemaAfterRestore)
    {
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: PASSED: dbSchemaAfterDBConversion == dbSchemaAfterRestore .");
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::validateGetDatabaseSchemaStep: FAILED: dbSchemaAfterDBConversion != dbSchemaAfterRestore .");
        pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_ERROR);
        return;
    }
    pUpgradePersistenceTestContext->executeNextStep (status);
}

//Changes for Integration and Backup restore support
void FrameworkTestability6ObjectManager::validatePrepareStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    ResourceId  status  =   WAVE_MESSAGE_SUCCESS;
    trace (TRACE_LEVEL_INFO,string ("FrameworkTestability6ObjectManager::validatePrepareStep start"));
    string xmlFileForinstrumentation = getTestParameterValue("xmlForModify");
    m_pUpgradeDatabaseSchemaParser = NULL;
    if ( xmlFileForinstrumentation  != "" )
    {
        ifstream inputXMLFile (xmlFileForinstrumentation.c_str());
        if (false == (inputXMLFile.is_open ()))
        {
            trace (TRACE_LEVEL_ERROR, string("FrameworkTestability6ObjectManager::validatePrepareStep: Please provide the correct filename for DB upgrade instrumentation: file not found"));
            status = WAVE_MESSAGE_ERROR;
        } else {
            m_pUpgradeDatabaseSchemaParser= new UpgradeDatabaseSchemaParser(xmlFileForinstrumentation);
            inputXMLFile.close();
        }
    } else {
        trace (TRACE_LEVEL_ERROR, string("FrameworkTestability6ObjectManager::validatePrepareStep: Please prepare the service with proper option for regression run: regression> prepare2 <serviceid> -xmlForModify <xmlfile> -applysql true"));
        status = WAVE_MESSAGE_ERROR;
    }

    if ( !FrameworkToolKit::isPrimaryLocation () && !FrameworkToolKit::isStandAloneLocation ())
    {
        /* 
            pausePersistence(called from backupDatabaseStep) is designed to call only from a Primary Location.
            This regression tests DB conversion and it is not required to test any cluster behavior.
            Hence adding this check to avoid calling pausePersistence from a secondary location.
        */
        trace (TRACE_LEVEL_ERROR, string("FrameworkTestability6ObjectManager::validatePrepareStep: This regression run can be only run on a Primary Location/ Standalone location."));
        status = WAVE_MESSAGE_ERROR;
    }
    pUpgradePersistenceTestContext->executeNextStep (status);
}

/* Do the instrumentation in the Database*/
void FrameworkTestability6ObjectManager::instrumentTheDatabaseStep(UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    RegressionTestMessage            *pRegressionTestMessage = reinterpret_cast<RegressionTestMessage *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    trace (TRACE_LEVEL_INFO,string ("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep start"));
    ResourceId  status  =   WAVE_MESSAGE_SUCCESS;
    if ( m_pUpgradeDatabaseSchemaParser )
    {
        trace (TRACE_LEVEL_INFO,string ("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep Parsing the XML file"));
        m_pUpgradeDatabaseSchemaParser->loadFromXML();
        //m_pUpgradeDatabaseSchemaParser->display();

        string sql;
        m_pUpgradeDatabaseSchemaParser->getSql(sql);
        trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager:: Get Difference Schema Sql Statement:\n") + sql );
        m_pUpgradeDatabaseSchemaParser->printDifferencesInVectors();

        if ( getTestParameterValue("instrumentcfg" ) == "true" )
        {
            //m_pUpgradeDatabaseSchemaParser->savePrismConfiguration();
            bool st = populateSchemaUpgradeTableInDb ();
            if (false == st) 
            {
                trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep: failed to save upgradeMO in the db. can not continue."));
                status = WAVE_MESSAGE_ERROR;
            }
        }

        if ( (WAVE_MESSAGE_SUCCESS == status ) && (getTestParameterValue("applysql") == "true" ))
        {
            vector <string>  sqlStmtVector;
            tokenize (sql, sqlStmtVector, ';' );
            for ( UI32 j=0; ( status == WAVE_MESSAGE_SUCCESS) && (j <sqlStmtVector.size()); j++ )
            {
                /*
                 * We can other wise check for ALTER/CREATE/DROP/DELETE
                 */
                if ( (""  == sqlStmtVector[j]) || (8 > sqlStmtVector[j].size() ) )
                {
                    continue;
                }
                DatabaseObjectManagerExecuteTransactionMessage *pDatabaseObjectManagerExecuteTransactionMessage = new DatabaseObjectManagerExecuteTransactionMessage((sqlStmtVector[j] + ";\n" ));
                status = sendSynchronously(pDatabaseObjectManagerExecuteTransactionMessage);
                //trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep send DatabaseObjectManagerExecuteTransactionMessage Status " +  ((status == WAVE_MESSAGE_SUCCESS)?string("Success"):string( "Failure")) ));

                status = pDatabaseObjectManagerExecuteTransactionMessage->getCompletionStatus();
                delete (pDatabaseObjectManagerExecuteTransactionMessage);
                 
            }
            trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep  DatabaseObjectManagerExecuteTransactionMessage Apply Transaction Handler Status " +  ((status == WAVE_MESSAGE_SUCCESS)? string("Success"):string( "Failure"))) );
            
        }

    }
    trace (TRACE_LEVEL_INFO,string ("FrameworkTestability6ObjectManager::instrumentTheDatabaseStep Completed"));
    pRegressionTestMessage->setCompletionStatus(status);
    /*
     * We will populate the regression Message with the completion status  so that we complete the restore operation
     */
    pUpgradePersistenceTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}


bool FrameworkTestability6ObjectManager::populateSchemaUpgradeTableInDb ()
{
    string upgradeMOName                        =  OrmRepository::getSchemaUpgradeTableName ();

    string sqlToTakeBackupForUpgradeTable       = string ("CREATE TABLE " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + "_bk AS SELECT * FROM " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + ";\n" + "CREATE TABLE " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + "_bk AS SELECT * FROM " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + ";\n");

    string sqlToDropAndCreateSchemaUpgradeTable = string ("DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + ";\n"
                                                        + "DELETE FROM " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + ";\n");

    string sqlToInsertUpgradeTableInCurrentSchema = "";
    m_pUpgradeDatabaseSchemaParser->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInCurrentSchema, OrmRepository::getWaveCurrentSchema ());

    string sqlToInsertUpgradeTableInStartSchema = "";
    m_pUpgradeDatabaseSchemaParser->generateSqlToPopulateUpgradeTable (sqlToInsertUpgradeTableInStartSchema, OrmRepository::getWaveStartSchema ());

    string sqlToDropAndRecreateUpgradeTable = sqlToTakeBackupForUpgradeTable + sqlToDropAndCreateSchemaUpgradeTable + sqlToInsertUpgradeTableInCurrentSchema + sqlToInsertUpgradeTableInStartSchema;

    //trace (TRACE_LEVEL_DEBUG, "FrameworkTestability6ObjectManager : sqlToDropAndRecreateUpgradeTable = " + sqlToDropAndRecreateUpgradeTable + "\n");

    DatabaseObjectManagerExecuteTransactionMessage message (sqlToDropAndRecreateUpgradeTable);
    ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
         trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::populateSchemaUpgradeTableInDb: Could not send message to create schemaInfo Object to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
        return (false);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::populateSchemaUpgradeTableInDb:: failed to updated schemaInfo object in db. Status : " + FrameworkToolKit::localize (status));
            return (false);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::populateSchemaUpgradeTableInDb: updated schemaInfo object in db successfully.");
        }
    }
    return (true);
}


void FrameworkTestability6ObjectManager::getManagedObjectsMessageHandler (FrameworkTestability6ObjectManagerGetManagedObjects *pFrameworkTestability6ObjectManagerGetManagedObjects)
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability6ObjectManager::getManagedObjectsMessageHandler : Entering ...");

    const string managedObjectClassName = pFrameworkTestability6ObjectManagerGetManagedObjects->getManagedObjectClassName ();
    const string schemaName          = pFrameworkTestability6ObjectManagerGetManagedObjects->getSchemaName           ();

    trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::getManagedObjectsMessageHandler : Managed Object Class Name : " + managedObjectClassName);
    trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::getManagedObjectsMessageHandler : Schema Name            : " + schemaName);

    WaveManagedObjectSynchronousQueryContext queryContext (managedObjectClassName);

    queryContext.setSchemaToQuery (schemaName);
    queryContext.addOrderField  ("name");

    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&queryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

    UI32               numberOfWaveManagedObjects = pWaveManagedObjects->size ();
    UI32               i                          = 0;
    string           showDump;
    vector<string>  &managedObjects          = pFrameworkTestability6ObjectManagerGetManagedObjects->getManagedObjects ();
    WaveManagedObject *pWaveManagedObject        = NULL;
    bool               useRawFromat            = pFrameworkTestability6ObjectManagerGetManagedObjects->getUseRawFormat ();

    for ( i = 0; i < numberOfWaveManagedObjects; i++)
    {
        pWaveManagedObject = (*pWaveManagedObjects)[i];

        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        showDump = managedObjectClassName + " " + pWaveManagedObject->getName ();

        if (true == (useRawFromat))
        {
            pWaveManagedObject->getRawShowDump (showDump, WAVE_MANAGED_OBJECT_SHOW_CONFIGURATION);
        }
        else
        {
            pWaveManagedObject->getShowDump (showDump, WAVE_MANAGED_OBJECT_SHOW_CONFIGURATION);
        }

        managedObjects.push_back (showDump);

        delete pWaveManagedObject;
    }

    delete pWaveManagedObjects;

    pFrameworkTestability6ObjectManagerGetManagedObjects->setNumberOfManagedObjects (numberOfWaveManagedObjects);

    pFrameworkTestability6ObjectManagerGetManagedObjects->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pFrameworkTestability6ObjectManagerGetManagedObjects);
}



void  FrameworkTestability6ObjectManager::pausePersistence()
{
    // Pause the persistence object manager before taking a backup of the DB.

    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability6ObjectManager::pausePersistence : Starting ...");

    PrismPauseObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::pausePersistence : Could not pause Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::pausePersistence : Could not pause Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::pausePersistence : Successfully paused Persistence.");
        }
    }
}

void  FrameworkTestability6ObjectManager::backupDatabaseStep(UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/BIST" + FrameworkToolKit::getDatabaseBackupFileName ());
    trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::backupDatabaseStep : Starting ..." + backupFileName);
    pausePersistence();

    /* Backup the DB*/

    DatabaseObjectManagerBackupMessage message (backupFileName);
    message.setBackupAllSchema(true);
    ResourceId                         status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::backupDatabaseStep : Could not send message to backup database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::backupDatabaseStep : Message to backup database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
             trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::backupDatabaseStep : Successfully backed up the database.");
        }
    }
    resumePersistence();
    resumeDatabase();

    backupConfigurationFile();
    pUpgradePersistenceTestContext->executeNextStep (status);


}

void  FrameworkTestability6ObjectManager::resumePersistence()
{
     trace (TRACE_LEVEL_DEVEL, "FrameworkTestability6ObjectManager::resumePersistence : Starting ...");

    PrismResumeObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::resumePersistence : Could not resume Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::resumePersistence : Could not resume Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::resumePersistence : Successfully resumed Persistence.");
        }
    }
}

void  FrameworkTestability6ObjectManager::cleanPreparedTransactions()
{
    DatabaseObjectManagerCleanPreparedTransactionsMessage databaseObjectManagerCleanPreparedTransactionsMessage;
    ResourceId                                            cleanPreparedTransactionsStatus                        = WAVE_MESSAGE_SUCCESS;

    cleanPreparedTransactionsStatus = sendSynchronously (&databaseObjectManagerCleanPreparedTransactionsMessage);

    if (WAVE_MESSAGE_SUCCESS != cleanPreparedTransactionsStatus)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::cleanPreparedTransactions: Could not send message to Clean the Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        cleanPreparedTransactionsStatus = databaseObjectManagerCleanPreparedTransactionsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == cleanPreparedTransactionsStatus)
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::cleanPreparedTransactions: Succeeded in cleaning the previously Prepared Transactions.");
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::cleanPreparedTransactions: Failed to Clean the previously Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void FrameworkTestability6ObjectManager::resumeDatabase ()
{
    trace (TRACE_LEVEL_DEVEL, "FrameworkTestability6ObjectManager::resumeDatabase : Starting ...");

    PrismResumeObjectManagerMessage message (DatabaseObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::resumeDatabase : Could not resume Database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::resumeDatabase : Could not resume Database.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::resumeDatabase : Successfully resumed Database.");
        }
    }
}

void  FrameworkTestability6ObjectManager::backupConfigurationFile()
{
    bool status = m_pUpgradeDatabaseSchemaParser->backupPrismConfiguration();
    if ( status != true )
    {
        trace (TRACE_LEVEL_FATAL, string( "FrameworkTestability6ObjectManager::backupConfigurationFile : operation failed"));
    } else {
        trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::backupConfigurationFile successfully backed up the configuration file"));
    }

}

void FrameworkTestability6ObjectManager::restoreConfigurationFile()
{
    bool status = m_pUpgradeDatabaseSchemaParser->restorePrismConfiguration();
    if ( status != true )
    {
        trace (TRACE_LEVEL_FATAL, string( "FrameworkTestability6ObjectManager::restoreConfigurationFile restore of configuration file failed"));
    } else {
        trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::restoreConfigurationFile successfully restore the configuration file from backup"));
    }
}

void  FrameworkTestability6ObjectManager::restoreDatabaseStep(UpgradePersistenceTestContext *pUpgradePersistenceTestContext)
{
    /* Verify the status of instrumentation */
    RegressionTestMessage            *pRegressionTestMessage = reinterpret_cast<RegressionTestMessage *> (pUpgradePersistenceTestContext->getPPrismMessage ());
    ResourceId status1 = pRegressionTestMessage->getCompletionStatus();
    if ( status1 == WAVE_MESSAGE_ERROR )
    {
        trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::restoreDatabaseStep: Bist Testcase has failed need to restore the Database and .cfg"));
    }else   if ( ( getTestParameterValue("instrumentcfg" ) == "true" ) && (getTestParameterValue("upgradeDB" ) == "true" ))
    {
        /* Call update if the instrumentation is successfull*/

        FrameworkObjectManagerUpdateDatabaseSchema updateDatabaseSchemaMessage;
        status1 = sendSynchronously (&updateDatabaseSchemaMessage);
        if ( WAVE_MESSAGE_SUCCESS != status1 )
        {
            trace (TRACE_LEVEL_FATAL, string("FrameworkTestability6ObjectManager::restoreDatabaseStep could not send Message for updateDatabaseSchema"));
        }
        status1 = updateDatabaseSchemaMessage.getCompletionStatus();

        trace (TRACE_LEVEL_INFO, string("FrameworkTestability6ObjectManager::restoreDatabaseStep:  Apply DB upgrade Changes : " +  ((status1 == WAVE_MESSAGE_SUCCESS)? string("Success"):string( "Failure"))) );

    }

    /* If either instrumentation has failed or update has failed restore the Database*/
    if ( status1 == WAVE_MESSAGE_ERROR )
    {
        pausePersistence();
        cleanPreparedTransactions();

        ResourceId                        status  = WAVE_MESSAGE_SUCCESS;
        string sqlToDropSchema = "DROP SCHEMA wavestart CASCADE;\n DROP SCHEMA wavecurrent CASCADE;\n";
        DatabaseObjectManagerExecuteTransactionMessage emessage(sqlToDropSchema);
        status  = sendSynchronously (&emessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::configureSecondaryNodeEmptyDatabaseStep : Could not send message to empty database.  Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = emessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::configureSecondaryNodeEmptyDatabaseStep : Message to empty database failed.  Completion Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/BIST" + FrameworkToolKit::getDatabaseBackupFileName ());
        trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::cleanPreparedTransactions: Entering ..." + backupFileName);
        DatabaseObjectManagerRestoreMessage message (backupFileName);
        message.setRestoreSchema(true);
        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::restoreDatabaseStep : Could not send message to restore database.  Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::restoreDatabaseStep : Message to restore database failed.  Completion Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::restoreDatabaseStep : SUCCESSFULLY Synced up local database with that of Primary.");
            }
        }
        resumePersistence();
        //resumeDatabase();
    }
    else
    {
    /*
     * Restore the cfg file as its not handled as of now in upgrade scenario
     */
        //restoreConfigurationFile();
        bool st = restoreUpgradeMo ();
        if (false == st)
        {
             trace (TRACE_LEVEL_WARN, "FrameworkTestability6ObjectManager::restoreDatabaseStep : failed to restore the upgradeMO");
        }
    }

    delete (m_pUpgradeDatabaseSchemaParser);
    m_pUpgradeDatabaseSchemaParser = NULL;
    pUpgradePersistenceTestContext->executeNextStep (status1);
}

bool FrameworkTestability6ObjectManager::backupAndAlterUpgradeMo (string &alterSql)
{
    string upgradeMOName                        =  OrmRepository::getSchemaUpgradeTableName ();
    string sqlToTakeBackupForUpgradeTable       = string ("CREATE TABLE " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + "_bk AS SELECT * FROM " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + ";\n" + "CREATE TABLE " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + "_bk AS SELECT * FROM " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + ";\n");
    string totalSql                             = sqlToTakeBackupForUpgradeTable + alterSql;

    trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::backupAndAlterUpgradeMo : totalSql is " + totalSql + "\n");

    DatabaseObjectManagerExecuteTransactionMessage message (totalSql);
    ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
         trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::backupAndAlterUpgradeMo : Could not send message to create schemaInfo Object to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
        return (false);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::backupAndAlterUpgradeMo : failed to updated schemaInfo object in db. Status : " + FrameworkToolKit::localize (status));
            return (false);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::backupAndAlterUpgradeMo : updated schemaInfo object in db successfully.");
        }
    }
    return (true);
}

bool FrameworkTestability6ObjectManager::restoreUpgradeMo ()
{
    string upgradeMOName =  OrmRepository::getSchemaUpgradeTableName ();
    string sqlToRestoreUpgradeMo = string ("DROP TABLE " +  OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + "; DROP TABLE "  +  OrmRepository::getWaveStartSchema () + "." + upgradeMOName + ";"
                                    + " ALTER TABLE " + OrmRepository::getWaveCurrentSchema () + "." + upgradeMOName + "_bk RENAME TO " + upgradeMOName + ";"
                                    + " ALTER TABLE " + OrmRepository::getWaveStartSchema () + "." + upgradeMOName + "_bk RENAME TO " + upgradeMOName + ";\n");
                 
    trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::restoreUpgradeMo : sqlToRestoreUpgradeMo = " + sqlToRestoreUpgradeMo);
 
    DatabaseObjectManagerExecuteTransactionMessage message (sqlToRestoreUpgradeMo);
    ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;
 
    status = sendSynchronously (&message);
 
    if (WAVE_MESSAGE_SUCCESS != status)
    {
         trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::restoreUpgradeMo: Could not send message to create schemaInfo Object to represent the Object Relational Mapping. Status : " + FrameworkToolKit::localize (status));
        return (false);
    }
    else
    {   
        status = message.getCompletionStatus ();
 
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "FrameworkTestability6ObjectManager::restoreUpgradeMo:: failed to updated schemaInfo object in db. Status : " + FrameworkToolKit::localize (status));
            return (false);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "FrameworkTestability6ObjectManager::restoreUpgradeMo: restored schemaInfo object in db successfully.");
        }
    }
    return (true);
}


}
