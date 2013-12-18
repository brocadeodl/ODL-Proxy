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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef VRFLOCALMESSAGES_H
#define VRFLOCALMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Rtm/RtmClientConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VrfLocalAddVrfMessage : public ManagementInterfaceMessage
{
    private :
        void                    setupAttributesForSerialization   ();

    protected :
    public :
                                VrfLocalAddVrfMessage             (void);
                                VrfLocalAddVrfMessage             (const string &vrfName);
        virtual                ~VrfLocalAddVrfMessage             (void);

                string          getVrfName                        (void) const;
                void            setVrfName                        (const string &vrfName);
                string          getRd                             (void) const { return m_rd; }
                void            setRd                             (const string &rd) { m_rd = rd;}
                IpV4Address     getRouterId                       (void) const { return m_ip; }
                void            setRouterId                       (const IpV4Address &ip) { m_ip = ip;}
                UI32            getCmdCode                        (void) const {return m_cmdCode;}
                void            setCmdCode                        (const UI32 &cmdCode) { m_cmdCode = cmdCode;}  
                void            setVrfIpv4Flag                    (const bool flag) { m_vrfIpv4Flag = flag; }
                bool            getVrfIpv4Flag                    (void) const {return m_vrfIpv4Flag; }  
                UI32            getMaxRoutes                      (void) const {return m_maxRoutes;}
                void            setMaxRoutes                      (const UI32 &maxRoutes) {m_maxRoutes = maxRoutes;}  

                void setRtType(const RtmMessageConfigRtType & rtType) { m_rtType = rtType;}
                RtmMessageConfigRtType getRtType(void)  const {return m_rtType;}

                void setRt(const string rt) { m_rt = rt;}
                string getRt(void) const {return m_rt;}


    // Now the data members
    private :
                string          m_vrfName;
                string          m_rd;
                IpV4Address     m_ip;
                UI32            m_cmdCode;
                bool            m_vrfIpv4Flag;
                UI32            m_maxRoutes;
                string          m_rt;
                RtmMessageConfigRtType m_rtType;

    protected :
    public :
    friend class RtmClientConfigMessage; 
};

class VrfLocalDeleteVrfMessage : public ManagementInterfaceMessage
{
    private :
        void                    setupAttributesForSerialization   ();

    protected :
    public :
                                VrfLocalDeleteVrfMessage          (void);
                                VrfLocalDeleteVrfMessage          (const string &vrfName);
        virtual                ~VrfLocalDeleteVrfMessage          (void);

                string          getVrfName                        (void) const;
                void            setVrfName                        (const string &vrfName);

                UI32            getVrfServiceId                   (void) ;
                void            setVrfServiceId                   (UI32 serviceId) ;

    // Now the data members
    private :
                string          m_vrfName;
                UI32            m_vrfServiceId;

    protected :
    public :
};

}

#endif // VRFLOCALMESSAGES_H
