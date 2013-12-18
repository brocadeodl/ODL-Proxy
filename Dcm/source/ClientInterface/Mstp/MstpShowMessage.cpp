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
 *   Author : Navin
 **************************************************************************/

#include "ClientInterface/Mstp/MstpShowMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Mstp/Local/MstpLocalShowSTPMessage.h"


namespace DcmNs
{

    MstpShowMessage::MstpShowMessage ()
        : DcmManagementInterfaceMessage (MSTP_SHOW)
    {
        m_cmdCode   = 0;
        m_type      = 0;
        m_inst      = 0;
        m_lastIndex = 0;
        m_ifType    = IF_TYPE_INVALID;
        m_mappedId  = 0;
        m_debugSeverity = DEBUG_SEVERITY_LEVEL_NONE;
    }

    MstpShowMessage::MstpShowMessage (MstpLocalShowSTPMessage *pMessage)
        : DcmManagementInterfaceMessage (MSTP_SHOW)
        , m_ifName  ( pMessage->m_ifName )
    {
        m_cmdCode   = pMessage->m_cmdCode;
        m_type      = pMessage->m_reqType;
        m_inst      = pMessage->m_instId;
        m_lastIndex = pMessage->m_lastInstId;
        m_ifType    = pMessage->m_ifType;
        m_mappedId  = 0;
        m_debugSeverity = DEBUG_SEVERITY_LEVEL_NONE;
    }

    MstpShowMessage::~MstpShowMessage ()
    {
    }

    void  MstpShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_inst,"inst"));
        addSerializableAttribute(new AttributeUI32(&m_type,"type"));
        addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute(new AttributeUI32(&m_lastIndex,"lastIndex"));
        addSerializableAttribute(new AttributeUI8(&m_debugSeverity,"debugSeverity"));
    }

    void  MstpShowMessage::setInterfaceName(InterfaceType ifType, const string &ifName)
    {
        m_ifType  =  ifType;
        m_ifName  =  ifName;
    }

    InterfaceType MstpShowMessage::getIfType() const
    {
        return m_ifType;
    }

    string  MstpShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  MstpShowMessage::setInstance(const UI32 &inst)
    {
        m_inst  =  inst;
    }

    UI32  MstpShowMessage::getInstance() const
    {
        return (m_inst);
    }

    void  MstpShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpShowMessage::setType(const UI32 &Type)
    {
        m_type  =  Type;
    }

    UI32  MstpShowMessage::getType() const
    {
        return (m_type);
    }

    void  MstpShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  MstpShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void MstpShowMessage::setLastIndex(const UI32 last_index) {
        m_lastIndex = last_index;
    }

    UI32 MstpShowMessage::getLastIndex() const {
        return m_lastIndex;
    }

    UI8 MstpShowMessage::getDebugSeverity() const {
        return m_debugSeverity;
    }

    void MstpShowMessage::setDebugSeverity(const UI8 &severity) {
        m_debugSeverity = severity;
    }

    const void *MstpShowMessage::getCStructureForInputs ()
    {
        mstp_show_input_msg_t *pInput =
                (mstp_show_input_msg_t*) calloc(1, sizeof(mstp_show_input_msg_t));
        if (pInput == NULL) {
            trace(TRACE_LEVEL_ERROR, "MstpShowMessage::getCStructureForInputs : calloc returned NULL!");
            return NULL;
        }

        pInput->type        = m_type;
        pInput->inst        = m_inst;
        pInput->cmd         = m_cmdCode;
        pInput->last_index  = m_lastIndex;
        pInput->severity    = m_debugSeverity;


        if (m_ifType == IF_TYPE_PO) {
            snprintf(pInput->if_name, MSTP_DCM_IF_NAME_LEN, "po%s", m_ifName.c_str());
        }
        else if (m_ifType != IF_TYPE_INVALID) {
            const char * ifname = m_ifName.c_str();
            const char *p = strchr(ifname, '/');
            if (p && strchr(p+1, '/')) ifname = p+1;
            GENERATE_SHORT_IF_NAME_FOR_BACK_END(m_ifType, ifname, pInput->if_name, MSTP_DCM_IF_NAME_LEN);
        }

        return (pInput);
    }

    void MstpShowMessage::loadOutputsFromCStructure (const void 
                        *pOutputCStructure)
    {
        mstp_show_msg_t *msg = ((mstp_show_msg_t *)pOutputCStructure);
        if (msg == NULL || msg->num_records == 0) {
            return;
        }

        UI32 size = msg->num_records * sizeof (mstp_show_msg_t);

        switch (msg->record_type) {
        case DCM_SSM_SHOW_RECORD_TYPE_BPDUGUARD: {
            nsm_mgmt_buffer_num_t  buff_num;
            buff_num.buff.mapped_id = this->getMappedId();
            buff_num.buff.tag = MSTP_TAG_ID_SHOW_BPDU;

            addBuffer(buff_num.num, size, pOutputCStructure, false);
            break;
        }
        default :
            addBuffer (MSTP_SHOW, size, pOutputCStructure, false);
            break;
        }
    }

}
