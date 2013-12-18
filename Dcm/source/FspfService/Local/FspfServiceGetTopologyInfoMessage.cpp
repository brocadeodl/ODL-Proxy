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
#include "FspfService/Local/FspfServiceGetTopologyInfoMessage.h"
#include "FspfService/Local/FspfServiceTypes.h"

namespace DcmNs
{

    FspfServiceGetTopologyInfoMessage::FspfServiceGetTopologyInfoMessage ()
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETTOPOLOGYINFO)
    {
    }

    FspfServiceGetTopologyInfoMessage::FspfServiceGetTopologyInfoMessage (const SI16 &domainCount,const UI32 &srcDomainId,const vector<UI32> &destDomainId,const vector<UI32> &lowestPathCost,const vector<string> &destDomainName,const vector<UI32> &noOfHopsFromSrc,const vector<UI32> &pathCount,const vector<UI32> &bitsPerSec,const vector<UI32> &pathFlags,const vector<UI32> &pathIngressPortCount,const vector<UI32> &pathIngressPortBM,const vector<UI32> &pathCost,const vector<UI32> &pathCurrentBWAlloc,const vector<UI32> &pathBWUtilization,const vector<UI32> &pathTotalBW,const vector<SI32> &srcBlade,const vector<SI32> &srcPort,const vector<SI32> &srcIndex,const vector<SI32> &srcDispType,const vector<SI32> &nbrDomain,const vector<SI32> &nbrBlade,const vector<SI32> &nbrPort,const vector<SI32> &nbrIndex,const vector<SI32> &nbrDispType,const vector<SI32> &interfaceType,const vector<UI32> &trunkPort,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx)
        : PrismMessage (FspfServiceLocalObjectManager::getPrismServiceId (), FSPFSERVICEGETTOPOLOGYINFO),
        m_domainCount    (domainCount),
        m_srcDomainId    (srcDomainId),
        m_destDomainId    (destDomainId),
        m_lowestPathCost    (lowestPathCost),
        m_destDomainName    (destDomainName),
        m_noOfHopsFromSrc    (noOfHopsFromSrc),
        m_pathCount    (pathCount),
        m_bitsPerSec    (bitsPerSec),
        m_pathFlags    (pathFlags),
        m_pathIngressPortCount    (pathIngressPortCount),
        m_pathIngressPortBM    (pathIngressPortBM),
        m_pathCost    (pathCost),
        m_pathCurrentBWAlloc    (pathCurrentBWAlloc),
        m_pathBWUtilization    (pathBWUtilization),
        m_pathTotalBW    (pathTotalBW),
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
        m_local_qsfpidx    (local_qsfpidx),
        m_remote_qsfpidx    (remote_qsfpidx)
    {
    }

    FspfServiceGetTopologyInfoMessage::~FspfServiceGetTopologyInfoMessage ()
    {
    }

    void  FspfServiceGetTopologyInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI16(&m_domainCount,"domainCount"));
        addSerializableAttribute (new AttributeUI32(&m_srcDomainId,"srcDomainId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_destDomainId,"destDomainId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_lowestPathCost,"lowestPathCost"));
        addSerializableAttribute (new AttributeStringVector(&m_destDomainName,"destDomainName"));
        addSerializableAttribute (new AttributeUI32Vector(&m_noOfHopsFromSrc,"noOfHopsFromSrc"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathCount,"pathCount"));
        addSerializableAttribute (new AttributeUI32Vector(&m_bitsPerSec,"bitsPerSec"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathFlags,"pathFlags"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathIngressPortCount,"pathIngressPortCount"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathIngressPortBM,"pathIngressPortBM"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathCost,"pathCost"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathCurrentBWAlloc,"pathCurrentBWAlloc"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathBWUtilization,"pathBWUtilization"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pathTotalBW,"pathTotalBW"));
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
        addSerializableAttribute (new AttributeSI32Vector(&m_local_qsfpidx,"local_qsfpidx"));
        addSerializableAttribute (new AttributeSI32Vector(&m_remote_qsfpidx,"remote_qsfpidx"));

    }

    void  FspfServiceGetTopologyInfoMessage::setDomainCount(const SI16 &domainCount)
    {
        m_domainCount  =  domainCount;
    }

    SI16  FspfServiceGetTopologyInfoMessage::getDomainCount() const
    {
        return (m_domainCount);
    }

    void  FspfServiceGetTopologyInfoMessage::setSrcDomainId(const UI32 &srcDomainId)
    {
        m_srcDomainId  =  srcDomainId;
    }

    UI32  FspfServiceGetTopologyInfoMessage::getSrcDomainId() const
    {
        return (m_srcDomainId);
    }

    void  FspfServiceGetTopologyInfoMessage::setDestDomainId(const vector<UI32> &destDomainId)
    {
        m_destDomainId  =  destDomainId;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getDestDomainId() const
    {
        return (m_destDomainId);
    }

    void  FspfServiceGetTopologyInfoMessage::setLowestPathCost(const vector<UI32> &lowestPathCost)
    {
        m_lowestPathCost  =  lowestPathCost;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getLowestPathCost() const
    {
        return (m_lowestPathCost);
    }

    void  FspfServiceGetTopologyInfoMessage::setDestDomainName(const vector<string> &destDomainName)
    {
        m_destDomainName  =  destDomainName;
    }

    vector<string>  FspfServiceGetTopologyInfoMessage::getDestDomainName() const
    {
        return (m_destDomainName);
    }

    void  FspfServiceGetTopologyInfoMessage::setNoOfHopsFromSrc(const vector<UI32> &noOfHopsFromSrc)
    {
        m_noOfHopsFromSrc  =  noOfHopsFromSrc;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getNoOfHopsFromSrc() const
    {
        return (m_noOfHopsFromSrc);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathCount(const vector<UI32> &pathCount)
    {
        m_pathCount  =  pathCount;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathCount() const
    {
        return (m_pathCount);
    }

    void  FspfServiceGetTopologyInfoMessage::setBitsPerSec(const vector<UI32> &bitsPerSec)
    {
        m_bitsPerSec  =  bitsPerSec;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getBitsPerSec() const
    {
        return (m_bitsPerSec);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathFlags(const vector<UI32> &pathFlags)
    {
        m_pathFlags  =  pathFlags;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathFlags() const
    {
        return (m_pathFlags);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathIngressPortCount(const vector<UI32> &pathIngressPortCount)
    {
        m_pathIngressPortCount  =  pathIngressPortCount;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathIngressPortCount() const
    {
        return (m_pathIngressPortCount);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathIngressPortBM(const vector<UI32> &pathIngressPortBM)
    {
        m_pathIngressPortBM  =  pathIngressPortBM;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathIngressPortBM() const
    {
        return (m_pathIngressPortBM);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathCost(const vector<UI32> &pathCost)
    {
        m_pathCost  =  pathCost;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathCost() const
    {
        return (m_pathCost);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathCurrentBWAlloc(const vector<UI32> &pathCurrentBWAlloc)
    {
        m_pathCurrentBWAlloc  =  pathCurrentBWAlloc;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathCurrentBWAlloc() const
    {
        return (m_pathCurrentBWAlloc);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathBWUtilization(const vector<UI32> &pathBWUtilization)
    {
        m_pathBWUtilization  =  pathBWUtilization;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathBWUtilization() const
    {
        return (m_pathBWUtilization);
    }

    void  FspfServiceGetTopologyInfoMessage::setPathTotalBW(const vector<UI32> &pathTotalBW)
    {
        m_pathTotalBW  =  pathTotalBW;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getPathTotalBW() const
    {
        return (m_pathTotalBW);
    }

    void  FspfServiceGetTopologyInfoMessage::setSrcBlade(const vector<SI32> &srcBlade)
    {
        m_srcBlade  =  srcBlade;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getSrcBlade() const
    {
        return (m_srcBlade);
    }

    void  FspfServiceGetTopologyInfoMessage::setSrcPort(const vector<SI32> &srcPort)
    {
        m_srcPort  =  srcPort;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getSrcPort() const
    {
        return (m_srcPort);
    }

    void  FspfServiceGetTopologyInfoMessage::setSrcIndex(const vector<SI32> &srcIndex)
    {
        m_srcIndex  =  srcIndex;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getSrcIndex() const
    {
        return (m_srcIndex);
    }

    void  FspfServiceGetTopologyInfoMessage::setSrcDispType(const vector<SI32> &srcDispType)
    {
        m_srcDispType  =  srcDispType;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getSrcDispType() const
    {
        return (m_srcDispType);
    }

    void  FspfServiceGetTopologyInfoMessage::setNbrDomain(const vector<SI32> &nbrDomain)
    {
        m_nbrDomain  =  nbrDomain;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getNbrDomain() const
    {
        return (m_nbrDomain);
    }

    void  FspfServiceGetTopologyInfoMessage::setNbrBlade(const vector<SI32> &nbrBlade)
    {
        m_nbrBlade  =  nbrBlade;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getNbrBlade() const
    {
        return (m_nbrBlade);
    }

    void  FspfServiceGetTopologyInfoMessage::setNbrPort(const vector<SI32> &nbrPort)
    {
        m_nbrPort  =  nbrPort;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getNbrPort() const
    {
        return (m_nbrPort);
    }

    void  FspfServiceGetTopologyInfoMessage::setNbrIndex(const vector<SI32> &nbrIndex)
    {
        m_nbrIndex  = nbrIndex;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getNbrIndex() const
    {
        return (m_nbrIndex);
    }

    void  FspfServiceGetTopologyInfoMessage::setNbrDispType(const vector<SI32> &nbrDispType)
    {
        m_nbrDispType  =  nbrDispType;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getNbrDispType() const
    {
        return (m_nbrDispType);
    }

    void  FspfServiceGetTopologyInfoMessage::setInterfaceType(const vector<SI32> &interfaceType)
    {
        m_interfaceType  =  interfaceType;
    }

    vector<SI32>  FspfServiceGetTopologyInfoMessage::getInterfaceType() const
    {
        return (m_interfaceType);
    }

    void  FspfServiceGetTopologyInfoMessage::setTrunkPort(const vector<UI32> &trunkPort)
    {
        m_trunkPort  =  trunkPort;
    }

    vector<UI32>  FspfServiceGetTopologyInfoMessage::getTrunkPort() const
    {
        return (m_trunkPort);
    }
    void FspfServiceGetTopologyInfoMessage::setLocal_qsfpidx(const vector<SI32> &local_qsfpidx)
    {
        m_local_qsfpidx = local_qsfpidx;
    }

    vector<SI32> FspfServiceGetTopologyInfoMessage::getLocal_qsfpidx() const
    {
       return (m_local_qsfpidx);
    }

    void FspfServiceGetTopologyInfoMessage::setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx)
    {
        m_remote_qsfpidx = remote_qsfpidx;
    }

    vector<SI32> FspfServiceGetTopologyInfoMessage::getRemote_qsfpidx() const
    {
         return (m_remote_qsfpidx);
    }
}
