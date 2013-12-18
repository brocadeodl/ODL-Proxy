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

#include "FspfService/Local/FspfServiceLocalObjectManager.h"
#include "FspfService/Local/FspfServiceGetBroadcastInfoMessage.h"
#include "FspfService/Local/FspfServiceTypes.h"

namespace DcmNs
{

    FspfServiceGetBroadcastInfoMessage::FspfServiceGetBroadcastInfoMessage ()
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETBROADCASTINFO)
    {
    }

    FspfServiceGetBroadcastInfoMessage::FspfServiceGetBroadcastInfoMessage (const UI32 &domain_id,const vector<UI32> &port_isl,const vector<UI32> &bcast_route_avail,const UI32 &bcastRootDomain,const UI32 &bcastRootPriority,const MacAddress &bcastRootMacAddr,const UI32 &bcastRootIPAddr,const string &bcastRootWwn,const string &bcastRootSwitchName,const UI32 &bcastLocalPriority,const vector<SI32> &srcBlade,const vector<SI32> &srcPort,const vector<SI32> &srcIndex,const vector<SI32> &srcDispType,const vector<SI32> &nbrDomain,const vector<SI32> &nbrBlade,const vector<SI32> &nbrPort,const vector<SI32> &nbrIndex,const vector<SI32> &nbrDispType,const vector<SI32> &interfaceType,const vector<UI32> &trunkPort,const vector<UI32> &bandwidth,const vector<SI32> &local_qsfpidx, const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETBROADCASTINFO),
        m_domain_id    (domain_id),
        m_port_isl    (port_isl),
        m_bcast_route_avail    (bcast_route_avail),
        m_bcastRootDomain    (bcastRootDomain),
        m_bcastRootPriority    (bcastRootPriority),
        m_bcastRootMacAddr    (bcastRootMacAddr),
        m_bcastRootIPAddr    (bcastRootIPAddr),
        m_bcastRootWwn    (bcastRootWwn),
        m_bcastRootSwitchName    (bcastRootSwitchName),
        m_bcastLocalPriority    (bcastLocalPriority),
        m_srcBlade    (srcBlade),
        m_srcPort    (srcPort),
        m_srcIndex    (srcIndex),
        m_srcDispType    (srcDispType),
        m_nbrDomain    (nbrDomain),
        m_nbrBlade    (nbrBlade),
        m_nbrPort    (nbrPort),
        m_nbrIndex    (nbrIndex),
        m_nbrDispType    (nbrDispType),
        m_interfaceType    (interfaceType),
        m_trunkPort    (trunkPort),
        m_bandwidth    (bandwidth),
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx    (remote_qsfpidx)
    {
    }

    FspfServiceGetBroadcastInfoMessage::~FspfServiceGetBroadcastInfoMessage ()
    {
    }

    void  FspfServiceGetBroadcastInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_port_isl,"port_isl"));
        addSerializableAttribute (new AttributeUI32Vector(&m_bcast_route_avail,"bcast_route_avail"));
        addSerializableAttribute (new AttributeUI32(&m_bcastRootDomain,"bcastRootDomain"));
        addSerializableAttribute (new AttributeUI32(&m_bcastRootPriority,"bcastRootPriority"));
        addSerializableAttribute (new AttributeMacAddress(&m_bcastRootMacAddr,"bcastRootMacAddr"));
        addSerializableAttribute (new AttributeUI32(&m_bcastRootIPAddr,"bcastRootIPAddr"));
        addSerializableAttribute (new AttributeString(&m_bcastRootWwn,"bcastRootWwn"));
        addSerializableAttribute (new AttributeString(&m_bcastRootSwitchName,"bcastRootSwitchName"));
        addSerializableAttribute (new AttributeUI32(&m_bcastLocalPriority,"bcastLocalPriority"));
        addSerializableAttribute (new AttributeSI32Vector(&m_srcBlade,"srcBlade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_srcPort,"srcPort"));
        addSerializableAttribute (new AttributeSI32Vector(&m_srcIndex,"srcIndex"));
        addSerializableAttribute (new AttributeSI32Vector(&m_srcDispType,"srcDispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbrDomain,"nbrDomain"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbrBlade,"nbrBlade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbrPort,"nbrPort"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbrIndex,"nbrIndex"));
        addSerializableAttribute (new AttributeSI32Vector(&m_nbrDispType,"nbrDispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_interfaceType,"interfaceType"));
        addSerializableAttribute (new AttributeUI32Vector(&m_trunkPort,"trunkPort"));
        addSerializableAttribute (new AttributeUI32Vector(&m_bandwidth,"bandwidth"));
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));

    }

    void  FspfServiceGetBroadcastInfoMessage::setDomain_id(const UI32 &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    UI32  FspfServiceGetBroadcastInfoMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FspfServiceGetBroadcastInfoMessage::setPort_isl(const vector<UI32> &port_isl)
    {
        m_port_isl  =  port_isl;
    }

    vector<UI32>  FspfServiceGetBroadcastInfoMessage::getPort_isl() const
    {
        return (m_port_isl);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcast_route_avail(const vector<UI32> &bcast_route_avail)
    {
        m_bcast_route_avail  =  bcast_route_avail;
    }

    vector<UI32>  FspfServiceGetBroadcastInfoMessage::getBcast_route_avail() const
    {
        return (m_bcast_route_avail);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootDomain(const UI32 &bcastRootDomain)
    {
        m_bcastRootDomain  =  bcastRootDomain;
    }

    UI32  FspfServiceGetBroadcastInfoMessage::getBcastRootDomain() const
    {
        return (m_bcastRootDomain);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootPriority(const UI32 &bcastRootPriority)
    {
        m_bcastRootPriority  =  bcastRootPriority;
    }

    UI32  FspfServiceGetBroadcastInfoMessage::getBcastRootPriority() const
    {
        return (m_bcastRootPriority);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootMacAddr(const MacAddress &bcastRootMacAddr)
    {
        m_bcastRootMacAddr  =  bcastRootMacAddr;
    }

    MacAddress  FspfServiceGetBroadcastInfoMessage::getBcastRootMacAddr() const
    {
        return (m_bcastRootMacAddr);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootIPAddr(const UI32 &bcastRootIPAddr)
    {
        m_bcastRootIPAddr  =  bcastRootIPAddr;
    }

    UI32  FspfServiceGetBroadcastInfoMessage::getBcastRootIPAddr() const
    {
        return (m_bcastRootIPAddr);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootWwn(const string &bcastRootWwn)
    {
        m_bcastRootWwn  =  bcastRootWwn;
    }

    string  FspfServiceGetBroadcastInfoMessage::getBcastRootWwn() const
    {
        return (m_bcastRootWwn);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastRootSwitchName(const string &bcastRootSwitchName)
    {
        m_bcastRootSwitchName  =  bcastRootSwitchName;
    }

    string  FspfServiceGetBroadcastInfoMessage::getBcastRootSwitchName() const
    {
        return (m_bcastRootSwitchName);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBcastLocalPriority(const UI32 &bcastLocalPriority)
    {
        m_bcastLocalPriority  =  bcastLocalPriority;
    }

    UI32  FspfServiceGetBroadcastInfoMessage::getBcastLocalPriority() const
    {
        return (m_bcastLocalPriority);
    }

    void  FspfServiceGetBroadcastInfoMessage::setSrcBlade(const vector<SI32> &srcBlade)
    {
        m_srcBlade  =  srcBlade;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getSrcBlade() const
    {
        return (m_srcBlade);
    }

    void  FspfServiceGetBroadcastInfoMessage::setSrcPort(const vector<SI32> &srcPort)
    {
        m_srcPort  =  srcPort;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getSrcPort() const
    {
        return (m_srcPort);
    }

    void  FspfServiceGetBroadcastInfoMessage::setSrcIndex(const vector<SI32> &srcIndex)
    {
        m_srcIndex  =  srcIndex;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getSrcIndex() const
    {
        return (m_srcIndex);
    }

    void  FspfServiceGetBroadcastInfoMessage::setSrcDispType(const vector<SI32> &srcDispType)
    {
        m_srcDispType  =  srcDispType;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getSrcDispType() const
    {
        return (m_srcDispType);
    }

    void  FspfServiceGetBroadcastInfoMessage::setNbrDomain(const vector<SI32> &nbrDomain)
    {
        m_nbrDomain  =  nbrDomain;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getNbrDomain() const
    {
        return (m_nbrDomain);
    }

    void  FspfServiceGetBroadcastInfoMessage::setNbrBlade(const vector<SI32> &nbrBlade)
    {
        m_nbrBlade  =  nbrBlade;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getNbrBlade() const
    {
        return (m_nbrBlade);
    }

    void  FspfServiceGetBroadcastInfoMessage::setNbrPort(const vector<SI32> &nbrPort)
    {
        m_nbrPort  =  nbrPort;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getNbrPort() const
    {
        return (m_nbrPort);
    }

    void  FspfServiceGetBroadcastInfoMessage::setNbrIndex(const vector<SI32> &nbrIndex)
    {
        m_nbrIndex  =  nbrIndex;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getNbrIndex() const
    {
        return (m_nbrIndex);
    }

    void  FspfServiceGetBroadcastInfoMessage::setNbrDispType(const vector<SI32> &nbrDispType)
    {
        m_nbrDispType  =  nbrDispType;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getNbrDispType() const
    {
        return (m_nbrDispType);
    }

    void  FspfServiceGetBroadcastInfoMessage::setInterfaceType(const vector<SI32> &interfaceType)
    {
        m_interfaceType  =  interfaceType;
    }

    vector<SI32>  FspfServiceGetBroadcastInfoMessage::getInterfaceType() const
    {
        return (m_interfaceType);
    }

    void  FspfServiceGetBroadcastInfoMessage::setTrunkPort(const vector<UI32> &trunkPort)
    {
        m_trunkPort  =  trunkPort;
    }

    vector<UI32>  FspfServiceGetBroadcastInfoMessage::getTrunkPort() const
    {
        return (m_trunkPort);
    }

    void  FspfServiceGetBroadcastInfoMessage::setBandwidth(const vector<UI32> &bandwidth)
    {
        m_bandwidth  =  bandwidth;
    }

    vector<UI32>  FspfServiceGetBroadcastInfoMessage::getBandwidth() const
    {
        return (m_bandwidth);
    }

    void FspfServiceGetBroadcastInfoMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FspfServiceGetBroadcastInfoMessage::getLocal_qsfpidx() const
    {
        return (m_local_qsfpidx);
    }

    void FspfServiceGetBroadcastInfoMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FspfServiceGetBroadcastInfoMessage::getRemote_qsfpidx() const
    {
        return (m_remote_qsfpidx);
    }
}
