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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Ganesh Sindigi
 *
 *   Declarations for VLAN provisioning message.
 ***************************************************************************/

#ifndef NSMVLANPROVISIONMESSAGE_H_
#define NSMVLANPROVISIONMESSAGE_H_

#include <memory>
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/UI32Range.h"

namespace DcmNs {

class NsmVlanProvisionMessage: public DcmManagementInterfaceMessage {

private:
    void interpretBuffers();

protected:
	virtual void setupAttributesForSerialization();
	NsmVlanProvisionMessage(const UI32 opCode);

public:
	NsmVlanProvisionMessage();
	virtual ~NsmVlanProvisionMessage ();

	UI32Range getProvisionVlanRange() const;
	void setProvisionVlanRange(const UI32Range &vlanRange);

	vlan_provision_code getProvisionCode() const;
	void setProvisionCode(const vlan_provision_code provisionCode);

	bool getNeedsExplicitUnprovision () const;
	void setNeedsExplicitUnprovision (bool flag);

	/**
	 * Should be invoked after the message is sent to client.
	 * Returns a VLAN Range with the specified provision status.
     */
    auto_ptr<UI32Range> getVlanRangeForStatus(
            const vlan_provision_status provisionStatus) ;

	virtual const void *getCStructureForInputs();
	virtual void loadOutputsFromCStructure(const void *pOutputCStructure);

protected:
	UI32Range m_provisionVlanRange;
	vlan_provision_code m_provisionCode;
	bool    m_needsExplicitUnprovision; // Only for PROVISION_XXX provision codes

private:
	bool buffersRead;
	map<vlan_provision_status,vector<UI32> > mapResponse;

};
}

#endif /* NSMVLANPROVISIONMESSAGE_H_ */
