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
 *   Author : Vidyasagara Reddy Guntaka / Mandar Datar                     *
 ***************************************************************************/

#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/XPath/XPathExpression.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>

#include "Modeling/XPath/WaveXPathExpressionElement.h"
#include "Framework/Utils/AssertUtils.h"

#include "Modeling/YANG/ObjectModel/YangElement.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>

XALAN_CPP_NAMESPACE_USE

namespace WaveNs
{

class PrefixResolverForYin : public PrefixResolver
{
    public:
        PrefixResolverForYin    ();
        PrefixResolverForYin    (const PrefixResolverForYin &src);
        virtual                        ~PrefixResolverForYin    ();
        virtual const XalanDOMString   *getNamespaceForPrefix   (const XalanDOMString &prefix) const;
        virtual const XalanDOMString   &getURI                  () const;

        // Now the data members

    private:
        XalanDOMString  m_baseUri;
        XalanDOMString  m_unknownUri;

        // namespace prefix information from YangModule should be used.
};

PrefixResolverForYin::PrefixResolverForYin ()
    : PrefixResolver    (),
    m_baseUri         ("base-uri"),
    m_unknownUri      ("unknown-uri")
{
}

PrefixResolverForYin::PrefixResolverForYin (const PrefixResolverForYin &src)
    : PrefixResolver    (src)
{
}

PrefixResolverForYin::~PrefixResolverForYin ()
{
}

const XalanDOMString *PrefixResolverForYin::getNamespaceForPrefix (const XalanDOMString &prefix) const
{
    return &m_unknownUri;
}

const XalanDOMString &PrefixResolverForYin::getURI () const
{
    return m_baseUri;
}



// WaveLocationPath

WaveLocationPath::WaveLocationPath ()
    : m_isAbsolute                          (true),
      m_numberOfStepsToReachRelativeRoot    (0)
{
}

WaveLocationPath::~WaveLocationPath ()
{
}

void WaveLocationPath::setIsAbsolute (const bool &isAbsolute)
{
    if (true == isAbsolute)
    {
        prismAssert (false != m_isAbsolute, __FILE__, __LINE__);
    }
    else
    {
        m_isAbsolute = false;
    }
}

void WaveLocationPath::incrementNumberOfStepsToReachRelativeRoot ()
{
    prismAssert (false == m_isAbsolute, __FILE__, __LINE__);
    m_numberOfStepsToReachRelativeRoot++;
}

void WaveLocationPath::addForwardStep (const string &stepName, const string &namespaceName)
{
    prismAssert (false == stepName.empty (), __FILE__, __LINE__);

    m_forwardSteps.push_back (stepName);
    m_forwardStepNamespaceStrings.push_back (namespaceName);
}

void WaveLocationPath::debugPrint (const UI32 &depth) const
{
    string spaceString (depth * 4, ' ');

    cout << spaceString << "location path [" << m_stringRepresentation << "]" << "\r\n";
}

bool WaveLocationPath::getIsAbsolute () const
{
    return m_isAbsolute;
}

UI32 WaveLocationPath::getNumberOfStepsToReachRelativeRoot () const
{
    prismAssert (false == m_isAbsolute, __FILE__, __LINE__);

    return m_numberOfStepsToReachRelativeRoot;
}

string WaveLocationPath::getXPathStringForAbsolutePath () const
{
    prismAssert (true == m_isAbsolute, __FILE__, __LINE__);

    return m_stringRepresentation;
}

UI32 WaveLocationPath::getNumberOfForwardSteps () const
{
    UI32 numberOfForwardSteps = m_forwardSteps.size ();

    return numberOfForwardSteps;
}

void WaveLocationPath::getForwardStepNameAtIndex (const UI32 &index, string &forwardStepName, string &forwardStepNamespaceString) const
{
    prismAssert (index < m_forwardSteps.size (), __FILE__, __LINE__);

    forwardStepName             = m_forwardSteps[index];
    forwardStepNamespaceString  = m_forwardStepNamespaceStrings[index];
}

void WaveLocationPath::computeStringRepresentation ()
{
    if (true == m_isAbsolute)
    {
        m_stringRepresentation += "/";
    }
    else
    {
        for (UI32 i = 0; i < m_numberOfStepsToReachRelativeRoot; i++)
        {
            m_stringRepresentation += "../";
        }
    }

    UI32 numberOfForwardSteps = m_forwardSteps.size ();

    prismAssert (numberOfForwardSteps == m_forwardStepNamespaceStrings.size (), __FILE__, __LINE__);

    for (UI32 i = 0; i < numberOfForwardSteps; i++)
    {
        if (false == (m_forwardStepNamespaceStrings[i]).empty ())
        {
            m_stringRepresentation += m_forwardStepNamespaceStrings[i];
            m_stringRepresentation += ":";
        }

        m_stringRepresentation += m_forwardSteps[i];

        if (i != (numberOfForwardSteps - 1))
        {
            m_stringRepresentation += "/";
        }
    }
}

// WaveXPathExpressionValue

WaveXPathExpressionValue::WaveXPathExpressionValue (const string &stringValue)
    : m_valueString     (stringValue)
{
}

WaveXPathExpressionValue::WaveXPathExpressionValue (const UI32 &intValue)
{
    m_valueString = string ("") + intValue;
}

WaveXPathExpressionValue::WaveXPathExpressionValue (const bool &booleanValue)
{
    if (true == booleanValue)
    {
        m_valueString = "true";
    }
    else
    {
        m_valueString = "false";
    }
}

WaveXPathExpressionValue::WaveXPathExpressionValue ()
{
}

WaveXPathExpressionValue::WaveXPathExpressionValue (const WaveXPathExpressionValue &rhsValue)
    : m_valueString     (rhsValue.m_valueString)
{
}

WaveXPathExpressionValue::~WaveXPathExpressionValue ()
{
}

WaveXPathExpressionValue &WaveXPathExpressionValue::operator = (const WaveXPathExpressionValue &rhsValue)
{
    m_valueString = rhsValue.m_valueString;

    return *this;
}

string WaveXPathExpressionValue::getStringValue () const
{
    return m_valueString;
}

UI32 WaveXPathExpressionValue::getUI32Value () const
{
    UI32 ui32Value = strtoll (m_valueString.c_str (), NULL, 10);

    return ui32Value;
}

bool WaveXPathExpressionValue::getIntegerValue (SI64 &ui64Value) const
{
    if (true == m_valueString.empty ())
    {
        return false;
    }

    char *firstInvalidCharacterPosition = NULL;

    ui64Value = strtoll (m_valueString.c_str (), &firstInvalidCharacterPosition, 10);

    if (*firstInvalidCharacterPosition != '\0')
    {
        return false;
    }

    return true;
}

bool WaveXPathExpressionValue::getBoolValue () const
{
    bool boolValue = false;

    if ("true" == m_valueString)
    {
        boolValue = true;
    }
    else if ("false" == m_valueString)
    {
        boolValue = false;
    }
    else
    {
        SI64 ui64Value = 0;

        bool isValidInteger = getIntegerValue (ui64Value);

        if (true == isValidInteger)
        {
            if (0 != ui64Value)
            {
                boolValue = true;
            }
        }
        else if (false == m_valueString.empty ())
        {
            boolValue = true;
        }
    }

    return boolValue;
}

void WaveXPathExpressionValue::setValueFromString (const string &stringValue)
{
    m_valueString = stringValue;
}

void WaveXPathExpressionValue::setValueFromUI32 (const UI32 &ui32Value)
{
    m_valueString = string ("") + ui32Value;
}

void WaveXPathExpressionValue::setValueFromBool (const bool &boolValue)
{
    if (true == boolValue)
    {
        m_valueString = "true";
    }
    else
    {
        m_valueString = "false";
    }
}

void WaveXPathExpressionValue::debugPrint (const UI32 &depth) const
{
    string spaceString (depth * 4, ' ');

    cout << spaceString << "value [" << m_valueString << "]\r\n";
}


// WaveXPathExpressionEvaluationContext

WaveXPathExpressionEvaluationContext::WaveXPathExpressionEvaluationContext ()
{
}

WaveXPathExpressionEvaluationContext::~WaveXPathExpressionEvaluationContext ()
{
}

/*
pure virtual
WaveXPathExpressionValue WaveXPathExpressionEvaluationContext::evaluateValueOfLocationPath (const WaveLocationPath &locationPath, const YangElement *pContextYangElement)
{
    return WaveXPathExpressionValue (false);
}
*/

// WaveXPathExpressionElement

WaveXPathExpressionElement::WaveXPathExpressionElement ()
{
}

WaveXPathExpressionElement::~WaveXPathExpressionElement ()
{
    UI32 numberOfChildElements = m_pChildElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        delete m_pChildElements[i];
    }
}

