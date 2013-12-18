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

#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "vcs.h"
#include "Framework/Utils/TraceUtils.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeStringUC.h"

namespace DcmNs
{

    LldpGlobalProfileConfManagedObject::LldpGlobalProfileConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpGlobalProfileConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager)
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalProfileConfManagedObject::Constructor"
			                                                                  "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFMANAGEDOBJECT,sizeof(LldpGlobalProfileConfManagedObject),DCMMEMSTATINCREMENT);
    }

    LldpGlobalProfileConfManagedObject::LldpGlobalProfileConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const string &profileName)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpGlobalProfileConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager),
        m_profileName    (profileName)
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalProfileConfManagedObject::Constructor profile name"
			                                                                              "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFMANAGEDOBJECT,sizeof(LldpGlobalProfileConfManagedObject),DCMMEMSTATINCREMENT);
    }

    LldpGlobalProfileConfManagedObject::~LldpGlobalProfileConfManagedObject ()
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalProfileConfManagedObject::Destructor"
			                                                                              "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFMANAGEDOBJECT,sizeof(LldpGlobalProfileConfManagedObject),DCMMEMSTATDECREMENT);
    }

    string  LldpGlobalProfileConfManagedObject::getClassName()
    {
        return ("LldpGlobalProfileConfManagedObject");
    }

    void  LldpGlobalProfileConfManagedObject::setupAttributesForPersistence()
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalProfileConfManagedObject::setupAttributesForPersistence");
		UI32 appTypeDefault = (UI32)LLDP_MODE_RXTX;
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeStringUC(&m_description, true, true,"",
								"description",lldp_description));
        addPersistableAttribute (new AttributeUI32UC(&m_hello, true, true, 0,
								"hello",lldp_hello));
        addPersistableAttribute (new AttributeEnumUC(&m_mode, true, true, appTypeDefault,
								"mode",lldp_mode));
  	    addPersistableAttribute (new AttributeUI32UC(&m_multiplier, true, true, 0,
						"multiplier",lldp_multiplier));
		addPersistableAttribute (new AttributeBoolUC(&m_dcbxFcoeAppTlv, true,
								"dcbxFcoeAppTlv", lldp_dcbx_fcoe_app_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_dcbxFcoeLogicalLinkTlv,	true,
								"dcbxFcoeLogicalLinkTlv",lldp_dcbx_fcoe_logical_link_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_dcbxIscsiAppTlv, true, 
								"dcbxIscsiAppTlv",lldp_dcbx_iscsi_app_tlv));
//        addPersistableAttribute (new AttributeUI32UC(&m_iscsiPriority, true, 
//								"iscsiPriority",lldp_iscsi_priority));
//        addPersistableAttribute (new AttributeBoolUC(&m_dcbxTlv, true, 
//								"dcbxTlv",lldp_dcbx_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_dcbxTlv, false, 
								"dcbxTlv",lldp_dcbx_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_dot1xTlv, true, 
								"dot1xTlv",lldp_dot1_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_dot3xTlv, true, 
								"dot3xTlv",lldp_dot3_tlv));
        addPersistableAttribute (new AttributeBoolUC(&m_mgmtAddrTlv, true,
								"mgmtAddrTlv",lldp_management_address));
        addPersistableAttribute (new AttributeBoolUC(&m_portDescrTlv, true, 
								"portDescrTlv",lldp_port_description));
        addPersistableAttribute (new AttributeBoolUC(&m_sysCapTlv, true, 
								"sysCapTlv",lldp_system_capabilities));
        addPersistableAttribute (new AttributeBoolUC(&m_sysDescrTlv, true, 
								"sysDescrTlv",lldp_adv_tlv_system_description));
        addPersistableAttribute (new AttributeBoolUC(&m_sysNameTlv, true, 
								"sysNameTlv",lldp_adv_tlv_system_name));
        addPersistableAttribute (new AttributeString(&m_profileName, "profileName"));
		
		setUserTagForAttribute ("profileName", lldp_profile_name);
    }

    void  LldpGlobalProfileConfManagedObject::setupAttributesForCreate()
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalProfileConfManagedObject::setupAttributesForCreate");
        DcmManagedObject::setupAttributesForCreate ();
		UI32 appTypeDefault = (UI32)LLDP_MODE_RXTX;
        addPersistableAttributeForCreate (new AttributeStringUC(&m_description, true, true,"",
								"description",lldp_description));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_hello, true, true, 0,
								"hello",lldp_hello));
		addPersistableAttributeForCreate (new AttributeEnumUC(&m_mode, true, true, appTypeDefault,
								"mode",lldp_mode));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_multiplier, true, true, 0,
								"multiplier",lldp_multiplier));
		addPersistableAttributeForCreate (new AttributeBoolUC(&m_dcbxFcoeAppTlv, true,
								"dcbxFcoeAppTlv", lldp_dcbx_fcoe_app_tlv));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_dcbxFcoeLogicalLinkTlv,	true,
								"dcbxFcoeLogicalLinkTlv",lldp_dcbx_fcoe_logical_link_tlv));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_dcbxIscsiAppTlv, true, 
								"dcbxIscsiAppTlv",lldp_dcbx_iscsi_app_tlv));
