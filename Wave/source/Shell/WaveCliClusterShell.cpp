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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Shell/WaveCliClusterShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/Local/HeartBeat/HeartBeatTypes.h"

#include <stdlib.h>

namespace WaveNs
{

WaveCliClusterShell::WaveCliClusterShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("cluster", connection)
{
    addCommandfunction ("create", reinterpret_cast<WaveShellCommandFunction> (&WaveCliClusterShell::clusterCreate), "Creates a cluster.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliClusterShell::clusterCreateHelp));
    addCommandfunction ("gethbstats", reinterpret_cast<WaveShellCommandFunction> (&WaveCliClusterShell::getHeartBeatStats), "Gets the Heartbeat Stats.", reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliClusterShell::clusterCreateHelp));
}

WaveCliClusterShell::~WaveCliClusterShell ()
{
}

void WaveCliClusterShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Cluster shell provides basic control to Configure Cluster");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        in Wave based application.  For more information on");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        configuring cluster, please type help in cluster shell");
}

ResourceId WaveCliClusterShell::getHeartBeatStats (const vector<string> &arguments)
{
    ResourceId                      status             = WAVE_MESSAGE_ERROR;
    WaveClientSynchronousConnection connection         = getConnection ();
    IpV4Address                     dstIpAddress;
    UI32                            dstPortNumber      = 0;
    UI32                            numberOfArguments  = arguments.size ();
    UI32                            i                  = 0;
    HeartBeatStat                   heartBeatStat;
    
    if ((numberOfArguments < 2) || (0 != (numberOfArguments % 2)))
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "Usage: gethbstats dstIpAddress dstPortNumber"); 
        return (status);
    }

    for (i = 0; i < numberOfArguments; i++)
    {
        if (0 == (i % 2))
        {
            dstIpAddress.fromString(arguments[i]);
        }
        else
        {
            dstPortNumber = atoi (arguments[i].c_str());
        }
    }

    status = connection.getHeartBeatStats (dstIpAddress, dstPortNumber, heartBeatStat);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        cout << endl;
        cout << "    ____________________________________________________________________________" << endl << endl;
        cout << "        HBSent            :" << heartBeatStat.m_heartBeatSent << endl;
        cout << "        HBReplySent       :" << heartBeatStat.m_heartBeatReplySent << endl;
        cout << "        LastHBReceived    :" << heartBeatStat.m_lastHeartBeatReceived << endl;
        cout << "        HBReceived        :" << heartBeatStat.m_heartBeatReceived << endl;
        cout << "        LastHBReplyRcved  :" << heartBeatStat.m_lastHeartBeatReplyReceived << endl;
        cout << "        HBReplyRcved      :" << heartBeatStat.m_heartBeatReplyReceived << endl;
        cout << "        HBLost            :" << heartBeatStat.m_heartBeatLost << endl;
        cout << "        HBLostInRow       :" << heartBeatStat.m_heartBeatLostInRow << endl;
        cout << "        HBDelayed         :" << heartBeatStat.m_heartBeatDelayed << endl;
        cout << "        HBDuplicate       :" << heartBeatStat.m_heartBeatDuplicate<< endl;
        cout << "    ____________________________________________________________________________" << endl << endl ;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId WaveCliClusterShell::clusterCreate (const vector<string> &arguments)
{
    ResourceId                      status             = WAVE_MESSAGE_ERROR;
    WaveClientSynchronousConnection connection         = getConnection ();
    vector<string>                  serverIpAddresses;
    vector<SI32>                    serverPorts;
    UI32                            numberOfArguments        = arguments.size ();
    UI32                            i                        = 0;
    UI32                            j                        = 0;
    string                          currentServerIp;
    vector<string>                  serverPortTokens;
    string                          serverPortString;
    UI32                            numberOfServerPortTokens = 0;
    UI32                            serverPortBegin          = 0;
    UI32                            serverPortEnd            = 0;

    if ((numberOfArguments < 2) || (0 != (numberOfArguments % 2)))
    {
        clusterCreateHelp ();
        return (status);
    }

    for (i = 0; i < numberOfArguments; i++)
    {
        if (0 == (i % 2))
        {
            currentServerIp = arguments[i];
            serverIpAddresses.push_back (currentServerIp);
        }
        else
        {
            serverPortString = arguments[i];

            tokenize (serverPortString, serverPortTokens, '-');

            numberOfServerPortTokens = serverPortTokens.size ();

            if ((1 != numberOfServerPortTokens) && (2 != numberOfServerPortTokens))
            {
                clusterCreateHelp ();
                return (status);
            }

            if (1 <= numberOfServerPortTokens)
            {
                serverPortBegin = atoi ((serverPortTokens[0]).c_str ());
                serverPortEnd   = serverPortBegin;
            }
            if (2 <= numberOfServerPortTokens)
            {
                serverPortEnd   = atoi ((serverPortTokens[1]).c_str ());
            }

            serverPorts.push_back (serverPortBegin);

            for (j = serverPortBegin + 1; j <= serverPortEnd; j++)
            {
                serverIpAddresses.push_back (currentServerIp);
                serverPorts.push_back (j);
            }
        }
    }

    status = connection.createCluster (serverIpAddresses, serverPorts);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Obtain the list of Test Services. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void WaveCliClusterShell::clusterCreateHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.  All arguments are ignored.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following fields are listed:");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Index        - A monotonically increasing number to identify a BIST service in the CLI.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       This index number is different from the Wave Service Id.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        State        - Enabled / Disabled based on the BIST service State");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       This will make a differnce only when the entire cluster is run using start.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       All of the BIST services in Disabled state are ignored when start CLI is issued.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       The state of the BIST service will not effect run CLI.  run CLI runs the particular");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       BIST irrespective of the state.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Service Name - The Name of the Wave BIST Service.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

}
