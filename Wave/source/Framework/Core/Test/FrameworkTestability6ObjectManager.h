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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.           *
 *   All rights reserved.                                                   *
 *   Author : Vidyasagara Reddy Guntaka                                 *
 ***************************************************************************/

#ifndef FRAMEWORKTESTABILITY6OBJECTMANAGER_H
#define FRAMEWORKTESTABILITY6OBJECTMANAGER_H

#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Regression/RegressionTestMessage.h"
#include "Framework/Core/Test/UpgradeDatabaseSchemaParser.h"
#include "Regression/PrismTestObjectManager.h"
#include <map>
#include <vector>
#include <string>


namespace WaveNs
{

class PrismAsynchronousContext;
class FrameworkTestabilityMessage8;
class FrameworkTestabilityMessage9;
class FrameworkTestabilityMessage10;
class UpgradePersistenceTestContext;
class WaveManagedObjectQueryContext;
class FrameworkTestability6ObjectManagerGetManagedObjects;
class WaveSendToClusterContext;
class UpgradeDatabaseSchemaParser;

class FrameworkTestability6ObjectManager: public PrismTestObjectManager
{
private :
    FrameworkTestability6ObjectManager                          ();

    void                initialize                                                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    WaveManagedObject *createManagedObjectInstance                                  (const string &managedClassName);
    PrismMessage        *createMessageInstance                                          (const UI32 &operationCode);

    void                frameworkTestabilityMessage11RequestHandler                 (FrameworkTestabilityMessage11   *pFrameworkTestabilityMessage8);
    void                simpleTransactionTestStep                                   (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleQueryTestStep                                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleQueryTestCallback                                     (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleQueryByObjectIdsTestStep                              (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleQueryByAttributeTestStep                              (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleQueryByAttributeRangeTestStep                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleSynchronousQueryTestStep                              (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleSynchronousQueryByObjectIdsTestStep                   (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleSynchronousQueryByObjectIdTestStep                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleUpdateTestStep                                        (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeUpdateTestStep                                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllObjectsTestStep                                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjectsTestStep                             (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeTransactionTestStep                                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToAssociationTransactionTestStep                   (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneAssociationQueryTestStep                      (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneAssociationQueryTestCallback                  (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleOneToOneAssociationQueryByObjectIdsTestStep           (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneAssociationSynchronousQueryTestStep           (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects2TestStep                            (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToAssociationUpdateTestStep                        (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToAssociationUpdateTestStep                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToAssociationTransactionTestStep                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleDerivationTransactionTestStep                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleDerivationQueryTestStep                               (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleDerivationQueryTestCallback                           (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleDerivationQueryByObjectIdsTestStep                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleDerivationSynchronousQueryTestStep                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleDerivationSynchronousQueryByObjectIdsTestStep         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects3TestStep                            (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeDerivationTransactionTestStep                          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationTransactionTestStep               (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationQueryTestStep                     (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationQueryTestCallback                 (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleOneToManyAssociationQueryByObjectIdsTestStep          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationSynchronousQueryTestStep          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationSynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyAssociationUpdateTestStep                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects4TestStep                            (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToManyAssociationTransactionTestStep                (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);

    void                simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep               (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep                     (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback                 (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep                (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects5TestStep                                                (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);

    void                simpleOneToOneCompositionTransactionTestStep                                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneCompositionQueryTestStep                                          (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToOneCompositionQueryTestCallback                                      (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleOneToOneCompositionUpdateTestStep                                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects6TestStep                                                (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToOneCompositionTransactionTestStep                                     (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);

    void                simpleOneToManyCompositionTransactionTestStep                                   (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyCompositionQueryTestStep                                         (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                simpleOneToManyCompositionQueryTestCallback                                     (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
    void                simpleOneToManyCompositionUpdateTestStep                                        (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                deleteAllCreatedObjects7TestStep                                                (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                largeOneToManyCompositionTransactionTestStep                                    (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                instrumentTheDatabaseStep                                                       (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                backupDatabaseStep                                                              (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                restoreDatabaseStep                                                             (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                validatePrepareStep                                                             (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                validateGetDatabaseSchemaStep                                                   (UpgradePersistenceTestContext *pUpgradePersistenceTestContext);
    void                resumePersistence                                                               ();
    void                resumeDatabase                                                                  ();
    void                cleanPreparedTransactions                                                       ();
    bool                populateSchemaUpgradeTableInDb                                                  ();
    void                pausePersistence                                                                ();
    void                backupConfigurationFile                                                         ();
    void                restoreConfigurationFile                                                        ();
    bool                restoreUpgradeMo                                                                ();
    bool                backupAndAlterUpgradeMo                                                         (string &alterSql);

    void                getManagedObjectsMessageHandler                                                 (FrameworkTestability6ObjectManagerGetManagedObjects *pFrameworkTestability6ObjectManagerGetManagedObjects);
    void                testRequestHandler                                                              (RegressionTestMessage *pMessage);

protected :
public :
    virtual ~FrameworkTestability6ObjectManager                  ();

    static  FrameworkTestability6ObjectManager *getInstance      ();
    static  PrismServiceId                      getPrismServiceId    ();
    static  string                              getPrismServiceName ();


    // Now the data members

private :
    UpgradeDatabaseSchemaParser                 *m_pUpgradeDatabaseSchemaParser;
protected :
public :
};

}

#endif // FRAMEWORKTESTABILITY4OBJECTMANAGER_H
