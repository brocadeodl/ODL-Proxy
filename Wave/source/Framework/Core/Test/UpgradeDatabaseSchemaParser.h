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
 *   Author : Awanish Kumar Trivedi                                     *
 ***************************************************************************/
#ifndef UPGRADEDATABASESCHEMAPARSER_H
#define UPGRADEDATABASESCHEMAPARSER_H

#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include <fstream>
#include "xercesc/dom/DOM.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/framework/MemBufFormatTarget.hpp"
#include "xercesc/framework/StdOutFormatTarget.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMNodeIterator.hpp"
#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSavePrismConfigurationMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

namespace WaveNs
{

class OrmTable;
class OrmRepository;
class WaveManagedObject;
class PrismPersistableObject;
class WaveObjectManager;

/*
 * DOM Element and Attributes
 *
 */

/*
 * class UpgradeDbXmlAttribute  represent an XML attribute.
 * 
 */

class UpgradeDbXmlAttribute
{
private :
protected :
public :
                                                UpgradeDbXmlAttribute           (const string& xmlAttribName,const  string &xmlAttribVal =string (""),const string &xmlAttribOwner = string("NDEF"), bool isxmlAttribUsed = false);
                                                UpgradeDbXmlAttribute           ();
    virtual                                     ~UpgradeDbXmlAttribute          ();
            void                                setxmlAttributeValue            (const string& xmlAttribVal);
            void                                setxmlIsAttributeUsed           (bool xmlAttribUsed);
            void                                setOwner                        (const string& owner);

            bool                                getxmlIsAttributeUsed           ();
            string                              getxmlAttributeValue            ();
            string                              getXmlAttributeName             ();
            string                              getOwner                        ();
    virtual bool                                loadFromXML                     (DOMNode *pDomNode);

private :

protected :
public :
            string                              m_xmlAttribName;
            string                              m_xmlAttribVal;
            bool                                m_isxmlAttribUsed;
            string                              m_xmlAttribOwner;
};

/*
 * class UpgradeDbXmlElement represent a DOM element. A dom elemnt can have a any number of DOM elemnts as child and Attributes
 */
class UpgradeDbXmlElement
{
private :
protected :
public :
                                                    UpgradeDbXmlElement                             (const string& xmlElementName, const string& xmlElementOwner = string(""), bool isxmlElementInuse = false );
                                                    UpgradeDbXmlElement                             ();
    virtual                                         ~UpgradeDbXmlElement                            ();
            string                                  getXmlElementName                               ();
            void                                    setXmlElementName                               (const string &xmlElementName);
            void                                    setxmlElementTextContent                        (const string &xmlElementTextContent);
            string                                  getxmlElementTextContent                        ();
            vector<UpgradeDbXmlAttribute*>          getxmlAttributes                                ();
            vector<UpgradeDbXmlElement*>            getxmlChildNodes                                ();
            bool                                    addXmlChildNode                                 (UpgradeDbXmlElement *pxmlChild);
            bool                                    addxmlAttributes                                (UpgradeDbXmlAttribute *pUpgradeDbXmlAttribute);
            bool                                    addXmlAttributes                                (const string& xmlAttribName, const string &xmlAttribVal = string(""), bool isxmlAttribUsed = false);
            bool                                    addXmlAttributeChild                            (const string& xmlAttribName, const string &xmlAttribVal = string(""), const string& ownerAttribute = string("") );
    virtual bool                                    loadFromXML                                     (DOMNode *pDomNode);
            bool                                    loadFromXML                                     (DOMElement *pDomNode);
            bool                                    isAttributePresent                              (const string &attributeName);
            bool                                    isChildPresent                                  (const string &elementName);
    virtual bool                                    handleXMLParserDomElement                       (DOMNode *pDomNode);
            bool                                    getIsElementinUse                               ();
            void                                    setIsElementinUse                               (bool isxmlElementInuse);
            void                                    setOwner                                        (const string& owner);
            string                                  getOwner                                        ();
            void                                    display                                         ();
    virtual void                                    getSql                                          (string &sql);
    virtual void                                    addRelations                                    (string &sql);

private :
protected :
            string                                  m_xmlElementName;
            string                                  m_xmlElementTextContent;
            
