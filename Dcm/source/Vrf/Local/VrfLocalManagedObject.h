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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef VRFLOCALMANAGEDOBJECT_H
#define VRFLOCALMANAGEDOBJECT_H
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressUC.h"
#include "Framework/Types/StringUC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Vrf/Local/VrfAfIpv4UcastManagedObject.h"
#include "Vrf/Local/VrfRouteTargetManagedObject.h"


using namespace WaveNs;

namespace DcmNs
{

class VrfLocalManagedObject : public DcmLocalManagedObject
{
    private :
    protected :
        virtual void        setupAttributesForPersistence ();
        virtual void        setupAttributesForCreate      ();

    public :
                            VrfLocalManagedObject         (WaveObjectManager *pWaveObjectManager);
        virtual            ~VrfLocalManagedObject         ();

        static  string      getClassName                  ();

                void        setVrfName                    (const string &vrfName);
                string      getVrfName                    (void) const;

                void               setRd                    (const string &rd, bool userCfg);
                string           getRd                    (void) const;

                IpV4AddressUC      getRouterId              (void) const { return m_routerId; }
                void               setRouterId              (const IpV4Address &ip, bool userCfg) { m_routerId.setIsUserConfigured(userCfg); m_routerId.setIpV4AddressValue(ip);}
                //WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> getVrfAfIpv4() const;
                void setVrfAfIpv4(const WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> &vrfAfIpv4Mo);
//this funtion also update vrfMo
                void setVrfAfIpv4( VrfAfIpv4UcastManagedObject *mo);
                VrfAfIpv4UcastManagedObject * getVrfAfIpv4(void);
                void setRt(VrfRouteTargetManagedObject *mo);
                void setRt(vector<WaveManagedObjectPointer<VrfRouteTargetManagedObject> > rt) {m_routeTarget = rt;}
                vector<WaveManagedObjectPointer<VrfRouteTargetManagedObject> > getRt(void) { return m_routeTarget;}

    // Now the data members

    private :
                string                 m_vrfName;
                string                 m_rd;
                IpV4AddressUC          m_routerId;
                WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> m_vrfAfIpv4;
                vector<WaveManagedObjectPointer<VrfRouteTargetManagedObject> > m_routeTarget;
    protected :
    public:
};

}

#endif // VRFLOCALMANAGEDOBJECT_H
