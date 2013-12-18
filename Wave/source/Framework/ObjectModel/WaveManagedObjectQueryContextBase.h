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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 *            Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PRISMMANAGEDOBJECTQUERYCONTEXTBASE_H
#define PRISMMANAGEDOBJECTQUERYCONTEXTBASE_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Utils/DatabaseQueryCursor.h"
#include "WaveResourceIdEnums.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveConditionSetBuilder;

class WaveManagedObjectQueryContextBase
{
    private :
        string                                  getClassToQueryFor                      () const;
        string                                  getSql                                  (const bool &forCount = false);

        bool                                    isNTuple                                (const string &keyName) const;
        string                                  getNTupleFormatForOrderField            (const string &keyName, const bool &ascending) const;
        AttributeConditionOperator              getConditionOperatorTypeFromString      (const string &conditionOperator);
        AttributeConcatenationOperator          getConcatenationOperatorTypeFromString  (const string &concatenationOperator);
        void                                    handleSlotAndLocationFilters            ();


    protected :
        virtual void                            updateAssociationTableAndWhereClause    (string &sql);

    public :
                                                WaveManagedObjectQueryContextBase           (const string &classToQueryFor);
		virtual					               ~WaveManagedObjectQueryContextBase           ();

        virtual void                            addObjectId                                 (const ObjectId &objectId);
        virtual void                            addObjectIds                                (const vector<ObjectId> &objectIds);

