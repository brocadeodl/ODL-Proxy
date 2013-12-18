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
 *   Author : Manju Mahajan                                                *
 **************************************************************************/

#include "ClientInterface/TrillOam/TrillOamTraceRouteMessage.h"
#include "TrillOam/Common/TrillOamPluginTypes.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    TrillOamTraceRouteMessage::TrillOamTraceRouteMessage ()
        : DcmManagementInterfaceMessage (TRILLOAMTRACEROUTE)
    {
    }

  TrillOamTraceRouteMessage::TrillOamTraceRouteMessage (const MacAddress2 &srcMac,const MacAddress2 &dstMac, const UI32 &vlan, const UI32 &rbridgeid,
			const string &protocol, const IpV4Address &srcIp, const IpV4Address &dstIp, 
			const UI32 &srcPort, const UI32 &dstPort,
			const UI32 &cmdCode, const UI32 &sessionid, const UI32 &begin, const UI32 &last) : DcmManagementInterfaceMessage (TRILLOAMTRACEROUTE),
        m_srcMac    (srcMac),
        m_dstMac    (dstMac),
        m_vlan    (vlan),
        m_rbridgeid (rbridgeid),
        m_protocol    (protocol),
        m_srcIp    (srcIp),
        m_dstIp    (dstIp),
        m_srcPort    (srcPort),
        m_dstPort    (dstPort),
        m_cmdCode    (cmdCode),
        m_sessionid  (sessionid),
        m_begin    (begin),
        m_last    (last)
    {
    }

    TrillOamTraceRouteMessage::~TrillOamTraceRouteMessage ()
    {
    }

    void  TrillOamTraceRouteMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeMacAddress2(&m_srcMac,"srcMac"));
        addSerializableAttribute (new AttributeMacAddress2(&m_dstMac,"dstMac"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeString(&m_protocol,"protocol"));
        addSerializableAttribute (new AttributeIpV4Address(&m_srcIp,"srcIp"));
        addSerializableAttribute (new AttributeIpV4Address(&m_dstIp,"dstIp"));
        addSerializableAttribute (new AttributeUI32(&m_srcPort,"srcPort"));
        addSerializableAttribute (new AttributeUI32(&m_dstPort,"dstPort"));
        //addSerializableAttribute (new AttributeUI16(&m_trillDa,"trillDa"));
        //addSerializableAttribute (new AttributeUI16(&m_Egid,"egid"));
		//addSerializableAttribute (new AttributeString(m_Ingress,"ingress"));
		//addSerializableAttribute (new AttributeString(m_Egress,"egress"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_sessionid,"sessionid"));
        addSerializableAttribute (new AttributeUI32(&m_begin,"begin"));
        addSerializableAttribute (new AttributeUI32(&m_last,"last"));
    }


    void  TrillOamTraceRouteMessage::setSrcMac(const MacAddress2 &srcMac)
    {
        m_srcMac  =  srcMac;
    }

    MacAddress2  TrillOamTraceRouteMessage::getSrcMac() const
    {
        return (m_srcMac);
    }

    /*UI16  TrillOamTraceRouteMessage::getTrillDa() const
    {
        return (m_trillDa);
    }

    UI16  TrillOamTraceRouteMessage::getEgid() const
    {
        return (m_Egid);
    }

	string  TrillOamTraceRouteMessage::getIngress() const
	{
	    return (m_Ingress);
	}
	void  TrillOamTraceRouteMessage::SetIngress( const string &ingress)
    {
	    m_Ingress = ingress ;
	}
	
    string  TrillOamTraceRouteMessage::getEgress() const
    {
	    return (m_Egress);
	}
	void  TrillOamTraceRouteMessage::SetEgress( const string &egress)
    {
	    m_Egress = egress ;
    }*/

    void  TrillOamTraceRouteMessage::setDstMac(const MacAddress2 &dstMac)
    {
        m_dstMac  =  dstMac;
    }

    MacAddress2  TrillOamTraceRouteMessage::getDstMac() const
    {
        return (m_dstMac);
    }


    void  TrillOamTraceRouteMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  TrillOamTraceRouteMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  TrillOamTraceRouteMessage::setProtocol(const string &protocol)
    {
        m_protocol  =  protocol;
    }

    string  TrillOamTraceRouteMessage::getProtocol() const
    {
        return (m_protocol);
    }

    void  TrillOamTraceRouteMessage::setSrcIp(const IpV4Address &srcIp)
    {
        m_srcIp  =  srcIp;
    }

    IpV4Address  TrillOamTraceRouteMessage::getSrcIp() const
    {
        return (m_srcIp);
    }

    void  TrillOamTraceRouteMessage::setDstIp(const IpV4Address &dstIp)
    {
        m_dstIp  =  dstIp;
    }

    IpV4Address  TrillOamTraceRouteMessage::getDstIp() const
    {
        return (m_dstIp);
    }


    void  TrillOamTraceRouteMessage::setSrcPort(const UI32 &srcPort)
    {
        m_srcPort  =  srcPort;
    }

    UI32  TrillOamTraceRouteMessage::getSrcPort() const
    {
        return (m_srcPort);
    }


    void  TrillOamTraceRouteMessage::setDstPort(const UI32 &dstPort)
    {
        m_dstPort  =  dstPort;
    }

    UI32  TrillOamTraceRouteMessage::getDstPort() const
    {
        return (m_dstPort);
    }

	
    void  TrillOamTraceRouteMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  TrillOamTraceRouteMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  TrillOamTraceRouteMessage::SetSessionId(const UI32 &sessionid)
    {
        m_sessionid  =  sessionid;
    }

    UI32  TrillOamTraceRouteMessage::getSessionId() const
    {
        return (m_sessionid);
    }

	void  TrillOamTraceRouteMessage::setBegin(const UI32 &begin)
	{
		m_begin  =  begin;
	}

	UI32  TrillOamTraceRouteMessage::getBegin() const
	{
		return (m_begin);
	}

	void  TrillOamTraceRouteMessage::setMsgStatus(const UI32 &last)
	{
		m_last  =  last;
	}

	UI32  TrillOamTraceRouteMessage::getMsgStatus() const
	{
		return (m_last);
	}
	/*void   TrillOamTraceRouteMessage::getBackendData(UI32 &size, void *&pBuffer) 
	{
		pBuffer = findBuffer(TRILLOAMTRACEROUTE, size);
		return ;
	}*/

   void TrillOamTraceRouteMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
    	trilloam_msg_traceroute_resp *resp_p = (trilloam_msg_traceroute_resp *)pOutputCStructure;
    	if (!resp_p) 
	    { 
	    	return; 
    	}
        int size = sizeof(trilloam_msg_traceroute_resp) + 
            (resp_p->row_count - 1) * sizeof(trilloam_msg_traceroute_resp_row);
	    addBuffer (TRILLOAMTRACEROUTE, size, pOutputCStructure, false);
	}

