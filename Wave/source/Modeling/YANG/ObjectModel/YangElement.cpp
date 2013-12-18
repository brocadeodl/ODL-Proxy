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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

#include "Modeling/YANG/ObjectModel/YangFactory/YangElementFactory.h"
#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Modeling/YANG/ObjectModel/YangUses.h"
#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "Modeling/YANG/ObjectModel/YangAugment.h"
#include "Modeling/YANG/ObjectModel/YangConfig.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangStatus.h"
#include "Modeling/YANG/ObjectModel/YangContainer.h"
#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Modeling/YANG/ObjectModel/YangRefine.h"

#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Modeling/YANG/ObjectModel/YangEnum.h"
#include "Modeling/YANG/ObjectModel/YangTypedef.h"
#include "Modeling/YANG/ObjectModel/YangValue.h"
#include "Modeling/YANG/ObjectModel/YangBit.h"
#include "Modeling/YANG/ObjectModel/YangPosition.h"
#include "Modeling/YANG/ObjectModel/YangChoice.h"
#include "Modeling/YANG/ObjectModel/YangCase.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Modeling/YANG/ObjectModel/YangGrouping.h"
#include "Modeling/YANG/ObjectModel/YangRpc.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <ctype.h>

#include <algorithm>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace WaveNs
{

UI32        YangElement::s_numberOfInstancesOfYangElement           = 0;
PrismMutex  YangElement::s_numberOfInstancesOfYangElementMutex;

YangElement::YangElement (const string &yangName, const string &name)
    : m_yangName                                   (yangName),
      m_name                                       (name),
      m_displayName                                (name),
      m_pParentElement                             (NULL),
      m_attributeQuotingCharacter                  ('"'),
      m_pOwnerYangModule                           (NULL),
      m_pOriginalOwnerYangModule                   (NULL),
      m_userTagValue                               (0),
      m_isConfigurationSegmentRoot                 (false),
      m_isConfiguration                            (false),
      m_isConfigurationExplicitlySet               (false),
      m_isDeprecated                               (false),
      m_willBeShownInConfigurationTree             (false),
      m_isFirstDataElementInHierarchy              (false),
      m_annotationBitMap                           (0),
      m_annotationFlags                            (),
      m_cliTargetNodeNameForDropNode			   ()
{
    setDefaultValues ();

    incrementNumberOfInstancesOfYangElementsInSystem ();
}

YangElement::~YangElement ()
{
    UI32 numberOfChildElements = m_childElements.size ();
    UI32 i                     = 0;

    for (i = 0; i < numberOfChildElements; i++)
    {
        delete m_childElements[i];
    }

    decrementNumberOfInstancesOfYangElementsInSystem ();
}

void YangElement::setDefaultValues ()
{
    m_isNodeSpecificBaseList            = false;
    m_isMultiPartitionBaseList          = false;
    m_underNodeSpecificTree             = false;
    m_underMultiPartitionTree           = false;

    m_pConfigurationSegmentInformation  = NULL;

    m_isHidden                          = false;
    m_cameFromAugment                   = false;

    m_isInLocalConfigurationSubtree     = false;
}

string YangElement::getYangName () const
{
    return (m_yangName);
}

void YangElement::setYangName (const string &yangName)
{
    m_yangName = yangName;
}

string YangElement::getName () const
{
    return (m_name);
}

void YangElement::setName (const string &name)
{
    m_name = name;
}

string YangElement::getDisplayName () const
{
    return (m_displayName);
}

void YangElement::setDisplayName (const string &displayName)
{
    m_displayName = displayName;
}

string YangElement::getValue () const
{
    return (m_value);
}

void YangElement::setValue (const string &value)
{
    m_value = value;
}

void YangElement::addValue (const string &value)
{
    m_value += value;
}

void YangElement::loadValue (const string &value)
{
}

YangElement *YangElement::getPParentElement () const
{
    return (m_pParentElement);
}

void YangElement::setPParentElement (YangElement * const pParentElement)
{
    m_pParentElement = pParentElement;
}

vector<YangElement *> &YangElement::getChildElements ()
{
    return (m_childElements);
}

bool YangElement::getIsInLocalConfigurationSubtree () const
{
    return m_isInLocalConfigurationSubtree;
}

void YangElement::setIsInLocalConfigurationSubtree ()
{
    m_isInLocalConfigurationSubtree = true;
}

void YangElement::addChildElement (YangElement * pChildElement)
{
    YangElement *pParentElement = pChildElement->getPParentElement ();

    if (NULL != pParentElement)
    {
        const string existingParentElementName = pParentElement->getName ();

        trace (TRACE_LEVEL_FATAL, "YangElement::addChildElement : A parent has already been set for this child.  Child : " + pChildElement->getName () + ", Parent : " + existingParentElementName + ".");
        prismAssert (false, __FILE__, __LINE__);

        return;
    }
    else
    {
        const string childElementYangName    = pChildElement->getYangName    ();
        const string childElementDisplayName = pChildElement->getDisplayName ();
        const string childElementName        = pChildElement->getName        ();

        // Processing for containers with same name

        if ((YangContainer::getYangName()) == childElementYangName)
        {
            YangElement *pSimilarChildElement = getSimilarChildElement (pChildElement);

            if (NULL != pSimilarChildElement)
            {
                trace (TRACE_LEVEL_INFO, string ("YangElement::addChildElement : merged container [") + pChildElement->getOriginalModuleNameSpaceUri () + string (":") + pChildElement->getName () + string ("] into -"));
                trace (TRACE_LEVEL_INFO, string ("YangElement::addChildElement :                  [") + pSimilarChildElement->getOriginalModuleNameSpaceUri () + string (":") + pSimilarChildElement->getName () + string ("]"));

                vector<YangElement *> clonedChildrenForSimilarChildelement;

                pChildElement->getClonedChildren (clonedChildrenForSimilarChildelement);

                YangModule *pOwnerYangModule       = m_pOwnerYangModule;
                UI32        numberOfClonedChildren = clonedChildrenForSimilarChildelement.size ();
                UI32        j                      = 0;

                for (j = 0; j < numberOfClonedChildren; j++)
                {
                    YangElement *pClonedChildElement = clonedChildrenForSimilarChildelement[j];

                    prismAssert (NULL != pClonedChildElement, __FILE__, __LINE__);

                    pClonedChildElement->setPOwnerYangModuleForTree (pOwnerYangModule);
                }

                pSimilarChildElement->addChildElements (clonedChildrenForSimilarChildelement);

                delete pChildElement;

                return;
            }
        }

        pChildElement->setPParentElement   (this);
        pChildElement->setPOwnerYangModule (m_pOwnerYangModule);

        m_childElements.push_back (pChildElement);

        m_childElementsMapByYangName[childElementYangName].push_back (pChildElement);
        m_childElementsMapByDisplayName[childElementDisplayName][pChildElement] = pChildElement;
        m_childElementsMapByName[childElementName][pChildElement]               = pChildElement;

        processChildElement (pChildElement);
    }
}

void YangElement::addChildElements (vector<YangElement *> &childElements)
{
    UI32 numberOfChildElements = childElements.size ();
    UI32 i                     = 0;

    for (i = 0; i < numberOfChildElements; i++)
    {
        addChildElement (childElements[i]);
    }
}

void YangElement::addChildElementsBefore (YangElement *pChildElement, vector<YangElement *> &newChildElements)
{
    vector<YangElement *>::iterator element    = std::find (m_childElements.begin (), m_childElements.end (), pChildElement);
    vector<YangElement *>::iterator endElement = m_childElements.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    m_childElements.insert (element, newChildElements.begin (), newChildElements.end ());

    UI32 numberOfNewChildElements = newChildElements.size ();
    UI32 i                        = 0;

    for (i = 0; i < numberOfNewChildElements; i++)
    {
        YangElement *pNewChildElement = newChildElements[i];

        prismAssert (NULL != pNewChildElement, __FILE__, __LINE__);

        const string       childElementYangName    = pNewChildElement->getYangName    ();
        const string       childElementDisplayName = pNewChildElement->getDisplayName ();
        const string       childElementName        = pNewChildElement->getName        ();

        // Processing for containers with same name

        if ((YangContainer::getYangName()) == childElementYangName)
        {
            YangElement *pSimilarChildElement = getSimilarChildElement (pNewChildElement);

            if (NULL != pSimilarChildElement)
            {
                trace (TRACE_LEVEL_INFO, string ("YangElement::addChildElementsBefore : merged container [") + pNewChildElement->getOriginalModuleNameSpaceUri () + string (":") + pNewChildElement->getName () + string ("] into -"));
                trace (TRACE_LEVEL_INFO, string ("YangElement::addChildElementsBefore :                  [") + pSimilarChildElement->getOriginalModuleNameSpaceUri () + string (":") + pSimilarChildElement->getName () + string ("]"));

                vector<YangElement *> clonedChildrenForSimilarChildelement;

                pNewChildElement->getClonedChildren (clonedChildrenForSimilarChildelement);

                YangModule *pOwnerYangModule       = m_pOwnerYangModule;
                UI32        numberOfClonedChildren = clonedChildrenForSimilarChildelement.size ();
                UI32        j                      = 0;

                for (j = 0; j < numberOfClonedChildren; j++)
                {
                    YangElement *pClonedChildElement = clonedChildrenForSimilarChildelement[j];

                    prismAssert (NULL != pClonedChildElement, __FILE__, __LINE__);

                    pClonedChildElement->setPOwnerYangModuleForTree (pOwnerYangModule);
                }

                pSimilarChildElement->addChildElements (clonedChildrenForSimilarChildelement);

                removeChildElement (pNewChildElement);

                delete pNewChildElement;

                continue;
            }
        }

        pNewChildElement->setPParentElement          (this);
        pNewChildElement->setPOwnerYangModuleForTree (m_pOwnerYangModule);

        m_childElementsMapByYangName[childElementYangName].push_back (pNewChildElement);
        m_childElementsMapByDisplayName[childElementDisplayName][pNewChildElement] = pNewChildElement;
        m_childElementsMapByName[childElementName][pNewChildElement]               = pNewChildElement;

        processChildElement (pNewChildElement);
    }
}

void YangElement::removeChildElement (YangElement * pChildElement)
{
    const string childElementYangName    = pChildElement->getYangName    ();
    const string childElementDisplayName = pChildElement->getDisplayName ();
    const string childElementName        = pChildElement->getName        ();

    unprocessChildElement (pChildElement);

    m_childElementsMapByName[childElementName].erase (pChildElement);
    m_childElementsMapByDisplayName[childElementDisplayName].erase (pChildElement);

    vector<YangElement *> &childElementsVectorByYangName            = m_childElementsMapByYangName[childElementYangName];
    vector<YangElement *>::iterator childElementByYangNameElement   = std::find (childElementsVectorByYangName.begin (), childElementsVectorByYangName.end (), pChildElement);
    vector<YangElement *>::iterator childElementByYangNameEnd       = childElementsVectorByYangName.end ();

    if (childElementByYangNameEnd != childElementByYangNameElement)
    {
        childElementsVectorByYangName.erase (childElementByYangNameElement);
    }

    vector<YangElement *>::iterator element    = std::find (m_childElements.begin (), m_childElements.end (), pChildElement);
    vector<YangElement *>::iterator endElement = m_childElements.end ();

    if (endElement != element)
    {
        m_childElements.erase (element);
    }
}

void YangElement::removeAllChildElementsByYangName (const string &yangName)
{
    vector<YangElement *> allChildElementsByYangName;
    UI32                  numberOfChildElementsByYangName = 0;
    UI32                  i                               = 0;

    getAllChildrenByYangName (yangName, allChildElementsByYangName);

    numberOfChildElementsByYangName = allChildElementsByYangName.size ();

    for (i = 0; i < numberOfChildElementsByYangName; i++)
    {
        removeChildElement (allChildElementsByYangName[i]);
        delete (allChildElementsByYangName[i]);
    }
}

void YangElement::removeAllChildElements ()
{
    vector<YangElement *>::iterator childElementsIterator;

    for (childElementsIterator = m_childElements.begin (); childElementsIterator != m_childElements.end (); childElementsIterator++)
    {
        removeChildElement (*childElementsIterator);
    }
}

void YangElement::loadFromDomElement (DOMElement *pDomElement)
{
    char        *pNodeName         = NULL;
    char        *pNodeValue        = NULL;
    char        *pAttributeName    = NULL;
    char        *pAttributeValue   = NULL;

    prismAssert (NULL != pDomElement, __FILE__, __LINE__);

    DOMElement  *pRootNode  = pDomElement;
    DOMNode     *pChildNode = NULL;

    pNodeName = XMLString::transcode (pRootNode->getNodeName ());

    //cout << "Node Name : " << pNodeName << "\n";

    if (0 != (strcasecmp (getYangName ().c_str (), pNodeName)))
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    XMLString::release (&pNodeName);

    DOMNamedNodeMap *pAttributesMap = pRootNode->getAttributes ();

    if (NULL != pAttributesMap)
    {
        UI32 numberOfAttributes = pAttributesMap->getLength ();
        UI32 i                  = 0;

        for (i = 0; i < numberOfAttributes; i++)
        {
            DOMNode *pAttributeDomNode = pAttributesMap->item (i);

            prismAssert (NULL != pAttributeDomNode, __FILE__, __LINE__);

            DOMAttr *pAttribute = dynamic_cast<DOMAttr *> (pAttributeDomNode);

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            const XMLCh *pAttributeNameXmlChar  = pAttribute->getName  ();
            const XMLCh *pAttributeValueXmlChar = pAttribute->getValue ();

            prismAssert (NULL != pAttributeNameXmlChar,  __FILE__, __LINE__);
            prismAssert (NULL != pAttributeValueXmlChar, __FILE__, __LINE__);

            pAttributeName  = XMLString::transcode (pAttributeNameXmlChar);
            pAttributeValue = XMLString::transcode (pAttributeValueXmlChar);

            prismAssert (NULL != pAttributeName,  __FILE__, __LINE__);
            prismAssert (NULL != pAttributeValue, __FILE__, __LINE__);

            addAttribute (pAttributeName, pAttributeValue);

            //cout << ">>>>>" << pAttributeName << "=" << pAttributeValue << "<<<<<\n";

            XMLString::release (&pAttributeName);
            XMLString::release (&pAttributeValue);
        }
    }

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        DOMNode::NodeType nodeType = (DOMNode::NodeType) pChildNode->getNodeType ();

        if (DOMNode::ELEMENT_NODE == nodeType)
        {
            YangElement *pYangElement     = NULL;
            DOMElement  *pChildDomElement = dynamic_cast<DOMElement *> (pChildNode);

            prismAssert (NULL != pChildDomElement, __FILE__, __LINE__);

            pNodeName = XMLString::transcode (pChildNode->getNodeName ());

            if (NULL != pNodeName)
            {
                pYangElement = YangElementFactory::createYangElement (pNodeName);

                if (NULL == pYangElement)
                {
                    cout << ">>>>>Does not know about " << pNodeName << "\n";
                }
                else
                {
                    // Set the Owner Module ahead of loading children so that they know what to set when they are loading themselves up.
                    // Pre Order

                    pYangElement->setPOwnerYangModule (m_pOwnerYangModule);

                    // Load the element data from the yin file content using the xerces dom parser.

                    pYangElement->loadFromDomElement (pChildDomElement);

                    // Adding must be done after loading it full.  Otherwise the internal maps in the parent will nto be set correctly since they depend on fields of the child.
                    // Post Order

                    addChildElement (pYangElement);
                }

                XMLString::release (&pNodeName);
            }
        }
        else if (DOMNode::TEXT_NODE == nodeType)
        {
            pNodeValue = XMLString::transcode (pChildNode->getNodeValue ());

            if (NULL != pNodeValue)
            {
                loadValue (string (pNodeValue));
                //cout << "  Node Value :" << m_yangName << ":" << pNodeValue << "\n";
                XMLString::release (&pNodeValue);
            }
        }
    }
}

