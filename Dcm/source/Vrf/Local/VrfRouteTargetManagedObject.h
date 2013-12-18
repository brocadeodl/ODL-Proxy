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

#ifndef VRF_ROUTE_TARGET_LOCAL_MO_H 
#define VRF_ROUTE_TARGET_LOCAL_MO_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

class VrfRouteTargetManagedObject: public DcmLocalManagedObject
{
    private :
    protected :
        virtual void        setupAttributesForPersistence ();
        virtual void        setupAttributesForCreate      ();

    public :
                            VrfRouteTargetManagedObject         (WaveObjectManager *pWaveObjectManager);
                            VrfRouteTargetManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage );
        virtual            ~VrfRouteTargetManagedObject         ();

        static  string     getClassName              ();

        void setRtType(const RtmMessageConfigRtType & rtType) { m_rtType = rtType;}
        RtmMessageConfigRtType getRtType(void)  const {return m_rtType;}

        void setVrfName(const string vrfName) { m_vrfName = vrfName;}
        string getVrfName(void) const {return m_vrfName;}

        void setRt(const string &rt) { m_rt = rt;}
        string getRt(void) const {return m_rt;}

    // Now the data members

    private :
        string m_vrfName;
        string m_rt;
        RtmMessageConfigRtType m_rtType;
    protected :
    public:
};

class VrfRouteTargetAfManagedObject: public DcmLocalManagedObject
{
    private :
    protected :
        virtual void        setupAttributesForPersistence ();
        virtual void        setupAttributesForCreate      ();

    public :
                            VrfRouteTargetAfManagedObject         (WaveObjectManager *pWaveObjectManager);
                            VrfRouteTargetAfManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage );
        virtual            ~VrfRouteTargetAfManagedObject         ();

        static  string      getClassName              ();

        void setRtType(const RtmMessageConfigRtType & rtType) { m_rtType = rtType;}
        RtmMessageConfigRtType getRtType(void)  const {return m_rtType;}

        void setVrfName(const string vrfName) { m_vrfName = vrfName;}
        string getVrfName(void) const {return m_vrfName;}

        void setRt(const string rt) { m_rt = rt;}
        string getRt(void) const {return m_rt;}

    // Now the data members

    private :
        string m_vrfName;
        string m_rt;
        RtmMessageConfigRtType m_rtType;
    protected :
    public:
};

}

#endif 
