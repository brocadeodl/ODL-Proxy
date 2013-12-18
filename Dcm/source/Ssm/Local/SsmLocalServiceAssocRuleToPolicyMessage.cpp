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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalServiceAssocRuleToPolicyMessage.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    SsmLocalServiceAssocRuleToPolicyMessage::SsmLocalServiceAssocRuleToPolicyMessage ()
        : PrismMessage (SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICEASSOCRULETOPOLICY)
    {
    }

    SsmLocalServiceAssocRuleToPolicyMessage::SsmLocalServiceAssocRuleToPolicyMessage (const string &policyName, 
		const UI32 &policyType, const UI64 &ruleid, const UI32 &action, const string &srcMac, 
		const string &srcMacMask, const AddrType &srcType, const string &dstMac, const string &dstMacMask, 
		const AddrType &dstType, const string &appType, const EtherType &appTypeEnum, const UI16 &appTypeInt,
		const bool &isCount, const bool &isLog, const UI32 &opcode, const UI8 &userEnteredSeqid)
        : PrismMessage		(SsmLocalObjectManager::getPrismServiceId (),SSMLOCALSERVICEASSOCRULETOPOLICY),
        m_policyName		(policyName),
        m_policyType		(policyType),
        m_ruleid			(ruleid),
        m_action			(action),
        m_srcMac			(srcMac),
        m_srcMacMask		(srcMacMask),
		m_srcType			(srcType),
        m_dstMac			(dstMac),
        m_dstMacMask		(dstMacMask),
		m_dstType			(dstType),
        m_appType			(appType),
		m_appTypeEnum		(appTypeEnum),
		m_appTypeInt		(appTypeInt),
        m_isCount			(isCount),
        m_isLog				(isLog),
        m_opcode			(opcode),
		m_userEnteredSeqid 	(userEnteredSeqid)
    {
    }

    SsmLocalServiceAssocRuleToPolicyMessage::~SsmLocalServiceAssocRuleToPolicyMessage ()
    {
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType,"policyType"));
        addSerializableAttribute (new AttributeUI64(&m_ruleid,"ruleid"));
        addSerializableAttribute (new AttributeUI32(&m_action,"action"));
        addSerializableAttribute (new AttributeString(&m_srcMac,"srcMac"));
        addSerializableAttribute (new AttributeString(&m_srcMacMask,"srcMacMask"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_srcType),"srcType"));
		addSerializableAttribute (new AttributeString(&m_dstMac,"dstMac"));
        addSerializableAttribute (new AttributeString(&m_dstMacMask,"dstMacMask"));
		addSerializableAttribute (new AttributeEnum((UI32 *)(&m_dstType),"dstType"));
        addSerializableAttribute (new AttributeString(&m_appType,"appType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_appTypeEnum),"appTypeEnum"));
        addSerializableAttribute (new AttributeUI16(&m_appTypeInt,"appTypeInt")); 
		addSerializableAttribute (new AttributeBool(&m_isCount,"isCount"));
        addSerializableAttribute (new AttributeBool(&m_isLog,"isLog"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
		addSerializableAttribute (new AttributeUI8(&m_userEnteredSeqid,"userEnteredSeqid"));
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmLocalServiceAssocRuleToPolicyMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  SsmLocalServiceAssocRuleToPolicyMessage::getRuleid() const
    {
        return (m_ruleid);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  SsmLocalServiceAssocRuleToPolicyMessage::getAction() const
    {
        return (m_action);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setSrcMac(const string &srcMac)
    {
        m_srcMac  =  srcMac;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getSrcMac() const
    {
        return (m_srcMac);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setSrcMacMask(const string &srcMacMask)
    {
        m_srcMacMask  =  srcMacMask;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getSrcMacMask() const
    {
        return (m_srcMacMask);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setDstMac(const string &dstMac)
    {
        m_dstMac  =  dstMac;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getDstMac() const
    {
        return (m_dstMac);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setDstMacMask(const string &dstMacMask)
    {
        m_dstMacMask  =  dstMacMask;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getDstMacMask() const
    {
        return (m_dstMacMask);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setAppType(const string &appType)
    {
        m_appType  =  appType;
    }

    string  SsmLocalServiceAssocRuleToPolicyMessage::getAppType() const
    {
        return (m_appType);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setAppTypeEnum(const EtherType &appTypeEnum)
    {
        m_appTypeEnum  =  appTypeEnum;
    }

    EtherType  SsmLocalServiceAssocRuleToPolicyMessage::getAppTypeEnum() const
    {
        return (m_appTypeEnum);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setAppTypeInt(const UI16 &appTypeInt)
    {
        m_appTypeInt  =  appTypeInt;
    }
    
    UI16  SsmLocalServiceAssocRuleToPolicyMessage::getAppTypeInt() const
    {
        return (m_appTypeInt);
    }
	
	void  SsmLocalServiceAssocRuleToPolicyMessage::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  SsmLocalServiceAssocRuleToPolicyMessage::getIsCount() const
    {
        return (m_isCount);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  SsmLocalServiceAssocRuleToPolicyMessage::getIsLog() const
    {
        return (m_isLog);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmLocalServiceAssocRuleToPolicyMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setSrcType(const AddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    AddrType  SsmLocalServiceAssocRuleToPolicyMessage::getSrcType() const
    {
        return (m_srcType);
    }
    void  SsmLocalServiceAssocRuleToPolicyMessage::setDstType(const AddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    AddrType  SsmLocalServiceAssocRuleToPolicyMessage::getDstType() const
    {
        return (m_dstType);
    }

    UI8  SsmLocalServiceAssocRuleToPolicyMessage::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  SsmLocalServiceAssocRuleToPolicyMessage::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }

}
