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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef NSMUPDATEPHYSTPCONFIGMESSAGE_H
#define NSMUPDATEPHYSTPCONFIGMESSAGE_H

#include "Nsm/Common/NsmPortStpConfigMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdatePhyStpConfigMessage : public NsmPortStpConfigMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdatePhyStpConfigMessage ();
            virtual    ~NsmUpdatePhyStpConfigMessage ();

            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setRestrictedRole(const bool &restrictedRole);
            bool getRestrictedRole()  const;
            void setRestrictedTcn(const bool &restrictedTcn);
            bool getRestrictedTcn()  const;
            void setAutoEdge(const bool &autoEdge);
            bool getAutoEdge()  const;
            void setPortFast(const bool &portFast);
            bool getPortFast()  const;
            void setEdgePort(const bool &edgePort);
            bool getEdgePort()  const;
            void setBpduGuardEnable(const bool &bpduGuardEnable);
            bool getBpduGuardEnable()  const;
            void setBpduGuard(const bool &bpduGuard);
            bool getBpduGuard()  const;
            void setBpduFilter(const bool &bpduFilter);
            bool getBpduFilter()  const;
            void setGuard(const MstpPortGuardRootEnum &guard);
            MstpPortGuardRootEnum getGuard()  const;
            void setLinkShared(const MstpPortLinkTypeEnum &linkShared);
            MstpPortLinkTypeEnum getLinkShared()  const;
            void setStpEnable(const bool &stpEnable);
            bool getStpEnable()  const;
            void setBpduMac(const MstpPortPvstBpDuMac &guard);
            MstpPortPvstBpDuMac getBpduMac()  const;
            UI8	 getBpduDropDirection()	const;
            void setBpduDropDirection(const UI8 direction);

            void  printMsg();
// Now the data members

        private:
        protected:
        public:
            UI32  m_helloTime;
            bool  m_restrictedRole;
            bool  m_restrictedTcn;
            bool  m_autoEdge;
            bool  m_portFast;
            bool  m_edgePort;
            bool  m_bpduGuardEnable;
            bool  m_bpduGuard;
            bool  m_bpduFilter;
            MstpPortGuardRootEnum  m_guard;
            MstpPortLinkTypeEnum  m_linkShared;
            MstpPortPvstBpDuMac		m_mac;
            bool  m_stpEnable;
            UI8  m_bpduDropDirection;

    };
}
#endif                                            //NSMUPDATEPHYSTPCONFIGMESSAGE_H
