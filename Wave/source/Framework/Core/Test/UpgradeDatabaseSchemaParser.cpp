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
#include "Framework/Core/Test/UpgradeDatabaseSchemaParser.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Framework/ObjectRelationalMapping/OrmToolKit.h"
#include <sys/file.h>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace WaveNs
{
    
map <string, UpgradeDbXmlElement*>  UpgradeDatabaseSchemaParser::m_mapOfNewManagedObjects;
vector<string>                      UpgradeDatabaseSchemaParser::m_managedObjectNames;
vector<string>                      UpgradeDatabaseSchemaParser::m_managedObjectFieldNamesStrings;
vector<string>                      UpgradeDatabaseSchemaParser::m_managedObjectFieldTypesStrings;
vector<string>                      UpgradeDatabaseSchemaParser::m_userDefinedKeyCombinationsFieldNamesForSchemaChange;
vector<string>                      UpgradeDatabaseSchemaParser::m_userDefinedKeyCombinationsFieldTypesForSchemaChange;
vector<string>                      UpgradeDatabaseSchemaParser::m_managedObjectFieldExpandedTypesStrings;
vector<UI32>                        UpgradeDatabaseSchemaParser::m_classIds;
vector<UI32>                        UpgradeDatabaseSchemaParser::m_parentClassIds;
vector<bool>                        UpgradeDatabaseSchemaParser::m_isALocalManagedObjectInfo;
vector<string>                      UpgradeDatabaseSchemaParser::m_derivedFromClassNames;
vector<pair<string, UI32> >         UpgradeDatabaseSchemaParser::m_tableClassIdTuples;

/*
 * 
 * Class:UpgradeDbXmlAttribute
 * This class is 1-1 mapping with xml attribute. 
 * 
 */
UpgradeDbXmlAttribute::UpgradeDbXmlAttribute(const std::string& xmlAttribName, const std::string& xmlAttribVal, const std::string& xmlAttribOwner, bool isxmlAttribUsed)

{
    m_xmlAttribName= xmlAttribName;
    m_xmlAttribVal = xmlAttribVal;
    m_isxmlAttribUsed = false;
    m_xmlAttribOwner = xmlAttribOwner;
}

UpgradeDbXmlAttribute::UpgradeDbXmlAttribute()
{
    m_isxmlAttribUsed = false;
}

UpgradeDbXmlAttribute::~UpgradeDbXmlAttribute()
{
    //Do nothing
}


string UpgradeDbXmlAttribute::getXmlAttributeName()
{
    return (m_xmlAttribName);
}

string UpgradeDbXmlAttribute::getxmlAttributeValue()
{
    return (m_xmlAttribVal);
}


bool UpgradeDbXmlAttribute::getxmlIsAttributeUsed()
{
    return (m_isxmlAttribUsed);
}

void UpgradeDbXmlAttribute::setxmlAttributeValue(const std::string& xmlAttribVal)
{
    m_xmlAttribVal = xmlAttribVal;
}

void UpgradeDbXmlAttribute::setxmlIsAttributeUsed(bool xmlAttribUsed )
{
    m_isxmlAttribUsed = xmlAttribUsed;
}

/*
 * Load the XML tree with attributes as per the tree definition
 */
bool UpgradeDbXmlAttribute::loadFromXML(DOMNode* pDomNode)
{
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbXmlAttribute::loadFromXML  :AttibuteELEMENT :") + m_xmlAttribName + string (" Owner: ") + m_xmlAttribOwner);
    bool status = false/*true*/;
    if ( pDomNode )
    {

        {
            char *pAttributeName = NULL;
            pAttributeName = XMLString::transcode (pDomNode->getNodeName ());
            cout <<pAttributeName<< endl;
            if ( m_xmlAttribName == (pAttributeName ))
            {
                char *pAttributeValue = NULL;
                pAttributeValue = XMLString::transcode (pDomNode->getNodeValue());
                cout << "Value" << pAttributeValue<< endl;
                m_xmlAttribVal = pAttributeValue;

                status = true ;
                m_isxmlAttribUsed = status;
                XMLString::release (&pAttributeValue);

            }
            XMLString::release (&pAttributeName);
        }
    }
    return (status);
}

string UpgradeDbXmlAttribute::getOwner()
{
    return (m_xmlAttribOwner);
}

void UpgradeDbXmlAttribute::setOwner(const std::string& owner)
{
    m_xmlAttribOwner = owner;
}


/*
 * 
 * Class:UpgradeDbXmlElement
 * 
 */

UpgradeDbXmlElement::UpgradeDbXmlElement(const std::string& xmlElementName, const std::string& xmlElementOwner ,bool isxmlElementInuse)

{
    m_xmlElementName = xmlElementName;
    m_xmlElementOwner = xmlElementOwner;
    m_isxmlElementInuse = isxmlElementInuse;
    m_isRoot = false;
}
UpgradeDbXmlElement::UpgradeDbXmlElement ()

{
    m_isxmlElementInuse = false;
    m_isRoot = false;
}


UpgradeDbXmlElement::~UpgradeDbXmlElement()
{
    UI32 i;
    for (i=0; i <m_xmlVectoChildElements.size(); i++)
    {
        delete (m_xmlVectoChildElements[i]);
    }
    m_xmlVectoChildElements.clear();

    for( i=0; i <m_xmlVectorAttributes.size(); i++)
    {
        delete (m_xmlVectorAttributes[i]);
    }
}

bool UpgradeDbXmlElement::isAttributePresent(const std::string& xmlAttribName)
{
    for ( UI32 i=0; i < m_xmlVectorAttributes.size(); i++ )
    {
        if ( m_xmlVectorAttributes[i]->getXmlAttributeName() ==  xmlAttribName )
        {
            return (false);
        }
    }
    return (false);

}
void UpgradeDbXmlElement::display()
{
    cout << "< Name: " << m_xmlElementName << ", Text:" << m_xmlElementTextContent << " ATTRIB:[";
    for (UI32 j=0; j <m_xmlVectorAttributes.size(); j++ )
    {
        cout << "(Name :" << m_xmlVectorAttributes[j]->getXmlAttributeName() << ", Value:" << m_xmlVectorAttributes[j]->getxmlAttributeValue() <<")";
    }
    for ( UI32 i=0; i < m_xmlVectoChildElements.size(); i++ )
    {
         m_xmlVectoChildElements[i]->display();

    }
    cout << ">";

}

bool UpgradeDbXmlElement::isChildPresent(const std::string& elementName)
{
    for ( UI32 i=0; i < m_xmlVectoChildElements.size(); i++ )
    {
        if ( m_xmlVectoChildElements[i]->getXmlElementName() ==  elementName )
        {
            return (false);
        }
    }
    return (false);
}

bool UpgradeDbXmlElement::addXmlAttributes(const std::string& xmlAttribName, const string& xmlAttribVal, bool isxmlAttribUsed)
{
    if ( ! isAttributePresent(xmlAttribName)  )
    {
        UpgradeDbXmlAttribute  *p=  new UpgradeDbXmlAttribute(xmlAttribName, xmlAttribVal, m_xmlElementName, true);
        p->setOwner(m_xmlElementName);
        m_xmlVectorAttributes.push_back(p);
        return (true);
    }
    return (false);
}

bool UpgradeDbXmlElement::addXmlChildNode(UpgradeDbXmlElement *pxmlChild)
{
    if ( !isChildPresent( pxmlChild->getXmlElementName() ) )
    {
        m_xmlVectoChildElements.push_back(pxmlChild);
        return (true);
    }
    return (false);
}

vector< UpgradeDbXmlAttribute *> UpgradeDbXmlElement::getxmlAttributes()
{
    return (m_xmlVectorAttributes);
}


vector< UpgradeDbXmlElement* > UpgradeDbXmlElement::getxmlChildNodes()
{
    return (m_xmlVectoChildElements);
}


string UpgradeDbXmlElement::getXmlElementName()
{
    return (m_xmlElementName);
}


string UpgradeDbXmlElement::getxmlElementTextContent()
{
    return (m_xmlElementTextContent);

}

void UpgradeDbXmlElement::setXmlElementName(const std::string& xmlElementName)
{
    m_xmlElementName = xmlElementName;
}

void UpgradeDbXmlElement::setxmlElementTextContent(const std::string& xmlElementTextContent)
{
    m_xmlElementTextContent = xmlElementTextContent;
}


bool UpgradeDbXmlElement::addXmlAttributeChild(const std::string& xmlAttribName, const  string& xmlAttribVal, const std::string& ownerAttribute)
{
    if ( ownerAttribute != m_xmlElementName )
    {
        //cout << "I am the owner"<< m_xmlElementName <<endl;
        UpgradeDbXmlAttribute *pUpgradeDbXmlAttribute = new UpgradeDbXmlAttribute(xmlAttribName, xmlAttribVal, ownerAttribute);
        m_xmlVectorAttributes.push_back(pUpgradeDbXmlAttribute);
        return (true);
    }
    for ( UI32 i=0; i < m_xmlVectoChildElements.size(); i ++ )
    {
        if ( true == m_xmlVectoChildElements[i]->addXmlAttributeChild(xmlAttribName, xmlAttribVal,ownerAttribute))
        {
            return (true);

        };
    }
    return (false);
}

bool UpgradeDbXmlElement::addxmlAttributes(UpgradeDbXmlAttribute* pUpgradeDbXmlAttribute)
{
    m_xmlVectorAttributes.push_back(pUpgradeDbXmlAttribute);
    return (true);
}

bool UpgradeDbXmlElement::getIsElementinUse()
{
    return (m_isxmlElementInuse);
}

void UpgradeDbXmlElement::setIsElementinUse ( bool isxmlElementInuse )
{
    m_isxmlElementInuse = isxmlElementInuse;
}

/*
 * Load from XML this DOM element.
 * 1. Verify the tree order is correct.
 * 2. Load XML attributes for the DOM element.
 * 3. Load the XML child elements into the tree. 
 * load from XML file happens in the inorder as per the tree hierarchy.
 */

bool UpgradeDbXmlElement::loadFromXML(DOMNode* pDomNode)
{
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbXmlElement::loadFromXML  :ELEMENT :") + m_xmlElementName + string (" Owner: ") + m_xmlElementOwner);
    if ( pDomNode )
    {

        char *pOwnerTag = NULL;
        char *pAttributeName = NULL;
        char *pTextContent  =  NULL;
        if ( ! m_isRoot )
            pOwnerTag = XMLString::transcode (pDomNode->getParentNode()->getNodeName());
        //cout << "Tag:" << pOwnerTag<<endl;
        if ( m_isRoot || (!m_xmlElementOwner.compare(pOwnerTag) ))
        {

            pAttributeName = XMLString::transcode (pDomNode->getNodeName ());
            //cout <<"AttributeName :" <<  pAttributeName<< endl;
            if ( ! m_xmlElementName.compare(pAttributeName))
            {

                pTextContent = XMLString::transcode (pDomNode->getTextContent());
                m_xmlElementTextContent = pTextContent;
                //cout <<pTextContent<< endl;

            }else{
                //cout << "loaded From" << pOwnerTag <<"-> ("<< pAttributeName <<")"<<endl;
                XMLString::release(&pOwnerTag);
                XMLString::release (&pAttributeName);
                return (false);
            }

            if ( pDomNode->hasAttributes() )
            {

                DOMNamedNodeMap *pDOMNamedNodeMap = pDomNode->getAttributes();
                //cout << "1load attributes" <<  m_xmlElementName<<m_xmlVectorAttributes.size() << pDOMNamedNodeMap->getLength() <<endl;
                for (UI32 i=0; i < pDOMNamedNodeMap->getLength(); i++ )
                {
                    for ( UI32 j=0; j <  (m_xmlVectorAttributes.size()); j++ )
                    {
                        //cout << m_xmlVectorAttributes[j]->getXmlAttributeName() <<  m_xmlVectorAttributes[j]->getxmlIsAttributeUsed()<<endl ;
                        if (  m_xmlVectorAttributes[j]->getxmlIsAttributeUsed() == false )
                        {
                            //cout << m_xmlVectorAttributes[j]->getXmlAttributeName() ;
                            if (  true == (m_xmlVectorAttributes[j]->loadFromXML( pDOMNamedNodeMap->item(i))))
                            {
                            //  cout << "2load attributes" << i << j << m_xmlElementName<<endl;
                                break;
                            }
                        }
                    }
                }
            }
            if ( pDomNode->hasChildNodes())
            {
                DOMNodeList *pDOMNodeList = pDomNode->getChildNodes();
                UI32 length = pDOMNodeList->getLength();
                for (UI32 i=0; i < length; i++ )
                {
                    //cout << "load chld i" <<i << pDOMNodeList->getLength() <<m_xmlElementName<<endl;
                    if ( DOMNode::ELEMENT_NODE ==  pDOMNodeList->item(i)->getNodeType())
                    {
                        /*Dynamically add my tree hierarchy*/
                        handleXMLParserDomElement(pDOMNodeList->item(i));
                    }
                }
            }
        }
        //cout << "loaded From outer" << pOwnerTag <<"-> ("<< pAttributeName <<")";// << pTextContent <<")";
        XMLString::release(&pOwnerTag);
        XMLString::release (&pAttributeName);
        XMLString::release (&pTextContent);
        m_isxmlElementInuse = true;
    }
    return (m_isxmlElementInuse);
}

/*
 * Get the Sql for all my childnodes 
 */
void UpgradeDbXmlElement::getSql(string& sql)
{
    for ( UI32 i=0; i< m_xmlVectoChildElements.size(); i++ )
    {
        m_xmlVectoChildElements[i]->getSql(sql);
    }
}

void UpgradeDbXmlElement::addRelations(string &sql)
{
    for ( UI32 i=0; i< m_xmlVectoChildElements.size(); i++ )
    {
        m_xmlVectoChildElements[i]->addRelations(sql);
    }
}

/*
 * In case of root element load all my child DOM elements
 */
bool UpgradeDbXmlElement::loadFromXML(DOMElement* pDomNode)
{
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbXmlElement::loadFromXML  :ELEMENT :") + m_xmlElementName + string (" Owner: ") + m_xmlElementOwner);
    if ( pDomNode->hasChildNodes())
    {
        DOMNodeList *pDOMNodeList = pDomNode->getChildNodes();
        for (UI32 i=0; i < pDOMNodeList->getLength(); i++ )
        {
            if ( DOMNode::ELEMENT_NODE == pDOMNodeList->item(i)->getNodeType())
            {
                handleXMLParserDomElement(pDOMNodeList->item(i));
            }
        }
    }
    return (true);

}

string UpgradeDbXmlElement::getOwner()
{
    return (m_xmlElementOwner);
}

void UpgradeDbXmlElement::setOwner ( const std::string& owner )
{
    m_xmlElementOwner = owner;
}

bool UpgradeDbXmlElement::handleXMLParserDomElement(DOMNode* pDomNode)
{
    // Do nothing Needs to be done as per tree hierarchy
    // This function is called to add the child element in the tree, while loading from XML file.
    return (true);
}

/*
 *
 * 
 * Class:UpgradeDbXMlSchemaFields
 * This class represents the Schema fields for addition/removal/modification or schema definition of new Managed Objects.
 * It has attributes like MO name, fieldName, fieldType, RelatedTo  as its XML attributes.
 * 
 */

UpgradeDbXMlSchemaFields::UpgradeDbXMlSchemaFields()
{
    m_xmlElementName = string ("SchemaFields");
    const string fieldname = string("FieldName");
    const string fieldtype =  string("FieldType");
    const string nameNo =  string("MoName");
    const string relatedTo = string("RelatedTo");
    const string dbType = string ("DbType");
    const string disableValidation = string ("DisableValidation");
    const string canBeEmpty = string ("CanBeEmpty");

    addXmlAttributes(fieldname);
    addXmlAttributes(fieldtype);
    addXmlAttributes(nameNo);
    addXmlAttributes(relatedTo);
    addXmlAttributes(dbType);
    addXmlAttributes(disableValidation);
    addXmlAttributes(canBeEmpty);
}

UpgradeDbXMlSchemaFields::~UpgradeDbXMlSchemaFields()
{

}

bool UpgradeDbXMlSchemaFields::handleXMLParserDomElement(DOMNode* pDomNode)
{
    return loadFromXML(pDomNode);
}

/*
 * Handle SQL generations for schema fields based on owner element
 */

void UpgradeDbXMlSchemaFields::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
        string moname, fieldName, fieldType, dbType ="", relatedTo ="";
        string disableValidation = "false", canBeEmpty = "false";

        for ( UI32 i=0; i <m_xmlVectorAttributes.size(); i++ )
        {
            if ( m_xmlVectorAttributes[i]->getXmlAttributeName() == "MoName" )
            {
                moname = m_xmlVectorAttributes[i]->getxmlAttributeValue();
            } else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "FieldName" )
            {
                fieldName =   m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }  else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "FieldType" )
            {
                fieldType =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }  else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "DbType" )
            {
                dbType =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }  else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "DisableValidation" )
            {
                disableValidation =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }  else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "CanBeEmpty" )
            {
                canBeEmpty =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }else if (m_xmlVectorAttributes[i]->getXmlAttributeName() == "RelatedTo" )
            {
                relatedTo =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
            }
        }

    if ((true == dbType.empty()) && (false == fieldType.empty()))
    {
        dbType =  UpgradeDatabaseSchemaParser::getSqlType (fieldType);
    }

    disableValidation = (0 == disableValidation.compare("true"))?"1":"0";
    canBeEmpty = (0 == canBeEmpty.compare("true"))?"1":"0";

    string extendedTypeExt = dbType + "|" + disableValidation + "|" + canBeEmpty;

    // Removal of Schema fields
    if ( m_xmlElementOwner == "OldSchemaFields" )
    {
        if (  moname != "" )
        {
            OrmRepository *pOrmRepository = OrmRepository::getInstance();
            OrmTable *pOrmTable = pOrmRepository->getTableByName(moname);
            if ( pOrmTable )
            {
                if ( pOrmTable->isAKnownColumn(fieldName) || pOrmTable->isAKnownRelation(fieldName) )
                {
                    UpgradeDatabaseSchemaParser::removeSchemaField(moname, fieldName);
                    if (pOrmTable->isAKnownRelation(fieldName))
                    {
                        //string relatedTo = "";
                        string relateToTo= "";
                        bool neededDerivations = UpgradeDatabaseSchemaParser::isAnyDerivationsTabledNeeded(moname, fieldName, relatedTo);
                        if ( false == neededDerivations )
                        {
                            sql += "DROP TABLE IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + moname + "derivations CASCADE;\n" ;
                            sql += "DROP TABLE IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + moname + "derivationsinstances CASCADE;\n" ;
                        }

                        if ( "" != relatedTo)
                        {
                            neededDerivations = UpgradeDatabaseSchemaParser::isAnyDerivationsTabledNeeded(relatedTo, fieldName, relateToTo );
                            if ( false == neededDerivations   )
                            {
                                sql += "DROP TABLE IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + relatedTo + "derivations  CASCADE;\n" ;
                                sql += "DROP TABLE IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + relatedTo + "derivationsinstances  CASCADE;\n" ;
                            }
                        }
                    }

                    sql += pOrmTable->getSqlForRemoveColumn(OrmRepository::getWaveCurrentSchema(), fieldName);
                }
            }

        }
    } else  if ( m_xmlElementOwner == "NewSchemaFields" )
    { // Addition of schema fields
        if (  moname != "" )
        {
            OrmRepository *pOrmRepository = OrmRepository::getInstance();
            OrmTable *pOrmTable = pOrmRepository->getTableByName(moname);
            if ( pOrmTable )
            {
                if ( "" != relatedTo )
                {
                    sql += UpgradeDatabaseSchemaParser::getSqlForRelations(moname, fieldName, fieldType, relatedTo);
                    UpgradeDatabaseSchemaParser::addRelationField( moname, fieldName, fieldType, extendedTypeExt, relatedTo);
                } else {
                        
                    if ( pOrmTable->isAKnownColumn(fieldName) )
                    {
                        
                        trace (TRACE_LEVEL_FATAL, "Cannot add the New Column as it is already present");
                    } else {
                        sql +=  string("ALTER TABLE ") + OrmRepository::getWaveCurrentSchema() + "." + moname + " ADD COLUMN " + fieldName + "  " + UpgradeDatabaseSchemaParser::getSqlType (fieldType) + ";\n";
                        UpgradeDatabaseSchemaParser::addSchemaField(moname, fieldName, fieldType, extendedTypeExt);
                    }
                }
            }
        }
    } else  if ( m_xmlElementOwner == "ModifiedSchemaFields" )
    {//Modification of schema fields.
        if (  moname != "" )
        {
            OrmRepository *pOrmRepository = OrmRepository::getInstance();
            OrmTable *pOrmTable = pOrmRepository->getTableByName(moname);
            if ( pOrmTable )
            {
                sql +=  string("ALTER TABLE ") + OrmRepository::getWaveCurrentSchema() + "." + moname + "  ALTER COLUMN " + fieldName + " TYPE "  +  UpgradeDatabaseSchemaParser::getSqlType (fieldType) + ";\n"; 
                UpgradeDatabaseSchemaParser::modifySchemaField(moname, fieldName, fieldType );
            }
        }
    }else   if ( m_xmlElementOwner == "Schema" )
    { //New managed object schema fields.
        if (  moname != "" )
        {
                if ( "" != relatedTo )
                {
                    /* Generate SQLS*/
                    UpgradeDatabaseSchemaParser::addRelationField( moname, fieldName, fieldType,extendedTypeExt, relatedTo);

                } else {
                    sql += string (" ") + fieldName + string("  ") + UpgradeDatabaseSchemaParser::getSqlType(fieldType) + string (",\n\t") ;
                    UpgradeDatabaseSchemaParser::addSchemaField(moname, fieldName, fieldType,extendedTypeExt);
                }
        }
    }
}