void YangElement::processAttribute (const string &attributeName, const string &attributeValue)
{
    if ("name" == attributeName)
    {
        setName        (attributeValue);
        setDisplayName (attributeValue);
    }
}

void YangElement::processChildElement (YangElement *pYangElement)
{
    pYangElement->updateParentAnnotations();
}


void YangElement::setAnnotationBitMap (UI64 childBitMap)
{
    m_annotationBitMap = m_annotationBitMap | childBitMap;
}


UI64 YangElement::getAnnotationBitMap ()
{
    return m_annotationBitMap;
}

void YangElement::setCliTargetNodeNameForDropNode (const string &cliTargetNodeNameForDropNode)
{
	m_cliTargetNodeNameForDropNode = cliTargetNodeNameForDropNode;
}

string YangElement::getCliTargetNodeNameForDropNode () const
{
    return m_cliTargetNodeNameForDropNode;
}

void YangElement::updateParentAnnotations ()
{

}

void YangElement::unprocessChildElement (YangElement *pYangElement)
{
}

bool YangElement::isAKnownAttribute (const string &attributeName) const
{
    map<string, string>::const_iterator element    = m_attributesMap.find (attributeName);
    map<string, string>::const_iterator endElement = m_attributesMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void YangElement::addAttribute (const string &attributeName, const string &attributeValue)
{
    if (false == (isAKnownAttribute (attributeName)))
    {
        m_attributes.push_back (attributeName);

        m_attributesMap[attributeName] = attributeValue;

        processAttribute (attributeName, attributeValue);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void YangElement::getAttributeValue (const string &attributeName, string &attributeValue) const
{
    map<string, string>::const_iterator element    = m_attributesMap.find (attributeName);
    map<string, string>::const_iterator endElement = m_attributesMap.end  ();

    if (endElement != element)
    {
        attributeValue = element->second;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void YangElement::printYin (const UI32 &nodeDepth, const bool &printXmlDeclaration, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    if (true == printXmlDeclaration)
    {
        fprintf (pFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    }

    UI32 i                     = 0;
    UI32 numberOfChildElements = m_childElements.size ();

    printYinForThisNodeHeader (nodeDepth, pFile);

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        if (NULL != pChildElement)
        {
            pChildElement->printYin (nodeDepth + 1, false, pFile);
        }
    }

    printYinForThisNodeValue  (nodeDepth, pFile);
    printYinForThisNodeFooter (nodeDepth, pFile);
}

void YangElement::printYinToString (string &yinString, const UI32 &nodeDepth, const bool &printXmlDeclaration) const
{
    if (true == printXmlDeclaration)
    {
        yinString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    }

    UI32 i                     = 0;
    UI32 numberOfChildElements = m_childElements.size ();

    printYinForThisNodeHeaderToString (yinString, nodeDepth);

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        if (NULL != pChildElement)
        {
            pChildElement->printYinToString (yinString, nodeDepth + 1, false);
        }
    }

    printYinForThisNodeValueToString  (yinString, nodeDepth);
    printYinForThisNodeFooterToString (yinString, nodeDepth);
}

void YangElement::printYinForThisNodeHeader (const UI32 &nodeDepth, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

          UI32   i                             = 0;
          bool   firstAttributeAlreadyPrinted  = false;
          UI32   numberOfAttributes            = m_attributes.size ();
          UI32   yangNameLength                = m_yangName.length ();
          UI32   paddingSpaceForAttributes     = yangNameLength + 1;
          string paddingStringForAttributes;
          char   attributeQuotingCharacter     = m_attributeQuotingCharacter;
    const string customNameSpaceAttributeName  = "xmlns:brocade";
    const string customNameSpaceAttributeValue = "http://brocade.com/yang/common";

    for (i = 0; i < nodeDepth; i++)
    {
        paddingStringForAttributes += "    ";
    }

    for (i = 0; i < paddingSpaceForAttributes; i++)
    {
        paddingStringForAttributes += ' ';
    }

    for (i = 0; i < nodeDepth; i++)
    {
        fprintf (pFile, "    ");
    }

    fprintf (pFile, "<%s", m_yangName.c_str ());

    if ("" != m_name)
    {
        fprintf (pFile, " name=\"%s\"",  m_name.c_str ());

        firstAttributeAlreadyPrinted = true;
    }

    for (i = 0; i < numberOfAttributes; i++)
    {
        const string                              attributeName  = m_attributes[i];
              map<string, string>::const_iterator element        = m_attributesMap.find (attributeName);
              map<string, string>::const_iterator endElement     = m_attributesMap.end  ();

        prismAssert (endElement != element, __FILE__, __LINE__);

              string                              attributeValue = element->second;

        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string ("&"), string ("&amp;"));
        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string ("<"), string ("&lt;"));
        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string (">"), string ("&gt;"));

        if ("name" != attributeName)
        {
            if (string::npos != (attributeValue.find (m_attributeQuotingCharacter)))
            {
                attributeQuotingCharacter = '\'';
            }

            if (false == firstAttributeAlreadyPrinted)
            {
                fprintf (pFile, " %s=%c%s%c",  attributeName.c_str (), attributeQuotingCharacter, attributeValue.c_str (), attributeQuotingCharacter);

                firstAttributeAlreadyPrinted = true;
            }
            else
            {
                fprintf (pFile, "\n%s %s=%c%s%c",  paddingStringForAttributes.c_str (), attributeName.c_str (), attributeQuotingCharacter, attributeValue.c_str (), attributeQuotingCharacter);
            }
        }
    }

    if (0 == nodeDepth)
    {
        if (false == (isAKnownAttribute (customNameSpaceAttributeName)))
        {
            if (false == firstAttributeAlreadyPrinted)
            {
                fprintf (pFile, " %s=%c%s%c",  customNameSpaceAttributeName.c_str (), attributeQuotingCharacter, customNameSpaceAttributeValue.c_str (), attributeQuotingCharacter);

                firstAttributeAlreadyPrinted = true;
            }
            else
            {
                fprintf (pFile, "\n%s %s=%c%s%c",  paddingStringForAttributes.c_str (), customNameSpaceAttributeName.c_str (), attributeQuotingCharacter, customNameSpaceAttributeValue.c_str (), attributeQuotingCharacter);
            }
        }
    }


    if ((0 == (m_childElements.size ())) && ("" == m_value))
    {
        fprintf (pFile, "/>\n");
    }
    else
    {
        if ("" == m_value)
        {
            fprintf (pFile, ">\n");
        }
        else
        {
            fprintf (pFile, ">");
        }
    }
}

void YangElement::printYinForThisNodeHeaderToString (string &yinString, const UI32 &nodeDepth) const
{
          UI32   i                             = 0;
          bool   firstAttributeAlreadyPrinted  = false;
          UI32   numberOfAttributes            = m_attributes.size ();
          UI32   yangNameLength                = m_yangName.length ();
          UI32   paddingSpaceForAttributes     = yangNameLength + 1;
          string paddingStringForAttributes;
          char   attributeQuotingCharacter     = m_attributeQuotingCharacter;
    const string customNameSpaceAttributeName  = "xmlns:brocade";
    const string customNameSpaceAttributeValue = "http://brocade.com/yang/common";

    for (i = 0; i < nodeDepth; i++)
    {
        paddingStringForAttributes += "    ";
    }

    for (i = 0; i < paddingSpaceForAttributes; i++)
    {
        paddingStringForAttributes += ' ';
    }

    for (i = 0; i < nodeDepth; i++)
    {
        yinString += "    ";
    }

    yinString += "<" + m_yangName;

    if ("" != m_name)
    {
        yinString += " name=\"" + m_name + "\"";

        firstAttributeAlreadyPrinted = true;
    }

    for (i = 0; i < numberOfAttributes; i++)
    {
        const string                              attributeName  = m_attributes[i];
              map<string, string>::const_iterator element        = m_attributesMap.find (attributeName);
              map<string, string>::const_iterator endElement     = m_attributesMap.end  ();

        prismAssert (endElement != element, __FILE__, __LINE__);

              string                              attributeValue = element->second;

        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string ("&"), string ("&amp;"));
        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string ("<"), string ("&lt;"));
        StringUtils::replaceAllInstancesOfInputStringWith (attributeValue, string (">"), string ("&gt;"));

        if ("name" != attributeName)
        {
            if (string::npos != (attributeValue.find (m_attributeQuotingCharacter)))
            {
                attributeQuotingCharacter = '\'';
            }

            if (false == firstAttributeAlreadyPrinted)
            {
                yinString += " " + attributeName + "=" + attributeQuotingCharacter + attributeValue + attributeQuotingCharacter;

                firstAttributeAlreadyPrinted = true;
            }
            else
            {
                yinString += "\n" + paddingStringForAttributes + " " + attributeName + "=" + attributeQuotingCharacter + attributeValue + attributeQuotingCharacter;
            }
        }
    }

    if (0 == nodeDepth)
    {
        if (false == (isAKnownAttribute (customNameSpaceAttributeName)))
        {
            if (false == firstAttributeAlreadyPrinted)
            {
                yinString += " " + customNameSpaceAttributeName + "=" + attributeQuotingCharacter + customNameSpaceAttributeValue + attributeQuotingCharacter;

                firstAttributeAlreadyPrinted = true;
            }
            else
            {
                yinString += "\n" + paddingStringForAttributes + " " + customNameSpaceAttributeName + "=" + attributeQuotingCharacter + customNameSpaceAttributeValue + attributeQuotingCharacter;
            }
        }
    }


    if ((0 == (m_childElements.size ())) && ("" == m_value))
    {
        yinString += "/>\n";
    }
    else
    {
        if ("" == m_value)
        {
            yinString += ">\n";
        }
        else
        {
            yinString += ">";
        }
    }
}

void YangElement::printYinForThisNodeValue (const UI32 &nodeDepth, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    string value = m_value;

    StringUtils::replaceAllInstancesOfInputStringWith (value, string ("&"), string ("&amp;"));
    StringUtils::replaceAllInstancesOfInputStringWith (value, string ("<"), string ("&lt;"));
    StringUtils::replaceAllInstancesOfInputStringWith (value, string (">"), string ("&gt;"));

    fprintf (pFile, "%s", value.c_str ());
}

void YangElement::printYinForThisNodeValueToString (string &yinString, const UI32 &nodeDepth) const
{
    string value = m_value;

    StringUtils::replaceAllInstancesOfInputStringWith (value, string ("&"), string ("&amp;"));
    StringUtils::replaceAllInstancesOfInputStringWith (value, string ("<"), string ("&lt;"));
    StringUtils::replaceAllInstancesOfInputStringWith (value, string (">"), string ("&gt;"));

    yinString += value;
}

void YangElement::printYinForThisNodeFooter (const UI32 &nodeDepth, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    if (0 != (m_childElements.size ()))
    {
        UI32 i = 0;

        for (i = 0; i < nodeDepth; i++)
        {
            fprintf (pFile, "    ");
        }

        fprintf (pFile, "</%s>\n", m_yangName.c_str ());
    }
    else if ("" != m_value)
    {
        fprintf (pFile, "</%s>\n", m_yangName.c_str ());
    }
}

void YangElement::printYinForThisNodeFooterToString (string &yinString, const UI32 &nodeDepth) const
{
    if (0 != (m_childElements.size ()))
    {
        UI32 i = 0;

        for (i = 0; i < nodeDepth; i++)
        {
            yinString += "    ";
        }

        yinString += "</" + m_yangName + ">\n";
    }
    else if ("" != m_value)
    {
        yinString += "</" + m_yangName + ">\n";
    }
}

