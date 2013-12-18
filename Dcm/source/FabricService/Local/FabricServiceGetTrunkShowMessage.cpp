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

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceGetTrunkShowMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceGetTrunkShowMessage::FabricServiceGetTrunkShowMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETTRUNKSHOW)
    {
    }

    FabricServiceGetTrunkShowMessage::FabricServiceGetTrunkShowMessage (const vector<UI32> &group_num,const vector<SI32> &src_blade,const vector<SI32> &src_port,const vector<UI32> &src_index,const vector<SI32> &src_disp_type,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<UI32> &dest_index,const vector<SI32> &dest_disp_type,const vector<SI32> &interface_type,const vector<WorldWideName> &switch_wwn,const vector<UI32> &deskew,const vector<UI32> &domain_id,const vector<UI32> &type,const vector<bool> &port_type,const vector<UI64> &tx_bandwidth,const vector<UI64> &tx_throughput,const vector<UI64> &tx_percent,const vector<UI64> &rx_bandwidth,const vector<UI64> &rx_throughput,const vector<UI64> &rx_percent,  const vector<SI32> &local_qsfpidx, const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETTRUNKSHOW),
        m_group_num    (group_num),
        m_src_blade    (src_blade),
        m_src_port    (src_port),
        m_src_index    (src_index),
        m_src_disp_type    (src_disp_type),
        m_dest_blade    (dest_blade),
        m_dest_port    (dest_port),
        m_dest_index    (dest_index),
        m_dest_disp_type    (dest_disp_type),
        m_interface_type    (interface_type),
        m_switch_wwn    (switch_wwn),
        m_deskew    (deskew),
        m_domain_id    (domain_id),
        m_type    (type),
        m_port_type    (port_type),
        m_tx_bandwidth    (tx_bandwidth),
        m_tx_throughput    (tx_throughput),
        m_tx_percent    (tx_percent),
        m_rx_bandwidth    (rx_bandwidth),
        m_rx_throughput    (rx_throughput),
        m_rx_percent    (rx_percent),
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx    (remote_qsfpidx)
    {
    }

    FabricServiceGetTrunkShowMessage::~FabricServiceGetTrunkShowMessage ()
    {
    }

    void  FabricServiceGetTrunkShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32Vector(&m_group_num,"group_num"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_blade,"src_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_port,"src_port"));
        addSerializableAttribute (new AttributeUI32Vector(&m_src_index,"src_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_disp_type,"src_disp_type"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_blade,"dest_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_port,"dest_port"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dest_index,"dest_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_disp_type,"dest_disp_type"));
        addSerializableAttribute (new AttributeSI32Vector(&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_switch_wwn,"switch_wwn"));
        addSerializableAttribute (new AttributeUI32Vector(&m_deskew,"deskew"));
        addSerializableAttribute (new AttributeUI32Vector(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_type,"type"));
		addSerializableAttribute (new AttributeBoolVector(&m_port_type,"port_type"));
        addSerializableAttribute (new AttributeUI64Vector(&m_tx_bandwidth,"tx_bandwidth"));
        addSerializableAttribute (new AttributeUI64Vector(&m_tx_throughput,"tx_throughput"));
        addSerializableAttribute (new AttributeUI64Vector(&m_tx_percent,"tx_percent"));
        addSerializableAttribute (new AttributeUI64Vector(&m_rx_bandwidth,"rx_bandwidth"));
        addSerializableAttribute (new AttributeUI64Vector(&m_rx_throughput,"rx_throughput"));
        addSerializableAttribute (new AttributeUI64Vector(&m_rx_percent,"rx_percent"));
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));
    }

    void  FabricServiceGetTrunkShowMessage::setGroup_num(const vector<UI32> &group_num)
    {
        m_group_num  =  group_num;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getGroup_num() const
    {
        return (m_group_num);
    }

    void  FabricServiceGetTrunkShowMessage::setSrc_blade(const vector<SI32> &src_blade)
    {
        m_src_blade  =  src_blade;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getSrc_blade() const
    {
        return (m_src_blade);
    }

    void  FabricServiceGetTrunkShowMessage::setSrc_port(const vector<SI32> &src_port)
    {
        m_src_port  =  src_port;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getSrc_port() const
    {
        return (m_src_port);
    }

    void  FabricServiceGetTrunkShowMessage::setSrc_index(const vector<UI32> &src_index)
    {
        m_src_index  =  src_index;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getSrc_index() const
    {
        return (m_src_index);
    }

    void  FabricServiceGetTrunkShowMessage::setSrc_disp_type(const vector<SI32> &src_disp_type)
    {
        m_src_disp_type  =  src_disp_type;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getSrc_disp_type() const
    {
        return (m_src_disp_type);
    }

    void  FabricServiceGetTrunkShowMessage::setDest_blade(const vector<SI32> &dest_blade)
    {
        m_dest_blade  =  dest_blade;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getDest_blade() const
    {
        return (m_dest_blade);
    }

    void  FabricServiceGetTrunkShowMessage::setDest_port(const vector<SI32> &dest_port)
    {
        m_dest_port  =  dest_port;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getDest_port() const
    {
        return (m_dest_port);
    }

    void  FabricServiceGetTrunkShowMessage::setDest_index(const vector<UI32> &dest_index)
    {
        m_dest_index  =  dest_index;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getDest_index() const
    {
        return (m_dest_index);
    }

    void  FabricServiceGetTrunkShowMessage::setDest_disp_type(const vector<SI32> &dest_disp_type)
    {
        m_dest_disp_type  =  dest_disp_type;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getDest_disp_type() const
    {
        return (m_dest_disp_type);
    }

    void  FabricServiceGetTrunkShowMessage::setInterface_type(const vector<SI32> &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    vector<SI32>  FabricServiceGetTrunkShowMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  FabricServiceGetTrunkShowMessage::setSwitch_wwn(const vector<WorldWideName> &switch_wwn)
    {
        m_switch_wwn  =  switch_wwn;
    }

    vector<WorldWideName>  FabricServiceGetTrunkShowMessage::getSwitch_wwn() const
    {
        return (m_switch_wwn);
    }

    void  FabricServiceGetTrunkShowMessage::setDeskew(const vector<UI32> &deskew)
    {
        m_deskew  =  deskew;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getDeskew() const
    {
        return (m_deskew);
    }

    void  FabricServiceGetTrunkShowMessage::setDomain_id(const vector<UI32> &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FabricServiceGetTrunkShowMessage::setType(const vector<UI32> &type)
    {
        m_type  =  type;
    }

    vector<UI32>  FabricServiceGetTrunkShowMessage::getType() const
    {
        return (m_type);
    }

    void  FabricServiceGetTrunkShowMessage::setPort_type(const vector<bool> &port_type)
    {
        m_port_type  =  port_type;
    }

    vector<bool>  FabricServiceGetTrunkShowMessage::getPort_type() const
    {
        return (m_port_type);
    }

    void  FabricServiceGetTrunkShowMessage::setTx_bandwidth(const vector<UI64> &tx_bandwidth)
    {
        m_tx_bandwidth  =  tx_bandwidth;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getTx_bandwidth() const
    {
        return (m_tx_bandwidth);
    }

    void  FabricServiceGetTrunkShowMessage::setTx_throughput(const vector<UI64> &tx_throughput)
    {
        m_tx_throughput  =  tx_throughput;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getTx_throughput() const
    {
        return (m_tx_throughput);
    }

    void  FabricServiceGetTrunkShowMessage::setTx_percent(const vector<UI64> &tx_percent)
    {
        m_tx_percent  =  tx_percent;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getTx_percent() const
    {
        return (m_tx_percent);
    }

    void  FabricServiceGetTrunkShowMessage::setRx_bandwidth(const vector<UI64> &rx_bandwidth)
    {
        m_rx_bandwidth  =  rx_bandwidth;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getRx_bandwidth() const
    {
        return (m_rx_bandwidth);
    }

    void  FabricServiceGetTrunkShowMessage::setRx_throughput(const vector<UI64> &rx_throughput)
    {
        m_rx_throughput  =  rx_throughput;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getRx_throughput() const
    {
        return (m_rx_throughput);
    }

    void  FabricServiceGetTrunkShowMessage::setRx_percent(const vector<UI64> &rx_percent)
    {
        m_rx_percent  =  rx_percent;
    }

    vector<UI64>  FabricServiceGetTrunkShowMessage::getRx_percent() const
    {
        return (m_rx_percent);
    }

    void FabricServiceGetTrunkShowMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FabricServiceGetTrunkShowMessage::getLocal_qsfpidx() const
    {
        return (m_local_qsfpidx);
    }

    void FabricServiceGetTrunkShowMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FabricServiceGetTrunkShowMessage::getRemote_qsfpidx() const
    {
        return (m_remote_qsfpidx);
    }
}
