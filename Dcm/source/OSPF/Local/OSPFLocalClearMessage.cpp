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
#include "OSPF/Local/OSPFLocalClearMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"


namespace DcmNs
{

    OSPFLocalClearMessage::OSPFLocalClearMessage ()
        :PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFLOCALCLEAR)
// DcmManagementInterfaceMessage (OSPFLOCALCLEAR)
    {
        m_cmd = 0;
        m_vrfName = "";
        m_isDistributed = false;
    }

 OSPFLocalClearMessage::OSPFLocalClearMessage (OSPFLocalClearMessage * pMsg)
        :PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFLOCALCLEAR)
    {
        m_cmd = pMsg->getCmdCode();
        m_vrfName = pMsg->getVRF();
        m_isDistributed = pMsg->getisDistributionRequired();
        m_argv = pMsg->getArgv();
        m_ifName = pMsg->getIfName();
    }



    OSPFLocalClearMessage::~OSPFLocalClearMessage ()
    {
    }

    void  OSPFLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeStringVector(&m_argv, "argv"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeBool(&m_isDistributed,"isDistributed"));
    }

    void  OSPFLocalClearMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  OSPFLocalClearMessage::getCmdCode() const
    {
        return (m_cmd);
    }
	void  OSPFLocalClearMessage::setIfName(const string &ifName)
	{
		m_ifName = ifName;
	}
	
	string  OSPFLocalClearMessage::getIfName() const
	{
		return (m_ifName);
	}

	void  OSPFLocalClearMessage::setVRF(const string &vrfName)
	{
		m_vrfName = vrfName;
	}
	
	string  OSPFLocalClearMessage::getVRF() const
	{
		return (m_vrfName);
	}

    int OSPFLocalClearMessage::getArgc()
    {
        return (m_argv.size());
    }

    void  OSPFLocalClearMessage::setArgv(char **argv, int argc)
    {
        string temp;

        m_argv.push_back("clear");
        m_argv.push_back("ip");
        for (int i = 0; i < argc; ++i) {
            temp = argv[i];
            m_argv.push_back(temp);
        }
    }        

    vector<string> OSPFLocalClearMessage::getArgv()
    {
        return (m_argv);
    }
    
    void OSPFLocalClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }
    bool  OSPFLocalClearMessage::getisDistributionRequired() const
    {
            return(m_isDistributed);
    }
    void  OSPFLocalClearMessage::setisDistributionRequired(const bool&  isDistributed)
    {
        m_isDistributed = isDistributed;
    }
 
#if 0
	// Convert info from C++ struct OSPFLocalClearMessage populated in the APWorker to a C struct for backend
    const void *OSPFLocalClearMessage::getCStructureForInputs ()
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
    OSPFLocalClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (!pOutputCStructure) return;

        ospf_clear_out_msg_t *msg = ((ospf_clear_out_msg_t *)pOutputCStructure);
		if(msg->size)
			addBuffer (OSPFCLEAR, msg->size, pOutputCStructure, false);
		return;
	}
#endif
}

