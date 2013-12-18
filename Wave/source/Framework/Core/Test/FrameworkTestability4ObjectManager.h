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

#ifndef FRAMEWORKTESTABILITY4OBJECTMANAGER_H
#define FRAMEWORKTESTABILITY4OBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

namespace WaveNs
{

class PrismAsynchronousContext;
class FrameworkTestabilityMessage8;
class FrameworkTestabilityMessage9;
class FrameworkTestabilityMessage10;
class FrameworkPersistenceTestContext;
class WaveManagedObjectQueryContext;
class FrameworkTestability4ObjectManagerGetManagedObjects;
class WaveSendToClusterContext;

class FrameworkTestability4ObjectManager: public WaveLocalObjectManager
{
    private :
                            FrameworkTestability4ObjectManager                                                  ();

        void                initialize                                                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        WaveManagedObject *createManagedObjectInstance                                                          (const string &managedClassName);
        PrismMessage      *createMessageInstance                                                                (const UI32 &operationCode);

        void                frameworkTestabilityMessage8RequestHandler                                          (FrameworkTestabilityMessage8    *pFrameworkTestabilityMessage8);
        void                frameworkTestability9MessageHandler                                                 (FrameworkTestabilityMessage9    *pMessage);
        void                frameworkTestability10MessageSendToWaveClusterCallback                              (WaveSendToClusterContext *pWaveSendToClusterContext);
        void                simpleTransactionTestStep                                                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryTestStep                                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryTestCallback                                                             (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleQueryUsingEqualTestStep                                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingNotEqualTestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingGreaterThanTest1Step                                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingGreaterThanTest2Step                                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingGreaterThanOrEqualToTest1Step                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingGreaterThanOrEqualToTest2Step                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingLessThanTest1Step                                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingLessThanTest2Step                                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingLessThanOrEqualToTest1Step                                          (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingLessThanOrEqualToTest2Step                                          (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingLikeTestStep                                                        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingNotLikeTestStep                                                     (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingILikeTestStep                                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryUsingNotILikeTestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByObjectIdsTestStep                                                      (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeTestStep                                                      (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeRangeTestStep                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeSqlBetweenTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeSqlBetweenStringTestStep                                      (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeSqlInTestStep                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryByAttributeSqlInStringTestStep                                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryWithOneConditionSetTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryWithTwoConditionSets1TestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryWithTwoConditionSets2TestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleQueryWithThreeConditionSetsTestStep                                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleSynchronousQueryTestStep                                                      (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleSynchronousQueryByObjectIdsTestStep                                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleSynchronousQueryByObjectIdTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleSynchronousQueryWithConditionSetsTestStep                                     (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleUpdateTestStep                                                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simplePartialUpdateTestStep                                                         (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simplePartialIncrementalUpdateTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeUpdateTestStep                                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                updateAllRowsWithUpdateContextTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                partialUpdateAllRowsWithUpdateContextTestStep                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                updateFewRowsWithUpdateContextTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllUsingDeleteWaveManagedObjectWithObjectIdTestStep                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllUsingDeleteWaveManagedObjectWithDeletionContextTestStep                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteOneToManyAssociationWithDeletionContextTestStep                               (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteOneToOneAssociationWithDeletionContextTestStep                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjectsTestStep                                                     (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeTransactionTestStep                                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToAssociationTransactionTestStep                                           (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToAssociationCanBeEmptyTransactionTestStep                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryUsingIsNullTestStep                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryUsingIsNotNullTestStep                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryByAttributeSqlInTestStep                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryByAttributeSqlExistsTestStep                          (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryByAttributeSqlNotExistsTestStep                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryByManagedViewTestStep                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryByManagedViewWithUnionSqlTestStep                     (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationQueryTestCallback                                          (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleOneToOneAssociationQueryByObjectIdsTestStep                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationSynchronousQueryTestStep                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneAssociationSynchronousQueryByObjectIdsTestStep                        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects2TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToAssociationUpdateTestStep                                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToAssociationUpdateTestStep                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                updateAssociationWithUpdateContextTestStep                                          (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToAssociationTransactionTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleDerivationTransactionTestStep                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleDerivationQueryTestStep                                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleDerivationQueryTestCallback                                                   (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleDerivationQueryByObjectIdsTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleDerivationSynchronousQueryTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleDerivationSynchronousQueryByObjectIdsTestStep                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects3TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeDerivationTransactionTestStep                                                  (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationTransactionTestStep                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationQueryTestStep                                             (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationQueryTestCallback                                         (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleOneToManyAssociationQueryByObjectIdsTestStep                                  (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationSynchronousQueryTestStep                                  (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationSynchronousQueryByObjectIdsTestStep                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyAssociationUpdateTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects4TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToManyAssociationTransactionTestStep                                        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                simpleOneToManyMultipleAssociationsInHierarchyTransactionTestStep                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyMultipleAssociationsInHierarchyQueryTestStep                         (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyMultipleAssociationsInHierarchyQueryTestCallback                     (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleOneToManyMultipleAssociationsInHierarchyQueryByObjectIdsTestStep              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryTestStep              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyMultipleAssociationsInHierarchySynchronousQueryByObjectIdsTestStep   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToManyMultipleAssociationsInHierarchyTransactionTestStep                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects5TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                simpleOneToOneCompositionTransactionTestStep                                        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneCompositionQueryTestStep                                              (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneCompositionQueryTestCallback                                          (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleOneToOneCompositionUpdateTestStep                                             (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteOneToOneCompositionWithDeletionContextTestStep                                (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllOneToOneCompositionUsingDeleteWaveManagedObjectWithObjectIdTestStep        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllOneToOneCompositionUsingDeleteWaveManagedObjectWithDeletionContextTestStep (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects6TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToOneCompositionTransactionTestStep                                         (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                simpleOneToManyCompositionTransactionTestStep                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyCompositionQueryTestStep                                             (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyCompositionQueryTestCallback                                         (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                simpleOneToManyCompositionUpdateTestStep                                            (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllOneToManyCompositionUsingDeleteWaveManagedObjectWithObjectIdTestStep       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllOneToManyCompositionUsingDeleteWaveManagedObjectWithDeletionContextTestStep (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllOneToManyCompositionUsingDeleteMultipleWaveManagedObjectsWithDeletionContextTestStep (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteManagedObjectCompositionsOneToManyTestStep (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                deleteAllCreatedObjects7TestStep                                                    (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                largeOneToManyCompositionTransactionTestStep                                        (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                simpleOneToOneToOneCompositionTransactionTestStep                                                   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneToOneCompositionQueryTestStep                                                         (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToOneToOneCompositionQueryTestCallback                                                     (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                deleteAllOneToOneToOneCompositionUsingDeleteMultipleWaveManagedObjectsWithDeletionContextTestStep   (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                simpleOneToManyToManyCompositionTransactionTestStep                                                 (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyToManyCompositionQueryTestStep                                                       (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
        void                simpleOneToManyToManyCompositionQueryTestCallback                                                   (WaveManagedObjectQueryContext   *pWaveManagedObjectQueryContext);
        void                deleteAllOneToManyToManyCompositionUsingDeleteMultipleWaveManagedObjectsWithDeletionContextTestStep (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);

        void                getManagedObjectsMessageHandler                                                     (FrameworkTestability4ObjectManagerGetManagedObjects *pFrameworkTestability4ObjectManagerGetManagedObjects);

		void                simpleSynchronousPageQueryTestStep                                                  (FrameworkPersistenceTestContext *pFrameworkPersistenceTestContext);
    protected :
    public :
        virtual ~FrameworkTestability4ObjectManager                     ();

        static  FrameworkTestability4ObjectManager *getInstance         ();
        static  PrismServiceId                      getPrismServiceId   ();
        static  string                              getPrismServiceName ();


    // Now the data members

    private :
    protected :
    public :
};

}

#endif // FRAMEWORKTESTABILITY4OBJECTMANAGER_H
