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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/OSPF/OSPFDebugMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"


namespace DcmNs
{

    OSPFDebugMessage::OSPFDebugMessage ()
        : DcmManagementInterfaceMessage (OSPFDEBUG)
    {
        m_cmd = 0;
        m_vrfName= "";
    }


    OSPFDebugMessage::~OSPFDebugMessage ()
    {
    }

    void  OSPFDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeStringVector(&m_argv, "argv"));
	addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
		#if 0
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeBool(&m_dynamic, "dynamic"));
		addSerializableAttribute (new AttributeBool(&m_static_arp, "static_arp"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		#endif
    }

    void  OSPFDebugMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  OSPFDebugMessage::getCmdCode() const
    {
        return (m_cmd);
    }

    int OSPFDebugMessage::getArgc()
    {
        return (m_argv.size());
    }

    void  OSPFDebugMessage::setArgv(char **argv, int argc)
    {
        string temp;

        m_argv.push_back("debug");
        m_argv.push_back("ip");
        for (int i = 0; i < argc; ++i) {
            temp = argv[i];
            m_argv.push_back(temp);
        }
    }        

    void  OSPFDebugMessage::setNoArgv(char **argv, int argc)
    {
        string temp;

        m_argv.push_back("no");
        m_argv.push_back("debug");
        m_argv.push_back("ip");
        for (int i = 0; i < argc; ++i) {
            temp = argv[i];
            m_argv.push_back(temp);
        }
    }        
    
    vector<string> OSPFDebugMessage::getArgv()
    {
        return (m_argv);
    }
    
    void OSPFDebugMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

    void  OSPFDebugMessage::setVRF(const string &vrfName)
    {
        m_vrfName = vrfName;
    }

    string  OSPFDebugMessage::getVRF() const
    {
       return (m_vrfName);
    }

#if 0
    void OSPFDebugMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
#endif

	// Convert info from C++ struct OSPFDebugMessage populated in the APWorker to a C struct for backend
    const void *OSPFDebugMessage::getCStructureForInputs ()
    {
        ospf_debug_in_msg_t  *msg = new ospf_debug_in_msg_t;

		msg->cmd = (ospf_debug_cmd_code_t) m_cmd;
             if(m_vrfName != string(""))
                strncpy(msg->vrf_name, m_vrfName.c_str(), OSPF_DCM_IF_NAME_LEN);

        msg->argc = m_argv.size();

        for (int i = 0; i < msg->argc; ++i)
           msg->argv[i] = m_argv[i].c_str();

#if 0
		string str_ipaddr;
		msg->cmd = m_cmd;
		msg->static_arps = m_static_arp;
		msg->dynamic = m_dynamic;
		msg->summary = m_summary;	
		if(msg->cmd == SHOW_ARP_TE_IF_CONFIG ||
			msg->cmd == SHOW_ARP_PO_IF_CONFIG || msg->cmd == SHOW_ARP_VLAN_IF_CONFIG)
			strncpy((char *)(msg->if_name), (const char*)(m_ifName.c_str()), ARP_DCM_IF_NAME_LEN);
		if(msg->cmd == SHOW_ARP_IP_CONFIG) {
				str_ipaddr = m_ip_addr.toString();
				strncpy(msg->ip, str_ipaddr.c_str(), 15);
				msg->ip[15] = '\0';
		}
		if(msg->cmd == SHOW_ARP_MAC_CONFIG) {
			str_ipaddr = m_mac;
			strncpy(msg->mac, str_ipaddr.c_str(), 15);
			msg->mac[14] = '\0';	
		}
#endif
        return msg;
    }

	// Add C struct returned from backend into buffer so APWorker can process it
	void 
    OSPFDebugMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (!pOutputCStructure) return;

        ospf_debug_out_msg_t *msg = ((ospf_debug_out_msg_t *)pOutputCStructure);

		if(msg->size !=0)
			addBuffer (OSPFDEBUG, msg->size, pOutputCStructure, false);

		return;
	}
}