void WaveXPathExpressionElement::addChildElement (WaveXPathExpressionElement *pWaveXPathExpressionElement)
{
    prismAssert (NULL != pWaveXPathExpressionElement, __FILE__, __LINE__);

    m_pChildElements.push_back (pWaveXPathExpressionElement);
}

UI32 WaveXPathExpressionElement::getNumberOfChildElements () const
{
    UI32 numberOfChildElements = m_pChildElements.size ();

    return numberOfChildElements;
}

WaveXPathExpressionElement *WaveXPathExpressionElement::getFirstChildElement () const
{
    prismAssert (1 <= m_pChildElements.size (), __FILE__, __LINE__);

    WaveXPathExpressionElement *pWaveXPathExpressionElement = m_pChildElements[0];

    prismAssert (NULL != pWaveXPathExpressionElement, __FILE__, __LINE__);

    return pWaveXPathExpressionElement;
}

WaveXPathExpressionElement *WaveXPathExpressionElement::getSecondChildElement () const
{
    prismAssert (2 <= m_pChildElements.size (), __FILE__, __LINE__);

    WaveXPathExpressionElement *pWaveXPathExpressionElement = m_pChildElements[1];

    prismAssert (NULL != pWaveXPathExpressionElement, __FILE__, __LINE__);

    return pWaveXPathExpressionElement;
}

