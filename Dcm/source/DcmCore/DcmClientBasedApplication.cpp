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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmResourceIds.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

using namespace WaveNs;
using namespace DcmNs;
using namespace std;

extern "C" int waveClientMain (int argc, char *argv[], bool wait);

const UI32 g_commandLineArgumentLength    = 512;
const UI32 g_numberOfCommandLineArguments = 8;

extern "C" int dcmClientMain (const char * const pProgramName, const int clientPort)
{
    char *pCommandLineArguments[g_numberOfCommandLineArguments];
    UI32  i, returnCode;

    if (NULL == pProgramName)
    {
        trace (TRACE_LEVEL_FATAL, "Program Name Must Be Specified.");
        prismAssert (false, __FILE__, __LINE__);
    }

    for (i = 0; i < g_numberOfCommandLineArguments; i++)
    {
        pCommandLineArguments[i] = new char[g_commandLineArgumentLength];
        memset (pCommandLineArguments[i], 0, g_commandLineArgumentLength);
    }

    strncpy (pCommandLineArguments[0], pProgramName, strlen(pProgramName));
    strncpy (pCommandLineArguments[1], "false", strlen("false"));
    strncpy (pCommandLineArguments[2], "0", strlen("0"));
    strncpy (pCommandLineArguments[3], "false", strlen("false"));
    strncpy (pCommandLineArguments[4], "3016", strlen("3016"));
    strncpy (pCommandLineArguments[5], "false", strlen("false"));
    strncpy (pCommandLineArguments[6], "9013", strlen("9013"));
    strncpy (pCommandLineArguments[7], "9710", strlen("9710"));

    if (0 < clientPort)
    {
        snprintf (pCommandLineArguments[7], g_commandLineArgumentLength, "%d", clientPort);
    }

    initializeDcmResourceIds ();

    returnCode = (waveClientMain (g_numberOfCommandLineArguments, pCommandLineArguments, false));
	
    for (i = 0; i < g_numberOfCommandLineArguments; i++)
    {
        delete [] pCommandLineArguments[i];
    }

	return returnCode;
}

