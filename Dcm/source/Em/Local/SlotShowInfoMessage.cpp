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
 *   Author : hungn                                                     *
 **************************************************************************/

#include "Em/Local/LinecardLocalObjectManager.h"
#include "Em/Local/SlotShowInfoMessage.h"
#include "Em/Local/LinecardTypes.h"

namespace DcmNs
{

    SlotShowInfoMessage::SlotShowInfoMessage ()
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), SLOTSHOWINFO)
    {
    }

    SlotShowInfoMessage::SlotShowInfoMessage (const UI32 &slotnum,const UI32 &cmdcode,const SI32 &result)
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), SLOTSHOWINFO),
        m_slotnum    (slotnum),
        m_cmdcode    (cmdcode),
        m_result    (result)
    {
		m_slotnum = slotnum;
		m_cmdcode = cmdcode;
		m_result = result;
    }

    SlotShowInfoMessage::~SlotShowInfoMessage ()
    {
    }

    void  SlotShowInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_slotnum,"slotnum"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"mode"));
        addSerializableAttribute (new AttributeSI32(&m_result,"result"));
        addSerializableAttribute (new AttributeString(&m_output, "output"));
    }

    void  SlotShowInfoMessage::setSlotnum(const UI32 &slotnum)
    {
        m_slotnum  =  slotnum;
    }

    UI32  SlotShowInfoMessage::getSlotnum() const
    {
        return (m_slotnum);
    }

    void  SlotShowInfoMessage::setCmdCode(const UI32 &cmdcode)
    {
        m_cmdcode  = cmdcode;
    }

    UI32  SlotShowInfoMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }

    void  SlotShowInfoMessage::setResult(const SI32 &result)
    {
        m_result  =  result;
    }

    SI32  SlotShowInfoMessage::getResult() const
    {
        return (m_result);
    }
	string  SlotShowInfoMessage::getOutput() const
	{
        return (m_output);
	}
	void  SlotShowInfoMessage::setOutput(const string &output)
	{
        m_output = output;
	}

}
