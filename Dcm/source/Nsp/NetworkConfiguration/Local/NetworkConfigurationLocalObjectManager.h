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

#ifndef NETWORKCONFIGURATIONLOCALOBJECTMANAGER_H
#define NETWORKCONFIGURATIONLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

using namespace WaveNs;
using namespace DcmNs;

namespace NspNs
{

class NetworkConfigurationCreateNetworkLocalWorker;
class NetworkConfigurationDeleteNetworkLocalWorker;
class NetworkConfigurationAttachVirtualInterfaceLocalWorker;

class NetworkConfigurationLocalObjectManager : public WaveLocalObjectManager
{
    private :
             NetworkConfigurationLocalObjectManager ();

        void boot                                   (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual                                        ~NetworkConfigurationLocalObjectManager ();
        static  NetworkConfigurationLocalObjectManager *getInstance                            ();
        static  string                                  getClassName                           ();
        static  PrismServiceId                          getPrismServiceId                      ();
        static  ValClientSynchronousConnection         *getPValClientSynchronousConnection     ();

    // Now the data members

    private :
        NetworkConfigurationCreateNetworkLocalWorker          *m_pNetworkConfigurationCreateNetworkLocalWorker;
        NetworkConfigurationDeleteNetworkLocalWorker          *m_pNetworkConfigurationDeleteNetworkLocalWorker;
        NetworkConfigurationAttachVirtualInterfaceLocalWorker *m_pNetworkConfigurationAttachVirtualInterfaceLocalWorker;

        ValClientSynchronousConnection                        *m_pValClientSynchronousConnection;

    protected :
    public :
};

}
#endif // NETWORKCONFIGURATIONLOCALOBJECTMANAGER_H
