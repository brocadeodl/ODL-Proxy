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
 *   Author : Bala 	                                                       *
 **************************************************************************/

#include "APPM/Local/APPMLocalShowMessage.h"
#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalTypes.h"
#include "ClientInterface/APPM/APPMShowMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    APPMLocalShowMessage::APPMLocalShowMessage()
        : PrismMessage(APPMLocalObjectManager::getPrismServiceId(), APPMLOCALSHOWPROFILE)
    {
		m_ifName = " ";
		m_interface = "tengigabitethernet";
    }

    APPMLocalShowMessage::APPMLocalShowMessage(const UI32 &cmdCode, const string &ifName, const string &profileName, bool isGwRequest, const string &lastProfileMac, const UI32 &numOfRecords, const UI32 &fetchModelType)
        : PrismMessage(APPMLocalObjectManager::getPrismServiceId(), APPMLOCALSHOWPROFILE),
        m_cmdCode   (cmdCode),
        m_ifName    (ifName),
        m_profileName (profileName),
		m_isGwRequest (isGwRequest),
		m_interface ("tengigabitethernet"),
        m_lastProfileMac (lastProfileMac),
        m_numOfRecords (numOfRecords),
        m_fetchModelType (fetchModelType)
    {
    }

    APPMLocalShowMessage::~APPMLocalShowMessage ()
    {
    }

    void  APPMLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_profileName, "profileName"));
		addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
		addSerializableAttribute (new AttributeString(&m_interface,"interface"));
        //AMPP Pagination Changes.
        addSerializableAttribute (new AttributeString(&m_lastProfileMac,"lastProfileMac")); 
        addSerializableAttribute (new AttributeUI32(&m_numOfRecords,"numOfRecords"));
        addSerializableAttribute (new AttributeUI32(&m_fetchModelType,"fetchModelType"));

    }

    void  APPMLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  APPMLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    void  APPMLocalShowMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  APPMLocalShowMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  APPMLocalShowMessage::setProfileName(const string &profileName)
    {
        m_profileName = profileName;
    }

    string  APPMLocalShowMessage::getProfileName() const
    {
        return (m_profileName);
    }
	
	void APPMLocalShowMessage::setPortProfileDomainName(const string &profileDomainName) {
		m_portProfileDomainName = profileDomainName;
	}
    
	string APPMLocalShowMessage::getPortProfileDomainName() const {
		return m_portProfileDomainName;
	}

    void  APPMLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  APPMLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void APPMLocalShowMessage::setIsGwRequest(bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool APPMLocalShowMessage::getIsGwRequest() const
    {
        return (m_isGwRequest);
    }
    
    void APPMLocalShowMessage::setLastProfileMac(const string &lastProfileMac) {
        m_lastProfileMac = lastProfileMac;
    }

    string APPMLocalShowMessage::getLastProfileMac() const {
        return (m_lastProfileMac);
    }

    void APPMLocalShowMessage::setNumberOfRecords(const UI32 &numOfRecords) {
        m_numOfRecords = numOfRecords;
    }

    UI32 APPMLocalShowMessage::getNumberOfRecords() const {
        return (m_numOfRecords);
    }

    void APPMLocalShowMessage::setFetchModelType(const UI32 &fetchModelType) {
    	m_fetchModelType = fetchModelType;
    }

    UI32 APPMLocalShowMessage::getFetchModelType() const {
    	return (m_fetchModelType);
    }

    void APPMLocalShowMessage::setLocationId(const UI32 &locationId) {
    	m_locationId = locationId;
    }

    UI32 APPMLocalShowMessage::getLocationId() const{
    	return (m_locationId);
    }
    void APPMLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

    void APPMLocalShowMessage::transferOutputBuffer(APPMShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string(" bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }

    void APPMLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage) {
        this->copyBuffersFrom(prismMessage);
    }
}