void UpgradeDbXMlSchemaFields::addRelations(string &sql)
{
    string moname, fieldName, fieldType, relatedTo;
    for ( UI32 i=0; i <m_xmlVectorAttributes.size(); i++ )
    {
       if ( m_xmlVectorAttributes[i]->getXmlAttributeName() == "MoName" )
       {
           moname = m_xmlVectorAttributes[i]->getxmlAttributeValue();
       } else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "FieldName" )
       {
           fieldName =   m_xmlVectorAttributes[i]->getxmlAttributeValue();
       }  else if  (m_xmlVectorAttributes[i]->getXmlAttributeName() == "FieldType" )
       {
           fieldType =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
       }else if (m_xmlVectorAttributes[i]->getXmlAttributeName() == "RelatedTo" )
        {
            relatedTo =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
        }

    }
    if (  moname != "" )
    {
        if ( "" != relatedTo )
        {
            /* Generate SQLS*/
            sql += UpgradeDatabaseSchemaParser::getSqlForRelations( moname, fieldName, fieldType, relatedTo);
        }
    }

}

/*
 * 
 * Class:UpgradeDbXMlSchema
 * This class represnt the Schema for a new ManagedObject. The Owner element is Mo and it has schemafields as its child elements.
 * 
 */

UpgradeDbXMlSchema::UpgradeDbXMlSchema()
{
    m_xmlElementName = string ("Schema");
}

