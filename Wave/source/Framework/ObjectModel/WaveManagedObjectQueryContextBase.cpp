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

#include "Framework/ObjectModel/WaveManagedObjectQueryContextBase.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveConditionSetBuilder.h"
#include "Cluster/MultiPartition/Global/MultiPartitionObjectManager.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

#include <algorithm>

namespace WaveNs
{

WaveManagedObjectQueryContextBase::WaveManagedObjectQueryContextBase (const string &classToQueryFor)
    : m_classToQueryFor                     (classToQueryFor),
      m_pAttributeObjectIdVectorAssociation (NULL),
      m_pRangeAttribute                     (NULL),
      m_pResults                            (NULL),
      m_schemaToQuery                       (OrmRepository::getWaveCurrentSchema ()),
      m_loadOneToManyRelationships          (true),
      m_loadCompositions                    (true),
      m_limitForQueryResults                (0),
      m_filterByConnectedNodes              (false),
      m_filterByDisconnectedNodes           (false),
      m_slotNumber                          (0),
      m_isSlotNumberSet                     (false),
      m_locationId                          (0),
      m_isLocationIdSet                     (false),
      m_skipPropagatedPartitionFiltersFlag  (false),
      m_pagedQuerySize				        (0),
      m_pagedQueryContext                   (NULL),
      m_pConditionSetBuilder                (NULL)
{
}

WaveManagedObjectQueryContextBase::~WaveManagedObjectQueryContextBase ()
{
    UI32 i                     = 0;
    UI32 numberOfResults       = 0;
    UI32 numberOfOrAttributes  = m_orAttributes.size ();
    UI32 numberOfAndAttributes = m_andAttributes.size ();

    if (NULL != m_pResults)
    {
        numberOfResults = m_pResults->size ();

        for (i = 0; i < numberOfResults; i++)
        {
            delete ((*m_pResults)[i]);
        }

        m_pResults->clear ();
        delete m_pResults;
        m_pResults = NULL;
    }

    for (i = 0; i < numberOfOrAttributes; i++)
    {
        delete m_orAttributes[i];
    }

    for (i = 0; i < numberOfAndAttributes; i++)
    {
        delete m_andAttributes[i];
    }

    if (NULL != m_pAttributeObjectIdVectorAssociation)
    {
        delete m_pAttributeObjectIdVectorAssociation;
    }

    if (isPageQueryContextSet())
    {
    	delete m_pagedQueryContext;
    }	

    if (NULL != m_pRangeAttribute)
    {
        delete m_pRangeAttribute;
        m_pRangeAttribute = NULL;
    }

    if (NULL != m_pConditionSetBuilder)
    {
        delete m_pConditionSetBuilder;
        m_pConditionSetBuilder = NULL;
    }
}

string WaveManagedObjectQueryContextBase::getClassToQueryFor () const
{
    return (m_classToQueryFor);
}

void WaveManagedObjectQueryContextBase::handleSlotAndLocationFilters ()
{
    if ((m_isSlotNumberSet == true))
    {
        UI32 slotNumber = getSlotFilter ();
        ObjectId slotObjectId;

        if (m_isLocationIdSet == true)
        {
            LocationId locationId = getLocationIdFilter ();
            slotObjectId = FrameworkToolKit::getObjectIdForSlotNumber (slotNumber, locationId);
        }
        else 
        {
            slotObjectId = FrameworkToolKit::getObjectIdForSlotNumber (slotNumber);
        }
        
        addAndAttribute (new AttributeObjectId (slotObjectId, "waveSlotLocalManagedObjectObjectId"));
        m_isSlotNumberSet = false;
    }
    else if (true == m_isLocationIdSet) 
    {
        LocationId locationId = getLocationIdFilter ();
        ObjectId locationIdObjectId = FrameworkToolKit::getObjectIdForLocationId (locationId);
        addAndAttribute (new AttributeObjectId (locationIdObjectId, "ownerWaveNodeObjectId"));
        m_isLocationIdSet = false;
    }
}
string WaveManagedObjectQueryContextBase::getSql (const bool &forCount) 
{
    string   sql;

    handleSlotAndLocationFilters ();

    UI32     numberOfOrderFields   = m_orderFields.size   ();
    UI32     i                     = 0;
    string   tempString;
    UI32     numberOfSelectFields  = m_selectFields.size ();
    string   selectFieldString;

    sql = "SELECT ";

    if (NULL != m_pAttributeObjectIdVectorAssociation)
    {
        sql += "DISTINCT ";
    }

    if (0 == numberOfSelectFields)
    {
        if (true == forCount)
        {
            sql += "count (t1.*) ";
        }
        else
        {
            sql += "t1.* ";
        }
    }
    else
    {
        for (i = 0; i < numberOfSelectFields; i++)
        {
            selectFieldString += "t1." + m_selectFields[i];

            if (i < (numberOfSelectFields - 1))
            {
                selectFieldString += ", ";
            }
            else
            {
                selectFieldString += " ";
            }
        }

        sql += selectFieldString;
    }

    sql += "FROM " + m_schemaToQuery + "." + m_classToQueryFor + " t1";

    updateAssociationTableAndWhereClause(sql);

    if (0 < numberOfOrderFields)
    {
        if (false == forCount)
        {
            sql += " ORDER BY ";
        
            for (i = 0; i < numberOfOrderFields; i++)
            {
                sql += (true == isNTuple(m_orderFields[i]) ? getNTupleFormatForOrderField (m_orderFields[i], m_orderFieldsAscending[i]) : string ("t1.") + m_orderFields[i] + string (" ") + (true == m_orderFieldsAscending[i] ? "ASC" : "DESC"));

                if (i < (numberOfOrderFields - 1))
                {
                    sql += ",";
                }
            }
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("WaveManagedObjectQueryContextBase::getSql : Ignoring ORDER BY clause since query for count is enabled."));
        }
    }

