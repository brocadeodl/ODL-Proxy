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

#include "Ssm/Global/SsmAssocRuleToPolicyMsgPayload.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/Attributes.h"

namespace DcmNs
{
	SsmAssocRuleToPolicyMsgPayload::SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg)
		: WaveMessagePayloadBase(pPrismMsg)
	{
	}

    SsmAssocRuleToPolicyMsgPayload::SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg, const string &policyName,const UI32 &policyType,const UI64 &ruleid,const UI32 &action,const string &srcMac, const bool &srcMask,const string &dstMac,const bool &dstMask, const string &appType, const bool &isCount,const UI32 &opcode)
        :   WaveMessagePayloadBase(pPrismMsg),
            m_policyName    (policyName),
            m_policyType    (policyType),
            m_ruleid    (ruleid),
            m_action    (action),
            m_srcMac    (srcMac),
            m_srcMask    (srcMask),
            m_dstMac    (dstMac),
            m_dstMask    (dstMask),
            m_appType   (appType),
            m_isCount    (isCount),
            m_opcode    (opcode)
    {
    }


	SsmAssocRuleToPolicyMsgPayload::SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg, 
		const string &policyName, const UI32 &policyType, const UI64 &ruleid, const UI32 &action,
		const string &srcMac, const string &srcMacMask, const AddrType &srcType, const string &dstMac, 
		const string &dstMacMask, const AddrType &dstType, const string &appType, const EtherType &appTypeEnum,
		const UI16 &appTypeInt, const bool &isCount, const bool &isLog, const UI32 &opcode, 
		const UI8 &userEnteredSeqid)
    	:   WaveMessagePayloadBase	(pPrismMsg),
        	m_policyName    		(policyName),
			m_policyType    		(policyType),
			m_ruleid    			(ruleid),
			m_action    			(action),
			m_srcMac    			(srcMac),
			m_srcMacMask 			(srcMacMask),
			m_srcType				(srcType),
			m_dstMac    			(dstMac),
			m_dstMacMask			(dstMacMask),
			m_dstType				(dstType),
			m_appType				(appType),
			m_appTypeEnum			(appTypeEnum),
			m_appTypeInt			(appTypeInt),
			m_isCount		 		(isCount),
			m_isLog					(isLog),
			m_opcode				(opcode),
			m_userEnteredSeqid		(userEnteredSeqid)
    {
    }


    SsmAssocRuleToPolicyMsgPayload::~SsmAssocRuleToPolicyMsgPayload ()
    {
    }

    void  SsmAssocRuleToPolicyMsgPayload::setupAttributesForSerialization()
    {
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
        addSerializableAttribute (new AttributeObjectId (&m_policyObjectId,"policyObjectId"));
    }

    void  SsmAssocRuleToPolicyMsgPayload::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmAssocRuleToPolicyMsgPayload::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  SsmAssocRuleToPolicyMsgPayload::getRuleid() const
    {
        return (m_ruleid);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  SsmAssocRuleToPolicyMsgPayload::getAction() const
    {
        return (m_action);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setSrcMac(const string &srcMac)
    {
        m_srcMac  =  srcMac;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getSrcMac() const
    {
        return (m_srcMac);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setSrcMacMask(const string &srcMacMask)
    {
        m_srcMacMask  =  srcMacMask;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getSrcMacMask() const
    {
        return (m_srcMacMask);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setDstMac(const string &dstMac)
    {
        m_dstMac  =  dstMac;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getDstMac() const
    {
        return (m_dstMac);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setDstMacMask(const string &dstMacMask)
    {
        m_dstMacMask  =  dstMacMask;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getDstMacMask() const
    {
        return (m_dstMacMask);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setAppTypeEnum(const EtherType &appType)
    {
        m_appTypeEnum  =  appType;
    }

    EtherType  SsmAssocRuleToPolicyMsgPayload::getAppTypeEnum() const
    {
        return (m_appTypeEnum);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setAppTypeInt(const UI16 &appType)
    {
        m_appTypeInt  =  appType;
    }

    UI16  SsmAssocRuleToPolicyMsgPayload::getAppTypeInt() const
    {
        return (m_appTypeInt);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setAppType(const string &appType)
    {
        m_appType  =  appType;
    }

    string  SsmAssocRuleToPolicyMsgPayload::getAppType() const
    {
        return (m_appType);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  SsmAssocRuleToPolicyMsgPayload::getIsCount() const
    {
        return (m_isCount);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  SsmAssocRuleToPolicyMsgPayload::getIsLog() const
    {
        return (m_isLog);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  SsmAssocRuleToPolicyMsgPayload::getOpcode() const
    {
        return (m_opcode);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setSrcType(const AddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    AddrType  SsmAssocRuleToPolicyMsgPayload::getSrcType() const
    {
        return (m_srcType);
    }
    void  SsmAssocRuleToPolicyMsgPayload::setDstType(const AddrType &dstType)
    {
        m_dstType  =  dstType;
    }

    AddrType  SsmAssocRuleToPolicyMsgPayload::getDstType() const
    {
        return (m_dstType);
    }

    UI8  SsmAssocRuleToPolicyMsgPayload::getUserEnteredSeqid() const
    {
        return (m_userEnteredSeqid);
    }

    void  SsmAssocRuleToPolicyMsgPayload::setUserEnteredSeqid(const UI8 &userEnteredSeqid)
    {
        m_userEnteredSeqid  =  userEnteredSeqid;
    }
    
    void  SsmAssocRuleToPolicyMsgPayload::setPolicyObjectId (const ObjectId policyObjectId)
    {
        m_policyObjectId = policyObjectId;
    }
    
    ObjectId SsmAssocRuleToPolicyMsgPayload::getPolicyObjectId () const
    {
        return (m_policyObjectId);
    }

	
}
