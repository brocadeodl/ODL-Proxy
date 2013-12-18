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

#ifndef FSPFSERVICEGETTOPOLOGYINFOMESSAGE_H
#define FSPFSERVICEGETTOPOLOGYINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FspfServiceGetTopologyInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FspfServiceGetTopologyInfoMessage ();
            FspfServiceGetTopologyInfoMessage (const SI16 &domainCount,const UI32 &srcDomainId,const vector<UI32> &destDomainId,const vector<UI32> &lowestPathCost,const vector<string> &destDomainName,const vector<UI32> &noOfHopsFromSrc,const vector<UI32> &pathCount,const vector<UI32> &bitsPerSec,const vector<UI32> &pathFlags,const vector<UI32> &pathIngressPortCount,const vector<UI32> &pathIngressPortBM,const vector<UI32> &pathCost,const vector<UI32> &pathCurrentBWAlloc,const vector<UI32> &pathBWUtilization,const vector<UI32> &pathTotalBW,const vector<SI32> &srcBlade,const vector<SI32> &srcPort,const vector<SI32> &srcIndex,const vector<SI32> &srcDispType,const vector<SI32> &nbrDomain,const vector<SI32> &nbrBlade,const vector<SI32> &nbrPort,const vector<SI32> &nbrIndex,const vector<SI32> &nbrDispType,const vector<SI32> &interfaceType,const vector<UI32> &trunkPort,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx);
            virtual    ~FspfServiceGetTopologyInfoMessage ();
            void setDomainCount(const SI16 &domainCount);
            SI16 getDomainCount()  const;
            void setSrcDomainId(const UI32 &srcDomainId);
            UI32 getSrcDomainId()  const;
            void setDestDomainId(const vector<UI32> &destDomainId);
            vector<UI32> getDestDomainId()  const;
            void setLowestPathCost(const vector<UI32> &lowestPathCost);
            vector<UI32> getLowestPathCost()  const;
            void setDestDomainName(const vector<string> &destDomainName);
            vector<string> getDestDomainName()  const;
            void setNoOfHopsFromSrc(const vector<UI32> &noOfHopsFromSrc);
            vector<UI32> getNoOfHopsFromSrc()  const;
            void setPathCount(const vector<UI32> &pathCount);
            vector<UI32> getPathCount()  const;
            void setBitsPerSec(const vector<UI32> &bitsPerSec);
            vector<UI32> getBitsPerSec()  const;
            void setPathFlags(const vector<UI32> &pathFlags);
            vector<UI32> getPathFlags()  const;
            void setPathIngressPortCount(const vector<UI32> &pathIngressPortCount);
            vector<UI32> getPathIngressPortCount()  const;
            void setPathIngressPortBM(const vector<UI32> &pathIngressPortBM);
            vector<UI32> getPathIngressPortBM()  const;
            void setPathCost(const vector<UI32> &pathCost);
            vector<UI32> getPathCost()  const;
            void setPathCurrentBWAlloc(const vector<UI32> &pathCurrentBWAlloc);
            vector<UI32> getPathCurrentBWAlloc()  const;
            void setPathBWUtilization(const vector<UI32> &pathBWUtilization);
            vector<UI32> getPathBWUtilization()  const;
            void setPathTotalBW(const vector<UI32> &pathTotalBW);
            vector<UI32> getPathTotalBW()  const;
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
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const;

// Now the data members

        private:
        protected:
        public:
            SI16  m_domainCount;
            UI32  m_srcDomainId;
            vector<UI32>  m_destDomainId;
            vector<UI32>  m_lowestPathCost;
            vector<string>  m_destDomainName;
            vector<UI32>  m_noOfHopsFromSrc;
            vector<UI32>  m_pathCount;
            vector<UI32>  m_bitsPerSec;
            vector<UI32>  m_pathFlags;
            vector<UI32>  m_pathIngressPortCount;
            vector<UI32>  m_pathIngressPortBM;
            vector<UI32>  m_pathCost;
            vector<UI32>  m_pathCurrentBWAlloc;
            vector<UI32>  m_pathBWUtilization;
            vector<UI32>  m_pathTotalBW;
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
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FSPFSERVICEGETTOPOLOGYINFOMESSAGE_H
