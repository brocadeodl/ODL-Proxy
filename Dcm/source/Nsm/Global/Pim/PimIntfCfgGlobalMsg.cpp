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
 *   Author : Rahul Jain                                                   *
 **************************************************************************/

#include "Nsm/Global/Pim/PimIntfCfgGlobalMsg.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    PimIntfCfgGlobalMsg::PimIntfCfgGlobalMsg ()
        : ManagementInterfaceMessage(NsmGlobalObjectManager::getClassName(), 
              NSMGLOBALPIMINTFCFG)
    {
    }

    PimIntfCfgGlobalMsg::~PimIntfCfgGlobalMsg ()
    {
    }

    void  PimIntfCfgGlobalMsg::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI8(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeUI32(&m_drPriority,"drPriority"));
		/* Changing the type as we are no accepting acl name now */
        //addSerializableAttribute (new AttributeString(&m_multBoundaryAcl,"multBoundaryAcl"));
		addSerializableAttribute (new AttributeBool(&m_multBoundaryAcl,"multBoundaryAcl"));
        addSerializableAttribute (new AttributeBool(&m_pimBorder,"pimBorder"));
        addSerializableAttribute (new AttributeUI32(&m_ttlThold,"ttlThold"));
    }

    void  PimIntfCfgGlobalMsg::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimIntfCfgGlobalMsg::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimIntfCfgGlobalMsg::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  PimIntfCfgGlobalMsg::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PimIntfCfgGlobalMsg::setIntfType(const UI8 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI8  PimIntfCfgGlobalMsg::getIntfType() const
    {
        return (m_intfType);
    }
    
    void  PimIntfCfgGlobalMsg::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  PimIntfCfgGlobalMsg::getIntfName() const
    {
        return (m_intfName);
    }
    
    void  PimIntfCfgGlobalMsg::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  PimIntfCfgGlobalMsg::getEnable() const
    {
        return (m_enable);
    }

    void  PimIntfCfgGlobalMsg::setDrPriority(const UI32 &drPriority)
    {
        m_drPriority  =  drPriority;
    }

    UI32  PimIntfCfgGlobalMsg::getDrPriority() const
    {
        return (m_drPriority);
    }
	
	/* Changing the type as we dont accept the acl name now */
#if 0
    void  PimIntfCfgGlobalMsg::setMultBoundaryAcl(const string &multBoundaryAcl)
    {
        m_multBoundaryAcl  =  multBoundaryAcl;
    }

    string  PimIntfCfgGlobalMsg::getMultBoundaryAcl() const
    {
        return (m_multBoundaryAcl);
    }
#endif
	void  PimIntfCfgGlobalMsg::setMultBoundaryAcl(const bool &multBoundaryAcl)
	{
		 m_multBoundaryAcl  =  multBoundaryAcl;		 
	}
	bool PimIntfCfgGlobalMsg::getMultBoundaryAcl() const
	{
		return (m_multBoundaryAcl);
	}

    void  PimIntfCfgGlobalMsg::setPimBorder(const bool &pimBorder)
    {
        m_pimBorder  =  pimBorder;
    }

    bool  PimIntfCfgGlobalMsg::getPimBorder() const
    {
        return (m_pimBorder);
    }
    
    void PimIntfCfgGlobalMsg::setTtlThold(const UI32 &ttlThold)
    {
        m_ttlThold  =  ttlThold;
    }
    
    UI32 PimIntfCfgGlobalMsg::getTtlThold() const
    {
        return (m_ttlThold);
    }
}
