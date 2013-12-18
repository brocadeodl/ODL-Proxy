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
 *   Author : hraza                                                        *
 **************************************************************************/

#ifndef OSPFPERMITDENYREDISTRIBUTEMESSAGE_H
#define OSPFPERMITDENYREDISTRIBUTEMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

	class OSPFPermitDenyRedistributeLocalMessage;

    class OSPFPermitDenyRedistributeMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFPermitDenyRedistributeMessage ();
            OSPFPermitDenyRedistributeMessage (const UI8 &opCode,const UI8 &cmdCode,const UI32 &index, const OSPFPermitDenyOption &permitDenyOption, const OSPFRouteOption &routeOption, const IpV4Address &subNet, const IpV4Address &mask, const UI32 &matchMetric, const UI32 &setMetric);
            virtual    ~OSPFPermitDenyRedistributeMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
            void setPermitDenyOption(const OSPFPermitDenyOption &permitDenyOption);
            OSPFPermitDenyOption getPermitDenyOption()  const;
            void setRedistOption(const OSPFRedistOption &redistOption);
            OSPFRedistOption getRedistOption() const;
            void setRouteOption(const OSPFRouteOption &routeOption);
            OSPFRouteOption getRouteOption() const;
            void setSubNet(const IpV4Address &subNet);
            IpV4Address getSubNet()  const;
            void setMask(const IpV4Address &mask);
            IpV4Address getMask()  const;
            void setMatchMetric(const UI32 &matchMetric);
            UI32 getMatchMetric()  const;
            void setSetMetric(const UI32 &setMetric);
            UI32 getSetMetric()  const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void updateFromLocalMessage (OSPFPermitDenyRedistributeLocalMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            UI32  m_index;
            OSPFPermitDenyOption m_permitDenyOption;
            OSPFRouteOption m_routeOption;
            IpV4Address  m_subNet;
            IpV4Address  m_mask;
            UI32  m_matchMetric;
			UI32  m_setMetric;
    };
}
#endif                                            //OSPFPermitDenyRedistributeMessage_H
