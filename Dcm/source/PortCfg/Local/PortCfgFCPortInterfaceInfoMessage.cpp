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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFCPortInterfaceInfoMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFCPortInterfaceInfoMessage::PortCfgFCPortInterfaceInfoMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTINTERFACEINFO)
    {
    }

    PortCfgFCPortInterfaceInfoMessage::PortCfgFCPortInterfaceInfoMessage (const string &portId,const string &portType,const string &portWWN,const string &remotePortWWN,const string &remoteNodeWWN,const string &state,const string &status,const string &statusMessage,const string &health,const bool &trunked,const UI32 &trunkMaster,const UI32 &actualDistance,const UI32 &desiredCredit,const UI32 &bufferAllocated,const UI32 &switchPort,const bool &licensed,const string &portaddr)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTINTERFACEINFO),
        m_portId    (portId),
        m_portType    (portType),
        m_portWWN    (portWWN),
        m_remotePortWWN    (remotePortWWN),
        m_remoteNodeWWN    (remoteNodeWWN),
        m_state    (state),
        m_status    (status),
        m_statusMessage    (statusMessage),
        m_health    (health),
        m_trunked    (trunked),
        m_trunkMaster    (trunkMaster),
        m_actualDistance    (actualDistance),
        m_desiredCredit    (desiredCredit),
        m_bufferAllocated    (bufferAllocated),
        m_switchPort    (switchPort),
        m_licensed    (licensed),
        m_portaddr    (portaddr)
    {
    }

    PortCfgFCPortInterfaceInfoMessage::~PortCfgFCPortInterfaceInfoMessage ()
    {
    }

    void  PortCfgFCPortInterfaceInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeString(&m_portType,"portType"));
        addSerializableAttribute (new AttributeString(&m_portWWN,"portWWN"));
        addSerializableAttribute (new AttributeString(&m_remotePortWWN,"remotePortWWN"));
        addSerializableAttribute (new AttributeString(&m_remoteNodeWWN,"remoteNodeWWN"));
        addSerializableAttribute (new AttributeString(&m_state,"state"));
        addSerializableAttribute (new AttributeString(&m_status,"status"));
        addSerializableAttribute (new AttributeString(&m_statusMessage,"statusMessage"));
        addSerializableAttribute (new AttributeString(&m_health,"health"));
        addSerializableAttribute (new AttributeBool(&m_trunked,"trunked"));
        addSerializableAttribute (new AttributeUI32(&m_trunkMaster,"trunkMaster"));
        addSerializableAttribute (new AttributeUI32(&m_actualDistance,"actualDistance"));
        addSerializableAttribute (new AttributeUI32(&m_desiredCredit,"desiredCredit"));
        addSerializableAttribute (new AttributeUI32(&m_bufferAllocated,"bufferAllocated"));
        addSerializableAttribute (new AttributeUI32(&m_switchPort,"switchPort"));
        addSerializableAttribute (new AttributeBool(&m_licensed,"licensed"));
        addSerializableAttribute (new AttributeString(&m_portaddr,"portaddr"));
    }

    void  PortCfgFCPortInterfaceInfoMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getPortId() const
    {
        return (m_portId);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setPortType(const string &portType)
    {
        m_portType  =  portType;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getPortType() const
    {
        return (m_portType);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setPortWWN(const string &portWWN)
    {
        m_portWWN  =  portWWN;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getPortWWN() const
    {
        return (m_portWWN);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setRemotePortWWN(const string &remotePortWWN)
    {
        m_remotePortWWN  =  remotePortWWN;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getRemotePortWWN() const
    {
        return (m_remotePortWWN);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setRemoteNodeWWN(const string &remoteNodeWWN)
    {
        m_remoteNodeWWN  =  remoteNodeWWN;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getRemoteNodeWWN() const
    {
        return (m_remoteNodeWWN);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setState(const string &state)
    {
        m_state  =  state;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getState() const
    {
        return (m_state);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setStatus(const string &status)
    {
        m_status  =  status;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getStatus() const
    {
        return (m_status);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setStatusMessage(const string &statusMessage)
    {
        m_statusMessage  =  statusMessage;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getStatusMessage() const
    {
        return (m_statusMessage);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setHealth(const string &health)
    {
        m_health  =  health;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getHealth() const
    {
        return (m_health);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setTrunked(const bool &trunked)
    {
        m_trunked  =  trunked;
    }

    bool  PortCfgFCPortInterfaceInfoMessage::getTrunked() const
    {
        return (m_trunked);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setTrunkMaster(const UI32 &trunkMaster)
    {
        m_trunkMaster  =  trunkMaster;
    }

    UI32  PortCfgFCPortInterfaceInfoMessage::getTrunkMaster() const
    {
        return (m_trunkMaster);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setActualDistance(const UI32 &actualDistance)
    {
        m_actualDistance  =  actualDistance;
    }

    UI32  PortCfgFCPortInterfaceInfoMessage::getActualDistance() const
    {
        return (m_actualDistance);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setDesiredCredit(const UI32 &desiredCredit)
    {
        m_desiredCredit  =  desiredCredit;
    }

    UI32  PortCfgFCPortInterfaceInfoMessage::getDesiredCredit() const
    {
        return (m_desiredCredit);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setBufferAllocated(const UI32 &bufferAllocated)
    {
        m_bufferAllocated  =  bufferAllocated;
    }

    UI32  PortCfgFCPortInterfaceInfoMessage::getBufferAllocated() const
    {
        return (m_bufferAllocated);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setSwitchPort(const UI32 &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    UI32  PortCfgFCPortInterfaceInfoMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }
    void  PortCfgFCPortInterfaceInfoMessage::setLicensed(const bool &licensed)
    {
        m_licensed  =  licensed;
    }

    bool  PortCfgFCPortInterfaceInfoMessage::getLicensed() const
    {
        return (m_licensed);
    }

    void  PortCfgFCPortInterfaceInfoMessage::setPortaddr(const string &portaddr)
    {
        m_portaddr  =  portaddr;
    }

    string  PortCfgFCPortInterfaceInfoMessage::getPortaddr() const
    {
        return (m_portaddr);
    }

}
