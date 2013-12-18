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

/**
 *@file RelationshipInfoFromSchemaDifference.h 
 * Copyright (C) 2011 Brocade Communications Systems,Inc.
 * All rights reserved. 
 * Description: This file declares the class that stores the info regarding a relation
 *              It will be used in the context of storing the added/removed relationships
 *              for each table.
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       10/20/2011 
 */



#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"
#include "Framework/Utils/TraceUtils.h"
namespace WaveNs
{


RelationshipInfoFromSchemaDifference::RelationshipInfoFromSchemaDifference (const string & relationName, const string & relatedToTable, OrmRelationUmlType relationUmlType, OrmRelationType relationType, bool disableValidations, bool canBeEmpty)
  : m_relationName      (relationName),
    m_relatedToTable    (relatedToTable),
    m_relationUmlType   (relationUmlType),
    m_relationType      (relationType), 
    m_disableValidations (disableValidations),
    m_canBeEmpty         (canBeEmpty),
    m_changedRelationshipAttributes (0)
{
}

RelationshipInfoFromSchemaDifference::RelationshipInfoFromSchemaDifference (string relationName, string relationFieldType)
{
    m_relationName = relationName;

    vector<string> tokens;
    tokenize(relationFieldType, tokens, '-');

    if (tokens[0]=="a")
    {
        m_relationUmlType = ORM_RELATION_UML_TYPE_ASSOCIATION;
    }
    else if (tokens[0]=="c")
    {
        m_relationUmlType = ORM_RELATION_UML_TYPE_COMPOSITION;
    }
    else if (tokens[0]=="g")
    {
        m_relationUmlType = ORM_RELATION_UML_TYPE_AGGREGATION;
    }

    if (tokens[1]=="1")
    {
       m_relationType = ORM_RELATION_TYPE_ONE_TO_ONE;
    }
    else if (tokens[1]=="m")
    {
       m_relationType = ORM_RELATION_TYPE_ONE_TO_MANY;
    }

    vector<string>tkns;
    tokenize(tokens[2],tkns,'|');
    m_relatedToTable = tkns[0];

    if (tkns[2] == "0")
    {
        m_disableValidations = false;
    }
    else
    {
        m_disableValidations = true;
    }

    if (tkns[3] == "0")
    {
        m_canBeEmpty = false;
    }
    else
    {
        m_canBeEmpty = true;
    }

    m_changedRelationshipAttributes = 0;
}

const RelationshipInfoFromSchemaDifference &  RelationshipInfoFromSchemaDifference::operator= (const RelationshipInfoFromSchemaDifference & relationInfo)
{

  if(this != &relationInfo)
  {
      m_relationName = relationInfo.getRelationName ();
      m_relatedToTable =  relationInfo.getRelatedToTable ();
      m_relationUmlType = relationInfo.getRelationUmlType ();
      m_relationType = relationInfo.getRelationType ();
      m_disableValidations = relationInfo.getDisableValidations ();
      m_canBeEmpty = relationInfo.getCanBeEmpty ();
      m_changedRelationshipAttributes = relationInfo.getChangedRelationshipAttributes ();
  }
  
  return (*this);
}


RelationshipInfoFromSchemaDifference::RelationshipInfoFromSchemaDifference (const RelationshipInfoFromSchemaDifference & relationInfo)
{
    m_relationName = relationInfo.getRelationName ();
    m_relatedToTable =  relationInfo.getRelatedToTable ();
    m_relationUmlType = relationInfo.getRelationUmlType ();
    m_relationType = relationInfo.getRelationType ();
    m_disableValidations = relationInfo.getDisableValidations ();
    m_canBeEmpty = relationInfo.getCanBeEmpty ();
    m_changedRelationshipAttributes = relationInfo.getChangedRelationshipAttributes ();
}

RelationshipInfoFromSchemaDifference::~RelationshipInfoFromSchemaDifference ()
{
}

const string &    RelationshipInfoFromSchemaDifference::getRelationName () const
{
   return (m_relationName);  
}

const string &    RelationshipInfoFromSchemaDifference::getRelatedToTable () const
{
  return (m_relatedToTable);
}

OrmRelationUmlType RelationshipInfoFromSchemaDifference::getRelationUmlType () const
{
  return (m_relationUmlType);
}

OrmRelationType   RelationshipInfoFromSchemaDifference::getRelationType () const
{
  return (m_relationType);
}

bool RelationshipInfoFromSchemaDifference::getDisableValidations () const
{
  return (m_disableValidations);
}

bool RelationshipInfoFromSchemaDifference::getCanBeEmpty () const
{
  return (m_canBeEmpty);
}

UI8 RelationshipInfoFromSchemaDifference::getChangedRelationshipAttributes () const
{
  return (m_changedRelationshipAttributes);
}

bool RelationshipInfoFromSchemaDifference::isChangedRelationship () const
{
    if (0 == m_changedRelationshipAttributes)
    {
        return false;
    }
    return true;
}

void RelationshipInfoFromSchemaDifference::setCanBeEmptyChanged ()
{
    m_changedRelationshipAttributes = ((m_changedRelationshipAttributes) | 0x01);
}

bool RelationshipInfoFromSchemaDifference::isCanBeEmptyChanged () const
{
    if (0x01 == ((m_changedRelationshipAttributes) & 0x01))
    {
        return true;
    }
    return false;
}

void RelationshipInfoFromSchemaDifference::setDisableValidationChanged ()
{
    m_changedRelationshipAttributes = ((m_changedRelationshipAttributes) | 0x02);
}

bool RelationshipInfoFromSchemaDifference::isDisableValidationChanged () const
{
    if (0x02 == ((m_changedRelationshipAttributes) & 0x02))
    {
        return true;
    }
    return false;
}

void RelationshipInfoFromSchemaDifference::printContentsForDebugging ()
{
    if (isChangedRelationship ())
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, " Relation Name : %s is a changed Relationship\n",m_relationName.c_str());
        if (isCanBeEmptyChanged ())
        {
            tracePrintf (TRACE_LEVEL_INFO, false, false, " Can Be Empty is modified in this relation and it is now set to : %d\n", m_canBeEmpty);
        }

