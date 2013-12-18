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
#include "Bgp/Local/BgpLocalBasicConfigWorker.h"
#include "Bgp/Local/BgpLocalBasicConfigManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


BgpLocalBasicConfigWorker::BgpLocalBasicConfigWorker(BgpLocalObjectManager *pBgpLocalObjectManager)
    :WaveWorker  (pBgpLocalObjectManager )

{
    BgpLocalBasicConfigManagedObject instanceBgpLocalBasicConfigManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalBasicConfigManagedObject::getClassName(), this);
    instanceBgpLocalBasicConfigManagedObject.setupOrm ();
    BgpLocalNeighborIpxAddrManagedObject instanceBgpLocalNeighborIpxAddrManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalNeighborIpxAddrManagedObject::getClassName(), this);
    instanceBgpLocalNeighborIpxAddrManagedObject.setupOrm ();
    BgpLocalNeighborPeerGrpManagedObject instanceBgpLocalNeighborPeerGrpManagedObject (pBgpLocalObjectManager);
    addManagedClass (BgpLocalNeighborPeerGrpManagedObject::getClassName(), this);
    instanceBgpLocalNeighborPeerGrpManagedObject.setupOrm ();
}

BgpLocalBasicConfigWorker::~BgpLocalBasicConfigWorker()
{
}

PrismMessage *BgpLocalBasicConfigWorker:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *BgpLocalBasicConfigWorker:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((BgpLocalBasicConfigManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalBasicConfigManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalNeighborIpxAddrManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalNeighborIpxAddrManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((BgpLocalNeighborPeerGrpManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new BgpLocalNeighborPeerGrpManagedObject (dynamic_cast<BgpLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"BgpLocalBasicConfigWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}



}