void YangElement::printYinForThisNode (const UI32 &nodeDepth, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    printYinForThisNodeHeader (nodeDepth, pFile);
    printYinForThisNodeValue  (nodeDepth, pFile);
    printYinForThisNodeFooter (nodeDepth, pFile);
}

void YangElement::printYinForThisNodeToString (string &yinString, const UI32 &nodeDepth) const
{
    printYinForThisNodeHeaderToString (yinString, nodeDepth);
    printYinForThisNodeValueToString  (yinString, nodeDepth);
    printYinForThisNodeFooterToString (yinString, nodeDepth);
}

void YangElement::printYinForTargetNode (const string &targetNode, FILE *pFile)
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    YangElement *pTargetNodeYangelement = getYangElementForTargetNode (targetNode);

    if (NULL != pTargetNodeYangelement)
    {
        pTargetNodeYangelement->printYin (0, true, pFile);
    }
}

void YangElement::printYinToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYin (0, true, pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYin (0, true, pFile);
    }
}

void YangElement::printYinForTargetNodeToFile (const string &targetNode, const string &filePath)
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYinForTargetNode (targetNode, pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYinForTargetNode (targetNode, pFile);
    }
}

char YangElement::getAttributeQuotingCharacter () const
{
    return (m_attributeQuotingCharacter);
}

void YangElement::setAttributeQuotingCharacter (const char &attributeQuotingCharatcer)
{
    m_attributeQuotingCharacter = attributeQuotingCharatcer;
}

YangElement *YangElement::clone () const
{
    YangElement *pClonedYangElement    = YangElementFactory::createYangElement (m_yangName);
    UI32         numberOfAttributes    = m_attributes.size ();
    UI32         numberOfChildElements = m_childElements.size ();
    UI32         i                     = 0;

    prismAssert (NULL != pClonedYangElement, __FILE__, __LINE__);

    pClonedYangElement->setName                      (m_name);
    pClonedYangElement->setDisplayName               (m_displayName);
    pClonedYangElement->setValue                     (m_value);
    pClonedYangElement->setAttributeQuotingCharacter (m_attributeQuotingCharacter);

    // Explicitly set this.  Do not use the setter function for owner module.

    pClonedYangElement->m_pOriginalOwnerYangModule = m_pOwnerYangModule;

    for (i = 0; i < numberOfAttributes; i++)
    {
        const string                              attributeName  = m_attributes[i];
              map<string, string>::const_iterator element        = m_attributesMap.find (attributeName);
              map<string, string>::const_iterator endElement     = m_attributesMap.end  ();

        prismAssert (endElement != element, __FILE__, __LINE__);

        const string                              attributeValue = element->second;

        pClonedYangElement->addAttribute (attributeName, attributeValue);
    }

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        YangElement *pClonedChildElement = pChildElement->clone ();

        prismAssert (NULL != pClonedChildElement, __FILE__, __LINE__);

        pClonedYangElement->addChildElement (pClonedChildElement);
    }

    if ((YangModule::getYangName()) == m_yangName)
    {
        //cout << "Fixing Owner Modules for entire tree : Module Name : " << m_name << "\n";

        YangModule *pClonedYangModule = dynamic_cast<YangModule *> (pClonedYangElement);

        prismAssert (NULL != pClonedYangModule, __FILE__, __LINE__);

        pClonedYangModule->setPOwnerYangModuleForTree (pClonedYangModule);
    }

    pClonedYangElement->setOriginalModuleNameSpaceUri (getOriginalModuleNameSpaceUri ());

    return (pClonedYangElement);
}

void YangElement::setPOwnerYangModule (YangModule *pOwnerYangModule)
{
    m_pOwnerYangModule = pOwnerYangModule;

    if (NULL == m_pOriginalOwnerYangModule)
    {
        m_pOriginalOwnerYangModule = m_pOwnerYangModule;
    }
}

void YangElement::setPOwnerYangModuleForTree (YangModule *pOwnerYangModule)
{
    UI32 numberOfChildElements = m_childElements.size ();
    UI32 i                     = 0;

    m_pOwnerYangModule = pOwnerYangModule;

    //cout << "    Processing " << numberOfChildElements << " for " << m_name << "\n";

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->setPOwnerYangModuleForTree (pOwnerYangModule);
    }
}

bool YangElement::considerForComputingUsageCountForGroupingsForProgrammingLanguages () const
{
    return (true);
}

void YangElement::computeUsageCountForGroupings (const YangModuleCollection *pYangModuleCollection)
{
    vector<YangElement *> allUsesChildElements;
    UI32                  numberOfUsesChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangUses::getYangName (), allUsesChildElements);

    numberOfUsesChildElements = allUsesChildElements.size ();

    for (i = 0; i < numberOfUsesChildElements; i++)
    {
        string                 usedGroupingName;
        string                 ownerModuleNamePrefixForUsedGrouping;
        string                 ownerModuleNameForUsedGrouping;
        YangUses              *pYangUses                             = dynamic_cast<YangUses *> (allUsesChildElements[i]);
        vector<string>        tokens;
        UI32                   numberOfTokens                        = 0;
        YangModule            *pOwnerYangModuleForUsedGrouping       = NULL;
        vector<YangElement *>  clonedChildrenForUsedgrouping;

        prismAssert (NULL != pYangUses, __FILE__, __LINE__);

        usedGroupingName = pYangUses->getName ();

        StringUtils::tokenize (usedGroupingName, tokens, ':');

        numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);

        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedGroupingName                     = tokens[1];
            ownerModuleNamePrefixForUsedGrouping = tokens[0];

            ownerModuleNameForUsedGrouping = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedGrouping);

            pOwnerYangModuleForUsedGrouping = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedGrouping);

            prismAssert (NULL != pOwnerYangModuleForUsedGrouping, __FILE__, __LINE__);
        }
        else
        {
            ownerModuleNameForUsedGrouping  = m_pOwnerYangModule->getName ();
            pOwnerYangModuleForUsedGrouping = m_pOwnerYangModule;
        }

        pOwnerYangModuleForUsedGrouping->incrementUsageCountForGrouping (usedGroupingName);
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeUsageCountForGroupings (pYangModuleCollection);
    }
}

void YangElement::incrementUsageCountForGroupingsInHierarchyForProgrammingLanguages (const YangModuleCollection *pYangModuleCollection, const UI32 &count)
{
    YangGrouping * const pYangGrouping = dynamic_cast<YangGrouping *> (this);

    if (NULL != pYangGrouping)
    {
        pYangGrouping->incrementUsageCountForProgrammingLanguages (count);
    }

    vector<YangElement *> allUsesChildElements;
    UI32                  numberOfUsesChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangUses::getYangName (), allUsesChildElements);

    numberOfUsesChildElements = allUsesChildElements.size ();

    for (i = 0; i < numberOfUsesChildElements; i++)
    {
        string                 usedGroupingName;
        string                 ownerModuleNamePrefixForUsedGrouping;
        string                 ownerModuleNameForUsedGrouping;
        YangUses              *pYangUses                             = dynamic_cast<YangUses *> (allUsesChildElements[i]);
        vector<string>        tokens;
        UI32                   numberOfTokens                        = 0;
        YangModule            *pOwnerYangModuleForUsedGrouping       = NULL;
        vector<YangElement *>  clonedChildrenForUsedgrouping;

        prismAssert (NULL != pYangUses, __FILE__, __LINE__);

        usedGroupingName = pYangUses->getName ();

        StringUtils::tokenize (usedGroupingName, tokens, ':');

        numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);

        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedGroupingName                     = tokens[1];
            ownerModuleNamePrefixForUsedGrouping = tokens[0];

            ownerModuleNameForUsedGrouping = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedGrouping);

            pOwnerYangModuleForUsedGrouping = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedGrouping);

            prismAssert (NULL != pOwnerYangModuleForUsedGrouping, __FILE__, __LINE__);
        }
        else
        {
            ownerModuleNameForUsedGrouping  = m_pOwnerYangModule->getName ();
            pOwnerYangModuleForUsedGrouping = m_pOwnerYangModule;
        }

        //cout << "USED ... " << usedGroupingName << " BY " << m_pOwnerYangModule->getName () << endl;

        pOwnerYangModuleForUsedGrouping->incrementUsageCountForGroupingForProgrammingLanguages (pYangModuleCollection, usedGroupingName);
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->incrementUsageCountForGroupingsInHierarchyForProgrammingLanguages (pYangModuleCollection, count);
    }
}

void YangElement::computeUsageCountForGroupingsForProgrammingLanguages (const YangModuleCollection *pYangModuleCollection)
{
    if (false == (considerForComputingUsageCountForGroupingsForProgrammingLanguages ()))
    {
        return;
    }

    vector<YangElement *> allUsesChildElements;
    UI32                  numberOfUsesChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangUses::getYangName (), allUsesChildElements);

    numberOfUsesChildElements = allUsesChildElements.size ();

    for (i = 0; i < numberOfUsesChildElements; i++)
    {
        string                 usedGroupingName;
        string                 ownerModuleNamePrefixForUsedGrouping;
        string                 ownerModuleNameForUsedGrouping;
        YangUses              *pYangUses                             = dynamic_cast<YangUses *> (allUsesChildElements[i]);
        vector<string>        tokens;
        UI32                   numberOfTokens                        = 0;
        YangModule            *pOwnerYangModuleForUsedGrouping       = NULL;
        vector<YangElement *>  clonedChildrenForUsedgrouping;

        prismAssert (NULL != pYangUses, __FILE__, __LINE__);

        usedGroupingName = pYangUses->getName ();

        StringUtils::tokenize (usedGroupingName, tokens, ':');

        numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);

        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedGroupingName                     = tokens[1];
            ownerModuleNamePrefixForUsedGrouping = tokens[0];

            ownerModuleNameForUsedGrouping = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedGrouping);

            pOwnerYangModuleForUsedGrouping = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedGrouping);

            prismAssert (NULL != pOwnerYangModuleForUsedGrouping, __FILE__, __LINE__);
        }
        else
        {
            ownerModuleNameForUsedGrouping  = m_pOwnerYangModule->getName ();
            pOwnerYangModuleForUsedGrouping = m_pOwnerYangModule;
        }

        cout << "USED ... " << usedGroupingName << " BY " << m_pOwnerYangModule->getName () << endl;

        pOwnerYangModuleForUsedGrouping->incrementUsageCountForGroupingForProgrammingLanguages (pYangModuleCollection, usedGroupingName);
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeUsageCountForGroupingsForProgrammingLanguages (pYangModuleCollection);
    }
}

void YangElement::inlineGroupingUsage (const YangModuleCollection *pYangModuleCollection)
{
    vector<YangElement *> allUsesChildElements;
    UI32                  numberOfUsesChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangUses::getYangName (), allUsesChildElements);

    numberOfUsesChildElements = allUsesChildElements.size ();

    for (i = 0; i < numberOfUsesChildElements; i++)
    {
        string                 usedGroupingName;
        string                 ownerModuleNamePrefixForUsedGrouping;
        string                 ownerModuleNameForUsedGrouping;
        YangUses              *pYangUses                             = dynamic_cast<YangUses *> (allUsesChildElements[i]);
        vector<string>        tokens;
        UI32                   numberOfTokens                        = 0;
        YangModule            *pOwnerYangModuleForUsedGrouping       = NULL;
        vector<YangElement *>  clonedChildrenForUsedgrouping;

        prismAssert (NULL != pYangUses, __FILE__, __LINE__);

        usedGroupingName = pYangUses->getName ();

        StringUtils::tokenize (usedGroupingName, tokens, ':');

        numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);

        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedGroupingName                     = tokens[1];
            ownerModuleNamePrefixForUsedGrouping = tokens[0];

            ownerModuleNameForUsedGrouping = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedGrouping);

            pOwnerYangModuleForUsedGrouping = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedGrouping);

            prismAssert (NULL != pOwnerYangModuleForUsedGrouping, __FILE__, __LINE__);
        }
        else
        {
            ownerModuleNameForUsedGrouping  = m_pOwnerYangModule->getName ();
            pOwnerYangModuleForUsedGrouping = m_pOwnerYangModule;
        }

        pOwnerYangModuleForUsedGrouping->getClonedChildrenForGrouping (usedGroupingName, clonedChildrenForUsedgrouping);

        vector<YangElement *> allRefineElements;
        pYangUses->getAllChildrenByYangName (YangRefine::getYangName (), allRefineElements);

        UI32 numberOfRefineElements = allRefineElements.size ();

        for (UI32 k = 0; k < numberOfRefineElements; k++)
        {
            YangRefine *pYangRefine = dynamic_cast<YangRefine *> (allRefineElements[k]);
            prismAssert (NULL != pYangRefine, __FILE__, __LINE__);

            pYangRefine->refineYangElements (clonedChildrenForUsedgrouping);
        }

        addChildElementsBefore (pYangUses, clonedChildrenForUsedgrouping);

        removeChildElement (allUsesChildElements[i]);

        delete allUsesChildElements[i];
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->inlineGroupingUsage (pYangModuleCollection);
    }
}

