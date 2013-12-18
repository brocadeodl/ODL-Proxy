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

/******************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	      *
 *   All rights reserved.                                                     *
 *   Author : Mobin Mohan                                                     *
 *****************************************************************************/

#include "ClientInterface/Nsm/NsmClientUpdateGlobalConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmClientUpdateGlobalConfigMessage::NsmClientUpdateGlobalConfigMessage ()
        : DcmManagementInterfaceMessage (NSM_GLOBAL_CONFIG_MSG)
    {
    }

    NsmClientUpdateGlobalConfigMessage::NsmClientUpdateGlobalConfigMessage (
                                bool nativevlanDisable, const UI8 &cmdCode, const UI32 &slotId)
        : DcmManagementInterfaceMessage (NSM_GLOBAL_CONFIG_MSG),
        m_nativevlanDisable    (nativevlanDisable),
        m_cmdCode    (cmdCode),
		m_slotId    (slotId)
    {
    }

    NsmClientUpdateGlobalConfigMessage::~NsmClientUpdateGlobalConfigMessage ()
    {
    }

    void  NsmClientUpdateGlobalConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
                                                         "nativevlanDisable"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
    }

    void  NsmClientUpdateGlobalConfigMessage::setNativevlanDisable(const bool 
                                                            &nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmClientUpdateGlobalConfigMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }


    void  NsmClientUpdateGlobalConfigMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  NsmClientUpdateGlobalConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	void  NsmClientUpdateGlobalConfigMessage::setSlotId(const UI32 &slotId)
   {
 		m_slotId  =  slotId;
   }

   UI32  NsmClientUpdateGlobalConfigMessage::getSlotId() const
   {
  		return (m_slotId);
   }

	const void *NsmClientUpdateGlobalConfigMessage::getCStructureForInputs ()
    {
        NsmGlbUpdConfMsg_t *pInput = new NsmGlbUpdConfMsg_t;

        pInput->nativevlanDisable = m_nativevlanDisable;
        pInput->cmdCode     = m_cmdCode;
		pInput->slotId     = m_slotId;

        return (pInput);
    }

	void NsmClientUpdateGlobalConfigMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}

}