            vector<UpgradeDbXmlAttribute*>          m_xmlVectorAttributes;
            vector<UpgradeDbXmlElement*>            m_xmlVectoChildElements;
            bool                                    m_isxmlElementInuse;
            bool                                    m_isRoot;
public :
            string                                  m_xmlElementOwner;
};

/*
 * DifferenceSchema XML tree definition
 *
 */

class UpgradeDbXMlSchemaFields : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                 UpgradeDbXMlSchemaFields                           ();
    virtual                                     ~UpgradeDbXMlSchemaFields                           ();
    virtual bool                                 handleXMLParserDomElement                          (DOMNode *pDomNode);
    virtual void                                getSql                                              (string &sql);
    virtual void                                addRelations                                        (string &sql);
private :
protected :
public :
};

class UpgradeDbXMlSchema : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbXMlSchema                                  ();
    virtual                                     ~UpgradeDbXMlSchema                                 ();
    virtual bool                                handleXMLParserDomElement                           ( DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
    virtual void                                addRelations                                        (string &sql);
private :
protected :
public :
};

class UpgradeDbXMlMoName : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbXMlMoName                                  ();
    virtual                                     ~UpgradeDbXMlMoName                                 ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode *pDomNode);
    virtual void                                getSql                                              (string &sql);
    virtual void                                addRelations                                        (string &sql);

private :
protected :
public :
};



class UpgradeDbxmlNewManagedObject : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlNewManagedObject                        ();
    virtual                                     ~UpgradeDbxmlNewManagedObject                       ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :

};

class UpgradeDbxmlNewSchemaFields : public UpgradeDbXmlElement
{

private :
protected :
public :                                        UpgradeDbxmlNewSchemaFields                         ();
    virtual                                     ~UpgradeDbxmlNewSchemaFields                        ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);

private :
protected :
public :
};

class UpgradeDbxmlAdd : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlAdd                                     ();
    virtual                                     ~UpgradeDbxmlAdd                                    ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlOldManagedObject : public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlOldManagedObject                        ();
                                                ~UpgradeDbxmlOldManagedObject                       ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlOldSchemaFields:  public UpgradeDbxmlNewSchemaFields
{
private :
protected :
public :
                                                UpgradeDbxmlOldSchemaFields                         ();
    virtual                                     ~UpgradeDbxmlOldSchemaFields                        ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode *pDomNode);
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlModifiedSchemaFields: public UpgradeDbxmlNewSchemaFields
{
private :
protected :
public :
                                                UpgradeDbxmlModifiedSchemaFields                    ();
    virtual                                     ~UpgradeDbxmlModifiedSchemaFields                   ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode *pDomNode);
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlRemove:   public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlRemove                                  ();
    virtual                                     ~UpgradeDbxmlRemove                                 ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlModified: public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlModified                                ();
    virtual                                     ~UpgradeDbxmlModified                               ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

class UpgradeDbxmlDifferenceSchema :  public UpgradeDbXmlElement
{
private :
protected :
public :
                                                UpgradeDbxmlDifferenceSchema                        ();
    virtual                                     ~UpgradeDbxmlDifferenceSchema                       ();
    virtual bool                                handleXMLParserDomElement                           (DOMNode* pDomNode );
    virtual void                                getSql                                              (string &sql);
private :
protected :
public :
};

/*
 * DOM Parser class encapsulates the tree hierarchy and is responsible for providing the interfaces for consumption
 *
 */
