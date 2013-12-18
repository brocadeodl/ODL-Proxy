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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "ClientInterface/Mstp/MstpVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"

namespace DcmNs
{

    MstpVlanConfigMessage::MstpVlanConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPVLANCONFIG)
    {
		m_id = 0;
		m_cmdCode = 0;
    }


    MstpVlanConfigMessage::MstpVlanConfigMessage (const UI32 &id, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (MSTPVLANCONFIG),
        m_id    (id),
        m_cmdCode    (cmdCode)
    {
    }

    MstpVlanConfigMessage::~MstpVlanConfigMessage ()
    {
    }

    void  MstpVlanConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  MstpVlanConfigMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  MstpVlanConfigMessage::getId() const
    {
        return (m_id);
    }


    void  MstpVlanConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpVlanConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	const void *MstpVlanConfigMessage::getCStructureForInputs() 
	{
		mstp_msg_vlan_config *vlan_config = new mstp_msg_vlan_config;
		vlan_config->cmd = m_cmdCode;
		vlan_config->vid = m_id;
		return vlan_config;
	}
}
