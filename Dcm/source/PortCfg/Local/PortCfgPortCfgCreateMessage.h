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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef PORTCFGPORTCFGCREATEMESSAGE_H
#define PORTCFGPORTCFGCREATEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgPortCfgCreateMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgPortCfgCreateMessage ();
            PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete);
			PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete, const int &switch_port, const string &switchPortType, const int &ifIndex);
			PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete, const int &switch_port, const string &portType, const string &switchPortType, const int &slot, const int &phyPort, const int &fcoePort, const int &vlan, const int &ifIndex);
            virtual    ~PortCfgPortCfgCreateMessage ();
            void setId(const string &id);
            string getId()  const;
            void setIsIdCompete(const bool &isIdCompete);
            bool getIsIdCompete()  const;
			void setSwitchPort(int switch_port) { m_switch_port = switch_port; }
			int  getSwitchPort(void) { return m_switch_port; }
			void setPortType(const string &portType);
			string getPortType()  const;
			void setSwitchPortType(const string &switchPortType);
			string getSwitchPortType()  const;
			void setDispId(const string &dispId);
			string getDispId()  const;
			void setSlot(const int &slot);
			int  getSlot() const;
			void setPhyPort(const int &phyPort);
			int  getPhyPort() const;
			void setFcoePort(const int &fcoePort);
			int  getFcoePort() const;
			void setVlan(const int &vlan);
			int  getVlan() const;
			void setIfIndex(const int &ifIndex);
			int  getIfIndex() const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool    m_isIdCompete;
			SI32 	m_switch_port;
			string  m_portType;
			string  m_switchPortType;
			string  m_dispId;
			int   m_slot;
			int   m_phyPort;
			int   m_fcoePort;
			int   m_vlan;
			int   m_ifIndex;
    };
}
#endif                                            //PORTCFGPORTCFGCREATEMESSAGE_H
