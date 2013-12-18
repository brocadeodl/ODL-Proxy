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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sbusch                                                     *
 **************************************************************************/

#ifndef FSPFSERVICEGETBROADCASTINFOMESSAGE_H
#define FSPFSERVICEGETBROADCASTINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

    class FspfServiceGetBroadcastInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FspfServiceGetBroadcastInfoMessage ();
            FspfServiceGetBroadcastInfoMessage (const UI32 &domain_id,const vector<UI32> &port_isl,const vector<UI32> &bcast_route_avail,const UI32 &bcastRootDomain,const UI32 &bcastRootPriority,const MacAddress &bcastRootMacAddr,const UI32 &bcastRootIPAddr,const string &bcastRootWwn,const string &bcastRootSwitchName,const UI32 &bcastLocalPriority,const vector<SI32> &srcBlade,const vector<SI32> &srcPort,const vector<SI32> &srcIndex,const vector<SI32> &srcDispType,const vector<SI32> &nbrDomain,const vector<SI32> &nbrBlade,const vector<SI32> &nbrPort,const vector<SI32> &nbrIndex,const vector<SI32> &nbrDispType,const vector<SI32> &interfaceType,const vector<UI32> &trunkPort,const vector<UI32> &bandwidth,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx);
            virtual    ~FspfServiceGetBroadcastInfoMessage ();
            void setDomain_id(const UI32 &domain_id);
            UI32 getDomain_id()  const;
            void setPort_isl(const vector<UI32> &port_isl);
            vector<UI32> getPort_isl()  const;
            void setBcast_route_avail(const vector<UI32> &bcast_route_avail);
            vector<UI32> getBcast_route_avail()  const;
            void setBcastRootDomain(const UI32 &bcastRootDomain);
            UI32 getBcastRootDomain()  const;
            void setBcastRootPriority(const UI32 &bcastRootPriority);
            UI32 getBcastRootPriority()  const;
            void setBcastRootMacAddr(const MacAddress &bcastRootMacAddr);
            MacAddress getBcastRootMacAddr()  const;
            void setBcastRootIPAddr(const UI32 &bcastRootIPAddr);
            UI32 getBcastRootIPAddr()  const;
            void setBcastRootWwn(const string &bcastRootWwn);
            string getBcastRootWwn()  const;
            void setBcastRootSwitchName(const string &bcastRootSwitchName);
            string getBcastRootSwitchName()  const;
            void setBcastLocalPriority(const UI32 &bcastLocalPriority);
            UI32 getBcastLocalPriority()  const;
            void setSrcBlade(const vector<SI32> &srcBlade);
            vector<SI32> getSrcBlade()  const;
            void setSrcPort(const vector<SI32> &srcPort);
            vector<SI32> getSrcPort()  const;
            void setSrcIndex(const vector<SI32> &srcIndex);
            vector<SI32> getSrcIndex()  const;
            void setSrcDispType(const vector<SI32> &srcDispType);
            vector<SI32> getSrcDispType()  const;
            void setNbrDomain(const vector<SI32> &nbrDomain);
            vector<SI32> getNbrDomain()  const;
            void setNbrBlade(const vector<SI32> &nbrBlade);
            vector<SI32> getNbrBlade()  const;
            void setNbrPort(const vector<SI32> &nbrPort);
            vector<SI32> getNbrPort()  const;
            void setNbrIndex(const vector<SI32> &nbrIndex);
            vector<SI32> getNbrIndex()  const;
            void setNbrDispType(const vector<SI32> &nbrDispType);
            vector<SI32> getNbrDispType()  const;
            void setInterfaceType(const vector<SI32> &interfaceType);
            vector<SI32> getInterfaceType()  const;
            void setTrunkPort(const vector<UI32> &trunkPort);
            vector<UI32> getTrunkPort()  const;
            void setBandwidth(const vector<UI32> &bandwidth);
            vector<UI32> getBandwidth()  const;
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_domain_id;
            vector<UI32>  m_port_isl;
            vector<UI32>  m_bcast_route_avail;
            UI32  m_bcastRootDomain;
            UI32  m_bcastRootPriority;
            MacAddress  m_bcastRootMacAddr;
            UI32  m_bcastRootIPAddr;
            string  m_bcastRootWwn;
            string  m_bcastRootSwitchName;
            UI32  m_bcastLocalPriority;
            vector<SI32>  m_srcBlade;
            vector<SI32>  m_srcPort;
            vector<SI32>  m_srcIndex;
            vector<SI32>  m_srcDispType;
            vector<SI32>  m_nbrDomain;
            vector<SI32>  m_nbrBlade;
            vector<SI32>  m_nbrPort;
            vector<SI32>  m_nbrIndex;
            vector<SI32>  m_nbrDispType;
            vector<SI32>  m_interfaceType;
            vector<UI32>  m_trunkPort;
            vector<UI32>  m_bandwidth;
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FSPFSERVICEGETBROADCASTINFOMESSAGE_H