void WaveXPathExpressionElement::debugPrintChildElements (const UI32 &depth) const
{
    UI32 numberOfChildElements = m_pChildElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        WaveXPathExpressionElement *pChildElement = m_pChildElements[i];
        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->debugPrint (depth);
    }
}

UI32 WaveXPathExpressionElement::getHeaderOffsetInParserObject () const
{
    return 2;
}

UI32 WaveXPathExpressionElement::loadFromParserXPathObject (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position)
{
    UI32 size = pXPathExpression->getOpCodeMapValue (position + 1);

    for (UI32 i = position + getHeaderOffsetInParserObject (); i < (position + size); i = i)
    {
        XPathExpression::eOpCodes opCode = (XPathExpression::eOpCodes) (pXPathExpression->getOpCodeMapValue (i));

        if (XPathExpression::eENDOP == opCode)
        {
            i += 1;

            continue; // break?
        }

        WaveXPathExpressionElement *pWaveXPathExpressionElement = NULL;

        if (XPathExpression::eOP_LITERAL == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionLiteral ();
        }
        else if (XPathExpression::eOP_NUMBERLIT == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionNumberLit ();
        }
        else if (XPathExpression::eOP_XPATH == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionXPath ();
        }
        else if (XPathExpression::eOP_LOCATIONPATH == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionLocationPath ();
        }
        else if (XPathExpression::eOP_AND == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionAnd ();
        }
        else if (XPathExpression::eOP_OR == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionOr ();
        }
        else if (XPathExpression::eOP_EQUALS == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionEquals ();
        }
        else if (XPathExpression::eOP_NOTEQUALS == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionNotEquals ();
        }
        else if (XPathExpression::eOP_LT == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionLt ();
        }
        else if (XPathExpression::eOP_GT == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionGt ();
        }
        else if (XPathExpression::eOP_GROUP == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionGroup ();
        }
        else if (XPathExpression::eOP_FUNCTION_NOT == opCode)
        {
            pWaveXPathExpressionElement = new WaveXPathExpressionFunctionNot ();
        }
        else if (XPathExpression::eOP_FUNCTION == opCode)
        {
            int functionIndex = pXPathExpression->getOpCodeMapValue (i + 2);

            stringstream functionNameStream;

            const XPath::FunctionTableType &ft = XPath::getFunctionTable ();

            functionNameStream << ft.s_functionNames[functionIndex].m_name;

            if (WaveXPathExpressionFunctionContains::getName () == functionNameStream.str ())
            {
                pWaveXPathExpressionElement = new WaveXPathExpressionFunctionContains ();
            }
            else
            {
                cerr << "WaveXPathExpressionXPath::loadFromParserXPathObject : does not know about [" << functionNameStream.str () << "]" << "\r\n";
            }
        }
        else
        {
            cerr << "unknown opCode [" << opCode << "] at i = [" << i << "]" << "\r\n";

            prismAssert (false, __FILE__, __LINE__);
        }

        if (NULL != pWaveXPathExpressionElement)
        {
            // cout << "opCode [" << opCode << "] at i = [" << i << "]" << "\r\n";

            UI32 spaceConsumed = pWaveXPathExpressionElement->loadFromParserXPathObject (pXPathExpression, i);

            // cout << "opCode [" << opCode << "] at i = [" << i << "], load returned [" << spaceConsumed << "]" << "\r\n";

            i += spaceConsumed;

            addChildElement (pWaveXPathExpressionElement);

            // pWaveXPathExpressionElement->debugPrint (10);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    return size;
}



WaveXPathExpressionLiteral::WaveXPathExpressionLiteral ()
    : WaveXPathExpressionElement    (),
      m_literalValue                ("")
{
}

WaveXPathExpressionLiteral::~WaveXPathExpressionLiteral ()
{
}

UI32 WaveXPathExpressionLiteral::loadFromParserXPathObject (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position)
{
    int tokenLocation = pXPathExpression->getOpCodeMapValue (position + 2);

    const XToken* pXToken = pXPathExpression->getToken (tokenLocation);

    stringstream literalValueStream;

    literalValueStream << pXToken->str ();

    m_literalValue.setValueFromString (literalValueStream.str ());

    return 3;
}

void WaveXPathExpressionLiteral::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Literal [" << m_literalValue.getStringValue () << "]\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionLiteral::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    return m_literalValue;
}



WaveXPathExpressionNumberLit::WaveXPathExpressionNumberLit ()
    : WaveXPathExpressionElement    (),
      m_numberValue                 ((UI32) 0)
{
}

WaveXPathExpressionNumberLit::~WaveXPathExpressionNumberLit ()
{
}

UI32 WaveXPathExpressionNumberLit::loadFromParserXPathObject (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position)
{
    int tokenLocation = pXPathExpression->getOpCodeMapValue (position + 3);

    const XToken* pXToken = pXPathExpression->getToken (tokenLocation);

    stringstream literalValueStream;

    literalValueStream << pXToken->str ();

    string numericValue = literalValueStream.str ();

    UI32 numberValue = strtoll (numericValue.c_str (), NULL, 10);

    m_numberValue.setValueFromUI32 (numberValue);

    return 4;
}

void WaveXPathExpressionNumberLit::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "NumberLit [" << m_numberValue.getStringValue () << "]\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionNumberLit::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    return m_numberValue;
}



