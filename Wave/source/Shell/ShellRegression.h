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

#ifndef SHELLREGRESSION_H
#define SHELLREGRESSION_H

#include "Shell/ShellBase.h"
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class ShellRegression : public ShellBase
{
    private :
    protected :
    public :
                ShellRegression                             (WaveObjectManager *pWaveObjectManager);
               ~ShellRegression                             ();
        static  UI32    shellExecuteListServices            (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetServiceState         (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetAllServicesState     (UI32 argc, vector<string> argv);
        static  UI32    shellExecutePrepareRegression       (UI32 argc, vector<string> argv);
        static  UI32    shellExecutePrepareRegression2      (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteStartRegression         (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteRunTestService          (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteQuitRegression          (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteHelpRegression          (UI32 argc, vector<string> argv);
        static  UI32    shellExecuteSetDefaultState         (UI32 argc, vector<string> argv);

                UI32    listServices                        (UI32 argc, vector<string> argv);
                UI32    setServiceState                     (UI32 argc, vector<string> argv);
                UI32    setAllServicesState                 (UI32 argc, vector<string> argv);
                UI32    prepareRegression                   (UI32 argc, vector<string> argv);
                UI32    prepareRegression2                  (UI32 argc, vector<string> argv);
                UI32    startRegression                     (UI32 argc, vector<string> argv);
                UI32    runTestService                      (UI32 argc, vector<string> argv);
                UI32    quitRegression                      (UI32 argc, vector<string> argv);
                UI32    helpRegression                      (UI32 argc, vector<string> argv);
                UI32    setDefaultState                     (UI32 argc, vector<string> argv);

                void    shellGetServicesList                (void);

        static  void    usageShellRegressionList            (void);
        static  void    usageShellRegressionSetState        (void);
        static  void    usageShellRegressionSetAllStates    (void);
        static  void    usageShellRegressionStart           (void);
        static  void    usageShellRegressionPrepare         (void);
        static  void    usageShellRegressionPrepare2        (void);
        static  void    usageShellRegressionRun             (void);
        static  void    usageShellRegressionHelp            (void);
        static  void    usageShellRegressionQuit            (void);
        static  void    usageShellRegressionSetDefaultState (void);

                    // Now the data members

    private :
                vector<PrismServiceId> m_serviceMap;
                UI32                   m_nServices;
    protected :
    public :
};

}

#endif //SHELLREGRESSION_H
