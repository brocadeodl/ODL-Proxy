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
 *   Copyright (C) 2005 - 2010 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ATTRIBUTESMAP_H
#define ATTRIBUTESMAP_H

#include "Framework/Attributes/Attributes.h"
#include <map>
//#include <xercesc/dom/DOM.hpp>

#include "libpq-fe.h"

using namespace std;
//using namespace XERCES_CPP_NAMESPACE;

namespace WaveNs
{

class OrmTable;
class WaveManagedObject;
class PrismPersistableObject;
class WaveManagedObjectOperation;

class AttributesMap
{
    private :
        void                loadAnAttributeFromPostgresQueryResult2 (Attribute *pAttribute, PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager);

    protected :
    public :
                           AttributesMap                         ();
                          ~AttributesMap                         ();
        void               copyFrom                              (const AttributesMap &attributesMap);
        void               addAttribute                          (Attribute *pAttribute);
        UI32               getSize                               ();
        Attribute         *getAttribute                          (UI32 attributeTag);
        Attribute         *getAttributeAt                        (const UI32 index);
//        void               serializeTo                         (DOMDocument *pDomDocument);
        void               serializeTo                           (string &serializedData);
        void               serializeToInAttributeOrderFormat     (string &serializedData);
        void               loadFromSerializedData                (const string &serializedData);
        void               loadFromSerializedDataInAttributeOrderFormat    (const string &serializedData);

        string             getSqlForCreate                       ();
        void               getSqlForInsert                       (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2);
        void               getSqlForUpdate                       (string &sqlForUpdate, string &sqlForUpdate2);
        void               getSqlForUpdate                       (string &sqlForUpdate, string &sqlForUpdate2, const vector<string> &attributeNames);

        void               setupOrm                              (OrmTable *pOrmTable);

        void               loadFromPostgresQueryResult           (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager = NULL, const bool loadOneToManyRelationships = true, const bool loadCompositions = true);
        void               loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager = NULL, const bool loadCompositions = true);

        void               loadFromPostgresQueryResult2          (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager);

        bool               isAKnownAttributeByName               (const string &attributeName);
        void               computeAttributesByName               ();
        Attribute         *getAttribute                          (const string &attributeName);
        bool               isAKnownAttributeByUserTag            (const UI32 &attributeUserTag);
        Attribute         *getAttributeByUserTag                 (const UI32 &attributeUserTag);
        void               setUserTagForAttribute                (const string &attributeName, const UI32 &attributeUserTag);

        void               updateOrmRelations                    (const PrismPersistableObject *pPrismPersistableObject);
        WaveManagedObject *getComposedManagedObject              (const ObjectId &childObjectId);

        void               getShowDump                           (string &showDump, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getRawShowDump                        (string &showDump, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        bool               isDeletableForOperation               (const WaveManagedObjectOperation &operation);

        void               getAttributeNamesForHtmlTable         (      vector<string> &attributeNamesForHtmlTable,                                                                                                                                 const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getHtmlTableHeaderNames               (const vector<string> &attributeNamesForHtmlTable,                                                                                     vector<string> &htmlTableHeaderNames,       const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getHtmlTableHeaderNamesString         (const vector<string> &attributeNamesForHtmlTable,                                                                                     string         &htmlTableHeaderNamesString, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getHtmlTableRowData                   (const vector<string> &attributeNamesForHtmlTable,                                                                                     vector<string> &htmlTableRowData,           const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getHtmlTableRowDataString             (const vector<string> &attributeNamesForHtmlTable,                                                                                     string         &htmlTableRowDataString,     const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);

        void               getRestRowData                        (const vector<string> &attributeNamesForRest,      const vector<string> &restHeaderNames, const string &managedObjectClassNameForRest, string         &restRowData,                const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);

        void               getAttributeNamesForJson              (      vector<string> &attributeNamesForJson,                                                                                                                                      const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        void               getJsonNames                          (const vector<string> &attributeNamesForJson,                                                                                          vector<string> &jsonNames);

        void               getJsonObjectData                     (const vector<string> &attributeNamesForJson,      const vector<string> &jsonHeaderNames, const string &managedObjectClassNameForJson, string         &jsonObjectData);

        void               updateKeyString                       (const PrismPersistableObject *pPrismPersistableObject);
        void               getAllAttributeNames                  (vector<string> &allAttributeNames) const;

        void               getOidsOfOneToOneCompositions                    (vector<ObjectId> &vectorOfCompositionOids);
        void               popOneToOneCompositionsFromResults               (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject);
        void               storeRelatedObjectIdVectorForAOneToNAssociation  (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds);
        void               storeRelatedObjectVectorForAOneToNComposition    (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects);

        void               addAttributeForOrderToNameMapping                (const string& attributeName);
        UI32               getAttributeNameForOrderFromMapping              (const UI32 &attributeOrder);
 

    // Now the data members

    private :
        map<UI32, Attribute *>   m_attributes;
        UI32                     m_nextAvailableAttributeTag;
        UI32                     m_nextAvailableAttributeTagForMapping;
        map<string, Attribute *> m_attributesByName;
        map<UI32, Attribute *>   m_attributesByUserTag;
        map<UI32, string>        m_attributesOrderToNameMapping;

    protected :
    public :
};

}

#endif // ATTRIBUTESMAP_H