UpgradeDbXMlSchema::~UpgradeDbXMlSchema()
{

}


bool UpgradeDbXMlSchema::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    UpgradeDbXMlSchemaFields *pUpgradeDbXMlSchemaFields =  new UpgradeDbXMlSchemaFields();
    pUpgradeDbXMlSchemaFields->setOwner(m_xmlElementName);
    addXmlChildNode( pUpgradeDbXMlSchemaFields);
    return (pUpgradeDbXMlSchemaFields->loadFromXML(pDomNode));
    
}

void UpgradeDbXMlSchema::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}

void UpgradeDbXMlSchema::addRelations(string &sql)
{
    for ( UI32 i=0; i< m_xmlVectoChildElements.size(); i++ )
    {
        m_xmlVectoChildElements[i]->addRelations(sql);
    }
}


/*
 * 
 * Class:UpgradeDbXMlMoName
 * This Class represents Mo that needs to be removed incase of owner element OldManagedObject it also represnt the leaf node in this case. 
 * In case of New ManagedObject  It has Schema as its child element.
 * 
 */

UpgradeDbXMlMoName::UpgradeDbXMlMoName()

{
    m_xmlElementName = string ( "MO");
    addXmlAttributes(string("MoName"));
    addXmlAttributes(string("InheritsFrom"));
}

UpgradeDbXMlMoName::~UpgradeDbXMlMoName()
{

}

bool UpgradeDbXMlMoName::handleXMLParserDomElement(DOMNode* pDomNode)
{
    if ( m_xmlElementOwner == "NewManagedObject" )
    {
        UpgradeDbXMlSchema *pUpgradeDbXMlSchema = new UpgradeDbXMlSchema();
        pUpgradeDbXMlSchema->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbXMlSchema);
        
        
        return (pUpgradeDbXMlSchema->loadFromXML(pDomNode));
    }
    return UpgradeDbXmlElement::handleXMLParserDomElement(pDomNode);
}

void UpgradeDbXMlMoName::addRelations(string &sql)
{
    for ( UI32 i=0; i< m_xmlVectoChildElements.size(); i++ )
    {
        m_xmlVectoChildElements[i]->addRelations(sql);
    }
}
void UpgradeDbXMlMoName::getSql(string& sql)
{
    string sqlMoname ="";
    string sqlToAdd = "";
    //Get the Sql for removal of Managed Object
    if ( m_xmlElementOwner == "OldMangedObject" )
    {
        
        WaveNs::UpgradeDbXmlElement::getSql(sqlToAdd);
        for ( UI32 i=0; i <m_xmlVectorAttributes.size(); i++ )
        {
            if ( m_xmlVectorAttributes[i]->getXmlAttributeName() == "MoName" )
            {
                string moname =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
                if (  moname != "" )
                {

                    OrmRepository *pOrmRepository = OrmRepository::getInstance();
                    OrmTable *pOrmTable = pOrmRepository->getTableByName(moname);
                    UpgradeDatabaseSchemaParser::removeOldManagedObjects(moname);
                    sql += pOrmTable->getSqlForRemoveTables(OrmRepository::getWaveCurrentSchema(), moname);
                    vector<string> references;
                    pOrmTable->getReferences(references);
                    for (UI32 j=0; j < references.size(); j++ )
                    {
                        string dummyfieldname = "";
                        string dummyRelatedTo = "";
                        /*
                         * Handle removal of the derivations from the To end of a relations only
                         */
                        bool neededDerivations = UpgradeDatabaseSchemaParser::isAnyDerivationsTabledNeeded(references[i], dummyfieldname, dummyRelatedTo);
                        if ( false == neededDerivations )
                        {
                            sql += "DROP TABLE  IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + references[i] + "derivations CASCADE;\n";
                            sql += "DROP TABLE  IF EXISTS " + OrmRepository::getWaveCurrentSchema() + "." + references[i] + "derivationsinstances CASCADE;\n";
                        }

                    }
                }
            }
        }
    } else if ( m_xmlElementOwner == "NewManagedObject" )
    {
        /*Handle create table request*/
        for ( UI32 i=0; i <m_xmlVectorAttributes.size(); i++ )
        {
            if ( m_xmlVectorAttributes[i]->getXmlAttributeName() == "MoName" )
            {
                string moname =  m_xmlVectorAttributes[i]->getxmlAttributeValue();
                if (  moname != "" )
                {
                    /* update configuration vectors*/
                    vector <string> vecFieldName;
                    string inheritsFrom = "";
                    tokenize("objectIdClassId:objectIdInstanceId:keyString:ownerKeyString:name:genericStatus:specificStatus:ownerManagedObjectIdClassId:ownerManagedObjectIdInstanceId:ownerPartitionManagedObjectIdClassId:ownerPartitionManagedObjectIdInstanceId:createdTimeStamp:lastModifiedTimeStamp:ownerWaveNodeObjectIdClassId:ownerWaveNodeObjectIdInstanceId:waveSlotLocalManagedObjectObjectIdClassId:waveSlotLocalManagedObjectObjectIdInstanceId:",vecFieldName,':');
                    vector <string> vecFieldType;
                    tokenize("WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_STRING:WAVE_ATTRIBUTE_TYPE_STRING:WAVE_ATTRIBUTE_TYPE_STRING:WAVE_ATTRIBUTE_TYPE_RESOURCEID:WAVE_ATTRIBUTE_TYPE_RESOURCEID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_DATETIME:WAVE_ATTRIBUTE_TYPE_DATETIME:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID:WAVE_ATTRIBUTE_TYPE_OBJECTID" , vecFieldType, ':');
                    UpgradeDatabaseSchemaParser::addNewManagedObject(moname, vecFieldName, vecFieldType, inheritsFrom);

                    /*Generate SQL */
                    WaveNs::UpgradeDbXmlElement::getSql(sqlToAdd);
                    
                    sqlMoname += string( "CREATE TABLE ") + OrmRepository::getWaveCurrentSchema()+ string(".")+moname + string (" (  ");
                    for (UI32 l=0; l <vecFieldName.size(); l++ )
                    {
                        sqlMoname += string("\n\t") + vecFieldName[l] + string (" ") + UpgradeDatabaseSchemaParser::getSqlType(vecFieldType[l]) + string(",");
                    }
					sqlMoname += sqlToAdd;
                    sqlMoname += string("\n\tPRIMARY KEY (objectidinstanceid) \n);\n");
                    sqlToAdd = sqlMoname;
                    addRelations(sqlToAdd);

                }
            }
        }
            
    }
    sql += sqlToAdd;
}

/*
 * 
 * Class:UpgradeDbxmlNewManagedObject
 * This class represnt an addition of new Managed Objects with its child as schema of the new ManagedObject. The DOM owner element is ADD.
 * 
 */
UpgradeDbxmlNewManagedObject::UpgradeDbxmlNewManagedObject()
{
    m_xmlElementName = string("NewManagedObject");
    m_xmlElementOwner = string ("Add");

}

bool UpgradeDbxmlNewManagedObject::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    UpgradeDbXMlMoName *pUpgradeDbXMlMoName = new UpgradeDbXMlMoName();
    pUpgradeDbXMlMoName->setOwner(m_xmlElementName);
    addXmlChildNode(pUpgradeDbXMlMoName);
    bool status = pUpgradeDbXMlMoName->loadFromXML(pDomNode);
    vector<UpgradeDbXmlAttribute*> loadedAttributes =  pUpgradeDbXMlMoName->getxmlAttributes();
    for ( UI32 i=0 ; i<loadedAttributes.size(); i++ )
    {
        if ( loadedAttributes.at(i)->getXmlAttributeName() == "MoName" )
        {
            string moname = loadedAttributes.at(i)->m_xmlAttribVal;
            //UpgradeDatabaseSchemaParser::addNewSchemaPointer(moname, reinterpret_cast<UpgradeDbXmlElement* &>(pUpgradeDbXMlMoName));
            break;
        }
    }
    return (status);
}



UpgradeDbxmlNewManagedObject::~UpgradeDbxmlNewManagedObject()
{

}
void UpgradeDbxmlNewManagedObject::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlNewSchemaFields
 * This represents the adiition of schema fields to an existing MO. Owner element is ADD.
 * 
 */
UpgradeDbxmlNewSchemaFields::UpgradeDbxmlNewSchemaFields()

{
    m_xmlElementName = string ("NewSchemaFields");
    setOwner( string("Add"));
}

bool UpgradeDbxmlNewSchemaFields::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    UpgradeDbXMlSchemaFields *pUpgradeDbXMlSchemaFields = new UpgradeDbXMlSchemaFields();
    pUpgradeDbXMlSchemaFields->setOwner(m_xmlElementName);
    addXmlChildNode(pUpgradeDbXMlSchemaFields);
    return (pUpgradeDbXMlSchemaFields->loadFromXML(pDomNode));
}


UpgradeDbxmlNewSchemaFields::~UpgradeDbxmlNewSchemaFields()
{
}


void UpgradeDbxmlNewSchemaFields::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlAdd
 * This class represents the addition to existing Database. It has 2 child elements i.e newmanagedObject and newschemafields.
 * 
 */
UpgradeDbxmlAdd::UpgradeDbxmlAdd()
{
    m_xmlElementName = string ("Add");
    m_xmlElementOwner = string  ("DifferenceSchema");
}

UpgradeDbxmlAdd::~UpgradeDbxmlAdd()
{

}

bool UpgradeDbxmlAdd::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    char *pElementName =  NULL;
    pElementName = XMLString::transcode (pDomNode->getNodeName());
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbxmlAdd::handleXMLParserDomElement  :ELEMENT :") + pElementName);

    string schemafield = string("NewSchemaFields");
    string newmangedObject = string("NewManagedObject");

    if (!schemafield.compare(pElementName) )
    {
        UpgradeDbxmlNewSchemaFields *pUpgradeDbxmlNewSchemaFields = new UpgradeDbxmlNewSchemaFields();
        pUpgradeDbxmlNewSchemaFields->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlNewSchemaFields);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlNewSchemaFields->loadFromXML(pDomNode));
    }else if (!newmangedObject.compare(pElementName))
    {
        cout << newmangedObject <<endl;
        UpgradeDbxmlNewManagedObject *pUpgradeDbxmlNewManagedObject = new UpgradeDbxmlNewManagedObject();
        pUpgradeDbxmlNewManagedObject->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlNewManagedObject);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlNewManagedObject->loadFromXML(pDomNode));
    }   
    XMLString::release(&pElementName);
    return (false);
}

void UpgradeDbxmlAdd::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}

/*
 * 
 * Class:UpgradeDbxmlOldManagedObject
 * This class represents the Managed object to be removed from the existing DB. it has attributes like MO name
 * 
 */
UpgradeDbxmlOldManagedObject::UpgradeDbxmlOldManagedObject()
{
    m_xmlElementName = string ("OldMangedObject");
}

UpgradeDbxmlOldManagedObject::~UpgradeDbxmlOldManagedObject()
{

}

