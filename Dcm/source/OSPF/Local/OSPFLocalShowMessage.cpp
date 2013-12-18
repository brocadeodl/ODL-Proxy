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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.
 *   Author : Brunda                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "OSPF/Local/OSPFLocalShowMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "OSPF/Local/OSPFTypes.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <netinet/in.h>

using namespace WaveNs;

namespace DcmNs
{

    OSPFLocalShowMessage::OSPFLocalShowMessage ()
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (), OSPFLOCALSHOW)
    {
        m_cmdCode = 0;
        m_areaId = "";
        m_ipAddr1.fromString("0.0.0.0");
        m_ipAddr2.fromString("0.0.0.0");
        m_intVal = 0;
        m_strVal = "";
        m_ifName = "";
        m_moreDataFlag=0;
        m_vrf = "";

    }


    OSPFLocalShowMessage::~OSPFLocalShowMessage ()
    {
    }

    void  OSPFLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_areaId, "areaId"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr1, "ipAddr1"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr2, "ipAddr2"));
        addSerializableAttribute (new AttributeUI32(&m_intVal, "intVal"));
        addSerializableAttribute (new AttributeString(&m_strVal, "strVal"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeUI32(&m_moreDataFlag, "moreDataFlag"));
        addSerializableAttribute (new AttributeString(&m_vrf, "vrf"));
    }


    void  OSPFLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
         m_cmdCode  =  cmdCode;
    }

    UI32  OSPFLocalShowMessage::getCmdCode() const
    {
         return (m_cmdCode);
    }

    void OSPFLocalShowMessage::setAreaId(const string &areaId)
    {
         m_areaId = areaId;
    }

    string OSPFLocalShowMessage::getAreaId() const
    {
         return m_areaId;
    }

    void OSPFLocalShowMessage::setIpAddr1(const string &str)
    {
         m_ipAddr1.fromString(str);
    }

    IpV4Address OSPFLocalShowMessage::getIpAddr1() const
    {
        return m_ipAddr1;
    }

    void OSPFLocalShowMessage::setIpAddr2(const string &str)
    {
         m_ipAddr2.fromString(str);
    }

    IpV4Address OSPFLocalShowMessage::getIpAddr2() const
    {
         return m_ipAddr2;
    }

    void OSPFLocalShowMessage::setIntVal(const string &str)
    {
         m_intVal = atoi(str.c_str());
    }

    UI32 OSPFLocalShowMessage::getIntVal() const
    {
         return m_intVal;
    }

    void OSPFLocalShowMessage::setStrVal(const string &str)
    {
         m_strVal = str;
    }

    string OSPFLocalShowMessage::getStrVal() const
    {
         return m_strVal;
    }

    void OSPFLocalShowMessage::setIfname(const string &str)
    {
         m_ifName = str;
    }

    string OSPFLocalShowMessage::getIfname() const
    {
         return m_ifName;
    }

    void OSPFLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
         pBuffer = findBuffer(bufferNum, size);
    }

    void OSPFLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
         this->copyBuffersFrom(prismMessage);
    }

    void OSPFLocalShowMessage::setMoreDataFlag(const UI32 &moreDataFlag)
    {
        m_moreDataFlag  =  moreDataFlag;
    }

    UI32 OSPFLocalShowMessage::getMoreDataFlag() const
    {
        return (m_moreDataFlag);
    }


    void OSPFLocalShowMessage::setVRF(const string &vrfName)
    {
         m_vrf = vrfName;
    }

    string OSPFLocalShowMessage::getVRF() const
    {
         return (m_vrf);
    }

}

