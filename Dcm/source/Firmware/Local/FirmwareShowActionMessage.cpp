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
 *   Author : lhu                                                          *
 **************************************************************************/

#include "Firmware/Local/FirmwareShowLocalObjectManager.h"
#include "Firmware/Local/FirmwareShowActionMessage.h"
#include "Firmware/Local/FirmwareShowTypes.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    FirmwareShowActionMessage::FirmwareShowActionMessage ()
        : PrismMessage (FirmwareShowLocalObjectManager::getPrismServiceId (), FIRMWARESHOWACTION)
    {
    }

    FirmwareShowActionMessage::FirmwareShowActionMessage (const string &command)
        : PrismMessage (FirmwareShowLocalObjectManager::getPrismServiceId (), FIRMWARESHOWACTION),
        m_command    (command),
        m_apiFlag    (false)
    {
    }

    FirmwareShowActionMessage::~FirmwareShowActionMessage ()
    {
    }

    void  FirmwareShowActionMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
         addSerializableAttribute (new AttributeString  (&m_command,    "command"));
         addSerializableAttribute (new AttributeString  (&m_result,     "result"));
         addSerializableAttribute (new AttributeBool    (&m_apiFlag,    "apiFlag"));
    }

    void  FirmwareShowActionMessage::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  FirmwareShowActionMessage::getCommand() const
    {
        return (m_command);
    }

    void  FirmwareShowActionMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  FirmwareShowActionMessage::getResult() const
    {
        return (m_result);
    }

    void  FirmwareShowActionMessage::setApiFlag(const bool &flag)
    {
        m_apiFlag  =  flag;
    }

    bool  FirmwareShowActionMessage::getApiFlag() const
    {
        return (m_apiFlag);
    }

    void FirmwareShowActionMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, ncFwInfo_t *&pBuffer)
    {
        pBuffer   =   (ncFwInfo_t*) findBuffer(bufferNum, size);
        return;
    }

    void FirmwareShowActionMessage::transferOutputBuffer(ncFwInfo_t *pBuffer, UI32 size, UI32 i)
    {
        WaveMessageStatus status;

        status = addBuffer(i, size, (void *) pBuffer);
        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR,
            "RASLogShowMessage::transferOutputBuffer : addBuffer is failed: " +
            status);
        }

        return;
    }
}
