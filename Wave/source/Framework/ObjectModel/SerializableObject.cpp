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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Messaging/MessageFactory/PrismMessageFactory.h"
#include "Version/PrismVersion.h"
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/framework/MemBufInputSource.hpp>
//#include <xercesc/framework/MemBufFormatTarget.hpp>
//#include <xercesc/framework/StdOutFormatTarget.hpp>
//#include <xercesc/parsers/XercesDOMParser.hpp>

//using namespace XERCES_CPP_NAMESPACE;

namespace WaveNs
{

SerializableObject::SerializableObject ()
    : m_isPreparedForSerialization (false)
{
}

SerializableObject::~SerializableObject ()
{
}

void SerializableObject::setIsPreparedForSerialization (bool isPreparedForSerialization)
{
    m_isPreparedForSerialization = isPreparedForSerialization;
}

void SerializableObject::addSerializableAttribute (Attribute *pAttribute)
{
    m_serializableAttributes.addAttribute (pAttribute);
}

void SerializableObject::addAttributeNameForOrderToNameMapping (const string& attributeName)
{
    m_serializableAttributes.addAttributeForOrderToNameMapping (attributeName);
}

void SerializableObject::setupAttributesForSerialization ()
{
}

void SerializableObject::setupAttributesForSerializationInAttributeOrderFormat ()
{
}

void SerializableObject::prepareForSerialization ()
{
    if (false == m_isPreparedForSerialization)
    {
        setupAttributesForSerialization ();
        setupAttributesForSerializationInAttributeOrderFormat ();
        m_isPreparedForSerialization = true;
    }
}

void SerializableObject::serialize (string &serializedData)
{
    trace (TRACE_LEVEL_FATAL, "SerializableObject::serialize : TEMPORARILY DISABLED.  MUST NOT USE THIS METHOD AT THIS TIME.");
    prismAssert (false, __FILE__, __LINE__);

#if 0
    static PrismMutex serializationMutex;

    serializationMutex.lock ();

    static const XMLCh              *pLsString           = XMLString::transcode ("LS");
//    static const XMLCh              *pNewLineString      = XMLString::transcode ("\n");
    static const XMLCh              *pDocumentTypeString = XMLString::transcode ("SO");

    static       DOMImplementation  *pDomImplementation  = DOMImplementationRegistry::getDOMImplementation (pLsString);
    static       DOMWriter          *pSerializer         = pDomImplementation->createDOMWriter ();
    static       MemBufFormatTarget *pTarget             = new MemBufFormatTarget (512);
    static       DOMDocumentType    *pDomDocumentType    = pDomImplementation->createDocumentType (pDocumentTypeString, NULL, NULL);
    static       DOMDocument        *pDomDocument        = pDomImplementation->createDocument (pDocumentTypeString, pDocumentTypeString, pDomDocumentType);

    if (NULL != pDomDocument)
    {
        m_serializableAttributes.serializeTo (pDomDocument);
        pTarget->reset ();
//        pSerializer->setNewLine (pNewLineString);
        pSerializer->writeNode (pTarget, *pDomDocument);

        // FIXME : sagar : Replace the following explicit conversion to const char * from the getRawBuffer to a more meaningful conversion.

        serializedData = reinterpret_cast<const char *> (pTarget->getRawBuffer ());

//        delete pDomDocument;

        // Remove all the child nodes added prior to this call.

        DOMElement *pRootElement = pDomDocument->getDocumentElement ();
        DOMNode    *pFirstNode   = NULL;

        prismAssert (NULL != pRootElement, __FILE__, __LINE__);

        while (NULL != (pFirstNode = (pRootElement->getFirstChild ())))
        {
            pRootElement->removeChild (pFirstNode);
            pFirstNode->release ();
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "SerializableObject::serialize : Could not create a Dom Document.");
        prismAssert (false, __FILE__, __LINE__);
    }

//    delete pDomDocumentType;
//    delete pTarget;
//    pSerializer->release ();
//    delete pSerializer;

    serializationMutex.unlock ();
#endif
}

void SerializableObject::loadFromSerializableObject (SerializableObject *pSerializableObject)
{
    m_serializableAttributes.copyFrom (pSerializableObject->m_serializableAttributes);
}

bool SerializableObject::isAttributeToBeLoadedFromSerializedData (SerializableObjectAttributeId soaid)
{
    return (true);
}

void SerializableObject::loadFromSerializedData (const string &serializedData)
{
    trace (TRACE_LEVEL_FATAL, "SerializableObject::loadFromSerializedData : TEMPORARILY DISABLED.  MUST NOT USE THIS METHOD AT THIS TIME.");
    prismAssert (false, __FILE__, __LINE__);

#if 0
    static PrismMutex loadFromSerializedDataMutex;

    loadFromSerializedDataMutex.lock ();

    // Make sure that the attributes are setup for serialization purposes.

    prepareForSerialization ();

    static       XercesDOMParser *pParser = new XercesDOMParser;
    static const char            *pId     = "SO-";

    pParser->setValidationScheme (XercesDOMParser::Val_Auto);
    pParser->setDoNamespaces (false);
    pParser->setDoSchema (false);
    pParser->setCreateEntityReferenceNodes (false);

    MemBufInputSource *pInputSource = new MemBufInputSource ((const XMLByte *) serializedData.c_str (), strlen (serializedData.c_str ()), pId, false);

    pParser->parse (*pInputSource);

    DOMDocument *pDocument  = pParser->getDocument ();
    DOMElement  *pRootNode  =  (pDocument->getDocumentElement ());
    DOMNode     *pChildNode = NULL;

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
        {
            char *pAttributeTag = NULL;
            char *pAttributeValue = NULL;

            pAttributeTag = XMLString::transcode (pChildNode->getNodeName ());
//            cout << pAttributeTag << ", ";


            DOMNode *pTempNode = pChildNode->getFirstChild ();

            // In case of a string data type, we can encounter null string value.  This causes the pTempNode to be NULL.

            if (NULL != pTempNode)
            {
                pAttributeValue = XMLString::transcode (pTempNode->getNodeValue ());
            }
            else
            {
                pAttributeValue = "";
            }
//            cout << pAttributeValue << endl;

            SerializableObjectAttributeId  attributeTag = strtoul (pAttributeTag + 1, NULL, 10); // pAttributeTag always starts with 'A', so skip it.  Rest of the string is the attribute id.
            Attribute                     *pAttribute   = m_serializableAttributes.getAttribute (attributeTag);

            pAttribute->fromString (pAttributeValue);

            XMLString::release (&pAttributeTag);
            XMLString::release (&pAttributeValue);
        }
    }

