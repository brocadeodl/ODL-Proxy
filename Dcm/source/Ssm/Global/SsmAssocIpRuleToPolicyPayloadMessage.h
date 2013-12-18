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
 *   Author : bpatel                                                     *
 **************************************************************************/

#ifndef SSMASSOCIPRULETOPOLICYPAYLOADMESSAGE_H
#define SSMASSOCIPRULETOPOLICYPAYLOADMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Messaging/Local/WaveMessagePayloadBase.h"
#include "Framework/Attributes/AttributeEnum.h"
#include <string>

using namespace WaveNs;

namespace DcmNs
{

	class SsmAssocIpRuleToPolicyPayloadMessage : public WaveMessagePayloadBase 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmAssocIpRuleToPolicyPayloadMessage (PrismMessage *pPrismMsg);
            
    		SsmAssocIpRuleToPolicyPayloadMessage (PrismMessage *pPrismMsg,
    				const string &policyName, const UI32 &policyType, const UI64 &ruleid,
    				const UI32 &action, const string &srcUnion, const IpAddrType &srcType,
    				const IpV4Address &srcHostIpAddress, const IpV4Address &srcIpAddress,
    				const IpV4Address &srcIpAddressMask, const bool &isCount,
    				const bool &isLog, const IpV4Address &dstHostIpAddress,
    				const IpV4Address &dstIpAddress, const IpV4Address &dstIpAddressMask,
    				const string &dstUnion, const IpAddrType &dstType,
    				const string &ProtocolTypeUnion, const UI32 &l3l4CustomProtoType,
    				const L3L4wellknownProtoType &l3l4WellknownProtoType, const PortOperator
    				&srcPortOperator, const string &srcPortNumberTcpUnion, const string &srcPortltNumberTcpUnion,
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
    				const WellknownPrecedence &wellknownPrecedence,
    				const WellknownTos &wellknownTos, const WellknownDscp &wellknownDscp, const UI32 &customPrecedence,
    				const UI32 &customTos, const UI32 &customDscp, const string &precedenceUnion, const string &tosUnion,
    				const string &dscpUnion, const bool &urg, const bool &ack, const bool &push, const bool &fin,
    				const bool &rst, const bool &sync, const UI32 &opcode, const UI8 &userEnteredSeqid);

			SsmAssocIpRuleToPolicyPayloadMessage (PrismMessage *pPrismMsg, 
				const string &policyName, const UI32 &policyType, const UI64 &ruleid,
				const UI32 &action, const string &srcUnion, const IpAddrType &srcType,
				const IpV4Address &srcHostIpAddress, const IpV4Address &srcIpAddress,
				const IpV4Address &srcIpAddressMask, const bool &isCount,
				const bool &isLog, const IpV4Address &dstHostIpAddress,
				const IpV4Address &dstIpAddress, const IpV4Address &dstIpAddressMask,
				const string &dstUnion, const IpAddrType &dstType,
				const string &ProtocolTypeUnion, const UI32 &l3l4CustomProtoType,
				const L3L4wellknownProtoType &l3l4WellknownProtoType, const PortOperator
				&srcPortOperator, const string &srcPortNumberTcpUnion, const string &srcPortltNumberTcpUnion,
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
				const WellknownPrecedence &wellknownPrecedence, 
				const WellknownTos &wellknownTos, const WellknownDscp &wellknownDscp, const UI32 &customPrecedence, 
				const UI32 &customTos, const UI32 &customDscp, const string &precedenceUnion, const string &tosUnion,
				const string &dscpUnion, const bool &urg, const bool &ack, const bool &push, const bool &fin, 
				const bool &rst, const bool &sync, const UI32 &opcode, const UI8 &userEnteredSeqid, const ObjectId &policyObjectId);
				
			virtual	~SsmAssocIpRuleToPolicyPayloadMessage ();
            
			void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            
			void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
            
			void setRuleid(const UI64 &ruleid);
            UI64 getRuleid()  const;
            
			void setAction(const UI32 &action);
            UI32 getAction()  const;
            
			void setSrcUnion(const string &srcUnion);
            string getSrcUnion()  const;
            
			void setSrcType(const IpAddrType &srcType);
            IpAddrType getSrcType()  const;
            
			void setSrcHostIpAddress(const IpV4Address &srcHostIpAddress);
            IpV4Address getSrcHostIpAddress()  const;
            
			void setSrcIpAddress(const IpV4Address &srcIpAddress);
            IpV4Address getSrcIpAddress()  const;
            
