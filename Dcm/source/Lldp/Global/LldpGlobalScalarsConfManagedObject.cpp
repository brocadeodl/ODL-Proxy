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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Authors : Mobin Mohan & Sanjeev Joshi                                 *
 **************************************************************************/

#include "Lldp/Global/LldpGlobalScalarsConfManagedObject.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "vcs.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    LldpGlobalScalarsConfManagedObject::LldpGlobalScalarsConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpGlobalScalarsConfManagedObject::getClassName (), LldpAdvertiseParamsManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager),
        LldpAdvertiseParamsManagedObject (pLldpGlobalObjectManager)
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalScalarsConfManagedObject::Constructor"
			                                          "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARCONFMANAGEDOBJECT,sizeof(LldpGlobalScalarsConfManagedObject),DCMMEMSTATINCREMENT);
    }

    LldpGlobalScalarsConfManagedObject::LldpGlobalScalarsConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const string &sysName,const string &sysDescr,const bool &disable, const bool &protocolFlag)
        : PrismElement  (pLldpGlobalObjectManager),
        PrismPersistableObject (LldpGlobalScalarsConfManagedObject::getClassName (), LldpAdvertiseParamsManagedObject::getClassName ()),
        WaveManagedObject (pLldpGlobalObjectManager),
        DcmManagedObject (pLldpGlobalObjectManager),
        LldpAdvertiseParamsManagedObject (pLldpGlobalObjectManager),
        m_sysName    (sysName),
        m_sysDescr    (sysDescr),
        m_disable    (disable)
    {
		trace (TRACE_LEVEL_INFO," LldpGlobalScalarsConfManagedObject::Constructor"
			                                                      "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARCONFMANAGEDOBJECT,sizeof(LldpGlobalScalarsConfManagedObject),DCMMEMSTATINCREMENT);
    }

    LldpGlobalScalarsConfManagedObject::~LldpGlobalScalarsConfManagedObject ()
    {
        trace (TRACE_LEVEL_INFO," LldpGlobalScalarsConfManagedObject::Destructor"
			                                                      "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARCONFMANAGEDOBJECT,sizeof(LldpGlobalScalarsConfManagedObject),DCMMEMSTATDECREMENT);
    }

    string  LldpGlobalScalarsConfManagedObject::getClassName()
    {
        return ("LldpGlobalScalarsConfManagedObject");
    }

    void  LldpGlobalScalarsConfManagedObject::setupAttributesForPersistence()
    {
        LldpAdvertiseParamsManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_sysName,"sysName",lldp_system_name));
        addPersistableAttribute (new AttributeString(&m_sysDescr,"sysDescr",lldp_system_description));
        addPersistableAttribute (new AttributeBool(&m_disable,"disable",lldp_disable));
        addPersistableAttribute (new AttributeBool(&m_protocolFlag,"protocolFlag",lldp_lldp));
    }

    void  LldpGlobalScalarsConfManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeString(&m_sysName,"sysName",lldp_system_name));
        addPersistableAttributeForCreate  (new AttributeString(&m_sysDescr,"sysDescr",lldp_system_description));
        addPersistableAttributeForCreate  (new AttributeBool(&m_disable,"disable",lldp_disable));
        addPersistableAttributeForCreate  (new AttributeBool(&m_protocolFlag,"protocolFlag",lldp_lldp));
    }

    void  LldpGlobalScalarsConfManagedObject::setSysName(const string &sysName)
    {
        m_sysName  =  sysName;
    }

    string  LldpGlobalScalarsConfManagedObject::getSysName() const
    {
        return (m_sysName);
    }

    void  LldpGlobalScalarsConfManagedObject::setSysDescr(const string &sysDescr)
    {
        m_sysDescr  =  sysDescr;
    }

    string  LldpGlobalScalarsConfManagedObject::getSysDescr() const
    {
        return (m_sysDescr);
    }

    void  LldpGlobalScalarsConfManagedObject::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  LldpGlobalScalarsConfManagedObject::getDisable() const
    {
        return (m_disable);
    }
    
	void  LldpGlobalScalarsConfManagedObject::setProtocolFlag(const bool &protocolFlag)
    {
        m_protocolFlag  =  protocolFlag;
    }

    bool  LldpGlobalScalarsConfManagedObject::getProtocolFlag() const
    {
        return (m_protocolFlag);
    }

}
