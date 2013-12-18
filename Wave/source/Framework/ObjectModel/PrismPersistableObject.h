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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMPERSISTABLEOBJECT_H
#define PRISMPERSISTABLEOBJECT_H

#include "Framework/Attributes/AttributesMap.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForSetOperation.h"
#include "Framework/Types/Uuid.h"

#include <string>

#include "libpq-fe.h"

using namespace std;

namespace WaveNs
{

class WaveManagedObjectOperation;

class PrismPersistableObject
{
    private :
        static bool getIsObjectRelationalMappingEnabled ();
        static void setIsObjectRelationalMappingEnabled (const bool &isObjectRelationalMappingEnabled);

               void obtainNewObjectIdObjectId           ();

        static void addGlobalUserTagToNameCombination   (const string &className, const UI32 &userTag, const string &name, const AttributeType &attributeType);
               void computeKeyString                    ();

    protected :
                     PrismPersistableObject           (const string &objectClassName, const string &objectDerivedFromClassName);
                     PrismPersistableObject           (const string &viewName);

        virtual void setupAttributesForPersistence    ();

                void setUserDefinedKeyCombination     (const vector<string> &userDefinedKeyCombination);
                void addPersistableAttribute          (Attribute *pAttribute);
                void addPersistableAttributeForCreate (Attribute *pAttribute);

        virtual void setupKeys                        ();
        virtual void setEmptyNeededOnPersistentBoot   (const bool emptyNeeded);
        virtual void setEmptyNeededOnPersistentBootWithDefault (const bool emptyNeededWithDefault);

    public :
        virtual                ~PrismPersistableObject                ();

        static  string          getClassName                          ();

                void            prepareForSerialization               ();
                string          getObjectClassName                    () const;
                string          getObjectDerivedFromClassName         ();
                string          getSqlForCreate                       (const string &waveSchema);
        virtual string          getSqlForCreateView                   ();
                string          computeSqlForCreateView               ();
                void            getSqlForInsert                       (string &sqlStringForInsert, const string &waveSchema);
                void            getSqlForUpdate                       (string &sqlStringForUpdate, const string &waveSchema);

                void            setupOrm                              ();
        virtual void            setupAttributesForCreate              ();
                void            prepareForCreate                      ();

                ObjectId        getObjectId                           () const;
                Uuid            getObjectUuid                         () const;

        virtual void            loadFromPostgresQueryResult           (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager = NULL, const bool loadOneToManyRelationships = true, const bool loadCompositions = true);
        virtual void            loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager = NULL, const bool loadCompositions = true);

        virtual void            loadFromPostgresQueryResult2          (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager);

                vector<string>  getUserDefinedKeyCombination          () const;
                string          getUserDefinedKeyCombinationValue     () const;

                Attribute      *getAttributeByName                    (const string &attributeName);
                Attribute      *getAttributeByUserTag                 (const UI32 &attributeUserTag);

        // Note : The function ("getAttributeByUserTagUserDefined") should return NULL  or  'an independently heap allocated (new) attribute.'
        //        Caller of this function will explicitly delete the attribute returned.

        virtual Attribute      *getAttributeByUserTagUserDefined      (const UI32 &attributeUserTagUserDefined);

                void            setUserTagForAttribute                (const string &attributeName, const UI32 &attributeUserTag);

                void            updateOrmRelations                    ();
                WaveManagedObject *getComposedManagedObject           (const ObjectId &childObjectId);
        virtual UI32            getCase                               (const UI32 &attributeUserTag);
        vector<UI32>            getAttributeUserTags                  () const;

