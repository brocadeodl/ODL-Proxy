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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef SSMGLOBALASSOCIPRULETOPOLICYMESSAGE_H
#define SSMGLOBALASSOCIPRULETOPOLICYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Ssm/Global/SsmAssocIpRuleToPolicyPayloadMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalAssocIpRuleToPolicyMessage : public ManagementInterfaceMessage, public SsmAssocIpRuleToPolicyPayloadMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmGlobalAssocIpRuleToPolicyMessage ();
			
			SsmGlobalAssocIpRuleToPolicyMessage(const string &policyName, const UI32 &policyType, 
				const UI64 &ruleid, const UI32 &action, const string &srcUnion,
				const IpAddrType &srcType, const IpV4Address &srcHostIpAddress, 
				const IpV4Address &srcIpAddress, const IpV4Address &srcIpAddressMask, 
				const bool &isCount, const bool &isLog, const IpV4Address &dstHostIpAddress, 
				const IpV4Address &dstIpAddress, const IpV4Address &dstIpAddressMask, const
				string &dstUnion, const IpAddrType &dstType, const string &ProtocolTypeUnion, 
				const UI32 &l3l4CustomProtoType, const L3L4wellknownProtoType &l3l4WellknownProtoType, 
				const PortOperator &srcPortOperator, const string &srcPortNumberTcpUnion, 
				const string &srcPortltNumberTcpUnion,
				const string &srcPortgtNumberTcpUnion, const string &srcPortNumberUdpUnion,
				const string &srcPortltNumberUdpUnion, const string &srcPortgtNumberUdpUnion,
				const string &srcPortrangeLowerTcpUnion, const string &srcPortrangeLowerUdpUnion,
				const string  &srcPortrangeHigherTcpUnion, const string &srcPortrangeHigherUdpUnion,
				const UI32 &srcPortCustomNumberTcp, const UI32 &srcPortltCustomNumberTcp,
				const UI32 &srcPortgtCustomNumberTcp, 
				const PortWellknownNumberTcp &srcPortWellknownNumberTcp,
				const PortWellknownNumberTcp &srcPortltWellknownNumberTcp,
				const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp,
				const UI32 &srcPortCustomNumberUdp, const UI32 &srcPortltCustomNumberUdp,
				const UI32 &srcPortgtCustomNumberUdp, 
				const PortWellknownNumberUdp &srcPortWellknownNumberUdp,
				const PortWellknownNumberUdp &srcPortltWellknownNumberUdp,
				const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp,
				const UI32 &srcPortCustomrangeLowerTcp,
				const PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp,
				const UI32 &srcPortCustomrangeHigherTcp,
				const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp,
				const UI32 &srcPortCustomrangeLowerUdp,
				const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp,
				const UI32 &srcPortCustomrangeHigherUdp,
				const PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp,
				const string &dstPortNumberTcpUnion, const string &dstPortltNumberTcpUnion,
				const string &dstPortgtNumberTcpUnion, const string &dstPortNumberUdpUnion,
				const string &dstPortltNumberUdpUnion, const string &dstPortgtNumberUdpUnion,
				const string &dstPortrangeLowerTcpUnion, const string &dstPortrangeLowerUdpUnion,
				const string &dstPortrangeHigherTcpUnion, const string &dstPortrangeHigherUdpUnion,
				const PortOperator &dstPortOperator, const UI32 &dstPortCustomNumberTcp,
				const UI32 &dstPortltCustomNumberTcp, const UI32 &dstPortgtCustomNumberTcp,
				const PortWellknownNumberTcp &dstPortWellknownNumberTcp,
				const PortWellknownNumberTcp &dstPortltWellknownNumberTcp,
				const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp,
				const UI32 &dstPortCustomNumberUdp, const UI32 &dstPortltCustomNumberUdp,
				const UI32 &dstPortgtCustomNumberUdp,
				const PortWellknownNumberUdp &dstPortWellknownNumberUdp,
				const PortWellknownNumberUdp &dstPortltWellknownNumberUdp,
				const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp,
				const UI32 &dstPortCustomrangeLowerTcp,
				const PortWellknownNumberTcp &dstPortWellknownrangeLowerTcp,
				const UI32 &dstPortCustomrangeHigherTcp,
				const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp,
				const UI32 &dstPortCustomrangeLowerUdp,
				const PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp,
				const UI32 &dstPortCustomrangeHigherUdp,
				const PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp,
				const WellknownPrecedence &wellknownPrecedence, const WellknownTos &wellknownTos, 
				const WellknownDscp &wellknownDscp, const UI32 &customPrecedence, 
				const UI32 &customTos, const UI32 &customDscp, const string &precedenceUnion, 
				const string &tosUnion, const string &dscpUnion, const bool &urg, const bool &ack, 
				const bool &push, const bool &fin, const bool  &rst, const bool &sync, 
			const  UI32 &opcode, const UI8 &userEnteredSeqid);
			virtual	~SsmGlobalAssocIpRuleToPolicyMessage ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif  //SSMGLOBALASSOCIPRULETOPOLICYMESSAGE_H                                           