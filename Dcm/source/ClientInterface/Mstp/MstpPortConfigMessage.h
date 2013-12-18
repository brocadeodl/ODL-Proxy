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

#ifndef MSTPPORTCONFIGMESSAGE_H
#define MSTPPORTCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmUpdatePoStpConfigSPMessage;
	class NsmUpdatePhyStpConfigMessage;
	class NsmUpdatePoInstanceStpConfigSPMessage;
	class NsmUpdatePhyInstanceStpConfigMessage;

    class MstpPortConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
            void initialyzeDefaultMembers();
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpPortConfigMessage ();
	        MstpPortConfigMessage (NsmUpdatePoStpConfigSPMessage &msg);
	        MstpPortConfigMessage (NsmUpdatePhyStpConfigMessage &msg);
	        MstpPortConfigMessage(NsmUpdatePoInstanceStpConfigSPMessage &msg);
	        MstpPortConfigMessage(NsmUpdatePhyInstanceStpConfigMessage &msg);

            virtual    ~MstpPortConfigMessage ();
            void setInterfaceInfo(const InterfaceType ifType, const string &ifName);
            InterfaceType getIfType() const;
            string getIfName()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
			void  setInstanceId(const UI8 &instanceId);
			UI8  getInstanceId() const;
			void  printMsg();

            void setRestrictedRole(const bool &restrictedRole);
            void setRestrictedTcn(const bool &restrictedTcn);
            void setAutoEdge(const bool &autoEdge);
            void setPortFast(const bool &portFast);
            void setEdgePort(const bool &edgePort);
            void setBpduGuardEnable(const bool &bpduGuardEnable);
            void setBpduGuard(const bool &bpduGuard);
            void setBpduDropDirection(const UI8 &direction);
            void setBpduFilter(const bool &bpduFilter);
            void setGuard(const bool &guard);
            void setLinkShared(const bool &linkShared);
            void setStpEnable(const bool &stpEnable);
            void setBpduMac(const bool &bpduMac);
            void setPortId(const UI32 portid);
            UI32 getPortId() const;
            void setTaggedBpduEnable (bool taggedBpduEnable);
            bool getTaggedBpduEnable () const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure) {
				return;
			}

            // Fill the interface name into backend field
            static void writeIfName(unsigned char *toBuff, const InterfaceType &ifType, const string &ifName);

// Now the data members

        private:
        protected:
        public:
            InterfaceType m_ifType;
            string  m_ifName;
            UI8  m_instanceId;
            UI32  m_priority;
            UI32  m_helloTime;
            UI32  m_cost;
            bool  m_restrictedRole;
            bool  m_restrictedTcn;
            bool  m_autoEdge;
            bool  m_portFast;
            bool  m_edgePort;
            bool  m_bpduGuardEnable;
            bool  m_bpduGuard;
            bool  m_bpduFilter;
            bool  m_guard;
            bool  m_linkShared;
            bool  m_bpduMac;
            bool  m_stpEnable;
            bool  m_taggedBpduEnable;
            UI32  m_cmdCode;
            UI32  m_portId;
            UI8	  m_bpduDropDirection;
    };
}
#endif                                            //MSTPPORTCONFIGMESSAGE_H
