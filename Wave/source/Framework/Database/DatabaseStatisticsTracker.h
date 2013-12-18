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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/
    
#ifndef DATABASESTATISTICSTRACKER_H
#define DATABASESTATISTICSTRACKER_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class DatabaseStatisticsTracker
{
    private :
    protected :
    public :
        // DatabaseQueryWorker Statistics

        static  void                incrementDirectQuery                                (const string &managedObjectName);
        static  void                incrementIndirectQuery                              (const string &managedObjectName);
        static  void                incrementManagedObjectsLoadedFromDirectQuery        (const string &managedObjectName);
        static  void                incrementManagedObjectsLoadedFromDirectQuery        (const string &managedObjectName, UI32 numberOfManagedObjectsLoaded);
        static  void                incrementManagedObjectsLoadedFromIndirectQuery      (const string &managedObjectName);
        static  void                incrementManagedObjectsLoadedFromIndirectQuery      (const string &managedObjectName, UI32 numberOfManagedObjectsLoaded);
        
        static  string              getDirectQueryStatistics                            ();
        static  string              getIndirectQueryStatistics                          ();
        static  string              getManagedObjectsLoadedFromDirectQueryStatistics    ();
        static  string              getManagedObjectsLoadedFromIndirectQueryStatistics  ();

        static  string              getAllQueryStatistics                               ();

        static  void                clearDirectQueryMap                                 ();
        static  void                clearIndirectQueryMap                               ();
        static  void                clearManagedObjectsLoadedFromDirectQueryMap         ();
        static  void                clearManagedObjectsLoadedFromIndirectQueryMap       ();

        static  void                clearAllQueryStatistics                             ();


        // DatabaseTransactionWorker Statistics

        static  void                incrementTransactionsRequested                      ();
        static  void                incrementTransactionsExecuted                       ();
        static  void                incrementTransactionsCommitted                      ();
        static  void                incrementVacuumsRequested                           ();
        static  void                incrementVacuumsExecuted                            ();
        static  void                setVacuumInterval                                   (UI32 vacuumInterval);

        static  string              getTransactionsRequestedStatistics                  ();
        static  string              getTransactionsExecutedStatistics                   ();
        static  string              getTransactionsCommittedStatistics                  ();
        static  string              getVacuumsRequestedStatistics                       ();
        static  string              getVacuumsExecutedStatistics                        ();
        static  string              getVacuumInterval                                   ();

        static  string              getAllTransactionStatistics                         ();

        static  void                clearTransactionsRequestedCounter                   ();
        static  void                clearTransactionsExecutedCounter                    ();
        static  void                clearTransactionsCommittedCounter                   ();
        static  void                clearVacuumsRequestedCounter                        ();
        static  void                clearVacuumsExecutedCounter                         ();
        
        static  void                clearAllTransactionStatistics                       ();

    // Now the data members
    private :
    protected :
    public :
};

}

#endif // DATABASESTATISTICSTRACKER_H
