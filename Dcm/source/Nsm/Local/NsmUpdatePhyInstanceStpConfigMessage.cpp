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

#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmUpdatePhyInstanceStpConfigMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"


namespace DcmNs
{

    NsmUpdatePhyInstanceStpConfigMessage::NsmUpdatePhyInstanceStpConfigMessage ()
        : NsmPortStpConfigMessage (NsmLocalObjectManager::getClassName(),NSMUPDATEPHYINSTANCESTPCONFIG)
        , m_instanceId ( 0 )
        , m_restrictedRole ( false )
        , m_restrictedTcn ( false )
        , m_guard ( MSTPPORTNOGUARDROOT )
    {
    }

    NsmUpdatePhyInstanceStpConfigMessage::NsmUpdatePhyInstanceStpConfigMessage (const InterfaceType &ifType, const string &ifName,const UI8 &instanceId,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode)
        : NsmPortStpConfigMessage (NsmLocalObjectManager::getClassName(),NSMUPDATEPHYINSTANCESTPCONFIG, cmdCode, ifType, ifName, cost, priority)
        , m_instanceId    (instanceId)
        , m_restrictedRole ( false )
        , m_restrictedTcn ( false )
        , m_guard ( MSTPPORTNOGUARDROOT )
    {
    }

    NsmUpdatePhyInstanceStpConfigMessage::~NsmUpdatePhyInstanceStpConfigMessage ()
    {
    }

    void  NsmUpdatePhyInstanceStpConfigMessage::setupAttributesForSerialization()
    {
        NsmPortStpConfigMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
    }

    void  NsmUpdatePhyInstanceStpConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  NsmUpdatePhyInstanceStpConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

   void  NsmUpdatePhyInstanceStpConfigMessage::setGuard(const MstpPortGuardRootEnum &guard)
    {
        m_guard  =  guard;
    }

    MstpPortGuardRootEnum  NsmUpdatePhyInstanceStpConfigMessage::getGuard() const
    {
        return (m_guard);
    }
  void  NsmUpdatePhyInstanceStpConfigMessage::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  NsmUpdatePhyInstanceStpConfigMessage::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    void  NsmUpdatePhyInstanceStpConfigMessage::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    bool  NsmUpdatePhyInstanceStpConfigMessage::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  NsmUpdatePhyInstanceStpConfigMessage::printMsg()
    {
            trace(TRACE_LEVEL_DEBUG, string("NsmUpdatePhyInstanceStpConfigMessage::printMsg()"));
            trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:         ")+toString(m_cmdCode));
            trace(TRACE_LEVEL_DEBUG, string("m_ifName:          ")+m_ifName);
            trace(TRACE_LEVEL_DEBUG, string("m_instanceId:       ")+toString(UI32(m_instanceId)));
            trace(TRACE_LEVEL_DEBUG, string("m_priority:        ")+toString((UI32 )m_priority));
            trace(TRACE_LEVEL_DEBUG, string("m_cost:            ")+toString(m_cost));
    }


}
