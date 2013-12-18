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
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Qos/Local/QosLocalShowMessage.h"


namespace DcmNs
{

    QosLocalShowMessage::QosLocalShowMessage () : PrismMessage ( QosLocalObjectManager::getPrismServiceId (), QOSLOCALSHOW)
    {
    }

    QosLocalShowMessage::QosLocalShowMessage (const string &ifName, const string &poNum, const UI32 &cmdCode, bool &isGwRequest, const UI32 &lineCard,
		const UI32 &portSet, const SI32 &profileId, const SI32 &chipIndex, const SI32 &portIndex, const UI32 &noOfRecords)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (), QOSLOCALSHOW),
        m_ifName       (ifName),
		m_poNum        (poNum),
        m_cmdCode      (cmdCode),
        m_isGwRequest  (isGwRequest),
		m_lineCard     (lineCard),
		m_portSet      (portSet),
		m_profileId		(profileId),
		m_chipIndex     (chipIndex),
		m_portIndex     (portIndex),
		m_noOfRecords   (noOfRecords)
    {
    }

    QosLocalShowMessage::~QosLocalShowMessage ()
    {
    }

    void  QosLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeString(&m_poNum,"poNum"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute(new AttributeUI32(&m_lineCard,"lineCard"));
        addSerializableAttribute(new AttributeUI32(&m_portSet,"portSet"));
        addSerializableAttribute(new AttributeSI32(&m_profileId,"profileId"));
        addSerializableAttribute(new AttributeSI32(&m_chipIndex,"chipIndex"));
        addSerializableAttribute(new AttributeSI32(&m_portIndex,"portIndex"));
        addSerializableAttribute(new AttributeUI32(&m_noOfRecords,"noOfRecords"));
    }

    void  QosLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  QosLocalShowMessage::setPoNum(const string &poNum)
    {
        m_poNum  =  poNum;
    }

    string  QosLocalShowMessage::getPoNum() const
    {
        return (m_poNum);
    }

    void  QosLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  QosLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void QosLocalShowMessage::setIsGwRequest (bool &isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool QosLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void  QosLocalShowMessage::setLineCard(const UI32 &lineCard)
    {
        m_lineCard  =  lineCard;
    }

    UI32  QosLocalShowMessage::getLineCard() const
    {
        return (m_lineCard);
    }

    void  QosLocalShowMessage::setPortSet(const UI32 &portSet)
    {
        m_portSet  =  portSet;
    }

    UI32  QosLocalShowMessage::getPortSet() const
    {
        return (m_portSet);
    }

	void QosLocalShowMessage::setProfileId (SI32 profileId)
	{
		m_profileId = profileId;
	}
	
	SI32 QosLocalShowMessage::getProfileId () const
	{
		return (m_profileId);
	}

	void QosLocalShowMessage::setChipIndex(SI32 &chipIndex)
	{
		m_chipIndex = chipIndex;
	}
	
	SI32 QosLocalShowMessage::getChipIndex() const
	{
		return (m_chipIndex);
	}

	void QosLocalShowMessage::setPortIndex(SI32 &portIndex)
	{
		m_portIndex = portIndex;
	}
	
	SI32 QosLocalShowMessage::getPortIndex() const
	{
		return (m_portIndex);
	}

	void QosLocalShowMessage::setNoOfRecords(UI32 &noOfRecords)
	{
		m_noOfRecords = noOfRecords;
	}
	
	UI32 QosLocalShowMessage::getNoOfRecords() const
	{
		return (m_noOfRecords);
	}

	void QosLocalShowMessage::setLocationId(UI32 &locationId)
	{
		m_locationId = locationId;
	}
	
	UI32 QosLocalShowMessage::getLocationId() const
	{
		return (m_locationId);
	}





    void QosLocalShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void QosLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
    }

}
