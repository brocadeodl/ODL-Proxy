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
 *   Author : ksubrama                                                     *
 **************************************************************************/

#ifndef FSPFSERVICEGETNEIGHBORSTATEINFOINFOMESSAGE_H
#define FSPFSERVICEGETNEIGHBORSTATEINFOINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FspfServiceGetNeighborStateinfoInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FspfServiceGetNeighborStateinfoInfoMessage ();
            FspfServiceGetNeighborStateinfoInfoMessage (const UI32 &input_port,const UI32 &isl_portcount,const vector<SI32> &src_port,const vector<SI32> &src_blade,const vector<SI32> &src_index,const vector<SI32> &src_dispType,const vector<SI32> &domain_id,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<SI32> &dest_index,const vector<SI32> &nbr_dispType,const vector<SI32> &interfaceType,const vector<SI32> &nbr_state,const vector<string> &nbr_state_str,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx);
            virtual    ~FspfServiceGetNeighborStateinfoInfoMessage ();
            void setInput_port(const UI32 &input_port);
            UI32 getInput_port()  const;
            void setIsl_portcount(const UI32 &isl_portcount);
            UI32 getIsl_portcount()  const;
            void setSrc_port(const vector<SI32> &srcPort);
            vector<SI32> getSrc_port()  const;
            void setDomain_id(const vector<SI32> &domain_id);
            vector<SI32> getDomain_id()  const;
            void setDest_blade(const vector<SI32> &dest_blade);
            vector<SI32> getDest_blade()  const;
            void setDest_port(const vector<SI32> &dest_port);
            vector<SI32> getDest_port()  const;
            void setDest_index(const vector<SI32> &dest_index);
            vector<SI32> getDest_index()  const;
            void setNbr_state(const vector<SI32> &nbr_state);
            vector<SI32> getNbr_state()  const;
            void setNbr_state_str(const vector<string> &nbr_state_str);
            vector<string> getNbr_state_str()  const;
            vector<SI32> getSrc_index()  const;
            void setSrc_index(const vector<SI32> &srcIndex);
            vector<SI32> getSrc_blade()  const;
            void setSrc_blade(const vector<SI32> &srcBlade);
            vector<SI32> getSrc_dispType()  const;
            void setSrc_dispType(const vector<SI32> &srcDispType);
            vector<SI32> getNbr_dispType()  const;
            void setNbr_dispType(const vector<SI32> &nbrDispType);
            vector<SI32> get_interfaceType() const;
            void set_interfaceType(const vector<SI32> &interfaceType);
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const; 
            
// Now the data members

        private:
        protected:
        public:
            UI32  m_input_port;
            UI32  m_isl_portcount;
            vector<SI32>  m_src_port;
            vector<SI32>  m_src_blade;
            vector<SI32>  m_src_index;
            vector<SI32>  m_src_dispType; 
            vector<SI32>  m_domain_id;
            vector<SI32>  m_dest_blade;
            vector<SI32>  m_dest_port;
            vector<SI32>  m_dest_index;
            vector<SI32>  m_nbr_dispType;
            vector<SI32>  m_interfaceType;
            vector<SI32>  m_nbr_state;
            vector<string>  m_nbr_state_str;
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FSPFSERVICEGETNEIGHBORSTATEINFOINFOMESSAGE_H
