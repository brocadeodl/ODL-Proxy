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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgPortCfgPortStateUpdateMessage::PortCfgPortCfgPortStateUpdateMessage ()
		: ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGPORTSTATEUPDATE)
    {
    }

    PortCfgPortCfgPortStateUpdateMessage::PortCfgPortCfgPortStateUpdateMessage (const string &id,
		                                  const bool &portStateEnable,const bool &isIdComplete, const SI32 &portDisReason)
		: ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGPORTSTATEUPDATE),
        m_id    (id),
        m_portStateEnable    (portStateEnable),
		m_isIdComplete    (isIdComplete),
		m_portDisReason   (portDisReason)
    {
    }

    PortCfgPortCfgPortStateUpdateMessage::~PortCfgPortCfgPortStateUpdateMessage ()
    {
    }

    void  PortCfgPortCfgPortStateUpdateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeBool(&m_portStateEnable,"portStateEnable"));
        addSerializableAttribute (new AttributeBool(&m_isIdComplete,"isIdComplete"));
        addSerializableAttribute (new AttributeSI32(&m_portDisReason,"portDisReason"));
    }

    void  PortCfgPortCfgPortStateUpdateMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgPortCfgPortStateUpdateMessage::getId() const
    {
        return (m_id);
    }

    void  PortCfgPortCfgPortStateUpdateMessage::setPortStateEnable(const bool &portStateEnable)
    {
        m_portStateEnable  =  portStateEnable;
    }

    bool  PortCfgPortCfgPortStateUpdateMessage::getPortStateEnable() const
    {
        return (m_portStateEnable);
    }

	void  PortCfgPortCfgPortStateUpdateMessage::setIsIdComplete(const bool &isIdComplete)
	{
		m_isIdComplete  =  isIdComplete;
	}
	
	bool  PortCfgPortCfgPortStateUpdateMessage::getIsIdComplete() const
	{
		return (m_isIdComplete);
	}

	void  PortCfgPortCfgPortStateUpdateMessage::setPortDisReason(const SI32 &portDisReason)
	{
		m_portDisReason  =  portDisReason;
	}

	SI32  PortCfgPortCfgPortStateUpdateMessage::getPortDisReason() const
	{
		return (m_portDisReason);
	}

}
