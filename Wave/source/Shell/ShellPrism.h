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

#ifndef SHELLWAVE_H
#define SHELLWAVE_H

#include "Shell/ShellBase.h"
#include "Shell/ShellRegression.h"
#include "Shell/ShellDebug.h"
#include "Shell/ShellTrace.h"
#include "Shell/ShellCluster.h"
#include "Shell/ShellWyser.h"
#include <string>

using namespace std;

namespace WaveNs
{

class ShellPrism : public ShellBase
{
    private :
    protected :
    public :
                                    ShellPrism                          (WaveObjectManager *pWaveObjectManager);
                                   ~ShellPrism                          ();
        static  UI32                shellExecuteRegressionService       (UI32 argc, vector<string> argv);
        static  UI32                shellExecuteTraceService            (UI32 argc, vector<string> argv);
        static  UI32                shellExecuteDebugService            (UI32 argc, vector<string> argv);
    	static  UI32                shellExecuteClusterService          (UI32 argc, vector<string> argv);
    	static  UI32                shellExecuteWyserService            (UI32 argc, vector<string> argv);
        static  UI32                shellExecuteLoadScript              (UI32 argc, vector<string> argv);
        static  UI32                shellExecuteQuit                    (UI32 argc, vector<string> argv);
        static  UI32                shellExecuteHelp                    (UI32 argc, vector<string> argv);

        static  void                usageShellPrismRegressionService    ();
        static  void                usageShellPrismTraceService         ();
        static  void                usageShellPrismDebugService         ();
    	static  void                usageShellPrismClusterService       ();
    	static  void                usageShellPrismWyserService         ();
        static  void                usageShellPrismLoadScript           ();
        static  void                usageShellPrismHelp                 ();
        static  WaveMessageStatus   shellSendSynchronously              (PrismMessage *pPrismMessage);
        static  void                shellTrace                          (TraceLevel traceLevel, const string &stringToTrace);
                ShellRegression    *getRegressionShell                  ();
                ShellDebug         *getDebugShell                       ();
                ShellTrace         *getTraceShell                       ();
		        ShellCluster       *getClusterShell                     ();
		        ShellWyser         *getWyserShell                       ();

    private :
        ShellRegression                *m_pRegressionShell;
        ShellDebug                     *m_pDebugShell;
        ShellTrace                     *m_pTraceShell;
	    ShellCluster                   *m_pClusterShell;
        ShellWyser                     *m_pWyserShell;

    protected :
    public :

};

}

#endif //SHELLWAVE_H
