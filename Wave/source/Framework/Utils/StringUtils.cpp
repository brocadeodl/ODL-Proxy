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

#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/Types.h"

#include <algorithm>

#include <stdio.h>
#include <ctype.h>

namespace WaveNs
{

string operator + (const string &string1, const unsigned long long int &ulli1)
{
    char tempBuffer[64] = {0};

    snprintf (tempBuffer, 64, "%llu", ulli1);

    return (string1 + tempBuffer);
}

string operator + (const unsigned long long int &ulli1, const string &string1)
{
    char tempBuffer[64] = {0};

    snprintf (tempBuffer, 64, "%llu", ulli1);

    return (tempBuffer + string1);
}

string operator + (const string &string1, int &si1)
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%d", si1);

    return (string1 + tempBuffer);
}

string operator + (int &si1, const string &string1)
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%d", si1);

    return (tempBuffer + string1);
}

string operator + (const string &string1, const void *&pPointer)
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%p", pPointer);

    return (string1 + tempBuffer);
}

string operator + (const void *&pPointer, const string &string1)
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%p", pPointer);

    return (tempBuffer + string1);
}

void tokenize (const string &inputString, vector<string> &tokens, const char separator)
{
    string::size_type inputLength   = inputString.size ();
    string::size_type startPosition = 0;
    string::size_type endPosition   = 0;
    string            token;

    tokens.clear ();

    while (startPosition < inputLength)
    {
        endPosition = inputString.find (separator, startPosition);

        if (string::npos != endPosition)
        {
            if (endPosition > startPosition)
            {
                token = inputString.substr (startPosition, endPosition - startPosition);
                tokens.push_back (token);
            }

            startPosition = endPosition + 1;
        }
        else
        {
            token = inputString.substr (startPosition, string::npos);
            tokens.push_back (token);
            break;
        }
    }
}

void StringUtils::replaceAllInstancesOfInputStringWith (string& source, const string& inputString, const string& replacementString)
{
    string::size_type position                = 0;
    string::size_type inputLength             = inputString.length       ();
    string::size_type replacementStringLength = replacementString.length ();

    if (0 == inputLength)
    {
        return;
    }

    if (inputString == replacementString)
    {
        return;
    }

    while (string::npos != (position = source.find (inputString, position)))
    {
        source.replace (position, inputLength, replacementString);
        position += replacementStringLength;
    }
}

void StringUtils::replaceAllInstancesOfInputCharWith (string& source, const char& inputChar, const char& replacementChar)
{
    string inputString;
    string replacementString;

    inputString       = inputChar;
    replacementString = replacementChar;

    if (inputChar == replacementChar)
    {
        return;
    }

    if ('\0' == inputChar)
    {
        return;
    }

    if ('\0' == replacementChar)
    {
        return;
    }

    replaceAllInstancesOfInputStringWith (source, inputString, replacementString);
}

void StringUtils::removeAllInstancesOfInputString (string& source, const string& inputString)
{
    replaceAllInstancesOfInputStringWith (source, inputString, "");
}

void StringUtils::removeAllInstancesOfInputChar (string& source, const char& inputChar)
{
    string::size_type position    = 0;

    if ('\0' == inputChar)
    {
        return;
    }

    while (string::npos != (position = (source.find (inputChar, position))))
    {
        source.erase (position, 1);
    }
}

void StringUtils::tokenize (const string& inputString, vector<string>& tokens, const char separator)
{
    WaveNs::tokenize (inputString, tokens, separator);
}

void StringUtils::stripFromLastOccuranceOf (string &inputString, const char &inputCharacter)
{
    string::size_type position = inputString.rfind (inputCharacter);

    if (string::npos != position)
    {
        inputString = inputString.substr (0, position);
    }
}

void StringUtils::stripFromFirstOccuranceOf (string &inputString, const char &inputCharacter)
{
    string::size_type position = inputString.find (inputCharacter);

    if (string::npos != position)
    {
        inputString = inputString.substr (0, position);
    }
}

char StringUtils::getLowerCase (char in)
{
    if (in<='Z' && in>='A')
    {
        return in-('Z'-'z');
    }
    return in;
}

bool StringUtils::endsWith (const string &inputString, const string &pattern)
{
    bool foundPattern = false;

    string::size_type position = inputString.rfind (pattern);

    if (string::npos != position)
    {
        if ((inputString.length ()) == position + (pattern.length ()))
        {
            foundPattern = true;
        }
    }

    return (foundPattern);
}

bool StringUtils::contains (const string &inputString, const string &pattern)
{
    bool foundPattern = false;

    string::size_type position = inputString.find (pattern);

    if (string::npos != position)
    {
        foundPattern = true;
    }

    return (foundPattern);
}

void StringUtils::splitBasedOnLastOccuranceOf (const string &inputString, const char &inputCharacter, string &firstPart, string &secondPart)
{
    string::size_type position = inputString.rfind (inputCharacter);

    if (string::npos != position)
    {
        firstPart  = inputString.substr (0, position);
        secondPart = inputString.substr (position + 1);
    }
    else
    {
        firstPart  = inputString;
        secondPart = "";
    }
}

void StringUtils::splitBasedOnFirstOccuranceOf (const string &inputString, const char &inputCharacter, string &firstPart, string &secondPart)
{
    string::size_type position = inputString.find (inputCharacter);

    if (string::npos != position)
    {
        firstPart  = inputString.substr (0, position);
        secondPart = inputString.substr (position + 1);
    }
    else
    {
        firstPart  = inputString;
        secondPart = "";
    }
}

