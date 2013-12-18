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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : vsatyana														*
 ****************************************************************************/

#ifndef SWITCHPORTVLANCLASSIFICATIONMESSAGE_H
#define SWITCHPORTVLANCLASSIFICATIONMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class SwitchportVlanClassificationMessage : public DcmManagementInterfaceMessage
{
	private:
	protected:
		virtual void setupAttributesForSerialization();
	public:
		SwitchportVlanClassificationMessage ();
		virtual ~SwitchportVlanClassificationMessage ();

		void setMacAddressRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const string &macAddress);
		void setMacGroupIdRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const UI32 &macGroupId);
		void setCtagIdRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const UI32 &ctagId);
		UI32 getNumRec() const;
		virtual const void *getCStructureForInputs();
		virtual void loadOutputsFromCStructure(const void *pOutputCStructure);

	// Now the data members

	private:
	protected:
	public:
		vector <UI32> m_cmdCode;
		vector <UI32> m_vlanId;
		vector <UI32> m_ifType;
		vector <string> m_ifName;
		vector <string> m_macAddress;
		vector <UI32> m_macGroupId;
		vector <UI32> m_ctagId;
};
}
#endif //SWITCHPORTVLANCLASSIFICATIONMESSAGE_H