    if (0 < m_limitForQueryResults)
    {
        sql += string (" LIMIT ") + m_limitForQueryResults; 

        if (0 < m_offsetForQueryResults)
        {
            sql += string (" OFFSET ") + m_offsetForQueryResults;
        }
    }

    sql += ";";

    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectQueryContextBase::getSql query: " + sql);

    return (sql);
}

void WaveManagedObjectQueryContextBase::updateAssociationTableAndWhereClause(string &sql)
{
    handleSlotAndLocationFilters ();

    UI32     numberOfObjectIds     = m_objectIds.size     ();
    UI32     numberOfOrAttributes  = m_orAttributes.size  ();
    UI32     numberOfAndAttributes = m_andAttributes.size ();
    UI32     numberOfUserDefinedAttributes = m_userDefinedAttributes.size ();
    UI32     i                     = 0;
    ObjectId tempObjectId;
    string   associationTableName;
    string   whereClause;

    if (NULL != m_pAttributeObjectIdVectorAssociation)
    {
        m_pAttributeObjectIdVectorAssociation->getSqlForSelect2 (associationTableName, whereClause);

        sql += ", " + m_schemaToQuery + "." + associationTableName + " t2";
    }

    // When connected/disconnect node filtering is enabled, we need to join the WaveNode table in the SQL
    // query statement so that WaveNode.specificStatus column can be used to determine if the node is
    // connected or disconnected from the management cluster.  The relationship between any LocalManagedObject
    // table and WaveNode table is through the OwnerWaveNodeObjectIdClassId and OwnerWaveNodeObjectIdInstanceId.

    if (true == m_filterByConnectedNodes || true == m_filterByDisconnectedNodes)
    {
        sql += ", " + m_schemaToQuery + "." + WaveNode::getClassName () + " tWN";
    }

    if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pRangeAttribute) || (NULL != m_pAttributeObjectIdVectorAssociation) || (true == m_filterByConnectedNodes) || (true == m_filterByDisconnectedNodes) || (NULL != m_pConditionSetBuilder) || (0 != numberOfUserDefinedAttributes))
    {
        sql += " WHERE ";
    }

    if (0 < numberOfObjectIds)
    {
        sql += " (";

        for (i = 0; i < numberOfObjectIds; i++)
        {
            tempObjectId = m_objectIds[i];

            sql += string ("(objectIdClassId = ") + tempObjectId.getClassId () + string (" AND objectIdInstanceId = ") + tempObjectId.getInstanceId () + string (")");

            if (i != (numberOfObjectIds - 1))
            {
                sql += " OR ";
            }
        }

        sql += ") ";
    }

    if (0 < numberOfOrAttributes)
    {
        if (0 < numberOfObjectIds)
        {
            sql += " AND ";
        }

        sql += " (";

        for (i = 0; i < numberOfOrAttributes; i++)
        {
            (m_orAttributes[i])->getSqlForSelect (sql, m_orAttributeConditionOperators[i]);

            if (i != (numberOfOrAttributes - 1))
            {
                sql += " OR ";
            }
        }

        sql += ")";
    }

    if (0 < numberOfAndAttributes)
    {
        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes))
        {
            sql += " AND ";
        }

        sql += " (";

        for (i = 0; i < numberOfAndAttributes; i++)
        {
            (m_andAttributes[i])->getSqlForSelect (sql, m_andAttributeConditionOperators[i]);

            if (i != (numberOfAndAttributes - 1))
            {
                sql += " AND ";
            }
        }

        sql += ")";
    }

    if (NULL != m_pRangeAttribute)
    {
        sql += " (";

        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes))
        {
            sql += " AND ";
        }

        m_pRangeAttribute->getSqlForSelectUsingRange (sql);

        sql += " )";
    }

    if (NULL != m_pAttributeObjectIdVectorAssociation)
    {
        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pRangeAttribute))
        {
            sql += " AND ";
        }

        sql += "(";

        sql += "(objectIdClassId = ownerClassId AND objectIdInstanceId = ownerInstanceId) AND";

        sql += " (" + whereClause + ")";

        sql += ")";
    }

    if (true == m_filterByConnectedNodes)
    {
        string                  primaryNodeConnectedStatus;
        string                  secondaryNodeConnectedStatus;
        string                  standAloneNodeConnectedStatus;

        AttributeResourceId     primaryConnectedAttribute       (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_PRIMARY);
        AttributeResourceId     secondaryConnectedAttribute     (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_CONNECTED_TO_CLUSTER);
        AttributeResourceId     standAloneConnectedAttribute    (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_STAND_ALONE);

        primaryConnectedAttribute.toString      (primaryNodeConnectedStatus);
        secondaryConnectedAttribute.toString    (secondaryNodeConnectedStatus);
        standAloneConnectedAttribute.toString   (standAloneNodeConnectedStatus);
        

        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pAttributeObjectIdVectorAssociation))
        {
            sql += " AND ";
        }

        sql += "(";

        sql += "(t1.ownerWaveNodeObjectIdClassId = tWN.objectIdClassId AND t1.ownerWaveNodeObjectIdInstanceId = tWN.objectIdInstanceId) AND";

        sql += string (" (tWN.specificStatus = '") + primaryNodeConnectedStatus + "' OR tWN.specificStatus = '" + secondaryNodeConnectedStatus + "' OR tWN.specificStatus = '" + standAloneNodeConnectedStatus + "')";

        sql += ")";
    }

    if (true == m_filterByDisconnectedNodes)
    {
        string                  primaryNodeConnectedStatus;
        string                  secondaryNodeConnectedStatus;

        AttributeResourceId     primaryConnectedAttribute       (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_PRIMARY);
        AttributeResourceId     secondaryConnectedAttribute     (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_CONNECTED_TO_CLUSTER);

        primaryConnectedAttribute.toString      (primaryNodeConnectedStatus);
        secondaryConnectedAttribute.toString    (secondaryNodeConnectedStatus);


        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pAttributeObjectIdVectorAssociation))
        {
            sql += " AND ";
        }

        sql += "(";

        sql += "(t1.ownerWaveNodeObjectIdClassId = tWN.objectIdClassId AND t1.ownerWaveNodeObjectIdInstanceId = tWN.objectIdInstanceId) AND";

        // All other states (including standalone, secondary add phases, and secondary rejoin phases) are considered to be disconnected from the cluster.
        
        sql += string (" (tWN.specificStatus <> '") + primaryNodeConnectedStatus + "' AND tWN.specificStatus <> '" + secondaryNodeConnectedStatus + "')";

        sql += ")";
    }

    if (0 != numberOfUserDefinedAttributes)
    {
        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pAttributeObjectIdVectorAssociation) || true == m_filterByConnectedNodes || true == m_filterByDisconnectedNodes)
        {
            sql += " AND ";
        }

        sql += "(";

        for (i = 0; i < numberOfUserDefinedAttributes; i++)
        {
            (m_userDefinedAttributes[i])->getSqlForSelect (sql, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

            if (i != (numberOfUserDefinedAttributes - 1))
            {
                sql += " AND ";
            }
        }

        sql += ")";
    }

    if (NULL != m_pConditionSetBuilder)
    {
        if ((0 < numberOfObjectIds) || (0 < numberOfOrAttributes) || (0 < numberOfAndAttributes) || (NULL != m_pAttributeObjectIdVectorAssociation) || true == m_filterByConnectedNodes || true == m_filterByDisconnectedNodes || 0 != numberOfUserDefinedAttributes)
        {
            sql += " AND ";
        }

        sql += "(";

        sql += m_pConditionSetBuilder->getSql ();

        sql += ")";
    }
}

