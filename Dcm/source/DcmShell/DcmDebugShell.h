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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DCMDEBUGSHELL_H
#define DCMDEBUGSHELL_H

#include "Shell/ShellBase.h"

using namespace WaveNs;

namespace DcmNs
{

class DcmDebugShell : public ShellBase
{
    private :
                    DcmDebugShell            (WaveObjectManager *pWaveObjectManager);

        static UI32 quit                     (UI32 argc, string argv[]);
        static UI32 testOperationalData      (UI32 argc, string argv[]);
        static void usageTestOperationalData ();

    protected :
    public :
        virtual      ~DcmDebugShell         ();

        static  UI32  executeThisShell      (UI32 argc, string argv[]);
        static  void  usageExecuteThisShell ();

    // Now the data members

    private :
        vector<ShellCommandHandler> m_commandHandlers;

    protected :
    public :
};

}

#endif // DCMDEBUGSHELL_H
