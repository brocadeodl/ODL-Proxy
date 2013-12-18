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

#ifndef FABRICSERVICEGETLINKINFOMESSAGE_H
#define FABRICSERVICEGETLINKINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

    class FabricServiceGetlinkInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FabricServiceGetlinkInfoMessage ();
            FabricServiceGetlinkInfoMessage (const vector<UI32> &domain_id,const vector<UI32> &domainreachable,const vector<UI32> &version,const vector<WorldWideName> &domainwwn,const vector<UI32> &links_count,const vector<UI32> &link_id,const vector<UI32> &remote_port,const vector<UI32> &costCount,const vector<UI32> &pathCost,const vector<UI32> &pathOutPort,const vector<UI32> &link_type,const vector<UI32> &trunked,const vector<SI32> &pathOutPort_dispType,const vector<SI32> &pathOutPort_tupleBlade,const vector<SI32> &pathOutPort_tuplePort,const vector<SI32> &pathRemPort_dispType,const vector<SI32> &pathRemPort_tupleBlade,const vector<SI32> &pathRemPort_tuplePort,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx);
            virtual    ~FabricServiceGetlinkInfoMessage ();
            void setDomain_id(const vector<UI32> &domain_id);
            vector<UI32> getDomain_id()  const;
            void setDomainreachable(const vector<UI32> &domainreachable);
            vector<UI32> getDomainreachable()  const;
            void setVersion(const vector<UI32> &version);
            vector<UI32> getVersion()  const;
            void setDomainwwn(const vector<WorldWideName> &domainwwn);
            vector<WorldWideName> getDomainwwn()  const;
            void setLinks_count(const vector<UI32> &links_count);
            vector<UI32> getLinks_count()  const;
            void setLink_id(const vector<UI32> &link_id);
            vector<UI32> getLink_id()  const;
            void setRemote_port(const vector<UI32> &remote_port);
            vector<UI32> getRemote_port()  const;
            void setCostCount(const vector<UI32> &costCount);
            vector<UI32> getCostCount()  const;
            void setPathCost(const vector<UI32> &pathCost);
            vector<UI32> getPathCost()  const;
            void setPathOutPort(const vector<UI32> &pathOutPort);
            vector<UI32> getPathOutPort()  const;
            void setLink_type(const vector<UI32> &link_type);
            vector<UI32> getLink_type()  const;
            void setTrunked(const vector<UI32> &trunked);
            vector<UI32> getTrunked()  const;
            void setPathOutPort_dispType(const vector<SI32> &pathOutPort_dispType);
            vector<SI32> getPathOutPort_dispType()  const;
            void setPathOutPort_tupleBlade(const vector<SI32> &pathOutPort_tupleBlade);
            vector<SI32> getPathOutPort_tupleBlade()  const;
            void setPathOutPort_tuplePort(const vector<SI32> &pathOutPort_tuplePort);
            vector<SI32> getPathOutPort_tuplePort()  const;
            void setPathRemPort_dispType(const vector<SI32> &pathRemPort_dispType);
            vector<SI32> getPathRemPort_dispType()  const;
            void setPathRemPort_tupleBlade(const vector<SI32> &pathRemPort_tupleBlade);
            vector<SI32> getPathRemPort_tupleBlade()  const;
            void setPathRemPort_tuplePort(const vector<SI32> &pathRemPort_tuplePort);
            vector<SI32> getPathRemPort_tuplePort()  const;
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const;

// Now the data members

        private:
        protected:
        public:
            vector<UI32>  m_domain_id;
            vector<UI32>  m_domainreachable;
            vector<UI32>  m_version;
            vector<WorldWideName>  m_domainwwn;
            vector<UI32>  m_links_count;
            vector<UI32>  m_link_id;
            vector<UI32>  m_remote_port;
            vector<UI32>  m_costCount;
            vector<UI32>  m_pathCost;
            vector<UI32>  m_pathOutPort;
            vector<UI32>  m_link_type;
            vector<UI32>  m_trunked;
            vector<SI32>  m_pathOutPort_dispType;
            vector<SI32>  m_pathOutPort_tupleBlade;
            vector<SI32>  m_pathOutPort_tuplePort;
            vector<SI32>  m_pathRemPort_dispType;
            vector<SI32>  m_pathRemPort_tupleBlade;
            vector<SI32>  m_pathRemPort_tuplePort;
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FABRICSERVICEGETLINKINFOMESSAGE_H
