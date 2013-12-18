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
 *   Author : mmahajan                                                     *
 **************************************************************************/

#include "TrillOam/Local/TrillOamLocalObjectManager.h"
#include "TrillOam/Local/TrillOamTrillOamTracerouteRequestMessage.h"
#include "TrillOam/Local/TrillOamTypes.h"
#include "Framework/Messaging/Local/PrismMessage.h"

namespace DcmNs
{

    TrillOamTrillOamTracerouteRequestMessage::TrillOamTrillOamTracerouteRequestMessage ()
        :ManagementInterfaceMessage(TrillOamLocalObjectManager::getClassName (),TRILLOAMTRILLOAMTRACEROUTEREQUEST)
    {
    }

    TrillOamTrillOamTracerouteRequestMessage::TrillOamTrillOamTracerouteRequestMessage (const MacAddress2 &srcmac,const MacAddress2 &destmac,const UI32 &vlanid,const UI32 &rbridgeid,const string &protocol,const IpV4Address &srcip,const IpV4Address &destip,const UI32 &srcport,const UI32 &destport, const UI32 &sessionid, const UI32 &begin, const UI32 &last)
        :ManagementInterfaceMessage(TrillOamLocalObjectManager::getClassName (),TRILLOAMTRILLOAMTRACEROUTEREQUEST),
        m_srcmac    (srcmac),
        m_destmac   (destmac),
    	m_vlanid    (vlanid),
        m_rbridgeid (rbridgeid),
        m_protocol  (protocol),
        m_srcip     (srcip),
        m_destip    (destip),
        m_srcport   (srcport),
        m_destport  (destport),
        m_sessionid (sessionid),
        m_begin     (begin),
        m_last      (last)
    {
    }

    TrillOamTrillOamTracerouteRequestMessage::~TrillOamTrillOamTracerouteRequestMessage ()
    {
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setupAttributesForSerialization()
    {
		ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeMacAddress2(&m_srcmac,"srcmac"));
        addSerializableAttribute (new AttributeMacAddress2(&m_destmac,"destmac"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_protocol,"protocol"));
        addSerializableAttribute (new AttributeIpV4Address(&m_srcip,"srcip"));
        addSerializableAttribute (new AttributeIpV4Address(&m_destip,"destip"));
        addSerializableAttribute (new AttributeUI32(&m_srcport,"srcport"));
        addSerializableAttribute (new AttributeUI32(&m_destport,"destport"));
        //addSerializableAttribute (new AttributeUI16(&m_trillDa,"trillDa"));
        //addSerializableAttribute (new AttributeUI16(&m_Egid,"egid"));
		//addSerializableAttribute (new AttributeString(&m_Ingress,"ingress"));
		//addSerializableAttribute (new AttributeString(&m_Egress,"egress"));
        //addSerializableAttribute (new AttributeUI32(&m_Rttime,"rttime"));
        addSerializableAttribute (new AttributeUI32(&m_sessionid,"sessionid"));
        addSerializableAttribute (new AttributeUI32(&m_begin,"begin"));
		addSerializableAttribute (new AttributeUI32(&m_last,"last"));
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setSrcmac(const MacAddress2 &srcmac)
    {
        m_srcmac  =  srcmac;
    }

    MacAddress2  TrillOamTrillOamTracerouteRequestMessage::getSrcmac() const
    {
        return (m_srcmac);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setDestmac(const MacAddress2 &destmac)
    {
        m_destmac  =  destmac;
    }

    MacAddress2  TrillOamTrillOamTracerouteRequestMessage::getDestmac() const
    {
        return (m_destmac);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setProtocol(const string &protocol)
    {
        m_protocol  =  protocol;
    }

    string  TrillOamTrillOamTracerouteRequestMessage::getProtocol() const
    {
        return (m_protocol);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setSrcip(const IpV4Address &srcip)
    {
        m_srcip  =  srcip;
    }

    IpV4Address  TrillOamTrillOamTracerouteRequestMessage::getSrcip() const
    {
        return (m_srcip);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setDestip(const IpV4Address &destip)
    {
        m_destip  =  destip;
    }

    IpV4Address  TrillOamTrillOamTracerouteRequestMessage::getDestip() const
    {
        return (m_destip);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setSrcport(const UI32 &srcport)
    {
        m_srcport  =  srcport;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getSrcport() const
    {
        return (m_srcport);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setDestport(const UI32 &destport)
    {
        m_destport  =  destport;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getDestport() const
    {
        return (m_destport);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::SetSessionId(const UI32 &sessionid)
    {
        m_sessionid = sessionid;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getSessionId() const
    {
        return (m_sessionid);
    }

    void  TrillOamTrillOamTracerouteRequestMessage::setBegin(const UI32 &begin)
    {
        m_begin  =  begin;
    }

    UI32  TrillOamTrillOamTracerouteRequestMessage::getBegin() const
    {
        return (m_begin);
    }

    void   TrillOamTrillOamTracerouteRequestMessage::setMsgStatus(const UI32 &last)
    {
        m_last  =  last;
    }
    UI32  TrillOamTrillOamTracerouteRequestMessage::getMsgStatus() const
    {
        return (m_last);
    }

    /*UI16  TrillOamTrillOamTracerouteRequestMessage::getTrillDa(UI16 result_index) const
    {
        return (m_result[result_index].m_trillDa);
    }
    void  TrillOamTrillOamTracerouteRequestMessage::SetTrillDa(UI16 result_index, const UI16 &trillDa) 
    {
        m_result[result_index].m_trillDa = trillDa ;
    }

    UI16  TrillOamTrillOamTracerouteRequestMessage::getEgid(UI16 result_index) const
    {
        return (m_result[result_index].m_Egid);
    }
    void  TrillOamTrillOamTracerouteRequestMessage::SetEgid(UI16 result_index, const UI16 &egid)
    {
        m_result[result_index].m_Egid = egid ; 
    }
    
	string  TrillOamTrillOamTracerouteRequestMessage::getIngress(UI16 result_index) const
    {
        return (m_result[result_index].m_Ingress);
    }
    void  TrillOamTrillOamTracerouteRequestMessage::SetIngress(UI16 result_index, const string &ingress)
    {
        m_result[result_index].m_Ingress = ingress ; 
    }
    
	string  TrillOamTrillOamTracerouteRequestMessage::getEgress(UI16 result_index) const
    {
		return (m_result[result_index].m_Egress);
    }
    void  TrillOamTrillOamTracerouteRequestMessage::SetEgress(UI16 result_index, const string &egress)
    {
        m_result[result_index].m_Egress = egress ;
    }
    
	UI32  TrillOamTrillOamTracerouteRequestMessage::getRttime(UI16 result_index) const
    {
        return (m_result[result_index].m_Rttime);
    }
    void  TrillOamTrillOamTracerouteRequestMessage::SetRttime(UI16 result_index, const UI32 &rttime)
    {
        m_result[result_index].m_Rttime = rttime ; 
    }*/

    void TrillOamTrillOamTracerouteRequestMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void TrillOamTrillOamTracerouteRequestMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
}
