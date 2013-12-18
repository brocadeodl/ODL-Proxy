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
#include "FabricService/Local/FabricServiceGetislshowMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceGetislshowMessage::FabricServiceGetislshowMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETISLSHOW)
    {
    }

    FabricServiceGetislshowMessage::FabricServiceGetislshowMessage (const UI32 &domain_id,const vector<UI32> &serialport,const vector<SI32> &src_blade,const vector<SI32> &src_port,const vector<UI32> &src_index,const vector<SI32> &src_disp_type,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<UI32> &dest_index,const vector<SI32> &dest_disp_type,const vector<SI32> &interface_type,const vector<WorldWideName> &dom_wwn,const vector<UI32> &dom_id,const vector<string> &switch_name,const vector<string> &type,const vector<UI32> &speed,const vector<UI32> &bandwidth,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETISLSHOW),
        m_domain_id    (domain_id),
        m_serialport    (serialport),
        m_src_blade    (src_blade),
        m_src_port    (src_port),
        m_src_index    (src_index),
        m_src_disp_type    (src_disp_type),
        m_dest_blade    (dest_blade),
        m_dest_port    (dest_port),
        m_dest_index    (dest_index),
        m_dest_disp_type    (dest_disp_type),
        m_interface_type    (interface_type),
        m_dom_wwn    (dom_wwn),
        m_dom_id    (dom_id),
        m_switch_name    (switch_name),
        m_type    (type),
        m_speed    (speed),
        m_bandwidth    (bandwidth),
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx        (remote_qsfpidx)
    {
    }

    FabricServiceGetislshowMessage::~FabricServiceGetislshowMessage ()
    {
    }

    void  FabricServiceGetislshowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_serialport,"serialport"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_blade,"src_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_port,"src_port"));
        addSerializableAttribute (new AttributeUI32Vector(&m_src_index,"src_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_disp_type,"src_disp_type"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_blade,"dest_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_port,"dest_port"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dest_index,"dest_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_disp_type,"dest_disp_type"));
        addSerializableAttribute (new AttributeSI32Vector(&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_dom_wwn,"dom_wwn"));
        addSerializableAttribute (new AttributeUI32Vector(&m_dom_id,"dom_id"));
        addSerializableAttribute (new AttributeStringVector(&m_switch_name,"switch_name"));
        addSerializableAttribute (new AttributeStringVector(&m_type,"type"));
        addSerializableAttribute (new AttributeUI32Vector(&m_speed,"speed"));
        addSerializableAttribute (new AttributeUI32Vector(&m_bandwidth,"bandwidth"));
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));
    }

    void  FabricServiceGetislshowMessage::setDomain_id(const UI32 &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    UI32  FabricServiceGetislshowMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FabricServiceGetislshowMessage::setSerialport(const vector<UI32> &serialport)
    {
        m_serialport  =  serialport;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getSerialport() const
    {
        return (m_serialport);
    }

    void  FabricServiceGetislshowMessage::setSrc_blade(const vector<SI32> &src_blade)
    {
        m_src_blade  =  src_blade;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getSrc_blade() const
    {
        return (m_src_blade);
    }

    void  FabricServiceGetislshowMessage::setSrc_port(const vector<SI32> &src_port)
    {
        m_src_port  =  src_port;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getSrc_port() const
    {
        return (m_src_port);
    }

    void  FabricServiceGetislshowMessage::setSrc_index(const vector<UI32> &src_index)
    {
        m_src_index  =  src_index;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getSrc_index() const
    {
        return (m_src_index);
    }

    void  FabricServiceGetislshowMessage::setSrc_disp_type(const vector<SI32> &src_disp_type)
    {
        m_src_disp_type  =  src_disp_type;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getSrc_disp_type() const
    {
        return (m_src_disp_type);
    }

    void  FabricServiceGetislshowMessage::setDest_blade(const vector<SI32> &dest_blade)
    {
        m_dest_blade  =  dest_blade;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getDest_blade() const
    {
        return (m_dest_blade);
    }

    void  FabricServiceGetislshowMessage::setDest_port(const vector<SI32> &dest_port)
    {
        m_dest_port  =  dest_port;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getDest_port() const
    {
        return (m_dest_port);
    }

    void  FabricServiceGetislshowMessage::setDest_index(const vector<UI32> &dest_index)
    {
        m_dest_index  =  dest_index;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getDest_index() const
    {
        return (m_dest_index);
    }

    void  FabricServiceGetislshowMessage::setDest_disp_type(const vector<SI32> &dest_disp_type)
    {
        m_dest_disp_type  =  dest_disp_type;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getDest_disp_type() const
    {
        return (m_dest_disp_type);
    }

    void  FabricServiceGetislshowMessage::setInterface_type(const vector<SI32> &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    vector<SI32>  FabricServiceGetislshowMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  FabricServiceGetislshowMessage::setDom_wwn(const vector<WorldWideName> &dom_wwn)
    {
        m_dom_wwn  =  dom_wwn;
    }

    vector<WorldWideName>  FabricServiceGetislshowMessage::getDom_wwn() const
    {
        return (m_dom_wwn);
    }

    void  FabricServiceGetislshowMessage::setDom_id(const vector<UI32> &dom_id)
    {
        m_dom_id  =  dom_id;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getDom_id() const
    {
        return (m_dom_id);
    }

    void  FabricServiceGetislshowMessage::setSwitch_name(const vector<string> &switch_name)
    {
        m_switch_name  =  switch_name;
    }

    vector<string>  FabricServiceGetislshowMessage::getSwitch_name() const
    {
        return (m_switch_name);
    }

    void  FabricServiceGetislshowMessage::setType(const vector<string> &type)
    {
        m_type  =  type;
    }

    vector<string>  FabricServiceGetislshowMessage::getType() const
    {
        return (m_type);
    }

    void  FabricServiceGetislshowMessage::setSpeed(const vector<UI32> &speed)
    {
        m_speed  =  speed;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getSpeed() const
    {
        return (m_speed);
    }

    void  FabricServiceGetislshowMessage::setBandwidth(const vector<UI32> &bandwidth)
    {
        m_bandwidth  =  bandwidth;
    }

    vector<UI32>  FabricServiceGetislshowMessage::getBandwidth() const
    {
        return (m_bandwidth);
    }
    void FabricServiceGetislshowMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FabricServiceGetislshowMessage::getLocal_qsfpidx() const
    {
        return (m_local_qsfpidx);
    }

    void FabricServiceGetislshowMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FabricServiceGetislshowMessage::getRemote_qsfpidx() const
    {
        return (m_remote_qsfpidx);
    }
}
