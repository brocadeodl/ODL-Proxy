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

#ifndef ExtIp6AclRuleManagedObject_H
#define ExtIp6AclRuleManagedObject_H

#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeEnumUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class ExtIp6AclRuleManagedObject : virtual public Ip6AclRuleManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ExtIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
    		ExtIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
        		const IpV6Address &dstHostIpAddress, const IpV6AddressNetworkMask &dstIpAddress, 
				const WaveUnion &dstUnion, 
				const IpAddrType &dstType, const WaveUnion &ProtocolTypeUnion,
        		const UI32 &l3l4CustomProtoType, const L3L4wellknownProtoType &l3l4WellknownProtoType,
        		const PortOperator &srcPortOperator, const WaveUnion &srcPortNumberTcpUnion,
        		const WaveUnion &srcPortltNumberTcpUnion, const WaveUnion &srcPortgtNumberTcpUnion,
				const WaveUnion &srcPortNumberUdpUnion, const WaveUnion &srcPortltNumberUdpUnion,
				const WaveUnion &srcPortgtNumberUdpUnion, const WaveUnion &srcPortrangeLowerTcpUnion,
				const WaveUnion &srcPortrangeLowerUdpUnion, const WaveUnion &srcPortrangeHigherTcpUnion,
				const WaveUnion &srcPortrangeHigherUdpUnion, const UI32 &srcPortCustomNumberTcp,
				const UI32 &srcPortltCustomNumberTcp, const UI32 &srcPortgtCustomNumberTcp,
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
				const WaveUnion &dstPortNumberTcpUnion,
				const WaveUnion &dstPortltNumberTcpUnion, const WaveUnion &dstPortgtNumberTcpUnion,
				const WaveUnion &dstPortNumberUdpUnion, const WaveUnion &dstPortltNumberUdpUnion,
				const WaveUnion &dstPortgtNumberUdpUnion, const WaveUnion &dstPortrangeLowerTcpUnion,
				const WaveUnion &dstPortrangeLowerUdpUnion,const WaveUnion &dstPortrangeHigherTcpUnion,
				const WaveUnion &dstPortrangeHigherUdpUnion, const PortOperator &dstPortOperator,
				const UI32 &dstPortCustomNumberTcp, const UI32 &dstPortltCustomNumberTcp,
				const UI32 &dstPortgtCustomNumberTcp, const PortWellknownNumberTcp &dstPortWellknownNumberTcp,
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
				const WellknownDscp &wellknownDscp, const UI32 &customPrecedence, const UI32 &customTos, 
				const UI32 &customDscp, const WaveUnion &precedenceUnion, const WaveUnion &tosUnion,
                const WaveUnion &dscpUnion, const bool &urg, const bool &ack, 
				const bool &push, const bool &fin, const bool &rst, const bool &sync);
            virtual    ~ExtIp6AclRuleManagedObject ();
            static string getClassName();
            void setDstHostIpAddress(const IpV6Address &dstHostIpAddress);
            IpV6Address getDstHostIpAddress()  const;
            void setDstIpAddress(const IpV6AddressNetworkMask &dstIpAddress);
            IpV6AddressNetworkMask getDstIpAddress()  const;
