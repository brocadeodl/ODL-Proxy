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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTSYNCHRONOUSINNERQUERYCONTEXT_H 
#define WAVEMANAGEDOBJECTSYNCHRONOUSINNERQUERYCONTEXT_H

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"

namespace WaveNs
{

class WaveManagedObjectSynchronousInnerQueryContext : public WaveManagedObjectSynchronousQueryContext
{
    private :
    protected :
    public :
                                                WaveManagedObjectSynchronousInnerQueryContext       (const string &classToQueryFor);
        virtual                                ~WaveManagedObjectSynchronousInnerQueryContext       ();

                UI32                            getPageSizeForQueryResults                          ();

                void                            setPageSizeForQueryResults                          (UI32 limitForQueryResults);
                void                            setPageQueryContext                                 (WaveObjectManager* callingOm);
                bool                            isPageQueryContextSet                               ();
                string                          getPageQuerySql                                     ();
                void                            moveOffsetOnSuccessfulQuery                         ();
                string                          getSqlWithPagingEnabled                             (WaveObjectManager* callingOm);

                string                          getSqlForSelect                                     (const bool &forCount = false);
                void                            addSelectField                                      (const string &selectField);
                void                            addSelectFields                                     (const vector<string> &selectFields);
                void                            addSelectFieldForObjectIdClassId                    (const string &selectField);
                void                            addSelectFieldForObjectIdInstanceId                 (const string &selectField);
                void                            addSelectFieldForObjectIdWithSuffix                 (const string &selectField, string objectIdSuffix);
                void                            addSelectFieldForObjectId                           (const string &selectField);
                bool                            isSelectFieldAlreadyAdded                           (const string &selectField) const;

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // WAVEMANAGEDOBJECTSYNCHRONOUSINNERQUERYCONTEXT_H 
