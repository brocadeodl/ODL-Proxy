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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rhkumar	                                                   *
 **************************************************************************/

#include "AG/Local/AgLocalShowMessage.h"
#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    AgLocalShowMessage::AgLocalShowMessage ()
        : PrismMessage (AgLocalObjectManager::getPrismServiceId (), AG_LOCAL_SHOW_MESSAGE)
    {
        m_cmdCode = 0;
    }

    AgLocalShowMessage::AgLocalShowMessage (const UI32 &cmdCode)
        : PrismMessage (AgLocalObjectManager::getPrismServiceId (), AG_LOCAL_SHOW_MESSAGE),
        m_cmdCode   (cmdCode)
    {
    }

    AgLocalShowMessage::~AgLocalShowMessage ()
    {
    }

    void  AgLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32Vector(&m_elements, "elements"));
    }

    void  AgLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  AgLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  AgLocalShowMessage::setElements(const vector<UI32> &elements)
    {
        m_elements  =  elements;
    }

    vector<UI32>  AgLocalShowMessage::getElements() const
    {
        return (m_elements);
    }

    void AgLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void AgLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}
