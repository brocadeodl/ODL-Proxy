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
 *   Author : Bala 	                                                       *
 **************************************************************************/

#include "ClientInterface/APPM/APPMShowMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    APPMShowMessage::APPMShowMessage()
        : DcmManagementInterfaceMessage (APPMSHOWMESSAGE)
    {
    }

    APPMShowMessage::APPMShowMessage(const UI32 &cmdCode, const string &ifName, const string &profileName, const UI32 &mappedId, const string &lastProfileMac, const UI32 &numOfRecords, const UI32 &fetchModelType)
        : DcmManagementInterfaceMessage (APPMSHOWMESSAGE),
        m_cmdCode   (cmdCode),
        m_ifName    (ifName),
        m_profileName (profileName),
		m_mappedId(mappedId),
        m_lastProfileMac (lastProfileMac),
        m_numOfRecords (numOfRecords),
        m_fetchModelType (fetchModelType)

    {
    }

    APPMShowMessage::~APPMShowMessage ()
    {
    }

    void  APPMShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_profileName, "profileName"));
		addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
		addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeString(&m_lastProfileMac,"lastProfileMac"));
        addSerializableAttribute (new AttributeUI32(&m_numOfRecords,"numOfRecords"));
        addSerializableAttribute (new AttributeUI32(&m_fetchModelType,"fetchModelType"));
    }

    void  APPMShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  APPMShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  APPMShowMessage::setProfileName(const string &profileName)
    {
        m_profileName = profileName;
    }

    string  APPMShowMessage::getProfileName() const
    {
        return (m_profileName);
    }

	void APPMShowMessage::setPortProfileDomainName(const string &profileDomainName) {
		m_portProfileDomainName = profileDomainName;
	}
    
	string APPMShowMessage::getPortProfileDomainName() const {
		return m_portProfileDomainName;
	}
	
    void  APPMShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  APPMShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  APPMShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }
    
    void APPMShowMessage::setLastProfileMac(const string &lastProfileMac) {
        m_lastProfileMac = lastProfileMac;
    } 

    string APPMShowMessage::getLastProfileMac() const {
        return (m_lastProfileMac);
    }

    void APPMShowMessage::setNumberOfRecords(const UI32 &numOfRecords) {
        m_numOfRecords = numOfRecords;
    }

    UI32 APPMShowMessage::getNumberOfRecords() const {
        return (m_numOfRecords);
    }

    void APPMShowMessage::setFetchModelType(const UI32 &fetchModelType) {
    	m_fetchModelType = fetchModelType;
    }

    UI32 APPMShowMessage::getFetchModelType() const {
    	return (m_fetchModelType);
    }



    const void *APPMShowMessage::getCStructureForInputs ()
    {
        //trace (TRACE_LEVEL_INFO, "Entering APPMShowMessage::getCStructureForInputs\n");
        AppmShowMsgInpt_t *pInput = (AppmShowMsgInpt_t *) calloc(1, sizeof(AppmShowMsgInpt_t));

        /*trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Command Code : ") +  m_cmdCode + "\n");
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Number of records : ") +  m_numOfRecords + "\n");
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Last Profile Mac : ")+ m_lastProfileMac.c_str()+ "\n");
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Interface name: ")+ m_ifName.c_str() + "\n");
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Profile name: ")+ m_profileName.c_str() + "\n");
		trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Profile Domain name: ")+ m_portProfileDomainName.c_str() + "\n");
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::getCStructureForInputs: Fetch Model Type: ")+ m_fetchModelType + "\n");*/

		strcpy(pInput->ifName, m_ifName.c_str());
        snprintf(pInput->profileName, (MAX_WYSEREA_PROFILE_NAME_LEN + 1), "%s", m_profileName.c_str());
		snprintf(pInput->portProfileDomainName, (MAX_WYSEREA_PROFILE_NAME_LEN + 1), "%s", m_portProfileDomainName.c_str());
        snprintf(pInput->lastProfileMac, (MAC_ADDR_STR_LEN), "%s", m_lastProfileMac.c_str());
		pInput->cmdCode = m_cmdCode;
		pInput->numOfRecords = m_numOfRecords;
		pInput->fetchModelType = m_fetchModelType;
        return (pInput);
    }

	void APPMShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

//        trace (TRACE_LEVEL_INFO, "Entering APPMShowMessage::loadOutputsFromCStructure\n");

		AppmShowMsgOutpt_t *msg = ((AppmShowMsgOutpt_t *)pOutputCStructure);
        if (!msg)
            return;

		int num_records = msg->num_records;
		appm_buffer_num_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));
        
/*        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::loadOutputsFromCStructur IsMoreFlag is  : ") +  msg->isMoreFlag);
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::loadOutputsFromCStructur RecordType  : ") +  msg->record_type);
        trace (TRACE_LEVEL_INFO, string("APPMShowMessage::loadOutputsFromCStructur Number of records  : ") +  num_records ); */

#if 0
		/* Set the command code to record type */
		m_cmdCode = msg->record_type;
#endif

		switch (msg->record_type) {
			
            case APPM_PROVIDE_ALL_PORT_PROFILES:
//                trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_PORT_PROFILES");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_PORT_PROFILES;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);

            break;

            case APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES:
//              trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;

            case APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;

            case APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;
			
            case APPM_PROVIDE_ALL_PORT_PROFILES_OF_A_PP_DOMAIN:
//                trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_PORT_PROFILES_OF_A_PP_DOMAIN");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_PORT_PROFILES_OF_A_PP_DOMAIN;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);

            break;

            case APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES_OF_A_PP_DOMAIN:
//              trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES_OF_A_PP_DOMAIN");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES_OF_A_PP_DOMAIN;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;

            case APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES_OF_A_PP_DOMAIN:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES_OF_A_PP_DOMAIN");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES_OF_A_PP_DOMAIN;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;

            case APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES_OF_A_PP_DOMAIN:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES_OF_A_PP_DOMAIN");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES_OF_A_PP_DOMAIN;
                addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
                ((num_records - 1) * sizeof (AppmProvidePPMsg_t))), pOutputCStructure, false);
            break;			

            case APPM_SHOW_PROFILE_INTERFACE:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_SHOW_PROFILE_INTERFACE");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_SHOW_PROFILE_INTERFACE;
        		addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
						((num_records - 1) * sizeof (AppmInterfaceMsg_t))), pOutputCStructure, false);
				break;
			case APPM_SHOW_STATUS_RECORD:
//            trace (TRACE_LEVEL_INFO, "Inside APPM_SHOW_STATUS_RECORD");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_SHOW_STATUS_RECORD;
        		addBuffer(buff_num.num, (sizeof(AppmShowMsgOutpt_t) +
						((num_records - 1) * sizeof (AppmShowStatusMsg_t))), pOutputCStructure, false);
				break;
			default:
//                trace (TRACE_LEVEL_INFO, "Inside Default case");
                buff_num.buff.mapped_id = this->getMappedId();
                buff_num.buff.tag = APPM_SHOW_UNKNOWN;
        		addBuffer (buff_num.num, sizeof(AppmShowMsgOutpt_t), pOutputCStructure, false);
				break;
		}
	}
}