void YangElement::inlineGroupingUsageForRpcs (const YangModuleCollection *pYangModuleCollection)
{
    vector<YangElement *> allUsesChildElements;
    UI32                  numberOfUsesChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangUses::getYangName (), allUsesChildElements);

    numberOfUsesChildElements = allUsesChildElements.size ();

    for (i = 0; i < numberOfUsesChildElements; i++)
    {
        string                 usedGroupingName;
        string                 ownerModuleNamePrefixForUsedGrouping;
        string                 ownerModuleNameForUsedGrouping;
        YangUses              *pYangUses                             = dynamic_cast<YangUses *> (allUsesChildElements[i]);
        vector<string>        tokens;
        UI32                   numberOfTokens                        = 0;
        YangModule            *pOwnerYangModuleForUsedGrouping       = NULL;
        vector<YangElement *>  clonedChildrenForUsedgrouping;

        prismAssert (NULL != pYangUses, __FILE__, __LINE__);

        if (true != (pYangUses->isInsideAnRpc ()))
        {
            continue;
        }

        usedGroupingName = pYangUses->getName ();

        StringUtils::tokenize (usedGroupingName, tokens, ':');

        numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);

        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedGroupingName                     = tokens[1];
            ownerModuleNamePrefixForUsedGrouping = tokens[0];

            ownerModuleNameForUsedGrouping = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedGrouping);

            pOwnerYangModuleForUsedGrouping = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedGrouping);

            prismAssert (NULL != pOwnerYangModuleForUsedGrouping, __FILE__, __LINE__);
        }
        else
        {
            ownerModuleNameForUsedGrouping  = m_pOwnerYangModule->getName ();
            pOwnerYangModuleForUsedGrouping = m_pOwnerYangModule;
        }

        cout << "  Inlining " << usedGroupingName << " in module " << m_pOwnerYangModule->getName () << " under Element " << getName () << endl;

        pOwnerYangModuleForUsedGrouping->getClonedChildrenForGrouping (usedGroupingName, clonedChildrenForUsedgrouping);

        vector<YangElement *> allRefineElements;
        pYangUses->getAllChildrenByYangName (YangRefine::getYangName (), allRefineElements);

        UI32 numberOfRefineElements = allRefineElements.size ();

        for (UI32 k = 0; k < numberOfRefineElements; k++)
        {
            YangRefine *pYangRefine = dynamic_cast<YangRefine *> (allRefineElements[k]);
            prismAssert (NULL != pYangRefine, __FILE__, __LINE__);

            pYangRefine->refineYangElements (clonedChildrenForUsedgrouping);
        }

        addChildElementsBefore (pYangUses, clonedChildrenForUsedgrouping);

        removeChildElement (allUsesChildElements[i]);

        delete allUsesChildElements[i];
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->inlineGroupingUsageForRpcs (pYangModuleCollection);
    }
}

void YangElement::inlineAugmentUsage (const YangModuleCollection *pYangModuleCollection, UI32 &numberOfAugmentsResolved, UI32 &numberOfAugmentsNotResolved)
{
    vector<YangElement *> allAugmentChildElements;
    UI32                  numberOfAugmentChildElements = 0;
    UI32                  i                            = 0;

    getAllChildrenByYangName (YangAugment::getYangName (), allAugmentChildElements);

    numberOfAugmentChildElements = allAugmentChildElements.size ();

    YangModule *pOwnerYangModuleForThisElement = getPOwnerYangModule ();
    prismAssert (NULL != pOwnerYangModuleForThisElement, __FILE__, __LINE__);

    for (i = 0; i < numberOfAugmentChildElements; i++)
    {
        YangAugment *pYangAugment            = dynamic_cast<YangAugment *> (allAugmentChildElements[i]);
        string       targetNodeModuleName;
        string       targetNodePathInModule;
        YangModule  *pYangModule            = NULL;
        YangElement *pYangElement           = NULL;

        prismAssert (NULL != pYangAugment, __FILE__, __LINE__);

        const string targetNode = pYangAugment->getTargetNode ();

        // Transfer the when conditions to the children of the augment
        pYangAugment->transferWhenConditionsToChildren ();

        pYangAugment->getTargetNodeDetails (targetNodeModuleName, targetNodePathInModule);

        pYangModule = pYangModuleCollection->getYangModuleByName (targetNodeModuleName);

        if (NULL != pYangModule)
        {
            pYangElement = pYangModule->getYangElementForTargetNode (targetNodePathInModule);
        }

        if (NULL != pYangElement)
        {
            cout << "    FOUND : " << targetNodeModuleName << ":" << targetNodePathInModule << endl;

            numberOfAugmentsResolved++;

            vector<YangElement *> clonedChildrenForAugment;

            pYangAugment->getClonedChildren (clonedChildrenForAugment);

            YangModule *pOwnerYangModule       = pYangElement->getPOwnerYangModule ();
            UI32        numberOfClonedChildren = clonedChildrenForAugment.size ();
            UI32        j                      = 0;

            for (j = 0; j < numberOfClonedChildren; j++)
            {
                YangElement *pClonedChildElement = clonedChildrenForAugment[j];

                prismAssert (NULL != pClonedChildElement, __FILE__, __LINE__);

                pClonedChildElement->setPOwnerYangModuleForTree (pOwnerYangModule);

                pClonedChildElement->setCameFromAugment ();
            }

            pYangElement->addChildElements (clonedChildrenForAugment);

            removeChildElement (pYangAugment);

            delete pYangAugment;
        }
        else
        {
            cout << "    NOT FOUND : " << targetNodeModuleName << ":" << targetNodePathInModule << endl;

            numberOfAugmentsNotResolved++;
        }
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->inlineAugmentUsage (pYangModuleCollection, numberOfAugmentsResolved, numberOfAugmentsNotResolved);
    }
}

void YangElement::getAllChildrenByYangName (const string &yangName, vector<YangElement *> &childElementsByYangName) const
{
    childElementsByYangName.clear ();

    map<string, vector<YangElement *> >::const_iterator element1    = m_childElementsMapByYangName.find (yangName);
    map<string, vector<YangElement *> >::const_iterator endElement1 = m_childElementsMapByYangName.end  ();

    if (endElement1 != element1)
    {
        childElementsByYangName = element1->second;
#if 0
        // map<YangElement *, YangElement *> &childYangElementsByYangName = m_childElementsMapByYangName[yangName];
        const map<YangElement *, YangElement *> &childYangElementsByYangName = element1->second;

        map<YangElement *, YangElement *>::const_iterator element    = childYangElementsByYangName.begin ();
        map<YangElement *, YangElement *>::const_iterator endElement = childYangElementsByYangName.end   ();

        while (endElement != element)
        {
            childElementsByYangName.push_back (element->first);

            element++;
        }
#endif
    }
}

void YangElement::getClonedChildren (vector<YangElement *> &clonedChildren) const
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    clonedChildren.clear ();

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement       = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        YangElement *pClonedChildelement = pChildElement->clone ();

        prismAssert (NULL != pClonedChildelement, __FILE__, __LINE__);

        clonedChildren.push_back (pClonedChildelement);
    }
}

void YangElement::checkIntegrity () const
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    if ((YangUserInterface::getYangName ()) != m_yangName)
    {
        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->checkIntegrity ();
    }
}

YangModule *YangElement::getPOwnerYangModule () const
{
    return (m_pOwnerYangModule);
}

YangElement *YangElement::getYangElementForTargetNode (const string &targetNode)
{
    vector<string>  tokens;
    UI32            numberOfTokens = 0;
    UI32            i              = 0;
    YangElement    *pYanglement    = this;

    StringUtils::tokenize (targetNode, tokens, '/');

    numberOfTokens = tokens.size ();

    for (i = 0; i < numberOfTokens; i++)
    {
        string yangElementName = tokens[i];

        if ("" == yangElementName)
        {
            continue;
        }

        pYanglement = pYanglement->getUniqueYangElementByName (yangElementName);

        if (NULL == pYanglement)
        {
            return (NULL);
        }
    }

    return (pYanglement);
}

YangElement *YangElement::getUniqueYangElementByName (const string &uniqueYangElementName) const
{
    YangElement *pYangElement = NULL;

    if ("" != uniqueYangElementName)
    {
        map<string, map<YangElement *, YangElement *> >::const_iterator element    = m_childElementsMapByName.find (uniqueYangElementName);
        map<string, map<YangElement *, YangElement *> >::const_iterator endElement = m_childElementsMapByName.end  ();

        if (endElement != element)
        {
            // map<YangElement *, YangElement *> &yangElements         = m_childElementsMapByName[uniqueYangElementName];
            const map<YangElement *, YangElement *> &yangElements   = element->second;
            UI32                               numberOfYangElements = yangElements.size ();

            prismAssert (1 == numberOfYangElements, __FILE__, __LINE__);

            pYangElement = (yangElements.begin ())->first;
        }
    }

    return (pYangElement);
}

void YangElement::getAllNames (set<string> &allNames) const
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    if ("" != m_name)
    {
        allNames.insert (m_name);
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->getAllNames (allNames);
    }
}

void YangElement::printAllNames (FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    set<string> allNames;

    getAllNames (allNames);

    set<string>::const_iterator element    = allNames.begin ();
    set<string>::const_iterator endElement = allNames.end   ();

    while (endElement != element)
    {
        fprintf (pFile, "%s\n", (*element).c_str ());

        element++;
    }
}

void YangElement::printAllNamesToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printAllNames (pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printAllNames (pFile);
    }
}

void YangElement::getAllCliTargetNodeNamesForData (vector<string> &allCliTargetNodeNamesForData) const
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    if (false == (getIsDeprecated ()))
    {
        string cliTargetNodeNameForData = getCliTargetNodeNameForData ();

        if (("" != cliTargetNodeNameForData) && (true == m_willBeShownInConfigurationTree))
        {
            vector<string>::const_iterator element    = std::find (allCliTargetNodeNamesForData.begin (), allCliTargetNodeNamesForData.end (), cliTargetNodeNameForData);
            vector<string>::const_iterator endElement = allCliTargetNodeNamesForData.end ();

            if (endElement == element)
            {
                allCliTargetNodeNamesForData.push_back (cliTargetNodeNameForData);
            }
            else
            {
                const YangLeaf * const pYangLeaf = dynamic_cast<const YangLeaf * const> (this);

                if (NULL != pYangLeaf)
                {
                    //prismAssert (false, __FILE__, __LINE__);
                }
            }
        }

        for (i = 0; i < numberOfchildren; i++)
        {
            YangElement *pChildElement = m_childElements[i];

            prismAssert (NULL != pChildElement, __FILE__, __LINE__);

            pChildElement->getAllCliTargetNodeNamesForData (allCliTargetNodeNamesForData);
        }
    }
}

void YangElement::computeCliTargetNodeNameMapForData (map<string, vector<YangElement *> > &cliTargetNodeNameMapForData, map<string, vector<YangElement *> > &cliTargetNodeNameMapForDataWithDropNodeName) const
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    if (false == (getIsDeprecated ()))
    {
        string cliTargetNodeNameForData = getCliTargetNodeNameForData ();

        YangElement *pThisYangElement = NULL;

        if (("" != cliTargetNodeNameForData) && (true == m_willBeShownInConfigurationTree) && (false == m_annotationFlags.m_dropNodeNameForCli))
        {
            pThisYangElement = const_cast<YangElement *> (this);

            (cliTargetNodeNameMapForData[cliTargetNodeNameForData]).push_back (pThisYangElement);
        }

        if (("" != m_cliTargetNodeNameForDropNode) && (true == m_willBeShownInConfigurationTree))
        {
            pThisYangElement = const_cast<YangElement *> (this);

            (cliTargetNodeNameMapForDataWithDropNodeName[m_cliTargetNodeNameForDropNode]).push_back (pThisYangElement);
        }

        for (i = 0; i < numberOfchildren; i++)
        {
            YangElement *pChildElement = m_childElements[i];

            prismAssert (NULL != pChildElement, __FILE__, __LINE__);

            pChildElement->computeCliTargetNodeNameMapForData (cliTargetNodeNameMapForData, cliTargetNodeNameMapForDataWithDropNodeName);
        }
    }
}

void YangElement::printAllCliTargetNodeNamesForData (FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    vector<string> allCliTargetNodeNamesForData;

    getAllCliTargetNodeNamesForData (allCliTargetNodeNamesForData);

    vector<string>::const_iterator element    = allCliTargetNodeNamesForData.begin ();
    vector<string>::const_iterator endElement = allCliTargetNodeNamesForData.end   ();

    while (endElement != element)
    {
        fprintf (pFile, "%s\n", (*element).c_str ());

        element++;
    }
}

void YangElement::printAllCliTargetNodeNamesForDataToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printAllCliTargetNodeNamesForData (pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printAllCliTargetNodeNamesForData (pFile);
    }
}

void YangElement::updateUserTags (const YangModuleCollection *pYangModuleCollection)
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    UI32 userTagValue = pYangModuleCollection->getUserTagValueByName (m_name);

    m_userTagValue = userTagValue;

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->updateUserTags (pYangModuleCollection);
    }
}

UI32 YangElement::getUserTagValue () const
{
    return (m_userTagValue);
}

void YangElement::setuserTagValue (const UI32 &userTagValue)
{
    m_userTagValue = userTagValue;
}

void YangElement::computeConfigurationSegmentNames ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    if (NULL != m_pParentElement)
    {
        if ("" == m_configurationSegmentName)
        {
            const string configurationSegmentNameForParent = m_pParentElement->getConfigurationSegmentName ();

            m_configurationSegmentName = configurationSegmentNameForParent;
        }
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeConfigurationSegmentNames ();
    }
}

string YangElement::getConfigurationSegmentName () const
{
    return (m_configurationSegmentName);
}

void YangElement::setConfigurationSegmentName (const string &configurationSegmentName)
{
    m_configurationSegmentName = configurationSegmentName;
}

bool YangElement::getIsConfigurationSegmentRoot () const
{
    return (m_isConfigurationSegmentRoot);
}

void YangElement::setIsConfigurationSegmentRoot (const bool &isConfigurationSegmentRoot)
{
    m_isConfigurationSegmentRoot = isConfigurationSegmentRoot;
}

bool YangElement::getIsConfiguration () const
{
    return (m_isConfiguration);
}

void YangElement::setIsConfiguration (const bool &isConfiguration)
{
    m_isConfiguration = isConfiguration;
}

bool YangElement::getIsConfigurationExplicitlySet () const
{
    return (m_isConfigurationExplicitlySet);
}

