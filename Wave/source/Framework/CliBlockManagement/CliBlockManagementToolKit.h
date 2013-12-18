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
 *   Author : Kuai Yu                                                *
 ***************************************************************************/

#ifndef CLIBLOCKMANAGEMENTTOOLKIT
#define CLIBLOCKMANAGEMENTTOOLKIT

#include "Framework/CliBlockManagement/CliBlockDetail.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveWorker.h"

using namespace std;

namespace WaveNs
{

class CliBlockManagementToolKit
{
    private :

        static ResourceId             blockCli   (const CliBlockDetail &detail);
        static ResourceId             unblockCli (const CliBlockDetail &detail); 

    protected :
    public :

        static bool                   isCliBlocked                                    (vector<ResourceId> &reasons);
        static ResourceId             getCliBlockDetail                               (const ResourceId givenReason, CliBlockDetail &detail);
        static ResourceId             cleanAllBlockedReasons                          ();
        static string                 getDisplayString                                ();

        static PrismMutexStatus       lock                                            ();
        static PrismMutexStatus       unlock                                          ();
        static PrismMutexStatus       tryLock                                         ();

        // Now the data members

    private :
        static map <ResourceId, CliBlockDetail>                                         m_cliBlockMap;
        static PrismMutex                                                               m_cliBlockMapMutex;
        static PrismMutex                                                               m_cliBlockOperationMutex;

    protected :
    public :

    friend class WaveObjectManager;
    friend class WaveWorker;
    friend class ClusterLocalObjectManager;
    friend class CliBlockWorker;


};
}

#endif // CLIBLOCKMANAGEMENTTOOLKIT