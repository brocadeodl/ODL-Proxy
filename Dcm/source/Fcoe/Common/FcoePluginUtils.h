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

#ifndef FCOE_PLUGIN_UTILS_H
#define FCOE_PLUGIN_UTILS_H

extern "C"
{
#include "fabos/license.h"
#include "FcoePluginTypes.h"
}

#define FCOE_PLUGIN_IS_VCS_ENABLED() (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED)

#define FCOE_PLUGIN_GET_FCOE_ENODES() \
 ((licenseCheck(BASE_FCOE_LICENSE) != 0) ? FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE : FCOE_DEFAULT_FCOE_ENODES_WITH_OUT_LICENSE)


#define FCOE_PLUGIN_VCS_ENABLE_CHECK_FOR_AP(_uinfo) \
{ \
   if (!FCOE_PLUGIN_IS_VCS_ENABLED()) { \
       string vcs_err_string = FrameworkToolKit::localize(STANDALONE_MODE_FEATURE_NOT_SUPPORTED); \
       return 0; \
   } \
}

#define FCOE_PLUGIN_VCS_ENABLE_CHECK() \
{ \
    if (!FCOE_PLUGIN_IS_VCS_ENABLED()) { \
        return STANDALONE_MODE_FEATURE_NOT_SUPPORTED; \
    } \
}

#define FCOE_PLUGIN_LICENSE_CHECK() \
{\
    if (licenseCheck(BASE_FCOE_LICENSE) == 0) { \
        return FCOE_COMMON_ERR_LICENSE_NOT_PRESENT; \
    } \
}

#define FCOE_PLUGIN_LICENSE_CHECK_FOR_SEQUENCER(_ctx_p) \
{\
    if (licenseCheck(BASE_FCOE_LICENSE) == 0) { \
        (_ctx_p)->executeNextStep(FCOE_COMMON_ERR_LICENSE_NOT_PRESENT); \
        return; \
    } \
}

#define FCOE_PLUGIN_DELETE(_obj_p) \
{ \
    if ((_obj_p)) { \
        delete (_obj_p);\
    }\
}

#endif
