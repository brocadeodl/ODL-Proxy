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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Types/UI32VectorUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UI32VectorUC::UI32VectorUC ()
{
    m_pUI32Vector.clear ();
    m_pIsUserConfigured = false;
}

UI32VectorUC::UI32VectorUC (const vector<UI32> &ui32Vector)
{
    m_pUI32Vector       = ui32Vector;
    m_pIsUserConfigured = false;
}

UI32VectorUC::UI32VectorUC (const vector<UI32> &ui32Vector, const bool &isUserConfigured)
{
    m_pUI32Vector       = ui32Vector;
    m_pIsUserConfigured = isUserConfigured;
}

UI32VectorUC::UI32VectorUC (const UI32VectorUC &ui32VectorUC)
{
    m_pUI32Vector       = ui32VectorUC.getUI32Vector ();
    m_pIsUserConfigured = ui32VectorUC.m_pIsUserConfigured;
}

UI32VectorUC::~UI32VectorUC ()
{
}

void UI32VectorUC::setUI32Vector(const vector<UI32> &ui32Vector)
{
    m_pUI32Vector = ui32Vector;
}

vector<UI32> UI32VectorUC::getUI32Vector() const
{
    return (m_pUI32Vector);
}

void UI32VectorUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool UI32VectorUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

void UI32VectorUC::toString (string &valueString)
{
    vector<UI32>::iterator element = m_pUI32Vector.begin ();
    vector<UI32>::iterator end     = m_pUI32Vector.end ();

    valueString = "";

    while (end != element)
    {
        valueString += string ("#") + (*element);
        element++;
    }

    valueString += string (":") + (m_pIsUserConfigured ? 1 : 0);
}

void UI32VectorUC::fromString (const string &ui32VectorUCInStringFormat)
{
    string tempString = ui32VectorUCInStringFormat;
    vector<string>  token;
    UI32 numberOfToken = 0;
    UI32 tempIsUserConfigured;

    tokenize (tempString, token, ':');

    numberOfToken = token.size ();

    prismAssert (2 >= numberOfToken, __FILE__, __LINE__);

    if (numberOfToken > 1)
    {
        tempIsUserConfigured =  strtoul (token[1].c_str (), NULL, 10);
        m_pIsUserConfigured  =  tempIsUserConfigured ? true : false;
    }
    else
    {
        m_pIsUserConfigured = true;
    }

    if ((numberOfToken > 0) && (token[0].length() > 1))
    {
        // Remove the first '#' character
        tempString = token[0].substr(1);

        vector<UI32>::iterator begin = m_pUI32Vector.begin ();
        vector<UI32>::iterator end   = m_pUI32Vector.end ();

        m_pUI32Vector.erase (begin, end);

        UI32 value = 0;
        vector<string>  vectorTokens;
        UI32 numberOfVectorTokens = 0;
    
        tokenize (tempString, vectorTokens, '#');
    
        numberOfVectorTokens = vectorTokens.size ();
    
        for (UI32 i = 0; i < numberOfVectorTokens; i++)
        {
            value = strtoul (vectorTokens[i].c_str (), NULL, 10);
            m_pUI32Vector.push_back (value);
        }
    }
}

bool UI32VectorUC::operator == (const UI32VectorUC &ui32VectorUC) const
{
    if ((m_pUI32Vector != ui32VectorUC.getUI32Vector ()) || (m_pIsUserConfigured != ui32VectorUC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool UI32VectorUC::operator != (const UI32VectorUC &ui32VectorUC) const
{
    return (! (operator == (ui32VectorUC)));
}

UI32VectorUC &UI32VectorUC::operator = (const UI32VectorUC &ui32VectorUC)
{
    m_pUI32Vector       = ui32VectorUC.getUI32Vector ();
    m_pIsUserConfigured = ui32VectorUC.m_pIsUserConfigured;

    return (*this);
}

}
