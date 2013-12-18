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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVECLIENT_H
#define WAVECLIENT_H

#include "Framework/Types/Types.h"

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/WaveClientConfiguration.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"                                 
#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

typedef        WaveObjectManager *  (* NativePrismServiceInstantiator) ();
typedef vector<WaveObjectManager *> (* NativeMultiplePrismServiceInstantiator) ();
typedef        SI32                 (* UpdateClientStatusFunctionPtr)( ManagementInterfaceMessage *pManagementInterfaceMessage );

class WaveClient
{
    private :
        static void   instantiateNativePrismServices ();

    protected :
    public :
        static void   initialize                        (const WaveClientConfiguration &waveClientConfiguration);
        static string getTraceFileDirectory             ();
        static string getConfigurationFileDirectory     ();
        static string getProfileFileDirectory           ();

        static void   registerNativeService             (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true);
        static void   registerNativeServiceInternal     (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true);
        static void   registerNativeService             (NativeMultiplePrismServiceInstantiator pNativeMultiplePrismServiceInstantiator);
        static void   registerUpdateClientStatusFunction(UpdateClientStatusFunctionPtr pUpdateClientStatusFunctionPtr);
        static SI32   updateClientStatusFunction        (ManagementInterfaceMessage *pManagementInterfaceMessage);

    // Now the data members

    private :
        static vector<NativePrismServiceInstantiator>         m_nativePrismServiceInstantiators;
        static vector<bool>                                   m_nativePrismServiceInstantiatorIsForNormalPhase;
        static vector<NativeMultiplePrismServiceInstantiator> m_nativeMultiplePrismServiceInstantiators;
        static UpdateClientStatusFunctionPtr                  m_updateClientStatusFunctionPtr;
        static PrismMutex                                     m_updateClientStatusFunctionPtrMutex;

    protected :
    public :
};

}

#endif // WAVECLIENT_H
