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

#ifndef MANAGEMENTINTERFACERECEIVEROBJECTMANAGER_H
#define MANAGEMENTINTERFACERECEIVEROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

class ServerStreamingSocket;
class ManagementInterfaceReceiverThread;
class BootCompleteForThisLocationEvent;
class WaveAsynchronousContextForBootPhases;

class ManagementInterfaceReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                       ManagementInterfaceReceiverObjectManager                    ();

        virtual void   initialize                                                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void   boot                                                        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void   listenForEvents                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void   bootCompleteForThisLocationEventHandler                     (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);


        static  bool   addManagementInterfaceReceiverThreadToCache                 (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, ManagementInterfaceReceiverThread *&pManagementInterfaceReceiverThread);
        static  void   removeManagemntInterfaceReceiverThreadFromCache             (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);
        static  string getRemoteLocationUniqueIdentifier                           (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    protected :
    public :
        virtual                                          ~ManagementInterfaceReceiverObjectManager ();
        static  ManagementInterfaceReceiverObjectManager *getInstance                              ();
        static  PrismServiceId                            getPrismServiceId                        ();

        static  bool                                      isACurrentlyConnectedLocation            (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

        static  void                                      debugPrint                               (UI32 argc, vector<string> argv);
        static  void                                      registerDebugShellEntries                ();
        static  void   terminateAndRemoveManagemntInterfaceReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

        static string                                     getMessageVersionForManagemntInterface    (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort); 
        static UI8                                        getSerializationTypeForManagemntInterface (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort);

    // Now the data members

    private :
        ServerStreamingSocket *m_pServerSocketForManagementInterfaceClients;

    protected :
    public :

    friend class ManagementInterfaceReceiverThread;
};

}

#endif // MANAGEMENTINTERFACERECEIVEROBJECTMANAGER_H
