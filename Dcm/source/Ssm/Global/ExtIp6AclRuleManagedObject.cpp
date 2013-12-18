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

#include "Ssm/Global/ExtIp6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmTypes.h"
#include "vcs.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
namespace DcmNs
{

    ExtIp6AclRuleManagedObject::ExtIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIp6AclRuleManagedObject::getClassName (), Ip6AclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclRuleManagedObject (pSsmGlobalObjectManager)
    {
			PortOperator 				defaultValsrcDstPortOperator 			= PORTOPERATOR_UNKNOWN;

            m_dstType							= IP_ADDR_TYPE_NONE;
			m_srcPortOperator					= defaultValsrcDstPortOperator;
			m_dstPortOperator					= defaultValsrcDstPortOperator;

			m_srcPortCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomNumberTcp.setIsUserConfigured(false);
			m_srcPortltCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortltCustomNumberTcp.setIsUserConfigured(false);
			m_srcPortgtCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortgtCustomNumberTcp.setIsUserConfigured(false);
			m_srcPortCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomNumberUdp.setIsUserConfigured(false);
			m_srcPortltCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortltCustomNumberUdp.setIsUserConfigured(false);
			m_srcPortgtCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortgtCustomNumberUdp.setIsUserConfigured(false);
			m_dstPortCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomNumberTcp.setIsUserConfigured(false);
			m_dstPortltCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortltCustomNumberTcp.setIsUserConfigured(false);
			m_dstPortgtCustomNumberTcp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortgtCustomNumberTcp.setIsUserConfigured(false);
			m_dstPortCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomNumberUdp.setIsUserConfigured(false);
			m_dstPortltCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortltCustomNumberUdp.setIsUserConfigured(false);
			m_dstPortgtCustomNumberUdp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortgtCustomNumberUdp.setIsUserConfigured(false);

			m_srcPortWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_srcPortWellknownNumberTcp.setIsUserConfigured(false);
			m_srcPortltWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_srcPortltWellknownNumberTcp.setIsUserConfigured(false);
			m_srcPortgtWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_srcPortgtWellknownNumberTcp.setIsUserConfigured(false);

			m_dstPortWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_dstPortWellknownNumberTcp.setIsUserConfigured(false);
			m_dstPortltWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_dstPortltWellknownNumberTcp.setIsUserConfigured(false);
			m_dstPortgtWellknownNumberTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_dstPortgtWellknownNumberTcp.setIsUserConfigured(false);

			m_srcPortWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_srcPortWellknownNumberUdp.setIsUserConfigured(false);
			m_srcPortltWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_srcPortltWellknownNumberUdp.setIsUserConfigured(false);
			m_srcPortgtWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_srcPortgtWellknownNumberUdp.setIsUserConfigured(false);

			m_dstPortWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_dstPortWellknownNumberUdp.setIsUserConfigured(false);
			m_dstPortltWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_dstPortltWellknownNumberUdp.setIsUserConfigured(false);
			m_dstPortgtWellknownNumberUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_dstPortgtWellknownNumberUdp.setIsUserConfigured(false);

			m_srcPortWellknownrangeLowerTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_srcPortWellknownrangeLowerTcp.setIsUserConfigured(false);
			m_srcPortWellknownrangeHigherTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_srcPortWellknownrangeHigherTcp.setIsUserConfigured(false);
			m_srcPortWellknownrangeLowerUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_srcPortWellknownrangeLowerUdp.setIsUserConfigured(false);
			m_srcPortWellknownrangeHigherUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_srcPortWellknownrangeHigherUdp.setIsUserConfigured(false);
			m_dstPortWellknownrangeLowerTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_dstPortWellknownrangeLowerTcp.setIsUserConfigured(false);
			m_dstPortWellknownrangeHigherTcp.setUI32Value(WELLKNOWNPORT_TCP_UNKNOWN);
			m_dstPortWellknownrangeHigherTcp.setIsUserConfigured(false);
			m_dstPortWellknownrangeLowerUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_dstPortWellknownrangeLowerUdp.setIsUserConfigured(false);
			m_dstPortWellknownrangeHigherUdp.setUI32Value(WELLKNOWNPORT_UDP_UNKNOWN);
			m_dstPortWellknownrangeHigherUdp.setIsUserConfigured(false);
			m_srcPortCustomrangeLowerTcp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomrangeLowerTcp.setIsUserConfigured(false);
			m_dstPortCustomrangeLowerTcp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomrangeLowerTcp.setIsUserConfigured(false);
			m_srcPortCustomrangeLowerUdp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomrangeLowerUdp.setIsUserConfigured(false);
			m_dstPortCustomrangeLowerUdp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomrangeLowerUdp.setIsUserConfigured(false);
			m_srcPortCustomrangeHigherTcp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomrangeHigherTcp.setIsUserConfigured(false);
			m_dstPortCustomrangeHigherTcp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomrangeHigherTcp.setIsUserConfigured(false);
			m_srcPortCustomrangeHigherUdp.setUI32Value(DEFAULT_PORTNUM);
			m_srcPortCustomrangeHigherUdp.setIsUserConfigured(false);
			m_dstPortCustomrangeHigherUdp.setUI32Value(DEFAULT_PORTNUM);
			m_dstPortCustomrangeHigherUdp.setIsUserConfigured(false);
			m_customPrecedence.setUI32Value(DEFAULT_PREC_DSCP_TOS_VAL);
			m_customPrecedence.setIsUserConfigured(false);
			m_wellknownPrecedence.setUI32Value(WELLKNOWN_PRECEDENCE_UNKNOWN);
			m_wellknownPrecedence.setIsUserConfigured(false);
			m_customTos.setUI32Value(DEFAULT_PREC_DSCP_TOS_VAL);
			m_customTos.setIsUserConfigured(false);
			m_wellknownTos.setUI32Value(WELLKNOWN_TOS_UNKNOWN);
			m_wellknownTos.setIsUserConfigured(false);
			m_customDscp.setUI32Value(DEFAULT_PREC_DSCP_TOS_VAL);
			m_customDscp.setIsUserConfigured(false);
			m_wellknownDscp.setUI32Value(WELLKNOWN_DSCP_UNKNOWN);
			m_wellknownDscp.setIsUserConfigured(false);
    }

    ExtIp6AclRuleManagedObject::ExtIp6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const IpV6Address &dstHostIpAddress, const IpV6AddressNetworkMask &dstIpAddress, 
//		const IpV6Address &dstIpAddressMask,
		 const WaveUnion &dstUnion, 
		const IpAddrType &dstType, const WaveUnion &ProtocolTypeUnion,
		const UI32 &l3l4CustomProtoType, const L3L4wellknownProtoType &l3l4WellknownProtoType,
		const PortOperator &srcPortOperator, const WaveUnion &srcPortNumberTcpUnion,
		const WaveUnion &srcPortltNumberTcpUnion, const WaveUnion &srcPortgtNumberTcpUnion,
		const WaveUnion &srcPortNumberUdpUnion,	const WaveUnion &srcPortltNumberUdpUnion,
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
		const UI32 &customDscp, const WaveUnion &precedenceUnion, const WaveUnion &tosUnion, const WaveUnion &dscpUnion,
		const bool &urg, const bool &ack, const bool &push, const bool &fin, const bool &rst, const bool &sync)	
		: PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIp6AclRuleManagedObject::getClassName (), Ip6AclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclRuleManagedObject (pSsmGlobalObjectManager),
        m_dstHostIpAddress    (dstHostIpAddress),
        m_dstIpAddress    (dstIpAddress),
