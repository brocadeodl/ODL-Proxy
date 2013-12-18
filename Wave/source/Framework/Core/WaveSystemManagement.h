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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVESYSTEMMANAGEMENT_H
#define WAVESYSTEMMANAGEMENT_H

#include "Framework/Types/Types.h"

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManager.h"
#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Timer/Test/TimerTestObjectManager.h"
#include "Framework/Core/WaveMainConfiguration.h"
#include "Framework/Core/Wave.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveSystemManagement
{
    private :
        static void instantiateNativePrismServices ();
        static void registerNativeServiceInternal      (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true, const ResourceId &serviceMode = WAVE_SERVICE_ACTIVE);

    protected :
    public :
        static void   initialize                                (const WaveMainConfiguration &waveMainConfiguration);
        static string getTraceFileDirectory                     ();
        static string getConfigurationFileDirectory             ();
        static string getProfileFileDirectory                   ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // WAVESYSTEMMANAGEMENT_H
