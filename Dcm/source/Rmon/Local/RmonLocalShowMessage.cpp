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
 *   Author : Mobin Mohan                                                *
 **************************************************************************/

#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Rmon/Local/RmonTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Rmon/Local/RmonLocalShowMessage.h"


namespace DcmNs
{

    RmonLocalShowMessage::RmonLocalShowMessage ()
        : PrismMessage (RmonLocalObjectManager::getPrismServiceId (),
                RMONLOCALSHOW)
    {
    }

    RmonLocalShowMessage::RmonLocalShowMessage (const SI32 &index, const bool &showAll, const bool &showBrief,
            const UI32 &cmdCode, bool isGwRequest, const vector<LocationId> &locationIds,
            const SI32 &startIndex, const SI32 &endIndex)
        : PrismMessage (RmonLocalObjectManager::getPrismServiceId (),
                RMONLOCALSHOW),
        m_index    (index),
		m_showAll (showAll),
		m_showBrief (showBrief),
        m_cmdCode    (cmdCode), 
        m_isGwRequest (isGwRequest),
        m_locationIds (locationIds),
        m_startIndex(startIndex),
		m_endIndex(endIndex)
    {
    }

    RmonLocalShowMessage::~RmonLocalShowMessage ()
    {
    }

    void  RmonLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeSI32(&m_index,"index"));
        addSerializableAttribute(new AttributeBool(&m_showBrief,"showBrief"));
        addSerializableAttribute(new AttributeBool (&m_showAll,"showAll"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeLocationIdVector(&m_locationIds,"locationIdList"));
        addSerializableAttribute(new AttributeSI32(&m_startIndex,"startIndex"));
		addSerializableAttribute(new AttributeSI32(&m_endIndex,"endIndex"));
		addSerializableAttribute(new AttributeSI32(&m_startSampleIndex,"startSampleIndex"));
		addSerializableAttribute(new AttributeSI32(&m_endSampleIndex,"endSampleIndex"));
    }

    void  RmonLocalShowMessage::setIndex(const SI32 &index)
    {
        m_index  =  index;
    }

    SI32  RmonLocalShowMessage::getIndex() const
    {
        return (m_index);
    }

    void  RmonLocalShowMessage::setShowAll(const bool &showAll)
    {
        m_showAll =  showAll;
    }

    bool RmonLocalShowMessage::getShowAll() const
    {
        return (m_showAll);
    }

    void  RmonLocalShowMessage::setShowBrief(const bool &showBrief)
    {
        m_showBrief =  showBrief;
    }

    bool RmonLocalShowMessage::getShowBrief() const
    {
        return (m_showBrief);
    }

    void  RmonLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  RmonLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void RmonLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool RmonLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    vector<LocationId> RmonLocalShowMessage::getLocationIds() const
    {
        return m_locationIds;
    }

    void RmonLocalShowMessage::setLocationIds(vector<LocationId> locationIds)
    {
        m_locationIds = locationIds;
    }

    void RmonLocalShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void RmonLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
        
    }

    SI32 RmonLocalShowMessage::getEndIndex() const
    {
        return m_endIndex;
    }

    SI32 RmonLocalShowMessage::getStartIndex() const
    {
        return m_startIndex;
    }

    void RmonLocalShowMessage::setEndIndex(SI32 endIndex)
    {
        m_endIndex = endIndex;
    }

    SI32 RmonLocalShowMessage::getEndSampleIndex() const
    {
        return m_endSampleIndex;
    }

    SI32 RmonLocalShowMessage::getStartSampleIndex() const
    {
        return m_startSampleIndex;
    }

    void RmonLocalShowMessage::setEndSampleIndex(SI32 endSampleIndex)
    {
        m_endSampleIndex = endSampleIndex;
    }

    void RmonLocalShowMessage::setStartSampleIndex(SI32 startSampleIndex)
    {
        m_startSampleIndex = startSampleIndex;
    }

    void RmonLocalShowMessage::setStartIndex(SI32 startIndex)
    {
        m_startIndex = startIndex;
    }

    /*RmonPaginationAttributes RmonLocalShowMessage::getRmonPaginationAttr() const
    {
        return m_rmonPaginationAttr;
    }

    void RmonLocalShowMessage::setRmonPaginationAttr(RmonPaginationAttributes rmonPaginationAttr)
    {
        m_rmonPaginationAttr = rmonPaginationAttr;
    }*/
}