//        m_dstIpAddressMask    (dstIpAddressMask),
        m_dstUnion    (dstUnion),
        m_dstType    (dstType),
        m_ProtocolTypeUnion    (ProtocolTypeUnion),
        m_l3l4CustomProtoType    (l3l4CustomProtoType),
        m_l3l4WellknownProtoType    (l3l4WellknownProtoType),
        m_srcPortOperator    (srcPortOperator),
        m_srcPortNumberTcpUnion    (srcPortNumberTcpUnion),
		m_srcPortltNumberTcpUnion    (srcPortltNumberTcpUnion),
		m_srcPortgtNumberTcpUnion    (srcPortgtNumberTcpUnion),
        m_srcPortNumberUdpUnion    (srcPortNumberUdpUnion),
		m_srcPortltNumberUdpUnion    (srcPortltNumberUdpUnion),
		m_srcPortgtNumberUdpUnion    (srcPortgtNumberUdpUnion),
        m_srcPortrangeLowerTcpUnion    (srcPortrangeLowerTcpUnion),
        m_srcPortrangeLowerUdpUnion    (srcPortrangeLowerUdpUnion),
        m_srcPortrangeHigherTcpUnion    (srcPortrangeHigherTcpUnion),
        m_srcPortrangeHigherUdpUnion    (srcPortrangeHigherUdpUnion),
        m_dstPortNumberTcpUnion    (dstPortNumberTcpUnion),
        m_dstPortltNumberTcpUnion    (dstPortgtNumberTcpUnion),
        m_dstPortgtNumberTcpUnion    (dstPortltNumberTcpUnion),
		m_dstPortNumberUdpUnion    (dstPortNumberUdpUnion),
		m_dstPortltNumberUdpUnion    (dstPortltNumberUdpUnion),
		m_dstPortgtNumberUdpUnion    (dstPortgtNumberUdpUnion),
        m_dstPortrangeLowerTcpUnion    (dstPortrangeLowerTcpUnion),
        m_dstPortrangeLowerUdpUnion    (dstPortrangeLowerUdpUnion),
        m_dstPortrangeHigherTcpUnion    (dstPortrangeHigherTcpUnion),
        m_dstPortrangeHigherUdpUnion    (dstPortrangeHigherUdpUnion),
        m_dstPortOperator    (dstPortOperator),
        m_urg    (urg),
        m_ack    (ack),
        m_push    (push),
        m_fin    (fin),
        m_rst    (rst),
        m_sync    (sync),
        m_precedenceUnion (precedenceUnion),
        m_tosUnion (tosUnion),
        m_dscpUnion (dscpUnion)
    {
		m_srcPortCustomNumberTcp.setUI32Value(srcPortCustomNumberTcp);
		m_srcPortCustomNumberTcp.setIsUserConfigured(true);
		m_srcPortltCustomNumberTcp.setUI32Value(srcPortltCustomNumberTcp);
		m_srcPortltCustomNumberTcp.setIsUserConfigured(true);
		m_srcPortgtCustomNumberTcp.setUI32Value(srcPortgtCustomNumberTcp);
		m_srcPortgtCustomNumberTcp.setIsUserConfigured(true);
		m_srcPortCustomNumberUdp.setUI32Value(srcPortCustomNumberUdp);
		m_srcPortCustomNumberUdp.setIsUserConfigured(true);
		m_srcPortltCustomNumberUdp.setUI32Value(srcPortltCustomNumberUdp);
		m_srcPortltCustomNumberUdp.setIsUserConfigured(true);
		m_srcPortgtCustomNumberUdp.setUI32Value(srcPortgtCustomNumberUdp);
		m_srcPortgtCustomNumberUdp.setIsUserConfigured(true);
		m_dstPortCustomNumberTcp.setUI32Value(dstPortCustomNumberTcp);
		m_dstPortCustomNumberTcp.setIsUserConfigured(true);
		m_dstPortltCustomNumberTcp.setUI32Value(dstPortltCustomNumberTcp);
		m_dstPortltCustomNumberTcp.setIsUserConfigured(true);
		m_dstPortgtCustomNumberTcp.setUI32Value(dstPortgtCustomNumberTcp);
		m_dstPortgtCustomNumberTcp.setIsUserConfigured(true);
		m_dstPortCustomNumberUdp.setUI32Value(dstPortCustomNumberUdp);
		m_dstPortCustomNumberUdp.setIsUserConfigured(true);
		m_dstPortltCustomNumberUdp.setUI32Value(dstPortltCustomNumberUdp);
		m_dstPortltCustomNumberUdp.setIsUserConfigured(true);
		m_dstPortgtCustomNumberUdp.setUI32Value(dstPortgtCustomNumberUdp);
		m_dstPortgtCustomNumberUdp.setIsUserConfigured(true);

		m_srcPortWellknownNumberTcp.setUI32Value(srcPortWellknownNumberTcp);
		m_srcPortWellknownNumberTcp.setIsUserConfigured(true);
		m_srcPortltWellknownNumberTcp.setUI32Value(srcPortltWellknownNumberTcp);
		m_srcPortltWellknownNumberTcp.setIsUserConfigured(true);
		m_srcPortgtWellknownNumberTcp.setUI32Value(srcPortgtWellknownNumberTcp);
		m_srcPortgtWellknownNumberTcp.setIsUserConfigured(true);

		m_dstPortWellknownNumberTcp.setUI32Value(dstPortWellknownNumberTcp);
		m_dstPortWellknownNumberTcp.setIsUserConfigured(true);
		m_dstPortltWellknownNumberTcp.setUI32Value(dstPortltWellknownNumberTcp);
		m_dstPortltWellknownNumberTcp.setIsUserConfigured(true);
		m_dstPortgtWellknownNumberTcp.setUI32Value(dstPortgtWellknownNumberTcp);
		m_dstPortgtWellknownNumberTcp.setIsUserConfigured(true);

		m_srcPortWellknownNumberUdp.setUI32Value(srcPortWellknownNumberUdp);
		m_srcPortWellknownNumberUdp.setIsUserConfigured(true);
		m_srcPortltWellknownNumberUdp.setUI32Value(srcPortltWellknownNumberUdp);
		m_srcPortltWellknownNumberUdp.setIsUserConfigured(true);
		m_srcPortgtWellknownNumberUdp.setUI32Value(srcPortgtWellknownNumberUdp);
		m_srcPortgtWellknownNumberUdp.setIsUserConfigured(true);

		m_dstPortWellknownNumberUdp.setUI32Value(dstPortWellknownNumberUdp);
		m_dstPortWellknownNumberUdp.setIsUserConfigured(true);
		m_dstPortltWellknownNumberUdp.setUI32Value(dstPortltWellknownNumberUdp);
		m_dstPortltWellknownNumberUdp.setIsUserConfigured(true);
		m_dstPortgtWellknownNumberUdp.setUI32Value(dstPortgtWellknownNumberUdp);
		m_dstPortgtWellknownNumberUdp.setIsUserConfigured(true);

		m_srcPortWellknownrangeLowerTcp.setUI32Value(srcPortWellknownrangeLowerTcp);
		m_srcPortWellknownrangeLowerTcp.setIsUserConfigured(true);
		m_srcPortWellknownrangeHigherTcp.setUI32Value(srcPortWellknownrangeHigherTcp);
		m_srcPortWellknownrangeHigherTcp.setIsUserConfigured(true);
		m_srcPortWellknownrangeLowerUdp.setUI32Value(srcPortWellknownrangeLowerUdp);
		m_srcPortWellknownrangeLowerUdp.setIsUserConfigured(true);
		m_srcPortWellknownrangeHigherUdp.setUI32Value(srcPortWellknownrangeHigherUdp);
		m_srcPortWellknownrangeHigherUdp.setIsUserConfigured(true);
		m_dstPortWellknownrangeLowerTcp.setUI32Value(dstPortWellknownrangeLowerTcp);
		m_dstPortWellknownrangeLowerTcp.setIsUserConfigured(true);
		m_dstPortWellknownrangeHigherTcp.setUI32Value(dstPortWellknownrangeHigherTcp);
		m_dstPortWellknownrangeHigherTcp.setIsUserConfigured(true);
		m_dstPortWellknownrangeLowerUdp.setUI32Value(dstPortWellknownrangeLowerUdp);
		m_dstPortWellknownrangeLowerUdp.setIsUserConfigured(true);
		m_dstPortWellknownrangeHigherUdp.setUI32Value(dstPortWellknownrangeHigherUdp);
		m_dstPortWellknownrangeHigherUdp.setIsUserConfigured(true);
		m_srcPortCustomrangeLowerTcp.setUI32Value(srcPortCustomrangeLowerTcp);
		m_srcPortCustomrangeLowerTcp.setIsUserConfigured(true);
		m_dstPortCustomrangeLowerTcp.setUI32Value(dstPortCustomrangeLowerTcp);
		m_dstPortCustomrangeLowerTcp.setIsUserConfigured(true);
		m_srcPortCustomrangeLowerUdp.setUI32Value(srcPortCustomrangeLowerUdp);
		m_srcPortCustomrangeLowerUdp.setIsUserConfigured(true);
		m_dstPortCustomrangeLowerUdp.setUI32Value(dstPortCustomrangeLowerUdp);
		m_dstPortCustomrangeLowerUdp.setIsUserConfigured(true);
		m_srcPortCustomrangeHigherTcp.setUI32Value(srcPortCustomrangeHigherTcp);
		m_srcPortCustomrangeHigherTcp.setIsUserConfigured(true);
		m_dstPortCustomrangeHigherTcp.setUI32Value(dstPortCustomrangeHigherTcp);
		m_dstPortCustomrangeHigherTcp.setIsUserConfigured(true);
		m_srcPortCustomrangeHigherUdp.setUI32Value(srcPortCustomrangeHigherUdp);
		m_srcPortCustomrangeHigherUdp.setIsUserConfigured(true);;
		m_dstPortCustomrangeHigherUdp.setUI32Value(dstPortCustomrangeHigherUdp);
		m_dstPortCustomrangeHigherUdp.setIsUserConfigured(true);
        m_customTos.setUI32Value(customTos);
        m_customTos.setIsUserConfigured(true);
        m_wellknownTos.setUI32Value(wellknownTos);
        m_wellknownTos.setIsUserConfigured(true);

		m_customPrecedence.setUI32Value(customPrecedence);
		m_customPrecedence.setIsUserConfigured(true);
		m_wellknownPrecedence.setUI32Value(wellknownPrecedence);
		m_wellknownPrecedence.setIsUserConfigured(true);

		m_customDscp.setUI32Value(customDscp);
		m_customDscp.setIsUserConfigured(true);
		m_wellknownDscp.setUI32Value(wellknownDscp);
		m_wellknownDscp.setIsUserConfigured(true);

    }

    ExtIp6AclRuleManagedObject::~ExtIp6AclRuleManagedObject ()
    {
    }

    string  ExtIp6AclRuleManagedObject::getClassName()
    {
        return ("ExtIp6AclRuleManagedObject");
    }

    void  ExtIp6AclRuleManagedObject::setupAttributesForPersistence()
    {
        IpAddrType	  	defaultValdstAddrType 			= IP_ADDR_TYPE_NONE;
       	PortOperator 	defaultValsrcDstPortOperator 	= PORTOPERATOR_UNKNOWN;

		Ip6AclRuleManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV6Address(&m_dstHostIpAddress, "dstHostIpAddress",
			ipv6_access_list_dst_host_ip));
        addPersistableAttribute (new AttributeIpV6AddressNetworkMask(&m_dstIpAddress, "dstIpAddress"));
