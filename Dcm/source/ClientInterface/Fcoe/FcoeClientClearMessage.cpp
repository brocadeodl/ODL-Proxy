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
 *   Author : Vikas B Dharwadkar                                           *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Fcoe/FcoeClientClearMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"


namespace DcmNs
{

    FcoeClientClearMessage::FcoeClientClearMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_CLEAR)
    {
    }

    FcoeClientClearMessage::FcoeClientClearMessage (const UI32 &cmdCode,
                                                  const UI32 &vlan,
                                                  const UI32 &vfid,
                                                  const UI32 &fc_uport,
                                                  const string &intf_name,
                                                  const string &wwn,
                                                  const UI64 &if_index,
                                                  const UI32 &po)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_CLEAR),
        m_cmdCode (cmdCode),
        m_vlan (vlan),
        m_vfid (vfid),
        m_fc_uport (fc_uport),
        m_intf_name (intf_name),
        m_wwn (wwn),
        m_if_index (if_index),
        m_po (po)
    {
    }

    FcoeClientClearMessage::~FcoeClientClearMessage ()
    {
    }

    void  FcoeClientClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeString(&m_wwn, "WWN"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "if_index"));
        addSerializableAttribute (new AttributeUI32(&m_po, "Portchannel"));
    }

    void  FcoeClientClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeClientClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void *FcoeClientClearMessage::getCStructureForInputs ()
    {
        fcoe_clear_cmd_req_t    *req_p = new fcoe_clear_cmd_req_t;
        uint64_t                wwn = 0;
        char                    wwn_str[FCOE_WWN_LEN];
        const char              *delims = ":";
        char                    *result = NULL;

        if (req_p == NULL) {
            return NULL;
        }
        
        memset(req_p, 0, sizeof(fcoe_clear_cmd_req_t));

        req_p->cmd_code = m_cmdCode;

        req_p->fc_uport_num = m_fc_uport;
        req_p->vlan = m_vlan;
        req_p->vfid = m_vfid;

        strncpy((char *)(req_p->if_name), m_intf_name.c_str(), FCOE_INTF_NAME_LEN_MAX);
        strncpy((char*)wwn_str, m_wwn.c_str(), FCOE_WWN_LEN);
        result = strtok(wwn_str, delims);
        while (result != NULL) {
            wwn = wwn << 8;
            wwn |= strtol(result, NULL, 16);
            result = strtok(NULL, delims);
        }
        req_p->wwn = wwn;

        req_p->if_index = m_if_index;

        req_p->po = m_po;

        return req_p;
    }

	void 
    FcoeClientClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        /*fcoe_show_cmd_resp_t *resp_p = (fcoe_show_cmd_resp_t *)pOutputCStructure;

        addBuffer(resp_p->cmd_code, resp_p->size,
                  (unsigned char *)resp_p, false);*/
	}

    UI32 
    FcoeClientClearMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeClientClearMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeClientClearMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeClientClearMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeClientClearMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeClientClearMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeClientClearMessage::getIfName() const
    {
        return m_intf_name;
    }
    
    void 
    FcoeClientClearMessage::setIfName(const string &if_name)
    {
        m_intf_name = if_name;
    }

    void
    FcoeClientClearMessage::setWWN (const string &wwn)
    {
        m_wwn = wwn;
    }

    string
    FcoeClientClearMessage::getWWN() const
    {
        return m_wwn;
    }

    UI64
    FcoeClientClearMessage::getIfindex() const
    {
        return m_if_index;
    }

    void
    FcoeClientClearMessage::setIfindex(const UI64 &if_index)
    {
        m_if_index = if_index;
    }

    UI32
    FcoeClientClearMessage::getPo() const
    {   
        return m_po;
    }

    void
    FcoeClientClearMessage::setPo(const UI32 &po)
    {   
        m_po = po;
    }
}