        if (isDisableValidationChanged ())
        {
            tracePrintf (TRACE_LEVEL_INFO, false, false, " Disable Validation is modified in this relation and it is now set to : %d\n", m_disableValidations);
        }
    }

    tracePrintf (TRACE_LEVEL_INFO, false, false, " Relation Name : %s\n",m_relationName.c_str());
    tracePrintf (TRACE_LEVEL_INFO, false, false, " Related To Table : %s\n",m_relatedToTable.c_str());

    if (m_relationUmlType == ORM_RELATION_UML_TYPE_ASSOCIATION)
    {
        trace (TRACE_LEVEL_INFO, " Relation UML Type : Association");
    }
    else if (m_relationUmlType == ORM_RELATION_UML_TYPE_AGGREGATION)
    {
        trace (TRACE_LEVEL_INFO, " Relation UML Type : Aggregation");
    }
    else if (m_relationUmlType == ORM_RELATION_UML_TYPE_COMPOSITION)
    {
        trace (TRACE_LEVEL_INFO, " Relation UML Type : Composition");
    }

    if (m_relationType == ORM_RELATION_TYPE_ONE_TO_ONE)
    {
        trace (TRACE_LEVEL_INFO, " Relation Type : One to One");
    }
    else if (m_relationType == ORM_RELATION_TYPE_ONE_TO_MANY)
    {
        trace (TRACE_LEVEL_INFO, " Relation Type : One to Many");
    }

    tracePrintf (TRACE_LEVEL_INFO, false, false, " Disable Validations is set to : %d\n",m_disableValidations);
    tracePrintf (TRACE_LEVEL_INFO, false, false, " Can Be Empty is set to : %d\n",m_canBeEmpty);

}

}

