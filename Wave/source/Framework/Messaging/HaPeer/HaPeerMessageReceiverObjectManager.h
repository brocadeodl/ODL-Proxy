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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                       *
 ***************************************************************************/

#ifndef HAPEERMESSAGERECEIVEROBJECTMANAGER_H
#define HAPEERMESSAGERECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class HaPeerMessageReceiverThread;
class WaveAsynchronousContextForBootPhases;

class HaPeerMessageReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                      HaPeerMessageReceiverObjectManager                                          ();
               void   boot                                                                               (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        static bool   addHaPeerMessageReceiverThreadToCache                                       (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, HaPeerMessageReceiverThread *&pHaPeerMessageReceiverThread);
        static void   removeHaPeerMessageReceiverThreadFromCache                                  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static void   closePeerServerStreamingSocketAndRemoveHaPeerMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static void   terminateAndRemoveHaPeerMessageReceiverThreadFromCache                      (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static string getRemoteLocationUniqueIdentifier                                                  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    protected :
    public :
        virtual                                          ~HaPeerMessageReceiverObjectManager ();
        static HaPeerMessageReceiverObjectManager *getInstance                               ();
        static PrismServiceId                             getPrismServiceId                         ();
        static bool                                       isACurrentlyConnectedLocation             (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

        static void                                       debugPrint                                (UI32 argc, vector<string> argv);
        static void                                       registerDebugShellEntries                 ();

        static string                                     getMessageVersionForHaPeer                        (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static UI8                                        getSerializationTypeForHaPeer                     (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    // Now the data members

    private :
    protected :
    public :

    friend class HaPeerMessageReceiverThread;
    friend class PrismFrameworkObjectManager;
    friend class PrismFrameworkObjectManagerHaSyncWorker;
};

}

#endif // HAPEERMESSAGERECEIVEROBJECTMANAGER_H
