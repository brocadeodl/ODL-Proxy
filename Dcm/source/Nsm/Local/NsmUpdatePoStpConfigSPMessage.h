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

#ifndef NSMUPDATEPOSTPCONFIGSPMESSAGE_H
#define NSMUPDATEPOSTPCONFIGSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"



using namespace WaveNs;

namespace DcmNs
{

	class NsmGlobalUpdatePoStpConfigMessage;
    class NsmUpdatePoStpConfigSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdatePoStpConfigSPMessage ();
            NsmUpdatePoStpConfigSPMessage (const string &ifName,const UI16 &priority,const UI32 &helloTime,const UI32 &cost,const UI32 &cmdCode);
	    NsmUpdatePoStpConfigSPMessage (NsmGlobalUpdatePoStpConfigMessage &globalMsg);

            virtual    ~NsmUpdatePoStpConfigSPMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setPriority(const UI16 &priority);
            UI16 getPriority()  const;
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
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

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIfIndex(const UI32 &ifIndex);
            UI32 getIfIndex()  const;

            void setPortId(const UI32 portId);
            UI32 getPortId() const;

	    void  printMsg();

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_ifIndex;
            UI16  m_priority;
            UI32  m_helloTime;
            UI32  m_cost;
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
            UI32 m_portId;
            UI32  m_cmdCode;
    };
}
#endif                                            //NSMUPDATEPOSTPCONFIGSPMESSAGE_H
