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

#ifndef FABRICSERVICEGETTRUNKSHOWMESSAGE_H
#define FABRICSERVICEGETTRUNKSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

    class FabricServiceGetTrunkShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FabricServiceGetTrunkShowMessage ();
            FabricServiceGetTrunkShowMessage (const vector<UI32> &group_num,const vector<SI32> &src_blade,const vector<SI32> &src_port,const vector<UI32> &src_index,const vector<SI32> &src_disp_type,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<UI32> &dest_index,const vector<SI32> &dest_disp_type,const vector<SI32> &interface_type,const vector<WorldWideName> &switch_wwn,const vector<UI32> &deskew,const vector<UI32> &domain_id,const vector<UI32> &type,const vector<bool> &port_type,const vector<UI64> &tx_bandwidth,const vector<UI64> &tx_throughput,const vector<UI64> &tx_percent,const vector<UI64> &rx_bandwidth,const vector<UI64> &rx_throughput,const vector<UI64> &rx_percent, const vector<SI32> &local_qsfpidx, const vector<SI32> &remote_qsfpidx);
            virtual    ~FabricServiceGetTrunkShowMessage ();
            void setGroup_num(const vector<UI32> &group_num);
            vector<UI32> getGroup_num()  const;
            void setSrc_blade(const vector<SI32> &src_blade);
            vector<SI32> getSrc_blade()  const;
            void setSrc_port(const vector<SI32> &src_port);
            vector<SI32> getSrc_port()  const;
            void setSrc_index(const vector<UI32> &src_index);
            vector<UI32> getSrc_index()  const;
            void setSrc_disp_type(const vector<SI32> &src_disp_type);
            vector<SI32> getSrc_disp_type()  const;
            void setDest_blade(const vector<SI32> &dest_blade);
            vector<SI32> getDest_blade()  const;
            void setDest_port(const vector<SI32> &dest_port);
            vector<SI32> getDest_port()  const;
            void setDest_index(const vector<UI32> &dest_index);
            vector<UI32> getDest_index()  const;
            void setDest_disp_type(const vector<SI32> &dest_disp_type);
            vector<SI32> getDest_disp_type()  const;
            void setInterface_type(const vector<SI32> &interface_type);
            vector<SI32> getInterface_type()  const;
            void setSwitch_wwn(const vector<WorldWideName> &switch_wwn);
            vector<WorldWideName> getSwitch_wwn()  const;
            void setDeskew(const vector<UI32> &deskew);
            vector<UI32> getDeskew()  const;
            void setDomain_id(const vector<UI32> &domain_id);
            vector<UI32> getDomain_id()  const;
            void setType(const vector<UI32> &type);
            vector<UI32> getType()  const;
            void setPort_type(const vector<bool> &port_type);
            vector<bool> getPort_type()  const;
            void setTx_bandwidth(const vector<UI64> &tx_bandwidth);
            vector<UI64> getTx_bandwidth()  const;
            void setTx_throughput(const vector<UI64> &tx_throughput);
            vector<UI64> getTx_throughput()  const;
            void setTx_percent(const vector<UI64> &tx_percent);
            vector<UI64> getTx_percent()  const;
            void setRx_bandwidth(const vector<UI64> &rx_bandwidth);
            vector<UI64> getRx_bandwidth()  const;
            void setRx_throughput(const vector<UI64> &rx_throughput);
            vector<UI64> getRx_throughput()  const;
            void setRx_percent(const vector<UI64> &rx_percent);
            vector<UI64> getRx_percent()  const;
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const;
// Now the data members

        private:
        protected:
        public:
            vector<UI32>  m_group_num;
            vector<SI32>  m_src_blade;
            vector<SI32>  m_src_port;
            vector<UI32>  m_src_index;
            vector<SI32>  m_src_disp_type;
            vector<SI32>  m_dest_blade;
            vector<SI32>  m_dest_port;
            vector<UI32>  m_dest_index;
            vector<SI32>  m_dest_disp_type;
            vector<SI32>  m_interface_type;
            vector<WorldWideName>  m_switch_wwn;
            vector<UI32>  m_deskew;
            vector<UI32>  m_domain_id;
            vector<UI32>  m_type;
			vector<bool>  m_port_type;
            vector<UI64>  m_tx_bandwidth;
            vector<UI64>  m_tx_throughput;
            vector<UI64>  m_tx_percent;
            vector<UI64>  m_rx_bandwidth;
            vector<UI64>  m_rx_throughput;
            vector<UI64>  m_rx_percent;
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FABRICSERVICEGETTRUNKSHOWMESSAGE_H
