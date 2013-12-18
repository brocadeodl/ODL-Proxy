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

#ifndef NETWORKCONFIGURATIONOBJECTMANAGER_H
#define NETWORKCONFIGURATIONOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationCreateNetworkWorker;
class NetworkConfigurationDeleteNetworkWorker;
class NetworkConfigurationCreateInterfaceWorker;
class NetworkConfigurationAttachVirtualInterfaceWorker;
class NspNetworksWaveServerMultiPage;
class NspNetworksWaveServerMultiPageForRestXml;
class NspNetworkInterfacesWaveServerMultiPageForRestXml;

class NetworkConfigurationObjectManager : public WaveObjectManager
{
    private :
                           NetworkConfigurationObjectManager ();

        WaveManagedObject *createManagedObjectInstance       (const string &managedClassName);

    protected :
    public :
        virtual                                   ~NetworkConfigurationObjectManager ();
        static  NetworkConfigurationObjectManager *getInstance                       ();
        static  string                             getClassName                      ();
        static  PrismServiceId                     getPrismServiceId                 ();

    // Now the data members

    private :
        NetworkConfigurationCreateNetworkWorker           *m_pNetworkConfigurationCreateNetworkWorker;
        NetworkConfigurationDeleteNetworkWorker           *m_pNetworkConfigurationDeleteNetworkWorker;
        NetworkConfigurationCreateInterfaceWorker         *m_pNetworkConfigurationCreateInterfaceWorker;
        NetworkConfigurationAttachVirtualInterfaceWorker  *m_pNetworkConfigurationAttachVirtualInterfaceWorker;
        NspNetworksWaveServerMultiPage                    *m_pNspNetworksWaveServerMultiPage;
        NspNetworksWaveServerMultiPageForRestXml          *m_pNspNetworksWaveServerMultiPageForRestXml;
        NspNetworkInterfacesWaveServerMultiPageForRestXml *m_pNspNetworkInterfacesWaveServerMultiPageForRestXml;

    protected :
    public :
};

}
#endif // NETWORKCONFIGURATIONOBJECTMANAGER_H