//        addPersistableAttribute (new AttributeIpV6Address(&m_dstIpAddressMask, "dstIpAddressMask", ipv6_access_list_dst_mask));
        addPersistableAttribute (new AttributeUnion(&m_dstUnion,"dstUnion", ipv6_access_list_dst_host_any_dip));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_dstType), defaultValdstAddrType, "dstType"));
        addPersistableAttribute (new AttributeUnion(&m_ProtocolTypeUnion, "ProtocolTypeUnion",
			ipv6_access_list_protocol_type));
        addPersistableAttribute (new AttributeUI32((UI32 *)(&m_l3l4CustomProtoType), "l3l4CustomProtoType"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_l3l4WellknownProtoType), "l3l4WellknownProtoType"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcPortOperator), defaultValsrcDstPortOperator,
			"srcPortOperator", ipv6_access_list_sport));
        addPersistableAttribute (new AttributeUnion(&m_srcPortNumberTcpUnion, "srcPortNumberTcpUnion", 
			ipv6_access_list_sport_number_eq_neq_tcp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortltNumberTcpUnion, "srcPortltNumberTcpUnion", 
			ipv6_access_list_sport_number_lt_tcp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortgtNumberTcpUnion, "srcPortgtNumberTcpUnion", 
			ipv6_access_list_sport_number_gt_tcp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortNumberUdpUnion, "srcPortNumberUdpUnion",
			ipv6_access_list_sport_number_eq_neq_udp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortltNumberUdpUnion, "srcPortltNumberUdpUnion",
            ipv6_access_list_sport_number_lt_udp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortgtNumberUdpUnion, "srcPortgtNumberUdpUnion",
            ipv6_access_list_sport_number_gt_udp));
		addPersistableAttribute (new AttributeUnion(&m_srcPortrangeLowerTcpUnion, "srcPortrangeLowerTcpUnion",
			ipv6_access_list_sport_number_range_lower_tcp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortrangeLowerUdpUnion, "srcPortrangeLowerUdpUnion",
			ipv6_access_list_sport_number_range_lower_udp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortrangeHigherTcpUnion, "srcPortrangeHigherTcpUnion",
			ipv6_access_list_sport_number_range_higher_tcp));
        addPersistableAttribute (new AttributeUnion(&m_srcPortrangeHigherUdpUnion, "srcPortrangeHigherUdpUnion",
			ipv6_access_list_sport_number_range_higher_udp));

		/* source port  Custom number TCP */
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomNumberTcp, true, "srcPortCustomNumberTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortltCustomNumberTcp, true, "srcPortltCustomNumberTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortgtCustomNumberTcp, true, "srcPortgtCustomNumberTcp"));

        /* source port wellknown number TCP */
		addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownNumberTcp), true, "srcPortWellknownNumberTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortltWellknownNumberTcp), true, "srcPortltWellknownNumberTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortgtWellknownNumberTcp), true, "srcPortgtWellknownNumberTcp"));

		/* source port Custom number UDP */
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomNumberUdp, true, "srcPortCustomNumberUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortltCustomNumberUdp, true, "srcPortltCustomNumberUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortgtCustomNumberUdp, true, "srcPortgtCustomNumberUdp"));

		/* source port wellknown number UDP */
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownNumberUdp), true, "srcPortWellknownNumberUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortltWellknownNumberUdp), true, "srcPortltWellknownNumberUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortgtWellknownNumberUdp), true, "srcPortgtWellknownNumberUdp"));
        
		addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomrangeLowerTcp, true, "srcPortCustomrangeLowerTcp"));

        addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownrangeLowerTcp), true, "srcPortWellknownrangeLowerTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomrangeHigherTcp, true, "srcPortCustomrangeHigherTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownrangeHigherTcp), true,	"srcPortWellknownrangeHigherTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomrangeLowerUdp, true, "srcPortCustomrangeLowerUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownrangeLowerUdp), true, "srcPortWellknownrangeLowerUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_srcPortCustomrangeHigherUdp, true, "srcPortCustomrangeHigherUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_srcPortWellknownrangeHigherUdp), true,	"srcPortWellknownrangeHigherUdp"));
        addPersistableAttribute (new AttributeUnion(&m_dstPortNumberTcpUnion, "dstPortNumberTcpUnion",
            ipv6_access_list_dport_number_eq_neq_tcp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortltNumberTcpUnion, "dstPortltNumberTcpUnion",
            ipv6_access_list_dport_number_lt_tcp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortgtNumberTcpUnion, "dstPortgtNumberTcpUnion",
            ipv6_access_list_dport_number_gt_tcp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortNumberUdpUnion, "dstPortNumberUdpUnion",
            ipv6_access_list_dport_number_eq_neq_udp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortltNumberUdpUnion, "dstPortltNumberUdpUnion",
            ipv6_access_list_dport_number_lt_udp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortgtNumberUdpUnion, "dstPortgtNumberUdpUnion",
            ipv6_access_list_dport_number_gt_udp));

        addPersistableAttribute (new AttributeUnion(&m_dstPortrangeLowerTcpUnion, "dstPortrangeLowerTcpUnion",
			ipv6_access_list_dport_number_range_lower_tcp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortrangeLowerUdpUnion, "dstPortrangeLowerUdpUnion",
			ipv6_access_list_dport_number_range_lower_udp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortrangeHigherTcpUnion, "dstPortrangeHigherTcpUnion",
			ipv6_access_list_dport_number_range_higher_tcp));
        addPersistableAttribute (new AttributeUnion(&m_dstPortrangeHigherUdpUnion, "dstPortrangeHigherUdpUnion",
			ipv6_access_list_dport_number_range_higher_udp));
        
		addPersistableAttribute (new AttributeEnum((UI32 *)(&m_dstPortOperator), PORTOPERATOR_UNKNOWN, "dstPortOperator", ipv6_access_list_dport));

        /* destination port  Custom number TCP */
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortCustomNumberTcp, true, "dstPortCustomNumberTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortltCustomNumberTcp, true, "dstPortltCustomNumberTcp"));
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortgtCustomNumberTcp, true, "dstPortgtCustomNumberTcp"));

        /* destination port wellknown number TCP */
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownNumberTcp), true, "dstPortWellknownNumberTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortltWellknownNumberTcp), true, "dstPortltWellknownNumberTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortgtWellknownNumberTcp), true, "dstPortgtWellknownNumberTcp"));

        /* destination port Custom number UDP */
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortCustomNumberUdp, true, "dstPortCustomNumberUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortltCustomNumberUdp, true, "dstPortltCustomNumberUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortgtCustomNumberUdp, true, "dstPortgtCustomNumberUdp"));

        /* destination port wellknown number UDP */
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownNumberUdp), true, "dstPortWellknownNumberUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortltWellknownNumberUdp), true, "dstPortltWellknownNumberUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortgtWellknownNumberUdp), true, "dstPortgtWellknownNumberUdp"));

		addPersistableAttribute (new AttributeUI32UC(&m_dstPortCustomrangeLowerTcp, true, "dstPortCustomrangeLowerTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownrangeLowerTcp), true, "dstPortWellknownrangeLowerTcp"));
        addPersistableAttribute (new AttributeUI32UC((&m_dstPortCustomrangeHigherTcp), true, "dstPortCustomrangeHigherTcp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownrangeHigherTcp), true, "dstPortWellknownrangeHigherTcp"));
        addPersistableAttribute (new AttributeUI32UC((&m_dstPortCustomrangeLowerUdp), true, "dstPortCustomrangeLowerUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownrangeLowerUdp), true, "dstPortWellknownrangeLowerUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_dstPortCustomrangeHigherUdp, true, "dstPortCustomrangeHigherUdp"));
        addPersistableAttribute (new AttributeEnumUC((&m_dstPortWellknownrangeHigherUdp), true, "dstPortWellknownrangeHigherUdp"));
        addPersistableAttribute (new AttributeUI32UC(&m_customPrecedence, true, "customPrecedence"));
		addPersistableAttribute (new AttributeEnumUC(&m_wellknownPrecedence, true, "wellknownPrecedence"));
        addPersistableAttribute (new AttributeUI32UC(&m_customTos, true, "customTos"));
		addPersistableAttribute (new AttributeEnumUC(&m_wellknownTos, true, "wellknownTos"));
        addPersistableAttribute (new AttributeUI32UC(&m_customDscp, true, "customDscp"));
		addPersistableAttribute (new AttributeEnumUC(&m_wellknownDscp, true, "wellknownDscp"));
		//addPersistableAttribute (new AttributeUnion(&m_precedenceUnion, "precedenceUnion",ipv6_access_list_precedence));
		addPersistableAttribute (new AttributeUnion(&m_precedenceUnion, "precedenceUnion"));
		addPersistableAttribute (new AttributeUnion(&m_dscpUnion, "dscpUnion", ipv6_access_list_dscp));
		//addPersistableAttribute (new AttributeUnion(&m_tosUnion, "tosUnion", ipv6_access_list_tos));
		addPersistableAttribute (new AttributeUnion(&m_tosUnion, "tosUnion"));
        addPersistableAttribute (new AttributeBool(&m_urg, "urg", ipv6_access_list_urg));
        addPersistableAttribute (new AttributeBool(&m_ack, "ack", ipv6_access_list_ack));
        addPersistableAttribute (new AttributeBool(&m_push, "push", ipv6_access_list_push));
        addPersistableAttribute (new AttributeBool(&m_fin, "fin", ipv6_access_list_fin));
        addPersistableAttribute (new AttributeBool(&m_rst, "rst", ipv6_access_list_rst));
        addPersistableAttribute (new AttributeBool(&m_sync,"sync", ipv6_access_list_sync));
    }

    void  ExtIp6AclRuleManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeIpV6Address(&m_dstHostIpAddress, "dstHostIpAddress"));
        addPersistableAttributeForCreate  (new AttributeIpV6AddressNetworkMask(&m_dstIpAddress, "dstIpAddress"));
//        addPersistableAttributeForCreate  (new AttributeIpV6Address(&m_dstIpAddressMask, "dstIpAddressMask"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstUnion, "dstUnion"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_dstType), "dstType"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_ProtocolTypeUnion, "ProtocolTypeUnion"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_l3l4CustomProtoType, "l3l4CustomProtoType"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_l3l4WellknownProtoType), "l3l4WellknownProtoType"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_srcPortOperator), "srcPortOperator"));
		
		/* Src port Union eq-neq and lt and gt */        
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortNumberTcpUnion, "srcPortNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortltNumberTcpUnion, "srcPortltNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortgtNumberTcpUnion, "srcPortgtNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortNumberUdpUnion, "srcPortNumberUdpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortltNumberUdpUnion, "srcPortltNumberUdpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_srcPortgtNumberUdpUnion, "srcPortgtNumberUdpUnion"));
        
		addPersistableAttributeForCreate  (new AttributeUnion(&m_srcPortrangeLowerTcpUnion, "srcPortrangeLowerTcpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_srcPortrangeLowerUdpUnion, "srcPortrangeLowerUdpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_srcPortrangeHigherTcpUnion, "srcPortrangeHigherTcpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_srcPortrangeHigherUdpUnion, "srcPortrangeHigherUdpUnion"));
        
        /* source port  Custom number TCP */
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortCustomNumberTcp, true, "srcPortCustomNumberTcp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortltCustomNumberTcp, true, "srcPortltCustomNumberTcp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortgtCustomNumberTcp, true, "srcPortgtCustomNumberTcp"));

        /* source port wellknown number TCP */
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortWellknownNumberTcp), true, "srcPortWellknownNumberTcp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortltWellknownNumberTcp), true, "srcPortltWellknownNumberTcp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortgtWellknownNumberTcp), true, "srcPortgtWellknownNumberTcp"));

        /* source port Custom number UDP */
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortCustomNumberUdp, true, "srcPortCustomNumberUdp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortltCustomNumberUdp, true, "srcPortltCustomNumberUdp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_srcPortgtCustomNumberUdp, true, "srcPortgtCustomNumberUdp"));

        /* source port wellknown number UDP */
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortWellknownNumberUdp), true, "srcPortWellknownNumberUdp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortltWellknownNumberUdp), true, "srcPortltWellknownNumberUdp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_srcPortgtWellknownNumberUdp), true, "srcPortgtWellknownNumberUdp"));
	
	
		addPersistableAttributeForCreate  (new AttributeUI32UC(&m_srcPortCustomrangeLowerTcp, true, "srcPortCustomrangeLowerTcp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_srcPortWellknownrangeLowerTcp), true, "srcPortWellknownrangeLowerTcp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_srcPortCustomrangeHigherTcp, true, "srcPortCustomrangeHigherTcp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_srcPortWellknownrangeHigherTcp), true, "srcPortWellknownrangeHigherTcp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_srcPortCustomrangeLowerUdp, true, "srcPortCustomrangeLowerUdp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_srcPortWellknownrangeLowerUdp), true, "srcPortWellknownrangeLowerUdp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_srcPortCustomrangeHigherUdp, true, "srcPortCustomrangeHigherUdp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_srcPortWellknownrangeHigherUdp), true, "srcPortWellknownrangeHigherUdp"));
		/* Destination port eq-neq, lt and gt union */
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortNumberTcpUnion, "dstPortNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortltNumberTcpUnion, "dstPortltNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortgtNumberTcpUnion, "dstPortgtNumberTcpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortNumberUdpUnion, "dstPortNumberUdpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortltNumberUdpUnion, "dstPortltNumberUdpUnion"));
        addPersistableAttributeForCreate (new AttributeUnion(&m_dstPortgtNumberUdpUnion, "dstPortgtNumberUdpUnion"));
     
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstPortrangeLowerTcpUnion, "dstPortrangeLowerTcpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstPortrangeLowerUdpUnion, "dstPortrangeLowerUdpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstPortrangeHigherTcpUnion, "dstPortrangeHigherTcpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstPortrangeHigherUdpUnion, "dstPortrangeHigherUdpUnion"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_dstPortOperator), "dstPortOperator"));
        /* destination port  Custom number TCP */
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortCustomNumberTcp, true, "dstPortCustomNumberTcp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortltCustomNumberTcp, true, "dstPortltCustomNumberTcp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortgtCustomNumberTcp, true, "dstPortgtCustomNumberTcp"));

        /* destination port wellknown number TCP */
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortWellknownNumberTcp), true, "dstPortWellknownNumberTcp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortltWellknownNumberTcp), true, "dstPortltWellknownNumberTcp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortgtWellknownNumberTcp), true, "dstPortgtWellknownNumberTcp"));

        /* destination port Custom number UDP */
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortCustomNumberUdp, true, "dstPortCustomNumberUdp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortltCustomNumberUdp, true, "dstPortltCustomNumberUdp"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_dstPortgtCustomNumberUdp, true, "dstPortgtCustomNumberUdp"));

        /* destination port wellknown number UDP */
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortWellknownNumberUdp), true, "dstPortWellknownNumberUdp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortltWellknownNumberUdp), true, "dstPortltWellknownNumberUdp"));
        addPersistableAttributeForCreate (new AttributeEnumUC((&m_dstPortgtWellknownNumberUdp), true, "dstPortgtWellknownNumberUdp"));

        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_dstPortCustomrangeLowerTcp, true, "dstPortCustomrangeLowerTcp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_dstPortWellknownrangeLowerTcp), true, "dstPortWellknownrangeLowerTcp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_dstPortCustomrangeHigherTcp, true, "dstPortCustomrangeHigherTcp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_dstPortWellknownrangeHigherTcp), true, "dstPortWellknownrangeHigherTcp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_dstPortCustomrangeLowerUdp, true, "dstPortCustomrangeLowerUdp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_dstPortWellknownrangeLowerUdp), true, "dstPortWellknownrangeLowerUdp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_dstPortCustomrangeHigherUdp, true, "dstPortCustomrangeHigherUdp"));
        addPersistableAttributeForCreate  (new AttributeEnumUC((&m_dstPortWellknownrangeHigherUdp), true, "dstPortWellknownrangeHigherUdp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_customPrecedence,true, "customPrecedence"));
		addPersistableAttributeForCreate  (new AttributeEnumUC(&m_wellknownPrecedence,true, "wellknownPrecedence"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_customTos, true, "customTos"));
		addPersistableAttributeForCreate  (new AttributeEnumUC(&m_wellknownTos, true, "wellknownTos"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_customDscp, true,"customDscp"));
		addPersistableAttributeForCreate  (new AttributeEnumUC(&m_wellknownDscp, true, "wellknownDscp"));
		addPersistableAttributeForCreate  (new AttributeUnion(&m_precedenceUnion, "precedenceUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dscpUnion, "dscpUnion"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_tosUnion, "tosUnion"));

        addPersistableAttributeForCreate  (new AttributeBool(&m_urg, "urg"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_ack, "ack"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_push, "push"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_fin, "fin"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rst, "rst"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_sync, "sync"));
    }

    void  ExtIp6AclRuleManagedObject::setDstHostIpAddress(const IpV6Address &dstHostIpAddress)
    {
        m_dstHostIpAddress  =  dstHostIpAddress;
    }

    IpV6Address ExtIp6AclRuleManagedObject::getDstHostIpAddress() const
    {
        return (m_dstHostIpAddress);
    }

    void  ExtIp6AclRuleManagedObject::setDstIpAddress(const IpV6AddressNetworkMask &dstIpAddress)
    {
        m_dstIpAddress  =  dstIpAddress;
    }

    IpV6AddressNetworkMask ExtIp6AclRuleManagedObject::getDstIpAddress() const
    {
        return (m_dstIpAddress);
    }

/*
    void  ExtIp6AclRuleManagedObject::setDstIpAddressMask(const IpV6Address &dstIpAddressMask)
    {
        m_dstIpAddressMask  =  dstIpAddressMask;
    }

    IpV6Address  ExtIp6AclRuleManagedObject::getDstIpAddressMask() const
    {
        return (m_dstIpAddressMask);
    }
*/

    void  ExtIp6AclRuleManagedObject::setDstUnion(const WaveUnion &dstUnion)
    {
        m_dstUnion  =  dstUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstUnion() const
    {
        return (m_dstUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstType(const IpAddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    IpAddrType  ExtIp6AclRuleManagedObject::getDstType() const
    {
        return (m_dstType);
    }

    void  ExtIp6AclRuleManagedObject::setProtocolTypeUnion(const WaveUnion &ProtocolTypeUnion)
    {
        m_ProtocolTypeUnion  =  ProtocolTypeUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getProtocolTypeUnion() const
    {
        return (m_ProtocolTypeUnion);
    }

    void  ExtIp6AclRuleManagedObject::setL3l4CustomProtoType(const UI32 &l3l4CustomProtoType)
    {
		m_l3l4CustomProtoType  =  l3l4CustomProtoType;
    }

    UI32  ExtIp6AclRuleManagedObject::getL3l4CustomProtoType() const
    {
		return (m_l3l4CustomProtoType);
    }

    void  ExtIp6AclRuleManagedObject::setL3l4WellknownProtoType(const L3L4wellknownProtoType &l3l4WellknownProtoType)
    {
        m_l3l4WellknownProtoType  =  l3l4WellknownProtoType;
    }

    L3L4wellknownProtoType  ExtIp6AclRuleManagedObject::getL3l4WellknownProtoType() const
    {
        return (m_l3l4WellknownProtoType);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortOperator(const PortOperator &srcPortOperator)
    {
        m_srcPortOperator  =  srcPortOperator;
    }

    PortOperator  ExtIp6AclRuleManagedObject::getSrcPortOperator() const
    {
        return (m_srcPortOperator);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortNumberTcpUnion(const WaveUnion &srcPortNumberTcpUnion)
    {
        m_srcPortNumberTcpUnion  =  srcPortNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortNumberTcpUnion() const
    {
        return (m_srcPortNumberTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltNumberTcpUnion(const WaveUnion &srcPortltNumberTcpUnion)
    {
        m_srcPortltNumberTcpUnion  =  srcPortltNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortltNumberTcpUnion() const
    {
        return (m_srcPortltNumberTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtNumberTcpUnion(const WaveUnion &srcPortgtNumberTcpUnion)
    {
        m_srcPortgtNumberTcpUnion  =  srcPortgtNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortgtNumberTcpUnion() const
    {
        return (m_srcPortgtNumberTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortNumberUdpUnion(const WaveUnion &srcPortNumberUdpUnion)
    {
        m_srcPortNumberUdpUnion  =  srcPortNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortNumberUdpUnion() const
    {
        return (m_srcPortNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltNumberUdpUnion(const WaveUnion &srcPortltNumberUdpUnion)
    {
        m_srcPortltNumberUdpUnion  =  srcPortltNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortltNumberUdpUnion() const
    {
        return (m_srcPortltNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtNumberUdpUnion(const WaveUnion &srcPortgtNumberUdpUnion)
    {
        m_srcPortgtNumberUdpUnion  =  srcPortgtNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortgtNumberUdpUnion() const
    {
        return (m_srcPortgtNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortrangeLowerTcpUnion(const WaveUnion &srcPortrangeLowerTcpUnion)
    {
        m_srcPortrangeLowerTcpUnion  =  srcPortrangeLowerTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortrangeLowerTcpUnion() const
    {
        return (m_srcPortrangeLowerTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortrangeLowerUdpUnion(const WaveUnion &srcPortrangeLowerUdpUnion)
    {
        m_srcPortrangeLowerUdpUnion  =  srcPortrangeLowerUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortrangeLowerUdpUnion() const
    {
        return (m_srcPortrangeLowerUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortrangeHigherTcpUnion(const WaveUnion &srcPortrangeHigherTcpUnion)
    {
        m_srcPortrangeHigherTcpUnion  =  srcPortrangeHigherTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortrangeHigherTcpUnion() const
    {
        return (m_srcPortrangeHigherTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortrangeHigherUdpUnion(const WaveUnion &srcPortrangeHigherUdpUnion)
    {
        m_srcPortrangeHigherUdpUnion  =  srcPortrangeHigherUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getSrcPortrangeHigherUdpUnion() const
    {
        return (m_srcPortrangeHigherUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortCustomNumberTcp(const UI32 &srcPortCustomNumberTcp)
    {
        m_srcPortCustomNumberTcp.setUI32Value(srcPortCustomNumberTcp);
		if (srcPortCustomNumberTcp != DEFAULT_PORTNUM)
			m_srcPortCustomNumberTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomNumberTcp() const
    {
        return (m_srcPortCustomNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltCustomNumberTcp(const UI32 &srcPortltCustomNumberTcp)
    {
        m_srcPortltCustomNumberTcp.setUI32Value(srcPortltCustomNumberTcp);
		if (srcPortltCustomNumberTcp != DEFAULT_PORTNUM)
			m_srcPortltCustomNumberTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortltCustomNumberTcp() const
    {
        return (m_srcPortltCustomNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtCustomNumberTcp(const UI32 &srcPortgtCustomNumberTcp)
    {
        m_srcPortgtCustomNumberTcp.setUI32Value(srcPortgtCustomNumberTcp);
		if (srcPortgtCustomNumberTcp != DEFAULT_PORTNUM)
			m_srcPortgtCustomNumberTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortgtCustomNumberTcp() const
    {
        return (m_srcPortgtCustomNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownNumberTcp(const PortWellknownNumberTcp &srcPortWellknownNumberTcp)
    {
        m_srcPortWellknownNumberTcp.setUI32Value(srcPortWellknownNumberTcp);
		if (srcPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_srcPortWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getSrcPortWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_srcPortWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltWellknownNumberTcp(const PortWellknownNumberTcp &srcPortltWellknownNumberTcp)
    {
        m_srcPortltWellknownNumberTcp.setUI32Value(srcPortltWellknownNumberTcp);
		if (srcPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_srcPortltWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getSrcPortltWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_srcPortltWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtWellknownNumberTcp(const PortWellknownNumberTcp &srcPortgtWellknownNumberTcp)
    {
        m_srcPortgtWellknownNumberTcp.setUI32Value(srcPortgtWellknownNumberTcp);
		if (srcPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_srcPortgtWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getSrcPortgtWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_srcPortgtWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortCustomNumberUdp(const UI32 &srcPortCustomNumberUdp)
    {
        m_srcPortCustomNumberUdp.setUI32Value(srcPortCustomNumberUdp);
		if (srcPortCustomNumberUdp != DEFAULT_PORTNUM)
			m_srcPortCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomNumberUdp() const
    {
        return (m_srcPortCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltCustomNumberUdp(const UI32 &srcPortltCustomNumberUdp)
    {
        m_srcPortltCustomNumberUdp.setUI32Value(srcPortltCustomNumberUdp);
		if (srcPortltCustomNumberUdp != DEFAULT_PORTNUM)
			m_srcPortltCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortltCustomNumberUdp() const
    {
        return (m_srcPortltCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtCustomNumberUdp(const UI32 &srcPortgtCustomNumberUdp)
    {
        m_srcPortgtCustomNumberUdp.setUI32Value(srcPortgtCustomNumberUdp);
		if (srcPortgtCustomNumberUdp != DEFAULT_PORTNUM)
			m_srcPortgtCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortgtCustomNumberUdp() const
    {
        return (m_srcPortgtCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownNumberUdp(const PortWellknownNumberUdp &srcPortWellknownNumberUdp)
    {
        m_srcPortWellknownNumberUdp.setUI32Value(srcPortWellknownNumberUdp);
		if (srcPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getSrcPortWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_srcPortWellknownNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortltWellknownNumberUdp(const PortWellknownNumberUdp &srcPortltWellknownNumberUdp)
    {
        m_srcPortltWellknownNumberUdp.setUI32Value(srcPortltWellknownNumberUdp);
		if (srcPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortltWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getSrcPortltWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_srcPortltWellknownNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortgtWellknownNumberUdp(const PortWellknownNumberUdp &srcPortgtWellknownNumberUdp)
    {
        m_srcPortgtWellknownNumberUdp.setUI32Value(srcPortgtWellknownNumberUdp);
		if (srcPortgtWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortgtWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getSrcPortgtWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_srcPortgtWellknownNumberUdp.getUI32Value());
    }

	
    void  ExtIp6AclRuleManagedObject::setSrcPortCustomrangeLowerTcp(const UI32 &srcPortCustomrangeLowerTcp)
    {
        m_srcPortCustomrangeLowerTcp.setUI32Value(srcPortCustomrangeLowerTcp);
		if (srcPortCustomrangeLowerTcp != DEFAULT_PORTNUM)
			m_srcPortCustomrangeLowerTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomrangeLowerTcp() const
    {
        return (m_srcPortCustomrangeLowerTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownrangeLowerTcp(const PortWellknownNumberTcp &srcPortWellknownrangeLowerTcp)
    {
        m_srcPortWellknownrangeLowerTcp.setUI32Value(srcPortWellknownrangeLowerTcp);
		if (srcPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_srcPortWellknownrangeLowerTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getSrcPortWellknownrangeLowerTcp() const
    {
        return ((PortWellknownNumberTcp)m_srcPortWellknownrangeLowerTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortCustomrangeHigherTcp(const UI32 &srcPortCustomrangeHigherTcp)
    {
        m_srcPortCustomrangeHigherTcp.setUI32Value(srcPortCustomrangeHigherTcp);
		if (srcPortCustomrangeHigherTcp != DEFAULT_PORTNUM)
			m_srcPortCustomrangeHigherTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomrangeHigherTcp() const
    {
        return (m_srcPortCustomrangeHigherTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &srcPortWellknownrangeHigherTcp)
    {
        m_srcPortWellknownrangeHigherTcp.setUI32Value(srcPortWellknownrangeHigherTcp);
		if (srcPortWellknownrangeHigherTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_srcPortWellknownrangeHigherTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getSrcPortWellknownrangeHigherTcp() const
    {
        return ((PortWellknownNumberTcp)m_srcPortWellknownrangeHigherTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortCustomrangeLowerUdp(const UI32 &srcPortCustomrangeLowerUdp)
    {
        m_srcPortCustomrangeLowerUdp.setUI32Value(srcPortCustomrangeLowerUdp);
		if (srcPortCustomrangeLowerUdp != DEFAULT_PORTNUM)
			m_srcPortCustomrangeLowerUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomrangeLowerUdp() const
    {
        return (m_srcPortCustomrangeLowerUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &srcPortWellknownrangeLowerUdp)
    {
        m_srcPortWellknownrangeLowerUdp.setUI32Value(srcPortWellknownrangeLowerUdp);
		if (srcPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortWellknownrangeLowerUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getSrcPortWellknownrangeLowerUdp() const
    {
        return ((PortWellknownNumberUdp)m_srcPortWellknownrangeLowerUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortCustomrangeHigherUdp(const UI32 &srcPortCustomrangeHigherUdp)
    {
        m_srcPortCustomrangeHigherUdp.setUI32Value(srcPortCustomrangeHigherUdp);
		if (srcPortCustomrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortCustomrangeHigherUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getSrcPortCustomrangeHigherUdp() const
    {
        return (m_srcPortCustomrangeHigherUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setSrcPortWellknownrangeHigherUdp(const PortWellknownNumberUdp &srcPortWellknownrangeHigherUdp)
    {
        m_srcPortWellknownrangeHigherUdp.setUI32Value(srcPortWellknownrangeHigherUdp);
		if (srcPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_srcPortWellknownrangeHigherUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getSrcPortWellknownrangeHigherUdp() const
    {
        return ((PortWellknownNumberUdp)m_srcPortWellknownrangeHigherUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortNumberTcpUnion(const WaveUnion &dstPortNumberTcpUnion)
    {
        m_dstPortNumberTcpUnion  =  dstPortNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortNumberTcpUnion() const
    {
        return (m_dstPortNumberTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortltNumberTcpUnion(const WaveUnion &dstPortltNumberTcpUnion)
    {
        m_dstPortltNumberTcpUnion  =  dstPortltNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortltNumberTcpUnion() const
    {
        return (m_dstPortltNumberTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortgtNumberTcpUnion(const WaveUnion &dstPortgtNumberTcpUnion)
    {
        m_dstPortgtNumberTcpUnion  =  dstPortgtNumberTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortgtNumberTcpUnion() const
    {
        return (m_dstPortgtNumberTcpUnion);
    }


    void  ExtIp6AclRuleManagedObject::setDstPortNumberUdpUnion(const WaveUnion &dstPortNumberUdpUnion)
    {
        m_dstPortNumberUdpUnion  =  dstPortNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortNumberUdpUnion() const
    {
        return (m_dstPortNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortltNumberUdpUnion(const WaveUnion &dstPortltNumberUdpUnion)
    {
        m_dstPortltNumberUdpUnion  =  dstPortltNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortltNumberUdpUnion() const
    {
        return (m_dstPortltNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortgtNumberUdpUnion(const WaveUnion &dstPortgtNumberUdpUnion)
    {
        m_dstPortgtNumberUdpUnion  =  dstPortgtNumberUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortgtNumberUdpUnion() const
    {
        return (m_dstPortgtNumberUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortrangeLowerTcpUnion(const WaveUnion &dstPortrangeLowerTcpUnion)
    {
        m_dstPortrangeLowerTcpUnion  =  dstPortrangeLowerTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortrangeLowerTcpUnion() const
    {
        return (m_dstPortrangeLowerTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortrangeLowerUdpUnion(const WaveUnion &dstPortrangeLowerUdpUnion)
    {
        m_dstPortrangeLowerUdpUnion  =  dstPortrangeLowerUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortrangeLowerUdpUnion() const
    {
        return (m_dstPortrangeLowerUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortrangeHigherTcpUnion(const WaveUnion &dstPortrangeHigherTcpUnion)
    {
        m_dstPortrangeHigherTcpUnion  =  dstPortrangeHigherTcpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortrangeHigherTcpUnion() const
    {
        return (m_dstPortrangeHigherTcpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortrangeHigherUdpUnion(const WaveUnion &dstPortrangeHigherUdpUnion)
    {
        m_dstPortrangeHigherUdpUnion  =  dstPortrangeHigherUdpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDstPortrangeHigherUdpUnion() const
    {
        return (m_dstPortrangeHigherUdpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortOperator(const PortOperator &dstPortOperator)
    {
        m_dstPortOperator  =  dstPortOperator;
    }

    PortOperator  ExtIp6AclRuleManagedObject::getDstPortOperator() const
    {
        return (m_dstPortOperator);
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomNumberTcp(const UI32 &dstPortCustomNumberTcp)
    {
        m_dstPortCustomNumberTcp.setUI32Value(dstPortCustomNumberTcp);
		if (dstPortCustomNumberTcp != DEFAULT_PORTNUM)
			m_dstPortCustomNumberTcp.setIsUserConfigured(true);
		
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomNumberTcp() const
    {
        return (m_dstPortCustomNumberTcp.getUI32Value());
    }

   	void  ExtIp6AclRuleManagedObject::setDstPortltCustomNumberTcp(const UI32 &dstPortltCustomNumberTcp)
    {
        m_dstPortltCustomNumberTcp.setUI32Value(dstPortltCustomNumberTcp);
		if (dstPortltCustomNumberTcp != DEFAULT_PORTNUM)
			m_dstPortltCustomNumberTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortltCustomNumberTcp() const
    {
        return (m_dstPortltCustomNumberTcp.getUI32Value());
    }

   	void  ExtIp6AclRuleManagedObject::setDstPortgtCustomNumberTcp(const UI32 &dstPortgtCustomNumberTcp)
    {
        m_dstPortgtCustomNumberTcp.setUI32Value(dstPortgtCustomNumberTcp);
		if (dstPortgtCustomNumberTcp != DEFAULT_PORTNUM)
			m_dstPortgtCustomNumberTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortgtCustomNumberTcp() const
    {
        return (m_dstPortgtCustomNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownNumberTcp(const PortWellknownNumberTcp &dstPortWellknownNumberTcp)
    {
        m_dstPortWellknownNumberTcp.setUI32Value(dstPortWellknownNumberTcp);
		if (dstPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_dstPortWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getDstPortWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_dstPortWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortltWellknownNumberTcp(const PortWellknownNumberTcp &dstPortltWellknownNumberTcp)
    {
        m_dstPortltWellknownNumberTcp.setUI32Value(dstPortltWellknownNumberTcp);
		if (dstPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_dstPortltWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getDstPortltWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_dstPortltWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortgtWellknownNumberTcp(const PortWellknownNumberTcp &dstPortgtWellknownNumberTcp)
    {
        m_dstPortgtWellknownNumberTcp.setUI32Value(dstPortgtWellknownNumberTcp);
		if (dstPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_dstPortgtWellknownNumberTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getDstPortgtWellknownNumberTcp() const
    {
        return ((PortWellknownNumberTcp)m_dstPortgtWellknownNumberTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomNumberUdp(const UI32 &dstPortCustomNumberUdp)
    {
        m_dstPortCustomNumberUdp.setUI32Value(dstPortCustomNumberUdp);
		if (dstPortCustomNumberUdp != DEFAULT_PORTNUM)
			m_dstPortCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomNumberUdp() const
    {
        return (m_dstPortCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortltCustomNumberUdp(const UI32 &dstPortltCustomNumberUdp)
    {
        m_dstPortltCustomNumberUdp.setUI32Value(dstPortltCustomNumberUdp);
		if (dstPortltCustomNumberUdp != DEFAULT_PORTNUM)
			m_dstPortltCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortltCustomNumberUdp() const
    {
        return (m_dstPortltCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortgtCustomNumberUdp(const UI32 &dstPortgtCustomNumberUdp)
    {
        m_dstPortgtCustomNumberUdp.setUI32Value(dstPortgtCustomNumberUdp);
		if (dstPortgtCustomNumberUdp != DEFAULT_PORTNUM)
			m_dstPortgtCustomNumberUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortgtCustomNumberUdp() const
    {
        return (m_dstPortgtCustomNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownNumberUdp(const PortWellknownNumberUdp &dstPortWellknownNumberUdp)
    {
        m_dstPortWellknownNumberUdp.setUI32Value(dstPortWellknownNumberUdp);
		if (dstPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_dstPortWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getDstPortWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_dstPortWellknownNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortltWellknownNumberUdp(const PortWellknownNumberUdp &dstPortltWellknownNumberUdp)
    {
        m_dstPortltWellknownNumberUdp.setUI32Value(dstPortltWellknownNumberUdp);
		if (dstPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_dstPortltWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getDstPortltWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_dstPortltWellknownNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortgtWellknownNumberUdp(const PortWellknownNumberUdp &dstPortgtWellknownNumberUdp)
    {
        m_dstPortgtWellknownNumberUdp.setUI32Value(dstPortgtWellknownNumberUdp);
		if (dstPortgtWellknownNumberUdp  != WELLKNOWNPORT_UDP_UNKNOWN)
			m_dstPortgtWellknownNumberUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getDstPortgtWellknownNumberUdp() const
    {
        return ((PortWellknownNumberUdp)m_dstPortgtWellknownNumberUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomrangeLowerTcp(const UI32 &dstPortCustomrangeLowerTcp)
    {
        m_dstPortCustomrangeLowerTcp.setUI32Value(dstPortCustomrangeLowerTcp);
		if (dstPortCustomrangeLowerTcp != DEFAULT_PORTNUM)
			m_dstPortCustomrangeLowerTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomrangeLowerTcp() const
    {
        return (m_dstPortCustomrangeLowerTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownrangeLowerTcp(const PortWellknownNumberTcp &dstPortWellknownrangeLowerTcp)
    {
        m_dstPortWellknownrangeLowerTcp.setUI32Value(dstPortWellknownrangeLowerTcp);
		if (dstPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)
			m_dstPortWellknownrangeLowerTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getDstPortWellknownrangeLowerTcp() const
    {
        return ((PortWellknownNumberTcp)m_dstPortWellknownrangeLowerTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomrangeHigherTcp(const UI32 &dstPortCustomrangeHigherTcp)
    {
        m_dstPortCustomrangeHigherTcp.setUI32Value(dstPortCustomrangeHigherTcp);
		if (dstPortCustomrangeHigherTcp != DEFAULT_PORTNUM)
			m_dstPortCustomrangeHigherTcp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomrangeHigherTcp() const
    {
        return (m_dstPortCustomrangeHigherTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownrangeHigherTcp(const PortWellknownNumberTcp &dstPortWellknownrangeHigherTcp)
    {
        m_dstPortWellknownrangeHigherTcp.setUI32Value(dstPortWellknownrangeHigherTcp);
		if (dstPortWellknownrangeHigherTcp  != WELLKNOWNPORT_TCP_UNKNOWN)
			m_dstPortWellknownrangeHigherTcp.setIsUserConfigured(true);
    }

    PortWellknownNumberTcp  ExtIp6AclRuleManagedObject::getDstPortWellknownrangeHigherTcp() const
    {
        return ((PortWellknownNumberTcp)m_dstPortWellknownrangeHigherTcp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomrangeLowerUdp(const UI32 &dstPortCustomrangeLowerUdp)
    {
        m_dstPortCustomrangeLowerUdp.setUI32Value(dstPortCustomrangeLowerUdp);
		if (dstPortCustomrangeLowerUdp != DEFAULT_PORTNUM)
			m_dstPortCustomrangeLowerUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomrangeLowerUdp() const
    {
        return (m_dstPortCustomrangeLowerUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownrangeLowerUdp(const PortWellknownNumberUdp &dstPortWellknownrangeLowerUdp)
    {
        m_dstPortWellknownrangeLowerUdp.setUI32Value(dstPortWellknownrangeLowerUdp);
		if (dstPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_dstPortWellknownrangeLowerUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getDstPortWellknownrangeLowerUdp() const
    {
        return ((PortWellknownNumberUdp)m_dstPortWellknownrangeLowerUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortCustomrangeHigherUdp(const UI32 &dstPortCustomrangeHigherUdp)
    {
        m_dstPortCustomrangeHigherUdp.setUI32Value(dstPortCustomrangeHigherUdp);
		if (dstPortCustomrangeHigherUdp != DEFAULT_PORTNUM)
			m_dstPortCustomrangeHigherUdp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getDstPortCustomrangeHigherUdp() const
    {
        return (m_dstPortCustomrangeHigherUdp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDstPortWellknownrangeHigherUdp(const PortWellknownNumberUdp &dstPortWellknownrangeHigherUdp)
    {
        m_dstPortWellknownrangeHigherUdp.setUI32Value(dstPortWellknownrangeHigherUdp);
		if (dstPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)
			m_dstPortWellknownrangeHigherUdp.setIsUserConfigured(true);
    }

    PortWellknownNumberUdp  ExtIp6AclRuleManagedObject::getDstPortWellknownrangeHigherUdp() const
    {
        return ((PortWellknownNumberUdp)m_dstPortWellknownrangeHigherUdp.getUI32Value());
    }

	void  ExtIp6AclRuleManagedObject::setWellknownPrecedence (const WellknownPrecedence &wellknownPrecedence)
	{
		m_wellknownPrecedence.setUI32Value(wellknownPrecedence);
		if (m_wellknownPrecedence != WELLKNOWN_PRECEDENCE_UNKNOWN)
			m_wellknownPrecedence.setIsUserConfigured(true);
	}

	WellknownPrecedence ExtIp6AclRuleManagedObject::getWellknownPrecedence() const
	{
		return ((WellknownPrecedence)m_wellknownPrecedence.getUI32Value());
	}
	
	void ExtIp6AclRuleManagedObject::setCustomPrecedence (const UI32 &customPrecedence)
	{
		m_customPrecedence.setUI32Value(customPrecedence);
		if (m_customPrecedence != DEFAULT_PREC_DSCP_TOS_VAL)
			m_customPrecedence.setIsUserConfigured(true);
	}
	
    UI32  ExtIp6AclRuleManagedObject::getCustomPrecedence() const
    {
        return (m_customPrecedence.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setPrecedenceUnion(const WaveUnion &precedenceUnion)
    {
        m_precedenceUnion  =  precedenceUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getPrecedenceUnion() const
    {
        return (m_precedenceUnion);
    }

    void  ExtIp6AclRuleManagedObject::setWellknownTos (const WellknownTos &wellknownTos)
    {
        m_wellknownTos.setUI32Value( wellknownTos);
        if (wellknownTos != WELLKNOWN_TOS_UNKNOWN)
        	m_wellknownTos.setIsUserConfigured(true);
    }

    WellknownTos ExtIp6AclRuleManagedObject::getWellknownTos() const
    {
        return ((WellknownTos)m_wellknownTos.getUI32Value());
    }

    void ExtIp6AclRuleManagedObject::setCustomTos (const UI32 &customTos)
    {
        m_customTos.setUI32Value(customTos);
        if (customTos != DEFAULT_PREC_DSCP_TOS_VAL)
        	m_customTos.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getCustomTos() const
    {
        return (m_customTos.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setTosUnion(const WaveUnion &tosUnion)
    {
        m_tosUnion  =  tosUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getTosUnion() const
    {
        return (m_tosUnion);
    }

    void  ExtIp6AclRuleManagedObject::setWellknownDscp (const WellknownDscp &wellknownDscp)
    {
        m_wellknownDscp.setUI32Value(wellknownDscp);
		if (m_wellknownDscp != WELLKNOWN_DSCP_UNKNOWN)
			m_wellknownDscp.setIsUserConfigured(true);
    }

    WellknownDscp ExtIp6AclRuleManagedObject::getWellknownDscp() const
    {
        return ((WellknownDscp)m_wellknownDscp.getUI32Value());
    }

    void ExtIp6AclRuleManagedObject::setCustomDscp (const UI32 &customDscp)
    {
        m_customDscp.setUI32Value(customDscp);
		if (m_customDscp != DEFAULT_PREC_DSCP_TOS_VAL)
			m_customDscp.setIsUserConfigured(true);
    }

    UI32  ExtIp6AclRuleManagedObject::getCustomDscp() const
    {
        return (m_customDscp.getUI32Value());
    }

    void  ExtIp6AclRuleManagedObject::setDscpUnion(const WaveUnion &dscpUnion)
    {
        m_dscpUnion  =  dscpUnion;
    }

    WaveUnion  ExtIp6AclRuleManagedObject::getDscpUnion() const
    {
        return (m_dscpUnion);
    }

    void  ExtIp6AclRuleManagedObject::setUrg(const bool &urg)
    {
        m_urg  =  urg;
    }

    bool  ExtIp6AclRuleManagedObject::getUrg() const
    {
        return (m_urg);
    }

    void  ExtIp6AclRuleManagedObject::setAck(const bool &ack)
    {
        m_ack  =  ack;
    }

    bool  ExtIp6AclRuleManagedObject::getAck() const
    {
        return (m_ack);
    }

    void  ExtIp6AclRuleManagedObject::setPush(const bool &push)
    {
        m_push  =  push;
    }

    bool  ExtIp6AclRuleManagedObject::getPush() const
    {
        return (m_push);
    }


    void  ExtIp6AclRuleManagedObject::setFin(const bool &fin)
    {
        m_fin  =  fin;
    }

    bool  ExtIp6AclRuleManagedObject::getFin() const
    {
        return (m_fin);
    }

    void  ExtIp6AclRuleManagedObject::setRst(const bool &rst)
    {
        m_rst  =  rst;
    }

    bool  ExtIp6AclRuleManagedObject::getRst() const
    {
        return (m_rst);
    }

    void  ExtIp6AclRuleManagedObject::setSync(const bool &sync)
    {
        m_sync  =  sync;
    }

    bool  ExtIp6AclRuleManagedObject::getSync() const
    {
        return (m_sync);
    }

}
