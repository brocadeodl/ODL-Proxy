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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWLOGICALSWITCHINSTANCELOCALOBJECTMANAGER_H
#define OPENFLOWLOGICALSWITCHINSTANCELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

#include <string>

using namespace WaveNs;
using namespace std;

using WaveNs::WaveAsynchronousContextForBootPhases;

namespace OpenFlowNs
{

class OpenFlowLogicalSwitchInstanceLocalObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
        OpenFlowLogicalSwitchInstanceLocalObjectManager (const string &openFlowLogicalSwitchName);

        virtual void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual                                                 ~OpenFlowLogicalSwitchInstanceLocalObjectManager ();

                OpenFlowLogicalSwitchInstanceLocalObjectManager *createInstance                                  (const string &openFlowLogicalSwitchName);
        static  string                                           getServiceNamePrefix                            ();

    // Now the Data Members

    private :
        string m_openFlowLogicalSwitchName;

    protected :
    public :

    friend class OpenFlowSwitchObjectManager;
    friend class OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker;
};

inline string OpenFlowLogicalSwitchInstanceLocalObjectManager::getServiceNamePrefix ()
{
    return ("Open Flow Logical Switch Instance");
}

}

#endif // OPENFLOWLOGICALSWITCHINSTANCELOCALOBJECTMANAGER_H
