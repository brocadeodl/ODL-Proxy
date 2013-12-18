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
 *   Author : Venkat                                                       *
 **************************************************************************/
#include "Nsm/Local/VRRPWorker/VRRPLocalShowMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    VRRPLocalShowMessage::VRRPLocalShowMessage()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), VRRP_LOCAL_SHOW_MSG)
    {
        m_vrid = 0;
        m_ifName = "";
        m_cmdCode = VRRP_SHOW_CMD;
        m_showType = VRRP_SHOW_CMD_TYPE_NONE;
        m_ifType = IF_TYPE_INVALID;
		m_lastIndex = 0;
    }

    VRRPLocalShowMessage::VRRPLocalShowMessage(const UI32 &vrid, const string &ifName, const UI32 &cmdCode, const UI32 &showType, const InterfaceType &ifType)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), VRRP_LOCAL_SHOW_MSG),
        m_vrid (vrid),
        m_ifName (ifName),
        m_cmdCode (cmdCode),
        m_showType (showType),
        m_ifType (ifType)
    {
		m_lastIndex = 0;
    }

    VRRPLocalShowMessage::~VRRPLocalShowMessage ()
    {
    }

    void  VRRPLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32 (&m_showType, "showType"));
        addSerializableAttribute (new AttributeUI32 (&m_vrid, "vrid"));
        addSerializableAttribute (new AttributeString (&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeEnum ((UI32 *)&m_ifType, "ifType"));
		addSerializableAttribute (new AttributeUI32 (&m_lastIndex, "lastIndex"));
    }

    void  VRRPLocalShowMessage::setVrid(const UI32 &vrid)
    {
        m_vrid = vrid;
    }

    UI32 VRRPLocalShowMessage::getVrid() const
    {
        return (m_vrid);
    }
    
    void  VRRPLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  VRRPLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  VRRPLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  VRRPLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  VRRPLocalShowMessage::setShowType(const UI32 &showType)
    {
        m_showType  =  showType;
    }

    UI32  VRRPLocalShowMessage::getShowType() const
    {
        return (m_showType);
    }

    void  VRRPLocalShowMessage::setIfType(const InterfaceType &ifType)
    {
        m_ifType  =  ifType;
    }

    InterfaceType  VRRPLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void VRRPLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void VRRPLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void  VRRPLocalShowMessage::setLastIndex(const UI32 &lastIndex)
    {
        m_lastIndex  =  lastIndex;
    }

    UI32  VRRPLocalShowMessage::getLastIndex() const
    {
        return (m_lastIndex);
    }

}
