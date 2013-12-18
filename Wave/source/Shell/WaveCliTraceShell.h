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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Subramanian                                            *
 ***************************************************************************/

#ifndef WAVECLITRACESHELL_H
#define WAVECLITRACESHELL_H 

#include "Shell/WaveCliShell.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class WaveCliTraceShell : public WaveCliShell
{
    private :
                   WaveCliTraceShell            (WaveClientSynchronousConnection &connection);

        void       briefHelp                    ();

        ResourceId traceList                    (const vector<string> &arguments);
        void       traceListHelp                (void);

        ResourceId traceSetLevel                (const vector<string> &arguments);
        void       traceSetLevelHelp            (void);

        ResourceId traceSetAll                  (const vector<string> &arguments);
        void       traceSetAllHelp              (void);

        ResourceId traceSetDefault              (const vector<string> &arguments);
        void       traceSetDefaultHelp          (void);

        ResourceId traceLegend                  (const vector<string> &arguments);
        void       traceLegendHelp              (void);
        
        ResourceId traceHelp                    (const vector<string> &arguments);
        void       traceHelpHelp                (void);

        static TraceLevel getTraceLevelResourceId (const char* trace);
    protected :
    public :
        virtual ~WaveCliTraceShell ();

    // Now the data members

    private :
    protected :
    public :

    friend class WaveCliShell;
};

}

#endif // WAVECLITRACESHELL_H
