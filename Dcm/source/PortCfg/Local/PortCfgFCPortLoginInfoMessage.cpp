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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFCPortLoginInfoMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFCPortLoginInfoMessage::PortCfgFCPortLoginInfoMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTLOGININFO)
    {
    }

    PortCfgFCPortLoginInfoMessage::PortCfgFCPortLoginInfoMessage (const UI32 &cmdCode,const vector<string> &portId,const vector<UI32> &rbridgeId,const vector<UI32> &index,const vector<string> &pid,const vector<string> &portWWN,const vector<string> &configMode,const vector<string> &portstatus,const vector<string> &portspeed,const vector<UI32> &device_count)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTLOGININFO),
        m_cmdCode    (cmdCode),
        m_portId    (portId),
        m_rbridgeId    (rbridgeId),
        m_index    (index),
        m_pid    (pid),
        m_portWWN    (portWWN),
        m_configMode    (configMode),
        m_portstatus    (portstatus),
        m_portspeed    (portspeed),
        m_device_count    (device_count)
    {
    }

    PortCfgFCPortLoginInfoMessage::~PortCfgFCPortLoginInfoMessage ()
    {
    }

    void  PortCfgFCPortLoginInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeStringVector(&m_portId,"portId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_index,"index"));
        addSerializableAttribute (new AttributeStringVector(&m_pid,"pid"));
        addSerializableAttribute (new AttributeStringVector(&m_portWWN,"portWWN"));
        addSerializableAttribute (new AttributeStringVector(&m_configMode,"configMode"));
        addSerializableAttribute (new AttributeStringVector(&m_portstatus,"portstatus"));
        addSerializableAttribute (new AttributeStringVector(&m_portspeed,"portspeed"));
        addSerializableAttribute (new AttributeUI32Vector(&m_device_count,"device_count"));
    }

    void  PortCfgFCPortLoginInfoMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PortCfgFCPortLoginInfoMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PortCfgFCPortLoginInfoMessage::setPortId(const vector<string> &portId)
    {
        m_portId  =  portId;
    }

    vector<string>  PortCfgFCPortLoginInfoMessage::getPortId() const
    {
        return (m_portId);
    }

    void  PortCfgFCPortLoginInfoMessage::setRbridgeId(const vector<UI32> &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    vector<UI32>  PortCfgFCPortLoginInfoMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void  PortCfgFCPortLoginInfoMessage::setIndex(const vector<UI32> &index)
    {
        m_index  =  index;
    }

    vector<UI32>  PortCfgFCPortLoginInfoMessage::getIndex() const
    {
        return (m_index);
    }

    void  PortCfgFCPortLoginInfoMessage::setPid(const vector<string> &pid)
    {
        m_pid  =  pid;
    }

    vector<string> PortCfgFCPortLoginInfoMessage::getPid() const
    {
        return (m_pid);
    }

    void  PortCfgFCPortLoginInfoMessage::setPortWWN(const vector<string> &portWWN)
    {
        m_portWWN  =  portWWN;
    }

    vector<string>  PortCfgFCPortLoginInfoMessage::getPortWWN() const
    {
        return (m_portWWN);
    }

    void  PortCfgFCPortLoginInfoMessage::setConfigMode(const vector<string> &configMode)
    {
        m_configMode  =  configMode;
    }

    vector<string>  PortCfgFCPortLoginInfoMessage::getConfigMode() const
    {
        return (m_configMode);
    }

    void  PortCfgFCPortLoginInfoMessage::setPortstatus(const vector<string> &portstatus)
    {
        m_portstatus  =  portstatus;
    }

    vector<string>  PortCfgFCPortLoginInfoMessage::getPortstatus() const
    {
        return (m_portstatus);
    }
    void  PortCfgFCPortLoginInfoMessage::setPortspeed(const vector<string> &portspeed)
    {
        m_portspeed  =  portspeed;
    }

    vector<string>  PortCfgFCPortLoginInfoMessage::getPortspeed() const
    {
        return (m_portspeed);
    }

    void  PortCfgFCPortLoginInfoMessage::setDevice_count(const vector<UI32> &device_count)
    {
        m_device_count  =  device_count;
    }

    vector<UI32>  PortCfgFCPortLoginInfoMessage::getDevice_count() const
    {
        return (m_device_count);
    }

}
