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
 *   Author : cgangwar                                                     *
 ***************************************************************************/
#include "ManagementInterface/ClientInterface/UnifiedClientNotifyMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{
	 
UnifiedClientNotifyMessage::UnifiedClientNotifyMessage ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), UNIFIEDCLIENTNOTIFY)
{
}

UnifiedClientNotifyMessage::~UnifiedClientNotifyMessage ()
{
}
 
void UnifiedClientNotifyMessage::setupAttributesForSerialization()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
	     
    addSerializableAttribute (new AttributeUI32 (&m_backendNotifyValue, "backendNotifyValue"));
}

void UnifiedClientNotifyMessage::setbackendNotifyValue(UI32 &backendNotifyValue)
{
    m_backendNotifyValue =backendNotifyValue;
}
 
UI32 UnifiedClientNotifyMessage::getbackendNotifyValue()  
{
    return (m_backendNotifyValue);
}

const void *UnifiedClientNotifyMessage::getCStructureForInputs ()
{
    UnifiedClientNotifyMessage_t *pInput                  = new UnifiedClientNotifyMessage_t ();

    prismAssert (NULL != pInput, __FILE__, __LINE__);

    pInput->backendNotifyValue = m_backendNotifyValue;

    return (pInput);
}

void UnifiedClientNotifyMessage::deleteCStructureForInputs (const void *pInputStruct)
{
    return;
}

}
