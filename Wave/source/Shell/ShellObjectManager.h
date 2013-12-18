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

#ifndef SHELLOBJECTMANAGER_H
#define SHELLOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Shell/ShellPrism.h"
#include <iostream>
#include <fstream>

namespace WaveNs
{

class ShellObjectManager;
class BootCompleteForThisLocationEvent;
class WaveAsynchronousContextForBootPhases;

class ShellObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                ShellObjectManager                             ();

        virtual void initialize                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void boot                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void   listenForEvents                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void   bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);


    protected :
    public :
        virtual                    ~ShellObjectManager ();
        static  ShellObjectManager *getInstance        ();
        static  PrismServiceId      getPrismServiceId  ();

    // Now the data members

    private :
    protected :
    public :
        ShellPrism *m_pPrismShell;
        ifstream    m_inputSource;
};

}

#endif // SHELLOBJECTMANAGER_H
