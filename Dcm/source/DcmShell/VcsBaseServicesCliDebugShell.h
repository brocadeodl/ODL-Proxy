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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSBASESERVICESCLIDEBUGSHELL_H
#define VCSBASESERVICESCLIDEBUGSHELL_H

#include "Shell/WaveCliShell.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsBaseServicesCliDebugShell : public WaveCliShell
{
    private :
        void       briefHelp             ();

        ResourceId getLocalMappedId            (const vector<string> &arguments);
        ResourceId getLocalMappedIdCWay        (const vector<string> &arguments);
        ResourceId setLocalFcfMacCWay          (const vector<string> &arguments);
        ResourceId setLocalFcfMac              (const vector<string> &arguments);
        ResourceId getVcsId                    (const vector<string> &arguments);
        ResourceId getVcsMode                  (const vector<string> &arguments);
        void       queryForBaseServicesHelp    (void);

    protected :
    public :
                                                VcsBaseServicesCliDebugShell                  (ValClientSynchronousConnection &connection);
        virtual                                ~VcsBaseServicesCliDebugShell                  ();

                ValClientSynchronousConnection &getValClientSynchronousConnection ();

    // Now the data members

    private :
        ValClientSynchronousConnection &m_valClientSynchronousConnection;

    protected :
    public :
};

}

#endif // VCSBASESERVICESCLIDEBUGSHELL_H