bool UpgradeDbxmlOldManagedObject::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    UpgradeDbXMlMoName *pUpgradeDbXMlMoName = new UpgradeDbXMlMoName();
    pUpgradeDbXMlMoName->setOwner(m_xmlElementName);
    addXmlChildNode(pUpgradeDbXMlMoName);
    return (pUpgradeDbXMlMoName->loadFromXML(pDomNode));

}
void UpgradeDbxmlOldManagedObject::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlOldSchemaFields
 * This class represents the old schema columns  to be removed from the existing DB. It has attributes like MO name, fieldname, field type
 * 
 */
UpgradeDbxmlOldSchemaFields::UpgradeDbxmlOldSchemaFields()
{
    m_xmlElementName = string (string("OldSchemaFields"));
    setOwner("Remove");
}

UpgradeDbxmlOldSchemaFields::~UpgradeDbxmlOldSchemaFields()
{

}

bool UpgradeDbxmlOldSchemaFields::handleXMLParserDomElement(DOMNode* pDomNode)
{
    UpgradeDbXMlSchemaFields *pUpgradeDbXMlSchemaFields = new UpgradeDbXMlSchemaFields();
    pUpgradeDbXMlSchemaFields->setOwner(m_xmlElementName);
    addXmlChildNode(pUpgradeDbXMlSchemaFields);
    return (pUpgradeDbXMlSchemaFields->loadFromXML(pDomNode));

}


void UpgradeDbxmlOldSchemaFields::getSql(string& sql)
{
    WaveNs::UpgradeDbxmlNewSchemaFields::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlModifiedSchemaFields
 * This represents the modification of schema columns part of parent element Modified. It has attributes like MO name, fieldname, field type
 * 
 */

UpgradeDbxmlModifiedSchemaFields::UpgradeDbxmlModifiedSchemaFields()
{
    m_xmlElementName = string (string("ModifiedSchemaFields"));
    setOwner("Modified");
}

UpgradeDbxmlModifiedSchemaFields::~UpgradeDbxmlModifiedSchemaFields()
{

}

void UpgradeDbxmlModifiedSchemaFields::getSql(string& sql)
{
    WaveNs::UpgradeDbxmlNewSchemaFields::getSql(sql);
}


bool UpgradeDbxmlModifiedSchemaFields::handleXMLParserDomElement(DOMNode* pDomNode)
{
    return WaveNs::UpgradeDbxmlNewSchemaFields::handleXMLParserDomElement(pDomNode);
}




/*
 * 
 * Class:UpgradeDbxmlRemove
 * This class Represents the all the removal instrumentation to be done in the existing Database. The parent Element is DifferenceSchema and it
 * has 2 child nodes i.s oldSchemafields and OldManagedObjects.
 * 
 */
UpgradeDbxmlRemove::UpgradeDbxmlRemove()

{
    m_xmlElementName = string("Remove");
}

UpgradeDbxmlRemove::~UpgradeDbxmlRemove()
{

}

bool UpgradeDbxmlRemove::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    
    char *pElementName =  XMLString::transcode (pDomNode->getNodeName());
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbxmlRemove::handleXMLParserDomElement  :ELEMENT :") + pElementName);
    if ( pElementName == string("OldSchemaFields" ))
    {
        UpgradeDbxmlOldSchemaFields *pUpgradeDbxmlOldSchemaFields = new UpgradeDbxmlOldSchemaFields();
        pUpgradeDbxmlOldSchemaFields->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlOldSchemaFields);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlOldSchemaFields->loadFromXML(pDomNode));
    } else if ( pElementName == string("OldMangedObject"))
    {
        UpgradeDbxmlOldManagedObject *pUpgradeDbxmlOldManagedObject = new UpgradeDbxmlOldManagedObject();
        pUpgradeDbxmlOldManagedObject->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlOldManagedObject);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlOldManagedObject->loadFromXML(pDomNode));
    }
    return (false);
}

void UpgradeDbxmlRemove::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlModified
 * This Class represents the modification to be done in the existing Database. Owner DOm elemnt is DifferenceSchema and it has one child node
 * i.e ModifiedSchemaFields
 * 
 */

UpgradeDbxmlModified::UpgradeDbxmlModified()
{
    m_xmlElementName = string("Modified");
}

UpgradeDbxmlModified::~UpgradeDbxmlModified()
{

}

bool UpgradeDbxmlModified::handleXMLParserDomElement(DOMNode* pDomNode)
{
    char *pElementName =  XMLString::transcode (pDomNode->getNodeName());
    trace(TRACE_LEVEL_DEBUG, string ( "UpgradeDbxmlModified::handleXMLParserDomElement  :ELEMENT :") + pElementName);
    if ( pElementName == string("ModifiedSchemaFields" ))
    {
        UpgradeDbxmlModifiedSchemaFields *pUpgradeDbxmlModifiedSchemaFields = new UpgradeDbxmlModifiedSchemaFields();
        pUpgradeDbxmlModifiedSchemaFields->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlModifiedSchemaFields);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlModifiedSchemaFields->loadFromXML(pDomNode));
    }
    XMLString::release(&pElementName);
    return (false);
}


void UpgradeDbxmlModified::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDbxmlDifferenceSchema
 * This class represents the entire DB instrumentation XML tree for parsing. It has 3 child nodes ie. Add,Remove,Modified which represent the
 * Type of DB instrumentation to be done.
 * 
 */
UpgradeDbxmlDifferenceSchema::UpgradeDbxmlDifferenceSchema()

{
    m_xmlElementName = string ("DifferenceSchema");
    m_isRoot = true;
}

UpgradeDbxmlDifferenceSchema::~UpgradeDbxmlDifferenceSchema()
{

}
bool UpgradeDbxmlDifferenceSchema::handleXMLParserDomElement ( DOMNode* pDomNode )
{
    char *pElementName =  XMLString::transcode (pDomNode->getNodeName());
    if ( pElementName == string("Add") )
    {
        UpgradeDbxmlAdd *pUpgradeDbxmlAdd = new UpgradeDbxmlAdd();
        pUpgradeDbxmlAdd->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlAdd);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlAdd->loadFromXML(pDomNode));

    } else if ( pElementName == string("Remove") )
    {
        UpgradeDbxmlRemove *pUpgradeDbxmlRemove = new UpgradeDbxmlRemove();
        pUpgradeDbxmlRemove ->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlRemove);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlRemove->loadFromXML(pDomNode));
    } else if ( pElementName == string("Modified") )
    {
        UpgradeDbxmlModified *pUpgradeDbxmlModified = new UpgradeDbxmlModified();
        pUpgradeDbxmlModified ->setOwner(m_xmlElementName);
        addXmlChildNode(pUpgradeDbxmlModified);
        XMLString::release(&pElementName);
        return (pUpgradeDbxmlModified->loadFromXML(pDomNode));
    }
    XMLString::release(&pElementName);
    return (false);
}

void UpgradeDbxmlDifferenceSchema::getSql(string& sql)
{
    WaveNs::UpgradeDbXmlElement::getSql(sql);
}


/*
 * 
 * Class:UpgradeDatabaseSchemaParser
 * 
 */

UpgradeDatabaseSchemaParser::UpgradeDatabaseSchemaParser(const string& xmlFile)
{
    m_isInitialized = true;
    try {
    XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        // Do your failure processing here
        m_isInitialized =false;
    }
    if ( m_isInitialized )
    {
        m_pParser  = new XercesDOMParser();

        m_pParser->setValidationScheme (XercesDOMParser::Val_Auto);
        m_pParser->setDoNamespaces (false);
        m_pParser->setDoSchema (false);
        m_pParser->setCreateEntityReferenceNodes (false);
        m_pParser->parse(xmlFile.c_str());

        m_pDocument           = m_pParser->getDocument ();
        m_pRootElement        = (m_pDocument->getDocumentElement ());
        m_pUpgradeDbxmlDifferenceSchema = new UpgradeDbxmlDifferenceSchema();
    }

}

bool UpgradeDatabaseSchemaParser::loadFromXML()
{
    if ( m_isInitialized )
    {
        m_pUpgradeDbxmlDifferenceSchema->loadFromXML(m_pRootElement);
        return true;
    }
    return false;
}


UpgradeDatabaseSchemaParser::~UpgradeDatabaseSchemaParser()
{
    if ( m_isInitialized )
    {
        XMLPlatformUtils::Terminate();
        delete  m_pUpgradeDbxmlDifferenceSchema;

        delete m_pParser;
    }
}

void UpgradeDatabaseSchemaParser::display()
{
    if ( m_isInitialized )
    {
        m_pUpgradeDbxmlDifferenceSchema->display();
    }
}

void UpgradeDatabaseSchemaParser::getSql(string& sql)
{
    if ( m_isInitialized )
    {
        clearMoSchemaInfoVectors();
        //loadPrismConfiguration();
        loadCurrentSchemaInfoObjects ();

        string generateSqlStatement;
        m_pUpgradeDbxmlDifferenceSchema->getSql(generateSqlStatement);
        string currentSchemaRepository = OrmRepository::getWaveCurrentSchema();
        string startSchemaRepository = OrmRepository::getWaveStartSchema();
        sql += generateSqlStatement;
        /* Handle Sql for WaveCurrentSchema */
        size_t pos;
        while ( (pos = generateSqlStatement.find(currentSchemaRepository))  != string::npos )
        {
            generateSqlStatement.replace(pos, currentSchemaRepository.size(), startSchemaRepository.c_str());//, startSchemaRepository.size());
        }
        sql += generateSqlStatement;
        printDifferencesInVectors();
        //savePrismConfiguration();
    }

}

bool UpgradeDatabaseSchemaParser::isPresentNewSchemaPointer(string& moName)
{
    map <string, UpgradeDbXmlElement*> ::iterator element = m_mapOfNewManagedObjects.find(moName);
    map <string, UpgradeDbXmlElement*> ::iterator endElement = m_mapOfNewManagedObjects.end();
    if ( element != endElement)
    {
        return (true);
    }
    return (false);
}

/*
 * This functtion is to cache the Schema pointers for new Managed Object which can be used by 
 * Dynamic Schema population Handler function to dynamically populate a MANAGED object.
 */
bool UpgradeDatabaseSchemaParser::addNewSchemaPointer(string& moName, UpgradeDbXmlElement* pSchema)
{
    if ( !isPresentNewSchemaPointer(moName))
    {
        m_mapOfNewManagedObjects[moName] = pSchema;
        return (true);
    }
    return (false);
}

UpgradeDbXmlElement* UpgradeDatabaseSchemaParser::getNewSchemaPointer(string& moName)
{
    UpgradeDbXmlElement* pSchema = NULL;
    map <string, UpgradeDbXmlElement*> ::iterator element = m_mapOfNewManagedObjects.find(moName);
    map <string, UpgradeDbXmlElement*> ::iterator endElement = m_mapOfNewManagedObjects.end();
    if ( element != endElement)
    {
        pSchema = element->second;
    }
    return (pSchema);
        
}

/*clear my moSchemaInfoObjects*/
bool UpgradeDatabaseSchemaParser::clearMoSchemaInfoVectors()
{
    m_managedObjectNames.clear();
    m_managedObjectFieldNamesStrings.clear();
    m_managedObjectFieldTypesStrings.clear();
    m_classIds.clear();
    m_parentClassIds.clear();
    m_tableClassIdTuples.clear();
    m_userDefinedKeyCombinationsFieldNamesForSchemaChange.clear();
    m_userDefinedKeyCombinationsFieldTypesForSchemaChange.clear();
    m_managedObjectFieldExpandedTypesStrings.clear();
    m_isALocalManagedObjectInfo.clear();
    m_derivedFromClassNames.clear();

    return (true);
}


