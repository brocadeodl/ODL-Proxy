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
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef SHELLTRACE_H
#define SHELLTRACE_H

#include "Shell/ShellBase.h"
#include <string>

using namespace std;

namespace WaveNs
{

class ShellTrace : public ShellBase
{
    private :
    protected :
    public :
                        ShellTrace                              (WaveObjectManager *pWaveObjectManager);
                       ~ShellTrace                              ();
        static  UI32    shellExecuteListServices                (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetTraceLevel               (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetAllServicesTraceLevel    (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteQuitTrace                   (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteHelpTrace                   (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetDefaultLevels            (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteLegend                      (UI32 argc, vector<string> argv);

                UI32    listServices                            (UI32 argc, vector<string> argv);
                UI32    setServiceLevel                         (UI32 argc, vector<string> argv);
                UI32    setAllServicesLevel                     (UI32 argc, vector<string> argv);
                UI32    quitTrace                               (UI32 argc, vector<string> argv);
                UI32    helpTrace                               (UI32 argc, vector<string> argv);
                UI32    setDefaultLevels                        (UI32 argc, vector<string> argv);
                UI32    legend                                  (UI32 argc, vector<string> argv);

                void    shellGetServicesList                    (void);

        static  void    usageShellTraceList                     (void);
        static  void    usageShellTraceSetLevel                 (void);
        static  void    usageShellTraceSetAllServicesTraceLevel (void);
        static  void    usageShellTraceHelp                     (void);
        static  void    usageShellTraceQuit                     (void);
        static  void    usageShellTraceSetDefaultLevels         (void);
        static  void    usageShellLegend                        (void);


                    // Now the data members
    private :
        vector<TraceClientId> m_serviceMap;
        UI32                  m_nServices;

    protected :
    public :

};

}

#endif //SHELLTRACE_H
