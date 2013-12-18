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
 *   Author : Sanjeev Joshi                                                *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Lldp/LldpShowMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/lldp_dcm_helper.h"


namespace DcmNs
{

    LldpShowMessage::LldpShowMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_SHOW)
    {
    }

    LldpShowMessage::LldpShowMessage (const string &ifName, 
                                      const UI32 &cmdCode, 
                                      const UI32 &mappedId,
                                      const UI32 &lastIfIndex,
                                      const UI32 &numRecords)
        : DcmManagementInterfaceMessage (MSG_LLDP_SHOW),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),
        m_mappedId   (mappedId),
        m_lastIfIndex(lastIfIndex),
        m_numRecords(numRecords)
    {
    }

    LldpShowMessage::~LldpShowMessage ()
    {
    }

    void  LldpShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute(new AttributeUI32(&m_lastIfIndex,"lastIfindex"));
        addSerializableAttribute(new AttributeUI32(&m_numRecords,"numRecords"));
    }

    void  LldpShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LldpShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  LldpShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  LldpShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  LldpShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }


    void LldpShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *LldpShowMessage::getCStructureForInputs ()
    {
        lldp_mgmt_req_t *req_p = (lldp_mgmt_req_t*) 
                                  calloc(1, sizeof(lldp_mgmt_req_t));
        switch(m_cmdCode) { 
            case LLDP_MGMT_CMD_SHOW_GBL: 
            {
                //nothing much to collect here
            }
            break;

            case LLDP_MGMT_CMD_SHOW_INTF:
            {   
                snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "te%s", m_ifName.c_str ());
            }
            break;
            case LLDP_MGMT_CMD_SHOW_INTF_GI:
            {   
                snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "gi%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_INTF;
            }
            break;
            case LLDP_MGMT_CMD_SHOW_INTF_FO:
            {   
                snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "fo%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_INTF;
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR:
            {
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR_DETL:
            {
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR_INTF:
            {
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "te%s", m_ifName.c_str ());
            }
            break;
            case LLDP_MGMT_CMD_SHOW_NBR_INTF_GI:
            {
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "gi%s", m_ifName.c_str ());
				m_cmdCode = LLDP_MGMT_CMD_SHOW_NBR_INTF;
            }
            break;
            case LLDP_MGMT_CMD_SHOW_NBR_INTF_FO:
            {
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "fo%s", m_ifName.c_str ());
				m_cmdCode = LLDP_MGMT_CMD_SHOW_NBR_INTF;
            }
            break;


            case LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL:
            {   
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.nbr.if_name, LLDP_IF_STR_MAX_LEN,
                        "te%s", m_ifName.c_str ()); 
            }
			break;
            case LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL_GI:
            {   
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.nbr.if_name, LLDP_IF_STR_MAX_LEN,
                        "gi%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL;
            }
			break;
            case LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL_FO:
            {   
            	req_p->entry.nbr.max_records = m_numRecords;
            	req_p->entry.nbr.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.nbr.if_name, LLDP_IF_STR_MAX_LEN,
                        "fo%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL;
            }
			break;

            case LLDP_MGMT_CMD_SHOW_STATS:
            {
            	req_p->entry.stats_intf.max_records = m_numRecords;
            	req_p->entry.stats_intf.last_ifindex = m_lastIfIndex;
            }
            break;

            case LLDP_MGMT_CMD_SHOW_STATS_INTF:
            {   
            	req_p->entry.stats_intf.max_records = m_numRecords;
            	req_p->entry.stats_intf.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.stats_intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "te%s", m_ifName.c_str ()); 
            }
            break;
            case LLDP_MGMT_CMD_SHOW_STATS_INTF_GI:
            {   
            	req_p->entry.stats_intf.max_records = m_numRecords;
            	req_p->entry.stats_intf.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.stats_intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "gi%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_STATS_INTF;
            }
            break;
            case LLDP_MGMT_CMD_SHOW_STATS_INTF_FO:
            {   
            	req_p->entry.stats_intf.max_records = m_numRecords;
            	req_p->entry.stats_intf.last_ifindex = m_lastIfIndex;
            	snprintf(req_p->entry.stats_intf.if_name, LLDP_IF_STR_MAX_LEN,
                        "fo%s", m_ifName.c_str ()); 
				m_cmdCode = LLDP_MGMT_CMD_SHOW_STATS_INTF;
            }
            break;

            default: 
            {
            }
        }
        req_p->cmd_code = (lldp_mgmt_cmd_code_t) m_cmdCode;

        return req_p;
    }

	void 
    LldpShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        lldp_mgmt_buffer_num_t  buff_num;

        memset(&buff_num, 0, sizeof(buff_num));


        switch (m_cmdCode) { 
            case LLDP_MGMT_CMD_SHOW_GBL:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_GBL;

                lldp_mgmt_gbl_resp_ntoh((lldp_mgmt_gbl_resp_t*) 
                                        pOutputCStructure);

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_gbl_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);
            }
            break;

            case LLDP_MGMT_CMD_SHOW_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_INTF;

                lldp_mgmt_show_intf_resp_ntoh((lldp_mgmt_show_intf_resp_t*) 
                                                pOutputCStructure);
                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_show_intf_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_NBR;
                lldp_mgmt_show_nbr_detail_resp_t    *nbr_resp_p = 
                    (lldp_mgmt_show_nbr_detail_resp_t*) pOutputCStructure;

                lldp_mgmt_show_nbr_detail_resp_ntoh(nbr_resp_p, FALSE_NTOH);

                addBuffer(buff_num.num, 
                          ntohl(nbr_resp_p->len), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR_DETL:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_NBR_DETL;
                lldp_mgmt_show_nbr_detail_resp_t    *nbr_dtl_resp_p = 
                    (lldp_mgmt_show_nbr_detail_resp_t*) pOutputCStructure;

                lldp_mgmt_show_nbr_detail_resp_ntoh(nbr_dtl_resp_p, FALSE_NTOH);

                addBuffer(buff_num.num, 
                          ntohl(nbr_dtl_resp_p->len), 
                          (unsigned char *)pOutputCStructure, 
                           false);                  
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_NBR_INTF;
                lldp_mgmt_show_nbr_detail_resp_t    *nbr_intf_resp_p = 
                    (lldp_mgmt_show_nbr_detail_resp_t*) pOutputCStructure;

                lldp_mgmt_show_nbr_detail_resp_ntoh(nbr_intf_resp_p, FALSE_NTOH);

                addBuffer(buff_num.num, 
                          ntohl(nbr_intf_resp_p->len), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_SHOW_NBR_INTF_DETL:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_NBR_INTF_DETL;

                lldp_mgmt_show_nbr_detail_resp_t    *nbr_intf_dtl_resp_p = 
                    (lldp_mgmt_show_nbr_detail_resp_t*) pOutputCStructure;

                lldp_mgmt_show_nbr_detail_resp_ntoh(nbr_intf_dtl_resp_p, FALSE_NTOH);

                addBuffer(buff_num.num, 
                          ntohl(nbr_intf_dtl_resp_p->len), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;            
            
            case LLDP_MGMT_CMD_SHOW_STATS:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_STATS;

                lldp_mgmt_show_stats_resp_t *resp_p = 
                    (lldp_mgmt_show_stats_resp_t*) pOutputCStructure;

                lldp_mgmt_show_stats_resp_ntoh(resp_p, FALSE_NTOH);

                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_show_stats_resp_t) + 
                          (( ntohl(resp_p->num_records) - 1)* 
                           sizeof(lldp_mgmt_show_stats_intf_t)), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_SHOW_STATS_INTF:
            {
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = LLDP_MGMT_SHOW_BUF_STATS_INTF;

                lldp_mgmt_show_stats_resp_ntoh(
                        (lldp_mgmt_show_stats_resp_t*)pOutputCStructure, 
                        FALSE_NTOH);


                addBuffer(buff_num.num, 
                          sizeof(lldp_mgmt_show_stats_resp_t), 
                          (unsigned char *)pOutputCStructure, 
                           false);                
            }
            break;

            case LLDP_MGMT_CMD_UNKNOWN:
            {

            }
            break;

            default:
            {
            }
        }
           
	}

	UI32 LldpShowMessage::getLastIfIndex() const
	{
	    return m_lastIfIndex;
	}

	UI32 LldpShowMessage::getNumRecords() const
	{
	    return m_numRecords;
	}

	void LldpShowMessage::setLastIfIndex(const UI32 lastIfIndex)
	{
	    m_lastIfIndex = lastIfIndex;
	}

	void LldpShowMessage::setNumRecords(const UI32 numRecords)
	{
	    m_numRecords = numRecords;
	}
}
