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

#ifndef DHCP_UTILS_H 
#define DHCP_UTILS_H 

#include "Framework/Types/Types.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
#define DHCP_IS_DIGIT(a) ((a) >= '0' && (a) <= '9')

#define DHCP_IF_TWO_TO_THREE_TUPLE(_s_ifname, _t_ifname, _rid)             \
if((! strncasecmp(_s_ifname, "vlan", 4)) ||                                 \
        (! strncasecmp(_s_ifname, "po", 2)) ||                              \
        (! strncasecmp(_s_ifname, "ve", 2)) ||                              \
        (WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled())) { \
    strncpy (_t_ifname, _s_ifname, DHCP_MAX_IFNAME_STR);                \
}                                                                           \
else {                                                                      \
    char _rid_a[DHCP_MAX_IFNAME_STR];                                   \
    uint32_t _count = 0;                                                    \
    uint32_t _rid_len = 0;                                                  \
    do {                                                                    \
        if (DHCP_IS_DIGIT(_s_ifname[_count])) {                            \
            snprintf(_rid_a, DHCP_MAX_IFNAME_STR, "%d", _rid);          \
            strncat (_rid_a, "/", DHCP_MAX_IFNAME_STR);                 \
            strncpy((_t_ifname + _count), _rid_a, (DHCP_MAX_IFNAME_STR - _count));  \
            _rid_len = strlen(_rid_a);                                      \
            while (_s_ifname[_count] != '\0') {                             \
                _t_ifname[_count + _rid_len] = _s_ifname[_count];           \
                _count++;                                                   \
            }                                                               \
            _t_ifname[_count+_rid_len] = '\0';                              \
        }                                                                   \
        else {                                                              \
            _t_ifname[_count] = _s_ifname[_count];                          \
            _count++;                                                       \
        }                                                                   \
    }while(_s_ifname[_count] != '\0');                                      \
}


    class DhcpUtils
    {
        private:
            DhcpUtils();
        public:
            static ResourceId mapBackEndErrorsToDcmdErrors(int err);
    };

}
#endif  //DHCP_UTILS_H


