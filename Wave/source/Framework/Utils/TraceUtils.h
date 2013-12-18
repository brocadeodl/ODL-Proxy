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
 ***************************************************************************/

#ifndef TRACEUTILS_H
#define TRACEUTILS_H

#include <string>
#include "Framework/Trace/TraceMessages.h"

using namespace std;

namespace WaveNs
{
    void trace       (TraceLevel traceLevel, const string &stringToTrace);
    void tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
    void tracePrintf (TraceLevel traceLevel, const char * const pFormat, ...);

static inline void traceStartProfile (string file, string func, UI32 line, UI32 seq)
{
        struct timeval timev; gettimeofday(&timev, NULL);
        char sec[64] = {0};
        sprintf (sec, "%d", (int)timev.tv_sec);
        char usec[64] = {0};
        sprintf (usec, "%d", (int)timev.tv_usec);
        char lineStr[64] = {0};
        sprintf (lineStr, "%d", line);
        char seqStr[64] = {0};
        sprintf (seqStr, "%d", seq);
        
        trace (TRACE_LEVEL_INFO, "BEGIN SEC " + string (sec) + (" USEC ") + string (usec) + " " + func + " SEQ " + seqStr + " " + file + " " + lineStr);
}


static inline void traceEndProfile (string file, string func, UI32 line, UI32 seq)
{
        struct timeval timev; gettimeofday(&timev, NULL);
        char sec[64] = {0};
        sprintf (sec, "%d", (int)timev.tv_sec);
        char usec[64] = {0};
        sprintf (usec, "%d", (int)timev.tv_usec);
        char lineStr[64] = {0};
        sprintf (lineStr, "%d", line);
        char seqStr[64] = {0};
        sprintf (seqStr, "%d", seq);
        
        trace (TRACE_LEVEL_INFO, "END SEC " + string (sec) + (" USEC ") + string (usec) + " " + func + " SEQ " + seqStr + " " + file + " " + lineStr);
}

}
#endif //TRACEUTILS_H
