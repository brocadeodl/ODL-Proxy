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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVECLIENTRECEIVEROBJECTMANAGER_H
#define WAVECLIENTRECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class ServerStreamingSocket;
class WaveClientReceiverThread;
class WaveAsynchronousContextForBootPhases;
class WaveClientTransportObjectManager;

class WaveClientReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                       WaveClientReceiverObjectManager ();

        virtual void   initialize                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void   boot                                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        static  bool   addWaveClientReceiverThreadToCache                  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, WaveClientReceiverThread *&pWaveClientReceiverThread, const string& remoteLocationMessageVersion);
        static  void   removeWaveClientReceiverThreadFromCache             (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static  void   terminateAndRemoveWaveClientReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static  string getRemoteLocationUniqueIdentifier                   (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    protected :
    public :
        virtual                                 ~WaveClientReceiverObjectManager ();

        static  WaveClientReceiverObjectManager *getInstance                     ();
        static  PrismServiceId                   getPrismServiceId               ();

        static  bool                             isACurrentlyConnectedLocation   (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

        static  void                             debugPrint                      (UI32 argc, vector<string> argv);
        static  void                             registerDebugShellEntries       ();

        static  void                             setAutoScanForAvailablePort     (const bool &autoScanForAvailablePort);

        static string                            getMessageVersionForWaveClient  (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static UI8                               getSerializationTypeForWaveClient(const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    // Now the Data Members

    private :
        static map<string, WaveClientReceiverThread *>  s_waveClientReceiverThreadCache;
        static PrismMutex                               s_waveClientReceiverThreadCacheMutex;

               ServerStreamingSocket                   *m_pServerSocketForWaveManagementInterfaceServers;

        static bool                                     m_autoScanForAvailablePort;
        static UI32                                     m_autoScanLimit;

    protected :
    public :

    friend class WaveClientReceiverThread;
    friend class WaveClientTransportObjectManager;
};

}

#endif // WAVECLIENTRECEIVEROBJECTMANAGER_H