WaveXPathExpressionXPath::WaveXPathExpressionXPath ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionXPath::~WaveXPathExpressionXPath ()
{
}

void WaveXPathExpressionXPath::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "XPath" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionXPath::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (1 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue = pChildElement->evaluateElement (evaluationContext, pContextYangElement);

    return xpathValue;
}



WaveXPathExpressionGroup::WaveXPathExpressionGroup ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionGroup::~WaveXPathExpressionGroup ()
{
}

void WaveXPathExpressionGroup::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Group" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionGroup::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (1 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue = pChildElement->evaluateElement (evaluationContext, pContextYangElement);

    return xpathValue;
}


WaveXPathExpressionLocationPath::WaveXPathExpressionLocationPath ()
    : WaveXPathExpressionElement    (),
      m_locationPath                ()
{
}

WaveXPathExpressionLocationPath::~WaveXPathExpressionLocationPath ()
{
}

UI32 WaveXPathExpressionLocationPath::loadFromParserXPathObject (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position)
{
    UI32 size = pXPathExpression->getOpCodeMapValue (position + 1);

    for (UI32 i = position + 2; i < (position + size); i = i)
    {
        XPathExpression::eOpCodes opCode = (XPathExpression::eOpCodes) (pXPathExpression->getOpCodeMapValue (i));

        if (XPathExpression::eENDOP == opCode)
        {
            i += 1;

            continue;
            // break;
        }

        if (XPathExpression::eFROM_ROOT == opCode)
        {
            m_locationPath.setIsAbsolute (true);

            i += pXPathExpression->getOpCodeMapValue (i + 1);
        }
        else if (XPathExpression::eFROM_PARENT == opCode)
        {
            m_locationPath.setIsAbsolute (false);

            m_locationPath.incrementNumberOfStepsToReachRelativeRoot ();

            i += pXPathExpression->getOpCodeMapValue (i + 1);
        }
        else if (XPathExpression::eFROM_CHILDREN == opCode)
        {
            if (XPathExpression::eNODENAME == pXPathExpression->getOpCodeMapValue (i + 3))
            {
                string namespaceName;

                int namespaceTokenLocation = pXPathExpression->getOpCodeMapValue (i + 4);

                if (XPathExpression::eEMPTY != namespaceTokenLocation)
                {
                    const XToken* pXTokenForNamespace = pXPathExpression->getToken (namespaceTokenLocation);

                    stringstream stepNameNamespaceStream;
                    
                    stepNameNamespaceStream << pXTokenForNamespace->str ();

                    namespaceName = stepNameNamespaceStream.str ();
                }

                int tokenLocation = pXPathExpression->getOpCodeMapValue (i + 5);

                const XToken* pXToken = pXPathExpression->getToken (tokenLocation);

                stringstream stepNameStream;

                stepNameStream << pXToken->str ();

                string stepName = stepNameStream.str ();

                m_locationPath.addForwardStep (stepName, namespaceName);
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }

            i += pXPathExpression->getOpCodeMapValue (i + 1);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    m_locationPath.computeStringRepresentation ();

    return size;
}

void WaveXPathExpressionLocationPath::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "LocationPath" << "\r\n";

    m_locationPath.debugPrint (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionLocationPath::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    return evaluationContext.evaluateValueOfLocationPath (m_locationPath, pContextYangElement);
}

WaveXPathExpressionFunctionNot::WaveXPathExpressionFunctionNot ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionFunctionNot::~WaveXPathExpressionFunctionNot ()
{
}

void WaveXPathExpressionFunctionNot::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "FunctionNot" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionFunctionNot::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (1 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue = pChildElement->evaluateElement (evaluationContext, pContextYangElement);

    if (true == xpathValue.getBoolValue ())
    {
        return WaveXPathExpressionValue ((bool) false);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) true);
    }
}