bool UpgradeDatabaseSchemaParser::removeSchemaField (string &moName, string& fieldName)
{
    for ( UI32 i=0; i < m_managedObjectNames.size(); i++ )
    {
        if ( moName.compare(m_managedObjectNames[i]) == 0 )
        {
            trace(TRACE_LEVEL_INFO, string("UpgradeDatabaseSchemaParser::removeSchemaField ManagedObjectName (" ) + moName +"," + m_managedObjectNames[i] + ")");
            vector<string> vecFieldName;
            vector<string> vecFieldType;
            vector<string> vecExtFieldType;

            tokenize (m_managedObjectFieldNamesStrings[i], vecFieldName, ':');
            tokenize (m_managedObjectFieldTypesStrings[i], vecFieldType, ':');
            tokenize (m_managedObjectFieldExpandedTypesStrings[i], vecExtFieldType, ':');

            for (UI32 j=0; j < vecFieldName.size(); j++ )
            {
                if ( 0 == vecFieldName[j].compare(fieldName) )
                {
                    trace(TRACE_LEVEL_INFO, string("UpgradeDatabaseSchemaParser::removeSchemaField Field Name(" ) + fieldName +"," + vecFieldName[j] + "," + vecExtFieldType[j] + ")");
                    vecFieldName.erase(vecFieldName.begin() + j );
                    vecFieldType.erase(vecFieldType.begin() + j );
                    vecExtFieldType.erase(vecExtFieldType.begin() + j );
                    string stringFieldName = vecFieldName[0];
                    string stringFieldType = vecFieldType[0];
                    string stringExtFieldType = vecExtFieldType[0];

                    for ( UI32 k=1; k<vecFieldName.size(); k++ )
                    {
                        stringFieldName += string(":")+vecFieldName[k];
/*
                    }
                    for (UI32 l=1; l <vecFieldType.size(); l++)
                    {
                        stringFieldType += string(":")+vecFieldType[l];
*/
                        stringFieldType += string(":")+vecFieldType[k];
                        stringExtFieldType += string(":")+vecExtFieldType[k];
                    }
                    m_managedObjectFieldNamesStrings[i] = stringFieldName;
                    m_managedObjectFieldTypesStrings[i] = stringFieldType;
                    m_managedObjectFieldExpandedTypesStrings[i] = stringExtFieldType;
                    trace(TRACE_LEVEL_INFO, string("Removed Schema field for  Moname:") + moName + string(" fieldName:" ) + m_managedObjectFieldNamesStrings[i] + string(" FieldType:") +  m_managedObjectFieldTypesStrings[i] );
                    return (true);
                }

            }
        }
    }
    return (false);
}

/*FIXME: <Pritee> Needs cahnges in this function modifySchemaField to support change in db_type/disableValication/canBeEmpty fields.
        Can be done when this is supported in db_conversion core logic.*/

bool UpgradeDatabaseSchemaParser::modifySchemaField(string &moName, string& fieldName, string &fieldType)
{
    for ( UI32 i=0; i < m_managedObjectNames.size(); i++ )
    {
        if ( moName.compare(m_managedObjectNames[i]) == 0 )
        {
            trace(TRACE_LEVEL_INFO, string("UpgradeDatabaseSchemaParser::modifySchemaField  ManagedObjectName (" ) + moName +"," + m_managedObjectNames[i] + ")");
            vector<string> vecFieldName;
            vector<string> vecFieldType;
            vector<string> vecExtFieldType;
            tokenize (m_managedObjectFieldNamesStrings[i], vecFieldName, ':');
            tokenize (m_managedObjectFieldTypesStrings[i], vecFieldType, ':');
            tokenize (m_managedObjectFieldExpandedTypesStrings[i], vecExtFieldType, ':');

            for ( UI32 j=0; j < vecFieldName.size(); j++ )
            {
                if ( 0 == vecFieldName[j].compare(fieldName) )
                {
                    trace(TRACE_LEVEL_INFO, string("UpgradeDatabaseSchemaParser::modifySchemaField Field Name(" ) + fieldName +"," + vecFieldName[j] + ")");
                    vecFieldType[j] = fieldType;
                    UpgradeDatabaseSchemaParser::modifyExtFieldType (vecExtFieldType[j], fieldType, 1);

                    string stringFieldName = vecFieldName[0];
                    string stringFieldType = vecFieldType[0];
                    string stringExtFieldType = vecExtFieldType[0];

                    for ( UI32 k=1; k<vecFieldName.size(); k++ )
                    {
                        stringFieldName += string(":")+vecFieldName[k];
                        stringFieldType += string(":")+vecFieldType[k];
                        stringExtFieldType += string(":")+vecExtFieldType[k];
                    }
                    m_managedObjectFieldNamesStrings[i] = stringFieldName;
                    m_managedObjectFieldTypesStrings[i] = stringFieldType;
                    m_managedObjectFieldExpandedTypesStrings[i] = stringExtFieldType;
                    trace(TRACE_LEVEL_INFO, string("Modified Schema field for  Moname:") + moName + string(" fieldName:" ) + m_managedObjectFieldNamesStrings[i] + string(" FieldType:") +  m_managedObjectFieldTypesStrings[i] + string(" fieldExtType:") + m_managedObjectFieldExpandedTypesStrings[i]);
                    return (true);
                }

            }
        }
    }
    return (false);
}

bool UpgradeDatabaseSchemaParser::addSchemaField(string &moName, string& fieldName, string &fieldType, string& extFieldTypeExt)
{
    for ( UI32 i=0; i < m_managedObjectNames.size(); i++ )
    {
        if ( moName.compare(m_managedObjectNames[i]) == 0 )
        {
            //m_managedObjectFieldNamesStrings[i] += fieldName + string (":");
            //m_managedObjectFieldTypesStrings[i] += fieldType + string (":");
            m_managedObjectFieldNamesStrings[i] += string (":") + fieldName;
            m_managedObjectFieldTypesStrings[i] += string (":") + fieldType;
            m_managedObjectFieldExpandedTypesStrings[i] += string (":") + fieldType + string("|") + extFieldTypeExt;
            trace(TRACE_LEVEL_INFO, string("New Schema field added for Moname:") + moName + string(" fieldName:" ) + m_managedObjectFieldNamesStrings[i] + string(" FieldType:") +  m_managedObjectFieldTypesStrings[i] +  string(" extFieldType:") + m_managedObjectFieldExpandedTypesStrings[i]);
            return (true);
        }
    }
    return (false);
}

UI32 UpgradeDatabaseSchemaParser::getMaxClassID()
{
    UI32  maxClassId=0;
    for ( UI32 i=0 ; i < m_classIds.size(); i++ )
    {
        if ( maxClassId <  m_classIds[i] )
            maxClassId = m_classIds[i];
    }
    return (maxClassId);
}

UI32  UpgradeDatabaseSchemaParser::getClassIDForManagedObject(string &moName)
{
    for ( UI32 i=0; i < m_managedObjectNames.size(); i++ )
    {
        if ( 0 == moName.compare(m_managedObjectNames[i]) )
        {
            return ( m_classIds[i]);
        }
    }
    return (0);

}

bool UpgradeDatabaseSchemaParser::addNewManagedObject(string &moName, vector<string> &vecFieldName, vector<string> &vecFieldType, string &inheritedFrom)
{
    if ( vecFieldType.size() != 0 && vecFieldName.size() != 0 )
    {
        m_managedObjectNames.push_back(moName);
        string stringFieldName = vecFieldName[0];
        string stringFieldType = vecFieldType[0];
        string stringExtFieldType = stringFieldType + string("|") + UpgradeDatabaseSchemaParser::getSqlType(stringFieldType) + string("|0|0");  

        for ( UI32 i=1; i<vecFieldName.size(); i++ )
        {
            stringFieldName += string(":")+vecFieldName[i];
            stringFieldType += string(":")+ vecFieldType[i];
            stringExtFieldType += string(":")+ vecFieldType[i] + string("|") + UpgradeDatabaseSchemaParser::getSqlType(vecFieldType[i]) + string("|0|0");
        }
/*
        stringFieldName += string(":");
        stringFieldType += string(":");
        stringExtFieldType += string(":");
*/

        m_managedObjectFieldNamesStrings.push_back(stringFieldName);
        m_managedObjectFieldTypesStrings.push_back(stringFieldType);
        m_managedObjectFieldExpandedTypesStrings.push_back(stringExtFieldType);
        UI32 classIdForNewMo = getMaxClassID() + 1;

        m_classIds.push_back(classIdForNewMo);
        m_parentClassIds.push_back(getClassIDForManagedObject(inheritedFrom));
        m_userDefinedKeyCombinationsFieldNamesForSchemaChange.push_back("");        
        m_userDefinedKeyCombinationsFieldTypesForSchemaChange.push_back("");        

        //FIXME <Pritee> : BIST needs to enhance to set these 2 values by user in xml file. 
        // right now hard-coding these values and should be set properly once BIST is enhanced to take these values from user.       
        m_isALocalManagedObjectInfo.push_back(true);
        m_derivedFromClassNames.push_back(inheritedFrom);

        string myMonaName = moName;
        m_tableClassIdTuples.push_back(make_pair(myMonaName,classIdForNewMo));
        trace(TRACE_LEVEL_INFO, string("New ManagedObject Added Moname:") + moName + string(" fieldName:" ) + stringFieldName + string(" FieldType:") + stringFieldType ); 

    }else{
        return (false);
    }
    return (true);

}

bool UpgradeDatabaseSchemaParser::removeOldManagedObjects(string &moName)
{
    for ( UI32 i=0; i< m_managedObjectNames.size(); i++ )
    {
        if ( 0 == m_managedObjectNames[i].compare(moName) )
        {
            m_managedObjectNames.erase(m_managedObjectNames.begin() + i );
            m_managedObjectFieldNamesStrings.erase(m_managedObjectFieldNamesStrings.begin() + i);
            m_managedObjectFieldTypesStrings.erase(m_managedObjectFieldTypesStrings.begin() + i);
            m_managedObjectFieldExpandedTypesStrings.erase(m_managedObjectFieldExpandedTypesStrings.begin() + i);
            m_classIds.erase(m_classIds.begin() + i );
            m_parentClassIds.erase(m_parentClassIds.begin() + i );
            m_userDefinedKeyCombinationsFieldNamesForSchemaChange.erase(m_userDefinedKeyCombinationsFieldNamesForSchemaChange.begin() + i );
            m_userDefinedKeyCombinationsFieldTypesForSchemaChange.erase(m_userDefinedKeyCombinationsFieldTypesForSchemaChange.begin() + i );
            m_isALocalManagedObjectInfo.erase(m_isALocalManagedObjectInfo.begin() + i );
            m_derivedFromClassNames.erase(m_derivedFromClassNames.begin() + i );
            //
            //The pair is not populated hence need not be part of instrumentation
            //
            //m_tableClassIdTuples.erase(m_tableClassIdTuples.begin() + i );
            return (true);

        }
    }
    return (false);
}

void UpgradeDatabaseSchemaParser::createLockFileForConfigurationFile()
{
    string cmdToCreateLockFile = "/bin/touch " + PrismFrameworkObjectManager::getLockFileForConfigurationFile();
    vector<string>  output;
    SI32 cmdStatus = 0;
    cmdStatus = FrameworkToolKit::systemCommandOutput ( cmdToCreateLockFile, output );
    if ( cmdStatus != 0 )
    {
        trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::createLockFileForConfigurationFile : cmdToCreateLockFile = ")+ cmdToCreateLockFile + " failed with error message : " + output[0]);
    }
}

