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

#ifndef WAVEOBJECTMANAGERSTATISTICSTRACKER_H
#define WAVEOBJECTMANAGERSTATISTICSTRACKER_H

#include "Framework/Types/Types.h"

#include <map>

namespace WaveNs
{

class WaveObjectManagerStatisticsTracker
{
    private :
    protected :
    public :
                                    WaveObjectManagerStatisticsTracker                  ();
        virtual                    ~WaveObjectManagerStatisticsTracker                  ();

                void                incrementManagedObjectsCreated                      (const string &managedObjectName);
                void                incrementManagedObjectsUpdated                      (const string &managedObjectName);
                void                incrementManagedObjectsDeleted                      (const string &managedObjectName);
                void                incrementCompositionsCreated                        (const string &compositionName);
                void                incrementCompositionsDeleted                        (const string &compositionName);
                void                incrementAssociationsCreated                        (const string &associationName);
                void                incrementAssociationsDeleted                        (const string &associationName);

                string              getManagedObjectsCreatedStatistics                  ();
                string              getManagedObjectsUpdatedStatistics                  ();
                string              getManagedObjectsDeletedStatistics                  ();
                string              getCompositionsCreatedStatistics                    ();
                string              getCompositionsDeletedStatistics                    ();
                string              getAssociationsCreatedStatistics                    ();
                string              getAssociationsDeletedStatistics                    ();

                string              getAllManagedObjectStatistics                       ();

                void                clearManagedObjectsCreatedMap                       ();
                void                clearManagedObjectsUpdatedMap                       ();
                void                clearManagedObjectsDeletedMap                       ();
                void                clearCompositionsCreatedMap                         ();
                void                clearCompositionsDeletedMap                         ();
                void                clearAssociationsCreatedMap                         ();
                void                clearAssociationsDeletedMap                         ();

                void                clearAllManagedObjectStatistics                     ();

    // Now the data members
    private :
                map<string, UI32>   m_managedObjectsCreated;
                map<string, UI32>   m_managedObjectsUpdated;
                map<string, UI32>   m_managedObjectsDeleted;
                map<string, UI32>   m_compositionsCreated;
                map<string, UI32>   m_compositionsDeleted;
                map<string, UI32>   m_associationsCreated;
                map<string, UI32>   m_associationsDeleted;

    protected :
    public :
};

}

#endif // WAVEOBJECTMANAGERSTATISTICSTRACKER_H
