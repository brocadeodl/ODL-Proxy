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

#ifndef WAVEXPATHEXPRESSIONELEMENT_H
#define WAVEXPATHEXPRESSIONELEMENT_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/StringUtils.h"

#include <string>
#include <vector>

#include <xalanc/Include/PlatformDefinitions.hpp>

using namespace std;

XALAN_DECLARE_CLASS(XALAN_CPP_NAMESPACE,XPathExpression)

namespace WaveNs
{

class YangElement;

class WaveLocationPath
{
    public:
                        WaveLocationPath                            ();
                       ~WaveLocationPath                            ();

        void            setIsAbsolute                               (const bool &isAbsolute);
        void            incrementNumberOfStepsToReachRelativeRoot   ();
        void            addForwardStep                              (const string &stepName, const string &namespaceName);

        void            debugPrint                                  (const UI32 &depth) const;

        bool            getIsAbsolute                               () const;
        UI32            getNumberOfStepsToReachRelativeRoot         () const;
        string          getXPathStringForAbsolutePath               () const;
        UI32            getNumberOfForwardSteps                     () const;
        void            getForwardStepNameAtIndex                   (const UI32 &index, string &forwardStepName, string &forwardStepNamespaceString) const;
        void            computeStringRepresentation                 ();

    private:
        bool            m_isAbsolute;
        UI32            m_numberOfStepsToReachRelativeRoot;
        vector<string>  m_forwardSteps;
        vector<string>  m_forwardStepNamespaceStrings;
        string          m_stringRepresentation;
};

// Wrapper for string data.
class WaveXPathExpressionValue
{
    public:
                                                    WaveXPathExpressionValue        (const string &stringValue);
                                                    WaveXPathExpressionValue        (const UI32 &intValue);
                                                    WaveXPathExpressionValue        (const bool &booleanValue);
                                                    WaveXPathExpressionValue        ();
        /*virtual*/                                ~WaveXPathExpressionValue        ();

                                                    WaveXPathExpressionValue        (const WaveXPathExpressionValue &value);
                    WaveXPathExpressionValue       &operator =                      (const WaveXPathExpressionValue &value);

                    string                          getStringValue                  () const;
                    UI32                            getUI32Value                    () const;
                    bool                            getBoolValue                    () const;

                    void                            setValueFromString              (const string &stringValue);
                    void                            setValueFromUI32                (const UI32 &ui32Value);
                    void                            setValueFromBool                (const bool &boolValue);

                    void                            debugPrint                      (const UI32 &depth) const;
                    bool                            getIntegerValue                 (SI64 &ui64Value) const;

    // Now the data members

    private:
        string      m_valueString;
};

class WaveXPathExpressionEvaluationContext
{
    public:
                                            WaveXPathExpressionEvaluationContext    ();
        virtual                            ~WaveXPathExpressionEvaluationContext    ();

        virtual WaveXPathExpressionValue    evaluateValueOfLocationPath             (const WaveLocationPath &locationPath, const YangElement *pContextYangElement) = 0;

    private:
};

class WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionElement      ();
        virtual                                ~WaveXPathExpressionElement      ();

                void                            addChildElement                 (WaveXPathExpressionElement *pWaveXPathExpressionElement);
        virtual UI32                            getHeaderOffsetInParserObject   () const;
        virtual UI32                            loadFromParserXPathObject       (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position);

                UI32                            getNumberOfChildElements        () const;
                WaveXPathExpressionElement     *getFirstChildElement            () const;
                WaveXPathExpressionElement     *getSecondChildElement           () const;

        virtual void                            debugPrint                      (const UI32 &depth) const = 0;
                void                            debugPrintChildElements         (const UI32 &depth) const;
    // public:
        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const = 0;

        static  WaveXPathExpressionElement     *buildWaveXPathExpressionElementTree (const string &xpathExpressionString);

    private:
        vector<WaveXPathExpressionElement *>    m_pChildElements;
};

class WaveXPathExpressionLiteral : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionLiteral      ();
        virtual                                ~WaveXPathExpressionLiteral      ();

        virtual UI32                            loadFromParserXPathObject       (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position);
        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
        WaveXPathExpressionValue        m_literalValue;
};

class WaveXPathExpressionNumberLit : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionNumberLit    ();
        virtual                                ~WaveXPathExpressionNumberLit    ();

        virtual UI32                            loadFromParserXPathObject       (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position);
        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
        WaveXPathExpressionValue        m_numberValue;
};

class WaveXPathExpressionXPath : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionXPath        ();
        virtual                                ~WaveXPathExpressionXPath        ();

        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionGroup : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionGroup        ();
        virtual                                ~WaveXPathExpressionGroup        ();

        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionLocationPath : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionLocationPath ();
        virtual                                ~WaveXPathExpressionLocationPath ();

        virtual UI32                            loadFromParserXPathObject       (const XALAN_CPP_NAMESPACE_QUALIFIER XPathExpression *pXPathExpression, const UI32 &position);
        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
        WaveLocationPath                m_locationPath;
};

class WaveXPathExpressionFunctionNot : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionFunctionNot  ();
        virtual                                ~WaveXPathExpressionFunctionNot  ();

        virtual WaveXPathExpressionValue        evaluateElement                 (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                      (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionFunctionContains : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionFunctionContains ();
        virtual                                ~WaveXPathExpressionFunctionContains ();

        virtual UI32                            getHeaderOffsetInParserObject       () const;
        static  string                          getName                             ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionAnd : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionAnd              ();
        virtual                                ~WaveXPathExpressionAnd              ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};




class WaveXPathExpressionOr : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionOr               ();
        virtual                                ~WaveXPathExpressionOr               ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionEquals : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionEquals           ();
        virtual                                ~WaveXPathExpressionEquals           ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionNotEquals : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionNotEquals        ();
        virtual                                ~WaveXPathExpressionNotEquals        ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionLt : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionLt               ();
        virtual                                ~WaveXPathExpressionLt               ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

class WaveXPathExpressionGt : public WaveXPathExpressionElement
{
    public:
                                                WaveXPathExpressionGt               ();
        virtual                                ~WaveXPathExpressionGt               ();

        virtual WaveXPathExpressionValue        evaluateElement                     (WaveXPathExpressionEvaluationContext &evaluationContext, const YangElement *pContextYangElement) const;
        virtual void                            debugPrint                          (const UI32 &depth) const;

    private:
};

}

#endif // WAVEXPATHEXPRESSIONELEMENT_H