//            void setDstIpAddressMask(const IpV6Address &dstIpAddressMask);
//            IpV6Address getDstIpAddressMask()  const;
            void setDstUnion(const WaveUnion &dstUnion);
            WaveUnion getDstUnion()  const;
            void setDstType(const IpAddrType &dstType);
            IpAddrType getDstType()  const;
            void setProtocolTypeUnion(const WaveUnion &ProtocolTypeUnion);
            WaveUnion getProtocolTypeUnion()  const;
            void setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType);
            UI32 getL3l4CustomProtoType()  const;
            void setL3l4WellknownProtoType(const L3L4wellknownProtoType &l3l4WellknownProtoType);
            L3L4wellknownProtoType getL3l4WellknownProtoType()  const;
            void setSrcPortOperator(const PortOperator &srcPortOperator);
            PortOperator getSrcPortOperator()  const;
            void setSrcPortNumberTcpUnion(const WaveUnion &srcPortNumberTcpUnion);
            WaveUnion getSrcPortNumberTcpUnion()  const;
            void setSrcPortltNumberTcpUnion(const WaveUnion &srcPortltNumberTcpUnion);
            WaveUnion getSrcPortltNumberTcpUnion()  const;
            void setSrcPortgtNumberTcpUnion(const WaveUnion &srcPortgtNumberTcpUnion);
            WaveUnion getSrcPortgtNumberTcpUnion()  const;
            void setSrcPortNumberUdpUnion(const WaveUnion &srcPortNumberUdpUnion);
            WaveUnion getSrcPortNumberUdpUnion()  const;
            void setSrcPortltNumberUdpUnion(const WaveUnion &srcPortltNumberUdpUnion);
            WaveUnion getSrcPortltNumberUdpUnion()  const;
            void setSrcPortgtNumberUdpUnion(const WaveUnion &srcPortgtNumberUdpUnion);
            WaveUnion getSrcPortgtNumberUdpUnion()  const;
            void setSrcPortrangeLowerTcpUnion(const WaveUnion &srcPortrangeLowerTcpUnion);
            WaveUnion getSrcPortrangeLowerTcpUnion()  const;
            void setSrcPortrangeLowerUdpUnion(const WaveUnion &srcPortrangeLowerUdpUnion);
            WaveUnion getSrcPortrangeLowerUdpUnion()  const;
            void setSrcPortrangeHigherTcpUnion(const WaveUnion &srcPortrangeHigherTcpUnion);
            WaveUnion getSrcPortrangeHigherTcpUnion()  const;
            void setSrcPortrangeHigherUdpUnion(const WaveUnion &srcPortrangeHigherUdpUnion);
            WaveUnion getSrcPortrangeHigherUdpUnion()  const;
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
            void setDstPortNumberTcpUnion(const WaveUnion &dstPortNumberTcpUnion);
            WaveUnion getDstPortNumberTcpUnion()  const;
            void setDstPortltNumberTcpUnion(const WaveUnion &dstPortltNumberTcpUnion);
            WaveUnion getDstPortltNumberTcpUnion()  const;
            void setDstPortgtNumberTcpUnion(const WaveUnion &dstPortgtNumberTcpUnion);
            WaveUnion getDstPortgtNumberTcpUnion()  const;
            void setDstPortNumberUdpUnion(const WaveUnion &dstPortNumberUdpUnion);
            WaveUnion getDstPortNumberUdpUnion()  const;
            void setDstPortltNumberUdpUnion(const WaveUnion &dstPortltNumberUdpUnion);
            WaveUnion getDstPortltNumberUdpUnion()  const;
            void setDstPortgtNumberUdpUnion(const WaveUnion &dstPortgtNumberUdpUnion);
            WaveUnion getDstPortgtNumberUdpUnion()  const;
            void setDstPortrangeLowerTcpUnion(const WaveUnion &dstPortrangeLowerTcpUnion);
            WaveUnion getDstPortrangeLowerTcpUnion()  const;
            void setDstPortrangeLowerUdpUnion(const WaveUnion &dstPortrangeLowerUdpUnion);
            WaveUnion getDstPortrangeLowerUdpUnion()  const;
            void setDstPortrangeHigherTcpUnion(const WaveUnion &dstPortrangeHigherTcpUnion);
            WaveUnion getDstPortrangeHigherTcpUnion()  const;
            void setDstPortrangeHigherUdpUnion(const WaveUnion &dstPortrangeHigherUdpUnion);
            WaveUnion getDstPortrangeHigherUdpUnion()  const;
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
            PortWellknownNumberUdp  getDstPortWellknownNumberUdp()  const;

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
            void setPrecedenceUnion(const WaveUnion &precedenceUnion);
            WaveUnion getPrecedenceUnion()  const;
            void setWellknownTos(const WellknownTos &wellknownTos);
            WellknownTos getWellknownTos()  const;
            void setCustomTos(const UI32 &customTos);
            UI32 getCustomTos() const;
            void setTosUnion(const WaveUnion &tosUnion);
            WaveUnion getTosUnion()  const;
            void setWellknownDscp(const WellknownDscp &wellknownDscp);
            WellknownDscp getWellknownDscp()  const;
            void setCustomDscp(const UI32 &customDscp);
            UI32 getCustomDscp() const;
            void setDscpUnion(const WaveUnion &dscpUnion);
            WaveUnion getDscpUnion()  const;
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

// Now the data members

        private:
        protected:
        public:
            IpV6Address 			m_dstHostIpAddress;
            IpV6AddressNetworkMask	m_dstIpAddress;
