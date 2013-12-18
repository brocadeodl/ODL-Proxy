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

#ifndef FABRICSERVICEGETISLSHOWMESSAGE_H
#define FABRICSERVICEGETISLSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

    class FabricServiceGetislshowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FabricServiceGetislshowMessage ();
            FabricServiceGetislshowMessage (const UI32 &domain_id,const vector<UI32> &serialport,const vector<SI32> &src_blade,const vector<SI32> &src_port,const vector<UI32> &src_index,const vector<SI32> &src_disp_type,const vector<SI32> &dest_blade,const vector<SI32> &dest_port,const vector<UI32> &dest_index,const vector<SI32> &dest_disp_type,const vector<SI32> &interface_type,const vector<WorldWideName> &dom_wwn,const vector<UI32> &dom_id,const vector<string> &switch_name,const vector<string> &type,const vector<UI32> &speed,const vector<UI32> &bandwidth,const vector<SI32> &local_qsfpidx,const vector<SI32> &remote_qsfpidx);
            virtual    ~FabricServiceGetislshowMessage ();
            void setDomain_id(const UI32 &domain_id);
            UI32 getDomain_id()  const;
            void setSerialport(const vector<UI32> &serialport);
            vector<UI32> getSerialport()  const;
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
            void setDom_wwn(const vector<WorldWideName> &dom_wwn);
            vector<WorldWideName> getDom_wwn()  const;
            void setDom_id(const vector<UI32> &dom_id);
            vector<UI32> getDom_id()  const;
            void setSwitch_name(const vector<string> &switch_name);
            vector<string> getSwitch_name()  const;
            void setType(const vector<string> &type);
            vector<string> getType()  const;
            void setSpeed(const vector<UI32> &speed);
            vector<UI32> getSpeed()  const;
            void setBandwidth(const vector<UI32> &bandwidth);
            vector<UI32> getBandwidth()  const;
            void setLocal_qsfpidx(const vector<SI32> &local_qsfpidx);
            vector<SI32> getLocal_qsfpidx() const;
            void setRemote_qsfpidx(const vector<SI32> &remote_qsfpidx);
            vector<SI32> getRemote_qsfpidx() const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_domain_id;
            vector<UI32>  m_serialport;
            vector<SI32>  m_src_blade;
            vector<SI32>  m_src_port;
            vector<UI32>  m_src_index;
            vector<SI32>  m_src_disp_type;
            vector<SI32>  m_dest_blade;
            vector<SI32>  m_dest_port;
            vector<UI32>  m_dest_index;
            vector<SI32>  m_dest_disp_type;
            vector<SI32>  m_interface_type;
            vector<WorldWideName>  m_dom_wwn;
            vector<UI32>  m_dom_id;
            vector<string>  m_switch_name;
            vector<string>  m_type;
            vector<UI32>  m_speed;
            vector<UI32>  m_bandwidth;
            vector<SI32>  m_local_qsfpidx;
            vector<SI32>  m_remote_qsfpidx;
    };
}
#endif                                            //FABRICSERVICEGETISLSHOWMESSAGE_H
