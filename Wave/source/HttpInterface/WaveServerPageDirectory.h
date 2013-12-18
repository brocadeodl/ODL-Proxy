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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVESERVERPAGEDIRECTORY_H
#define WAVESERVERPAGEDIRECTORY_H

#include "Framework/Types/Types.h"

#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveServerPage;
class WaveServerPageDirectoryEntry;

class WaveServerPageDirectory
{
    private :
    protected :
    public :
                                WaveServerPageDirectory ();
        virtual                ~WaveServerPageDirectory ();

        static  bool            isAKnownServerPage      (const string &path);

        static  void            registerServerPage      (WaveServerPage *pWaveServerPage);
        static  void            registerServerPage      (const string &path, WaveServerPage *pWaveServerPage);
        static  WaveServerPage *getWaveServerPage       (const string &path);

        static  ResourceId      print                   (UI32 argc, vector<string> argv);

    // Now the data members

    private :
        static map<string, WaveServerPage *> m_serverPageMap;
        static WaveServerPageDirectoryEntry  m_waveServerPageDirectoryRoot;

    protected :
    public :
};

}

#endif // WAVESERVERPAGEDIRECTORY_H
