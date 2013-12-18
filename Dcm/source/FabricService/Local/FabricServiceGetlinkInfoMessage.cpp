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
#include "FabricService/Local/FabricServiceGetlinkInfoMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceGetlinkInfoMessage::FabricServiceGetlinkInfoMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETLINKINFO)
    {
    }

    FabricServiceGetlinkInfoMessage::FabricServiceGetlinkInfoMessage (const vector<UI32> &domain_id,const vector<UI32> &domainreachable,const vector<UI32> &version,const vector<WorldWideName> &domainwwn,const vector<UI32> &links_count,const vector<UI32> &link_id,const vector<UI32> &remote_port,const vector<UI32> &costCount,const vector<UI32> &pathCost,const vector<UI32> &pathOutPort,const vector<UI32> &link_type,const vector<UI32> &trunked,const vector<SI32> &pathOutPort_dispType,const vector<SI32> &pathOutPort_tupleBlade,const vector<SI32> &pathOutPort_tuplePort,const vector<SI32> &pathRemPort_dispType,const vector<SI32> &pathRemPort_tupleBlade,const vector<SI32> &pathRemPort_tuplePort,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETLINKINFO),
        m_domain_id    (domain_id),
        m_domainreachable    (domainreachable),
        m_version    (version),
        m_domainwwn    (domainwwn),
        m_links_count    (links_count),
        m_link_id    (link_id),
        m_remote_port    (remote_port),
        m_costCount    (costCount),
        m_pathCost    (pathCost),
        m_pathOutPort    (pathOutPort),
        m_link_type    (link_type),
        m_trunked    (trunked),
        m_pathOutPort_dispType    (pathOutPort_dispType),
        m_pathOutPort_tupleBlade    (pathOutPort_tupleBlade),
        m_pathOutPort_tuplePort    (pathOutPort_tuplePort),
        m_pathRemPort_dispType    (pathRemPort_dispType),
        m_pathRemPort_tupleBlade    (pathRemPort_tupleBlade),
        m_pathRemPort_tuplePort    (pathRemPort_tuplePort),
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx    (remote_qsfpidx)
    {
    }

    FabricServiceGetlinkInfoMessage::~FabricServiceGetlinkInfoMessage ()
    {
    }

    void  FabricServiceGetlinkInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32Vector(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_domainreachable,"domainreachable"));
        addSerializableAttribute (new AttributeUI32Vector(&m_version,"version"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_domainwwn,"domainwwn"));
        addSerializableAttribute (new AttributeUI32Vector(&m_links_count,"links_count"));
        addSerializableAttribute (new AttributeUI32Vector(&m_link_id,"link_id"));
        addSerializableAttribute (new AttributeUI32Vector(&m_remote_port,"remote_port"));
        addSerializableAttribute (new AttributeUI32Vector(&m_costCount,"costCount"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathCost,"pathCost"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathOutPort,"pathOutPort"));
        addSerializableAttribute (new AttributeUI32Vector(&m_link_type,"link_type"));
        addSerializableAttribute (new AttributeUI32Vector(&m_trunked,"trunked"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathOutPort_dispType,"pathOutPort_dispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathOutPort_tupleBlade,"pathOutPort_tupleBlade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathOutPort_tuplePort,"pathOutPort_tuplePort"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathRemPort_dispType,"pathRemPort_dispType"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathRemPort_tupleBlade,"pathRemPort_tupleBlade"));
        addSerializableAttribute (new AttributeSI32Vector(&m_pathRemPort_tuplePort,"pathRemPort_tuplePort"));
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));
 
    }

    void  FabricServiceGetlinkInfoMessage::setDomain_id(const vector<UI32> &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FabricServiceGetlinkInfoMessage::setDomainreachable(const vector<UI32> &domainreachable)
    {
        m_domainreachable  =  domainreachable;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getDomainreachable() const
    {
        return (m_domainreachable);
    }

    void  FabricServiceGetlinkInfoMessage::setVersion(const vector<UI32> &version)
    {
        m_version  =  version;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getVersion() const
    {
        return (m_version);
    }

    void  FabricServiceGetlinkInfoMessage::setDomainwwn(const vector<WorldWideName> &domainwwn)
    {
        m_domainwwn  =  domainwwn;
    }

    vector<WorldWideName>  FabricServiceGetlinkInfoMessage::getDomainwwn() const
    {
        return (m_domainwwn);
    }

    void  FabricServiceGetlinkInfoMessage::setLinks_count(const vector<UI32> &links_count)
    {
        m_links_count  =  links_count;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getLinks_count() const
    {
        return (m_links_count);
    }

    void  FabricServiceGetlinkInfoMessage::setLink_id(const vector<UI32> &link_id)
    {
        m_link_id  =  link_id;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getLink_id() const
    {
        return (m_link_id);
    }

    void  FabricServiceGetlinkInfoMessage::setRemote_port(const vector<UI32> &remote_port)
    {
        m_remote_port  =  remote_port;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getRemote_port() const
    {
        return (m_remote_port);
    }

    void  FabricServiceGetlinkInfoMessage::setCostCount(const vector<UI32> &costCount)
    {
        m_costCount  =  costCount;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getCostCount() const
    {
        return (m_costCount);
    }

    void  FabricServiceGetlinkInfoMessage::setPathCost(const vector<UI32> &pathCost)
    {
        m_pathCost  =  pathCost;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getPathCost() const
    {
        return (m_pathCost);
    }

    void  FabricServiceGetlinkInfoMessage::setPathOutPort(const vector<UI32> &pathOutPort)
    {
        m_pathOutPort  =  pathOutPort;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getPathOutPort() const
    {
        return (m_pathOutPort);
    }

    void  FabricServiceGetlinkInfoMessage::setLink_type(const vector<UI32> &link_type)
    {
        m_link_type  =  link_type;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getLink_type() const
    {
        return (m_link_type);
    }

    void  FabricServiceGetlinkInfoMessage::setTrunked(const vector<UI32> &trunked)
    {
        m_trunked  =  trunked;
    }

    vector<UI32>  FabricServiceGetlinkInfoMessage::getTrunked() const
    {
        return (m_trunked);
    }

    void  FabricServiceGetlinkInfoMessage::setPathOutPort_dispType(const vector<SI32> &pathOutPort_dispType)
    {
        m_pathOutPort_dispType  =  pathOutPort_dispType;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathOutPort_dispType() const
    {
        return (m_pathOutPort_dispType);
    }

    void  FabricServiceGetlinkInfoMessage::setPathOutPort_tupleBlade(const vector<SI32> &pathOutPort_tupleBlade)
    {
        m_pathOutPort_tupleBlade  =  pathOutPort_tupleBlade;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathOutPort_tupleBlade() const
    {
        return (m_pathOutPort_tupleBlade);
    }

    void  FabricServiceGetlinkInfoMessage::setPathOutPort_tuplePort(const vector<SI32> &pathOutPort_tuplePort)
    {
        m_pathOutPort_tuplePort  =  pathOutPort_tuplePort;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathOutPort_tuplePort() const
    {
        return (m_pathOutPort_tuplePort);
    }
    void  FabricServiceGetlinkInfoMessage::setPathRemPort_dispType(const vector<SI32> &pathRemPort_dispType)
    {
        m_pathRemPort_dispType  =  pathRemPort_dispType;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathRemPort_dispType() const
    {
        return (m_pathRemPort_dispType);
    }

    void  FabricServiceGetlinkInfoMessage::setPathRemPort_tupleBlade(const vector<SI32> &pathRemPort_tupleBlade)
    {
        m_pathRemPort_tupleBlade  =  pathRemPort_tupleBlade;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathRemPort_tupleBlade() const
    {
        return (m_pathRemPort_tupleBlade);
    }

    void  FabricServiceGetlinkInfoMessage::setPathRemPort_tuplePort(const vector<SI32> &pathRemPort_tuplePort)
    {
        m_pathRemPort_tuplePort  =  pathRemPort_tuplePort;
    }

    vector<SI32>  FabricServiceGetlinkInfoMessage::getPathRemPort_tuplePort() const
    {
        return (m_pathRemPort_tuplePort);
    }

    void FabricServiceGetlinkInfoMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FabricServiceGetlinkInfoMessage::getLocal_qsfpidx() const
    {
        return (m_local_qsfpidx);
    }

    void FabricServiceGetlinkInfoMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FabricServiceGetlinkInfoMessage::getRemote_qsfpidx() const
    {
        return (m_remote_qsfpidx);
    }
}