    // Remove all the child nodes added prior to this call.

    DOMNode *pFirstNode = NULL;

    prismAssert (NULL != pRootNode, __FILE__, __LINE__);

    while (NULL != (pFirstNode = (pRootNode->getFirstChild ())))
    {
        pRootNode->removeChild (pFirstNode);
        pFirstNode->release ();
    }

    pParser->resetDocumentPool ();

//    delete pParser;
    delete pInputSource;

    loadFromSerializedDataMutex.unlock ();

#endif
}

void SerializableObject::loadFromSerializedData (const UI8 *pData, const UI32 dataSize)
{
    trace (TRACE_LEVEL_FATAL, "SerializableObject::loadFromSerializedData : TEMPORARILY DISABLED.  MUST NOT USE THIS METHOD AT THIS TIME.");
    prismAssert (false, __FILE__, __LINE__);

#if 0
    static PrismMutex loadFromSerializedDataMutex;

    loadFromSerializedDataMutex.lock ();

    // Make sure that the attributes are setup for serialization purposes.

    prepareForSerialization ();

    static       XercesDOMParser *pParser = new XercesDOMParser;
    static const char            *pId     = "SO-";

    pParser->setValidationScheme (XercesDOMParser::Val_Auto);
    pParser->setDoNamespaces (false);
    pParser->setDoSchema (false);
    pParser->setCreateEntityReferenceNodes (false);

    MemBufInputSource *pInputSource = new MemBufInputSource ((const XMLByte *) pData, dataSize, pId, false);

    pParser->parse (*pInputSource);

    DOMDocument *pDocument  = pParser->getDocument ();
    DOMElement  *pRootNode  =  (pDocument->getDocumentElement ());
    DOMNode     *pChildNode = NULL;

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
        {
            char *pAttributeTag = NULL;
            char *pAttributeValue = NULL;

            pAttributeTag = XMLString::transcode (pChildNode->getNodeName ());
//            cout << pAttributeTag << ", ";


            DOMNode *pTempNode = pChildNode->getFirstChild ();

            // In case of a string data type, we can encounter null string value.  This causes the pTempNode to be NULL.

            if (NULL != pTempNode)
            {
                pAttributeValue = XMLString::transcode (pTempNode->getNodeValue ());
            }
            else
            {
                pAttributeValue = "";
            }
//            cout << pAttributeValue << endl;

            SerializableObjectAttributeId  attributeTag = strtoul (pAttributeTag + 1, NULL, 10); // pAttributeTag always starts with 'A', so skip it.  Rest of the string is the attribute id.
            Attribute                     *pAttribute   = m_serializableAttributes.getAttribute (attributeTag);

            pAttribute->fromString (pAttributeValue);

            XMLString::release (&pAttributeTag);
            XMLString::release (&pAttributeValue);
        }
    }

    // Remove all the child nodes added prior to this call.

    DOMNode *pFirstNode = NULL;

    prismAssert (NULL != pRootNode, __FILE__, __LINE__);

    while (NULL != (pFirstNode = (pRootNode->getFirstChild ())))
    {
        pRootNode->removeChild (pFirstNode);
        pFirstNode->release ();
    }

    pParser->resetDocumentPool ();

