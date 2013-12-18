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
 *   Author : skbalasu                                                     *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmShowIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmShowIntfMessage::NsmShowIntfMessage ()
        : PoPaginationMessage (NSM_SHOW_INTERFACE)
    {
		m_reqNoRecords = INTF_PER_PAGE;
		m_reqStartIndex = "-1";
		m_reqEndIndex = "-1";
		m_reqIfType = IF_TYPE_INVALID;
		m_reqRequestType = 0;
    }

    NsmShowIntfMessage::NsmShowIntfMessage (const string &ifName,const UI32 &cmdCode,const UI32 &mappedId)
        : PoPaginationMessage (NSM_SHOW_INTERFACE),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
		m_reqNoRecords = INTF_PER_PAGE;
		m_reqStartIndex = "-1";
		m_reqEndIndex = "-1";
		m_reqIfType = IF_TYPE_INVALID;
		m_reqRequestType = 0;
    }

    NsmShowIntfMessage::~NsmShowIntfMessage ()
    {
    }

    void  NsmShowIntfMessage::setupAttributesForSerialization()
    {
    	PoPaginationMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
		addSerializableAttribute (new AttributeUI32(&m_reqNoRecords,"reqNoRecords"));
        addSerializableAttribute (new AttributeString(&m_reqStartIndex,"reqStartIndex"));
        addSerializableAttribute (new AttributeString(&m_reqEndIndex,"reqEndIndex"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_reqIfType),"reqIfType"));
		addSerializableAttribute (new AttributeUI32(&m_reqRequestType,"reqRequestType"));
    }

    void  NsmShowIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmShowIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmShowIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmShowIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmShowIntfMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  NsmShowIntfMessage::getMappedId() const
    {
        return (m_mappedId);
    }

	void NsmShowIntfMessage::setReqNoRecords(const UI32 &reqNoRecords)
	{
		m_reqNoRecords = reqNoRecords;
	}

	UI32 NsmShowIntfMessage::getReqNoRecords()  const
	{
		return m_reqNoRecords;
	}

	void NsmShowIntfMessage::setReqStartIndex(const string &reqStartIndex)
	{
		m_reqStartIndex = reqStartIndex;
	}

	string NsmShowIntfMessage::getReqStartIndex()  const
	{
		return m_reqStartIndex;
	}

	void NsmShowIntfMessage::setReqEndIndex(const string &reqEndIndex)
	{
		m_reqEndIndex = reqEndIndex;
	}

	string NsmShowIntfMessage::getReqEndIndex()  const
	{
		return m_reqEndIndex;
	}

	void NsmShowIntfMessage::setReqIfType(const InterfaceType &reqIfType)
	{
		m_reqIfType = reqIfType;
	}

	InterfaceType NsmShowIntfMessage::getReqIfType()  const
	{
		return m_reqIfType;
	}

	void NsmShowIntfMessage::setReqRequestType(const UI32 &reqRequestType)
	{
		m_reqRequestType = reqRequestType;
	}

	UI32 NsmShowIntfMessage::getReqRequestType()  const
	{
		return m_reqRequestType;
	}


    void NsmShowIntfMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *NsmShowIntfMessage::getCStructureForInputs ()
    {
        NsmShowIntf_InputMsg_t *pInput = new NsmShowIntf_InputMsg_t;
//		printf("ifname %s cmdcode %d\n", m_ifName.c_str (), m_cmdCode);
        strncpy (pInput->ifName, m_ifName.c_str (), 64);
		pInput->ifName[63] = '\0';
        pInput->cmdCode     = m_cmdCode;
		pInput->pageReq.no_of_records = m_reqNoRecords;
        strncpy (pInput->pageReq.startIndex, m_reqStartIndex.c_str (), 63);
		pInput->pageReq.startIndex[63] = '\0';
        strncpy (pInput->pageReq.endIndex, m_reqEndIndex.c_str (), 63);
		pInput->pageReq.endIndex[63] = '\0';
		pInput->pageReq.ifType = m_reqIfType;
		pInput->pageReq.requestType = m_reqRequestType;
        pInput->startId = m_startPoId;
        pInput->endId = m_endPoId;

        return (pInput);
    }

	void NsmShowIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
//		NSM_PLUG_DBG("Entering...");
        int no_of_records = 0 ;
        
        if(pOutputCStructure != NULL)
            no_of_records = *((int *)pOutputCStructure);
        
		nsm_mgmt_buffer_num_t  buff_num = {{0}};