WaveXPathExpressionFunctionContains::WaveXPathExpressionFunctionContains ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionFunctionContains::~WaveXPathExpressionFunctionContains ()
{
}

WaveXPathExpressionValue WaveXPathExpressionFunctionContains::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    string stringValue1 = xpathValue1.getStringValue ();
    string stringValue2 = xpathValue2.getStringValue ();

    size_t matchPosition = stringValue1.find (stringValue2);

    if (string::npos == matchPosition)
    {
        return WaveXPathExpressionValue ((bool) false);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) true);
    }
}

void WaveXPathExpressionFunctionContains::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "FunctionContains" << "\r\n";

    debugPrintChildElements (depth + 1);
}

string WaveXPathExpressionFunctionContains::getName ()
{
    return "contains";
}

UI32 WaveXPathExpressionFunctionContains::getHeaderOffsetInParserObject () const
{
    return 4;
}


WaveXPathExpressionAnd::WaveXPathExpressionAnd ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionAnd::~WaveXPathExpressionAnd ()
{
}

void WaveXPathExpressionAnd::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "And" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionAnd::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    if (false == xpathValue1.getBoolValue ())
    {
        return WaveXPathExpressionValue ((bool) false);
    }

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    return xpathValue2;
}




WaveXPathExpressionOr::WaveXPathExpressionOr ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionOr::~WaveXPathExpressionOr ()
{
}