void WaveManagedObjectQueryContextBase::addObjectId (const ObjectId &objectId)
{
    m_objectIds.push_back (objectId);
}

void WaveManagedObjectQueryContextBase::addObjectIds (const vector<ObjectId> &objectIds)
{
    UI32 numberOfObjectIds = objectIds.size ();
    UI32 i                 = 0;

    for (i = 0; i < numberOfObjectIds; i++)
    {
        addObjectId (objectIds[i]);
    }
}

void WaveManagedObjectQueryContextBase::addOrAttribute (Attribute *pAttribute, const string &conditionOperator)
{
    AttributeConditionOperator attributeConditionOperator = getConditionOperatorTypeFromString (conditionOperator);

    addOrAttribute (pAttribute, attributeConditionOperator);
}

void WaveManagedObjectQueryContextBase::addOrAttribute (Attribute *pAttribute, AttributeConditionOperator conditionOperator)
{
    m_orAttributes.push_back (pAttribute);
    m_orAttributeConditionOperators.push_back (conditionOperator);
}

void WaveManagedObjectQueryContextBase::addAndAttribute (Attribute *pAttribute, const string &conditionOperator)
{
    AttributeConditionOperator attributeConditionOperator = getConditionOperatorTypeFromString (conditionOperator);

    addAndAttribute (pAttribute, attributeConditionOperator);
}

void WaveManagedObjectQueryContextBase::addAndAttribute (Attribute *pAttribute, AttributeConditionOperator conditionOperator)
{
    m_andAttributes.push_back (pAttribute);
    m_andAttributeConditionOperators.push_back (conditionOperator);
}