        virtual void            getShowDump                           (string &showDump, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getRawShowDump                        (string &showDump, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);

        static  string          getNameFromUserTag                    (const string &className, const UI32 &userTag);
        static  UI32            getUserTagFromName                    (const string &className, const string &attributeName);
        static  AttributeType   getTypeFromUserTag                    (const string &className, const UI32 &userTag);
                bool            isHierarchyDeletableForOperation      (const WaveManagedObjectOperation &operation);

        virtual void            getAttributeNamesForHtmlTable         (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getHtmlTableHeaderNames               (vector<string> &htmlTableHeaderNames,       const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getHtmlTableHeaderNamesString         (string         &htmlTableHeaderNamesString, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getHtmlTableRowData                   (vector<string> &htmlTableRowData,           const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getHtmlTableRowDataString             (string         &htmlTableRowDataString,     const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getManagedObjectClassNameForRest      (string         &managedObjectClassNameForRest) const;
        virtual void            getAttributeNamesForRest              (vector<string> &attributeNamesForRest,      const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getRestHeaderNames                    (vector<string> &restHeaderNames,            const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getRestRowData                        (string         &restRowData,                const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getRestRowData                        (string         &restRowData,                const vector<string> &attributeNamesForRest);

        virtual void            getAttributeNamesForJson              (vector<string> &attributeNamesForJson,      const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getJsonNames                          (vector<string> &jsonNames,                  const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getManagedObjectClassNameForJson      (string         &managedObjectClassNameForJson) const;
        virtual void            getJsonObjectData                     (string         &jsonObjectData,             const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void            getJsonObjectData                     (string         &jsonObjectData,             const vector<string> &attributeNamesForRest);

               string           getKeyString                          () const;
               void             updateKeyString                       ();
               void             prependOwnerKeyString                 (const string &keyString);

               vector<string>   getAttributesToBeUpdated              () const;
               void             setAttributesToBeUpdated              (const vector<string> &attributesToBeUpdated);
               void             addAttributeToBeUpdated               (const string &attributeName);
               void             clearAttributesToBeUpdated            ();

               void             setDisableValidations                 (const string &attributeName, const bool disableValidations);

               void             getOidsOfOneToOneCompositions                   (vector<ObjectId> &vectorOfCompositionOids);
               void             popOneToOneCompositionsFromResults              (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject);
               void             storeRelatedObjectIdVectorForAOneToNAssociation (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds);
               void             storeRelatedObjectVectorForAOneToNComposition   (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects);
       //virtual WaveManagedObjectSynchronousQueryContextForSetOperation* getSetContextForCreateView            ();
       //virtual void             getSetContextForCreateView            (WaveManagedObjectSynchronousQueryContextForSetOperation* &pViewContext);
       virtual void             getSetContextForCreateView            (WaveManagedObjectSynchronousQueryContextForSetOperation &viewContext);

    // Now the data members

    private :
        string                                        m_objectClassName;
        string                                        m_objectDerivedFromClassName;
        ObjectId                                      m_objectId;
        Uuid                                          m_objectUuid;
        AttributesMap                                 m_persistableAttributes;
        AttributesMap                                 m_persistableAttributesForCreate;
        bool                                          m_isPreparedForPersistence;
        bool                                          m_isPreparedForCreate;
        bool                                          m_emptyNeededOnPersistentBoot;
        string                                        m_keyString;
        string                                        m_ownerKeyString;

        vector<string>                                m_userDefinedKeyCombination;
        vector<UI32>                                  m_attributeUserTags;
        string                                        m_userDefinedKeyCombinationValue;

        vector<string>                                m_attributesToBeUpdated;

        static map<string, map<UI32, string> >        m_globalUserTagToNameMap;
        static map<string, map<string, UI32> >        m_globalNameToUserTagMap; 
        static map<string, map<UI32, AttributeType> > m_globalUserTagToTypeMap;
        static PrismMutex                             m_globalUserTagToNameMapMutex;
        bool                                          m_emptyNeededOnPersistentBootWithDefault;
        bool                                          m_isPrismView;
        string                                        m_viewName;

    protected :
    public :

    friend class PersistenceObjectManager;
    friend class CommonManagedObjectBase;
};

}

#endif // PRISMPERSISTABLEOBJECT_H
