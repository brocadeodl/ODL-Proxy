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
 *   Author : bvreddy	                                                   *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmPortMediaShowMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    NsmPortMediaShowIntfMessage::NsmPortMediaShowIntfMessage ()
        : DcmManagementInterfaceMessage (NSM_PORTMEDIA_SHOW_INTERFACE)
    {
    }

    NsmPortMediaShowIntfMessage::NsmPortMediaShowIntfMessage (const string &ifTypeName, const string &ifName,const UI32 &slotId, const UI32 &cmdCode,const UI32 &mappedId)
        : DcmManagementInterfaceMessage (NSM_PORTMEDIA_SHOW_INTERFACE),
        m_ifTypeName    (ifTypeName),
        m_ifName    (ifName),
        m_slotId   (slotId),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
    }


    NsmPortMediaShowIntfMessage::~NsmPortMediaShowIntfMessage ()
    {
    }

    void  NsmPortMediaShowIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeString(&m_ifTypeName,"ifTypeName"));
		addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  NsmPortMediaShowIntfMessage::setIfName(const string &ifName)
	{
	    m_ifName  =  ifName;
	}

	string  NsmPortMediaShowIntfMessage::getIfName() const
	 {
	   return (m_ifName);
	}


    void  NsmPortMediaShowIntfMessage::setIfTypeName(const string &ifTypeName)
	{
	    m_ifTypeName  =  ifTypeName;
	}

	string  NsmPortMediaShowIntfMessage::getIfTypeName() const
	 {
	   return (m_ifTypeName);
	}

    void  NsmPortMediaShowIntfMessage::setCmdSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  NsmPortMediaShowIntfMessage::getCmdSlotId() const
    {
        return (m_slotId);
    }

    void  NsmPortMediaShowIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmPortMediaShowIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmPortMediaShowIntfMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  NsmPortMediaShowIntfMessage::getMappedId() const
    {
        return (m_mappedId);
    }


    void NsmPortMediaShowIntfMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *NsmPortMediaShowIntfMessage::getCStructureForInputs ()
    {
        NsmPortMediaShowIntf_InputMsg_t *pInput = new NsmPortMediaShowIntf_InputMsg_t;
		strncpy (pInput->ifName, m_ifName.c_str (), 64);
		pInput->ifName[63] = '\0';
		strncpy (pInput->ifTypeName, m_ifTypeName.c_str (), 64);
		pInput->ifTypeName[63] = '\0';
		pInput->cmdCode     = m_cmdCode;
		pInput->slotId     = m_slotId;
        return (pInput);
    }

	void NsmPortMediaShowIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		NSM_PLUG_DBG("Entering...");
        int no_of_records;

		if(pOutputCStructure)
		{
        no_of_records = *((int *)pOutputCStructure);
		nsm_mgmt_buffer_num_t  buff_num = {{0}};
		
		switch(m_cmdCode)
		{
		    case NSMPORTMEDIASHOW_ALL:
			{	
				buff_num.buff.mapped_id = this->getMappedId();
			    buff_num.buff.tag = NSM_PORTMEDIA_SHOW_INTERFACE;
				
				if(no_of_records)
                	addBuffer (buff_num.num, ((sizeof(NsmPortMediaShowIntf_OutputMsg_t) * no_of_records)+sizeof(int)), ( ((unsigned char *)pOutputCStructure )) , false);
		//		NSM_PLUG_DBG_S(string("client ifName =") + ifName);
			}	
			break;
		    case NSMPORTMEDIASHOW_LINECARD:
			{	
				buff_num.buff.mapped_id = this->getMappedId();
			    buff_num.buff.tag = NSM_PORTMEDIA_SHOW_INTERFACE;

				if(no_of_records)
                	addBuffer (buff_num.num, ((sizeof(NsmPortMediaShowIntf_OutputMsg_t) * no_of_records)+sizeof(int)), ( ((unsigned char *)pOutputCStructure )) , false);
		//		NSM_PLUG_DBG_S(string("client ifName =") + ifName);
			}	
			break;
		    case NSMPORTMEDIASHOW_INTF:
			{	
				buff_num.buff.mapped_id = this->getMappedId();
			    buff_num.buff.tag = NSM_PORTMEDIA_SHOW_INTERFACE;
				if(no_of_records)
                	addBuffer (buff_num.num, ((sizeof(NsmPortMediaShowIntf_OutputMsg_t) * no_of_records)+sizeof(int)), ( ((unsigned char *)pOutputCStructure )) , false);
		//		NSM_PLUG_DBG_S(string("client ifName =") + ifName);
			}	
			break;

			default :
			{
			    NSM_PLUG_DBG_S(string("Unknown cmd code") + m_cmdCode);
			}	
				break;
		}	
		//NSM_PLUG_DBG("#####Done");
		}        
	}
}