//		NSM_PLUG_DBG_S(string(" No of REc") + no_of_records);

		switch(m_cmdCode)
		{
		    case NSMSHOW_ALL:
			{
                
                if(no_of_records == 0 )
                    break;

				NSM_PLUG_DBG("cmdCode = NSMSHOW_ALL");
				buff_num.buff.mapped_id = this->getMappedId();
			    buff_num.buff.tag = NSMSHOW_INTERFACE;
                //addBuffer (buff_num.num, (sizeof(NsmShowIntf_OutputMsg_t) * no_of_records), ( ((unsigned char *)pOutputCStructure )+ sizeof(int)), false);
				addBuffer (buff_num.num, ( (sizeof(NsmShowIntf_OutputMsg_t) * no_of_records) +  sizeof(int) + sizeof(NsmPaginationResp_t)) , ( ((unsigned char *)pOutputCStructure )), false);
				NsmShowIntf_OutputMsg_t *p = (NsmShowIntf_OutputMsg_t*)pOutputCStructure;
				string ifName = p->ifName;
		//		NSM_PLUG_DBG_S(string("client ifName =") + ifName);
			}	
			break;

	        case (NSMSHOW_IP_INTF):
				NSM_PLUG_DBG("Loading outputs for show ip int");
				if (no_of_records <= 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_IP_INTERFACE;
				//addBuffer(buff_num.num, (sizeof (NsmShowIntf_OutputMsg_t) * no_of_records),  (((unsigned char *) pOutputCStructure) + sizeof(int)), false);
				addBuffer (buff_num.num, ( (sizeof(NsmShowIntf_OutputMsg_t) * no_of_records) +  sizeof(int) + sizeof(NsmPaginationResp_t)) , ( ((unsigned char  *)pOutputCStructure )), false);
				break;
	        
			case (NSMSHOW_PORTCHANNEL):
			{
				NSM_PLUG_DBG("Loading outputs for show port-channel");
				if (no_of_records < 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_PORTCHANNEL;
				addBuffer(buff_num.num, (sizeof(nsm_portchannel_show_msg_t)+ ((no_of_records - 1) * sizeof (nsm_portchannel_show_info_t))),
						  pOutputCStructure, false);
				break;
			}
	        
			case (NSMSHOW_PORTCHANNEL_DETAIL):
			{
				NSM_PLUG_DBG("Loading outputs for show port-channel (detail|summary)");
				if (no_of_records < 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_PORTCHANNEL_DETAIL;
				addBuffer(buff_num.num, (sizeof(nsm_portchannel_show_msg_t)+ ((no_of_records - 1) * sizeof (nsm_portchannel_show_info_t))),
						  pOutputCStructure, false);
				break;
			}

			case (NSMSHOW_PORTCHANNEL_LOADBALANCE):
			{
				NSM_PLUG_DBG("Loading outputs for show port-channel load-balance");
				if (no_of_records < 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_PORTCHANNEL_LOADBALANCE;
				addBuffer(buff_num.num, (sizeof(nsm_po_loadbalance_all_msg_t)+ ((no_of_records - 1) * sizeof (nsm_po_loadbalance_info_t))),
						  pOutputCStructure, false);
				break;
			}

			case (NSMSHOW_PORTCHANNEL_ID):
			{
				NSM_PLUG_DBG("Loading outputs for show port-channel po_num");
				if (no_of_records <= 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_PORTCHANNEL_ID;
				addBuffer(buff_num.num, (sizeof(nsm_portchannel_show_msg_t)+ ((no_of_records - 1) * sizeof (nsm_portchannel_show_info_t))),
						  pOutputCStructure, false);
				break;
			}

            case (NSMSHOW_VLAN_VLANID):
            trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs:NSMSHOW_VLAN_VLANID"));

            case NSMSHOW_VLAN_BRIEF: 
			case NSMSHOW_REMOTE_SPAN_VLAN: 
            case NSMSHOW_VLAN_BRIEF_PROVISIONED:
            case NSMSHOW_VLAN_BRIEF_UNPROVISIONED: {
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs: Number of records : <") +  no_of_records + ">\n");
                int len = 0;

                if (no_of_records <= 0) {
                    break;
                }

                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = NSMSHOW_VLAN_BR;

                len = ((NsmShowVlanBr_OutputMsg_t*)pOutputCStructure)->bufferSize;
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs:bufferSize<") + len + ">\n");

                addBuffer(buff_num.num, len, pOutputCStructure, false);
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs:bufferSize<") + len + "> added to buffer\n");                    
                break;
            }
	

			case (NSMSHOW_VLAN_PVLAN): {
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs: Number of records : <") +  no_of_records + ">\n");
                int len = 0;

                if (no_of_records <= 0) {
                    break;
                }

                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = NSMSHOW_VLAN_PVLAN;

                len = ((NsmShowVlanPVlanOutputMsg_t*)pOutputCStructure)->bufferSize;
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs:bufferSize<") + len + ">\n");

                addBuffer(buff_num.num, len, pOutputCStructure, false);
                trace (TRACE_LEVEL_DEBUG, string("NsmShowIntfMessage::getCStructureForInputs:bufferSize<") + len + "> added to buffer\n");
                break;
			}
														
			case (NSMSHOW_ALL_PORTCHANNEL_IDS): {
				NSM_PLUG_DBG("NsmShowIntfMessage::loadOutputsFromCStructure. Loading all port-channel Ids across cluster");
				if (no_of_records <= 0)
					break;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = NSMSHOW_ALL_PORTCHANNEL_IDS;
				addBuffer(buff_num.num, (sizeof(nsm_show_all_portchannel_ids_msg_t) + ((no_of_records - 1) * sizeof (nsm_show_poid_output_msg_t))), pOutputCStructure, false);
				break;
			}

		    default :
			{
			    NSM_PLUG_DBG_S(string("Unknown cmd code") + m_cmdCode);
			}
			break;
		}

		//NSM_PLUG_DBG("#####Done");
                
	}
}
