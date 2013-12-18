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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef INTERLOCATIONMESSAGERECEIVEROBJECTMANAGER_H
#define INTERLOCATIONMESSAGERECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class InterLocationMessageReceiverThread;
class WaveAsynchronousContextForBootPhases;

class InterLocationMessageReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                      InterLocationMessageReceiverObjectManager                                          ();
               void   boot                                                                               (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        static bool   addInterLocationMessageReceiverThreadToCache                                       (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, InterLocationMessageReceiverThread *&pInterLocationMessageReceiverThread);
        static void   removeInterLocationMessageReceiverThreadFromCache                                  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static void   closePeerServerStreamingSocketAndRemoveInterLocationMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static void   terminateAndRemoveInterLocationMessageReceiverThreadFromCache                      (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static string getRemoteLocationUniqueIdentifier                                                  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    protected :
    public :
        virtual                                          ~InterLocationMessageReceiverObjectManager ();
        static InterLocationMessageReceiverObjectManager *getInstance                               ();
        static PrismServiceId                             getPrismServiceId                         ();
        static bool                                       isACurrentlyConnectedLocation             (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

        static void                                       debugPrint                                (UI32 argc, vector<string> argv);
        static void                                       registerDebugShellEntries                 ();

        static string                                     getMessageVersionForRemoteLocation        (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static UI8                                        getSerializationTypeForRemoteLocation     (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);  

    // Now the data members

    private :
    protected :
    public :

    friend class InterLocationMessageReceiverThread;
    friend class PrismFrameworkObjectManager;
};

}

#endif // INTERLOCATIONMESSAGERECEIVEROBJECTMANAGER_H
