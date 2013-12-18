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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "Framework/Types/Types.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

string operator + (const string &string1, const unsigned long long int &ulli1);
string operator + (const unsigned long long int &ulli1, const string &string1);

string operator + (const string &string1, int &si1);
string operator + (int &si1, const string &string1);

string operator + (const string &string1, const void *&pPointer);
string operator + (const void *&pPointer, const string &string1);

void   tokenize   (const string &inputString, vector<string> &tokens, const char separator = ' ');

class StringUtils
{
    private :
    protected :
    public :
        static void replaceAllInstancesOfInputStringWith (string &source, const string &inputString, const string &replacementString);
        static void replaceAllInstancesOfInputCharWith   (string& source, const char& inputChar, const char& replacementChar);
        static void removeAllInstancesOfInputString      (string &source, const string &inputString);
        static void removeAllInstancesOfInputChar        (string &source, const char &inputChar);

        static void tokenize                             (const string &inputString, vector<string> &tokens, const char separator = ' ');
        static char getLowerCase                         (char in);

        static void stripFromLastOccuranceOf             (string &inputString, const char &inputCharacter = '/');
        static void stripFromFirstOccuranceOf            (string &inputString, const char &inputCharacter = '/');

        static bool endsWith                             (const string &inputString, const string &pattern);

        static void splitBasedOnLastOccuranceOf          (const string &inputString, const char &inputCharacter, string &firstPart, string &secondPart);
        static void splitBasedOnFirstOccuranceOf         (const string &inputString, const char &inputCharacter, string &firstPart, string &secondPart);

        static void stripAllOccurancesAtTheBegining      (string &inputString, const char &inputCharacter);
        static void stripAllOccurancesAtTheEnd           (string &inputString, const char &inputCharacter);

        static void getCommonStringAtBeginning           (const vector<string> &inputStrings, string &commonStringAtBeginning);
        static void tokenizeStringWithCount              (const string &inputString, vector<string> &tokens);

        static string joinStrings                        (const string &string1, const string &string2, const char separator = ' ');
        static void   getLowerCaseString                 (const string &inputString, string &outputString);
        static void   applyConfigStringFormatting        (string &stringToBeProcessed, const bool &allowMultiWordValueWithoutQuotes);
        static bool   contains                           (const string &inputString, const string &pattern);
        static bool   isStringAlphaNumeric               (const string &inputString);

        static string removeAllCharactersBetweenNewLineCharacterAndReturnCharacter (const string &inputString);

        static UI32   getCharCountAtTheBegining          (const string &inputString, const char &inputChar);

        static void   convertToUpperCase                 (string &inputString);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // STRINGUTILS_H
