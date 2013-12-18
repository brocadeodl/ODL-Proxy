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

#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "AppDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Cluster/Test/ClusterTestObjectManager.h"


namespace WaveNs
{

    // This variable is used if async reply is used for dup commands.
void *pDuplicateCommandContext;

void duplicateCmdsReply0 (int status, void * context, int nNodes, remoteCmdResult_t nodeResults [])
{
    cout << "Got Dup CMD Reply for service 0" << endl;
    (ClusterTestObjectManager::getInstance ())->duplicateCmdReply (status, context, nNodes, nodeResults);

    //duplicateCmdsReply (status, context, nNodes, nodeResults);
}

void duplicateCmdsReply1 (int status, void * context, int nNodes, remoteCmdResult_t nodeResults [])
{
    cout << "Got Dup CMD Reply for service 0" << endl;
    (ClusterTestObjectManager::getInstance ())->duplicateCmdReply (status, context, nNodes, nodeResults);

//    duplicateCmdsReply (status, context, nNodes, nodeResults);
}


AppDebug::AppDebug  ()
{
}

AppDebug::~AppDebug  ()
{
}

void AppDebug::initialize ()
{
    addDebugFunction ((ShellCmdFunction) (&AppDebug::shellExecuteDuplicateCmdDebug),    "dc");
    addDebugFunction ((ShellCmdFunction) (&AppDebug::shellExecuteSendDupCmdReplyDebug), "sr");

}

UI32 AppDebug::shellExecuteSendDupCmdReplyDebug (UI32 argc, vector<string> argv)
{
    commandData_t   resultList [11];
    UI32            nResults;

    nResults = 2;
    cout << "Duplicate Command Test Handler" << endl;

    resultList[0].cmdLen = 100;
    resultList[0].cmdPtr = (char *) new UI8[100];
    memcpy (resultList[0].cmdPtr, "1st RESULT BUFFER", strlen ("1st RESULT BUFFER") + 1);

    resultList[1].cmdLen = 200;
    resultList[1].cmdPtr = (char *) new UI8[200];
    memcpy (resultList[1].cmdPtr, "2nd RESULT BUFFER", strlen ("2nd RESULT BUFFER") + 1);


    sendRemoteCommandsReply (0, pDuplicateCommandContext, nResults, resultList);

    return SHELL_OK;
}

UI32 AppDebug::shellExecuteRegisterCallBackDebug (UI32 argc, vector<string> argv)
{
    registerDuplicateCmdsHandler (0, &ClusterTestObjectManager::duplicateCmdTestHandler);
    registerDuplicateCmdsHandler (1, &ClusterTestObjectManager::duplicateCmdTestHandler);
    return SHELL_OK;
}


UI32 AppDebug::shellExecuteDuplicateCmdDebug (UI32 argc, vector<string> argv)
{
    UI32 oneWay     = 0;
    UI32 multicast  = 0;
    UI32 service    = 0;

    if (argc > 1)
    {
        service = atoi (argv[1].c_str ());
    }

    if (argc > 2)
    {
        oneWay = atoi (argv[2].c_str ());
    }

    if (argc > 3)
    {
        multicast = atoi (argv[3].c_str ());
    }

//    (ClusterTestObjectManager::getInstance ())->sendDuplicateCmd (service, oneWay, multicast, &ClusterTestObjectManager::duplicateCmdReply, NULL);
    if( 0 ==service)
    {
        (ClusterTestObjectManager::getInstance ())->sendDuplicateCmd (service, oneWay, multicast, &duplicateCmdsReply0, NULL);
    }
    else
    {
        (ClusterTestObjectManager::getInstance ())->sendDuplicateCmd (service, oneWay, multicast, &duplicateCmdsReply1, NULL);
    }

    return SHELL_OK;
}

}