//            IpV6Address  			m_dstIpAddressMask;
            WaveUnion  				m_dstUnion;
            IpAddrType  			m_dstType;
            WaveUnion  				m_ProtocolTypeUnion;
            UI32  					m_l3l4CustomProtoType;
            L3L4wellknownProtoType  m_l3l4WellknownProtoType;
            PortOperator  			m_srcPortOperator;
            WaveUnion  				m_srcPortNumberTcpUnion;
            WaveUnion  				m_srcPortltNumberTcpUnion;
            WaveUnion  				m_srcPortgtNumberTcpUnion;
            WaveUnion  				m_srcPortNumberUdpUnion;
            WaveUnion  				m_srcPortltNumberUdpUnion;
            WaveUnion  				m_srcPortgtNumberUdpUnion;
            WaveUnion  				m_srcPortrangeLowerTcpUnion;
            WaveUnion  				m_srcPortrangeLowerUdpUnion;
            WaveUnion  				m_srcPortrangeHigherTcpUnion;
            WaveUnion  				m_srcPortrangeHigherUdpUnion;
            UI32UC  				m_srcPortCustomNumberTcp;
            UI32UC  				m_srcPortltCustomNumberTcp;
            UI32UC  				m_srcPortgtCustomNumberTcp;
            UI32UC  				m_srcPortWellknownNumberTcp;
            UI32UC  				m_srcPortltWellknownNumberTcp;
            UI32UC  				m_srcPortgtWellknownNumberTcp;
            UI32UC  				m_srcPortCustomNumberUdp;
            UI32UC  				m_srcPortltCustomNumberUdp;
            UI32UC  				m_srcPortgtCustomNumberUdp;
            UI32UC  				m_srcPortWellknownNumberUdp;
            UI32UC  				m_srcPortltWellknownNumberUdp;
            UI32UC  				m_srcPortgtWellknownNumberUdp;
            UI32UC  				m_srcPortCustomrangeLowerTcp;
            UI32UC  				m_srcPortWellknownrangeLowerTcp;
            UI32UC  				m_srcPortCustomrangeHigherTcp;
            UI32UC  				m_srcPortWellknownrangeHigherTcp;
            UI32UC  				m_srcPortCustomrangeLowerUdp;
            UI32UC  				m_srcPortWellknownrangeLowerUdp;
            UI32UC  				m_srcPortCustomrangeHigherUdp;
            UI32UC  				m_srcPortWellknownrangeHigherUdp;
            WaveUnion  				m_dstPortNumberTcpUnion;
            WaveUnion  				m_dstPortltNumberTcpUnion;
            WaveUnion  				m_dstPortgtNumberTcpUnion;
            WaveUnion  				m_dstPortNumberUdpUnion;
            WaveUnion  				m_dstPortltNumberUdpUnion;
            WaveUnion  				m_dstPortgtNumberUdpUnion;
            WaveUnion  				m_dstPortrangeLowerTcpUnion;
            WaveUnion  				m_dstPortrangeLowerUdpUnion;
            WaveUnion  				m_dstPortrangeHigherTcpUnion;
            WaveUnion  				m_dstPortrangeHigherUdpUnion;
            PortOperator  			m_dstPortOperator;
            UI32UC  				m_dstPortCustomNumberTcp;
            UI32UC  				m_dstPortltCustomNumberTcp;
            UI32UC  				m_dstPortgtCustomNumberTcp;
            UI32UC  				m_dstPortWellknownNumberTcp;
            UI32UC  				m_dstPortltWellknownNumberTcp;
            UI32UC  				m_dstPortgtWellknownNumberTcp;
            UI32UC  				m_dstPortCustomNumberUdp;
            UI32UC  				m_dstPortltCustomNumberUdp;
            UI32UC  				m_dstPortgtCustomNumberUdp;
            UI32UC  				m_dstPortWellknownNumberUdp;
            UI32UC  				m_dstPortltWellknownNumberUdp;
            UI32UC  				m_dstPortgtWellknownNumberUdp;
            UI32UC  				m_dstPortCustomrangeLowerTcp;
            UI32UC  				m_dstPortWellknownrangeLowerTcp;
            UI32UC  				m_dstPortCustomrangeHigherTcp;
            UI32UC  				m_dstPortWellknownrangeHigherTcp;
            UI32UC  				m_dstPortCustomrangeLowerUdp;
            UI32UC  				m_dstPortWellknownrangeLowerUdp;
            UI32UC  				m_dstPortCustomrangeHigherUdp;
            UI32UC  				m_dstPortWellknownrangeHigherUdp;
            bool  					m_urg;
            bool  					m_ack;
            bool  					m_push;
            bool  					m_fin;
            bool  					m_rst;
            bool  					m_sync;
            UI32UC                  m_customPrecedence;
            UI32UC 				    m_wellknownPrecedence;
            UI32UC                  m_customTos;
            UI32UC		            m_wellknownTos;
            UI32UC                  m_customDscp;
            UI32UC		            m_wellknownDscp;
            WaveUnion               m_precedenceUnion;
            WaveUnion               m_tosUnion;
            WaveUnion               m_dscpUnion;
    };
}
#endif                                            //ExtIp6AclRuleManagedObject_H
