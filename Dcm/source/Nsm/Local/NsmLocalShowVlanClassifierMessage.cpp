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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalShowVlanClassifierMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmLocalShowVlanClassifierMessage::NsmLocalShowVlanClassifierMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWVLANCLASSIFIER)
    {
		m_cmdCode = VLANCLASSIFIER_SHOW_UNKNOWN;
		m_isGwRequest = true;
		m_backEndData = " ";
		m_locationId = -1;
		m_lastIndex = 0;
    }

    NsmLocalShowVlanClassifierMessage::NsmLocalShowVlanClassifierMessage (const string &backEndData,const UI32 &cmdCode,const bool isGwRequest, const SI32 locationId)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWVLANCLASSIFIER),
        m_backEndData    (backEndData),
        m_cmdCode   (cmdCode),
		m_isGwRequest  (isGwRequest),
		m_locationId (locationId)	
    {
		m_lastIndex = 0;
    }

    NsmLocalShowVlanClassifierMessage::~NsmLocalShowVlanClassifierMessage ()
    {
    }

    void  NsmLocalShowVlanClassifierMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_backEndData,"backEndData"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
		addSerializableAttribute(new AttributeSI32(&m_locationId,"locationId"));
        addSerializableAttribute (new AttributeUI32(&m_lastIndex,"lastIndex"));
    }

    void  NsmLocalShowVlanClassifierMessage::setBackEndData(const string &backEndData)
    {
        m_backEndData  =  backEndData;
    }

    string  NsmLocalShowVlanClassifierMessage::getBackEndData() const
    {
        return (m_backEndData);
    }

    void  NsmLocalShowVlanClassifierMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

	UI32  NsmLocalShowVlanClassifierMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}
	void NsmLocalShowVlanClassifierMessage::setLocationId(const SI32 &locationId)
	{
		m_locationId = locationId;

	}	
	SI32 NsmLocalShowVlanClassifierMessage::getLocationId()  const
	{
		return m_locationId;
	}	

    void NsmLocalShowVlanClassifierMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalShowVlanClassifierMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void  NsmLocalShowVlanClassifierMessage::setLastIndex (const UI32 &lastIndex)
    {
        m_lastIndex =  lastIndex;
    }

    UI32  NsmLocalShowVlanClassifierMessage::getLastIndex () const
    {
        return (m_lastIndex);
    }

    void NsmLocalShowVlanClassifierMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void NsmLocalShowVlanClassifierMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}