        virtual void                            addOrAttribute                              (Attribute *pAttribute, const string &conditionOperator);
        virtual void                            addOrAttribute                              (Attribute *pAttribute, AttributeConditionOperator conditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                            addAndAttribute                             (Attribute *pAttribute, const string &conditionOperator);
        virtual void                            addAndAttribute                             (Attribute *pAttribute, AttributeConditionOperator conditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                            setAttributeObjectIdVectorAssociation       (AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation);
        virtual void                            setAttributeRangeForQuery                   (AttributeUI32Range *pAttributeUI32Range);
        virtual void                            addOrderField                               (const string &orderField, const bool &ascending = true);

        virtual vector<WaveManagedObject *>    *getPResults                                 () const;
        virtual void                            setPResults                                 (vector<WaveManagedObject *> *pResults);
        virtual void                            setSchemaToQuery                            (const string &schema);
        virtual string                          getSchemaToQuery                            () const;
        virtual void                            addNTupleFormatForOrderField                (const string &keyName, const char &tupleSeparator, const UI32 &numberOfTuples);
        virtual void                            addNTupleFormatForOrderField                (const string &keyName, const string &tupleSeparators, const UI32 &numberOfTuples);

        virtual void                            addSelectField                              (const string &selectField);
        virtual void                            addSelectFieldForObjectId                   (const string &selectField);
        virtual void                            addSelectFields                             (const vector<string> &selectFields);
        virtual vector<string>                  getSelectFields                             () const;
        virtual vector<string>                  getSelectFieldsInManagedObject              () const;
                bool                            isSelectFieldAlreadyAdded                   (const string &selectField) const;
                void                            addSelectFieldsForUserDefinedKeyCombination ();
        virtual bool                            getLoadOneToManyRelationships               () const;
        virtual void                            setLoadOneToManyRelationships               (const bool &loadOneToManyRelationships);
        virtual bool                            getLoadCompositions                         () const;
        virtual void                            setLoadCompositions                         (const bool loadCompositions);
        virtual void                            addLimitForQueryResults                     (UI32 limitForQueryResults, UI32 offsetForQueryResults = 0);

        virtual bool                            getFilterByConnectedNodes                   () const;
        virtual void                            setFilterByConnectedNodes                   (const bool filterByConnectedNodes = true);
        virtual bool                            getFilterByDisconnectedNodes                () const;
        virtual void                            setFilterByDisconnectedNodes                (const bool filterByDisconnectedNodes = true);

		virtual UI32                            getPageSizeForQueryResults                  ();

		/* Function setPageSizeForQueryResults() ignores limitForQueryResults with value 0 */
		virtual void                            setPageSizeForQueryResults                  (UI32 limitForQueryResults);
		virtual void                            setPageQueryContext                         (WaveObjectManager* callingOm);
		virtual bool                            isPageQueryContextSet                       ();
		virtual string                          getPageQuerySql                             ();
		virtual void                            moveOffsetOnSuccessfulQuery                 ();
		virtual string                          getSqlWithPagingEnabled                     (WaveObjectManager* callingOm);

        virtual void                            setSlotFilter                               (const UI32 &slotNumber);
        virtual UI32                            getSlotFilter                               () const ;

        virtual void                            setLocationIdFilter                         (const LocationId &locationId);
        virtual LocationId                      getLocationIdFilter                         () const ;

        virtual void                            createConditionSet                          (const string &conditionSetName);
        virtual void                            addAttributeToConditionSet                  (const string &conditionSetName, Attribute *pAttribute, const string &concatenationOperator, const string &conditionOperator);
        virtual void                            addAttributeToConditionSet                  (const string &conditionSetName, Attribute *pAttribute, AttributeConcatenationOperator concatenationOperator = WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, AttributeConditionOperator conditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                            combineConditionSets                        (const string &conditionSetCombination);

                void                            setSkipPropagatedPartitionFiltersFlag       (const bool &skipPropagatedPartitionFiltersFlag);
                bool                            getSkipPropagatedPartitionFiltersFlag       () const;
                void                            setPartitionFilter                          (const string &partitionName, const LocationId &partitionLocationId);
                void                            printSqlToDebugError                        ();

        virtual void                            filterByUserDefinedAttributes               (vector<Attribute *> userDefinedAttributes);
    // Now the data members

    private :
        string                                  m_classToQueryFor;
        vector<ObjectId>                        m_objectIds;
        vector<Attribute *>                     m_orAttributes;
        vector<AttributeConditionOperator>      m_orAttributeConditionOperators;
        vector<Attribute *>                     m_andAttributes;
        vector<AttributeConditionOperator>      m_andAttributeConditionOperators;
        AttributeObjectIdVectorAssociation     *m_pAttributeObjectIdVectorAssociation;
        AttributeUI32Range                     *m_pRangeAttribute;
        vector<string>                          m_orderFields;
        vector<bool>                            m_orderFieldsAscending;
        vector<WaveManagedObject *>            *m_pResults;
        string                                  m_schemaToQuery;
        map<string, string>                     m_nTupleSeparator;
        map<string, UI32>                       m_nTupleNumber;
        vector<string>                          m_selectFields;
        vector<string>                          m_selectFieldsInManagedObject;
        bool                                    m_loadOneToManyRelationships;
        bool                                    m_loadCompositions;
        UI32                                    m_limitForQueryResults;
        UI32                                    m_offsetForQueryResults;
        bool                                    m_filterByConnectedNodes;
        bool                                    m_filterByDisconnectedNodes;
        UI32                                    m_slotNumber;
        bool                                    m_isSlotNumberSet;
        LocationId                              m_locationId;
        bool                                    m_isLocationIdSet;
        bool                                    m_skipPropagatedPartitionFiltersFlag;
        vector<Attribute *>                     m_userDefinedAttributes;

    protected :
		UI32                                    m_pagedQuerySize;
		DatabaseQueryCursor                    *m_pagedQueryContext;
        WaveConditionSetBuilder                *m_pConditionSetBuilder;

    public :

    friend class WaveObjectManager;
    friend class DatabaseObjectManagerExecuteQueryWorker;
    friend class WaveManagedObjectSynchronousQueryContextForDeletion;
    friend class WaveManagedObjectSynchronousQueryContextForSetOperation;
    friend class WaveManagedObjectSynchronousInnerQueryContext;
};

}

#endif // PRISMMANAGEDOBJECTQUERYCONTEXTBASE_H
