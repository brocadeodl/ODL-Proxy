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

#ifndef VCENTER_TOOL_KIT__H
#define VCENTER_TOOL_KIT__H

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/MacAddress.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "DcmResourceIds.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs {

class vCenterToolkit: public WaveObjectManagerToolKit {
private:
	vCenterToolkit();
public:
	static vector<VcenterManagedObject *> getVcenterConfigObjects();
	static VcenterManagedObject* getVcenterConfigFromMo(string vcenterName);
	static ResourceId getNumberVcenterConfigFromMo(UI32 &count);
	static string getNameFromDiscoveryStatus();
};

}

#endif	// VCENTER_TOOL_KIT__H
