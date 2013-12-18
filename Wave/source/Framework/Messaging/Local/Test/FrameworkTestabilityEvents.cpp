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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/Local/Test/FrameworkTestabilityEvents.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"

namespace WaveNs
{

FrameworkTestabilityEvent1::FrameworkTestabilityEvent1 ()
    : PrismEvent (FrameworkLocalMessagingTestObjectManager::getPrismServiceId (), FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1)
{
    m_eventUI32  = 0;
}

FrameworkTestabilityEvent1::~FrameworkTestabilityEvent1 ()
{
}

void FrameworkTestabilityEvent1::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_eventUI32,       "eventUI32"));
     addSerializableAttribute (new AttributeString (&m_eventString,     "eventString"));
}

UI32 FrameworkTestabilityEvent1::getEventUI32 () const
{
    return (m_eventUI32);
}

void FrameworkTestabilityEvent1::setEventUI32 (const UI32 &eventUI32)
{
    m_eventUI32 = eventUI32;
}

string FrameworkTestabilityEvent1::getEventString () const
{
    return (m_eventString);
}

void FrameworkTestabilityEvent1::setEventString (const string &eventString)
{
    m_eventString = eventString;
}

}