			void setSrcIpAddressMask(const IpV4Address &srcIpAddressMask);
            IpV4Address getSrcIpAddressMask()  const;
            
			void setIsCount(const bool &isCount);
            bool getIsCount()  const;
            
			void setIsLog(const bool &isLog);
            bool getIsLog()  const;
            
			void setDstHostIpAddress(const IpV4Address &dstHostIpAddress);
            IpV4Address getDstHostIpAddress()  const;
            
			void setDstIpAddress(const IpV4Address &dstIpAddress);
            IpV4Address getDstIpAddress()  const;
            
			void setDstIpAddressMask(const IpV4Address &dstIpAddressMask);
            IpV4Address getDstIpAddressMask()  const;
            
			void setDstUnion(const string &dstUnion);
            string getDstUnion()  const;
            
			void setDstType(const IpAddrType &dstType);
            IpAddrType getDstType()  const;
            
			void setProtocolTypeUnion(const string &ProtocolTypeUnion);
            string getProtocolTypeUnion()  const;
            
			void setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType);
            UI32 getL3l4CustomProtoType()  const;
            
			void setL3l4WellknownProtoType(const L3L4wellknownProtoType &l3l4WellknownProtoType);
            L3L4wellknownProtoType getL3l4WellknownProtoType()  const;
            
			void setSrcPortOperator(const PortOperator &srcPortOperator);
            PortOperator getSrcPortOperator()  const;
            
			void setSrcPortNumberTcpUnion(const string &srcPortNumberTcpUnion);
            string getSrcPortNumberTcpUnion()  const;
            
            void setSrcPortltNumberTcpUnion(const string &srcPortltNumberTcpUnion);
            string getSrcPortltNumberTcpUnion()  const;

            void setSrcPortgtNumberTcpUnion(const string &srcPortgtNumberTcpUnion);
            string getSrcPortgtNumberTcpUnion()  const;
	
			void setSrcPortNumberUdpUnion(const string &srcPortNumberUdpUnion);
            string getSrcPortNumberUdpUnion()  const;

            void setSrcPortltNumberUdpUnion(const string &srcPortltNumberUdpUnion);
            string getSrcPortltNumberUdpUnion()  const;

            void setSrcPortgtNumberUdpUnion(const string &srcPortgtNumberUdpUnion);
            string getSrcPortgtNumberUdpUnion()  const;
            
			void setSrcPortrangeLowerTcpUnion(const string &srcPortrangeLowerTcpUnion);
            string getSrcPortrangeLowerTcpUnion()  const;
            
			void setSrcPortrangeLowerUdpUnion(const string &srcPortrangeLowerUdpUnion);
            string getSrcPortrangeLowerUdpUnion()  const;
            
			void setSrcPortrangeHigherTcpUnion(const string &srcPortrangeHigherTcpUnion);
            string getSrcPortrangeHigherTcpUnion()  const;
            
			void setSrcPortrangeHigherUdpUnion(const string &srcPortrangeHigherUdpUnion);
            string getSrcPortrangeHigherUdpUnion()  const;
            
			void setSrcPortCustomNumberTcp(const UI32 &srcPortCustomNumberTcp);
            UI32 getSrcPortCustomNumberTcp()  const;

            void setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp);
            UI32 getSrcPortltCustomNumberTcp()  const;

            void setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp);
            UI32 getSrcPortgtCustomNumberTcp()  const;
            
			void setSrcPortWellknownNumberTcp(const PortWellknownNumberTcp &srcPortWellknownNumberTcp);
            PortWellknownNumberTcp getSrcPortWellknownNumberTcp()  const;


            void setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp);
            PortWellknownNumberTcp getSrcPortltWellknownNumberTcp()  const;
            
			void setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp);
            PortWellknownNumberTcp getSrcPortgtWellknownNumberTcp()  const;
            
			void setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp);
            UI32 getSrcPortCustomNumberUdp()  const;
            
            void setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp);
            UI32 getSrcPortltCustomNumberUdp()  const;

            void setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp);
            UI32 getSrcPortgtCustomNumberUdp()  const;

			void setSrcPortWellknownNumberUdp(const PortWellknownNumberUdp &srcPortWellknownNumberUdp);
            PortWellknownNumberUdp getSrcPortWellknownNumberUdp()  const;

            void setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp);
            PortWellknownNumberUdp getSrcPortltWellknownNumberUdp()  const;

            void setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp);
            PortWellknownNumberUdp getSrcPortgtWellknownNumberUdp()  const;
            
			void setSrcPortCustomrangeLowerTcp(const UI32 &srcPortCustomrangeLowerTcp);
            UI32 getSrcPortCustomrangeLowerTcp()  const;
            
			void setSrcPortWellknownrangeLowerTcp(const PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp);
            PortWellknownNumberTcp getSrcPortWellknownrangeLowerTcp()  const;
            
			void setSrcPortCustomrangeHigherTcp(const UI32 &srcPortCustomrangeHigherTcp);
            UI32 getSrcPortCustomrangeHigherTcp()  const;
            
			void setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp);
            PortWellknownNumberTcp getSrcPortWellknownrangeHigherTcp()  const;
            
			void setSrcPortCustomrangeLowerUdp(const UI32 &srcPortCustomrangeLowerUdp);
            UI32 getSrcPortCustomrangeLowerUdp()  const;
            
			void setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp);
            PortWellknownNumberUdp getSrcPortWellknownrangeLowerUdp()  const;
            
			void setSrcPortCustomrangeHigherUdp(const UI32 &srcPortCustomrangeHigherUdp);
            UI32 getSrcPortCustomrangeHigherUdp()  const;
            
			void setSrcPortWellknownrangeHigherUdp(const PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp);
            PortWellknownNumberUdp getSrcPortWellknownrangeHigherUdp()  const;
            
			void setDstPortNumberTcpUnion(const string &dstPortNumberTcpUnion);
            string getDstPortNumberTcpUnion()  const;

            void setDstPortltNumberTcpUnion(const string &dstPortltNumberTcpUnion);
            string getDstPortltNumberTcpUnion()  const;

            void setDstPortgtNumberTcpUnion(const string &dstPortgtNumberTcpUnion);
            string getDstPortgtNumberTcpUnion()  const;
           
			void setDstPortNumberUdpUnion(const string &dstPortNumberUdpUnion);
            string getDstPortNumberUdpUnion()  const;
            

            void setDstPortltNumberUdpUnion(const string &dstPortltNumberUdpUnion);
            string getDstPortltNumberUdpUnion()  const;

            void setDstPortgtNumberUdpUnion(const string &dstPortgtNumberUdpUnion);
            string getDstPortgtNumberUdpUnion()  const;		

			void setDstPortrangeLowerTcpUnion(const string &dstPortrangeLowerTcpUnion);
            string getDstPortrangeLowerTcpUnion()  const;
            
			void setDstPortrangeLowerUdpUnion(const string &dstPortrangeLowerUdpUnion);
            string getDstPortrangeLowerUdpUnion()  const;
            
			void setDstPortrangeHigherTcpUnion(const string &dstPortrangeHigherTcpUnion);
            string getDstPortrangeHigherTcpUnion()  const;
            
			void setDstPortrangeHigherUdpUnion(const string &dstPortrangeHigherUdpUnion);
            string getDstPortrangeHigherUdpUnion()  const;
            
			void setDstPortOperator(const PortOperator &dstPortOperator);
            PortOperator getDstPortOperator()  const;
            
			void setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp);
            UI32 getDstPortCustomNumberTcp()  const;
            
            void setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp);
            UI32 getDstPortltCustomNumberTcp()  const;

            void setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp);
            UI32 getDstPortgtCustomNumberTcp()  const;

			void setDstPortWellknownNumberTcp(const PortWellknownNumberTcp &dstPortWellknownNumberTcp);
            PortWellknownNumberTcp getDstPortWellknownNumberTcp()  const;
            
            void setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp);
            PortWellknownNumberTcp getDstPortltWellknownNumberTcp()  const;
            
		    void setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp);
            PortWellknownNumberTcp getDstPortgtWellknownNumberTcp()  const;

			void setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp);
            UI32 getDstPortCustomNumberUdp()  const;

            void setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp);
            UI32 getDstPortltCustomNumberUdp()  const;

            void setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp);
            UI32 getDstPortgtCustomNumberUdp()  const;

			void setDstPortWellknownNumberUdp(const PortWellknownNumberUdp &dstPortWellknownNumberUdp);
            PortWellknownNumberUdp getDstPortWellknownNumberUdp()  const;

            void setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp);
            PortWellknownNumberUdp getDstPortltWellknownNumberUdp()  const;

            void setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp);
            PortWellknownNumberUdp getDstPortgtWellknownNumberUdp()  const;
            
			void setDstPortCustomrangeLowerTcp(const UI32 &dstPortCustomrangeLowerTcp);
            UI32 getDstPortCustomrangeLowerTcp()  const;
            
			
			void setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp &dstPortWellknownrangeLowerTcp);
            PortWellknownNumberTcp getDstPortWellknownrangeLowerTcp()  const;
            
			void setDstPortCustomrangeHigherTcp(const UI32 &dstPortCustomrangeHigherTcp);
            UI32 getDstPortCustomrangeHigherTcp()  const;
            
			void setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp);
            PortWellknownNumberTcp getDstPortWellknownrangeHigherTcp()  const;
            
			void setDstPortCustomrangeLowerUdp(const UI32 &dstPortCustomrangeLowerUdp);
            UI32 getDstPortCustomrangeLowerUdp()  const;
            
			void setDstPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp);
            PortWellknownNumberUdp getDstPortWellknownrangeLowerUdp()  const;
            
			void setDstPortCustomrangeHigherUdp(const UI32 &dstPortCustomrangeHigherUdp);
            UI32 getDstPortCustomrangeHigherUdp()  const;
            
			void setDstPortWellknownrangeHigherUdp(const PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp);
            PortWellknownNumberUdp getDstPortWellknownrangeHigherUdp()  const;
            
			void setWellknownPrecedence(const WellknownPrecedence &wellknownPrecedence);
			WellknownPrecedence getWellknownPrecedence()  const;
			
			void setCustomPrecedence(const UI32 &customPrecedence);
            UI32 getCustomPrecedence()  const;
           
            void setPrecedenceUnion(const string &precedenceUnion);
            string getPrecedenceUnion()  const;

			void setWellknownTos(const WellknownTos &wellknownTos);
			WellknownTos getWellknownTos()  const;

			void setCustomTos(const UI32 &customTos);
			UI32 getCustomTos() const;

			void setTosUnion(const string &tosUnion);
			string getTosUnion()  const;

			void setWellknownDscp(const WellknownDscp &wellknownDscp);
			WellknownDscp getWellknownDscp()  const;

			void setCustomDscp(const UI32 &customDscp);
			UI32 getCustomDscp() const;

			void setDscpUnion(const string &dscpUnion);
			string getDscpUnion()  const;
		
			void setUrg(const bool &urg);
            bool getUrg()  const;
			
			void setAck(const bool &ack);
            bool getAck()  const;
			
			void setPush(const bool &push);
            bool getPush()  const;
            
			void setFin(const bool &fin);
            bool getFin()  const;
            
			void setRst(const bool &rst);
            bool getRst()  const;
            
			void setSync(const bool &sync);
			bool getSync()  const;
            
			void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;

            void setUserEnteredSeqid(const UI8 &userEnteredSeqid);
            UI8 getUserEnteredSeqid()  const;

            void setPolicyObjectId (const ObjectId policyObjectId);
            ObjectId getPolicyObjectId () const;
			