void YangElement::setIsConfigurationExplicitlySet (const bool &isconfigurationExplicitlySet)
{
    m_isConfigurationExplicitlySet = isconfigurationExplicitlySet;
}

UI32 YangElement::getNumberOfChildElements () const
{
    return (m_childElements.size ());
}

YangElement *YangElement::getYangElementByTargetNodePath (const string &targetNode)
{
    YangElement *pTargetNodeYangElement = getYangElementForTargetNode (targetNode);

    return (pTargetNodeYangElement);
}

void YangElement::displayConfiguration (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
}

void YangElement::debugPrint () const
{
    trace (TRACE_LEVEL_INFO,         "Yang Name                  : " + m_yangName);
    trace (TRACE_LEVEL_INFO,         "Name                       : " + m_name);
    trace (TRACE_LEVEL_INFO,         "Display Name               : " + m_displayName);
    trace (TRACE_LEVEL_INFO,         "Configuration Segment      : " + m_configurationSegmentName);
    trace (TRACE_LEVEL_INFO, string ("Configuration Segment Root : ") + (m_isConfigurationSegmentRoot ? "TRUE" : "FALSE"));
    trace (TRACE_LEVEL_INFO, string ("Is Configuration           : ") + (m_isConfiguration ? "TRUE" : "FALSE"));
    trace (TRACE_LEVEL_INFO, string ("Drop Node Name For CLI     : ") + (m_annotationFlags.m_dropNodeNameForCli ? "TRUE" : "FALSE"));
    trace (TRACE_LEVEL_INFO,         "CLI Target Node Name       : " + m_cliTargetNodeName);
}

void YangElement::computeCliTargetNodeNameForSelf ()
{
    string parentCliTargetNodeName;

    if (NULL != m_pParentElement)
    {
        parentCliTargetNodeName = m_pParentElement->getMostRecentCliTargetNodeName ();
    }

    m_cliTargetNodeName = parentCliTargetNodeName;

    computeCliTargetNodeNameForSelfInternal ();
}

void YangElement::computeCliTargetNodeNameForSelfInternal ()
{
	string tmpSpaceStr = "";

    if ("" != m_cliTargetNodeName && "" != m_displayName)
    {
    	tmpSpaceStr = " ";
    }

    if (false == m_annotationFlags.m_dropNodeNameForCli)
    {
        m_cliTargetNodeName += tmpSpaceStr + m_displayName;
    }
    else
    {
        m_cliTargetNodeNameForDropNode = m_cliTargetNodeName + tmpSpaceStr + m_displayName;
    }
}

void YangElement::computeCliTargetNodeNames ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    computeCliTargetNodeNameForSelf ();

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeCliTargetNodeNames ();
    }
}

bool YangElement::getDropNodeNameForCli () const
{
    return (m_annotationFlags.m_dropNodeNameForCli);
}

void YangElement::setDropNodeNameForCli (const bool &dropNodeNameForCli)
{
    m_annotationFlags.m_dropNodeNameForCli = dropNodeNameForCli;
}

string YangElement::getCliTargetNodeName () const
{
    return (m_cliTargetNodeName);
}

void YangElement::setCliTargetNodeName (const string &cliTargetNodeName)
{
    m_cliTargetNodeName = cliTargetNodeName;
}

string YangElement::getMostRecentCliTargetNodeName () const
{
    if ("" != m_cliTargetNodeName)
    {
        return (m_cliTargetNodeName);
    }
    else
    {
        if (NULL != m_pParentElement)
        {
            return (m_pParentElement->getMostRecentCliTargetNodeName ());
        }
        else
        {
            return ("");
        }
    }
}

string YangElement::getCliTargetNodeNameForData () const
{
    return ("");
}

bool YangElement::getIsDeprecated () const
{
    return (m_isDeprecated);
}

void YangElement::setIsDeprecated (const bool &isDeprecated)
{
    m_isDeprecated = isDeprecated;
}

void YangElement::computeWillBeShownInConfigurationTree ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeWillBeShownInConfigurationTree ();
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        bool willBeShownInConfigurationTree = pChildElement->getWillBeShownInConfigurationTree ();

        if (true == willBeShownInConfigurationTree)
        {
            setWillBeShownInConfigurationTree (true);
            break;
        }
    }

    if (true == (meetsCriteriaToBeShownInConfiguration ()))
    {
        setWillBeShownInConfigurationTree (true);
    }
}

bool YangElement::meetsCriteriaToBeShownInConfiguration () const
{
    return (getIsConfiguration ());
}

bool YangElement::getWillBeShownInConfigurationTree () const
{
    return (m_willBeShownInConfigurationTree);
}

void YangElement::setWillBeShownInConfigurationTree (const bool &willBeShownInConfigurationTree)
{
    m_willBeShownInConfigurationTree = willBeShownInConfigurationTree;
}

bool YangElement::isAtTopLevelInAModule () const
{
    bool atTopLevelInAModule = false;

    if (NULL != m_pParentElement)
    {
        if ((YangModule::getYangName ()) == (m_pParentElement->getYangName ()))
        {
            atTopLevelInAModule = true;
        }
    }

    return (atTopLevelInAModule);
}

void YangElement::computeIsConfiguration ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    computeIsConfigurationForSelf ();

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeIsConfiguration ();
    }
}

void YangElement::computeIsConfigurationForSelf ()
{
}

void YangElement::setIsConfigurationForEntireTree (const bool &isConfiguration)
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    setIsConfiguration (isConfiguration);

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->setIsConfigurationForEntireTree (isConfiguration);
    }
}

void YangElement::computeFirstDataElementInHierarchy ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeFirstDataElementInHierarchy ();
    }
}

bool YangElement::getIsFirstDataElementInHierarchy () const
{
    return (m_isFirstDataElementInHierarchy);
}

void YangElement::setIsFirstDataElementInHierarchy (const bool &isFirstDataElementInHierarchy)
{
    m_isFirstDataElementInHierarchy = isFirstDataElementInHierarchy;
}

YangElement *YangElement::getSimilarChildElement (YangElement *pYangElement)
{
    YangElement *pSimilarChildElement = NULL;

    const string name     = pYangElement->getName ();
    const string yangName = pYangElement->getYangName ();

    map<string, vector<YangElement *> >::const_iterator element    = m_childElementsMapByYangName.find (yangName);
    map<string, vector<YangElement *> >::const_iterator endElement = m_childElementsMapByYangName.end  ();

    if (endElement != element)
    {
        const   vector<YangElement *>                  &childElementsVectorByYangName   = element->second;
                vector<YangElement *>::const_iterator   element1                        = childElementsVectorByYangName.begin ();
                vector<YangElement *>::const_iterator   endElement1                     = childElementsVectorByYangName.end   ();

        while (endElement1 != element1)
        {
            YangElement *pTempYangElement = (*element1);

            prismAssert (NULL != pTempYangElement, __FILE__, __LINE__);

            if (name == (pTempYangElement->getName ()))
            {
                pSimilarChildElement = pTempYangElement;

                break;
            }

            element1++;
        }
    }

    return (pSimilarChildElement);
}

int YangElement::getMaxDepth ()
{
    SI32 maxDepth       = 0;
    SI32 tempMaxDepth   = 0;

    // First call the getMaxDepth function for each m_childElement
    vector<YangElement *>::iterator childElementsIterator;

    for (childElementsIterator = m_childElements.begin ();
         childElementsIterator != m_childElements.end ();
         childElementsIterator++)
    {
        tempMaxDepth = (*childElementsIterator)->getMaxDepth ();

        if (tempMaxDepth > maxDepth)
        {
            maxDepth = tempMaxDepth;
        }
    }

    /* After getting the maxDepth of all the children,
     * add the current YangElements depth to the computed maxDepth
     *
     * If current YangElement is a Yang List then increment maxDepth by 2
     * (one for the element and one for the key)
     * Else if the current YangElement is only a Yang Data Element then do maxDepth++
     */

    YangList *pYangList = dynamic_cast<YangList *>(this);
    if (pYangList != NULL)
    {
        maxDepth++;
        maxDepth += (pYangList->getKeyCombination ()).size ();
    }
    else
    {
        YangDataElement *pYangDataElement = dynamic_cast<YangDataElement *>(this);
        if (pYangDataElement != NULL)
        {
            maxDepth++;
        }
    }

    return (maxDepth);
}

//Annotation flags accessor method


bool YangElement::isYangElementForData () const
{
    return false;
}

// virtual.
UI32 YangElement::getDataIndex () const
{
    return 0;
}

// Used by sort algorithm.
// static

bool YangElement::compareYangElementsByDataIndex (YangElement *pYangElement1, YangElement *pYangElement2)
{
    prismAssert (NULL != pYangElement1, __FILE__, __LINE__);
    prismAssert (NULL != pYangElement2, __FILE__, __LINE__);

    UI32 dataIndex1 = pYangElement1->getDataIndex ();
    UI32 dataIndex2 = pYangElement2->getDataIndex ();

    return (dataIndex1 < dataIndex2);
}

void YangElement::setupYangElementTreeForData ()
{
    UI32 numberOfchildren = m_childElements.size ();
    UI32 i                = 0;

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        if (true == pChildElement->isYangElementForData ())
        {
            m_pChildElementsForData.push_back (pChildElement);
        }
    }

    std::stable_sort (m_pChildElementsForData.begin (), m_pChildElementsForData.end (), YangElement::compareYangElementsByDataIndex);

    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pChildElementForData = m_pChildElementsForData[i];
        prismAssert (NULL != pChildElementForData, __FILE__, __LINE__);

        pChildElementForData->setupYangElementTreeForData ();
    }
}

void YangElement::debugPrintChildElementInforamtionForData (FILE *pOutputFile, const UI32 &depth) const
{
    string depthSpecifier (depth, ' ');
    fprintf (pOutputFile, "%s [%u] %s\r\n", depthSpecifier.c_str (), getDataIndex (), (getCliTargetNodeNameForData ()).c_str ());

    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pChildElementForData = m_pChildElementsForData[i];
        prismAssert (NULL != pChildElementForData, __FILE__, __LINE__);

        pChildElementForData->debugPrintChildElementInforamtionForData (pOutputFile, depth + 1);
    }
}

void YangElement::setIsNodeSpecificBaseList ()
{
    m_isNodeSpecificBaseList = true;
}

bool YangElement::getIsNodeSpecificBaseList () const
{
    return m_isNodeSpecificBaseList;
}

bool YangElement::getIsInNodeSpecificTree () const
{
    return m_underNodeSpecificTree;
}

void YangElement::setIsMultiPartitionBaseList ()
{
    m_isMultiPartitionBaseList = true;
}

bool YangElement::getIsMultiPartitionBaseList () const
{
    return m_isMultiPartitionBaseList;
}

bool YangElement::getIsInMultiPartitionTree () const
{
    return m_underMultiPartitionTree;
}

void YangElement::propagateNodeSpecificInformation ()
{
    if (false == m_isNodeSpecificBaseList)
    {
        YangElement *pParentYangElement = getPParentElement ();

        if (NULL != pParentYangElement)
        {
            if (true == pParentYangElement->m_isNodeSpecificBaseList)
            {
                if (getConfigurationSegmentName () == pParentYangElement->getConfigurationSegmentName ())
                {
                    m_isNodeSpecificBaseList = true;
                }
                else
                {
                    m_underNodeSpecificTree = true;
                }
            }
            else if (true == pParentYangElement->m_underNodeSpecificTree)
            {
                m_underNodeSpecificTree = true;
            }
        }
    }

    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = m_pChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->propagateNodeSpecificInformation ();
    }
}

void YangElement::propagateMultiPartitionInformation ()
{
    if (false == m_isMultiPartitionBaseList)
    {
        YangElement *pParentYangElement = getPParentElement ();

        if (NULL != pParentYangElement)
        {
            if (true == pParentYangElement->m_isMultiPartitionBaseList)
            {
                if (getConfigurationSegmentName () == pParentYangElement->getConfigurationSegmentName ())
                {
                    m_isMultiPartitionBaseList = true;
                }
                else
                {
                    m_underMultiPartitionTree = true;
                }
            }
            else if (true == pParentYangElement->m_underMultiPartitionTree)
            {
                m_underMultiPartitionTree = true;
            }
        }
    }

    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = m_pChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->propagateMultiPartitionInformation ();
    }
}

void YangElement::prepareHierarchyInformation ()
{
    string configurationSegmentName = getConfigurationSegmentName ();

    if (false == configurationSegmentName.empty ())
    {
        prismAssert (NULL == m_pConfigurationSegmentInformation, __FILE__, __LINE__);

        m_pConfigurationSegmentInformation = WaveConfigurationSegmentMap::getPConfigurationSegmentInformation (configurationSegmentName);

        if (NULL == m_pConfigurationSegmentInformation)
        {
            trace (TRACE_LEVEL_ERROR, string ("YangDataElement::prepareHierarchyInformation : ConfigurationSegment not present for [") + configurationSegmentName + string ("]"));
            return;
        }
    }

    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = m_pChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->prepareHierarchyInformation ();
    }
}

ConfigurationSegmentInformation *YangElement::getPConfigurationSegmentInformation () const
{
    return m_pConfigurationSegmentInformation;
}

void YangElement::setIsHidden ()
{
    m_isHidden = true;
}

