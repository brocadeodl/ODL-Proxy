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
#include "ClientInterface/OSPF/OSPFShowMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <netinet/in.h>

using namespace WaveNs;

namespace DcmNs
{

    OSPFShowMessage::OSPFShowMessage ()
        : DcmManagementInterfaceMessage (OSPFSHOW)
    {
		m_summary = 0;
		m_cmdCode = 0;
        m_areaId = "";
		m_ipAddr1.fromString("0.0.0.0");
		m_ipAddr2.fromString("0.0.0.0");
		m_intVal = 0;
		m_strVal = "";
		m_ifName = "";
		m_moreDataFlag=0;
		m_vrf = "";
    }


    OSPFShowMessage::~OSPFShowMessage ()
    {
    }

    void  OSPFShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeBool(&m_summary, "summary"));
		addSerializableAttribute (new AttributeStringVector(&m_argv, "argv"));

		addSerializableAttribute (new AttributeString(&m_areaId, "areaId"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr1, "ipAddr1"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr2, "ipAddr2"));
		addSerializableAttribute (new AttributeUI32(&m_intVal, "intVal"));
		addSerializableAttribute (new AttributeString(&m_strVal, "strVal"));
		addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeUI32(&m_moreDataFlag, "moreDataFlag"));

		addSerializableAttribute (new AttributeString(&m_vrf, "vrf"));
    }

    int OSPFShowMessage::getArgc()
    {
        return (m_argv.size());
    }

    void  OSPFShowMessage::setArgv(char **argv, int argc)
    {
		string temp;

		m_argv.push_back("show");
		m_argv.push_back("ip");
		for (int i = 0; i < argc; ++i) {
			temp = argv[i];
			m_argv.push_back(temp);
		}

    }

    vector<string> OSPFShowMessage::getArgv()
    {
        return (m_argv);
    }

    void  OSPFShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  OSPFShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
  
    void OSPFShowMessage::setMoreDataFlag(const UI32 &moreDataFlag)
    {
        m_moreDataFlag  =  moreDataFlag;
    }

    UI32 OSPFShowMessage::getMoreDataFlag() const
    {
        return (m_moreDataFlag);
    }

	/*
	 * Obsolete  -- BEGIN
	 */
	void OSPFShowMessage::setSummary (const bool &summary)
	{
		m_summary = summary;
	}

	bool OSPFShowMessage::getSummary() const
	{
		return (m_summary);
	}

    void OSPFShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }



	/*
	 * Obsolete -- END
	 */

	void OSPFShowMessage::setVRF(const string &vrfName)
	{
		m_vrf = vrfName;
	}

	string OSPFShowMessage::getVRF() const
	{
		return (m_vrf);
	}

	void OSPFShowMessage::setAreaId(const string &areaId)
	{
		m_areaId = areaId;
	}

	string OSPFShowMessage::getAreaId() const
	{
		return m_areaId;
	}

	void OSPFShowMessage::setIpAddr1(const string &str)
	{
		m_ipAddr1.fromString(str);
	}

	IpV4Address OSPFShowMessage::getIpAddr1() const
	{
		return m_ipAddr1;
	}

	void OSPFShowMessage::setIpAddr2(const string &str)
	{
		m_ipAddr2.fromString(str);
	}

	IpV4Address OSPFShowMessage::getIpAddr2() const
	{
		return m_ipAddr2;
	}

	void OSPFShowMessage::setIntVal(const string &str)
	{
		m_intVal = atoi(str.c_str());
	}

	UI32 OSPFShowMessage::getIntVal() const
	{
		return m_intVal;
	}

	void OSPFShowMessage::setStrVal(const string &str)
	{
		m_strVal = str;
	}

	string OSPFShowMessage::getStrVal() const
	{
		return m_strVal;
	}

	void OSPFShowMessage::setIfname(const string &str)
	{
		m_ifName = str;
	}

	string OSPFShowMessage::getIfname() const
	{
		return m_ifName;
	}

	void OSPFShowMessage::setSequenceNumIntVal (char *hex_str)
	{

		m_intVal = (UI32 )strtol(hex_str, 0, 16);

	}
	// Convert info from C++ struct OSPFShowMessage populated in the APWorker to a C struct for backend
    const void *OSPFShowMessage::getCStructureForInputs ()
    {
        ospf_show_in_msg_t  *msg = new ospf_show_in_msg_t;
		struct in_addr ipAddr;
		void *show_pagehandle_data = NULL;
		char *myshow_pagehandle = NULL;	
		UI32 size =0;
       
		show_pagehandle_data = findBuffer(SHOW_OSPF_HANDLE_REQ_TAG, size);
                
		if (size > 0 && show_pagehandle_data) {
			msg->page_handle.size = size;
			myshow_pagehandle = (char *)calloc (1, size);

			if (NULL == myshow_pagehandle) {
				delete msg;
                                msg = NULL;
				return (NULL);
			}
			memcpy(myshow_pagehandle, show_pagehandle_data, size);
			msg->page_handle.data = myshow_pagehandle;
		}
		msg->data_pending = m_moreDataFlag;
		msg->cmdCode = (OSPFShowCmdCode_t) m_cmdCode;
		strncpy(msg->area_id, m_areaId.c_str(), OSPF_IPV4_ADDR_STR_SIZE);
		msg->intVal = m_intVal;
		strncpy(msg->strVal, m_strVal.c_str(), 64);
		strncpy(msg->if_name, m_ifName.c_str(), OSPF_DCM_IF_NAME_LEN);
		inet_pton(AF_INET, m_ipAddr1.toString().c_str(), &ipAddr);
		msg->ipaddr1 = ipAddr.s_addr;
		inet_pton(AF_INET, m_ipAddr2.toString().c_str(), &ipAddr);
		msg->ipaddr2 = ipAddr.s_addr;
		strcpy(msg->m_vrf ,m_vrf.c_str());

		/*
		 * Obsolete
		 */
		msg->cmd = (ospf_show_cmd_code_t) m_cmdCode;

		if (m_argv.size() > OSPF_CLI_MAX_ARGS)
                {
			delete msg;        
                        msg = NULL;        
			return 0;
                }
		msg->summary = m_summary;
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
    OSPFShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (!pOutputCStructure) return;

		ospf_show_out_msg_t *msg = (ospf_show_out_msg_t*)pOutputCStructure;
		
		addBuffer (OSPFSHOW, sizeof(ospf_show_out_msg_t) + msg->size, pOutputCStructure, false);
		/* Add buffer only if data_pending is set by backend */
		if (msg->data_pending) {
			addBuffer(SHOW_OSPF_HANDLE_RESP_TAG, msg->page_handle.size,
				(char *)(msg->page_handle.data), false);
		}
		setMoreDataFlag(msg->data_pending);
	}

    void  OSPFShowMessage::copyAllBuffers (const PrismMessage
          &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }


}

