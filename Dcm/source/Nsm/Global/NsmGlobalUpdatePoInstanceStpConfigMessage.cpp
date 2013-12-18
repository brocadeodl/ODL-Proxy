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
 *   Author : sudheend                                                     *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Global/NsmGlobalUpdatePoInstanceStpConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    NsmGlobalUpdatePoInstanceStpConfigMessage::NsmGlobalUpdatePoInstanceStpConfigMessage ()
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName(), NSMGLOBALUPDATEPOINSTANCESTPCONFIG)
        , m_instanceId ( 0 )
    {
    }

    NsmGlobalUpdatePoInstanceStpConfigMessage::NsmGlobalUpdatePoInstanceStpConfigMessage (const string &ifName,const UI8 &instanceId,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode)
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName(), NSMGLOBALUPDATEPOINSTANCESTPCONFIG, cmdCode, IF_TYPE_PO, ifName, cost, priority)
        , m_instanceId ( instanceId )
    {
    }

    NsmGlobalUpdatePoInstanceStpConfigMessage::~NsmGlobalUpdatePoInstanceStpConfigMessage ()
    {
    }

    void  NsmGlobalUpdatePoInstanceStpConfigMessage::setupAttributesForSerialization()
    {
        NsmPortStpConfigMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
    }

    void  NsmGlobalUpdatePoInstanceStpConfigMessage::setIfName(const string &ifName)
    {
        setInterfaceInfo(IF_TYPE_PO, ifName);
    }


    void  NsmGlobalUpdatePoInstanceStpConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  NsmGlobalUpdatePoInstanceStpConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  NsmGlobalUpdatePoInstanceStpConfigMessage::printMsg()
    {
            trace(TRACE_LEVEL_DEBUG, string("NsmGlobalUpdatePoStpConfigMessage::printMsg()"));
            trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:         ")+toString(m_cmdCode));
            trace(TRACE_LEVEL_DEBUG, string("m_ifName:          ")+m_ifName);
            trace(TRACE_LEVEL_DEBUG, string("m_instanceId:              ")+toString(UI32(m_instanceId)));
            trace(TRACE_LEVEL_DEBUG, string("m_priority:        ")+toString((UI32 )m_priority));
            trace(TRACE_LEVEL_DEBUG, string("m_cost:            ")+toString(m_cost));
    }

}
