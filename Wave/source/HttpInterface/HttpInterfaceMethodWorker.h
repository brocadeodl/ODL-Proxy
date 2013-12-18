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

#ifndef HTTPINTERFACEMETHODWORKER_H
#define HTTPINTERFACEMETHODWORKER_H

#include <Framework/ObjectModel/WaveWorker.h>
#include "Framework/Utils/ServerStreamingSocket.h"
#include "HttpInterface/HttpRequest.h"

namespace WaveNs
{

class HttpInterfaceReceiverObjectManager;

class HttpInterfaceMethodWorker : public WaveWorker
{
    private :
    protected :
    public :
                                         HttpInterfaceMethodWorker  (HttpInterfaceReceiverObjectManager* pHttpInterfaceReceiverObjectManager, const WaveHttpInterfaceMethod &waveHttpInterfaceMethod);
        virtual                         ~HttpInterfaceMethodWorker  ();

        virtual void                     process                    (const HttpRequest &httpRequest) = 0;

                WaveHttpInterfaceMethod  getWaveHttpInterfaceMethod () const;

    // Now the data members

    private :
        WaveHttpInterfaceMethod m_waveHttpInterfaceMethod;

    protected :
    public :

    
};

}

#endif // HTTPINTERFACEMETHODWORKER_H