void WaveManagedObjectQueryContextBase::filterByUserDefinedAttributes (vector<Attribute *> userDefinedAttributes)
{
    map<string, Attribute*>       udkcMap; // Map of user defined key names to its Attribute type
    vector <string>               userDefinedKeyNames;
    udkcMap = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (getClassToQueryFor (), userDefinedKeyNames);

    if (userDefinedKeyNames.size () != userDefinedAttributes.size ())
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::filterByUserDefinedAttributes : Count of User Defined Attributes should be same as number of user Defined keys defined in MO.");
        prismAssert (false, __FILE__, __LINE__);
    }

    bool found;
    UI32 keyNameSize = userDefinedKeyNames.size (); 
    UI32 attributesSize = userDefinedAttributes.size (); 
    UI32 i              = 0;
    UI32 j              = 0;
    for (i = 0 ; i < keyNameSize ; i++)
    {
        found = false;
        //Check if userDefinedKeyNames[i] is present in userDefinedAttributes
        for (j = 0 ; j < attributesSize; j++)
        {
            if (userDefinedKeyNames[i] == userDefinedAttributes[j]->getAttributeName ())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::filterByUserDefinedAttributes : User Defined key " + userDefinedKeyNames[i] + "is missing in the attribute list.");
            prismAssert (false, __FILE__, __LINE__);
        }
    }


    m_userDefinedAttributes = userDefinedAttributes;
}

void WaveManagedObjectQueryContextBase::setAttributeObjectIdVectorAssociation (AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation)
{
    if (NULL != m_pAttributeObjectIdVectorAssociation)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setAttributeObjectIdVectorAssociation : Already set.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_pAttributeObjectIdVectorAssociation = pAttributeObjectIdVectorAssociation;
}

void WaveManagedObjectQueryContextBase::setAttributeRangeForQuery (AttributeUI32Range *pAttributeUI32Range)
{
    m_pRangeAttribute = pAttributeUI32Range;
}

vector<WaveManagedObject *> *WaveManagedObjectQueryContextBase::getPResults () const
{
    return (m_pResults);
}

void WaveManagedObjectQueryContextBase::setPResults (vector<WaveManagedObject *> *pResults)
{
    m_pResults = pResults;
}

void WaveManagedObjectQueryContextBase::setSchemaToQuery (const string &schema)
{
    m_schemaToQuery = schema;
}

string WaveManagedObjectQueryContextBase::getSchemaToQuery () const
{
    return (m_schemaToQuery);
}

void WaveManagedObjectQueryContextBase::addOrderField (const string &orderField, const bool &ascending)
{
    m_orderFields.push_back           (orderField);
    m_orderFieldsAscending.push_back  (ascending);
}

/**
 * Name:        getNTupleFormatForOrderField
 * Description: Helper function to generate an ORDER BY SQL regular expression to sort n-tuple keys.
 * 
 * @param  keyName : name of the key which contains n-tuple values. n-tuple values are 'n' number of one or more digits separated by a tuple delimiter.
 * @param  ascending : if true, then n-tuple sorting is in acending order, else n-tuple soring is in decending order.
 *
 * @return string : An ORDER BY clause SQL regular expression to sort n-tuple keys.
 */
string WaveManagedObjectQueryContextBase::getNTupleFormatForOrderField (const string &keyName, const bool &ascending) const
{
    UI32                                numberOfTuples;
    string                              nTupleSeparator;
    string                              nTupleRegExpString      = "\\\\d";
    vector<string>                      nTupleOrderFieldRegExp;
    UI32                                i                       = 0;
    UI32                                j                       = 0;
    string                              sql;
    
    // Verify key is found in m_nTupleNumber and m_nTupleSeparator maps

    if (false == isNTuple(keyName))
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::getNTupleFormateForOrderField : Key '" + keyName + "' has not been defined as an n-tuple format");
        prismAssert (false, __FILE__, __LINE__);
    }

    numberOfTuples = m_nTupleNumber.find(keyName)->second;
    nTupleSeparator = string("[") + (m_nTupleSeparator.find(keyName)->second) + string ("]");

    // Build the regular expression for nTuples

    for (i = 0; i < numberOfTuples; i++)
    {
        string         tempRegExp;

        // construct regular expression from temp n-tuples

        for (j = 0; j < numberOfTuples; j++)
        {
            if (0 == j)
            {
                tempRegExp += ("^" + nTupleRegExpString + "+");
            }
            else if ((numberOfTuples - 1) == j)
            {   
                tempRegExp += (nTupleSeparator + "*" + nTupleRegExpString + "*$");
            }
            else if (j == i)
            {
                // update temp nth tuple for substring return value

                tempRegExp += (nTupleSeparator + "+" + "(" + nTupleRegExpString + "+)");
            }
            else if (j > i)
            {   
                tempRegExp += (nTupleSeparator + "*" + nTupleRegExpString + "*");
            }
            else
            {
                tempRegExp += (nTupleSeparator + "+" + nTupleRegExpString + "+");
            }

            // handle start and end reg exp uniqely

            if (0 == j  && i == j)
            {
                tempRegExp = ("^(" + nTupleRegExpString + "+)");
                break;
            }
            else if ((numberOfTuples - 1) == j && i == j)
            {
                tempRegExp = ("(" + nTupleRegExpString + "+)$");
            }
        }

        nTupleOrderFieldRegExp.push_back (tempRegExp);
    }

    // finally construct n-tuple order field

    for (i = 0; i < numberOfTuples; i++)
    {
        sql += "substring(" + keyName + " from E'" + nTupleOrderFieldRegExp[i] + "')::integer " + (true == ascending ? "ASC" : "DESC");

        if (i < (numberOfTuples - 1))
        {
            sql += ",";
        }
    }

    return (sql);
}