void UpgradeDatabaseSchemaParser::removeLockFileForConfigurationFile()
{
    string          cmdToDeleteCFG  = "/bin/rm -rf " + PrismFrameworkObjectManager::getLockFileForConfigurationFile();
    vector<string>  output;
    SI32 cmdStatus = FrameworkToolKit::systemCommandOutput (cmdToDeleteCFG, output);
    if ( cmdStatus != 0 )
    {
        trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::removeLockFileForConfigurationFile  cmdToDeleteCFG = ")+ cmdToDeleteCFG + " failed with error message : " + output[0]);
    }
    
}

ResourceId UpgradeDatabaseSchemaParser::configurationWithFlock (const UI32 &operationType)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    FILE *pFile ;
    string lockFilename = PrismFrameworkObjectManager::getLockFileForConfigurationFile();

    //Acquire flock for all operation types
    pFile = fopen (lockFilename.c_str(),"w");

    if (pFile == NULL)
    {
        trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::configurationWithFlock : Unable to open the lockfile --> ") + lockFilename.c_str() );
        return (WAVE_MESSAGE_ERROR);
    }

    if (0 != flock( fileno(pFile), LOCK_EX ))
    {
        trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::configurationWithFlock : Unable to secure the lock on Configurationlockfile --> ") + lockFilename.c_str() );
        fclose( pFile );
        return (WAVE_MESSAGE_ERROR);
    }

    trace (TRACE_LEVEL_DEBUG, string("UpgradeDatabaseSchemaParser::configurationWithFlock : Acquired lock on Configurationlockfile --> ") + lockFilename.c_str() );

    switch ( operationType )
    {

        case SAVE_CONFIGURATION :
                                    {
                                        string   prismFrameworkConfigurationFileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
                                        status = m_prismFrameworkConfiguration.saveConfiguration (prismFrameworkConfigurationFileName);
                                        break;
                                    }

        case LOAD_CONFIGURATION :
                                    {
                                        string   prismFrameworkConfigurationFileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
                                        status = m_prismFrameworkConfiguration.loadConfiguration (prismFrameworkConfigurationFileName);
                                        break;
                                    }
        case BACKUP_CONFIGURATION : {
                                        string   prismFrameworkConfigurationFileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
                                        string cmdToCreateBackup = string("cp -f ") + prismFrameworkConfigurationFileName + " " + prismFrameworkConfigurationFileName + string(".BIST");

                                        vector<string>  output;
                                        SI32 cmdStatus = 0;
                                        cmdStatus = FrameworkToolKit::systemCommandOutput (cmdToCreateBackup, output);
                                        if ( cmdStatus != 0 )
                                        {   
                                            trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::configurationWithFlock unable to create a backup of the configuration File")); 
                                            status = WAVE_MESSAGE_ERROR;
                                        } else {
                                            status = WAVE_MESSAGE_SUCCESS;
                                        }
                                    }
                                    break;
        case RESTORE_CONFIGURATION : 
                                    {
                                        string   prismFrameworkConfigurationFileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
                                        string cmdToRestoreBackup = string("cp -f ") + prismFrameworkConfigurationFileName + string(".BIST") + string(" ") + prismFrameworkConfigurationFileName;
                                        vector<string>  output;
                                        SI32 cmdStatus = 0;
                                        cmdStatus = FrameworkToolKit::systemCommandOutput (cmdToRestoreBackup, output);
                                        if ( cmdStatus != 0 )
                                        {
                                            trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::configurationWithFlock unable to restore the configuration file from backup"));
                                            status = WAVE_MESSAGE_ERROR;
                                        } else {
                                            status = WAVE_MESSAGE_SUCCESS;
                                        }
                                    }
                                    break;

        default :
                                        trace (TRACE_LEVEL_ERROR, string("UpgradeDatabaseSchemaParser::configurationWithFlock : Unknown operation type = ") + operationType );
                                        status = WAVE_MESSAGE_ERROR;
                                        break;

    }
    // Since the operation is complete release the flock
    flock( fileno(pFile), LOCK_UN );
    trace (TRACE_LEVEL_DEBUG, string("UpgradeDatabaseSchemaParser::configurationWithFlock : released lock on Configurationlockfile --> ") + lockFilename.c_str() );
    fclose( pFile );

    return (status);
}


bool UpgradeDatabaseSchemaParser::savePrismConfiguration()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    if ( true ==  FrameworkToolKit::isNodeZeroized ())
    {
        trace (TRACE_LEVEL_ERROR,"UpgradeDatabaseSchemaParser::validateStep: Failed to save cfg file as node is zeroized.");
        return (false);
    }
    createLockFileForConfigurationFile();
    m_prismFrameworkConfiguration.setSchemaChangeInfo (m_managedObjectNames, m_managedObjectFieldNamesStrings, m_managedObjectFieldTypesStrings, m_classIds, m_parentClassIds, m_userDefinedKeyCombinationsFieldNamesForSchemaChange, m_userDefinedKeyCombinationsFieldTypesForSchemaChange, m_managedObjectFieldExpandedTypesStrings);
    m_prismFrameworkConfiguration.setIsStartupValid(true);
    status = configurationWithFlock (SAVE_CONFIGURATION);
    removeLockFileForConfigurationFile();
    if ( WAVE_MESSAGE_SUCCESS == status )
    {
        return (true);
    }
    return (false);
}

void UpgradeDatabaseSchemaParser::generateSqlToPopulateUpgradeTable(string & sqlForUpgradeTablePopulation,const string & waveSchema)
{   
    UI32 numberOfTables = m_managedObjectNames.size();
    
    for (UI32 i = 0; i < numberOfTables; i++)
    {
        string tableName                    = m_managedObjectNames[i];
        string fieldNamesString             = m_managedObjectFieldNamesStrings[i];
        string fieldTypesString             = m_managedObjectFieldTypesStrings[i];
        UI32   tableClassId                 = m_classIds[i];
        UI32   parentTableId                = m_parentClassIds[i];
        string userDefinedFieldNamesString  = m_userDefinedKeyCombinationsFieldNamesForSchemaChange[i];
        string userDefinedFieldTypesString  = m_userDefinedKeyCombinationsFieldTypesForSchemaChange[i];
        string extendedFiledTypesString     = m_managedObjectFieldExpandedTypesStrings[i];
        bool   isALocalMO                   = m_isALocalManagedObjectInfo[i]; 
        string localManagedObjectInfo       = (true == isALocalMO)?"true":"false";
        string derivedFromClassName         = m_derivedFromClassNames[i];
        
        tracePrintf (TRACE_LEVEL_INFO, true, false, "UpgradeDatabaseSchemaParser::generateSqlToPopulateUpgradeTable: table:%s, classid: %u, parentClassid:%u, isALocalMO: %s, derivedFromClassName:%s", tableName.c_str(), tableClassId, parentTableId, localManagedObjectInfo.c_str(), derivedFromClassName.c_str());
        //tracePrintf (TRACE_LEVEL_DEBUG, true, false, "UpgradeDatabaseSchemaParser::generateSqlToPopulateUpgradeTable: table:%s, classid: %u, parentClassid:%u, isALocalMO: %u, derivedFromClassName:%s", tableName.c_str(), tableClassId, parentTableId, isALocalMO, derivedFromClassName);
            
        vector<string> fieldNames;
        vector<string> fieldTypes;
        vector<string> userDefinedFieldNames;
        vector<string> userDefinedFieldTypes;
        vector<string> extendedFieldTypes;

        tokenize (fieldNamesString, fieldNames, ':');
        tokenize (fieldTypesString, fieldTypes, ':');
        tokenize (userDefinedFieldNamesString, userDefinedFieldNames, ':');
        tokenize (userDefinedFieldTypesString, userDefinedFieldTypes, ':');
        tokenize (extendedFiledTypesString, extendedFieldTypes, ':');

        AttributeStringVector fieldNameAttr (fieldNames);
        AttributeStringVector fieldTypeAttr (fieldTypes);
        AttributeStringVector userDefinedFieldNameAttr (userDefinedFieldNames);
        AttributeStringVector userDefinedFieldTypeAttr (userDefinedFieldTypes);
        AttributeStringVector extendedFieldTypeattr (extendedFieldTypes);

        fieldNameAttr.toEscapedString (fieldNamesString);
        fieldTypeAttr.toEscapedString (fieldTypesString);
        userDefinedFieldNameAttr.toEscapedString (userDefinedFieldNamesString);
        userDefinedFieldTypeAttr.toEscapedString (userDefinedFieldTypesString);
        extendedFieldTypeattr.toEscapedString (extendedFiledTypesString);

        sqlForUpgradeTablePopulation += "INSERT INTO " + waveSchema + "." + OrmRepository::getSchemaUpgradeTableName ();
        sqlForUpgradeTablePopulation += string(" (objectidinstanceid,classname,fieldnames,fieldtypes,classid,parentclassid,expandedfieldtypes,userdefinedfieldnames,userdefinedfieldtypes,isalocalmanagedObject, derivedfromclassname) VALUES (" );
        sqlForUpgradeTablePopulation += i + string(",");
        sqlForUpgradeTablePopulation += string("'") + tableName + string("',");
        sqlForUpgradeTablePopulation += string("'") + fieldNamesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + fieldTypesString + string("',");
        sqlForUpgradeTablePopulation += tableClassId + string(",");
        sqlForUpgradeTablePopulation += parentTableId + string (",");
        sqlForUpgradeTablePopulation += string("'") + extendedFiledTypesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + userDefinedFieldNamesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + userDefinedFieldTypesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + localManagedObjectInfo + string("',");
        sqlForUpgradeTablePopulation += string("'") + derivedFromClassName + string("');");
    }

    trace (TRACE_LEVEL_DEBUG, string ("SQL to populate upgradeMO: ") + sqlForUpgradeTablePopulation);
}

bool UpgradeDatabaseSchemaParser::backupPrismConfiguration()
{
    createLockFileForConfigurationFile();
    ResourceId status = configurationWithFlock (BACKUP_CONFIGURATION);
    removeLockFileForConfigurationFile();
    if ( WAVE_MESSAGE_SUCCESS == status )
    {
        return (true);
    }
    return (false);
}

bool UpgradeDatabaseSchemaParser::restorePrismConfiguration()
{
    createLockFileForConfigurationFile();
    ResourceId status = configurationWithFlock (RESTORE_CONFIGURATION);
    removeLockFileForConfigurationFile();
    if ( WAVE_MESSAGE_SUCCESS == status )
    {
        return (true);
    }

    return (false);
}

