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

#ifndef OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHWORKER_H
#define OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace OpenFlowNs
{

class OpenFlowSwitchObjectManager;
class OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage;
class AddOpenFlowControllerToOpenFlowLogicalSwitchContext;

class OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker : public WaveWorker
{
    private :
        virtual PrismMessage      *createMessageInstance                                      (const UI32 &operationCode);
        virtual WaveManagedObject *createManagedObjectInstance                                (const string &managedClassName);

                void               addOpenFlowControllerToOpenFlowLogicalSwitchMessageHandler (OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage *pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage);
                void               addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep   (AddOpenFlowControllerToOpenFlowLogicalSwitchContext *pAddOpenFlowControllerToOpenFlowLogicalSwitchContext);
                void               addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep     (AddOpenFlowControllerToOpenFlowLogicalSwitchContext *pAddOpenFlowControllerToOpenFlowLogicalSwitchContext);

    protected :
    public :
                 OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager);
        virtual ~OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker ();

    // Now the Data Members

    private :
    protected :
    public :
};

}

#endif // OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHWORKER_H
