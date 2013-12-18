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
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Fcoe/FcoeClientInternalMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"


namespace DcmNs
{   

    FcoeClientInternalMessage::FcoeClientInternalMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_INTERNAL)
    {
    }

    FcoeClientInternalMessage::FcoeClientInternalMessage (const UI32 &cmdCode,
                                                      const UI32 &vlan,
                                                      const UI32 &vfid,
                                                      const UI32 &fc_uport,
                                                      const string &intf_name,
                                                      const UI64 &if_index)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_INTERNAL),
        m_cmdCode (cmdCode),
        m_vlan (vlan),
        m_vfid (vfid),
        m_fc_uport (fc_uport),
        m_intf_name (intf_name),
        m_if_index(if_index)
    {
    }

    FcoeClientInternalMessage::~FcoeClientInternalMessage ()
    {
    }

    void  FcoeClientInternalMessage::setupAttributesForSerialization()
    {   
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "if_index"));
    }

    void  FcoeClientInternalMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeClientInternalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void FcoeClientInternalMessage::getOutputBufferRef (UI32 bufferNum,
                                                    UI32 &size,
                                                    void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }


    const void *FcoeClientInternalMessage::getCStructureForInputs ()
    {   
        fcoe_internal_cmd_req_t *req_p = new fcoe_internal_cmd_req_t;

        req_p->cmd_code = m_cmdCode;
        req_p->if_index = m_if_index;
        strncpy((char *)req_p->intf_name, m_intf_name.c_str(),
                FCOE_INTF_NAME_LEN_MAX);
        req_p->vlan_id = m_vlan;
        req_p->vfid = m_vfid;
        req_p->fc_uport = m_fc_uport;
        return req_p;
    }

    void
    FcoeClientInternalMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        fcoe_internal_cmd_resp_t *resp_p = (fcoe_internal_cmd_resp_t *)pOutputCStructure;
        if (resp_p) {
            addBuffer(resp_p->cmd_code, resp_p->size,
                      (unsigned char *)resp_p, false);
        }
    }

    UI32
    FcoeClientInternalMessage::getVlan() const
    {   
        return m_vlan;
    }

    void
    FcoeClientInternalMessage::setVlan(const UI32 &vlan)
    {   
        m_vlan = vlan;
    }

    UI32
    FcoeClientInternalMessage::getVfid() const
    {
        return m_vfid;
    }

    void
    FcoeClientInternalMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }

    UI32
    FcoeClientInternalMessage::getFcUport() const
    {
        return m_fc_uport;
    }

    void
    FcoeClientInternalMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }

    string
    FcoeClientInternalMessage::getIfName() const
    {
        return m_intf_name;
    }

    void
    FcoeClientInternalMessage::setIfName(const string &if_name)
    {
        m_intf_name = if_name;
    }

    UI64
    FcoeClientInternalMessage::getIfindex() const
    {   
        return m_if_index;
    }

    void
    FcoeClientInternalMessage::setIfindex(const UI64 &if_index)
    {   
        m_if_index = if_index;
    }

}
