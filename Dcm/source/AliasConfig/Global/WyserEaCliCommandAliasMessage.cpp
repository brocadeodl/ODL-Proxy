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

#include "AliasConfig/Global/WyserEaCliCommandAliasMessage.h"
#include "AliasConfig/Global/AliasConfigObjectManager.h"
#include "AliasConfig/Global/AliasConfigObjectManagerTypes.h"

 
namespace DcmNs
{

WyserEaCliCommandAliasMessage::WyserEaCliCommandAliasMessage (const UI32 &opcode)
    : PrismMessage (AliasConfigObjectManager::getPrismServiceId (), opcode)
{
    m_cmdcode = 0;	       
}

WyserEaCliCommandAliasMessage::WyserEaCliCommandAliasMessage (const UI32 &opcode, const UI32 &cmdcode,const string &username, const string &aliasname,const string  &aliasexpansion, bool &startTimer, const UI32 &operationmode, const UI32 &aliasTypeEnumValue)
    : PrismMessage      (AliasConfigObjectManager::getPrismServiceId (), opcode),
      m_cmdcode         (cmdcode),
      m_username        (username),
      m_aliasname       (aliasname),
      m_aliasexpansion  (aliasexpansion),
      m_startTimer      (startTimer),
      m_operationMode   (operationmode),
      m_aliasType       (aliasTypeEnumValue)
{
}

WyserEaCliCommandAliasMessage::~WyserEaCliCommandAliasMessage ()
{
}
 
void  WyserEaCliCommandAliasMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
    addSerializableAttribute (new AttributeString(&m_username,"username"));
    addSerializableAttribute (new AttributeString(&m_aliasname,"aliasname"));
    addSerializableAttribute (new AttributeString(&m_aliasexpansion,"aliasexpansion"));
    addSerializableAttribute (new AttributeBool(&m_startTimer,"startTimer"));
    addSerializableAttribute (new AttributeUI32(&m_aliasType, "aliasType"));
    addSerializableAttribute (new AttributeUI32(&m_operationMode,"operationmode"));
}



void WyserEaCliCommandAliasMessage::setusername(const string &username)
{
    m_username =  username;
}
     
string  WyserEaCliCommandAliasMessage::getusername() const
{
    return (m_username);
}
    
void WyserEaCliCommandAliasMessage::setaliasname(const string &aliasname)
{
    m_aliasname =  aliasname;
}
 
string  WyserEaCliCommandAliasMessage::getaliasname() const
{
    return (m_aliasname);
}
 
void  WyserEaCliCommandAliasMessage::setaliasexpansion(const string &aliasexpansion)
{
    m_aliasexpansion =  aliasexpansion;
}
 
string  WyserEaCliCommandAliasMessage::getaliasexpansion() const
{
    return (m_aliasexpansion);
}

void  WyserEaCliCommandAliasMessage::setCmdCode(const UI32 &cmdcode)
{
    m_cmdcode  =  cmdcode;
}

UI32  WyserEaCliCommandAliasMessage::getCmdCode() const
{
    return (m_cmdcode);
}

void WyserEaCliCommandAliasMessage::setStartTimer(bool &startTimer)
{
    m_startTimer = startTimer;        
}

bool WyserEaCliCommandAliasMessage::getStartTimer()
{
    return (m_startTimer);       
}

UI32 WyserEaCliCommandAliasMessage::getOperationMode ()
{
    return (m_operationMode);        
}

UI32 WyserEaCliCommandAliasMessage::getAliasType () const
{
    return (m_aliasType);
}

}
