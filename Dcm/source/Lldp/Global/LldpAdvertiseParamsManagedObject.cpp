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

#include "Lldp/Global/LldpAdvertiseParamsManagedObject.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "vcs.h"

namespace DcmNs
{

    LldpAdvertiseParamsManagedObject::LldpAdvertiseParamsManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpAdvertiseParamsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager)
    {
    }

    LldpAdvertiseParamsManagedObject::LldpAdvertiseParamsManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const string &description,const UI32 &hello,const LldpMode &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const UI32 &iscsiPriority,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpAdvertiseParamsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager),
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
        m_sysNameTlv    (sysNameTlv)
    {
    }

    LldpAdvertiseParamsManagedObject::~LldpAdvertiseParamsManagedObject ()
    {
    }

    string  LldpAdvertiseParamsManagedObject::getClassName()
    {
        return ("LldpAdvertiseParamsManagedObject");
    }

    void  LldpAdvertiseParamsManagedObject::setupAttributesForPersistence()
    {
		UI32 appTypeDefault = (UI32)LLDP_MODE_RXTX;
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_description,true,"","description",lldp_description));
        addPersistableAttribute (new AttributeUI32(&m_hello,"hello",lldp_hello));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_mode),appTypeDefault,"mode",lldp_mode));
        addPersistableAttribute (new AttributeUI32(&m_multiplier,"multiplier",lldp_multiplier));
		addPersistableAttribute (new AttributeBool(&m_dcbxFcoeAppTlv,"dcbxFcoeAppTlv",lldp_dcbx_fcoe_app_tlv));
        addPersistableAttribute (new AttributeBool(&m_dcbxFcoeLogicalLinkTlv,"dcbxFcoeLogicalLinkTlv",lldp_dcbx_fcoe_logical_link_tlv));
        addPersistableAttribute (new AttributeBool(&m_dcbxIscsiAppTlv,"dcbxIscsiAppTlv",lldp_dcbx_iscsi_app_tlv));
        addPersistableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority",lldp_iscsi_priority));
        addPersistableAttribute (new AttributeBool(&m_dcbxTlv,"dcbxTlv",lldp_dcbx_tlv));
        addPersistableAttribute (new AttributeBool(&m_dot1xTlv,"dot1xTlv",lldp_dot1_tlv));
        addPersistableAttribute (new AttributeBool(&m_dot3xTlv,"dot3xTlv",lldp_dot3_tlv));
        addPersistableAttribute (new AttributeBool(&m_mgmtAddrTlv,"mgmtAddrTlv",lldp_management_address));
        addPersistableAttribute (new AttributeBool(&m_portDescrTlv,"portDescrTlv",lldp_port_description));
        addPersistableAttribute (new AttributeBool(&m_sysCapTlv,"sysCapTlv",lldp_system_capabilities));
        addPersistableAttribute (new AttributeBool(&m_sysDescrTlv,"sysDescrTlv",lldp_adv_tlv_system_description));
        addPersistableAttribute (new AttributeBool(&m_sysNameTlv,"sysNameTlv",lldp_adv_tlv_system_name));
    }

    void  LldpAdvertiseParamsManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description",lldp_description));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_hello,"hello",lldp_hello));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_mode),"mode",lldp_mode));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_multiplier,"multiplier",lldp_multiplier));
		addPersistableAttributeForCreate  (new AttributeBool(&m_dcbxFcoeAppTlv,"dcbxFcoeAppTlv",lldp_dcbx_fcoe_app_tlv));
	    addPersistableAttributeForCreate  (new AttributeBool(&m_dcbxFcoeLogicalLinkTlv,"dcbxFcoeLogicalLinkTlv",lldp_dcbx_fcoe_logical_link_tlv));
        addPersistableAttributeForCreate  (new AttributeBool(&m_dcbxIscsiAppTlv,"dcbxIscsiAppTlv",lldp_dcbx_iscsi_app_tlv));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_iscsiPriority,"iscsiPriority",lldp_iscsi_priority));
        addPersistableAttributeForCreate  (new AttributeBool(&m_dcbxTlv,"dcbxTlv",lldp_dcbx_tlv));
        addPersistableAttributeForCreate  (new AttributeBool(&m_dot1xTlv,"dot1xTlv",lldp_dot1_tlv));
        addPersistableAttributeForCreate  (new AttributeBool(&m_dot3xTlv,"dot3xTlv",lldp_dot3_tlv));
        addPersistableAttributeForCreate  (new AttributeBool(&m_mgmtAddrTlv,"mgmtAddrTlv",lldp_management_address));
        addPersistableAttributeForCreate  (new AttributeBool(&m_portDescrTlv,"portDescrTlv",lldp_port_description));
        addPersistableAttributeForCreate  (new AttributeBool(&m_sysCapTlv,"sysCapTlv",lldp_system_capabilities));
        addPersistableAttributeForCreate  (new AttributeBool(&m_sysDescrTlv,"sysDescrTlv",lldp_adv_tlv_system_description));
        addPersistableAttributeForCreate  (new AttributeBool(&m_sysNameTlv,"sysNameTlv",lldp_adv_tlv_system_name));
    }

    void  LldpAdvertiseParamsManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpAdvertiseParamsManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  LldpAdvertiseParamsManagedObject::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpAdvertiseParamsManagedObject::getHello() const
    {
        return (m_hello);
    }

    void  LldpAdvertiseParamsManagedObject::setMode(const LldpMode &mode)
    {
        m_mode  =  mode;
    }

    LldpMode  LldpAdvertiseParamsManagedObject::getMode() const
    {
        return (m_mode);
    }

    void  LldpAdvertiseParamsManagedObject::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpAdvertiseParamsManagedObject::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpAdvertiseParamsManagedObject::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpAdvertiseParamsManagedObject::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	bool  LldpAdvertiseParamsManagedObject::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpAdvertiseParamsManagedObject::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  LldpAdvertiseParamsManagedObject::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

	void  LldpAdvertiseParamsManagedObject::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpAdvertiseParamsManagedObject::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpAdvertiseParamsManagedObject::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

}
