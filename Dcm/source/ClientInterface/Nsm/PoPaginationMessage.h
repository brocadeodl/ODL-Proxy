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

#ifndef POPAGINATIONMESSAGE_H_
#define POPAGINATIONMESSAGE_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs {
	class PoPaginationMessage : public DcmManagementInterfaceMessage {
	private:
	protected:
		virtual void setupAttributesForSerialization();
	public:
		PoPaginationMessage(const UI32 &operationId);
		PoPaginationMessage(const UI32 &operationId, const UI32 &startPoId, const UI32 &endPoId);
		virtual ~PoPaginationMessage();

		void setStartPoId(const UI32 &poNum);
		UI32 getStartPoId() const;
		void setEndPoId(const UI32 &poNum);
		UI32 getEndPoId() const;

	private:
	protected:
	public:
		UI32 m_startPoId;
		UI32 m_endPoId;
	};
}

#endif /* POPAGINATIONMESSAGE_H_ */
