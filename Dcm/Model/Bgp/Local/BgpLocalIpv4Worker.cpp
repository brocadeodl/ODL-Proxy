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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#include "Bgp/Local/BgpLocalObjectManager.h"
#include "Bgp/Local/BgpLocalIpv4Worker.h"
#include "Bgp/Local/BgpLocalAfIpv4UcastManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborIpAddrManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NetworkManagedObject.h"
#include "Bgp/Local/BgpLocalAggregateIpv4AddressManagedObject.h"
#include "Bgp/Local/BgpLocalIpv4StaticNetworkManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


BgpLocalIpv4Worker::BgpLocalIpv4Worker(BgpLocalObjectManager *pBgpLocalObjectManager)
    :WaveWorker  (pBgpLocalObjectManager )

{
    BgpLocalAfIpv4UcastManagedObject instanceBgpLocalAfIpv4UcastManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalAfIpv4UcastManagedObject::getClassName(), this);
    instanceBgpLocalAfIpv4UcastManagedObject.setupOrm ();
    BgpLocalAfIpv4NeighborIpAddrManagedObject instanceBgpLocalAfIpv4NeighborIpAddrManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(), this);
    instanceBgpLocalAfIpv4NeighborIpAddrManagedObject.setupOrm ();
    BgpLocalAfIpv4NeighborPeerGrpManagedObject instanceBgpLocalAfIpv4NeighborPeerGrpManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(), this);
    instanceBgpLocalAfIpv4NeighborPeerGrpManagedObject.setupOrm ();
    BgpLocalAfIpv4NetworkManagedObject instanceBgpLocalAfIpv4NetworkManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalAfIpv4NetworkManagedObject::getClassName(), this);
    instanceBgpLocalAfIpv4NetworkManagedObject.setupOrm ();
    BgpLocalAggregateIpv4AddressManagedObject instanceBgpLocalAggregateIpv4AddressManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalAggregateIpv4AddressManagedObject::getClassName(), this);
    instanceBgpLocalAggregateIpv4AddressManagedObject.setupOrm ();
    BgpLocalIpv4StaticNetworkManagedObject instanceBgpLocalIpv4StaticNetworkManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalIpv4StaticNetworkManagedObject::getClassName(), this);
    instanceBgpLocalIpv4StaticNetworkManagedObject.setupOrm ();
}

BgpLocalIpv4Worker::~BgpLocalIpv4Worker()
{
}

PrismMessage *BgpLocalIpv4Worker:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *BgpLocalIpv4Worker:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((BgpLocalAfIpv4UcastManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalAfIpv4UcastManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalAfIpv4NeighborIpAddrManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalAfIpv4NeighborPeerGrpManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalAfIpv4NetworkManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalAfIpv4NetworkManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalAggregateIpv4AddressManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalAggregateIpv4AddressManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalIpv4StaticNetworkManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalIpv4StaticNetworkManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"BgpLocalIpv4Worker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}



}

