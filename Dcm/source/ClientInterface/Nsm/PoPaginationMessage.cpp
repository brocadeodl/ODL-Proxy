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
 *  Created on: Dec 21, 2011
 *      Author: Mayank Maheshwari
 *
 *
 **********************************************************/

#include "ClientInterface/Nsm/PoPaginationMessage.h"

namespace DcmNs {
	PoPaginationMessage :: PoPaginationMessage(const UI32 &operationId)
		: DcmManagementInterfaceMessage (operationId) {
		m_startPoId = 0;
		m_endPoId = 0;
	}

	PoPaginationMessage :: PoPaginationMessage(const UI32 &operationId, const UI32 &startPoId, const UI32 &endPoId)
		: DcmManagementInterfaceMessage (operationId),
		m_startPoId (startPoId),
		m_endPoId (endPoId) {
	}

	PoPaginationMessage :: ~PoPaginationMessage() {
	}

	void PoPaginationMessage :: setupAttributesForSerialization() {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_startPoId,"startPoId"));
		addSerializableAttribute (new AttributeUI32(&m_endPoId,"endPoId"));
	}

	void PoPaginationMessage ::  setStartPoId(const UI32 &poNum) {
		m_startPoId = poNum;
	}

	UI32 PoPaginationMessage :: getStartPoId() const {
		return (m_startPoId);
	}

	void PoPaginationMessage :: setEndPoId(const UI32 &poNum) {
		m_endPoId = poNum;
	}

	UI32 PoPaginationMessage :: getEndPoId() const {
		return (m_endPoId);
	}
}
