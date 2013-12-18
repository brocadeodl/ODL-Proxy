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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Framework/Utils/AssertUtils.h"

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationDeleteNetworkWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceWorker.h"

#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkInterfaceManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/ServiceInterfaceManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/ExtensionInterfaceManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/RouterInterfaceManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/VirtualMachineManagedObject.h"

#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworksWaveServerMultiPage.h"
#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworksWaveServerMultiPageForRestXml.h"
#include "Nsp/NetworkConfiguration/Global/Rest/NspNetworkInterfacesWaveServerMultiPageForRestXml.h"

namespace NspNs
{

NetworkConfigurationObjectManager::NetworkConfigurationObjectManager ()
    : WaveObjectManager (getClassName ())
{
    NetworkManagedObject            networkManagedObject            (this);
    NetworkSegmentManagedObject     networkSegmentManagedObject     (this);
    NetworkInterfaceManagedObject   networkInterfaceManagedObject   (this);
    VnicManagedObject               vnicManagedObject               (this);
    ServiceInterfaceManagedObject   serviceInterfaceManagedObject   (this);
    ExtensionInterfaceManagedObject extensionInterfaceManagedObject (this);
    RouterInterfaceManagedObject    routerInterfaceManagedObject    (this);
    VirtualMachineManagedObject     virtualMachineManagedObject     (this);

    networkManagedObject.setupOrm            ();
    networkSegmentManagedObject.setupOrm     ();
    networkInterfaceManagedObject.setupOrm   ();
    vnicManagedObject.setupOrm               ();
    serviceInterfaceManagedObject.setupOrm   ();
    extensionInterfaceManagedObject.setupOrm ();
    routerInterfaceManagedObject.setupOrm    ();
    virtualMachineManagedObject.setupOrm     ();

    addManagedClass (NetworkManagedObject::getClassName ());
    addManagedClass (NetworkSegmentManagedObject::getClassName ());
    addManagedClass (NetworkInterfaceManagedObject::getClassName ());
    addManagedClass (VnicManagedObject::getClassName ());
    addManagedClass (ServiceInterfaceManagedObject::getClassName ());
    addManagedClass (ExtensionInterfaceManagedObject::getClassName ());
    addManagedClass (RouterInterfaceManagedObject::getClassName ());
    addManagedClass (VirtualMachineManagedObject::getClassName ());

    m_pNetworkConfigurationCreateNetworkWorker = new NetworkConfigurationCreateNetworkWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationCreateNetworkWorker, __FILE__, __LINE__);

    m_pNetworkConfigurationDeleteNetworkWorker = new NetworkConfigurationDeleteNetworkWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationDeleteNetworkWorker, __FILE__, __LINE__);

    m_pNetworkConfigurationCreateInterfaceWorker = new NetworkConfigurationCreateInterfaceWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationCreateInterfaceWorker, __FILE__, __LINE__);

    m_pNetworkConfigurationAttachVirtualInterfaceWorker = new NetworkConfigurationAttachVirtualInterfaceWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationAttachVirtualInterfaceWorker, __FILE__, __LINE__);

    m_pNspNetworksWaveServerMultiPage = new NspNetworksWaveServerMultiPage (HttpInterfaceReceiverObjectManager::getInstance (), "/tenants/*/networks");

    prismAssert (NULL != m_pNspNetworksWaveServerMultiPage, __FILE__, __LINE__);

    m_pNspNetworksWaveServerMultiPageForRestXml = new NspNetworksWaveServerMultiPageForRestXml (HttpInterfaceReceiverObjectManager::getInstance (), "/rest/slm/tenants/*/networks");

    prismAssert (NULL != m_pNspNetworksWaveServerMultiPageForRestXml, __FILE__, __LINE__);

    m_pNspNetworkInterfacesWaveServerMultiPageForRestXml = new NspNetworkInterfacesWaveServerMultiPageForRestXml (HttpInterfaceReceiverObjectManager::getInstance (), "/rest/slm/tenants/*/networks/*/ports");

    prismAssert (NULL != m_pNspNetworkInterfacesWaveServerMultiPageForRestXml, __FILE__, __LINE__);
}

NetworkConfigurationObjectManager::~NetworkConfigurationObjectManager ()
{
    if (NULL != m_pNetworkConfigurationCreateNetworkWorker)
    {
        delete m_pNetworkConfigurationCreateNetworkWorker;
    }

    if (NULL != m_pNetworkConfigurationDeleteNetworkWorker)
    {
        delete m_pNetworkConfigurationDeleteNetworkWorker;
    }

    if (NULL != m_pNetworkConfigurationCreateInterfaceWorker)
    {
        delete m_pNetworkConfigurationCreateInterfaceWorker;
    }

    if (NULL != m_pNetworkConfigurationAttachVirtualInterfaceWorker)
    {
        delete m_pNetworkConfigurationAttachVirtualInterfaceWorker;
    }

    if (NULL != m_pNspNetworksWaveServerMultiPage)
    {
        delete m_pNspNetworksWaveServerMultiPage;
    }

    if (NULL != m_pNspNetworksWaveServerMultiPageForRestXml)
    {
        delete m_pNspNetworksWaveServerMultiPageForRestXml;
    }

    if (NULL != m_pNspNetworkInterfacesWaveServerMultiPageForRestXml)
    {
        delete m_pNspNetworkInterfacesWaveServerMultiPageForRestXml;
    }
}

string NetworkConfigurationObjectManager::getClassName()
{
    return ("Network Service Provider");
}

NetworkConfigurationObjectManager *NetworkConfigurationObjectManager::getInstance ()
{
    static NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager = NULL;

    if (NULL == pNetworkConfigurationObjectManager)
    {
        pNetworkConfigurationObjectManager = new NetworkConfigurationObjectManager ();

        WaveNs::prismAssert (NULL != pNetworkConfigurationObjectManager, __FILE__, __LINE__);
    }

    return (pNetworkConfigurationObjectManager);
}

PrismServiceId NetworkConfigurationObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

WaveManagedObject *NetworkConfigurationObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((NetworkManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NetworkManagedObject (this);
    }
    else if ((NetworkSegmentManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NetworkSegmentManagedObject (this);
    }
    else if ((NetworkInterfaceManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NetworkInterfaceManagedObject (this);
    }
    else if ((VnicManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new VnicManagedObject (this);
    }    else
    {
        trace (TRACE_LEVEL_FATAL, "NetworkConfigurationObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pWaveManagedObject);
}

}