/*Load the configuration vectors*/
bool UpgradeDatabaseSchemaParser::loadPrismConfiguration()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    if (WAVE_MGMT_INTF_ROLE_SERVER != (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        // Wave client need not use flock mechanism to change configuration
        string prismFrameworkConfigurationFileName= (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
        status = m_prismFrameworkConfiguration.loadConfiguration (prismFrameworkConfigurationFileName);
    }
    else
    {
        status = configurationWithFlock (LOAD_CONFIGURATION);
    }

    if ( status ==  WAVE_MESSAGE_SUCCESS )
    {
        /* Get the SchemaInfoVectors for modification*/
        size_t numElements = m_prismFrameworkConfiguration.getSchemaChangeInfo (m_managedObjectNames, m_managedObjectFieldNamesStrings, m_managedObjectFieldTypesStrings, m_classIds, m_parentClassIds, m_userDefinedKeyCombinationsFieldNamesForSchemaChange, m_userDefinedKeyCombinationsFieldTypesForSchemaChange, m_managedObjectFieldExpandedTypesStrings);
        trace(TRACE_LEVEL_DEBUG, string ( "Number of Mo in configuration files") + numElements );

    } else {
        return (false);
    }

    return (true);
}

bool UpgradeDatabaseSchemaParser::loadCurrentSchemaInfoObjects ()
{
    trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::loadCurrentSchemaInfoObjects: load the schemaChangeInfo");
    UI32 numOfMo = getSchemaInfoObjectsFromUpgradeMO (m_managedObjectNames, m_managedObjectFieldNamesStrings, m_managedObjectFieldTypesStrings, m_classIds, m_parentClassIds, m_userDefinedKeyCombinationsFieldNamesForSchemaChange, m_userDefinedKeyCombinationsFieldTypesForSchemaChange, m_managedObjectFieldExpandedTypesStrings, m_isALocalManagedObjectInfo, m_derivedFromClassNames);

    return ((0 < numOfMo) ? false:true);
}
        

size_t UpgradeDatabaseSchemaParser::getSchemaInfoObjectsFromUpgradeMO (vector<string> & managedObjectNames, vector<string> & managedObjectFieldNamesStrings,vector<string> & managedObjectFieldTypesStrings,vector<UI32> & classIds, vector<UI32> & parentClassIds, vector<string> & userDefinedKeyCombinationsFieldNamesForSchemaChange, vector<string> & userDefinedKeyCombinationsFieldTypesForSchemaChange, vector<string> &fieldNamesExpandedTypesForSchemaChange, vector<bool>& isLocalMOInfo, vector<string>& derivedFromClassNames)
{
    trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::getSchemaInfoObjectsFromUpgradeMO: reading from upgradeMO in db.");
    // clearup the recevied mo schema info objects.
    managedObjectNames.clear();
    managedObjectFieldNamesStrings.clear();
    managedObjectFieldTypesStrings.clear();
    classIds.clear();
    parentClassIds.clear();
    userDefinedKeyCombinationsFieldNamesForSchemaChange.clear();
    userDefinedKeyCombinationsFieldTypesForSchemaChange.clear();
    fieldNamesExpandedTypesForSchemaChange.clear();
    isLocalMOInfo.clear();
    derivedFromClassNames.clear();

    // Received from upgradeMO from db for all MOs.
    vector<string>              managedObjectFieldNames;
    vector<string>              managedObjectFieldTypes;
    vector<string>              userDefinedFieldNames;
    vector<string>              userDefinedFieldTypes;
    vector<string>              extendedFieldTypes;

    // per MO
    vector<string>              moFieldNames;
    vector<string>              moFieldTypes;
    vector<string>              moUserDefinedFields;
    vector<string>              moUserDefinedTypes;
    vector<string>              moExtendedTypes;

    DatabaseSchema                    databaseSchema;

    if (FRAMEWORK_SUCCESS != OrmToolKit::getDatabaseSchema (databaseSchema))
    {
        return (0);
    }

    managedObjectNames = databaseSchema.getManagedObjectNames ();
    managedObjectFieldNames = databaseSchema.getFieldNamesStrings ();
    managedObjectFieldTypes = databaseSchema.getFieldNamesTypes ();
    classIds = databaseSchema.getClassIds ();
    parentClassIds = databaseSchema.getParentTableIds ();
    userDefinedFieldNames = databaseSchema.getUserDefinedKeyCombinationsFieldNames ();
    userDefinedFieldTypes = databaseSchema.getUserDefinedKeyCombinationsFieldTypes ();
    extendedFieldTypes = databaseSchema.getFieldNamesExpandedTypes ();
    isLocalMOInfo = databaseSchema.getIsLocalManagedObjectInfo ();
    derivedFromClassNames = databaseSchema.getDerivedFromClassNames ();

    const UI32 numTables = managedObjectNames.size ();

    for (UI32 i = 0; i < numTables; i++)
    {
        moFieldNames.clear();
        moFieldTypes.clear();
        moUserDefinedFields.clear();
        moUserDefinedTypes.clear();
        moExtendedTypes.clear();

        StringUtils::tokenizeStringWithCount (managedObjectFieldNames[i], moFieldNames);
        StringUtils::tokenizeStringWithCount (managedObjectFieldTypes [i], moFieldTypes);
        StringUtils::tokenizeStringWithCount (extendedFieldTypes [i], moExtendedTypes);

        string moFieldNameString = "";
        string moFieldTypesString = "";
        string moExtendedFieldTypesString = "";

        if (0 < moFieldNames.size())
        {
            moFieldNameString = moFieldNames[0];
            moFieldTypesString = moFieldTypes[0];
            moExtendedFieldTypesString = moExtendedTypes[0];
        }

        for (UI32 j = 1; j < moFieldNames.size(); j++)
        {
            moFieldNameString += string(":") + moFieldNames[j];
            moFieldTypesString += string(":") + moFieldTypes[j];
            moExtendedFieldTypesString += string(":") + moExtendedTypes[j];
        }

        StringUtils::tokenizeStringWithCount (userDefinedFieldNames[i], moUserDefinedFields);
        StringUtils::tokenizeStringWithCount (userDefinedFieldTypes[i], moUserDefinedTypes);

        string moUserDefinedFieldsString = "";
        string moUserDefinedTypesString = "";

        if (0 < moUserDefinedFields.size())
        {
            moUserDefinedFieldsString = moUserDefinedFields[0];
            moUserDefinedTypesString = moUserDefinedTypes[0];
        }

        for (UI32 j = 1; j < moUserDefinedFields.size(); j++)
        {
            moUserDefinedFieldsString += string(":") +  moUserDefinedFields[j];
            moUserDefinedTypesString += string(":") + moUserDefinedTypes[j];
        }
        
        managedObjectFieldNamesStrings.push_back(moFieldNameString);
        managedObjectFieldTypesStrings.push_back(moFieldTypesString);
        userDefinedKeyCombinationsFieldNamesForSchemaChange.push_back(moUserDefinedFieldsString);
        userDefinedKeyCombinationsFieldTypesForSchemaChange.push_back(moUserDefinedTypesString);
        fieldNamesExpandedTypesForSchemaChange.push_back(moExtendedFieldTypesString);

        
/*
        trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::getSchemaInfoObjectsFromUpgradeMO: managedObjectName : " + managedObjectNames[i]);
        trace (TRACE_LEVEL_INFO, "                                                              : managedObjectFieldNamesString : " + moFieldNameString);
        trace (TRACE_LEVEL_INFO, "                                                              : managedObjectFieldTypesString : " + moFieldTypesString);
        trace (TRACE_LEVEL_INFO, "                                                              : userDefinedKeyCombinationsFieldNames : " + moUserDefinedFieldsString);
        trace (TRACE_LEVEL_INFO, "                                                              : userDefinedKeyCombinationsFieldTypes : " + moUserDefinedTypesString);
        trace (TRACE_LEVEL_INFO, "                                                              : fieldNamesExpandedTypes : " + moExtendedFieldTypesString);
*/
    }

    return (numTables);
}
    

/*FIXME: <Pritee> This function printDifferencesInVectors needs extensive changes.
        As this function is not needed for core function of BIsT, not making them now.*/
void UpgradeDatabaseSchemaParser::printDifferencesInVectors()
{
    string prismFrameworkConfigurationFileName= (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    PrismFrameworkConfiguration prismFrameworkConfiguration;
    prismFrameworkConfiguration.loadConfiguration (prismFrameworkConfigurationFileName);
    vector<string>                              managedObjectNames;
    vector<string>                              managedObjectFieldNamesStrings;
    vector<string>                              managedObjectFieldTypesStrings;
    vector<UI32>                                classIds;
    vector<UI32>                                parentClassIds;
    vector<string>                              userDefinedKeyCombinationsFieldNames;
    vector<string>                              userDefinedKeyCombinationsFieldTypes;
    vector<string>                              managedObjectFieldExpandedTypesStrings;
    vector<bool>                                isALocalMOInfo;
    vector<string>                              derivedFromClassNames;
    //vector<pair<string, UI32> >                 tableClassIdTuples;

    //size_t numElements = prismFrameworkConfiguration.getSchemaChangeInfo (managedObjectNames, managedObjectFieldNamesStrings, managedObjectFieldTypesStrings, classIds, parentClassIds, userDefinedKeyCombinationsFieldNames, userDefinedKeyCombinationsFieldTypes, managedObjectFieldExpandedTypesStrings);
    size_t numElements = getSchemaInfoObjectsFromUpgradeMO (managedObjectNames, managedObjectFieldNamesStrings, managedObjectFieldTypesStrings, classIds, parentClassIds, userDefinedKeyCombinationsFieldNames, userDefinedKeyCombinationsFieldTypes, managedObjectFieldExpandedTypesStrings, isALocalMOInfo, derivedFromClassNames);


    UI32 j=0;
    for(UI32 i=0; (i < numElements) && (j < m_managedObjectNames.size()); i++,j++ )
    {
        if ( managedObjectNames[i] == m_managedObjectNames[j] )
        {
            vector <string> vecFieldName;
            vector <string> vecFieldType;
            vector <string> vecFieldName1;
            vector <string> vecFieldType1;
            tokenize(managedObjectFieldNamesStrings[i],vecFieldName,':');
            tokenize(managedObjectFieldTypesStrings[i],vecFieldType,':');
            tokenize(m_managedObjectFieldNamesStrings[j],vecFieldName1,':');
            tokenize(m_managedObjectFieldTypesStrings[j],vecFieldType1,':');
            UI32 m=0;
            for(UI32 l=0; (l < vecFieldName.size()) && (m < vecFieldName1.size()); l++ ,m++)
            {
                if ( vecFieldName[l] == vecFieldName1[m] )
                {
                    if ( vecFieldType[l] != vecFieldType1[m] )
                    {
                        trace(TRACE_LEVEL_DEBUG, string("Column Type Changes "));
                        trace(TRACE_LEVEL_DEBUG, string("Managed Object Name :") +  managedObjectNames[i] );
                        trace(TRACE_LEVEL_DEBUG, string("Field Type changed From :") + vecFieldType[l] + "->" +  vecFieldType1[m] );

                    }
                } else {

                    trace(TRACE_LEVEL_DEBUG, string("Column removed" ) );
                    trace(TRACE_LEVEL_DEBUG, string("Managed Object Name :") +  managedObjectNames[i] );
                    trace(TRACE_LEVEL_DEBUG, string(" FieldName " ) + vecFieldName[l] + vecFieldName1[m] );
                    m--;
                }
            }

            for ( ; m< vecFieldName1.size(); m++ )
            {
                trace(TRACE_LEVEL_DEBUG, string("Column Added" ));
                trace(TRACE_LEVEL_DEBUG, string("Managed Object Name :") +  managedObjectNames[i] );
                trace(TRACE_LEVEL_DEBUG, string(" Field Name: ") +  vecFieldName1[m] + " \t -> \t  Field Type: " + vecFieldType1[m] );
            }
            // Identify the differences in fields
        } else {
            trace(TRACE_LEVEL_DEBUG, string ("Following Mo details are not present after instrumentation:" ));
            trace(TRACE_LEVEL_DEBUG, string("Managed Object Name :") +  managedObjectNames[i] + string("->") + m_managedObjectNames[j] ); 
            vector <string> vecFieldName;
            vector <string> vecFieldType;
            tokenize(managedObjectFieldNamesStrings[i],vecFieldName,':');
            tokenize(managedObjectFieldTypesStrings[i],vecFieldType,':');
            for(UI32 l=0; l < vecFieldName.size(); l++ )
            {
                trace(TRACE_LEVEL_DEBUG, string(" Field Name: ") +  vecFieldName[l] + string(" \t -> \t  Field Type: ") + vecFieldType[l] );
            }

            /* repeat the search for the same managedObject from next in the list*/
            j--;
        }

        // *Identify the differences in the Schema and fille the details for the tracefile verification.
    }
    for (;j< m_managedObjectNames.size() ; j++ )
    {
        trace(TRACE_LEVEL_DEBUG, string ("Following Mo details are Additions after instrumentation:" ));
        trace(TRACE_LEVEL_DEBUG, string("Managed Object Name :") +  m_managedObjectNames[j] );
        vector <string> vecFieldName;
        vector <string> vecFieldType;
        tokenize(m_managedObjectFieldNamesStrings[j], vecFieldName,':');
        tokenize(m_managedObjectFieldTypesStrings[j], vecFieldType,':');
        for(UI32 l=0; l < vecFieldName.size(); l++ )
        {
            trace(TRACE_LEVEL_DEBUG, string(" Field Name: ") +  vecFieldName[l] + " \t -> \t  Field Type: " + vecFieldType[l] );
        }

    }

}


string UpgradeDatabaseSchemaParser::getSqlType (string waveType)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance();
    string sqlType = pOrmRepository->getSqlStringForAttributeTypeString(waveType);
    return (sqlType);
}

/*
 * This function should be called only after the schema MO vectors are modified
 */
bool UpgradeDatabaseSchemaParser::isAnyDerivationsTabledNeeded(string &tableName, string &fieldName, string &relatedTo)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance();
    OrmTable *pOrmTable = pOrmRepository->getTableByName(tableName);
    if ( pOrmTable )
    {
        vector<OrmTable *> derivedTables;
        vector<OrmRelation *> relationColumn;
        pOrmTable->getDeivedTablesAndRelations(derivedTables, relationColumn);

        UI32 i=0;

        for ( i=0; i <relationColumn.size(); i++ )
        {
            if ( relationColumn[i]->getName() ==  fieldName )
            {
                relatedTo = relationColumn[i]->getRelatedTo();
            }
        }
        /*
         * Handle derivations table 
         */

        for ( i=0; i < derivedTables.size(); i++ )
        {
            for ( UI32 j=0 ; j <  m_managedObjectNames.size(); j++)
            {
                if (  m_managedObjectNames[j] == derivedTables[i]->getName() )
                {
					trace(TRACE_LEVEL_INFO, string ("derived table exists for Mo ") + tableName + " For derived MO " + m_managedObjectNames[j]);
                    return (true);
                }
            }
        }

        /*
         * Handle non existent FROM end of a relation
         */
        for ( i=0; i <relationColumn.size(); i++ )
        {
            for ( UI32 j=0 ; j <  m_managedObjectNames.size(); j++)
            {
                if (  m_managedObjectNames[j] == tableName )
                {
                    size_t found = string::npos;
                    string searchString = string(":") + relationColumn[i]->getName() + string(":") ;
                    found = m_managedObjectFieldNamesStrings[j].find(searchString);
                    if ( found != string::npos )
                    {
						trace(TRACE_LEVEL_INFO, string ("Relations exists for MO ") + tableName + " With Mo " + relationColumn[i]->getRelatedTo());
                        return (true);
                    }
                }

            }
        }

        /*
         * Handle To End of a relation
         */
        for ( i=0; i < m_managedObjectFieldTypesStrings.size(); i++ )
        {
            size_t found = string::npos;
            string searchString = string("-") + tableName + ":";
            found = m_managedObjectFieldTypesStrings[i].find(searchString);
            if ( found != string::npos )
            {
				trace(TRACE_LEVEL_INFO, string ("Relations exists for MO ") + tableName + " For Mo " +  m_managedObjectNames[i] );
                return (true);
            }

        }


    }
    return (false);
}

