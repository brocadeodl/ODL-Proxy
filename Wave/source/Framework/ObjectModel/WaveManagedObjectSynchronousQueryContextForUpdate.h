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
 *   Author : Mithun B S
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORUPDATE_H
#define WAVEMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORUPDATE_H

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveManagedObjectSynchronousQueryContextForUpdate: public WaveManagedObjectSynchronousQueryContext
{
    private :
    protected :
    public :
                                                WaveManagedObjectSynchronousQueryContextForUpdate   (const string &classToQueryFor);
        virtual                                ~WaveManagedObjectSynchronousQueryContextForUpdate   ();

                void                            getSqlForUpdate                                     (string &sql);
                WaveManagedObject *             getWaveManagedObjectToAddUpdateAttributes           ();

                void                            setAttributeObjectIdVectorAssociation               (AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation);
                void                            addOrderField                                       (const string &orderField, const bool &ascending = true);

                void                            addNTupleFormatForOrderField                        (const string &keyName, const char &tupleSeparator, const UI32 &numberOfTuples);

                void                            addSelectField                                      (const string &selectField);
                void                            addSelectFieldForObjectId                           (const string &selectField);
                void                            addSelectFields                                     (const vector<string> &selectFields);
                vector<string>                  getSelectFields                                     () const;
                vector<string>                  getSelectFieldsInManagedObject                      () const;
                bool                            getLoadOneToManyRelationships                       () const;
                void                            setLoadOneToManyRelationships                       (const bool &loadOneToManyRelationships);
                bool                            getLoadCompositions                                 () const;
                void                            setLoadCompositions                                 (const bool loadCompositions);
                void                            addLimitForQueryResults                             (UI32 limitForQueryResults);

                UI32                            getPageSizeForQueryResults                          ();
                void                            setPageSizeForQueryResults                          (UI32 limitForQueryResults);
                void                            setPageQueryContext                                 (WaveObjectManager* callingOm);
                bool                            isPageQueryContextSet                               ();
                string                          getPageQuerySql                                     ();
                void                            moveOffsetOnSuccessfulQuery                         ();
                string                          getSqlWithPagingEnabled                             (WaveObjectManager* callingOm);


    // Now the data members
    private :
                WaveManagedObject *             m_WaveManagedObjectToAddUpdateAttributes;
    protected :
    public :
};

}

#endif // WAVEMANAGEDOBJECTSYNCHRONOUSQUERYCONTEXTFORUPDATE_H
