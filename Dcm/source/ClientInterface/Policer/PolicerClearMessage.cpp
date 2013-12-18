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
 *   Author : Sudha
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Policer/PolicerClearMessage.h"
#include "ssm_policer_dcmd_public.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    PolicerClearMessage::PolicerClearMessage ()
        : DcmManagementInterfaceMessage (POLICER_CLEAR_ALL)
    {
    }

    PolicerClearMessage::PolicerClearMessage (const string &Interface, const string &ifName, const string &mapName, const UI32 &direction, const UI32 &cmdCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (POLICER_CLEAR_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
		m_mapName	(mapName),
		m_direction (direction),
        m_cmdCode   (cmdCode),
		m_mappedId    (mappedId)                                
    {
    }

    PolicerClearMessage::~PolicerClearMessage ()
    {
    }

    void  PolicerClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
		addSerializableAttribute (new AttributeString(&m_mapName,"mapName"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  PolicerClearMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  PolicerClearMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  PolicerClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PolicerClearMessage::getIfName() const
    {
        return (m_ifName);
    }

	void  PolicerClearMessage::setMapName(const string &mapName)
    {
        m_mapName  =  mapName;
    }

    string  PolicerClearMessage::getMapName() const
    {
        return (m_mapName);
    }
    
	void  PolicerClearMessage::setDirection(const UI32 &direction)
    {
        m_direction  =  direction;
    }

    UI32  PolicerClearMessage::getDirection() const
    {
        return (m_direction);
    }

    void  PolicerClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PolicerClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  PolicerClearMessage::setMappedId(const UI32 &mappedId)
	{
	  m_mappedId  =  mappedId;
	}

	UI32  PolicerClearMessage::getMappedId() const
	{
	  return (m_mappedId);
	}

      void PolicerClearMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
      {
          pBuffer = findBuffer(bufferNum, size);
      }


    void PolicerClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *PolicerClearMessage::getCStructureForInputs ()
    {
        ssm_pol_show_or_clr_input_msg_t *pInput = new ssm_pol_show_or_clr_input_msg_t;
		trace (TRACE_LEVEL_DEBUG, "Entering PolicerClearMessage:: getCStructureForInputs:- " + m_cmdCode);

        strncpy (pInput->interface , m_ifName.c_str (), MAX_IF_NAME_LEN);
		pInput->interface[MAX_IF_NAME_LEN] = '\0';
		strncpy ((pInput->map_name).name , m_mapName.c_str (), MAX_IF_NAME_LEN);
        pInput->interface[MAX_IF_NAME_LEN] = '\0';
		pInput->direction	= (ssm_pol_intf_direction)m_direction;
        pInput->cmdcode     = (ssm_pol_cmd_code)m_cmdCode;

        return (pInput);
    }

	void PolicerClearMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
		return;
	}
}
