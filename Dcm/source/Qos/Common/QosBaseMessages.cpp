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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Qos/Common/QosBaseMessages.h"

namespace DcmNs
{
    QosNasBaseMessage::QosNasBaseMessage ():
        m_opCode (0),
        m_cmdCode (0),
        m_cos (0),
        m_dscp (0),
        m_vrfName(""),
        m_vlanNumber (0)
    {
    }
    QosNasBaseMessage::QosNasBaseMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber):
        m_opCode (opCode),
        m_cmdCode (cmdCode),
        m_cos (cos),
        m_dscp (dscp),
        m_serverIpAddressPrefix (serverIpAddressPrefix),
        m_vrfName(vrfName),
        m_vlanNumber (vlanNumber)
    {
    }

    QosNasBaseMessage::~QosNasBaseMessage ()
    {
    }

    void QosNasBaseMessage::setOpCode (const UI32 &opCode)
    {
        m_opCode = opCode;
    }

    UI32 QosNasBaseMessage::getOpCode ()
    {
        return (m_opCode);
    }

	void QosNasBaseMessage::setCmdCode (const UI32 &cmdCode)
    {
        m_cmdCode = cmdCode;
    }

    UI32 QosNasBaseMessage::getCmdCode ()
    {
        return (m_cmdCode);
    }

    void QosNasBaseMessage::setCos (const UI32 &cos)
    {
        m_cos = cos;
    }

    UI32 QosNasBaseMessage::getCos ()
    {
        return (m_cos);
    }

    void QosNasBaseMessage::setDscp(const UI32 &dscp)
    {
        m_dscp = dscp;
    }

    UI32 QosNasBaseMessage::getDscp ()
    {
        return (m_dscp);
    }

    void QosNasBaseMessage::setServerIpAddressPrefix (const IpV4AddressNetworkMask &serverIpAddressPrefix)
    {
        m_serverIpAddressPrefix = serverIpAddressPrefix;
    }


    IpV4AddressNetworkMask QosNasBaseMessage::getServerIpAddressPrefix()
    {
        return (m_serverIpAddressPrefix);
    }

    void QosNasBaseMessage::setVrfName (const string &vrfName)
    {
        m_vrfName = vrfName;
    }

    string QosNasBaseMessage::getVrfName ()
    {
        return (m_vrfName);
    }

	UI32 QosNasBaseMessage::getVlanNumber ()
    {
        return (m_vlanNumber);
    }

	void QosNasBaseMessage::setVlanNumber (const UI32 &vlanNumber)
    {
        m_vlanNumber = vlanNumber;
    }



	/*

    void QosNasBaseMessage::copyFromGlobal (QosGlobalNasMessage *msg)
    {
        m_opCode = msg->getOpCode();
        m_cmdCode = msg->getCmdCode();
        m_cos = msg->getCos();
        m_dscp = msg->getDscp();
        m_serverIpAddressPrefix = msg->getServerIpAddressPrefix();
        m_vrfName = msg->getVrfName();
        m_vlanNumber = msg->getVlanNumber();

    }
    void QosNasBaseMessage::copyFromLocal (QosLocalNasMessage *msg)
    {
        m_opCode = msg->getOpCode();
        m_cmdCode = msg->getCmdCode();
        m_cos = msg->getCos();
        m_dscp = msg->getDscp();
        m_serverIpAddressPrefix = msg->getServerIpAddressPrefix();
        m_vrfName = msg->getVrfName();
        m_vlanNumber = msg->getVlanNumber();
    }
	*/
}
