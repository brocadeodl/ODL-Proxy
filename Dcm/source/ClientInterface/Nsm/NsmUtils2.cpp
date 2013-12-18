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

#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

bool
NsmUtils::isValidThreeTuple (const string& ifName)
{
    string  delimiter = "/";
    int count = 0;
    size_t  last_pos = ifName.find_first_of(delimiter, 0);
    size_t len = 0;

    while (string::npos != last_pos) {
        count ++;
        len = last_pos + 1;
        last_pos = ifName.find_first_of(delimiter, len);
    }

    if (count != 2) {
        return false;
    }
    return true;
}

void
NsmUtils::getTwoTupleIfName (const string& ifName, string& twoTupleIfName)
{
    size_t  pos = 0;

    pos = ifName.find("/");
    twoTupleIfName = ifName.substr(pos + 1);
}

}
