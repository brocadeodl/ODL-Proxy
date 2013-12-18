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

#include "AliasConfig/Global/WyserEaAliasUserMessage.h"
#include "AliasConfig/Global/AliasConfigObjectManager.h"
#include "AliasConfig/Global/AliasConfigObjectManagerTypes.h"

namespace DcmNs
{
    
WyserEaAliasUserMessage::WyserEaAliasUserMessage ()
    : PrismMessage (AliasConfigObjectManager::getPrismServiceId (), WYSEREA_ALIAS_USER)
{
    m_cmdcode = 0;
}

WyserEaAliasUserMessage::WyserEaAliasUserMessage (const UI32 &cmdcode, const string &username, bool &startTimer)
    : PrismMessage (AliasConfigObjectManager::getPrismServiceId (), WYSEREA_ALIAS_USER),
    m_cmdcode      (cmdcode),
    m_username     (username),
    m_startTimer   (startTimer)    
{
}

WyserEaAliasUserMessage::~WyserEaAliasUserMessage ()
{
}

void  WyserEaAliasUserMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
    addSerializableAttribute (new AttributeString(&m_username,"username"));
    addSerializableAttribute (new AttributeBool(&m_startTimer,"startTimer"));
}

void WyserEaAliasUserMessage::setusername(const string &username)
{
    m_username =  username;
}
 
string  WyserEaAliasUserMessage::getusername() const
{
    return (m_username);
}


void  WyserEaAliasUserMessage::setCmdCode(const UI32 &cmdcode)
{
    m_cmdcode  =  cmdcode;
}
 
UI32  WyserEaAliasUserMessage::getCmdCode() const
{
    return (m_cmdcode);
}

void WyserEaAliasUserMessage::setStartTimer(bool &startTimer)
{
    m_startTimer = startTimer;        
}

bool WyserEaAliasUserMessage::getStartTimer()
{
    return (m_startTimer);        
}

}
