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
#include "ClientInterface/Nsm/NsmShowVlanClassifierMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    NsmShowVlanClassifierMessage::NsmShowVlanClassifierMessage ()
        : DcmManagementInterfaceMessage (NSM_SHOW_VLAN_CLASSIFIER)
    {
		m_cmdCode = VLANCLASSIFIER_SHOW_UNKNOWN;
		m_backEndData = " ";
		m_mappedId = 0;
		m_lastIndex = 0;
    }

    NsmShowVlanClassifierMessage::NsmShowVlanClassifierMessage (const string &backEndData,const UI32 &cmdCode,const UI32 &mappedId)
        : DcmManagementInterfaceMessage (NSM_SHOW_VLAN_CLASSIFIER),
        m_backEndData    (backEndData),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
		m_lastIndex = 0;
    }

    NsmShowVlanClassifierMessage::~NsmShowVlanClassifierMessage ()
    {
    }

    void  NsmShowVlanClassifierMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_backEndData,"backEndData"));
		addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeUI32(&m_lastIndex,"lastIndex"));
    }

    void  NsmShowVlanClassifierMessage::setIfName(const string &backEndData)
    {
        m_backEndData  =  backEndData;
    }

    string  NsmShowVlanClassifierMessage::getIfName() const
    {
        return (m_backEndData);
    }

    void  NsmShowVlanClassifierMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmShowVlanClassifierMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmShowVlanClassifierMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  NsmShowVlanClassifierMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  NsmShowVlanClassifierMessage::setLastIndex(const UI32 &lastIndex)
    {
        m_lastIndex = lastIndex;
    }

    UI32  NsmShowVlanClassifierMessage::getLastIndex() const
    {
        return (m_lastIndex);
    }


    void NsmShowVlanClassifierMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *NsmShowVlanClassifierMessage::getCStructureForInputs ()
    {
        VlanClassifierShow_InputMsg_t_ *pInput = new VlanClassifierShow_InputMsg_t_;
        strncpy (pInput->back_end_data, m_backEndData.c_str (), 64);
		pInput->back_end_data[63] = '\0';
        pInput->cmdCode     = m_cmdCode;
		pInput->last_rule_id = m_lastIndex;
		pInput->last_group_id = m_lastIndex;

        return (pInput);
    }

	void NsmShowVlanClassifierMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        int no_of_records = 0 ;
		nsm_mgmt_buffer_num_t  buff_num = {{0}};

		if(pOutputCStructure != NULL) {
			VlanClassifierShow_Cmd_t *msg = (VlanClassifierShow_Cmd_t*)pOutputCStructure;
			no_of_records = msg->no_of_records;
		}	
		switch(m_cmdCode)
		{

			case VLANCLASSIFIER_SHOW_RULE :
			case VLANCLASSIFIER_SHOW_RULE_ALL:
			case VLANCLASSIFIER_SHOW_GROUP:
			case VLANCLASSIFIER_SHOW_GROUP_ALL:
			case VLANCLASSIFIER_SHOW_INTF_TE:
			case VLANCLASSIFIER_SHOW_INTF_PO:
			case VLANCLASSIFIER_SHOW_INTF_GROUP:
			case VLANCLASSIFIER_SHOW_INTF_GROUP_ALL:
				if(no_of_records == 0 )
					return;
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = VLANCLASSIFIER_SHOW;
				addBuffer (buff_num.num, ( (sizeof(VlanClassifierShow_Cmd_t) ) + ((no_of_records -1) * sizeof(VlanClassifierShow_OutputMsg_t))), ( ((unsigned char *)pOutputCStructure )), false);
				break;
			default:	
			    break;
		}
	}
}
