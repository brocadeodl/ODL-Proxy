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

/**********************************************************
 * 	Copyright (C) 2011 Brocade Communications Systems, Inc.
 * 	All rights reserved.
 *
 *  Created on: Dec 20, 2011
 *      Author: Mayank Maheshwari
 *
 *
 **********************************************************/

#include "Nsm/Local/PoPaginationLocalMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs {
	PoPaginationLocalMessage :: PoPaginationLocalMessage(PrismServiceId serviceCode, const UI32 &operationId)
		: ManagementInterfaceMessage  (FrameworkToolKit::getServiceNameById (serviceCode), operationId) {
		m_startPoId = 0;
		m_endPoId = 0;
		m_isGwRequest = true;
		destLocationIds = NULL;
	}

	PoPaginationLocalMessage :: PoPaginationLocalMessage(PrismServiceId serviceCode, const UI32 &operationId, const bool isGwRequest)
		: ManagementInterfaceMessage  (FrameworkToolKit::getServiceNameById (serviceCode), operationId) {
		m_startPoId = 0;
		m_endPoId = 0;
		m_isGwRequest = isGwRequest;
		destLocationIds = NULL;
	}

	PoPaginationLocalMessage :: PoPaginationLocalMessage(PrismServiceId serviceCode, const UI32 &operationId, const bool isGwRequest, const UI32 &startPoId, const UI32 &endPoId)
		: ManagementInterfaceMessage (FrameworkToolKit::getServiceNameById (serviceCode), operationId),
		  m_startPoId (startPoId),
		  m_endPoId (endPoId),
		  m_isGwRequest (isGwRequest) {
		destLocationIds = NULL;
	}

	PoPaginationLocalMessage :: ~PoPaginationLocalMessage() {
	}

	void PoPaginationLocalMessage :: setupAttributesForSerialization() {
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_startPoId,"startPoId"));
		addSerializableAttribute (new AttributeUI32(&m_endPoId,"endPoId"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        if (destLocationIds != NULL) {
        	addSerializableAttribute(new AttributeUI32Vector(destLocationIds, "destLocationIds"));
        }
	}

	void PoPaginationLocalMessage ::  setStartPoId(const UI32 &poNum) {
		m_startPoId = poNum;
	}

	UI32 PoPaginationLocalMessage :: getStartPoId() const {
		return (m_startPoId);
	}

	void PoPaginationLocalMessage :: setEndPoId(const UI32 &poNum) {
		m_endPoId = poNum;
	}

	UI32 PoPaginationLocalMessage :: getEndPoId() const {
		return (m_endPoId);
	}

	void PoPaginationLocalMessage :: setIsGwRequest (bool isGwRequest) {
		m_isGwRequest = isGwRequest;
	}

	bool PoPaginationLocalMessage :: getIsGwRequest ()  const {
		return m_isGwRequest;
	}

	void PoPaginationLocalMessage :: copyAllBuffers (const PrismMessage &prismMessage) {
		this->copyBuffersFrom(prismMessage);
	}

	void PoPaginationLocalMessage :: setDestinationLocations(vector<LocationId> *destLoc) {
		destLocationIds = destLoc;
	}

	vector<LocationId>* PoPaginationLocalMessage :: getDestinationLocations() {
		return destLocationIds;
	}
}