//    delete pParser;
    delete pInputSource;

    loadFromSerializedDataMutex.unlock ();

#endif
}

SerializableObject *SerializableObject::createAndLoadFromSerializedData (const string &serializedData)
{
    trace (TRACE_LEVEL_FATAL, "SerializableObject::createAndLoadFromSerializedData : TEMPORARILY DISABLED.  MUST NOT USE THIS METHOD AT THIS TIME.");
    prismAssert (false, __FILE__, __LINE__);

    return (NULL);

#if 0
    static PrismMutex createAndLoadFromSerializedDataMutex;

    createAndLoadFromSerializedDataMutex.lock ();

    static       XercesDOMParser *pParser = new XercesDOMParser;
    static const char            *pId     = "SO-";

    pParser->setValidationScheme (XercesDOMParser::Val_Auto);
    pParser->setDoNamespaces (false);
    pParser->setDoSchema (false);
    pParser->setCreateEntityReferenceNodes (false);

    MemBufInputSource *pInputSource = new MemBufInputSource ((const XMLByte *) serializedData.c_str (), strlen (serializedData.c_str ()), pId, false);

    pParser->parse (*pInputSource);

    DOMDocument        *pDocument           = pParser->getDocument ();
    DOMElement         *pRootNode           =  (pDocument->getDocumentElement ());
    DOMNode            *pChildNode          = NULL;
    SerializableObject *pSerializableObject = NULL;

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
        {
            char *pAttributeTag = NULL;
            char *pAttributeValue = NULL;

            pAttributeTag = XMLString::transcode (pChildNode->getNodeName ());
//            cout << pAttributeTag << ", ";


            DOMNode *pTempNode = pChildNode->getFirstChild ();

            // In case of a string data type, we can encounter null string value.  This causes the pTempNode to be NULL.

            if (NULL != pTempNode)
            {
                pAttributeValue = XMLString::transcode (pTempNode->getNodeValue ());
            }
            else
            {
                pAttributeValue = "";
            }
//            cout << pAttributeValue << endl;

            SerializableObjectAttributeId  attributeTag = strtoul (pAttributeTag + 1, NULL, 10); // pAttributeTag always starts with 'A', so skip it.  Rest of the string is the attribute id.

            // We know that the first attributeTag corresponds to the serailizableobjecttype.
            // If this changes then we need to change the logic.  We do this for efficiency.
            // If pSerializableObject is NULL then we know that we got the first attributetag from the serialized data.

            if (NULL == pSerializableObject)
            {
                SerializableObjectType sot = strtoul (pAttributeValue, NULL, 10);

                pSerializableObject = PrismMessageFactory::getMessageInstance (sot);
                prismAssert (NULL != pSerializableObject, __FILE__, __LINE__);

                if (NULL != pSerializableObject)
                {
                    // Make sure that the attributes are setup for serialization purposes.

                    pSerializableObject->prepareForSerialization ();
                }
            }

            if (NULL != pSerializableObject)
            {
                Attribute *pAttribute  = pSerializableObject->m_serializableAttributes.getAttribute (attributeTag);

                pAttribute->fromString (pAttributeValue);
            }

            XMLString::release (&pAttributeTag);
            XMLString::release (&pAttributeValue);
        }

        if (NULL == pSerializableObject)
        {
            break;
        }
    }

    // Remove all the child nodes added prior to this call.

    DOMNode *pFirstNode = NULL;

    prismAssert (NULL != pRootNode, __FILE__, __LINE__);

    while (NULL != (pFirstNode = (pRootNode->getFirstChild ())))
    {
        pRootNode->removeChild (pFirstNode);
        pFirstNode->release ();
    }

    pParser->resetDocumentPool ();

//    delete pParser;
    delete pInputSource;

    createAndLoadFromSerializedDataMutex.unlock ();

    return (pSerializableObject);

#endif
}

