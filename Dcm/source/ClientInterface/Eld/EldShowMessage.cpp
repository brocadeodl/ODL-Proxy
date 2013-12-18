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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Eld/EldShowMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    EldShowMessage::EldShowMessage (const string &interface, const string &ifName,
			const string &Mac, const UI32 &vlan_id, const UI32 &type,
			const UI32 &cmdcode,  const UI32 &bridge, const UI32 &ifindex,
			const UI32 &interface_type, const UI32 &port_channel_id,const UI32 &mappedid)
        : DcmManagementInterfaceMessage (MSG_ELD_SHOW_INTERFACE_VLAN),
        m_ifName    (ifName),
        m_interface    (interface),
        m_Mac    	(Mac),
        m_vlanid    (vlan_id),
        m_type   (type),
        m_cmdcode   (cmdcode),
        m_bridge   (bridge),
		m_ifindex (ifindex),
		m_interface_type (interface_type),
		m_port_channel_id (port_channel_id),
		m_mappedid(mappedid)
    {
		m_cmdcode = 0;
		m_rbridge = 0;
		m_vlanid = 0;
    }

    EldShowMessage::EldShowMessage(EldLocalShowMessage *pMsg)
        : DcmManagementInterfaceMessage (MSG_ELD_SHOW_INTERFACE_VLAN)

    {
        m_mappedid = DcmToolKit::getLocalMappedId();
        m_vlanid = pMsg->m_vlanId;
        m_cmdcode = pMsg->m_cmdCode;
        m_interface = pMsg->m_ifName;
        m_rbridge = pMsg->m_rbridgeId;
    }

}
