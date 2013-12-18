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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sanjeev Joshi                                                *
 ***************************************************************************/
#include "vCenter/User/vCenterToolkit.h"

#include <iostream>
#include <sstream>
#include <string>
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/mo_iterator.h"
#include "discover/Framework/discoverManagedObjects.h"

using namespace WaveNs;

namespace DcmNs {

vector<VcenterManagedObject *> vCenterToolkit::getVcenterConfigObjects() {

	vector<VcenterManagedObject *> vectorvCenterList;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());

	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	VcenterManagedObject* pMO = NULL;
	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size();

		for (UI32 i = 0; i < numberOfResults; i++) {
			pMO = dynamic_cast<VcenterManagedObject*>((*pResults)[i]);
			vectorvCenterList.push_back(pMO);
		}
		pResults->clear();
		delete pResults;
	}
	return vectorvCenterList;
}

VcenterManagedObject* vCenterToolkit::getVcenterConfigFromMo(
		string vcenterName) {
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());
	if (vcenterName.length() != 0) {
		syncQueryCtxt.addAndAttribute(new AttributeString(&vcenterName, "id"));
	}
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	VcenterManagedObject* pMO = NULL;

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size();
		if (0 == numberOfResults) {
		} else if (1 == numberOfResults) {
			pMO = dynamic_cast<VcenterManagedObject*>((*pResults)[0]);
			pResults->clear();
			delete pResults;
		} else {
			/* Multiple Vcenter is not supported hence release the
			 *  memory and return NULL
			 */
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}

	return pMO;
}

ResourceId vCenterToolkit::getNumberVcenterConfigFromMo(UI32 &count) {
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			VcenterManagedObject::getClassName());

	ResourceId status = querySynchronouslyForCount(&syncQueryCtxt, count);
	return status;
}

string vCenterToolkit::getNameFromDiscoveryStatus() {
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			DiscoverManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	DiscoverManagedObject* pMO = NULL;
	string vname;

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size();
		if (0 == numberOfResults) {
		} else if (1 == numberOfResults) {
			pMO = dynamic_cast<DiscoverManagedObject*>((*pResults)[0]);
			vname = pMO->getId();
		}
	}
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	return vname;
}

} // namespace DcmNs

