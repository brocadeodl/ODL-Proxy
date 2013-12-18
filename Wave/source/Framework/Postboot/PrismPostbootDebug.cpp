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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "Framework/Postboot/PrismPostbootDebug.h"
#include "Framework/Postboot/PrismPostbootAgent.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

PrismPostbootDebug::PrismPostbootDebug  ()
{
}

PrismPostbootDebug::~PrismPostbootDebug  ()
{
}

void PrismPostbootDebug::initialize ()
{
    addDebugFunction ((ShellCmdFunction) (&PrismPostbootDebug::shellExecuteTriggerPostbootPasses), "startPostbootPasses");
}

UI32 PrismPostbootDebug::shellExecuteTriggerPostbootPasses (UI32 argc, vector<string> argv)
{
    ExecutePostbootPassTableMessage *pMessage;

    pMessage = new ExecutePostbootPassTableMessage();
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}

}
