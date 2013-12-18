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


#ifndef BGPLOCALAFIPV4COMMONATTRIBUTESCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALAFIPV4COMMONATTRIBUTESCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Bgp/Local/BgpLocalAfIpv4NetworkManagedObject.h"
#include "Bgp/Local/BgpLocalAggregateIpv4AddressManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"



using namespace WaveNs;


namespace DcmNs{
class BgpLocalAggregateIpv4AddressManagedObject;
class BgpLocalAfIpv4NetworkManagedObject;


class BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase   (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase  ();
                void                                        setaggregateAddress                                     (const vector<WaveManagedObjectPointer<BgpLocalAggregateIpv4AddressManagedObject> > &aggregateAddress);
                vector<WaveManagedObjectPointer<BgpLocalAggregateIpv4AddressManagedObject> > getaggregateAddress                                     ();
                void                                        setnetwork                                              (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NetworkManagedObject> > &network);
                vector<WaveManagedObjectPointer<BgpLocalAfIpv4NetworkManagedObject> > getnetwork                                              ();
    private   :
    protected :
    public    :
        vector<WaveManagedObjectPointer<BgpLocalAggregateIpv4AddressManagedObject> > m_aggregateAddress;
        vector<WaveManagedObjectPointer<BgpLocalAfIpv4NetworkManagedObject> > m_network;
};
 
}
#endif
