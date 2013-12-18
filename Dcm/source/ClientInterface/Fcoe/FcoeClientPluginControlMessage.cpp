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

#include "ClientInterface/Fcoe/FcoeClientPluginControlMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientPluginControlMessage::FcoeClientPluginControlMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_PLUGIN_CONTROL)
    {
    }

    FcoeClientPluginControlMessage::FcoeClientPluginControlMessage (const UI32 &oper_bmp, const UI32 &plugin_status)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_PLUGIN_CONTROL),
		m_oper_bmp    (oper_bmp),
        m_plugin_status    (plugin_status)
    {
    }

    FcoeClientPluginControlMessage::~FcoeClientPluginControlMessage ()
    {
    }

    void  FcoeClientPluginControlMessage::setupAttributesForSerialization()
    {   
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_plugin_status,"plugin_status"));
    }

    void  FcoeClientPluginControlMessage::setOper_bmp(const UI32 &oper_bmp)
    {   
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientPluginControlMessage::getOper_bmp() const
    {   
        return (m_oper_bmp);
    }

    void  FcoeClientPluginControlMessage::setPlugin_status(const UI32 &plugin_status)
    {   
        m_plugin_status  =  plugin_status;
    }

    UI32  FcoeClientPluginControlMessage::getPlugin_status() const
    {
        return (m_plugin_status);
    }

    const void * FcoeClientPluginControlMessage::getCStructureForInputs()
    {
        fcoe_plugin_ctrl_msg_t *msg_p = new fcoe_plugin_ctrl_msg_t;

		msg_p->oper_bmp = m_oper_bmp;
        msg_p->plugin_status = m_plugin_status;

        return msg_p;
    }
}

