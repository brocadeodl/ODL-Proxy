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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef MSTPPORTVLANCONFIGMESSAGE_H
#define MSTPPORTVLANCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmUpdatePoVlanStpConfigSPMessage;
	class NsmUpdatePhyVlanStpConfigMessage;
    class MstpPortVlanConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpPortVlanConfigMessage ();
			MstpPortVlanConfigMessage(NsmUpdatePoVlanStpConfigSPMessage &message);
			MstpPortVlanConfigMessage(NsmUpdatePhyVlanStpConfigMessage &message);
            MstpPortVlanConfigMessage (const InterfaceType &ifType, const string &ifName,const UI32 &vid,const UI16 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~MstpPortVlanConfigMessage ();

            void setInterfaceInfo(const InterfaceType &ifType, const string &ifName);
            InterfaceType getIfType() const;
            string getIfName()  const;
            void setVid(const UI32 &vid);
            UI32 getVid()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setGuard(const UI32 &guard);
            UI32 getGuard()  const;
            void setRestrictedTcn(const UI32 &restrictedTcn);
            UI32 getRestrictedTcn()  const;
            void setRestrictedRole(const UI32 &restrictedRole);
            UI32 getRestrictedRole()  const;

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure) {
				return;
			}

// Now the data members

        private:
        protected:
        public:
            InterfaceType m_ifType;
            string  m_ifName;
            UI32  m_vid;
            UI32  m_priority;
            UI32  m_cost;
            UI32  m_guard;
            UI32  m_restrictedTcn;
            UI32  m_restrictedRole;
            UI32  m_cmdCode;
    };
}
#endif                                            //MSTPPORTVLANCONFIGMESSAGE_H