//        addPersistableAttributeForCreate (new AttributeUI32UC(&m_iscsiPriority, true, 
//								"iscsiPriority",lldp_iscsi_priority));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_dcbxTlv, true, 
								"dcbxTlv",lldp_dcbx_tlv));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_dot1xTlv, true, 
								"dot1xTlv",lldp_dot1_tlv));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_dot3xTlv, true, 
								"dot3xTlv",lldp_dot3_tlv));
		addPersistableAttributeForCreate (new AttributeBoolUC(&m_mgmtAddrTlv, true,
								"mgmtAddrTlv",lldp_management_address));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_portDescrTlv, true, 
								"portDescrTlv",lldp_port_description));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_sysCapTlv, true, 
								"sysCapTlv",lldp_system_capabilities));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_sysDescrTlv, true, 
								"sysDescrTlv",lldp_adv_tlv_system_description));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_sysNameTlv, true, 
								"sysNameTlv",lldp_adv_tlv_system_name));
      	addPersistableAttributeForCreate (new AttributeString(&m_profileName, "profileName"));
		
		vector<string > keyName;
        keyName.push_back ("profileName");
        setUserDefinedKeyCombination (keyName);
    }
    
	void  LldpGlobalProfileConfManagedObject::setDescription(const StringUC &description)
    {
        m_description  =  description;
    }

    StringUC  LldpGlobalProfileConfManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  LldpGlobalProfileConfManagedObject::setHello(const UI32UC &hello)
    {
        m_hello  =  hello;
    }

    UI32UC  LldpGlobalProfileConfManagedObject::getHello() const
    {
        return (m_hello);
    }

    void  LldpGlobalProfileConfManagedObject::setMode(const EnumUC &mode)
    {
        m_mode  =  mode;
    }

    EnumUC  LldpGlobalProfileConfManagedObject::getMode() const
    {
        return (m_mode);
    }

    void  LldpGlobalProfileConfManagedObject::setMultiplier(const UI32UC &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32UC  LldpGlobalProfileConfManagedObject::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpGlobalProfileConfManagedObject::setDcbxFcoeAppTlv(const BoolUC &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setDcbxFcoeLogicalLinkTlv(const BoolUC &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpGlobalProfileConfManagedObject::setDcbxIscsiAppTlv(const BoolUC &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	BoolUC  LldpGlobalProfileConfManagedObject::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpGlobalProfileConfManagedObject::setIscsiPriority(const UI32UC &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32UC  LldpGlobalProfileConfManagedObject::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

	void  LldpGlobalProfileConfManagedObject::setDcbxTlv(const BoolUC &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setDot1xTlv(const BoolUC &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setDot3xTlv(const BoolUC &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setMgmtAddrTlv(const BoolUC &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setPortDescrTlv(const BoolUC &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setSysCapTlv(const BoolUC &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setSysDescrTlv(const BoolUC &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setSysNameTlv(const BoolUC &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    BoolUC  LldpGlobalProfileConfManagedObject::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

    void  LldpGlobalProfileConfManagedObject::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  LldpGlobalProfileConfManagedObject::getProfileName() const
    {
        return (m_profileName);
    }

}
