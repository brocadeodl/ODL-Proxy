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

#include "FspfService/Local/FspfServiceLocalObjectManager.h"
#include "FspfService/Local/FspfServiceGetNeighborStateinfoInfoMessage.h"
#include "FspfService/Local/FspfServiceTypes.h"

namespace DcmNs
{

    FspfServiceGetNeighborStateinfoInfoMessage::FspfServiceGetNeighborStateinfoInfoMessage ()
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETNEIGHBORSTATEINFOINFO)
    {
    }

    FspfServiceGetNeighborStateinfoInfoMessage::FspfServiceGetNeighborStateinfoInfoMessage (const UI32 &input_port,const UI32 &isl_portcount,const vector<SI32> &src_port,const vector<SI32> &src_blade,const vector<SI32> &src_index,const vector<SI32> &src_dispType,const vector<SI32> &domain_id,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<SI32> &dest_index,const vector<SI32> &nbr_dispType,const vector<SI32> &interfaceType,const vector<SI32> &nbr_state,const vector<string> &nbr_state_str,const vector<SI32> &local_qsfpidx, const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETNEIGHBORSTATEINFOINFO),
        m_input_port       (input_port),
        m_isl_portcount    (isl_portcount),
        m_src_port          (src_port),
        m_src_blade         (src_blade),
        m_src_index         (src_index),
        m_src_dispType      (src_dispType),
        m_domain_id        (domain_id),
        m_dest_blade       (dest_blade),
        m_dest_port        (dest_port),
        m_dest_index       (dest_index),
        m_nbr_dispType      (nbr_dispType),
        m_interfaceType    (interfaceType),
        m_nbr_state        (nbr_state),
        m_nbr_state_str    (nbr_state_str),
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx    (remote_qsfpidx)
    {
    }

    FspfServiceGetNeighborStateinfoInfoMessage::~FspfServiceGetNeighborStateinfoInfoMessage ()
    {
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_input_port,"input_port"));
        addSerializableAttribute (new AttributeUI32(&m_isl_portcount,"isl_portcount"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_port,"src_port"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_blade,"src_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_index,"src_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_src_dispType,"src_dispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_blade,"dest_blade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_port,"dest_port"));
        addSerializableAttribute (new AttributeSI32Vector(&m_dest_index,"dest_index"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbr_dispType,"nbr_dispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_interfaceType,"interfaceType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbr_state,"nbr_state"));
        addSerializableAttribute (new AttributeStringVector(&m_nbr_state_str,"nbr_state_str"));
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));

    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setInput_port(const UI32 &input_port)
    {
        m_input_port  =  input_port;
    }

    UI32  FspfServiceGetNeighborStateinfoInfoMessage::getInput_port() const
    {
        return (m_input_port);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setIsl_portcount(const UI32 &isl_portcount)
    {
        m_isl_portcount  =  isl_portcount;
    }

    UI32  FspfServiceGetNeighborStateinfoInfoMessage::getIsl_portcount() const
    {
        return (m_isl_portcount);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setSrc_port(const vector<SI32> &srcPort)
    {
        m_src_port  =  srcPort;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getSrc_port() const
    {
        return (m_src_port);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setDomain_id(const vector<SI32> &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setDest_blade(const vector<SI32> &dest_blade)
    {
        m_dest_blade  =  dest_blade;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getDest_blade() const
    {
        return (m_dest_blade);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setDest_port(const vector<SI32> &dest_port)
    {
        m_dest_port  =  dest_port;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getDest_port() const
    {
        return (m_dest_port);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setDest_index(const vector<SI32> &dest_index)
    {
        m_dest_index  =  dest_index;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getDest_index() const
    {
        return (m_dest_index);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setNbr_state(const vector<SI32> &nbr_state)
    {
        m_nbr_state  =  nbr_state;
    }

    vector<SI32>  FspfServiceGetNeighborStateinfoInfoMessage::getNbr_state() const
    {
        return (m_nbr_state);
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setNbr_state_str(const vector<string> &nbr_state_str)
    {
        m_nbr_state_str  =  nbr_state_str;
    }

    vector<string>  FspfServiceGetNeighborStateinfoInfoMessage::getNbr_state_str() const
    {
        return (m_nbr_state_str);
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getSrc_index()  const
    {
        return (m_src_index); 
    }

    void  FspfServiceGetNeighborStateinfoInfoMessage::setSrc_index(const vector<SI32> &srcIndex)
    {
        m_src_index = srcIndex;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getSrc_blade()  const
    {
        return (m_src_blade);
    }

    void FspfServiceGetNeighborStateinfoInfoMessage::setSrc_blade(const vector<SI32> &srcBlade)
    {
        m_src_blade = srcBlade;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getSrc_dispType()  const
    {
        return (m_src_dispType);
    }

    void FspfServiceGetNeighborStateinfoInfoMessage::setSrc_dispType(const vector<SI32> &srcDispType)
    {
        m_src_dispType = srcDispType;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getNbr_dispType()  const
    {
        return (m_nbr_dispType);
    }
    void FspfServiceGetNeighborStateinfoInfoMessage::setNbr_dispType(const vector<SI32> &nbrDispType)
    {
        m_nbr_dispType = nbrDispType;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::get_interfaceType() const
    {
        return (m_interfaceType);
    }

    void FspfServiceGetNeighborStateinfoInfoMessage::set_interfaceType(const vector<SI32> &interfaceType)
    {
        m_interfaceType = interfaceType;
    }
    void FspfServiceGetNeighborStateinfoInfoMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getLocal_qsfpidx() const
    {
        return (m_local_qsfpidx);
    }

    void FspfServiceGetNeighborStateinfoInfoMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FspfServiceGetNeighborStateinfoInfoMessage::getRemote_qsfpidx() const
    {
        return (m_remote_qsfpidx);
    }
}