SerializableObject *SerializableObject::createAndLoadFromSerializedData (const UI8 *pData, const UI32 dataSize)
{
    trace (TRACE_LEVEL_FATAL, "SerializableObject::createAndLoadFromSerializedData : TEMPORARILY DISABLED.  MUST NOT USE THIS METHOD AT THIS TIME.");
    prismAssert (false, __FILE__, __LINE__);

    return (NULL);

#if 0
    static PrismMutex createAndLoadFromSerializedDataMutex;

    createAndLoadFromSerializedDataMutex.lock ();

    static       XercesDOMParser *pParser = new XercesDOMParser;
    static const char            *pId     = "SO-";

    pParser->setValidationScheme (XercesDOMParser::Val_Auto);
    pParser->setDoNamespaces (false);
    pParser->setDoSchema (false);
    pParser->setCreateEntityReferenceNodes (false);

    MemBufInputSource *pInputSource = new MemBufInputSource ((const XMLByte *) pData, dataSize, pId, false);

    pParser->parse (*pInputSource);

    DOMDocument        *pDocument           = pParser->getDocument ();
    DOMElement         *pRootNode           =  (pDocument->getDocumentElement ());
    DOMNode            *pChildNode          = NULL;
    SerializableObject *pSerializableObject = NULL;

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
        {
            char *pAttributeTag = NULL;
            char *pAttributeValue = NULL;

            pAttributeTag = XMLString::transcode (pChildNode->getNodeName ());
//            cout << pAttributeTag << ", ";


            DOMNode *pTempNode = pChildNode->getFirstChild ();

            // In case of a string data type, we can encounter null string value.  This causes the pTempNode to be NULL.

            if (NULL != pTempNode)
            {
                pAttributeValue = XMLString::transcode (pTempNode->getNodeValue ());
            }
            else
            {
                pAttributeValue = "";
            }
//            cout << pAttributeValue << endl;

            SerializableObjectAttributeId  attributeTag = strtoul (pAttributeTag + 1, NULL, 10); // pAttributeTag always starts with 'A', so skip it.  Rest of the string is the attribute id.

            // We know that the first attributeTag corresponds to the serailizableobjecttype.
            // If this changes then we need to change the logic.  We do this for efficiency.
            // If pSerializableObject is NULL then we know that we got the first attributetag from the serialized data.

            if (NULL == pSerializableObject)
            {
                SerializableObjectType sot = strtoul (pAttributeValue, NULL, 10);

                pSerializableObject = PrismMessageFactory::getMessageInstance (sot);
                prismAssert (NULL != pSerializableObject, __FILE__, __LINE__);

                if (NULL != pSerializableObject)
                {
                    // Make sure that the attributes are setup for serialization purposes.

                    pSerializableObject->prepareForSerialization ();
                }
            }

            if (NULL != pSerializableObject)
            {
                Attribute *pAttribute  = pSerializableObject->m_serializableAttributes.getAttribute (attributeTag);

                pAttribute->fromString (pAttributeValue);
            }

            XMLString::release (&pAttributeTag);
            XMLString::release (&pAttributeValue);
        }

        if (NULL == pSerializableObject)
        {
            break;
        }
    }

    // Remove all the child nodes added prior to this call.

    DOMNode *pFirstNode = NULL;

    prismAssert (NULL != pRootNode, __FILE__, __LINE__);

    while (NULL != (pFirstNode = (pRootNode->getFirstChild ())))
    {
        pRootNode->removeChild (pFirstNode);
        pFirstNode->release ();
    }

    pParser->resetDocumentPool ();

//    delete pParser;
    delete pInputSource;

    createAndLoadFromSerializedDataMutex.unlock ();

    return (pSerializableObject);
#endif
}

void SerializableObject::loadFromSerializedData2 (const string &serializedData, const UI8 serializationType) 
{
    prepareForSerialization ();

    tracePrintf (TRACE_LEVEL_DEVEL, true, false, "SerializableObject::loadFromSerializedData2: serializationType = %d", serializationType); 

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {
        m_serializableAttributes.loadFromSerializedDataInAttributeOrderFormat (serializedData);
    }
    else
    {
        m_serializableAttributes.loadFromSerializedData (serializedData);
    }
}

void SerializableObject::serialize2 (string &serializedData, const UI8 serializationType) 
{
    prepareForSerialization ();

    tracePrintf (TRACE_LEVEL_DEVEL, true, false, "SerializableObject::serialize2: serializationType = %d", serializationType); 

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {
        m_serializableAttributes.serializeToInAttributeOrderFormat (serializedData);
    }
    else
    {
        m_serializableAttributes.serializeTo (serializedData);
    }

    return;
}

Attribute *SerializableObject::getAttribute (const string &attributeName)
{
    return (m_serializableAttributes.getAttribute (attributeName));
}

void SerializableObject::getAllAttributeNames (vector<string> &allAttributeNames) const
{
    m_serializableAttributes.getAllAttributeNames (allAttributeNames);
}

void SerializableObject::copyToAttributesMap (AttributesMap &attributesMap) const
{
    SerializableObject *pSerializableObject = const_cast<SerializableObject *> (this);

    pSerializableObject->prepareForSerialization ();

    attributesMap.copyFrom (m_serializableAttributes);
}

SerializableObject &SerializableObject::operator = (const SerializableObject &serializableObject)
{
    prepareForSerialization ();

    serializableObject.copyToAttributesMap (m_serializableAttributes);

    return (*this);    
}

}