bool UpgradeDatabaseSchemaParser::addRelationField(string &moName, string& fieldName, string &fieldType, string &extFieldType, string &relatedTo)
{
    if (fieldType == "WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR" ) 
    {
        fieldType = "c-m-" + relatedTo;
        UpgradeDatabaseSchemaParser::addSchemaField(moName, fieldName, fieldType, extFieldType);

            
    }else if (fieldType == "WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR" )
    {
        fieldType = "a-m-" + relatedTo;
        UpgradeDatabaseSchemaParser::addSchemaField(moName, fieldName, fieldType, extFieldType);

    } else if ( fieldType == "WAVE_ATTRIBUTE_TYPE_OBJECTID"  || ( fieldType == "WAVE_ATTRIBUTE_TYPE_COMPOSITION" ))
    {
        string columnClassId = fieldName + "classid";
        string columnInstanceID = fieldName + "instanceid";
        string fieldTypeToUse = "WAVE_ATTRIBUTE_TYPE_OBJECTID";
        UpgradeDatabaseSchemaParser::addSchemaField(moName, columnClassId, fieldTypeToUse,extFieldType);
        UpgradeDatabaseSchemaParser::addSchemaField(moName, columnInstanceID, fieldTypeToUse , extFieldType);

    }
    return (true);
}

string   UpgradeDatabaseSchemaParser::getSqlForRelations(string &moName, string& fieldName, string &fieldType, string &relatedTo)
{
    string sql="";
    string alterSql = "" ;
    string waveSchema = OrmRepository::getWaveCurrentSchema();
    if ((fieldType == "WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR" ) || (fieldType == "WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR" ) )
    {
        UI32 tableIdFrom = getClassIDForManagedObject(moName);
        UI32 tableIdTo = getClassIDForManagedObject(relatedTo);
        string auxilliaryTableName= string("t") + tableIdFrom + fieldName + string("t") + tableIdTo ;
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + moName + "Derivations\n(\n    objectIdClassId integer PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + moName + "DerivationsInstances\n(\n   objectIdInstanceId bigint PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + relatedTo+ "Derivations\n(\n    objectIdClassId integer PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + relatedTo+ "DerivationsInstances\n(\n   objectIdInstanceId bigint PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + auxilliaryTableName +"\n(\n    ownerclassid integer,\n ownerinstanceid bigint,\n   relatedtoclassid int,\n relatedtoinstanceid bigint\n);\n";
        alterSql += "ALTER TABLE " + waveSchema+ "." + auxilliaryTableName + " ADD CONSTRAINT " + auxilliaryTableName+ "_ownerclassid_fkey FOREIGN KEY (ownerclassid) REFERENCES "
        +   waveSchema+ "." + moName +"derivations(objectidclassid) DEFERRABLE INITIALLY DEFERRED ;\n";
        alterSql += "ALTER TABLE " + waveSchema+ "." + auxilliaryTableName + " ADD CONSTRAINT " + auxilliaryTableName+ "_ownerinstanceid_fkey FOREIGN KEY (ownerinstanceid) REFERENCES " + waveSchema+ "." + moName +"derivationsinstances(objectidinstanceid) DEFERRABLE INITIALLY DEFERRED;\n";
        alterSql += "ALTER TABLE " + waveSchema+ "." + auxilliaryTableName + " ADD CONSTRAINT " + auxilliaryTableName + "_relatedtoclassid_fkey  FOREIGN KEY (relatedtoclassid) REFERENCES " + waveSchema+ "." + relatedTo +"derivations(objectidclassid) DEFERRABLE INITIALLY DEFERRED ;\n";
        alterSql += "ALTER TABLE " + waveSchema+ "." + auxilliaryTableName + " ADD CONSTRAINT " + auxilliaryTableName + "_relatedtoinstanceid_fkey FOREIGN KEY (relatedtoinstanceid) REFERENCES " +  waveSchema+ "." + relatedTo +"derivationsinstances(objectidinstanceid) DEFERRABLE INITIALLY DEFERRED;\n";
        alterSql += "INSERT INTO " + waveSchema + "." + moName + "Derivations (objectIdClassId) SELECT objectIdClassId FROM " + waveSchema + "." + moName + "Derivations UNION VALUES (" + tableIdFrom +") EXCEPT SELECT objectIdClassId FROM " + waveSchema + "." + moName + "Derivations;\n";
    } else if ( fieldType == "WAVE_ATTRIBUTE_TYPE_OBJECTID"  || ( fieldType == "WAVE_ATTRIBUTE_TYPE_COMPOSITION" ))
    {
        UI32 tableIdFrom = getClassIDForManagedObject(moName);
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + moName + "Derivations\n(\n    objectIdClassId integer PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + moName + "DerivationsInstances\n(\n   objectIdInstanceId bigint PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + relatedTo+ "Derivations\n(\n    objectIdClassId integer PRIMARY KEY\n);";
        alterSql += "CREATE TABLE IF NOT EXISTS " + waveSchema + "." + relatedTo+ "DerivationsInstances\n(\n   objectIdInstanceId bigint PRIMARY KEY\n);";
        alterSql += "ALTER TABLE " + waveSchema + "." + moName + " ADD COLUMN " + fieldName + "classid  integer;\n";
        alterSql += "ALTER TABLE " + waveSchema + "." + moName +"  ADD COLUMN " + fieldName + "instanceid bigint;\n";
        alterSql += "ALTER TABLE " + waveSchema + "." + moName +"  ADD  CONSTRAINT " + moName + "_" +  fieldName + "classid_fkey  FOREIGN KEY (" + fieldName + "classid ) REFERENCES " + waveSchema + "." + relatedTo+ "derivations(objectidclassid) DEFERRABLE INITIALLY DEFERRED;\n";
        alterSql += "ALTER TABLE " + waveSchema + "." + moName +"  ADD  CONSTRAINT " + moName + "_" + fieldName + "instanceid_fkey  FOREIGN KEY (" + fieldName + "instanceid ) REFERENCES " + waveSchema + "." + relatedTo+ "derivationsinstances(objectidinstanceid) DEFERRABLE INITIALLY DEFERRED;\n";
        alterSql += "INSERT INTO " + waveSchema + "." + moName + "Derivations (objectIdClassId) SELECT objectIdClassId FROM " + waveSchema + "." + moName + "Derivations UNION VALUES (" + tableIdFrom +") EXCEPT SELECT objectIdClassId FROM " + waveSchema + "." + moName + "Derivations;\n";

    }
    return (alterSql);
}


bool UpgradeDatabaseSchemaParser::modifyExtFieldType (string &extFieldType, string& newValue, UI8 pos)
{
    if (pos <= 0 || pos > 4)
    {
        trace (TRACE_LEVEL_INFO, string("UpgradeDatabaseSchemaParser::modifyExtFieldType: invalid value for position. Acceptable value is [1-4]"));
        return (false);
    }

    trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::modifyExtFieldType: extFieldType = " + extFieldType + ", newValue = " + newValue + ", position = " + pos);
    
    vector<string> fieldTypesValues;
    tokenize (extFieldType, fieldTypesValues, '|');

    trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::modifyExtFieldType: changing [" + fieldTypesValues[pos-1] + "] to [" + newValue + "]");

    fieldTypesValues[pos-1] = newValue;

    extFieldType = fieldTypesValues[0];
    for (UI8 i = 1; i < fieldTypesValues.size (); i++)
    { 
        extFieldType += "|" + fieldTypesValues[i];
    }

    trace (TRACE_LEVEL_INFO, "UpgradeDatabaseSchemaParser::modifyExtFieldType: new extFieldType = " + extFieldType);

    return true;
}

}

