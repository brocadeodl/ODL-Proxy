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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <stdlib.h>
#include "SystemManagement/Configuration/CliConfigurationFile.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Types/UI32Range.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <sstream>

using namespace std;

namespace WaveNs
{

CliConfigurationFile::CliConfigurationFile ()
    : ConfigurationFile ()
{
}

CliConfigurationFile::CliConfigurationFile (const string &cliConfigurationFilePath)
    : ConfigurationFile (cliConfigurationFilePath)
{
    load ();
}

CliConfigurationFile::~CliConfigurationFile ()
{
}

void CliConfigurationFile::splitConfigForNonBulking (vector<pair<string, bool> > &configFiles)
{
    trace (TRACE_LEVEL_DEBUG, "CliConfigurationFile::splitConfigForNonBulking .. Entering");

    string finalFilePath;
    int fd;

    for (UI32 i = 0; i < m_configFileBulkRanges.size (); i++)
    {
        char *tmpPath = new char[80];
        strcpy (tmpPath, "/tmp/configXXXXXX");
        fd = mkstemp (tmpPath);
        if (fd == -1)
        {
            trace (TRACE_LEVEL_ERROR, "Not able to create file for configuration. Returning from this function");
            return;
        }

        finalFilePath = tmpPath;       
        pair<string, bool> configFile (finalFilePath, m_configFileBulkRanges[i].second);

        vector<UI32> rangeIntegers;
        m_configFileBulkRanges[i].first.getUI32RangeVector (rangeIntegers);
        // Populate the file with the contents
        ofstream outputFile (finalFilePath.c_str ());

        for (UI32 j = rangeIntegers[0]; j <= rangeIntegers[rangeIntegers.size () - 1]; j++)
        {
            outputFile << m_cliLines[j];
            outputFile << endl;
        }

        outputFile.close ();

        configFiles.push_back (configFile);
        delete [] tmpPath;
    }
}

void CliConfigurationFile::load ()
{
    if (true == (getIsAlreadyLoaded ()))
    {
        trace (TRACE_LEVEL_WARN, "CliConfigurationFile::load : " + getConfigurationFilePath () + " is already loaded.");
        return;
    }

    m_cliNodeTree.destroyAndClearChildren ();

    string         cliConfigurationFilePath = getConfigurationFilePath ();
    string         cliLine;

    trace (TRACE_LEVEL_DEBUG, string ("CliConfigurationFile::load : loading from ") + cliConfigurationFilePath);

    ifstream inputFile (cliConfigurationFilePath.c_str (), ios_base::in);

    while (getline (inputFile, cliLine, '\n'))
    {
        StringUtils::stripAllOccurancesAtTheEnd (cliLine, '\r');

        m_cliLines.push_back (cliLine);
    }

    UI32          numberOfCliLines  = m_cliLines.size ();
    UI32          i                 = 0;
    string        lastCliLineRead;
    UI32          previousLineDepth = 0;
    UI32          currentLineDepth  = 0;
    UI32          cliLineLength     = 0;
    string        effectiveCliLine;
    UI32          currentRangeStart = 0;
    UI32          currentRangeEnd   = 0;
    bool          isCurrentRangeBulk = false;

    stack<string> parentModeHierarchyStack;

    trace (TRACE_LEVEL_INFO, string ("CliConfigurationFile::load : ") + numberOfCliLines + string (" lines loaded from ") + cliConfigurationFilePath);

    for (i = 0; i < numberOfCliLines; i++)
    {
        cliLine = m_cliLines[i];

        currentLineDepth = StringUtils::getCharCountAtTheBegining (cliLine, ' ');

        StringUtils::stripAllOccurancesAtTheBegining (cliLine, ' ');

        cliLineLength = cliLine.size ();

        if (0 == cliLineLength)
        {
            continue;
        }

        if ("!" == cliLine)
        {
            if (false == (parentModeHierarchyStack.empty ()))
            {
                parentModeHierarchyStack.pop ();
            }

            continue;
        }

        if (previousLineDepth < currentLineDepth)
        {
            parentModeHierarchyStack.push (lastCliLineRead);
        }

        if (false == (parentModeHierarchyStack.empty ()))
        {
            effectiveCliLine = (parentModeHierarchyStack.top ()) + (string (" ")) + cliLine;
        }
        else
        {
            effectiveCliLine = cliLine;
        }

        //trace (TRACE_LEVEL_INFO, effectiveCliLine);
        m_cliNodeTree.addCommandLine (effectiveCliLine);

        // Split effectiveCliLine into tokens and for the first token see if its zoning
        vector<string> commandLineTokens;
        StringUtils::tokenize (effectiveCliLine, commandLineTokens, ' ');

        if (commandLineTokens[0] == "zoning" && isCurrentRangeBulk == false)
        {
            // End previous range
            if (i)
            {
                currentRangeEnd = i-1;
                UI32Range range (currentRangeStart, currentRangeEnd);
                pair<UI32Range, bool> rangePair (range, isCurrentRangeBulk);
                m_configFileBulkRanges.push_back (rangePair);
            }
            // Start new range
            isCurrentRangeBulk = true;
            currentRangeEnd = 0;
            currentRangeStart = i;
        }
        else if (commandLineTokens[0] != "zoning" && isCurrentRangeBulk == true)
        {
            // End previous range 
            currentRangeEnd = i-1;
            UI32Range range (currentRangeStart, currentRangeEnd);
            pair<UI32Range, bool> rangePair (range, isCurrentRangeBulk);
            m_configFileBulkRanges.push_back (rangePair);

            // Start new range
            isCurrentRangeBulk = false;
            currentRangeEnd = 0;
            currentRangeStart = i;
        }

        previousLineDepth = currentLineDepth;
        lastCliLineRead   = cliLine;
    }

    // At the end of the file complete the last range
    currentRangeEnd = i-1;
    UI32Range range (currentRangeStart, currentRangeEnd);
    pair<UI32Range, bool> rangePair (range, isCurrentRangeBulk);
    m_configFileBulkRanges.push_back (rangePair);

    trace (TRACE_LEVEL_INFO, string ("CliConfigurationFile::load : Loading from ") + cliConfigurationFilePath + string ("completed."));
}

void CliConfigurationFile::reload ()
{
}

void CliConfigurationFile::reload (const string &cliConfigurationFilePath)
{
}

bool CliConfigurationFile::validate () const
{
    return (true);
}

void CliConfigurationFile::print () const
{
}

UI32 CliConfigurationFile::debugLoad (UI32 argc, vector<string> argv)
{
    CliConfigurationFile cf1 (argv[1]);

    return (0);
}

void CliConfigurationFile::getChildrenCliNodeNamesForCommandLine (const string &commandLine, vector<string> &childrenCliNodeNames)
{
    m_cliNodeTree.getChildrenCliNodeNamesForCommandLine (commandLine, childrenCliNodeNames);
}

}
