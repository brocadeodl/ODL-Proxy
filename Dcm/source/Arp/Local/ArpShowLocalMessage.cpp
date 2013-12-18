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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpShowLocalMessage.h"
#include "Arp/Local/ArpTypes.h"

namespace DcmNs
{

    ArpShowLocalMessage::ArpShowLocalMessage ()
        : PrismMessage (ArpLocalObjectManager::getPrismServiceId (), ARPSHOWLOCAL)
    {
    }
    ArpShowLocalMessage::ArpShowLocalMessage(ArpShowLocalMessage *m)
        : PrismMessage (ArpLocalObjectManager::getPrismServiceId (), ARPSHOWLOCAL)
    {
        m_ifName = m->m_ifName;
        m_mac = m->m_mac;
        m_summary = m->m_summary;
        m_static_arp = m->m_static_arp;
        m_dynamic = m->m_dynamic;
        m_cmd = m->m_cmd;
        m_ip_addr = m->m_ip_addr;
        m_skip_count = m->m_skip_count;
        m_last_printed = m->m_last_printed;
    }
    
    ArpShowLocalMessage::ArpShowLocalMessage(const string &ifName,const UI32 &cmd, const IpV4Address &ip_addr, const string &mac, const bool &summary, const bool &static_arp, const bool &dynamic, const int &skip_count, const string &ip_mask, const int &last_printed)
        : PrismMessage (ArpLocalObjectManager::getPrismServiceId (), ARPSHOWLOCAL)
	{
        m_ifName    = ifName;
        m_cmd       =cmd;
        m_ip_addr   =ip_addr;
        m_mac          =mac;
        m_summary      =summary;
        m_static_arp   =static_arp;
        m_dynamic      =dynamic;
        m_skip_count   =skip_count;
        m_ip_mask      =ip_mask;
        m_last_printed =last_printed;
    
    }

	ArpShowLocalMessage::~ArpShowLocalMessage ()
    {
    }
    void  ArpShowLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeBool(&m_summary, "summary"));	
		addSerializableAttribute (new AttributeBool(&m_dynamic, "dynamic"));
		addSerializableAttribute (new AttributeBool(&m_static_arp, "static_arp"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		addSerializableAttribute (new AttributeSI32(&m_skip_count,"skip_count"));
		addSerializableAttribute (new AttributeString(&m_ip_mask, "ip_mask"));
		addSerializableAttribute (new AttributeSI32(&m_last_printed,"last_printed"));
        addSerializableAttribute (new AttributeBool(&m_is_distribution_required, "is_distribution_required"));
        addSerializableAttribute (new AttributeUI32Vector(&m_locs, "locs"));
    }

    void  ArpShowLocalMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  ArpShowLocalMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  ArpShowLocalMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  ArpShowLocalMessage::getCmdCode() const
    {
        return (m_cmd);
    }
    void  ArpShowLocalMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  ArpShowLocalMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }
	void ArpShowLocalMessage::setMac(const string &mac)
	{
		m_mac = mac;
	}
	string ArpShowLocalMessage::getMac() const
	{
		return (m_mac);
	}
	void ArpShowLocalMessage::setSummary (const bool &summary)
	{
		m_summary = summary;
	}
	bool ArpShowLocalMessage::getSummary	() const
	{
		return (m_summary);
	}
	void ArpShowLocalMessage::setStatic_arp(const bool &static_arp)
	{
		m_static_arp = static_arp;
	}
	bool ArpShowLocalMessage::getStatic_arp() const
	{
		return (m_static_arp);
	}
	void ArpShowLocalMessage::setDynamic(const bool &dynamic)
	{
		m_dynamic = dynamic;
	}
	bool ArpShowLocalMessage::getDynamic( ) const
	{
		return (m_dynamic);
	}
	void ArpShowLocalMessage::setSkipCount(const int &skip_count)
	{
		m_skip_count = skip_count;
	}
	int ArpShowLocalMessage::getSkipCount( ) const
	{
		return (m_skip_count);
	}
	void ArpShowLocalMessage::setIpMask(const string &ip_mask)
	{
		m_ip_mask = ip_mask;
	}
	string ArpShowLocalMessage::getIpMask() const
	{
		return (m_ip_mask);
	}
    void ArpShowLocalMessage::setLastPrinted(const int &last_printed)
    {
        m_last_printed = last_printed;
    }
    int ArpShowLocalMessage::getLastPrinted( ) const
    {
        return (m_last_printed);
    }
    void ArpShowLocalMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    void ArpShowLocalMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
}
