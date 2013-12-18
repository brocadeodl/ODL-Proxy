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
 *  Created on: Nov 17, 2011
 *      Author: Mayank Maheshwari
 *
 *	Common utilities related to Layer 3 Licensing
 **********************************************************/

#ifndef LAYER3LICENSEUTIL_H_
#define LAYER3LICENSEUTIL_H_

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

using namespace WaveNs;

namespace DcmNs {
	class Layer3LicenseUtil : public WaveObjectManagerToolKit {
	private:
		Layer3LicenseUtil();
        static ResourceId isOspfConfigured(bool &b);
        static ResourceId isPimConfigured(bool &b);
        static ResourceId isVrrpConfigured(bool &b);
        static ResourceId isRtmConfigured(bool &b);
        static ResourceId isArpConfigured(bool &b);
        static ResourceId isL3AclConfigured(bool &b);
        static ResourceId isIpPolicyConfigured(bool &b);
        static ResourceId isIgmpConfigured(bool &b);
        static ResourceId isL3NsmConfigured(bool &b);
        static ResourceId isL3FibConfigured(bool &b);
		static ResourceId isBgpConfigured(bool &b);

	protected:
	public:
		~ Layer3LicenseUtil();

		static ResourceId isLayer3Configured(bool &b);
        static ResourceId checkLayer3License();
        static bool isLayer3Supported();

	};
}


extern "C"
{
#include "fabos/license.h"
}

#define LAYER_3_LICENSE_CHECK() {                       \
    if (!(Layer3LicenseUtil::isLayer3Supported())) {    \
        return WRC_LAYER_3_NOT_SUPPORTED_SA_MODE;       \
    }                                                   \
    if (licenseCheck(LAYER_3_LICENSE) == 0) {           \
        trace(TRACE_LEVEL_DEBUG, "Layer3LicenseUtil :: LAYER_3_LICENSE is not available");  \
        return WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR;   \
    }                                                   \
}

#define LAYER_3_LICENSE_CHECK_FOR_SEQUENCER(_ctx_p) {                       \
    if (!(Layer3LicenseUtil::isLayer3Supported())) {                        \
        return WRC_LAYER_3_NOT_SUPPORTED_SA_MODE;                           \
    }                                                                       \
    if (licenseCheck(LAYER_3_LICENSE) == 0) {                               \
        (_ctx_p)->executeNextStep(WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR);   \
        return;                                                             \
    }                                                                       \
}

#endif /* LAYER3LICENSEUTIL_H_ */
