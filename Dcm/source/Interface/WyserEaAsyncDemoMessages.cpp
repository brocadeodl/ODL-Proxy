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

/**************************************************************************
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#include "Interface/WyserEaAsyncDemoMessages.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ValInterfaceTypes.h"

namespace DcmNs
{

WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage (UI32 numberOfExtraReplies, UI32 delayBetweenReplies, UI32 messageNumber)
    : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE),
      m_commandCode             (WYSEREA_ASYNC_DEMO_MESSAGE_ASYNC_REPLY_TEST),
      m_numberOfExtraReplies    (numberOfExtraReplies),
      m_delayBetweenReplies     (delayBetweenReplies),
      m_messageNumber           (messageNumber),
      m_sleepDuration           (0)
{
    // tracePrintf (TRACE_LEVEL_INFO, "WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage(UI32,UI32,UI32) : this [0x%p]", this);
}

WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage (UI32 sleepDuration)
    : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE),
      m_commandCode             (WYSEREA_ASYNC_DEMO_MESSAGE_RECALL_TEST),
      m_numberOfExtraReplies    (0),
      m_delayBetweenReplies     (0),
      m_messageNumber           (0),
      m_sleepDuration           (sleepDuration)
{
    // tracePrintf (TRACE_LEVEL_INFO, "WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage(UI32) : this [0x%p]", this);
}

WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage ()
    : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE)
{
    // for clone verification.
    // tracePrintf (TRACE_LEVEL_INFO, "WyserEaAsyncDemoMessage::WyserEaAsyncDemoMessage() : this [0x%p]", this);
}

WyserEaAsyncDemoMessage::~WyserEaAsyncDemoMessage ()
{
    // for clone verification.
    // tracePrintf (TRACE_LEVEL_INFO, "WyserEaAsyncDemoMessage::~WyserEaAsyncDemoMessage() : this [0x%p]", this);
}

void WyserEaAsyncDemoMessage::setupAttributesForSerialization ()
{
    // This method is required for cloning.

    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32     (&m_commandCode,            "commandCode"));
   
    addSerializableAttribute (new AttributeUI32     (&m_numberOfExtraReplies,   "numberOfExtraReplies"));
    addSerializableAttribute (new AttributeUI32     (&m_delayBetweenReplies,    "delayBetweenReplies"));
    addSerializableAttribute (new AttributeUI32     (&m_messageNumber,          "messageNumber"));
    addSerializableAttribute (new AttributeString   (&m_demoString,             "demoString"));

    addSerializableAttribute (new AttributeUI32     (&m_sleepDuration,          "sleepDuration"));
}

}
