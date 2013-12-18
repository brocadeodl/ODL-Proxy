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

#ifndef PORTCFGPORTCFGPORTTRANSLATEMESSAGE_H
#define PORTCFGPORTCFGPORTTRANSLATEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
// ************* PortCfgPortCfgPortTranslateMessage ************
    class PortCfgPortCfgPortTranslateMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgPortCfgPortTranslateMessage ();
            PortCfgPortCfgPortTranslateMessage (const string &id,const SI32 &switchPort, const SI32 &rbridge);
            PortCfgPortCfgPortTranslateMessage (const SI32 &switchPort,const SI32 &rbridge);
            PortCfgPortCfgPortTranslateMessage (const string &id,const string &type);
            PortCfgPortCfgPortTranslateMessage (const string &id,const string &type,const string &phyPortId,const string &phyPortType,const SI32 &switchPort,const SI32 &rbridge);
            virtual    ~PortCfgPortCfgPortTranslateMessage ();
            void setId(const string &id);
            string getId()  const;
            void setType(const string &type);
            string getType()  const;
            void setPhyPortId(const string &phyPortId);
            string getPhyPortId()  const;
            void setPhyPortType(const string &phyPortType);
            string getPhyPortType()  const;
            void setSwitchPort(const SI32 &switchPort);
            SI32 getSwitchPort()  const;
			void setRbridge(const SI32 &rbridge);
            SI32 getRbridge()  const;

			// Additional message members - not from Breeze
            void reset();
            bool isDISet()  const;
            bool isIdSet()  const;
            bool isPhyPortSet()  const;

// Now the data members
        private:
        protected:
		public:
            string  m_id;
            string  m_type;
            string  m_phyPortId;
            string  m_phyPortType;
            SI32  m_switchPort;
			SI32  m_rbridge;
    };

}
#endif 
