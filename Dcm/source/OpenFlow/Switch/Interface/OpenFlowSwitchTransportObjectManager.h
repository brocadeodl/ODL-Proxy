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

#ifndef OPENFLOWSWITCHTRANSPORTOBJECTMANAGER_H
#define OPENFLOWSWITCHTRANSPORTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

#include <string>

using namespace WaveNs;
using namespace std;

namespace OpenFlowNs
{

class WaveAsynchronousContextForBootPhases;

class OpenFlowSwitchTransportObjectManager : public WaveLocalObjectManager
{
    private :
              OpenFlowSwitchTransportObjectManager ();

         void boot                                 (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual                                       ~OpenFlowSwitchTransportObjectManager ();

        static   OpenFlowSwitchTransportObjectManager *getInstance                          ();
        static   PrismServiceId                        getPrismServiceId                    ();
        static   string                                getServiceName                       ();

    // Now the Data Members

    private :
    protected :
    public :
};

inline string OpenFlowSwitchTransportObjectManager::getServiceName ()
{
    return ("Open Flow Switch Transport");
}

}

#endif // OPENFLOWSWITCHTRANSPORTOBJECTMANAGER_H
