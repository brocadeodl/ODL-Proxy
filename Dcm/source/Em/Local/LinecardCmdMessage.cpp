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
#include "Em/Local/LinecardCmdMessage.h"
#include "Em/Local/LinecardTypes.h"

namespace DcmNs
{

    LinecardCmdMessage::LinecardCmdMessage ()
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), LINECARDCMD)
    {
    }

    LinecardCmdMessage::LinecardCmdMessage (const UI32 &slotnum,const string &cmd,const SI32 &result)
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), LINECARDCMD),
        m_slotnum    (slotnum),
        m_cmd (cmd),
        m_result    (result)
    {
		m_slotnum = slotnum;
		m_cmd = cmd;
		m_result = result;
    }

    LinecardCmdMessage::~LinecardCmdMessage ()
    {
    }

    void  LinecardCmdMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_slotnum,"slotnum"));
        addSerializableAttribute (new AttributeString(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeSI32(&m_result,"result"));
        addSerializableAttribute (new AttributeString(&m_output, "output"));
    }

    void  LinecardCmdMessage::setSlotnum(const UI32 &slotnum)
    {
        m_slotnum  =  slotnum;
    }

    UI32  LinecardCmdMessage::getSlotnum() const
    {
        return (m_slotnum);
    }

    void  LinecardCmdMessage::setCmd(const string &cmd)
    {
        m_cmd  = cmd;
    }

    string  LinecardCmdMessage::getCmd() const
    {
        return (m_cmd);
    }

    void  LinecardCmdMessage::setResult(const SI32 &result)
    {
        m_result  =  result;
    }

    SI32  LinecardCmdMessage::getResult() const
    {
        return (m_result);
    }
	string  LinecardCmdMessage::getOutput() const
	{
        return (m_output);
	}
	void  LinecardCmdMessage::setOutput(const string &output)
	{
        m_output = output;
	}

}