bool YangElement::getIsHidden (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const
{
    if (false == m_isHidden)
    {
        return false;
    }
    else
    {
        return true;
    }
}

const set<string> &YangElement::getKnownDisplayNames () const
{
    return m_knownDisplayNames;
}

void YangElement::prepareSetOfKnownDisplayNames ()
{
    UI32 numberOfChildElementsForData = m_pChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = m_pChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->prepareSetOfKnownDisplayNames ();
    }

    if ((false == getDropNodeNameForCli ()) && (YangCase::getYangName () != getYangName ()) && (YangChoice::getYangName () != getYangName ()))
    {
        string displayName = getDisplayName ();
        prismAssert (false == displayName.empty (), __FILE__, __LINE__);

        m_knownDisplayNames.insert (getDisplayName ());
    }
    else if ((YangList::getYangName () == getYangName ()) && (true == getDropNodeNameForCli ()))
    {
        m_knownDisplayNames.insert ("");
    }
    else
    {
        for (UI32 i = 0; i < numberOfChildElementsForData; i++)
        {
            YangElement *pYangElement = m_pChildElementsForData[i];
            prismAssert (NULL != pYangElement, __FILE__, __LINE__);

            const set<string> &knownDisplayNamesForChildElement = pYangElement->getKnownDisplayNames ();
            m_knownDisplayNames.insert (knownDisplayNamesForChildElement.begin (), knownDisplayNamesForChildElement.end ());
        }
    }

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = m_pChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        const set<string> &knownDisplayNamesForChildElement = pYangElement->getKnownDisplayNames ();

        for (set<string>::const_iterator itr = knownDisplayNamesForChildElement.begin (); itr != knownDisplayNamesForChildElement.end (); itr++)
        {
            addYangElementForCliToken (*itr, pYangElement);
        }
    }
}

void YangElement::addYangElementForCliToken (const string &tokenString, YangElement *pYangElement)
{
    prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    m_knownDisplayNamesToChildElementsMap[tokenString].push_back (pYangElement);

    string tokenStringLowerCase;
    StringUtils::getLowerCaseString (tokenString, tokenStringLowerCase);
    m_knownCaseInsensitiveDisplayNamesToChildElementsMap[tokenStringLowerCase].push_back (pYangElement);
}

void YangElement::getSelectedChildElementsForDataWithExactMatch (const string &tokenString, vector<YangElement *> &selectedChildElementsForData) const
{
    prismAssert (false == tokenString.empty (), __FILE__, __LINE__);

    map<string, vector<YangElement *> >::const_iterator itr = m_knownDisplayNamesToChildElementsMap.find (tokenString);
    map<string, vector<YangElement *> >::const_iterator end = m_knownDisplayNamesToChildElementsMap.end  ();

    if (end != itr)
    {
        prismAssert (false == (itr->second).empty (), __FILE__, __LINE__);
        selectedChildElementsForData = itr->second;

        // trace (TRACE_LEVEL_INFO, "YangElement::getSelectedChildElementsForDataWithExactMatch " + getYangName () + ":" + getName () + " found [" + tokenString + "]");
    }
}

void YangElement::getSelectedChildElementsForData (const string &tokenString, vector<YangElement *> &selectedChildElementsForData) const
{
    prismAssert (false == tokenString.empty (), __FILE__, __LINE__);

    map<string, vector<YangElement *> >::const_iterator itr = m_knownDisplayNamesToChildElementsMap.find (tokenString);
    map<string, vector<YangElement *> >::const_iterator end = m_knownDisplayNamesToChildElementsMap.end  ();

    if (end != itr)
    {
        prismAssert (false == (itr->second).empty (), __FILE__, __LINE__);
        selectedChildElementsForData = itr->second;

        // trace (TRACE_LEVEL_INFO, "YangElement::getSelectedChildElementsForData " + getYangName () + ":" + getName () + " found [" + tokenString + "]");
    }
    else
    {
        string tokenStringLowerCase;
        StringUtils::getLowerCaseString (tokenString, tokenStringLowerCase);

        map<string, vector<YangElement *> >::const_iterator itr2 = m_knownCaseInsensitiveDisplayNamesToChildElementsMap.lower_bound (tokenStringLowerCase);
        map<string, vector<YangElement *> >::const_iterator end2 = m_knownCaseInsensitiveDisplayNamesToChildElementsMap.end ();

        if (end2 != itr2)
        {
            for (; end2 != itr2; itr2++)
            {
                if ((itr2->first).size () < tokenStringLowerCase.size ())
                {
                    break;
                }

                if (0 == (itr2->first).compare (0, tokenStringLowerCase.size (), tokenStringLowerCase))
                {
                    selectedChildElementsForData.insert (selectedChildElementsForData.end (), (itr2->second).begin (), (itr2->second).end ());
#if 0
                    // Need to merge matching elements as per sort priority.
                    // For now, select only first prefix match.
#else
                    break;
#endif
                }
                else
                {
                    break;
                }
            }
        }
    }

    // if (true == selectedChildElementsForData.empty ())
    {
        if (YangContainer::getYangName () == getYangName ())
        {
            map<string, vector<YangElement *> >::const_iterator itr2 = m_knownDisplayNamesToChildElementsMap.find ("");
            map<string, vector<YangElement *> >::const_iterator end2 = m_knownDisplayNamesToChildElementsMap.end  ();

            if (end2 != itr2)
            {
                prismAssert (false == (itr2->second).empty (), __FILE__, __LINE__);
                selectedChildElementsForData = itr2->second;
            }
        }
    }
}

void YangElement::refineFirstLevelNonDataChildElements (const string &childElementYangName, YangElement *pYangElementToBeRefined)
{
    do
    {
        vector<YangElement *> childElementsByYangName;

        getAllChildrenByYangName (childElementYangName, childElementsByYangName);

        UI32 numberOfYangChildElements = childElementsByYangName.size ();

        if (0 == numberOfYangChildElements)
        {
            trace (TRACE_LEVEL_INFO, string ("YangElement::refineFirstLevelNonDataChildElements : ") + childElementYangName + string (" not found."));
            break;
        }

        if (1 == numberOfYangChildElements)
        {
            trace (TRACE_LEVEL_INFO, string ("YangElement::refineFirstLevelNonDataChildElements : One ") + childElementYangName + string (" found."));

            vector<YangElement *> childElementsByYangNameInTarget;

            pYangElementToBeRefined->getAllChildrenByYangName (childElementYangName, childElementsByYangNameInTarget);

            UI32 numberOfChildElementsByYangNameInTarget = childElementsByYangNameInTarget.size ();

            if (0 == numberOfChildElementsByYangNameInTarget)
            {
                // pass
            }
            else if (1 == numberOfChildElementsByYangNameInTarget)
            {
                trace (TRACE_LEVEL_INFO, string ("YangElement::refineFirstLevelNonDataChildElements : removing ") + childElementYangName + string (" from [") + pYangElementToBeRefined->getYangName () + ":" + pYangElementToBeRefined->getName () + string ("]"));

                pYangElementToBeRefined->removeChildElement (childElementsByYangNameInTarget[0]);

                delete childElementsByYangNameInTarget[0];
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }

            trace (TRACE_LEVEL_INFO, string ("YangElement::refineFirstLevelNonDataChildElements : added ") + childElementYangName + string (" to  [") + pYangElementToBeRefined->getYangName () + ":" + pYangElementToBeRefined->getName () + string ("]"));

            YangElement *pClonedChildElement = (childElementsByYangName[0])->clone ();

            pYangElementToBeRefined->addChildElement (pClonedChildElement);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    while (0);
}

void YangElement::refineYangElement (YangElement *pYangElementToBeRefined)
{
    // trace (TRACE_LEVEL_INFO, string ("YangElement::refineYangElement : this     - ") + getYangName () + string (":") + getName ());
    // trace (TRACE_LEVEL_INFO, string ("YangElement::refineYangElement : target   - ") + pYangElementToBeRefined->getYangName () + string (":") + pYangElementToBeRefined->getName ());

    prismAssert (((getYangName () == pYangElementToBeRefined->getYangName ()) || (getYangName () == YangRefine::getYangName ())), __FILE__, __LINE__);

    UI32 numberOfchildren = m_childElements.size ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        if (false == pChildElement->isYangElementForData ())
        {
            // trace (TRACE_LEVEL_INFO, string ("YangElement::refineYangElement : skipped ") + pChildElement->getYangName () + string (":") + pChildElement->getName ());
            continue;
        }

        YangElement *pSimilarChildElementOfYangElementToBeRefined = pYangElementToBeRefined->getSimilarChildElement (pChildElement);

        if (NULL == pSimilarChildElementOfYangElementToBeRefined)
        {
            trace (TRACE_LEVEL_INFO, string ("YangElement::refineYangElement : ") + pChildElement->getYangName () + string (":") + pChildElement->getName () + string (" not found in target."));
            continue;
        }

        pChildElement->refineYangElement (pSimilarChildElementOfYangElementToBeRefined);
    }
}

void YangElement::displayConfiguration1 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    if (true == getIsHidden (pYangDisplayConfigurationContext))
    {
        return;
    }
/*
    if (false == getIsConfiguration ())
    {
        return;
    }
*/

    // pYangDisplayConfigurationContext->debugPrintDisplayState ();

    bool    decrementTokenPositionFlag  = false;
    string  cmdTokenString          = "";
    //int noOfKeyabsorbed = 0;

    if (((false == getDropNodeNameForCli ()) && (YangCase::getYangName () != getYangName ()) && (YangChoice::getYangName () != getYangName ())))
    {
        if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
        {
            cmdTokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

            //trace (TRACE_LEVEL_DEBUG, "YangElement::displayConfiguration1 " + getDisplayName () + " # " + cmdTokenString);

            decrementTokenPositionFlag = true;
        }
    }

    vector<YangElement *> selectedChildElementsForData;

    UI32 numberOfKeysAbsorbed = absorbeKey (pYangDisplayConfigurationContext, selectedChildElementsForData);

    if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

        getSelectedChildElementsForData (tokenString, selectedChildElementsForData);

        pYangDisplayConfigurationContext->decrementTokenPosition ();

        if (0 == selectedChildElementsForData.size ())
        {
            if (true == decrementTokenPositionFlag)
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();
            }

            restoreAbsorbedKeys (pYangDisplayConfigurationContext, numberOfKeysAbsorbed);

#if 0
            while (numberOfKeysAbsorbed > 0)
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();
                numberOfKeysAbsorbed--;
            }
#endif

            return;
        }
    }

#if 0
    if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();
        getSelectedChildElementsForData (tokenString, selectedChildElementsForData);

        if (true == selectedChildElementsForData.empty ())
        {
            // trace (TRACE_LEVEL_DEBUG, "YangElement::displayConfiguration1 " + getYangName () + ":" + getName () + " could not find child elements for [" + tokenString + "]. Checking if it is a key value");

            pYangDisplayConfigurationContext->decrementTokenPosition ();
            noOfKeyabsorbed = absorbeKey (pYangDisplayConfigurationContext , selectedChildElementsForData);

            // tracePrintf (TRACE_LEVEL_DEBUG, "YangElement::displayConfiguration1 : No of keys entered is : [%d]", noOfKeyabsorbed);

            if ((0 == noOfKeyabsorbed) && (true == decrementTokenPosition))
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();
            }

            if (0 == noOfKeyabsorbed)
            {
                return;
            }
        }
        else
        {
            pYangDisplayConfigurationContext->decrementTokenPosition ();
        }
    }
#endif
    if (true == selectedChildElementsForData.empty ())
    {
        selectedChildElementsForData = m_pChildElementsForData;
#if 0
        //trace (TRACE_LEVEL_INFO, "YangElement::displayConfiguration1 : " + getYangName () + ":" + getName () + " selected all child elements for data : ");

        for (UI32 i = 0; i < selectedChildElementsForData.size (); i++)
        {
            //trace (TRACE_LEVEL_INFO, string ("YangElement::displayConfiguration1 : ") + (i + 1) + string (". ") + selectedChildElementsForData[i]->getYangName () + string (":") + selectedChildElementsForData[i]->getName ());
        }
#endif
    }

    string configurationSegmentName = getConfigurationSegmentName ();

    pYangDisplayConfigurationContext->addSlotForPartialOutput ();

    if (false == configurationSegmentName.empty ())
    {
        if (NULL != m_pConfigurationSegmentInformation)
        {
            if (false == pYangDisplayConfigurationContext->isConfigFilterApplicable (this))
            {
                displayConfiguration2 (pYangDisplayConfigurationContext, selectedChildElementsForData);
            }
            else
            {
                // filter discarded.
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration1 : does not know about [") + configurationSegmentName + string ("]"));
        }
    }
    else
    {
        displayConfiguration3 (pYangDisplayConfigurationContext, selectedChildElementsForData);
    }

    pYangDisplayConfigurationContext->discardContentsOfPartialOutputBuffer ();

    restoreAbsorbedKeys (pYangDisplayConfigurationContext, numberOfKeysAbsorbed);

#if 0
    while (numberOfKeysAbsorbed > 0)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
        numberOfKeysAbsorbed--;
    }
#endif

    if (true == decrementTokenPositionFlag)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
    }
}

void YangElement::displayConfiguration2 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
{
    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration2 : Yang Name : [") + m_yangName + string ("]"));
    trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration2 : Name      : [") + m_name     + string ("]"));
}

void YangElement::displayConfiguration3 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
{
    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration3 : Yang Name : [") + m_yangName + string ("]"));
    trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration3 : Name      : [") + m_name     + string ("]"));
}

const vector<YangElement *> &YangElement::getPChildElementsForData () const
{
    return m_pChildElementsForData;
}

bool YangElement::getIsFullCommand () const
{
    return (m_annotationFlags.m_isFullCommand);
}

void YangElement::setIsFullCommand (const bool &isFullCommand)
{
    m_annotationFlags.m_isFullCommand = isFullCommand;
}

bool YangElement::getIsCliShowNo () const
{
    return (m_annotationFlags.m_isCliShowNo);
}

void YangElement::setIsCliShowNo (const bool &isCliShowNo)
{
    m_annotationFlags.m_isCliShowNo = isCliShowNo;
}

bool YangElement::getIsCliAddMode () const
{
    return (m_annotationFlags.m_isCliAddMode);
}