// Now the data members

        private:
        protected:
            string  				m_policyName;
            UI32  				    m_policyType;
            UI64  				    m_ruleid;
            UI32  				    m_action;
            string  			    m_srcUnion;
            IpAddrType  		    m_srcType;
            IpV4Address 		    m_srcHostIpAddress;
            IpV4Address			    m_srcIpAddress;
            IpV4Address 		    m_srcIpAddressMask;
            bool  				    m_isCount;
            bool  				    m_isLog;
            IpV4Address 		    m_dstHostIpAddress;
            IpV4Address 		    m_dstIpAddress;
            IpV4Address 		    m_dstIpAddressMask;
            string  			    m_dstUnion;
            IpAddrType  		    m_dstType;
            string  			    m_ProtocolTypeUnion;
            UI32  				    m_l3l4CustomProtoType;
            L3L4wellknownProtoType  m_l3l4WellknownProtoType;
            PortOperator  			m_srcPortOperator;
            string  				m_srcPortNumberTcpUnion;
            string  				m_srcPortltNumberTcpUnion;
            string  				m_srcPortgtNumberTcpUnion;
			string  				m_srcPortNumberUdpUnion;
			string  				m_srcPortltNumberUdpUnion;
			string  				m_srcPortgtNumberUdpUnion;
            string  				m_srcPortrangeLowerTcpUnion;
            string  				m_srcPortrangeLowerUdpUnion;
            string  				m_srcPortrangeHigherTcpUnion;
            string 					m_srcPortrangeHigherUdpUnion;
            UI32  					m_srcPortCustomNumberTcp;
            UI32  					m_srcPortltCustomNumberTcp;
            UI32  					m_srcPortgtCustomNumberTcp;
            PortWellknownNumberTcp  m_srcPortWellknownNumberTcp;
            PortWellknownNumberTcp  m_srcPortltWellknownNumberTcp;
            PortWellknownNumberTcp  m_srcPortgtWellknownNumberTcp;
            UI32  					m_srcPortCustomNumberUdp;
            UI32  					m_srcPortltCustomNumberUdp;
            UI32  					m_srcPortgtCustomNumberUdp;
            PortWellknownNumberUdp  m_srcPortWellknownNumberUdp;
            PortWellknownNumberUdp  m_srcPortltWellknownNumberUdp;
            PortWellknownNumberUdp  m_srcPortgtWellknownNumberUdp;
            UI32  					m_srcPortCustomrangeLowerTcp;
            PortWellknownNumberTcp  m_srcPortWellknownrangeLowerTcp;
            UI32  					m_srcPortCustomrangeHigherTcp;
            PortWellknownNumberTcp  m_srcPortWellknownrangeHigherTcp;
            UI32  					m_srcPortCustomrangeLowerUdp;
            PortWellknownNumberUdp  m_srcPortWellknownrangeLowerUdp;
            UI32  					m_srcPortCustomrangeHigherUdp;
            PortWellknownNumberUdp  m_srcPortWellknownrangeHigherUdp;
            string  				m_dstPortNumberTcpUnion;
            string  				m_dstPortltNumberTcpUnion;
            string  				m_dstPortgtNumberTcpUnion;
            string  				m_dstPortNumberUdpUnion;
            string  				m_dstPortltNumberUdpUnion;
            string  				m_dstPortgtNumberUdpUnion;
            string  				m_dstPortrangeLowerTcpUnion;
            string  				m_dstPortrangeLowerUdpUnion;
            string  				m_dstPortrangeHigherTcpUnion;
            string 					m_dstPortrangeHigherUdpUnion;
            PortOperator  			m_dstPortOperator;
            UI32  					m_dstPortCustomNumberTcp;
            UI32  					m_dstPortltCustomNumberTcp;
            UI32  					m_dstPortgtCustomNumberTcp;
            PortWellknownNumberTcp  m_dstPortWellknownNumberTcp;
            PortWellknownNumberTcp  m_dstPortltWellknownNumberTcp;
            PortWellknownNumberTcp  m_dstPortgtWellknownNumberTcp;
            UI32  					m_dstPortCustomNumberUdp;
            UI32  					m_dstPortltCustomNumberUdp;
            UI32  					m_dstPortgtCustomNumberUdp;
            PortWellknownNumberUdp  m_dstPortWellknownNumberUdp;
            PortWellknownNumberUdp  m_dstPortltWellknownNumberUdp;
            PortWellknownNumberUdp  m_dstPortgtWellknownNumberUdp;
            UI32  					m_dstPortCustomrangeLowerTcp;
            PortWellknownNumberTcp  m_dstPortWellknownrangeLowerTcp;
            UI32  					m_dstPortCustomrangeHigherTcp;
            PortWellknownNumberTcp  m_dstPortWellknownrangeHigherTcp;
            UI32  					m_dstPortCustomrangeLowerUdp;
            PortWellknownNumberUdp  m_dstPortWellknownrangeLowerUdp;
            UI32  					m_dstPortCustomrangeHigherUdp;
            PortWellknownNumberUdp  m_dstPortWellknownrangeHigherUdp;
            UI32  					m_customPrecedence;
			WellknownPrecedence     m_wellknownPrecedence;
            UI32  					m_customTos;
			WellknownTos			m_wellknownTos;
            UI32  					m_customDscp;
			WellknownDscp			m_wellknownDscp;
			string					m_precedenceUnion;
			string					m_tosUnion;
			string					m_dscpUnion;
            bool  					m_urg;
            bool  					m_ack;
            bool  					m_push;
            bool  					m_fin;
            bool  					m_rst;
            bool  					m_sync;
            UI32  					m_opcode;
    		UI8             		m_userEnteredSeqid;
    		ObjectId                m_policyObjectId;
	public:
    };
}
#endif                                            //SSMASSOCIPRULETOPOLICYPAYLOADMESSAGE_H
