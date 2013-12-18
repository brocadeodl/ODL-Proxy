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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpScalarsConfSetMessage.h"
#include "Lldp/Local/LldpTypes.h"

namespace DcmNs
{

    LldpScalarsConfSetMessage::LldpScalarsConfSetMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),LLDPSCALARSCONFSET)
    {
    }

    LldpScalarsConfSetMessage::LldpScalarsConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &description,const UI32 &hello,const UI32 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const UI32 &iscsiPriority,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv,const string &sysName,const string &sysDescr,const bool &disable)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),LLDPSCALARSCONFSET),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_description    (description),
        m_hello    (hello),
        m_mode    (mode),
        m_multiplier    (multiplier),
        m_dcbxFcoeAppTlv    (dcbxFcoeAppTlv),
        m_dcbxFcoeLogicalLinkTlv    (dcbxFcoeLogicalLinkTlv),
        m_dcbxIscsiAppTlv    (dcbxIscsiAppTlv),
        m_iscsiPriority    (iscsiPriority),
        m_dcbxTlv    (dcbxTlv),
        m_dot1xTlv    (dot1xTlv),
        m_dot3xTlv    (dot3xTlv),
        m_mgmtAddrTlv    (mgmtAddrTlv),
        m_portDescrTlv    (portDescrTlv),
        m_sysCapTlv    (sysCapTlv),
        m_sysDescrTlv    (sysDescrTlv),
        m_sysNameTlv    (sysNameTlv),
        m_sysName    (sysName),
        m_sysDescr    (sysDescr),
        m_disable    (disable)
    {
    }

    LldpScalarsConfSetMessage::~LldpScalarsConfSetMessage ()
    {
    }

    void  LldpScalarsConfSetMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeUI32(&m_hello,"hello"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_multiplier,"multiplier"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeAppTlv,"dcbxFcoeAppTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeLogicalLinkTlv,"dcbxFcoeLogicalLinkTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxIscsiAppTlv,"dcbxIscsiAppTlv"));
        addSerializableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority"));
        addSerializableAttribute (new AttributeBool(&m_dcbxTlv,"dcbxTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot1xTlv,"dot1xTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot3xTlv,"dot3xTlv"));
        addSerializableAttribute (new AttributeBool(&m_mgmtAddrTlv,"mgmtAddrTlv"));
        addSerializableAttribute (new AttributeBool(&m_portDescrTlv,"portDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysCapTlv,"sysCapTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysDescrTlv,"sysDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysNameTlv,"sysNameTlv"));
        addSerializableAttribute (new AttributeString(&m_sysName,"sysName"));
        addSerializableAttribute (new AttributeString(&m_sysDescr,"sysDescr"));
        addSerializableAttribute (new AttributeBool(&m_disable,"disable"));
    }

    void  LldpScalarsConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpScalarsConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpScalarsConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpScalarsConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpScalarsConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpScalarsConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpScalarsConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpScalarsConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpScalarsConfSetMessage::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpScalarsConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpScalarsConfSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LldpScalarsConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpScalarsConfSetMessage::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpScalarsConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpScalarsConfSetMessage::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpScalarsConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpScalarsConfSetMessage::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpScalarsConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

    void  LldpScalarsConfSetMessage::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
    {
        m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
    }

    bool  LldpScalarsConfSetMessage::getDcbxIscsiAppTlv() const
    {
        return (m_dcbxIscsiAppTlv);
    }

    void  LldpScalarsConfSetMessage::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  LldpScalarsConfSetMessage::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

    void  LldpScalarsConfSetMessage::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpScalarsConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpScalarsConfSetMessage::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpScalarsConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpScalarsConfSetMessage::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpScalarsConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpScalarsConfSetMessage::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpScalarsConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpScalarsConfSetMessage::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpScalarsConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpScalarsConfSetMessage::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpScalarsConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpScalarsConfSetMessage::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpScalarsConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpScalarsConfSetMessage::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpScalarsConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

    void  LldpScalarsConfSetMessage::setSysName(const string &sysName)
    {
        m_sysName  =  sysName;
    }

    string  LldpScalarsConfSetMessage::getSysName() const
    {
        return (m_sysName);
    }

    void  LldpScalarsConfSetMessage::setSysDescr(const string &sysDescr)
    {
        m_sysDescr  =  sysDescr;
    }

    string  LldpScalarsConfSetMessage::getSysDescr() const
    {
        return (m_sysDescr);
    }

    void  LldpScalarsConfSetMessage::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  LldpScalarsConfSetMessage::getDisable() const
    {
        return (m_disable);
    }

}
