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
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalBasicConfigWorker.h"
#include "Bgp/Local/BgpLocalIpv4Worker.h"
#include "Bgp/Local/BgpLocalBasicConfigManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4UcastManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborIpAddrManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NetworkManagedObject.h"
#include "Bgp/Local/BgpLocalAggregateIpv4AddressManagedObject.h"
#include "Bgp/Local/BgpLocalIpv4StaticNetworkManagedObject.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "DcmResourceIdEnums.h"
#include "Utils/Layer3LicenseUtil.h"


using namespace WaveNs;


namespace DcmNs{


BgpLocalObjectManager::BgpLocalObjectManager()
    :WaveLocalObjectManager (BgpLocalObjectManager::getClassName())

{
  	associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
    m_pBgpLocalBasicConfigWorker =  new BgpLocalBasicConfigWorker(this);
    prismAssert (NULL != m_pBgpLocalBasicConfigWorker, __FILE__, __LINE__);
    m_pBgpLocalIpv4Worker =  new BgpLocalIpv4Worker(this);
    prismAssert (NULL != m_pBgpLocalIpv4Worker, __FILE__, __LINE__);
    setPostbootPhases ();
}

BgpLocalObjectManager::~BgpLocalObjectManager()
{
}

BgpLocalObjectManager *BgpLocalObjectManager::getInstance()
{
    static BgpLocalObjectManager *s_pBgpLocalObjectManager = new BgpLocalObjectManager ();
    WaveNs::prismAssert (NULL != s_pBgpLocalObjectManager, __FILE__, __LINE__);
    return (s_pBgpLocalObjectManager);
}

string BgpLocalObjectManager::getClassName()
{
    return ("BgpLocalObjectManager");
}

PrismServiceId  BgpLocalObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *BgpLocalObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *BgpLocalObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ( "" == managedClassName )
    {
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"BgpLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return(pWaveManagedObject);
}

void BgpLocalObjectManager::setPostbootPhases ()
{
    map<string, vector<string> > postbootManagedObjectNames;
    {
        string managedObject = string ("BgpLocalBasicConfigManagedObject");
        vector<string> managedObjectNames;
        managedObjectNames = postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE5"];
        managedObjectNames.push_back(managedObject);
        postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE5"] =  managedObjectNames;
    }
    setPostbootMap( postbootManagedObjectNames );
}

ResourceId   BgpLocalObjectManager::postbootValidate (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
{
    if (LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
        trace (TRACE_LEVEL_INFO, string("BgpLocalObjectManager::postbootValidate. LAYER_3_LICENSE not present"));
        return (WAVE_MESSAGE_SKIP_POSTBOOT);
    }
    return (WAVE_MESSAGE_SUCCESS);
}




}