class UpgradeDatabaseSchemaParser
{
private :
protected :
public :
                                                UpgradeDatabaseSchemaParser                         (const string& xmlFile = /*string("/home/awanish/projects/parser1/Test.xml")*/ string("/users/home40/atrivedi/Test.xml"));
    virtual                                     ~UpgradeDatabaseSchemaParser                        ();
            bool                                loadFromXML                                         ();
            void                                display                                             ();
    virtual void                                getSql                                              (string &sql);
    static  bool                                addNewSchemaPointer                                 (string& moName,  UpgradeDbXmlElement * pNewMO);
    static  UpgradeDbXmlElement *               getNewSchemaPointer                                 (string& moName);
    static  bool                                isPresentNewSchemaPointer                           (string& moName);
    /* Feature Integration Changes*/
            bool                                clearMoSchemaInfoVectors                            ();
    static  bool                                removeSchemaField                                   (string &moName, string& fieldName);
    static  bool                                modifySchemaField                                   (string &moName, string& fieldName, string &fieldType);
    static  bool                                addSchemaField                                      (string &moName, string& fieldName, string &fieldType, string &extFieldType);
    static  bool                                addNewManagedObject                                 (string &moName, vector<string> &vecFieldName, vector<string> &vecFieldType, string &inheritedFrom); 
    static  UI32                                getMaxClassID                                       ();
    static  UI32                                getClassIDForManagedObject                          (string &moName);
    static  bool                                removeOldManagedObjects                             (string &moName);
            bool                                loadPrismConfiguration                              ();
            bool                                savePrismConfiguration                              ();
            bool                                backupPrismConfiguration                            ();
            bool                                restorePrismConfiguration                           ();
            void                                createLockFileForConfigurationFile                  ();
            void                                removeLockFileForConfigurationFile                  ();
            ResourceId                          configurationWithFlock                              (const UI32 &operationType);
    static  string                              getSqlType                                          (string waveType);
    static  bool                                isAnyDerivationsTabledNeeded                        (string &tableName, string &fieldName, string &relatedTo);
    static  bool                                addRelationField                                    (string &moName, string& fieldName, string &fieldType, string &extFieldType, string &relatedTo);
    static  string                              getSqlForRelations                                  (string &moName, string& fieldName, string &fieldType, string &relatedTo);
            void                                printDifferencesInVectors                           ();
            bool                                populateSchemaUpgradeTableInDb                      ();
            void                                generateSqlToPopulateUpgradeTable                   (string& sqlForUpgradeTablePopulation,const string& waveSchema);
            bool                                loadCurrentSchemaInfoObjects                        ();
            size_t                              getSchemaInfoObjectsFromUpgradeMO                   (vector<string> & moNames, vector<string> & moFieldNamesStrings,vector<string> & moFieldTypesStrings,vector<UI32> & classIds, vector<UI32> & parentClassIds, vector<string> & userDefinedKeyCombinationsFieldNames, vector<string> & userDefinedKeyCombinationsFieldTypes, vector<string> &fieldNamesExpandedTypes, vector<bool>& isLocalMOInfo, vector<string>& derivedFromClassNames);
    static  bool                                modifyExtFieldType                                  (string &extFieldType, string& newValue, UI8 pos);
                

private :
            bool                                        m_isInitialized;
            XercesDOMParser                             *m_pParser;
            DOMDocument                                 *m_pDocument;
            DOMElement                                  *m_pRootElement;
            UpgradeDbxmlDifferenceSchema                *m_pUpgradeDbxmlDifferenceSchema;
            PrismFrameworkConfiguration                 m_prismFrameworkConfiguration;
/* Define the MoSchemaInfoChangesMembers*/
    static  vector<string>                              m_managedObjectNames;
    static  vector<string>                              m_managedObjectFieldNamesStrings;
    static  vector<string>                              m_managedObjectFieldTypesStrings;
    static  vector<UI32>                                m_classIds;
    static  vector<UI32>                                m_parentClassIds;
    static  vector<pair<string, UI32> >                 m_tableClassIdTuples;
    static  vector<string>                              m_userDefinedKeyCombinationsFieldNamesForSchemaChange;
    static  vector<string>                              m_userDefinedKeyCombinationsFieldTypesForSchemaChange;
    static  vector<string>                              m_managedObjectFieldExpandedTypesStrings;
    static  vector<bool>                                m_isALocalManagedObjectInfo;
    static  vector<string>                              m_derivedFromClassNames;
        
protected :
public :
        static map <string, UpgradeDbXmlElement*>   m_mapOfNewManagedObjects;
};

    
}
#endif // UPGRADEDATABASESCHEMAPARSER_H

