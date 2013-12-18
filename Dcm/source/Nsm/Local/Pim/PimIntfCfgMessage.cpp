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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Local/PimLocalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "PimIntfCfgMessage.h"

namespace DcmNs
{

    PimIntfCfgMessage::PimIntfCfgMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALPIMINTFMSG)
    {
    }

    PimIntfCfgMessage::~PimIntfCfgMessage ()
    {
    }

    void  PimIntfCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI8(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeUI32(&m_drPriority,"drPriority"));
		/* Changing the type as we dont accept the acl name as of now */
        //addSerializableAttribute (new AttributeString(&m_multBoundaryAcl,"multBoundaryAcl"));
		addSerializableAttribute (new AttributeBool(&m_multBoundaryAcl,"multBoundaryAcl"));
        addSerializableAttribute (new AttributeBool(&m_pimBorder,"pimBorder"));
        addSerializableAttribute (new AttributeUI32(&m_ttlThold,"ttlThold"));
    }

    void  PimIntfCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimIntfCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimIntfCfgMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  PimIntfCfgMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PimIntfCfgMessage::setIntfType(const UI8 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI8  PimIntfCfgMessage::getIntfType() const
    {
        return (m_intfType);
    }
    
    void  PimIntfCfgMessage::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  PimIntfCfgMessage::getIntfName() const
    {
        return (m_intfName);
    }
    
    void  PimIntfCfgMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  PimIntfCfgMessage::getEnable() const
    {
        return (m_enable);
    }

    void  PimIntfCfgMessage::setDrPriority(const UI32 &drPriority)
    {
        m_drPriority  =  drPriority;
    }

    UI32  PimIntfCfgMessage::getDrPriority() const
    {
        return (m_drPriority);
    }

	/* Changing the type as we dont accept the acl name now */
#if 0
    void  PimIntfCfgMessage::setMultBoundaryAcl(const string &multBoundaryAcl)
    {
        m_multBoundaryAcl  =  multBoundaryAcl;
    }

    string  PimIntfCfgMessage::getMultBoundaryAcl() const
    {
        return (m_multBoundaryAcl);
    }
#endif
	void  PimIntfCfgMessage::setMultBoundaryAcl(const bool &multBoundaryAcl)
	{
		m_multBoundaryAcl  =  multBoundaryAcl;
	}
	bool PimIntfCfgMessage::getMultBoundaryAcl() const
	{
		return (m_multBoundaryAcl);
	}

    void  PimIntfCfgMessage::setPimBorder(const bool &pimBorder)
    {
        m_pimBorder  =  pimBorder;
    }

    bool  PimIntfCfgMessage::getPimBorder() const
    {
        return (m_pimBorder);
    }
    
    void PimIntfCfgMessage::setTtlThold(const UI32 &ttlThold)
    {
        m_ttlThold  =  ttlThold;
    }
    
    UI32 PimIntfCfgMessage::getTtlThold() const
    {
        return (m_ttlThold);
    }

}
