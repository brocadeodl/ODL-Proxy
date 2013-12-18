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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "ClientInterface/Igmps/IgmpsClientClearMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsClientClearMessage::IgmpsClientClearMessage()
        : DcmManagementInterfaceMessage (IGMPSCLIENTCLEARCONFIG),
          m_cmdCode       (IGMPS_DEFAULT_CMD_CODE)
    {
    }

    IgmpsClientClearMessage::IgmpsClientClearMessage(const string &grpName, const string &ifName, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (IGMPSCLIENTCLEARCONFIG),
        m_grp_name      (grpName),
        m_if_name       (ifName),
        m_cmdCode       (cmdCode)
    {
    }

    IgmpsClientClearMessage::~IgmpsClientClearMessage ()
    {
    }

    void  IgmpsClientClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grp_name,"grpName"));
        addSerializableAttribute (new AttributeString(&m_if_name,"ifName"));
    }

    void  IgmpsClientClearMessage::setGrpName(const string &grpName)
    {
        m_grp_name  =  grpName;
    }

    string  IgmpsClientClearMessage::getGrpName() const
    {
        return (m_grp_name);
    }
    
    void  IgmpsClientClearMessage::setIfName(const string &ifName)
    {
        m_if_name  =  ifName;
    }

    string  IgmpsClientClearMessage::getIfName() const
    {
        return (m_if_name);
    }
    
    void  IgmpsClientClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsClientClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void IgmpsClientClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *IgmpsClientClearMessage::getCStructureForInputs ()
    {
        igmps_clear_input_msg_t *pInput = (igmps_clear_input_msg_t *) malloc (sizeof (igmps_clear_input_msg_t));

        strncpy (pInput->grp_name, m_grp_name.c_str (), IGMPS_MAX_STR);
        strncpy (pInput->if_name, m_if_name.c_str (), IGMPS_MAX_STR);
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void IgmpsClientClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

		igmps_clear_output_msg_t *msg = ((igmps_clear_output_msg_t *)pOutputCStructure);

        if (!msg) return;

        /* FIXME. I don't know what the following code does. :-( */
       	addBuffer (IGMPSCLIENTCLEARCONFIG, sizeof(igmps_clear_output_msg_t), pOutputCStructure, false);
	}
}
