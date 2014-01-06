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


#ifndef BGPLOCALAFIPV4UNICASTANDVRFCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALAFIPV4UNICASTANDVRFCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Bgp/Local/BgpLocalIpv4StaticNetworkManagedObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

#include "Bgp/Local/BgpLocalRedistributeCommonManagedObjectBase.h"


using namespace WaveNs;


namespace DcmNs{
class BgpLocalIpv4StaticNetworkManagedObject;


class BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase:  virtual public BgpLocalRedistributeCommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase      (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase     ();
                void                                        setbgpRedistributeInternalFlag                          (const bool &bgpRedistributeInternalFlag);
                bool                                        getbgpRedistributeInternalFlag                          ();
                void                                        setstaticNetwork                                        (const vector<WaveManagedObjectPointer<BgpLocalIpv4StaticNetworkManagedObject> > &staticNetwork);
                vector<WaveManagedObjectPointer<BgpLocalIpv4StaticNetworkManagedObject> > getstaticNetwork                                        ();
    private   :
    protected :
    public    :
        bool                                            m_bgpRedistributeInternalFlag;
        vector<WaveManagedObjectPointer<BgpLocalIpv4StaticNetworkManagedObject> > m_staticNetwork;
};
 
}
#endif