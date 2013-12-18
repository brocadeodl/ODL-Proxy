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

#ifndef PRISMMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORDELETION_H
#define PRISMMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORDELETION_H

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"

namespace WaveNs
{

class WaveManagedObjectSynchronousQueryContextForDeletion : public WaveManagedObjectSynchronousQueryContext
{
    private :
                void                            getSqlForDeleteFromDerivationAndAuxilliaryTables    (string &sql, string instanceIdSql, OrmTable *pTable);
    protected :
    public :
                                                WaveManagedObjectSynchronousQueryContextForDeletion (const string &classToQueryFor);
        virtual                                ~WaveManagedObjectSynchronousQueryContextForDeletion ();

                UI32                            getPageSizeForQueryResults                          ();

                void                            setPageSizeForQueryResults                          (UI32 limitForQueryResults);
                void                            setPageQueryContext                                 (WaveObjectManager* callingOm);
                bool                            isPageQueryContextSet                               ();
                string                          getPageQuerySql                                     ();
                void                            moveOffsetOnSuccessfulQuery                         ();
                string                          getSqlWithPagingEnabled                             (WaveObjectManager* callingOm);
                void                            getSqlForDelete                                     (string &sql);
                void                            getSqlForDeletingCompositions                       (string &sql, set<string> compositions = set<string>());
                void                            getSqlForDeletingCompositionsPerTable               (string &sql, set<string> compositions, OrmTable *pTable);
                void                            getSqlForPartialDeleteFromVectorCompositions        (string &sql, string &parentClassName, string &compositionName);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // PRISMMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORDELETION_H
