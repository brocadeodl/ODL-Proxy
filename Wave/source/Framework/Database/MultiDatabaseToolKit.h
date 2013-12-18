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

#ifndef MULTIDATABASETOOLKIT_H
#define MULTIDATABASETOOLKIT_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class MultiDatabaseToolKit
{
    private :
    protected :
    public :
        static void addServerConnection          (const string &serverConnection);
        static void getServerConnections         (vector<string> &serverConnections);
        static void initializeServerConnections  (const string &serverConnectionString);
        static UI32 getNumberOfServerConnections ();
        static void getServerInformationAtIndex  (const UI32 &index, string &serverName, string &serverIpAddress, UI32 &serverPort);

        static bool getIsMultiModeEnabled        ();
        static void setIsMultiModeEnabled        (const bool &isMultiModeEnabled);

        // Now the data members

    private :
    protected :
    public :
        static vector<string> m_serverConnections;
        static vector<string> m_serverNames;
        static vector<string> m_serverIpAddresses;
        static vector<UI32>   m_serverPorts;
        static bool           m_isMultiModeEnabled;
};

}
#endif // MULTIDATABASETOOLKIT_H
