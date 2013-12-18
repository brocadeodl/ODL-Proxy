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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : lhu                                                     *
 **************************************************************************/
#include "Framework/Attributes/AttributeEnum.h"
#include "Terminal/Global/TerminalSessionCfgMessage.h"
#include "Terminal/Global/TerminalObjectManager.h"
#include "Terminal/Global/TerminalTypes.h"

namespace DcmNs
{

    TerminalSessionCfgMessage::TerminalSessionCfgMessage ()
        : PrismMessage (TerminalObjectManager::getPrismServiceId (), TERMINALSESSIONCFG)
    {
    }

    TerminalSessionCfgMessage::TerminalSessionCfgMessage (const Session_Id &sessionid, const Terminal_cfg &operationtype, const LeafValue_flag &exectimeout_flag, const UI32 &exectimeout)
        : PrismMessage (TerminalObjectManager::getPrismServiceId (), TERMINALSESSIONCFG),
        m_sessionid    (sessionid),
        m_operationtype    (operationtype),
        m_exectimeout_flag    (exectimeout_flag),
        m_exectimeout    (exectimeout)
    {
    }

    TerminalSessionCfgMessage::~TerminalSessionCfgMessage ()
    {
    }

    void  TerminalSessionCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_sessionid),"sessionid"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_operationtype),"operationtype"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_exectimeout_flag),"exectimeout_flag"));
        addSerializableAttribute (new AttributeUI32(&m_exectimeout,"exectimeout"));
    }

    void  TerminalSessionCfgMessage::setSessionid(const Session_Id &sessionid)
    {
        m_sessionid  =  sessionid;
    }

    Session_Id  TerminalSessionCfgMessage::getSessionid() const
    {
        return (m_sessionid);
    }

    void  TerminalSessionCfgMessage::setOperationtype(const Terminal_cfg &operationtype)
    {
        m_operationtype  =  operationtype;
    }

    Terminal_cfg  TerminalSessionCfgMessage::getOperationtype() const
    {
        return (m_operationtype);
    }

    void  TerminalSessionCfgMessage::setExectimeout_flag(const LeafValue_flag &exectimeout_flag)
    {
        m_exectimeout_flag  =  exectimeout_flag;
    }

    LeafValue_flag TerminalSessionCfgMessage::getExectimeout_flag() const
    {
        return (m_exectimeout_flag);
    }

    void  TerminalSessionCfgMessage::setExectimeout(const UI32 &exectimeout)
    {
        m_exectimeout  =  exectimeout;
    }

    UI32  TerminalSessionCfgMessage::getExectimeout() const
    {
        return (m_exectimeout);
    }

}
