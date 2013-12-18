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

#ifndef ATTRIBUTESQLEXISTS_H
#define ATTRIBUTESQLEXISTS_H

#include "Framework/Attributes/Attribute.h"
#include "WaveResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousInnerQueryContext.h"

namespace WaveNs
{

class AttributeSqlExists : public Attribute
{
    private :
        virtual bool                        validate                                () const;
        virtual bool                        isConditionOperatorSupported            (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                            // This Attribute can be added in Where condition of Outer query context only if the outer context is Synchronous
                                            AttributeSqlExists                      (WaveManagedObjectSynchronousInnerQueryContext *pWaveManagedObjectSynchronousInnerQueryContext);

        virtual                            ~AttributeSqlExists                      ();
                AttributeSqlExists         &operator =                              (const AttributeSqlExists &attribute);
        virtual string                      getSqlType                              ();
        virtual void                        setupOrm                                (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate                         ();
        virtual void                        getSqlForInsert                         (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate                         (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect                         (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                                (string &valueString);
        virtual void                        fromString                              (const string &valueString);
        virtual void                       *getPData                                ();
        virtual void                        setValue                                (const void *pData);
        virtual Attribute                  *clone                                   ();

        virtual void                        loadFromPostgresQueryResult             (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        virtual void                        loadFromPostgresAuxilliaryQueryResult   (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);

        virtual void                        updateOrmRelations                      (const PrismPersistableObject *pPrismPersistableObject);
        virtual WaveManagedObject          *getComposedManagedObject                (const ObjectId &childObjectId);
        virtual bool                        isDeletableForOperation                 (const WaveManagedObjectOperation &operation);
        virtual void                        updateKeyString                         (const PrismPersistableObject *pPrismPersistableObject);
        virtual void                        getRestRowData                          (string &restRowData);
        static  map<string, string>         getSupportedConversions                 ();
        virtual void                        setDefaultValue                         ();
        virtual void                        getCValue                               (WaveCValue *pCValue);
                bool                        isCurrentValueSameAsDefault             () const;
    // Now the data members

    private :
                WaveManagedObjectSynchronousInnerQueryContext *m_pWaveManagedObjectSynchronousInnerQueryContext;

    protected :
    public :
};

}

#endif // ATTRIBUTESQLEXISTS_H