bool WaveManagedObjectQueryContextBase::isNTuple (const string &keyName) const
{
    bool isTupleNumberFound = false;
    bool isTupleSeparatorFound = false;

    if (m_nTupleNumber.end () != m_nTupleNumber.find(keyName))
    {
        isTupleNumberFound = true; 
    }
    else
    {   
        isTupleNumberFound = false;
    }
  
    if (m_nTupleSeparator.end () != m_nTupleSeparator.find(keyName))
    {
        isTupleSeparatorFound = true;
    }
    else
    {
        isTupleSeparatorFound = false;
    }

    if ((true == isTupleNumberFound) && (true == isTupleSeparatorFound))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

AttributeConditionOperator WaveManagedObjectQueryContextBase::getConditionOperatorTypeFromString (const string &conditionOperator)
{
    AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL;

    if ("=" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL;
    }
    else if ("<>" == conditionOperator || "!=" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL;
    }
    else if (">" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN;
    }
    else if (">=" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO;
    }
    else if ("<" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN;
    }
    else if ("<=" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN_OR_EQUAL_TO;
    }
    else if ("LIKE" == conditionOperator || "like" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE;
    }
    else if ("NOT LIKE" == conditionOperator || "not like" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE;
    }
    else if ("ILIKE" == conditionOperator || "ilike" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE;
    }
    else if ("NOT ILIKE" == conditionOperator || "not ilike" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE;
    }
    else if ("BETWEEN" == conditionOperator || "between" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_BETWEEN;
    }
    else if ("NOT BETWEEN" == conditionOperator || "not between" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_BETWEEN;
    }
    else if ("IN" == conditionOperator || "in" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN;
    }
    else if ("NOT IN" == conditionOperator || "not in" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_IN;
    }
    else if ("SIMILAR TO" == conditionOperator || "similar to" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_SIMILAR_TO;
    }
    else if ("NOT SIMILAR TO" == conditionOperator || "not similar to" == conditionOperator)
    {
        attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_SIMILAR_TO;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::getConditionOperatorTypeFromString : Invalid or unsupported SQL condition operator specified: " + conditionOperator);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (attributeConditionOperator);
}

AttributeConcatenationOperator WaveManagedObjectQueryContextBase::getConcatenationOperatorTypeFromString (const string &concatenationOperator)
{
    AttributeConcatenationOperator attributeConcatenationOperator = WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND;

    if ("AND" == concatenationOperator || "and" == concatenationOperator)
    {
        attributeConcatenationOperator = WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND;
    }
    else if ("OR" == concatenationOperator || "or" == concatenationOperator)
    {
        attributeConcatenationOperator = WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::getConcatenationOperatorTypeFromString : Invalid SQL concatenation operator specified: " + concatenationOperator);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (attributeConcatenationOperator);
}

void WaveManagedObjectQueryContextBase::addNTupleFormatForOrderField (const string &keyName, const char &tupleSeparator, const UI32 &numberOfTuples)
{
    char tupleSeparatorChar[2] = {tupleSeparator};

    addNTupleFormatForOrderField (keyName, string (tupleSeparatorChar), numberOfTuples);
}

/**
 * Name:        addNTupleFormatForOrderField
 * Description: Method to define sorting rules to a n-tuple field to produce query results in a desired n-tuple numeric order.  
 * 
 * @param  keyName : name of the key which contains n-tuple values. n-tuple values are 'n' number of one or more digits separated by a tuple delimiter.
 * @param  tupleSeparators : One ore more tuple delimiters that separate digits.  Tuple delimiters will be matched as a character expression.
 * @param  numberOfTuples : Maximum number of digit groups in the tuple.  Smaller number of digit groups will still be sorted properly when a higher number is given.
 *
 * @return void
 */
void WaveManagedObjectQueryContextBase::addNTupleFormatForOrderField (const string &keyName, const string &tupleSeparators, const UI32 &numberOfTuples)
{
    if (0 < numberOfTuples)
    {
        m_nTupleSeparator[keyName] = tupleSeparators;
        m_nTupleNumber[keyName] = numberOfTuples;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectQueryContextBase::addNTupleFormatForOrderField : numberOfTuples must be greater than zero");
    }
}

bool WaveManagedObjectQueryContextBase::isSelectFieldAlreadyAdded (const string &selectField) const
{
    bool isAlreadyPresent = false;

    vector<string>::const_iterator element = find (m_selectFieldsInManagedObject.begin (), m_selectFieldsInManagedObject.end (), selectField);

    if (m_selectFieldsInManagedObject.end () != element)
    {
        isAlreadyPresent = true;
    }
    else
    {
        isAlreadyPresent = false;
    }

    return (isAlreadyPresent);
}

void WaveManagedObjectQueryContextBase::addSelectFieldsForUserDefinedKeyCombination ()
{
    vector<string>  keyNames;

    OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (m_classToQueryFor, keyNames);

    addSelectFields (keyNames);
}

void WaveManagedObjectQueryContextBase::addSelectField (const string& selectField)
{
    if (0 == (m_selectFields.size ()))
    {
        m_selectFields.push_back ("objectIdClassId");
        m_selectFields.push_back ("objectIdInstanceId");
        m_selectFieldsInManagedObject.push_back ("objectId");

        addSelectFieldsForUserDefinedKeyCombination ();
    }

    if (true == (isSelectFieldAlreadyAdded (selectField)))
    {
        return;
    }

    bool            isAColumn       = false;
    bool            isARelation     = false;
    OrmRelationType ormRelationType = ORM_RELATION_TYPE_ONE_TO_ONE;

    OrmRepository::getEntityDetails (m_classToQueryFor, selectField, isAColumn, isARelation, ormRelationType);

    if (true == isAColumn)
    {
    m_selectFields.push_back (selectField);
    m_selectFieldsInManagedObject.push_back (selectField);
}
    else if (true == isARelation)
    {
        if (ORM_RELATION_TYPE_ONE_TO_ONE == ormRelationType)
        {
            addSelectFieldForObjectId (selectField);
        }
        else
        {
            m_selectFieldsInManagedObject.push_back (selectField);
        }
    }
    else
    {
        const bool isASimpleObjectIdColumn = OrmRepository::isASimpleObjectIdColumn (m_classToQueryFor, selectField);

        if (true == isASimpleObjectIdColumn)
        {
            addSelectFieldForObjectId (selectField);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void WaveManagedObjectQueryContextBase::addSelectFieldForObjectId (const string& selectField)
{
    if (0 == (m_selectFields.size ()))
    {
        m_selectFields.push_back ("objectIdClassId");
        m_selectFields.push_back ("objectIdInstanceId");
        m_selectFieldsInManagedObject.push_back ("objectId");

        addSelectFieldsForUserDefinedKeyCombination ();
    }

    if (true == (isSelectFieldAlreadyAdded (selectField)))
    {
        return;
    }

    m_selectFields.push_back (selectField + "ClassId");
    m_selectFields.push_back (selectField + "InstanceId");
    m_selectFieldsInManagedObject.push_back (selectField);
}

void WaveManagedObjectQueryContextBase::addSelectFields (const vector<string> &selectFields)
{
    const UI32 numberOfSelectFields = selectFields.size ();
          UI32 i                    = 0;

    for (i = 0; i < numberOfSelectFields; i++)
    {
        addSelectField (selectFields[i]);
    }
}

vector<string> WaveManagedObjectQueryContextBase::getSelectFields () const
{
    return (m_selectFields);
}

vector<string> WaveManagedObjectQueryContextBase::getSelectFieldsInManagedObject () const
{
    return (m_selectFieldsInManagedObject);
}

bool WaveManagedObjectQueryContextBase::getLoadOneToManyRelationships () const
{
    return (m_loadOneToManyRelationships);
}

void WaveManagedObjectQueryContextBase::setLoadOneToManyRelationships (const bool &loadOneToManyRelationships)
{
    m_loadOneToManyRelationships = loadOneToManyRelationships;
}

bool WaveManagedObjectQueryContextBase::getLoadCompositions () const
{
    return (m_loadCompositions);
}

void WaveManagedObjectQueryContextBase::setLoadCompositions (const bool loadCompositions)
{
    m_loadCompositions = loadCompositions;
}

void WaveManagedObjectQueryContextBase::addLimitForQueryResults (UI32 limitForQueryResults, UI32 offsetForQueryResults)
{
    m_limitForQueryResults = limitForQueryResults;
    m_offsetForQueryResults = offsetForQueryResults;
}

bool WaveManagedObjectQueryContextBase::getFilterByConnectedNodes () const
{
    return (m_filterByConnectedNodes);
}

/**
 * Name:        setFilterByConnectedNodes
 * Description: Enable or disable filtering by connected nodes for this query context's 
 *              managed object.  Filtering by connected nodes should only be done on 
 *              local managed objects and should not be enabled along with filtering by 
 *              disconnected nodes.
 * 
 * @param  filterByConnectedNodes : boolean to enable or disable the connected     
 *                                  node filtering.  
 * @return void
 */
void WaveManagedObjectQueryContextBase::setFilterByConnectedNodes (const bool filterByConnectedNodes)
{
    if (false == OrmRepository::isALocalManagedObject (m_classToQueryFor))
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setFilterByConnectedNodes : Filtering by connected nodes should only be applied when querying for a local managed object.  " + m_classToQueryFor + " is detected to be a global managed object.");

        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == m_filterByDisconnectedNodes && true == filterByConnectedNodes)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setFilterByConnectedNodes : Filtering by connected nodes and filtering by disconnected nodes are mutually exclusive. Enabling both should not be done.");

        prismAssert (false, __FILE__, __LINE__);
    }

    m_filterByConnectedNodes = filterByConnectedNodes;
}

bool WaveManagedObjectQueryContextBase::getFilterByDisconnectedNodes () const
{
    return (m_filterByDisconnectedNodes);
}

/**
 * Name:        setFilterByDisconnectedNodes
 * Description: Enable or disable filtering by disconnected nodes for this query context's 
 *              managed object.  Filtering by disconnected nodes should only be done on 
 *              local managed objects and should not be enabled along with filtering by 
 *              connected nodes.
 * 
 * @param  filterByDisconnectedNodes : boolean to enable or disable the disconnected
 *                                     node filtering.
 * @return void
 */
void WaveManagedObjectQueryContextBase::setFilterByDisconnectedNodes (const bool filterByDisconnectedNodes)
{
    if (false == OrmRepository::isALocalManagedObject (m_classToQueryFor))
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setFilterByDisconnectedNodes : Filtering by disconnected nodes should only be applied when querying for a local managed object.  " + m_classToQueryFor + " is detected to be a global managed object.");

        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == m_filterByConnectedNodes && true == filterByDisconnectedNodes)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setFilterByDisconnectedNodes : Filtering by disconnected nodes and filtering by connected nodes are mutually exclusive. Enabling both should not be done.");

        prismAssert (false, __FILE__, __LINE__);
    }

    m_filterByDisconnectedNodes = filterByDisconnectedNodes;
}

UI32 WaveManagedObjectQueryContextBase::getPageSizeForQueryResults ()
{

	if (isPageQueryContextSet()) 
	{

		if ( m_pagedQuerySize != m_pagedQueryContext->getPageSize())
		{
			trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::getPageSizeForQueryResults : Paged query size is not in sync.");
			prismAssert (false, __FILE__, __LINE__);
		}
	}

	return m_pagedQuerySize;
}


/*
  *  If limitForQueryResults = 0 then it is ignored.
  *  Todo:- In future 0 (limitForQueryResults = 0) can be used to return all the remaining results of query.
  *  Need to revisit 0 case.
*/
void WaveManagedObjectQueryContextBase::setPageSizeForQueryResults (UI32 limitForQueryResults)
{

	if (0 == limitForQueryResults)
	{
	        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectQueryContextBase::setPageSizeForQueryResults : Setting to zero is not supported now");
		return ;
	}

	m_pagedQuerySize = limitForQueryResults;

	if (isPageQueryContextSet())
	{
		m_pagedQueryContext->setPageSize(limitForQueryResults);
	}

}

void WaveManagedObjectQueryContextBase::setPageQueryContext (WaveObjectManager* callingOm)
{
	if (!isPageQueryContextSet())
	{
		m_pagedQueryContext = new DatabaseQueryCursor(callingOm, m_classToQueryFor, getSql(), getPageSizeForQueryResults());
	} else
	{
		//Assert here;
		trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::setPageQueryContext : PageQueryContext is already set.");
		prismAssert (false, __FILE__, __LINE__);
	}

}

bool WaveManagedObjectQueryContextBase::isPageQueryContextSet ()
{
	if (NULL == m_pagedQueryContext)
	{
		return false;
	} else
	{
		return true;
	}

}


string WaveManagedObjectQueryContextBase::getPageQuerySql ()
{
	if (!isPageQueryContextSet())
	{
		//Assert here;
		trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::getPageQuerySql : PageQueryContext is not set.");
		prismAssert (false, __FILE__, __LINE__);
	}
	return m_pagedQueryContext->getSqlOfFetchCursor();
}


string WaveManagedObjectQueryContextBase::getSqlWithPagingEnabled (WaveObjectManager* callingOm)
{
    string sqlString = getSql();
    
    if (getPageSizeForQueryResults())
    {
    	if (!isPageQueryContextSet())
    	{
    		setPageQueryContext(callingOm);
    	}
    	sqlString = getPageQuerySql();
    }
    return sqlString;
}


void WaveManagedObjectQueryContextBase::moveOffsetOnSuccessfulQuery ()
{
	if (isPageQueryContextSet())
	{
		m_pagedQueryContext->setStartOffset(
			m_pagedQueryContext->getStartOffset() + 
			m_pagedQueryContext->getPageSize());
	} 
}
        
UI32 WaveManagedObjectQueryContextBase::getSlotFilter () const
{
    return (m_slotNumber);
}

void WaveManagedObjectQueryContextBase::setSlotFilter (const UI32 &slotNumber)
{
    m_slotNumber = slotNumber;
    m_isSlotNumberSet = true;
}

void WaveManagedObjectQueryContextBase::setLocationIdFilter (const LocationId &locationId)
{
    m_locationId = locationId;
    m_isLocationIdSet = true;
}
        
LocationId WaveManagedObjectQueryContextBase::getLocationIdFilter () const
{
    return (m_locationId);
}

/**
 *Name:         createConditionSet
 *Description:  A condition set contains one or more attribute comparisons that map into the SQL WHERE clause.
 *
 * @param       conditionSetName :  string label of the user-defined condition set.
 * @return      void
 */
void WaveManagedObjectQueryContextBase::createConditionSet (const string &conditionSetName)
{
    // Instantiate the builder if one hasn't been created yet.

    if (NULL == m_pConditionSetBuilder)
    {
        m_pConditionSetBuilder = new WaveConditionSetBuilder ();
    }

    prismAssert (NULL != m_pConditionSetBuilder, __FILE__, __LINE__);

    m_pConditionSetBuilder->createConditionSet (conditionSetName);
}

void WaveManagedObjectQueryContextBase::addAttributeToConditionSet (const string &conditionSetName, Attribute *pAttribute, const string &concatenationOperator, const string &conditionOperator)
{
    AttributeConcatenationOperator  attributeConcatenationOperator  = getConcatenationOperatorTypeFromString (concatenationOperator);
    AttributeConditionOperator      attributeConditionOperator      = getConditionOperatorTypeFromString     (conditionOperator);

    addAttributeToConditionSet (conditionSetName, pAttribute, attributeConcatenationOperator, attributeConditionOperator);
}

void WaveManagedObjectQueryContextBase::addAttributeToConditionSet (const string &conditionSetName, Attribute *pAttribute, AttributeConcatenationOperator concatenationOperator, AttributeConditionOperator conditionOperator)
{
    // Ensure condition set builder has been instantiated.

    if (NULL == m_pConditionSetBuilder)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::addAttributeToConditionSet : No condition sets have been created.  Please create a condition set before invoking this API.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_pConditionSetBuilder->addAttributeToConditionSet (conditionSetName, pAttribute, concatenationOperator, conditionOperator);
}

void WaveManagedObjectQueryContextBase::combineConditionSets (const string &conditionSetCombination)
{
    // Ensure condition set builder has been instantiated.

    if (NULL == m_pConditionSetBuilder)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectQueryContextBase::combineConditionSets : No condition sets have been created.  Please create a condition set before invoking this API.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_pConditionSetBuilder->combineConditionSets (conditionSetCombination);
}

void WaveManagedObjectQueryContextBase::setSkipPropagatedPartitionFiltersFlag (const bool &skipPropagatedPartitionFiltersFlag)
{
    m_skipPropagatedPartitionFiltersFlag = skipPropagatedPartitionFiltersFlag;
}

bool WaveManagedObjectQueryContextBase::getSkipPropagatedPartitionFiltersFlag () const
{
    return m_skipPropagatedPartitionFiltersFlag;
}

void WaveManagedObjectQueryContextBase::setPartitionFilter (const string &partitionName, const LocationId &partitionLocationId)
{
    WaveObjectManager *pWaveObjectManager = PrismThread::getWaveObjectManagerForCurrentThread ();
    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    // Object Id for partition is obtained by sending message to MultiPartitionObjectManager.
    // Object Id for location  is obtained by sending message to ClusterLocalObjectManager.
    // In both these object managers, any 'automatic filters for propagated partition-location information' are not required.
    // (If automatic filter is attempted in these object Managers, they send synchronous message to self, causing hang.

    if (pWaveObjectManager->getServiceId () == MultiPartitionObjectManager::getPrismServiceId ())
    {
        return;
    }

    if (pWaveObjectManager->getServiceId () == ClusterLocalObjectManager::getPrismServiceId ())
    {
        return;
    }

    if (true == getSkipPropagatedPartitionFiltersFlag ())
    {
        return;
    }

    if (false == OrmRepository::isALocalManagedObject (getClassToQueryFor ()))
    {
        return;
    }

    // 1. Location id filter should be set.

    if (true == m_isLocationIdSet)
    {
        LocationId locationIdSetByUser = getLocationIdFilter ();

        if (partitionLocationId != locationIdSetByUser)
        {
            trace (TRACE_LEVEL_WARN, string ("WaveManagedObjectQueryContextBase::setPartitionFilterWithPropagatedContextIfRequired : partitionLocationId [") + partitionLocationId + string ("], locationIdSetByUser [") + locationIdSetByUser + string ("]"));
            return;
        }
    }
    else
    {
        setLocationIdFilter (partitionLocationId);
    }

    // 2. Set Partition filter.

    ObjectId partitionObjectId;

    if (false == partitionName.empty ())
    {
        partitionObjectId = FrameworkToolKit::getObjectIdForPartition (partitionName);
    }
    else
    {
        // partitionObjectId is NullObjectId.
        // i.e. Get objects which are not in any partition. (i.e. objects outside of all partitions.)
    }

    addAndAttribute (new AttributeObjectId (partitionObjectId, "ownerPartitionManagedObjectId"));
}

void WaveManagedObjectQueryContextBase::printSqlToDebugError ()
{
    trace (TRACE_LEVEL_ERROR, string ("SQL [ ") + getSql () + string (" ]"));
}

}
