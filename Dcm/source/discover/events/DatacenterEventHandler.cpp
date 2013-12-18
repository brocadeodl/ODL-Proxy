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

#include "DatacenterEventHandler.h"
#include "VirtualDb/Domain/DataCenterDomain.h"
#include <iostream>

namespace DcmNs {
DatacenterEventHandler::DatacenterEventHandler(string vcenter, Event* event):EventHandler(vcenter, event) {

}

DatacenterEventHandler::~DatacenterEventHandler() {
}

void DatacenterEventHandler::handle(ServiceInstance *serviceInstance) {
	Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,
			"------DATACENTER Event Handler ----------- %s ", m_event->type.c_str());
	if (m_event->type.compare(DATACENTER_CREATE) == 0)  {
		// dc create
		Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,
				" %s: %s ",
				m_event->type.c_str());
		Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Event DatacEnter ID %s\n",
                m_event->datacenter.datacenter.val.c_str());
		Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Event DatacEnter Name %s\n",
				m_event->datacenter.name.c_str());

		DatacenterEventMessage *pMessage = new DatacenterEventMessage();
		DataCenterDomain dc(m_event->datacenter.datacenter.val, m_event->datacenter.name);
		pMessage->m_opcode=VNM_CREATE;
		pMessage->m_vcenter=m_vcenter;
		pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);

	}else if (m_event->type.compare(DATACENTER_RENAMED) == 0) {
		// dc rename
		DatacenterEventMessage *pMessage = new DatacenterEventMessage();
		DataCenterDomain dc(m_event->datacenter.datacenter.val, m_event->datacenter.name);
		pMessage->m_opcode=VNM_UPDATE;
		pMessage->m_vcenter=m_vcenter;
		pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
	}

}

} /* namespace DcmNs */
