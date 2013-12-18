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


#ifndef BGPLOCALAFIPV4UCASTANDMCASTATTRIBUTESCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALAFIPV4UCASTANDMCASTATTRIBUTESCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborIpAddrManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NeighborPeerGrpManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

#include "Bgp/Local/BgpLocalAfCommonAttributesCommonManagedObjectBase.h"


using namespace WaveNs;


namespace DcmNs{
class BgpLocalAfIpv4NeighborIpAddrManagedObject;
class BgpLocalAfIpv4NeighborPeerGrpManagedObject;


class BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase:  virtual public BgpLocalAfCommonAttributesCommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase();
                void                                        setaddressFamilyNeighborIpAddr                          (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborIpAddrManagedObject> > &addressFamilyNeighborIpAddr);
                vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborIpAddrManagedObject> > getaddressFamilyNeighborIpAddr                          ();
                void                                        setaddressFamilyNeighborPeerGrp                         (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborPeerGrpManagedObject> > &addressFamilyNeighborPeerGrp);
                vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborPeerGrpManagedObject> > getaddressFamilyNeighborPeerGrp                         ();
    private   :
    protected :
    public    :
        vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborIpAddrManagedObject> > m_addressFamilyNeighborIpAddr;
        vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborPeerGrpManagedObject> > m_addressFamilyNeighborPeerGrp;
};
 
}
#endif