void WaveXPathExpressionOr::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Or" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionOr::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    if (true == xpathValue1.getBoolValue ())
    {
        return WaveXPathExpressionValue ((bool) true);
    }

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    return xpathValue2;
}

WaveXPathExpressionEquals::WaveXPathExpressionEquals ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionEquals::~WaveXPathExpressionEquals ()
{
}

void WaveXPathExpressionEquals::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Equals" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionEquals::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    string stringValue1 = xpathValue1.getStringValue ();
    string stringValue2 = xpathValue2.getStringValue ();

    if (stringValue1 == stringValue2)
    {
        return WaveXPathExpressionValue ((bool) true);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) false);
    }
}

WaveXPathExpressionNotEquals::WaveXPathExpressionNotEquals ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionNotEquals::~WaveXPathExpressionNotEquals ()
{
}

void WaveXPathExpressionNotEquals::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "NotEquals" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionNotEquals::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    string stringValue1 = xpathValue1.getStringValue ();
    string stringValue2 = xpathValue2.getStringValue ();

    if (stringValue1 == stringValue2)
    {
        return WaveXPathExpressionValue ((bool) false);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) true);
    }
}

WaveXPathExpressionLt::WaveXPathExpressionLt ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionLt::~WaveXPathExpressionLt ()
{
}

void WaveXPathExpressionLt::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Lt" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionLt::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    UI32 ui32Value1 = xpathValue1.getUI32Value ();
    UI32 ui32Value2 = xpathValue2.getUI32Value ();

    if (ui32Value1 < ui32Value2)
    {
        return WaveXPathExpressionValue ((bool) true);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) false);
    }
}

WaveXPathExpressionGt::WaveXPathExpressionGt ()
    : WaveXPathExpressionElement    ()
{
}

WaveXPathExpressionGt::~WaveXPathExpressionGt ()
{
}

void WaveXPathExpressionGt::debugPrint (const UI32 &depth) const
{
    cout << string (depth * 4, ' ') << "Gt" << "\r\n";

    debugPrintChildElements (depth + 1);
}

WaveXPathExpressionValue WaveXPathExpressionGt::evaluateElement (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const
{
    UI32 numberOfChildElements = getNumberOfChildElements ();

    prismAssert (2 == numberOfChildElements, __FILE__, __LINE__);

    WaveXPathExpressionElement *pChildElement1 = getFirstChildElement ();

    WaveXPathExpressionValue xpathValue1 = pChildElement1->evaluateElement (evaluationContext, pContextYangElement);

    WaveXPathExpressionElement *pChildElement2 = getSecondChildElement ();

    WaveXPathExpressionValue xpathValue2 = pChildElement2->evaluateElement (evaluationContext, pContextYangElement);

    UI32 ui32Value1 = xpathValue1.getUI32Value ();
    UI32 ui32Value2 = xpathValue2.getUI32Value ();

    if (ui32Value1 > ui32Value2)
    {
        return WaveXPathExpressionValue ((bool) true);
    }
    else
    {
        return WaveXPathExpressionValue ((bool) false);
    }
}




WaveXPathExpressionElement *WaveXPathExpressionElement::buildWaveXPathExpressionElementTree (const string &xpathExpressionString)
{
    WaveXPathExpressionElement *pWaveXPathExpressionElement = new WaveXPathExpressionXPath ();
    prismAssert (NULL != pWaveXPathExpressionElement, __FILE__, __LINE__);

    XMLPlatformUtils::Initialize();         // [
    XPathEvaluator::initialize();       // [

    {
        PrefixResolverForYin    prefixResolverForYinInstance;
        XPathEvaluator          theEvaluator1;

        XPath                  *pXPath = theEvaluator1.createXPath (XalanDOMString (xpathExpressionString.c_str ()).c_str (), prefixResolverForYinInstance);

        const XPathExpression&  currentExpression = pXPath->getExpression();

        pWaveXPathExpressionElement->loadFromParserXPathObject (&currentExpression, 0);
    }

    XPathEvaluator::terminate();        // ]
    XMLPlatformUtils::Terminate();          // ]

    return pWaveXPathExpressionElement;
}

}