void YangElement::setIsCliAddMode (const bool &isCliAddMode)
{
    m_annotationFlags.m_isCliAddMode = isCliAddMode;
}

bool YangElement::getIsCliSuppressMode () const
{
    return (m_annotationFlags.m_isCliSuppressMode);
}

void YangElement::setIsCliSuppressMode (const bool &isCliSuppressMode)
{
    m_annotationFlags.m_isCliSuppressMode = isCliSuppressMode;
}

void YangElement::setCliCompactSyntax ()
{
    m_annotationFlags.m_cliCompactSyntax = "";
}

string YangElement::getCliCompactSyntax () const
{
    return (m_annotationFlags.m_cliCompactSyntax);
}

void YangElement::setExposeKeyName ()
{
    m_annotationFlags.m_exposeKeyName = m_displayName;
}

string YangElement::getExposeKeyName () const
{
    return (m_annotationFlags.m_exposeKeyName);
}

string YangElement::getInfo () const
{
    return (m_annotationFlags.m_info);
}

void YangElement::setInfo (const string  &info)
{
    m_annotationFlags.m_info = info;
}


bool YangElement::getCliSuppressKeySort () const
{
    return (m_annotationFlags.m_cliSuppressKeySort);
}

void  YangElement::setCliSuppressKeySort (const bool  &cliSuppressKeySort)
{
    m_annotationFlags.m_cliSuppressKeySort = cliSuppressKeySort;
}


bool YangElement::getCliNoMatchCompletion () const
{
    return (m_annotationFlags.m_cliNoMatchCompletion);
}

void YangElement::setCliNoMatchCompletion (const bool  &cliNoMatchCompletion)
{
    m_annotationFlags.m_cliNoMatchCompletion = cliNoMatchCompletion;
}


bool YangElement::getCliSuppressShowConfPath () const
{
    return (m_annotationFlags.m_cliSuppressShowConfPath);
}

void YangElement::setCliSuppressShowConfPath (const bool  &cliSuppressShowConfPath)
{
    m_annotationFlags.m_cliSuppressShowConfPath = cliSuppressShowConfPath;
}

bool YangElement::getCliNoKeyCompletion () const
{
    return (m_annotationFlags.m_cliNoKeyCompletion);
}

void YangElement::setCliNoKeyCompletion (const bool  &cliNoKeyCompletion)
{
    m_annotationFlags.m_cliNoKeyCompletion = cliNoKeyCompletion;
}


bool YangElement::getCliIncompleteCommand () const
{
    return (m_annotationFlags.m_cliIncompleteCommand);
}

void YangElement::setCliIncompleteCommand (const bool  &cliIncompleteCommand)
{
    m_annotationFlags.m_cliIncompleteCommand = cliIncompleteCommand;
}


bool YangElement::getIsCodeName () const
{
    return (m_annotationFlags.m_isCodeName);
}

void YangElement::setIsCodeName (const bool  &isCodeName)
{
    m_annotationFlags.m_isCodeName = isCodeName;
}


bool YangElement::getCliSuppressRange () const
{
    return (m_annotationFlags.m_cliSuppressRange);
}

void YangElement::setCliSuppressRange (const bool  &cliSuppressRange)
{
    m_annotationFlags.m_cliSuppressRange = cliSuppressRange;
}

bool YangElement::getCliSuppressShowMatch () const
{
    return (m_annotationFlags.m_cliSuppressShowMatch);
}

void YangElement::setCliSuppressShowMatch (const bool  &cliSuppressShowMatch)
{
    m_annotationFlags.m_cliSuppressShowMatch = cliSuppressShowMatch;
}

bool YangElement::getIsCliRunTemplate () const
{
    return (m_annotationFlags.m_cliRunTemplate);
}

void YangElement::setIsCliRunTemplate (const bool &isCliRunTemplate)
{
    m_annotationFlags.m_cliRunTemplate = isCliRunTemplate;
}

bool YangElement::getIsCliRunTemplateEnter () const
{
    return (m_annotationFlags.m_cliRunTemplateEnter);
}

void YangElement::setIsCliRunTemplateEnter (const bool &isCliRunTemplateEnter)
{
    m_annotationFlags.m_cliRunTemplateEnter = isCliRunTemplateEnter;
}

void YangElement::setIsCliFlatListSyntax (const bool &cliFlatListSyntax)
{
    m_annotationFlags.m_cliFlatListSyntax = cliFlatListSyntax;
}

bool YangElement::getIsCliFlatListSyntax() const
{
    return m_annotationFlags.m_cliFlatListSyntax;
}

void YangElement::setIsCliMultiValue ()
{
    m_annotationFlags.m_cliMultiValue = true;
}

bool YangElement::getIsCliMultiValue () const
{
    return m_annotationFlags.m_cliMultiValue;
}

void YangElement::setCliHideInSubMode ()
{
    m_annotationFlags.m_cliHideInSubMode = true;
}

bool YangElement::getCliHideInSubMode () const
{
    return m_annotationFlags.m_cliHideInSubMode;
}

UI32 YangElement::absorbeKey (YangDisplayConfigurationContext *pYangDisplayConfigurationContext,vector<YangElement *> &selectedChildElementsForData) const
{
     // trace (TRACE_LEVEL_DEBUG, string ("YangElement::absorbeKey : -------------"));
     return 0;
}

void YangElement::restoreAbsorbedKeys (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const UI32 &numberOfKeysAbsorbed) const
{
}

void YangElement::inlineTypedefFromImportedModule (const YangModuleCollection *pYangModuleCollection)
{
    vector<YangElement *> allTypeChildElements;

    getAllChildrenByYangName (YangType::getYangName (), allTypeChildElements);

    UI32 numberOfTypeChildElements = allTypeChildElements.size ();

    for ( UI32 i = 0; i < numberOfTypeChildElements; i++)
    {
        vector<string>         tokens;
        YangModule            *pOwnerYangModuleForUsedTypedef        = NULL;
        vector<YangElement *>  clonedChildrenForUsedTypedef;

        YangType  *pYangType = dynamic_cast<YangType *> (allTypeChildElements[i]);
        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        string usedTypedefName = pYangType->getName ();

        if (pYangType->isKnownBuiltinType(usedTypedefName) )
        {
            continue ;
        }

        StringUtils::tokenize (usedTypedefName, tokens, ':');
        UI32 numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);
        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            usedTypedefName                     = tokens[1];
            string ownerModuleNamePrefixForUsedTypedef = tokens[0];

            if(tokens[0] == m_pOwnerYangModule->getName ())
            {
                pOwnerYangModuleForUsedTypedef = m_pOwnerYangModule;
            }
            else
            {
                string ownerModuleNameForUsedTypedef = m_pOwnerYangModule->getImportedModuleName (ownerModuleNamePrefixForUsedTypedef);
                pOwnerYangModuleForUsedTypedef = pYangModuleCollection->getYangModuleByName (ownerModuleNameForUsedTypedef);
            }

            if (NULL == pOwnerYangModuleForUsedTypedef)
            {
                continue;
            }

            pOwnerYangModuleForUsedTypedef->getClonedChildrenForTypedef (usedTypedefName, clonedChildrenForUsedTypedef);

            addChildElementsBefore (pYangType, clonedChildrenForUsedTypedef);

            removeChildElement (allTypeChildElements[i]);
            delete allTypeChildElements[i];
        }
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];
        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->inlineTypedefFromImportedModule (pYangModuleCollection);
    }
}

void YangElement::inlineTypedefInsideModule ()
{
    vector<YangElement *> allTypeChildElements;

    getAllChildrenByYangName (YangType::getYangName (), allTypeChildElements);

    UI32 numberOfTypeChildElements = allTypeChildElements.size ();

    for (UI32 i = 0; i < numberOfTypeChildElements; i++)
    {
        string                 ownerModuleNamePrefixForUsedTypedef;
        string                 ownerModuleNameForUsedTypedef;
        vector<string>         tokens;
        vector<YangElement *>  clonedChildrenForUsedTypedef;

        YangType *pYangType = dynamic_cast<YangType *> (allTypeChildElements[i]);
        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        string usedTypedefName = pYangType->getName ();

        if (pYangType->isKnownBuiltinType(usedTypedefName) )
        {
            continue ;
        }

        StringUtils::tokenize (usedTypedefName, tokens, ':');
        UI32 numberOfTokens = tokens.size ();

        prismAssert (2 >= numberOfTokens, __FILE__, __LINE__);
        prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

        if (2 == numberOfTokens)
        {
            continue;  // to be processed in 2nd pass
        }

        pYangType->getclonedChildrenForTypedefFromParents(usedTypedefName,clonedChildrenForUsedTypedef);

        addChildElementsBefore (pYangType, clonedChildrenForUsedTypedef);

        removeChildElement (allTypeChildElements[i]);
        delete allTypeChildElements[i];
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];
        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->inlineTypedefInsideModule ();
    }
}

void YangElement::processTypeInformations ()
{
    vector<YangElement *> allTypeChildElements;

    getAllChildrenByYangName (YangType::getYangName (), allTypeChildElements);

    UI32 numberOfTypeChildElements = allTypeChildElements.size ();

    for (UI32 i = 0; i < numberOfTypeChildElements; i++)
    {
        YangType  *pYangType = dynamic_cast<YangType *> (allTypeChildElements[i]);
        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        string usedTypeName = pYangType->getName ();

        if ( "enumeration" == usedTypeName )
        {
            vector<YangElement *> allEnumChildElements;
            UI32                  counter                   = 0;    // next enum value.

            pYangType->getAllChildrenByYangName (YangEnum::getYangName (), allEnumChildElements);

            UI32 numberOfEnumChildElements = allEnumChildElements.size();

            for (UI32 j = 0; j < numberOfEnumChildElements; j++)
            {
                UI32                    enumValue = 0;
                vector<YangElement*>    allValueChildElements;

                YangEnum  *pYangEnum  = dynamic_cast<YangEnum *> (allEnumChildElements[j]);
                prismAssert (NULL != pYangEnum, __FILE__, __LINE__);

                pYangEnum->getAllChildrenByYangName (YangValue::getYangName (), allValueChildElements);

                UI32 numberOfValueChildElements = allValueChildElements.size();
                prismAssert (2 > numberOfValueChildElements, __FILE__, __LINE__);

                if (numberOfValueChildElements == 1)
                {
                    YangValue  *pYangValue  = dynamic_cast<YangValue *> (allValueChildElements[0]);
                    prismAssert (NULL != pYangValue, __FILE__, __LINE__);

                    enumValue = pYangValue->getValue();

                    counter = enumValue + 1;
                }
                else
                {
                    enumValue = counter;
                    counter++;
                }

                string enumName  = pYangEnum->getName();
                pYangType->populateEnumAttribute (enumName, enumValue);
            }
        }
        else if ( "bits" == usedTypeName )
        {
            vector<YangElement *> allBitChildElements;

            pYangType->getAllChildrenByYangName (YangBit::getYangName (), allBitChildElements);

            UI32 numberOfBitChildElements = allBitChildElements.size();

            for (UI32 j = 0; j < numberOfBitChildElements; j++)
            {
                vector<YangElement*>    allPositionChildElements;

                YangBit  *pYangBit  = dynamic_cast<YangBit *> (allBitChildElements[j]);
                prismAssert (NULL != pYangBit, __FILE__, __LINE__);

                pYangBit->getAllChildrenByYangName (YangPosition::getYangName (), allPositionChildElements);

                UI32 numberOfPositionChildElements = allPositionChildElements.size();
                prismAssert (1 == numberOfPositionChildElements, __FILE__, __LINE__);

                YangPosition  *pYangPosition  = dynamic_cast<YangPosition *> (allPositionChildElements[0]);
                prismAssert (NULL != pYangPosition, __FILE__, __LINE__);

                UI32 bitPosition = pYangPosition->getValue();

                string bitName  = pYangBit->getName();

                pYangType->populateBitMapAttribute (bitName, bitPosition);
            }
        }
    }

    UI32 numberOfChildElements = m_childElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = m_childElements[i];
        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->processTypeInformations ();
    }
}


void YangElement::getclonedChildrenForTypedefFromParents(const string &typedefName , vector<YangElement *> &clonedChildrenForUsedTypedef)
{
    prismAssert (NULL != m_pParentElement , __FILE__, __LINE__);

    vector<YangElement *> allTypedefChildElements;
    bool found =false;
    vector <YangElement *> allTypeChildElements;

    m_pParentElement->getAllChildrenByYangName(YangTypedef::getYangName (), allTypedefChildElements);

    UI32 size = allTypedefChildElements.size();

    for (UI32 i=0 ;i < size ; i++)
    {
         string childName = allTypedefChildElements[i]->getName();

         if(typedefName == childName)
         {
             found = true;
             allTypedefChildElements[i]->getClonedChildren (clonedChildrenForUsedTypedef);
             break;
         }
    }

    if (!found)
    {
        m_pParentElement->getclonedChildrenForTypedefFromParents(typedefName, clonedChildrenForUsedTypedef);
    }
}

UI32 YangElement::getNumberOfAllChildYangElementsInTree () const
{
    UI32 numberOfAllChildElementsInTree = 0;

    UI32 numberOfChildElements = m_childElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        numberOfAllChildElementsInTree++;

        YangElement *pChildElement = m_childElements[i];
        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        numberOfAllChildElementsInTree += pChildElement->getNumberOfAllChildYangElementsInTree ();
    }

    return numberOfAllChildElementsInTree;
}

void YangElement::incrementNumberOfInstancesOfYangElementsInSystem ()
{
    s_numberOfInstancesOfYangElementMutex.lock ();

    s_numberOfInstancesOfYangElement++;

    s_numberOfInstancesOfYangElementMutex.unlock ();
}

