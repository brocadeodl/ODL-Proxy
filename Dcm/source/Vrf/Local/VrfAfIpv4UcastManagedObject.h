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
 *   Author : rmuthigi                                                     *
 ***************************************************************************/

#ifndef VRFAFIPV4LOCALMANAGEDOBJECT_H
#define VRFAFIPV4LOCALMANAGEDOBJECT_H 

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "Vrf/Local/VrfRouteTargetManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

using namespace WaveNs;

namespace DcmNs
{

class VrfAfIpv4UcastManagedObject : public DcmLocalManagedObject
{
    private :
    protected :
        virtual void        setupAttributesForPersistence ();
        virtual void        setupAttributesForCreate      ();

    public :
                            VrfAfIpv4UcastManagedObject         (WaveObjectManager *pWaveObjectManager);
                            VrfAfIpv4UcastManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage );
        virtual            ~VrfAfIpv4UcastManagedObject         ();

        static  string      getClassName                  ();
        void setMaxRoutes(const UI32 &maxRoutes) {m_maxRoutes = maxRoutes;}
        UI32 getMaxRoutes(void) const {return m_maxRoutes;}

        void setAfIpv4Flag(const bool flag) { m_ipv4PresenceFlag = flag;}
        bool getAfIpv4Flag(void)  const {return m_ipv4PresenceFlag;}
        void setRt(VrfRouteTargetAfManagedObject *mo);
        void setRt(vector<WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> > rt) {m_routeTarget = rt;}
        vector<WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> > getRt(void) { return m_routeTarget;}


    // Now the data members

    private :
        bool m_ipv4PresenceFlag; 
        UI32 m_maxRoutes;
        string m_vrfName;
        vector<WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> > m_routeTarget;
    protected :
    public:
};

}

#endif // VRFLOCALMANAGEDOBJECT_H
