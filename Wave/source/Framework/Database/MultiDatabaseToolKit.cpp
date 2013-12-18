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

#include "Framework/Database/MultiDatabaseToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/Integer.h"

#include <stdlib.h>

namespace WaveNs
{

vector<string> MultiDatabaseToolKit::m_serverConnections;
vector<string> MultiDatabaseToolKit::m_serverNames;
vector<string> MultiDatabaseToolKit::m_serverIpAddresses;
vector<UI32>   MultiDatabaseToolKit::m_serverPorts;
bool           MultiDatabaseToolKit::m_isMultiModeEnabled = false;

void MultiDatabaseToolKit::addServerConnection (const string &serverConnection)
{
    vector<string> tokensWithinAServerConnection;
    UI32           numberOfTokensWithinAServerConnection = 0;
    string         serverName;
    string         serverIpAddress;
    UI32           serverPort                            = 0;

    StringUtils::tokenize (serverConnection, tokensWithinAServerConnection, ':');

    numberOfTokensWithinAServerConnection = tokensWithinAServerConnection.size ();

    prismAssert (3 == numberOfTokensWithinAServerConnection, __FILE__, __LINE__);

    serverName      = tokensWithinAServerConnection[0];
    serverIpAddress = tokensWithinAServerConnection[1];
    serverPort      = strtoul ((tokensWithinAServerConnection[2]).c_str (), NULL, 10);

    m_serverConnections.push_back (serverConnection);
    m_serverNames.push_back       (serverName);
    m_serverIpAddresses.push_back (serverIpAddress);
    m_serverPorts.push_back       (serverPort);

}

void MultiDatabaseToolKit::getServerConnections (vector<string> &serverConnections)
{
    serverConnections = m_serverConnections;
}

void MultiDatabaseToolKit::initializeServerConnections (const string &serverConnectionString)
{
    vector<string> serverConnectionStringTokens;
    UI32           numberOfServerConnectionsTokens = 0;
    UI32           i                               = 0;

    StringUtils::tokenize (serverConnectionString, serverConnectionStringTokens, ';');

    numberOfServerConnectionsTokens = serverConnectionStringTokens.size ();

    for (i = 0; i < numberOfServerConnectionsTokens; i++)
    {
        addServerConnection (serverConnectionStringTokens[i]);
    }
}

UI32 MultiDatabaseToolKit::getNumberOfServerConnections ()
{
    return (m_serverConnections.size ());
}

void  MultiDatabaseToolKit::getServerInformationAtIndex (const UI32 &index, string &serverName, string &serverIpAddress, UI32 &serverPort)
{
    UI32 numberOfServerConnections = getNumberOfServerConnections ();

    prismAssert (index <= (numberOfServerConnections - 1), __FILE__, __LINE__);

    serverName      = m_serverNames[index];
    serverIpAddress = m_serverIpAddresses[index];
    serverPort      = m_serverPorts[index];
}

bool MultiDatabaseToolKit::getIsMultiModeEnabled ()
{
    return (m_isMultiModeEnabled);
}

void MultiDatabaseToolKit::setIsMultiModeEnabled (const bool &isMultiModeEnabled)
{
    m_isMultiModeEnabled = isMultiModeEnabled;
}

}