/* it is part of backend/daemon, cli->backend convert CPP-to_C */
	const void * TrillOamTraceRouteMessage::getCStructureForInputs()
	{
		trilloam_msg_traceroute   *msg_p = new trilloam_msg_traceroute;
		msg_p->begin = m_begin;
        msg_p->sessionid = m_sessionid;
		msg_p->last = m_last;
        if (m_begin) {
    		string my_mac = m_srcMac.toString();
		    strncpy(msg_p->src_mac, my_mac.c_str(), TRILLOAM_MAX_MAC_STR);
	    	my_mac = m_dstMac.toString();
    		strncpy((char *)msg_p->dst_mac, my_mac.c_str(), TRILLOAM_MAX_MAC_STR);
		    strncpy((char *)msg_p->proto, m_protocol.c_str(), TRILLOAM_MAX_PROTO_STR);
    		msg_p->vlanid = m_vlan;
	    	msg_p->src_port = m_srcPort;
		    msg_p->dst_port = m_dstPort;
    		string my_ip = m_srcIp.toString();
		    strncpy((char *)msg_p->src_ip, my_ip.c_str(), TRILLOAM_MAX_IP_STR);
    		my_ip = m_dstIp.toString();
		    strncpy((char *)msg_p->dst_ip, my_ip.c_str(), TRILLOAM_MAX_IP_STR);
        }
		return msg_p;
	}

}