void YangElement::decrementNumberOfInstancesOfYangElementsInSystem ()
{
    s_numberOfInstancesOfYangElementMutex.lock ();

    s_numberOfInstancesOfYangElement--;

    s_numberOfInstancesOfYangElementMutex.unlock ();
}

UI32 YangElement::getNumberOfInstancesOfYangElementsInSystem ()
{
    UI32 numberOfInstancesOfYangElement = 0;

    s_numberOfInstancesOfYangElementMutex.lock ();

    numberOfInstancesOfYangElement = s_numberOfInstancesOfYangElement;

    s_numberOfInstancesOfYangElementMutex.unlock ();

    return numberOfInstancesOfYangElement;
}

bool YangElement::getCameFromAugment () const
{
    return m_cameFromAugment;
}

void YangElement::setCameFromAugment ()
{
    m_cameFromAugment = true;
}

string YangElement::getOriginalModuleNameSpaceUri () const
{
    return m_originalModuleNameSpaceUri;
}

void YangElement::setOriginalModuleNameSpaceUri (const string &originalModuleNameSpaceUri)
{
    m_originalModuleNameSpaceUri = originalModuleNameSpaceUri;
}

void YangElement::setOriginalModuleNameSpaceUriForSelf ()
{
    prismAssert (NULL != m_pOwnerYangModule, __FILE__, __LINE__);

    m_originalModuleNameSpaceUri = m_pOwnerYangModule->getNamespaceUri ();
}

void YangElement::setOriginalModuleNameSpaceUriForTree ()
{
    setOriginalModuleNameSpaceUriForSelf ();

    UI32 numberOfChildElements = m_childElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildYangElement = m_childElements[i];

        prismAssert (NULL != pChildYangElement, __FILE__, __LINE__);

        pChildYangElement->setOriginalModuleNameSpaceUriForTree ();
    }
}

void YangElement::rearrangeAugmenterChildElements ()
{
    UI32 numberOfChildElements = m_childElements.size ();

    vector<YangElement *>::iterator itr = m_childElements.begin ();

    for (; itr != m_childElements.end (); itr++)
    {
        if (true == (*itr)->getCameFromAugment ())
        {
            break;
        }
    }

    if (itr != m_childElements.end ())
    {
        std::stable_sort (itr, m_childElements.end (), YangElement::compareYangElementsByOriginalModuleNameSpaceUri);
        // std::reverse     (itr, m_childElements.end ());
    }

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildYangElement = m_childElements[i];

        prismAssert (NULL != pChildYangElement, __FILE__, __LINE__);

        pChildYangElement->rearrangeAugmenterChildElements ();
    }
}

bool YangElement::compareYangElementsByOriginalModuleNameSpaceUri (YangElement *pYangElement1, YangElement *pYangElement2)
{
    prismAssert (NULL != pYangElement1, __FILE__, __LINE__);
    prismAssert (NULL != pYangElement2, __FILE__, __LINE__);

    string originalModuleNameSpaceUri1 = pYangElement1->getOriginalModuleNameSpaceUri ();
    string originalModuleNameSpaceUri2 = pYangElement2->getOriginalModuleNameSpaceUri ();

    return (originalModuleNameSpaceUri1 > originalModuleNameSpaceUri2); // reverse sort ?
}

void YangElement::prepareTypeInformation ()
{
    UI32 numberOfchildren = m_childElements.size ();

    prepareTypeInformationForSelf ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->prepareTypeInformation ();
    }
}

void YangElement::prepareTypeInformationForSelf ()
{
    // virtual.
}

void YangElement::collectHideInSubModeLeaf ()
{
    UI32 numberOfchildren = m_childElements.size ();

    collectHideInSubModeLeafForSelf ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->collectHideInSubModeLeaf ();
    }
}

void YangElement::collectHideInSubModeLeafForSelf ()
{
    // virtual.
}

void YangElement::propagateCompactSyntaxOneLevel ()
{
    UI32 numberOfchildren = m_childElements.size ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->propagateCompactSyntaxOneLevel ();
    }

    propagateCompactSyntaxOneLevelForSelf ();
}

void YangElement::propagateCompactSyntaxOneLevelForSelf ()
{
    // virtual.
}

void YangElement::collectInformationAboutChildLeafWithCustomDisplay ()
{
    UI32 numberOfchildren = m_childElements.size ();

    collectInformationAboutChildLeafWithCustomDisplayForSelf ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->collectInformationAboutChildLeafWithCustomDisplay ();
    }
}

void YangElement::collectInformationAboutChildLeafWithCustomDisplayForSelf ()
{
    // virtual.
}

ResourceId YangElement::doesElementValueMatch (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const string &matchValue)
{
    return WAVE_MESSAGE_SUCCESS;
}

WaveYangMemberElementFilterInformation *YangElement::collectMemberElementFilterInformation (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const
{
    return NULL;
}

UI32 YangElement::getMaxNestingDepthForYangLists () const
{
    UI32 maxNextingDepthForYangLists = 0;

    UI32 numberOfchildren = m_childElements.size ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        UI32 nextingDepthForYangLists = pChildElement->getMaxNestingDepthForYangLists ();

        if (maxNextingDepthForYangLists < nextingDepthForYangLists)
        {
            maxNextingDepthForYangLists = nextingDepthForYangLists;
        }
    }

    if (YangList::getYangName () == getYangName ())
    {
        maxNextingDepthForYangLists++;
    }

    return maxNextingDepthForYangLists;
}

void YangElement::computeIsInLocalConfigSubtree ()
{
    if (false == (getConfigurationSegmentName ()).empty ())
    {
        ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation ();

        if (NULL == pConfigurationSegmentInformation)
        {
            return;
        }

        if (true == pConfigurationSegmentInformation->getIsLocalManagedObject ())
        {
            markEntireSubtreeAsLocalConfig ();
        }
    }
    else
    {
        UI32 numberOfchildren = m_childElements.size ();

        for (UI32 i = 0; i < numberOfchildren; i++)
        {
            YangElement *pChildElement = m_childElements[i];

            prismAssert (NULL != pChildElement, __FILE__, __LINE__);

            pChildElement->computeIsInLocalConfigSubtree ();
        }
    }
}

void YangElement::markEntireSubtreeAsLocalConfig ()
{
    UI32 numberOfchildren = m_childElements.size ();

    setIsInLocalConfigurationSubtree ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->markEntireSubtreeAsLocalConfig ();
    }
}

bool YangElement::considerForHFileForCLanguage () const
{
    return (false);
}

bool YangElement::considerSubTreeForHFileForCLanguage () const
{
    return (true);
}

void YangElement::getNameForHFileForCLanguage (string &actualName, string &processedNameForCLanguage, string &cLanguageValue) const
{
    string nameForHFileForCLanguage = getVariableNameInProgrammingLanguage ();

    if (NULL != m_pOwnerYangModule)
    {
        string modulePrefix = m_pOwnerYangModule->getPrefix ();

        if (("" != modulePrefix) && ("" != nameForHFileForCLanguage))
        {
            nameForHFileForCLanguage = modulePrefix + "_" + nameForHFileForCLanguage;
        }
    }

    StringUtils::replaceAllInstancesOfInputCharWith (nameForHFileForCLanguage, '-', '_');
    StringUtils::replaceAllInstancesOfInputStringWith (nameForHFileForCLanguage, "/", "0x2f");
    StringUtils::replaceAllInstancesOfInputStringWith (nameForHFileForCLanguage, "+", "0x2b");
    StringUtils::replaceAllInstancesOfInputStringWith (nameForHFileForCLanguage, ".", "0x2e");

    actualName                = getVariableNameInProgrammingLanguage ();
    processedNameForCLanguage = nameForHFileForCLanguage;
    cLanguageValue            = "";

    return;
}

void YangElement::getAllExplicitlySetNamesForHFileForCLanguage (map<string, string> &allNamesForHFileForCLanguage, map<string, string> &allValuesForHFileForCLanguage) const
{
    UI32   numberOfchildren         = m_childElements.size ();
    UI32   i                        = 0;
    string actualName;
    string nameForHFileForCLanguage;
    string cLanguageValue;

    getNameForHFileForCLanguage (actualName, nameForHFileForCLanguage, cLanguageValue);

    if ("" != m_programmingLanguageVariableName)
    {
        map<string, string>::const_iterator element    = allNamesForHFileForCLanguage.find (actualName);
        map<string, string>::const_iterator endElement = allNamesForHFileForCLanguage.end  ();

        if ((YangEnum::getYangName ()) == (getYangName ()))
        {
            if (endElement == element)
            {
                allNamesForHFileForCLanguage[actualName]  = nameForHFileForCLanguage;
                allValuesForHFileForCLanguage[actualName] = cLanguageValue;
            }
        }
        else
        {
            allNamesForHFileForCLanguage[actualName]  = nameForHFileForCLanguage;
            allValuesForHFileForCLanguage[actualName] = cLanguageValue;
        }
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->getAllExplicitlySetNamesForHFileForCLanguage (allNamesForHFileForCLanguage, allValuesForHFileForCLanguage);
    }

}

void YangElement::getAllNamesForHFileForCLanguage (map<string, string> &allNamesForHFileForCLanguage, map<string, string> &allValuesForHFileForCLanguage) const
{
    UI32   numberOfchildren         = m_childElements.size ();
    UI32   i                        = 0;
    string actualName;
    string nameForHFileForCLanguage;
    string cLanguageValue;

    getNameForHFileForCLanguage (actualName, nameForHFileForCLanguage, cLanguageValue);

    if (("" != actualName) && ((true == considerForHFileForCLanguage ()) || ("" != m_programmingLanguageVariableName)))
    {
        map<string, string>::const_iterator element    = allNamesForHFileForCLanguage.find (actualName);
        map<string, string>::const_iterator endElement = allNamesForHFileForCLanguage.end  ();

        if ((YangEnum::getYangName ()) == (getYangName ()))
        {
            if (endElement == element)
            {
                allNamesForHFileForCLanguage[actualName]  = nameForHFileForCLanguage;
                allValuesForHFileForCLanguage[actualName] = cLanguageValue;
            }
        }
        else
        {
            allNamesForHFileForCLanguage[actualName]  = nameForHFileForCLanguage;
            allValuesForHFileForCLanguage[actualName] = cLanguageValue;
        }
    }

    if (true == (considerSubTreeForHFileForCLanguage ()))
    {
        for (i = 0; i < numberOfchildren; i++)
        {
            YangElement *pChildElement = m_childElements[i];

            prismAssert (NULL != pChildElement, __FILE__, __LINE__);

            pChildElement->getAllNamesForHFileForCLanguage (allNamesForHFileForCLanguage, allValuesForHFileForCLanguage);
        }
    }

    for (i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = m_childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->getAllExplicitlySetNamesForHFileForCLanguage (allNamesForHFileForCLanguage, allValuesForHFileForCLanguage);
    }

}

void YangElement::printAllNamesForHFileForCLanguage (const YangModuleCollection &yangModuleCollection, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    map<string, string> allNamesForHFileForCLanguage;
    map<string, string> allValuesForHFileForCLanguage;

    getAllNamesForHFileForCLanguage (allNamesForHFileForCLanguage, allValuesForHFileForCLanguage);

    map<string, string>::const_iterator element    = allNamesForHFileForCLanguage.begin ();
    map<string, string>::const_iterator endElement = allNamesForHFileForCLanguage.end   ();

    while (endElement != element)
    {
        string actualName                = element->first;
        string processedNameForCLanguage = element->second;
        string cLanguageValue            = allValuesForHFileForCLanguage[actualName];

        if ("" != processedNameForCLanguage)
        {
            if ("" == cLanguageValue)
            {
                UI32 wyserUserTag = yangModuleCollection.getUserTagValueByName (actualName);

                prismAssert (0 != wyserUserTag, __FILE__, __LINE__);

                fprintf (pFile, "#define %s %u\n", processedNameForCLanguage.c_str (), wyserUserTag);
            }
            else
            {
                fprintf (pFile, "#define %s %s\n", processedNameForCLanguage.c_str (), cLanguageValue.c_str ());
            }
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        element++;
    }
}

void YangElement::printAllNamesToFileForHFileForCLanguage (const string &filePath, const YangModuleCollection &yangModuleCollection, const string &prefix) const
{
    FILE *pFile = NULL;

    string hFilePrefix;
    string hFilePostfix;

    if (NULL != m_pOwnerYangModule)
    {
        m_pOwnerYangModule->getPreifxAndPostfixStringsForHFileForCLanguage (hFilePrefix, hFilePostfix, yangModuleCollection);
    }

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            fprintf (pFile, "%s", hFilePrefix.c_str ());

            printAllNamesForHFileForCLanguage (yangModuleCollection, pFile);

            fprintf (pFile, "%s", hFilePostfix.c_str ());

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        fprintf (pFile, "%s", hFilePrefix.c_str ());

        printAllNamesForHFileForCLanguage (yangModuleCollection, pFile);

        fprintf (pFile, "%s", hFilePostfix.c_str ());
    }
}


void YangElement::generateHFileForCLanguage (const YangModuleCollection &yangModuleCollection) const
{
    prismAssert (false, __FILE__, __LINE__);
}

string YangElement::getVariableNameInProgrammingLanguage () const
{
    if ("" != m_programmingLanguageVariableName)
    {
        return (m_programmingLanguageVariableName);
    }
    else
    {
        return (m_name);
    }
}

bool YangElement::isInsideAnRpc () const
{
    bool rpcInParentHierarchy = false;

    YangElement *pParentYangElement = m_pParentElement;

    while (NULL != pParentYangElement)
    {
        if ((YangRpc::getYangName ()) == (pParentYangElement->getYangName ()))
        {
            rpcInParentHierarchy = true;

            break;
        }

        pParentYangElement = pParentYangElement->getPParentElement ();
    }

    return (rpcInParentHierarchy);
}

}

