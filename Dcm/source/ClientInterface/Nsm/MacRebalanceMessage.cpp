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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/

#include "ClientInterface/Nsm/MacRebalanceMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs {

	MacRebalanceMessage::MacRebalanceMessage () : DcmManagementInterfaceMessage (VLAGEXMREBALANCE) {
    }


	MacRebalanceMessage::MacRebalanceMessage (const UI32 &poIfIndex)
        : DcmManagementInterfaceMessage (VLAGEXMREBALANCE), m_poIfIndex(poIfIndex) {
	}

	MacRebalanceMessage::~MacRebalanceMessage () {
	}

	void  MacRebalanceMessage::setupAttributesForSerialization() {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_poIfIndex, "poIfIndex"));
	}

	void  MacRebalanceMessage::setPoIfIndex(const UI32 &poIfIndex) {
        m_poIfIndex = poIfIndex;
	}

	UI32  MacRebalanceMessage::getPoIfIndex() const	{
        return (m_poIfIndex);
	}

	const void *MacRebalanceMessage::getCStructureForInputs () {
		NsmVlagExmRebalanceMsg_t *pInput = (NsmVlagExmRebalanceMsg_t *) malloc (sizeof(NsmVlagExmRebalanceMsg_t));
		pInput->port_channel = m_poIfIndex;
		return (pInput);
	}

	void MacRebalanceMessage::loadOutputsFromCStructure (const void *pOutputCStructure)	{
	}
}
