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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpShowMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    ArpShowMessage::ArpShowMessage (const string &ifName,const UI32 &cmd, const IpV4Address &ip_addr)
        : DcmManagementInterfaceMessage (ARPSHOW),
        m_ifName    (ifName),
        m_cmd 	   	(cmd),
        m_ip_addr	(ip_addr)
    {
    }

    ArpShowMessage::ArpShowMessage(ArpShowLocalMessage *m)   
        : DcmManagementInterfaceMessage (ARPSHOW)
    {
        m_ifName = m->m_ifName;
        m_cmd = m->m_cmd;
        m_ip_addr = m->m_ip_addr;
        m_mac = m->m_mac;
        m_summary = m->m_summary;
        m_static_arp = m->m_static_arp;
        m_dynamic = m->m_dynamic;
        m_skip_count = m->m_skip_count;          
        m_last_printed = m->m_last_printed;          
        m_mappedId = DcmToolKit::getLocalMappedId();
    }

    ArpShowMessage::ArpShowMessage (const string &ifName,const UI32 &cmd, const IpV4Address &ip_addr, const string &mac, const bool &summary, const bool &static_arp, const bool &dynamic, const int &skip_count, const string &ip_mask, const int &last_printed)
        : DcmManagementInterfaceMessage (ARPSHOW),
        m_ifName    (ifName),
        m_cmd 	   	(cmd),
		m_ip_addr	(ip_addr),
		m_mac          (mac),
		m_summary      (summary),
		m_static_arp   (static_arp),
		m_dynamic      (dynamic),
		m_skip_count   (skip_count),
		m_ip_mask      (ip_mask),
		m_last_printed (last_printed)
    {
    }

}
