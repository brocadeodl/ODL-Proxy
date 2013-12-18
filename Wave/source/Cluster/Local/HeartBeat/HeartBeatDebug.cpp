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
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "Cluster/Local/HeartBeat/HeartBeatDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"

namespace WaveNs
{

HeartBeatDebug::HeartBeatDebug  ()
{
}

HeartBeatDebug::~HeartBeatDebug  ()
{
}

void HeartBeatDebug::initialize ()
{
    // Register debug function to test heartbeat service.
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteStartHeartBeat),    "starthb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteStopHeartBeat),     "stophb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecutePauseHeartBeat),    "pausehb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteResumeHeartBeat),   "resumehb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteConfigHeartBeat),   "confighb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteShowStatHeartBeat), "showstathb"    );
    addDebugFunction ((ShellCmdFunction) (&HeartBeatDebug::shellExecuteGetStatHeartBeat),  "getstathb"    );


}

// Debug function to test StartHeartBeat message
UI32 HeartBeatDebug::shellExecuteStartHeartBeat (UI32 argc, vector<string> argv)
{
    // Defualt values for heartbeat interval and max Lost Heartbeats before
    // sending heartbeat lost message.

    UI32        heartBeatInterval = 5000;
    UI32        maxLostHeartbeat  = 3;

    if (argc < 3)
    {
        cout << "starthb ipAddr port [Interval] [MaxLost]" << endl;
        return SHELL_OK;
    }

    if (argc > 3)
    {
        heartBeatInterval = atoi (argv [3].c_str ());
    }

    if (argc > 4)
    {
        maxLostHeartbeat = atoi (argv [4].c_str ());
    }


    StartHeartBeatMessage   *pMessage;

    pMessage = new StartHeartBeatMessage ();
    pMessage->setDstIpAddress (argv [1]);
    pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    pMessage->setHeartBeatInterval (heartBeatInterval);
    pMessage->setHeartBeatFailureThreshold (maxLostHeartbeat);
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}


UI32 HeartBeatDebug::shellExecuteStopHeartBeat (UI32 argc, vector<string> argv)
{
    if (argc < 3)
    {
        cout << "stophb ipAddr port" << endl;
        return SHELL_OK;
    }

    StopHeartBeatMessage   *pMessage;

    pMessage = new StopHeartBeatMessage ();
    pMessage->setDstIpAddress (argv [1]);
    pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}


UI32 HeartBeatDebug::shellExecutePauseHeartBeat (UI32 argc, vector<string> argv)
{
    if (argc < 3)
    {
        cout << "pausehb ipAddr port" << endl;
        return SHELL_OK;
    }

    PauseHeartBeatMessage   *pMessage;

    pMessage = new PauseHeartBeatMessage ();
    pMessage->setDstIpAddress (argv [1]);
    pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}


UI32 HeartBeatDebug::shellExecuteResumeHeartBeat (UI32 argc, vector<string> argv)
{
    if (argc < 3)
    {
        cout << "resumehb ipAddr port" << endl;
        return SHELL_OK;
    }

    ResumeHeartBeatMessage   *pMessage;

    pMessage = new ResumeHeartBeatMessage ();
    pMessage->setDstIpAddress (argv [1]);
    pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}

UI32 HeartBeatDebug::shellExecuteConfigHeartBeat (UI32 argc, vector<string> argv)
{
    // Defualt values for heartbeat interval and max Lost Heartbeats before
    // sending heartbeat lost message.

    UI32        heartBeatInterval = 10000;
    UI32        maxLostHeartbeat  = 10;

    if (argc < 3)
    {
        cout << "starthb ipAddr port [Interval] [MaxLost]" << endl;
        return SHELL_OK;
    }

    if (argc > 2)
    {
        heartBeatInterval = atoi (argv [2].c_str ());;
    }

    if (argc > 3)
    {
        maxLostHeartbeat = atoi (argv [3].c_str ());
    }

    ConfigHeartBeatMessage   *pMessage;

    pMessage = new ConfigHeartBeatMessage ();
    pMessage->setDstIpAddress (argv [1]);
    pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    pMessage->setHeartBeatInterval (heartBeatInterval);
    pMessage->setHeartBeatFailureThreshold (maxLostHeartbeat);
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}

UI32 HeartBeatDebug::shellExecuteShowStatHeartBeat (UI32 argc, vector<string> argv)
{
    ShowHeartBeatStatMessage   *pMessage;

    pMessage = new ShowHeartBeatStatMessage ();
    if (argc > 1)
    {
        pMessage->setDstIpAddress (argv [1]);
    }

    if (argc > 2)
    {
        pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    }
    ShellPrism::shellSendSynchronously (pMessage);
    delete (pMessage);

    return SHELL_OK;
}


UI32 HeartBeatDebug::shellExecuteGetStatHeartBeat (UI32 argc, vector<string> argv)
{
    GetHeartBeatStatMessage   *pMessage;
//    UI32                       i;

    pMessage = new GetHeartBeatStatMessage ();
    if (argc > 1)
    {
        pMessage->setDstIpAddress (argv [1]);
    }

    if (argc > 2)
    {
        pMessage->setDstPortNumber (atoi (argv [2].c_str ()));
    }
    ShellPrism::shellSendSynchronously (pMessage);

#if 0    
    for (i = 0; i < pMessage->m_heartBeatStat.size (); i++)
    {
        cout << "    ____________________________________________________________________________" << endl << endl;
        cout << "    - Node Name:" << pMessage->getDstIpAddress().toString() << endl;
        cout << "        HBSent            :" << pMessage->m_heartBeatStat [i].m_heartBeatSent << endl;
        cout << "        HBReplySent       :" << pMessage->m_heartBeatStat [i].m_heartBeatReplySent << endl;
        cout << "        LastHBReceived    :" << pMessage->m_heartBeatStat [i].m_lastHeartBeatReceived << endl;
        cout << "        HBReceived        :" << pMessage->m_heartBeatStat [i].m_heartBeatReceived << endl;
        cout << "        LastHBReplyRcved  :" << pMessage->m_heartBeatStat [i].m_lastHeartBeatReplyReceived << endl;
        cout << "        HBReplyRcved      :" << pMessage->m_heartBeatStat [i].m_heartBeatReplyReceived << endl;
        cout << "        HBLost            :" << pMessage->m_heartBeatStat [i].m_heartBeatLost << endl;
        cout << "        HBLostInRow       :" << pMessage->m_heartBeatStat [i].m_heartBeatLostInRow << endl;
        cout << "        HBDelayed         :" << pMessage->m_heartBeatStat [i].m_heartBeatDelayed << endl;
        cout << "        HBDuplicate       :" << pMessage->m_heartBeatStat [i].m_heartBeatDuplicate<< endl;
        cout << "    ____________________________________________________________________________" << endl << endl ;
    }
#endif
    delete (pMessage);

    return SHELL_OK;
}


}
