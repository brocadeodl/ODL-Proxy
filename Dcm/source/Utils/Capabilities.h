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
 **************************************************************************/
/*
 * Capabilities.h
 *
 *  Created on: Dec 20, 2011
 *      Author: rmadapur
 */

#ifndef CAPABILITIES_H_
#define CAPABILITIES_H_
#include "fos_swbd.h"
#include <time.h>
#include "Framework/Utils/TraceUtils.h"
#include "WaveResourceIdEnums.h"
#include "Framework/Types/BitMap.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"



using namespace std;
using namespace WaveNs;

#define MAX_CAP_COUNT 4096
namespace DcmNs {

class Capabilities {

private:

	 BitMap *cap;
	 void setCapability(capability_t capability, bool enable);
	 void populateCapabilities();
	 int getBitMapIndex(capability_t capability);
	 bool isCapabilityPresent_int(capability_t capability);
	 Capabilities();

public:

	virtual ~Capabilities();

  /**
   * Sample Usage
   * bool pres = Capabilities::isCapabilityPresent(FEAT_CAPABILITY_POLICER);
   *
   * Developers should preferably make use of this call from the local plugins.
   * In a mixed fabric(mix of different platforms). Its more realistic to make the API call from
   * local plugins.
   * Moreover there is no restriction in calling the API from Global plugins(if the situation so demands)
   *
   * FEAT_CAPABILITY_POLICER is an Resource defined in dcm/source/Resources/Capabilities.xml
   * The enum definitions for the same are in DcmResourceIdEnums.h (Auto-generated
   * during compilation of dcm/source/Resources)
   *
   * To add new capabilities make an entry in Capabilities.xml and put the appropriate logic in
   * populateCapabilities()
   */

	static bool isCapabilityPresent(capability_t capability);
	static Capabilities * instance();
	int getSwModel();

};
}
#endif /* CAPABILITIES_H_ */

