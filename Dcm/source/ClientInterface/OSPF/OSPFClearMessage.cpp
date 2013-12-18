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
#include "ClientInterface/OSPF/OSPFClearMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"


namespace DcmNs
{

    OSPFClearMessage::OSPFClearMessage ()
        : DcmManagementInterfaceMessage (OSPFCLEAR)
    {
        m_cmd = 0;
        m_vrfName = "";
    }


    OSPFClearMessage::OSPFClearMessage (OSPFLocalClearMessage *pMsg)
        : DcmManagementInterfaceMessage (OSPFCLEAR) 
    {
         m_cmd = pMsg->m_cmd;
         m_argv = pMsg->m_argv;
         m_vrfName = pMsg->m_vrfName;
         m_ifName = pMsg->m_ifName;
    }
    OSPFClearMessage::~OSPFClearMessage ()
    {
    }

    void  OSPFClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeStringVector(&m_argv, "argv"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }

    void  OSPFClearMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  OSPFClearMessage::getCmdCode() const
    {
        return (m_cmd);
    }
	void  OSPFClearMessage::setIfName(const string &ifName)
	{
		m_ifName = ifName;
	}
	
	string  OSPFClearMessage::getIfName() const
	{
		return (m_ifName);
	}

	void  OSPFClearMessage::setVRF(const string &vrfName)
	{
		m_vrfName = vrfName;
	}
	
	string  OSPFClearMessage::getVRF() const
	{
		return (m_vrfName);
	}

    int OSPFClearMessage::getArgc()
    {
        return (m_argv.size());
    }

    void  OSPFClearMessage::setArgv(char **argv, int argc)
    {
        string temp;

        m_argv.push_back("clear");
        m_argv.push_back("ip");
        for (int i = 0; i < argc; ++i) {
            temp = argv[i];
            m_argv.push_back(temp);
        }
    }        

    vector<string> OSPFClearMessage::getArgv()
    {
        return (m_argv);
    }
    
    void OSPFClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

	// Convert info from C++ struct OSPFClearMessage populated in the APWorker to a C struct for backend
    const void *OSPFClearMessage::getCStructureForInputs ()
    {
        ospf_clear_in_msg_t  *msg = new ospf_clear_in_msg_t;

		msg->cmd = (ospf_clear_cmd_code_t) m_cmd;
		if(m_ifName != string(""))
	        strncpy(msg->if_name, m_ifName.c_str(), OSPF_DCM_IF_NAME_LEN);
		if(m_vrfName != string(""))
	            strncpy(msg->vrf_name, m_vrfName.c_str(), OSPF_DCM_IF_NAME_LEN);
        msg->argc = m_argv.size();
        for (int i = 0; i < OSPF_CLI_MAX_ARGS; ++i) {
		   if (i < msg->argc)
               msg->argv[i] = m_argv[i].c_str();
		   else
               msg->argv[i] = NULL;
		}
		return msg;
	}

	// Add C struct returned from backend into buffer so APWorker can process it
	void 
    OSPFClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (!pOutputCStructure) return;

        ospf_clear_out_msg_t *msg = ((ospf_clear_out_msg_t *)pOutputCStructure);
		if(msg->size)
			addBuffer (OSPFCLEAR, msg->size, pOutputCStructure, false);
		return;
	}
}

