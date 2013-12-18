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

#ifndef HTTPINTERFACERECEIVEROBJECTMANAGER_H
#define HTTPINTERFACERECEIVEROBJECTMANAGER_H

#include <Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h>

namespace WaveNs
{

class ServerStreamingSocket;
class BootCompleteForThisLocationEvent;
class WaveAsynchronousContextForBootPhases;
class HttpInterfaceMethodWorker;

class HttpInterfaceReceiverObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                      HttpInterfaceReceiverObjectManager      ();
        
        virtual void  initialize                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void  boot                                    (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void  listenForEvents                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void  bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

    protected :
    public :
        virtual                                    ~HttpInterfaceReceiverObjectManager ();

        static  string                              getServiceName                     ();
        static  HttpInterfaceReceiverObjectManager *getInstance                        ();
        static  PrismServiceId                      getPrismServiceId                  ();

        static  bool                                isAKnownHttpInterfaceMethod        (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod);
        static  void                                addHttpInterfaceMethodWorker       (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod, HttpInterfaceMethodWorker *pHttpInterfaceMethodWorker);
        static  HttpInterfaceMethodWorker          *getHttpInterfaceMethodWorker       (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod);

    // Now the data members

    private :
               ServerStreamingSocket                                     *m_pServerSocketForHttpInterfaceClients;

        static map<WaveHttpInterfaceMethod, HttpInterfaceMethodWorker *>  m_httpInterfaceMethodWorkers;

    protected :
    public :
};

}

#endif // HTTPINTERFACERECEIVEROBJECTMANAGER_H