void StringUtils::stripAllOccurancesAtTheBegining (string &inputString, const char &inputCharacter)
{
    string::size_type position = inputString.find_first_not_of (inputCharacter);

    if (string::npos == position)
    {
        inputString.clear ();
    }
    else
    {
        inputString.erase (0, position);
    }
}

void StringUtils::stripAllOccurancesAtTheEnd (string &inputString, const char &inputCharacter)
{
    string::size_type position = inputString.find_last_not_of (inputCharacter);

    if (string::npos == position)
    {
        inputString.clear ();
    }
    else
    {
        inputString.erase (position + 1, string::npos);
    }
}

void StringUtils::tokenizeStringWithCount (const string &inputString, vector<string> &tokens)
{
    tokens.clear ();
    string::size_type  startFromIndex = 0;
    string::size_type  firstIndex     = 0;
    string::size_type  stringSize     = 0;

    while (string::npos != (firstIndex = inputString.find ("#", startFromIndex)))
    {
        stringSize = atoi (inputString.c_str () + startFromIndex);
        tokens.push_back (inputString.substr (firstIndex + 1, stringSize));
        startFromIndex = firstIndex + stringSize + 1;
    }
}

void StringUtils::getCommonStringAtBeginning (const vector<string> &inputStrings, string &commonStringAtBeginning)
{
    UI32 numberOfInputStrings = inputStrings.size ();
    UI32 leastStringLength    = 0;
    UI32 i                    = 0;
    UI32 stringLength         = 0;
    char inputCharacter;
    UI32 j                    = 0;
    bool isInputCharacterSame = true;

    if (0 == numberOfInputStrings)
    {
        commonStringAtBeginning = "";
    }
    else
    {
        leastStringLength = inputStrings[0].size ();

        for (i = 0; i < numberOfInputStrings; i++)
        {
            stringLength = inputStrings[i].size ();

            if (stringLength < leastStringLength)
            {
                leastStringLength = stringLength;
            }
        }

        if (0 == leastStringLength)
        {
            commonStringAtBeginning = "";
        }
        else
        {
            for (i = 0; i < leastStringLength; i++)
            {
                inputCharacter       = inputStrings[0][i];
                isInputCharacterSame = true;

                for (j = 0; j < numberOfInputStrings; j++)
                {
                    if (inputCharacter != inputStrings[j][i])
                    {
                        isInputCharacterSame = false;
                        break;
                    }
                }

                if (false == isInputCharacterSame)
                {
                    break;
                }
            }

            commonStringAtBeginning = inputStrings[0].substr (0, i);
        }
    }
}

string StringUtils::joinStrings (const string &string1, const string &string2, const char separator)
{
    string outputString = string1;

    if (false == string2.empty ())
    {
        if (false == outputString.empty ())
        {
            outputString += separator;
        }

        outputString += string2;
    }

    return outputString;
}

void StringUtils::getLowerCaseString (const string &inputString, string &outputString)
{
    outputString = inputString;

    std::transform (outputString.begin (), outputString.end (), outputString.begin (), StringUtils::getLowerCase);
}

void StringUtils::applyConfigStringFormatting (string &stringToBeProcessed, const bool &allowMultiWordValueWithoutQuotes)
{
    if (true == stringToBeProcessed.empty ())
    {
        stringToBeProcessed = "\"\"";
    }
    else
    {
        bool needsToBeQuoted = false;

        if (string::npos != stringToBeProcessed.find ("\n"))
        {
            needsToBeQuoted = true;
        }

        StringUtils::replaceAllInstancesOfInputStringWith (stringToBeProcessed, "\n", "\\n");

        if (true == needsToBeQuoted)
        {
            stringToBeProcessed = string ("\"") + stringToBeProcessed + string ("\"");
        }
        else if (false == allowMultiWordValueWithoutQuotes)
        {
            if (string::npos != stringToBeProcessed.find (' '))
            {
                stringToBeProcessed = string ("\"") + stringToBeProcessed + string ("\"");
            }
        }
    }
}

bool StringUtils::isStringAlphaNumeric (const string &inputString)
{

    for (UI32 i=0; i < inputString.length(); ++i)
    {
         if ( !(isalpha (inputString.at(i)) || isdigit (inputString.at(i)) ) )
         {
            return false;
         }
    }

    return true;
}

string StringUtils::removeAllCharactersBetweenNewLineCharacterAndReturnCharacter (const string &inputString)
{
    string outputString;

    string::const_iterator itrElement = inputString.begin ();
    string::const_iterator endElement = inputString.end ();

    string lastToken;

    for (; endElement != itrElement; itrElement++)
    {
        char characterAtIterator = *itrElement;

        if ('\n' == characterAtIterator)
        {
            lastToken.append (1, characterAtIterator);

            outputString.append (lastToken);

            lastToken.clear ();
        }
        else if ('\r' == characterAtIterator)
        {
            lastToken.clear ();
        }
        else
        {
            lastToken.append (1, characterAtIterator);
        }
    }

    outputString.append (lastToken);

    return outputString;
}

UI32 StringUtils::getCharCountAtTheBegining (const string &inputString, const char &inputChar)
{
    UI32 numberOfOccurancesAtTheBegining = 0;
    UI32 numberOfCharsInInputString      = inputString.size ();
    UI32 i                               = 0;

    for (i = 0; i < numberOfCharsInInputString; i++)
    {
        if (inputChar == inputString[i])
        {
            numberOfOccurancesAtTheBegining++;
        }
        else
        {
            break;
        }
    }

    return (numberOfOccurancesAtTheBegining);
}

void StringUtils::convertToUpperCase (string &inputString)
{
    string::iterator element    = inputString.begin ();
    string::iterator endElement = inputString.end   ();

    while (endElement != element)
    {
        (*element) = toupper (*element);

        element++;
    }
}

}
