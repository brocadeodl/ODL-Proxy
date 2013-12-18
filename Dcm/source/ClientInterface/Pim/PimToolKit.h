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

#ifndef _PIM_TOOL_KIT_H_
#define _PIM_TOOL_KIT_H_

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Pim/PimMsgDef.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

template<class T>
string toString(const T & t)
{
    stringstream ss;
    ss<<t;
    return ss.str();
}

static inline string pimGetIntfTypeStr(UI8 intfType)
{
    switch(intfType) {
        case INTERFACE_TENGIG:
            return("te");
            break;
        case INTERFACE_VLAN:
            return("vlan");
            break;
        case INTERFACE_PORT_CHANNEL:
            return("po");
            break;
        case INTERFACE_ONEGIG:
            return("gi");
            break;
        case INTERFACE_FORTYGIG:
            return("fortygi");
            break;
    }
    return(" ");
}

template<class T>
UI32 toUI32(const T & t)
{
    stringstream ss;
    UI32 ui32_elem;
    ss<<t;
    ss>>ui32_elem;
    return ui32_elem;
}

#define PIM_PLUG_ASSERT_PTR(exp) \
            prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define PIM_FALSE 0
#define PIM_TRUE (!PIM_FALSE)

}

#endif /* _PIM_TOOL_KIT_H_ */
