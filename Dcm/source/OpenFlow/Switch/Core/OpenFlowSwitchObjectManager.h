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

#ifndef OPENFLOWSWITCHOBJECTMANAGER_H
#define OPENFLOWSWITCHOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

#include <string>

using namespace WaveNs;
using namespace std;

namespace OpenFlowNs
{

class OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker;
class OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker;
class OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker;
class OpenFlowSwitchObjectManagerAddFlowWorker;
class OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker;

class OpenFlowSwitchObjectManager : public WaveObjectManager
{
    private :
        OpenFlowSwitchObjectManager ();

    protected :
    public :
        virtual                             ~OpenFlowSwitchObjectManager ();

        static  OpenFlowSwitchObjectManager *getInstance                 ();
        static  PrismServiceId               getPrismServiceId           ();
        static  string                       getServiceName              ();

    // Now the Data Members

    private :
        OpenFlowSwitchObjectManagerAddFlowWorker                                          *m_pOpenFlowSwitchObjectManagerAddFlowWorker;
        OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker                             *m_pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker;
        OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker                      *m_pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker;
        OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker     *m_pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker;
        OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker *m_pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker;

    protected :
    public :
};

inline string OpenFlowSwitchObjectManager::getServiceName ()
{
    return ("Open Flow Switch");
}

}

#endif // OPENFLOWSWITCHOBJECTMANAGER_H
